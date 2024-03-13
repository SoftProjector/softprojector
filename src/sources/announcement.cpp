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

#include <QtSql>
#include <QDebug>
#include "announcement.hpp"

Announcement::Announcement()
{
    setDefaults();
}

Announcement::Announcement(int id)
{
    setDefaults();
    idNum = id;
}

void Announcement::readData()
{
    QSqlQuery sq;
    //              0       1       2           3       4   5           6               7
    sq.exec("SELECT title, text, usePrivate, useAuto, loop, slideTime, useBackground, backgoundPath, "
            "font, color, alignment FROM Announcements WHERE id = " + QString::number(idNum));
    //          8     9     10
    sq.first();
    title = sq.value(0).toString().trimmed();
    text = sq.value(1).toString().trimmed();
    usePrivateSettings = sq.value(2).toBool();
    useAutoNext = sq.value(3).toBool();
    loop = sq.value(4).toBool();
    slideTimer = sq.value(5).toInt();
    useBackground = sq.value(6).toBool();
    backgroundPath = sq.value(7).toString().trimmed();
    font.fromString(sq.value(8).toString().trimmed());
    color = QColor(sq.value(9).toUInt());
    QString str = sq.value(10).toString().trimmed();
    QStringList l = str.split(",");
    alignmentV = l.at(0).toInt();
    alignmentH = l.at(1).toInt();
}

QStringList Announcement::getAnnounceList()
{
    QStringList alist;
    QStringList l = text.split("\n");
    QString anPart;
    foreach(const QString s,l)
    {
        if(isAnnounceTitle(s))
        {
            if(!anPart.isEmpty())
                alist.append(anPart.trimmed());
            anPart.clear();
        }
        anPart += s + "\n";
    }

    anPart = anPart.trimmed();
    if(!anPart.isEmpty())
        alist.append(anPart.trimmed());

    return alist;
}

void Announcement::saveNew()
{
    QSqlQuery sq;
    sq.prepare("INSERT INTO Announcements (title, text, usePrivate, useAuto, loop, slideTime, "
               "useBackground, backgoundPath, font, color, alignment) VALUES (?,?,?,?,?,?,?,?,?,?,?)");
    sq.addBindValue(title); // 1
    sq.addBindValue(text); // 2
    sq.addBindValue(usePrivateSettings); // 3
    sq.addBindValue(useAutoNext); // 4
    sq.addBindValue(loop); // 5
    sq.addBindValue(slideTimer); // 6
    sq.addBindValue(useBackground); // 7
    sq.addBindValue(backgroundPath); // 8
    sq.addBindValue(font.toString()); // 9
    unsigned int txtColor = (unsigned int)(color.rgb());
    sq.addBindValue(QString::number(txtColor)); // 10
    sq.addBindValue(QString("%1,%2").arg(alignmentV).arg(alignmentH)); // 11
    sq.exec();
    sq.clear();

    // Update id number
    sq.exec("SELECT seq FROM sqlite_sequence WHERE name = 'Announcements'");
    sq.first();
    idNum = sq.value(0).toInt();
}

void Announcement::saveUpdate()
{
    QSqlQuery sq;
    sq.prepare("UPDATE Announcements SET title = ?, text = ?, usePrivate = ?, useAuto = ?, loop = ?, slideTime = ?, "
               "useBackground = ?, backgoundPath = ?, font = ?, color = ?, alignment = ? WHERE id  = ?");
    sq.addBindValue(title);
    sq.addBindValue(text);
    sq.addBindValue(usePrivateSettings);
    sq.addBindValue(useAutoNext);
    sq.addBindValue(loop);
    sq.addBindValue(slideTimer);
    sq.addBindValue(useBackground);
    sq.addBindValue(backgroundPath);
    sq.addBindValue(font.toString());
    unsigned int txtColor = (unsigned int)(color.rgb());
    sq.addBindValue(QString::number(txtColor));
    sq.addBindValue(QString("%1,%2").arg(alignmentV).arg(alignmentH));
    sq.addBindValue(idNum);
    sq.exec();
}

void Announcement::deleteAnnouce()
{
    QSqlQuery sq;
    sq.exec("DELETE FROM Announcements WHERE id = " + QString::number(idNum));
}

void Announcement::setDefaults()
{
    idNum = 0;
    title = "";
    text = "";
    usePrivateSettings = false;
    useAutoNext = true;
    loop = true;
    slideTimer = 10;
    font = QFont();
    color = QColor(Qt::white);
    useBackground = false;
    backgroundPath = "";
    alignmentV = 0;
    alignmentH = 1;
}

