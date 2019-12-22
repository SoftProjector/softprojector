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

#ifndef SETTINGS_HPP
#define SETTINGS_HPP

#include <QtSql>
#include <QPixmap>
#include "spfunctions.hpp"

void saveIndividualSettings(QSqlQuery &sq, QString sId, int tId, QString name, const QVariant &value);
void updateIndividualSettings(QSqlQuery &sq, QString sId, int tId, QString name, const QVariant &value);

enum HorizontalAlignment
{
    A_TOP,
    A_MIDDLE,
    A_BOTTOM
};

enum VerticalAlignment
{
    A_LEFT,
    A_CENTER,
    A_RIGHT
};

enum TransitionType
{
    TR_NONE,
    TR_FADE,
    TR_FADE_OUT,
    TR_MOVE_RIGHT,
    TR_MOVE_LEFT,
    TR_MOVE_UP,
    TR_MOVE_DOWN,
    TR_COMMON
};

enum EffectType
{
    E_NONE,
    E_SHADOW,
    E_BLURRED_SHADDOW,
    E_COMMON
};

enum BackgroundType
{
    B_NONE,
    B_SOLID_COLOR,
    B_PICTURE,
    B_VIDEO,
    B_VIDEO_PLAYBACK,
    B_COMMON
};

enum ScreenPosition
{
    TOP_OF_SCREEN,
    BOTTOM_OF_SCREEN
};

enum SongEndingType
{
    SE_STAR,
    SE_DASH,
    SE_CIRCLE,
    SE_SMALL_DOT,
    SE_BIG_DOT,
    SE_SMALL_BOX,
    SE_BIG_BOX,
    SE_COPYRIGHT
};

class TextSettingsBase
{
public:
    TextSettingsBase();

    QString id;
    int themeId;

    // if to update bool handles
    bool isChangedTextFont, isChangedTextColor, isChangedTextShadowColor;
    bool isChangedAlingV, isChangedAlingH, isChangesTranType, isChangedEffectType;
    bool isChangedBackType, isChangedBackColor, isChangedBackPix, isChangedBackVid;
    bool isChangedScreenUse, isChangedScreenPos, isChangedSameDisp2;

    //Text
    QFont textFont;
    QColor textColor;
    QColor textShadowColor;
    int textAlignmentV;
    // 0 - Top, 1 - Middle, 3 - Bottom
    int textAlignmentH;
    // 0 - Left, 1 - Center, 3 - Right

    //Effects
    int transitionType;
    // -1 - Common, 0 - None, 1 - Fade, 2 - Fade out->in,
    // 3 - Move Right, 4 - Move Left, 5 - Move Up, 6 - Move Down
    int effectsType;
    // -1 - Common, 0 - None, 1 - Shadow, 2 - Blurred Shadow

    //Background
    int backgroundType;
    // -1 - Common, 0 - Solid Color, 1 - Picture, 2 - Video
    QColor backgroundColor;
    QString backgroundName;
    QPixmap backgroundPix; //TODO: Rename to backgroundPix
    QString backgroundVideoPath;


    // older implementation, Use it for now
    bool useShadow;
    bool useFading;
    bool useBlurShadow;
    bool useBackground;
    bool useDisp2settings;

    //Layout
    int screenUse;
    int screenPosition;
    // 0 - Top of Screen, 1 - Bottom of Screen

    bool useSameForDisp2;

    void saveBase();
    void saveBase(QSqlQuery &sq);
    void updateBase();
    void updateBase(QSqlQuery &sq);
    void loadBase();
    void loadBase(QSqlQuery &sq);

    void setBaseChangeHandles();
    void resetBaseChangeHandles();
};

class TextSettings : public TextSettingsBase
{
public:
    TextSettings();
    //Text
    bool isNotCommonFont;
    bool isNotCommonColor;

    //Layout
    bool isNotCommonLayout;

    // Change Handles
    bool isChangedNotFont, isChangedNotColor, isChangedNotLayout;

    void saveMain();
    void saveMain(QSqlQuery &sq);
    void updateMain();
    void updateMain(QSqlQuery &sq);
    void loadMain();
    void loadMain(QSqlQuery &sq);

    void setMainChangeHandles();
    void resetMainChangeHandles();

};

