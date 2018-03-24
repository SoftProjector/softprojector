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

#ifndef PROJECTORDISPLAYSCREEN_HPP
#define PROJECTORDISPLAYSCREEN_HPP

#include <QWidget>
#include <QQmlEngine>
#include <QQuickView>
#include <QQuickItem>
#include <QtQml>
#include <QMediaPlaylist>
#include <QMediaPlayer>
#include "spimageprovider.hpp"
#include "imagegenerator.hpp"
#include "settings.hpp"
#include "bible.hpp"
#include "song.hpp"
#include "announcement.hpp"
#include "videoinfo.hpp"
//#include "slideshow.hpp"

namespace Ui {
class ProjectorDisplayScreen;
}

class ProjectorDisplayScreen : public QWidget
{
    Q_OBJECT

public:
    explicit ProjectorDisplayScreen(QWidget *parent = 0);
    ~ProjectorDisplayScreen();

public slots:
    void resetImGenSize();

    void renderNotText();
    void renderPassiveText(QPixmap &back,bool useBack);
    void renderBibleText(Verse bVerse, BibleSettings &bSets);
    void renderSongText(Stanza stanza, SongSettings &sSets);
    void renderAnnounceText(AnnounceSlide announce, TextSettings &aSets);
    void renderSlideShow(QPixmap slide,SlideShowSettings &ssSets);
    void renderVideo(VideoInfo videoDetails);

    void playVideo();
    void pauseVideo();
    void stopVideo();
    void setVideoVolume(int level);
    void setVideoMuted(bool muted);
    void setVideoPosition(qint64 position);

    void positionControls(DisplayControlsSettings & dSettings);
    void setControlsVisible(bool visible);

private slots:
    void setBackPixmap(QPixmap p,int fillMode); // 0 = Strech, 1 = keep aspect, 2 = keep aspect by expanding
    void setBackPixmap(QPixmap p, QColor c);
    void setTextPixmap(QPixmap p);
    void setBackVideo(QString path);
    void setVideoSource(QObject *playerObject, QUrl path);
    void updateScreen();

    void exitSlideClicked();
    void nextSlideClicked();
    void prevSlideClicked();

    void videoPositionChanged(int position);
    void videoDurationChanged(int duration);
    void videoPlaybackStateChanged(int state);

signals:
    void exitSlide();
    void nextSlide();
    void prevSlide();

    void videoPositionChanged(qint64 position);
    void videoDurationChanged(qint64 duration);
    void videoPlaybackStateChanged(QMediaPlayer::State state);

protected:
    void keyReleaseEvent(QKeyEvent *event);

private:
    Ui::ProjectorDisplayScreen *ui;
    QQuickView *dispView;
    SpImageProvider *imProvider;
    ImageGenerator imGen;
    bool backImSwitch1, textImSwitch1, backImSwitch2, textImSwitch2;
    bool isNewBack, back1to2, text1to2;
    int tranType,backType;
    QColor m_color;
   // DisplayControlsSettings mySettings;

    QPixmap back;
};

#endif // PROJECTORDISPLAYSCREEN_HPP
