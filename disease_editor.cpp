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
}

disease_editor::~disease_editor()
{
    delete ui;
}

void disease_editor::on_pushButtonBack_clicked()
{
    this->close();
    emit mainEditor();
}

void disease_editor::on_commandLinkButton_clicked()
{
    QSqlRecord newRecord = mModel->record();
    newRecord.setValue("Наименование", ui->lineEdit->text());
    newRecord.setValue("Наименование", ui->lineEdit->text());
    //mModel->insertRow(mModel->rowCount(QModelIndex()));
    mModel->insertRecord(mModel->rowCount(QModelIndex()),newRecord);
    ui->lineEdit->text();
}



void disease_editor::on_pushButtonDel_clicked()
{
    mModel->removeRow(ui->listView->currentIndex().row());
    mModel->select();
}
