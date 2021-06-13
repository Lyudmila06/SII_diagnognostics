#include "attribute_editor.h"
#include "ui_attribute_editor.h"
#include <QSqlTableModel>
#include <QSqlError>
#include <QMessageBox>
#include <QSortFilterProxyModel>
#include <QSqlRecord>
#include <QSqlError>
#include <QException>

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
    mModel->setTable("Attributes");
    mModel->select();
    ui->listView->setModel(mModel);
    ui->listView->setModelColumn(0);

    mModel2 = new QSqlTableModel(this);
    mModel2->setTable("Diseases");
    mModel2->select();

    mModel3 = new QSqlTableModel(this);
    mModel3->setTable("Disease_attributes");
    mModel3->select();

    edit_d = new edit_dialog();
    edit_d->getLabel()->setText("Введите новое наименование признака");
    connect(edit_d, &edit_dialog::editorWin, this, &attribute_editor::show);
    connect(edit_d, &edit_dialog::editorUpd, this, &attribute_editor::update_Models);
    edit_d->setModelTable("Attributes");
}

attribute_editor::~attribute_editor()
{
    delete ui;
}

void attribute_editor::update_Models()
{
    mModel->select();
    mModel2->select();
    mModel3->select();
}

void attribute_editor::clean_Lines()
{
    ui->lineEdit->clear();
}

void attribute_editor::on_pushButtonBack_clicked()
{
    clean_Lines();
    this->close();
    emit mainEditor();
}

void attribute_editor::on_commandLinkButton_clicked()
{
    try {
        if (ui->lineEdit->text().contains(";")) {
            QMessageBox::warning(this, "Error", "Наименование признака не должно содержать ';'");
            return;
        }
        if (ui->lineEdit->text().isEmpty()) {
            QMessageBox::warning(this, "Error", "Наименование признака не должно быть пустым");
            return;
        }
        QSqlRecord newRecord = mModel->record();
        newRecord.setValue("Наименование", ui->lineEdit->text());
        if (!mModel->insertRecord(mModel->rowCount(QModelIndex()),newRecord)) {
            QMessageBox::critical(this, "Error", "Признак с таким именем уже существует");
        }
        //ui->lineEdit->text();
        clean_Lines();
    } catch(QSqlError e) {
        QMessageBox::critical(this, "Error", e.text());
    }
    catch (QException ex) {
        QMessageBox::critical(this, "Error", ex.what());
    }
}


void attribute_editor::on_pushButtonDel_clicked()
{
    QString attr;
    QSqlTableModel *select_model = new QSqlTableModel();
    select_model->setTable("Disease_attributes");
    attr = mModel->record(ui->listView->currentIndex().row()).value(0).toString();
    select_model->setFilter("Признак = '" + mModel->record(ui->listView->currentIndex().row()).value(0).toString() +  "'");
    for (int i = 0; i < select_model->rowCount(); i++) {
        select_model->removeRow(i);
    }
    //query.exec("UPDATE Diseases SET [Клиническая картина] = REPLACE(';" + attr + ";', ';')");
    if (!query.exec("UPDATE Diseases SET [Клиническая картина] = ';' + [Клиническая картина] WHERE [Клиническая картина] LIKE '"
                    + attr + ";%'")) {
        QMessageBox::warning(this, "Error", query.lastError().text());
        return;
    }
    if (!query.exec("UPDATE Diseases SET [Клиническая картина] = REPLACE([Клиническая картина], ';" + attr + ";', ';') WHERE [Клиническая картина] LIKE '%;"+attr+";%'")) {
        QMessageBox::warning(this, "Error", query.lastError().text());
        return;
    }
    if (!query.exec("UPDATE Diseases SET [Клиническая картина] = RIGHT([Клиническая картина], LEN([Клиническая картина])-1) WHERE [Клиническая картина] LIKE ';%'")){
        QMessageBox::warning(this, "Error", query.lastError().text());
        return;
    }
    mModel->removeRow(ui->listView->currentIndex().row());
    mModel->select();
    QMessageBox::information(this, "Success", "Признак " + attr + " успешно удален");
}

//{
//    //ui->listView->editTriggers().QFlags()
//}

void attribute_editor::on_pushButtonEdit_clicked()
{
    if (!ui->listView->selectionModel()->selectedIndexes().isEmpty()){
        edit_d->setPrevName(mModel->record(ui->listView->currentIndex().row()).value(0).toString());
        edit_d->updateModels();
        edit_d->show();
        edit_d->getLineEdit()->setText(mModel->record(ui->listView->currentIndex().row()).value(0).toString());
        this->close();
    }

    //ui->listView->openPersistentEditor(ui->listView->currentIndex());
    //query.exec("SELECT")
}

void attribute_editor::on_listView_doubleClicked(const QModelIndex &index)
{
    edit_d->setPrevName(mModel->record(ui->listView->currentIndex().row()).value(0).toString());
    edit_d->updateModels();
    edit_d->show();
    edit_d->getLineEdit()->setText(mModel->record(ui->listView->currentIndex().row()).value(0).toString());
    this->close();
}
