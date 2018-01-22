#ifndef NODE__H
#define NODE__H

typedef struct node{
    char hasData;
    unsigned char data;
    struct node* left;
    struct node* right;
    unsigned long frequency;
}node;


/* Function: newNode
 * -----------------
 * Initiate a new node at the memory space addressed by nodePtr
 * Set the node's data and frequency based on given inputs
 * Set the childNode pointers to NULL
 * -----------------
 * @param:
 * 1. nodePtr: A pointer that addresses the node to be initiated
 * 2. data: the data this node will hold
 * 3. frequency the frequency this node will hold
 */
void newNode(node* nodePtr, unsigned char data, unsigned long frequency);

/* Function: disposeNodeTree
 * =========================
 * Dispose any allocated memory contained in any of the nodes
 * within the node tree, rooted at the node addressed by nodePtr
 * =========================
 * @param:
 * 1. root : The relative root of the node tree
 * @requires:
 * 1. root != NULL
 * NOTE:
 * In this program, the memories for actual nodes are allocated
 * either in generateNodeArray or makeNodeTree. However, they should
 * all be contained inside a node tree, thus allowing a systematic cleanup
 */
void disposeNodeTree(node* root);

/* Function: nodeCmp
 *-------------------
 * Return -1 if thisNode's frequency is greater than otherNode's
 * This ensures that the array is sorted in descending order when 
 * applied to library qsort, which helps insertion and merging of nodes
 * -------------------
 * @param:
 * 1. thisNodePtr: a pointer that addresses thisNode to be compared
 * 2. otherNodePtr: a pointer that addresses otherNode to be compared
 * @requires: thisNode and otherNode point to valid, instantiated node structs
 * @returns:
 * -1 if thisNode.frequency > otherNode.frequency
 * 1  if .................. < ...................
 * 0  if .................. == ..................
 */
int nodeCmp(const void* thisNodePtr, const void* otherNodePtr);

/* Function: nodeMerge
 * ---------------------------
 * Merge the two child nodes and set the fields of the target node
 * based on the result of merged child nodes. It's frequency will be
 * the sum of its childNodes' frequencies
 * ============================
 * @param:
 * 1. target: A pointer to the node that will become the merged result of childNodes
 * 2. childLeft: The would-be left child of merged node.
 * 3. childRight: The would-be right child of merged node.
 * @requires: All input pointers != NULL
 * @ensures:
 * 1. target->left == childLeft
 * 2. target->right == childRight
 * 3. target->frequency == childLeft->frequency + childRight->frequency
 */
void nodeMerge(node* target, node* childLeft, node* childRight);

/* Function: nodeInsertToSortedArray
 * =================================
 * Insert toInsesrt into the range of nodePtrArr [0,arrLen)
 * Assuption: 1. nodePtrArr is sorted in descending order from 0 to arrLen
 */
void nodeInsertToSortedArray(node* toInsert, node* nodePtrArr[], int arrLen);

/* Function: printNodeTree
 * =============================
 * Print out the tree contained in this node.
 * Use tabs to represent node's depth so that printout resembles a 
 * horizontal tree
 * =============================
 * @param:
 * 1. root - represents the root of this node tree
 * @requires:
 * 1. root != NULL
 */
void printfNodeTree(const node * root);

#endif
