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

#ifndef GENERALSETTINGWIDGET_HPP
#define GENERALSETTINGWIDGET_HPP

#include <QtWidgets>
#include "settings.hpp"
#include "addsongbookdialog.hpp"
#include "theme.hpp"

namespace Ui {
class GeneralSettingWidget;
}

class GeneralSettingWidget : public QWidget
{
    Q_OBJECT

public:
    explicit GeneralSettingWidget(QWidget *parent = 0);
    ~GeneralSettingWidget();

private:
    Ui::GeneralSettingWidget *ui;
    GeneralSettings mySettings;
    Settings allSetings;
    QStringList monitors;
    QStringList themes;
    QList<int> themeIdList;

public slots:
    void setSettings(GeneralSettings settings);
    void updateSecondaryDisplayScreen();
    GeneralSettings getSettings();

signals:
    void setDisp2Use(bool toUse);
    void themeChanged(int theme_id);

private slots:
    void on_pushButtonDefault_clicked();
    void loadThemes();
    void loadSettings();
    void on_comboBoxDisplayScreen_activated(const QString &arg1);
    void on_comboBoxDisplayScreen_2_activated(int index);
    void on_pushButtonAddTheme_clicked();
    void on_comboBoxTheme_activated(int index);
protected:
    virtual void changeEvent(QEvent *e);
};

#endif // GENERALSETTINGWIDGET_HPP
