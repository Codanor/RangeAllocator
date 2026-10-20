//
// Created by Tim Klöpper on 10/20/2026.
//

#ifndef RANGEALLOCATOR_NODEREQUESTHANDLER_H
#define RANGEALLOCATOR_NODEREQUESTHANDLER_H
#include "BinaryTrackerNode.h"


class NodeRequestHandler {

public:
    virtual ~NodeRequestHandler() = default;

    virtual Node* findNode(int size, Node* root) const = 0;

};

class BestFitRequestHandler final : public NodeRequestHandler {

public:
    Node *findNode(int size, Node *root) const override;

};

class FirstFitRequestHandler final : public NodeRequestHandler {

public:
    Node *findNode(int size, Node *root) const override;

};


#endif //RANGEALLOCATOR_NODEREQUESTHANDLER_H