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

#include "settings.hpp"

TextSettingsBase::TextSettingsBase()
{
    id = "-1";
    themeId = -1;
    textFont.fromString("Arial,16,-1,5,50,0,0,0,0,0");
    textColor = QColor(Qt::white);
    textShadowColor = QColor(Qt::black);
    textAlignmentV = 0;
    textAlignmentH = 0;
    backgroundType = 0;
    backgroundColor = QColor(Qt::black);
    backgroundName = "";
    backgroundPix = QPixmap(1,1);
    backgroundVideoPath = "";
    screenUse = 100;
    screenPosition = 0;
    transitionType = 0;
    effectsType = 1;
    useSameForDisp2 = true;

    // older implementation, Use it for now
    useShadow = true;
    useFading = true;
    useBlurShadow = false;
    useBackground = false;
    useDisp2settings = false;

    setBaseChangeHandles();
}

TextSettings::TextSettings()
{
    isNotCommonFont = false;
    isNotCommonColor = false;
    isNotCommonLayout = false;
    transitionType = -1;
    effectsType = -1;
    backgroundType = -1;
    setMainChangeHandles();
}

BibleSettings::BibleSettings()
{
    captionFont.fromString("Arial,15,-1,5,50,0,0,0,0,0");
    isNotSameFont = true;
    captionColor = QColor(Qt::white);
    isNotSameColor = false;
    captionShadowColor = QColor(Qt::black);
    captionAlignment = 2;
    captionPosition = 1;
    useAbbriviation = false;
    setChangeHandes();
}

SongSettings::SongSettings()
{
    textAlignmentV = 1;
    textAlignmentH = 1;
    showStanzaTitle = false;
    showSongKey = false;
    showSongNumber = false;
    showSongEnding = true;
    isNotSameInfoFont = false;
    isNotSameInfoColor = false;
    infoColor = QColor(Qt::white);
    infoShadowColor = QColor(Qt::black);
    infoFont.fromString("Arial,15,-1,5,50,0,0,0,0,0");
    infoAling = 0;
    isNotSameEndingFont = false;
    isNotSameEndingColor = false;
    endingColor = QColor(Qt::white);
    endingShadowColor = QColor(Qt::black);
    endingFont.fromString("Arial,16,-1,5,50,0,0,0,0,0");
    endingType = 0;
    endingPosition = 0;
    setChangeHandes();
}

void saveIndividualSettings(QSqlQuery &sq, QString sId, int tId, QString name, const QVariant &value)
{
    sq.addBindValue(sId);
    if(tId!=-2)
        sq.addBindValue(tId);
    sq.addBindValue(name);
    sq.addBindValue(value);
    sq.exec();
    qDebug()<<"save:"<<name<<sId<<tId<<value;
}

void updateIndividualSettings(QSqlQuery &sq, QString sId, int tId, QString name, const QVariant &value)
{
    sq.addBindValue(value);
    sq.addBindValue(sId);
    if(tId!=-2)
    {
        sq.addBindValue(tId);
        qDebug()<<"TID";
    }
    sq.addBindValue(name);
    sq.exec();
    qDebug()<<"update:"<<name<<sId<<tId<<value;
}

//******************************************************************/
//********************* TextSettingsBase ***************************/
//******************************************************************/
void TextSettingsBase::setBaseChangeHandles()
{
    isChangedTextFont = isChangedTextColor = isChangedTextShadowColor =
            isChangedAlingV = isChangedAlingH = isChangesTranType =
            isChangedEffectType = isChangedBackType = isChangedBackColor =
            isChangedBackPix = isChangedBackVid = isChangedScreenUse =
            isChangedScreenPos = isChangedSameDisp2 = true;
}

void TextSettingsBase::resetBaseChangeHandles()
{
    isChangedTextFont = isChangedTextColor = isChangedTextShadowColor =
            isChangedAlingV = isChangedAlingH = isChangesTranType =
            isChangedEffectType = isChangedBackType = isChangedBackColor =
            isChangedBackPix = isChangedBackVid = isChangedScreenUse =
            isChangedScreenPos = isChangedSameDisp2 = false;
}

void TextSettingsBase::loadBase()
{
    QSqlQuery sq;
    loadBase(sq);
}

