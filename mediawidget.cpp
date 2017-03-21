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

#include <QMediaMetaData>

#include "mediawidget.hpp"
#include "ui_mediawidget.h"

MediaWidget::MediaWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MediaWidget)//,
//    m_AudioOutput(Phonon::VideoCategory)
{
    ui->setupUi(this);
    player = new QMediaPlayer(this);

    connect(player, SIGNAL(durationChanged(qint64)), this, SLOT(updateDuration(qint64)));
    connect(player, SIGNAL(positionChanged(qint64)), this, SLOT(updateTime(qint64)));
    connect(player, SIGNAL(metaDataChanged()), this, SLOT(updateInfo()));
//    connect(player, SIGNAL(mediaStatusChanged(QMediaPlayer::MediaStatus)),
//            this, SLOT(statusChanged(QMediaPlayer::MediaStatus)));
    connect(player, SIGNAL(stateChanged(QMediaPlayer::State)), this, SLOT(stateChanged(QMediaPlayer::State)));
//    connect(player, SIGNAL(bufferStatusChanged(int)), this, SLOT(bufferingProgress(int)));
    connect(player, SIGNAL(videoAvailableChanged(bool)), this, SLOT(hasVideoChanged(bool)));
//    connect(player, SIGNAL(error(QMediaPlayer::Error)), this, SLOT(displayErrorMessage()));

    videoWidget = new VideoPlayerWidget(this);
    player->setVideoOutput(videoWidget);

    ui->horizontalSliderTime->setRange(0,player->duration()/1000);
//    connect(slider, SIGNAL(sliderMoved(int)), this, SLOT(seek(int))); // USE TimeSlider Call

        ui->verticalLayoutMedia->addWidget(videoWidget);
        videoWidget->setVisible(false);


    /**********************************************/

    ui->pushButtonGoLive->setEnabled(false);

    playIcon = QIcon(":icons/icons/play.png");
    pauseIcon = QIcon(":icons/icons/pause.png");

    ui->pushButtonPlayPause->setIcon(playIcon);
//    ui->comboBoxAspectRatio->setEnabled(false);

//    timeSlider = new Phonon::SeekSlider(this);
//    timeSlider->setMediaObject(&mediaPlayer);
//    volumeSlider = new Phonon::VolumeSlider(&m_AudioOutput);

    QPalette palette;
    palette.setBrush(QPalette::WindowText, Qt::white);

    ui->labelInfo->setStyleSheet("border-image:url(:icons/icons/playerScreen.png)");
    ui->labelInfo->setPalette(palette);
    ui->labelInfo->setText(tr("<center>No media</center>"));


//    connect(&mediaPlayer, SIGNAL(metaDataChanged()), this, SLOT(updateInfo()));
//    connect(&mediaPlayer, SIGNAL(totalTimeChanged(qint64)), this, SLOT(updateTime()));
//    connect(&mediaPlayer, SIGNAL(tick(qint64)), this, SLOT(updateTime()));
//    connect(&mediaPlayer, SIGNAL(finished()), this, SLOT(finished()));
//    connect(&mediaPlayer, SIGNAL(stateChanged(Phonon::State,Phonon::State)),
//            this, SLOT(stateChanged(Phonon::State,Phonon::State)));
//    connect(&mediaPlayer, SIGNAL(hasVideoChanged(bool)), this, SLOT(hasVideoChanged(bool)));

//    connect(videoWidget, SIGNAL(playPause()), this, SLOT(playPause()));
//    connect(videoWidget, SIGNAL(handleDrops(QDropEvent*)), this, SLOT(handleDrop(QDropEvent*)));

    ui->pushButtonPlayPause->setEnabled(false);
    setAcceptDrops(true);


    audioExt = "*.mp3 *.acc *.ogg *.oga *.wma *.wav *.asf *.mka";
    videoExt = "*.wmv *.avi *.mkv *.flv *.mp4 *.mpg *.mpeg *.mov *.ogv *.ts";
    loadMediaLibrary();
}

MediaWidget::~MediaWidget()
{
    delete player;
    delete videoWidget;
//    delete timeSlider;
//    delete volumeSlider;
    delete ui;
}

