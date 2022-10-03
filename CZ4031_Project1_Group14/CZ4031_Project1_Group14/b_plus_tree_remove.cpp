#include "b_plus_tree.h"

#include <bits/stdc++.h>

using namespace std;

int BPTree::remove(keys_struct x)
{
  if (root == nullptr)
  {
    throw std::logic_error("B+ tree is empty! \n");
  }

  else
  {
    Node *parentNode;
    int left_sib_ptr = -999999;
    int right_sib_ptr = -999999;
    Node *currentNode = root;

    while (!(currentNode->isLeaf))
    {
      int i;
      parentNode = currentNode;
      if (currentNode->keys[currentNode->size - 1].key_value <= x.key_value)
      {
        i = currentNode->size;
      }

      else
      {
        i = 0;
        while (currentNode->keys[i].key_value <= x.key_value)
        {
          i+=1;
        }
      }

      currentNode = currentNode->pointers[i];
      right_sib_ptr = 1 + i;
      left_sib_ptr = 1 - i;
    }

    int key_pos = 0;
    while (x.key_value > currentNode->keys[key_pos].key_value){
      key_pos = key_pos + 1;
    }

    if (currentNode->keys[key_pos].key_value != x.key_value)
    {
      cout << "Key does not exist! \n";
      return 0;
    }

    int i = key_pos;

    for (int i = key_pos; i <= currentNode->size -1 ; i++)
    {
      currentNode->keys[i] = currentNode->keys[i + 1];
    }

    currentNode->size--;

    if (currentNode == root) 
    {
      cout << "DELETED! \n";
      
      for (int i = 0; i <= MAX; i++)
      {
        currentNode->pointers[i] = nullptr;
      }
      
      if (!currentNode->size) 
      {
        cout << "Entire B+ tree has been deleted! \n";
        delete[] currentNode->keys;
        delete[] currentNode->pointers;
        delete currentNode;
        root = nullptr;
        return 1;
      }

      return 0;
    }

    currentNode->pointers[currentNode->size] = currentNode->pointers[currentNode->size + 1];
    currentNode->pointers[currentNode->size + 1] = nullptr;
    cout << "DELETED! \n";

    if (!key_pos)//If key_pos in leaf node == 0, update
    {
      cout << "Parent node updated! \n";
      updateParent(currentNode, currentNode->keys[0]);
    }

    if (currentNode->size >= (MAX + 1) / 2)
    {
      cout << "There are sufficient nodes! \n";
      return 0;
    }

    cout << "UNDERFLOW! \n";

    if (left_sib_ptr >= 0)
    {
      cout << "Transfer from left sibling! \n";
      Node *left_node = parentNode->pointers[left_sib_ptr];

      if (left_node->size >= (MAX + 1) / 2 + 1)
      {
        for (int i = currentNode->size; i >= 1; i--)
        {
          currentNode->keys[i] = currentNode->keys[i - 1];
        }

        int currentNodeSize = currentNode->size;
        currentNode->size++;
        currentNode->pointers[currentNodeSize] = currentNode->pointers[currentNodeSize - 1];
        currentNode->pointers[currentNodeSize - 1] = nullptr;
        currentNode->keys[0] = left_node->keys[left_node->size - 1];

        left_node->size--;
        left_node->pointers[left_node->size] = currentNode;
        left_node->pointers[left_node->size + 1] = nullptr;

        parentNode->keys[left_sib_ptr] = currentNode->keys[0];
        return 0;
      }
    }

    if (right_sib_ptr <= parentNode->size)//Check right sibling
    {
      cout << "Transfer from right sibling! \n";
      Node *right_node = parentNode->pointers[right_sib_ptr];

      if (right_node->size >= (MAX + 1) / 2 + 1)
      {
        int currentNodeSize = currentNode->size;
        currentNode->size++;
        currentNode->pointers[currentNodeSize] = currentNode->pointers[currentNodeSize - 1];
        currentNode->pointers[currentNodeSize - 1] = nullptr;
        currentNode->keys[currentNodeSize - 1] = right_node->keys[0];

        right_node->size--;
        right_node->pointers[right_node->size] = right_node->pointers[right_node->size + 1];
        right_node->pointers[right_node->size + 1] = nullptr;

        for (int i = 0; i <= right_node->size -1; i++)
        {
          right_node->keys[i] = right_node->keys[i + 1];
        }

        parentNode->keys[right_sib_ptr - 1] = right_node->keys[0];
        return 0;
      }
    }

    if (left_sib_ptr >= 0)//Merge if left sibling exists
    {
      cout << "Merging with left sibling! \n";
      Node *left_node = parentNode->pointers[left_sib_ptr];

      for (int i = left_node->size, j = 0; j < currentNode->size; i++, j++)
      {
        left_node->keys[i] = currentNode->keys[j];
      }

      left_node->pointers[left_node->size] = nullptr;
      left_node->size += currentNode->size;
      left_node->pointers[left_node->size] = currentNode->pointers[currentNode->size];

      int num_nodes_merged = deleteInternal(parentNode->keys[left_sib_ptr], parentNode, currentNode);

      delete[] currentNode->keys;
      delete[] currentNode->pointers;
      delete currentNode;
      cout << "Deleted 1 in left sibling! \n";
      return 1 + num_nodes_merged;
    }

    else if (right_sib_ptr <= parentNode->size) //If no left sibling, merge with right sibling
    {
      cout << "Merging with right sibling! \n";
      Node *right_node = parentNode->pointers[right_sib_ptr];

      for (int i = currentNode->size, j = 0; j < right_node->size; i++, j++)
      {
        currentNode->keys[i] = right_node->keys[j];
      }

      int currentNodeSize = currentNode->size;

      currentNode->pointers[currentNodeSize] = nullptr; 
      currentNode->size += right_node->size;
      currentNodeSize = currentNode->size;
      currentNode->pointers[currentNodeSize] = right_node->pointers[right_node->size];

      int num_nodes_merged = deleteInternal(parentNode->keys[right_sib_ptr - 1], parentNode, right_node);
      delete[] right_node->keys;
      delete[] right_node->pointers;
      delete right_node;
      cout << "Deleted 1 in right sibling! \n";
      return num_nodes_merged + 1;
    }
  }

  return 0;
}

