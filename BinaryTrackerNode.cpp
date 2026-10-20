//
// Created by Tim Klöpper on 10/20/2026.
//

#include "BinaryTrackerNode.h"

#include <utility>

Node::Node(int size, int pos) {
    _size = size;
    _pos = pos;
    _lower = nullptr;
    _higher = nullptr;
}

Node *Node::getHigher() const {
    return _higher;
}
Node *Node::getLower() const {
    return _lower;
}

void Node::setHigher(Node* higher) {
    _higher = higher;
}
void Node::setLower(Node *lower) {
    _lower = lower;
}

void Node::rmvHigher() {
    if (_higher == nullptr) return;
    _higher->rmvHigher();
    _higher->rmvLower();

    delete _higher;

    _higher = nullptr;
}
void Node::rmvLower() {
    if (_lower == nullptr) return;
    _lower->rmvHigher();
    _lower->rmvLower();

    delete _lower;

    _lower = nullptr;
}

int Node::getPos() const {
    return _pos;
}
int Node::getSize() const {
    return _size;
}

bool Node::rmvNode(Node *node) {
    if (node == nullptr) return false;

    if (node->getSize() < _size) {
        if (_lower == nullptr) return false;
        if (_lower != node) return _lower->rmvNode(node);

        Node* lLower = _lower->_lower;
        Node* lHigher = _lower->_higher;

        if (lLower != nullptr) {
            _lower = lLower;
            _lower->addNode(lHigher);
        } else {
            _lower = lHigher;
        }

        delete node;
        return true;
    }

    if (_higher == nullptr) return false;
    if (_higher != node) return _higher->rmvNode(node);

    Node* hLower = _higher->_lower;
    Node* hHigher = _higher->_higher;

    if (hHigher != nullptr) {
        _higher = hHigher;
        _higher->addNode(hLower);
    } else {
        _higher = hLower;
    }

    delete node;
    return true;
}
bool Node::addNode(Node *node) {
    if (node == this) return false;
    if (node == nullptr) return false;

    if (node->getSize() < _size) {
        if (_lower != nullptr) return _lower->addNode(node);
        _lower = node;

        return true;
    }

    if (_higher != nullptr) return _higher->addNode(node);
    _higher = node;

    return true;
}