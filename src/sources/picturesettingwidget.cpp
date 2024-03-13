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

#include "picturesettingwidget.hpp"
#include "ui_picturesettingwidget.h"

PictureSettingWidget::PictureSettingWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::PictureSettingWidget)
{
    ui->setupUi(this);
}

PictureSettingWidget::~PictureSettingWidget()
{
    delete ui;
}

void PictureSettingWidget::changeEvent(QEvent *e)
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

void PictureSettingWidget::setSettings(SlideShowSettings &settings)
{
    mySettings = settings;
    ui->checkBoxExpand->setChecked(mySettings.expandSmall);

    if(mySettings.fitType == 0)
        ui->radioButtonFit->setChecked(true);
    else if(mySettings.fitType == 1)
        ui->radioButtonFitExpand->setChecked(true);

    ui->groupBoxResize->setChecked(mySettings.resize);
    ui->comboBoxBoundAmount->setCurrentIndex(mySettings.boundType);
    if(mySettings.boundType == 7)
        ui->lineEditBound->setText(QString::number(mySettings.boundWidth));
    else
        ui->lineEditBound->clear();
}

void PictureSettingWidget::getSettings(SlideShowSettings &settings)
{
    mySettings.expandSmall = ui->checkBoxExpand->isChecked();

    if(ui->radioButtonFit->isChecked())
        mySettings.fitType = 0;
    else if(ui->radioButtonFitExpand->isChecked())
        mySettings.fitType = 1;

    mySettings.resize = ui->groupBoxResize->isChecked();
    mySettings.boundType = ui->comboBoxBoundAmount->currentIndex();
    if(mySettings.boundType == 0)
        mySettings.boundWidth = 800;
    else if(mySettings.boundType == 1)
        mySettings.boundWidth = 1024;
    else if(mySettings.boundType == 2)
        mySettings.boundWidth = 1280;
    else if(mySettings.boundType == 3)
        mySettings.boundWidth = 1366;
    else if(mySettings.boundType == 4)
        mySettings.boundWidth = 1440;
    else if(mySettings.boundType == 5)
        mySettings.boundWidth = 1600;
    else if(mySettings.boundType == 6)
        mySettings.boundWidth = 1920;
    else if(mySettings.boundType == 7)
    {
        bool ok;
        mySettings.boundWidth = ui->lineEditBound->text().toInt(&ok);
        if(!ok)
        {
            mySettings.boundType = 1;
            mySettings.boundWidth = 1024;
        }
    }

    settings = mySettings;
}

void PictureSettingWidget::on_comboBoxBoundAmount_currentIndexChanged(int index)
{
    if(index == 7)
    {
        ui->lineEditBound->setEnabled(true);
        ui->lineEditBound->setFocus();
    }
    else
        ui->lineEditBound->setEnabled(false);
}

void PictureSettingWidget::on_lineEditBound_textChanged(const QString &arg1)
{
    ui->labelBoundBy->setText("x " + arg1);
}

void PictureSettingWidget::on_lineEditBound_editingFinished()
{
    bool ok;
    QString t = ui->lineEditBound->text();
    if(!t.isEmpty())
    {
        t.toInt(&ok);
        if(!ok)
        {
            QMessageBox mb(this);
            mb.setWindowTitle(tr("Invalid Numeric Value"));
            mb.setText(tr("Entered '%1' custom width is not numeric. ").arg(t));
            mb.exec();
            ui->lineEditBound->setFocus();
            ui->lineEditBound->selectAll();
        }
    }
}
