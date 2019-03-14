#ifndef WPFHANLDER_H
#define WPFHANDLER_H

#include "Process.h"
#include "PageTableManager.h"

#include <MMU.h>

#include <algorithm>
#include <cctype>
#include <cstring>
#include <iomanip>
#include <iostream>
#include <sstream>

using mem::Addr;
using mem::kPageSize;
using std::cin;
using std::cout;
using std::cerr;
using std::getline;
using std::istringstream;
using std::string;
using std::vector;

/**
 * WritePermissionFaultHandler
 */
class WritePermissionFaultHandler : public mem::MMU::FaultHandler {
public:

  WritePermissionFaultHandler(Process &proc_):proc(proc_){}
  /**
   * Run - handle fault
   * 
   * Print message and return false
   * 
   * @param pmcb user mode pmcb
   * @return false
   */
  virtual bool Run(const mem::PMCB &pmcb) {

    (proc.debug? proc.outStream: cout) << "Write Permission Fault at address "
                                       << std:: setfill('0')
                                       << std::hex << std::setw(6)
                                       << pmcb.next_vaddress
                                       << "\n";
    return false;
  }


private:
  Process &proc;
};

#endif