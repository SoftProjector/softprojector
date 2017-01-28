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

#ifndef ANNOUNCEWIDGET_HPP
#define ANNOUNCEWIDGET_HPP

#include <QtGui>
#include "announcement.hpp"
#include "editannouncementdialog.hpp"

namespace Ui {
class AnnounceWidget;
}

class AnnounceWidget : public QWidget {
    Q_OBJECT
    Q_DISABLE_COPY(AnnounceWidget)
public:
    explicit AnnounceWidget(QWidget *parent = 0);
    virtual ~AnnounceWidget();

public slots:
    void loadAnnouncements();
    void newAnnouncement();
    void editAnnouncement();
    void copyAnnouncement();
    void deleteAnnouncement();
    bool isAnnounceValid();
    Announcement getAnnouncement();
    void setAnnouncementFromHistory(Announcement &announce);

protected:
    virtual void changeEvent(QEvent *e);

private slots:
    void loadAnnouncement();
    void announceViewRowChanged(const QModelIndex &current, const QModelIndex &previous);
    void setAnnounceList();
    void setPreview(Announcement announce);
    void addNewAnnouncement(Announcement announce);
    void updateAnnouncement();
    void sendToProjector();
    void on_pushButtonLive_clicked();
    void on_listWidgetAnnouncement_doubleClicked(const QModelIndex &index);
    Announcement currentAnnouncement();
    void on_tableViewAnnouncements_doubleClicked(const QModelIndex &index);

signals:
    void sendAnnounce(Announcement announce, int row);
    void addToSchedule(Announcement &announce);

private:
    Ui::AnnounceWidget *ui;
    EditAnnouncementDialog * editAnounceDialog;
    QList<Announcement> announceList;
    Announcement previewAnnounce;
    AnnounceModel *announceModel;
    AnnounceProxyModel * announceProxy;
};

#endif // ANNOUNCEWIDGET_HPP
