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



#ifndef LOG_H
#define LOG_H



#include <string>
#include <iostream>
#include <fstream>
#include <vector>
#include <unistd.h>
#include <boost/thread.hpp>
#include <boost/bind.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>



class cLog
{
public:
    //give path to logfile and set the loglevel
    cLog(std::string _LogFileName, int _LogLevel = 2);

    ~cLog();

    //add a message to logqueue - will be executey as resources are free
    //_Name will be displayed first followed by " - " and the mesage
    //set the loglevel of the message
    void WriteLog(std::string _Name, std::string _Message, int _LogLevel);

    //number of messages i queue
    int getSize();

private:
    cLog();

    void HandleQueue();

    std::string mLogFileName;

    int mLogLevel;

    std::vector<std::string> mQueue_Name;

    std::vector<std::string> mQueue_Message;

    std::vector<int> mQueue_LogLevel;

    boost::mutex mLogMutex;

    std::fstream mLogFile;

    bool mEnabled;

    bool mStop;
};



#endif
