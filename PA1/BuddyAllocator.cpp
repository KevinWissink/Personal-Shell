#include "BuddyAllocator.h"
#include <iostream>
using namespace std;

BuddyAllocator::BuddyAllocator (int _basic_block_size, int _total_memory_length){
  basic_block_size = _basic_block_size, total_memory_size = _total_memory_length;

  //make the first memory block
  void* mem_start = (char*) malloc(total_memory_size);
  BlockHeader *Block = (BlockHeader *)mem_start;

  Block->block_size = total_memory_size;
  Block->free = true;
  Block->next = nullptr;
  
  //Initialize the vector of freelist 
  double size =  log2(total_memory_size/basic_block_size) + 1;
  
  FreeList.resize(size);

  //Put the block of memory into the last spot or the vector
  FreeList[size -1].insert(Block);
}

BuddyAllocator::~BuddyAllocator (){

  //need to go through the linked list and delete all the heads
  for(int i = 0; i < FreeList.size(); i++)
  {
    while(FreeList[i].getHead() != nullptr)
    {
      FreeList[i].remove();
    }
  }
  delete mem_start;
}

void* BuddyAllocator::alloc(int length) {

  //need to make sure there is enough room for the memory and block header
  int total_Length = pow(2, ceil(log2(length + sizeof(BlockHeader))));

  //also if the new length is too small for the basic block size set it equal to the basic block size
  if (total_Length < basic_block_size)
  {
    total_Length = basic_block_size;
  }

  //now i need to find which part of the vector to check in
  int point = log2(total_Length / basic_block_size);

  //I need to start at that index and go up from there till we 
  //find the correct size to split
  BlockHeader *address = nullptr;
  int Data_Point;
  for(int i = point; i < FreeList.size(); i++)
  {
      if (FreeList[i].getHead() != nullptr)
      {
          address = FreeList[i].getHead();
          Data_Point = i;
          break;
      }
  }
  
  //Next I need to make sure we have an address
  if (address == nullptr){
      return nullptr;
  }
  //otherwise i need to see if i need to split or just send the value
  else if (address->block_size > total_Length)
  {
      //need to split
      point = log2(address->block_size / total_Length);
      for(int i = 0; i < point; i++)
      {
          address = split(address);
      }
  }
  else
  {
    //the right size is already in the list
    FreeList[Data_Point].remove(address);
    address->free = false;
    address->next = nullptr;
  }

  return (char*)address + sizeof(BlockHeader);
  
}

void BuddyAllocator::free(void* a) {
    
}

void BuddyAllocator::printlist (){
  cout << "Printing the Freelist in the format \"[index] (block size) : # of blocks\"" << endl;
  int64_t total_free_memory = 0;
  for (int i=0; i<FreeList.size(); i++){
    int blocksize = ((1<<i) * basic_block_size); // all blocks at this level are this size
    cout << "[" << i <<"] (" << blocksize << ") : ";  // block size at index should always be 2^i * bbs
    int count = 0;
    BlockHeader* b = FreeList [i].head;
    // go through the list from head to tail and count
    while (b){
      total_free_memory += blocksize;
      count ++;
      // block size at index should always be 2^i * bbs
      // checking to make sure that the block is not out of place
      if (b->block_size != blocksize){
        cerr << "ERROR:: Block is in a wrong list" << endl;
        exit (-1);
      }
      b = b->next;
    }
    cout << count << endl;
    cout << "Amount of available free memory: " << total_free_memory << " bytes" << endl;  
  }
}

