#include "gtest/gtest.h"

#include "FrameAllocator.h"
#include "PageTableManager.h"
#include "Process.h"
#include "PageFaultHandler.h"
#include "WritePermissionFaultHandler.h"

#include <MMU.h>

#include <algorithm>
#include <cctype>
#include <cstring>
#include <iomanip>
#include <iostream>
#include <sstream>

auto processOutputGetter =
  [](std::string filePath, mem::MMU &memory, PageTableManager &ptm){
    Process proc(filePath, memory, ptm);
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
    while(getline(outputStream, outputLine)){
      getline(validationStream, validationLine);
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
  ss = processOutputGetter("./trace1-3.txt", memory, ptm);
  validateOutput(ss.str(), getExpectedOutput("./trace1-3.txt.out"),false);
}

TEST(ProcessOutput, trace2){
  mem::MMU memory(128);  // fixed memory size of 128 pages
  FrameAllocator allocator(memory);
  PageTableManager ptm(memory, allocator);
  std::istringstream ss;
  ss = processOutputGetter("./trace2-3_multi-page.txt", memory, ptm);
  validateOutput(ss.str(),
                 getExpectedOutput("./trace2-3_multi-page.txt.out"),false);
}

TEST(ProcessOutput, trace3){
  mem::MMU memory(128);  // fixed memory size of 128 pages
  FrameAllocator allocator(memory);
  PageTableManager ptm(memory, allocator);
  std::istringstream ss;
  ss = processOutputGetter("./trace3-3_edge-addr.txt", memory, ptm);
  validateOutput(ss.str(),
                 getExpectedOutput("./trace3-3_edge-addr.txt.out"),false);
}

TEST(ProcessOutput, trace4){
  mem::MMU memory(128);  // fixed memory size of 128 pages
  FrameAllocator allocator(memory);
  PageTableManager ptm(memory, allocator);
  std::istringstream ss;
  ss = processOutputGetter("./trace4-3_wprotect.txt", memory, ptm);
  validateOutput(ss.str(),
                 getExpectedOutput("./trace4-3_wprotect.txt.out"),false);
}

TEST(ProcessOutput, trace5){
  mem::MMU memory(128);  // fixed memory size of 128 pages
  FrameAllocator allocator(memory);
  PageTableManager ptm(memory, allocator);
  std::istringstream ss;
  ss = processOutputGetter("./trace5-3_pagefaults.txt", memory, ptm);
  validateOutput(ss.str(),
                 getExpectedOutput("./trace5-3_pagefaults.txt.out"),true);
}


// TEST(ProcessOutput, traceAll_ts2){
//   mem::MMU memory(128);  // fixed memory size of 128 pages
//   FrameAllocator allocator(memory);
//   PageTableManager ptm(memory, allocator);
//   std::istringstream ss;
//   ss = processOutputGetter("./trace3-3_edge-addr.txt", memory, ptm);
//   validateOutput(ss.str(),
//                  getExpectedOutput("./trace3-3_edge-addr.txt.out"),true);
// }

// TEST(ProcessOutput, traceAll_ts3_2){
//   mem::MMU memory(128);  // fixed memory size of 128 pages
//   FrameAllocator allocator(memory);
//   PageTableManager ptm(memory, allocator);
//   std::istringstream ss;
//   ss = processOutputGetter("./trace3-3_edge-addr.txt", memory, ptm);
//   validateOutput(ss.str(),
//                  getExpectedOutput("./trace3-3_edge-addr.txt.out"),true);
// }

// TEST(ProcessOutput, traceAll_ts4){
//   mem::MMU memory(128);  // fixed memory size of 128 pages
//   FrameAllocator allocator(memory);
//   PageTableManager ptm(memory, allocator);
//   std::istringstream ss;
//   ss = processOutputGetter("./trace3-3_edge-addr.txt", memory, ptm);
//   validateOutput(ss.str(),
//                  getExpectedOutput("./trace3-3_edge-addr.txt.out"),true);
// }

// TEST(ProcessOutput, traceAll_ts5_2){
//   mem::MMU memory(128);  // fixed memory size of 128 pages
//   FrameAllocator allocator(memory);
//   PageTableManager ptm(memory, allocator);
//   std::istringstream ss;
//   ss = processOutputGetter("./trace3-3_edge-addr.txt", memory, ptm);
//   validateOutput(ss.str(),
//                  getExpectedOutput("./trace3-3_edge-addr.txt.out"),true);
// }

int main(int argc, char* argv[]){
  ::testing::InitGoogleTest(&argc,argv);
  return RUN_ALL_TESTS();
}
