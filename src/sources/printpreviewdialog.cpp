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

#include "printpreviewdialog.hpp"
#include "ui_printpreviewdialog.h"

PrintPreviewDialog::PrintPreviewDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::PrintPreviewDialog)
{
    ui->setupUi(this);
    printer = new QPrinter;
    ui->fontComboBox->setFont(ui->textEdit->font());
    ui->spinBoxFontSize->setValue(ui->textEdit->font().pointSize());

    // set default margins
    on_comboBox_currentIndexChanged(0);
    ui->doubleSpinBoxLeft->setValue(0.5);
    ui->doubleSpinBoxTop->setValue(0.5);
    ui->doubleSpinBoxRight->setValue(0.5);
    ui->doubleSpinBoxBottom->setValue(0.5);
    updateMargins();

    // set default font
    QFont f;
    f.fromString("Arial,12,-1,5,50,0,0,0,0,0");
    ui->fontComboBox->setCurrentFont(f);
}

PrintPreviewDialog::~PrintPreviewDialog()
{
    delete ui;
}

void PrintPreviewDialog::on_fontComboBox_currentFontChanged(const QFont &f)
{
    QFont font = f;
    font.setPointSize(ui->spinBoxFontSize->value());
    ui->textEdit->setFont(font);
}

void PrintPreviewDialog::on_spinBoxFontSize_valueChanged(int arg1)
{
    QFont font = ui->textEdit->font();
    font.setPointSize(arg1);
    ui->textEdit->setFont(font);
}

void PrintPreviewDialog::setText(Song song)
// This will prepare print text edit for current song
{
    QString s;
    s = QString("%1 %2\n%3\n").arg(song.songbook_name).arg(song.number).arg(song.title);
    // Do not print Tune, WordsBy, and MusicBy if they are empty
    if(!song.tune.isEmpty())
        s+= tr("Tune: %1\n").arg(song.tune);
    if(!song.wordsBy.isEmpty() && !song.musicBy.isEmpty())
        s+= tr("Words By: %1\tMusic By: %2\n\n").arg(song.wordsBy).arg(song.musicBy);
    else if(!song.wordsBy.isEmpty() && song.musicBy.isEmpty())
        s+= tr("Words By: %1\n\n").arg(song.wordsBy);
    else if(song.wordsBy.isEmpty() && !song.musicBy.isEmpty())
        s+= tr("Music By: %1\n\n").arg(song.musicBy);
    else
        s+= "\n";
//    song.songText=song.songText.split("@$").join("\n\n");
//    song.songText=song.songText.split("@%").join("\n");
    s += song.songText;

    //Check for notes
    if(!song.notes.isEmpty())
        s += tr("\n\nNotes:\n%1").arg(song.notes);

    ui->textEdit->setText(s);
    ui->spinBoxFontSize->setValue(14);// default font size for Songs
}

void PrintPreviewDialog::setText(QString bible, QString book, int chapter)
// This will prepare print text edit for Current chapter
{
    QString s;

    // get proper bible id if operator bible == "same"
    QStringList bb = bible.split(",");
    if(bb.at(0) == "same")
        bible = bb.at(1); // primary bible
    else
        bible = bb.at(0); // operator bible

    Bible b;
    b.setBiblesId(bible);
    b.loadOperatorBible();

    // get bible name instead of id
    bible = b.getBibleName();

    QStringList clist;
    // Get chapter with correct book id
    for(int i(0);i<b.books.count();++i)
    {
        if(b.books.at(i).book == book)
        {
            clist = b.getChapter(b.books.at(i).bookId.toInt(),chapter);
            break;
        }
    }

    s = QString("%1\n%2 %3\n\n").arg(bible).arg(book).arg(chapter);
    s += clist.join("\n");

    ui->textEdit->setText(s);
    ui->spinBoxFontSize->setValue(11);// default font size for Bible chapter
}

void PrintPreviewDialog::setText(Announcement announce)
{
    QString s;
    s = tr("Announcements: %1\n\n").arg(announce.title);
    s += announce.text;

    ui->textEdit->setText(s);
    ui->spinBoxFontSize->setValue(14);// default font size for Announcements
}

void PrintPreviewDialog::setSchedule(QString scheduleName, const QList<Schedule> &schedule, bool printDetail)
{
    QFileInfo fi(scheduleName);
    scheduleName = fi.fileName();
    scheduleName.remove(".spsc");
    QString s;

    // start
    if(scheduleName.isEmpty())
        s = "";
    else
        s = tr("SoftProject Schedule: ") + scheduleName + "\n\n";

    if(printDetail)
    {

    }
    else
    {
        foreach(const Schedule &si,schedule)
        {
            s += QString("%1: %2\n").arg(si.stype).arg(si.name);
        }
    }

    ui->textEdit->setText(s);
    ui->spinBoxFontSize->setValue(11); // default font size for spftProjector Schedule
}

