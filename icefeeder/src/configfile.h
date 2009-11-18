/***************************************************************************
 *   Copyright (C) 2008 by Thomas Maass                                    *
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



#ifndef cCONFIGFILE_H_
#define cCONFIGFILE_H_



#include <string>
#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>



enum FType {CONFIG, INI};

class cConfigEntry;



class cConfigFile
{
public:

    //Filetype is one of CONFIG or INI.
    cConfigFile(FType _FileType);

    ~cConfigFile()
    {}

    //Load or save a configfile with the specified full filepath.
    bool Load(std::string _FilePath);
    bool Save(std::string _FilePath);

    //Get position number of an entry from beginning or end of a
    //header, or whole list beginning with 1. Returns 0, if not found.
    //Header-lines are not counted.
    int GetEntryPosition(std::string _Head, std::string _Keyword) const;
    int GetEntryPosition(std::string _Keyword) const;
    int GetFirstPosition(std::string _Head) const;
    int GetLastPosition(std::string _Head) const;

    //Get number of entries of one header, or the whole list.
    int GetNumberOfEntries(std::string _Head) const;
    int GetNumberOfEntries() const;

    //Set the iterator to the first or last position of the list.
    bool SetBegin();
    bool SetEnd();

    //Set the iterator to a specific position,
    //or move to next or previous entry.
    bool SetPosition(int _Pos);
    bool SetNext();
    bool SetPrev();

    //Returns true, if list is empty.
    bool IsEmpty() const;

    //Get elements from the current iterator position.
    //The position of the values begins with 1.
    std::string GetHead() const;
    std::string GetKeyword() const;
    std::string GetValue(int _Pos) const;

    //Get elements from the specified header and keyword for ini,
    //or keyword for config.
    //The position of the values begins with 1.
    std::string GetValue(std::string _Head, std::string _Keyword, int _Pos = 1);
    std::string GetValue(std::string _Keyword, int _Pos = 1);

    //Get number of values from the current iterator position.
    int GetNumberOfValues() const;

    //add or edit entries
    //for line enter the keyword and values separated by whitespaces
    bool PushBack(std::string _Head, std::string _Line);
    bool PushBack(std::string _Line);
    bool PushFront(std::string _Head, std::string _Line);
    bool PushFront(std::string _Line);
    bool PushPosition(int _Pos, std::string _Line);
    bool EditEntry(int _Pos, std::string _Line);

    //delete entries
    bool DeleteAll();
    bool DeleteAll(std::string _Head);
    bool DeletePosition(int _Pos);
    bool DeletePosition(int _StartPos, int _EndPos);

    //Swap or move entries. Header-lines are ignored.
    //_Up and _Down are the number of lines to move up or down.
    bool SwapEntry(int _Pos1, int _Pos2);
    bool MoveUp(int _SourcePosStart, int _SourcePosEnd, int _Up);
    bool MoveDown(int _SourcePosStart, int _SourcePosEnd, int _Down);
    bool MoveBegin(int _SourcePosStart, int _SourcePosEnd);
    bool MoveEnd(int _SourcePosStart, int _SourcePosEnd);

    //sort entries, either only in one header, or the complete list
    void SortList(std::string _Head);
    void SortList();

    //undo changes
    void Undo();



private:

    cConfigFile()
    {}

    FType m_FileType;

    std::string m_FilePath;

    std::string m_Head;

    std::vector<cConfigEntry> m_Configuration;

    std::vector<cConfigEntry> m_UndoList;

    std::vector<cConfigEntry>::iterator m_Iter;

    cConfigEntry* FindEntries(std::string _Word);

    std::string DelLeadingWhitespaces(std::string _Word);

    std::string DelTrailingWhitespaces(std::string _Word);

    std::string FindHead(std::string _Word);

    bool FindQuotes(std::string _Word, std::string _Quote, std::string::size_type& _LocQuote1, std::string::size_type& _LocQuote2, int _Start=0);

    std::string GetWord(std::string _Word, int _Loc1, int _Loc2=std::string::npos);

    void SaveUndoData();
};






class cConfigEntry
{
public:

    ~cConfigEntry()
    {}



private:

    cConfigEntry()
    {}

    void SetHead(std::string _Head);

    std::string GetHead() const;

    void SetKeyword(std::string _Keyword);

    std::string GetKeyword() const;

    void SetValue(std::string _Value);

    std::string GetValue(int _Pos) const;

    int GetNumberOfValues() const;

    std::vector<std::string> m_Value;

    std::string m_Head;

    std::string m_Keyword;

    friend bool operator < (const cConfigEntry& _Entry1, const cConfigEntry& _Entry2);

    friend class cConfigFile;
};



#endif /*cCONFIGFILE_H_*/
