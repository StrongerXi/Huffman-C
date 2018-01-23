#include "encryption.h"
#include <assert.h>
#include <string.h>
#include <stdio.h>
#include "boolean.h"


/*
 * ===============================================================
 *
 *            STATIC  FUNCTIONS  SECTION  BEGINNING
 *
 * ===============================================================
 */

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


/* Function: writeBits:
 * =======================
 * A method with an internal static buffer, which helps writing individual
 * bits into the file writeTo.
 * flush the buffer immediately if endFlush is set to true
 * At final flush, write down the current bufferPos(indicating number of
 * valid bits in the buffer) to the reserved first byte at file
 * writeTo.
 * ========================
 * @param:
 * 1. code: An array of '0' or '1' char that indicate the bit pattern
 * to be encoded. 
 * 2. bitsLen: indicate the number of char to be read from code.
 * 3. writeTo: the file to write encrypted huffman message to.
 * 4. endFlush: a boolean that indicates whether the function execute the final flush 
 * as described above.
 * =========================
 * @requires:
 * 1. code is a valid array of '0' or '1' char.
 * 2. bitsLen indicate the correct number of bits to be read.
 * 3. writeTo != NULL
 */
static void writeBits(unsigned char code[], int bitsLen, FILE* writeTo, bool endFlush) {
  static unsigned char bitBuffer = 0;
  static unsigned char bufferPos = 0;
  static int bufferSize = sizeof(char) * 8;
  static unsigned char mask = 1 << (sizeof(char)*8 - 1);

  for(int i = 0; i <  bitsLen; i++) {
    if (bufferPos == bufferSize) {
      putc(bitBuffer, writeTo);
      bitBuffer = 0;
      bufferPos = 0;
    }
    if (code[i] == '0') {
      bitBuffer &= (~(mask >> bufferPos));
    } else {
      bitBuffer |= mask >> bufferPos;
    }
    putchar(code[i]);
    bufferPos++;
  }

  if (endFlush) {
    putc(bitBuffer, writeTo);
    rewind(writeTo);
    // This puts a char at the beginning which indicates the number of
    // bits to be read near at the end of file
    putc(bufferPos, writeTo);
    printf("%d to read\n", (bufferPos == 0) ? 8 : bufferPos); // If it's 0, it 
    //indicates the case that original file only has one character.
    //In such a circumstance, all bits 
    return;
  }
}


/* Function: writeTreeMessage
 * ===========================
 * Encode message about the node tree into the encrypted file so that
 * the decryptor can use this information to reconstruct the tree and
 * therefore decrypt the huffman code into original text.
 * ===========================
 * @param:
 * 1. root : the root node of the current tree to be encrypted
 * 2. writeTo: the output encrypted huffman code file
 * 3. sentinel: addresses a char to represent the node that contains no data, but
 * two child nodes.
 * 4. passedLeftmost: addresses boolean which indicates whether the function
 * has encoded the leftmost node of the original tree.
 *
 * NOTE:
 * In theory, this leftmost node will always be the first non-sentinal char
 * in the key message, thus it's safe to use the data of this node
 * as sentinal after this node is encountered.
 * This eliminates all possibility of intermingled data and sentinel
 */
static void writeTreeMessage(node* root, FILE* writeTo, unsigned char* sentinel, bool* passedLeftmost) {
  if (root->hasData) {
    if (!(*passedLeftmost)) {
      *sentinel = root->data;
      *passedLeftmost = true;
    }
    putc(root->data, writeTo);
  } else {
    putc((*sentinel), writeTo);
    writeTreeMessage(root->left, writeTo, sentinel, passedLeftmost);
    writeTreeMessage(root->right, writeTo, sentinel, passedLeftmost);
  }
}


/* Function: writeEncoded
 * =======================
 * Given the entire map for encoded bit pattern for each char,
 * read in each char from the original file and write the corresponding
 * bit pattern to the encrypted file.
 * ========================
 * @param:
 * 1. encodedMap: an array of the bitcode struct, such that encodedMap[i]
 * contains the bit pattern for char i.
 * 2. readFrom: the original file to read message from.
 * 3. writeTo: the output file for encrypted huffman code.
 */
