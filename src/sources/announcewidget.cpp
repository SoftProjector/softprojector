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
#include "announcewidget.hpp"
#include "ui_announcewidget.h"

AnnounceWidget::AnnounceWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::AnnounceWidget)
{
    ui->setupUi(this);

    editAnounceDialog = new EditAnnouncementDialog(this);
    connect(editAnounceDialog,SIGNAL(announceToAdd(Announcement)),this,SLOT(addNewAnnouncement(Announcement)));
    connect(editAnounceDialog,SIGNAL(announceToUpdate()),this,SLOT(updateAnnouncement()));

    announceModel = new AnnounceModel;
    announceProxy = new AnnounceProxyModel(this);
    announceProxy->setSourceModel(announceModel);
    announceProxy->setDynamicSortFilter(true);
    ui->tableViewAnnouncements->setModel(announceProxy);
    setAnnounceList();
    loadAnnouncements();

    connect(ui->tableViewAnnouncements->selectionModel(), SIGNAL(currentRowChanged(QModelIndex,QModelIndex)),
            this, SLOT(announceViewRowChanged(QModelIndex,QModelIndex)));

    // Derease row height
    ui->tableViewAnnouncements->resizeRowsToContents();
}

AnnounceWidget::~AnnounceWidget()
{
    delete editAnounceDialog;
    delete announceModel;
    delete ui;
}

void AnnounceWidget::changeEvent(QEvent *e)
{
    QWidget::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        ui->retranslateUi(this);
        break;
    default:
        break;
    }
}

void AnnounceWidget::announceViewRowChanged(const QModelIndex &current, const QModelIndex &previous)
{
    if(current.isValid())
    {
        int row = announceProxy->mapToSource(current).row();
        previewAnnounce = announceModel->getAnnounce(row);
        loadAnnouncement();
    }
}

void AnnounceWidget::loadAnnouncements()
{
    announceModel->setAnnoucements(announceList);
}

void AnnounceWidget::loadAnnouncement()
{
    ui->listWidgetAnnouncement->clear();
    ui->listWidgetAnnouncement->addItems(previewAnnounce.getAnnounceList());
    ui->labelAnnounceTitle->setText(previewAnnounce.title);
}

void AnnounceWidget::setAnnounceList()
{
    QSqlQuery sq;
    Announcement a;
    announceList.clear();
    //              0       1       2           3       4   5           6               7
    sq.exec("SELECT title, text, usePrivate, useAuto, loop, slideTime, useBackground, backgoundPath, "
            "font, color, alignment, id FROM Announcements");
    //          8     9     10       11
    while(sq.next())
    {
        a.title = sq.value(0).toString().trimmed();
        a.text = sq.value(1).toString().trimmed();
        a.usePrivateSettings = sq.value(2).toBool();
        a.useAutoNext = sq.value(3).toBool();
        a.loop = sq.value(4).toBool();
        a.slideTimer = sq.value(5).toInt();
        a.useBackground = sq.value(6).toBool();
        a.backgroundPath = sq.value(7).toString().trimmed();
        a.font.fromString(sq.value(8).toString().trimmed());
        a.color = QColor(sq.value(9).toUInt());
        QString str = sq.value(10).toString().trimmed();
        QStringList l = str.split(",");
        a.alignmentV = l.at(0).toInt();
        a.alignmentH = l.at(1).toInt();
        a.idNum = sq.value(11).toInt();

        announceList.append(a);
    }
}

void AnnounceWidget::setPreview(Announcement announce)
{
    previewAnnounce = announce;
    ui->listWidgetAnnouncement->clear();
    ui->labelAnnounceTitle->setText(previewAnnounce.title);
    ui->listWidgetAnnouncement->addItems(previewAnnounce.getAnnounceList());
}

void AnnounceWidget::newAnnouncement()
{
    editAnounceDialog->setNewAnnouce();
    editAnounceDialog->exec();
}

void AnnounceWidget::editAnnouncement()
{
    if(previewAnnounce.idNum>0)
    {
        editAnounceDialog->setEditAnnouce(previewAnnounce);
        editAnounceDialog->exec();
    }
}

void AnnounceWidget::copyAnnouncement()
{
    if(previewAnnounce.idNum>0)
    {
        editAnounceDialog->setCopyAnnouce(previewAnnounce);
        editAnounceDialog->exec();
    }
}

void AnnounceWidget::deleteAnnouncement()
{
    if(previewAnnounce.idNum>0)
    {
        previewAnnounce.deleteAnnouce();
        previewAnnounce = Announcement();
        ui->listWidgetAnnouncement->clear();
        ui->labelAnnounceTitle->clear();
        int row = ui->tableViewAnnouncements->currentIndex().row();
        announceModel->removeRow(row);
    }
}

void AnnounceWidget::addNewAnnouncement(Announcement announce)
{
    announceModel->addAnnouncement(announce);
    ui->tableViewAnnouncements->selectRow(announceModel->rowCount()-1);
}

void AnnounceWidget::updateAnnouncement()
{
    announceModel->updateAnnounceFromDatabase(previewAnnounce.idNum);
    setPreview(currentAnnouncement());
    loadAnnouncement();
}

bool AnnounceWidget::isAnnounceValid()
{
    if(ui->tableViewAnnouncements->selectionModel()->selectedRows().count()>0)
        return true;
    else
        return false;
}

Announcement AnnounceWidget::getAnnouncement()
{
    return previewAnnounce;
}

void AnnounceWidget::sendToProjector()
{
    if(isAnnounceValid())
        emit sendAnnounce(previewAnnounce,ui->listWidgetAnnouncement->currentRow());
}

void AnnounceWidget::on_pushButtonLive_clicked()
{
    sendToProjector();
}

void AnnounceWidget::on_listWidgetAnnouncement_doubleClicked(const QModelIndex &index)
{
    sendToProjector();
}

void AnnounceWidget::on_tableViewAnnouncements_doubleClicked(const QModelIndex &index)
{
    int row = announceProxy->mapToSource(index).row();
    Announcement announce = announceModel->getAnnounce(row);
    emit addToSchedule(announce);
    setPreview(announce);
}

Announcement AnnounceWidget::currentAnnouncement()
{
    Announcement ancmnt;
    QModelIndex index;
    int row;
    index = announceProxy->mapToSource(ui->tableViewAnnouncements->currentIndex());
    row = index.row();

    if(row>=0)
        ancmnt = announceModel->getAnnounce(row);

    return ancmnt;
}

void AnnounceWidget::setAnnouncementFromHistory(Announcement &announce)
{
    ui->tableViewAnnouncements->clearSelection();
    setPreview(announce);
}
