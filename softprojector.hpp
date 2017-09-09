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

#ifndef SOFTPROJECTOR_HPP
#define SOFTPROJECTOR_HPP

#include <QMainWindow>
#include "songwidget.hpp"
#include "biblewidget.hpp"
#include "announcewidget.hpp"
#include "projectordisplayscreen.hpp"
#include "editwidget.hpp"
#include "bible.hpp"
#include "managedatadialog.hpp"
#include "mediacontrol.hpp"
#include "settingsdialog.hpp"
#include "songcounter.hpp"
#include "settings.hpp"
#include "printpreviewdialog.hpp"
#include "helpdialog.hpp"
#include "picturewidget.hpp"
#include "slideshow.hpp"
#include "mediawidget.hpp"
#include "videoinfo.hpp"
#include "slideshoweditor.hpp"
#include "schedule.hpp"

class QActionGroup;

namespace Ui
{
class SoftProjectorClass;
}

enum PresentationType
{
    BIBLE,
    SONG,
    ANNOUCEMENT,
    PICTURE,
    VIDEO
};

class SoftProjector : public QMainWindow
{
    Q_OBJECT

public:
    SoftProjector(QWidget *parent = 0);
    ~SoftProjector();
    SongWidget *songWidget;
    BibleWidget *bibleWidget;
    AnnounceWidget *announceWidget;
    ManageDataDialog *manageDialog;
    QDesktopWidget *desktop;
    EditWidget *editWidget;
    ProjectorDisplayScreen *pds1;
    ProjectorDisplayScreen *pds2;
    PictureWidget *pictureWidget;
    MediaWidget *mediaPlayer;
    MediaControl *mediaControls;

    bool showing; // whether we are currently showing to the projector
    Song current_song;
    int current_song_verse;
    Verse current_verse;
    Announcement currentAnnounce;
    QString version_string;
    Theme theme;
    Settings mySettings;

    SoftProjector *softProjector;

public slots:
    void updateSetting(GeneralSettings &g,Theme &t, SlideShowSettings &ssets,
                       BibleVersionSettings &bsets, BibleVersionSettings &bsets2);
    void saveSettings();
    void positionDisplayWindow();
    void updateScreen();

    void setWaitCursor();
    void setArrowCursor();
    void setAppDataDir(QDir d){appDataDir = d;}

private:
    Ui::SoftProjectorClass *ui;
    SettingsDialog *settingsDialog;
    HelpDialog *helpDialog;
    PresentationType pType;
    bool new_list;
    QActionGroup *languageGroup;
    QString languagePath;
    QTranslator translator;

    //For saving and opening schedule files
    //QString project_file_path;
    QString schedule_file_path;
    bool is_schedule_saved;
    QString cur_locale;
    bool isSingleScreen;
    bool hasDisplayScreen2;

    // shortcuts
    QShortcut *shpgUP;
    QShortcut *shpgDwn;
    QShortcut *shSart1;
    QShortcut *shSart2;

    // Pictures
    QList<SlideShowItem> pictureShowList;

    // video items
//    Phonon::SeekSlider *playerSlider;
//    Phonon::VolumeSlider *volumeSlider;
    VideoInfo currentVideo;
//    Phonon::AudioOutput *playerAudioOutput;

    // Schelude list
    QList<Schedule> schedule;
    QDir appDataDir;

private slots:
    void showDisplayScreen(bool show);

    void applySetting(GeneralSettings &g, Theme &t, SlideShowSettings &s,
                      BibleVersionSettings &b1, BibleVersionSettings &b2);
    void on_actionSong_Counter_triggered();
    void on_projectTab_currentChanged(int index);
    void updateEditActions();
    void on_actionNew_triggered();
    void on_actionEdit_triggered();
    void on_actionCopy_triggered();
    void on_actionDelete_triggered();
    void updateWindowText();
    void showBible();
    void showSong(int currentRow);
    void showAnnounce(int currentRow);
    void showPicture(int currentRow);
    void showVideo();

