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

#ifndef SET_DEADLINE
#define SET_DEADLINE

#include "getnode.cpp"

// Setting deadline of specific task
void ttSetDeadline(double value, const char *nameOfTask) {
  
  DataNode* dn = getNode(nameOfTask, rtsys->taskList);
  if (dn == NULL) {
    char buf[200];
    sprintf(buf, "ttSetDeadline: Non-existent task '%s'!", nameOfTask);
    TT_MEX_ERROR(buf);
    return;
  }

  UserTask* task = (UserTask*) dn->data; 
  task->deadline = value;

  task->absDeadline = task->release + task->deadline;

  // Reshuffle readyQ if task there (relevant for DM/EDF)
  if (task->myList == rtsys->readyQs[task->affinity])
    task->moveToList(rtsys->readyQs[task->affinity]);

} 

// Setting deadline of calling task
void ttSetDeadline(double value) {
  
  if(rtsys->running == NULL) {
    TT_MEX_ERROR("ttSetDeadline: No running task!");
    return;
  }

  if (!rtsys->running->isUserTask()) {
    TT_MEX_ERROR("ttSetDeadline: Can not be called by interrupt handler!");
    return;
  }

  ttSetDeadline(value, rtsys->running->name);
}

#endif
