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

#include "moduledownloaddialog.hpp"
#include "ui_moduledownloaddialog.h"

ModuleDownloadDialog::ModuleDownloadDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ModuleDownloadDialog)
{
    ui->setupUi(this);
}

ModuleDownloadDialog::~ModuleDownloadDialog()
{
    delete ui;
}

void ModuleDownloadDialog::setList(QStringList &modList)
{
    ui->listWidget->clear();
    foreach(const QString &mod, modList)
        ui->listWidget->addItem(mod);
 }

QList<int> ModuleDownloadDialog::getSelected()
{
    QList<int> mods;
    int count = ui->listWidget->count();
    for(int i(0);count>i;++i)
    {
        if(ui->listWidget->item(i)->isSelected())
            mods.append(i);
    }
    return mods;
}

void ModuleDownloadDialog::on_buttonBox_accepted()
{

}

void ModuleDownloadDialog::on_buttonBox_rejected()
{

}

void ModuleDownloadDialog::on_pushButtonSelectAll_clicked()
{
    ui->listWidget->selectAll();
}

void ModuleDownloadDialog::on_pushButtonDeselectAll_clicked()
{
    ui->listWidget->clearSelection();
}
