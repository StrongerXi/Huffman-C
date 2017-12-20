#include "node.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

void decrypt(char* fileName){
  FILE* file = fopen(fileName, "r");
  fclose(file);
}

// Read in each char and count their frequency
void readFrequency(long counts[], FILE* file);


// Construct an array of ptr to nodes with non-zero frequency chars
void generateNodePtrArray(node* nodePtrs[], long counts[]);

void encrypt(char* fileName){
  FILE* file = fopen(fileName, "r");
  printf("file opened, filename: %s\n", fileName);

  long counts[256] = {0};
  readFrequency(counts, file);

  int numOfNodes = 0;
  for(int i = 0; i < 256; i++){
    if(counts[i] > 0){
      printf("%c has frequency %ld\n", i, counts[i]);
      numOfNodes++; 
    }
  }

  node* nodePtrs[numOfNodes];
  generateNodePtrArray(nodePtrs, counts);

  // Sort the array of nodes into descending order based on frequency
  qsort(nodePtrs, numOfNodes, sizeof(node*), nodeCmp);
  for(int i = 0; i < numOfNodes; i++){
    printf("after sorting, %c has %ld frequency, at index %d\n", nodePtrs[i]->data, nodePtrs[i]->frequency, i);
  }

  // Take the last two nodes(the least frequencies) and make them into a new node N
  // Remove the last two nodes, and insert N into the remaining array(length -= 2 now, length++
  // after insertion)
  // Repeat until there is only one last node(repeat for numOfNodes-1 times
  
  for(int i = numOfNodes-2; i >= 0; i--){
    node* mergeTarget = (node*) malloc(sizeof(node));
    assert(mergeTarget != NULL);
    nodeMerge(mergeTarget, nodePtrs[i], nodePtrs[i+1]);
    nodeInsertToSortedArray(mergeTarget, nodePtrs, i);
  }

  printf("root node established!\n");

  node* root = nodePtrs[0];
  printfNodeTree(root);

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
