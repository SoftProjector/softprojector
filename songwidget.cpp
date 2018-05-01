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

#include <QDebug>
#include "songwidget.hpp"
#include "ui_songwidget.h"

SongWidget::SongWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SongWidget)
{
    ui->setupUi(this);

    songs_model = new SongsModel;
    proxy_model = new SongProxyModel(this);
    proxy_model->setSourceModel(songs_model);
    proxy_model->setDynamicSortFilter(true);
    ui->songs_view->setModel(proxy_model);
    connect(ui->songs_view->selectionModel(), SIGNAL(currentRowChanged(const QModelIndex&, const QModelIndex&)),
            this, SLOT(songsViewRowChanged(const QModelIndex&, const QModelIndex&)));

    // Decrease the row height:
    ui->songs_view->resizeRowsToContents();

    // Modify the column widths:
    ui->songs_view->setColumnWidth(0, 0);//Category
    ui->songs_view->setColumnHidden(0, true); // Hide category
    ui->songs_view->resizeColumnToContents(1); // Song Number
    ui->songs_view->setColumnWidth(2, 150);//Song Title
    ui->songs_view->resizeColumnToContents(3); // Songbook
    ui->songs_view->setColumnWidth(4, 50);//Tune
    
    proxy_model->setSongbookFilter("ALL");
    proxy_model->setCategoryFilter(-1);
    loadSongbooks();
    loadCategories(false);

    isSpinboxEditing = false;

    // set highligher
    highlight = new HighlighterDelegate(ui->listPreview);
    ui->listWidgetDummy->setVisible(false);
}

SongWidget::~SongWidget()
{
    delete ui;
    delete songs_model;
}

void SongWidget::songsViewRowChanged(const QModelIndex &current, const QModelIndex &previous)
{
    if( current.isValid() )
    {
        // Called when a new song is selected in the songs table
        int row = proxy_model->mapToSource(current).row();
        Song song = songs_model->getSong(row);
        sendToPreview(song);
        isSongFromSchelude = false;
    }
    updateButtonStates();
}

void SongWidget::updateButtonStates()
{
    bool enable_live;

    // focus in songs table
    if( proxy_model->rowCount() == 0 )
        enable_live = false;
    else
        enable_live = ui->songs_view->currentIndex().isValid();

    ui->btnLive->setEnabled(enable_live);
}

void SongWidget::changeEvent(QEvent *e)
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

void SongWidget::loadSongbooks()
{
    QSqlQuery sq;
    QStringList sbor;
    sq.exec("SELECT id, name FROM Songbooks");
    while (sq.next())
    {
        songbookList << sq.value(0).toString();
        sbor << sq.value(1).toString();
    }
    ui->songbook_menu->addItem(tr("All songbooks"));
    ui->songbook_menu->addItems(sbor);
    allSongs = song_database.getSongs();
    songs_model->setSongs(allSongs);

    // Hide song search items
    ui->comboBoxSearchType->setVisible(false);
    if(ui->pushButtonClearResults->isVisible())
        ui->listPreview->setItemDelegate(ui->listWidgetDummy->itemDelegate());
    ui->pushButtonClearResults->setVisible(false);
    ui->labelSearchType->setText(tr("Filter Type:"));
    ui->labelFilter->setText(tr("Filter:"));

    // update the song spin box and redraw the table:
    on_songbook_menu_currentIndexChanged( ui->songbook_menu->currentIndex() );
}

Song SongWidget::currentSong()
{
    // Returns the selected song
    QModelIndex current_index;
    int current_row;
    current_index = proxy_model->mapToSource(ui->songs_view->currentIndex());
    current_row = current_index.row();

    Song return_song;
    if(current_row>=0)
        return_song = songs_model->getSong(current_row);
    return return_song;
}

void SongWidget::selectMatchingSong(QString text)
{
    bool startonly = (ui->comboBoxFilterType->currentIndex() == 1);
    // Look for a song matching <text>. Select it and scroll to show it.
    for (int i = 0; i < songs_model->song_list.size(); i++)
    {
        QString s = songs_model->song_list.at(i).title;
        bool matches;
        if( startonly )
            matches = s.startsWith(text);
        else
            matches = s.contains(text);

        if( matches )
        {
            // Select the row <i>:
            ui->songs_view->selectRow(i);
            // Scroll the songs table to the row <i>:
            ui->songs_view->scrollTo( songs_model->index(i, 0) );
            return;
        }
    }
}

void SongWidget::sendToPreview(Song song)
{
    QStringList song_list = song.getSongTextList();
    ui->listPreview->clear();
    ui->listPreview->addItems(song_list);
    ui->listPreview->setCurrentRow(0);
    ui->preview_label->setText(song.title);
    if(song.notes.isEmpty())
        ui->label_notes->setVisible(false);
    else
    {
        ui->label_notes->setText(QString("%1\n%2").arg(tr("Notes:","Notes to songs")).arg(song.notes));
        ui->label_notes->setVisible(true);
    }
    preview_song = song;
}

