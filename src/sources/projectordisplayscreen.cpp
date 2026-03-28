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

#include "../headers/projectordisplayscreen.hpp"
#include "ui_projectordisplayscreen.h"


ProjectorDisplayScreen::ProjectorDisplayScreen(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ProjectorDisplayScreen)
{
    ui->setupUi(this);
    dispView = new QQuickView;
    imProvider = new SpImageProvider;
    dispView->engine()->addImageProvider(QLatin1String("improvider"),imProvider);
    m_videoWidget = QWidget::createWindowContainer(dispView,this);
    dispView->setSource(QUrl("qrc:/qml/qml/DisplayArea.qml"));
    ui->verticalLayout->addWidget(m_videoWidget);

    // Create Display object for retriaving signals back from the display screen
    QObject *dispObj = dispView->rootObject();
    connect(dispObj,SIGNAL(exitClicked()),this,SLOT(exitSlideClicked()));
    connect(dispObj,SIGNAL(nextClicked()),this,SLOT(nextSlideClicked()));
    connect(dispObj,SIGNAL(prevClicked()),this,SLOT(prevSlideClicked()));

    // Connect Media Player objects
    connect(dispObj,SIGNAL(positionChanged(int)),this,SLOT(videoPositionChanged(int)));
    connect(dispObj,SIGNAL(durationChanged(int)),this,SLOT(videoDurationChanged(int)));
    connect(dispObj,SIGNAL(playbackStateChanged(int)),this,SLOT(videoPlaybackStateChanged(int)));
    connect(dispObj,SIGNAL(playbackStopped()),this,SLOT(playbackStopped()));

    backImSwitch1 = backImSwitch2 = textImSwitch1 = textImSwitch2 = false;
    back1to2 = text1to2 = isNewBack = true;
    tranType = TR_FADE;
    m_color.setRgb(0,0,0,0);// = QColor(QColor::black());
}

ProjectorDisplayScreen::~ProjectorDisplayScreen()
{
    delete dispView;
    delete ui;
}

void ProjectorDisplayScreen::resetImGenSize()
{
    imGen.setScreenSize(this->size());
}

void ProjectorDisplayScreen::setBackPixmap(QPixmap p, int fillMode)
{
    // fill mode -->>  0 = Strech, 1 = keep aspect, 2 = keep aspect by expanding

    if(back.cacheKey() == p.cacheKey())
    {
        isNewBack = false;
        return;
    }
    back = p;
    isNewBack = true;

    switch(fillMode)
    {
    case 0:
        p = p.scaled(imGen.getScreenSize(),Qt::IgnoreAspectRatio,Qt::SmoothTransformation);
        break;
    case 1:
        p = p.scaled(imGen.getScreenSize(),Qt::KeepAspectRatio,Qt::SmoothTransformation);
        break;
    case 2:
        p = p.scaled(imGen.getScreenSize(),Qt::KeepAspectRatioByExpanding,Qt::SmoothTransformation);
        break;
    default:
        // Do No Scaling/resizing
        break;
    }

    imProvider->setPixMap(p);
    back1to2 = (!back1to2);

    QObject *item1 = dispView->rootObject()->findChild<QObject*>("backImage1");
    QObject *item2 = dispView->rootObject()->findChild<QObject*>("backImage2");
    if(item1 && item2)
    {
        if(back1to2)
        {
            backImSwitch2 = (!backImSwitch2);
            if(backImSwitch2)
                item2->setProperty("source","image://improvider/imB2a");
            else
                item2->setProperty("source","image://improvider/imB2b");

            if(p.height()<imGen.height())
                item2->setProperty("y",(imGen.height()-p.height())/2);
            else
                item2->setProperty("y",0);
            if(p.width()<imGen.width())
                item2->setProperty("x",(imGen.width()-p.width())/2);
            else
                item2->setProperty("x",0);
        }
        else
        {
            backImSwitch1 = (!backImSwitch1);
            if(backImSwitch1)
                item1->setProperty("source","image://improvider/imB1a");
            else
                item1->setProperty("source","image://improvider/imB1b");
            if(p.height()<imGen.height())
                item1->setProperty("y",(imGen.height()-p.height())/2);
            else
                item1->setProperty("y",0);
            if(p.width()<imGen.width())
                item1->setProperty("x",(imGen.width()-p.width())/2);
            else
                item1->setProperty("x",0);
        }
    }
}