void TextSettingsBase::loadBase(QSqlQuery &sq)
{
    if(id == "-1" || themeId == -1)
        return;

    QString n;
    sq.exec(QString("SELECT sname, svalue, sdata FROM ThemeData WHERE sid = '%1' AND theme_id = %2").arg(id).arg(themeId));
    while(sq.next())
    {
        n = sq.value(0).toString();
        if(n == "textFont")
            textFont.fromString(sq.value(1).toString());
        else if(n == "textColor")
            textColor = QColor(sq.value(1).toUInt());
        else if(n == "textShadowColor")
            textShadowColor = QColor(sq.value(1).toUInt());
        else if(n == "textAlignmentV")
            textAlignmentV = sq.value(1).toInt();
        else if(n == "textAlignmentH")
            textAlignmentH = sq.value(1).toInt();
        else if(n == "backgroundType")
            backgroundType = sq.value(1).toInt();
        else if(n == "backgroundColor")
            backgroundColor = QColor(sq.value(1).toUInt());
        else if(n == "backgroundPix")
        {
            backgroundName = sq.value(1).toString();
            backgroundPix.loadFromData(sq.value(2).toByteArray());
        }
        else if(n == "backgroundVideoPath")
            backgroundVideoPath = sq.value(1).toString();
        else if(n == "screenUse")
            screenUse = sq.value(1).toInt();
        else if(n == "screenPosition")
            screenPosition = sq.value(1).toInt();
        else if(n == "transitionType")
            transitionType = sq.value(1).toInt();
        else if(n == "effectsType")
            effectsType = sq.value(1).toInt();
        else if(n == "useSameForDisp2")
            useSameForDisp2 = sq.value(1).toBool();
    }
    resetBaseChangeHandles();
}

void TextSettingsBase::saveBase()
{
    QSqlQuery sq;
    saveBase(sq);
}

void TextSettingsBase::saveBase(QSqlQuery &sq)
{
    if(id == "-1" || themeId == -1)
        return;

    sq.prepare("INSERT INTO ThemeData (sid, theme_id, sname, svalue) VALUES (?,?,?,?)");

    saveIndividualSettings(sq,id,themeId,"textFont",textFont.toString());
    saveIndividualSettings(sq,id,themeId,"textColor",textColor.rgb());
    saveIndividualSettings(sq,id,themeId,"textShadowColor",textShadowColor.rgb());
    saveIndividualSettings(sq,id,themeId,"textAlignmentV",textAlignmentV);
    saveIndividualSettings(sq,id,themeId,"textAlignmentH",textAlignmentH);
    saveIndividualSettings(sq,id,themeId,"transitionType",transitionType);
    saveIndividualSettings(sq,id,themeId,"effectsType",effectsType);
    saveIndividualSettings(sq,id,themeId,"backgroundType",backgroundType);
    saveIndividualSettings(sq,id,themeId,"backgroundColor",backgroundColor.rgb());
    saveIndividualSettings(sq,id,themeId,"backgroundVideoPath",backgroundVideoPath);
    saveIndividualSettings(sq,id,themeId,"screenUse",screenUse);
    saveIndividualSettings(sq,id,themeId,"screenPosition",screenPosition);
    saveIndividualSettings(sq,id,themeId,"useSameForDisp2",useSameForDisp2);

    sq.prepare("INSERT INTO ThemeData (sid, theme_id, sname, svalue, sdata) VALUES (?,?,?,?,?)");
    sq.addBindValue(id);
    sq.addBindValue(themeId);
    sq.addBindValue("backgroundPix");
    sq.addBindValue(backgroundName);
    sq.addBindValue(pixToByte(backgroundPix));
    sq.exec();

    resetBaseChangeHandles();
}

void TextSettingsBase::updateBase()
{
    QSqlQuery sq;
    updateBase(sq);
}

