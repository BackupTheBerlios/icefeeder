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



#ifndef PLAYLIST_H
#define PLAYLIST_H



#include <string>
#include <fstream>
#include <vector>
#include <time.h>
#include <boost/thread.hpp>
#include <boost/bind.hpp>



class cPlaylist
{
public:
    //shuffle or loop playlist?
    cPlaylist(bool _Shuffle = false, bool _Loop = false);

    ~cPlaylist();

    //open playlist file
    int OpenFile(std::string _Filename);

    //get the path of the next title
    int getNext(std::string& _NextTitle);

private:
    void HandleRandom(int _Random);

    int Parse();

    std::string mNext;

    std::vector<std::string> mPlaylist;

    std::vector<int> mPlaylistNumbers;

    std::vector<int>::iterator mIterator;

    int mPlaylistPos;

    bool mShuffle;

    bool mLoop;
};



#endif
