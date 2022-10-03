#ifndef MEMORY_POOL_H
#define MEMORY_POOL_H
#include <math.h>

#include <algorithm>
#include <cstring>
#include <iostream>
#include <tuple>
#include <vector>
using namespace std;

const int MAX = 5;

struct Record { //Records hold 3 data as specified in the project
  char tconst[10]; //tconst is a string
  float averageRating;  
  unsigned int numVotes;
};

class memoryPool {
private:
  unsigned char *memoryPtr;
  unsigned char *blockPtr;
  unsigned int blockTailAddress;

  unsigned int blockSize;
  unsigned int memorySize;
  unsigned int sizeOfAssignedBlocks;
  unsigned int sizeOfAssignedRecords;

  int numBlocksAssigned;
  int numBlocksAvailable;
  int numRecordsAssigned;

public:
  memoryPool(unsigned int memorySize, unsigned int blockSize);

  //~memoryPool();

  bool checkSpaceAvail();

  bool assignBlock();

  bool checkRecordSpace(unsigned int recordSize);

  tuple<void *, unsigned int> insertRecord(unsigned int recordSize);

  bool deleteRecord(unsigned char *blockAddress, unsigned int offset, unsigned int recordSize);

  unsigned int getBlockSize() { return blockSize; }

  unsigned int getmemorySize() { return memorySize; }

  int getBlocksAssigned() { return numBlocksAssigned; }

  int getBlocksAvailable() { return numBlocksAvailable; }

  int getNumberOfRecords() { return numRecordsAssigned; }

  int getMax() { return MAX; }

  unsigned char *getmemoryPtr() { return memoryPtr; }
};

#endif