void TextSettingsBase::updateBase(QSqlQuery &sq)
{
    if(id == "-1" || themeId == -1)
        return;

    sq.prepare("UPDATE ThemeData SET svalue = ? WHERE sid = ? AND theme_id = ? AND sname = ?");

    if(isChangedTextFont)
        updateIndividualSettings(sq,id,themeId,"textFont",textFont.toString());

    if(isChangedTextColor)
        updateIndividualSettings(sq,id,themeId,"textColor",textColor.rgb());

    if(isChangedTextShadowColor)
        updateIndividualSettings(sq,id,themeId,"textShadowColor",textShadowColor.rgb());

    if(isChangedAlingV)
        updateIndividualSettings(sq,id,themeId,"textAlignmentV",textAlignmentV);

    if(isChangedAlingH)
        updateIndividualSettings(sq,id,themeId,"textAlignmentH",textAlignmentH);

    if(isChangesTranType)
        updateIndividualSettings(sq,id,themeId,"transitionType",transitionType);

    if(isChangedEffectType)
        updateIndividualSettings(sq,id,themeId,"effectsType",effectsType);

    if(isChangedBackType)
        updateIndividualSettings(sq,id,themeId,"backgroundType",backgroundType);

    if(isChangedBackColor)
        updateIndividualSettings(sq,id,themeId,"backgroundColor",backgroundColor.rgb());

    if(isChangedBackVid)
        updateIndividualSettings(sq,id,themeId,"backgroundVideoPath",backgroundVideoPath);

    if(isChangedScreenUse)
        updateIndividualSettings(sq,id,themeId,"screenUse",screenUse);

    if(isChangedScreenPos)
        updateIndividualSettings(sq,id,themeId,"screenPosition",screenPosition);

    if(isChangedSameDisp2)
        updateIndividualSettings(sq,id,themeId,"useSameForDisp2",useSameForDisp2);

    if(isChangedBackPix)
    {
        sq.prepare("UPDATE ThemeData SET svalue = ?, sdata = ? WHERE sid = ? AND theme_id = ? AND sname = ?");
        sq.addBindValue(backgroundName);
        sq.addBindValue(pixToByte(backgroundPix));
        sq.addBindValue(id);
        sq.addBindValue(themeId);
        sq.addBindValue("backgroundPix");
        sq.exec();
        qDebug()<<"backgroundPix Updated"<<id;
    }

    resetBaseChangeHandles();
}

//******************************************************************/
//********************* TextSettings *******************************/
//******************************************************************/
void TextSettings::setMainChangeHandles()
{
    setBaseChangeHandles();
    isChangedNotFont = isChangedNotColor = isChangedNotLayout = true;
}

void TextSettings::resetMainChangeHandles()
{
    resetBaseChangeHandles();
    isChangedNotFont = isChangedNotColor = isChangedNotLayout = false;
}

void TextSettings::loadMain()
{
    QSqlQuery sq;
    loadMain(sq);
}

void TextSettings::loadMain(QSqlQuery &sq)
{
    if(id == "-1" || themeId == -1)
        return;

    loadBase(sq);

    QString n;
    sq.exec(QString("SELECT sname, svalue, sdata FROM ThemeData WHERE sid = '%1' AND theme_id = %2").arg(id).arg(themeId));
    while(sq.next())
    {
        n = sq.value(0).toString();
        if(n == "isNotCommonFont")
            isNotCommonFont = sq.value(1).toBool();
        else if(n == "isNotCommonColor")
            isNotCommonColor = sq.value(1).toBool();
        else if(n == "isNotCommonLayout")
            isNotCommonLayout = sq.value(1).toBool();
    }
    resetMainChangeHandles();
}

void TextSettings::saveMain()
{
    QSqlQuery sq;
    saveMain(sq);
}

void TextSettings::saveMain(QSqlQuery &sq)
{
    if(id == "-1" || themeId == -1)
        return;

    saveBase(sq);

    sq.prepare("INSERT INTO ThemeData (sid, theme_id, sname, svalue) VALUES (?,?,?,?)");

    saveIndividualSettings(sq,id,themeId,"isNotCommonFont",isNotCommonFont);
    saveIndividualSettings(sq,id,themeId,"isNotCommonColor",isNotCommonColor);
    saveIndividualSettings(sq,id,themeId,"isNotCommonLayout",isNotCommonLayout);
}

void TextSettings::updateMain()
{
    QSqlQuery sq;
    updateMain(sq);
}

void TextSettings::updateMain(QSqlQuery &sq)
{
    if(id == "-1" || themeId == -1)
        return;

    updateBase(sq);

    sq.prepare("UPDATE ThemeData SET svalue = ? WHERE sid = ? AND theme_id = ? AND sname = ?");

    if(isChangedNotFont)
        updateIndividualSettings(sq,id,themeId,"isNotCommonFont",isNotCommonFont);

    if(isChangedNotColor)
        updateIndividualSettings(sq,id,themeId,"isNotCommonColor",isNotCommonColor);

    if(isChangedNotLayout)
        updateIndividualSettings(sq,id,themeId,"isNotCommonLayout",isNotCommonLayout);
}

//******************************************************************/
//********************* BibleSettings ******************************/
//******************************************************************/
void BibleSettings::setChangeHandes()
{
    setMainChangeHandles();
    isChangedCapFont = isChangedCapColor = isChangedCapShadColor =
            isChangedNotSameFont = isChangedNotSameColor = isChangedCapAlign =
            isChangedCapPos = isChangedUseAbbriv = true;
}

