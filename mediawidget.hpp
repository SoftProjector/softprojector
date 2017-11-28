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

#ifndef MEDIAWIDGET_HPP
#define MEDIAWIDGET_HPP

#include <QtWidgets>
#include <QMediaPlayer>
#include <QtSql>
//#include <phonon>
//#include <phonon/MediaObject>
//#include <phonon/AudioOutput>
//#include <phonon/SeekSlider>
//#include <phonon/VolumeSlider>
#include "mediacontrol.hpp"
#include "videoplayerwidget.hpp"
#include "videoinfo.hpp"

namespace Ui {
class MediaWidget;
}

class MediaWidget : public QWidget
{
    Q_OBJECT

public:
    explicit MediaWidget(QWidget *parent = 0);
    ~MediaWidget();

public slots:
    void addToLibrary();
    void removeFromLibrary();
    VideoInfo getMedia();
    void setMediaFromSchedule(VideoInfo &v);
    void goLiveFromSchedule();
    bool isValidMedia();
protected:
    void dragEnterEvent(QDragEnterEvent *e);
    void dragMoveEvent(QDragMoveEvent *e);
    void dropEvent(QDropEvent *e);
    void changeEvent(QEvent *e);

signals:
    void toProjector(VideoInfo &vid);

private slots:
    void playFile(QUrl filePath);
    void updateInfo();

    void handleDrop(QDropEvent *e);
    void loadMediaLibrary();
    void statusChanged(QMediaPlayer::MediaStatus status);
    void displayErrorMessage();

    void hasVideoChanged(bool);
    void insertFiles(QStringList &files);
    void prepareForProjection();

//    void on_pushButtonOpen_clicked();
    void on_pushButtonGoLive_clicked();
    void on_listWidgetMediaFiles_itemSelectionChanged();
    void on_listWidgetMediaFiles_doubleClicked(const QModelIndex &index);

private:
    Ui::MediaWidget *ui;

    QIcon playIcon;
    QIcon pauseIcon;
    QIcon muteIcon;
    QIcon unmuteIcon;

    QMediaPlayer *player;
    VideoPlayerWidget *videoWidget;
    MediaControl *mediaControls;

    qint64 duration;

    QString audioExt;
    QString videoExt;
    QList<QUrl> mediaFilePaths;
    QStringList mediaFileNames;
    QUrl currentMediaUrl;

    bool isReadyToPlay;
};

#endif // MEDIAWIDGET_HPP
