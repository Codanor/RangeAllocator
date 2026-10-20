//
// Created by Tim Klöpper on 10/20/2026.
//

#include "NodeRequestHandler.h"

Node* BestFitRequestHandler::findNode(const int size, Node *root) const {
    Node* best = nullptr;
    Node* current = root;

    while (current != nullptr) {
        if (current->getSize() >= size) {
            if (best == nullptr) best = current;
            if (current->getSize() < best->getSize()) best = current;

            current = current->getLower();
            continue;
        }
        current = current->getHigher();
    }

    return best;
}

Node* FirstFitRequestHandler::findNode(const int size, Node *root) const {
    Node* current = root;

    while (current != nullptr) {
        if (current->getSize() >= size) return current;
        current = current->getHigher();
    }

    return nullptr;
}