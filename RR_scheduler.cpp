/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   RR_scheduler.cpp
 * Author: daniel
 * 
 * Created on March 13, 2019, 7:10 PM
 */

#include "RR_scheduler.h"
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

RR_scheduler::RR_scheduler(vector<Process*> &p, bool debug) {
    
    int k = 0;
    int l = 0;
    Process *Ppoint;
    while (p.size() > 0){
        p[k]->Exec();
        // delete out of the vector
        // delete off of heap
        if (p[k]->getDone()){
          (debug? this->outStream: cout) << p[k]->getStream();
            // cout << "Process " << k << " is done!\n";
            Ppoint = p[k];
            p.erase(p.begin() + k);
            delete Ppoint;
            Ppoint = nullptr;
            // cout << "p.size() = " << p.size() << "\n";
            if (p.size() > 0){
                k = k%p.size();
            }
        }
        else{
          (debug? this->outStream: cout) << p[k]->getStream();
            // cout << k << "\n";
            k = (k+1)%p.size();
        }
        // cout << std::dec << l << " this is l\n";
        l++;
    }
}


