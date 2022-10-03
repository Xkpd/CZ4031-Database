#include "memory_pool.h"

#include <algorithm>
#include <cstring>
#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <iostream>
#include <sstream>

using namespace std;

//Constructor 
MemoryPool::MemoryPool(unsigned int memorySize, unsigned int blockSize) {
  this->memorySize = memorySize;
  this->blockSize = blockSize;
  this->memoryPtr = new unsigned char[memorySize];
  this->blockPtr = nullptr;
  this->blockOffset = 0;
  this->numBlocksAssigned = 0;
  this->numBlocksAvailable = memorySize / blockSize;
  this->numRecordsAssigned = 0;
}

//Destructor 
MemoryPool::~MemoryPool() {
  delete memoryPtr;
  memoryPtr = nullptr;
}

bool MemoryPool::checkBlockSpace(){
  if (numBlocksAvailable > 0){
    return true; //Return true when there is still block space available
  }

  else { 
    cout << "There is not enough memory left in the disk for a new block! \n";
    return false;
  }
}

bool MemoryPool::assignBlock() {
  if (checkBlockSpace()){
    if (numBlocksAssigned == 0){
      blockPtr = memoryPtr;
    }
    else {
      blockPtr = blockPtr + blockSize;
    }
    numBlocksAvailable -= 1;
    numBlocksAssigned += 1;
    blockOffset = 0; 
    return true;
  }

  else { 
    return false; //Return false when there is no block available
  }

}

bool MemoryPool::checkRecordSpace(unsigned int recordSize){
  if (numBlocksAssigned == 0 || blockSize < (blockOffset + recordSize)) {
    if (!assignBlock()){//When assignment fails
      throw "There is no free space in the blocks, and assignment of blocks is not possible! \n";
      return false;
    }
    else if (recordSize > blockSize){
      throw "Record size must be smaller than block size! \n";
      return false;
    }
  }
  return true;
}

tuple<void *, unsigned int> MemoryPool::writeRecord(unsigned int recordSize) {
  if (checkRecordSpace(recordSize)){
    tuple<void *, unsigned int> rec(blockPtr, blockOffset);
    blockOffset += recordSize;
    numRecordsAssigned += 1;
    return rec;
  }

  else
  {
    throw "Fail!";
  }
}

bool MemoryPool::deleteRecord(unsigned char *blockAddress, unsigned int offset, const unsigned int recordSize) {
  try {
    fill(blockAddress + offset, blockAddress + offset + recordSize, '\0');
    if (offset == 0) {
      numBlocksAssigned -= 1;
      numBlocksAvailable += 1;
    }

    numRecordsAssigned -= 1;
    return true;
  }

  catch (exception &e) {
    cout << "Removal failed! \n";
    return false;
  }
}