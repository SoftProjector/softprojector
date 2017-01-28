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

#ifndef SONGCOUNTER_HPP
#define SONGCOUNTER_HPP

#include <QDialog>
#include <QtSql>
#include "song.hpp"

namespace Ui {
class SongCounter;
}

class Counter
{
public:
    Counter();
    QString id;
    QString title;
    QString songbook;
    int count;
    QString date;
    int number;
};

class SongCounterModel : public QAbstractTableModel
{
    //  Class for storing data for Song Use Counter Table
    Q_OBJECT
    Q_DISABLE_COPY(SongCounterModel)

public:
    SongCounterModel();
    QList<Counter> song_count_list;

    void setCounter(QList<Counter> song_counts);
    void addCounter(Counter song_count);
    Counter getSongCount(int row);

    int rowCount(const QModelIndex &parent = QModelIndex()) const;
    int columnCount(const QModelIndex &parent = QModelIndex()) const;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const;
    bool removeRows(int row, int count, const QModelIndex &parent = QModelIndex());

};

class SongCounter : public QDialog
{
    Q_OBJECT
public:
    explicit SongCounter(QWidget *parent = 0,QString loc = "en");
    ~SongCounter();

private:
    QString splocale;
    QList<Counter> song_count_list;
    SongCounterModel *songCounterModel;
    QSortFilterProxyModel *songCounterProxyModel;
    Ui::SongCounter *ui;

public slots:
    void addSongCount(Song song);

private slots:
    void updateMonth(QString& date);
    void loadCounts();
    void on_resetOneButton_clicked();
    void on_resetButton_clicked();
    void on_closeButton_clicked();
    QList<Counter> getSongCounts();
};

#endif // SONGCOUNTER_HPP
