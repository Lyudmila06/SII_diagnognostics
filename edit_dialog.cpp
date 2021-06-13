#include "edit_dialog.h"
#include "ui_edit_dialog.h"

#include <QMessageBox>
#include <QSqlError>
#include <QSqlQuery>
#include <QSqlRecord>

edit_dialog::edit_dialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::edit_dialog)
{
    ui->setupUi(this);
    mModel = new QSqlTableModel(this);
}

edit_dialog::~edit_dialog()
{
    delete ui;
}

QLabel* edit_dialog::getLabel()
{
    return ui->label;
}

QLineEdit* edit_dialog::getLineEdit()
{
    return ui->lineEdit;
}

void edit_dialog::setModelTable(QString m)
{
    mModel = new QSqlTableModel(this);
    mModel->setTable(m);
}

void edit_dialog::updateModels()
{
    mModel->select();
}

void edit_dialog::setPrevName(QString name)
{
    prev_name = name;
}

void edit_dialog::changeNameinTables(QString new_n)
{
    try {
        QSqlQuery query;
        long count = 0;
        for (; count < mModel->rowCount(); count++) {
            if (mModel->record(count).value(0).toString() == prev_name) {
                //QMessageBox::warning(this, "Success", "Признак " + prev_name + " найден, строка " + QString::number(count));
                break;
            }
        }
        if(count == mModel->rowCount()){
            QMessageBox::warning(this, "Error", "Записи с наименованием " + prev_name + " не существует");
            return;
        }
        QSqlRecord newRecord = mModel->record(count);
        newRecord.setValue("Наименование"
, new_n);
        mModel->setRecord(count, newRecord);

//        query.exec("SELECT * FROM " + mModel->tableName() + "WHERE Наименование = '" + prev_name + "'");
//        if(!query.next()){
//            QMessageBox::warning(this, "Error", "Признака " + prev_name + " не существует");
//            return;
//        }
        mModel->select();

        //Изменяем наименование в таблице Disease_attributes
        QString table_type;
        if (mModel->tableName() == "Diseases")
            table_type = "Заболевание";
        else if (mModel->tableName() == "Attributes")
            table_type = "Признак";
        if (!query.exec("UPDATE Disease_attributes SET " + table_type + " = '" + new_n + "' WHERE " + table_type + " = '" + prev_name + "'"))
            QMessageBox::critical(this, "Error", query.lastError().text());
        QString ss = "UPDATE Disease_attributes SET " + table_type + " = '" + new_n + "' WHERE " + table_type + " = '" + prev_name + "'";

        //Изменяем название в таблице Disease (если переименовываем признак)
        if (table_type == "Признак") {
            QString s1 = "UPDATE Diseases SET [Клиническая картина] = REPLACE([Клиническая картина], ';" + prev_name + ";', ';" + new_n + ";') WHERE [Клиническая картина] LIKE '%"+prev_name+"%'";
            QString s2 = "UPDATE Diseases SET [Клиническая картина] = '" + new_n
                    + ";' + [Клиническая картина] WHERE [Клиническая картина] LIKE '" + prev_name
                    + ";%'";
            QString s3 = "UPDATE Diseases SET [Клиническая картина] = REPLACE([Клиническая картина], ';" + prev_name + ";', ';') WHERE [Клиническая картина] LIKE '%"+prev_name+"%'";
            if (!query.exec(s1)) {
                QMessageBox::critical(this, "Error", query.lastError().text());
            }
            if (!query.exec(s2)) {
                QMessageBox::critical(this, "Error", query.lastError().text());
            }
            if(!query.exec(s3)) {
                QMessageBox::critical(this, "Error", query.lastError().text());
            }

        }

        setPrevName(new_n);
    } catch(QSqlError e) {
        QMessageBox::critical(this, "Error", e.text());
    }
}

void edit_dialog::on_pushButtonSave_clicked()
{
    try {
        QString new_name = ui->lineEdit->text();
        if(prev_name == new_name) {
            QMessageBox::information(this, "Warning", "Новое наименование соответствует старому");
            return;
        }
        if (new_name.contains(";")) {
            QMessageBox::warning(this, "Error", "Наименование не должно содержать ';'");
            return;
        }
        if (new_name.isEmpty()) {
            QMessageBox::warning(this, "Error", "Наименование не должно быть пустым");
            return;
        }

        QSqlQuery query;
        query.exec("SELECT COUNT(*) FROM " + mModel->tableName() + "WHERE Наименование = '" + new_name + "'");
        query.next();
        if (query.value(0).toInt() != 0) {
            QMessageBox::warning(this, "Error", "Запись с таким именем уже существует");
            return;
        }
        changeNameinTables(new_name);
        emit editorUpd();
        emit editorWin();
        this->close();

    } catch(QSqlError e) {
        QMessageBox::critical(this, "Error", e.text());
    }
}

void edit_dialog::on_pushButtonClose_clicked()
{
    emit editorUpd();
    emit editorWin();
    this->close();
}
