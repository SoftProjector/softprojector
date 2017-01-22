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

#include "videoplayerwidget.h"

VideoPlayerWidget::VideoPlayerWidget(QWidget *parent): QMediaPlayer(parent)/*:
    Phonon::VideoWidget(parent)*/

{

}

void VideoPlayerWidget::setFullScreen(bool enabled)
{
//    Phonon::VideoWidget::setFullScreen(enabled);
}

void VideoPlayerWidget::mouseDoubleClickEvent(QMouseEvent *e)
{
//    Phonon::VideoWidget::mouseDoubleClickEvent(e);
//    setFullScreen(!isFullScreen());
}

void VideoPlayerWidget::keyPressEvent(QKeyEvent *e)
{
    int key = e->key();
    if (key == Qt::Key_Space)
    {
        emit playPause();
        e->accept();
        return;
    }
    else if (key == Qt::Key_Escape)
    {
        setFullScreen(false);
        e->accept();
        return;
    }
//    Phonon::VideoWidget::keyPressEvent(e);
}

bool VideoPlayerWidget::event(QEvent *e)
{
    switch(e->type())
    {
    case QEvent::Close:
        //we just ignore the cose events on the video widget
        //this prevents ALT+F4 from having an effect in fullscreen mode
        e->ignore();
        return true;
    case QEvent::MouseMove:
//#ifndef QT_NO_CURSOR
////        unsetCursor();
//#endif
//        //fall through
//    case QEvent::WindowStateChange:
//    {
//        if (windowState() & Qt::WindowFullScreen)
//            m_timer.start(1000, this);
//        else
//        {
//            m_timer.stop();
//#ifndef QT_NO_CURSOR
//            unsetCursor();
//#endif
//        }
//    }
        break;
    default:
        break;
    }

//    return Phonon::VideoWidget::event(e);
    return false;
}

void VideoPlayerWidget::timerEvent(QTimerEvent *e)
{
//    if (e->timerId() == m_timer.timerId())
//    {
        ///let's store the cursor shape
//#ifndef QT_NO_CURSOR
//        setCursor(Qt::BlankCursor);
//#endif
//    }
//    Phonon::VideoWidget::timerEvent(e);
}
