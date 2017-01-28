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

#ifndef SCHEDULE_HPP
#define SCHEDULE_HPP

#include "bible.hpp"
#include "song.hpp"
#include "slideshow.hpp"
#include "videoinfo.hpp"
#include "announcement.hpp"

class Schedule
{
public:
    Schedule();
    Schedule(BibleHistory &b);
    Schedule(Song &s);
    Schedule(SlideShow &s);
    Schedule(VideoInfo &m);
    Schedule(Announcement &a);

    QString stype;
    QString name;
    QIcon icon;
    int scid;

    BibleHistory bible;
    Song song;
    SlideShow slideshow;
    VideoInfo media;
    Announcement announce;
};

#endif // SCHEDULE_HPP
