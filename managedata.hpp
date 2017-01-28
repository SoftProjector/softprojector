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

#ifndef MANAGEDATA_HPP
#define MANAGEDATA_HPP

#include <QtGui>
#include <QtSql>
#include "theme.hpp"

class ManageData
{
public:
    ManageData();
};

///////////////////////
// Bibles Class
///////////////////////
class Bibles
{
public:
    Bibles();
    QString bibleId;
    QString title;
    QString abbr;
    QString info;
    bool isRtoL;
};

///////////////////////
// Bibles Model Class
///////////////////////
class BiblesModel : public QAbstractTableModel
{
    Q_OBJECT
    Q_DISABLE_COPY(BiblesModel)

public:
    BiblesModel();
    QList<Bibles> bible_list;

    void setBible(QList<Bibles> bibles);
    void addBible(Bibles bible);
    Bibles getBible(int row);

    int rowCount(const QModelIndex &parent = QModelIndex()) const;
    int columnCount(const QModelIndex &parent = QModelIndex()) const;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const;
    bool removeRows(int row, int count, const QModelIndex &parent = QModelIndex());

};

///////////////////////
// Songbook Class
///////////////////////
class Songbook
{
public:
    Songbook();
    QString songbookId;
    QString title;
    QString info;
};

///////////////////////
// Songbooks Model Class
///////////////////////
class SongbooksModel : public QAbstractTableModel
//  Class for storing data for Songbook Table
{
    Q_OBJECT
    Q_DISABLE_COPY(SongbooksModel)

public:
    SongbooksModel();
    QList<Songbook> songbook_list;

    void setSongbook(QList<Songbook> songbooks);
    void addSongbook(Songbook songbook);
    Songbook getSongbook(int row);

    int rowCount(const QModelIndex &parent = QModelIndex()) const;
    int columnCount(const QModelIndex &parent = QModelIndex()) const;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const;
    bool removeRows(int row, int count, const QModelIndex &parent = QModelIndex());
};

//*************************************/
//*** Theme Model Class ***************/
//*************************************/
class ThemeModel : public QAbstractTableModel
{
    Q_OBJECT
    Q_DISABLE_COPY(ThemeModel)

public:
    ThemeModel();
    QList<ThemeInfo> themeList;

    void setThemes(QList<ThemeInfo> themes);
    void addTheme(ThemeInfo theme);
    ThemeInfo getTheme(int row);

    int rowCount(const QModelIndex &parent = QModelIndex()) const;
    int columnCount(const QModelIndex &parent = QModelIndex()) const;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const;
    bool removeRows(int row, int count, const QModelIndex &parent = QModelIndex());
};

///////////////////////
// Database Class
///////////////////////
class Database
{
public:
    Database();
    QList<Songbook> getSongbooks();
    QList<Bibles> getBibles();
    QList<ThemeInfo> getThemes();
};

#endif // MANAGEDATA_HPP
