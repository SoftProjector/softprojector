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

#ifndef MODULEDOWNLOADDIALOG_H
#define MODULEDOWNLOADDIALOG_H

#include <QDialog>

namespace Ui {
class ModuleDownloadDialog;
}

class ModuleDownloadDialog : public QDialog
{
    Q_OBJECT
    
public:
    explicit ModuleDownloadDialog(QWidget *parent = 0);
    ~ModuleDownloadDialog();

public slots:
    void setList(QStringList &modList);
    QList<int> getSelected();
    
private slots:
    void on_buttonBox_accepted();
    void on_buttonBox_rejected();
    void on_pushButtonSelectAll_clicked();
    void on_pushButtonDeselectAll_clicked();

private:
    Ui::ModuleDownloadDialog *ui;
};

#endif // MODULEDOWNLOADDIALOG_H
