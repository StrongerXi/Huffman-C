#ifndef ENCRYPTION__H
#define ENCRYPTION__H
#include<stdlib.h>
#include<stdio.h>
#include"node.h"

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
