/*
 * Copyright (c) 2009 Lund University
 *
 * Written by Anton Cervin, Dan Henriksson and Martin Ohlin,
 * Department of Automatic Control LTH, Lund University, Sweden.
 *   
 * This file is part of Truetime 2.0 beta.
 *
 * Truetime 2.0 beta is free software: you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation, either version 3 of the
 * License, or (at your option) any later version.
 *
 * Truetime 2.0 beta is distributed in the hope that it will be useful, but
 * without any warranty; without even the implied warranty of
 * merchantability or fitness for a particular purpose. See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Truetime 2.0 beta. If not, see <http://www.gnu.org/licenses/>
 */

#ifndef __TT_KERNEL_H__
#define __TT_KERNEL_H__

#include "truetime.h"

enum { FP, DM, EDF };                // pre-defined priority functions
enum { ARRIVAL, RELEASE, START, SUSPEND, RESUME, FINISH, RUNKERNEL }; // hooks
enum { UNUSED, OVERRUN, TIMER, NETWORK, EXTERNAL }; // handler types 
enum { RISING=1, FALLING=2, EITHER=3 }; // external trigger types 
enum { ZERO, POSITIVE, NEGATIVE }; // external trigger states
enum { CBS_OK, CBS_OVERLOAD }; // CBS states

// Execution time for finished task
#define FINISHED -1.0

// Task execution log types
#define NBRLOGS 4
enum { RESPONSETIMELOG, RELEASELATENCYLOG, STARTLATENCYLOG, EXECTIMELOG }; 

// Task states
enum { SLEEPING, READY, RUNNING, WAITING }; 

void init();
void cleanup();

class Task;
class List;
class InterruptHandler;
class UserTask;
class Trigger;
class NetworkInterface;

class RTsys {
 public:
  bool init_phase;    // false when the simulation is running
  bool initialized;   // true if ttInitKernel has been called
  bool error;         // true if simulation should stop
  bool started;       // true after time zero
  
  int nbrOfInputs;
  int nbrOfOutputs;
  int nbrOfTriggers;
  int nbrOfNetworks;

  int nbrOfSchedTasks;
  
  double time;      // Current time in simulation
  double prevHit;   // Previous invocation of kernel
  double nextHit;   // Next invocation of kernel

  double *inputs;          // Vector of input port (analogin) values
  double *outputs;         // Vector of output port (analogout) values
  double *oldtriggerinputs;
  int trigType;            // Trigger type (rising, falling, either)

  double *nwSnd;           // Vector of snd outputs 
  double *oldnwSnd;        
  double *oldnetworkinputs;

  Task *running;           // Currently running task being simulated
  //Task *runningUserTask; 

  Task **runnings;         // Currently running tasks on the different CPUs
  Task **runningUserTasks; // Currently running user tasks (may be preempted by handlers)

  int nbrOfCPUs;    // number of cores
  int currentCPU;   // core being currently simulated
  List **readyQs;   // vector of Ready Qs

  List *timeQ;    // usertasks and handlers waiting for release, time-sorted
  List *tmpQ;     // temporary Q, used for some re-sorting operations

  List *taskList;      // List of datanodes with pointers to created tasks
  List *handlerList;   // List of datanodes with pointers to created handlers
  List *timerList;     // List of datanodes with pointers to timer handlers
  List *monitorList;   // List of datanodes with pointers to created monitors
  List *eventList;     // List of datanodes with pointers to created event
  List *mailboxList;   // List of datanodes with pointers to created mailboxes
  List *semaphoreList; // List of datanodes with pointers to created semaphores
  List *logList;       // List of datanodes with pointers to created logs
  List *cbsList;       // List of datanodes with pointers to created CBSs

  char blockName[MAXCHARS]; // Name of Simulink block  

  mxArray *initArg;     // Pointer to block init argument (C++ only)
  void *userData;       // Pointer to arbitrary user data (C++ only)

  double (*prioFcn)(UserTask*);    // Priority function (see priofunctions.cpp)

