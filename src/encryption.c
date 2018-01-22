#include "encryption.h"
#include <assert.h>
#include <string.h>
#include <stdio.h>

/* Function: nonZeroFreqs
 * ----------------------
 * @param: counts; an array of 256 unsigned longs such that
 * counts[i] represents the frequency of unsigned char i
 * @return: the number of unsigned character with non-zero frequency
 * according to counts
 */
static int nonZeroFreqs(unsigned long counts[256]) {
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

void generateEncodeMap(bitcode encodeMap[256],
                        node* rootNode, unsigned char bitCode[256], int bitsNum) {
  if (rootNode->hasData == 0) {
    bitCode[bitsNum] = '0';
    generateEncodeMap(encodeMap, rootNode->left, bitCode, bitsNum + 1);
    bitCode[bitsNum] = '1';
    generateEncodeMap(encodeMap, rootNode->right, bitCode, bitsNum + 1);
  } else {
    bitcode* targetStruct = encodeMap + rootNode->data;
    targetStruct->numOfBits = bitsNum;
    memcpy(targetStruct->bits, bitCode, bitsNum);
  }
}

/* Function: writeBits:
 * =======================
 * A method with an internal static buffer, which helps writing individual
 * bits into the file writeTo.
 * flush the buffer immediately if endFlush is set to 1
 * At final flush, write down the current bufferPos(indicating number of
 * valid bits in the buffer) to the reserved first byte at file
 * writeTo.
 */
static void writeBits(bitcode code, FILE* writeTo, char endFlush) {
  static unsigned char bitBuffer = 0;
  static unsigned char bufferPos = 0;
  static int bufferSize = sizeof(char) * 8;
  static unsigned char mask = 1 << (sizeof(char)*8 - 1);

  if (endFlush == 1) {
    putc(bitBuffer, writeTo);
    rewind(writeTo);
    // This puts a char at the beginning which indicates the number of
    // bits to be read near at the end of file
    putc(bufferSize, writeTo);
  }

  for(int i = 0; i < code.numOfBits; i++) {
    if (bufferPos == bufferSize) {
      putc(bitBuffer, writeTo);
      bitBuffer = 0;
      bufferPos = 0;
    }
    if (code.bits[i] == '0') {
      bitBuffer &= (~(mask >> bufferPos));
    } else {
      bitBuffer |= mask >> bufferPos;
    }
    bufferPos++;
  }
}

void encrypt(char* fileName){
  FILE* file = fopen(fileName, "r");
  if (file == NULL) {
    fprintf(stderr, "%scannot be opened, either file does not exist or there are other problems unknown.\n", fileName);
  }

  printf("file opened, filename: %s\n", fileName);

  /* read in frequency of the 256 possible unsigned characters in fileName
   * then count the number of unsigned chars with non-zero frequency
   */
  unsigned long counts[256] = {0};
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

  bitcode encodeMap[256];
  unsigned char bitAccumulator[256];
  generateEncodeMap(encodeMap, rootNode, bitAccumulator, 0);
  printf("encode map generated!\n");
  
  char encryptFileName[strlen(fileName)+1];
  strcpy(encryptFileName, fileName);
  strcat(encryptFileName, ".huff");

  FILE* encryptedFile = fopen(encryptFileName, "w");
  if (encryptedFile == NULL) {
    fprintf(stderr, "%scannot be opened/created for writing encrypted message\n", encryptFileName); 
  }
  //See writeBits function for the purpose of this reserved byte
  putc('0', encryptedFile);
  rewind(file);
  int c = '0';
  while(1) {
    c = fgetc(file);
    if (feof(file)) {
      writeBits(encodeMap[c], encryptedFile, 1);
      break;
    } else {
      writeBits(encodeMap[c], encryptedFile, 0);
    }
  }
  
  disposeNodeTree(rootNode);
  fclose(file);
  fclose(encryptedFile);
}

void readFrequency(unsigned long counts[256], FILE* file){
  char ch;
  while((ch = fgetc(file)) != EOF){
    counts[(unsigned char) ch]++;
  }
  printf("frequencies reading finished\n");
}

void generateNodePtrArray(node* nodePtrs[], unsigned long counts[]){
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


