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

#include "editwidget.hpp"
#include "ui_editwidget.h"
#include "song.hpp"

EditWidget::EditWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::EditWidget)
{
    song_database = SongDatabase();
    ui->setupUi(this);
    highlight = new Highlight(ui->textEditSong->document());

    //Add caterories to the list
    loadCategories(false);

    // Allow only positive values for the song number:
    song_num_validator = new QIntValidator(1,10000000,ui->lineEditSongNumber);
    ui->lineEditSongNumber->setValidator(song_num_validator);
}

EditWidget::~EditWidget()
{
    delete highlight;
    delete song_num_validator;
    delete ui;
}

void EditWidget::changeEvent(QEvent *e)
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

void EditWidget::on_btnSave_clicked()
{
    // Check if song title exists. A song title MUST exits
    QString song_title = ui->lineEditTitle->text();
    song_title = song_title.simplified(); // make sure that its not all empty spaces

    if(song_title.isEmpty())
    {
        QMessageBox mb(this);
        mb.setText(tr("Song title cannot be left empty.\nPlease enter song title."));
        mb.setWindowTitle(tr("Song title is missing"));
        mb.setIcon(QMessageBox::Warning);
        mb.exec();
        ui->lineEditTitle->setFocus();
        return;
    }
    setSave();

    setWaitCursor();
    if (is_new)
    {
        newSong.saveNew();
        // Get new songs ID
        QSqlQuery sq;
        sq.exec("SELECT seq FROM sqlite_sequence WHERE name = 'Songs'");
        sq.last();
        newSong.songID = sq.value(0).toInt();

        emit addedNew(newSong,song_to_edit_id);
    }
    else
    {
        newSong.saveUpdate();
        emit updateSongFromDatabase(newSong.songID,song_to_edit_id);
    }
    setArrowCursor();

    resetUiItems();
    close();
}

void EditWidget::setArrowCursor()
{
    this->setCursor(Qt::ArrowCursor);
}

void EditWidget::setWaitCursor()
{
    this->setCursor(Qt::WaitCursor);
}

void EditWidget::on_btnCancel_clicked()
{
    resetUiItems();
    close();
}

QString EditWidget::setSongText(QString song)
{
    QString text, text2, verselist;
    QStringList split, editlist;
    int i(0),j(0),k(0);

    editlist = song.split("@$");// split the text into verses seperated by @$

    while (i <editlist.size()){
        text = editlist[i];
        split = text.split("@%"); // split the text into rythmic line seperated by @%
        k=split.size();
        text=""; // clear text
        j=0;
        text2=split[0];
        while (j<k){
            if (j==k-1)
                text += split[j];
            else
                text += split[j] + "\n";
            ++j;
        }
        verselist += text.trimmed() + "\n\n";
        ++i;
    }
    return verselist;
}

void EditWidget::resetUiItems()
{
    Song ss;
    ui->lineEditTitle->setText(ss.title);
    ui->lineEditMusicBy->setText(ss.musicBy);
    ui->lineEditWordsBy->setText(ss.wordsBy);
    ui->lineEditKey->setText(ss.tune);
    ui->comboBoxCategory->setCurrentIndex(cat_ids.indexOf(ss.category));
    ui->checkBoxSongSettings->setChecked(ss.usePrivateSettings);
    ui->groupBoxSettings->setVisible(ss.usePrivateSettings);
    ui->comboBoxVAlignment->setCurrentIndex(ss.alignmentV);
    ui->comboBoxHAlignment->setCurrentIndex(ss.alignmentH);
    QPalette p;
    p.setColor(QPalette::Base,ss.color);
    ui->graphicsViewTextColor->setPalette(p);
    p.setColor(QPalette::Base,ss.infoColor);
    ui->graphicsViewInfoColor->setPalette(p);
    p.setColor(QPalette::Base,ss.endingColor);
    ui->graphicsViewEndingColor->setPalette(p);
    ui->checkBoxUseBackground->setChecked(ss.useBackground);
    ui->lineEditBackgroundPath->setText(ss.backgroundName);
    //ui->textEditSong->setPlainText(setSongText(ss.songText));
    ui->textEditSong->setPlainText(ss.songText);
    ui->plainTextEditNotes->setPlainText(ss.notes);
}

