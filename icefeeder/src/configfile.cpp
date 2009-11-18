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



#include "configfile.h"



using std::string;
using std::fstream;
using std::vector;
using std::ios_base;
using std::iter_swap;
using std::sort;



cConfigFile::cConfigFile(FType _FileType) : m_FileType(_FileType)
{
    m_Configuration.clear();
}



bool cConfigFile::Load(string _FilePath)
{
    //try to open file
    m_FilePath = _FilePath;
    fstream File;
    File.open(m_FilePath.c_str(), ios_base::in);

    if (!File)
    {
        return false;
    }

    m_Head = "";

    //read lines till eof
    string TempLine;

    while (!File.eof())
    {
        getline(File, TempLine);

        //remove whitespaces
        TempLine = DelLeadingWhitespaces(TempLine);
        TempLine = DelTrailingWhitespaces(TempLine);

        //skip comment lines
        if (!TempLine.find_first_of("#",0))
        {
            continue;
        }

        //skip empty lines
        if (TempLine.empty())
        {
            continue;
        }

        cConfigEntry *ConfigEntry = FindEntries(TempLine);

        if (!TempLine.empty())
        {
            //insert entry into list if entry is not empty
            if (ConfigEntry->GetKeyword() != "")
            {
                m_Configuration.push_back(*ConfigEntry);
            }
        }

        delete ConfigEntry;
    }

    File.close();

    return true;
}



bool cConfigFile::Save(string _FilePath)
{
    //open file
    m_FilePath = _FilePath;
    fstream File;
    File.open(m_FilePath.c_str(), ios_base::out);
    m_Head = "";

    //run from begin to end of the configuration
    for (m_Iter = m_Configuration.begin(); m_Iter != m_Configuration.end(); m_Iter++)
    {
        //write header and keyword for ini, or only keyword for config
        if (m_FileType == INI)
        {
            //find out, if header changed
            string NextHead = m_Iter->GetHead();

            if (m_Head != NextHead && NextHead != "")
            {
                m_Head = NextHead;

                if (m_Iter != m_Configuration.begin())
                {
                    File << "\n";
                }

                File << "[" << m_Head << "]\n";
            }

        }

        File << m_Iter->GetKeyword();

        //write values
        for (int i = 0; i < m_Iter->GetNumberOfValues(); i++)
        {
            File << "   " << m_Iter->GetValue(i);
        }

        File << "\n";
    }

    File.close();

    return true;
}



int cConfigFile::GetEntryPosition(string _Head, string _Keyword) const
{
    for (int Pos = 0; Pos < m_Configuration.size(); Pos++)
    {
        if (m_Configuration[Pos].GetHead() == _Head && m_Configuration[Pos].GetKeyword() == _Keyword)
        {
            return Pos + 1;
        }

    }

    return 0;
}



int cConfigFile::GetEntryPosition(string _Keyword) const
{
    for (int Pos = 0; Pos < m_Configuration.size(); Pos++)
    {
        if (m_Configuration[Pos].GetKeyword() == _Keyword)
        {
            return Pos + 1;
        }

    }

    return 0;
}



int cConfigFile::GetFirstPosition(string _Head) const
{
    for (int Pos = 0; Pos < m_Configuration.size(); Pos++)
    {
        if (m_Configuration[Pos].GetHead() == _Head)
        {
            return Pos + 1;
        }
    }

    return 0;
}



int cConfigFile::GetLastPosition(string _Head) const
{
    for (int Pos = m_Configuration.size() - 1; Pos > 0; Pos--)
    {
        if (m_Configuration[Pos].GetHead() == _Head)
        {
            return Pos + 1;
        }

    }

    return 0;
}



int cConfigFile::GetNumberOfEntries(string _Head) const
{
    int Number = 0;

    for (int Pos = 0; Pos < m_Configuration.size(); Pos++)
    {
        if (m_Configuration[Pos].GetHead() == _Head)
        {
            Number++;
        }

    }

    return Number;
}



int cConfigFile::GetNumberOfEntries() const
{
    return m_Configuration.size();
}



