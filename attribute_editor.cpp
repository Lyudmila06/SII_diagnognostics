#include "attribute_editor.h"
#include "ui_attribute_editor.h"
#include <QSqlTableModel>
#include <QSqlError>
#include <QMessageBox>
#include <QSortFilterProxyModel>
#include <QSqlRecord>

//#define ACCESS "DRIVER={Microsoft Access Driver (*.mdb, *.accdb)};FIL={MS Access};DBQ=C:\\Users\\Lyudmila\\Documents\\SII_diagnostics\\SII_medical_DB.accdb"

attribute_editor::attribute_editor(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::attribute_editor)
{
    ui->setupUi(this);
//    mDatabase = QSqlDatabase::addDatabase("QODBC");
//    mDatabase.setDatabaseName(ACCESS);
//    if (!mDatabase.open()) {
//        QMessageBox::critical(this, "Error", mDatabase.lastError().text());
//        return;
//    }

    mModel = new QSqlTableModel(this);
    mModel->setTable("Disease_attributes");
    mModel->select();
    ui->listView->setModel(mModel);
    ui->listView->setModelColumn(0);



}

attribute_editor::~attribute_editor()
{
    delete ui;
}

void attribute_editor::on_pushButtonBack_clicked()
{
    this->close();
    emit mainEditor();
}

void attribute_editor::on_commandLinkButton_clicked()
{
    QSqlRecord newRecord = mModel->record();
    newRecord.setValue("Наименование", ui->lineEdit->text());
    newRecord.setValue("Наименование", ui->lineEdit->text());
    //mModel->insertRow(mModel->rowCount(QModelIndex()));
    mModel->insertRecord(mModel->rowCount(QModelIndex()),newRecord);
    ui->lineEdit->text();
}



void attribute_editor::on_pushButtonDel_clicked()
{
    mModel->removeRow(ui->listView->currentIndex().row());
    mModel->select();
}

//void attribute_editor::on_pushButtonOK_clicked()
//{
//    //ui->listView->editTriggers().QFlags()
//}
