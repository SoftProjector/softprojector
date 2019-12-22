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

#include "song.hpp"
#include <QDebug>
#include "spfunctions.hpp"

// for future use or chord import
// to filter out ChorPro chords from within the song text
// Use following QRegExp = "(\\[[\\w]*[\\w]\\]|\\[[\\w]*[#b♭♯][\\w]*\\])"

QString clean(QString str)
{
    //Removes all none alphanumeric characters from the string
    str.replace(QRegExp("[\\W*]")," ");
    str = str.simplified();
    return str;
}

bool isStanzaTitle(QString string)
{
    // Checks if the line is stanza title line
    if(isStanzaVerseTitle(string))
        return true;
    else if (isStanzaAndVerseTitle(string))
        return true;
    else if (isStanzaRefrainTitle(string))
        return true;
    else if (isStanzaAndRefrainTitle(string))
        return true;
    else if (isStanzaSlideTitle(string))
        return true;
    else
        return false;
}

bool isStanzaVerseTitle(QString string)
{
    // Check if the line is verse title line
    if(string.startsWith("Verse"))
        return true;
    else if (string.startsWith(QString::fromUtf8("Куплет")))
        return true;
    else if (string.startsWith(QString::fromUtf8("Strophe")))
        return true;
    else if (string.startsWith(QString::fromUtf8("Verš")))
        return true;
    else
        return false;
}

bool isStanzaAndVerseTitle(QString string)
{
    // Check if the line is additional verse title line
    if(string.startsWith("&Verse"))
        return true;
    else if (string.startsWith(QString::fromUtf8("&Куплет")))
        return true;
    else if (string.startsWith(QString::fromUtf8("&Strophe")))
        return true;
    else if (string.startsWith(QString::fromUtf8("&Verš")))
        return true;
    else
        return false;
}

bool isStanzaRefrainTitle(QString string)
{
    // Check if line is refrain title line
    if(string.startsWith("Chorus"))
        return true;
    else if (string.startsWith(QString::fromUtf8("Sbor")))
        return true;
    else if (string.startsWith("Refrain"))
        return true;
    else if (string.startsWith(QString::fromUtf8("Припев")))
        return true;
    else if (string.startsWith(QString::fromUtf8("Приспів")))
        return true;
    else if (string.startsWith(QString::fromUtf8("Refrén")))
        return true;
    else
        return false;
}

bool isStanzaAndRefrainTitle(QString string)
{
    // Check if line is additional refrain title line
    if(string.startsWith("&Chorus"))
        return true;
    else if (string.startsWith(QString::fromUtf8("&Sbor")))
        return true;
    else if (string.startsWith("&Refrain"))
        return true;
    else if (string.startsWith(QString::fromUtf8("&Припев")))
        return true;
    else if (string.startsWith(QString::fromUtf8("&Приспів")))
        return true;
    else if (string.startsWith(QString::fromUtf8("&Refrén")))
        return true;
    else
        return false;
}

bool isStanzaSlideTitle(QString string)
{
    // Check if line is slide or other stanza title line
    if(string.startsWith("Slide"))
        return true;
    else if (string.startsWith(QString::fromUtf8("Слайд")))
        return true;
    else if (string.startsWith(QString::fromUtf8("Dia")))
        return true;
    else if (string.startsWith(QString::fromUtf8("Snímek")))
        return true;
    else if (string.startsWith("Insert"))
        return true;
    else if (string.startsWith(QString::fromUtf8("Вставка")))
        return true;
    else if (string.startsWith(QString::fromUtf8("Einfügung")))
        return true;
    else if (string.startsWith(QString::fromUtf8("Vložka")))
        return true;
    else if (string.startsWith("Intro"))
        return true;
    else if (string.startsWith(QString::fromUtf8("Вступление")))
        return true;
    //else if (string.startsWith(QString::fromUtf8("Вступ")))
    //    return true;
    else if (string.startsWith(QString::fromUtf8("Einleitung")))
        return true;
    else if (string.startsWith(QString::fromUtf8("Úvod")))
        return true;
    else if (string.startsWith("Ending"))
        return true;
    else if (string.startsWith(QString::fromUtf8("Окончание")))
        return true;
    else if (string.startsWith(QString::fromUtf8("Закінчення")))
        return true;
    else if (string.startsWith(QString::fromUtf8("Ende")))
        return true;
    else if (string.startsWith(QString::fromUtf8("Závěr")))
        return true;
    else
        return false;
}