void ProjectorDisplayScreen::setBackPixmap(QPixmap p, QColor c)
{
    if(backType == 0)
        setBackPixmap(imGen.generateColorImage(c),0);
    else if(backType == 1)
        setBackPixmap(p,0);
    else if(backType ==2)
        setBackPixmap(imGen.generateEmptyImage(),0);
}

void ProjectorDisplayScreen::setTextPixmap(QPixmap p)
{
    imProvider->setPixMap(p);
    text1to2 = (!text1to2);

    QObject *item1 = dispView->rootObject()->findChild<QObject*>("textImage1");
    QObject *item2 = dispView->rootObject()->findChild<QObject*>("textImage2");
    if(item1 && item2)
    {
        if(text1to2)
        {
            textImSwitch2 = (!textImSwitch2);
            if(textImSwitch2)
                item2->setProperty("source","image://improvider/imT2a");
            else
                item2->setProperty("source","image://improvider/imT2b");
        }
        else
        {
            textImSwitch1 = (!textImSwitch1);
            if(textImSwitch1)
                item1->setProperty("source","image://improvider/imT1a");
            else
                item1->setProperty("source","image://improvider/imT1b");
        }
    }
}

void ProjectorDisplayScreen::setBackVideo(QString path)
{
    QObject *item = dispView->rootObject()->findChild<QObject*>("player");
    QObject *item2 = dispView->rootObject()->findChild<QObject*>("vidOut");

    setVideoSource(item,path);
    item->setProperty("volume",0.0);
    item->setProperty("loops", -1); // Qt6 QML: MediaPlayer.Infinite = loop forever
    item2->setProperty("fillMode",Qt::IgnoreAspectRatio);
}

void ProjectorDisplayScreen::setVideoSource(QObject* playerObject, QUrl path)
{
//#if  (defined(Q_OS_UNIX))
//    // Prefix "file://" to the file name only for Unix like systems.
//    path = "file://" + path;
//#endif
//    playerObject->setProperty("source",path);
    playerObject->setProperty("source",path.toString());
}

void ProjectorDisplayScreen::updateScreen()
{
    QObject *root = dispView->rootObject();
    root->setProperty("backType", backType);    // Inject current background type (None/Image/Video)
    
    QMetaObject::invokeMethod(root,"stopTransitions");
    //    QString tranType = "seq";

    // if background is a video, play video, else stop it.
    if(backType == B_VIDEO)
    {
        QMetaObject::invokeMethod(root,"playVideo");
    }
    else
    {
        QMetaObject::invokeMethod(root,"stopVideo");
    }

    if(text1to2 && back1to2)
    {
        if(isNewBack)
            QMetaObject::invokeMethod(root,"transitionBack1to2",Q_ARG(QVariant,tranType));
        QMetaObject::invokeMethod(root,"transitionText1to2",Q_ARG(QVariant,tranType));
    }
    else if(text1to2 && (!back1to2))
    {
        if(isNewBack)
            QMetaObject::invokeMethod(root,"transitionBack2to1",Q_ARG(QVariant,tranType));
        QMetaObject::invokeMethod(root,"transitionText1to2",Q_ARG(QVariant,tranType));
    }
    else if((!text1to2) && back1to2)
    {
        if(isNewBack)
            QMetaObject::invokeMethod(root,"transitionBack1to2",Q_ARG(QVariant,tranType));
        QMetaObject::invokeMethod(root,"transitionText2to1",Q_ARG(QVariant,tranType));
    }
    else
    {
        if(isNewBack)
            QMetaObject::invokeMethod(root,"transitionBack2to1",Q_ARG(QVariant,tranType));
        QMetaObject::invokeMethod(root,"transitionText2to1",Q_ARG(QVariant,tranType));
    }
}

void ProjectorDisplayScreen::exitSlideClicked()
{
    emit exitSlide();
}

void ProjectorDisplayScreen::nextSlideClicked()
{
    qDebug() << "PDS: nextSlideClicked() from QML.";
    emit nextSlide();
}

void ProjectorDisplayScreen::prevSlideClicked()
{
    qDebug() << "PDS: prevSlideClicked() from QML.";
    emit prevSlide();
}

void ProjectorDisplayScreen::videoPositionChanged(int position)
{
    emit videoPositionChanged((qint64)position);
}

void ProjectorDisplayScreen::videoDurationChanged(int duration)
{
    emit videoDurationChanged((qint64)duration);
}

