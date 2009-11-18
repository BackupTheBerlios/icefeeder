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



#include "icefeeder.h"



using std::cerr;
using std::string;
using std::stringstream;
using boost::ptr_vector;



int main(int argc, char *argv[])
{
    //open configuration file
    ConfigDir = "/var/icefeeder";
    cConfigFile Config(INI);

    string ConfigFile = ConfigDir;
    ConfigFile.append("/icefeeder.conf");

    if (!Config.Load(ConfigFile))
    {
        cerr << "icefeeder - could not load icefeeder.conf - exiting...\n";
        return 0;
    }

    //read path for logfile
    LogFile = RemoveQuotes(Config.GetValue("general", "logfile"));

    //read loglevel
    LogLevel = StringToInt(RemoveQuotes(Config.GetValue("general", "loglevel")));

    Log = new cLog(LogFile, LogLevel);

    Log->WriteLog("icefeeder", "started", 1);

    Retry = StringToInt(RemoveQuotes(Config.GetValue("general", "retry")));

    //how many streams are configured in config file?
    NumberOfStreams = Config.GetNumberOfEntries("streams");

    //find streams in configfile
    Config.SetBegin();

    for (int i = 0; i < NumberOfStreams; i++)
    {
        while (Config.GetHead() != "streams")
        {
            Config.SetNext();
        }

        Streams.push_back(new cStream(ConfigDir, RemoveQuotes(Config.GetKeyword()), Log));
        Config.SetNext();
    }

    for (int i = 0; i < NumberOfStreams; i++)
    {
        if (Streams[i].IsRunning())
        {
            continue;
        }

        //start if stream is enabled, remove if disabled
        if (Streams[i].getEnabled())
        {
            TryStart(i);
            boost::this_thread::sleep(boost::posix_time::seconds(1));

            if (!Streams[i].IsRunning())
            {
                Log->WriteLog(Streams[i].getName(), "could not start stream - skipping", 1);
                RemoveStream(i);
                i--;
                continue;
            }

            continue;
        }

        RemoveStream(i);
        i--;
    }

    //are there any streams running?
    while (NumberOfStreams)
    {
        for (int i = 0; i < NumberOfStreams; i++)
        {
            if (Streams[i].IsRunning())
            {
                continue;
            }

            if (Streams[i].getEnabled() && !Streams[i].IsEmpty())
            {
                TryStart(i);
            }

            if (!Streams[i].IsRunning() && Streams[i].getEnabled())
            {
                Log->WriteLog(Streams[i].getName(), "could not restart stream - skipping", 1);

                RemoveStream(i);
                i--;
            }

        }

    }

    Log->WriteLog("icefeeder", "no streams left - exiting...", 1);

    Log->WriteLog("icefeeder", "exited", 1);

    while (Log->getSize())
    {
        boost::this_thread::sleep(boost::posix_time::seconds(1));
    }

    delete Log;

    return 1;
}



//start streams, <Retry> attempts per stream if fail
void TryStart(int _StreamNumber)
{
    for (int i= 0; i < Retry + 1; i++)
    {
        if (!Streams[_StreamNumber].StartStream())
        {
            break;
        }

        Log->WriteLog(Streams[_StreamNumber].getName(), "could not start stream - retrying...", 1);

        Streams[_StreamNumber].CloseStream();
        Streams[_StreamNumber].OpenStream();
        Streams[_StreamNumber].StartStream();
    }

}



void RemoveStream(int _StreamNumber)
{
    //remove stream completely
    Streams[_StreamNumber].CloseStream();
    Streams.release(Streams.begin() + _StreamNumber);
    NumberOfStreams--;

    return;
}



int StringToInt(string _StringValue)
{
    stringstream SStream(_StringValue);
    int IntValue;

    if (SStream >> IntValue)
    {
        return IntValue;
    }

    return -1;
}



string RemoveQuotes(string _StringValue)
{
    //remove first and last character, if they are quotes
    if (_StringValue.substr(0,1) == "\"")
    {
        return _StringValue.substr(1, _StringValue.length() - 2);
    }
    else
    {
        return _StringValue;
    }

}
