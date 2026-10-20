//
// Created by Tim Klöpper on 10/20/2026.
//

#ifndef RANGEALLOCATOR_BINARYTRACKER_H
#define RANGEALLOCATOR_BINARYTRACKER_H
#include "BinaryTrackerNode.h"

#include <unordered_map>

#include "NodeRequestHandler.h"

class BinaryTracker {

public:
    BinaryTracker(const BinaryTracker&) = delete;
    BinaryTracker& operator=(const BinaryTracker&) = delete;

    ~BinaryTracker();
    BinaryTracker(int initSize, const NodeRequestHandler* handler);

    int request(int size);
    void giveBack(int pos, int size);

private:
    bool rmvNode(Node* node);
    bool addNode(Node* node);

    const NodeRequestHandler* _handler;

    int _initSize;
    Node* _root;

    std::pmr::unordered_map<int, Node*> _nodeFronts;
    std::pmr::unordered_map<int, Node*> _nodeBacks;
    std::pmr::unordered_map<int, int> _allocated;

    int _wholeSize;

};


#endif //RANGEALLOCATOR_BINARYTRACKER_H