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

#ifndef PASSIVESETTINGWIDGET_HPP
#define PASSIVESETTINGWIDGET_HPP

#include <QWidget>
#include <QFileDialog>
#include <QDebug>
#include "theme.hpp"
#include "settings.hpp"

namespace Ui {
class PassiveSettingWidget;
}

class PassiveSettingWidget : public QWidget
{
    Q_OBJECT
    
public:
    explicit PassiveSettingWidget(QWidget *parent = 0);
    ~PassiveSettingWidget();

public slots:
    void setSetings(TextSettings &settings, TextSettings &settings2);
    void getSettings(TextSettings &settings, TextSettings &settings2);
    void setDispScreen2Visible(bool visible);

private slots:
    void loadSettings();
    void on_buttonBrowseBackgound_clicked();
    void on_groupBoxDisp2Sets_toggled(bool arg1);
    void on_buttonBrowseBackgound2_clicked();
    void on_pushButtonDefault_clicked();

private:
    Ui::PassiveSettingWidget *ui;
    TextSettings mySettings,mySettings2;
protected:
    virtual void changeEvent(QEvent *e);
};

#endif // PASSIVESETTINGWIDGET_HPP