void EditWidget::setUiItems()
{
    ui->lineEditTitle->setText(editSong.title);
    ui->lineEditMusicBy->setText(editSong.musicBy);
    ui->lineEditWordsBy->setText(editSong.wordsBy);
    ui->lineEditKey->setText(editSong.tune);
    ui->comboBoxCategory->setCurrentIndex(cat_ids.indexOf(editSong.category));
    setSongbook(editSong.songID);
    ui->checkBoxSongSettings->setChecked(editSong.usePrivateSettings);
    ui->groupBoxSettings->setVisible(editSong.usePrivateSettings);
    ui->comboBoxVAlignment->setCurrentIndex(editSong.alignmentV);
    ui->comboBoxHAlignment->setCurrentIndex(editSong.alignmentH);
    updateColor();
    updateInfoColor();
    updateEndingColor();
    ui->checkBoxUseBackground->setChecked(editSong.useBackground);
    ui->lineEditBackgroundPath->setEnabled(editSong.useBackground);
    ui->toolButtonBrowseBackground->setEnabled(editSong.useBackground);
    ui->lineEditBackgroundPath->setText(editSong.backgroundName);
//    ui->textEditSong->setPlainText(setSongText(editSong.songText));
    ui->textEditSong->setPlainText(editSong.songText);
    ui->plainTextEditNotes->setPlainText(editSong.notes);
}

void EditWidget::setSave(){
    newSong = editSong;
    newSong.number = ui->lineEditSongNumber->text().toInt();
    newSong.songbook_id = song_database.getSongbookIdStringFromName(ui->songbook_label->text());
    newSong.songbook_name = ui->songbook_label->text();
    newSong.title = ui->lineEditTitle->text().trimmed();
    newSong.category = cat_ids.at(ui->comboBoxCategory->currentIndex());
    newSong.tune = ui->lineEditKey->text();
    newSong.wordsBy = ui->lineEditWordsBy->text();
    newSong.musicBy = ui->lineEditMusicBy->text();
//    newSong.songText = resetLyric(ui->textEditSong->toPlainText());
    newSong.songText = ui->textEditSong->toPlainText().trimmed();
    newSong.alignmentV = ui->comboBoxVAlignment->currentIndex();
    newSong.alignmentH = ui->comboBoxHAlignment->currentIndex();
    newSong.usePrivateSettings = ui->checkBoxSongSettings->isChecked();
    newSong.useBackground = ui->checkBoxUseBackground->isChecked();
    newSong.backgroundName = ui->lineEditBackgroundPath->text();
    newSong.notes = ui->plainTextEditNotes->toPlainText();
}

QString EditWidget::resetLyric(QString lyric)
{
    QString fSong;
    QStringList lSong = lyric.split("\n");
    int i(1);
    lyric = lSong[0];
    while (i<lSong.size()){
        fSong = lSong[i];
        if(isStanzaTitle(fSong))
        {
            lyric += "@$" + lSong[i];
        }
        else if ((fSong == "") || (fSong == " "))
            ;
        else
        {
            lyric += "@%" + lSong[i];
        }
        ++i;
    }
    return lyric;
}

void EditWidget::setSongbook(int id)
{
    QSqlQuery sq;
    QString songbook, song_num;

    sq.exec("SELECT songbook_id, number FROM Songs WHERE id = "+QString::number(id));
    sq.first();
    songbook = sq.value(0).toString();
    song_num = sq.value(1).toString();
    sq.clear();

    sq.exec("SELECT name FROM Songbooks WHERE id = " + songbook);
    sq.first();
    songbook = sq.value(0).toString();
    sq.clear();

    ui->songbook_label->setText(songbook);
    ui->lineEditSongNumber->setText(song_num);
}

void EditWidget::setEdit(Song sEdit)
{
    song_to_edit_id = sEdit.songID;
    int songInDB_id = isInDatabase(&sEdit);
    if(songInDB_id!=0)
    {
        editSong = sEdit;
        editSong.songID = songInDB_id;
        setUiItems();
        is_new = false;
    }
    else
    {
        sEdit.songbook_id="0";//initialize no to be empty
        addNewSong(sEdit,tr("Cannot find exact match in database"),
                   tr("The exact match of a song you are editing was not found in database.\n"
                      "In order to edit this song, you need to add it to database.\n\n"
                      "Please select a Songbook to which you want to copy this song to:"));
    }
}

void EditWidget::setCopy(Song copy)
{
    addNewSong(copy,tr("Copy to a new Songbook"),tr("Select a Songbook to which you want to copy this song to"));
}

