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

#ifndef ADDSONGBOOKDIALOG_HPP
#define ADDSONGBOOKDIALOG_HPP

#include <QDialog>

namespace Ui {
    class AddSongbookDialog;
}

class AddSongbookDialog : public QDialog {
    Q_OBJECT
    Q_DISABLE_COPY(AddSongbookDialog)
public:
    QString title;
    QString info;
    explicit AddSongbookDialog(QWidget *parent = 0);
    virtual ~AddSongbookDialog();

public slots:
    void setSongbook(QString i_title, QString i_info);
    void setWindowText(QString lblName, QString lblInfo);

protected:
    virtual void changeEvent(QEvent *e);

private:
    Ui::AddSongbookDialog *ui;

private slots:
    void on_buttonBox_rejected();
    void on_buttonBox_accepted();
};

#endif // ADDSONGBOOKDIALOG_HPP
