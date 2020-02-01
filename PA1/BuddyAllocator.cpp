#include "BuddyAllocator.h"
#include <iostream>
using namespace std;

BuddyAllocator::BuddyAllocator (int _basic_block_size, int _total_memory_length){
  basic_block_size = _basic_block_size, total_memory_size = _total_memory_length;

  //make the first memory block
  mem_start = new char(total_memory_size);
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
}

void* BuddyAllocator::alloc(int length) {

  return malloc (length);
}

void BuddyAllocator::free(void* a) {
  /* Same here! */
  std::free (a);
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

