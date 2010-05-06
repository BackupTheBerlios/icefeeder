/***************************************************************************
 *   Copyright (C) 2009 by Thomas Maass                                    *
 *   inyourface@gmx.de                                                     *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/



#ifndef ICEFEEDER_H
#define ICEFEEDER_H



#include <iostream>
#include <string>
#include <sstream>
#include <unistd.h>
#include <boost/thread.hpp>
#include <boost/ptr_container/ptr_vector.hpp>
#include "stream.h"
#include "log.h"
#include "configfile.h"



int StringToInt(std::string _StringValue);

std::string RemoveQuotes(std::string _StringValue);

void TryStart(int _StreamNumber);

void RemoveStream(int _StreamNumber);

std::string ConfigDir;

std::string LogFile;

int LogLevel;

cLog *Log;

boost::ptr_vector<cStream> Streams;

int Retry;

int NumberOfStreams;



#endif
