#ifndef ATTRIBUTE_EDITOR_H
#define ATTRIBUTE_EDITOR_H

#include <QWidget>
#include <QSqlDatabase>
#include <QSqlTableModel>

namespace Ui {
class attribute_editor;
}

class QSqlTableModel;

class attribute_editor : public QWidget
{
    Q_OBJECT

public:
    explicit attribute_editor(QWidget *parent = nullptr);
    ~attribute_editor();

signals:
    void mainEditor();

private slots:
    void on_pushButtonBack_clicked();

    void on_commandLinkButton_clicked();


    void on_pushButtonDel_clicked();

    //void on_pushButtonOK_clicked();

private:
    Ui::attribute_editor *ui;
    QSqlTableModel *mModel;
    QSqlDatabase mDatabase;
};

#endif // ATTRIBUTE_EDITOR_H
