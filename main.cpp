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

#include <QApplication>
#include <QMessageBox>
#include <QtSql>
#include "softprojector.hpp"

// Definitions for database versions 'dbVer' numbers
// x - Official release. ex: 2 - for SoftProjector 2
// xxx - Official sub realeas. ex: 201 - for SoftProjector 2.01
// 990xxx - Development release. ex: 990206 - for SoftProjector 2 Development Build 6 (2db6)
int const dbVer = 2;

bool connect(QString database_file)
{
    database_file += "spData.sqlite";
    bool database_exists = ( QFile::exists(database_file) );

    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName(database_file);
    if (!db.open())
    {
        QMessageBox mb;
        mb.setText("spData Error"
                   "Could not connect to the database spData.sqlite!\n\n"
                   "Following Errors:\n"
                   + db.lastError().databaseText() + "\n"
                   + db.lastError().driverText() +"\n"
                   + db.databaseName()
                   + "\n\nThis is a Fatal Error. Please make sure that all QtSql libraries are inlcuded."
                   "\nThe program will terminate");
        mb.setWindowTitle("Database Connection Error");
        mb.setIcon(QMessageBox::Critical);
        mb.exec();
        return false;
    }
    else
    {
        // If no files exited, then database has been created now we need to fill it
        if(!database_exists)
        {
            QSqlQuery sq;
            sq.exec(QString("PRAGMA user_version = %1").arg(dbVer));
            sq.exec("CREATE TABLE 'Announcements' ('id' INTEGER PRIMARY KEY  AUTOINCREMENT  NOT NULL  UNIQUE , "
                    "'title' TEXT, 'text' TEXT, 'usePrivate' BOOL, 'useAuto' BOOL, 'loop' BOOL, 'slideTime' INTEGER, "
                    "'useBackground' BOOL, 'backgoundPath' TEXT, 'font' TEXT, 'color' TEXT, 'alignment' TEXT)");
            sq.exec("CREATE TABLE 'BibleBooks' ('bible_id' INTEGER, 'id' INTEGER, 'book_name' "
                    "TEXT, 'chapter_count' INTEGER DEFAULT 0)");
            sq.exec("CREATE TABLE 'BibleVerse' ('verse_id' TEXT, 'bible_id' TEXT, 'book' TEXT, "
                    "'chapter' INTEGER, 'verse' INTEGER, 'verse_text' TEXT)");
            sq.exec("CREATE TABLE 'BibleVersions' ('id' INTEGER PRIMARY KEY  AUTOINCREMENT  NOT NULL, "
                    "'bible_name' TEXT, 'abbreviation' TEXT, 'information' TEXT, 'right_to_left' INTEGER DEFAULT 0)");
            sq.exec("CREATE TABLE 'Media' ('long_path' TEXT, 'short_path' TEXT)");
            sq.exec("CREATE TABLE 'Settings' ('type' TEXT, 'sets' TEXT)");
            sq.exec("CREATE TABLE 'SlideShows' ('id' INTEGER PRIMARY KEY  AUTOINCREMENT  NOT NULL , 'name' TEXT, 'info' TEXT)");
            sq.exec("CREATE TABLE 'Slides' ('id' INTEGER PRIMARY KEY  AUTOINCREMENT  NOT NULL , "
                    "'ss_id' INTEGER, 'p_order' INTEGER, 'name' TEXT, 'path' TEXT, "
                    "'pix' BLOB, 'pix_small' BLOB, 'pix_prev' BLOB)");
            sq.exec("CREATE TABLE 'Songbooks' ('id' INTEGER PRIMARY KEY  AUTOINCREMENT  NOT NULL , 'name' TEXT, 'info' TEXT)");
            sq.exec("CREATE TABLE 'Songs' ('id' INTEGER PRIMARY KEY  AUTOINCREMENT  NOT NULL , "
                    "'songbook_id' INTEGER, 'number' INTEGER, 'title' TEXT, 'category' INTEGER DEFAULT 0, "
                    "'tune' TEXT, 'words' TEXT, 'music' TEXT, 'song_text' TEXT, 'notes' TEXT, "
                    "'use_private' BOOL, 'alignment_v' INTEGER, 'alignment_h' INTEGER, 'color' INTEGER, 'font' TEXT, "
                    "'info_color' INTEGER, 'info_font' TEXT, 'ending_color' INTEGER, 'ending_font' TEXT, "
                    "'use_background' BOOL, 'background_name' TEXT, 'background' BLOB, 'count' INTEGER DEFAULT 0, 'date' TEXT)");
            sq.exec("CREATE TABLE 'ThemeAnnounce' ('theme_id' INTEGER, 'disp' INTEGER, 'use_shadow' BOOL, 'use_fading' BOOL, "
                    "'use_blur_shadow' BOOL, 'use_background' BOOL, 'background_name' TEXT, 'background' BLOB, 'text_font' TEXT, "
                    "'text_color' INTEGER, 'text_align_v' INTEGER, 'text_align_h' INTEGER, 'use_disp_2' BOOL)");
            sq.exec("CREATE TABLE 'ThemeBible' ('theme_id' INTEGER, 'disp' INTEGER, 'use_shadow' BOOL, 'use_fading' BOOL, "
                    "'use_blur_shadow' BOOL, 'use_background' BOOL, 'background_name' TEXT, 'background' BLOB, 'text_font' TEXT, "
                    "'text_color' INTEGER, 'text_align_v' INTEGER, 'text_align_h' INTEGER, 'caption_font' TEXT, "
                    "'caption_color' INTEGER, 'caption_align' INTEGER, 'caption_position' INTEGER, 'use_abbr' BOOL, "
                    "'screen_use' INTEGER, 'screen_position' INTEGER, 'use_disp_2' BOOL)");
            sq.exec("CREATE TABLE 'ThemePassive' ('theme_id' INTEGER, 'disp' INTEGER, 'use_background' BOOL, "
                    "'background_name' TEXT, 'background' BLOB, 'use_disp_2' BOOL)");
            sq.exec("CREATE TABLE 'ThemeSong' ('theme_id' INTEGER, 'disp' INTEGER, 'use_shadow' BOOL, 'use_fading' BOOL, "
                    "'use_blur_shadow' BOOL, 'show_stanza_title' BOOL, 'show_key' BOOL, 'show_number' BOOL, "
                    "'info_color' INTEGER, 'info_font' TEXT, 'info_align' INTEGER, 'show_song_ending' BOOL, "
                    "'ending_color' INTEGER, 'ending_font' TEXT, 'ending_type' INTEGER, 'ending_position' INTEGER, "
                    "'use_background' BOOL, 'background_name' TEXT, 'background' BLOB, 'text_font' TEXT, "
                    "'text_color' INTEGER, 'text_align_v' INTEGER, 'text_align_h' INTEGER, "
                    "'screen_use' INTEGER, 'screen_position' INTEGER, 'use_disp_2' BOOL)");
            //sq.exec("CREATE TABLE 'ThemeData' ('theme_id' INTEGER, 'type' TEXT, 'sets' TEXT)");
            sq.exec("CREATE TABLE 'Themes' ('id' INTEGER PRIMARY KEY  AUTOINCREMENT  NOT NULL , 'name' TEXT, 'comment' TEXT)");
        }
        return true;
    }
}

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    a.setApplicationName("SoftProjector");

    QPixmap pixmap(":icons/icons/splash.png");
    QSplashScreen splash(pixmap);
    splash.setMask(pixmap.mask());
    splash.show();
    a.processEvents();

