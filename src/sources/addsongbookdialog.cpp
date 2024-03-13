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

#include "addsongbookdialog.hpp"
#include "ui_addsongbookdialog.h"

AddSongbookDialog::AddSongbookDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AddSongbookDialog)
{
    ui->setupUi(this);
}

AddSongbookDialog::~AddSongbookDialog()
{
    delete ui;
}

void AddSongbookDialog::changeEvent(QEvent *e)
{
    QDialog::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        ui->retranslateUi(this);
        break;
    default:
        break;
    }
}

void AddSongbookDialog::on_buttonBox_accepted()
{
    title = ui->songbook_title_box->text().trimmed();
    info = ui->songbook_info_box->toPlainText().trimmed();
    close();
}

void AddSongbookDialog::on_buttonBox_rejected()
{
    close();
}

void AddSongbookDialog::setSongbook(QString i_title, QString i_info)
{
    ui->songbook_title_box->setText(i_title);
    ui->songbook_info_box->setPlainText(i_info);
}

void AddSongbookDialog::setWindowText(QString lblName, QString lblInfo)
{
    ui->sbonik_title_label->setText(lblName);
    ui->songbook_info_label->setText(lblInfo);
}
