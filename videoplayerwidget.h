/***************************************************************************
//
//    softProjector - an open source media projection software
//    Copyright (C) 2014  Vladislav Kobzar, Matvey Adzhigirey and Ilya Spivakov
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

#ifndef VIDEOPLAYERWIDGET_H
#define VIDEOPLAYERWIDGET_H

#include <QtWidgets>
#include <QMediaPlayer>
//#include <phonon>
//#include <phonon/VideoWidget>

class VideoPlayerWidget :QMediaPlayer//: public Phonon::VideoWidget
{
    Q_OBJECT
public:
    VideoPlayerWidget(QWidget *parent = 0);
public slots:

    void setFullScreen(bool);

signals:
    void playPause();
    void handleDrops(QDropEvent *e);

protected:
    void mouseDoubleClickEvent(QMouseEvent *e);
    void keyPressEvent(QKeyEvent *e);
    bool event(QEvent *e);
    void timerEvent(QTimerEvent *e);

private:
    QBasicTimer m_timer;
};

#endif // VIDEOPLAYERWIDGET_H
