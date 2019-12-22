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

#include "songsettingwidget.hpp"
#include "ui_songsettingwidget.h"

SongSettingWidget::SongSettingWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SongSettingWidget)
{
    ui->setupUi(this);
}

void SongSettingWidget::changeEvent(QEvent *e)
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

SongSettingWidget::~SongSettingWidget()
{
    delete ui;
}

void SongSettingWidget::setSettings(SongSettings &settings, SongSettings &settings2)
{
    mySettings = settings;
    mySettings2 = settings2;
    loadSettings();
}

void SongSettingWidget::getSettings(SongSettings &settings, SongSettings &settings2)
{
    // Effects
    mySettings.useShadow = ui->checkBoxUseShadow->isChecked();
    mySettings.useFading = ui->checkBoxUseFading->isChecked();
    mySettings.useBlurShadow = ui->checkBoxUseBlurredShadow->isChecked();

    mySettings2.useShadow = ui->checkBoxUseShadow2->isChecked();
    mySettings2.useFading = ui->checkBoxUseFading2->isChecked();
    mySettings2.useBlurShadow = ui->checkBoxUseBlurredShadow2->isChecked();

    // Save Song info
    mySettings.showStanzaTitle = ui->checkBoxStanzaTitle->isChecked();
    mySettings.showSongKey = ui->checkBoxSongKey->isChecked();
    mySettings.showSongNumber = ui->checkBoxSongNumber->isChecked();
    mySettings.infoAling = ui->comboBoxInfoAlign->currentIndex();

    mySettings2.showStanzaTitle = ui->checkBoxStanzaTitle2->isChecked();
    mySettings2.showSongKey = ui->checkBoxSongKey2->isChecked();
    mySettings2.showSongNumber = ui->checkBoxSongNumber2->isChecked();
    mySettings2.infoAling = ui->comboBoxInfoAlign2->currentIndex();

    // Save song ending
    mySettings.showSongEnding = ui->groupBoxSongEnding->isChecked();
    mySettings.endingType = ui->comboBoxEndingType->currentIndex();
    mySettings.endingPosition = ui->comboBoxEndingPosition->currentIndex();

    mySettings2.showSongEnding = ui->groupBoxSongEnding2->isChecked();
    mySettings2.endingType = ui->comboBoxEndingType2->currentIndex();
    mySettings2.endingPosition = ui->comboBoxEndingPosition2->currentIndex();

    // Save song background
    mySettings.useBackground = ui->groupBoxSongBackground->isChecked();
    mySettings.backgroundName = ui->lineEditSongBackground->text();

    mySettings2.useBackground = ui->groupBoxSongBackground2->isChecked();
    mySettings2.backgroundName = ui->lineEditSongBackground2->text();

    // Save alignment
    mySettings.textAlignmentV = ui->comboBoxVerticalAling->currentIndex();
    mySettings.textAlignmentH = ui->comboBoxHorizontalAling->currentIndex();

    mySettings2.textAlignmentV = ui->comboBoxVerticalAling2->currentIndex();
    mySettings2.textAlignmentH = ui->comboBoxHorizontalAling2->currentIndex();

    // Screen use
    mySettings.screenUse = ui->spinBoxScreenUse->value();
    mySettings.screenPosition = ui->comboBoxScreenUse->currentIndex();

    mySettings2.screenUse = ui->spinBoxScreenUse2->value();
    mySettings2.screenPosition = ui->comboBoxScreenUse2->currentIndex();

    // Use secondary display screen settings
    mySettings2.useDisp2settings = ui->groupBoxDisplay2->isChecked();

    settings = mySettings;
    settings2 = mySettings2;
}