void PrintPreviewDialog::on_pushButtonPDF_clicked()
{
    QString filepath = QFileDialog::getSaveFileName(this,tr("Save PDF as"),".","Portable Document Format(*.pdf)");
    if(filepath.isEmpty())
        return;
    printer->setOutputFormat(QPrinter::PdfFormat);
    printer->setOutputFileName(filepath);

    ui->textEdit->document()->print(printer);
}

void PrintPreviewDialog::on_pushButtonPrint_clicked()
{
    printer->setOutputFormat(QPrinter::NativeFormat);
    printer->setOutputFileName("");

    QPrintDialog *dlg = new QPrintDialog(printer, this);
    if (dlg->exec() != QDialog::Accepted)
        return;

    ui->textEdit->document()->print(printer);
}
void PrintPreviewDialog::on_comboBox_currentIndexChanged(int index)
{

//}

//void PrintPreviewDialog::on_comboBox_currentIndexChanged(const QString &arg1)
//{
    // set margin boxes
    double l(0),t(0),r(0),b(0);

    if(index == 0)
        printer->getPageMargins(&l,&t,&r,&b,QPrinter::Inch);
    else if(index == 1)
        printer->getPageMargins(&l,&t,&r,&b,QPrinter::Millimeter);
    else if(index == 2)
        printer->getPageMargins(&l,&t,&r,&b,QPrinter::DevicePixel);
    else if(index == 3)
        printer->getPageMargins(&l,&t,&r,&b,QPrinter::Point);

    if(index == 0)
    {
        ui->doubleSpinBoxLeft->setDecimals(2);
        ui->doubleSpinBoxLeft->setSingleStep(0.01);
        ui->doubleSpinBoxTop->setDecimals(2);
        ui->doubleSpinBoxTop->setSingleStep(0.01);
        ui->doubleSpinBoxRight->setDecimals(2);
        ui->doubleSpinBoxRight->setSingleStep(0.01);
        ui->doubleSpinBoxBottom->setDecimals(2);
        ui->doubleSpinBoxBottom->setSingleStep(0.01);
    }
    else
    {
        ui->doubleSpinBoxLeft->setDecimals(0);
        ui->doubleSpinBoxLeft->setSingleStep(1.0);
        ui->doubleSpinBoxTop->setDecimals(0);
        ui->doubleSpinBoxTop->setSingleStep(1.0);
        ui->doubleSpinBoxRight->setDecimals(0);
        ui->doubleSpinBoxRight->setSingleStep(1.0);
        ui->doubleSpinBoxBottom->setDecimals(0);
        ui->doubleSpinBoxBottom->setSingleStep(1.0);
    }

    ui->doubleSpinBoxLeft->setValue(l);
    ui->doubleSpinBoxTop->setValue(t);
    ui->doubleSpinBoxRight->setValue(r);
    ui->doubleSpinBoxBottom->setValue(b);
}

void PrintPreviewDialog::updateMargins()
{
    double l = ui->doubleSpinBoxLeft->value();
    double t = ui->doubleSpinBoxTop->value();
    double r = ui->doubleSpinBoxRight->value();
    double b = ui->doubleSpinBoxBottom->value();

    if(ui->comboBox->currentIndex() == 0)
        printer->setPageMargins(l,t,r,b,QPrinter::Inch);
    else if(ui->comboBox->currentIndex() == 1)
        printer->setPageMargins(l,t,r,b,QPrinter::Millimeter);
    else if(ui->comboBox->currentIndex() == 2)
        printer->setPageMargins(l,t,r,b,QPrinter::DevicePixel);
    else if(ui->comboBox->currentIndex() == 3)
        printer->setPageMargins(l,t,r,b,QPrinter::Point);
}

void PrintPreviewDialog::on_doubleSpinBoxLeft_editingFinished()
{
    updateMargins();
}

void PrintPreviewDialog::on_doubleSpinBoxTop_editingFinished()
{
    updateMargins();
}

void PrintPreviewDialog::on_doubleSpinBoxRight_editingFinished()
{
    updateMargins();
}

void PrintPreviewDialog::on_doubleSpinBoxBottom_editingFinished()
{
    updateMargins();
}