class BibleVersionSettings
{
public:
    BibleVersionSettings();
    QString primaryBible;
    QString secondaryBible;
    QString trinaryBible;
    QString operatorBible;
    bool settingsChanged;
};

class BibleSettings : public TextSettings
{
public:
    BibleSettings();
    QFont captionFont;
    bool isNotSameFont;
    QColor captionColor;
    bool isNotSameColor;
    QColor captionShadowColor;
    int captionAlignment;
    int captionPosition;
    bool useAbbriviation;

    // Change Handles
    bool isChangedCapFont, isChangedCapColor, isChangedCapShadColor, isChangedNotSameFont;
    bool isChangedNotSameColor, isChangedCapAlign, isChangedCapPos, isChangedUseAbbriv;

    void save();
    void save(QSqlQuery &sq);
    void update();
    void update(QSqlQuery &sq);
    void load();
    void load(QSqlQuery &sq);

    void setChangeHandes();
    void resetChangeHandles();

    BibleVersionSettings versions;
};

class SongSettings : public TextSettings
{
public:
    SongSettings();
    // Stanza Details
    bool showStanzaTitle;
    bool showSongKey;
    bool showSongNumber;
    bool showSongEnding;
    // Info
    QFont infoFont;
    QColor infoColor;
    QColor infoShadowColor;
    bool isNotSameInfoFont;
    bool isNotSameInfoColor;
    int infoAling; // 0 = Top, 1 = Bottom
    //Ending
    QFont endingFont;
    QColor endingColor;
    QColor endingShadowColor;
    bool isNotSameEndingFont;
    bool isNotSameEndingColor;
    int endingType; // 0 = ***, 1 = ---, 2 = °°°, 3 = •••, 4 = ●●●, 5 = ▪▪▪, 6 = ■■■, 7 = for song copyright info
    int endingPosition;

    // Change Handles
    bool isChangedShowTitle, isChangedShowKey, isChangedShowNum, isChangedShowEnding;
    bool isChangedInfoFont, isChangedInfoColor, isChangedInfoShadColor, isChangedNotSameInfoFont;
    bool isChangedNotSameInfoColor, isChangedInfoAlign, isChangedEndingFont, isChangedEndingColor;
    bool isChangedEndingShadColor, isChangedNotSameEndingFont, isChangedNotSameEndingColor;
    bool isChangedEndingType, isChangedEndingPosition;

    void save();
    void save(QSqlQuery &sq);
    void update();
    void update(QSqlQuery &sq);
    void load();
    void load(QSqlQuery &sq);

    void setChangeHandes();
    void resetChangeHandles();
};

class DisplayControlsSettings
{
public:
    DisplayControlsSettings();
    int buttonSize;
    int alignmentV;
    int alignmentH;
    qreal opacity;
};

class GeneralSettings
{   // To store General Program Settings
public:
    GeneralSettings();
    bool displayIsOnTop;
    int displayScreen; // stores primary display screen location
    int displayScreen2; // stores secondary display screen location
    DisplayControlsSettings displayControls;
    int currentThemeId;
    bool displayOnStartUp;
    bool settingsChangedAll;
    bool settingsChangedMulti;
    bool settingsChangedSingle;
};

class DisplaySettings
{   // to store display settings for concurrent projection
public:
    bool useBackground;
    QString backgroundPath;
    QFont textFont;
    QColor textColor;
    int textAlignmentV;
    int textAlignmentH;
};

class SpSettings
{   // stores main window settings, none user modifiable
public:
    SpSettings();
    QByteArray spSplitter;
    QByteArray bibleHiddenSplitter;
    QByteArray bibleShowSplitter;
    QByteArray songSplitter;
    bool isWindowMaximized;
    QString uiTranslation;
};

class SlideShowSettings
{
public:
    SlideShowSettings();
    bool expandSmall;
    int fitType;
    bool resize;
    int boundType;
    int boundWidth;
    bool settingsChanged;
    int transitionType;
};

class Settings
{
public:
    Settings();
    GeneralSettings general;
    SpSettings spMain;
    BibleVersionSettings bibleSets;
    BibleVersionSettings bibleSets2;
    SlideShowSettings slideSets;

    bool isSpClosing;

public slots:
    void loadSettings();
    void saveSettings();
    void saveNewSettings();

private slots:
    QByteArray textToByt(QString text);
};

#endif // SETTINGS_HPP
