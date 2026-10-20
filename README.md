# RangeAllocator

# About

RangeAllocator is a **range allocator** (also called offset allocator) written in **C++20**. <br>
It does not hand out memory itself. Instead it manages a range of a given size and hands out **positions inside that range**,
which makes it useful for anything where you have one big block and need to split it up:
GPU buffers, vertex and index batches, space inside a file and so on.

Free space is kept in a **binary tree ordered by size**, so finding a fitting block does not mean looking at every free block. <br>
When a block is given back, it is **merged with its free neighbours right away**, so the range does not slowly fall apart into tiny unusable pieces.

# Features

## 🎯 Fit Strategies - Swappable

The tracker does not decide by itself which free block to use, it asks a request handler. <br>
Two handlers are shipped:

- **BestFitRequestHandler** picks the smallest free block that is still big enough. This wastes the least space.
- **FirstFitRequestHandler** takes the first block it finds on its way through the tree that is big enough. This is quicker, but can split up bigger blocks than necessary.

You can write your own by extending `NodeRequestHandler` and implementing `findNode()`.

## 🧩 Merging - Immediate and cheap

Every free block is registered by where it starts and where it ends. <br>
When a block is given back, the tracker looks up whether a free block ends exactly where it starts,
or starts exactly where it ends, and merges them into one. <br>
Both lookups are hash map lookups, so merging does not need to search the tree.

## 🛡️ Safe Give Back - Mistakes get ignored

The tracker remembers every block it handed out together with its size. <br>
Giving back a position that was never handed out, giving it back with the wrong size or giving it back twice does nothing,
instead of corrupting the free space.

# Getting Started

You need a C++20 compiler and CMake 4.0 or newer.

~~~
cmake -S . -B build
cmake --build build
./build/RangeAllocator
~~~

## Minimal Setup

~~~
#include "BinaryTracker.h"

int main() {
    // Decides which free block to use, has to live at least as long as the tracker.
    BestFitRequestHandler bestFit;

    // Manages the range [0, 100).
    BinaryTracker tracker(100, &bestFit);

    // Returns the position of the block, or -1 if nothing fits.
    int pos = tracker.request(30);

    // Give it back with the same position and size you got it with.
    tracker.giveBack(pos, 30);
}
~~~

## Typical Mistakes

### Request Handler Lifetime

The tracker only keeps a pointer to the request handler, it does not copy it. <br>
If the handler gets destroyed before the tracker, the next call to `request()` will crash or worse.

### Not Checking for -1

`request()` returns -1 if no free block is big enough or the size is zero or negative. <br>
Using -1 as a position will not throw anything, it will just silently be wrong.

### Copying the Tracker

The tracker owns its tree, so it cannot be copied. Copying would leave two trackers deleting the same nodes. <br>
Pass it by reference or pointer instead.

# How it works

## The Problem

When blocks of different sizes get handed out and given back in any order, the free space ends up spread over many holes. <br>
The simple way to find a fitting hole is to look at all of them, which turns every request into an O(n) operation.
Finding out whether a hole has a free neighbour to merge with is the same story.

## The Solution

Free blocks are stored as nodes in a binary tree, with the size as the key. <br>
To find a block that fits, you only need to walk down one path of the tree instead of looking at all of them. <br>
For merging, every free block is also stored in two hash maps: one by its start position and one by its end position.
A freed block can then ask "does a free block end where I start?" and "does a free block start where I end?" in O(1).

## How RangeAllocator implements it

On `request(size)`:

1. The request handler walks down the tree and returns a node that is big enough.
2. That node is removed from the tree and both hash maps.
3. If the node was bigger than needed, the rest is put back in as a new, smaller free block.
4. The position and size get stored, so `giveBack()` can check them later.

On `giveBack(pos, size)`:

1. The position and size are checked against the stored ones, wrong calls are ignored.
2. Free neighbours in front and behind are looked up in the hash maps.
3. The neighbours are removed and one merged free block is inserted instead.

## Time Complexity

n is the number of free blocks, so the number of nodes in the tree. <br>
"Average" assumes free blocks of mixed sizes, which keeps the tree roughly balanced.

| Operation | Best | Average | Worst |
|---|---|---|---|
| `request()` | O(1) | O(log n) | O(n) |
| `giveBack()` | O(1) | O(log n) | O(n) |
| Finding a fitting block (both handlers) | O(1) | O(log n) | O(n) |
| Finding neighbours to merge | O(1) | O(1) | O(1) |
| Inserting / removing a tree node | O(1) | O(log n) | O(n) |
| Constructor | O(1) | O(1) | O(1) |
| Destructor | O(n) | O(n) | O(n) |

The neighbour lookups are hash map lookups. In theory those can get slow if lots of keys land in the same bucket,
but with plain integer positions that does not happen in practice. <br>
The worst case of everything else comes from the tree not balancing itself. <br>
Blocks with the same size always go to the higher side of the tree,
so many free blocks of the same size end up as one long chain instead of a tree.

## Space Complexity

The tracker needs **O(n + m)** space, with n free blocks and m handed out blocks.

- Every free block costs one tree node (24 bytes on 64-bit) and an entry in each of the two hash maps.
- Every handed out block costs one hash map entry.

All of this lives outside of the managed range, which is the point: the range itself is never touched,
so it can be anything, even memory the CPU cannot write to.

# Known Limitations

- The tree is not balanced, so the worst case above can actually happen, most likely with lots of blocks of the same size.
- `FirstFitRequestHandler` takes the first fit it finds in the tree, not the block with the lowest position.
- Sizes and positions are `int`, there is no alignment support.
- Not thread safe. If multiple threads use one tracker, you have to lock around it yourself.
