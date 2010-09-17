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

#ifndef __MEX_HELP_H__
#define __MEX_HELP_H__

#define TT_SCALAR 1
#define TT_STRING 2
#define TT_STRUCT 3

// These functions should exist...
#define mxIsDoubleScalar(M) (mxIsDouble(M) && !mxIsComplex(M) && mxGetM(M)==1 && mxGetN(M)==1)
#define mxIsPlainString(M) (mxIsChar(M) && mxGetM(M)==1)

// Handle errors and warnings in kernel primitives
#ifdef KERNEL_MATLAB
#define TT_MEX_ERROR(e) rtsys->error = true; mexErrMsgTxt(e)
#else
#define TT_MEX_ERROR(e) rtsys->error = true; snprintf(errbuf, MAXERRBUF, "%s", e)
#endif
#define TT_MEX_WARNING(e) mexPrintf("Warning: %s\n\n", e)

// Backwards compatibility with R12.1
//#ifndef mexPutArray  /* this symbol is only defined in R13 and later */
//#define mexPutVariable(w,n,d) mxSetName(d,n); mexPutArray(d,w)
//#define mexGetVariable(w,n) mexGetArray(n,w)
//#define mexGetVariablePtr(w,n) mexGetArrayPtr(n,w)
//#endif

#endif
