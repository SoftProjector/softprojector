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

#include "displaysetting.hpp"

DisplaySetting::DisplaySetting()
{
}

void BibleDisplaySettings::clear()
{
    ptRect = QRect();
    pcRect = QRect();
    stRect = QRect();
    scRect = QRect();
    ttRect = QRect();
    scRect = QRect();
    tFont = QFont();
    cFont = QFont();
}

void SongDisplaySettings::clear()
{
    cRect = QRect();
    nRect = QRect();
    tRect = QRect();
    eRect = QRect();
    tFlags = 0;
}

void AnnounceDisplaySettings::clear()
{
    tRect = QRect();
}
