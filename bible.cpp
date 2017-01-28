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

#include "bible.hpp"

Bible::Bible()
{
}

void Bible::setBiblesId(QString& id)
{
    bibleId = id;
    retrieveBooks();
}

QString Bible::getBibleName()
{
    if(bibleId.isEmpty())
        return "";
    QSqlQuery sq;
    sq.exec("SELECT bible_name FROM BibleVersions WHERE id = "+ bibleId );
    sq.first();
    QString b = sq.value(0).toString().trimmed();
    return b;
}

void Bible::retrieveBooks()
{
    BibleBook book;
    QSqlQuery sq;
    books.clear();
    sq.exec("SELECT book_name, id, chapter_count FROM BibleBooks WHERE bible_id = "+ bibleId );
    while (sq.next())
    {
        book.book = sq.value(0).toString().trimmed();
        book.bookId = sq.value(1).toString();
        book.chapterCount = sq.value(2).toInt();
        books.append(book);
    }
}

QStringList Bible::getBooks()
{
    QStringList book_list;
    if( books.count() == 0 )
        retrieveBooks();
    for(int i(0); books.count()>i;++i)
        book_list.append(books.at(i).book);

    return book_list;
}

QString Bible::getBookName(int id)
{
    QString book;
    foreach (const BibleBook bk, books)
    {
        if(bk.bookId.toInt() == id)
        {
            book = bk.book;
            break;
        }
    }
    return book;
}

void Bible::getVerseRef(QString vId, QString &book, int &chapter, int &verse)
{
    if(vId.contains(","))
        vId = vId.split(",").first();

    foreach(const BibleVerse &bv, operatorBible)
    {
        if(bv.verseId == vId)
        {
            book = QString::number(bv.book);
            chapter = bv.chapter;
            verse = bv.verseNumber;
        }
    }

    foreach (const BibleBook bk, books)
    {
        if(bk.bookId == book)
            book = bk.book;
    }
}

int Bible::getVerseNumberLast(QString vId)
{
    int vernum(0);
    if(vId.contains(","))
        vId = vId.split(",").last();

    foreach(const BibleVerse &bv, operatorBible)
    {
        if(bv.verseId == vId)
        {
            vernum = bv.verseNumber;
            break;
        }
    }
    return vernum;
}

int Bible::getCurrentBookRow(QString book)
{
    int chapters(0);
    for(int i(0); books.count()>i;++i)
    {
        if(books.at(i).book==book)
        {
            chapters = i;
            break;
        }
    }
    return chapters;
}

QStringList Bible::getChapter(int book, int chapter)
{
    QString verseText, id;
    int verse(0), verse_old(0);

    previewIdList.clear();
    verseList.clear();
    foreach (const BibleVerse &bv,operatorBible)
    {
        if(bv.book == book && bv.chapter == chapter)
        {
            verse  = bv.verseNumber;
            if(verse==verse_old)
            {
                verseText = verseText.simplified() + " " + bv.verseText;
                id += "," + bv.verseId;
                verseList.removeLast();
                previewIdList.removeLast();
            }
            else
            {
                verseText = bv.verseText;
                id = bv.verseId;
            }
            verseList << QString::number(verse) + ". " + verseText;
            previewIdList << id;
            verse_old = verse;
        }
    }

    return verseList;
}

Verse Bible::getCurrentVerseAndCaption(QList<int>  currentRows, BibleSettings& sets, BibleVersionSettings &bv)
{
    QString verse_id;
    for(int i(0);i<currentRows.count();++i)
    {
        verse_id += currentIdList.at(currentRows.at(i)) + ",";
    }
    verse_id.chop(1);

    Verse v;

    // get primary verse
    getVerseAndCaption(v.primary_text,v.primary_caption,verse_id,bv.primaryBible,sets.useAbbriviation);

    // get secondary verse
    if(bv.primaryBible!=bv.secondaryBible && bv.secondaryBible!="none")
        getVerseAndCaption(v.secondary_text,v.secondary_caption,verse_id,bv.secondaryBible,sets.useAbbriviation);

    // get trinary versse
    if(bv.trinaryBible!=bv.primaryBible && bv.trinaryBible!=bv.secondaryBible && bv.trinaryBible!="none")
        getVerseAndCaption(v.trinary_text,v.trinary_caption,verse_id,bv.trinaryBible,sets.useAbbriviation);

    return v;
}

