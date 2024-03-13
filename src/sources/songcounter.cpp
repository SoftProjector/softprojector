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

#include "songcounter.hpp"
#include "ui_songcounter.h"

SongCounter::SongCounter(QWidget *parent, QString loc) :
    QDialog(parent),
    ui(new Ui::SongCounter)
{
    ui->setupUi(this);
    ui->closeButton->setFocus();

    splocale = loc;

    song_count_list = getSongCounts();
    songCounterModel = new SongCounterModel;
    loadCounts();

    // Modify the column widths:
    ui->countTable->setColumnWidth(0, 150);//songbook
    ui->countTable->setColumnWidth(1, 65);//number
    ui->countTable->setColumnWidth(2, 250);//title
    ui->countTable->setColumnWidth(3, 60);////count
    ui->countTable->setColumnWidth(4, 100);//date
    // Decrease the row height:
    ui->countTable->resizeRowsToContents();
}

SongCounter::~SongCounter()
{
    delete ui;
}

void SongCounter::on_closeButton_clicked()
{
    close();
}

void SongCounter::on_resetButton_clicked()
{
    // Code to reset counters to 0
    QSqlQuery sq;
    sq.exec("UPDATE Songs SET count = 0 , date = '' WHERE count > 0");

    song_count_list = getSongCounts();
    loadCounts();
}


void SongCounter::on_resetOneButton_clicked()
{
    // reset curently selected to 0
    int row = songCounterProxyModel->mapToSource(ui->countTable->currentIndex()).row();

    if(row>=0)
    {
        Counter count_to_remove = songCounterModel->getSongCount(row);

        QSqlQuery sq;
        sq.exec("UPDATE Songs SET count = 0, date = '' WHERE id = " + count_to_remove.id);
        song_count_list = getSongCounts();
        loadCounts();
    }
}

void SongCounter::loadCounts()
{
    songCounterProxyModel = new QSortFilterProxyModel;
    songCounterProxyModel->setSourceModel(songCounterModel);
    songCounterModel->setCounter(song_count_list);
    ui->countTable->setModel(songCounterProxyModel);
}

void SongCounter::addSongCount(Song song)
{
    int id = song.songID;
    int current_count(0);

    // get current song count
    QSqlQuery sq;
    sq.exec("SELECT count FROM Songs WHERE id = '" + QString::number(id) + "' ");
    sq.first();
    current_count = sq.value(0).toInt();
    sq.clear();

    // add one count to song
    ++current_count;

    // set todays date
    QDate d(QDate::currentDate());

    sq.exec(QString("UPDATE Songs SET count = %1 , date = '%2' WHERE id = %3").arg(QString::number(current_count)).arg(d.toString("MM:dd:yyyy")).arg(QString::number(id)));
    //    sq.exec("UPDATE Songs SET count = " + QString::number(current_count) + " WHERE id = " + QString::number(id));
}

//***********************************
//***********************************

QList<Counter> SongCounter::getSongCounts()
{
    QList<Counter> song_counts;
    Counter song_count;
    QSqlQuery sq, sq1;

    // Get counts
    //              0   1               2       3   4       5
    sq.exec("SELECT id, songbook_id, number, title, count, date FROM Songs WHERE count > 0");
    while (sq.next())
    {
        song_count.id = sq.value(0).toString();
        QString sbid = sq.value(1).toString();
        song_count.number = sq.value(2).toInt();
        song_count.title = sq.value(3).toString();
        song_count.count = sq.value(4).toInt();
        song_count.date = sq.value(5).toString();
        updateMonth(song_count.date);

        // get songbook name
        sq1.exec("SELECT name FROM Songbooks WHERE id = " + sbid);
        sq1.first();
        song_count.songbook = sq1.value(0).toString();
        song_counts.append(song_count);
    }
    return song_counts;
}

