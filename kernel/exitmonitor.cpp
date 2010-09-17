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

#ifndef EXIT_MONITOR
#define EXIT_MONITOR

#include "getnode.cpp"

void ttExitMonitor(const char *nameOfMonitor) {

  Monitor* mon;
  UserTask* task;

  DataNode* dn = getNode(nameOfMonitor, rtsys->monitorList);
  if (dn == NULL) {
    // Monitor does not exist 
    char buf[200];
    sprintf(buf, "ttExitMonitor: Non-existent monitor '%s'!", nameOfMonitor);
    TT_MEX_ERROR(buf);
    return;
  }

  task = (UserTask*) rtsys->running;
  mon = (Monitor*) dn->data;
  
  if (mon->heldBy != task) {
    char buf[200];
    sprintf(buf, "ttExitMonitor: Task '%s' not holding monitor '%s'!", task->name, nameOfMonitor);
    TT_MEX_ERROR(buf);
    return;
  }

  // Priority Inheritance, reset
  task->tempPrio = 0.0;
  task->prioRaised = false;
  // Reshuffle readyQ
  if (task->myList == rtsys->readyQs[rtsys->currentCPU]) {
    task->moveToList(rtsys->readyQs[rtsys->currentCPU]);
    task->state = READY;
  }

  mon->heldBy = NULL;

  // Move first waiting task to readyQ
  task = (UserTask*) mon->waitingQ->getFirst();
  if (task != NULL) {
    task->moveToList(rtsys->readyQs[task->affinity]);
    task->state = READY;
    mon->heldBy = task;
  }

}

#endif