void SongSettingWidget::loadSettings()
{
    QPalette p;
    // Set Effects
    ui->checkBoxUseShadow->setChecked(mySettings.useShadow);
    ui->checkBoxUseFading->setChecked(mySettings.useFading);
    ui->checkBoxUseBlurredShadow->setChecked(mySettings.useBlurShadow);

    ui->checkBoxUseShadow2->setChecked(mySettings2.useShadow);
    ui->checkBoxUseFading2->setChecked(mySettings2.useFading);
    ui->checkBoxUseBlurredShadow2->setChecked(mySettings2.useBlurShadow);

    // Set Song Information
    ui->checkBoxStanzaTitle->setChecked(mySettings.showStanzaTitle);
    ui->checkBoxSongKey->setChecked(mySettings.showSongKey);
    ui->checkBoxSongNumber->setChecked(mySettings.showSongNumber);
    p.setColor(QPalette::Base,mySettings.infoColor);
    ui->graphicViewInfoColor->setPalette(p);
    ui->labelInfoFont->setText(getFontText(mySettings.infoFont));
    ui->comboBoxInfoAlign->setCurrentIndex(mySettings.infoAling);

    ui->checkBoxStanzaTitle2->setChecked(mySettings2.showStanzaTitle);
    ui->checkBoxSongKey2->setChecked(mySettings2.showSongKey);
    ui->checkBoxSongNumber2->setChecked(mySettings2.showSongNumber);
    p.setColor(QPalette::Base,mySettings2.infoColor);
    ui->graphicViewInfoColor2->setPalette(p);
    ui->labelInfoFont2->setText(getFontText(mySettings2.infoFont));
    ui->comboBoxInfoAlign2->setCurrentIndex(mySettings2.infoAling);

    // Set Song Ending
    ui->groupBoxSongEnding->setChecked(mySettings.showSongEnding);
    p.setColor(QPalette::Base,mySettings.endingColor);
    ui->graphicViewEndingColor->setPalette(p);
    ui->labelEndingFont->setText(getFontText(mySettings.endingFont));
    ui->comboBoxEndingType->setCurrentIndex(mySettings.endingType);
    ui->comboBoxEndingPosition->setCurrentIndex(mySettings.endingPosition);

    ui->groupBoxSongEnding2->setChecked(mySettings2.showSongEnding);
    p.setColor(QPalette::Base,mySettings2.endingColor);
    ui->graphicViewEndingColor2->setPalette(p);
    ui->labelEndingFont2->setText(getFontText(mySettings2.endingFont));
    ui->comboBoxEndingType2->setCurrentIndex(mySettings2.endingType);
    ui->comboBoxEndingPosition2->setCurrentIndex(mySettings2.endingPosition);

    // Set Song Background
    ui->groupBoxSongBackground->setChecked(mySettings.useBackground);
    ui->lineEditSongBackground->setText(mySettings.backgroundName);

    ui->groupBoxSongBackground2->setChecked(mySettings2.useBackground);
    ui->lineEditSongBackground2->setText(mySettings2.backgroundName);

    // Set Text Properties
    p.setColor(QPalette::Base,mySettings.textColor);
    ui->graphicViewTextColor->setPalette(p);
    ui->labelFont->setText(getFontText(mySettings.textFont));
    ui->comboBoxVerticalAling->setCurrentIndex(mySettings.textAlignmentV);
    ui->comboBoxHorizontalAling->setCurrentIndex(mySettings.textAlignmentH);

    p.setColor(QPalette::Base,mySettings2.textColor);
    ui->graphicViewTextColor2->setPalette(p);
    ui->labelFont2->setText(getFontText(mySettings2.textFont));
    ui->comboBoxVerticalAling2->setCurrentIndex(mySettings2.textAlignmentV);
    ui->comboBoxHorizontalAling2->setCurrentIndex(mySettings2.textAlignmentH);

    // Set Screen Use
    ui->spinBoxScreenUse->setValue(mySettings.screenUse);
    ui->comboBoxScreenUse->setCurrentIndex(mySettings.screenPosition);

    ui->spinBoxScreenUse2->setValue(mySettings2.screenUse);
    ui->comboBoxScreenUse2->setCurrentIndex(mySettings2.screenPosition);

    // Set secondary screen
    ui->groupBoxDisplay2->setChecked(mySettings2.useDisp2settings);
    on_groupBoxDisplay2_toggled(mySettings2.useDisp2settings);
}

void SongSettingWidget::setDispScreen2Visible(bool visible)
{
    ui->groupBoxDisplay2->setVisible(visible);
}

void SongSettingWidget::on_checkBoxUseShadow_stateChanged(int arg1)
{
    if(arg1==2)
        ui->checkBoxUseBlurredShadow->setEnabled(true);
    else
    {
        ui->checkBoxUseBlurredShadow->setChecked(false);
        ui->checkBoxUseBlurredShadow->setEnabled(false);
    }
}

void SongSettingWidget::on_checkBoxUseShadow2_stateChanged(int arg1)
{
    if(arg1==2)
        ui->checkBoxUseBlurredShadow2->setEnabled(true);
    else
    {
        ui->checkBoxUseBlurredShadow2->setChecked(false);
        ui->checkBoxUseBlurredShadow2->setEnabled(false);
    }
}
void SongSettingWidget::on_toolButtonInfoColor_clicked()
{
    QColor c(QColorDialog::getColor(mySettings.infoColor,this));
    if(c.isValid())
        mySettings.infoColor = c;
    QPalette p;
    p.setColor(QPalette::Base,mySettings.infoColor);
    ui->graphicViewInfoColor->setPalette(p);
}

void SongSettingWidget::on_toolButtonInfoColor2_clicked()
{
    QColor c(QColorDialog::getColor(mySettings2.infoColor,this));
    if(c.isValid())
        mySettings2.infoColor = c;
    QPalette p;
    p.setColor(QPalette::Base,mySettings2.infoColor);
    ui->graphicViewInfoColor2->setPalette(p);
}

void SongSettingWidget::on_toolButtonInfoFont_clicked()
{
    bool ok;
    QFont font = QFontDialog::getFont(&ok,mySettings.infoFont,this);
    if(ok)
        mySettings.infoFont = font;

    ui->labelInfoFont->setText(getFontText(mySettings.infoFont));
}

void SongSettingWidget::on_toolButtonInfoFont2_clicked()
{
    bool ok;
    QFont font = QFontDialog::getFont(&ok,mySettings2.infoFont,this);
    if(ok)
        mySettings2.infoFont = font;

    ui->labelInfoFont2->setText(getFontText(mySettings2.infoFont));
}