bool cConfigFile::SetBegin()
{
    if (m_Configuration.empty())
    {
        return false;
    }

    m_Iter = m_Configuration.begin();

    return true;
}



bool cConfigFile::SetEnd()
{
    if (m_Configuration.empty())
    {
        return false;
    }

    m_Iter = --m_Configuration.end();

    return true;
}



bool cConfigFile::SetPosition(int _Pos)
{
    if (_Pos > m_Configuration.size() || !_Pos)
    {
        return false;
    }

    m_Iter = m_Configuration.begin() + _Pos - 1;

    return true;
}



bool cConfigFile::SetNext()
{
    if (m_Iter + 1 == m_Configuration.end())
    {
        return false;
    }

    m_Iter++;

    return true;
}



bool cConfigFile::SetPrev()
{
    if (m_Iter == m_Configuration.begin())
    {
        return false;
    }

    m_Iter--;

    return true;
}



bool cConfigFile::IsEmpty() const
{
    return m_Configuration.empty();
}



string cConfigFile::GetHead() const
{
    return m_Iter->GetHead();
}



string cConfigFile::GetKeyword() const
{
    return m_Iter->GetKeyword();
}



string cConfigFile::GetValue(int _Pos) const
{
    if (_Pos == 0)
    {
        return "";
    }

    if (_Pos > m_Iter->GetNumberOfValues())
    {
        return "";
    }

    return m_Iter->GetValue(_Pos - 1);
}



string cConfigFile::GetValue(string _Head, string _Keyword, int _Pos)
{
    for (int Pos = 0; Pos < m_Configuration.size(); Pos++)
    {
        if (m_Configuration[Pos].GetHead() == _Head && m_Configuration[Pos].GetKeyword() == _Keyword)
        {
            if (_Pos > m_Configuration[Pos].GetNumberOfValues())
            {
                return "";
            }

            return m_Configuration[Pos].GetValue(_Pos - 1);
        }

    }

    return "";
}



string cConfigFile::GetValue(string _Keyword, int _Pos)
{
    for (int Pos = 0; Pos < m_Configuration.size(); Pos++)
    {
        if (m_Configuration[Pos].GetKeyword() == _Keyword)
        {
            if (_Pos > m_Configuration[Pos].GetNumberOfValues())
            {
                return "";
            }

            return m_Configuration[Pos].GetValue(_Pos - 1);
        }

    }

    return "";
}



int cConfigFile::GetNumberOfValues() const
{
    return m_Iter->GetNumberOfValues();
}



bool cConfigFile::PushBack(string _Head, string _Line)
{
    if (m_FileType == INI && _Head == "")
    {
        return false;
    }

    cConfigEntry *TempEntry = (FindEntries(_Line));

    if (TempEntry->GetKeyword() == "")
    {
        return false;
    }

    SaveUndoData();

    if (m_FileType != INI)
    {
        TempEntry->SetHead("");
    }
    else
    {
        TempEntry->SetHead(_Head);
    }

    //check, if head already exists
    for (m_Iter = m_Configuration.begin(); m_Iter != m_Configuration.end(); m_Iter++)
    {
        if (m_Iter->GetHead() == _Head)
        {
            //check, if head changed
            if ((m_Iter + 1) != m_Configuration.end() && (m_Iter + 1)->GetHead() != _Head)
            {
                m_Configuration.insert((m_Iter + 1), *TempEntry);
                return true;
            }

        }

    }

    //insert with new head at the end of the list, if head doesn't already exist
    m_Configuration.push_back(*TempEntry);

    return true;
}



bool cConfigFile::PushBack(string _Line)
{
    cConfigEntry *TempEntry = (FindEntries(_Line));

    if (TempEntry->GetKeyword() == "")
    {
        return false;
    }

    //set the same header than the last entry has
    if (m_Configuration.empty())
    {
        TempEntry->SetHead("");
    }
    else
    {
        TempEntry->SetHead((m_Configuration.end() - 1)->GetHead());
    }

    SaveUndoData();

    m_Configuration.push_back(*TempEntry);

    return true;
}



