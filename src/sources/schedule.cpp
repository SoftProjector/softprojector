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

#include "schedule.hpp"

Schedule::Schedule()
{
    scid = -1;
}

Schedule::Schedule(BibleHistory &b)
{
    scid = -1;
    stype = "bible";
    name = b.caption;
    icon = QIcon(":/icons/icons/book.png");
    bible = b;
}

Schedule::Schedule(Song &s)
{
    scid = -1;
    stype = "song";
    name = QString("%1 %2").arg(s.number).arg(s.title);
    icon = QIcon(":/icons/icons/song_tab.png");
    song = s;
}

Schedule::Schedule(SlideShow &s)
{
    scid = -1;
    stype = "slideshow";
    name = s.name;
    icon = QIcon(":/icons/icons/photo.png");
    slideshow = s;
}

Schedule::Schedule(VideoInfo &m)
{
    scid = -1;
    stype = "media";
    name = m.fileName;
    icon = QIcon(":/icons/icons/video.png");
    media = m;
}

Schedule::Schedule(Announcement &a)
{
    scid = -1;
    stype = "announce";
    name = a.title;
    icon = QIcon(":/icons/icons/announce.png");
    announce = a;
}
