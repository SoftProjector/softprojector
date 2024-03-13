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

#include "slideshoweditor.hpp"
#include "ui_slideshoweditor.h"

SlideShowEditor::SlideShowEditor(QWidget *parent) :
    //    QWidget(parent),
    QDialog(parent),
    ui(new Ui::SlideShowEditor)
{
    ui->setupUi(this);
    updateButtonState();
}

SlideShowEditor::~SlideShowEditor()
{
    delete ui;
}

void SlideShowEditor::setSlideShow(SlideShow ss)
{
    editSS = ss;
    deleteList.clear();
    loadSlideShow();
}

void SlideShowEditor::loadSlideShow()
{
    ui->lineEditTitle->setText(editSS.name);
    ui->plainTextEditInfo->setPlainText(editSS.info);

    reloadSlides();
    ui->listWidgetSlides->setCurrentRow(0);
}

void SlideShowEditor::reloadSlides()
{
    ui->listWidgetSlides->clear();
    foreach(const SlideShowItem &ssi, editSS.slides)
    {
        QListWidgetItem *itm = new QListWidgetItem;
        QIcon ico(ssi.imageSmall);
        itm->setIcon(ico);
        ui->listWidgetSlides->addItem(itm);
    }
}

void SlideShowEditor::updateButtonState()
{
    int c = ui->listWidgetSlides->currentRow();
    int ct = editSS.slides.count();

    ui->pushButtonRemoveImage->setEnabled(c>=0 && c<ct);
    ui->pushButtonMoveUp->setEnabled(c>0 && c<ct);
    ui->pushButtonMoveDown->setEnabled(c>=0 && (c<ct-1));
}

void SlideShowEditor::on_pushButtonAddImages_clicked()
{
    QStringList imageFilePaths = QFileDialog::getOpenFileNames(this,tr("Select Images to Open"),".",
                                                               tr("Images(%1)").arg(getSupportedImageFormats()));
    if(imageFilePaths.count()>0)
    {
        this->setCursor(Qt::WaitCursor);
        int i(0);
        QProgressDialog progress(tr("Adding files..."), tr("Cancel"), 0, imageFilePaths.count(), this);
        ui->listWidgetSlides->setIconSize(QSize(100,100));
        foreach(const QString &file, imageFilePaths)
        {
            ++i;
            progress.setValue(i);

            QPixmap img;
            SlideShowItem sd;
            img.load(file);
            // set display image. If to resize, resize them
            if(mySettings.resize)
            {
                if(img.width()>mySettings.boundWidth || img.height()>mySettings.boundWidth )
                    sd.image = img.scaled(mySettings.boundWidth ,mySettings.boundWidth , Qt::KeepAspectRatio);
                else
                    sd.image = img;
            }
            else
                sd.image = img;

            // set preview image
            if(img.width()>400 || img.height()>400)
                sd.imagePreview = img.scaled(400,400, Qt::KeepAspectRatio);
            else
                sd.imagePreview = img;

            // set list image
            if(img.width()>100 || img.height()>100)
                sd.imageSmall = img.scaled(100,100, Qt::KeepAspectRatio);
            else
                sd.imageSmall = img;

            // set file name
            QFileInfo f(file);
            sd.name = f.fileName();
            sd.path = f.filePath();

            // add to slideshow
            editSS.slides.append(sd);

            // add to slide show list
            QListWidgetItem *itm = new QListWidgetItem;
            QIcon ico(sd.imageSmall);

            itm->setIcon(ico);
            ui->listWidgetSlides->addItem(itm);
        }
        this->setCursor(Qt::ArrowCursor);
        updateButtonState();
    }

}

void SlideShowEditor::on_pushButtonRemoveImage_clicked()
{
    int c = ui->listWidgetSlides->currentRow();
    if(c>=0 && c<editSS.slides.count())
    {
        deleteList.append(editSS.slides.at(c).slideId);
        editSS.slides.removeAt(c);
        reloadSlides();
        updateButtonState();
    }
}

void SlideShowEditor::on_pushButtonMoveUp_clicked()
{
    int c = ui->listWidgetSlides->currentRow();
    int u = c-1;
    if(u>=0)
    {
        editSS.slides.move(c,u);
        reloadSlides();
        ui->listWidgetSlides->setCurrentRow(u);
        updateButtonState();
    }
}

void SlideShowEditor::on_pushButtonMoveDown_clicked()
{
    int c = ui->listWidgetSlides->currentRow();
    int d = c+1;
    if(d<editSS.slides.count())
    {
        editSS.slides.move(c,d);
        reloadSlides();
        ui->listWidgetSlides->setCurrentRow(d);
        updateButtonState();
    }
}

void SlideShowEditor::on_pushButtonSave_clicked()
{
    // Make sure that slide show is not empty before saving.
    if(editSS.slides.count()<=0)
    {
        //todo: message box saying that title cannot be empty
        close();
        return;
    }

    editSS.name = ui->lineEditTitle->text().trimmed();

    // Make sure that slide show Title is not empty before saving.
    if(editSS.name.isEmpty())
    {
        QMessageBox mb(this);
        mb.setText(tr("Slide show title cannot be left empty.\nPlease enter a title."));
        mb.setWindowTitle(tr("Slide show title is missing"));
        mb.setIcon(QMessageBox::Warning);
        mb.exec();
        ui->lineEditTitle->setFocus();
        return;
    }

    editSS.info = ui->plainTextEditInfo->toPlainText();
    editSS.saveSideShow(tr("Saving Slide Show"),this,deleteList);
    close();
}

void SlideShowEditor::on_pushButtonCancel_clicked()
{
    close();
}

void SlideShowEditor::on_listWidgetSlides_currentRowChanged(int currentRow)
{
    if(currentRow>=0)
    {
        ui->labelPreview->setPixmap(editSS.slides.at(currentRow).imagePreview);
        ui->labelPixInfo->setText(tr("Preview slide: %1").arg(editSS.slides.at(currentRow).name));
        updateButtonState();
    }
}