void SongCounter::updateMonth(QString &date)
{
    // need to use this function because Qt does not provide locale translations for all languages.
    QStringList dl = date.split(":");

    if(splocale == "en" || splocale.isEmpty())
    {
        // If current translation is English, use standard English date format
        if(dl.at(0)=="01")
            date = QString("%1 %2, %3").arg(tr("January")).arg(dl.at(1)).arg(dl.at(2));
        else if(dl.at(0)=="02")
            date = QString("%1 %2, %3").arg(tr("February")).arg(dl.at(1)).arg(dl.at(2));
        else if(dl.at(0)=="03")
            date = QString("%1 %2, %3").arg(tr("March")).arg(dl.at(1)).arg(dl.at(2));
        else if(dl.at(0)=="04")
            date = QString("%1 %2, %3").arg(tr("April")).arg(dl.at(1)).arg(dl.at(2));
        else if(dl.at(0)=="05")
            date = QString("%1 %2, %3").arg(tr("May")).arg(dl.at(1)).arg(dl.at(2));
        else if(dl.at(0)=="06")
            date = QString("%1 %2, %3").arg(tr("June")).arg(dl.at(1)).arg(dl.at(2));
        else if(dl.at(0)=="07")
            date = QString("%1 %2, %3").arg(tr("July")).arg(dl.at(1)).arg(dl.at(2));
        else if(dl.at(0)=="08")
            date = QString("%1 %2, %3").arg(tr("August")).arg(dl.at(1)).arg(dl.at(2));
        else if(dl.at(0)=="09")
            date = QString("%1 %2, %3").arg(tr("September")).arg(dl.at(1)).arg(dl.at(2));
        else if(dl.at(0)=="10")
            date = QString("%1 %2, %3").arg(tr("October")).arg(dl.at(1)).arg(dl.at(2));
        else if(dl.at(0)=="11")
            date = QString("%1 %2, %3").arg(tr("November")).arg(dl.at(1)).arg(dl.at(2));
        else if(dl.at(0)=="12")
            date = QString("%1 %2, %3").arg(tr("December")).arg(dl.at(1)).arg(dl.at(2));
    }
    else
    {
        // If current translation is NOT English, then use standart Europe date format
        if(dl.at(0)=="01")
            date = QString("%2 %1 %3").arg(tr("January")).arg(dl.at(1)).arg(dl.at(2));
        else if(dl.at(0)=="02")
            date = QString("%2 %1 %3").arg(tr("February")).arg(dl.at(1)).arg(dl.at(2));
        else if(dl.at(0)=="03")
            date = QString("%2 %1 %3").arg(tr("March")).arg(dl.at(1)).arg(dl.at(2));
        else if(dl.at(0)=="04")
            date = QString("%2 %1 %3").arg(tr("April")).arg(dl.at(1)).arg(dl.at(2));
        else if(dl.at(0)=="05")
            date = QString("%2 %1 %3").arg(tr("May")).arg(dl.at(1)).arg(dl.at(2));
        else if(dl.at(0)=="06")
            date = QString("%2 %1 %3").arg(tr("June")).arg(dl.at(1)).arg(dl.at(2));
        else if(dl.at(0)=="07")
            date = QString("%2 %1 %3").arg(tr("July")).arg(dl.at(1)).arg(dl.at(2));
        else if(dl.at(0)=="08")
            date = QString("%2 %1 %3").arg(tr("August")).arg(dl.at(1)).arg(dl.at(2));
        else if(dl.at(0)=="09")
            date = QString("%2 %1 %3").arg(tr("September")).arg(dl.at(1)).arg(dl.at(2));
        else if(dl.at(0)=="10")
            date = QString("%2 %1 %3").arg(tr("October")).arg(dl.at(1)).arg(dl.at(2));
        else if(dl.at(0)=="11")
            date = QString("%2 %1 %3").arg(tr("November")).arg(dl.at(1)).arg(dl.at(2));
        else if(dl.at(0)=="12")
            date = QString("%2 %1 %3").arg(tr("December")).arg(dl.at(1)).arg(dl.at(2));
    }

}

//***************************************
//****    Song Use Counter Model     ****
//***************************************
Counter::Counter()
{
}

SongCounterModel::SongCounterModel()
{
}

void SongCounterModel::setCounter(QList<Counter> song_counts)
{
    emit layoutAboutToBeChanged();
    song_count_list.clear();
    for (int i(0); i < song_counts.size(); i++)
    {
        Counter song_count = song_counts.at(i);
        song_count_list.append(song_count);
    }
    emit layoutChanged();
}

void SongCounterModel::addCounter(Counter song_count)
{
    beginInsertRows(QModelIndex(), rowCount(), rowCount());
    song_count_list.append(song_count);
    endInsertRows();
}

Counter SongCounterModel::getSongCount(int row)
{
    return song_count_list.at(row);
}

int SongCounterModel::rowCount(const QModelIndex &parent) const
{
    return song_count_list.count();
}

int SongCounterModel::columnCount(const QModelIndex &parent) const
{
    return 5;
}

QVariant SongCounterModel::data(const QModelIndex &index, int role) const
{
    if( index.isValid() && role == Qt::DisplayRole )
    {
        Counter song_count = song_count_list.at(index.row());
        if( index.column() == 0 )
            return QVariant(song_count.songbook);
        else if(index.column() == 1 )
            return QVariant(song_count.number);
        else if(index.column() == 2)
            return QVariant(song_count.title);
        else if(index.column() == 3)
            return QVariant(song_count.count);
        else if(index.column() == 4)
            return QVariant(song_count.date);
    }
    return QVariant();
}

QVariant SongCounterModel::headerData(int section,
                                      Qt::Orientation orientation,
                                      int role) const
{
    if (role == Qt::DisplayRole && orientation == Qt::Horizontal )
    {
        switch(section) {
        case 0:
            return QVariant(tr("Songbook"));
        case 1:
            return QVariant(tr("Number"));
        case 2:
            return QVariant(tr("Title"));
        case 3:
            return QVariant(tr("Count"));
        case 4:
            return QVariant(tr("Date"));
        }
    }
    return QVariant();
}

bool SongCounterModel::removeRows(int row, int count, const QModelIndex &parent)
{
    beginRemoveRows(parent, row, row+count-1);
    // Need to remove starting from the end:
    for(int i=row+count-1; i>=row; i--)
        song_count_list.removeAt(i);
    endRemoveRows();
    return true;
}