Song::Song()
{
    // initialize songId to be zero at start;
    setDefaults();
    songID = 0;
}

Song::Song(int id)
{
    setDefaults();
    songID = id;
}

Song::Song(int song_id, int song_num, QString song_songbook_id, QString song_songbook_name)
{
    setDefaults();
    songID = song_id;
    number = song_num;
    songbook_id = song_songbook_id;
    songbook_name = song_songbook_name;
}

void Song::setDefaults()
{
    number = 1;
    title = "";
    tune = "";
    wordsBy = "";
    musicBy = "";
    songText = "";
    category = 0;
    usePrivateSettings = false;
    alignmentV = 1;
    alignmentH = 1;
    color = QColor(Qt::white);
    font.fromString("Arial,28,-1,5,50,0,0,0,0,0");
    infoColor = QColor(Qt::white);
    infoFont.fromString("Arial,28,-1,5,50,0,0,0,0,0");
    endingColor = QColor(Qt::white);
    endingFont.fromString("Arial,28,-1,5,50,0,0,0,0,0");
    useBackground = false;
    backgroundName = "";
    background = QPixmap();
    notes = "";
}

void Song::readData()
{
    QSqlQuery sq;
    //              0               1       2     3        4    5      6       7         8
    //        9               10        11          12     13    14            15          16         17
    //        18                19              20
    sq.exec("SELECT songbook_id, number, title, category, tune, words, music, song_text, notes, "
            "use_private, alignment_v, alignment_h, color, font, info_color, info_font, ending_color, ending_font, "
            "use_background, background_name, background FROM Songs WHERE id = " + QString::number(songID));
    sq.first();
    songbook_id = sq.value(0).toString();
    number = sq.value(1).toInt();
    title = sq.value(2).toString();
    category = sq.value(3).toInt();
    tune = sq.value(4).toString();
    wordsBy = sq.value(5).toString();
    musicBy = sq.value(6).toString();
    songText = sq.value(7).toString();
    notes = sq.value(8).toString();
    usePrivateSettings = sq.value(9).toBool();
    if(!sq.value(10).isNull())
        alignmentV = sq.value(10).toInt();
    if(!sq.value(11).isNull())
        alignmentH = sq.value(11).toInt();
    if(!sq.value(12).isNull())
        color = QColor::fromRgb(sq.value(12).toUInt());
    if(!sq.value(13).isNull())
        font.fromString(sq.value(13).toString());
    if(!sq.value(14).isNull())
        infoColor = QColor::fromRgb(sq.value(14).toUInt());
    if(!sq.value(15).isNull())
        infoFont.fromString(sq.value(15).toString());
    if(!sq.value(16).isNull())
        endingColor = QColor::fromRgb(sq.value(16).toUInt());
    if(!sq.value(17).isNull())
        endingFont.fromString(sq.value(17).toString());
    useBackground = sq.value(18).toBool();
    backgroundName = sq.value(19).toString();
    background.loadFromData(sq.value(20).toByteArray());
}

