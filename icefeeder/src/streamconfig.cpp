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



#include "streamconfig.h"



using std::string;
using std::stringstream;



cStreamConfig::cStreamConfig()
{
}



cStreamConfig::cStreamConfig(string _Name, string _ConfigDir, cLog *_Log) : mName(_Name), mConfigDir(_ConfigDir), mLog(_Log)
{
}



cStreamConfig::~cStreamConfig()
{
}



int cStreamConfig::LoadConfig()
{
    bool Error = false;

    //try to open configfile and read entries
    cConfigFile *Config = new cConfigFile(INI);

    string ConfigFile = mConfigDir;
    ConfigFile.append("/streams/");
    ConfigFile.append(mName);
    ConfigFile.append(".conf");

    if (!Config->Load(ConfigFile))
    {
        mLog->WriteLog(mName, "Could not load configfile", 1);
        return 1;
    }

    string Enabled = RemoveQuotes(Config->GetValue("stream", "enabled"));
    if (Enabled == "")
    {
        mLog->WriteLog(mName, "Could not read enabled in configfile", 1);
        Error = true;
    }
    else
    {
        mEnabled = StringToInt(Enabled);
    }

    if (!mEnabled)
    {
        mLog->WriteLog(mName, "Stream disabled", 1);
        return 1;
    }

    mHost = RemoveQuotes(Config->GetValue("stream", "host"));
    if (mHost == "")
    {
        mLog->WriteLog(mName, "Could not read hostname in configfile", 1);
        Error = true;
    }

    mUser = RemoveQuotes(Config->GetValue("stream", "username"));
    if (mUser == "")
    {
        mLog->WriteLog(mName, "Could not read username in configfile", 1);
        Error = true;
    }

    mPassword = RemoveQuotes(Config->GetValue("stream", "password"));
    if (mPassword == "")
    {
        mLog->WriteLog(mName, "Could not read password in configfile", 1);
        Error = true;
    }

    string Port = RemoveQuotes(Config->GetValue("stream", "port"));
    if (Port == "")
    {
        mLog->WriteLog(mName, "Could not read port in configfile", 1);
        Error = true;
    }
    else
    {
        mPort = StringToInt(Port);
    }

    mMountpoint = RemoveQuotes(Config->GetValue("stream", "mountpoint"));
    if (mMountpoint == "")
    {
        mLog->WriteLog(mName, "Could not read mountpoint in configfile", 1);
        Error = true;
    }

    string Protocol = RemoveQuotes(Config->GetValue("stream", "protocol"));
    if (Protocol == "")
    {
        mLog->WriteLog(mName, "Could not read protocol in configfile", 1);
        Error = true;
    }
    else
    {
        mProtocol = StringToInt(Protocol);
    }

    string Format = RemoveQuotes(Config->GetValue("stream", "format"));
    if (Format == "")
    {
        mLog->WriteLog(mName, "Could not read format in configfile", 1);
        Error = true;
    }
    else
    {
        mFormat = StringToInt(Format);
    }

    string Public = RemoveQuotes(Config->GetValue("stream", "public"));
    if (Public == "")
    {
        mLog->WriteLog(mName, "Could not read public in configfile", 1);
        Error = true;
    }
    else
    {
        mPublic = StringToInt(Public);
    }

    mUrl = RemoveQuotes(Config->GetValue("stream", "url"));
    if (mUrl == "")
    {
        mLog->WriteLog(mName, "Could not read url in configfile", 1);
        Error = true;
    }

    mGenre = RemoveQuotes(Config->GetValue("stream", "genre"));
    if (mGenre == "")
    {
        mLog->WriteLog(mName, "Could not read genre in configfile", 1);
        Error = true;
    }

    mDescription = RemoveQuotes(Config->GetValue("stream", "description"));
    if (mDescription == "")
    {
        mLog->WriteLog(mName, "Could not read description in configfile", 1);
        Error = true;
    }

    mBitrate = RemoveQuotes(Config->GetValue("stream", "bitrate"));
    if (mBitrate == "")
    {
        mLog->WriteLog(mName, "Could not read bitrate in configfile", 1);
        Error = true;
    }

    mSamplerate = RemoveQuotes(Config->GetValue("stream", "samplerate"));
    if (mSamplerate == "")
    {
        mLog->WriteLog(mName, "Could not read samplerate in configfile", 1);
        Error = true;
    }

    mChannels = RemoveQuotes(Config->GetValue("stream", "channels"));
    if (mChannels == "")
    {
        mLog->WriteLog(mName, "Could not read channels in configfile", 1);
        Error = true;
    }

    mQuality = RemoveQuotes(Config->GetValue("stream", "quality"));
    if (mQuality == "")
    {
        mLog->WriteLog(mName, "Could not read quality in configfile", 1);
        Error = true;
    }

    string Shuffle = RemoveQuotes(Config->GetValue("stream", "shuffle"));
    if (Shuffle == "")
    {
        mLog->WriteLog(mName, "Could not read shuffle in configfile", 1);
        Error = true;
    }
    else
    {
        mShuffle = StringToInt(Shuffle);
    }

    string Loop = RemoveQuotes(Config->GetValue("stream", "loop"));
    if (Loop == "")
    {
        mLog->WriteLog(mName, "Could not read loop in configfile", 1);
        Error = true;
    }
    else
    {
        mLoop = StringToInt(Loop);
    }

    if (!Config->GetNumberOfEntries("playlists"))
    {
        mLog->WriteLog(mName, "No playlist(s) in configfile", 1);
        Error = true;
    }

    //break on any error
    if (Error)
    {
        return 1;
    }

    //read playlist path from configfile
    Config->SetBegin();

    while (Config->GetHead() != "playlists")
    {
        Config->SetNext();
    }

    mPlaylistFilename = RemoveQuotes(Config->GetKeyword());

    return 0;
}



string cStreamConfig::getName()
{
    return mName;
}



bool cStreamConfig::getEnabled()
{
    return mEnabled;
}



string cStreamConfig::getHost()
{
    return mHost;
}



string cStreamConfig::getUser()
{
    return mUser;
}



string cStreamConfig::getPassword()
{
    return mPassword;
}



int cStreamConfig::getPort()
{
    return mPort;
}



string cStreamConfig::getMountpoint()
{
    return mMountpoint;
}



int cStreamConfig::getProtocol()
{
    return mProtocol;
}



int cStreamConfig::getFormat()
{
    return mFormat;
}



bool cStreamConfig::getPublic()
{
    return mPublic;
}



string cStreamConfig::getUrl()
{
    return mUrl;
}



string cStreamConfig::getGenre()
{
    return mGenre;
}



string cStreamConfig::getDescription()
{
    return mDescription;
}



string cStreamConfig::getBitrate()
{
    return mBitrate;
}



string cStreamConfig::getSamplerate()
{
    return mSamplerate;
}



string cStreamConfig::getChannels()
{
    return mChannels;
}



string cStreamConfig::getQuality()
{
    return mQuality;
}



bool cStreamConfig::getShuffle()
{
    return mShuffle;
}



bool cStreamConfig::getLoop()
{
    return mLoop;
}

string cStreamConfig::getPlaylistFilename()
{
    return mPlaylistFilename;
}



string cStreamConfig::RemoveQuotes(string _StringValue)
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



int cStreamConfig::StringToInt(string _StringValue)
{
    stringstream SStream(_StringValue);
    int IntValue;

    if (SStream >> IntValue)
    {
        return IntValue;
    }

    return -1;
}