void ProjectorDisplayScreen::videoPlaybackStateChanged(int state)
{
    emit videoPlaybackStateChanged((QMediaPlayer::PlaybackState)state);
}

void ProjectorDisplayScreen::keyReleaseEvent(QKeyEvent *event)
{
    // Will get called when a key is released
    int key = event->key();
    if(key == Qt::Key_Left)
        emit prevSlide();
    else if(key == Qt::Key_Up)
        emit prevSlide();
    else if(key == Qt::Key_PageUp)
        emit prevSlide();
    else if(key == Qt::Key_Back)
        emit prevSlide();
    else if(key == Qt::Key_Right)
        emit nextSlide();
    else if(key == Qt::Key_Down)
        emit nextSlide();
    else if(key == Qt::Key_PageDown)
        emit nextSlide();
    else if(key == Qt::Key_Forward)
        emit nextSlide();
    else if(key == Qt::Key_Enter)
        emit nextSlide();
    else if(key == Qt::Key_Return)
        emit nextSlide();
    else if(key == Qt::Key_Escape)
        emit exitSlide();
    else
        QWidget::keyReleaseEvent(event);
}

void ProjectorDisplayScreen::setPassiveBackground(QPixmap pix)
{
    m_passiveBack = pix;
}

void ProjectorDisplayScreen::renderNotText()
{
    setTextPixmap(imGen.generateEmptyImage());
    updateScreen();
}

void ProjectorDisplayScreen::renderPassiveText(QPixmap pix, bool useBack)
{
    qDebug() << "PDS: renderPassiveText called. UseBack:" << useBack << "PixSize:" << pix.size();
    setPassiveBackground(pix);
    
    tranType = TR_FADE;
    setTextPixmap(imGen.generateEmptyImage());
    if(useBack)
    {
        backType = B_PICTURE;
        setBackPixmap(m_passiveBack,0);
    }
    else
    {
        backType = B_NONE;
        setBackPixmap(imGen.generateColorImage(m_color),0);
    }

    updateScreen();
}

void ProjectorDisplayScreen::renderBibleText(Verse bVerse, BibleSettings &bSets)
{
    // TODO: This is temporary until database is fixed
    if(bSets.useFading)
    {
        tranType = TR_FADE;
    }
    else
    {
        tranType = TR_NONE;
    }

    if(bSets.useBackground)
    {
        setBackPixmap(bSets.backgroundPix,0);
        backType = B_PICTURE;
    }
    else
    {
        setBackPixmap(imGen.generateColorImage(m_color),0);
        backType = B_NONE;
    }

    //tranType = bSets.transitionType;
    //backType = bSets.backgroundType;
    setTextPixmap(imGen.generateBibleImage(bVerse,bSets));
    //setBackPixmap(bSets.backgroundPix,bSets.backgroundColor);
    //if(backType ==2)
    //{
    //    setVideoSource(bSets.backgroundVideoPath);
    //}

    updateScreen();
}

void ProjectorDisplayScreen::renderSongText(Stanza stanza, SongSettings &sSets)
{
    // TODO: This is temporary until database is fixed
    if(sSets.useFading)
    {
        tranType = TR_FADE;
    }
    else
    {
        tranType = TR_NONE;
    }

    if(sSets.useBackground)
    {
        setBackPixmap(sSets.backgroundPix,0);
        backType = B_PICTURE;
    }
    else
    {
        setBackPixmap(imGen.generateColorImage(m_color),0);
        backType = B_NONE;
    }

    setTextPixmap(imGen.generateSongImage(stanza,sSets));
    //if(sSets.backgroundType == 1)
    //    setBackPixmap(sSets.backgroundPix,0);
    //else
    //    setBackPixmap(imGen.generateColorImage(m_color),0);
    //tranType = sSets.transitionType;
    updateScreen();
}

void ProjectorDisplayScreen::renderAnnounceText(AnnounceSlide announce, TextSettings &aSets)
{
    // TODO: This is temporary until database is fixed
    if(aSets.useFading)
    {
        tranType = TR_FADE;
    }
    else
    {
        tranType = TR_NONE;
    }

    if(aSets.useBackground)
    {
        setBackPixmap(aSets.backgroundPix,0);
        backType = B_PICTURE;
    }
    else
    {
        setBackPixmap(imGen.generateColorImage(m_color),0);
        backType = B_NONE;
    }

    setTextPixmap(imGen.generateAnnounceImage(announce,aSets));
    //if(aSets.transitionType == 1)
    //    setBackPixmap(aSets.backgroundPix,0);
    //else
    //    setBackPixmap(imGen.generateColorImage(m_color),0);
    //tranType = aSets.transitionType;
    updateScreen();
}