QStringList Song::getSongTextList()
{
    // This function prepares a song list that will be shown in the song preview and show list.
    // It will it will automatically prepare correct sining order of verses and choruses.
    QStringList formatedSong; // List container for correctely ordered item. This item will be returned.
    QString text, line;
    QStringList songlist;
    QStringList chorus;
    bool has_chorus=false;
    bool has_vstavka=false;
    int pnum = 0;
    int chor = 0;
    int listcount(0);

    songlist = songText.split("\n");
    listcount = songlist.count();
    while(pnum < listcount)
    {
        line = songlist.at(pnum);
        if(isStanzaVerseTitle(line))
        {
            // Fill Verse
            text = getStanzaBlock(pnum,songlist);
            formatedSong.append(text);

            if (has_chorus)// add Chorus stansa to the formated list if it exists
                formatedSong.append(chorus);

            has_vstavka = false;
        }
        else if(isStanzaAndVerseTitle(line))
        {

            // Fill Additional parts of the verse
            text = getStanzaBlock(pnum,songlist);
            // it chorus esits, this means that it was added to the formated list
            // and needs to be removed before adding addintion Veres stansas to formated list
            if(has_chorus)
                removeLastChorus(chorus,formatedSong);

            formatedSong.append(text);

            if (has_chorus)// add Chorus stansa to the formated list if it exists
                formatedSong.append(chorus);

            has_vstavka = false;
        }
        else if (isStanzaSlideTitle(line))
        {
            // Fill Insert
            text = getStanzaBlock(pnum,songlist);
            formatedSong.append(text);

            // Chorus is not added to Insert, if one is needed,
            // it should be added when song is edited, otherwise
            // there is no difirence between Veres and Insert
            has_vstavka = true;
        }
        else if (isStanzaRefrainTitle(line))
        {
            // Fill Chorus
            text = getStanzaBlock(pnum,songlist);
            QStringList chorusold = chorus;
            chorus.clear();
            chorus.append(text);
            has_chorus = true;
            ++chor;

            if (chor == 1) // if first Chorus, add chorus to formated list
                formatedSong.append(chorus);
            else if ((chor == 2) && !has_vstavka ) // if second chorus and Insert was not added
            {
                // remove exising chorus
                removeLastChorus(chorusold,formatedSong);

                // and add new chorus to formated list
                formatedSong.append(chorus);
                chor--;
            }
            else if ((chor == 2) && has_vstavka ) // if second chorus and Insert was added
            {
                // and add new chorus to formated list
                formatedSong += chorus;
                chor-- ;
            }
            has_vstavka = false;
        }
        else if(isStanzaAndRefrainTitle(line))
        {
            // Fill other chorus parts to Chorus block
            text = getStanzaBlock(pnum,songlist);

            removeLastChorus(chorus,formatedSong);
            chorus.append(text);
            formatedSong.append(chorus);// replace removed chorus parts with complete chorus list
        }
        ++pnum;
    }

    return formatedSong;
}

QString Song::getStanzaBlock(int &i, QStringList &list)
{
    QString line,block;
    int j(i);

    while(i < list.count())
    {
        line = list.at(i);
        if(line.contains(QRegExp("^&")))
            line.remove("&");

        if(isStanzaTitle(line) && (i!=j))
        {
                i--;
            break;
        }
        block += line + "\n";
        ++i;
    }

    return block.trimmed();
}

void Song::removeLastChorus(QStringList ct, QStringList &list)
{
    for(int i(0);i<ct.count();++i)
        list.removeLast();
}

Stanza Song::getStanza(int current)
{
    Stanza stanza;
    QStringList song_list = getSongTextList();
    stanza.isLast = (current == song_list.count()-1);
    stanza.number = number;
    stanza.tune = tune;
    stanza.musicBy = musicBy;
    stanza.wordsBy = wordsBy;
    stanza.usePrivateSettings = usePrivateSettings;
    stanza.alignmentV = alignmentV;
    stanza.alignmentH = alignmentH;
    stanza.useBackground = useBackground;
    stanza.backgroundName = backgroundName;
    stanza.background = background;
    stanza.color = color;
    stanza.font = font;
    stanza.infoColor = infoColor;
    stanza.infoFont = infoFont;
    stanza.endingColor = endingColor;
    stanza.endingFont = endingFont;

    QStringList lines_list = song_list.at(current).split("\n");
    if(isStanzaTitle(lines_list.at(0)))
    {
        stanza.stanzaTitle = lines_list.at(0);
        lines_list.removeFirst();
    }

    stanza.stanza = lines_list.join("\n").trimmed();
    return stanza;
}

QString Song::getSongbookName()
{
    if( songbook_name.isEmpty() )
        readData();
    return songbook_name;
}

SongsModel::SongsModel()
{
}

Song SongsModel::getSong(int row)
{
    return song_list.at(row);
}

Song SongsModel::getSong(QModelIndex index)
{
    return song_list.at(index.row());
}

void SongsModel::setSongs(QList<Song> songs)
{
    emit layoutAboutToBeChanged();
    song_list.clear();
    song_list = songs;
    emit layoutChanged();
}

