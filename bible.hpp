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

#ifndef BIBLE_HPP
#define BIBLE_HPP

#include <QtSql>
#include "theme.hpp"
#include "settings.hpp"

class BibleVerse
{
public:
    // Hold Bible Verse Info
    QString verseId;
    int book;
    int chapter;
    int verseNumber;
    QString verseText;
};

class Verse
{
public:
    // For now both primary and secondary information is stored in
    // primary_text and primary_caption
    QString primary_text;
    QString secondary_text;
    QString primary_caption;
    QString secondary_caption;
    QString trinary_text;
    QString trinary_caption;
};

class BibleSearch
{
    // For holding search results
public:
    QString book;
    QString chapter;
    QString verse;
    QString verse_text;
    QString verse_id;
    QString display_text;
    int first_v;
    int last_v;
};

class BibleHistory
{
public:
    QString verseIds;
    QString caption;
    QString captionLong;
};

class BibleBook
{
    // For Holding Bible book infromation
public:
    QString book;
    QString bookId;
    int chapterCount;
};

class Bible
{
public:
    Bible();
    QStringList verseList;
    QStringList previewIdList; // Verses that are in the preview (chapter) list
    QStringList currentIdList; // Verses that are in the show list
    QList<BibleBook> books;
public slots:
    QList<BibleSearch> searchBible(bool begins, QRegExp searchExp);
    QList<BibleSearch> searchBible(bool allWords, QRegExp searchExp, int book);
    QList<BibleSearch> searchBible(bool allWords, QRegExp searchExp, int book, int chapter);
    QStringList getBooks();
    QString getBookName(int id);
    void getVerseRef(QString vId, QString &book, int &chapter, int &verse);
    int getVerseNumberLast(QString vId);
    QStringList getChapter(int book, int chapter);
    void getVerseAndCaption(QString &verse, QString &caption, QString verId, QString &bibId, bool useAbbr);
    int getCurrentBookRow(QString book);
    Verse getCurrentVerseAndCaption(QList<int> currentRows, BibleSettings& sets, BibleVersionSettings& bv);
    void setBiblesId(QString& id);
    QString getBibleName();
    void loadOperatorBible();
private:
    QString bibleId;
    QList<BibleVerse> operatorBible;
    void retrieveBooks();
private slots:
    void addSearchResult(const BibleVerse &bv,QList<BibleSearch> &bsl);
};

#endif // BIBLE_HPP