  double contextSwitchTime;         // Time for a full context save/restore
  InterruptHandler* kernelHandler;  // Handler simulating context switches
  double contextSimTime;            // Execution time of handler code function
  UserTask* suspended;              // Last suspended usertask (context switch
                                    // if another task is resumed or started)

  Trigger *triggers;                   // Vector of triggers
  NetworkInterface *networkInterfaces; // Vector of network interfaces

  // Function pointers
  double (*contextSwitchCode)(int, void*); // Code function for context switch handler

  int (*prioCmp)(Node* , Node*);  // Compare function for priority-sorted lists
  int (*timeCmp)(Node* , Node*);  // Compare function for time-sorted lists
                                   // (see compfunctions.cpp)

  void (*default_arrival)(UserTask*);  // Default kernel hooks
  void (*default_release)(UserTask*);  // (see defaulthooks.cpp)
  void (*default_start)(UserTask*);
  void (*default_suspend)(UserTask*);
  void (*default_resume)(UserTask*);
  void (*default_finish)(UserTask*);
  void (*default_runkernel)(UserTask*, double duration); 

  double (*prioFP)(UserTask*);         // Standard priority functions
  double (*prioEDF)(UserTask*);
  double (*prioDM)(UserTask*);

  double energyLevel;       // Input from the battery
  double energyConsumption; // How much we consume
  double cpuScaling;        // How fast we run
  double clockDrift;        // 
  double clockOffset;       // Offset from the nominal time

#ifdef KERNEL_MATLAB
  mxArray* rtsysptr;        // Pointer to global variable "_rtsys"
  mxArray *segArray;
#endif

  RTsys();  // constructor
  ~RTsys(); // deconstructor
};

#include "getnode.cpp"

#include "ttnetwork.h"

#include "matlab/getrtsys.cpp"

#include "task.h"
#include "log.h"
#include "blockdata.h"
#include "usertask.h"
#include "cbs.h"

#include "timer.h"
#include "trigger.h"
#include "networkinterface.h"
#include "handler.h"

#include "monitor.h"
#include "event.h"
#include "mailbox.h"
#include "semaphore.h"


/**
 * RTsys Constructor 
 */
RTsys::RTsys() {

  init_phase = true;
  initialized = false;
  error = false;
  started = false;

  nbrOfInputs = 0;
  nbrOfOutputs = 0;
  nbrOfTriggers = 0;
  nbrOfSchedTasks = 0;
  
  time = 0.0; 
  prevHit = 0.0;
  nextHit = 0.0;

  inputs = NULL;   
  outputs = NULL;  
  oldtriggerinputs = NULL;

  running = NULL;
  //runningUserTask = NULL;

  runnings = NULL;
  runningUserTasks = NULL;

  //readyQ = NULL;
  timeQ = NULL;
  tmpQ = NULL;

  nbrOfCPUs = 1;  // default only 1 CPU
  currentCPU = 0;
  readyQs = NULL;

  taskList = NULL;
  handlerList = NULL;
  timerList = NULL;
  monitorList = NULL;
  eventList = NULL;
  mailboxList = NULL;
  semaphoreList = NULL;
  logList = NULL;
  cbsList = NULL;

  triggers = NULL;
  networkInterfaces = NULL;

  initArg = NULL;
  userData = NULL;

  prioFcn = NULL;

  contextSwitchTime = 0.0;
  contextSimTime = 0.0;
  kernelHandler = NULL;
  suspended = NULL;
  
  nbrOfNetworks = 0;
  nwSnd = NULL;  
  oldnwSnd = NULL;
  oldnetworkinputs = NULL;

  energyLevel = 1;       // Energy level from the battery
  energyConsumption = 0; // How much we consume
  cpuScaling = 1;        // No scaling as default
  clockDrift = 1;        // No drift as default
  clockOffset = 0;       // No offset as default

#ifdef KERNEL_MATLAB
  rtsysptr = NULL;
#endif
}

/**
 * RTsys Destructor 
 */