void SongsModel::updateSongFromDatabase(int songid)
{
    emit layoutAboutToBeChanged();
    for( int i=0; i < song_list.size(); ++i) {
        Song *song = (Song*)&(song_list.at(i));
        if( song->songID == songid )
        {
            song->readData();
            emit layoutChanged(); // To redraw the table
            return;
        }
    }
}

void SongsModel::updateSongFromDatabase(int newSongId, int oldSongId)
{
    emit layoutAboutToBeChanged();
    for( int i=0; i < song_list.size(); i++)
    {
        Song *song = (Song*)&(song_list.at(i));
        if( song->songID == oldSongId )
        {
            song->songID = newSongId;
            // get song number and songbook id
            song->readData();
            QSqlQuery sq;
            // get songbook name
            sq.exec("SELECT name FROM Songbooks WHERE id = " + song->songbook_id );
            sq.first();
            song->songbook_name = sq.value(0).toString();

            emit layoutChanged(); // To redraw the table
            return;
        }
    }
}

void SongsModel::addSong(Song song)
{
    beginInsertRows(QModelIndex(), rowCount(), rowCount());
    song_list.append(song);
    endInsertRows();
}

bool SongsModel::removeRows( int row, int count, const QModelIndex & parent)
{
    beginRemoveRows(parent, row, row+count-1);
    // Need to remove starting from the end:
    for(int i=row+count-1; i>=row; i--)
        song_list.removeAt(i);
    endRemoveRows();
    return true;
}

int SongsModel::rowCount(const QModelIndex &parent) const
{
    return song_list.count();
}

int SongsModel::columnCount(const QModelIndex &parent) const
{
    return 5;
}

QVariant SongsModel::data(const QModelIndex &index, int role) const
{
    if( !index.isValid() )
        return QVariant();

    if( role == Qt::DisplayRole )
    {
        Song song = song_list.at(index.row());
        if( index.column() == 0 )       //Category
            return QVariant(song.category);
        else if( index.column() == 1 )  //Song Number
            return QVariant(song.number);
        else if( index.column() == 2)   //Song Title
            return QVariant(song.title);
        else if( index.column() == 3)   //Songbook
            return QVariant(song.getSongbookName());
        else if( index.column() == 4)   //Tune
            return QVariant(song.tune);
    }
    return QVariant();
}

QVariant SongsModel::headerData(int section, Qt::Orientation orientation,int role) const
{
    if (role == Qt::DisplayRole && orientation == Qt::Horizontal )
    {
        switch(section) {
        case 1:
            return QVariant(tr("Num"));
        case 2:
            return QVariant(tr("Title"));
        case 3:
            return QVariant(tr("Songbook"));
        case 4:
            return QVariant(tr("Tune"));
        }
    }
    return QVariant();
}

void SongsModel::emitLayoutChanged()
{
    emit layoutChanged();
}

void SongsModel::emitLayoutAboutToBeChanged()
{
    emit layoutAboutToBeChanged();
}

bool SongsModel::isInTable(int songid)
{
    foreach(Song agent, song_list)
        if(agent.songID == songid)
            return true;
    return false;
}

SongProxyModel::SongProxyModel(QObject *parent) : QSortFilterProxyModel(parent)
{
}

void SongProxyModel::setFilterString(QString new_string, bool new_match_beginning, bool new_exact_match)
{
    filter_string = new_string;
    match_beginning = new_match_beginning;
    exact_match = new_exact_match;
}

void SongProxyModel::setSongbookFilter(QString new_songbook)
{
    songbook_filter = new_songbook;
}

void SongProxyModel::setCategoryFilter(int category)
{
    category_filter = QString::number(category);
}

