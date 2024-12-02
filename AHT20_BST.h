/**
* @file AHT20_BST.h
 * @brief
 * This is header file for a binary search tree ADT targeted to the
 * Temp/Humidity sensor readings from the AHT20 used in the io module 361 r2
 *
 * @version:	1.0.0
 * @author:		Keenan Mondragon (kmondra2@pdx.edu)
 * @date: 		30-Nov-2023
 *
 */

#ifndef AHT20_BST_H
#define AHT20_BST_H

#include <stdbool.h>
#include <stdint.h>
#include <time.h>

// define data intem struct
typedef struct _AHT20data_s {
        time_t timestamp;
        uint32_t temp;
        uint32_t humidity;
} AHT20data_t, *AHT20dataPtr_t;

// define BST node for AHT20 BST
typedef struct _nodeBST_s {
    AHT20data_t            data;
    struct _nodeBST_s     *left;
    struct _nodeBST_s     *right;
} nodeBST_t, *nodeBSTptr_t;

// define AHT20 binary search tree
typedef struct _AHT20_BST_s {
    nodeBSTptr_t    root;
    int             size;
} BST_t, *BSTptr_t;

/***** API FUNCTIONS *****/

/**
 * createBST() - creates a Temperature/Humidity tree for AHT20 readings
 *
 * @return	a pointer to the new Temp/Humidity tree if succeeds.  Null if it fails
 * root node will start as NULL because the tree starts with 0 nodes
 *
 */
BSTptr_t createBST(void);

/**
 * insertBSTnode() - inserts a temp/humid data record into the tree
 *
 * @param	tree	pointer to the TempHumidtree to add the node to
 * @param	info	Temp/Humid data node to add to tree
 * @return			pointer to the new BST node

 * @note Not a good idea to expose the data node but w/o a pointer to
 * root I don't see much harm and it could be useful for debug
 */
nodeBSTptr_t insertBSTnode(BSTptr_t tree, AHT20data_t readings);

/**
 * searchBST() - searches for a temp/humid data record in the tree w/ the specified timestamp
 *
 * @param	tree		pointer to the TempHumidtree to search
 * @param	timestamp	timestamp of the Temp/Humid data node we are seeking
 * @return				pointer to the BST node with that timestamp or NULL if not found

 * @note Not a good idea to expose the data node but w/o a pointer to
 * root I don't see much harm and it could be useful for debug
 */
nodeBSTptr_t searchBST(BSTptr_t tree, time_t timestamp);

/**
 * inorderDiplay() - performs inorder traversal of tree
 *
 * @param	BST_root		pointer to BST root node
 *
 * @brief
 * Performs an inorder traversal of the BST.  The data in the nodes are
 * displayed using the helper (non-API) function _displayDataNode(). This function
 * is a wrapper for _inorder() (non-AIPI) which does the work.
 */
void inorderDisplay(nodeBSTptr_t BST_root);


/**
 * @function displayNode()
 *
 * @param	node		pointer to the node of interest
 *
 * @brief
 * Displays the node data to user
 */
void displayNode(nodeBSTptr_t node);

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
BSTptr_t populateBST(time_t startTime, int numEntries);
#endif //AHT20_BST_H
