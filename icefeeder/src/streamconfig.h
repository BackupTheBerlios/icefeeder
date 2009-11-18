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



#ifndef STREAMCONFIG_H
#define STREAMCONFIG_H



#include <string>
#include <sstream>
#include "configfile.h"
#include "log.h"



class cStreamConfig
{
public:
    cStreamConfig(std::string _Name, std::string _ConfigDir, cLog *_Log);

    ~cStreamConfig();

    int LoadConfig();

    std::string getName();

    bool getEnabled();

    std::string getHost();

    std::string getUser();

    std::string getPassword();

    int getPort();

    std::string getMountpoint();

    int getProtocol();

    int getFormat();

    bool getPublic();

    std::string getUrl();

    std::string getGenre();

    std::string getDescription();

    std::string getBitrate();

    std::string getSamplerate();

    std::string getChannels();

    std::string getQuality();

    bool getShuffle();

    bool getLoop();

    std::string getPlaylistFilename();

private:
    cStreamConfig();

    std::string RemoveQuotes(std::string _StringValue);

    int StringToInt(std::string _StringValue);

    std::string mConfigDir;

    cLog *mLog;

    std::string mName;

    bool mEnabled;

    std::string mHost;

    std::string mUser;

    std::string mPassword;

    int mPort;

    std::string mMountpoint;

    int mProtocol;

    int mFormat;

    bool mPublic;

    std::string mUrl;

    std::string mGenre;

    std::string mDescription;

    std::string mBitrate;

    std::string mSamplerate;

    std::string mChannels;

    std::string mQuality;

    bool mShuffle;

    bool mLoop;

    std::string mPlaylistFilename;
};



#endif
