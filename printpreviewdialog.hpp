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

#ifndef PRINTPREVIEWDIALOG_HPP
#define PRINTPREVIEWDIALOG_HPP

#include <QtGui>
#include <QtCore>
#include <QtPrintSupport>
#include "song.hpp"
#include "bible.hpp"
#include "announcewidget.hpp"
#include "schedule.hpp"

namespace Ui {
class PrintPreviewDialog;
}

class PrintPreviewDialog : public QDialog
{
    Q_OBJECT
    
public:
    explicit PrintPreviewDialog(QWidget *parent = 0);
    ~PrintPreviewDialog();

public slots:
    void setText(Song song);
    void setText(QString bible,QString book,int chapter);
    void setText(Announcement announce);
    void setSchedule(QString scheduleName, const QList<Schedule> &schedule, bool printDetail);

private slots:
    void on_fontComboBox_currentFontChanged(const QFont &f);
    void on_spinBoxFontSize_valueChanged(int arg1);
    void on_pushButtonPDF_clicked();
    void on_pushButtonPrint_clicked();
//    void on_comboBox_currentIndexChanged(const QString &arg1);
    void on_comboBox_currentIndexChanged(int index);

    void updateMargins();
    void on_doubleSpinBoxLeft_editingFinished();
    void on_doubleSpinBoxTop_editingFinished();
    void on_doubleSpinBoxRight_editingFinished();
    void on_doubleSpinBoxBottom_editingFinished();

private:
    QPrinter* printer;
    Ui::PrintPreviewDialog *ui;
};

#endif // PRINTPREVIEWDIALOG_HPP