void SongSettingWidget::on_toolButtonEndingColor_clicked()
{
    QColor c(QColorDialog::getColor(mySettings.endingColor,this));
    if(c.isValid())
        mySettings.endingColor = c;
    QPalette p;
    p.setColor(QPalette::Base,mySettings.endingColor);
    ui->graphicViewEndingColor->setPalette(p);
}

void SongSettingWidget::on_toolButtonEndingColor2_clicked()
{
    QColor c(QColorDialog::getColor(mySettings2.endingColor,this));
    if(c.isValid())
        mySettings2.endingColor = c;
    QPalette p;
    p.setColor(QPalette::Base,mySettings2.endingColor);
    ui->graphicViewEndingColor2->setPalette(p);
}

void SongSettingWidget::on_toolButtonEndingFont_clicked()
{
    bool ok;
    QFont font = QFontDialog::getFont(&ok,mySettings.endingFont,this);
    if(ok)
        mySettings.endingFont = font;

    ui->labelEndingFont->setText(getFontText(mySettings.endingFont));
}

void SongSettingWidget::on_toolButtonEndingFont2_clicked()
{
    bool ok;
    QFont font = QFontDialog::getFont(&ok,mySettings2.endingFont,this);
    if(ok)
        mySettings2.endingFont = font;

    ui->labelEndingFont2->setText(getFontText(mySettings2.endingFont));
}

void SongSettingWidget::on_buttonSongBackground_clicked()
{
    QString filename = QFileDialog::getOpenFileName(this, tr("Select a image for song wallpaper"),
                                                    ".", tr("Images(%1)").arg(getSupportedImageFormats()));
    if(!filename.isNull())
    {
        QPixmap p(filename);
        mySettings.backgroundPix = p;
        QFileInfo fi(filename);
        filename = fi.fileName();
        mySettings.backgroundName = filename;
        ui->lineEditSongBackground->setText(filename);
    }
}

void SongSettingWidget::on_buttonSongBackground2_clicked()
{
    QString filename = QFileDialog::getOpenFileName(this, tr("Select a image for song wallpaper"),
                                                    ".", tr("Images(%1)").arg(getSupportedImageFormats()));
    if(!filename.isNull())
    {
        QPixmap p(filename);
        mySettings2.backgroundPix = p;
        QFileInfo fi(filename);
        filename = fi.fileName();
        mySettings2.backgroundName = filename;
        ui->lineEditSongBackground2->setText(filename);
    }
}

void SongSettingWidget::on_toolButtonColor_clicked()
{
    QColor c(QColorDialog::getColor(mySettings.textColor,this));
    if(c.isValid())
        mySettings.textColor = c;
    QPalette p;
    p.setColor(QPalette::Base,mySettings.textColor);
    ui->graphicViewTextColor->setPalette(p);
}

void SongSettingWidget::on_toolButtonColor2_clicked()
{
    QColor c(QColorDialog::getColor(mySettings2.textColor,this));
    if(c.isValid())
        mySettings2.textColor = c;
    QPalette p;
    p.setColor(QPalette::Base,mySettings2.textColor);
    ui->graphicViewTextColor2->setPalette(p);
}

void SongSettingWidget::on_toolButtonFont_clicked()
{
    bool ok;
    QFont font = QFontDialog::getFont(&ok,mySettings.textFont,this);
    if(ok)
        mySettings.textFont = font;

    ui->labelFont->setText(getFontText(mySettings.textFont));
}

void SongSettingWidget::on_toolButtonFont2_clicked()
{
    bool ok;
    QFont font = QFontDialog::getFont(&ok,mySettings2.textFont,this);
    if(ok)
        mySettings2.textFont = font;

    ui->labelFont2->setText(getFontText(mySettings2.textFont));
}

void SongSettingWidget::on_groupBoxDisplay2_toggled(bool arg1)
{
    ui->groupBoxEffects2->setVisible(arg1);
    ui->groupBoxSongBackground2->setVisible(arg1);
    ui->groupBoxSongEnding2->setVisible(arg1);
    ui->groupBoxSongInfo2->setVisible(arg1);
    ui->groupBoxTextProperties2->setVisible(arg1);
    ui->groupBoxScreenUse2->setVisible(arg1);
}

void SongSettingWidget::on_pushButtonDefault_clicked()
{
    SongSettings s;
    mySettings = s;
    mySettings2 = s;
    loadSettings();
}

QString SongSettingWidget::getFontText(QFont font)
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

void SongSettingWidget::on_pushButtonApplyToAll_clicked()
{
    emit applyBackToAll(2,mySettings.backgroundName,mySettings.backgroundPix);
}

void SongSettingWidget::setBackgroungds(QString name, QPixmap back)
{
    mySettings.backgroundName = name;
    mySettings.backgroundPix = back;
    mySettings2.backgroundName = name;
    mySettings2.backgroundPix = back;
    ui->lineEditSongBackground->setText(name);
    ui->lineEditSongBackground2->setText(name);
}