void MediaWidget::changeEvent(QEvent *e)
{
    QWidget::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        ui->retranslateUi(this);
        break;
    default:
        break;
    }
}

void MediaWidget::loadMediaLibrary()
{
    QSqlQuery sq;
    sq.exec("SELECT * FROM Media");
    while(sq.next())
    {
        mediaFilePaths.append(sq.value(0).toString());
        mediaFileNames.append(sq.value(1).toString());
        ui->listWidgetMediaFiles->clear();
        ui->listWidgetMediaFiles->addItems(mediaFileNames);
    }
}


void MediaWidget::stateChanged(QMediaPlayer::State state)
{
    switch (state)
    {
    case QMediaPlayer::StoppedState:
        ui->pushButtonPlayPause->setIcon(playIcon);
        ui->pushButtonPlayPause->setEnabled(true);
        break;
    case QMediaPlayer::PausedState:
        ui->pushButtonPlayPause->setIcon(playIcon);
//        if (mediaPlayer.currentSource().type() != Phonon::MediaSource::Invalid)
//            ui->pushButtonPlayPause->setEnabled(true);
//        else
//            ui->pushButtonPlayPause->setEnabled(false);
        break;
    case QMediaPlayer::PlayingState:
        ui->pushButtonPlayPause->setEnabled(true);
        ui->pushButtonPlayPause->setIcon(pauseIcon);

        break;
    }
    /*
    switch (status)
    {
    case Phonon::ErrorState:
        if (mediaPlayer.errorType() == Phonon::FatalError)
        {
            ui->pushButtonPlayPause->setEnabled(false);
            hasVideoChanged(false);
            ui->labelInfo->setText(QString("<center>%1</center>").arg(tr("No Media")));
        }
        else
            mediaPlayer.pause();

        QMessageBox::warning(this, "softProjector MediaPlayer", mediaPlayer.errorString(), QMessageBox::Close);
        break;

    case Phonon::StoppedState:
        ui->pushButtonPlayPause->setIcon(playIcon);
        ui->pushButtonPlayPause->setEnabled(true);
        break;
    case Phonon::PausedState:
        ui->pushButtonPlayPause->setIcon(playIcon);
        if (mediaPlayer.currentSource().type() != Phonon::MediaSource::Invalid)
            ui->pushButtonPlayPause->setEnabled(true);
        else
            ui->pushButtonPlayPause->setEnabled(false);
        break;
    case Phonon::PlayingState:
        ui->pushButtonPlayPause->setEnabled(true);
        ui->pushButtonPlayPause->setIcon(pauseIcon);

        break;
    case Phonon::BufferingState:

        break;
    case Phonon::LoadingState:

        break;
    }*/
}


void MediaWidget::handleDrop(QDropEvent *e)
{
    QList<QUrl> urls = e->mimeData()->urls();
    QStringList fileList;
    QStringList fileNameList;

    // Make sure that only supported files get to media player
    QString fext = audioExt + " " + videoExt;
    fext.remove("*");
    fext.replace(" ","$|");
    fext.append("$");
    QRegExp rx(fext);
    rx.setCaseSensitivity(Qt::CaseInsensitive);
    foreach(const QUrl &url, urls)
    {
        QString fp = url.toLocalFile();
        if(fp.contains(rx))
        {
            QFileInfo fn(fp);
            fileList.append(fp);
            fileNameList.append(fn.fileName());
        }
    }

    // Add files to library
    if(fileList.count()>0)
    {
        int mcount = ui->listWidgetMediaFiles->count(); // get total media count
        insertFiles(fileList);
        ui->listWidgetMediaFiles->setCurrentRow(mcount); // select first in list of just added and play it
    }
}

void MediaWidget::dropEvent(QDropEvent *e)
{
    if (e->mimeData()->hasUrls() && e->proposedAction() != Qt::LinkAction) {
        e->acceptProposedAction();
        handleDrop(e);
    } else {
        e->ignore();
    }
}

void MediaWidget::dragEnterEvent(QDragEnterEvent *e)
{
    dragMoveEvent(e);
}

void MediaWidget::dragMoveEvent(QDragMoveEvent *e)
{
    if (e->mimeData()->hasUrls()) {
        if (e->proposedAction() == Qt::CopyAction || e->proposedAction() == Qt::MoveAction){
            e->acceptProposedAction();
        }
    }
}