bool SongProxyModel::filterAcceptsRow(int sourceRow,
                                      const QModelIndex &sourceParent) const
{
    QModelIndex index0 = sourceModel()->index(sourceRow, 0, sourceParent);//Category
    QModelIndex index1 = sourceModel()->index(sourceRow, 1, sourceParent);//Song Number
    QModelIndex index2 = sourceModel()->index(sourceRow, 2, sourceParent);//Song Title
    QModelIndex index3 = sourceModel()->index(sourceRow, 3, sourceParent);//Songbook

    QString str0 = sourceModel()->data(index0).toString();//Category
    QString str1 = sourceModel()->data(index1).toString();//Song Number
    QString str2 = sourceModel()->data(index2).toString();//Song Title
    QString str3 = sourceModel()->data(index3).toString();//Songbook


    // Exclude rows that are not part of the selected songbook:
    if( songbook_filter != "ALL" )
        if( str3 != songbook_filter )
            return false;

    // Exclude rows that are not part of selected category
    if(category_filter != "-1")
        if( str0 != category_filter)
            return false;

    if( filter_string.isEmpty() )
        // No filter specified
        return true;

    QRegExp rx;
    rx.setCaseSensitivity(Qt::CaseInsensitive);
    QString s = filter_string;
    s.replace(" ","\\W*");
    if(exact_match)
        return ( str1.compare(filter_string, Qt::CaseInsensitive) == 0
                 || str2.compare(filter_string, Qt::CaseInsensitive) == 0 );
    else if(match_beginning)
    {
        rx.setPattern("^"+s);
        return (str1.contains(rx)
                || str2.contains(rx) );
    }
    else
    {
        rx.setPattern(s);
        return (str1.contains(rx)
                || str2.contains(rx) );
    }
}

SongDatabase::SongDatabase()
{
}

void Song::saveUpdate()
{
    // Update song information
    QSqlQuery sq;
    sq.prepare("UPDATE Songs SET songbook_id = ?, number = ?, title = ?, category = ?, tune = ?, words = ?, music = ?, "
               "song_text = ?, notes = ?, use_private = ?, alignment_v = ?, alignment_h = ?, color = ?, font = ?, "
               "info_color = ?, info_font = ?, ending_color = ?, ending_font = ?, use_background = ?, "
               "background_name = ?, background = ? WHERE id = ?");
    sq.addBindValue(songbook_id);
    sq.addBindValue(number);
    sq.addBindValue(title);
    sq.addBindValue(category);
    sq.addBindValue(tune);
    sq.addBindValue(wordsBy);
    sq.addBindValue(musicBy);
    sq.addBindValue(songText);
    sq.addBindValue(notes);
    sq.addBindValue(usePrivateSettings);
    sq.addBindValue(alignmentV);
    sq.addBindValue(alignmentH);
    sq.addBindValue((unsigned int)(color.rgb()));
    sq.addBindValue(font.toString());
    sq.addBindValue((unsigned int)(infoColor.rgb()));
    sq.addBindValue(infoFont.toString());
    sq.addBindValue((unsigned int)(endingColor.rgb()));
    sq.addBindValue(endingFont.toString());
    sq.addBindValue(useBackground);
    sq.addBindValue(backgroundName);
    sq.addBindValue(pixToByte(background));
    sq.addBindValue(songID);
    sq.exec();
}

void Song::saveNew()
{
    // Add a new song
    QSqlQuery sq;
    sq.prepare("INSERT INTO Songs (songbook_id,number,title,category,tune,words,music,song_text,notes,"
               "use_private,alignment_v,alignment_h,color,font,info_color,info_font,ending_color,"
               "ending_font,use_background,background_name,background) "
               "VALUES(?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?)");
    sq.addBindValue(songbook_id);
    sq.addBindValue(number);
    sq.addBindValue(title);
    sq.addBindValue(category);
    sq.addBindValue(tune);
    sq.addBindValue(wordsBy);
    sq.addBindValue(musicBy);
    sq.addBindValue(songText);
    sq.addBindValue(notes);
    sq.addBindValue(usePrivateSettings);
    sq.addBindValue(alignmentV);
    sq.addBindValue(alignmentH);
    sq.addBindValue((unsigned int)(color.rgb()));
    sq.addBindValue(font.toString());
    sq.addBindValue((unsigned int)(infoColor.rgb()));
    sq.addBindValue(infoFont.toString());
    sq.addBindValue((unsigned int)(endingColor.rgb()));
    sq.addBindValue(endingFont.toString());
    sq.addBindValue(useBackground);
    sq.addBindValue(backgroundName);
    sq.addBindValue(pixToByte(background));
    sq.exec();
}

Song SongDatabase::getSong(int id)
{
    Song song(id);
    song.readData();
    return song;
}

