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

#ifndef MEDIACONTROL_HPP
#define MEDIACONTROL_HPP

#include <QWidget>
#include <QMediaPlayer>
#include <QIcon>
#include <QTime>

namespace Ui {
class MediaControl;
}

class MediaControl : public QWidget
{
    Q_OBJECT

public:
    explicit MediaControl(QWidget *parent = 0);
    ~MediaControl();

public slots:
    void setVolume(int level);
    void updateTime(qint64 time);
    void setMaximumTime(qint64 maxTime);
    void updatePlayerState(QMediaPlayer::State state);

signals:
    void volumeChanged(int);
    void timeChanged(qint64);
    void play();
    void pause();
    void stop();
    void muted(bool muted);

private slots:
    void on_pushButtonStop_clicked();
    void on_pushButtonPlayPause_clicked();
    void on_pushButtonMute_toggled(bool checked);
    void on_horizontalSliderTime_sliderReleased();
    void on_horizontalSliderVolume_sliderMoved(int position);



private:
    Ui::MediaControl *ui;
    QMediaPlayer::State mPlayerState;
    qint64 mDuration;

    QIcon mPlayIcon;
    QIcon mPauseIcon;
    QIcon mMuteIcon;
    QIcon mUnmuteIcon;
};

#endif // MEDIACONTROL_HPP
