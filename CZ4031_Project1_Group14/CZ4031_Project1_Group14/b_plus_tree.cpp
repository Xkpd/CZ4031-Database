#include "b_plus_tree.h"

#include <bits/stdc++.h>
#include <queue>

using namespace std;

extern void *startAddress;
extern unsigned int blockSize;

Node::Node()
{
  keys = new keys_struct[MAX]; 
  pointers = new Node *[MAX + 1]; 
  isLeaf = true;
}

void BPTree::findSingle(float key)
{
  int numIndexNodes = 0;

  //Iterate till the leaf node is reached
  Node *currentNode = root;
  while (!currentNode->isLeaf)
  {
    if(numIndexNodes < 5){
      cout << "The keys of the index node at level: " << numIndexNodes+1 << endl;
      for (int k = 0; k < currentNode->size; k++){
        cout << currentNode->keys[k].key_value << "|";
      }
    }
    cout << endl;

    numIndexNodes++;

    if(key >= currentNode->keys[currentNode->size - 1].key_value){
      currentNode = currentNode->pointers[currentNode->size];
    }

    else{
      for(int i = 0;i<currentNode->size;i++){
        if(currentNode->keys[i].key_value >= key){
          currentNode = currentNode->pointers[i];
          break;
        }
      }
    }

  }

  cout << "Number of index nodes the process accesses: " << numIndexNodes << endl;
  
  int i = 0;

  while (currentNode->keys[i].key_value < key){
    i++;
  }

  float exp_3_rating = 0;
  set<unsigned int> s;
  if (currentNode->keys[i].key_value == key)
  {
    int exp_3_records = currentNode->keys[i].add_vect.size();
      
    cout << "\ntconst    | Memory Address | numVotes" << endl;
    for (int j = 0; j < exp_3_records; j++)
    {
      //Locate the exact address of that record and find the exp_3_rating
      exp_3_rating += (*(Record *)currentNode->keys[i].add_vect[j]).averageRating;
      uintptr_t dataBlockNum = ((uintptr_t)(Record *)currentNode->keys[i].add_vect[j] - (uintptr_t)startAddress) / blockSize;
      s.insert(dataBlockNum);          
      cout << (*(Record *)currentNode->keys[i].add_vect[j]).tconst << " | " << (Record *)currentNode->keys[i].add_vect[j]
            << " | " << (*(Record *)currentNode->keys[i].add_vect[j]).numVotes << endl;
    }
    
    cout << "\nNumber of data blocks the process accesses: " << s.size() << endl;
    cout << "The average rating of the records returned: " << exp_3_rating / exp_3_records << endl;
  }

  else
  {
      cout << "Does not exist!" << endl;
      return;
  }

}


void BPTree::findRange(float lowerBound, float upperBound)
{
  int numIndexNodes = 0;
  Node *currentNode = root;
  
  while (!currentNode->isLeaf)
  {
    if(numIndexNodes < 5){
      cout << "The keys of the index node at level: " << numIndexNodes+1 << endl;
      for (int k = 0; k < currentNode->size; k++){
        cout << currentNode->keys[k].key_value << "|";
      }
    }

    cout << endl;
    numIndexNodes++;

    if (lowerBound >= currentNode->keys[currentNode->size - 1].key_value){
      currentNode = currentNode->pointers[currentNode->size];
    }

    else{
      int i = 0;
      for(int i = 0;i<currentNode->size;i++){
        if(currentNode->keys[i].key_value >= lowerBound){
          currentNode = currentNode->pointers[i];
          break;
        }
      }
    }
  }
  cout << "Number of index nodes the process accesses: " << numIndexNodes << endl;

  int i = 0;
  while (currentNode->keys[i].key_value < lowerBound){
    i++;
  }

  float exp_4_rating = 0;
  int exp_4_record = 0;
  set<unsigned int> s;

  cout << "\ntconst    | Memory Address | numVotes" << endl;
  while (currentNode->keys[i].key_value <= upperBound)
  {
    for (int j = 0; j < currentNode->keys[i].add_vect.size(); j++)
    {
      exp_4_rating += (*(Record *)currentNode->keys[i].add_vect[j]).averageRating;
      uintptr_t dataBlockNum = ((uintptr_t)(Record *)currentNode->keys[i].add_vect[j] - (uintptr_t)startAddress) /blockSize;
      s.insert(dataBlockNum);
      cout << (*(Record *)currentNode->keys[i].add_vect[j]).tconst << " | " << (Record *)currentNode->keys[i].add_vect[j]
            << " | " << (*(Record *)currentNode->keys[i].add_vect[j]).numVotes << endl;
    }

    exp_4_record += currentNode->keys[i].add_vect.size();
    i++;
    
    if (currentNode->pointers[currentNode->size] != nullptr && i == currentNode->size)
    {
      i = 0;
      currentNode = currentNode->pointers[currentNode->size];
      continue;
    }
  }

  cout << "\nNumber of data blocks the process accesses: " << s.size() << endl;
  cout << "The average rating of the records returned: " << exp_4_rating / exp_4_record << endl;
}

int BPTree::getHeight(Node *currentNode)
{
  // travel recursively until we reach the bottom level
  if (currentNode->isLeaf){
    return 1;
  }
  return getHeight(currentNode->pointers[0]) + 1;
}

void BPTree::display(Node *currentNode)
{
  cout << "Content of root node: ";
  for (int i = 0; i < currentNode->size; i++)
  {
    cout << currentNode->keys[i].key_value << "|";
  }
  cout << endl;

  cout << "Content of 1st child node: ";
  currentNode = currentNode->pointers[0];
  for (int i = 0; i < currentNode->size; i++)
  {
    cout << currentNode->keys[i].key_value << "|";
  }
  cout << endl;
}

int BPTree::calculateNumNodes(Node *currentNode)
{
  if (currentNode == NULL){
    return 0;
  }

  //Use queue for a search algorithm
  queue<Node *> q;
  q.push(currentNode);

  int numNodes = 0;

  while (!q.empty())
  {
    int queueSize = q.size();
    numNodes += queueSize;

    for (int i = 0; i < queueSize; i++)
    {
      Node *node = q.front();

      for (int i = 0; i < node->size; i++){
        if (!node->isLeaf){
          q.push(node->pointers[i]);
        }
      }

      if (!node->isLeaf){
        q.push(node->pointers[node->size]);
      }
      q.pop();
    }
  }
  
  return numNodes;
}

int BPTree::getMax() { return MAX; }

Node *BPTree::getRoot() { return root; }

BPTree::BPTree() : root{NULL} {}