/**
 * AHT20_BST.c - Source file for ECE 361 Homework 5 Problem 1
 *
 * @file:		AHT20_BST.c
 *
 * @author:		Keenan Mondragon (kmondra2@pdx.edu)
 * @date:		12/01/2024
 * @version:	1.0
 *
 * This is the source file of my AHT20 sensor binary search tree API.
 *
 */

#include "AHT20_BST.h"
#include <stdio.h>
#include <stdlib.h>
#include "iom361_r2.h"
#include <math.h>

#define MAX_TEMP 100
#define MIN_TEMP 0
#define MAX_HUMIDITY 99
#define MIN_HUMIDITY 20

uint32_t* io_base;

// Helper Function Prototypes
/**
 * @function displayNode()
 *
 * @param	node		pointer to the node of interest
 *
 * @brief
 * Displays the node data to user
 */
void displayNode(nodeBSTptr_t node) {
 time_t nodeTime = node->data.timestamp;
 struct tm *localConversion = localtime(&nodeTime);
 printf("%s  Temperature: %d   Humidity: %d\n", asctime(localConversion), node->data.temp, node->data.humidity);
}

/**
 * @function populateBST()
 *
 * @param startTime  - the first day to begin pulling AHT20 data
 * @param numEntries	- number of days to pull AHT20 data
 *
 * @return returns a pointer to a populated BST
 *
 * @brief
 * Uses the 361 IO module to create a BST and populate with the number of entries requested
 */
BSTptr_t populateBST(time_t startTime, int numEntries) {
 int *rtn_codePtr = NULL;
 int rtn_code = 0;

 // initialize IO module
 io_base = iom361_initialize(0, 0, &rtn_code);
 if (rtn_code != 0) {
  // initialization failed
  printf("ERROR (populateBST): Could not initialize I/O module\n");
  return NULL;
 }

 // populate data array, this gives sensor readings in series
 AHT20data_t data[numEntries];
 for (int i = 0; i < (numEntries); i++) {
  _iom361_setSensor1_rndm(MIN_TEMP, MAX_TEMP, MIN_HUMIDITY, MAX_HUMIDITY);
  data[i].timestamp = startTime + i*86400;
  data[i].temp = iom361_readReg(io_base, TEMP_REG, rtn_codePtr);
  data[i].temp = ((data[i].temp)/pow(2,20))*200-50;
  if (rtn_code != 0) {
   // readReg failed
   printf("ERROR (populateBST): Could not read temperature register from I/O module\n");
   return NULL;
  }
  //read humidity
  data[i].humidity = iom361_readReg(io_base, HUMID_REG, rtn_codePtr);
  data[i].humidity = ((data[i].humidity)/pow(2, 20))*100;
  if (rtn_code != 0) {
   // readReg failed
   printf("ERROR (populateBST): Could not read humidity register from I/O module\n");
   return NULL;
  }
 }

 srand(time(NULL));
 int shuffleArray[numEntries];

 //initializes array to be 1 through the number of entries
 for (int i = 0; i < (numEntries); i++) {
  shuffleArray[i] = i+1;
 }
 //shuffles array using Fisher-Yates shuffle
 for (int i = (numEntries - 1); i > 0; i--) {
  int j = rand() % (i+1);
  int temp = shuffleArray[i];
  shuffleArray[i] = shuffleArray[j];
  shuffleArray[j] = temp;
 }

 //Create BST, insert data nodes in shuffled order
 BSTptr_t tree = createBST();
 for (int i = 0; i < (numEntries); i++) {
  insertBSTnode(tree, data[shuffleArray[i]-1]);
 }

 return tree;
}

// API functions
void inorderDisplay(nodeBSTptr_t root) {
 if (root == NULL){
  return;
 }
 // Recursive inorder traversal (left-root-right) *utilizing Karumanchi BinaryTree inOrder function
 inorderDisplay(root->left);
 displayNode(root);
 inorderDisplay(root->right);
}

BSTptr_t createBST(void) {
 BSTptr_t pointer = (BSTptr_t) malloc(sizeof(BSTptr_t));
 if (pointer == NULL) {
 printf("ERROR: [createBST API] Memory allocation failed.\n");
 return NULL;
 }
 pointer->root = NULL;
 pointer->size = 0;
 return pointer;
}

nodeBSTptr_t insertBSTnode(BSTptr_t tree, AHT20data_t readings) {
 nodeBSTptr_t currNode = tree->root;
 nodeBSTptr_t newNode = (nodeBSTptr_t) malloc(sizeof(nodeBST_t));
 if (newNode == NULL) {
  printf("ERROR(insertBST API): Memory allocation failed.\n");
  return NULL;
 }
 newNode->data = readings;
 newNode->left = NULL;
 newNode->right = NULL;

 //Checks if tree is empty, if so inserts newNode into the root
 if (currNode == NULL) {
  tree->root = newNode;
  printf("INFO(insertBSTnode): added root node with timestamp %lld to AHT20 BST\n", readings.timestamp);
  tree->size++;
  return newNode;
 }

 //ends by returning from function
 while (1) {
  if (currNode->data.timestamp > readings.timestamp) {
   if (currNode->left == NULL) {
    currNode->left = newNode;
    printf("INFO(insertBSTnode): added left leaf with timestamp %lld to AHT20 BST\n", readings.timestamp);
    tree->size++;
    return newNode;
   }
   else {
    currNode = currNode->left;
   }
  }
   if (currNode->data.timestamp < readings.timestamp) {
    if (currNode->right == NULL) {
     currNode->right = newNode;
     printf("INFO(insertBSTnode): added right leaf with timestamp %lld to AHT20 BST\n", readings.timestamp);
     tree->size++;
     return newNode;
    }
    else {
     currNode = currNode->right;
    }
   }
    if (currNode->data.timestamp == readings.timestamp) {
     return NULL;
    }
 }
}

nodeBSTptr_t searchBST(BSTptr_t tree, time_t inputTime) {
 //function parameters
 nodeBSTptr_t currNode = tree->root;
 AHT20data_t currData = currNode->data;

printf("INFO (searchBST): Starting search for timestamp %lld\n", inputTime);
printf("INFO (searchBST): Visiting these nodes:\n");

 //search for node
 while (currData.timestamp != inputTime) {
  if (inputTime < currData.timestamp) {
   if (currNode->left == NULL) {
    printf("ERROR: [searchBST API] Node does not exist.\n");
    return NULL;
   }
   printf("-> [%lld] %s", currNode->data.timestamp, asctime(localtime(&currNode->data.timestamp)));
   currNode = currNode->left;
   currData = currNode->data;
  }

  if (inputTime > currData.timestamp) {
   if (currNode->right == NULL) {
    printf("ERROR: [searchBST API] Node does not exist.\n");
    return NULL;
   }
   printf("-> [%lld] %s", currNode->data.timestamp, asctime(localtime(&currNode->data.timestamp)));
   currNode = currNode->right;
   currData = currNode->data;
  }
 }

 if (inputTime == currData.timestamp) {
  printf("FOUND-> %s", asctime(localtime(&currNode->data.timestamp)));
  return currNode;
 }

 printf("ERROR: [searchBST API] Bug in code.\n");
 return NULL;
}
