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

#ifndef MODULEPROGRESSDIALOG_HPP
#define MODULEPROGRESSDIALOG_HPP

#include <QDialog>

namespace Ui {
class ModuleProgressDialog;
}

class ModuleProgressDialog : public QDialog
{
    Q_OBJECT
    
public:
    explicit ModuleProgressDialog(QWidget *parent = 0);
    ~ModuleProgressDialog();

public slots:
    void setCurrentValue(int value);
    void setCurrentMax(int max);
    void setCurrentMin(int min);
    void setCurrent(int value, int max);
    void setTotalValue(int value);
    void setTotalMax(int max);
    void setTotal(int value, int max);
    void increaseTotal();
    void setSpeed(QString speed);
    void appendText(QString text);
    void clearAll();
    void enableCloseButton(bool enable);
    void setToMax();
    
private slots:
    void on_pushButton_clicked();

private:
    Ui::ModuleProgressDialog *ui;
};

#endif // MODULEPROGRESSDIALOG_HPP
