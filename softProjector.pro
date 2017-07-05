##**************************************************************************
##
##    softProjector - an open source media projection software
##    Copyright (C) 2017  Vladislav Kobzar
##
##    This program is free software: you can redistribute it and/or modify
##    it under the terms of the GNU General Public License as published by
##    the Free Software Foundation version 3 of the License.
##
##    This program is distributed in the hope that it will be useful,
##    but WITHOUT ANY WARRANTY; without even the implied warranty of
##    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
##    GNU General Public License for more details.
##
##    You should have received a copy of the GNU General Public License
##    along with this program.  If not, see <http:##www.gnu.org/licenses/>.
##
##**************************************************************************


QT += core \
    gui \
    widgets \
    network \
    sql \
    qml \
    quick \
    printsupport \
    multimedia \
    multimediawidgets

TARGET = SoftProjector
TEMPLATE = app
CONFIG += x86 ppc x86_64 ppc64 # Compile a universal build

RES_DIR = $${PWD}/unknownsys_build
win32: RES_DIR = $${PWD}/win32_build
unix:  RES_DIR = $${PWD}/unix_build
macx: RES_DIR = $${PWD}/mac_build

DESTDIR = $${RES_DIR}/bin
OBJECTS_DIR = $${RES_DIR}/obj
MOC_DIR = $${RES_DIR}/moc
UI_DIR = $${RES_DIR}/ui
RCC_DIR = $${RES_DIR}/rcc
OUT_PWD = $${RES_DIR}/bin

SOURCES += main.cpp \
    softprojector.cpp \
    songwidget.cpp \
    biblewidget.cpp \
    editwidget.cpp \
    song.cpp \
    bible.cpp \
    settingsdialog.cpp \
    aboutdialog.cpp \
    addsongbookdialog.cpp \
    highlight.cpp \
    managedatadialog.cpp \
    managedata.cpp \
    announcewidget.cpp \
    helpdialog.cpp \
    songcounter.cpp \
    bibleinformationdialog.cpp \
    settings.cpp \
    generalsettingwidget.cpp \
    biblesettingwidget.cpp \
    songsettingwidget.cpp \
    announcementsettingwidget.cpp \
    printpreviewdialog.cpp \
    controlbutton.cpp \
    passivesettingwidget.cpp \
    theme.cpp \
    picturewidget.cpp \
    slideshow.cpp \
    mediawidget.cpp \
    videoplayerwidget.cpp \
    videoinfo.cpp \
    spfunctions.cpp \
    slideshoweditor.cpp \
    editannouncementdialog.cpp \
    announcement.cpp \
    schedule.cpp \
    picturesettingwidget.cpp \
    moduledownloaddialog.cpp \
    moduleprogressdialog.cpp \
    displaysetting.cpp \
    projectordisplayscreen.cpp \
    imagegenerator.cpp \
    spimageprovider.cpp \
    mediacontrol.cpp
HEADERS += softprojector.hpp \
    songwidget.hpp \
    biblewidget.hpp \
    editwidget.hpp \
    song.hpp \
    bible.hpp \
    settingsdialog.hpp \
    aboutdialog.hpp \
    addsongbookdialog.hpp \
    highlight.hpp \
    managedatadialog.hpp \
    managedata.hpp \
    announcewidget.hpp \
    helpdialog.hpp \
    songcounter.hpp \
    bibleinformationdialog.hpp \
    settings.hpp \
    generalsettingwidget.hpp \
    biblesettingwidget.hpp \
    songsettingwidget.hpp \
    announcementsettingwidget.hpp \
    printpreviewdialog.hpp \
    controlbutton.hpp \
    passivesettingwidget.hpp \
    theme.hpp \
    picturewidget.hpp \
    slideshow.hpp \
    mediawidget.hpp \
    videoplayerwidget.hpp \
    videoinfo.hpp \
    spfunctions.hpp \
    slideshoweditor.hpp \
    editannouncementdialog.hpp \
    announcement.hpp \
    schedule.hpp \
    picturesettingwidget.hpp \
    moduledownloaddialog.hpp \
    moduleprogressdialog.hpp \
    displaysetting.hpp \
    projectordisplayscreen.hpp \
    imagegenerator.hpp \
    spimageprovider.hpp \
    mediacontrol.hpp
FORMS += softprojector.ui \
    songwidget.ui \
    biblewidget.ui \
    editwidget.ui \
    settingsdialog.ui \
    aboutdialog.ui \
    addsongbookdialog.ui \
    managedatadialog.ui \
    announcewidget.ui \
    helpdialog.ui \
    songcounter.ui \
    bibleinformationdialog.ui \
    generalsettingwidget.ui \
    biblesettingwidget.ui \
    songsettingwidget.ui \
    announcementsettingwidget.ui \
    printpreviewdialog.ui \
    passivesettingwidget.ui \
    picturewidget.ui \
    mediawidget.ui \
    slideshoweditor.ui \
    editannouncementdialog.ui \
    picturesettingwidget.ui \
    moduledownloaddialog.ui \
    moduleprogressdialog.ui \
    projectordisplayscreen.ui \
    mediacontrol.ui
TRANSLATIONS += translations/softpro_de.ts\
    translations/softpro_ru.ts\
    translations/softpro_cs.ts\
    translations/softpro_ua.ts\
    translations/softpro_hy.ts
CODECFORTR = UTF-8
RESOURCES += softprojector.qrc

win32 {
    RC_FILE = softprojector.rc
}