void ProjectorDisplayScreen::renderSlideShow(QPixmap slide, SlideShowSettings &ssSets)
{
    tranType = TR_FADE;

    // Use passive background if available, otherwise solid color
    if (!m_passiveBack.isNull()) {
        setBackPixmap(m_passiveBack, 0); // 0 = Stretch
    } else {
        setBackPixmap(imGen.generateColorImage(m_color), 0);
    }

    // Generate foreground with the slide centered on top
    QPixmap foreground = imGen.generateEmptyImage();
    QPainter painter(&foreground);
    
    bool expand;
    if(slide.width() < imGen.width() && slide.height() < imGen.height())
        expand = ssSets.expandSmall;
    else
        expand = true;

    Qt::AspectRatioMode aspect = Qt::KeepAspectRatio;
    if (expand) {
        if (ssSets.fitType == 0)      aspect = Qt::IgnoreAspectRatio;           
        else if (ssSets.fitType == 1) aspect = Qt::KeepAspectRatio;             
        else if (ssSets.fitType == 2) aspect = Qt::KeepAspectRatioByExpanding;  
    }

    QPixmap scaledSlide = slide.scaled(imGen.getScreenSize(), aspect, Qt::SmoothTransformation);
    int x = (foreground.width() - scaledSlide.width()) / 2;
    int y = (foreground.height() - scaledSlide.height()) / 2;
    
    painter.drawPixmap(x, y, scaledSlide);
    painter.end();

    setTextPixmap(foreground);
    updateScreen();
}

void ProjectorDisplayScreen::renderVideo(VideoInfo videoDetails)
{
    backType = B_VIDEO;
    setTextPixmap(imGen.generateEmptyImage());
    
    if (videoDetails.hasVideo) {
        setBackPixmap(imGen.generateColorImage(m_color),0);
    } else {
        // Fallback to passive background image if available
        if (!m_passiveBack.isNull()) {
            setBackPixmap(m_passiveBack, 0); // 0 = Stretch
        } else {
            setBackPixmap(imGen.generateColorImage(m_color), 0);
        }
    }
    QObject *root = dispView->rootObject();
    QObject *item = root->findChild<QObject*>("player");
    QObject *item2 = root->findChild<QObject*>("vidOut");

    setVideoSource(item,videoDetails.filePath);

    item->setProperty("volume",1.0);
    item->setProperty("loops", 1); // Qt6 QML: play once
    item2->setProperty("fillMode",Qt::KeepAspectRatio);
    
    // Ensure display view has focus to capture global keys like Escape
    m_videoWidget->setFocus(Qt::ActiveWindowFocusReason);

    updateScreen();
}

void ProjectorDisplayScreen::playVideo()
{
    QObject *root = dispView->rootObject();
    QMetaObject::invokeMethod(root,"playVideo");
}

void ProjectorDisplayScreen::pauseVideo()
{
    QObject *root = dispView->rootObject();
    QMetaObject::invokeMethod(root,"pauseVideo");
}

void ProjectorDisplayScreen::stopVideo()
{
    QObject *root = dispView->rootObject();
    QMetaObject::invokeMethod(root,"stopVideo");
}

void ProjectorDisplayScreen::playbackStopped()
{
    emit videoStopped();
}

void ProjectorDisplayScreen::setVideoVolume(int level)
{
    float vol = 1.0*level/100;
    QObject *root = dispView->rootObject();
    QMetaObject::invokeMethod(root,"setVideoVolume",Q_ARG(QVariant,vol));
}

void ProjectorDisplayScreen::setVideoMuted(bool muted)
{
    QObject *root = dispView->rootObject();
    QMetaObject::invokeMethod(root,"setVideoMuted",Q_ARG(QVariant,muted));
}

void ProjectorDisplayScreen::setVideoPosition(qint64 position)
{
    QObject *root = dispView->rootObject();
    QMetaObject::invokeMethod(root,"setVideoPosition",Q_ARG(QVariant,position));
}

void ProjectorDisplayScreen::setAudioEnabled(bool enabled)
{
    dispView->rootContext()->setContextProperty("audioEnabledInjected", enabled);
}