bool cConfigFile::PushFront(string _Head, string _Line)
{
    if (m_FileType == INI && _Head == "")
    {
        return false;
    }

    cConfigEntry *TempEntry = (FindEntries(_Line));

    if (TempEntry->GetKeyword() == "")
    {
        return false;
    }

    SaveUndoData();

    if (m_FileType != INI)
    {
        TempEntry->SetHead("");
    }
    else
    {
        TempEntry->SetHead(_Head);
    }

    //check, if head already exists
    if (!m_Configuration.empty())
    {
        for (m_Iter = m_Configuration.begin(); m_Iter != m_Configuration.end(); m_Iter++)
        {
            if (m_Iter->GetHead() == _Head)
            {
                m_Configuration.insert(m_Iter, *TempEntry);
                return true;
            }

        }

    }

    //insert with new header at the beginning of the list, if head doesn't already exist
    m_Configuration.insert(m_Configuration.begin(), *TempEntry);

    return true;
}



bool cConfigFile::PushFront(string _Line)
{
    cConfigEntry *TempEntry = FindEntries(_Line);

    if (TempEntry->GetKeyword() == "")
    {
        return false;
    }

    //set the same header than the first entry has
    if (m_Configuration.empty())
    {
        TempEntry->SetHead("");
    }
    else
    {
        TempEntry->SetHead((m_Configuration.begin())->GetHead());
    }

    SaveUndoData();

    m_Configuration.insert(m_Configuration.begin(), *TempEntry);

    return true;
}



bool cConfigFile::PushPosition(int _Pos, std::string _Line)
{
    if (_Pos > m_Configuration.size() || _Pos == 0)
    {
        return false;
    }

    SaveUndoData();

    cConfigEntry *TempEntry = FindEntries(_Line);

    //set the same header than the previous entry has
    if (m_Configuration.empty())
    {
        TempEntry->SetHead("");
    }
    else
    {
        TempEntry->SetHead(m_Configuration[_Pos - 1].GetHead());
    }

    m_Configuration.insert(m_Configuration.begin() + _Pos - 1, *TempEntry);

    return true;
}



bool cConfigFile::EditEntry(int _Pos, string _Line)
{
    if (m_Configuration.empty() || _Pos > m_Configuration.size() || _Pos == 0)
    {
        return false;
    }

    SaveUndoData();

    m_Head = m_Configuration[_Pos - 1].GetHead();

    cConfigEntry *TempEntry = FindEntries(_Line);

    //keep the header
    TempEntry->SetHead(m_Configuration[_Pos - 1].GetHead());

    //replace the old entry with the new one
    m_Configuration.erase(m_Configuration.begin() + _Pos - 1);
    m_Configuration.insert(m_Configuration.begin() + _Pos - 1, *TempEntry);

    return true;
}



bool cConfigFile::DeleteAll()
{
    SaveUndoData();

    m_Configuration.clear();

    return true;
}



bool cConfigFile::DeleteAll(string _Head)
{
    if (m_Configuration.empty())
    {
        return false;
    }

    SaveUndoData();

    //find out the first and last potition of the given header
    int Start = GetFirstPosition(_Head);
    int End = GetLastPosition(_Head);

    if (!Start || !End)
    {
        return false;
    }

    //delete all entries with the given header
    if (Start == End)
    {
        m_Configuration.erase(m_Configuration.begin() + Start - 1);
    }
    else
    {
        m_Configuration.erase(m_Configuration.begin() + Start - 1, m_Configuration.begin() + End);
    }

    return true;
}



bool cConfigFile::DeletePosition(int _Pos)
{
    if (_Pos == 0 || _Pos > m_Configuration.size())
    {
        return false;
    }

    SaveUndoData();

    m_Configuration.erase(m_Configuration.begin() + _Pos - 1);

    return true;
}



bool cConfigFile::DeletePosition(int _StartPos, int _EndPos)
{
    if (_StartPos == 0 || _EndPos == 0 || _StartPos > _EndPos)
    {
        return false;
    }

    if (_StartPos > m_Configuration.size() || _EndPos > m_Configuration.size())
    {
        return false;
    }

    SaveUndoData();

    m_Configuration.erase(m_Configuration.begin() + _StartPos - 1, m_Configuration.begin() + _EndPos);

    return true;
}



