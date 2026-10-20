//
// Created by Tim Klöpper on 10/20/2026.
//

#ifndef RANGEALLOCATOR_BINARYTRACKERNODE_H
#define RANGEALLOCATOR_BINARYTRACKERNODE_H


class Node {

public:
    Node(int size, int pos);

    void setLower(Node* lower);
    void setHigher(Node* higher);

    Node* getLower() const;
    Node* getHigher() const;

    void rmvLower();
    void rmvHigher();

    int getSize() const;
    int getPos() const;

    bool rmvNode(Node* node);
    bool addNode(Node* node);

private:
    int _size;
    int _pos;
    Node* _lower;
    Node* _higher;

};


#endif //RANGEALLOCATOR_BINARYTRACKERNODE_H