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

void SongSettingWidget::setSettings(SongSettings &settings, SongSettings &settings2, SongSettings &settings3, SongSettings &settings4)
{
    mySettings = settings;
    mySettings2 = settings2;
    mySettings3 = settings3;
    mySettings4 = settings4;
    loadSettings();
}

void SongSettingWidget::getSettings(SongSettings &settings, SongSettings &settings2, SongSettings &settings3, SongSettings &settings4)
{
    // Effects
    mySettings.useShadow = ui->checkBoxUseShadow->isChecked();
    mySettings.useFading = ui->checkBoxUseFading->isChecked();
    mySettings.useBlurShadow = ui->checkBoxUseBlurredShadow->isChecked();

    mySettings2.useShadow = ui->checkBoxUseShadow2->isChecked();
    mySettings2.useFading = ui->checkBoxUseFading2->isChecked();
    mySettings2.useBlurShadow = ui->checkBoxUseBlurredShadow2->isChecked();

    mySettings3.useShadow = ui->checkBoxUseShadow3->isChecked();
    mySettings3.useFading = ui->checkBoxUseFading3->isChecked();
    mySettings3.useBlurShadow = ui->checkBoxUseBlurredShadow3->isChecked();

    mySettings4.useShadow = ui->checkBoxUseShadow4->isChecked();
    mySettings4.useFading = ui->checkBoxUseFading4->isChecked();
    mySettings4.useBlurShadow = ui->checkBoxUseBlurredShadow4->isChecked();

    // Save Song info
    mySettings.showStanzaTitle = ui->checkBoxStanzaTitle->isChecked();
    mySettings.showSongKey = ui->checkBoxSongKey->isChecked();
    mySettings.showSongNumber = ui->checkBoxSongNumber->isChecked();
    mySettings.infoAling = ui->comboBoxInfoAlign->currentIndex();

    mySettings2.showStanzaTitle = ui->checkBoxStanzaTitle2->isChecked();
    mySettings2.showSongKey = ui->checkBoxSongKey2->isChecked();
    mySettings2.showSongNumber = ui->checkBoxSongNumber2->isChecked();
    mySettings2.infoAling = ui->comboBoxInfoAlign2->currentIndex();

    mySettings3.showStanzaTitle = ui->checkBoxStanzaTitle3->isChecked();
    mySettings3.showSongKey = ui->checkBoxSongKey3->isChecked();
    mySettings3.showSongNumber = ui->checkBoxSongNumber3->isChecked();
    mySettings3.infoAling = ui->comboBoxInfoAlign3->currentIndex();

    mySettings4.showStanzaTitle = ui->checkBoxStanzaTitle4->isChecked();
    mySettings4.showSongKey = ui->checkBoxSongKey4->isChecked();
    mySettings4.showSongNumber = ui->checkBoxSongNumber4->isChecked();
    mySettings4.infoAling = ui->comboBoxInfoAlign4->currentIndex();

    // Save song ending
    mySettings.showSongEnding = ui->groupBoxSongEnding->isChecked();
    mySettings.endingType = ui->comboBoxEndingType->currentIndex();
    mySettings.endingPosition = ui->comboBoxEndingPosition->currentIndex();

    mySettings2.showSongEnding = ui->groupBoxSongEnding2->isChecked();
    mySettings2.endingType = ui->comboBoxEndingType2->currentIndex();
    mySettings2.endingPosition = ui->comboBoxEndingPosition2->currentIndex();

    mySettings3.showSongEnding = ui->groupBoxSongEnding3->isChecked();
    mySettings3.endingType = ui->comboBoxEndingType3->currentIndex();
    mySettings3.endingPosition = ui->comboBoxEndingPosition3->currentIndex();

    mySettings4.showSongEnding = ui->groupBoxSongEnding4->isChecked();
    mySettings4.endingType = ui->comboBoxEndingType4->currentIndex();
    mySettings4.endingPosition = ui->comboBoxEndingPosition4->currentIndex();

    // Save song background
    mySettings.useBackground = ui->groupBoxSongBackground->isChecked();
    mySettings.backgroundName = ui->lineEditSongBackground->text();

    mySettings2.useBackground = ui->groupBoxSongBackground2->isChecked();
    mySettings2.backgroundName = ui->lineEditSongBackground2->text();

    mySettings3.useBackground = ui->groupBoxSongBackground3->isChecked();
    mySettings3.backgroundName = ui->lineEditSongBackground3->text();

    mySettings4.useBackground = ui->groupBoxSongBackground4->isChecked();
    mySettings4.backgroundName = ui->lineEditSongBackground4->text();

    // Save alignment
    mySettings.textAlignmentV = ui->comboBoxVerticalAling->currentIndex();
    mySettings.textAlignmentH = ui->comboBoxHorizontalAling->currentIndex();

    mySettings2.textAlignmentV = ui->comboBoxVerticalAling2->currentIndex();
    mySettings2.textAlignmentH = ui->comboBoxHorizontalAling2->currentIndex();

    mySettings3.textAlignmentV = ui->comboBoxVerticalAling3->currentIndex();
    mySettings3.textAlignmentH = ui->comboBoxHorizontalAling3->currentIndex();

    mySettings4.textAlignmentV = ui->comboBoxVerticalAling4->currentIndex();
    mySettings4.textAlignmentH = ui->comboBoxHorizontalAling4->currentIndex();

    // Background Color 4 Text Group
    mySettings.songAddBKColorToText = ui->groupBoxSongAddBKToText->isChecked();
    mySettings2.songAddBKColorToText = ui->groupBoxSongAddBKToText2->isChecked();
    mySettings3.songAddBKColorToText = ui->groupBoxSongAddBKToText3->isChecked();
    mySettings4.songAddBKColorToText = ui->groupBoxSongAddBKToText4->isChecked();

    // Screen use
    mySettings.screenUse = ui->spinBoxScreenUse->value();
    mySettings.screenPosition = ui->comboBoxScreenUse->currentIndex();

    mySettings2.screenUse = ui->spinBoxScreenUse2->value();
    mySettings2.screenPosition = ui->comboBoxScreenUse2->currentIndex();

    mySettings3.screenUse = ui->spinBoxScreenUse3->value();
    mySettings3.screenPosition = ui->comboBoxScreenUse3->currentIndex();

    mySettings4.screenUse = ui->spinBoxScreenUse4->value();
    mySettings4.screenPosition = ui->comboBoxScreenUse4->currentIndex();

    // Use secondary display screen settings
    mySettings2.useDisp1settings = !ui->groupBoxDisplay2->isChecked();
    mySettings2.useDisp2settings = ui->groupBoxDisplay2->isChecked();

    mySettings3.useDisp1settings = !ui->groupBoxDisplay3->isChecked();
    mySettings3.useDisp3settings = ui->groupBoxDisplay3->isChecked();

    mySettings4.useDisp1settings = !ui->groupBoxDisplay4->isChecked();
    mySettings4.useDisp4settings = ui->groupBoxDisplay4->isChecked();

    settings = mySettings;
    settings2 = mySettings2;
    settings3 = mySettings3;
    settings4 = mySettings4;
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

    ui->checkBoxUseShadow3->setChecked(mySettings3.useShadow);
    ui->checkBoxUseFading3->setChecked(mySettings3.useFading);
    ui->checkBoxUseBlurredShadow3->setChecked(mySettings3.useBlurShadow);

    ui->checkBoxUseShadow4->setChecked(mySettings4.useShadow);
    ui->checkBoxUseFading4->setChecked(mySettings4.useFading);
    ui->checkBoxUseBlurredShadow4->setChecked(mySettings4.useBlurShadow);

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

    ui->checkBoxStanzaTitle3->setChecked(mySettings3.showStanzaTitle);
    ui->checkBoxSongKey3->setChecked(mySettings3.showSongKey);
    ui->checkBoxSongNumber3->setChecked(mySettings3.showSongNumber);
    p.setColor(QPalette::Base,mySettings3.infoColor);
    ui->graphicViewInfoColor3->setPalette(p);
    ui->labelInfoFont3->setText(getFontText(mySettings3.infoFont));
    ui->comboBoxInfoAlign3->setCurrentIndex(mySettings3.infoAling);

    ui->checkBoxStanzaTitle4->setChecked(mySettings4.showStanzaTitle);
    ui->checkBoxSongKey4->setChecked(mySettings4.showSongKey);
    ui->checkBoxSongNumber4->setChecked(mySettings4.showSongNumber);
    p.setColor(QPalette::Base,mySettings4.infoColor);
    ui->graphicViewInfoColor4->setPalette(p);
    ui->labelInfoFont4->setText(getFontText(mySettings4.infoFont));
    ui->comboBoxInfoAlign4->setCurrentIndex(mySettings4.infoAling);

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

    ui->groupBoxSongEnding3->setChecked(mySettings3.showSongEnding);
    p.setColor(QPalette::Base,mySettings3.endingColor);
    ui->graphicViewEndingColor3->setPalette(p);
    ui->labelEndingFont3->setText(getFontText(mySettings3.endingFont));
    ui->comboBoxEndingType3->setCurrentIndex(mySettings3.endingType);
    ui->comboBoxEndingPosition3->setCurrentIndex(mySettings3.endingPosition);

    ui->groupBoxSongEnding4->setChecked(mySettings4.showSongEnding);
    p.setColor(QPalette::Base,mySettings4.endingColor);
    ui->graphicViewEndingColor4->setPalette(p);
    ui->labelEndingFont4->setText(getFontText(mySettings4.endingFont));
    ui->comboBoxEndingType4->setCurrentIndex(mySettings4.endingType);
    ui->comboBoxEndingPosition4->setCurrentIndex(mySettings4.endingPosition);

    // Set Song Background
    ui->groupBoxSongBackground->setChecked(mySettings.useBackground);
    ui->lineEditSongBackground->setText(mySettings.backgroundName);

    ui->groupBoxSongBackground2->setChecked(mySettings2.useBackground);
    ui->lineEditSongBackground2->setText(mySettings2.backgroundName);

    ui->groupBoxSongBackground3->setChecked(mySettings3.useBackground);
    ui->lineEditSongBackground3->setText(mySettings3.backgroundName);

    ui->groupBoxSongBackground4->setChecked(mySettings4.useBackground);
    ui->lineEditSongBackground4->setText(mySettings4.backgroundName);

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

    p.setColor(QPalette::Base,mySettings3.textColor);
    ui->graphicViewTextColor3->setPalette(p);
    ui->labelFont3->setText(getFontText(mySettings3.textFont));
    ui->comboBoxVerticalAling3->setCurrentIndex(mySettings3.textAlignmentV);
    ui->comboBoxHorizontalAling3->setCurrentIndex(mySettings3.textAlignmentH);

    p.setColor(QPalette::Base,mySettings4.textColor);
    ui->graphicViewTextColor4->setPalette(p);
    ui->labelFont4->setText(getFontText(mySettings4.textFont));
    ui->comboBoxVerticalAling4->setCurrentIndex(mySettings4.textAlignmentV);
    ui->comboBoxHorizontalAling4->setCurrentIndex(mySettings4.textAlignmentH);

    // Background Color 4 Text
    // Display 1
    ui->groupBoxSongAddBKToText->setChecked(mySettings.songAddBKColorToText);
    p.setColor(QPalette::Base,mySettings.songTextRecBKColor);
    ui->graphicViewSongAddBKColor->setPalette(p);

    p.setColor(QPalette::Base,mySettings.songTextGenBKColor);
    ui->graphicViewSongChangeGenBKColor->setPalette(p);

    // Display 2
    ui->groupBoxSongAddBKToText2->setChecked(mySettings2.songAddBKColorToText);
    p.setColor(QPalette::Base,mySettings2.songTextRecBKColor);
    ui->graphicViewSongAddBKColor2->setPalette(p);

    p.setColor(QPalette::Base,mySettings2.songTextGenBKColor);
    ui->graphicViewSongChangeGenBKColor2->setPalette(p);

    // Display 3
    ui->groupBoxSongAddBKToText3->setChecked(mySettings3.songAddBKColorToText);
    p.setColor(QPalette::Base,mySettings3.songTextRecBKColor);
    ui->graphicViewSongAddBKColor3->setPalette(p);

    p.setColor(QPalette::Base,mySettings3.songTextGenBKColor);
    ui->graphicViewSongChangeGenBKColor3->setPalette(p);

    // Display 4
    ui->groupBoxSongAddBKToText4->setChecked(mySettings4.songAddBKColorToText);
    p.setColor(QPalette::Base,mySettings4.songTextRecBKColor);
    ui->graphicViewSongAddBKColor4->setPalette(p);

    p.setColor(QPalette::Base,mySettings4.songTextGenBKColor);
    ui->graphicViewSongChangeGenBKColor4->setPalette(p);


    // Set Screen Use
    ui->spinBoxScreenUse->setValue(mySettings.screenUse);
    ui->comboBoxScreenUse->setCurrentIndex(mySettings.screenPosition);

    ui->spinBoxScreenUse2->setValue(mySettings2.screenUse);
    ui->comboBoxScreenUse2->setCurrentIndex(mySettings2.screenPosition);

    ui->spinBoxScreenUse3->setValue(mySettings3.screenUse);
    ui->comboBoxScreenUse3->setCurrentIndex(mySettings3.screenPosition);

    ui->spinBoxScreenUse4->setValue(mySettings4.screenUse);
    ui->comboBoxScreenUse4->setCurrentIndex(mySettings4.screenPosition);

    // Set secondary screen
    ui->groupBoxDisplay2->setChecked(!mySettings2.useDisp1settings);
    on_groupBoxDisplay2_toggled(!mySettings2.useDisp1settings);

    ui->groupBoxDisplay3->setChecked(!mySettings3.useDisp1settings);
    on_groupBoxDisplay3_toggled(!mySettings3.useDisp1settings);

    ui->groupBoxDisplay4->setChecked(!mySettings4.useDisp1settings);
    on_groupBoxDisplay4_toggled(!mySettings4.useDisp1settings);
}