void EditWidget::setNew()
{
    Song new_song;
    resetUiItems();
    new_song.songText = tr("Verse 1\n - words of verse go here\n\nRefrain\n"
                           "- words of Chorus/Refrain\ngo here\n\nVerse 2\n - words of verse go here");
    addNewSong(new_song,tr("Add a new Songbook"),tr("Select a Songbook to which you want to add a song"));
}

void EditWidget::addNewSong(Song song, QString msgNewSongbook, QString msgCaption)
{
    editSong = song;
    if (song.songbook_id.isEmpty())
        ui->textEditSong->setPlainText(song.songText);
    else
        setUiItems();
    is_new = true;
    bool ok;

    QSqlQuery sq;
    QStringList songbook_list;
    songbook_list << msgNewSongbook;
    sq.exec("SELECT id, name FROM Songbooks");
    while (sq.next())
        songbook_list << sq.value(1).toString();

    int current_songbook(0);
    if (!add_to_songbook.isEmpty())
        current_songbook = songbook_list.indexOf(add_to_songbook);
    else
        current_songbook = 0;

    add_to_songbook = QInputDialog::getItem(this,tr("Select Songbook"),msgCaption,
                                            songbook_list,current_songbook,false,&ok);

    if (ok && !add_to_songbook.isEmpty())
    {
        if (add_to_songbook == msgNewSongbook)
        {
            // Add a Songbook to add a new song into
            addSongbook();
        }
        else
        {
            int last = song_database.lastUser(song_database.getSongbookIdStringFromName(add_to_songbook));
            ui->songbook_label->setText(add_to_songbook);
            ui->lineEditSongNumber->setText(QString::number(last));
        }
    }
    else
        close();
}

void EditWidget::addSongbook()
{
    AddSongbookDialog add_sbor;
    add_sbor.setWindowTitle(tr("Add a Songbook"));
    int last(0);
    int ret = add_sbor.exec();
    switch(ret)
    {
    case AddSongbookDialog::Accepted:
        song_database.addSongbook(add_sbor.title,add_sbor.info);
        last = song_database.lastUser(song_database.getSongbookIdStringFromName(add_sbor.title));
        ui->songbook_label->setText(add_sbor.title);
        ui->lineEditSongNumber->setText(QString::number(last));
        add_to_songbook = add_sbor.title;
        break;
    case AddSongbookDialog::Rejected:
        close();
        break;
    }
}

QStringList EditWidget::categories()
{
    QStringList cat;
    cat<<tr("Other")<<tr("Bible Stories")<<tr("Gospel")<<tr("God, His love and greatness")
      <<tr("The Resurrection of Christ")<<tr("Time")<<tr("The second coming of Christ and the judgement")
     <<tr("Children and Family")<<tr("For new converts")<<tr("Spiritual struggle and victory")
    <<tr("Harvest")<<tr("Jesus Christ")<<tr("Love")<<tr("Mother")<<tr("Prayer")<<tr("Youth")
    <<tr("Wedding")<<tr("Sunset / Sunrise")<<tr("Baptism")
    <<tr("New Years")<<tr("Funeral")<<tr("At the ordination")
    <<tr("On the Lord\'s Supper")<<tr("Heavenly abode")<<tr("Instruction and self-test")
    <<tr("Holy Ghost")<<tr("Church")<<tr("Before church meeting")
    <<tr("Last Days")<<tr("Practical life with God")<<tr("At the end of church meeting")
    <<tr("Welcome and farewell")<<tr("The call to work")<<tr("Call to repentance")
    <<tr("Journey of faith, faith and hope")<<tr("Various Christian holidays")
    <<tr("Determination and faithfulness")<<tr("Christmas")<<tr("Following Christ")
    <<tr("The Word of God")<<tr("Salvation")<<tr("Suffering and death of Christ")
    <<tr("Consolation and encouragement")<<tr("Praise and thanksgiving")<<tr("Christian Joy");

    return cat;
}

void EditWidget::retranslateUis()
{
    loadCategories(true);
}

void EditWidget::loadCategories(bool ui_update)
{
    // retrieve current category id
    int cur_cat_id(-1);
    int cur_index = ui->comboBoxCategory->currentIndex();
    if(cur_index>=0)
        cur_cat_id = cat_ids.at(cur_index);

    // get categories
    QStringList cat_list;
    cat_list = categories();

    // create sorting by name and refrance categories id
    QMap<QString,int> cmap;
    for(int i(0); i< cat_list.count(); ++i)
        cmap.insert(cat_list.at(i),i);
    cat_ids.clear();
    cat_list.clear();
    cat_ids.append(cmap.values());
    cat_list.append(cmap.keys());

    if(ui_update)// update ui translations
    {
        for(int i(0); i<= ui->comboBoxCategory->count()-1;++i)
            ui->comboBoxCategory->setItemText(i,cat_list.at(i));

        // reset to selected category
        if(cur_cat_id>=0)
            cur_index = cat_ids.indexOf(cur_cat_id);
        ui->comboBoxCategory->setCurrentIndex(cur_index);
    }
    else if(!ui_update)// initialize
        ui->comboBoxCategory->addItems(cat_list);
}

