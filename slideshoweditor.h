/***************************************************************************
//
//    softProjector - an open source media projection software
//    Copyright (C) 2014  Vladislav Kobzar, Matvey Adzhigirey and Ilya Spivakov
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

#ifndef SLIDESHOWEDITOR_H
#define SLIDESHOWEDITOR_H

//#include <QWidget>
#include <QDialog>
#include <QFileDialog>
#include <QMessageBox>
#include <QtSql>

#include "slideshow.h"
#include "settings.h"

namespace Ui {
class SlideShowEditor;
}

//class SlideShowEditor : public QWidget
class SlideShowEditor : public QDialog
{
    Q_OBJECT
    Q_DISABLE_COPY(SlideShowEditor)// for Dialog test
public:
    explicit SlideShowEditor(QWidget *parent = 0);
    ~SlideShowEditor();
    void setSettings(SlideShowSettings &settings){mySettings = settings;}
    void setSlideShow(SlideShow ss);
    
private slots:
    void loadSlideShow();
    void reloadSlides();
    void updateButtonState();

    void on_pushButtonAddImages_clicked();
    void on_pushButtonRemoveImage_clicked();
    void on_pushButtonMoveUp_clicked();
    void on_pushButtonMoveDown_clicked();
    void on_pushButtonSave_clicked();
    void on_pushButtonCancel_clicked();
    void on_listWidgetSlides_currentRowChanged(int currentRow);

private:
    Ui::SlideShowEditor *ui;
    SlideShow editSS;
    QProgressDialog progress;
    QList<int> deleteList;
    SlideShowSettings mySettings;
};

#endif // SLIDESHOWEDITOR_H
