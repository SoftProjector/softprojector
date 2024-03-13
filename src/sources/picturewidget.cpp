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

#include "picturewidget.hpp"
#include "ui_picturewidget.h"

PictureWidget::PictureWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::PictureWidget)
{
    ui->setupUi(this);
    loadSlideShows();
    ui->pushButtonGoLive->setEnabled(false);
}

PictureWidget::~PictureWidget()
{
    delete ui;
}

void PictureWidget::changeEvent(QEvent *e)
{
    QWidget::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        ui->retranslateUi(this);
        break;
    default:
        break;
    }
}

void PictureWidget::loadSlideShows()
{
    QSqlQuery sq;
    sq.exec("SELECT * FROM SlideShows");
    QStringList sl;

    slideShows.clear();

    while(sq.next())
    {
        SlideShowInfo ssi;
        ssi.slideSwId = sq.value(0).toInt();
        ssi.name = sq.value(1).toString();
        ssi.info = sq.value(2).toString();
        slideShows.append(ssi);
        sl.append( sq.value(1).toString());
    }
    ui->listWidgetSlideShow->clear();
    ui->listWidgetSlideShow->addItems(sl);
}

void PictureWidget::on_listWidgetSlides_currentRowChanged(int currentRow)
{
    if(currentRow>=0)
    {
        int pw,ph;
        pw = slides.at(currentRow).imagePreview.width();
        ph = slides.at(currentRow).imagePreview.height();
        if(pw>300 || ph>200)
            ui->labelPreview->setPixmap(slides.at(currentRow).imagePreview.scaled(300,200,Qt::KeepAspectRatio));
        else
            ui->labelPreview->setPixmap(slides.at(currentRow).imagePreview);
        ui->labelPixInfo->setText(tr("Preview slide: ")+slides.at(currentRow).name);
    }
}

void PictureWidget::on_listWidgetSlides_doubleClicked(const QModelIndex &index)
{
    sendToProjector();
}

void PictureWidget::on_pushButtonAddImages_clicked()
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
            slides.append(sd);

            // add to slide show list
            QListWidgetItem *itm = new QListWidgetItem;
            QIcon ico(sd.imageSmall);

            itm->setIcon(ico);
            ui->listWidgetSlides->addItem(itm);
        }
        this->setCursor(Qt::ArrowCursor);
        ui->pushButtonGoLive->setEnabled(true);
    }
}

void PictureWidget::on_pushButtonRemoveImage_clicked()
{
    int c = ui->listWidgetSlides->currentRow();
    if(c>=0)
    {
        slides.removeAt(c);
        ui->listWidgetSlides->clear();
        foreach(const SlideShowItem &sst, slides)
        {
            QListWidgetItem *itm = new QListWidgetItem;
            QIcon ico(sst.imageSmall);
            itm->setIcon(ico);
            ui->listWidgetSlides->addItem(itm);
        }
        ui->listWidgetSlides->setCurrentRow(c);
    }
}

void PictureWidget::on_pushButtonClearImages_clicked()
{
    ui->listWidgetSlideShow->setCurrentRow(-1);
    ui->listWidgetSlides->clear();
    slides.clear();
    ui->labelPreviewSlideShow->clear();
    ui->labelPreview->clear();
    ui->labelPreview->setText(tr("Picture Preview"));
    ui->labelPixInfo->setText(tr("Preview slide: "));
    ui->pushButtonGoLive->setEnabled(false);
    currentSlideShow = SlideShow();
}

void PictureWidget::on_pushButtonMoveUp_clicked()
{
    int c = ui->listWidgetSlides->currentRow();
    int u = c-1;
    if(u>=0)
    {
        slides.move(c,u);
        ui->listWidgetSlides->clear();
        foreach(const SlideShowItem &sst, slides)
        {
            QListWidgetItem *itm = new QListWidgetItem;
            QIcon ico(sst.imageSmall);
            itm->setIcon(ico);
            ui->listWidgetSlides->addItem(itm);
        }
        ui->listWidgetSlides->setCurrentRow(u);
    }
}

void PictureWidget::on_pushButtonMoveDown_clicked()
{
    int c = ui->listWidgetSlides->currentRow();
    int d = c+1;
    if(d<slides.count())
    {
        slides.move(c,d);
        ui->listWidgetSlides->clear();
        foreach(const SlideShowItem &sst, slides)
        {
            QListWidgetItem *itm = new QListWidgetItem;
            QIcon ico(sst.imageSmall);
            itm->setIcon(ico);
            ui->listWidgetSlides->addItem(itm);
        }
        ui->listWidgetSlides->setCurrentRow(d);
    }
}

void PictureWidget::on_pushButtonGoLive_clicked()
{
    sendToProjector();
}

void PictureWidget::sendToProjector()
{
    emit sendSlideShow(slides, ui->listWidgetSlides->currentRow(),currentSlideShow.name);
}

void PictureWidget::loadSlideShow(int ss_id)
{
    SlideShow ss;
    ss.loadSlideShow(ss_id);
    sendToPreview(ss);
    ui->pushButtonGoLive->setEnabled(true);
}

void PictureWidget::sendToPreview(SlideShow &sshow)
{
    currentSlideShow = sshow;
    slides.clear();
    ui->listWidgetSlides->clear();
    ui->labelPreviewSlideShow->setText(tr("Slide Show: %1").arg(currentSlideShow.name));
    foreach(const SlideShowItem &sst, currentSlideShow.slides)
    {
        slides.append(sst);
        QListWidgetItem *itm = new QListWidgetItem;
        QIcon ico(sst.imageSmall);
        itm->setIcon(ico);
        ui->listWidgetSlides->addItem(itm);
    }
    ui->listWidgetSlides->setCurrentRow(0);
}

void PictureWidget::sendToPreviewFromSchedule(SlideShow &sshow)
{
    ui->listWidgetSlideShow->clearSelection();
    sendToPreview(sshow);
}

SlideShow PictureWidget::getCurrentSlideshow()
{
    currentSlideShow.slides = slides;
    return currentSlideShow;
}

bool PictureWidget::isSlideShowSelected()
{
    int css = ui->listWidgetSlideShow->selectionModel()->selection().count();
    if(css>0)
        return true;
    else
        return false;
}

void PictureWidget::deleteSlideShow()
{
    QSqlQuery sq;
    int ssId = slideShows.at(ui->listWidgetSlideShow->currentRow()).slideSwId;
    sq.exec(QString("DELETE FROM SlideShows WHERE id = %1").arg(ssId));
    sq.exec(QString("DELETE FROM Slides WHERE ss_id = %1").arg(ssId));
    loadSlideShows();
    on_pushButtonClearImages_clicked();
}

void PictureWidget::on_listWidgetSlideShow_itemSelectionChanged()
{
    int cRow = ui->listWidgetSlideShow->currentRow();
    if(cRow>=0 && ui->listWidgetSlideShow->selectedItems().count()>0)
        loadSlideShow(slideShows.at(cRow).slideSwId);
}

void PictureWidget::on_listWidgetSlideShow_doubleClicked(const QModelIndex &index)
{
    currentSlideShow.slides = slides;
    emit sendToSchedule(currentSlideShow);
}