int EditWidget::isInDatabase(Song *song)
{
    QString s_title(""), s_id("0"), sb_id("0");
    QSqlQuery sq;

    // check if song is part of songbook
    sq.exec("SELECT id FROM Songbooks WHERE name = '" + song->songbook_name + "'");
    while(sq.next())
        sb_id = sq.value(0).toString().trimmed();
    sq.clear();
    if(sb_id == "0")
        return 0; // no such songbook in database

    // get song id
    sq.exec("SELECT id, title from Songs WHERE songbook_id = '" + sb_id +"' AND number = '" + QString::number(song->number) +"'");
    while(sq.next())
    {
        s_id = sq.value(0).toString().trimmed();
        s_title = sq.value(1).toString().trimmed();
    }
    sq.clear();
    if(s_id == "0")
        return 0; // no matching song
    song->songID = s_id.toInt();

    // get song title
    if(s_title!=song->title.trimmed())
        return 0;
    else
        return s_id.toInt();
}

void EditWidget::on_checkBoxSongSettings_toggled(bool checked)
{
    ui->groupBoxSettings->setVisible(checked);
}

void EditWidget::updateColor()
{
    QPalette p;
    p.setColor(QPalette::Base,editSong.color);
    ui->graphicsViewTextColor->setPalette(p);
}

void EditWidget::updateInfoColor()
{
    QPalette p;
    p.setColor(QPalette::Base,editSong.infoColor);
    ui->graphicsViewInfoColor->setPalette(p);
}

void EditWidget::updateEndingColor()
{
    QPalette p;
    p.setColor(QPalette::Base,editSong.endingColor);
    ui->graphicsViewEndingColor->setPalette(p);
}

void EditWidget::on_pushButtonPrint_clicked()
{
    setSave();
    PrintPreviewDialog* p;
    p = new PrintPreviewDialog(this);
    p->setText(newSong);
    p->exec();
}

void EditWidget::on_toolButtonMainColor_clicked()
{
    QColor c(QColorDialog::getColor(editSong.color,this));
    if(c.isValid())
        editSong.color = c;
    updateColor();
}

void EditWidget::on_toolButtonMainFont_clicked()
{
    bool ok;
    QFont font = QFontDialog::getFont(&ok,editSong.font,this);
    if(ok)
        editSong.font = font;
}

void EditWidget::on_toolButtonInfoColor_clicked()
{
    QColor c(QColorDialog::getColor(editSong.infoColor,this));
    if(c.isValid())
        editSong.infoColor = c;
    updateInfoColor();
}

void EditWidget::on_toolButtonFont_clicked()
{
    bool ok;
    QFont font = QFontDialog::getFont(&ok,editSong.infoFont,this);
    if(ok)
        editSong.infoFont = font;
}

void EditWidget::on_toolButtonEndingColor_clicked()
{
    QColor c(QColorDialog::getColor(editSong.endingColor,this));
    if(c.isValid())
        editSong.endingColor = c;
    updateEndingColor();
}

void EditWidget::on_toolButtonEndingFont_clicked()
{
    bool ok;
    QFont font = QFontDialog::getFont(&ok,editSong.endingFont,this);
    if(ok)
        editSong.endingFont = font;
}

void EditWidget::on_checkBoxUseBackground_toggled(bool checked)
{
    ui->lineEditBackgroundPath->setEnabled(checked);
    ui->toolButtonBrowseBackground->setEnabled(checked);
}

void EditWidget::on_toolButtonBrowseBackground_clicked()
{
    QString filename = QFileDialog::getOpenFileName(this, tr("Select an image for the wallpaper"),
                                                    ".", tr("Images(%1)").arg(getSupportedImageFormats()));

    if( !filename.isNull() )
    {
        QPixmap p(filename);
        editSong.background = p;
        QFileInfo fi(filename);
        filename = fi.fileName();
        editSong.backgroundName = filename;
        ui->lineEditBackgroundPath->setText(filename);
    }
}