bool cConfigFile::SwapEntry(int _Pos1, int _Pos2)
{
    if (_Pos1 > m_Configuration.size() || _Pos2 > m_Configuration.size())
    {
        return false;
    }

    if (_Pos1 > m_Configuration.size() || _Pos2 > m_Configuration.size())
    {
        return false;
    }

    SaveUndoData();

    vector<cConfigEntry>::iterator FirstIter = m_Configuration.begin() + _Pos1 - 1;
    vector<cConfigEntry>::iterator SecondIter= m_Configuration.begin() + _Pos2 - 1;

    string TempHead = FirstIter->GetHead();
    FirstIter->SetHead(SecondIter->GetHead());
    SecondIter->SetHead(TempHead);
    iter_swap(FirstIter, SecondIter);

    m_Iter = m_Configuration.begin();

    return true;
}



bool cConfigFile::MoveUp(int _SourcePosStart, int _SourcePosEnd, int _Up)
{
    if (!_SourcePosStart || !_SourcePosEnd || !_Up)
    {
        return false;
    }

    if (_SourcePosStart > _SourcePosEnd)
    {
        return false;
    }

    if (_SourcePosStart > m_Configuration.size() || _SourcePosEnd > m_Configuration.size())
    {
        return false;
    }

    if (_SourcePosStart < _Up)
    {
        return false;
    }

    if (_SourcePosStart - _Up < GetFirstPosition(m_Configuration[_SourcePosStart - 1 - _Up].GetHead()))
    {
        return false;
    }

    SaveUndoData();

    vector <cConfigEntry> TempList;

    for (int i = 0 + _SourcePosStart - 1 - _Up; i != _SourcePosStart - 1; i++)
    {
        TempList.push_back(m_Configuration[i]);
    }

    m_Configuration.erase(m_Configuration.begin() + _SourcePosStart - 1 - _Up, m_Configuration.begin() + _SourcePosStart - 1);

    m_Configuration.insert(m_Configuration.begin() + _SourcePosEnd - _Up, TempList.begin(), TempList.end());

    return true;
}



bool cConfigFile::MoveDown(int _SourcePosStart, int _SourcePosEnd, int _Down)
{
    if (!_SourcePosStart || !_SourcePosEnd || !_Down)
    {
        return false;
    }

    if (_SourcePosStart > _SourcePosEnd)
    {
        return false;
    }

    if (_SourcePosStart > m_Configuration.size() || _SourcePosEnd > m_Configuration.size())
    {
        return false;
    }

    if (_SourcePosEnd + _Down > GetLastPosition(m_Configuration[_SourcePosStart - 1].GetHead()))
    {
        return false;
    }

    SaveUndoData();

    vector <cConfigEntry> TempList;

    for (int i = _SourcePosEnd; i != _SourcePosEnd + _Down; i++)
    {
        TempList.push_back(m_Configuration[i]);
    }

    m_Configuration.erase(m_Configuration.begin() + _SourcePosEnd, m_Configuration.begin() + _SourcePosEnd + _Down);

    m_Configuration.insert(m_Configuration.begin() + _SourcePosStart - 1, TempList.begin(), TempList.end());

    return true;
}



bool cConfigFile::MoveBegin(int _SourcePosStart, int _SourcePosEnd)
{
    if (!_SourcePosStart || !_SourcePosEnd)
    {
        return false;
    }

    if (_SourcePosStart > m_Configuration.size() || _SourcePosEnd > m_Configuration.size())
    {
        return false;
    }

    int FirstPosition = GetFirstPosition(m_Configuration[_SourcePosStart - 1].GetHead());

    if (_SourcePosStart == FirstPosition)
    {
        return false;
    }

    SaveUndoData();

    if (MoveUp(_SourcePosStart, _SourcePosEnd, _SourcePosStart - FirstPosition))
    {
        return true;
    }

    return false;
}



