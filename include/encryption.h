#ifndef ENCRYPTION__H
#define ENCRYPTION__H
#include<stdlib.h>
#include<stdio.h>
#include"node.h"

// Read in each char and count their frequency
void readFrequency(long counts[], FILE* file);

/* Construct an array of ptr to nodes with non-zero frequency chars
 * Required: counts is an array of 256 longs, where counts[i]
 * represents the frequency of char i;
 * nodePtrs should be an array of 256 node* as well
 */
void generateNodePtrArray(node* nodePtrs[], long counts[]);

// Encrypt the file with given fileName
// Write encrypted message to a file "fileName.huff"
void encrypt(char* fileName);

#endif
