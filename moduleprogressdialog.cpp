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

#include "moduleprogressdialog.hpp"
#include "ui_moduleprogressdialog.h"

ModuleProgressDialog::ModuleProgressDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ModuleProgressDialog)
{
    ui->setupUi(this);
    clearAll();
}

ModuleProgressDialog::~ModuleProgressDialog()
{
    delete ui;
}

void ModuleProgressDialog::setCurrentValue(int value)
{
    ui->progressBarCurrent->setValue(value);
}

void ModuleProgressDialog::setCurrentMax(int max)
{
    ui->progressBarCurrent->setMaximum(max);
}

void ModuleProgressDialog::setCurrentMin(int min)
{
    ui->progressBarCurrent->setMinimum(min);
}

void ModuleProgressDialog::setCurrent(int value, int max)
{
    ui->progressBarCurrent->setMaximum(max);
    ui->progressBarCurrent->setValue(value);
}

void ModuleProgressDialog::setTotalValue(int value)
{
    ui->progressBarTotal->setValue(value);
}

void ModuleProgressDialog::setTotalMax(int max)
{
    ui->progressBarTotal->setMaximum(max);
}

void ModuleProgressDialog::setTotal(int value, int max)
{
    ui->progressBarTotal->setMaximum(max);
    ui->progressBarTotal->setValue(value);
}

void ModuleProgressDialog::increaseTotal()
{
    int c = ui->progressBarTotal->value();
    ++c;
    ui->progressBarTotal->setValue(c);
}

void ModuleProgressDialog::setSpeed(QString speed)
{
    ui->labelSpeed->setText(speed);
}

void ModuleProgressDialog::appendText(QString text)
{
    ui->plainTextEdit->appendPlainText(text);
}

void ModuleProgressDialog::clearAll()
{
    ui->plainTextEdit->clear();
    ui->labelSpeed->clear();
    ui->progressBarCurrent->setValue(0);
    ui->progressBarTotal->setValue(0);
    ui->pushButton->setEnabled(false);
}

void ModuleProgressDialog::enableCloseButton(bool enable)
{
    ui->pushButton->setEnabled(enable);
}

void ModuleProgressDialog::setToMax()
{
    ui->progressBarCurrent->setMaximum(1);
    ui->progressBarCurrent->setValue(1);
    ui->progressBarTotal->setMaximum(1);
    ui->progressBarTotal->setValue(1);
}

void ModuleProgressDialog::on_pushButton_clicked()
{
    close();
}
