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

#include "announcementsettingwidget.hpp"
#include "ui_announcementsettingwidget.h"

AnnouncementSettingWidget::AnnouncementSettingWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::AnnouncementSettingWidget)
{
    ui->setupUi(this);
}

AnnouncementSettingWidget::~AnnouncementSettingWidget()
{
    delete ui;
}

void AnnouncementSettingWidget::changeEvent(QEvent *e)
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

void AnnouncementSettingWidget::setSettings(TextSettings &settings, TextSettings &settings2)
{
    mySettings = settings;
    mySettings2 = settings2;
    loadSettings();
}

void AnnouncementSettingWidget::loadSettings()
{
    // Set Effects
    ui->checkBoxUseShadow->setChecked(mySettings.useShadow);
    ui->checkBoxUseFading->setChecked(mySettings.useFading);
    ui->checkBoxUseBlurredShadow->setChecked(mySettings.useBlurShadow);

    ui->checkBoxUseShadow2->setChecked(mySettings2.useShadow);
    ui->checkBoxUseFading2->setChecked(mySettings2.useFading);
    ui->checkBoxUseBlurredShadow2->setChecked(mySettings2.useBlurShadow);

    // Set background
    ui->groupBoxBackground->setChecked(mySettings.useBackground);
    ui->lineEditBackground->setText(mySettings.backgroundName);

    ui->groupBoxBackground2->setChecked(mySettings2.useBackground);
    ui->lineEditBackground2->setText(mySettings2.backgroundName);

    // Set Alignment
    ui->comboBoxVerticalAling->setCurrentIndex(mySettings.textAlignmentV);
    ui->comboBoxHorizontalAling->setCurrentIndex(mySettings.textAlignmentH);

    ui->comboBoxVerticalAling2->setCurrentIndex(mySettings2.textAlignmentV);
    ui->comboBoxHorizontalAling2->setCurrentIndex(mySettings2.textAlignmentH);

    // Set text color
    QPalette p;
    p.setColor(QPalette::Base,mySettings.textColor);
    ui->graphicViewTextColor->setPalette(p);

    p.setColor(QPalette::Base,mySettings2.textColor);
    ui->graphicViewTextColor2->setPalette(p);

    // Set text font lable
    ui->labelFont->setText(getFontText(mySettings.textFont));
    ui->labelFont2->setText(getFontText(mySettings2.textFont));

    ui->groupBoxUseDisp2->setChecked(mySettings2.useDisp2settings);
    on_groupBoxUseDisp2_toggled(mySettings2.useDisp2settings);
}

void AnnouncementSettingWidget::getSettings(TextSettings &settings, TextSettings &settings2)
{
    // Effects
    mySettings.useShadow = ui->checkBoxUseShadow->isChecked();
    mySettings.useFading = ui->checkBoxUseFading->isChecked();
    mySettings.useBlurShadow = ui->checkBoxUseBlurredShadow->isChecked();

    mySettings2.useShadow = ui->checkBoxUseShadow2->isChecked();
    mySettings2.useFading = ui->checkBoxUseFading2->isChecked();
    mySettings2.useBlurShadow = ui->checkBoxUseBlurredShadow2->isChecked();

    // Get Background
    mySettings.useBackground = ui->groupBoxBackground->isChecked();
    mySettings.backgroundName = ui->lineEditBackground->text();

    mySettings2.useBackground = ui->groupBoxBackground2->isChecked();
    mySettings2.backgroundName = ui->lineEditBackground2->text();

    // Alingmet
    mySettings.textAlignmentV = ui->comboBoxVerticalAling->currentIndex();
    mySettings.textAlignmentH = ui->comboBoxHorizontalAling->currentIndex();

    mySettings2.textAlignmentV = ui->comboBoxVerticalAling2->currentIndex();
    mySettings2.textAlignmentH = ui->comboBoxHorizontalAling2->currentIndex();

    mySettings2.useDisp2settings = ui->groupBoxUseDisp2->isChecked();

    settings = mySettings;
    settings2 = mySettings2;
}

void AnnouncementSettingWidget::on_checkBoxUseShadow_stateChanged(int arg1)
{
    if(arg1==2)
        ui->checkBoxUseBlurredShadow->setEnabled(true);
    else
    {
        ui->checkBoxUseBlurredShadow->setChecked(false);
        ui->checkBoxUseBlurredShadow->setEnabled(false);
    }
}

