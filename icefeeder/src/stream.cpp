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



#include "stream.h"



using std::string;
using std::stringstream;



cStream::cStream()
{
}



cStream::cStream(string _ConfigDir, string _Name, cLog *_Log) : mConfigDir(_ConfigDir), mName(_Name), mLog(_Log),mStream(0), mStreamfile(0), mPublic(false), mStop(false), mOpen(false), mRunning(false), mEmpty(false)
{
    mStreamConfig = new cStreamConfig(mName, mConfigDir, mLog);
    OpenStream();
    shout_init();
    mMetadata = shout_metadata_new();
}



cStream::~cStream()
{
    mStop = true;
    boost::this_thread::sleep(boost::posix_time::seconds(1));
    CloseStream();

    delete mPlaylist;

    if (mStream)
    {
        shout_free(mStream);
    }

    shout_metadata_free(mMetadata);

    shout_shutdown();
}



int cStream::OpenStream()
{
    bool Error = false;

    //configfile readable?
    if (mStreamConfig->LoadConfig())
    {
        return 1;
    }

    mEnabled = mStreamConfig->getEnabled();

    if (mEnabled)
    {
        //set stream parameters
        if (!(mStream = shout_new()))
        {
            mLog->WriteLog(mName, "Could not allocate shout_t", 1);
            Error = true;
        }

        if (shout_set_name(mStream, mName.c_str()) != SHOUTERR_SUCCESS)
        {
            mLog->WriteLog(mName, "Error setting name", 1);
            Error = true;
        }

        mHost = mStreamConfig->getHost();
        if (shout_set_host(mStream, mHost.c_str()) != SHOUTERR_SUCCESS)
        {
            mLog->WriteLog(mName, "Error setting hostname", 1);
            Error = true;
        }

        mPort = mStreamConfig->getPort();
        if (shout_set_port(mStream, mPort) != SHOUTERR_SUCCESS)
        {
            mLog->WriteLog(mName, "Error setting port", 1);
            Error = true;
        }

        mUser = mStreamConfig->getUser();
        if (shout_set_user(mStream, mUser.c_str()) != SHOUTERR_SUCCESS)
        {
            mLog->WriteLog(mName, "Error setting user", 1);
            Error = true;
        }

        mPassword = mStreamConfig->getPassword();
        if (shout_set_password(mStream, mPassword.c_str()) != SHOUTERR_SUCCESS)
        {
            mLog->WriteLog(mName, "Error setting password", 1);
            Error = true;
        }

        mMountpoint = mStreamConfig->getMountpoint();
        if (shout_set_mount(mStream, mMountpoint.c_str()) != SHOUTERR_SUCCESS)
        {
            mLog->WriteLog(mName, "Error setting mountpoint", 1);
            Error = true;
        }

        mProtocol = mStreamConfig->getProtocol();
        if (shout_set_protocol(mStream, mProtocol) != SHOUTERR_SUCCESS)
        {
            mLog->WriteLog(mName, "Error setting protocol", 1);
            Error = true;
        }

        mFormat = mStreamConfig->getFormat();
        if (shout_set_format(mStream, mFormat) != SHOUTERR_SUCCESS)
        {
            mLog->WriteLog(mName, "Error setting format", 1);
            Error = true;
        }

        mPublic = mStreamConfig->getPublic();
        if (shout_set_public(mStream, mPublic) != SHOUTERR_SUCCESS)
        {
            mLog->WriteLog(mName, "Error setting public", 1);
            Error = true;
        }

        mUrl = mStreamConfig->getUrl();
        if (shout_set_url(mStream, mUrl.c_str()) != SHOUTERR_SUCCESS)
        {
            mLog->WriteLog(mName, "Error setting url", 1);
            Error = true;
        }

        mGenre = mStreamConfig->getGenre();
        if (shout_set_genre(mStream, mGenre.c_str()) != SHOUTERR_SUCCESS)
        {
            mLog->WriteLog(mName, "Error setting genre", 1);
            Error = true;
        }

        mDescription = mStreamConfig->getDescription();
        if (shout_set_description(mStream, mDescription.c_str()) != SHOUTERR_SUCCESS)
        {
            mLog->WriteLog(mName, "Error setting description", 1);
            Error = true;
        }

        mBitrate = mStreamConfig->getBitrate();
        if (shout_set_audio_info(mStream, SHOUT_AI_BITRATE, mBitrate.c_str()) != SHOUTERR_SUCCESS)
        {
            mLog->WriteLog(mName, "Error setting bitrate", 1);
            Error = true;
        }

        mSamplerate = mStreamConfig->getSamplerate();
        if (shout_set_audio_info(mStream, SHOUT_AI_SAMPLERATE, mSamplerate.c_str()) != SHOUTERR_SUCCESS)
        {
            mLog->WriteLog(mName, "Error setting samplerate", 1);
            Error = true;
        }

        mChannels = mStreamConfig->getChannels();
        if (shout_set_audio_info(mStream, SHOUT_AI_CHANNELS, mChannels.c_str()) != SHOUTERR_SUCCESS)
        {
            mLog->WriteLog(mName, "Error setting channels", 1);
            Error = true;
        }

        mQuality = mStreamConfig->getQuality();
        if (shout_set_audio_info(mStream, SHOUT_AI_QUALITY, mQuality.c_str()) != SHOUTERR_SUCCESS)
        {
            mLog->WriteLog(mName, "Error setting quality", 1);
            Error = true;
        }

        mShuffle = mStreamConfig->getShuffle();
        mLoop = mStreamConfig->getLoop();
        mPlaylist = new cPlaylist(mShuffle, mLoop);
        mPlaylistFilename = mStreamConfig->getPlaylistFilename();

        //try to open stream
        if (shout_open(mStream) == SHOUTERR_SUCCESS)
        {
            mLog->WriteLog(mName, "Connected to server", 2);
            mTotal = 0;
            mOpen = true;
            return 0;
        }

        mLog->WriteLog(mName, "Error connecting", 1);
        CloseStream();
        return 1;
    }

    return 0;
}



