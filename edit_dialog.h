#ifndef EDIT_DIALOG_H
#define EDIT_DIALOG_H

#include <QDialog>
#include <QLabel>
#include <QLineEdit>
#include <QSqlTableModel>

namespace Ui {
class edit_dialog;
}

class edit_dialog : public QDialog
{
    Q_OBJECT

public:
    explicit edit_dialog(QWidget *parent = nullptr);
    QLabel* getLabel();
    QLineEdit* getLineEdit();
    void setModelTable(QString m);
    void setPrevName(QString name);
    void updateModels();
    ~edit_dialog();

signals:
    void editorWin();
    void editorUpd();

private slots:
    void on_pushButtonSave_clicked();

    void on_pushButtonClose_clicked();

private:
    void changeNameinTables(QString new_n);
    Ui::edit_dialog *ui;
    QSqlTableModel *mModel;
    QString prev_name;
};

#endif // EDIT_DIALOG_H
