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

#ifndef SONG_HPP
#define SONG_HPP
#include <QtGui>
#include <QtSql>
#include "settings.hpp"

QString clean(QString str);
bool isStanzaTitle(QString string);
bool isStanzaVerseTitle(QString string);
bool isStanzaAndVerseTitle(QString string);
bool isStanzaRefrainTitle(QString string);
bool isStanzaAndRefrainTitle(QString string);
bool isStanzaSlideTitle(QString string);

class Stanza
{
    // Class to hold current verse text and song info to send to projection
public:
    int number;
    QString stanza;
    QString stanzaTitle;
    QString wordsBy;
    QString musicBy;
    QString tune;
    bool isLast;
    bool usePrivateSettings;
    int alignmentV;
    int alignmentH;
    QColor color;
    QFont font;
    QColor infoColor;
    QFont infoFont;
    QColor endingColor;
    QFont endingFont;
    bool useBackground;
    QString backgroundName;
    QPixmap background;
};

class Song
{
    // Class for storing song information: number, name, songbook
    // The instance of this class is specific to a song & songbook.
public:
    //functions
    Song();
    Song(int id);
    Song(int id, int num, QString songbook_id, QString songbook_name);
    void readData();
    void saveUpdate();
    void saveNew();
    QStringList getSongTextList();
    Stanza getStanza(int current);
    QString getSongbookName();
    bool isValid();
    void getSettings(SongSettings &settings);

    //members
    int songID; // Database ID of this song
    QString songbook_id;
    QString songbook_name;
    int number; // Number of the song in the specified songbook
    QString title;
    int category;
    QString tune;
    QString wordsBy;
    QString musicBy;
    QString songText;
    QString notes;
    bool usePrivateSettings;
    int alignmentV;
    int alignmentH;
    QColor color;
    QFont font;
    QColor infoColor;
    QFont infoFont;
    QColor endingColor;
    QFont endingFont;
    bool useBackground;
    QString backgroundName;
    QPixmap background;

private:
    void setDefaults();
    QString getStanzaBlock(int &i, QStringList &list);
    void removeLastChorus(QStringList ct, QStringList &list);
};

class SongsModel : public QAbstractTableModel
{
    // Class for storing the data for the song table
    Q_OBJECT
    Q_DISABLE_COPY(SongsModel)
public:
    SongsModel();
    void setSongs(QList<Song> songs);
    void addSong(Song song);
    Song getSong(int row);
    Song getSong(QModelIndex index);

    int rowCount(const QModelIndex &parent = QModelIndex()) const;
    int columnCount(const QModelIndex &parent = QModelIndex()) const;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const;

    bool removeRows( int row, int count, const QModelIndex & parent = QModelIndex() );
    QList<Song> song_list;
    void emitLayoutChanged();
    void emitLayoutAboutToBeChanged();
    void updateSongFromDatabase(int songid);
    void updateSongFromDatabase(int newSongId, int oldSongId);
    bool isInTable(int songid);
};

class SongProxyModel : public QSortFilterProxyModel
{
    Q_OBJECT

public:
    SongProxyModel(QObject *parent = 0);
    void setFilterString(QString new_string, bool new_match_beginning, bool new_exact_match);
    void setSongbookFilter(QString new_songbook);
    void setCategoryFilter(int category);
    bool filterAcceptsRow(int sourceRow, const QModelIndex &sourceParent) const;

private:
    QString filter_string, songbook_filter, category_filter;
    bool match_beginning, exact_match;
};

class SongDatabase
{
public:
    SongDatabase();
    void addSongbook(QString name, QString info);
    void saveUpdate();
    void saveNew();
    void deleteSong(int songId);
    QString getSongbookIdStringFromName(QString songbook_name);
    Song getSong(int id);
    QList<Song> getSongs();
    int lastUser(QString songbook_id);
};

#endif // SONG_HPP
