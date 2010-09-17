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

#ifndef POST
#define POST

#include "getnode.cpp"

void ttPost(const char* mailbox, void* msg) {

  DataNode* dn = getNode(mailbox, rtsys->mailboxList);
  if (dn == NULL) {
    char buf[200];
    sprintf(buf, "ttPost: Non-existent mailbox '%s'!", mailbox);
    TT_MEX_ERROR(buf);
    return;
  }

  Mailbox* m = (Mailbox*) dn->data;

  UserTask* task = (UserTask*) rtsys->running;

  if (m->count == m->maxSize) {
    //mexPrintf("ttPost: Mailbox '%s' is full, blocking\n", mailbox);
    // block the posting task
    task->moveToList(m->writerQ);
    task->state = WAITING;
    // Execute suspend hook
    task->suspend_hook(task);
    // Store the msg pointer in the task struct
    task->mb_data = msg;

  } else {

    m->buffer->appendNode(new DataNode(msg, NULL));
    m->count++;   // number of messages

    // release first waiting reader, if any
    task = (UserTask*) m->readerQ->getFirst();
    if (task != NULL) {

      dn = (DataNode*) m->buffer->getFirst();
      task->mb_data = (void*)dn->data;  // save it for later (ttRetrieve)
      m->buffer->deleteNode(dn);
      m->count--;

      task->moveToList(rtsys->readyQs[task->affinity]);
      task->state = READY;

    }
  }
}
#endif