RTsys::~RTsys() {
  DataNode *dn, *tmp;

  if (inputs) delete[] inputs;
  if (outputs) delete[] outputs;
  if (oldtriggerinputs) delete[] oldtriggerinputs;
  if (nwSnd) delete[] nwSnd;
  if (oldnwSnd) delete[] oldnwSnd;
  if (oldnetworkinputs) delete[] oldnetworkinputs;

  //if (readyQ) delete readyQ;
  if (timeQ) delete timeQ;
  if (tmpQ) delete tmpQ;

  if (readyQs) {
    for (int i=0; i<nbrOfCPUs; i++) {
      if (readyQs[i]) delete readyQs[i];
    }
    delete[] readyQs;
  }

  if (runnings) {
    delete[] runnings;
  }
  if (runningUserTasks) {
    delete[] runningUserTasks;
  }
   
  // Delete all tasks and the tasklist
  if (taskList) {
    dn = (DataNode*) taskList->getFirst();
    while (dn != NULL) {
      UserTask* task = (UserTask*) dn->data; 
      delete task;
      tmp = dn;
      dn = (DataNode*) dn->getNext();
      delete tmp;
    }
    delete taskList;
  }
  // Delete all handlers and the handlerlist
  if (handlerList) {
    dn = (DataNode*) handlerList->getFirst();
    while (dn != NULL) {
      InterruptHandler* hdl = (InterruptHandler*) dn->data;
      delete hdl;
      tmp = dn;
      dn = (DataNode*) dn->getNext();
      delete tmp;
    }
    delete handlerList;
  }
  // Delete the timerlist
  if (timerList) {
    dn = (DataNode*) timerList->getFirst();
    while (dn != NULL) {
      tmp = dn;
      dn = (DataNode*) dn->getNext();
      delete tmp;
    }
    delete timerList;
  }  
  // Delete all monitors and the monitorlist
  if (monitorList) {
    dn = (DataNode*) monitorList->getFirst();
    while (dn != NULL) {
      Monitor* mon = (Monitor*) dn->data;
      delete mon;
      tmp = dn;
      dn = (DataNode*) dn->getNext();
      delete tmp;
    }
    delete monitorList;
  }
  // Delete all events and the eventlist
  if (eventList) {
    dn = (DataNode*) eventList->getFirst();
    while (dn != NULL) {
      Event* ev = (Event*) dn->data;
      delete ev;
      tmp = dn;
      dn = (DataNode*) dn->getNext();
      delete tmp;
    }
    delete eventList;
  }
  // Delete triggers
  if (triggers) {
    delete[] triggers;
  }
  // Delete network interfaces
  if (networkInterfaces) {
    delete[] networkInterfaces;
  }

  // Delete all mailboxes and the mailboxlist
  if (mailboxList) {
    dn = (DataNode*) mailboxList->getFirst();
    while (dn != NULL) {
      Mailbox* mb = (Mailbox*) dn->data;
      delete mb;
      tmp = dn;
      dn = (DataNode*) dn->getNext();
      delete tmp;
    }
    delete mailboxList;
  } 
  // Delete all semaphores and the semaphorelist
  if (semaphoreList) {
    dn = (DataNode*) semaphoreList->getFirst();
    while (dn != NULL) {
      Semaphore* sem = (Semaphore*) dn->data;
      delete sem;
      tmp = dn;
      dn = (DataNode*) dn->getNext();
      delete tmp;
    }
    delete semaphoreList;
  } 
  // Delete all logs and the loglist
  if (logList) {
    dn = (DataNode*) logList->getFirst();
    while (dn != NULL) {
      Log* log = (Log*) dn->data;
      delete log;
      tmp = dn;
      dn = (DataNode*) dn->getNext();
      delete tmp;
    }
    delete logList;
  } 
  // Delete all CBSs and the CBS list
  if (cbsList) {
    dn = (DataNode*) cbsList->getFirst();
    while (dn != NULL) {
      CBS* cbs = (CBS*) dn->data;
      delete cbs;
      tmp = dn;
      dn = (DataNode*) dn->getNext();
      delete tmp;
    }
    delete cbsList;
  } 
}


RTsys *rtsys;   // GLOBAL VARIABLE USED BY ALL KERNEL BLOCKS / KERNEL PRIMITIVES
char errbuf[MAXERRBUF];

#endif // __TT_KERNEL_H__
