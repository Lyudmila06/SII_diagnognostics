#include "disease_editor.h"
#include "ui_disease_editor.h"
#include <QSqlTableModel>
#include <QSqlError>
#include <QMessageBox>
#include <QSortFilterProxyModel>
#include <QSqlRecord>

//#define ACCESS "DRIVER={Microsoft Access Driver (*.mdb, *.accdb)};FIL={MS Access};" \
//    "DBQ=C:\\Users\\Lyudmila\\Documents\\SII_diagnostics\\SII_medical_DB.accdb"

disease_editor::disease_editor(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::disease_editor)
{
    ui->setupUi(this);
//    mDatabase = QSqlDatabase::addDatabase("QODBC");
//    mDatabase.setDatabaseName(ACCESS);
//    if (!mDatabase.open()) {
//        QMessageBox::critical(this, "Error", mDatabase.lastError().text());
//        return;
//    }

    mModel = new QSqlTableModel(this);
    mModel->setTable("Diseases");
    mModel->select();
    ui->listView->setModel(mModel);
    ui->listView->setModelColumn(0);

    mModel2 = new QSqlTableModel(this);
    mModel2->setTable("Disease_attributes");
    mModel2->select();

    edit_d = new edit_dialog();
    edit_d->getLabel()->setText("Введите новое наименование заболевания");
    connect(edit_d, &edit_dialog::editorWin, this, &disease_editor::show);
    connect(edit_d, &edit_dialog::editorUpd, this, &disease_editor::update_Models);
    edit_d->setModelTable("Diseases");
}

disease_editor::~disease_editor()
{
    delete ui;
}

void disease_editor::update_Models()
{
    mModel->select();
    mModel2->select();
}

void disease_editor::clean_Lines()
{
    ui->lineEdit->clear();
}

void disease_editor::on_pushButtonBack_clicked()
{
    clean_Lines();
    this->close();
    emit mainEditor();
}

void disease_editor::on_commandLinkButton_clicked()
{
    try {
        if (ui->lineEdit->text().contains(";")) {
            QMessageBox::warning(this, "Error", "Наименование заболевания не должно содержать ';'");
            return;
        }
        if (ui->lineEdit->text().isEmpty()) {
            QMessageBox::warning(this, "Error", "Наименование заболевания не должно быть пустым");
            return;
        }
        if (ui->lineEdit->text() == "Здоров" || ui->lineEdit->text() == "здоров") {
            QMessageBox::warning(this, "Error", "Запрещено использовать зарезервированное слово " + ui->lineEdit->text());
            return;
        }
        QSqlRecord newRecord = mModel->record();
        newRecord.setValue("Наименование", ui->lineEdit->text());
        newRecord.setValue("Наименование", ui->lineEdit->text());
        //mModel->insertRow(mModel->rowCount(QModelIndex()));
        if (!mModel->insertRecord(mModel->rowCount(QModelIndex()),newRecord)) {
            QMessageBox::critical(this, "Error", "Заболевание с таким именем уже существует");
        }
        clean_Lines();
        //ui->lineEdit->text();
    } catch(QSqlError e) {
        QMessageBox::critical(this, "Error", e.text());
    }
}



void disease_editor::on_pushButtonDel_clicked()
{
    QString dis = mModel->record(ui->listView->currentIndex().row()).value(0).toString();
    mModel->removeRow(ui->listView->currentIndex().row());
    mModel->select();
    if (!query.exec("DELETE FROM Disease_attributes WHERE Заболевание = '" + dis + "'")) {
        QMessageBox::warning(this, "Error", query.lastError().text());
    } else
    QMessageBox::information(this, "Success", "Заболевание " + dis + " успешно удалено");
}

void disease_editor::on_pushButtonEdit_clicked()
{
    if (!ui->listView->selectionModel()->selectedIndexes().isEmpty()){
        edit_d->setPrevName(mModel->record(ui->listView->currentIndex().row()).value(0).toString());
        edit_d->updateModels();
        edit_d->show();
        edit_d->getLineEdit()->setText(mModel->record(ui->listView->currentIndex().row()).value(0).toString());
        this->close();
    }
}

void disease_editor::on_listView_doubleClicked(const QModelIndex &index)
{
    edit_d->setPrevName(mModel->record(ui->listView->currentIndex().row()).value(0).toString());
    edit_d->updateModels();
    edit_d->show();
    edit_d->getLineEdit()->setText(mModel->record(ui->listView->currentIndex().row()).value(0).toString());
    this->close();
}
