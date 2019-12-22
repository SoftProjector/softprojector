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

#ifndef THEME_HPP
#define THEME_HPP

//#include <QtCore>
#include <QtSql>
//#include "spfunctions.hpp"
#include "settings.hpp"
/*
class PassiveSettings
{
public:
    PassiveSettings();
    bool useBackground;
    QString backgroundName;
    QPixmap background;
    bool useDisp2settings;
};

class BibleSettings
{
    // To store Bible projection related settings
public:
    BibleSettings();
    bool useShadow;
    bool useFading;
    bool useBlurShadow;
    bool useBackground;
    QString backgroundName;
    QPixmap background;
    QFont textFont;
    QColor textColor;
    int textAlignmentV;
    int textAlignmentH;
    QFont captionFont;
    QColor captionColor;
    int captionAlignment;
    int captionPosition;
    bool useAbbriviations;
    int screenUse;
    int screenPosition;

    bool useDisp2settings;
};

class SongSettings
{
    // To store Song related settings
public:
    SongSettings();
    bool useFading;
    bool useShadow;
    bool useBlurShadow;
    bool showStanzaTitle;
    bool showSongKey;
    bool showSongNumber;
    QColor infoColor;
    QFont infoFont;
    int infoAling; // 0 = Top, 1 = Bottom
    bool showSongEnding;
    QColor endingColor;
    QFont endingFont;
    int endingType; // 0 = ***, 1 = ---, 2 = °°°, 3 = •••, 4 = ●●●, 5 = ▪▪▪, 6 = ■■■, 7 = for song copyright info
    int endingPosition;
    bool useBackground;
    QString backgroundName; // file path for background image
    QPixmap background;
    QColor textColor;
    QFont textFont;
    int textAlignmentV;
    int textAlignmentH;
    int screenUse;
    int screenPositon; // 0 = Top, 1 = Bottom
    bool useDisp2settings;
};

class AnnounceSettings
{   // To store Announcement related settings
public:
    AnnounceSettings();
    bool useShadow;
    bool useFading;
    bool useBlurShadow;
    bool useBackground;
    QString backgroundName; // file path for background image
    QPixmap background;
    QFont textFont;
    QColor textColor;
    int textAlignmentV;
    int textAlignmentH;

    bool useDisp2settings;
};
*/

class ThemeInfo
{
public:
    ThemeInfo();
    int themeId;
    QString name;
    QString comments;
};

class Theme
{
public:
    Theme();
    TextSettingsBase common;
    TextSettingsBase common2; // Holds secondary display screen settings
    TextSettings passive;
    TextSettings passive2; // Holds secondary display screen settings
    BibleSettings bible;
    BibleSettings bible2; // Holds secondary display screen settings
    SongSettings song;
    SongSettings song2; // Holds secondary display screen settings
    TextSettings announce;
    TextSettings announce2; // Holds secondary display screen settings

public slots:
    void saveThemeNew();
    void saveThemeUpdate();
    void loadTheme();
    void setThemeId(int id){m_info.themeId = id;}
    int getThemeId(){return m_info.themeId;}
    void setThemeInfo(ThemeInfo info);
    ThemeInfo getThemeInfo();

private:
     ThemeInfo m_info;

private slots:
    void savePassiveNew(int screen, TextSettings &settings);
    void saveBibleNew(int screen, BibleSettings &settings);
    void saveSongNew(int screen, SongSettings &settings);
    void saveAnnounceNew(int screen, TextSettings &settings);
    void savePassiveUpdate(int screen, TextSettings &settings);
    void saveBibleUpdate(int screen, BibleSettings &settings);
    void saveSongUpdate(int screen, SongSettings &settings);
    void saveAnnounceUpdate(int screen, TextSettings &settings);
    void loadPassive(int screen, TextSettings &settings);
    void loadBible(int screen, BibleSettings &settings);
    void loadSong(int screen, SongSettings &settings);
    void loadAnnounce(int screen, TextSettings &settings);

};

#endif // THEME_HPP
