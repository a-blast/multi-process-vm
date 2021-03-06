/*
 * Process - execute memory trace file
 * 
 * COMP3361 Winter 2019 Program 2 Sample Solution
 *
 * File:   Process.h
 * Author: Mike Goss <mikegoss@cs.du.edu>
 */

#ifndef PROCESS_H
#define PROCESS_H

#include "PageTableManager.h"

#include <MMU.h>

#include <fstream>
#include <memory>
#include <string>
#include <vector>
#include <iomanip>
#include <iostream>
#include <sstream>

class Process {
public:
  /**
   * Constructor - open trace file, initialize processing.
   *   Must be called in kernel mode.
   * 
   * @param file_name_ source of trace commands
   * @param memory MMU class object to use for memory
   * @param ptm page table manager
   */
  Process(const int time_slice, const std::string &file_name_, 
          mem::MMU &memory_, PageTableManager &ptm_,
          FrameAllocator &frame_alloc_, int pid);

  /**
   * Destructor - close trace file, clean up processing
   */
  virtual ~Process(void);

  // Other constructors, assignment
  Process(const Process &other) = delete;
  Process(Process &&other) = delete;
  Process operator=(const Process &other) = delete;
  Process operator=(Process &&other) = delete;
  
  /**
   * Exec - read and process commands from trace file
   * 
   */
  void Exec(void);

  /*
    setDebug - divert output from cout to a buffer stream for testing validation 
   */
  void setDebug(void){this->debug = true;}
  std::string getStream(void){
    std::string out = outStream.str();
    std::stringstream().swap(outStream);
    return out;
  }
  
  // for testing output
  bool debug=false;
  std::stringstream outStream;

  bool getDone(){
    return done;
  }

  /*
    killSelf - delete all pages (table & allocated pages) associated w/ the process
    exits with memory in KERNEL mode.
   */
  void killSelf(void);

private:

  friend class WritePermissionFaultHandler;
  friend class PageFaultHandler;

  // Trace file
  std::string file_name;
  std::fstream trace;
  long line_number;
  
  // Multiprocess variable
  int num_pages;
  int quota;
  int num_cmd;
  const int ts;
  bool done = false;

  // Memory contents
  mem::MMU &memory;
  mem::PMCB proc_pmcb;  // PMCB for this process
  std::vector<mem::Addr> pagesAllocatedPhysical;
  FrameAllocator &frame_alloc;

  // PID
  int pid;

  // Page table access
  PageTableManager &ptm;
  /**
   * ParseCommand - parse a trace file command.
   *   Aborts program if invalid trace file.
   * 
   * @param line return the original command line
   * @param cmd return the command name
   * @param cmdArgs returns a vector of argument values
   * @return true if command parsed, false if end of file
   */
  bool ParseCommand(
      std::string &line, std::string &cmd, std::vector<uint32_t> &cmdArgs);
  
  /**
   * Command executors. Arguments are the same for each command.
   *   Form of the function is CmdX, where "X' is the command name, capitalized.
   * @param line original text of command line
   * @param cmd command, converted to all lower case
   * @param cmdArgs arguments to command
   */
  void CmdAlloc(const std::string &line, 
                const std::string &cmd, 
                const std::vector<uint32_t> &cmdArgs);
  void CmdQuota(const std::string &line, 
                const std::string &cmd, 
                const std::vector<uint32_t> &cmdArgs);
  void CmdCmp(const std::string &line, 
              const std::string &cmd, 
              const std::vector<uint32_t> &cmdArgs);
  void CmdSet(const std::string &line, 
              const std::string &cmd, 
              const std::vector<uint32_t> &cmdArgs);
  void CmdFill(const std::string &line, 
               const std::string &cmd, 
               const std::vector<uint32_t> &cmdArgs);
  void CmdDup(const std::string &line, 
              const std::string &cmd, 
              const std::vector<uint32_t> &cmdArgs);
  void CmdPrint(const std::string &line, 
                const std::string &cmd, 
                const std::vector<uint32_t> &cmdArgs);
  void CmdPerm(const std::string &line, 
               const std::string &cmd, 
               const std::vector<uint32_t> &cmdArgs);
};

#endif /* PROCESS_H */
