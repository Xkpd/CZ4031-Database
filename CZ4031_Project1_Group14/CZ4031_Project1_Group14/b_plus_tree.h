#ifndef B_PLUS_TREE_H
#define B_PLUS_TREE_H

#include "memory_pool.h"

#include <climits>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <unordered_map>

using namespace std;

struct keys_struct
{
  float key_value; // as per project specification, key value is based on numVotes
  vector<void *> add_vect;
};

class BPTree;

class Node
{
  bool isLeaf; 
  keys_struct *keys;
  int size;
  Node **pointers;   //pointers to a child node
  friend class BPTree;

public:
  Node();
  
};

class BPTree
{
  Node *root;

  void insertInternal(keys_struct x, Node *cursor, Node *child);

  int deleteInternal(keys_struct x, Node *cursor, Node *child);

  Node *findParent(Node *cursor, Node *child);

  void updateParent(Node *child, keys_struct key);

  int numNode = 0;

public:
  BPTree();

  void findSingle(float key);

  void findRange(float lowerKeyBound, float upperKeyBound);

  int getHeight(Node *cursor);

  void insert(keys_struct x);

  int remove(keys_struct x);

  Node *getRoot();

  int getMax();

  int getNumNodes() { return numNode; }

  int calculateNumNodes(Node *cursor);

  void display(Node *cursor);
};

#endif