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

#ifndef DISPLAYSCREEN_H
#define DISPLAYSCREEN_H

#include <QtWidgets>
//#include <phonon>
//#include <phonon/MediaObject>
//#include <phonon/VideoWidget>
#include "settings.h"
#include "theme.h"
#include "controlbutton.h"
#include "bible.h"
#include "song.h"
#include "announcewidget.h"
#include "videoinfo.h"
#include "displaysetting.h"

namespace Ui {
class DisplayScreen;
}

class DisplayScreen : public QWidget
{
    Q_OBJECT

public:
    explicit DisplayScreen(QWidget *parent = 0);
    ~DisplayScreen();
//    Phonon::MediaObject *videoPlayer;
/*
public slots:
    void setNewWallpaper(QString path, bool isToUse);
    void setNewWallpaper(QPixmap wallPix, bool isToUse);
    void setNewPassiveWallpaper(QPixmap wallPix, bool isToUse);

    void fadeIn();
    void fadeOut();
    void setDisplaySettings(DisplaySettings sets);
    void setFading(bool fade);

    void fastbluralpha(QImage &img, int radius);
    void setControlsSettings(DisplayControlsSettings &settings);
    void setControlButtonsVisible(bool visible);
    void positionOpjects();

    void renderText(bool text_present);
    void renderBibleText(Verse verse, BibleSettings &bibleSetings);
    void renderSongText(Stanza stanza, SongSettings &songSettings);
    void renderAnnounceText(AnnounceSlide announce, TextSettings &announceSettings);
    void renderPicture(QPixmap image, SlideShowSettings ssSets);
    void renderVideo(VideoInfo &vid);
    void renderClear();

signals:
    void exitSlide();
    void nextSlide();
    void prevSlide();
    void sendTimeText(QString cTime);
    void updatePlayButton(bool isPlaying);

protected:
    void paintEvent(QPaintEvent *event);
    void keyReleaseEvent(QKeyEvent *event);

private slots:
    void positionControlButtons();
    void btnNextClicked();
    void btnPrevClicked();
    void btnExitClicked();

    void drawBibleText(QPainter *painter, int width, int height, bool isShadow);
    void drawBibleTextToRect(QPainter *painter, QRect& trect, QRect& crect, QString ttext, QString ctext, int tflags, int cflags, int top, int left, int width, int height);
    void drawSongText(QPainter *painter, int width, int height, bool isShadow);
    QRect boundRectOrDrawText(QPainter *painter, bool draw, int left, int top, int width, int height, int flags, QString text);
    void drawAnnounceText(QPainter *painter, int width, int height, bool isShadow);

    void updateTimeText();
//    void playerStateChanged(Phonon::State newstate, Phonon::State oldstate);

private:
    Ui::DisplayScreen *ui;
    DisplayControlsSettings controlsSettings;
    bool useFading;
    bool useShadow;
    bool useBluredShadow;
    bool isTextPrepared;
    QString wallpaperPath; // Wallpaper image file path
    QPixmap wallpaper;
    QString passiveWallpaperPath;
    QPixmap passiveWallpaper;
    QColor foregroundColor;

    QPixmap previous_image_pixmap;
    QImage output_image;
    bool use_active_background;
    bool background_needs_transition;
    int acounter[2];
    QTimer *timer;
    QTimer *timer_out;

    ControlButton *btnNext;
    ControlButton *btnPrev;
    ControlButton *btnExit;

    SongSettings songSets;
    BibleSettings bibleSets;
    TextSettings annouceSets;

    Verse bibleVerse;
    Stanza songStanza;
    AnnounceSlide announcement;

    QString displayType;
    QLabel *textRenderLabel;
//    Phonon::VideoWidget *videoWidget;

    BibleDisplaySettings bdSets;
    SongDisplaySettings sdSets;
    AnnounceDisplaySettings adSets;
    */
};

#endif // DISPLAYSCREEN_H