int cStream::CloseStream()
{
    if (!mOpen)
    {
        return 0;
    }

    if (shout_get_connected(mStream) == SHOUTERR_CONNECTED)
    {
        mLog->WriteLog(mName, "Disconnected from server", 2);
    }

    if (mRunning)
    {
        StopStream();
    }

    shout_close(mStream);
    mOpen = false;

    return 0;
}



bool cStream::StartStream()
{
    mStop = false;
    //the first title is a new one
    mNewFile = true;

    if (!mOpen)
    {
        return 1;
    }

    mPlaylist->OpenFile(mPlaylistFilename);

    //start playing in a new thread
    boost::thread StreamThread(boost::bind(&cStream::Play, this));

    if (mRunning)
    {
        mLog->WriteLog(mName, "Stream started", 1);
    }

    return 0;
}



void cStream::StopStream()
{
    //set stop variableto true - stream will be stopped with next read of variable
    mStop = true;
    mRunning = false;
    return;
}



string cStream::getName()
{
    return mName;
}



bool cStream::getEnabled()
{
    return mEnabled;
}



bool cStream::IsOpen()
{
    return mOpen;
}



bool cStream::IsRunning()
{
    return mRunning;
}



bool cStream::IsEmpty()
{
    return mEmpty;
}



void cStream::Play()
{
    //play until mStop variable is set to true
    while (!mStop && mOpen)
    {
        mRunning = true;

        switch (FillBuffer())
        {
        case OK:
            if (SendData())
            {
                mLog->WriteLog(mName, "Send error - closing connection...", 2);
                CloseStream();
                break;
            }

            continue;

        case EMPTY:
            //close stream if no titles left to play
            mLog->WriteLog(mName, "No more files to play", 2);
            mEmpty = true;
            CloseStream();
            break;
        }

    }

    mLog->WriteLog(mName, "Stream stopped", 1);
    mRunning = false;
    CloseStream();

    return;
}



