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



#include "log.h"



using std::string;
using std::cerr;
using std::fstream;
using std::ios;
using std::ios_base;



cLog::cLog()
{
}



cLog::cLog(string _LogFileName, int _LogLevel) : mLogFileName(_LogFileName), mLogLevel(_LogLevel), mEnabled(true), mStop(false)
{
    //start logging in a own thread
    boost::thread LogThread(boost::bind(&cLog::HandleQueue, this));

    return;
}



cLog::~cLog()
{
    //stop thread and wait a second before destruction
    mStop = true;
    boost::this_thread::sleep(boost::posix_time::seconds(1));

    mQueue_Name.clear();
    mQueue_Message.clear();
    mQueue_LogLevel.clear();

    if (mLogFile)
    {
        mLogFile.close();
    }

}



void cLog::WriteLog(std::string _Name, std::string _Message, int _LogLevel)
{
    mLogMutex.lock();
    mQueue_Name.push_back(_Name);
    mQueue_Message.push_back(_Message);
    mQueue_LogLevel.push_back(_LogLevel);
    mLogMutex.unlock();

    return;
}



int cLog::getSize()
{
    return mQueue_LogLevel.size();
}



void cLog::HandleQueue()
{
    //loop until mStop is set to true
    while (!mStop)
    {
        mLogMutex.lock();

        //if logqueue is filled handle first message
        if (!mQueue_LogLevel.empty())
        {
            if (mQueue_LogLevel[0] <= mLogLevel)
            {
                //build message
                string LogMessage = "stream \"";
                LogMessage.append(mQueue_Name[0]);
                LogMessage.append("\" - ");
                LogMessage.append(mQueue_Message[0]);
                LogMessage.append("\n");

                //log to standard error
                if (!mQueue_LogLevel[0])
                {
                    cerr << LogMessage;
                }

                //get systemtime
                boost::posix_time::ptime SystemTime = boost::posix_time::second_clock::local_time();

                //open and write logfile
                //open logfile
                mLogFile.open(mLogFileName.c_str(), ios::app | ios_base::out);

                if (!mLogFile)
                {
                    cerr << SystemTime << " - icefeeder - could not write logfile - logging to file disabled\n";
                    mEnabled = false;
                }

                if (mLogFile)
                {
                    mLogFile << SystemTime << " - " << LogMessage;
                }

                mLogFile.close();
            }

            //delete the first message
            mQueue_Name.erase(mQueue_Name.begin());
            mQueue_Message.erase(mQueue_Message.begin());
            mQueue_LogLevel.erase(mQueue_LogLevel.begin());
        }

        mLogMutex.unlock();
    }

    return;
}
