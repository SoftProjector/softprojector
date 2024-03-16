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

#ifndef PICTUREWIDGET_HPP
#define PICTUREWIDGET_HPP

#include <QtGui>
#include <QtSql>
#include "slideshow.hpp"
#include "slideshoweditor.hpp"
#include "settings.hpp"
#include "spfunctions.hpp"

namespace Ui {
class PictureWidget;
}

class PictureWidget : public QWidget
{
    Q_OBJECT
    
public:
    explicit PictureWidget(QWidget *parent = 0);
    ~PictureWidget();
    
signals:
    void sendSlideShow(QList<SlideShowItem> &slideShow, int row,QString name);
    void sendToSchedule(SlideShow &sshow);

protected:
    virtual void changeEvent(QEvent *e);

public slots:
    void loadSlideShows();
    SlideShow getCurrentSlideshow();
    bool isSlideShowSelected();
    void deleteSlideShow();
    void sendToPreviewFromSchedule(SlideShow &sshow);
    void setSettings(SlideShowSettings &settings){mySettings = settings;}

private slots:
    void on_listWidgetSlides_doubleClicked(const QModelIndex &index);
    void on_pushButtonAddImages_clicked();
    void on_pushButtonRemoveImage_clicked();
    void on_pushButtonMoveUp_clicked();
    void on_pushButtonMoveDown_clicked();
    void on_pushButtonGoLive_clicked();
    void sendToProjector();
    void on_pushButtonClearImages_clicked();
    void on_listWidgetSlideShow_itemSelectionChanged();
    void on_listWidgetSlides_currentRowChanged(int currentRow);
    void loadSlideShow(int ss_id);
    void sendToPreview(SlideShow &sshow);
    void on_listWidgetSlideShow_doubleClicked(const QModelIndex &index);

private:
    Ui::PictureWidget *ui;
    QList<SlideShowInfo> slideShows;
    SlideShow currentSlideShow;
    QList<SlideShowItem> slides;
    QList<QPixmap> imagesPreview;
    QList<QPixmap> imagesToShow;
    SlideShowSettings mySettings;
};

#endif // PICTUREWIDGET_HPP
