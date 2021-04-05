#ifndef DISEASE_EDITOR_H
#define DISEASE_EDITOR_H

#include <QWidget>
#include <QSqlDatabase>
#include <QSqlTableModel>

namespace Ui {
class disease_editor;
}

class disease_editor : public QWidget
{
    Q_OBJECT

public:
    explicit disease_editor(QWidget *parent = nullptr);
    ~disease_editor();

signals:
    void mainEditor();

private slots:
    void on_commandLinkButton_clicked();

    void on_pushButtonBack_clicked();

    void on_pushButtonDel_clicked();

private:
    Ui::disease_editor *ui;
    QSqlTableModel *mModel;
    QSqlDatabase mDatabase;
};

#endif // DISEASE_EDITOR_H
