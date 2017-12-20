#include "node.h"
#include <stdlib.h>
#include <assert.h>
#include <stdio.h>
#include <string.h>

static void printNode(const node * thisNode, int numOfTabs);

void newNode(node* nodePtr, char data, long frequency){
  assert(nodePtr != NULL);
  
  nodePtr->data = data;
  nodePtr->frequency = frequency;
  nodePtr->left = NULL;
  nodePtr->right = NULL;
}

int nodeCmp(const void* thisNodePtr, const void* otherNodePtr)
{

  node* thisN = *(node**) thisNodePtr;
  node* otherN = *(node**) otherNodePtr;
  //printf("comparing %c %ld against %c %ld\n", thisN->data, thisN->frequency, otherN->data, otherN->frequency);
  if(thisN->frequency > otherN->frequency){
    return -1;
  }else{
    return 1;
  }
}

void nodeMerge(node* target, node* childLeft, node* childRight){
  target->data = 0;
  target->left = childLeft;
  target->right = childRight;
  target->frequency = childLeft->frequency + childRight->frequency;
}

void nodeInsertToSortedArray(node* toInsert, node* nodePtrArr[], int arrLen)
{
  int i = 0;
  for(; i < arrLen; i++){
    if(nodeCmp(&toInsert, &nodePtrArr[i]) == -1) break;
  }
  printf("inserting %ld, before %c %ld\n", toInsert->frequency, nodePtrArr[i]->data, nodePtrArr[i]->frequency);
  memmove(&nodePtrArr[i+1], &nodePtrArr[i], (arrLen-i) * sizeof(node*));
  memcpy(&nodePtrArr[i], &toInsert, sizeof(node*));
}


void printfNodeTree(const node * root){
    printNode(root,0);
}

/*
 * print out the node and all the nodes contained within thisNode
 * recursively
 */
static void printNode(const node * thisNode, int numOfTabs){

    if(thisNode->left == NULL){
        char line[numOfTabs+1];

        for(int i = 0; i < numOfTabs; i++)
            line[i] = '\t';

        line[numOfTabs] = '\0';
        printf("%s%c : %ld\n", line, thisNode->data, thisNode->frequency);
    }else{
        printNode(thisNode->left, numOfTabs+1);
        printNode(thisNode->right, numOfTabs+1);
    }
}
