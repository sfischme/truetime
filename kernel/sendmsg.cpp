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

#ifndef SEND_MSG
#define SEND_MSG

#include "getnetwork.cpp"

// do the dirty work: poke around inside nwsys of the network block 
void nwSendMsg(NWmsg *nwmsg, RTnetwork *nwsys) {
  // set time when finished preprocessing
  nwmsg->waituntil = (rtsys->time- rtsys->clockOffset) / rtsys->clockDrift
    + nwsys->nwnodes[nwmsg->sender]->predelay;
  nwmsg->collided = 0; // This message has not collided (802.11)

  // enqueue message in preprocQ
  nwsys->nwnodes[nwmsg->sender]->preprocQ->appendNode(nwmsg);
}


void ttSendMsg(int receiver, void *data, int length);
void ttSendMsg(int receiver, void *data, int length, int priority);
void ttSendMsg(int networkNbr, int receiver, void *data, int length);
void ttSendMsg(int networkNbr, int receiver, void *data, int length, int priority);
void ttUltrasoundPing();
void ttUltrasoundPing(int network);


void ttSendMsg(int receiver, void *data, int length)
{
  ttSendMsg(1, receiver, data, length);
}

void ttSendMsg(int networkNbr, int receiver, void *data, int length)
{
  NetworkInterface* nwi = getNetwork(networkNbr);
  if (nwi == NULL) {
    char buf[200];
    sprintf(buf, "ttSendMsg: Network #%d not present!", networkNbr);
    TT_MEX_ERROR(buf);
    return;
  }
  int priority = nwi->nodeNbr;
  ttSendMsg(networkNbr, receiver, data, length, priority);
}

void ttSendMsg(int receiver, void *data, int length, int priority)
{
  ttSendMsg(1, receiver, data, length, priority);
}

void ttSendMsg(int networkNbr, int receiver, void *data, int length, int priority)
{
  NetworkInterface* nwi = getNetwork(networkNbr);
  if (nwi == NULL) {
    char buf[200];
    sprintf(buf, "ttSendMsg: Network #%d not present!", networkNbr);
    TT_MEX_ERROR(buf);
    return;
  }
  
  if (receiver < 0 || receiver > nwi->nwsys->nbrOfNodes) {
	char buf2[200];
	sprintf(buf2,"ttSendMsg: receiver number out of bounds! #%d",receiver);
 //   TT_MEX_ERROR("ttSendMsg: receiver number out of bounds!");
	TT_MEX_ERROR(buf2);
    return;
  }

  NWmsg *nwmsg = new NWmsg();
  nwmsg->sender = nwi->nodeNbr;
  nwmsg->receiver = receiver-1;
  nwmsg->data = data;
  nwmsg->dataMATLAB = NULL;
  nwmsg->length = length;
  nwmsg->prio = priority;
  nwmsg->timestamp = rtsys->time;

  nwSendMsg(nwmsg, nwi->nwsys);
  if (rtsys->oldnwSnd[nwi->portNbr] == 0.0) {
    rtsys->nwSnd[nwi->portNbr] = 1.0; // trigger output
  } else {
    rtsys->nwSnd[nwi->portNbr] = 0.0; // trigger output
  }

}

// For use from Matlab mex function
void ttSendMsgMATLAB(int networkNbr, int receiver, int length, mxArray* dataMATLAB, double priority)
{
  NetworkInterface* nwi = getNetwork(networkNbr);

  if (nwi == NULL) {
    // is there no network?
    char buf[200];
    sprintf(buf, "ttSendMsg: Network #%d not present!", networkNbr);
	TT_MEX_ERROR(buf);
    return;
  }

  if (receiver < 0 || receiver > nwi->nwsys->nbrOfNodes) {
    char buf1[200];
	sprintf(buf1, "ttSendMsg: receiver number out of bounds! #%d",receiver);
//    TT_MEX_ERROR("ttSendMsg: receiver number out of bounds");
	TT_MEX_ERROR(buf1);
    return;
  }

  NWmsg *nwmsg = new NWmsg();
  nwmsg->sender = nwi->nodeNbr;
  nwmsg->receiver = receiver-1;
  nwmsg->data = NULL;
  nwmsg->dataMATLAB = dataMATLAB;
  nwmsg->length = length;
  nwmsg->prio = priority;
  nwmsg->timestamp = rtsys->time;
  nwmsg->signalPower = 0.0;
    
  nwSendMsg(nwmsg, nwi->nwsys);
  if (rtsys->oldnwSnd[nwi->portNbr] == 0.0) {
    rtsys->nwSnd[nwi->portNbr] = 1.0; // trigger output
  } else {
    rtsys->nwSnd[nwi->portNbr] = 0.0; // trigger output
  }
}
 
void ttSendMsgMATLAB(int networkNbr, int receiver, int length, mxArray* dataMATLAB)
{
  NetworkInterface* nwi = getNetwork(networkNbr);
  if (nwi == NULL) {
    // is there no network?
    char buf[200];
    sprintf(buf, "ttSendMsg: Network #%d not present!", networkNbr);
    TT_MEX_ERROR(buf);
    return;
  }
  int priority = nwi->nodeNbr;
  ttSendMsgMATLAB(networkNbr, receiver, length, dataMATLAB, priority);
}


void ttUltrasoundPing() 
{
  // Default network nbr == 1
  ttUltrasoundPing(1);
}

void ttUltrasoundPing(int networkNbr)
{
  NetworkInterface* nwi = getNetwork(networkNbr);
  if (nwi == NULL) {
    char buf[200];
    sprintf(buf, "ttUltrasoundPing: Network #%d not present!", networkNbr);
    TT_MEX_ERROR(buf);
    return;
  }
  
  NWmsg *nwmsg = new NWmsg();
  nwmsg->sender = nwi->nodeNbr;
  nwmsg->receiver = -1; // broadcast
  nwmsg->data = NULL;
  nwmsg->dataMATLAB = NULL;
  nwmsg->length = 0;
  nwmsg->prio = 0;
  nwmsg->signalPower = 0.0;

  nwSendMsg(nwmsg, nwi->nwsys);
  if (rtsys->oldnwSnd[nwi->portNbr] == 0.0) {
    rtsys->nwSnd[nwi->portNbr] = 1.0; // trigger output
  } else {
    rtsys->nwSnd[nwi->portNbr] = 0.0; // trigger output
  }

}

#endif