int BPTree::deleteInternal(keys_struct x, Node *currentNode, Node *child)
{
  cout << "Delete internal node: " << x.key_value << endl;

  if (currentNode == root)//If root key is being deleted
  {
    if (currentNode->size == 1)
    {
      if (currentNode->pointers[1] == child || currentNode->pointers[0] == child)
      {
        root = currentNode->pointers[0] == child ? currentNode->pointers[1] : currentNode->pointers[0];
      
        delete[] child->keys;
        delete[] child->pointers;
        delete child;
        delete[] currentNode->keys;
        delete[] currentNode->pointers;
        delete currentNode;
        
        return 0;
      }
    }
  }

  int posKey = 0;
  while (posKey < currentNode->size && x.key_value != currentNode->keys[posKey].key_value){
    posKey++;
  }

  for (int i = posKey; i <= currentNode->size-2; i++) 
  {
    currentNode->keys[i] = currentNode->keys[i + 1];
  }

  int ptr_pos = 0;
  while (ptr_pos < currentNode->size + 1 && currentNode->pointers[ptr_pos] != child){
    ptr_pos++;
  }

  for (int i = ptr_pos; i < currentNode->size-1; i++) 
  {
    currentNode->pointers[i] = currentNode->pointers[i + 1];
  }

  currentNode->size--;

  if (currentNode->size >= (MAX + 1) / 2 - 1 || currentNode == root)
  {
    cout << "Deleted internal node! \n";
    return 0;
  }

  Node *parentNode = findParent(root, currentNode);
  int left_sib_ptr, right_sib_ptr;

  int ptr_pos_par = 0;
  while (ptr_pos_par < parentNode->size + 1 && parentNode->pointers[ptr_pos_par] != currentNode){
    ptr_pos_par++;
  }

  left_sib_ptr = ptr_pos_par - 1;
  right_sib_ptr = ptr_pos_par + 1;

  if (left_sib_ptr >= 0)
  {
    cout << "Transfer from left sibling! \n";
    Node *left_node = parentNode->pointers[left_sib_ptr];

    if (left_node->size >= (MAX + 1) / 2)
    {
      for (int i = currentNode->size; i > 0; i--)
      {
        currentNode->keys[i] = currentNode->keys[i - 1];
      }

      for (int i = currentNode->size + 1; i > 0; i--)
      {
        currentNode->pointers[i] = currentNode->pointers[i - 1];
      }

      currentNode->keys[0] = parentNode->keys[left_sib_ptr];
      parentNode->keys[left_sib_ptr] = left_node->keys[left_node->size - 1];
      currentNode->pointers[0] = left_node->pointers[left_node->size];
      currentNode->size++;

      left_node->size--;

      cout << "Transferred " << currentNode->keys[0].key_value << "from left sibling! \n";
      return 0;
    }
  }

  if (right_sib_ptr <= parentNode->size)
  {
    cout << "Transfer from right sibling! \n";
    Node *right_node = parentNode->pointers[right_sib_ptr];

    if (right_node->size >= (MAX + 1) / 2)
    {
      currentNode->keys[currentNode->size] = parentNode->keys[ptr_pos_par];
      parentNode->keys[ptr_pos_par] = right_node->keys[0];

      for (int i = 0; i < right_node->size - 1; i++)
      {
        right_node->keys[i] = right_node->keys[i + 1];
      }

      currentNode->pointers[currentNode->size + 1] = right_node->pointers[0];

      for (int i = 0; i < right_node->size; i++)
      {
        right_node->pointers[i] = right_node->pointers[i + 1];
      }

      currentNode->size++;
      right_node->size--;

      cout << "Transferred " << currentNode->keys[0].key_value << " from right sibling! \n";
      return 0;
    }
  }

  if (left_sib_ptr >= 0)
  {
    cout << "Merged with left sibling! \n";
    Node *left_node = parentNode->pointers[left_sib_ptr];
    left_node->keys[left_node->size] = parentNode->keys[left_sib_ptr];

    for (int i = left_node->size + 1, j = 0; j < currentNode->size; j++)
    {
      left_node->keys[i] = currentNode->keys[j];
    }

    for (int i = left_node->size + 1, j = 0; j <= currentNode->size; j++)
    {
      left_node->pointers[i] = currentNode->pointers[j];
      currentNode->pointers[j] = nullptr;
    }

    left_node->size += currentNode->size + 1;
    currentNode->size = 0;

    int num_nodes_merged = deleteInternal(parentNode->keys[left_sib_ptr], parentNode, currentNode);
    return 1 + num_nodes_merged;
  }

  else if (right_sib_ptr <= parentNode->size)
  {
    cout << "Merged with right sibling! \n";
    Node *right_node = parentNode->pointers[right_sib_ptr];
    currentNode->keys[currentNode->size] = parentNode->keys[right_sib_ptr - 1];

    for (int i = currentNode->size + 1, j = 0; j < right_node->size; j++)
    {
      currentNode->keys[i] = right_node->keys[j];
    }

    for (int i = currentNode->size + 1, j = 0; j <= right_node->size; j++)
    {
      currentNode->pointers[i] = right_node->pointers[j];
      right_node->pointers[j] = NULL;
    }

    currentNode->size += right_node->size + 1;
    right_node->size = 0;

    int num_nodes_merged = deleteInternal(parentNode->keys[right_sib_ptr - 1], parentNode, right_node);
    return 1 + num_nodes_merged;
  }
  
  return 0;
}

Node *BPTree::findParent(Node *currentNode, Node *child)
{
  Node *parentNode;
  if (currentNode->isLeaf || (currentNode->pointers[0])->isLeaf){
    return NULL;
  }

  for (int i = 0; i < currentNode->size + 1; i++)
  {
    if (currentNode->pointers[i] == child)
    {
      parentNode = currentNode;
      return parentNode;
    }

    else
    {
      parentNode = findParent(currentNode->pointers[i], child);
      if (parentNode != NULL){
        return parentNode;
      }
    }
  }

  return NULL;
}

void BPTree::updateParent(Node *child, keys_struct key)
{
  Node *parentNode = findParent(root, child);
  
  int i = 0;
  while (i <= parentNode->size && parentNode->pointers[i] != child){
      i++;
  }

  if (i == 0)
  {
    if (parentNode == root){
      updateParent(parentNode, key);
    }

    else{
      parentNode->keys[i - 1] = key;
      return; 
    }
  }

  else
  {
    parentNode->keys[i - 1] = key;
    return;
  }
}