QList<Song> SongDatabase::getSongs()
{
    QList<Song> songs;

    QSqlQuery sq;
    QStringList sb_ids, sb_names;

    // get songbook names and ids
    sq.exec("SELECT id, name FROM Songbooks");
    while (sq.next())
    {
        sb_ids << sq.value(0).toString();
        sb_names << sq.value(1).toString();
    }
    sq.clear();

    // get songs
    //              0               1       2     3        4    5      6       7         8
    //        9               10        11          12     13    14            15          16         17
    //        18                19              20
    sq.exec("SELECT id, songbook_id, number, title, category, tune, words, music, song_text, notes, "
            "use_private, alignment_v, alignment_h, color, font, info_color, info_font, ending_color, ending_font, "
            "use_background, background_name, background FROM Songs");
    while(sq.next())
    {
        Song song;
        song.songID = sq.value(0).toInt();
        song.songbook_id = sq.value(1).toString();
        song.number = sq.value(2).toInt();
        song.title = sq.value(3).toString();
        song.category = sq.value(4).toInt();
        song.tune = sq.value(5).toString();
        song.wordsBy = sq.value(6).toString();
        song.musicBy = sq.value(7).toString();
        song.songText = sq.value(8).toString();
        song.notes = sq.value(9).toString();
        song.usePrivateSettings = sq.value(10).toBool();
        if(!sq.value(11).isNull())
            song.alignmentV = sq.value(11).toInt();
        if(!sq.value(12).isNull())
            song.alignmentH = sq.value(12).toInt();
        if(!sq.value(13).isNull())
            song.color = QColor::fromRgb(sq.value(13).toUInt());
        if(!sq.value(14).isNull())
            song.font.fromString(sq.value(14).toString());
        if(!sq.value(15).isNull())
            song.infoColor = QColor::fromRgb(sq.value(15).toUInt());
        if(!sq.value(16).isNull())
            song.infoFont.fromString(sq.value(16).toString());
        if(!sq.value(17).isNull())
            song.endingColor = QColor::fromRgb(sq.value(17).toUInt());
        if(!sq.value(18).isNull())
            song.endingFont.fromString(sq.value(18).toString());
        song.useBackground = sq.value(19).toBool();
        song.backgroundName = sq.value(20).toString();
        song.background.loadFromData(sq.value(21).toByteArray());
        song.songbook_name = sb_names.at(sb_ids.indexOf(song.songbook_id));

        songs.append(song);
    }
    return songs;
}

bool Song::isValid()
{
    // Check if song is valid by song id
    if(songID>0)
        return true;
    else
        return false;
}

void Song::getSettings(SongSettings &settings)
{
    settings.textAlignmentV = alignmentV;
    settings.textAlignmentH = alignmentH;
    settings.useBackground = useBackground;
    settings.backgroundName = backgroundName;
    settings.backgroundPix = background;
    settings.textColor = color;
    settings.textFont = font;
    settings.infoColor = infoColor;
    settings.infoFont = infoFont;
    settings.endingColor = endingColor;
    settings.endingFont = endingFont;
}

int SongDatabase::lastUser(QString songbook_id)
{
    int last;
    QList <int> lastInt;
    QSqlQuery sq;
    sq.exec("SELECT number FROM Songs WHERE songbook_id = " +songbook_id);
    while (sq.next())
        lastInt << sq.value(0).toInt();
    qSort(lastInt);
    if (lastInt.isEmpty())
        last=1;
    else
        last = lastInt.takeLast() + 1;
    return last;
}

void SongDatabase::deleteSong(int song_id)
{
    QSqlQuery sq;
    sq.exec("DELETE FROM Songs WHERE id = " + QString::number(song_id) );
}

QString SongDatabase::getSongbookIdStringFromName(QString songbook_name)
{
    QSqlQuery sq;
    sq.exec("SELECT id FROM Songbooks WHERE name = '" + songbook_name +"'"); //ilya
    sq.first();
    return sq.value(0).toString();
}

void SongDatabase::addSongbook(QString name, QString info)
{
    QSqlTableModel sqt;
    sqt.setTable("Songbooks");
    sqt.insertRows(0,1);
    sqt.setData(sqt.index(0,1),name.trimmed());
    sqt.setData(sqt.index(0,2),info);
    sqt.setData(sqt.index(0,3),1);
    sqt.submitAll();
}
