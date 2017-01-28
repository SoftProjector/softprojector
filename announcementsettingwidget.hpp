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

#ifndef ANNOUNCEMENTSETTINGWIDGET_HPP
#define ANNOUNCEMENTSETTINGWIDGET_HPP

#include <QtWidgets>
#include "theme.hpp"
#include "spfunctions.hpp"
#include "settings.hpp"

namespace Ui {
class AnnouncementSettingWidget;
}

class AnnouncementSettingWidget : public QWidget
{
    Q_OBJECT

public:
    explicit AnnouncementSettingWidget(QWidget *parent = 0);
    ~AnnouncementSettingWidget();

public slots:
    void setSettings(TextSettings& settings, TextSettings &settings2);
    void getSettings(TextSettings& settings, TextSettings &settings2);
    void setDispScreen2Visible(bool visible);
    void setBackgroungds(QString name, QPixmap back);

signals:
    void applyBackToAll(int t, QString backName, QPixmap background);

private slots:
    void loadSettings();
    void on_buttonBackground_clicked();
    void on_pushButtonDefault_clicked();
    void on_checkBoxUseShadow_stateChanged(int arg1);
    void on_groupBoxUseDisp2_toggled(bool arg1);
    void on_checkBoxUseShadow2_stateChanged(int arg1);
    void on_buttonBackground2_clicked();
    void on_toolButtonColor_clicked();
    void on_toolButtonColor2_clicked();
    void on_toolButtonFont_clicked();
    void on_toolButtonFont2_clicked();
    QString getFontText(QFont font);

    void on_pushButtonApplyToAll_clicked();

private:
    TextSettings mySettings, mySettings2;
    Ui::AnnouncementSettingWidget *ui;
protected:
    virtual void changeEvent(QEvent *e);
};

#endif // ANNOUNCEMENTSETTINGWIDGET_HPP