void SongWidget::sendToPreviewFromSchedule(Song &song)
{
    ui->songs_view->clearSelection();
    isSongFromSchelude = true;
    sendToPreview(song);
}

void SongWidget::sendToProjector(Song song, int row)
{
    // Display the specified song text in the right-most column of softProjector:
    emit sendSong(song, row);

    // Add a count to a song
    counter.addSongCount(song);
}

void SongWidget::on_songbook_menu_currentIndexChanged(int index)
{
    // Called when a different songbook is selected from the pull-down menu

    songs_model->emitLayoutAboutToBeChanged(); //prepeare to chage layout
    if( index == 0 )
    {
        proxy_model->setSongbookFilter("ALL");
        ui->song_num_spinbox->setEnabled(false);
    }
    else
    {
        QString songbook_name = ui->songbook_menu->currentText();
        proxy_model->setSongbookFilter(songbook_name);
        ui->song_num_spinbox->setEnabled(true);
    }

    updateButtonStates();

    // Sort by song number at initial load, and with songbook change
    proxy_model->sort(1);

    songs_model->emitLayoutChanged(); // forces the view to redraw
}

void SongWidget::on_song_num_spinbox_valueChanged(int value)
{
    // checks if spinbox just got into eding mode, it yes, then it reset searchbox and sorts song table view
    if (!isSpinboxEditing)
    {
        isSpinboxEditing = true;
        ui->lineEditSearch->clear();
        on_lineEditSearch_textEdited("");

        // Sort by song number
        proxy_model->sort(1);
    }

    //int max_num = 0;
    // Look for a song with number <value>. Select it and scroll to show it.
    for (int i = 0; i < songs_model->song_list.size(); i++)
    {
        Song s = songs_model->song_list.at(i);
        if( s.number == value && s.songbook_name == ui->songbook_menu->currentText() )
        {
            // Found a song with this song number
            QModelIndex source_index = songs_model->index(i, 0);
            if( proxy_model->filterAcceptsRow(source_index.row(), source_index) )
            {
                // If this row is visible
                QModelIndex proxy_index = proxy_model->mapFromSource(source_index);

                // Select the row <i>:
                ui->songs_view->selectRow(proxy_index.row());
                // Scroll the songs table to the row <i>:
                ui->songs_view->scrollTo(proxy_index);
            }
            else
            {
                // This song is filtered out using text filter, so can't select
                // it in the table. Just show it:
                sendToPreview(s);
                isSongFromSchelude = false;
            }
            return;
        }
    }

    QMessageBox mb(this);
    mb.setText(tr("Could not find song with number ") + QString::number(value) );
    mb.setWindowTitle(tr("No such song"));
    mb.setIcon(QMessageBox::Warning);
    mb.exec();
}

void SongWidget::on_song_num_spinbox_editingFinished()
{
    // Called when the user presses enter after editing the song number
    // At this point, the song is already selected in the songs table
    // Resets spin box to non eding mode
    isSpinboxEditing = false;
}

void SongWidget::on_btnLive_clicked()
{
    sendToProjector(preview_song, ui->listPreview->currentRow()); // Send current selected
}

void SongWidget::on_lineEditSearch_textEdited(QString text)
{
    // Check if full-text search is in progress
    // If no full-text search is in progress, then filter
    if(!ui->pushButtonClearResults->isVisible())
    {

        // If search text is numeric, sort by the number, else sort by title
        bool ok;
        text.toInt(&ok);
        if(ok)
        {
            proxy_model->sort(1);
        }
        else
        {
            proxy_model->sort(2);
        }

        // These two options are mutually exclusive:
        bool match_beginning = (ui->comboBoxFilterType->currentIndex() == 1);
        bool exact_match = (ui->comboBoxFilterType->currentIndex() == 2);

        songs_model->emitLayoutAboutToBeChanged(); // prepares view to be redrawn
        proxy_model->setFilterString(text, match_beginning, exact_match);
        songs_model->emitLayoutChanged(); // forces the view to redraw

        // Select the first row that matches the new filter:
        ui->songs_view->selectRow(0);
        ui->songs_view->scrollToTop();

        // Load Preview on song changes
        int row = proxy_model->mapToSource(ui->songs_view->currentIndex()).row();
        if( row>=0)
        {
            Song song = songs_model->getSong(row);
            sendToPreview(song);
            isSongFromSchelude = false;
        }
    }
    updateButtonStates();
}