void MediaWidget::playPause()
{
    if (QMediaPlayer::PlayingState == player->state())
        player->pause();
    else
    {
//        if (mediaPlayer.currentTime() == mediaPlayer.totalTime())
//            mediaPlayer.seek(0);
        player->play();
    }
}

void MediaWidget::playFile(QString filePath)
{
    player->stop();
    QUrl fileUrl = QUrl::fromLocalFile(filePath);
    QMediaContent m(fileUrl);
    player->setMedia(m);
    player->play();
//    mediaPlayer.setCurrentSource(Phonon::MediaSource(filePath));
//    mediaPlayer.play();
}

void MediaWidget::updateInfo()
{
    int maxLength = 50;
    QString font = "<font color=#49fff9>";

    QString fName = player->currentMedia().canonicalUrl().fileName();// = mediaPlayer.currentSource().fileName();
//    QFileInfo f(fileName);
//    QString fName = f.fileName();

//    QMap <QString, QString> metaData = mediaPlayer.metaData();
    QString tAlbum = player->metaData(QMediaMetaData::AlbumTitle).toString();
    QString tTitle = player->metaData(QMediaMetaData::Title).toString();
    QString tArtist = player->metaData(QMediaMetaData::AlbumArtist).toString();
    int tBitrate = player->metaData(QMediaMetaData::AudioBitRate).toInt();

    if (fName.length() > maxLength)
        fName = fName.left(maxLength) + "...";

    if (tAlbum.length() > maxLength)
        tAlbum = tAlbum.left(maxLength) + "...";

    if (tTitle.length() > maxLength)
        tTitle = tTitle.left(maxLength) + "...";

    if (tArtist.length() > maxLength)
        tArtist = tArtist.left(maxLength) + "...";

    QString file;
    if(!fName.isEmpty())
        file = "File: " + font + fName + "<br></font>";

    QString album;
    if(!tAlbum.isEmpty())
        album = "Album: " + font + tAlbum + "<br></font>";

    QString title;
    if(!tTitle.isEmpty())
    {
        title = "Title: " + font + tTitle + "<br></font>";
        file.clear();
    }

    QString artist;
    if (!tArtist.isEmpty())
        artist = "Artist: " + font + tArtist + "<br></font>";

    QString bitrate;
    if (tBitrate != 0)
        bitrate = "Bitrate: " + font + QString::number(tBitrate/1000) + "kbit</font>";

    ui->labelInfo->setText(file + album + title + artist + bitrate);

}

void MediaWidget::insertFiles(QStringList &files)
{
    // Insert filenames and path into appropriate lists and database
    QSqlQuery sq;
    foreach(const QString &file, files)
    {
        QFileInfo f(file);
        mediaFileNames.append(f.fileName());
        mediaFilePaths.append(file);
        sq.exec(QString("INSERT INTO Media (long_Path, short_path) VALUES('%1', '%2')").arg(file).arg(f.fileName()));
        ui->listWidgetMediaFiles->addItem(f.fileName());
    }
}

void MediaWidget::updateDuration(qint64 newDuration)
{
    duration = newDuration;
    ui->horizontalSliderTime->setMaximum(newDuration / 1000);
}

void MediaWidget::updateTime(qint64 progress)
{
//    if(ui->horizontalSliderTime->isSliderDown())
//    {
        ui->horizontalSliderTime->setValue(progress/1000);
//    }

    QString timeString;
    if (progress || duration)
    {
        int sec = progress/1000;
        int min = sec/60;
        int hour = min/60;
        int msec = progress;

        QTime playTime(hour%60, min%60, sec%60, msec%1000);
        sec = duration / 1000;
        min = sec / 60;
        hour = min / 60;
        msec = duration;

        QTime stopTime(hour%60, min%60, sec%60, msec%1000);
        QString timeFormat = "mm:ss";
        if (hour > 0)
            timeFormat = "h:mm:ss";
        timeString = playTime.toString(timeFormat);
        if (duration)
            timeString += " / " + stopTime.toString(timeFormat);
    }
    ui->labelTime->setText(timeString);
}