void SongSettingWidget::setDispScreen2Visible(bool visible)
{
    ui->groupBoxDisplay2->setVisible(visible);
}

void SongSettingWidget::setDispScreen3Visible(bool visible)
{
    ui->groupBoxDisplay3->setVisible(visible);
}

void SongSettingWidget::setDispScreen4Visible(bool visible)
{
    ui->groupBoxDisplay4->setVisible(visible);
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

void SongSettingWidget::on_checkBoxUseShadow3_stateChanged(int arg1)
{
    if(arg1==2)
        ui->checkBoxUseBlurredShadow3->setEnabled(true);
    else
    {
        ui->checkBoxUseBlurredShadow3->setChecked(false);
        ui->checkBoxUseBlurredShadow3->setEnabled(false);
    }
}

void SongSettingWidget::on_checkBoxUseShadow4_stateChanged(int arg1)
{
    if(arg1==2)
        ui->checkBoxUseBlurredShadow4->setEnabled(true);
    else
    {
        ui->checkBoxUseBlurredShadow4->setChecked(false);
        ui->checkBoxUseBlurredShadow4->setEnabled(false);
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

void SongSettingWidget::on_toolButtonInfoColor3_clicked()
{
    QColor c(QColorDialog::getColor(mySettings3.infoColor,this));
    if(c.isValid())
        mySettings3.infoColor = c;
    QPalette p;
    p.setColor(QPalette::Base,mySettings3.infoColor);
    ui->graphicViewInfoColor3->setPalette(p);
}

void SongSettingWidget::on_toolButtonInfoColor4_clicked()
{
    QColor c(QColorDialog::getColor(mySettings4.infoColor,this));
    if(c.isValid())
        mySettings4.infoColor = c;
    QPalette p;
    p.setColor(QPalette::Base,mySettings4.infoColor);
    ui->graphicViewInfoColor4->setPalette(p);
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

void SongSettingWidget::on_toolButtonInfoFont3_clicked()
{
    bool ok;
    QFont font = QFontDialog::getFont(&ok,mySettings3.infoFont,this);
    if(ok)
        mySettings3.infoFont = font;

    ui->labelInfoFont3->setText(getFontText(mySettings3.infoFont));
}

void SongSettingWidget::on_toolButtonInfoFont4_clicked()
{
    bool ok;
    QFont font = QFontDialog::getFont(&ok,mySettings4.infoFont,this);
    if(ok)
        mySettings4.infoFont = font;

    ui->labelInfoFont4->setText(getFontText(mySettings4.infoFont));
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

void SongSettingWidget::on_toolButtonEndingColor3_clicked()
{
    QColor c(QColorDialog::getColor(mySettings3.endingColor,this));
    if(c.isValid())
        mySettings3.endingColor = c;
    QPalette p;
    p.setColor(QPalette::Base,mySettings3.endingColor);
    ui->graphicViewEndingColor3->setPalette(p);
}

void SongSettingWidget::on_toolButtonEndingColor4_clicked()
{
    QColor c(QColorDialog::getColor(mySettings4.endingColor,this));
    if(c.isValid())
        mySettings4.endingColor = c;
    QPalette p;
    p.setColor(QPalette::Base,mySettings4.endingColor);
    ui->graphicViewEndingColor4->setPalette(p);
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

void SongSettingWidget::on_toolButtonEndingFont3_clicked()
{
    bool ok;
    QFont font = QFontDialog::getFont(&ok,mySettings3.endingFont,this);
    if(ok)
        mySettings3.endingFont = font;

    ui->labelEndingFont3->setText(getFontText(mySettings3.endingFont));
}

void SongSettingWidget::on_toolButtonEndingFont4_clicked()
{
    bool ok;
    QFont font = QFontDialog::getFont(&ok,mySettings4.endingFont,this);
    if(ok)
        mySettings4.endingFont = font;

    ui->labelEndingFont4->setText(getFontText(mySettings4.endingFont));
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

void SongSettingWidget::on_buttonSongBackground3_clicked()
{
    QString filename = QFileDialog::getOpenFileName(this, tr("Select a image for song wallpaper"),
                                                    ".", tr("Images(%1)").arg(getSupportedImageFormats()));
    if(!filename.isNull())
    {
        QPixmap p(filename);
        mySettings3.backgroundPix = p;
        QFileInfo fi(filename);
        filename = fi.fileName();
        mySettings3.backgroundName = filename;
        ui->lineEditSongBackground3->setText(filename);
    }
}

void SongSettingWidget::on_buttonSongBackground4_clicked()
{
    QString filename = QFileDialog::getOpenFileName(this, tr("Select a image for song wallpaper"),
                                                    ".", tr("Images(%1)").arg(getSupportedImageFormats()));
    if(!filename.isNull())
    {
        QPixmap p(filename);
        mySettings4.backgroundPix = p;
        QFileInfo fi(filename);
        filename = fi.fileName();
        mySettings4.backgroundName = filename;
        ui->lineEditSongBackground4->setText(filename);
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

void SongSettingWidget::on_toolButtonColor3_clicked()
{
    QColor c(QColorDialog::getColor(mySettings3.textColor,this));
    if(c.isValid())
        mySettings3.textColor = c;
    QPalette p;
    p.setColor(QPalette::Base,mySettings3.textColor);
    ui->graphicViewTextColor3->setPalette(p);
}

void SongSettingWidget::on_toolButtonColor4_clicked()
{
    QColor c(QColorDialog::getColor(mySettings4.textColor,this));
    if(c.isValid())
        mySettings4.textColor = c;
    QPalette p;
    p.setColor(QPalette::Base,mySettings4.textColor);
    ui->graphicViewTextColor4->setPalette(p);
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

void SongSettingWidget::on_toolButtonFont3_clicked()
{
    bool ok;
    QFont font = QFontDialog::getFont(&ok,mySettings3.textFont,this);
    if(ok)
        mySettings3.textFont = font;

    ui->labelFont3->setText(getFontText(mySettings3.textFont));
}

void SongSettingWidget::on_toolButtonFont4_clicked()
{
    bool ok;
    QFont font = QFontDialog::getFont(&ok,mySettings4.textFont,this);
    if(ok)
        mySettings4.textFont = font;

    ui->labelFont4->setText(getFontText(mySettings4.textFont));
}

void SongSettingWidget::on_groupBoxSongAddBKToText_toggled(bool arg1)
{
    QColor c(mySettings.songTextGenBKColor);
    if(arg1)
    {
        QPixmap px(1920, 1080);
        px.fill(c);
        mySettings.useBackground = arg1;
        mySettings.backgroundName = c.name();
        mySettings.backgroundPix = px;
        ui->lineEditSongBackground->setText(c.name());
        ui->groupBoxSongBackground->setChecked(arg1);
    }
    else
    {
        mySettings.useBackground = arg1;
        mySettings.backgroundName = "";
        mySettings.backgroundPix = QPixmap(1,1);
        ui->groupBoxSongBackground->setChecked(arg1);
        ui->lineEditSongBackground->clear();
    }
}

void SongSettingWidget::on_groupBoxSongAddBKToText2_toggled(bool arg1)
{
    QColor c(mySettings2.songTextGenBKColor);

    if(arg1)
    {
        QPixmap px(1920, 1080);
        px.fill(c);
        mySettings2.useBackground = arg1;
        mySettings2.backgroundName = c.name();
        mySettings2.backgroundPix = px;
        ui->lineEditSongBackground2->setText(c.name());
        ui->groupBoxSongBackground2->setChecked(arg1);
    }
    else
    {
        mySettings2.useBackground = arg1;
        mySettings2.backgroundName = "";
        mySettings2.backgroundPix = QPixmap(1,1);
        ui->groupBoxSongBackground2->setChecked(arg1);
        ui->lineEditSongBackground2->clear();
    }
}

void SongSettingWidget::on_groupBoxSongAddBKToText3_toggled(bool arg1)
{
    QColor c(mySettings3.songTextGenBKColor);

    if(arg1)
    {
        QPixmap px(1920, 1080);
        px.fill(c);
        mySettings3.useBackground = arg1;
        mySettings3.backgroundName = c.name();
        mySettings3.backgroundPix = px;
        ui->lineEditSongBackground3->setText(c.name());
        ui->groupBoxSongBackground3->setChecked(arg1);
    }
    else
    {
        mySettings3.useBackground = arg1;
        mySettings3.backgroundName = "";
        mySettings3.backgroundPix = QPixmap(1,1);
        ui->groupBoxSongBackground3->setChecked(arg1);
        ui->lineEditSongBackground3->clear();
    }
}

void SongSettingWidget::on_groupBoxSongAddBKToText4_toggled(bool arg1)
{
    QColor c(mySettings4.songTextGenBKColor);

    if(arg1)
    {
        QPixmap px(1920, 1080);
        px.fill(c);
        mySettings4.useBackground = arg1;
        mySettings4.backgroundName = c.name();
        mySettings4.backgroundPix = px;
        ui->lineEditSongBackground4->setText(c.name());
        ui->groupBoxSongBackground4->setChecked(arg1);
    }
    else
    {
        mySettings4.useBackground = arg1;
        mySettings4.backgroundName = "";
        mySettings4.backgroundPix = QPixmap(1,1);
        ui->groupBoxSongBackground4->setChecked(arg1);
        ui->lineEditSongBackground4->clear();
    }
}

void SongSettingWidget::on_tBSongAddBKColorText_clicked()
{
    QColor c(QColorDialog::getColor(mySettings.songTextRecBKColor,this));
    if(c.isValid())
        mySettings.songTextRecBKColor = c;
    QPalette p;
    p.setColor(QPalette::Base,mySettings.songTextRecBKColor);
    ui->graphicViewSongAddBKColor->setPalette(p);
}

void SongSettingWidget::on_tBSongAddBKColorText2_clicked()
{
    QColor c(QColorDialog::getColor(mySettings2.songTextRecBKColor,this));
    if(c.isValid())
        mySettings2.songTextRecBKColor = c;
    QPalette p;
    p.setColor(QPalette::Base,mySettings2.songTextRecBKColor);
    ui->graphicViewSongAddBKColor2->setPalette(p);
}

void SongSettingWidget::on_tBSongAddBKColorText3_clicked()
{
    QColor c(QColorDialog::getColor(mySettings3.songTextRecBKColor,this));
    if(c.isValid())
        mySettings3.songTextRecBKColor = c;
    QPalette p;
    p.setColor(QPalette::Base,mySettings3.songTextRecBKColor);
    ui->graphicViewSongAddBKColor3->setPalette(p);
}

void SongSettingWidget::on_tBSongAddBKColorText4_clicked()
{
    QColor c(QColorDialog::getColor(mySettings4.songTextRecBKColor,this));
    if(c.isValid())
        mySettings4.songTextRecBKColor = c;
    QPalette p;
    p.setColor(QPalette::Base,mySettings4.songTextRecBKColor);
    ui->graphicViewSongAddBKColor4->setPalette(p);
}

void SongSettingWidget::on_tBSongChangeGenBKColor_clicked()
{
    QColor c(QColorDialog::getColor(mySettings.songTextGenBKColor,this));
    if(c.isValid())
    {
        mySettings.songTextGenBKColor = c;
        QPixmap px(1920, 1080);
        px.fill(c);
        mySettings.backgroundName = c.name();
        mySettings.backgroundPix = px;
        ui->lineEditSongBackground->setText(c.name());
    }
    QPalette p;
    p.setColor(QPalette::Base,mySettings.songTextGenBKColor);
    ui->graphicViewSongChangeGenBKColor->setPalette(p);
}

void SongSettingWidget::on_tBSongChangeGenBKColor2_clicked()
{
    QColor c(QColorDialog::getColor(mySettings2.songTextGenBKColor,this));
    if(c.isValid())
    {
        mySettings2.songTextGenBKColor = c;
        QPixmap px(1920, 1080);
        px.fill(c);
        mySettings2.backgroundName = c.name();
        mySettings2.backgroundPix = px;
        ui->lineEditSongBackground2->setText(c.name());
    }
    QPalette p;
    p.setColor(QPalette::Base,mySettings2.songTextGenBKColor);
    ui->graphicViewSongChangeGenBKColor2->setPalette(p);
}

void SongSettingWidget::on_tBSongChangeGenBKColor3_clicked()
{
    QColor c(QColorDialog::getColor(mySettings3.songTextGenBKColor,this));
    if(c.isValid())
    {
        mySettings3.songTextGenBKColor = c;
        QPixmap px(1920, 1080);
        px.fill(c);
        mySettings3.backgroundName = c.name();
        mySettings3.backgroundPix = px;
        ui->lineEditSongBackground3->setText(c.name());
    }
    QPalette p;
    p.setColor(QPalette::Base,mySettings3.songTextGenBKColor);
    ui->graphicViewSongChangeGenBKColor3->setPalette(p);
}

void SongSettingWidget::on_tBSongChangeGenBKColor4_clicked()
{
    QColor c(QColorDialog::getColor(mySettings4.songTextGenBKColor,this));
    if(c.isValid())
    {
        mySettings4.songTextGenBKColor = c;
        QPixmap px(1920, 1080);
        px.fill(c);
        mySettings4.backgroundName = c.name();
        mySettings4.backgroundPix = px;
        ui->lineEditSongBackground4->setText(c.name());
    }
    QPalette p;
    p.setColor(QPalette::Base,mySettings4.songTextGenBKColor);
    ui->graphicViewSongChangeGenBKColor4->setPalette(p);
}

void SongSettingWidget::on_groupBoxDisplay2_toggled(bool arg1)
{
    ui->groupBoxEffects2->setVisible(arg1);
    ui->groupBoxSongBackground2->setVisible(arg1);
    ui->groupBoxSongEnding2->setVisible(arg1);
    ui->groupBoxSongInfo2->setVisible(arg1);
    ui->groupBoxTextProperties2->setVisible(arg1);
    ui->groupBoxSongAddBKToText2->setVisible(arg1);
    ui->groupBoxScreenUse2->setVisible(arg1);
}

void SongSettingWidget::on_groupBoxDisplay3_toggled(bool arg1)
{
    ui->groupBoxEffects3->setVisible(arg1);
    ui->groupBoxSongBackground3->setVisible(arg1);
    ui->groupBoxSongEnding3->setVisible(arg1);
    ui->groupBoxSongInfo3->setVisible(arg1);
    ui->groupBoxTextProperties3->setVisible(arg1);
    ui->groupBoxSongAddBKToText3->setVisible(arg1);
    ui->groupBoxScreenUse3->setVisible(arg1);
}

void SongSettingWidget::on_groupBoxDisplay4_toggled(bool arg1)
{
    ui->groupBoxEffects4->setVisible(arg1);
    ui->groupBoxSongBackground4->setVisible(arg1);
    ui->groupBoxSongEnding4->setVisible(arg1);
    ui->groupBoxSongInfo4->setVisible(arg1);
    ui->groupBoxTextProperties4->setVisible(arg1);
    ui->groupBoxSongAddBKToText4->setVisible(arg1);
    ui->groupBoxScreenUse4->setVisible(arg1);
}

void SongSettingWidget::on_pushButtonDefault_clicked()
{
    SongSettings s;
    mySettings = s;
    mySettings2 = s;
    mySettings3 = s;
    mySettings4 = s;
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
    mySettings3.backgroundName = name;
    mySettings3.backgroundPix = back;
    mySettings4.backgroundName = name;
    mySettings4.backgroundPix = back;
    ui->lineEditSongBackground->setText(name);
    ui->lineEditSongBackground2->setText(name);
    ui->lineEditSongBackground3->setText(name);
    ui->lineEditSongBackground4->setText(name);
}
