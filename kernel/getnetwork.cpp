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

#ifndef GET_NETWORK
#define GET_NETWORK

NetworkInterface *getNetwork(int networkNbr) {

  NetworkInterface *nwi;
  int found = 0;

  for (int i=0; i<rtsys->nbrOfNetworks; i++) {
    nwi = &(rtsys->networkInterfaces[i]);
    if (nwi->networkNbr == networkNbr) {
      found = 1;
      break;
    }
  }
  if (!found) {
    return NULL;
  } else {
    return nwi;
  }
}

#endif
