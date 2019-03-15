/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   RR_scheduler.h
 * Author: daniel
 *
 * Created on March 13, 2019, 7:10 PM
 */

#ifndef RR_SCHEDULER_H
#define RR_SCHEDULER_H

#include "FrameAllocator.h"
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

using std::cout;
using std::vector;


class RR_scheduler {
public:
    
    /**
    * Constructor
    */
    RR_scheduler(vector<Process*> &processes, bool debug=false);
//    
    
    /**
    * Destructor - empty destructor
    */
    virtual ~RR_scheduler(void) {};

    // Other constructors, assignment
    RR_scheduler(const RR_scheduler &other) = delete;
    RR_scheduler(RR_scheduler &&other) = delete;
    RR_scheduler operator=(const RR_scheduler &other) = delete;
    RR_scheduler operator=(RR_scheduler &&other) = delete;

    std::string getString(void){return outStream.str();};

private:

    std::stringstream outStream;

};

#endif /* RR_SCHEDULER_H */