Song SongWidget::getSongToEdit()
{
    isScheduleSongEdited = isSongFromSchelude;
    return preview_song;
}

void SongWidget::on_songs_view_doubleClicked(QModelIndex index)
{
    // Called when a song is double-clicked
    int row = proxy_model->mapToSource(index).row();
    Song song = songs_model->getSong(row);

    emit addToSchedule(song);
    sendToPreview(song);
    isSongFromSchelude = false;
}

void SongWidget::on_songs_view_clicked(QModelIndex index)
{
    // This method is implemented for the case where the use clicks
    // in the playlist table without changing the previous selection.
    Song song = songs_model->getSong(proxy_model->mapToSource(index));
    sendToPreview(song);
    isSongFromSchelude = false;
    updateButtonStates();
}

void SongWidget::on_listPreview_doubleClicked(QModelIndex index)
{
    sendToProjector(preview_song, index.row());
}

void SongWidget::updateSongbooks()
{
    emit setWaitCursor();
    // Reload the songbook and reselect the one that used to be selected
    // if it's still available, otherwise show all songbooks

    QString current_songbook = ui->songbook_menu->currentText();
    QString item0 = ui->songbook_menu->itemText(0);
    ui->songbook_menu->clear();
    loadSongbooks();

    int new_index = ui->songbook_menu->findText(current_songbook);
    if( new_index == -1 )
        new_index = 0; // All songbooks

    ui->songbook_menu->setCurrentIndex(new_index);
    emit setArrowCursor();
}

void SongWidget::updateSongFromDatabase(int songid, int initial_sid)
{
    songs_model->updateSongFromDatabase(songid, initial_sid);

    // Update in allSongs list
    for(int i(0);i<allSongs.count();++i)
    {

        if(allSongs.at(i).songID == songid)
        {
            Song s;
            s = allSongs.at(i);
            s.readData();
            allSongs.removeAt(i);
            allSongs.append(s);
            break;
        }

    }

    // Update the preview table:
    sendToPreview( currentSong() );
}

void SongWidget::deleteSong()
{
    song_database.deleteSong(currentSong().songID);
    preview_song = Song();
    ui->preview_label->clear();
    ui->listPreview->clear();
    int row = ui->songs_view->currentIndex().row();
    proxy_model->removeRow(row);
}

void SongWidget::addNewSong(Song song, int initial_sid)
{

    songs_model->addSong(song);
    allSongs.append(song);

    // Get added song row number to select it.
    // If added song is not found list, no selection will be done
    int row = 0;
    for(int i(0);i<proxy_model->rowCount();++i)
    {
        row = i;
        if(song.title == songs_model->getSong(
                    proxy_model->mapToSource(
                        ui->songs_view->indexAt(QPoint(0,i*20)))).title)
        {
            ui->songs_view->selectRow(row);
            break;
        }
    }

    sendToPreview(song);
}

void SongWidget::filterModeChanged()
{
    // Re-apply the filter:
    QString new_text = ui->lineEditSearch->text();
    on_lineEditSearch_textEdited(new_text);
}

void SongWidget::on_comboBoxFilterType_currentIndexChanged(int index)
{
    filterModeChanged();
}

QByteArray SongWidget::getSplitterState()
{
    return ui->splitter->saveState();
}

void SongWidget::setSplitterState(QByteArray& state)
{
    ui->splitter->restoreState(state);
}

void SongWidget::retranslateUis()
{
    ui->songbook_menu->setItemText(0,tr("All songbooks"));
    loadCategories(true);
}

bool SongWidget::isSongSelected()
{
    if(ui->songs_view->selectionModel()->selectedRows().count() > 0)
        return true;
    else
        return false;
}

void SongWidget::loadCategories(bool ui_update)
{
    EditWidget e;

    // retrieve current category id
    int cur_cat_id(-1);
    int cur_index = ui->comboBoxCategory->currentIndex();
    if(cur_index>0)
        cur_cat_id = cat_ids.at(cur_index-1);

    // get categories
    QStringList cat_list;
    cat_list = e.categories();

    // create sorting by name and refrance categories id
    QMap<QString,int> cmap;
    for(int i(0); i< cat_list.count(); ++i)
        cmap.insert(cat_list.at(i),i);
    cat_ids.clear();
    cat_list.clear();
    cat_ids.append(cmap.values());
    cat_list.append(cmap.keys());

    if(ui_update) // update categories to retranslate
    {
        ui->comboBoxCategory->setItemText(0,tr("All song categories"));
        for(int i(1); i <= ui->comboBoxCategory->count()-1;++i)
        {
            ui->comboBoxCategory->setItemText(i,cat_list.at(i-1));
        }

        // reset to selected category
        if(cur_cat_id==-1)
            cur_index=0;
        else
            cur_index= cat_ids.indexOf(cur_cat_id)+1;
        ui->comboBoxCategory->setCurrentIndex(cur_index);
    }
    else if(!ui_update) // initialize categories
    {
        ui->comboBoxCategory->addItem(tr("All song categories"));
        ui->comboBoxCategory->addItems(cat_list);
    }
}

