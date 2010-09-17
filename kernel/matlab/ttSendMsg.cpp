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

#define KERNEL_MATLAB
#include "../ttkernel.h" 



#include "../sendmsg.cpp"
#include "getrtsys.cpp"

void mexFunction( int nlhs, mxArray *plhs[],
                  int nrhs, const mxArray *prhs[] )
{
  int network, receiver, length;
  mxArray *data;
  double priority;

  rtsys = getrtsys(); // Get pointer to rtsys 

  if (rtsys==NULL) {
    return;
  }

  // Check number and type of arguments. 
  if (nrhs < 3 || nrhs > 4) {
    TT_MEX_ERROR("ttSendMsg: Wrong number of input arguments!\nUsage: ttSendMsg(receiver, data, length) or\n       ttSendMsg(receiver, data, length, priority) or\n       ttSendMsg([network receiver], data, length) or\n       ttSendMsg([network receiver], data, length, priority)");
    return;
  }

  if (mxIsDoubleScalar(prhs[0])) { // no network specified
    network = 1;
    receiver = (int)*mxGetPr(prhs[0]);
  } else if (mxIsDouble(prhs[0]) && !mxIsComplex(prhs[0]) && mxGetM(prhs[0])==1 && mxGetN(prhs[0])==2) {
    network = (int)*mxGetPr(prhs[0]);
    receiver = (int)*(mxGetPr(prhs[0])+1);
  } else {
    TT_MEX_ERROR("ttSendMsg: receiver must be a number or a vector [network receiver]");
    return;
  }

  data = mxDuplicateArray(prhs[1]);
  mexMakeArrayPersistent(data);

  if (!mxIsDoubleScalar(prhs[2])) {
    TT_MEX_ERROR("ttSendMsg: length must be a number");
    return;
  }
  length = (int)*mxGetPr(prhs[2]);
  if (nrhs == 4) {
    if(!mxIsDoubleScalar(prhs[3])) {
      TT_MEX_ERROR("ttSendMsg: priority must be a number");
      return;
    }
    priority = *mxGetPr(prhs[3]);
    ttSendMsgMATLAB(network, receiver, length, data, priority);
  } else {
    ttSendMsgMATLAB(network, receiver, length, data);
  }
}
