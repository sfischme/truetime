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

#ifndef LOG_STOP
#define LOG_STOP
#define simtime2time(time) ((time - rtsys->clockOffset) / rtsys->clockDrift)

// utility function also used in defaulthooks
void logstop(Log* log) {
  if (log) {
    if (log->entries < log->size) {
      // Subtract value written by logstart
      double r = simtime2time(rtsys->time) - log->vals[log->entries];
      log->vals[log->entries] = r;
      log->entries++;
    }
  }
}

void ttLogStop(const char *logname) {

  DataNode* dn;
  Log* log;

  dn = getNode(logname, rtsys->logList);
  if (dn == NULL) {
    // Log does not exist 
    char buf[MAXCHARS];
    sprintf(buf, "ttLogStop: Non-existent log '%s'!", logname);
    TT_MEX_ERROR(buf);
    return;
  }

  log = (Log*)dn->data;
  logstop(log);
}

#endif
