#ifndef ATTRIBUTE_EDITOR_H
#define ATTRIBUTE_EDITOR_H

#include <QWidget>
#include <QSqlDatabase>
#include <QSqlTableModel>
#include <QSqlQuery>
#include <edit_dialog.h>

namespace Ui {
class attribute_editor;
}

class QSqlTableModel;

class attribute_editor : public QWidget
{
    Q_OBJECT

public:
    explicit attribute_editor(QWidget *parent = nullptr);
    void update_Models();
    void clean_Lines();
    ~attribute_editor();

signals:
    void mainEditor();
    void editSignal();

private slots:
    void on_pushButtonBack_clicked();

    void on_commandLinkButton_clicked();


    void on_pushButtonDel_clicked();

    //void on_pushButtonOK_clicked();

    void on_pushButtonEdit_clicked();

    void on_listView_doubleClicked(const QModelIndex &index);

private:
    Ui::attribute_editor *ui;
    QSqlTableModel *mModel, *mModel2, *mModel3;
    QSqlQuery query;
    QSqlDatabase mDatabase;
    edit_dialog *edit_d;
};

#endif // ATTRIBUTE_EDITOR_H
