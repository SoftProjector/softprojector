#ifndef STYLEEDITOR_H
#define STYLEEDITOR_H

#include <QDialog>

namespace Ui {
class StyleEditor;
}

class StyleEditor : public QDialog
{
    Q_OBJECT

public:
    explicit StyleEditor(QWidget *parent = nullptr);
    ~StyleEditor();

    void loadStyleSheet(const QString &sheetName);

private slots:
    void on_pushButtonApply_clicked();

    void on_pushButtonClose_clicked();

private:
    Ui::StyleEditor *ui;
    QString styleName;
};

#endif // STYLEEDITOR_H
