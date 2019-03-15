/* COMP3361 Operating Systems Winter 2019
 * Program 2 Sample Solution
 * 
 * File:   main.cpp
 * Author: Mike Goss <mikegoss@cs.du.edu>
 */

#include "FrameAllocator.h"
#include "RR_scheduler.h"
#include "PageTableManager.h"
#include "Process.h"

#include <MMU.h>

#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <stdlib.h>
#include <unistd.h>

using mem::MMU;

using std::cerr;
using std::cout;
using std::string;
using std::vector;

using namespace std;

int main(int argc, char* argv[]) {
  // Use command line argument as file name
  if (argc != 4) {
    std::cerr << "usage: program3 input_file\n";
    exit(1);
  }
  
  // Create allocator and page table manager (these will be shared among all 
  // processes in programming assignment 2)
  mem::MMU memory(128, 64);  // fixed memory size of 128 pages and TLB to 64 entries
  FrameAllocator allocator(memory);
  PageTableManager ptm(memory, allocator);
  
  // Variables to create processes
  int i = 2;
  int j = 0;
  vector<Process*> p;
  Process *Ppoint; 
  vector<int> procs;
  
  // Create the processes
  while (i < argc){
    Ppoint = new Process(stoi(argv[1]), argv[i], memory, ptm, allocator, i-1);
      p.push_back(Ppoint);
      procs.push_back(i-1);
      i++;
  }
  Ppoint = nullptr;

  RR_scheduler sched(p);

}