void AnnouncementSettingWidget::on_checkBoxUseShadow2_stateChanged(int arg1)
{
    if(arg1==2)
        ui->checkBoxUseBlurredShadow2->setEnabled(true);
    else
    {
        ui->checkBoxUseBlurredShadow2->setChecked(false);
        ui->checkBoxUseBlurredShadow2->setEnabled(false);
    }
}

void AnnouncementSettingWidget::on_buttonBackground_clicked()
{
    QString filename = QFileDialog::getOpenFileName(this, tr("Select a image for announcement wallpaper"),
                                                    ".", tr("Images(%1)").arg(getSupportedImageFormats()));
    if(!filename.isNull())
    {
        QPixmap p(filename);
        mySettings.backgroundPix = p;
        QFileInfo fi(filename);
        filename = fi.fileName();
        mySettings.backgroundName = filename;
        ui->lineEditBackground->setText(filename);
    }
}

void AnnouncementSettingWidget::on_buttonBackground2_clicked()
{
    QString filename = QFileDialog::getOpenFileName(this, tr("Select a image for announcement wallpaper"),
                                                    ".", tr("Images(%1)").arg(getSupportedImageFormats()));
    if(!filename.isNull())
    {
        QPixmap p(filename);
        mySettings2.backgroundPix = p;
        QFileInfo fi(filename);
        filename = fi.fileName();
        mySettings2.backgroundName = filename;
        ui->lineEditBackground2->setText(filename);
    }
}

void AnnouncementSettingWidget::setDispScreen2Visible(bool visible)
{
    ui->groupBoxUseDisp2->setVisible(visible);
}

void AnnouncementSettingWidget::on_toolButtonColor_clicked()
{
    QColor c(QColorDialog::getColor(mySettings.textColor,this));
    if(c.isValid())
        mySettings.textColor = c;
    QPalette p;
    p.setColor(QPalette::Base,mySettings.textColor);
    ui->graphicViewTextColor->setPalette(p);
}

void AnnouncementSettingWidget::on_toolButtonColor2_clicked()
{
    QColor c(QColorDialog::getColor(mySettings2.textColor,this));
    if(c.isValid())
        mySettings2.textColor = c;
    QPalette p;
    p.setColor(QPalette::Base,mySettings2.textColor);
    ui->graphicViewTextColor2->setPalette(p);
}

void AnnouncementSettingWidget::on_toolButtonFont_clicked()
{
    bool ok;
    QFont font = QFontDialog::getFont(&ok,mySettings.textFont,this);
    if(ok)
        mySettings.textFont = font;

    ui->labelFont->setText(getFontText(mySettings.textFont));
}

void AnnouncementSettingWidget::on_toolButtonFont2_clicked()
{
    bool ok;
    QFont font = QFontDialog::getFont(&ok,mySettings2.textFont,this);
    if(ok)
        mySettings2.textFont = font;

    ui->labelFont2->setText(getFontText(mySettings2.textFont));
}

void AnnouncementSettingWidget::on_groupBoxUseDisp2_toggled(bool arg1)
{
    ui->groupBoxBackground2->setVisible(arg1);
    ui->groupBoxEffects2->setVisible(arg1);
    ui->groupBoxTextProperties2->setVisible(arg1);
}

void AnnouncementSettingWidget::on_pushButtonDefault_clicked()
{
    TextSettings a;
    mySettings = a;
    mySettings2 = a;
    loadSettings();
}

QString AnnouncementSettingWidget::getFontText(QFont font)
{
    QString f = font.toString().split(",").at(0);
    QString st(QString("%1: %2").arg(f).arg(font.pointSize()));
    if(font.bold())
        st += ", " + tr("Bold");
    if(font.italic())
        st += ", " + tr("Italic");
    if(font.strikeOut())
        st += ", " + tr("StrikeOut");
    if(font.underline())
        st += ", " + tr("Underline");

    return st;
}

void AnnouncementSettingWidget::on_pushButtonApplyToAll_clicked()
{
    emit applyBackToAll(3,mySettings.backgroundName,mySettings.backgroundPix);
}

void AnnouncementSettingWidget::setBackgroungds(QString name, QPixmap back)
{
    mySettings.backgroundName = name;
    mySettings.backgroundPix = back;
    mySettings2.backgroundName = name;
    mySettings2.backgroundPix = back;
    ui->lineEditBackground->setText(name);
    ui->lineEditBackground2->setText(name);
}
