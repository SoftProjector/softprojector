#ifndef IMAGEGENERATOR_HPP
#define IMAGEGENERATOR_HPP

#include <QPixmap>
#include <QPainter>
#include <QGraphicsBlurEffect>
#include <QGraphicsScene>
#include <QGraphicsPixmapItem>
#include <settings.h>
#include <displaysetting.h>
#include <bible.h>
#include <song.h>
#include <announcement.h>

class ImageGenerator
{
public:
    ImageGenerator();
    void setScreenSize(QSize size);
    QSize getScreenSize();

    QPixmap generateEmptyImage();
    QPixmap generateColorImage(QColor &color);
    QPixmap generateBibleImage(Verse verse, BibleSettings &bSets);
    QPixmap generateSongImage(Stanza stanza, SongSettings &sSets);
    QPixmap generateAnnounceImage(AnnounceSlide announce, TextSettings &aSets);

    int width();
    int height();

private:
    QSize m_screenSize;
    bool m_shadow, m_blurShadow, m_isTextPrepared;
    int m_type; // 0 = empty, 1 = bible, 2 = song, 3 = announce
    int m_shadowOffset, m_blurRadius;

    Verse m_verse;
    BibleSettings m_bSets;
    BibleDisplaySettings m_bdSets;

    Stanza m_stanza;
    SongSettings m_sSets;
    SongDisplaySettings m_sdSets;

    AnnounceSlide m_announce;
    TextSettings m_aSets;
    AnnounceDisplaySettings m_adSets;


    QPixmap renderText();

    QRect boundRectOrDrawText(QPainter *painter, bool draw, int left, int top, int width, int height, int flags, QString text);
    void drawBibleText(QPainter *painter, bool isShadow);
    void drawBibleTextToRect(QPainter *painter, QRect& trect, QRect& crect, QString ttext, QString ctext, int tflags, int cflags, int top, int left, int width, int height);
    void drawSongText(QPainter *painter, bool isShadow);
    void drawAnnounceText(QPainter *painter, bool isShadow);
//    void fastbluralpha(QImage &img, int radius);
    QPixmap applyEffectToImage(QPixmap src, QGraphicsEffect *effect);

};

#endif // IMAGEGENERATOR_HPP
