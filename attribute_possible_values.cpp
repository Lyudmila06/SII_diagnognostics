#include "attribute_possible_values.h"
#include "ui_attribute_possible_values.h"
#include <QMessageBox>
#include <QSqlRecord>

#define logtype "Логический"
#define numtype "Количественный"
#define qualtype "Качественный"

attribute_possible_values::attribute_possible_values(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::attribute_possible_values)
{
    ui->setupUi(this);

    mModel = new QSqlTableModel(this);
    mModel->setTable("Disease_attributes");
    mModel->select();
    ui->comboBox->setModel(mModel);
    ui->comboBox->setModelColumn(0);

    //mModel2->setTable("Desease_attributes");

}

attribute_possible_values::~attribute_possible_values()
{
    delete ui;
}

void attribute_possible_values::update_mModel() {
   this->mModel->select();
}

void attribute_possible_values::on_pushButtonBack_clicked()
{
    this->clean_lineEdits();
    this->close();
    emit mainEditor();
}

void attribute_possible_values::clean_lineEdits() {
    ui->lineEditTrue->setText(nullptr);
    ui->lineEditFalse->setText(nullptr);
    ui->lineEditMax->setText(nullptr);
    ui->lineEditMin->setText(nullptr);
    ui->plainTextEdit->setPlainText(nullptr);

}

void attribute_possible_values::on_pushButtonNext_clicked()
{
    QSqlRecord record = mModel->record(ui->comboBox->currentIndex());

    if (ui->tab_log->isVisible()){
        //mModel2->selectRow(ui->comboBox->currentIndex());
        QString true_val = ui->lineEditTrue->text();
        QString false_val = ui->lineEditFalse->text();
        if(true_val.isEmpty() | false_val.isEmpty()) {
            QMessageBox::critical(this, "Возможные значения", "Не введены возможные значения логического признака");
        } else {
            record.setValue("Тип", "Логический");
            mModel->setRecord(ui->comboBox->currentIndex(), record);
            record.setValue("Возможные значения", true_val+";"+false_val);
            mModel->setRecord(ui->comboBox->currentIndex(), record);
            QMessageBox::information(this, "Success", "Успешно добавлен тип к "+ui->comboBox->currentText());
        }
    }
    if (ui->tab_num->isVisible()){
        QString min_val = ui->lineEditMin->text();
        QString max_val = ui->lineEditMax->text();
        if(min_val.isEmpty() | max_val.isEmpty()) {
            QMessageBox::critical(this, "Возможные значения", "Не введены возможные значения количественного признака");
        } else {
            record.setValue("Тип", "Количественный");
            mModel->setRecord(ui->comboBox->currentIndex(), record);
            record.setValue("Возможные значения", min_val+";"+max_val);
            mModel->setRecord(ui->comboBox->currentIndex(), record);
            QMessageBox::information(this, "Success", "Успешно добавлен тип к "+ui->comboBox->currentText());
        }
    }
    if (ui->tab_qual->isVisible()){
        QString val = ui->plainTextEdit->toPlainText();
        if(val.isEmpty()) {
            QMessageBox::critical(this, "Возможные значения", "Не введены возможные значения качественного признака");
        } else {
            val.replace("\n", ";");
            QMessageBox::information(this, "Success", val);
            record.setValue("Тип", "Качественный");
            mModel->setRecord(ui->comboBox->currentIndex(), record);
            record.setValue("Возможные значения", val);
            mModel->setRecord(ui->comboBox->currentIndex(), record);
            QMessageBox::information(this, "Success", "Успешно добавлен тип к "+ui->comboBox->currentText());
        }
    }
}


void attribute_possible_values::on_comboBox_currentIndexChanged(int index)
{
    clean_lineEdits();
    QSqlRecord record = mModel->record(index);
    if (record.value(1).toString() == logtype) {
        ui->tabWidget->setCurrentIndex(0);
        QString values = record.value(2).toString();
        if (values != nullptr) {
            ui->lineEditTrue->setText(values.left(values.indexOf(';')));
            ui->lineEditFalse->setText(values.right(values.size() - values.indexOf(';') - 1));
        }
    }
    if (record.value(1).toString() == numtype) {
        ui->tabWidget->setCurrentIndex(1);
        QString values = record.value(2).toString();
        if (values != nullptr) {
            ui->lineEditMin->setText(values.left(values.indexOf(';')));
            ui->lineEditMax->setText(values.right(values.size() - values.indexOf(';') - 1));
        }
    }
    if (record.value(1).toString() == qualtype) {
        ui->tabWidget->setCurrentIndex(2);
        QString values = record.value(2).toString();
        if (values != nullptr) {
            ui->plainTextEdit->setPlainText(values.replace(';', '\n'));
        }
    }
}
