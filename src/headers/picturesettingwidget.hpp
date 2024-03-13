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

#ifndef PICTURESETTINGWIDGET_HPP
#define PICTURESETTINGWIDGET_HPP

#include <QWidget>
#include <QMessageBox>
#include "settings.hpp"

namespace Ui {
class PictureSettingWidget;
}

class PictureSettingWidget : public QWidget
{
    Q_OBJECT
    
public:
    explicit PictureSettingWidget(QWidget *parent = 0);
    ~PictureSettingWidget();
    void setSettings(SlideShowSettings &settings);
    void getSettings(SlideShowSettings &settings);
    
private slots:
    void on_comboBoxBoundAmount_currentIndexChanged(int index);
    void on_lineEditBound_textChanged(const QString &arg1);
    void on_lineEditBound_editingFinished();

private:
    SlideShowSettings mySettings;
    Ui::PictureSettingWidget *ui;
protected:
    virtual void changeEvent(QEvent *e);
};

#endif // PICTURESETTINGWIDGET_HPP
