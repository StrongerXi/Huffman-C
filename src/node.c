#include "node.h"
#include <stdlib.h>
#include <assert.h>
#include <stdio.h>
#include <string.h>

/* Function: printNode
 * ---------------------
 * print out the node and all the nodes contained within thisNode
 * recursively
 * @param:
 * 1. thisNode: a pointer to the current relative root node
 * 2. numOfTabs: an accumulator that represents the number of tabs
 * that should be in the prefix of the node's data. This creates a
 * printout that suggests the depth of each node.
 * @requires:
 * 1. thisNode != NULL
 * 2. numOfTabs >= 0
 */
static void printNode(const node * thisNode, int numOfTabs){

    if(thisNode->left == NULL){
        unsigned char line[numOfTabs+1];

        for(int i = 0; i < numOfTabs; i++)
            line[i] = '\t';

        line[numOfTabs] = '\0';
        printf("%s%c : %ld\n", line, thisNode->data, thisNode->frequency);
    }else{
        printNode(thisNode->right, numOfTabs+1);
        printNode(thisNode->left, numOfTabs+1);
    }
}

void newNode(node* nodePtr, unsigned char data, unsigned long frequency){
  assert(nodePtr != NULL);
  
  nodePtr->hasData = 1;
  nodePtr->data = data;
  nodePtr->frequency = frequency;
  nodePtr->left = NULL;
  nodePtr->right = NULL;
}

void disposeNodeTree(node* root){
  if (root != NULL) {
    disposeNodeTree(root->left);
    disposeNodeTree(root->right);
    free(root);
  }
}


int nodeCmp(const void* thisNodePtr, const void* otherNodePtr) {

  node* thisN = *(node**) thisNodePtr;
  node* otherN = *(node**) otherNodePtr;
  //printf("comparing %c %ld against %c %ld\n", thisN->data, thisN->frequency, otherN->data, otherN->frequency);
  if(thisN->frequency > otherN->frequency){
    return -1;
  }else if(thisN->frequency < otherN->frequency){
    return 1;
  }else{
    return 0;
  }
}

void nodeMerge(node* target, node* childLeft, node* childRight){
  target->data = 0;
  target->hasData = 0;
  target->left = childLeft;
  target->right = childRight;
  target->frequency = childLeft->frequency + childRight->frequency;
}

void nodeInsertToSortedArray(node* toInsert, node* nodePtrArr[], int arrLen) {
  int i = 0;
  for(; i < arrLen; i++){
    if(nodeCmp(&toInsert, &nodePtrArr[i]) == -1) break;
  }
  //printf("inserting %ld, before %c %ld\n", toInsert->frequency, nodePtrArr[i]->data, nodePtrArr[i]->frequency);
  memmove(&nodePtrArr[i+1], &nodePtrArr[i], (arrLen-i) * sizeof(node*));
  memcpy(&nodePtrArr[i], &toInsert, sizeof(node*));
}


void printfNodeTree(const node * root){
    printNode(root,0);
}

