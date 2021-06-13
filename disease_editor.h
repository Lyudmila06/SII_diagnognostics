#ifndef DISEASE_EDITOR_H
#define DISEASE_EDITOR_H

#include "edit_dialog.h"

#include <QWidget>
#include <QSqlDatabase>
#include <QSqlTableModel>
#include <QSqlQuery>

namespace Ui {
class disease_editor;
}

class disease_editor : public QWidget
{
    Q_OBJECT

public:
    explicit disease_editor(QWidget *parent = nullptr);
    void update_Models();
    void clean_Lines();
    ~disease_editor();

signals:
    void mainEditor();

private slots:
    void on_commandLinkButton_clicked();

    void on_pushButtonBack_clicked();

    void on_pushButtonDel_clicked();

    void on_pushButtonEdit_clicked();

    void on_listView_doubleClicked(const QModelIndex &index);

private:
    Ui::disease_editor *ui;
    QSqlTableModel *mModel, *mModel2;
    QSqlDatabase mDatabase;
    QSqlQuery query;
    edit_dialog *edit_d;
};

#endif // DISEASE_EDITOR_H
