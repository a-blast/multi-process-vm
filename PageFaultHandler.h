#ifndef PFHANLDER_H
#define PFHANDLER_H

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
 * PageFaultHandler
 */
class PageFaultHandler : public mem::MMU::FaultHandler {
 public:
 PageFaultHandler(Process &proc_):proc(proc_){}
  /**
   * Run - handle fault
   * 
   * Print message and return false
   * 
   * @param pmcb user mode pmcb
   * @return false
   */
  virtual bool Run(const mem::PMCB &pmcb) {


    if(pmcb.operation_state == mem::PMCB::WRITE_OP){
      if(proc.num_pages >= proc.quota){
        (proc.debug? proc.outStream: cout) << "Quota exceeded, process terminated, ";
        proc.killSelf();
        return false;
      }
      
      proc.memory.set_kernel_PMCB();
      mem::Addr basePageVirtAddr = (pmcb.next_vaddress >> 14) << 14;
      proc.ptm.MapProcessPages(pmcb, basePageVirtAddr ,
                               1, proc.pagesAllocatedPhysical);
      proc.memory.set_user_PMCB(pmcb);
      proc.num_pages ++;
      return true;
    }else{
      (proc.debug? proc.outStream: cout)
        <<  "Read Page Fault at address " << std::setw(6) << std:: setfill('0')
        << std::hex << pmcb.next_vaddress << "\n";
      return false;
    }
  }
 private:
  Process &proc;
};

#endif
