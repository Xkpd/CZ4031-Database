#include "b_plus_tree.h"

#include <bits/stdc++.h>

using namespace std;

void BPTree::insert(keys_struct x)
{
  if (root == nullptr)
  {
    root = new Node;
    root->keys[0] = x;
    root->isLeaf = true;
    root->size = 1;
    return;
  }
  
  else 
  {
    Node *currentNode = root;
    Node *parent;

    while (!currentNode->isLeaf)
    {
      parent = currentNode;
      if (x.key_value >= currentNode->keys[currentNode->size - 1].key_value){
        currentNode = currentNode->pointers[currentNode->size];
      }

      else{
        for(int i = 0;i<currentNode->size;i++){
            if(currentNode->keys[i].key_value >= x.key_value){
              currentNode = currentNode->pointers[i];
              break;
            }
          }
          
      }
    }

    for (int i = 0; i < currentNode->size; i++)
    {
      if (currentNode->keys[i].key_value == x.key_value)
      {
        currentNode->keys[i].add_vect.push_back(x.add_vect[0]);
        return;
      }
    }

    if (currentNode->size < MAX)
    {
      int i = 0;
      while (currentNode->keys[i].key_value < x.key_value){
        i++;
        if (i == currentNode->size) break;
      }
      
      if(i == currentNode->size){
        currentNode->keys[i] = x;
        currentNode->size++;
      }

      else{
        for (int k = currentNode->size; k > i; k--){
          currentNode->keys[k] = currentNode->keys[k - 1];
        }

        currentNode->keys[i] = x;
        currentNode->size++;
      }

      currentNode->pointers[currentNode->size] = currentNode->pointers[currentNode->size - 1];
      currentNode->pointers[currentNode->size - 1] = nullptr;
      return;
    }

    else
    {
      Node *neighborNode = new Node;
      neighborNode->isLeaf = true;
      
      keys_struct tempKeys[MAX + 1];
      for (int i = 0; i < MAX; i++){
        tempKeys[i] = currentNode->keys[i];
      }

      if (tempKeys[MAX - 1].key_value < x.key_value){
        tempKeys[MAX] = x;
      }
      
      else
      {
        int i = 0;
        while (tempKeys[i].key_value <= x.key_value){
          i++;
        }

        for (int k = MAX; k > i; k--){
          tempKeys[k] = tempKeys[k - 1];
        }

        tempKeys[i] = x;
      }

      currentNode->size = (MAX + 1) / 2;
      neighborNode->size = MAX + 1 - (MAX + 1) / 2;

      int currentNodeNeighbor = MAX;
      neighborNode->pointers[neighborNode->size] = currentNode->pointers[currentNodeNeighbor];
      currentNode->pointers[currentNodeNeighbor] = nullptr;
      currentNodeNeighbor = currentNode->size;
      currentNode->pointers[currentNodeNeighbor] = neighborNode;

      int i;
      for (i = 0; i < currentNode->size; i++){
        currentNode->keys[i] = tempKeys[i];
      }

      for (int j = 0; j < neighborNode->size; j++) {
        neighborNode->keys[j] = tempKeys[i];
        i++;
      }

      if (root == currentNode)
      {
        Node *newRoot = new Node;
        newRoot->pointers[0] = currentNode;
        newRoot->pointers[1] = neighborNode;
        newRoot->keys[0] = neighborNode->keys[0];
        newRoot->size = 1;
        newRoot->isLeaf = false;
        root = newRoot;
      }

      else insertInternal(neighborNode->keys[0], parent, neighborNode);
    }
  }
}

void BPTree::insertInternal(keys_struct x, Node *currentNode, Node *child)
{
  if (currentNode->size < MAX)
  {
    int i = 0;
    while (currentNode->keys[i].key_value <= x.key_value){
      i++;
      if(i == currentNode->size){
        break;
      }
    }
    
    if(i == currentNode->size)
    {
      currentNode->keys[i] = x;
      currentNode->pointers[i + 1] = child;
      currentNode->size++;
    }

    else{
      for (int k = currentNode->size; k > i; k--){
        currentNode->keys[k] = currentNode->keys[k - 1];
        currentNode->pointers[k + 1] = currentNode->pointers[k];
      }

      currentNode->keys[i] = x;
      currentNode->pointers[i + 1] = child;
      currentNode->size++;
    }

    return;
  }

  else
  {
    Node *newInternal = new Node;
    newInternal->isLeaf = false;
    keys_struct tempKeys[MAX + 1];
    Node *tempPointers[MAX + 2];

    for (int i = 0; i < MAX; i++)
    {
        tempKeys[i] = currentNode->keys[i];
        tempPointers[i] = currentNode->pointers[i];
    }

    tempPointers[MAX] = currentNode->pointers[MAX];

    int j;
    int i = 0;

    while (i < MAX && x.key_value > tempKeys[i].key_value){
      i++;
    }

    for (j = MAX; j > i; j--)
    {
        tempKeys[j] = tempKeys[j - 1];
        tempPointers[j + 1] = tempPointers[j];
    }

    tempKeys[i] = x;
    tempPointers[i + 1] = child;

    int originalSize = currentNode->size;
    currentNode->size = (MAX + 1) / 2;
    newInternal->size = MAX - (MAX + 1) / 2;

    for (i = 0; i < currentNode->size; i++)
    {
      currentNode->keys[i] = tempKeys[i];
      currentNode->pointers[i] = tempPointers[i];
    }

    currentNode->pointers[currentNode->size] = tempPointers[i];
  
    keys_struct n;
    keys_struct *p = &n;
    p = NULL;

    for (i = currentNode->size;i<originalSize;i++)
    {
      currentNode->keys[i] = n;
      currentNode->pointers[i+1] = nullptr;
    }
    
    for (i = 0, j = currentNode->size + 1; i < newInternal->size; i++, j++)
    {
      newInternal->keys[i] = tempKeys[j];
      newInternal->pointers[i] = tempPointers[j];
    }

    newInternal->pointers[newInternal->size] = tempPointers[j];

    if (root == currentNode)
    {
      Node *newRoot = new Node;
      Node *copy = newInternal;
      while(!copy->isLeaf) 
      copy = copy->pointers[0];
      newRoot->keys[0] = copy->keys[0];
      newRoot->pointers[0] = currentNode;
      newRoot->pointers[1] = newInternal;
      newRoot->isLeaf = false;
      newRoot->size = 1;
      root = newRoot;
      cout << "ENTRY! \n";
    }

    else
    {
      Node *copy = newInternal;
      while(!copy->isLeaf) 
      copy = copy->pointers[0];
      insertInternal(copy->keys[0], findParent(root, currentNode), newInternal);
    }
  }
}