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

#include "passivesettingwidget.hpp"
#include "ui_passivesettingwidget.h"

PassiveSettingWidget::PassiveSettingWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::PassiveSettingWidget)
{
    ui->setupUi(this);
}

PassiveSettingWidget::~PassiveSettingWidget()
{
    delete ui;
}

void PassiveSettingWidget::changeEvent(QEvent *e)
{
    QWidget::changeEvent(e);
     switch ( e->type() ) {
     case QEvent::LanguageChange:
         ui->retranslateUi(this);
         break;
     default:
         break;
     }
}

void PassiveSettingWidget::setSetings(TextSettings &settings, TextSettings &settings2, TextSettings &settings3, TextSettings &settings4)
{
    mySettings = settings;
    mySettings2 = settings2;
    mySettings3 = settings3;
    mySettings4 = settings4;
    loadSettings();
}

void PassiveSettingWidget::getSettings(TextSettings &settings, TextSettings &settings2, TextSettings &settings3, TextSettings &settings4)
{
    mySettings.useBackground = ui->groupBoxBackground->isChecked();
    mySettings.backgroundName = ui->lineEditBackgroundPath->text();

    // Get display two settings
    mySettings2.useDisp1settings = !ui->groupBoxDisp2Sets->isChecked();
    mySettings2.useDisp2settings = ui->groupBoxDisp2Sets->isChecked();
    mySettings2.useBackground = ui->groupBoxBackground2->isChecked();
    mySettings2.backgroundName = ui->lineEditBackgroundPath2->text();

    // Get display three settings
    mySettings3.useDisp1settings = !ui->groupBoxDisp3Sets->isChecked();
    mySettings3.useDisp3settings = ui->groupBoxDisp3Sets->isChecked();
    mySettings3.useBackground = ui->groupBoxBackground3->isChecked();
    mySettings3.backgroundName = ui->lineEditBackgroundPath3->text();

    // Get display four settings
    mySettings4.useDisp1settings = !ui->groupBoxDisp4Sets->isChecked();
    mySettings4.useDisp4settings = ui->groupBoxDisp4Sets->isChecked();
    mySettings4.useBackground = ui->groupBoxBackground4->isChecked();
    mySettings4.backgroundName = ui->lineEditBackgroundPath4->text();

    settings = mySettings;
    settings2 = mySettings2;
    settings3 = mySettings3;
    settings4 = mySettings4;
}

void PassiveSettingWidget::loadSettings()
{
    ui->groupBoxBackground->setChecked(mySettings.useBackground);
    ui->lineEditBackgroundPath->setText(mySettings.backgroundName);

    // Displpay screen two
    ui->groupBoxDisp2Sets->setChecked(!mySettings2.useDisp1settings);
    ui->groupBoxBackground2->setChecked(mySettings2.useBackground);
    ui->lineEditBackgroundPath2->setText(mySettings2.backgroundName);

    // Displpay screen three
    ui->groupBoxDisp3Sets->setChecked(!mySettings3.useDisp1settings);
    ui->groupBoxBackground3->setChecked(mySettings3.useBackground);
    ui->lineEditBackgroundPath3->setText(mySettings3.backgroundName);

    // Displpay screen four
    ui->groupBoxDisp4Sets->setChecked(!mySettings4.useDisp1settings);
    ui->groupBoxBackground4->setChecked(mySettings4.useBackground);
    ui->lineEditBackgroundPath4->setText(mySettings4.backgroundName);
}

void PassiveSettingWidget::setDispScreen2Visible(bool visible)
{
    ui->groupBoxDisp2Sets->setVisible(visible);
}

void PassiveSettingWidget::setDispScreen3Visible(bool visible)
{
    ui->groupBoxDisp3Sets->setVisible(visible);
}

void PassiveSettingWidget::setDispScreen4Visible(bool visible)
{
    ui->groupBoxDisp4Sets->setVisible(visible);
}

void PassiveSettingWidget::on_buttonBrowseBackgound_clicked()
{
    QString filename = QFileDialog::getOpenFileName(this, tr("Select a image for passive wallpaper"),
                                                    ".", tr("Images(%1)").arg(getSupportedImageFormats()));
    if(!filename.isNull())
    {
        QPixmap p(filename);
        mySettings.backgroundPix = p;
        QFileInfo fi(filename);
        filename = fi.fileName();
        mySettings.backgroundName = filename;
        ui->lineEditBackgroundPath->setText(filename);
    }
}

void PassiveSettingWidget::on_groupBoxDisp2Sets_toggled(bool arg1)
{
    ui->groupBoxBackground2->setVisible(arg1);
}

void PassiveSettingWidget::on_groupBoxDisp3Sets_toggled(bool arg1)
{
    ui->groupBoxBackground3->setVisible(arg1);
}

void PassiveSettingWidget::on_groupBoxDisp4Sets_toggled(bool arg1)
{
    ui->groupBoxBackground4->setVisible(arg1);
}

void PassiveSettingWidget::on_buttonBrowseBackgound2_clicked()
{
    QString filename = QFileDialog::getOpenFileName(this, tr("Select a image for passive wallpaper"),
                                                    ".", tr("Images(%1)").arg(getSupportedImageFormats()));
    if(!filename.isNull())
    {
        QPixmap p(filename);
        mySettings2.backgroundPix = p;
        QFileInfo fi(filename);
        filename = fi.fileName();
        mySettings2.backgroundName = filename;
        ui->lineEditBackgroundPath2->setText(filename);
    }
}

void PassiveSettingWidget::on_buttonBrowseBackgound3_clicked()
{
    QString filename = QFileDialog::getOpenFileName(this, tr("Select a image for passive wallpaper"),
                                                    ".", tr("Images(%1)").arg(getSupportedImageFormats()));
    if(!filename.isNull())
    {
        QPixmap p(filename);
        mySettings3.backgroundPix = p;
        QFileInfo fi(filename);
        filename = fi.fileName();
        mySettings3.backgroundName = filename;
        ui->lineEditBackgroundPath3->setText(filename);
    }
}

void PassiveSettingWidget::on_buttonBrowseBackgound4_clicked()
{
    QString filename = QFileDialog::getOpenFileName(this, tr("Select a image for passive wallpaper"),
                                                    ".", tr("Images(%1)").arg(getSupportedImageFormats()));
    if(!filename.isNull())
    {
        QPixmap p(filename);
        mySettings4.backgroundPix = p;
        QFileInfo fi(filename);
        filename = fi.fileName();
        mySettings4.backgroundName = filename;
        ui->lineEditBackgroundPath4->setText(filename);
    }
}

void PassiveSettingWidget::on_pushButtonDefault_clicked()
{
    TextSettings p;
    mySettings = p;
    mySettings2 = p;
    mySettings3 = p;
    mySettings4 = p;
    loadSettings();
}
