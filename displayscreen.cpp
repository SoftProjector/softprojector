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

#include "displayscreen.h"
#include "ui_displayscreen.h"

#define BLUR_RADIUS 5

DisplayScreen::DisplayScreen(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::DisplayScreen)
{
    ui->setupUi(this);
    /*
    setPalette(QPalette(QColor(Qt::black),QColor(Qt::black)));

    timer = new QTimer(this);
    timer_out = new QTimer(this);

    acounter[0]=255;

    // add video player

    videoPlayer = new Phonon::MediaObject;
    videoWidget = new Phonon::VideoWidget(this);
    videoWidget->setVisible(false);
    Phonon::createPath(videoPlayer,videoWidget);

    connect(videoPlayer, SIGNAL(tick(qint64)),this,SLOT(updateTimeText()));
    connect(videoPlayer, SIGNAL(totalTimeChanged(qint64)),this,SLOT(updateTimeText()));
    connect(videoPlayer, SIGNAL(stateChanged(Phonon::State,Phonon::State)),this,SLOT(playerStateChanged(Phonon::State,Phonon::State)));


    // add text render lable
    textRenderLabel = new QLabel(this);

    // Add control buttons
    btnNext = new ControlButton(QIcon(":/icons/icons/controlNext.png"),
                                QIcon(":/icons/icons/controlNextHovered.png"),
                                QIcon(":/icons/icons/controlNextPressed.png"),this);
    btnPrev = new ControlButton(QIcon(":/icons/icons/controlPrev.png"),
                                QIcon(":/icons/icons/controlPrevHovered.png"),
                                QIcon(":/icons/icons/controlPrevPressed.png"),this);
    btnExit = new ControlButton(QIcon(":/icons/icons/controlExit.png"),
                                QIcon(":/icons/icons/controlExitHovered.png"),
                                QIcon(":/icons/icons/controlExitPressed.png"),this);

    connect(timer, SIGNAL(timeout()), this, SLOT(fadeIn()));
    connect(btnNext,SIGNAL(clicked()),this,SLOT(btnNextClicked()));
    connect(btnPrev,SIGNAL(clicked()),this,SLOT(btnPrevClicked()));
    connect(btnExit,SIGNAL(clicked()),this,SLOT(btnExitClicked()));
    */
}

