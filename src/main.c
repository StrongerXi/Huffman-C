#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "encryption.h"

void decrypt(char* fileName){
  FILE* file = fopen(fileName, "r");
  fclose(file);
}



int main(int argc, char* argv[]){
  if(argc != 3){
    fprintf(stderr, "Usage: huffman -flag filename\n flags: -e (encrypt); -d (decrypt)\n");
    exit(EXIT_FAILURE);
  }
  
  char* flag = argv[1];
  char* fileName = argv[2];

  if(strcmp(flag, "-d") == 0){
    decrypt(fileName);
  }else if(strcmp(flag, "-e") == 0){
    encrypt(fileName);
  }else{
    fprintf(stderr, "Usage: huffman -flag filename\n flags: -e (encrypt); -d (decrypt)\n");
    exit(EXIT_FAILURE);
  }
}