void BibleSettings::resetChangeHandles()
{
    resetMainChangeHandles();
    isChangedCapFont = isChangedCapColor = isChangedCapShadColor =
            isChangedNotSameFont = isChangedNotSameColor = isChangedCapAlign =
            isChangedCapPos = isChangedUseAbbriv = false;
}

void BibleSettings::load()
{
    QSqlQuery sq;
    load(sq);
}

void BibleSettings::load(QSqlQuery &sq)
{
    if(id == "-1" || themeId == -1)
        return;

    loadMain(sq);

    QString n;
    sq.exec(QString("SELECT sname, svalue, sdata FROM ThemeData WHERE sid = '%1' AND theme_id = %2").arg(id).arg(themeId));
    while(sq.next())
    {
        n = sq.value(0).toString();
        if(n == "captionFont")
            captionFont.fromString(sq.value(1).toString());
        else if(n == "isNotSameFont")
            isNotSameFont = sq.value(1).toBool();
        else if(n == "captionColor")
            captionColor = QColor(sq.value(1).toUInt());
        else if(n == "isNotSameColor")
            isNotSameColor = sq.value(1).toBool();
        else if(n == "captionShadowColor")
            captionShadowColor = QColor(sq.value(1).toUInt());
        else if(n == "captionAlignment")
            captionAlignment = sq.value(1).toInt();
        else if(n == "captionPosition")
            captionPosition = sq.value(1).toInt();
        else if(n == "useAbbriviation")
            useAbbriviation = sq.value(1).toBool();
    }
    resetChangeHandles();
}

void BibleSettings::save()
{
    QSqlQuery sq;
    save(sq);
}

void BibleSettings::save(QSqlQuery &sq)
{
    if(id == "-1" || themeId == -1)
        return;

    saveMain(sq);

    sq.prepare("INSERT INTO ThemeData (sid, theme_id, sname, svalue) VALUES (?,?,?,?)");

    saveIndividualSettings(sq,id,themeId,"captionFont",captionFont.toString());
    saveIndividualSettings(sq,id,themeId,"isNotSameFont",isNotSameFont);
    saveIndividualSettings(sq,id,themeId,"captionColor",captionColor.rgb());
    saveIndividualSettings(sq,id,themeId,"isNotSameColor",isNotSameColor);
    saveIndividualSettings(sq,id,themeId,"captionShadowColor",captionShadowColor.rgb());
    saveIndividualSettings(sq,id,themeId,"captionAlignment",captionAlignment);
    saveIndividualSettings(sq,id,themeId,"captionPosition",captionPosition);
    saveIndividualSettings(sq,id,themeId,"useAbbriviation",useAbbriviation);
}

void BibleSettings::update()
{
    QSqlQuery sq;
    update(sq);
}

void BibleSettings::update(QSqlQuery &sq)
{
    if(id == "-1" || themeId == -1)
        return;

    updateMain(sq);

    sq.prepare("UPDATE ThemeData SET svalue = ? WHERE sid = ? AND theme_id = ? AND sname = ?");

    if(isChangedCapFont)
        updateIndividualSettings(sq,id,themeId,"captionFont",captionFont.toString());

    if(isChangedNotSameFont)
        updateIndividualSettings(sq,id,themeId,"isNotSameFont",isNotSameFont);

    if(isChangedCapColor)
        updateIndividualSettings(sq,id,themeId,"captionColor",captionColor.rgb());

    if(isChangedNotSameColor)
        updateIndividualSettings(sq,id,themeId,"isNotSameColor",isNotSameColor);

    if(isChangedCapShadColor)
        updateIndividualSettings(sq,id,themeId,"captionShadowColor",captionShadowColor.rgb());

    if(isChangedCapAlign)
        updateIndividualSettings(sq,id,themeId,"captionAlignment",captionAlignment);

    if(isChangedCapPos)
        updateIndividualSettings(sq,id,themeId,"captionPosition",captionPosition);

    if(isChangedUseAbbriv)
        updateIndividualSettings(sq,id,themeId,"useAbbriviation",useAbbriviation);
}

