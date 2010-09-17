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

#ifndef CREATE_TRIGGER
#define CREATE_TRIGGER

#include "getnode.cpp"

void ttCreateExternalTrigger(const char *nameOfHandler, double latency) {

  DataNode* dn = getNode(nameOfHandler, rtsys->handlerList);
  if (dn==NULL) {
    char buf[200];
    sprintf(buf, "ttCreateExternalTrigger: Non-existent interrupt handler '%s'!",nameOfHandler);
    TT_MEX_ERROR(buf);
    return;
  }
  if (rtsys->prioFcn == NULL) {
    TT_MEX_ERROR("ttCreateExternalTrigger: Kernel must be initialized before creation of triggers!");
    return;
  }

  InterruptHandler* hdl = (InterruptHandler*) dn->data;

  Trigger *trigger = new Trigger(latency,hdl);
  rtsys->triggerList->appendNode(new DataNode(trigger, ""));
  rtsys->nbrOfTriggers++;
}

#endif
