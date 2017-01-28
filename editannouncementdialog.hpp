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

#ifndef EDITANNOUNCEMENTDIALOG_HPP
#define EDITANNOUNCEMENTDIALOG_HPP

#include <QDialog>
#include <QDebug>
#include "announcement.hpp"
#include "highlight.hpp"

namespace Ui {
class EditAnnouncementDialog;
}

class EditAnnouncementDialog : public QDialog
{
    Q_OBJECT
    
public:
    explicit EditAnnouncementDialog(QWidget *parent = 0);
    ~EditAnnouncementDialog();

public slots:
    void setNewAnnouce();
    void setEditAnnouce(Announcement &editAnnoucement);
    void setCopyAnnouce(Announcement &copyAnnoucement);

signals:
    void announceToAdd(Announcement announce);
    void announceToUpdate();
    
private slots:
    void on_checkBoxUsePrivateSettings_stateChanged(int arg1);
    void on_checkBoxTimedSlides_stateChanged(int arg1);
    void on_pushButtonSave_clicked();
    void on_pushButtonCancel_clicked();

    void resetUiItems();
    void setUiItems();
    bool setSave();

private:
    Ui::EditAnnouncementDialog *ui;
    Announcement editAnnounce;
    bool isNew;
    HighlightAnnounce * highlight;
protected:
    virtual void changeEvent(QEvent *e);
};

#endif // EDITANNOUNCEMENTDIALOG_HPP
