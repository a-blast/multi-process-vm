#include "gtest/gtest.h"

#include "FrameAllocator.h"
#include "PageTableManager.h"
#include "Process.h"
#include "PageFaultHandler.h"
#include "WritePermissionFaultHandler.h"
#include "RR_scheduler.h"

#include <MMU.h>

#include <algorithm>
#include <vector>
#include <cctype>
#include <cstring>
#include <iomanip>
#include <iostream>
#include <sstream>

auto processOutputGetter =
  [](std::string filePath, mem::MMU &memory,
     PageTableManager &ptm, FrameAllocator &alloc){
    Process proc(5000, filePath, memory, ptm, alloc, 1);
    proc.setDebug();
    proc.Exec();
    std::istringstream outStream(proc.getStream());
    return outStream;
  };
auto getExpectedOutput =
  [](std::string filePath){
    std::ifstream testFile;
    testFile.open(filePath);
    if (testFile.fail()){
      throw std::runtime_error("ERROR: file not found: "+ filePath);
    }
    return testFile;
  };
auto validateOutput =
  [](std::string output, std::ifstream validationStream,
     bool debug, int breakCount = 5){
    std::string outputLine;
    std::string validationLine;
    std::istringstream outputStream(output);
    bool linesAreEqual;
    int failureCounter = 0;
    while(getline(validationStream, validationLine)){
      getline(outputStream, outputLine);
      linesAreEqual = (validationLine == outputLine);
      if(debug){std::cout << (linesAreEqual?"\n~~~\n":"")
                          << "O: " << outputLine << "\n"
                          << "E: " << validationLine
                          << (linesAreEqual?"\n~~~\n":"\n");}
      EXPECT_EQ(true, linesAreEqual);
      if(!linesAreEqual){failureCounter++;}
      if(failureCounter == breakCount){break;}
    }
  };


TEST(ProcessOutput, trace1){
  mem::MMU memory(128);  // fixed memory size of 128 pages
  FrameAllocator allocator(memory);
  PageTableManager ptm(memory, allocator);
  std::istringstream ss;
  ss = processOutputGetter("./trace1-3.txt", memory, ptm, allocator);
  validateOutput(ss.str(), getExpectedOutput("./trace1-3.txt.out"),false);
}

TEST(ProcessOutput, trace2){
  mem::MMU memory(128);  // fixed memory size of 128 pages
  FrameAllocator allocator(memory);
  PageTableManager ptm(memory, allocator);
  std::istringstream ss;
  ss = processOutputGetter("./trace2-3_multi-page.txt", memory, ptm, allocator);
  validateOutput(ss.str(),
                 getExpectedOutput("./trace2-3_multi-page.txt.out"),false);
}

TEST(ProcessOutput, trace3){
  mem::MMU memory(128);  // fixed memory size of 128 pages
  FrameAllocator allocator(memory);
  PageTableManager ptm(memory, allocator);
  std::istringstream ss;
  ss = processOutputGetter("./trace3-3_edge-addr.txt", memory, ptm, allocator);
  validateOutput(ss.str(),
                 getExpectedOutput("./trace3-3_edge-addr.txt.out"),false);
}

TEST(ProcessOutput, trace4){
  mem::MMU memory(128);  // fixed memory size of 128 pages
  FrameAllocator allocator(memory);
  PageTableManager ptm(memory, allocator);
  std::istringstream ss;
  ss = processOutputGetter("./trace4-3_wprotect.txt", memory, ptm, allocator);
  validateOutput(ss.str(),
                 getExpectedOutput("./trace4-3_wprotect.txt.out"),false);
}

TEST(ProcessOutput, trace5){
  mem::MMU memory(128);  // fixed memory size of 128 pages
  FrameAllocator allocator(memory);
  PageTableManager ptm(memory, allocator);
  std::istringstream ss;
  ss = processOutputGetter("./trace5-3_pagefaults.txt", memory, ptm, allocator);
  validateOutput(ss.str(),
                 getExpectedOutput("./trace5-3_pagefaults.txt.out"),false);
}

std::vector<Process*> GetAllProcesses(const int timeSlice, mem::MMU &memory,
                                      PageTableManager &ptm, FrameAllocator &alloc){
  std::vector<Process*> processes;
  Process *processPointer;
  int pid = 0;
  std::vector<std::string> fileNames = {"./trace1-3.txt","./trace2-3_multi-page.txt",
                                        "./trace3-3_edge-addr.txt", "./trace4-3_wprotect.txt",
                                        "./trace5-3_pagefaults.txt"};
  while(pid<5){
                processPointer = new Process(timeSlice, fileNames[pid],
                                             memory, ptm, alloc, pid+1);
                processPointer->setDebug();
                processes.push_back(processPointer);
                pid++;
  }
  processPointer = nullptr;
  return processes;
}

TEST(ProcessOutput, traceAll_ts2){
  mem::MMU memory(128);  // fixed memory size of 128 pages
  FrameAllocator allocator(memory);
  PageTableManager ptm(memory, allocator);
  std::vector<Process*> processes = GetAllProcesses(2, memory, ptm, allocator);
  RR_scheduler out(processes,true);
  validateOutput(out.getString(),
                 getExpectedOutput("./trace_all_ts2.out"),false);
}

TEST(ProcessOutput, traceAll_ts3_2){
  mem::MMU memory(128);  // fixed memory size of 128 pages
  FrameAllocator allocator(memory);
  PageTableManager ptm(memory, allocator);
  std::vector<Process*> processes = GetAllProcesses(3, memory, ptm, allocator);
  RR_scheduler ss(processes,true);
  validateOutput(ss.getString(),
                 getExpectedOutput("./trace_all_ts3_2.out"),true);
}

TEST(ProcessOutput, traceAll_ts4){
  mem::MMU memory(128);  // fixed memory size of 128 pages
  FrameAllocator allocator(memory);
  PageTableManager ptm(memory, allocator);
  std::vector<Process*> processes = GetAllProcesses(4, memory, ptm, allocator);
  RR_scheduler ss(processes,true);
  validateOutput(ss.getString(),
                 getExpectedOutput("./trace_all_ts4.out"),false);
}

TEST(ProcessOutput, traceAll_ts5_2){
  mem::MMU memory(128);  // fixed memory size of 128 pages
  FrameAllocator allocator(memory);
  PageTableManager ptm(memory, allocator);
  std::vector<Process*> processes = GetAllProcesses(5, memory, ptm, allocator);
  RR_scheduler ss(processes,true);
  validateOutput(ss.getString(),
                 getExpectedOutput("./trace_all_ts5_2.out"),true);
}

int main(int argc, char* argv[]){
  ::testing::InitGoogleTest(&argc,argv);
  return RUN_ALL_TESTS();
}
