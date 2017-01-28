/***************************************************************************
//
//    softProjector - an open source media projection software
//    Copyright (C) 2017  Vladislav Kobzar
//
//    This program is free software: you can redistribute it and/or modify
//    it under the terms of the GNU General Public License as published by
//    the Free Software Foundation version 3 of the License.
//
//    This program is distributed in the hope that it will be useful,
//    but WITHOUT ANY WARRANTY; without even the implied warranty of
//    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//    GNU General Public License for more details.
//
//    You should have received a copy of the GNU General Public License
//    along with this program.  If not, see <http://www.gnu.org/licenses/>.
//
***************************************************************************/

#ifndef DISPLAYSETTING_HPP
#define DISPLAYSETTING_HPP

#include <QRect>
#include <QFont>

class DisplaySetting
{
public:
    DisplaySetting();
};

class BibleDisplaySettings
{
public:
    QRect ptRect;
    QRect pcRect;
    QRect stRect;
    QRect scRect;
    QRect ttRect;
    QRect tcRect;
    QFont tFont;
    QFont cFont;
    void clear();
};

class SongDisplaySettings
{
public:
    QRect cRect;
    QRect nRect;
    QRect tRect;
    QRect eRect;
    int tFlags;
    void clear();
};

class AnnounceDisplaySettings
{
public:
    QRect tRect;
    void clear();
};

#endif // DISPLAYSETTING_HPP