    void retranslateUis();
    void createLanguageActions();
    void switchLanguage(QAction *action);
    void on_actionDonate_triggered();
    void on_action_Help_triggered();
    void on_actionManage_Database_triggered();
    void on_actionAbout_triggered();
    void on_listShow_doubleClicked(QModelIndex index);
    void on_actionSettings_triggered();
    void newSong();
    void copySong();
    void editSong();
    void deleteSong();
    void newSlideShow();
    void editSlideShow();
    void deleteSlideShow();
    void addMediaToLibrary();
    void removeMediaFromLibrary();
    void newAnnouncement();
    void editAnnouncement();
    void copyAnnouncement();
    void deleteAnnoucement();
    void on_actionShow_triggered();
    void on_actionHide_triggered();
    void on_listShow_currentRowChanged(int currentRow);
    void on_actionClose_triggered();
    void setSongList(Song song, int row);
    void setAnnounceText(Announcement announce, int row);
    void setChapterList(QStringList chapter_list, QString caption, QItemSelection selectedItems);
    void setPictureList(QList<SlideShowItem> &image_list, int row, QString name);
    void setVideo(VideoInfo &video);
    void playVideo();
    void pauseVideo();
    void stopVideo();
    void setVideoPosition(qint64 position);

    void on_listShow_itemSelectionChanged();
    void on_rbMultiVerse_toggled(bool checked);
    void on_actionPrint_triggered();
    void on_actionPrintSchedule_triggered();

    void nextSlide();
    void prevSlide();

    // schedule functions
    void on_actionScheduleAdd_triggered();
    void on_actionScheduleRemove_triggered();
    void on_actionScheduleClear_triggered();
    void addToShcedule(BibleHistory &b);
    void addToShcedule(Song &s);
    void addToShcedule(SlideShow &s);
    void addToShcedule(VideoInfo &v);
    void addToShcedule(Announcement &a);
    void reloadShceduleList();
    void on_listWidgetSchedule_doubleClicked(const QModelIndex &index);
    void on_listWidgetSchedule_itemSelectionChanged();
    void on_actionMoveScheduleTop_triggered();
    void on_actionMoveScheduleUp_triggered();
    void on_actionMoveScheduleDown_triggered();
    void on_actionMoveScheduleBottom_triggered();
    void on_actionNewSchedule_triggered();
    void on_actionOpenSchedule_triggered();
    void on_actionSaveSchedule_triggered();
    void on_actionSaveScheduleAs_triggered();
    void on_actionCloseSchedule_triggered();
    void openSchedule();
    void saveSchedule(bool overWrite);
    void saveScheduleNew(QSqlQuery &q);
    void saveScheduleItemNew(QSqlQuery &q, int scid, const BibleHistory &b);
    void saveScheduleItemNew(QSqlQuery &q, int scid, const Song &s);
    void saveScheduleItemNew(QSqlQuery &q, int scid, const SlideShow &s);
    void saveScheduleItemNew(QSqlQuery &q, int scid, const VideoInfo &v);
    void saveScheduleItemNew(QSqlQuery &q, int scid, const Announcement &a);
    void saveScheduleUpdate(QSqlQuery &q);
    void saveScheduleItemUpdate(QSqlQuery &q, int scid, const BibleHistory &b);
    void saveScheduleItemUpdate(QSqlQuery &q, int scid, const Song &s);
    void saveScheduleItemUpdate(QSqlQuery &q, int scid, const SlideShow &s);
    void saveScheduleItemUpdate(QSqlQuery &q, int scid, const VideoInfo &v);
    void saveScheduleItemUpdate(QSqlQuery &q, int scid, const Announcement &a);
    void openScheduleItem(QSqlQuery &q, const int scid, BibleHistory &b);
    void openScheduleItem(QSqlQuery &q, const int scid, Song &s);
    void openScheduleItem(QSqlQuery &q, const int scid, SlideShow &s);
    void openScheduleItem(QSqlQuery &q, const int scid, VideoInfo &v);
    void openScheduleItem(QSqlQuery &q, const int scid, Announcement &a);

    void on_actionClear_triggered();

    void on_actionCloseDisplay_triggered();
    void updateCloseDisplayButtons(bool isOn);

protected:
    void closeEvent(QCloseEvent *event);
    void keyPressEvent(QKeyEvent *event);
};

#endif // SOFTPROJECTOR_HPP
