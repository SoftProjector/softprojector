#ifndef PROJECTORDISPLAYSCREEN_HPP
#define PROJECTORDISPLAYSCREEN_HPP

#include <QWidget>
#include <QQmlEngine>
#include <QQuickView>
#include <QQuickItem>
#include <QtQml>
#include <QMediaPlaylist>
#include "spimageprovider.hpp"
#include "imagegenerator.hpp"
#include "settings.h"
#include "bible.h"
#include "song.h"
#include "announcement.h"
//#include "slideshow.h"

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
//    void renderVideo();

    void positionControls(DisplayControlsSettings & dSettings);
    void setControlsVisible(bool visible);

private slots:
    void setBackPixmap(QPixmap p,int fillMode); // 0 = Strech, 1 = keep aspect, 2 = keep aspect by expanding
    void setBackPixmap(QPixmap p, QColor c);
    void setTextPixmap(QPixmap p);
    void setVideoSource(QString path);
    void updateScreen();

signals:
    void exitSlide();
    void nextSlide();
    void prevSlide();

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