AnnounceSlide Announcement::getAnnounceSlide(int current)
{
    AnnounceSlide aslide;
    QStringList alist = getAnnounceList();
    QStringList slist = alist.at(current).split("\n");
    if(isAnnounceTitle(slist.at(0)))
        slist.removeFirst();

    aslide.text = slist.join("\n").trimmed();
    aslide.alignmentH = alignmentH;
    aslide.alignmentV = alignmentV;
    aslide.backgroundPath = backgroundPath;
    aslide.color = color;
    aslide.font = font;
    aslide.usePrivateSettings = usePrivateSettings;
    return aslide;
}

////////////////////////////////////////////////////////////////////////
//      ANNOUNCE MODEL
////////////////////////////////////////////////////////////////////////
AnnounceModel::AnnounceModel()
{
}

void AnnounceModel::setAnnoucements(QList<Announcement> announcements)
{
    emit layoutAboutToBeChanged();
    announceList.clear();
    announceList = announcements;
    emit layoutChanged();
}

void AnnounceModel::addAnnouncement(Announcement announce)
{
    beginInsertRows(QModelIndex(), rowCount(),rowCount());
    announceList.append(announce);
    endInsertRows();
}

Announcement AnnounceModel::getAnnounce(int row)
{
    return announceList.at(row);
}

Announcement AnnounceModel::getAnnounce(QModelIndex index)
{
    return announceList.at(index.row());
}

int AnnounceModel::rowCount(const QModelIndex &parent) const
{
    return announceList.count();
}

int AnnounceModel::columnCount(const QModelIndex &parent) const
{
    return 1;
}

QVariant AnnounceModel::data(const QModelIndex &index, int role) const
{
    if(!index.isValid())
        return QVariant();

    if(role == Qt::DisplayRole)
    {
        Announcement announce = announceList.at(index.row());
        if(index.column() == 0) // title
            return QVariant(announce.title.trimmed());
    }
    return QVariant();
}

QVariant AnnounceModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role == Qt::DisplayRole && orientation == Qt::Horizontal )
    {
        switch(section)
        {
        case 0:
            return QVariant(tr("Title"));
        }
    }
    return QVariant();
}

bool AnnounceModel::removeRows(int row, int count, const QModelIndex &parent)
{
    beginRemoveRows(parent,row,row+count-1);
    for(int i=row+count-1; i>=row;i--)
        announceList.removeAt(i);
    endRemoveRows();
    return true;
}

void AnnounceModel::emitLayoutChanged()
{
    emit layoutChanged();
}

void AnnounceModel::emitLayoutAboutToBeChanged()
{
    emit layoutAboutToBeChanged();
}

void AnnounceModel::updateAnnounceFromDatabase(int annId)
{
    emit layoutAboutToBeChanged();
    for(int i=0; i<announceList.count(); ++i)
    {
        Announcement *ann = (Announcement*)&(announceList.at(i));
        if(ann->idNum == annId)
        {
            ann->readData();
            emit layoutChanged();
            return;
        }
    }
}

//void AnnounceModel::updateAnnounceFromDatabase(int newAnnId, int oldAnnId)
//{
//    // TODO: do if needed
//}

bool AnnounceModel::isInTable(int annId)
{
    foreach(Announcement agent, announceList)
        if(agent.idNum == annId)
            return true;
    return false;
}

AnnounceProxyModel::AnnounceProxyModel(QObject *parent) : QSortFilterProxyModel(parent)
{
    filterString = QString();
    matchExact = false;
    matchBeginning = false;
}

void AnnounceProxyModel::setFilterString(QString new_string, bool new_match_beginning, bool new_exact_match)
{
    filterString = new_string;
    matchExact = new_exact_match;
    matchBeginning = new_match_beginning;
}

bool AnnounceProxyModel::filterAcceptsRow(int sourceRow, const QModelIndex &sourceParent) const
{
    QModelIndex index0 = sourceModel()->index(sourceRow,0,sourceParent); // Title
    QString str0 = sourceModel()->data(index0).toString(); // Title

    if(filterString.isEmpty())
        return true;

    QRegExp rx;
    rx.setCaseSensitivity(Qt::CaseInsensitive);
    QString fs = filterString;
    fs.replace(" ","\\W");
    if(matchExact)
        return(str0.compare(filterString, Qt::CaseInsensitive) == 0);
    else if(matchBeginning)
    {
        rx.setPattern("^"+fs);
        return(str0.contains(rx));
    }
    else
    {
        rx.setPattern(fs);
        return(str0.contains(rx));
    }
}
