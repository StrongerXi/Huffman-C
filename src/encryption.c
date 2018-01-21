#include "encryption.h"
#include <assert.h>

/* Function: nonZeroFreqs
 * ----------------------
 * @param: counts; an array of 256 longs such that
 * counts[i] represents the frequency of char i
 * @return: the number of character with non-zero frequency
 * according to counts
 */
static int nonZeroFreqs(long counts[256]) {
  int numOfNodes = 0;
  for(int i = 0; i < 256; i++){
    if(counts[i] > 0){
      printf("%c has frequency %ld\n", i, counts[i]);
      numOfNodes++; 
    }
  }
  return numOfNodes;
}

/* Function: makeNodeTree
 * ----------------------
 * Take the last two nodes(the least frequencies) and make them into a new node N
 * Remove the last two nodes, and insert N into the remaining array
 * (length -= 2 now, length++ after insertion)
 * Repeat until there is only one last node(repeat for numOfNodes-1 times
 * ----------------------
 * @param:
 * 1. nodePtrs: an array of pointers to already generated nodes
 * 2. numOfNodes: the length of nodePtrs (i.e. number of nodes) 
 * @return:
 * a pointer to the root node of the constructed node tree
 */
 
static node* makeNodeTree(node* nodePtrs[], int numOfNodes) {

 for(int i = numOfNodes-2; i >= 0; i--){
    node* mergeTarget = (node*) malloc(sizeof(node));
    assert(mergeTarget != NULL);
    nodeMerge(mergeTarget, nodePtrs[i], nodePtrs[i+1]);
    nodeInsertToSortedArray(mergeTarget, nodePtrs, i);
  }
 return nodePtrs[0];
}

void encrypt(char* fileName){
  FILE* file = fopen(fileName, "r");
  printf("file opened, filename: %s\n", fileName);

  /* read in frequency of the 256 possible characters in fileName
   * then count the number of chars with non-zero frequency
   */
  long counts[256] = {0};
  readFrequency(counts, file);
  int numOfNodes = nonZeroFreqs(counts);
  node* nodePtrs[numOfNodes];

  /* Generate and Sort the array of nodes into 
   * descending order based on frequency
   */
  generateNodePtrArray(nodePtrs, counts);
  qsort(nodePtrs, numOfNodes, sizeof(node*), nodeCmp);

  /* Construct a node tree based on the node addressed in nodePtrs*/
  node* rootNode = makeNodeTree(nodePtrs, numOfNodes);

  printf("root node established!\n");
  printfNodeTree(rootNode);

  fclose(file);
}


void readFrequency(long counts[], FILE* file){
  char ch;
  while((ch = fgetc(file)) != EOF){
    counts[(int) ch]++;
  }
  printf("frequencies reading finished\n");
}

void generateNodePtrArray(node* nodePtrs[], long counts[]){
  int index = 0;
  for(int i = 0; i < 256; i++){
    if(counts[i] > 0){
      nodePtrs[index] = (node*) malloc(sizeof(node));
      assert(nodePtrs[index] != NULL);
      newNode(nodePtrs[index++], i, counts[i]);
      printf("Node created with data %c, frequency %ld, and index %d\n", nodePtrs[index-1]->data, nodePtrs[index-1]->frequency, index-1);
    }
  }
  printf("array of nodes with non-zero frequency established\n");
}


