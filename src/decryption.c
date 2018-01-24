#include "decryption.h"
#include <stdlib.h>
#include "boolean.h"
#include <string.h>

/* Function: readBitsWrite
 * ========================
 * Read a single byte each time from encryptedFile, and trace a temp node down the
 * root of nodeTree based on the bit-pattern of the byte reading from left
 * to right(higher significant bit to lower, as per the masks)
 * Whenever the temp node reaches a data-node, write the data to decodedFile,
 * and place temp back to beginning again.
 * If the last byte has been read, read only finalBits # of bits from that byte
 * ===========================
 * @param
 * 1. encryptedFile: The file containing encoded huffman code and other
 * pre-processing information(finalBits and tree info)
 * 2. decodedFile: The file to which decoded messages will be written to
 * 3. nodeTree: The node tree constructed based on info at beginning of
 * encryptedFile; it will be used for deciphering the bit patterns.
 * 4. finalBits: the number of valid bits(excluding padded bits) to be
 * read from the last byte
 */
static void readBitsWrite(FILE* encryptedFile, FILE* decodedFile, node* nodeTree, char finalBits) {
  static unsigned char masks[] = 
  { 0b10000000,
    0b01000000,
    0b00100000,
    0b00010000,
    0b00001000,
    0b00000100,
    0b00000010,
    0b00000001};
  node* temp = nodeTree;
  bool flag = true;

  if (temp->hasData) {
    printf("special case, where original text only has one kind of char\n");
    while(flag) {
      //unsigned char c = fgetc(encryptedFile);
      int bitsToRead = 8;
      if (fgetc(encryptedFile) == EOF) {
        //printf("\nend Reached!, %d to read, %d\n",finalBits, c);
        bitsToRead = finalBits;
        flag = false;
      }
      fseek(encryptedFile, -1L, SEEK_CUR);
      for (int i = 0; i < bitsToRead; i++) {
          fputc(temp->data, decodedFile);
          //putchar(temp->data);
      }
    }
    return;
  }

  while (flag) {
    unsigned char c = fgetc(encryptedFile);
    int bitsToRead = 8;
    if (fgetc(encryptedFile) == EOF) {
      //printf("\nend Reached!, %d to read, %d\n",finalBits, c);
      bitsToRead = finalBits;
      flag = false;
    }
    fseek(encryptedFile, -1L, SEEK_CUR);
    for (int i = 0; i < bitsToRead; i++) {
      if ((c & masks[i]) != 0) {
        temp = temp->right;
        //putchar('1');
      } else {
        temp = temp->left;
        //putchar('0');
      }
      if (temp->hasData) {
        fputc(temp->data, decodedFile);
        //putchar(temp->data);
        temp = nodeTree;
      }
    }
  }
}


/* Function: generateNodeTree
 * =========================
 * Based on the tree information included at the beginning of encryptedFile
 * (see encrypt.c for a more thorough description for this)
 * construct a node tree which can then be used for decoding the actual
 * huffman code in encryptedFile
 * ==========================
 * @param:
 * 1. encryptedFile: The huffman code file that also contains information
 * about the node tree used to encode it.
 * 2. root: the root from which the tree(part of a tree) will be constructed
 * 3. sentinel: addresses a char used to indicate node that contains no data,
 * but two childNodes.
 * 4. passedLeftmost: addresses a boolean used to indicate whether the function
 * has passed the leftmost Node. After which point the message will use that
 * specific node's data as a new sentinel. (see encrypt.c for reasons behind this)
 */
static void generateNodeTree(FILE* encryptedFile, node* root, 
    unsigned char* sentinel, bool* passedLeftmost) {
  unsigned char data = getc(encryptedFile);
  if (data != (*sentinel)) {
    //In this case the data represents a node with data
    if (!(*passedLeftmost)) {
      *sentinel = data;
      *passedLeftmost = true;
    }
    //printf("read %c\n", data);
    newNode(root, data, 0);
  } else {
    //In this case the data represents a node without data
    putchar((*sentinel));
    root->left = (node*) malloc(sizeof(node));
    root->right = (node*) malloc(sizeof(node));
    root->hasData = false;
    root->frequency = 0;
    root->data = 0;
    generateNodeTree(encryptedFile, root->left, sentinel, passedLeftmost);
    generateNodeTree(encryptedFile, root->right, sentinel, passedLeftmost);
    //printf("left is%c, right is %c\n", root->left->data, root->right->data);
  }
}


void decrypt(const char* fileName){
  FILE* file = fopen(fileName, "r");
  if (file == NULL) {
    fprintf(stderr, "Cannot open/write to file %s\n", fileName);
    exit(EXIT_FAILURE);
  }

  char finalBitsToread = getc(file);
  unsigned char sentinel = getc(file);
  bool passedLeftmost = false;
  node* root = (node*) malloc(sizeof(node));
  generateNodeTree(file, root, &sentinel, &passedLeftmost);
  printf("node tree generated\n");

  char decodedFileName[strlen(fileName) + 1];//.huff suffix into .de
  strcpy(decodedFileName, fileName);
  decodedFileName[strlen(fileName) - 4] = 'd';
  decodedFileName[strlen(fileName) - 3] = 'e';
  decodedFileName[strlen(fileName) - 2] = '\0';

  FILE* decodedFile = fopen(decodedFileName, "w");
  if (decodedFile == NULL) {
    fprintf(stderr, "Cannot open/write to the file: %s\n", decodedFileName);
    exit(EXIT_FAILURE);
  }
  readBitsWrite(file, decodedFile, root, finalBitsToread);

  printfNodeTree(root);
  disposeNodeTree(root);
  fclose(file);
  fclose(decodedFile);
}