//******************************************************************/
//********************* SongSettings *******************************/
//******************************************************************/
void SongSettings::setChangeHandes()
{
    setMainChangeHandles();
    isChangedShowTitle = isChangedShowKey = isChangedShowNum = isChangedShowEnding =
            isChangedInfoFont = isChangedInfoColor = isChangedInfoShadColor =
            isChangedNotSameInfoFont = isChangedNotSameInfoColor = isChangedInfoAlign =
            isChangedEndingFont = isChangedEndingColor = isChangedEndingShadColor =
            isChangedNotSameEndingFont = isChangedNotSameEndingColor =
            isChangedEndingType = isChangedEndingPosition = true;
}

void SongSettings::resetChangeHandles()
{
    resetMainChangeHandles();
    isChangedShowTitle = isChangedShowKey = isChangedShowNum = isChangedShowEnding =
            isChangedInfoFont = isChangedInfoColor = isChangedInfoShadColor =
            isChangedNotSameInfoFont = isChangedNotSameInfoColor = isChangedInfoAlign =
            isChangedEndingFont = isChangedEndingColor = isChangedEndingShadColor =
            isChangedNotSameEndingFont = isChangedNotSameEndingColor =
            isChangedEndingType = isChangedEndingPosition = false;
}

void SongSettings::load()
{
    QSqlQuery sq;
    load(sq);
}

void SongSettings::load(QSqlQuery &sq)
{
    if(id == "-1" || themeId == -1)
        return;

    loadMain(sq);

    QString n;
    sq.exec(QString("SELECT sname, svalue, sdata FROM ThemeData WHERE sid = '%1' AND theme_id = %2").arg(id).arg(themeId));
    while(sq.next())
    {
        n = sq.value(0).toString();
        if(n == "showStanzaTitle")
            showStanzaTitle = sq.value(1).toBool();
        else if(n == "showSongKey")
            showSongKey = sq.value(1).toBool();
        else if(n == "showSongNumber")
            showSongNumber = sq.value(1).toBool();
        else if(n == "showSongEnding")
            showSongEnding = sq.value(1).toBool();
        else if(n == "infoFont")
            infoFont.fromString(sq.value(1).toString());
        else if(n == "infoColor")
            infoColor = QColor(sq.value(1).toUInt());
        else if(n == "infoShadowColor")
            infoShadowColor = QColor(sq.value(1).toUInt());
        else if(n == "isNotSameInfoFont")
            isNotSameInfoFont = sq.value(1).toBool();
        else if(n == "isNotSameInfoColor")
            isNotSameInfoColor = sq.value(1).toBool();
        else if(n == "infoAling")
            infoAling = sq.value(1).toInt();
        else if(n == "endingFont")
            endingFont.fromString(sq.value(1).toString());
        else if(n == "endingColor")
            endingColor = QColor(sq.value(1).toUInt());
        else if(n == "endingShadowColor")
            endingShadowColor = QColor(sq.value(1).toUInt());
        else if(n == "isNotSameEndingFont")
            isNotSameEndingFont = sq.value(1).toBool();
        else if(n == "isNotSameEndingColor")
            isNotSameEndingColor = sq.value(1).toBool();
        else if(n == "endingType")
            endingType = sq.value(1).toInt();
        else if(n == "endingPosition")
            endingPosition = sq.value(1).toInt();
    }
    resetChangeHandles();
}

void SongSettings::save()
{
    QSqlQuery sq;
    save(sq);
}

void SongSettings::save(QSqlQuery &sq)
{
    if(id == "-1" || themeId == -1)
        return;

    saveMain(sq);

    sq.prepare("INSERT INTO ThemeData (sid, theme_id, sname, svalue) VALUES (?,?,?,?)");

    saveIndividualSettings(sq,id,themeId,"showStanzaTitle",showStanzaTitle);
    saveIndividualSettings(sq,id,themeId,"showSongKey",showSongKey);
    saveIndividualSettings(sq,id,themeId,"showSongNumber",showSongNumber);
    saveIndividualSettings(sq,id,themeId,"showSongEnding",showSongEnding);
    saveIndividualSettings(sq,id,themeId,"infoFont",infoFont.toString());
    saveIndividualSettings(sq,id,themeId,"infoColor",infoColor.rgb());
    saveIndividualSettings(sq,id,themeId,"infoShadowColor",infoShadowColor.rgb());
    saveIndividualSettings(sq,id,themeId,"isNotSameInfoFont",isNotSameInfoFont);
    saveIndividualSettings(sq,id,themeId,"isNotSameInfoColor",isNotSameInfoColor);
    saveIndividualSettings(sq,id,themeId,"infoAling",infoAling);
    saveIndividualSettings(sq,id,themeId,"endingFont",endingFont.toString());
    saveIndividualSettings(sq,id,themeId,"endingColor",endingColor.rgb());
    saveIndividualSettings(sq,id,themeId,"endingShadowColor",endingShadowColor.rgb());
    saveIndividualSettings(sq,id,themeId,"isNotSameEndingFont",isNotSameEndingFont);
    saveIndividualSettings(sq,id,themeId,"isNotSameEndingColor",isNotSameEndingColor);
    saveIndividualSettings(sq,id,themeId,"endingType",endingType);
    saveIndividualSettings(sq,id,themeId,"endingPosition",endingPosition);
}

