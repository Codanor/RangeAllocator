//
// Created by Tim Klöpper on 10/20/2026.
//

#include "BinaryTracker.h"
#include "BinaryTrackerNode.h"

BinaryTracker::~BinaryTracker() {
    _root->rmvHigher();
    _root->rmvLower();

    delete _root;
    _root = nullptr;
}
BinaryTracker::BinaryTracker(const int initSize, const NodeRequestHandler* handler) {
    _initSize = initSize;
    _wholeSize = initSize;
    _root = new Node(-1, -1);
    _root->addNode(new Node(initSize, 0));

    _nodeFronts[0] = _root->getHigher();
    _nodeBacks[initSize] = _root->getHigher();

    _handler = handler;
}

int BinaryTracker::request(const int size) {
    if (_wholeSize < size) return -1;
    if (size <= 0) return -1;

    Node *found = _handler->findNode(size, _root);
    if (found == nullptr) return -1;

    const int foundPos = found->getPos();
    const int foundSize = found->getSize();

    if (!rmvNode(found)) return -1;
    if (foundSize > size) addNode(new Node(foundSize - size, foundPos + size));

    _allocated[foundPos] = size;
    _wholeSize -= size;

    return foundPos;
}
void BinaryTracker::giveBack(int pos, int size) {
    const auto foundIterator = _allocated.find(pos);
    if (foundIterator == _allocated.end()) return;
    if (foundIterator->second != size) return;
    _allocated.erase(foundIterator);

    _wholeSize += size;

    // Look for adjacent nodes.

    // Find front neighbour.
    const auto frontNeighbourIterator = _nodeBacks.find(pos);
    Node* frontNeighbour = nullptr;

    if (frontNeighbourIterator != _nodeBacks.end()) {
        frontNeighbour = frontNeighbourIterator->second;
        pos = frontNeighbour->getPos();
        size += frontNeighbour->getSize();
    }

    // Find back neighbour.
    const auto backNeighbourIterator = _nodeFronts.find(pos + size);
    Node* backNeighbour = nullptr;

    if (backNeighbourIterator != _nodeFronts.end()) {
        backNeighbour = backNeighbourIterator->second;
        size += backNeighbour->getSize();
    }

    // Construct new Node.

    rmvNode(frontNeighbour);
    rmvNode(backNeighbour);
    addNode(new Node(size, pos));
}

bool BinaryTracker::rmvNode(Node* node) {
    if (node == nullptr) return false;

    const int nodePos = node->getPos();
    const int nodeSize = node->getSize();

    if (!_root->rmvNode(node)) return false;

    _nodeFronts.erase(nodePos);
    _nodeBacks.erase(nodePos + nodeSize);

    return true;
}
bool BinaryTracker::addNode(Node *node) {
    if (!_root->addNode(node)) return false;

    _nodeFronts[node->getPos()] = node;
    _nodeBacks[node->getPos() + node->getSize()] = node;

    return true;
}