void Bible::getVerseAndCaption(QString& verse, QString& caption, QString verId, QString& bibId, bool useAbbr)
{
    QString verse_old, verse_show, verse_n, verse_nold, verse_nfirst, chapter;
    QString book;
    QStringList ids;
    QSqlQuery sq;

    // clean old verses
    verse.clear();
    caption.clear();

    if (verId.contains(","))// Run if more than one database verse items exist or show muliple verses
    {
        ids = verId.split(",");
        verId.replace(",", "' OR verse_id = '");
        sq.exec("SELECT book,chapter,verse,verse_text FROM BibleVerse WHERE ( verse_id = '"
                + verId +"' ) AND bible_id = " + bibId);
        while (sq.next())
        {
            book = sq.value(0).toString();
            chapter = sq.value(1).toString();
            verse_n = sq.value(2).toString();
            verse = sq.value(3).toString().trimmed();

            // Set first verse number
            if (verse_nfirst.isEmpty())
                verse_nfirst = verse_n;

            // If second(nold) verse number is the same or it is empty, then create a regular sigle verse
            // Else create a single display of muliple verses
            if (verse_n==verse_nold)
            {
                // If current verse number is same as first verse number,
                // then remove verse number from verse text and caption
                // shows only current verse number
                if(verse_n == verse_nfirst)
                {
                    int j(0);
                    for(int i(0);i<verse_show.count(); ++i)
                    {
                        j = 1 + i;
                        QString p =verse_show.at(i);
                        if(p==(")"))
                            break;
                    }
                    verse_show = verse_show.remove(0,j);

                    caption = " " + chapter + ":" + verse_n;
                }
                else
                    // Else if current verse number does match first verse number,
                    // then show bigening and eding verse numbers in caption
                    caption = " " + chapter + ":" + verse_nfirst + "-" + verse_n;

                verse_show += " " + verse;
            }
            else
            {
                caption =" " + chapter + ":" + verse_nfirst + "-" + verse_n;
                verse = " (" + verse_n + ") " + verse;
                verse_show += verse;
                if(!verse_show.startsWith(" ("))
                    verse_show = " ("+ verse_nfirst + ") " + verse_show;
            }
            verse_old = verse;
            verse_nold = verse_n;
        }
        verse = verse_show.simplified();
    }
    else // Run as standard single verse item from database
    {
        sq.exec("SELECT book,chapter,verse,verse_text FROM BibleVerse WHERE verse_id = '"
                +verId+"' AND bible_id = " + bibId);

        sq.first();
        verse = sq.value(3).toString().trimmed();// Remove the empty line at the end using .trimmed()

        book = sq.value(0).toString();
        caption =" " + sq.value(1).toString() + ":" + sq.value(2).toString();
        sq.clear();
    }

    // Add book name to caption
    sq.exec("SELECT book_name FROM BibleBooks WHERE id = "
            + book + " AND bible_id = " + bibId);
    sq.first();
    caption = sq.value(0).toString() + caption;
    sq.clear();

    // Add bible abbreveation if to to use it
    if(useAbbr)
    {
        sq.exec("SELECT abbreviation FROM BibleVersions WHERE id = " + bibId);
        sq.first();
        QString abr = sq.value(0).toString().trimmed();
        if (!abr.isEmpty())
            caption = QString("%1 (%2)").arg(caption).arg(abr);
    }

    verse = verse.simplified();
    caption = caption.simplified();
}