bool cConfigFile::MoveEnd(int _SourcePosStart, int _SourcePosEnd)
{
    if (!_SourcePosStart || !_SourcePosEnd)
    {
        return false;
    }

    if (_SourcePosStart > m_Configuration.size() || _SourcePosEnd > m_Configuration.size())
    {
        return false;
    }

    int LastPosition = GetLastPosition(m_Configuration[_SourcePosStart - 1].GetHead());

    if (_SourcePosEnd == LastPosition)
    {
        return false;
    }

    SaveUndoData();

    if (MoveDown(_SourcePosStart, _SourcePosEnd, LastPosition - _SourcePosEnd))
    {
        return true;
    }

    return false;
}



void cConfigFile::SortList(string _Head)
{
    if (m_Configuration.empty())
    {
        return;
    }


    SaveUndoData();

    sort(m_Configuration.begin() + GetFirstPosition(_Head) - 1, m_Configuration.begin() + GetLastPosition(_Head) -1);
}



void cConfigFile::SortList()
{
    SaveUndoData();

    sort(m_Configuration.begin(), m_Configuration.end());
}



void cConfigFile::Undo()
{
    if (!m_UndoList.empty())
    {
        m_Configuration.clear();

        for (int i = 0; i != m_UndoList.size(); i++)
        {
            m_Configuration.push_back(m_UndoList[i]);
        }

    }

}



//find entries and fill list
cConfigEntry* cConfigFile::FindEntries(string _Word)
{
    cConfigEntry* ConfigEntry = new cConfigEntry;
    string::size_type LocQuote1 = string::npos;
    string::size_type LocQuote2 = string::npos;
    string::size_type LocSeparator = string::npos;
    string::size_type Pos = 0;
    string::size_type StartPos = 0;
    int EntryCounter = 0;

    //delete leading whitespaces
    _Word = DelLeadingWhitespaces(_Word);

    do
    {
        //find "
        FindQuotes(_Word, "\"", LocQuote1, LocQuote2, Pos);

        if (m_FileType == INI && LocQuote1 == string::npos)
        {
            //find []
            FindQuotes(_Word, "[", LocQuote1, LocQuote2, Pos);
        }

        //throw out an error and erase all entries, if no trailing quote present
        if (LocQuote1 != string::npos && LocQuote2 == string::npos)
        {
            ConfigEntry->SetHead("");
            ConfigEntry->SetKeyword("");
            return ConfigEntry;
        }

        //find next unquoted separator
        do
        {
            LocSeparator = _Word.find_first_of(" \t", Pos);
            Pos = LocSeparator +1;
        }
        while (LocQuote1 < LocSeparator && LocQuote2 > LocSeparator && LocSeparator != string::npos);

        Pos = _Word.find_first_not_of(" \t", Pos);

        EntryCounter++;

        //write the word to the right position, depending on filetype (ini or config)
        if (m_FileType == INI)
        {
            string NextHead = FindHead(GetWord(_Word, StartPos, LocSeparator));

            if (EntryCounter == 1)
            {
                if (m_Head != NextHead && NextHead != "")
                {
                    m_Head = NextHead;

                    //prepare to search for the next entry
                    StartPos = _Word.find_first_not_of(" \t", LocSeparator);
                    Pos = _Word.find_first_not_of(" ", Pos);
                    LocQuote1 = string::npos;
                    LocQuote2 = string::npos;

                    //break, if end of line is reached
                    if (Pos -1 >= _Word.size())
                    {
                        break;
                    }

                    continue;
                }

                ConfigEntry->SetHead(m_Head);
            }

            if (EntryCounter == 1 && NextHead == "")
            {
                ConfigEntry->SetKeyword(GetWord(_Word, StartPos, LocSeparator));
            }

            if (EntryCounter > 1 && NextHead == "")
            {
                ConfigEntry->SetValue(GetWord(_Word, StartPos, LocSeparator));
            }

        }


        if (m_FileType == CONFIG)
        {
            if (EntryCounter == 1)
            {
                ConfigEntry->SetHead("");
                ConfigEntry->SetKeyword(GetWord(_Word, StartPos, LocSeparator));
            }

            if (EntryCounter > 1)
            {
                ConfigEntry->SetValue(GetWord(_Word, StartPos, LocSeparator));
            }

        }

        //prepare to search for the next entry
        StartPos = _Word.find_first_not_of(" \t", LocSeparator);
        Pos = _Word.find_first_not_of(" ", Pos);
        LocQuote1 = string::npos;
        LocQuote2 = string::npos;

        //break, if end of line is reached
        if (Pos -1 >= _Word.size())
        {
            break;
        }

    }
    while (LocSeparator != string::npos);

    return ConfigEntry;
}



