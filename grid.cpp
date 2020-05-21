#include "grid.h"
#include "grid_given.cpp"

// PA1 functions


/**
 * Destroys the current Grid. This function should ensure that
 * memory does not leak on destruction of a grid.
 */
Grid::~Grid(){ 
    clear();
}
    /*
    
}*/

/**
 * Rotate row r (row 0 is the first) by count positions.
 * If row r is ABCDE (left to right) in grid g then row r
 * in grid g should be DEABC after the call g.rotateR(r, 2).
 * Rotate headOfCol_ if necessary.
 */
void Grid::rotateR(int r, int count) { 
    if(headOfRow_.size() > 0 && headOfCol_.size() > 0){
        r = r%headOfRow_.size();
        count = count%headOfCol_.size();
        for (int i = 0; i < count; i++) {
        rotateRowOnce(r);
    }
    }
}

void Grid::rotateRowOnce(int r) {
    Node * curr = headOfRow_[r];
    Node * initialCurr = headOfRow_[r];
    for (int i = 0; i < (int) headOfCol_.size(); i++) { // change every node
        if (r == 0) { // headOfCol only needs to be changed if its the 1st row
            headOfCol_[i] = curr->left;
        }
        curr->down->up = curr->left;   // change the pointer below curr points to
        curr->up->down = curr->left;   // change the pointer above curr points to

        if (i < (int) headOfCol_.size() - 1) {
            curr->down = curr->right->down; // change the pointers curr points to
            curr->up = curr->right->up;
        } else {
            curr->down = curr->right->down->left; // last pointer needs to take in account that the
            curr->up = curr->right->up->left;     // pointer above it has already been changed
        }
        curr = curr->right; // change to next node
    }
    headOfRow_[r] = initialCurr->left; // change heaOfRow's pointer
}

/**
 * Rotate column c (column 0 is the first) by count positions.
 * If column c is ABCDE (top to bottom) in grid g then column c
 * in grid g should be DEABC after the call g.rotateC(c, 2). 
 * Rotate headOfRow_ if necessary.
 */
void Grid::rotateC(int c, int count) { /* your code here */
   if(headOfCol_.size()>0 && headOfRow_.size() > 0){
        c = c%headOfCol_.size();
        count = count%headOfRow_.size();
   for (int i = 0; i < count; i++) {
        rotateColumnOnce(c);
    }
   }
}

void Grid::rotateColumnOnce(int c){
    Node * curr = headOfCol_[c];
    Node * initialCurr = headOfCol_[c];
    for (int i = 0; i < (int) headOfRow_.size(); i++) { // change every node
        curr->right->left = curr->up;   // change the pointer right of curr points to
        curr->left->right = curr->up;   // change the pointer left of curr points to

        if (i < (int) headOfRow_.size() - 1) {
            curr->right = curr->down->right;  // change the pointers curr points to
            curr->left = curr->down->left;
        } else {
            curr->right = curr->down->right->up; // last pointer needs to take in account that the
            curr->left = curr->down->left->up;   // pointer above it has already been changed
        }

        curr = curr->down; // change  to next node
    }
    headOfCol_[c] = initialCurr->up; // change heaOfRow's pointer
    if(c == 0){
        std::vector <Node *> headOfRow2;
        headOfRow2[0] = headOfRow_[headOfRow_.size()-1];
        for (unsigned long i = 1; i < headOfRow_.size(); i++){
            
        }  
    }
}

/**
 * Destroys all dynamically allocated memory associated with the
 * current Grid class. Clears headOfRow_ and headOfCol_ vectors.
 * Sets bwidth_, bheight_ to zero.
 * After clear() the grid represents an empty grid.
 */
void Grid::clear() { 
    for (unsigned long i = 0; i < headOfCol_.size(); i++){
        eraseColNodes (headOfCol_[i]); 
        headOfCol_[i] = NULL;
    }
    headOfCol_.clear();
    
    for (unsigned long i = 0; i < headOfRow_.size(); i++){
        eraseRowNodes (headOfRow_[i]);
        delete(headOfRow_[i]);
        headOfRow_[i] = NULL;
    }
    headOfRow_.clear();
    bwidth_ = 0;
    bheight_= 0;
}

//disconnects all nodes in a column, leaving a vector (headOfRow) of 
//1D circular doubly linked lists (all up and down pointers are set to NULL and thus the
//Nodes are not connected vertically)
void Grid::eraseColNodes(Node * colHead){
    Node* temp;
    Node * tempNext;
    if(colHead->down != colHead){
        temp = colHead -> up;
        tempNext = temp ->up;
        temp -> up = NULL;
        (temp -> down) ->up = tempNext;
        temp -> down = NULL;
        tempNext -> down = colHead;
        eraseColNodes(colHead);
    }
    colHead->down = NULL;
    colHead->up = NULL;
}
//disconnects all rows of Nodes by pointing left and right pointers to NULL and
//deletes all Nodes
void Grid::eraseRowNodes(Node * rowHead){
    Node* temp;
    Node* tempNext;
    if(rowHead -> right != rowHead){
        temp = rowHead -> left;
        tempNext = temp -> left;
        temp -> left = NULL;
        (temp -> right) ->left = tempNext;
        temp -> right = NULL;
        tempNext -> right = rowHead;
        delete temp;
        eraseRowNodes(rowHead);
    }
}


/**
 * Makes the current Grid a copy of the "other" Grid.
 * The dimensions should be the same.  The Nodes should
 * contain the same blocks, but the Nodes should be newly
 * allocated. This function is used in both the copy
 * constructor and the assignment operator for Grids.
 */

void Grid::copy(Grid const& other) { /*your code here*/
  clear();
  bwidth_ = other.bwidth_;		// width of all Node blocks
  bheight_ = other.bheight_;	// height of all Node blocks
  // Set up a temporary two-dimensional vector of ptrs to Nodes containing
  // the correct sub-blocks from "other".
  vector < vector < Node * > > A;
  Node* tracker;
  for( int j=0; j < other.numRows(); j++ ) {
    tracker = other.headOfRow_[j];
    vector < Node * > temp;
    for( int i=0; i < other.numCols(); i++ ) {
      Node* p = new Node(tracker->block);
      tracker = tracker -> right;
      temp.push_back(p);
    }
    A.push_back(temp);
  }
  // Set up the pointer structure between Nodes
  for( int j=0; j< other.numRows(); j++ ) {
    for( int i=0; i< other.numCols(); i++ ) {
      Node *p = A[j][i];
      p->up    = A[(j==0) ? other.numRows()-1 : j-1] [i]; 
      p->down  = A[(j==other.numRows()-1) ? 0 : j+1][i];
      p->left  = A[j] [(i==0) ? other.numCols()-1 : i-1];
      p->right = A[j] [(i==other.numCols()-1) ? 0 : i+1];
    }
  }
  for( int j=0; j<other.numRows(); j++ ) {
    headOfRow_.push_back(A[j][0]);
  }
  for( int i=0; i<other.numCols(); i++ ) {
    headOfCol_.push_back(A[0][i]);
  }
}  