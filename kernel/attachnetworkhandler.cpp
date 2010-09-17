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

#ifndef ATTACH_NETWORK_HANDLER
#define ATTACH_NETWORK_HANDLER

#include "getnode.cpp"
#include "getnetwork.cpp"

// Register that there is a network but delay some initialization

void ttAttachNetworkHandler(int nwnbr, const char *nwhandler) {

  DataNode* dn = getNode(nwhandler, rtsys->handlerList);
  if (dn==NULL) {
    char buf[200];
    sprintf(buf, "ttAttachNetworkHandler: Non-existent interrupt handler '%s'!", nwhandler);
    TT_MEX_ERROR(buf);
    return;
  }
  if (nwnbr < 1) {
    TT_MEX_ERROR("ttAttachNetworkHandler: Network number must be positive!");
    return;
  }

  InterruptHandler* hdl = (InterruptHandler*) dn->data;
  NetworkInterface *nwi = getNetwork(nwnbr);

  if (nwi != NULL) {
    if (nwi->handler == NULL) {
      nwi->handler = hdl;
    } else {
      TT_MEX_ERROR("ttAttachNetworkHandler: Network handler already assigned!");
      return;
    }
  } else {
    char buf[200];
    sprintf(buf, "ttAttachNetworkHandler: The node is not connected to network %d!",nwnbr);
    TT_MEX_ERROR(buf);
  }
}

// If there is only one network
void ttAttachNetworkHandler(const char *nwhandler)
{
  ttAttachNetworkHandler(1, nwhandler);
}

#endif