void SongSettings::update()
{
    QSqlQuery sq;
    update(sq);
}

void SongSettings::update(QSqlQuery &sq)
{
    if(id == "-1" || themeId == -1)
        return;

    updateMain(sq);

    sq.prepare("UPDATE ThemeData SET svalue = ? WHERE sid = ? AND theme_id = ? AND sname = ?");

    if(isChangedShowTitle)
        updateIndividualSettings(sq,id,themeId,"showStanzaTitle",showStanzaTitle);

    if(isChangedShowKey)
        updateIndividualSettings(sq,id,themeId,"showSongKey",showSongKey);

    if(isChangedShowNum)
        updateIndividualSettings(sq,id,themeId,"showSongNumber",showSongNumber);

    if(isChangedShowEnding)
        updateIndividualSettings(sq,id,themeId,"showSongEnding",showSongEnding);

    if(isChangedInfoFont)
        updateIndividualSettings(sq,id,themeId,"infoFont",infoFont.toString());

    if(isChangedInfoColor)
        updateIndividualSettings(sq,id,themeId,"infoColor",infoColor.rgb());

    if(isChangedInfoShadColor)
        updateIndividualSettings(sq,id,themeId,"infoShadowColor",infoShadowColor.rgb());

    if(isChangedNotSameInfoFont)
        updateIndividualSettings(sq,id,themeId,"isNotSameInfoFont",isNotSameInfoFont);

    if(isChangedNotSameInfoColor)
        updateIndividualSettings(sq,id,themeId,"isNotSameInfoColor",isNotSameInfoColor);

    if(isChangedInfoAlign)
        updateIndividualSettings(sq,id,themeId,"infoAling",infoAling);

    if(isChangedEndingFont)
        updateIndividualSettings(sq,id,themeId,"endingFont",endingFont.toString());

    if(isChangedEndingColor)
        updateIndividualSettings(sq,id,themeId,"endingColor",endingColor.rgb());

    if(isChangedEndingShadColor)
        updateIndividualSettings(sq,id,themeId,"endingShadowColor",endingShadowColor.rgb());

    if(isChangedNotSameEndingFont)
        updateIndividualSettings(sq,id,themeId,"isNotSameEndingFont",isNotSameEndingFont);

    if(isChangedNotSameEndingColor)
        updateIndividualSettings(sq,id,themeId,"isNotSameEndingColor",isNotSameEndingColor);

    if(isChangedEndingType)
        updateIndividualSettings(sq,id,themeId,"endingType",endingType);

    if(isChangedEndingPosition)
        updateIndividualSettings(sq,id,themeId,"endingPosition",endingPosition);
}

/*  **************************************************************************************************
    **************************************************************************************************
    **************************************************************************************************
    **************************************************************************************************
    **************************************************************************************************
    **************************************************************************************************
    **************************************************************************************************
    **************************************************************************************************
    **************************************************************************************************
*/

GeneralSettings::GeneralSettings()
{
    // Apply General Defauls
    displayIsOnTop = false;
    displayScreen = 0;
    displayScreen2 = -1; // interger "-1" mean "None" or not to display
    currentThemeId = 0;
    displayOnStartUp = false;
    settingsChangedAll = false;
    settingsChangedMulti = false;
    settingsChangedSingle = false;
}

DisplayControlsSettings::DisplayControlsSettings()
{
    buttonSize = 3; // 0=16,1=24,2=32,3=48,4=64,5=96
    alignmentV = 2;
    alignmentH = 1;
    opacity = 0.3;
}

SpSettings::SpSettings()
{
    // Apply main window defaults
    isWindowMaximized = false;
    uiTranslation = "en";
}

BibleVersionSettings::BibleVersionSettings()
{
    primaryBible = "none";
    secondaryBible = "none";
    trinaryBible = "none";
    operatorBible = "same";
    settingsChanged = false;
}

