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

#include "../headers/mediacontrol.hpp"
#include "ui_mediacontrol.h"
#include <QTimer>
#include <QMouseEvent>
#include <QStyle>

bool MediaControl::isMuted() const
{
    return ui->pushButtonMute->isChecked();
}

MediaControl::MediaControl(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MediaControl)
{
    ui->setupUi(this);

    mPlayerState = QMediaPlayer::StoppedState;
    mDuration = 0;

    // Setup Icons
    mPlayIcon = QIcon(":icons/icons/play.png");
    mPauseIcon = QIcon(":icons/icons/pause.png");
    mMuteIcon = QIcon(":icons/icons/speakerMute.png");
    mUnmuteIcon = QIcon(":icons/icons/speaker.png");
    
    ui->horizontalSliderTime->installEventFilter(this);
}

MediaControl::~MediaControl()
{
    delete ui;
}

void MediaControl::setVolume(int level)
{
    ui->horizontalSliderVolume->setValue(level);
}

void MediaControl::updateTime(qint64 time)
{
    if (mIsSeeking) return;   // don't fight the user's drag

    // Throttle slider updates to avoid UI stuttering with millisecond resolution
    if (qAbs(ui->horizontalSliderTime->value() - time) > 100 || time == 0) {
        ui->horizontalSliderTime->setValue(time);
    }

    QString timeString;
    if (time || mDuration)
    {
        int sec = time/1000;
        int min = sec/60;
        int hour = min/60;
        int msec = time;

        QTime playTime(hour%60, min%60, sec%60, msec%1000);
        sec = mDuration / 1000;
        min = sec / 60;
        hour = min / 60;
        msec = mDuration;

        QTime stopTime(hour%60, min%60, sec%60, msec%1000);
        QString timeFormat = "mm:ss";
        if (hour > 0)
        {
            timeFormat = "h:mm:ss";
        }

        timeString = playTime.toString(timeFormat);
        if (mDuration)
        {
            timeString += " / " + stopTime.toString(timeFormat);
        }
    }
    
    if (m_lastTimeString != timeString) {
        ui->labelTime->setText(timeString);
        m_lastTimeString = timeString;
    }
}

void MediaControl::setMaximumTime(qint64 maxTime)
{
    mDuration = maxTime;
    ui->horizontalSliderTime->setMaximum(maxTime);
}

void MediaControl::updatePlayerState(QMediaPlayer::PlaybackState state)
{
    mPlayerState = state;

    switch (mPlayerState)
    {
        case QMediaPlayer::StoppedState:
    case QMediaPlayer::PausedState:
        ui->pushButtonPlayPause->setIcon(mPlayIcon);
        break;
    case QMediaPlayer::PlayingState:
        ui->pushButtonPlayPause->setIcon(mPauseIcon);
        break;
    default:
        ui->pushButtonPlayPause->setIcon(mPlayIcon);
        break;
    }
}

void MediaControl::on_pushButtonStop_clicked()
{
    emit stop();
}

void MediaControl::on_pushButtonPlayPause_clicked()
{
    if(QMediaPlayer::PlayingState == mPlayerState)
    {
        emit pause();
    }
    else
    {
        emit play();
    }
}

void MediaControl::on_pushButtonMute_toggled(bool checked)
{
    emit muted(checked);

    ui->horizontalSliderVolume->setEnabled(!checked);

    if(checked)
    {
        ui->pushButtonMute->setIcon(mMuteIcon);
    }
    else
    {
        ui->pushButtonMute->setIcon(mUnmuteIcon);
    }
}

void MediaControl::on_horizontalSliderTime_sliderPressed()
{
    mIsSeeking = true;   // block position updates from the player
}

void MediaControl::on_horizontalSliderTime_sliderReleased()
{
    // Delayed release of mIsSeeking to avoid jump-back/stuttering UI.
    // Give the player time (300ms) to update its position before resume UI synchronization.
    QTimer::singleShot(300, this, [this](){ mIsSeeking = false; });

    emit timeChanged(ui->horizontalSliderTime->value());
}

void MediaControl::on_horizontalSliderVolume_sliderMoved(int position)
{
    emit volumeChanged(position);
}

bool MediaControl::eventFilter(QObject *obj, QEvent *event)
{
    if (obj == ui->horizontalSliderTime && event->type() == QEvent::MouseButtonPress)
    {
        QMouseEvent *mouseEvent = static_cast<QMouseEvent *>(event);
        if (mouseEvent->button() == Qt::LeftButton)
        {
            // Calculate new value based on click position
            int newVal = QStyle::sliderValueFromPosition(ui->horizontalSliderTime->minimum(), 
                                                        ui->horizontalSliderTime->maximum(), 
                                                        mouseEvent->position().toPoint().x(), 
                                                        ui->horizontalSliderTime->width());
            
            mIsSeeking = true;
            ui->horizontalSliderTime->setValue(newVal);
            emit timeChanged((qint64)newVal);
            
            // Allow 500ms for seeking to settle before resuming UI position synchronization
            QTimer::singleShot(500, this, [this](){ mIsSeeking = false; });
            return true; // event handled
        }
    }
    return QWidget::eventFilter(obj, event);
}

