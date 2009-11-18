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



#ifndef STREAM_H
#define STREAM_H



#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <boost/thread.hpp>
#include <boost/bind.hpp>
#include <shout/shout.h>
#include <tag.h>
#include <fileref.h>
#include "playlist.h"
#include "streamconfig.h"
#include "log.h"



enum ErrorCode {OK, FILEERROR, EMPTY};



class cStream
{
public:
    //path to configdir, name of the stream, and pointer to cLog object
    cStream(std::string _ConfigDir, std::string _Name, cLog *mLog);

    ~cStream();

    //open and close stream
    int OpenStream();

    int CloseStream();

    //start and stop stream
    bool StartStream();

    void StopStream();

    std::string getName();

    bool getEnabled();

    bool IsOpen();

    bool IsRunning();

    bool IsEmpty();

private:
    cStream();

    void Play();

    int OpenFile();

    int CloseFile();

    int GetNext();

    int FillBuffer();

    int SendData();

    void ReadMetadata();

    int SendMetadata();

    cStreamConfig *mStreamConfig;

    shout_t *mStream;

    shout_metadata_t *mMetadata;

    cLog *mLog;

    cPlaylist *mPlaylist;

    unsigned char mStreambuffer[4096];

    std::string mFilename;

    FILE *mStreamfile;

    bool mNewFile;

    long mTotal;

    long mRead;

    std::string mPlaylistFilename;

    bool mStop;

    std::string mConfigDir;

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

    std::string mMeta_Artist;

    std::string mMeta_Title;

    std::string mMeta_Album;

    int mMeta_Year;

    std::string mMeta_Comment;

    int mMeta_Track;

    std::string mMeta_Genre;

    int mMeta_Bitrate;

    std::string mMeta_Samplerate;

    int mMeta_Channels;

    int mMeta_Length;

    bool mOpen;

    bool mRunning;

    bool mEmpty;
};



#endif