void SongWidget::on_comboBoxCategory_currentIndexChanged(int index)
{
    if(index!=-1)
    {
        songs_model->emitLayoutAboutToBeChanged(); //prepeare to chage layout
        if(index==0)
            proxy_model->setCategoryFilter(index-1);
        else
            proxy_model->setCategoryFilter(cat_ids.at(index-1));
        songs_model->emitLayoutChanged();
    }
}

void SongWidget::on_pushButtonSearch_clicked()
{
    QString search_text = ui->lineEditSearch->text();
    search_text = clean(search_text); // remove all none alphanumeric charecters
    QList<Song> search_results;
    int type = ui->comboBoxSearchType->currentIndex();

    // Make sure that there is some text to do a search for, if none, then return
    if(search_text.count()<1)
    {
        ui->lineEditSearch->clear();
        ui->lineEditSearch->setPlaceholderText(tr("Please enter search text"));
        return;
    }

    // set filter
    QRegExp rx;
    rx.setCaseSensitivity(Qt::CaseInsensitive);
    search_text.replace(" ","\\W*");
    if(type == 1 )
        // Search whole word exsact phrase only
        rx.setPattern("\\b"+search_text+"\\b");
    else if(type == 2) // contains all words
        // Search begining of every line
        rx.setPattern("\n"+search_text);
    else if(type == 3 || type == 4)
    {
        // Search for any of the search words
        search_text.replace("\\W*","|");
        rx.setPattern("\\b("+search_text+")\\b");
    }
    else
        // Search text phrase
        rx.setPattern(search_text);

    // perform search
    for(int i(0);i<allSongs.count();++i)
    {
        if(type == 4)
        {
            QStringList stl = search_text.split("|");
            bool hasAll = false;
            for(int j(0);j<stl.count();++j)
            {
                hasAll = allSongs.at(i).songText.contains(QRegExp("\\b"+stl.at(j)+"\\b",Qt::CaseInsensitive));
                if(!hasAll)
                    break;
            }
            if(hasAll)
                search_results.append(allSongs.at(i));
        }
        else
        {
            if(allSongs.at(i).songText.contains(rx))
                search_results.append(allSongs.at(i));
        }
    }

    // Hide song filter items and show search items
    //ui->pushButtonSearch->setText(tr("Search"));
    ui->comboBoxFilterType->setVisible(false);
    ui->labelSearchType->setText(tr("Search Type:"));
    ui->labelFilter->setText(tr("Search:"));
    ui->comboBoxSearchType->setVisible(true);
    ui->pushButtonClearResults->setVisible(true);

    // set new songs_model with search relusts
    songs_model->setSongs(search_results);

    // setup higligher
    ui->listPreview->setItemDelegate(highlight);
    if(type == 2)
        highlight->highlighter->setHighlightText(search_text);
    else
        highlight->highlighter->setHighlightText(rx.pattern());
    // reset filter on song table to show all results
    songs_model->emitLayoutAboutToBeChanged(); // prepares view to be redrawn
    proxy_model->setFilterString("", false, false);
    songs_model->emitLayoutChanged(); // forces the view to redraw

    ui->songs_view->selectRow(0);
    ui->songs_view->scrollToTop();

    int row = proxy_model->mapToSource(ui->songs_view->currentIndex()).row();
    if( row>=0)
    {
        sendToPreview(songs_model->getSong(row));
        isSongFromSchelude = false;
    }
    else
    {
        Song s;
        sendToPreview(s);
    }

    updateButtonStates();
}

void SongWidget::on_pushButtonClearResults_clicked()
{
    // try to reset highliting settings on preview list
    ui->listPreview->setItemDelegate(ui->listWidgetDummy->itemDelegate());

    ui->lineEditSearch->setPlaceholderText("");

    // Hide song filter items and show search items
    //ui->pushButtonSearch->setText("");
    ui->comboBoxFilterType->setVisible(true);
    ui->comboBoxSearchType->setVisible(false);
    ui->pushButtonClearResults->setVisible(false);
    ui->labelSearchType->setText(tr("Filter Type:"));
    ui->labelFilter->setText(tr("Filter:"));
    songs_model->setSongs(allSongs);
    ui->lineEditSearch->clear();
    Song s;
    sendToPreview(s);
    updateButtonStates();
}

void SongWidget::setSearchActive()
{
    ui->lineEditSearch->setFocus();
    ui->lineEditSearch->selectAll();
}