SlideShowSettings::SlideShowSettings()
{
    expandSmall = true;
    fitType = 0;
    resize = true;
    boundType = 2;
    boundWidth = 1280;
    settingsChanged = false;
    transitionType = 0;
}

Settings::Settings()
{
    isSpClosing = false;
}

void Settings::loadSettings()
{
    QString t,n,v,s,sets; // type, name, value, userValues
    QStringList set,values;
    bool dataGenOk,dataSpOk,dataB1Ok,dataB2Ok, dataPixOk;
    dataGenOk = dataSpOk = dataB1Ok = dataB2Ok = dataPixOk = false;
    QSqlQuery sq;
    sq.exec(QString("SELECT type, sets FROM Settings "));
    while (sq.next())
    {
        t = sq.value(0).toString();
        sets = sq.value(1).toString();

        if(t == "general") // set general setting values
        {
            dataGenOk = true;
            values = sets.split("\n");
            for(int i(0);i<values.count();++i)
            {
                s = values.at(i);
                set = s.split("=");
                n = set.at(0).trimmed();
                v = set.at(1).trimmed();
                if(n == "displayIsOnTop")
                    general.displayIsOnTop = (v=="true");
                else if(n == "displayOnStartUp")
                    general.displayOnStartUp = (v=="true");
                else if(n == "currentThemeId")
                    general.currentThemeId = v.toInt();
                else if (n == "displayScreen")
                    general.displayScreen = v.toInt();
                // Display Screen 2 settings
                else if (n == "displayScreen2")
                    general.displayScreen2 = v.toInt();
                // End display screen 2 settings
                else if (n == "dcIconSize")
                    general.displayControls.buttonSize = v.toInt();
                else if (n == "dcAlignment")
                {
                    QStringList alinglist = v.split(",");
                    general.displayControls.alignmentV = alinglist.at(0).toInt();
                    general.displayControls.alignmentH = alinglist.at(1).toInt();
                }
                else if (n == "dcOpacity")
                    general.displayControls.opacity = v.toDouble();
            }
        }
        else if(t == "spMain")
        {
            dataSpOk = true;
            values = sets.split("\n");
            for(int i(0);i<values.count();++i)
            {
                s = values.at(i);
                set = s.split("=");
                n = set.at(0).trimmed();
                v = set.at(1).trimmed();

                if(n=="spSplitter")
                    spMain.spSplitter = textToByt(v);
                else if(n=="bibleHiddenSplitter")
                    spMain.bibleHiddenSplitter = textToByt(v);
                else if(n=="bibleShowSplitter")
                    spMain.bibleShowSplitter = textToByt(v);
                else if(n=="songSplitter")
                    spMain.songSplitter = textToByt(v);
                else if(n=="uiTranslation")
                    spMain.uiTranslation = v;
                else if(n=="isWindowMaximized")
                    spMain.isWindowMaximized = (v=="true");
            }
        }
        else if(t == "bible1")
        {
            dataB1Ok = true;
            values = sets.split("\n");
            for(int i(0);i<values.count();++i)
            {
                s = values.at(i);
                set = s.split("=");
                n = set.at(0).trimmed();
                v = set.at(1).trimmed();
                if(n == "primary")
                    bibleSets.primaryBible = v;
                else if(n == "secondary")
                    bibleSets.secondaryBible = v;
                else if (n == "trinary")
                    bibleSets.trinaryBible = v;
                else if (n == "operator")
                    bibleSets.operatorBible = v;
            }
        }
        else if(t == "bible2")
        {
            dataB2Ok = true;
            values = sets.split("\n");
            for(int i(0);i<values.count();++i)
            {
                s = values.at(i);
                set = s.split("=");
                n = set.at(0).trimmed();
                v = set.at(1).trimmed();
                if(n == "primary")
                    bibleSets2.primaryBible = v;
                else if(n == "secondary")
                    bibleSets2.secondaryBible = v;
                else if (n == "trinary")
                    bibleSets2.trinaryBible = v;
            }
        }
        else if(t == "pix")
        {
            dataPixOk = true;
            values = sets.split("\n");
            for(int i(0);i<values.count();++i)
            {
                s = values.at(i);
                set = s.split("=");
                n = set.at(0).trimmed();
                v = set.at(1).trimmed();
                if(n == "expandSmall")
                    slideSets.expandSmall = (v == "true");
                else if(n == "fitType")
                    slideSets.fitType = v.toInt();
                else if (n == "resize")
                    slideSets.resize = (v == "true");
                else if (n == "boundType")
                    slideSets.boundType = v.toInt();
                else if (n == "boundWidth")
                    slideSets.boundWidth = v.toInt();
            }
        }
    }

    // if no data exist, then create
    if(!dataGenOk || !dataSpOk || !dataB1Ok || !dataB2Ok || !dataPixOk)
        saveNewSettings();
}

