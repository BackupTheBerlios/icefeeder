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



#include "playlist.h"



using std::string;
using std::vector;
using std::fstream;
using std::ios_base;



cPlaylist::cPlaylist(bool _Shuffle, bool _Loop) : mShuffle(_Shuffle), mLoop(_Loop)
{
    mPlaylist.clear();
}



cPlaylist::~cPlaylist()
{
}



//get the path to the next title
int cPlaylist::getNext(string& _NextTitle)
{
    if (mShuffle)
    {
        //stop if list ran empty and loop is not set
        if (mPlaylistNumbers.empty() && !mLoop)
        {
            _NextTitle = "";
            return 1;
        }

        //take a random title
        std::srand(static_cast<unsigned>(std::time(0)));
        int Random = (rand() % (mPlaylistNumbers.size()));

        _NextTitle = mPlaylist[mPlaylistNumbers[Random]];
        mPlaylistPos= mPlaylistNumbers[Random];

        //call in a own thread because of eventually long playlist operations
        boost::thread HandleRandomThread(boost::bind(&cPlaylist::HandleRandom, this, Random));

        return 0;
    }

    mPlaylistPos++;

    if (mPlaylistPos >= mPlaylist.size())
    {
        if (!mLoop)
        {
            mPlaylistPos = -1;
            _NextTitle = "";

            return 1;
        }

        mPlaylistPos = 0;
    }

    _NextTitle = mPlaylist[mPlaylistPos];

    return 0;
}



void cPlaylist::HandleRandom(int _Random)
{
    //remove played title from playlistnumbers
    mPlaylistNumbers.erase(mPlaylistNumbers.begin() + _Random);

    //refill playlistnumbers if ran empty
    if (mPlaylistNumbers.empty() && mLoop)
    {
        for (int i = 0; i < mPlaylist.size(); i++)
        {
            mPlaylistNumbers.push_back(i);
        }

    }

    return;
}



int cPlaylist::OpenFile(string _Filename)
{
    //try to open file
    fstream File;
    File.open(_Filename.c_str(), ios_base::in);

    if (!File)
    {
        return 1;
    }

    //read playlist file and fill playlist, then fill vector with playlistnumbers
    string TempLine;

    int i = 0;
    while (!File.eof())
    {
        getline(File, TempLine);

        if (TempLine == "")
        {
            break;
        }

        mPlaylist.push_back(TempLine);
        mPlaylistNumbers.push_back(i);
        i++;
    }

    File.close();

    mPlaylistPos = -1;

    return 0;
}



int cPlaylist::Parse()
{
}
