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

#ifndef SONGSETTINGWIDGET_HPP
#define SONGSETTINGWIDGET_HPP

//#include <QWidget>
#include <QtWidgets>
#include "theme.hpp"

namespace Ui {
class SongSettingWidget;
}

class SongSettingWidget : public QWidget
{
    Q_OBJECT
public:
    explicit SongSettingWidget(QWidget *parent = 0);
    ~SongSettingWidget();

public slots:
    void getSettings(SongSettings &settings, SongSettings &settings2, SongSettings &settings3, SongSettings &settings4);
    void setSettings(SongSettings &settings, SongSettings &settings2, SongSettings &settings3, SongSettings &settings4);
    void setDispScreen2Visible(bool visible);
    void setDispScreen3Visible(bool visible);
    void setDispScreen4Visible(bool visible);
    void setBackgroungds(QString name, QPixmap back);

signals:
    void applyBackToAll(int t, QString backName, QPixmap background);

private slots:
    void loadSettings();
    void on_checkBoxUseShadow_stateChanged(int arg1);
    void on_checkBoxUseShadow2_stateChanged(int arg1);
    void on_checkBoxUseShadow3_stateChanged(int arg1);
    void on_checkBoxUseShadow4_stateChanged(int arg1);
    void on_toolButtonInfoColor_clicked();
    void on_toolButtonInfoColor2_clicked();
    void on_toolButtonInfoColor3_clicked();
    void on_toolButtonInfoColor4_clicked();
    void on_toolButtonInfoFont_clicked();
    void on_toolButtonInfoFont2_clicked();
    void on_toolButtonInfoFont3_clicked();
    void on_toolButtonInfoFont4_clicked();
    void on_toolButtonEndingColor_clicked();
    void on_toolButtonEndingColor2_clicked();
    void on_toolButtonEndingColor3_clicked();
    void on_toolButtonEndingColor4_clicked();
    void on_toolButtonEndingFont_clicked();
    void on_toolButtonEndingFont2_clicked();
    void on_toolButtonEndingFont3_clicked();
    void on_toolButtonEndingFont4_clicked();
    void on_buttonSongBackground_clicked();
    void on_buttonSongBackground2_clicked();
    void on_buttonSongBackground3_clicked();
    void on_buttonSongBackground4_clicked();
    void on_toolButtonColor_clicked();
    void on_toolButtonColor2_clicked();
    void on_toolButtonColor3_clicked();
    void on_toolButtonColor4_clicked();
    void on_toolButtonFont_clicked();
    void on_toolButtonFont2_clicked();
    void on_toolButtonFont3_clicked();
    void on_toolButtonFont4_clicked();
    void on_groupBoxSongAddBKToText_toggled(bool arg1);
    void on_groupBoxSongAddBKToText2_toggled(bool arg1);
    void on_groupBoxSongAddBKToText3_toggled(bool arg1);
    void on_groupBoxSongAddBKToText4_toggled(bool arg1);
    void on_tBSongAddBKColorText_clicked();
    void on_tBSongAddBKColorText2_clicked();
    void on_tBSongAddBKColorText3_clicked();
    void on_tBSongAddBKColorText4_clicked();
    void on_tBSongChangeGenBKColor_clicked();
    void on_tBSongChangeGenBKColor2_clicked();
    void on_tBSongChangeGenBKColor3_clicked();
    void on_tBSongChangeGenBKColor4_clicked();
    void on_groupBoxDisplay2_toggled(bool arg1);
    void on_groupBoxDisplay3_toggled(bool arg1);
    void on_groupBoxDisplay4_toggled(bool arg1);
    void on_pushButtonDefault_clicked();
    QString getFontText(QFont font);
    void on_pushButtonApplyToAll_clicked();

private:
    SongSettings mySettings;
    SongSettings mySettings2;
    SongSettings mySettings3;
    SongSettings mySettings4;

    Ui::SongSettingWidget *ui;
protected:
    virtual void changeEvent(QEvent *e);
};

#endif // SONGSETTINGWIDGET_HPP