//delete leading whitespaces
string cConfigFile::DelLeadingWhitespaces(string _Word)
{
    //find first position containing no whitespace
    int Pos = _Word.find_first_not_of(" \t");

    //remove leading whitespaces, if there are any
    if (Pos != string::npos)
    {
        _Word.erase(0, Pos);
    }
    else
    {
        _Word.clear();
    }

    return _Word;
}



//delete trailing whitespaces
string cConfigFile::DelTrailingWhitespaces(string _Word)
{
    //find last position containing no whitespace
    int Pos = _Word.find_last_not_of(" \t");

    //remove trailing whitespaces, if there are any
    if (Pos != string::npos)
    {
        _Word.erase(Pos +1);
    }
    else
    {
        _Word.clear();
    }

    return _Word;
}



//check, if word is in header format, return word, or an empty string, if no header
string cConfigFile::FindHead(string _Word)
{
    if (_Word.find_first_of("[") == string::npos || _Word.find_last_of("]") == string::npos)
    {
        return "";
    }

    return GetWord(_Word, 1, _Word.find_last_of("]"));
}



//find quotes, return false, if only one or no quotes were found
bool cConfigFile::FindQuotes(string _Word, string _Quote, string::size_type& _LocQuote1, string::size_type& _LocQuote2, int _Start)
{
    //search for the first quote
    _LocQuote1 = _Word.find(_Quote, _Start);

    if (_LocQuote1 == string::npos)
    {
        return false;
    }

    //if filetype is ini, search for a "]" if "[" was found first
    if (m_FileType == INI && _Quote == "[")
    {
        _Quote = "]";
    }

    _LocQuote2 = _Word.find(_Quote, _LocQuote1 +1);

    if (_LocQuote2 != string::npos)
    {
        return true;
    }

    return false;
}



//get a word between 2 positions
string cConfigFile::GetWord(string _Word, int _Loc1, int _Loc2)
{
    return _Word.substr(_Loc1, _Loc2 - _Loc1);
}



//save undo data
void cConfigFile::SaveUndoData()
{
    if (!m_Configuration.empty())
    {
        m_UndoList.clear();

        for (int i = 0; i != m_Configuration.size(); i++)
        {
            m_UndoList.push_back(m_Configuration[i]);
        }

    }

}






void cConfigEntry::SetHead(string _Head)
{
    m_Head = _Head;
    return;
}



string cConfigEntry::GetHead() const
{
    return m_Head;
}



void cConfigEntry::SetKeyword(string _Keyword)
{
    m_Keyword = _Keyword;

    return;
}



string cConfigEntry::GetKeyword() const
{
    return m_Keyword;
}



void cConfigEntry::SetValue(string _Value)
{
    m_Value.push_back(_Value);

    return;
}



string cConfigEntry::GetValue(int _Pos) const
{
    //return an empty sting, if value does not exist
    if (_Pos + 1 > m_Value.size())
    {
        return "";
    }
    else
    {
        return m_Value[_Pos];
    }
}



int cConfigEntry::GetNumberOfValues() const
{
    return m_Value.size();
}



bool operator < (const cConfigEntry& _Entry1, const cConfigEntry& _Entry2)
{
    if (_Entry1.m_Head < _Entry2.m_Head)
    {
        return true;
    }

    if (_Entry1.m_Head == _Entry2.m_Head && _Entry1.m_Keyword < _Entry2.m_Keyword)
    {
        return true;
    }

    return false;
}