void Settings::saveSettings()
{
    QSqlQuery sq;
    QString gset,spset,b1set,b2set,pset;//general,bible,song,annouce,spmain

    // **** Prepare general settings ***************************************
    if(general.displayIsOnTop)
        gset = "displayIsOnTop = true";
    else
        gset = "displayIsOnTop = false";
    if(general.displayOnStartUp)
        gset += "\ndisplayOnStartUp = true";
    else
        gset += "\ndisplayOnStartUp = false";
    gset += "\ncurrentThemeId = " + QString::number(general.currentThemeId);
    gset += "\ndisplayScreen = " + QString::number(general.displayScreen);
    gset += "\ndisplayScreen2 = " + QString::number(general.displayScreen2);
    gset += "\ndcIconSize = " + QString::number(general.displayControls.buttonSize);
    gset += QString("\ndcAlignment = %1,%2").arg(general.displayControls.alignmentV).arg(general.displayControls.alignmentH);
    gset += "\ndcOpacity = " + QString::number(general.displayControls.opacity);

    // **** prepare softProjector main settings
    spset += "spSplitter = " + spMain.spSplitter.toHex();
    spset += "\nbibleHiddenSplitter = " + spMain.bibleHiddenSplitter.toHex();
    spset += "\nbibleShowSplitter = " + spMain.bibleShowSplitter.toHex();
    spset += "\nsongSplitter = " + spMain.songSplitter.toHex();
    spset += "\nuiTranslation = " + spMain.uiTranslation;
    if(spMain.isWindowMaximized)
        spset += "\nisWindowMaximized = true";
    else
        spset += "\nisWindowMaximized = false";

    // **** prepare screen 1 bible versions
    b1set = "primary = " + bibleSets.primaryBible;
    b1set += "\nsecondary = " + bibleSets.secondaryBible;
    b1set += "\ntrinary = " + bibleSets.trinaryBible;
    b1set += "\noperator = " + bibleSets.operatorBible;

    // **** prepare screen 1 bible versions
    b2set = "primary = " + bibleSets2.primaryBible;
    b2set += "\nsecondary = " + bibleSets2.secondaryBible;
    b2set += "\ntrinary = " + bibleSets2.trinaryBible;

    // **** prepare pix settings
    if(slideSets.expandSmall)
        pset = "expandSmall = true";
    else
        pset = "expandSmall = false";
    pset += "\nfitType = " + QString::number(slideSets.fitType);
    if(slideSets.resize)
        pset += "\nresize = true";
    else
        pset += "\nresize = false";
    pset += "\nboundType = " + QString::number(slideSets.boundType);
    pset += "\nboundWidth = " + QString::number(slideSets.boundWidth);

    sq.exec(QString("UPDATE Settings SET sets = '%1' WHERE type = 'general'").arg(gset));
    sq.exec(QString("UPDATE Settings SET sets = '%1' WHERE type = 'spMain'").arg(spset));
    sq.exec(QString("UPDATE Settings SET sets = '%1' WHERE type = 'bible1'").arg(b1set));
    sq.exec(QString("UPDATE Settings SET sets = '%1' WHERE type = 'bible2'").arg(b2set));
    sq.exec(QString("UPDATE Settings SET sets = '%1' WHERE type = 'pix'").arg(pset));
}

void Settings::saveNewSettings()
{
    QSqlQuery sq;
    sq.exec("INSERT OR REPLACE INTO Settings (type, sets) VALUES ('general', 'n=v')");
    sq.exec("INSERT OR REPLACE INTO Settings (type, sets) VALUES ('spMain', 'n=v')");
    sq.exec("INSERT OR REPLACE INTO Settings (type, sets) VALUES ('bible1', 'n=v')");
    sq.exec("INSERT OR REPLACE INTO Settings (type, sets) VALUES ('bible2', 'n=v')");
    sq.exec("INSERT OR REPLACE INTO Settings (type, sets) VALUES ('pix', 'n=v')");

    saveSettings();
}

QByteArray Settings::textToByt(QString text)
{
    QByteArray b;
    b.insert(0,text);
    b = b.fromHex(b);
    return b;
}
