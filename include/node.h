#ifndef NODE__H
#define NODE__H

typedef struct node{
    char data;
    struct node* left;
    struct node* right;
    long frequency;
}node;


void newNode(node* nodePtr, char data, long frequency);

// Return -1 if thisNode's frequency is greater than otherNode's
// This ensures that the array is sorted in descending order, which helps
// later's insertion and merging of nodes
// REQUIRES: thisNode and otherNode point to valid, instantiated node structs
int nodeCmp(const void* thisNodePtr, const void* otherNode);


// Merge the two child nodes into the node pointed to by target
// REQUIRES: none of the pointer is NULL
void nodeMerge(node* target, node* childLeft, node* childRight);

/*
 * Insert toInsesrt into the range of nodePtrArr [0,insertRange)
 * Assuption: 1. nodePtrArr is sorted in descending order from 0 to insertRange
 *            2. nodePtrArr's length >= insertRange
 */
void nodeInsertToSortedArray(node* toInsert, node* nodePtrArr[], int insertRange);

/*
 * Print out the tree contained in this node.
 * @param: root - represents the root of this node tree
 */
void printfNodeTree(const node * root);

#endif