//    QTextCodec::setCodecForTr(QTextCodec::codecForName("utf8"));

    // Look for the database in all the same places that the QSql module will look,
    // and display a friendly error if it was not found:
    QString database_dir;
#ifdef Q_WS_WIN
    // If running on Windows, check if SoftProjector is Installed.
    // If it is installed, then provide proper directory for database.
    QDir d;
    QString cur_app_path = a.applicationDirPath();
    //    if(cur_app_path.contains(QString("C:%1Program Files").arg(d.separator())))
    if(cur_app_path.contains("C:/Program Files") || cur_app_path.contains("C:\\Program Files"))
    {
        // Check if it is on Windows Vista and Later or before Vista
        bool is_vista = (QSysInfo::WindowsVersion >= QSysInfo::WV_VISTA);
        if(is_vista)
        {
            d.cd(d.rootPath());
            if(d.cd("ProgramData"))
            {
                // Check if 'SoftProjector directory exists, if not, create one
                if(d.cd("SoftProjector"))
                    database_dir = d.absolutePath() + d.separator();
                else
                {
                    d.mkdir("SoftProjector");
                    if(d.cd("SoftProjector"))
                        database_dir = d.absolutePath() + d.separator();
                    else
                        database_dir = cur_app_path + d.separator();
                }
            }
            else if(d.cd("Public"))
            {
                // Check if 'SoftProjector directory exists, if not, create one
                if(d.cd("SoftProjector"))
                    database_dir = d.absolutePath() + d.separator();
                else
                {
                    d.mkdir("SoftProjector");
                    if(d.cd("SoftProjector"))
                        database_dir = d.absolutePath() + d.separator();
                    else
                        database_dir = cur_app_path + d.separator();
                }
            }
            else
                database_dir = cur_app_path + d.separator();
        }
        else
        {
            d.cd(d.homePath());
            d.cdUp();
            if(d.cd("All Users"))
            {
                if(d.cd("Application Data"))
                {
                    // Check if 'SoftProjector directory exists, if not, create one
                    if(d.cd("SoftProjector"))
                        database_dir = d.absolutePath() + d.separator();
                    else
                    {
                        d.mkdir("SoftProjector");
                        if(d.cd("SoftProjector"))
                            database_dir = d.absolutePath() + d.separator();
                        else
                            database_dir = cur_app_path + d.separator();
                    }
                }
                else
                    database_dir = cur_app_path + d.separator();
            }
            else
                database_dir = cur_app_path + d.separator();
        }
    }
    else
        database_dir = cur_app_path + QDir::separator();
#else
    database_dir = a.applicationDirPath() + QDir::separator();
#endif

    // Try to connect to database
    if( !connect(database_dir) )
    {
        QMessageBox mb;
        mb.setText("Failed to connect to database 'spData.sqlite'");
        mb.setWindowTitle("Database File Error");
        mb.setIcon(QMessageBox::Critical);
        mb.exec();
        return 1;
    }
    // Connected to the database OK:

    // Make sure that database is of correct version
    QSqlQuery sq;
    sq.exec("PRAGMA user_version");
    sq.first();
    int dbVersion = sq.value(0).toInt();
    if(dbVer != dbVersion)
    {
        QString errortxt = QString("SoftProjector requires database vesion # %1\n"
                                   "The database you are trying to open has vesion # %2\n"
                                   "Please use database with current version\n"
                                   "OR rename/remove all the database files\n"
                                   "and let softProjector to create needed database file.\n"
                                   "The program will terminate!"
                                   ).arg(dbVer).arg(dbVersion);
        QMessageBox mb;
        mb.setText(errortxt);
        mb.setWindowTitle("Incorrect Database Version");
        mb.setIcon(QMessageBox::Critical);
        mb.exec();
        return 1;
    }
    // Database is of correct version

    SoftProjector w;
    w.setAppDataDir(QDir(database_dir));
    w.show();
    splash.finish(&w);
    return a.exec();
}
