#include "Ackerman.h"
#include "BuddyAllocator.h"
#include <unistd.h>

void easytest(BuddyAllocator* ba){
  // be creative here
  // know what to expect after every allocation/deallocation cycle

  // here are a few examples
  ba->printlist();
  // allocating a byte
  char * mem = (char *) ba->alloc (1);
  // now print again, how should the list look now
  ba->printlist ();

  ba->free (mem); // give back the memory you just allocated
  ba->printlist(); // shouldn't the list now look like as in the beginning

}

int main(int argc, char ** argv) {

  int basic_block_size = 128, memory_length = 512;

  //my getopt()
  int opt;
  int basic_bs;
  int total_ms;

  while((opt = getopt(argc, argv, ":if:lrx")) != -1)
  {
      switch(opt)
      {
        case 'b':
          basic_bs = stoi(optarg);
          cout << "Input basic block length of:" << basic_bs << endl;
          break;

        case 's':
          total_ms = stoi(optarg);
          cout << "Input memory total length of:" << total_ms << endl;

          break;

        case '?':
          cout << "Unknown option: " << optopt << endl;
          break;

        default:
          basic_bs = basic_block_size;
          total_ms = memory_length;
      }
  }

  // create memory manager
  BuddyAllocator * allocator = new BuddyAllocator(basic_block_size, memory_length);

  // the following won't print anything until you start using FreeList and replace the "new" with your own implementation
  easytest (allocator);
  
  // stress-test the memory manager, do this only after you are done with small test cases
  Ackerman* am = new Ackerman ();
  am->test(allocator); // this is the full-fledged test. 
  
  // destroy memory manager
  
  delete allocator;
  
}