QList<BibleSearch> Bible::searchBible(bool allWords, QRegExp searchExp)
{   ///////// Search entire Bible //////////

    QList<BibleSearch> return_results;

    QString sw = searchExp.pattern();
    sw.remove("\\b(");
    sw.remove(")\\b");

    foreach(const BibleVerse &bv,operatorBible)
    {
        if(bv.verseText.contains(searchExp))
        {
            if(allWords)
            {
                QStringList stl = sw.split("|");
                bool hasAll = false;
                for (int j(0);j<stl.count();++j)
                {
                    hasAll = bv.verseText.contains(QRegExp("\\b"+stl.at(j)+"\\b",Qt::CaseInsensitive));
                    if(!hasAll)
                        break;
                }
                if(hasAll)
                    addSearchResult(bv,return_results);
            }
            else
                addSearchResult(bv,return_results);
        }

    }

    return return_results;
}

QList<BibleSearch> Bible::searchBible(bool allWords, QRegExp searchExp, int book)
{   ///////// Search in selected book //////////

    QList<BibleSearch> return_results;

    QString sw = searchExp.pattern();
    sw.remove("\\b(");
    sw.remove(")\\b");

    foreach(const BibleVerse &bv,operatorBible)
    {
        if(bv.verseText.contains(searchExp) && bv.book == book)
        {
            if(allWords)
            {
                QStringList stl = sw.split("|");
                bool hasAll = false;
                for (int j(0);j<stl.count();++j)
                {
                    hasAll = bv.verseText.contains(QRegExp("\\b"+stl.at(j)+"\\b",Qt::CaseInsensitive));
                    if(!hasAll)
                        break;
                }
                if(hasAll)
                    addSearchResult(bv,return_results);
            }
            else
                addSearchResult(bv,return_results);
        }
    }

    return return_results;
}

QList<BibleSearch> Bible::searchBible(bool allWords, QRegExp searchExp, int book, int chapter)
{   ///////// Search in selected chapter //////////

    QList<BibleSearch> return_results;

    QString sw = searchExp.pattern();
    sw.remove("\\b(");
    sw.remove(")\\b");

    foreach(const BibleVerse &bv,operatorBible)
    {
        if(bv.verseText.contains(searchExp) && bv.book == book && bv.chapter == chapter)
        {
            if(allWords)
            {
                QStringList stl = sw.split("|");
                bool hasAll = false;
                for (int j(0);j<stl.count();++j)
                {
                    hasAll = bv.verseText.contains(QRegExp("\\b"+stl.at(j)+"\\b",Qt::CaseInsensitive));
                    if(!hasAll)
                        break;
                }
                if(hasAll)
                    addSearchResult(bv,return_results);
            }
            else
                addSearchResult(bv,return_results);
        }
    }

    return return_results;
}

void Bible::addSearchResult(const BibleVerse &bv, QList<BibleSearch> &bsl)
{
    BibleSearch  results;
    foreach (const BibleBook &bk,books)
    {
        if(bk.bookId == QString::number(bv.book))
        {
            results.book = bk.book;
            break;
        }
    }
    results.chapter = QString::number(bv.chapter);
    results.verse = QString::number(bv.verseNumber);
    results.verse_text = QString("%1 %2:%3 %4").arg(results.book).arg(results.chapter).arg(results.verse).arg(bv.verseText);

    bsl.append(results);
}

void Bible::loadOperatorBible()
{
    operatorBible.clear();
    BibleVerse bv;
    QSqlQuery sq;
    sq.exec("SELECT verse_id, book, chapter, verse, verse_text FROM BibleVerse WHERE bible_id = '"+bibleId+"'");
    while(sq.next())
    {
        bv.verseId = sq.value(0).toString().trimmed();
        bv.book = sq.value(1).toInt();
        bv.chapter = sq.value(2).toInt();
        bv.verseNumber = sq.value(3).toInt();
        bv.verseText = sq.value(4).toString().trimmed();
        operatorBible.append(bv);
    }
}