void MediaWidget::finished()
{

}

void MediaWidget::hasVideoChanged(bool bHasVideo)
{
    if(!bHasVideo && videoWidget->isFullScreen())
        videoWidget->setFullScreen(false);
    ui->labelInfo->setVisible(!bHasVideo);
    ui->pushButtonGoLive->setEnabled(bHasVideo);
    videoWidget->setVisible(bHasVideo);
//    ui->comboBoxAspectRatio->setEnabled(bHasVideo);
}

void MediaWidget::prepareForProjection()
{
//    mediaPlayer.pause();
//    QFileInfo fn( mediaPlayer.currentSource().fileName());
//    VideoInfo v;
//    v.aspectRatio = ui->comboBoxAspectRatio->currentIndex();
//    v.fileName = fn.fileName();
//    v.filePath = fn.filePath();
//    emit toProjector(v);
}

void MediaWidget::on_pushButtonOpen_clicked()
{
    QString file = QFileDialog::getOpenFileName(this,tr("Open Music/Video File"),".",
                                                tr("Media Files (%1);;Audio Files (%2);;Video Files (%3)")
                                                .arg(audioExt + " " + videoExt) // media files
                                                .arg(audioExt) // audio files
                                                .arg(videoExt)); // video files
    if(!file.isEmpty())
    {
        ui->listWidgetMediaFiles->clearSelection();
        playFile(file);       
    }
}

void MediaWidget::on_pushButtonPlayPause_clicked()
{
    playPause();
}

void MediaWidget::on_pushButtonGoLive_clicked()
{
    prepareForProjection();
}

void MediaWidget::addToLibrary()
{
    QStringList mfp = QFileDialog::getOpenFileNames(this,tr("Select Music/Video Files to Open"),".",
                                                    tr("Media Files (%1);;Audio Files (%2);;Video Files (%3)")
                                                    .arg(audioExt + " " + videoExt) // media files
                                                    .arg(audioExt) // audio files
                                                    .arg(videoExt)); // video files

    if(mfp.count()>0)
        insertFiles(mfp);
}

void MediaWidget::removeFromLibrary()
{
    int cm = ui->listWidgetMediaFiles->currentRow();
    if(cm>=0)
    {
        QSqlQuery sq;
        sq.exec("DELETE FROM Media WHERE short_path = '" +mediaFileNames.at(cm)+ "'");
        mediaFilePaths.removeAt(cm);
        mediaFileNames.removeAt(cm);

        ui->listWidgetMediaFiles->setCurrentRow(-1);
        ui->listWidgetMediaFiles->clear();
        if(mediaFileNames.count()>0)
            ui->listWidgetMediaFiles->addItems(mediaFileNames);
//        mediaPlayer.stop();

        hasVideoChanged(false);

        ui->labelInfo->setText(tr("<center>No media</center>"));
    }
}

void MediaWidget::on_listWidgetMediaFiles_itemSelectionChanged()
{
    int cRow = ui->listWidgetMediaFiles->currentRow();
    if(cRow>=0)
        playFile(mediaFilePaths.at(cRow));
}

void MediaWidget::on_listWidgetMediaFiles_doubleClicked(const QModelIndex &index)
{
    if(ui->pushButtonGoLive->isEnabled())
        prepareForProjection();
}

VideoInfo MediaWidget::getMedia()
{
    VideoInfo v;
    int cm = ui->listWidgetMediaFiles->currentRow();
    v.fileName = mediaFileNames.at(cm);
    v.filePath = mediaFilePaths.at(cm);
    return v;
}

void MediaWidget::setMediaFromSchedule(VideoInfo &v)
{
    ui->listWidgetMediaFiles->clearSelection();
    playFile(v.filePath);
    player->pause();
}

void MediaWidget::goLiveFromSchedule()
{
    if(ui->pushButtonGoLive->isEnabled())
        prepareForProjection();
    else
        player->play();
}

bool MediaWidget::isValidMedia()
{
    if(ui->listWidgetMediaFiles->selectionModel()->selectedRows().count() > 0)
        return true;
    else
        return false;
}

void MediaWidget::on_horizontalSliderTime_sliderMoved(int position)
{
    player->setPosition(position * 1000);
}
