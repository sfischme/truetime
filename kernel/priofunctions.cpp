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

#ifndef PRIO_FUNCTIONS
#define PRIO_FUNCTIONS

/* Fixed-priority scheduling */
double prioFP(UserTask* t) {

  //debugPrintf("prioFP for task '%s': %f\n", t->name, t->priority);

  return t->priority;
}

/* Deadline-monotonic scheduling */
double prioDM(UserTask* t) {

  //debugPrintf("prioDM for task '%s': %f\n", t->name, t->deadline);

  return t->deadline;
}

/* Earliest-deadline-first scheduling, with
   support for constant bandwidth servers */
double prioEDF(UserTask* t) {
  
  //debugPrintf("prioEDF for task '%s': %f\n", t->name, t->absDeadline);
  
  if (t->cbs) {
    // The task is associated with a CBS: inherit the deadline of the CBS
    return t->cbs->ds;
  } else {
    // Else, return the absolute deadline of the task
    return t->absDeadline;
  }
}

#endif
