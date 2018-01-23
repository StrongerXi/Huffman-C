#ifndef ENCRYPTION__H
#define ENCRYPTION__H
#include<stdlib.h>
#include<stdio.h>
#include"node.h"

typedef struct bitcode {
  int numOfBits;
  unsigned char bits[256];
} bitcode;

/* Function: readFrequency
 * =======================
 * Read in each unsigned char and count their frequency
 * =======================
 * @param:
 * 1. counts: An array of 256 unsigned longs.
 * 2. file : a pointer to the FILE to read from
 * @requires:
 * 1. file != NULL
 * 2. file is readable.(has access)
 * @ensures:
 * 1. counts[i] represents the frequency of unsigned char i in file
 */
void readFrequency(unsigned long counts[256], FILE* file);

/* Function: generateNodePtrArray
 * ================================
 * Construct an array of pointers to nodes with non-zero frequency unsigned chars,
 * based on frequencies recored in counts
 * ================================
 * @requires::
 * 1. counts is an array of 256 unsigned longs, where counts[i]
 * represents the frequency of unsigned char i;
 * 2. nodePtrs should be an array of n node*, which addresses valid memory
 * spaces for n nodes. n should be the number of non-zero frequencies in counts
 */
void generateNodePtrArray(node* nodePtrs[], unsigned long counts[]);


/* Function: generateEncodeMap
 * ============================
 * Based on the constructed node tree, construct huffman bit code
 * for the unsigned character held at each node. 
 * After the function application, the bit-code of unsigned char c will be
 * stored in encodeMap[c].
 * Not all elements in encodeMap have to contain huffman code,
 * the non-zero element in this array should equal to the # of nodes.
 * ==============================
 * @param:
 * 1. encodeMap: An array of struct bitcode. It stores the encoding
 * information of unsigned char c at encodeMap[c].
 * 2. rootNode: Addresses the root node of a pre-constructed nodeTree
 * 3. bitCOde: An accumulator for the current bitCode so far 
 * 4. bitsNum: An accumulator that keeps track of the number of bits
 * stored so far.
 */
void generateEncodeMap(bitcode encodeMap[256],
                        node* rootNode, unsigned char bitCode[256], int bitsNum);

/* Function: encrypt
 * ==================
 * The main function for huffman encryption. 
 * Encrypt the file with given fileName
 * Write encrypted message to a file "fileName.huff"
 * The encrypted message is decodable by the same program, 
 * using decrypt("fileName.huff")
 * ===================
 * @param: 
 * 1. fileName: the name of the file being encrypted.
 * @requires:
 * 1. fileName is readable, and exists.
 */
void encrypt(char* fileName);

#endif