static void writeEncoded(bitcode encodedMap[], FILE* readFrom, FILE* writeTo) {
  rewind(readFrom);
  int c = '0';
  while(1) {
    c = fgetc(readFrom);
    if (fgetc(readFrom) == EOF) {
      printf("writing a %c with %d bits final\n", c, encodedMap[c].numOfBits);
      writeBits(encodedMap[c].bits, encodedMap[c].numOfBits, writeTo, true);
      break;
    } else {
      fseek(readFrom, -1L, SEEK_CUR);
      printf("writing a %c with %d bits\n", c, encodedMap[c].numOfBits);
      writeBits(encodedMap[c].bits, encodedMap[c].numOfBits, writeTo, false);
    }
  }
}


/* Function: initiateEncryptedFile
 * ==============================
 * Given the fileName of the file to be read from for encryption.
 * Create/Open another file with the same name, but suffixed with ".huff".
 * This .huff file would be where the encoded huffman bits are written to
 * ===============================
 * @param:
 * 1. fileName: Name of the original text file for encryption
 * @return:
 * A file pointer to the file where encrypted huffman code will be written
 */
static FILE* initiateEncrytedFile(const char* fileName) {
char encryptFileName[strlen(fileName)+1];
  strcpy(encryptFileName, fileName);
  strcat(encryptFileName, ".huff");
  FILE* encryptedFile = fopen(encryptFileName, "w");
  if (encryptedFile == NULL) {
    fprintf(stderr, "%scannot be opened/created for writing encrypted message\n", encryptFileName); 
  }
  //See writeBits function for the purpose of this reserved byte
  putc('0', encryptedFile);
  //==========================================================
  return encryptedFile;
}

/*
 * ===============================================================
 *
 *            STATIC  FUNCTIONS  SECTION  ENDING
 *
 * ===============================================================
 */



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


void generateEncodeMap(bitcode encodedMap[256],
                        node* rootNode, unsigned char bitCode[256], int bitsNum) {
  if (rootNode->hasData == false) {
    bitCode[bitsNum] = '0';
    generateEncodeMap(encodedMap, rootNode->left, bitCode, bitsNum + 1);
    bitCode[bitsNum] = '1';
    generateEncodeMap(encodedMap, rootNode->right, bitCode, bitsNum + 1);
  } else {
    bitcode* targetStruct = encodedMap + rootNode->data;
    targetStruct->numOfBits = bitsNum;
    memcpy(targetStruct->bits, bitCode, bitsNum);
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
  printfNodeTree(rootNode);

  /* Construct the encode map for each character encountered while reading*/
  bitcode encodedMap[256];
  unsigned char bitAccumulator[256];
  if (rootNode->hasData) {
    generateEncodeMap(encodedMap, rootNode, bitAccumulator, 1);
  } else {
    generateEncodeMap(encodedMap, rootNode, bitAccumulator, 0);
  }
  printf("encode map generated!\n");

  /* Create/open the file to which encoded huffman message will be written to*/
  FILE* encryptedFile = initiateEncrytedFile(fileName);

  /* retrives the leftmost node and compares whether its data is 'z'
   * Pass in an initial sentinal so that it differs from this node's data
   */
  node* leftMost = getLeftmostNode(rootNode);
  unsigned char sentinel = leftMost->data == 'z' ? 'y' : 'z';
  bool passedLeftmost = false;
  fputc(sentinel, encryptedFile);
  writeTreeMessage(rootNode, encryptedFile, &sentinel, &passedLeftmost);
  
  /* Start over reading the original file, for each character read,
   * encode its corresponding bit pattern into encryptedFile
   */
  writeEncoded(encodedMap,  file, encryptedFile);
  
  disposeNodeTree(rootNode);
  fclose(file);
  fclose(encryptedFile);
}
