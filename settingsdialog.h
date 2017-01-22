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

#ifndef SETTINGSDIALOG_H
#define SETTINGSDIALOG_H

#include <QtWidgets>
#include "settings.h"
#include "theme.h"
#include "generalsettingwidget.h"
#include "passivesettingwidget.h"
#include "biblesettingwidget.h"
#include "songsettingwidget.h"
#include "announcementsettingwidget.h"
#include "picturesettingwidget.h"

namespace Ui {
class SettingsDialog;
}

class SettingsDialog : public QDialog {
    Q_OBJECT
    Q_DISABLE_COPY(SettingsDialog)

public:
    explicit SettingsDialog(QWidget *parent = 0);
    virtual ~SettingsDialog();
    void updateSecondaryBibleMenu();

public slots:
    void loadSettings(GeneralSettings& sets, Theme &thm, SlideShowSettings &ssets,
                      BibleVersionSettings &bsets, BibleVersionSettings &bsets2);

signals:
    void updateSettings(GeneralSettings& sets, Theme &thm, SlideShowSettings &ssets,
                        BibleVersionSettings& bsets, BibleVersionSettings& bsets2);
    void positionsDisplayWindow();
    void updateScreen();

private:
    Ui::SettingsDialog *ui;

    int current_display_screen;
    int currentDisplayScreen2;
    bool is_always_on_top;

    GeneralSettings gsettings;
    Theme theme;
    BibleVersionSettings bsettings;
    BibleVersionSettings bsettings2;
    SlideShowSettings ssettings;

    GeneralSettingWidget *generalSettingswidget;
    PassiveSettingWidget *passiveSettingwidget;
    BibleSettingWidget *bibleSettingswidget;
    SongSettingWidget *songSettingswidget;
    PictureSettingWidget *pictureSettingWidget;
    AnnouncementSettingWidget *announcementSettingswidget;

    QPushButton *btnOk;
    QPushButton *btnCancel;
    QPushButton *btnApply;

private slots:
    void on_listWidget_currentRowChanged(int currentRow);
    void setUseDispScreen2(bool toUse);
    void on_buttonBox_clicked(QAbstractButton *button);
    void applySettings();
    void changeTheme(int theme_id);
    void getThemes();
    void setThemes();
    void applyToAllActive(int t, QString backName, QPixmap background);

protected:
    virtual void changeEvent(QEvent *e);
};

#endif // SETTINGSDIALOG_H