DisplayScreen::~DisplayScreen()
{
//    delete timer;
//    delete timer_out;
//    delete videoPlayer;
//    delete videoWidget;
//    delete textRenderLabel;
//    delete btnPrev;
//    delete btnNext;
//    delete btnExit;
    delete ui;
}
/*
void DisplayScreen::keyReleaseEvent(QKeyEvent *event)
{
    // Will get called when a key is released
    int key = event->key();
    if(key == Qt::Key_Left)
        prevSlide();
    else if(key == Qt::Key_Up)
        prevSlide();
    else if(key == Qt::Key_PageUp)
        prevSlide();
    else if(key == Qt::Key_Back)
        prevSlide();
    else if(key == Qt::Key_Right)
        nextSlide();
    else if(key == Qt::Key_Down)
        nextSlide();
    else if(key == Qt::Key_PageDown)
        nextSlide();
    else if(key == Qt::Key_Forward)
        nextSlide();
    else if(key == Qt::Key_Enter)
        nextSlide();
    else if(key == Qt::Key_Return)
        nextSlide();
    else if(key == Qt::Key_Escape)
        exitSlide();
    else
        QWidget::keyReleaseEvent(event);
}

void DisplayScreen::positionOpjects()
{
//    videoWidget->setGeometry(0,0,width(),height());
    textRenderLabel->setGeometry(0,0,width(),height());
}

void DisplayScreen::setControlsSettings(DisplayControlsSettings &settings)
{
    controlsSettings = settings;
    positionControlButtons();
}

void DisplayScreen::setControlButtonsVisible(bool visible)
{
    btnPrev->setVisible(visible);
    btnNext->setVisible(visible);
    btnExit->setVisible(visible);
}

void DisplayScreen::positionControlButtons()
{
    // set icon sise
    int buttonSize(controlsSettings.buttonSize);
    if(buttonSize == 0)
        buttonSize = 16;
    else if(buttonSize == 1)
        buttonSize = 24;
    else if(buttonSize == 2)
        buttonSize = 32;
    else if(buttonSize == 3)
        buttonSize = 48;
    else if(buttonSize == 4)
        buttonSize = 64;
    else if(buttonSize == 5)
        buttonSize = 96;
    else
        buttonSize = 48;
    btnNext->setIconSize(QSize(buttonSize,buttonSize));
    btnPrev->setIconSize(QSize(buttonSize,buttonSize));
    btnExit->setIconSize(QSize(buttonSize,buttonSize));

    // set buttons size to be 2px greater than the icon size
    buttonSize +=2;

    // calculate button position
    int y(this->height()), x(this->width()), margin(5);

    // calculate y position
    if(controlsSettings.alignmentV==0)//top
        y = margin;
    else if(controlsSettings.alignmentV==1)//middle
        y = (y-buttonSize)/2;
    else if(controlsSettings.alignmentV==2)//buttom
        y = y-buttonSize-margin;
    else
        y = y-buttonSize-margin;

    // calculate x position
    int xt((buttonSize*3)+10); //total width of the button group
    if(controlsSettings.alignmentH==0)
        x = margin;
    else if(controlsSettings.alignmentH==1)
        x = (x-xt)/2;
    else if (controlsSettings.alignmentH==2)
        x = x-xt-margin;
    else
        x = (x-xt)/2;

    int x1(x);
    int x2(x1+buttonSize+5);
    int x3(x2+buttonSize+5);

    //set button positon
    btnPrev->setGeometry(x1,y,buttonSize,buttonSize);
    btnNext->setGeometry(x2,y,buttonSize,buttonSize);
    btnExit->setGeometry(x3,y,buttonSize,buttonSize);

    //set button opacity
    btnPrev->setOpacity(controlsSettings.opacity);
    btnNext->setOpacity(controlsSettings.opacity);
    btnExit->setOpacity(controlsSettings.opacity);

    // repaint buttons
    btnPrev->repaint();
    btnNext->repaint();
    btnExit->repaint();
}

void DisplayScreen::btnNextClicked()
{
    emit nextSlide();
}

void DisplayScreen::btnPrevClicked()
{
    emit prevSlide();
}

void DisplayScreen::btnExitClicked()
{
    emit exitSlide();
}

void DisplayScreen::fadeIn()
{
    if (useFading)
    {
        acounter[0]+=64; // fade step increaments
        if (acounter[0]>255)
            acounter[0]=255;

        if (acounter[0]>254)
            timer->stop();
        update();
    }
}

void DisplayScreen::fadeOut() // For future
{
    //    acounter[0]-=24;
    //    if (acounter[0]<0)acounter[0]=0;
    //    if (acounter[0]<1){timer_out->stop();}
    //    update();
}

void DisplayScreen::renderText(bool text_present)
{
//    if(displayType=="video")
//    {
//        if(!videoWidget->isVisible())
//            videoWidget->setVisible(true);
//        if(textRenderLabel->isVisible())    // Need to remove when text lable on top of video properly works
//            textRenderLabel->setVisible(false);
//    }
//    else
//    {
        if(!textRenderLabel->isVisible())   // Need to remove when text lable on top of video properly works
            textRenderLabel->setVisible(true);
//        if(videoWidget->isVisible())
//        {
//            videoPlayer->stop();
//            videoWidget->setVisible(false);
//        }
//    }

    if(!text_present)
        displayType.clear();

    if(useFading)
    {
        acounter[0]=0;
    }
    // Save the previous image for fade-out effect:
    previous_image_pixmap = QPixmap::fromImage(output_image);

    // For later determening which background to draw, and whether to transition to it:
    background_needs_transition = ( use_active_background != text_present );
    use_active_background = text_present;

    // Render the foreground text:
    QImage text_image(width(), height(), QImage::Format_ARGB32);//_Premultiplied);

    // Fill transparent background instead of initial garbage (fixes issues on MacOSX):
    text_image.fill(qRgba(0, 0, 0, 0)); //transparent background

    QPainter text_painter(&text_image);
    //text_painter.setRenderHint(QPainter::TextAntialiasing);
    //text_painter.setRenderHint(QPainter::Antialiasing);

    // Request to write its text to the QPainter:
    if(displayType == "bible")
        drawBibleText(&text_painter, width(), height(),false);
    else if(displayType == "song")
        drawSongText(&text_painter, width(), height(),false);
    else if(displayType == "announce")
        drawAnnounceText(&text_painter, width(), height(),false);
    text_painter.end();

    // Draw the shadow image:
    QImage shadow_image(width(), height(), QImage::Format_ARGB32);//_Premultiplied);
    shadow_image.fill(qRgba(0, 0, 0, 0)); //transparent background
    QPainter shadow_painter(&shadow_image);
    shadow_painter.setPen(QColor(Qt::black));

    if(useShadow)
    {
        if(displayType == "bible")
            drawBibleText(&shadow_painter, width(), height(),true);
        else if(displayType == "song")
            drawSongText(&shadow_painter, width(), height(),true);
        else if(displayType == "announce")
            drawAnnounceText(&shadow_painter, width(), height(),true);
        shadow_painter.end();
    }

    // Set the blured image to the produced text image:
    if(useBluredShadow) // Blur the shadow:
        fastbluralpha(shadow_image, BLUR_RADIUS);

    QImage temp_image(width(), height(), QImage::Format_ARGB32);//_Premultiplied);
    output_image = temp_image;
    output_image.fill(qRgba(0, 0, 0, 0)); //transparent background

    // Painter for drawing the final image:
    QPainter output_painter(&output_image);
    //output_painter.setRenderHint(QPainter::TextAntialiasing);
    //output_painter.setRenderHint(QPainter::Antialiasing);

    // Offset the shadow by a fraction of the font size:
    int shadow_offset(0);
    if(displayType == "bible")
        shadow_offset = (bdSets.tFont.pointSize() / 15);
    else if(displayType == "song")
        shadow_offset = (songSets.textFont.pointSize() / 15);
    else if(displayType == "announce")
        shadow_offset = (annouceSets.textFont.pointSize() / 15 );

    // Draw the shadow:
    output_painter.drawImage(shadow_offset, shadow_offset, shadow_image);

    // Draw the text:
    output_painter.drawImage(0, 0, text_image);
    output_painter.end();

    if(useFading)
        timer->start(32); // time beween fade steps in milliseconds
    else
        update();
}

void DisplayScreen::renderBibleText(Verse verse, BibleSettings &bibleSetings)
{
    // Render bible verse text
    displayType = "bible";
    bibleVerse = verse;
    bibleSets = bibleSetings;

    useFading = bibleSets.useFading;
    useShadow = bibleSets.useShadow;
    useBluredShadow = bibleSets.useBlurShadow;
    setNewWallpaper(bibleSets.background,bibleSets.useBackground);

    isTextPrepared = false;
    renderText(true);
}

void DisplayScreen::renderSongText(Stanza stanza, SongSettings &songSettings)
{
    // Render song stanza text
    displayType = "song";
    songStanza = stanza;
    songSets = songSettings;

    useFading = songSets.useFading;
    useShadow = songSets.useShadow;
    useBluredShadow = songSets.useBlurShadow;
    if(songStanza.usePrivateSettings)
    {
        // Set song specific settings
        songSets.useBackground = songStanza.useBackground;
        songSets.backgroundName = songStanza.backgroundName;
        songSets.background = songStanza.background;
        songSets.textFont = songStanza.font;
        songSets.textColor = songStanza.color;
        songSets.infoColor = songStanza.infoColor;
        songSets.infoFont = songStanza.infoFont;
        songSets.endingColor = songStanza.endingColor;
        songSets.infoFont = songStanza.endingFont;
        songSets.textAlingmentV = songStanza.alignmentV;
        songSets.textAlingmentH = songStanza.alignmentH;
    }

    setNewWallpaper(songSets.background,songSets.useBackground);

    isTextPrepared = false;
    renderText(true);
}

void DisplayScreen::renderAnnounceText(AnnounceSlide announce, AnnounceSettings &announceSettings)
{
    // Render announcement slide text
    displayType = "announce";
    announcement = announce;
    annouceSets = announceSettings;

    useFading = annouceSets.useFading;
    useShadow = annouceSets.useShadow;
    useBluredShadow = annouceSets.useBlurShadow;
    setNewWallpaper(annouceSets.background,annouceSets.useBackground);

    isTextPrepared = false;
    renderText(true);
}

void DisplayScreen::renderPicture(QPixmap image, SlideShowSettings ssSets)
{
    displayType = "pix";
    bool expand;
    if(image.width()<width() && image.height()<height())
        expand = ssSets.expandSmall;
    else
        expand = true;

    if(expand)
    {
        if(ssSets.fitType == 0)
            wallpaper = image.scaled(width(),height(),Qt::KeepAspectRatio);
        else if(ssSets.fitType == 1)
            wallpaper = image.scaled(width(),height(),Qt::KeepAspectRatioByExpanding);
    }
    else
        wallpaper = image;

    renderText(true);
    useBluredShadow = false;
    useShadow = false;
}

void DisplayScreen::renderVideo(VideoInfo &vid)
{
//    displayType = "video";
//    renderText(false);
//    videoPlayer->stop();
//    videoPlayer->setCurrentSource(Phonon::MediaSource(vid.filePath));
//    videoWidget->setAspectRatio(Phonon::VideoWidget::AspectRatio(vid.aspectRatio));
//    videoPlayer->play();
}

void DisplayScreen::renderClear()
{
    displayType = "clear";
    renderText(true);
}

void DisplayScreen::updateTimeText()
{
    long len = 0;//videoPlayer->totalTime();
    long pos = 0;//videoPlayer->currentTime();
    QString timeString;
    if (pos || len)
    {
        int sec = pos/1000;
        int min = sec/60;
        int hour = min/60;
        int msec = pos;

        QTime playTime(hour%60, min%60, sec%60, msec%1000);
        sec = len / 1000;
        min = sec / 60;
        hour = min / 60;
        msec = len;

        QTime stopTime(hour%60, min%60, sec%60, msec%1000);
        QString timeFormat = "m:ss";
        if (hour > 0)
            timeFormat = "h:mm:ss";
        timeString = playTime.toString(timeFormat);
        if (len)
            timeString += " / " + stopTime.toString(timeFormat);
    }

    emit sendTimeText(timeString);
}

void DisplayScreen::playerStateChanged(Phonon::State newstate, Phonon::State oldstate)
{
    Q_UNUSED(oldstate);
    switch (newstate)
    {
    case Phonon::ErrorState:
        videoPlayer->pause();
        QMessageBox::warning(this,tr("Video Player Error"),videoPlayer->errorString(),QMessageBox::Close);
        break;
    case Phonon::StoppedState:
    case Phonon::PausedState:
        emit updatePlayButton(false);
        break;
    case Phonon::PlayingState:
        emit updatePlayButton(true);
    case Phonon::BufferingState:
        break;
    case Phonon::LoadingState:
        break;
    }
}


void DisplayScreen::drawBibleText(QPainter *painter, int width, int height, bool isShadow)
{
    // Margins:
    int left = 30;
    int top = 20;

    //int right = width - left;
    int w = width - left - left;
    int h = height - top - top;

    // set maximum screen size - For primary bibile  only
    int maxh = h * bibleSets.screenUse/100; // maximun screen height
    int maxtop; // top of max screen
    if(bibleSets.screenPosition == 0)
        maxtop  = top;
    if(bibleSets.screenPosition == 1)
        maxtop = top+h-maxh;

    // apply max screen use settings
    h=maxh;
    top=maxtop;

    // Keep decreasing the font size until the text fits into the allocated space:

    // Rects for storing the position of the text and caption drawing:
    QRect trect1, crect1, trect2, crect2, trect3, crect3;
    // Flags to be used for drawing verse text and caption:
    int tflags = Qt::TextWordWrap;
    tflags = Qt::TextWordWrap;

    if(bibleSets.textAlingmentV==0)
        tflags += Qt::AlignTop;
    else if(bibleSets.textAlingmentV==1)
        tflags += Qt::AlignVCenter;
    else if(bibleSets.textAlingmentV==2)
        tflags += Qt::AlignBottom;

    if(bibleSets.textAlingmentH==0)
        tflags += Qt::AlignLeft;
    else if(bibleSets.textAlingmentH==1)
        tflags += Qt::AlignHCenter;
    else if(bibleSets.textAlingmentH==2)
        tflags += Qt::AlignRight;

    int cflags = Qt::AlignTop ;
    if(bibleSets.captionAlingment==0)
        cflags += Qt::AlignLeft;
    else if(bibleSets.captionAlingment==1)
        cflags += Qt::AlignHCenter;
    else if(bibleSets.captionAlingment==2)
        cflags += Qt::AlignRight;

    bool exit = false;
    if(!isTextPrepared)
    {
        bdSets.clear();
        while( !exit )
        {
            if(bibleVerse.secondary_text.isEmpty() && bibleVerse.trinary_text.isEmpty())
            {
                // Prepare primary version only, 2nd and 3rd do not exist
                // Figure out how much space the drawing will take at the current font size:
                drawBibleTextToRect(painter,trect1,crect1,bibleVerse.primary_text,bibleVerse.primary_caption,
                                    tflags,cflags,top,left,w,h);

                // Make sure that all fits into the screen
                int th = trect1.height()+crect1.height();
                exit = (th<=h);
            }
            else if(!bibleVerse.secondary_text.isEmpty() && bibleVerse.trinary_text.isEmpty())
            {
                // Prepare primary and secondary versions, trinary does not exist
                // Figure out how much space the drawing will take at the current font size for primary:
                drawBibleTextToRect(painter,trect1,crect1,bibleVerse.primary_text,bibleVerse.primary_caption,
                                    tflags,cflags,top,left,w,h/2);

                // set new top for secondary
                int top2 = crect1.bottom();
                if(top2<h/2+top)
                    top2=h/2+top;

                // Figure out how much space the drawing will take at the current font size for secondary:
                drawBibleTextToRect(painter,trect2,crect2,bibleVerse.secondary_text,bibleVerse.secondary_caption,
                                    tflags,cflags,top2,left,w,h/2);

                int th1 = trect1.height()+crect1.height();
                int th2 = trect2.height()+crect2.height();

                // Make sure that primary fits
                exit = (th1<=h/2);
                if (exit)
                    // If secondary fits, make sure secondary fits
                    exit = (th2<=h/2);
            }
            else if(!bibleVerse.secondary_text.isEmpty() && !bibleVerse.trinary_text.isEmpty())
            {
                // Prepare primary and secondary and trinary versions
                // Figure out how much space the drawing will take at the current font size for primary:
                drawBibleTextToRect(painter,trect1,crect1,bibleVerse.primary_text,bibleVerse.primary_caption,
                                    tflags,cflags,top,left,w,h*1/3);

                // set new top for secondary
                int top2 = crect1.bottom();
                if(top2<h*1/3+top)
                    top2=h*1/3+top;

                // Figure out how much space the drawing will take at the current font size for secondary:
                drawBibleTextToRect(painter,trect2,crect2,bibleVerse.secondary_text,bibleVerse.secondary_caption,
                                    tflags,cflags,top2,left,w,h*1/3);

                // set new top for trinaty
                top2 = crect2.bottom();
                if(top2<h*2/3+top)
                    top2 = h*2/3+top;

                // Figure out how much space the drawing will take at the current font size for trinary:
                drawBibleTextToRect(painter,trect3,crect3,bibleVerse.trinary_text,bibleVerse.trinary_caption,
                                    tflags,cflags,top2,left,w,h*1/3);

                int th1 = trect1.height()+crect1.height();
                int th2 = trect2.height()+crect2.height();
                int th3 = trect3.height()+crect3.height();

                // Make sure that primary fits
                exit = (th1<=h*1/3);
                if(exit)
                    // If secondary fits, make sure secondary fits
                    exit = (th2<=h*1/3);
                if(exit)
                    // If also trinary fits, make sure trinary fits
                    exit = (th3<=h*1/3);

            }

            if( !exit ) // The current font is too large, decrease and try again:
            {
                int current_size = bibleSets.textFont.pointSize();
                current_size--;
                bibleSets.textFont.setPointSize(current_size);
            }
        }

        isTextPrepared = true;
        bdSets.ptRect = trect1;
        bdSets.pcRect = crect1;
        bdSets.stRect = trect2;
        bdSets.scRect = crect2;
        bdSets.ttRect = trect3;
        bdSets.tcRect = crect3;
        bdSets.tFont = bibleSets.textFont;
        bdSets.cFont = bibleSets.captionFont;
    }

    // Draw the bible text verse(s) at the final size:

    painter->setFont(bdSets.tFont);
    if(isShadow)
        painter->setPen(QColor(Qt::black));
    else
        painter->setPen(bibleSets.textColor);
    painter->drawText(bdSets.ptRect, tflags, bibleVerse.primary_text);
    if(!bibleVerse.secondary_text.isNull())
        painter->drawText(bdSets.stRect, tflags, bibleVerse.secondary_text);
    if(!bibleVerse.trinary_text.isNull())
        painter->drawText(bdSets.ttRect, tflags, bibleVerse.trinary_text);

    // Draw the verse caption(s) at the final size:
    painter->setFont(bdSets.cFont);
    if(isShadow)
        painter->setPen(QColor(Qt::black));
    else
        painter->setPen(bibleSets.captionColor);
    painter->drawText(bdSets.pcRect, cflags, bibleVerse.primary_caption);
    if(!bibleVerse.secondary_text.isNull())
        painter->drawText(bdSets.scRect, cflags, bibleVerse.secondary_caption);
    if(!bibleVerse.trinary_caption.isNull())
        painter->drawText(bdSets.tcRect, cflags, bibleVerse.trinary_caption);
}

void DisplayScreen::drawBibleTextToRect(QPainter *painter, QRect& trect, QRect& crect, QString ttext,
                                        QString ctext, int tflags, int cflags, int top, int left,
                                        int width, int height)
{
    // prepare caption
    painter->setFont(bibleSets.captionFont);
    crect = painter->boundingRect(left, top, width, height, cflags, ctext);

    // prepare text
    painter->setFont(bibleSets.textFont);
    trect = painter->boundingRect(left, top, width, height-crect.height(), tflags, ttext);

    // reset capion location
    int ch = crect.height();
    int th = trect.height();
    if(bibleSets.captionPosition == 0)
    {
        crect.setTop(trect.top());
        crect.setHeight(ch);
        trect.setTop(crect.bottom());
        trect.setHeight(th);
    }
    else if(bibleSets.captionPosition == 1)
    {
        crect.setTop(trect.bottom());
        crect.setHeight(ch);
    }
}

void DisplayScreen::drawSongText(QPainter *painter, int width, int height, bool isShadow)
{
    // Draw the text of the current song verse to the specified painter; making
    // sure that the output rect is narrower than <width> and shorter than <height>.

    QString main_text = songStanza.stanza;
    QString caption_str;
    QString song_ending = " ";

    //QStringList lines_list = song_list.at(current_song_verse).split("\n");
    QString song_num_str = QString::number(songStanza.number);
    QString song_key_str = songStanza.tune;

    // Check whether to display song numbers
    if (songSets.showSongNumber)
        song_num_str = song_num_str;
    else
        song_num_str = " ";

    // Check whether to display song key
    if (songSets.showSongKey)
        song_num_str = song_key_str + "  " + song_num_str;
    else
        song_num_str = song_num_str;

    // Check wheter to display stanza tiles
    if (songSets.showStanzaTitle)
        caption_str = songStanza.stanzaTitle;
    else
        caption_str = " ";

    // If No cation,number or tune, give the space to song text
    if(!songSets.showSongNumber && !songSets.showSongKey && !songSets.showStanzaTitle)
    {
        song_num_str.clear();
        caption_str.clear();
    }

    // Prepare Song ending string
    if(songStanza.isLast)
    {
        // first check if to show ending
        if(songSets.showSongEnding)
        {
            if(songSets.endingType == 0)
                song_ending = "*    *    *";
            else if(songSets.endingType == 1)
                song_ending = "-    -    -";
            else if(songSets.endingType == 2)
                song_ending = QString::fromUtf8("°    °    °");
            else if(songSets.endingType == 3)
                song_ending = QString::fromUtf8("•    •    •");
            else if(songSets.endingType == 4)
                song_ending = QString::fromUtf8("●    ●    ●");
            else if(songSets.endingType == 5)
                song_ending = QString::fromUtf8("▪    ▪    ▪");
            else if(songSets.endingType == 6)
                song_ending = QString::fromUtf8("■    ■    ■");
            else if(songSets.endingType == 7)
            {
                // First check if copyrigth info exist. If it does show it.
                // If some exist, then show what exist. If nothing exist, then show '* * *'
                if(!songStanza.wordsBy.isEmpty() && !songStanza.musicBy.isEmpty())
                    song_ending = QString(tr("Words by: %1, Music by: %2")).arg(songStanza.wordsBy).arg(songStanza.musicBy);
                else if(!songStanza.wordsBy.isEmpty() && songStanza.musicBy.isEmpty())
                    song_ending = QString(tr("Words by: %1")).arg(songStanza.wordsBy);
                else if(songStanza.wordsBy.isEmpty() && !songStanza.musicBy.isEmpty())
                    song_ending = QString(tr("Music by: %1")).arg(songStanza.musicBy);
                else if(songStanza.wordsBy.isEmpty() && songStanza.musicBy.isEmpty())
                    song_ending = "*    *    *";
            }
        }
    }

    // if not to show song ending, return its space to main text
    if(!songSets.showSongEnding)
        song_ending.clear();

    // Margins:
    int left = 30;
    int top = 20;
    int w = width - left - left;
    int h = height - top - top;
    int maxh = h * songSets.screenUse/100;
    int maxtop; // top of max screen
    if(songSets.screenPositon == 0)
        maxtop  = top;
    if(songSets.screenPositon == 1)
        maxtop = top+h-maxh;

    height = maxh;
    top = maxtop;
    width = w;

    QRect caption_rect, num_rect, main_rect, ending_rect;
    int main_flags(0);

    if(songSets.textAlingmentV==0)
        main_flags += Qt::AlignTop;
    else if(songSets.textAlingmentV==1)
        main_flags += Qt::AlignVCenter;
    else if(songSets.textAlingmentV==2)
        main_flags += Qt::AlignBottom;
    if(songSets.textAlingmentH==0)
        main_flags += Qt::AlignLeft;
    else if(songSets.textAlingmentH==1)
        main_flags += Qt::AlignHCenter;
    else if(songSets.textAlingmentH==2)
        main_flags += Qt::AlignRight;

    QFont main_font = songSets.textFont;

    int caph, endh, mainh, mainw, totalh;

    if(!isTextPrepared)
    {
        sdSets.clear();

        // Prepare Caption
        painter->setFont(songSets.infoFont);
        caption_rect = boundRectOrDrawText(painter, false, left, top, width, height, Qt::AlignLeft | Qt::AlignTop, caption_str);
        caph = caption_rect.height();

        // Prepare Ending
        painter->setFont(songSets.endingFont);
        ending_rect = boundRectOrDrawText(painter, false, left, top, width, height, Qt::AlignHCenter | Qt::AlignTop, song_ending);

        // Decrease songe ending font size so that it would fit in the screen width
        while(ending_rect.width()> width)
        {
            songSets.endingFont.setPointSize(songSets.endingFont.pointSize()-1);
            painter->setFont(songSets.endingFont);
            ending_rect = boundRectOrDrawText(painter, false, left, top, width, height, Qt::AlignHCenter | Qt::AlignTop, song_ending);
        }
        endh = ending_rect.height();

        // Prepare Main Text
        painter->setFont(main_font);
        main_rect = boundRectOrDrawText(painter, false, left, top, width, height, main_flags, main_text);
        mainh = main_rect.height();
        mainw = main_rect.width();
        totalh = caph+endh+mainh;

        // Decrease song text to fit the screen
        while(mainw > width || totalh > height)
        {
            main_font.setPointSize(main_font.pointSize() - 1);
            painter->setFont(main_font);
            main_rect = boundRectOrDrawText(painter, false, left, top, width, height, main_flags, main_text);
            mainh = main_rect.height();
            mainw = main_rect.width();
            totalh = caph+endh+mainh;
        }

        // Check if main font is less then 4/5 of original. if so, then song preparation again with text wrap
        if(main_font.pointSize() <(songSets.textFont.pointSize()*4/5))
        {
            main_flags += Qt::TextWordWrap;
            main_font = songSets.textFont;

            // Prepare Main Text
            painter->setFont(songSets.textFont);
            main_rect = boundRectOrDrawText(painter, false, left, top, width, height, main_flags, main_text);
            mainh = main_rect.height();
            mainw = main_rect.width();
            totalh = caph+endh+mainh;

            // Decrease song text to fit the screen
            while(mainw > width || totalh > height)
            {
                main_font.setPointSize(main_font.pointSize() - 1);
                painter->setFont(main_font);
                main_rect = boundRectOrDrawText(painter, false, left, top, width, height, main_flags, main_text);
                mainh = main_rect.height();
                mainw = main_rect.width();
                totalh = caph+endh+mainh;
            }
        }
        songSets.textFont = main_font;
        isTextPrepared = true;
        sdSets.cRect = caption_rect;
        sdSets.tRect = main_rect;
        sdSets.eRect = ending_rect;
        sdSets.tFlags = main_flags;
    }

    // AT This piont, all sizes should be good.
    // Set object location and DRAW
    caption_rect = sdSets.cRect;
    main_rect = sdSets.tRect;
    ending_rect = sdSets.eRect;
    caph = sdSets.cRect.height();
    endh = sdSets.eRect.height();
    main_flags = sdSets.tFlags;
    mainh = height-caph-endh;
    if(songSets.infoAling == 0 && songSets.endingPosition == 0)
    {
        painter->setFont(songSets.infoFont);
        if(isShadow)
            painter->setPen(QColor(Qt::black));
        else
            painter->setPen(songSets.infoColor);
        caption_rect = boundRectOrDrawText(painter, true, left, top, width, height, Qt::AlignLeft | Qt::AlignTop, caption_str);
        num_rect = boundRectOrDrawText(painter, true, left, top, width, height, Qt::AlignRight | Qt::AlignTop, song_num_str);
        painter->setFont(songSets.textFont);
        if(isShadow)
            painter->setPen(QColor(Qt::black));
        else
            painter->setPen(songSets.textColor);
        main_rect = boundRectOrDrawText(painter, true, left, caption_rect.bottom(), width, mainh, main_flags, main_text);
        painter->setFont(songSets.endingFont);
        if(isShadow)
            painter->setPen(QColor(Qt::black));
        else
            painter->setPen(songSets.endingColor);
        ending_rect = boundRectOrDrawText(painter, true, left, main_rect.bottom(), width, height, Qt::AlignHCenter | Qt::AlignTop, song_ending);
    }
    else if(songSets.infoAling == 0 && songSets.endingPosition == 1)
    {
        painter->setFont(songSets.infoFont);
        if(isShadow)
            painter->setPen(QColor(Qt::black));
        else
            painter->setPen(songSets.infoColor);
        caption_rect = boundRectOrDrawText(painter, true, left, top, width, height, Qt::AlignLeft | Qt::AlignTop, caption_str);
        num_rect = boundRectOrDrawText(painter, true, left, top, width, height, Qt::AlignRight | Qt::AlignTop, song_num_str);
        painter->setFont(songSets.endingFont);
        if(isShadow)
            painter->setPen(QColor(Qt::black));
        else
            painter->setPen(songSets.endingColor);
        ending_rect = boundRectOrDrawText(painter, true, left, top, width, height, Qt::AlignHCenter | Qt::AlignBottom, song_ending);
        painter->setFont(songSets.textFont);
        if(isShadow)
            painter->setPen(QColor(Qt::black));
        else
            painter->setPen(songSets.textColor);
        main_rect = boundRectOrDrawText(painter, true, left, caption_rect.bottom(), width, mainh, main_flags, main_text);
    }
    else if(songSets.infoAling == 1 && songSets.endingPosition == 0)
    {
        painter->setFont(songSets.textFont);
        if(isShadow)
            painter->setPen(QColor(Qt::black));
        else
            painter->setPen(songSets.textColor);
        main_rect = boundRectOrDrawText(painter, true, left, top, width, mainh, main_flags, main_text);
        painter->setFont(songSets.infoFont);
        if(isShadow)
            painter->setPen(QColor(Qt::black));
        else
            painter->setPen(songSets.infoColor);
        caption_rect = boundRectOrDrawText(painter, true, left, top, width, height, Qt::AlignLeft | Qt::AlignBottom, caption_str);
        num_rect = boundRectOrDrawText(painter, true, left, top, width, height, Qt::AlignRight | Qt::AlignBottom, song_num_str);
        painter->setFont(songSets.endingFont);
        if(isShadow)
            painter->setPen(QColor(Qt::black));
        else
            painter->setPen(songSets.endingColor);
        ending_rect = boundRectOrDrawText(painter, true, left, main_rect.bottom(), width, height, Qt::AlignHCenter | Qt::AlignTop, song_ending);
    }
    else if(songSets.infoAling == 1 && songSets.endingPosition == 1)
    {
        endh = height-caph;
        painter->setFont(songSets.textFont);
        if(isShadow)
            painter->setPen(QColor(Qt::black));
        else
            painter->setPen(songSets.textColor);
        main_rect = boundRectOrDrawText(painter, true, left, top, width, mainh, main_flags, main_text);
        painter->setFont(songSets.infoFont);
        if(isShadow)
            painter->setPen(QColor(Qt::black));
        else
            painter->setPen(songSets.infoColor);
        caption_rect = boundRectOrDrawText(painter, true, left, top, width, height, Qt::AlignLeft | Qt::AlignBottom, caption_str);
        num_rect = boundRectOrDrawText(painter, true, left, top, width, height, Qt::AlignRight | Qt::AlignBottom, song_num_str);
        painter->setFont(songSets.endingFont);
        if(isShadow)
            painter->setPen(QColor(Qt::black));
        else
            painter->setPen(songSets.endingColor);
        ending_rect = boundRectOrDrawText(painter, true, left, top, width, endh, Qt::AlignHCenter | Qt::AlignBottom, song_ending);
    }
}

QRect DisplayScreen::boundRectOrDrawText(QPainter *painter, bool draw, int left, int top, int width, int height, int flags, QString text)
{
    // If draw is false, calculate the rectangle that the specified text would be
    // drawn into if it was draw. If draw is true, draw as well.
    // Output rect is returned.

    QRect out_rect;
    if(draw)
        painter->drawText(left, top, width, height, flags, text, &out_rect);
    else
        out_rect = painter->boundingRect(left, top, width, height, flags, text);
    return out_rect;
}

void DisplayScreen::drawAnnounceText(QPainter *painter, int width, int height, bool isShadow)
{
    // Margins:
    int left = 30;
    int top = 20;
    int w = width - left - left;
    int h = height - top - top;

    int flags = Qt::TextWordWrap;
    if(annouceSets.textAlingmentV==0)
        flags += Qt::AlignTop;
    else if(annouceSets.textAlingmentV==1)
        flags += Qt::AlignVCenter;
    else if(annouceSets.textAlingmentV==2)
        flags += Qt::AlignBottom;
    if(annouceSets.textAlingmentH==0)
        flags += Qt::AlignLeft;
    else if(annouceSets.textAlingmentH==1)
        flags += Qt::AlignHCenter;
    else if(annouceSets.textAlingmentH==2)
        flags += Qt::AlignRight;

    QFont font = annouceSets.textFont;
    int orig_font_size = font.pointSize();

    // Keep decreasing the font size until the text fits into the allocated space:
    QRect rect;

    if(!isTextPrepared)
    {
        painter->setFont(font);
        bool exit = false;
        while( !exit )
        {
            rect = painter->boundingRect(left, top, w, h, flags, announcement.text);
            exit = ( rect.width() <= w && rect.height() <= h );
            if( !exit )
            {
                font.setPointSize( font.pointSize()-1 );
                painter->setFont(font);
            }
        }

        // Force wrapping of songs that have really wide lines:
        // (Do not allow font to be shrinked less than a 4/5 of the desired font)
        if( font.pointSize() < (orig_font_size*4/5) )
        {
            font.setPointSize(orig_font_size);
            painter->setFont(font);
            flags = (flags | Qt::TextWordWrap);
            exit = false;
            while( !exit )
            {
                rect = painter->boundingRect(left, top, w, h, flags, announcement.text);
                exit = ( rect.width() <= w && rect.height() <= h );
                if( !exit )
                {
                    font.setPointSize( font.pointSize()-1 );
                    painter->setFont(font);
                }
            }
        }
        annouceSets.textFont = font;
        adSets.tRect = rect;
        isTextPrepared = true;
    }

    painter->setFont(annouceSets.textFont);
    if(isShadow)
        painter->setPen(QColor(Qt::black));
    else
        painter->setPen(annouceSets.textColor);
    painter->drawText(adSets.tRect, flags, announcement.text);
}

void DisplayScreen::setFading(bool fade)
{
    useFading = fade;
}

void DisplayScreen::setDisplaySettings(DisplaySettings sets)
{
}

void DisplayScreen::setNewWallpaper(QString path, bool isToUse)
{
    if(isToUse)
        wallpaperPath = path;
    else
        wallpaperPath.clear();

    if(wallpaperPath.simplified().isEmpty() )
        wallpaper = QPixmap();
    else
    {
        wallpaper.load(wallpaperPath);
        wallpaper = wallpaper.scaled(width(),height());
    }
}

void DisplayScreen::setNewWallpaper(QPixmap wallPix, bool isToUse)
{
    if(isToUse)
        wallpaper = wallPix.scaled(width(),height());
    else
        wallpaper = QPixmap();
}

void DisplayScreen::setNewPassiveWallpaper(QPixmap wallPix, bool isToUse)
{
    if(isToUse)
        //passiveWallpaper = wallPix.scaled(width(),height(),Qt::IgnoreAspectRatio,Qt::SmoothTransformation);
        passiveWallpaper = wallPix;
    else
        passiveWallpaper = QPixmap();
}

void DisplayScreen::paintEvent(QPaintEvent *event )
{
    QPainter painter(this);
    QImage txtPix(width(), height(), QImage::Format_ARGB32);//_Premultiplied);
    txtPix.fill(qRgba(0, 0, 0, 0)); //transparent background
    QPainter txtPainter(&txtPix);

    // This code will, with each iteraction, draw the previous image with increasing transparency, and draw
    // the current image with increasing opacity; making a smooth transition:
    double curr_opacity = acounter[0] / 255.0;
    double prev_opacity = 1.0 - curr_opacity;

    // FIXME transition out of the previous background as well
    // Draw the background at the current opacity:
    if(background_needs_transition)
        painter.setOpacity(curr_opacity);

    if( use_active_background )
    {
        // Draw the active wallpaper if there is text to display
        if (displayType != "pix" && (wallpaper.width()!=width() || wallpaper.isNull()))
        {
            wallpaper.load(wallpaperPath);
            if( !wallpaper.isNull() )
                wallpaper = wallpaper.scaled(width(),height());
        }
        if( ! wallpaper.isNull() )
        {
            int ww = wallpaper.width();
            int wh = wallpaper.height();
            if(displayType == "pix" && ww!=width() && wh!=height())
                painter.drawPixmap((width()-ww)/2,(height()-wh)/2,wallpaper);
            else if(displayType == "pix" && ww!=width())
                painter.drawPixmap((width()-ww)/2,0,wallpaper);
            else if(displayType == "pix" && wh!=height())
                painter.drawPixmap(0,(height()-wh)/2,wallpaper);
            else
                painter.drawPixmap(0,0,wallpaper);
        }
        else
        {
            // Use black for the background:
            painter.setPen(QColor(Qt::black));
            painter.drawRect( 0, 0, width(), height() );
        }
    }
    else
    {
        // Draw the passive wallpaper if set:
        if(!passiveWallpaper.isNull())
        {
            if(passiveWallpaper.width()!=width() || passiveWallpaper.height()!=height())
                passiveWallpaper = passiveWallpaper.scaled(width(),height());

            painter.drawPixmap(0,0, passiveWallpaper);
        }
        else
        {
            // Use black for the background:
            painter.setPen(QColor(Qt::black));
            painter.drawRect( 0, 0, width(), height() );
        }
//        if (passiveWallpaper.width()!=width() || passiveWallpaper.isNull())
//        {
//            passiveWallpaper.load(passiveWallpaperPath);
//            if( !passiveWallpaper.isNull() )
//                //passiveWallpaper = passiveWallpaper.scaled(width(),height(),Qt::IgnoreAspectRatio,Qt::SmoothTransformation);
//                passiveWallpaper = passiveWallpaper.scaled(width(),height());
//        }
//        if( ! passiveWallpaper.isNull() )
//            painter.drawPixmap(0,0, passiveWallpaper);
//        else
//        {
//            // Use black for the background:
//            painter.setPen(QColor(Qt::black));
//            painter.drawRect( 0, 0, width(), height() );
//        }
    }

    // Draw the previous image into the window, at decreasing opacity:
    txtPainter.setOpacity(prev_opacity);
    txtPainter.drawPixmap(0, 0, previous_image_pixmap);

    // Draw the output_image into the window, at increasing opacity:
    txtPainter.setOpacity(curr_opacity);
    txtPainter.drawImage(0, 0, output_image);

    textRenderLabel->setPixmap(QPixmap::fromImage(txtPix));

    // Reset the opacity to default opaque:
    painter.setOpacity(1.0);
    txtPainter.setOpacity(1.0);
}

// Stack Blur Algorithm by Mario Klingemann <mario@quasimondo.com>
void DisplayScreen::fastbluralpha(QImage &img, int radius)
{
    if (radius < 1)
        return;

    QRgb *pix = (QRgb*)img.bits();
    int w   = img.width();
    int h   = img.height();
    int wm  = w-1;
    int hm  = h-1;
    int wh  = w*h;
    int div = radius+radius+1;
    double junk;

    int *r = new int[wh];
    int *g = new int[wh];
    int *b = new int[wh];
    int *a = new int[wh];
    int rsum, gsum, bsum, asum, x, y, i, yp, yi, yw;
    QRgb p;
    int *vmin = new int[qMax(w,h)];

    int divsum = (div+1)>>1;
    divsum *= divsum;
    int *dv = new int[256*divsum];
    for (i=0; i < 256*divsum; ++i)
    {
        dv[i] = (i/divsum);
    }

    yw = yi = 0;

    int **stack = new int*[div];
    for (int i = 0; i < div; ++i)
    {
        stack[i] = new int[4];
    }

    int stackpointer;
    int stackstart;
    int *sir;
    int rbs;
    int r1 = radius+1;
    int routsum, goutsum, boutsum, aoutsum;
    int rinsum, ginsum, binsum, ainsum;

    for (y = 0; y < h; ++y)
    {
        rinsum = ginsum = binsum = ainsum
                = routsum = goutsum = boutsum = aoutsum
                = rsum = gsum = bsum = asum = 0;
        for (i =- radius; i <= radius; ++i)
        {
            p = pix[yi+qMin(wm,qMax(i,0))];
            sir = stack[i+radius];
            sir[0] = qRed(p);
            sir[1] = qGreen(p);
            sir[2] = qBlue(p);
            sir[3] = qAlpha(p);

            rbs = r1-abs(i);
            rsum += sir[0]*rbs;
            gsum += sir[1]*rbs;
            bsum += sir[2]*rbs;
            asum += sir[3]*rbs;

            if (i > 0)
            {
                rinsum += sir[0];
                ginsum += sir[1];
                binsum += sir[2];
                ainsum += sir[3];
            }
            else
            {
                routsum += sir[0];
                goutsum += sir[1];
                boutsum += sir[2];
                aoutsum += sir[3];
            }
        }
        stackpointer = radius;

        for (x=0; x < w; ++x)
        {
            r[yi] = dv[rsum];
            g[yi] = dv[gsum];
            b[yi] = dv[bsum];
            a[yi] = dv[asum];

            rsum -= routsum;
            gsum -= goutsum;
            bsum -= boutsum;
            asum -= aoutsum;

            stackstart = stackpointer-radius+div;
            sir = stack[stackstart%div];

            routsum -= sir[0];
            goutsum -= sir[1];
            boutsum -= sir[2];
            aoutsum -= sir[3];

            if (y == 0)
            {
                vmin[x] = qMin(x+radius+1,wm);
            }
            p = pix[yw+vmin[x]];

            sir[0] = qRed(p);
            sir[1] = qGreen(p);
            sir[2] = qBlue(p);
            sir[3] = qAlpha(p);

            rinsum += sir[0];
            ginsum += sir[1];
            binsum += sir[2];
            ainsum += sir[3];

            rsum += rinsum;
            gsum += ginsum;
            bsum += binsum;
            asum += ainsum;

            stackpointer = (stackpointer+1)%div;
            sir = stack[(stackpointer)%div];

            routsum += sir[0];
            goutsum += sir[1];
            boutsum += sir[2];
            aoutsum += sir[3];

            rinsum -= sir[0];
            ginsum -= sir[1];
            binsum -= sir[2];
            ainsum -= sir[3];

            ++yi;
        }
        yw += w;
    }
    for (x=0; x < w; ++x)
    {
        rinsum = ginsum = binsum = ainsum
                = routsum = goutsum = boutsum = aoutsum
                = rsum = gsum = bsum = asum = 0;

        yp =- radius * w;

        for (i=-radius; i <= radius; ++i)
        {
            yi=qMax(0,yp)+x;

            sir = stack[i+radius];

            sir[0] = r[yi];
            sir[1] = g[yi];
            sir[2] = b[yi];
            sir[3] = a[yi];

            rbs = r1-abs(i);

            rsum += r[yi]*rbs;
            gsum += g[yi]*rbs;
            bsum += b[yi]*rbs;
            asum += a[yi]*rbs;

            if (i > 0)
            {
                rinsum += sir[0];
                ginsum += sir[1];
                binsum += sir[2];
                ainsum += sir[3];
            }
            else
            {
                routsum += sir[0];
                goutsum += sir[1];
                boutsum += sir[2];
                aoutsum += sir[3];
            }

            if (i < hm)
            {
                yp += w;
            }
        }

        yi = x;
        stackpointer = radius;

        for (y=0; y < h; ++y)
        {
            junk=dv[asum];
            junk=junk*2.4;
            if (junk>255)junk=255;
            pix[yi] = qRgba(dv[rsum], dv[gsum], dv[bsum], int(junk));///dv[asum]);

            rsum -= routsum;
            gsum -= goutsum;
            bsum -= boutsum;
            asum -= aoutsum;

            stackstart = stackpointer-radius+div;
            sir = stack[stackstart%div];

            routsum -= sir[0];
            goutsum -= sir[1];
            boutsum -= sir[2];
            aoutsum -= sir[3];

            if (x==0)
            {
                vmin[y] = qMin(y+r1,hm)*w;
            }
            p = x+vmin[y];

            sir[0] = r[p];
            sir[1] = g[p];
            sir[2] = b[p];
            sir[3] = a[p];

            rinsum += sir[0];
            ginsum += sir[1];
            binsum += sir[2];
            ainsum += sir[3];

            rsum += rinsum;
            gsum += ginsum;
            bsum += binsum;
            asum += ainsum;

            stackpointer = (stackpointer+1)%div;
            sir = stack[stackpointer];

            routsum += sir[0];
            goutsum += sir[1];
            boutsum += sir[2];
            aoutsum += sir[3];

            rinsum -= sir[0];
            ginsum -= sir[1];
            binsum -= sir[2];
            ainsum -= sir[3];

            yi += w;
        }
    }
    delete [] r;
    delete [] g;
    delete [] b;
    delete [] a;
    delete [] vmin;
    delete [] dv;
}
*/
