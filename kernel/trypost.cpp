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

#ifndef TRY_POST
#define TRY_POST

#include "getnode.cpp"

bool ttTryPost(const char* mailbox, void* msg) {

  DataNode* dn;
  Mailbox* m;

  dn = getNode(mailbox, rtsys->mailboxList);
  if (dn == NULL) {
    char buf[200];
    sprintf(buf, "ttTryPost: Non-existent mailbox '%s'!", mailbox);
    TT_MEX_ERROR(buf);
    return false;
  }

  m = (Mailbox*) dn->data;

  if (m->count == m->maxSize) {
    //mexPrintf("ttTryPost: Mailbox '%s' is full\n", mailbox);
    return false;
  } else {
    m->buffer->appendNode(new DataNode(msg, NULL));
    m->count++;
  }

  // release first waiting reader, if any
  UserTask* task = (UserTask*) m->readerQ->getFirst();
  if (task != NULL) {

    dn = (DataNode*) m->buffer->getFirst();
    task->mb_data = (void*)dn->data;  // save it for later (ttRetrieve)
    m->buffer->deleteNode(dn);
    m->count--;

    task->moveToList(rtsys->readyQs[task->affinity]);
  }

  return true;
}
#endif
