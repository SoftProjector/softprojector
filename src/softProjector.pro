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

SOURCES += sources/main.cpp \
    sources/softprojector.cpp \
    sources/songwidget.cpp \
    sources/biblewidget.cpp \
    sources/editwidget.cpp \
    sources/song.cpp \
    sources/bible.cpp \
    sources/settingsdialog.cpp \
    sources/aboutdialog.cpp \
    sources/addsongbookdialog.cpp \
    sources/highlight.cpp \
    sources/managedatadialog.cpp \
    sources/managedata.cpp \
    sources/announcewidget.cpp \
    sources/helpdialog.cpp \
    sources/songcounter.cpp \
    sources/bibleinformationdialog.cpp \
    sources/settings.cpp \
    sources/generalsettingwidget.cpp \
    sources/biblesettingwidget.cpp \
    sources/songsettingwidget.cpp \
    sources/announcementsettingwidget.cpp \
    sources/printpreviewdialog.cpp \
    sources/controlbutton.cpp \
    sources/passivesettingwidget.cpp \
    sources/theme.cpp \
    sources/picturewidget.cpp \
    sources/slideshow.cpp \
    sources/mediawidget.cpp \
    sources/videoplayerwidget.cpp \
    sources/videoinfo.cpp \
    sources/spfunctions.cpp \
    sources/slideshoweditor.cpp \
    sources/editannouncementdialog.cpp \
    sources/announcement.cpp \
    sources/schedule.cpp \
    sources/picturesettingwidget.cpp \
    sources/moduledownloaddialog.cpp \
    sources/moduleprogressdialog.cpp \
    sources/displaysetting.cpp \
    sources/projectordisplayscreen.cpp \
    sources/imagegenerator.cpp \
    sources/spimageprovider.cpp \
    sources/mediacontrol.cpp
HEADERS += headers/softprojector.hpp \
    headers/songwidget.hpp \
    headers/biblewidget.hpp \
    headers/editwidget.hpp \
    headers/song.hpp \
    headers/bible.hpp \
    headers/settingsdialog.hpp \
    headers/aboutdialog.hpp \
    headers/addsongbookdialog.hpp \
    headers/highlight.hpp \
    headers/managedatadialog.hpp \
    headers/managedata.hpp \
    headers/announcewidget.hpp \
    headers/helpdialog.hpp \
    headers/songcounter.hpp \
    headers/bibleinformationdialog.hpp \
    headers/settings.hpp \
    headers/generalsettingwidget.hpp \
    headers/biblesettingwidget.hpp \
    headers/songsettingwidget.hpp \
    headers/announcementsettingwidget.hpp \
    headers/printpreviewdialog.hpp \
    headers/controlbutton.hpp \
    headers/passivesettingwidget.hpp \
    headers/theme.hpp \
    headers/picturewidget.hpp \
    headers/slideshow.hpp \
    headers/mediawidget.hpp \
    headers/videoplayerwidget.hpp \
    headers/videoinfo.hpp \
    headers/spfunctions.hpp \
    headers/slideshoweditor.hpp \
    headers/editannouncementdialog.hpp \
    headers/announcement.hpp \
    headers/schedule.hpp \
    headers/picturesettingwidget.hpp \
    headers/moduledownloaddialog.hpp \
    headers/moduleprogressdialog.hpp \
    headers/displaysetting.hpp \
    headers/projectordisplayscreen.hpp \
    headers/imagegenerator.hpp \
    headers/spimageprovider.hpp \
    headers/mediacontrol.hpp
FORMS += ui/softprojector.ui \
    ui/songwidget.ui \
    ui/biblewidget.ui \
    ui/editwidget.ui \
    ui/settingsdialog.ui \
    ui/aboutdialog.ui \
    ui/addsongbookdialog.ui \
    ui/managedatadialog.ui \
    ui/announcewidget.ui \
    ui/helpdialog.ui \
    ui/songcounter.ui \
    ui/bibleinformationdialog.ui \
    ui/generalsettingwidget.ui \
    ui/biblesettingwidget.ui \
    ui/songsettingwidget.ui \
    ui/announcementsettingwidget.ui \
    ui/printpreviewdialog.ui \
    ui/passivesettingwidget.ui \
    ui/picturewidget.ui \
    ui/mediawidget.ui \
    ui/slideshoweditor.ui \
    ui/editannouncementdialog.ui \
    ui/picturesettingwidget.ui \
    ui/moduledownloaddialog.ui \
    ui/moduleprogressdialog.ui \
    ui/projectordisplayscreen.ui \
    ui/mediacontrol.ui
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
