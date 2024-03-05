#include <QFile>
#include <QTextStream>
#include "styleeditor.h"
#include "ui_styleeditor.h"

StyleEditor::StyleEditor(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::StyleEditor)
{
    ui->setupUi(this);
}

StyleEditor::~StyleEditor()
{
    delete ui;
}

void StyleEditor::on_pushButtonApply_clicked()
{
    qApp->setStyleSheet(ui->textEdit->toPlainText());
}

void StyleEditor::on_pushButtonClose_clicked()
{
    close();
}

void StyleEditor::loadStyleSheet(const QString &sheetName)
{
    styleName = sheetName.toLower();
    QFile file(":/styles/styles/" + styleName + ".qss");
    file.open(QFile::ReadOnly);
    QString styleSheet = QString::fromLatin1(file.readAll());
    file.close();

    ui->textEdit->setPlainText(styleSheet);
    qApp->setStyleSheet(styleSheet);
}