int cStream::OpenFile()
{
    //close any open streamfiles
    if (mStreamfile)
    {
        CloseFile();
    }

    //get path of the next title
    if (GetNext())
    {
        mNewFile = false;
        return EMPTY;
    }

    //try to open the next title
    mStreamfile = fopen (mFilename.c_str(), "rb");

    if (!mStreamfile)
    {
        string Message = "Could not open file \"";
        Message.append(mFilename);
        Message.append("\"");
        mLog->WriteLog(mName, Message, 2);
        mNewFile = false;
        return FILEERROR;
    }

    //it is a new opened title
    mNewFile = true;
    return OK;
}



int cStream::CloseFile()
{
    fclose(mStreamfile);
    mStreamfile = 0;

    return 0;
}



int cStream::GetNext()
{
    //try to get path of the next title
    if (mPlaylist->getNext(mFilename))
    {
        return EMPTY;
    }

    return OK;
}



int cStream::FillBuffer()
{
    if (!mStreamfile)
    {
        while (true)
        {
            int FileError = OpenFile();

            //break if no error occured
            //open next if file is not readable
            //leave if there are no more files to play
            switch (FileError)
            {
            case OK:
                break;

            case FILEERROR:
                continue;

            case EMPTY:
                return EMPTY;

            }

            break;
        }

    }

    //fill buffer
    mRead = fread(mStreambuffer, 1, sizeof(mStreambuffer), mStreamfile);
    mTotal += mRead;

    //eof reached?
    if (feof(mStreamfile) != 0)
    {
        CloseFile();
    }

    return OK;
}



int cStream::SendData()
{
    long Ret;

    //send while buffer is filled
    while (mTotal)
    {
        //wait for server
        shout_sync(mStream);

        //if a new title is played - read and send metadata
        if (mNewFile && mRunning)
        {
            ReadMetadata();
            SendMetadata();

            string Message = "Playing file \"";
            Message.append(mFilename);
            Message.append("\"");
            mLog->WriteLog(mName, Message, 2);

            mNewFile = false;
        }

        //send buffer to server
        Ret = shout_send(mStream, mStreambuffer, mRead);
        mTotal -= mRead;

        //was sending successful?
        if (Ret != SHOUTERR_SUCCESS)
        {
            return 1;
        }

    }

    return 0;
}



void cStream::ReadMetadata()
{
    if (mFormat != 1)
    {
        return;
    }

    TagLib::FileRef File(mFilename.c_str());
    TagLib::Tag *Tag = File.tag();
    TagLib::AudioProperties *Properties = File.audioProperties();

    mMeta_Artist = Tag->artist().to8Bit();
    mMeta_Title = Tag->title().to8Bit();
    mMeta_Album = Tag->album().to8Bit();
    mMeta_Year = Tag->year();
    mMeta_Comment = Tag->comment().to8Bit();
    mMeta_Track = Tag->track();
    mMeta_Genre = Tag->genre().to8Bit();
    mMeta_Bitrate = Properties->bitrate();
    mMeta_Samplerate = Properties->sampleRate();
    mMeta_Channels = Properties->channels();
    mMeta_Length = Properties->length();

    return;
}



int cStream::SendMetadata()
{
    if (mFormat != 1)
    {
        return 1;
    }

    string Metadata;

    //set to "Unknown..." if no tag entry exists
    if (mMeta_Artist == "")
    {
        Metadata.append("Unknown Artist");
    }
    else
    {
        Metadata.append(mMeta_Artist);
    }

    Metadata.append(" - ");

    if (mMeta_Title == "")
    {
        Metadata.append("Unknown Title");
    }
    else
    {
        Metadata.append(mMeta_Title);
    }

    //only "song" supported by libshout
    shout_metadata_add(mMetadata, "song", Metadata.c_str());

    if (shout_set_metadata(mStream, mMetadata))
    {
        mLog->WriteLog(mName, "Could not send metadata", 2);
        shout_metadata_free(mMetadata);
        return 1;
    }

    return 0;
}
