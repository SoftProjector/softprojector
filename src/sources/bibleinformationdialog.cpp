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

#include "bibleinformationdialog.hpp"
#include "ui_bibleinformationdialog.h"

BibleInformationDialog::BibleInformationDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::BibleInformationDialog)
{
    ui->setupUi(this);
}

BibleInformationDialog::~BibleInformationDialog()
{
    delete ui;
}

void BibleInformationDialog::on_buttonBox_accepted()
{
    title = ui->bible_name_lineEdit->text().trimmed();
    abbr = ui->abbr_lineEdit->text().trimmed();
    info = ui->info_TextEdit->toPlainText().trimmed();
    if (ui->rtol_checkBox->isChecked())
        isRtoL = true;
    else
        isRtoL = false;

    close();
}

void BibleInformationDialog::on_buttonBox_rejected()
{
    close();
}

void BibleInformationDialog::setBibleIformation(QString _title, QString _abbr, QString _info, bool is_rtol)
{
   ui->bible_name_lineEdit->setText(_title);
   ui->abbr_lineEdit->setText(_abbr);
   ui->info_TextEdit->setPlainText(_info);
   ui->rtol_checkBox->setChecked(is_rtol);
}
