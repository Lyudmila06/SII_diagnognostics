#include "attribute_possible_values.h"
#include "ui_attribute_possible_values.h"
#include <QMessageBox>
#include <QSqlRecord>
#include <QString>

#define logtype "Логический"
#define numtype "Количественный"
#define qualtype "Качественный"

attribute_possible_values::attribute_possible_values(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::attribute_possible_values)
{
    ui->setupUi(this);

    mModel = new QSqlTableModel(this);
    mModel->setTable("Attributes");
    mModel->select();
    ui->comboBox->setModel(mModel);
    ui->comboBox->setModelColumn(0);
    //mModel2->setTable("Disease_attributes");

}

attribute_possible_values::~attribute_possible_values()
{
    delete ui;
}

void attribute_possible_values::update_mModel()
{
   this->mModel->select();
}

void attribute_possible_values::on_pushButtonBack_clicked()
{
    update_mModel();
    this->clean_lineEdits();
    this->close();
    emit mainEditor();
}

void attribute_possible_values::clean_lineEdits()
{
    ui->lineEditTrue->setText(nullptr);
    ui->lineEditFalse->setText(nullptr);
    ui->lineEditMax->setText(nullptr);
    ui->lineEditMin->setText(nullptr);
    ui->plainTextEdit->setPlainText(nullptr);

}

void attribute_possible_values::deleteDisAttr(QString name)
{
    QSqlTableModel *select_model = new QSqlTableModel();
    select_model->setTable("Disease_attributes");
    select_model->setFilter("Признак = '" + name +  "'");
    for (int i = 0; i < select_model->rowCount(); i++) {
        select_model->removeRow(i);
    }
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
            record.setValue("Нормальные значения", "");
            mModel->setRecord(ui->comboBox->currentIndex(), record);
            QMessageBox::information(this, "Success", "Успешно добавлен тип и возможные значения признака "+ui->comboBox->currentText());
        }
    }
    if (ui->tab_num->isVisible()){
        bool isnum1 = new bool(true), isnum2 = new bool(true);
        QString min_val = ui->lineEditMin->text();
        QString max_val = ui->lineEditMax->text();
        min_val.toFloat(&isnum1);
        max_val.toFloat(&isnum2);
        if(min_val.isEmpty() || max_val.isEmpty()) {
            QMessageBox::critical(this, "Возможные значения", "Не введены возможные значения количественного признака");
        } else if (!isnum1 || !isnum2) {
            QMessageBox::critical(this, "Возможные значения", "Недопустимые символы в возможных значениях количественного признака ");
        } else if(min_val.toFloat() > max_val.toFloat()) {
             QMessageBox::critical(this, "Возможные значения", "Минимальное значение признака не может быть больше максимального");
        } else {
            record.setValue("Тип", "Количественный");
            mModel->setRecord(ui->comboBox->currentIndex(), record);
            record.setValue("Возможные значения", min_val+";"+max_val);
            record.setValue("Нормальные значения", "");
            mModel->setRecord(ui->comboBox->currentIndex(), record);
            QMessageBox::information(this, "Success", "Успешно добавлен тип и возможные значения признака " + ui->comboBox->currentText());
        }
    }
    if (ui->tab_qual->isVisible()){
        QString val = ui->plainTextEdit->toPlainText();
        QRegularExpression regex("/^\n*");
        if(val.isEmpty()) {
            QMessageBox::critical(this, "Возможные значения", "Не введены возможные значения качественного признака");
           // return;
        } else if(val.indexOf(";") != -1) {
            QMessageBox::critical(this, "Возможные значения", "Возможное значение не должно содержать символ ';'");
           // return;
        } else if(val.indexOf("\n\n") != -1 || regex.match(val).hasMatch()) {
             QMessageBox::critical(this, "Возможные значения", "Возможные значения признака не должны быть пустыми");
        } else {
            val.replace("\n", ";");
            if (checkQualVal(val)) {
                //QMessageBox::information(this, "Success", val);
                record.setValue("Тип", "Качественный");
                mModel->setRecord(ui->comboBox->currentIndex(), record);
                record.setValue("Возможные значения", val);
                record.setValue("Нормальные значения", "");
                mModel->setRecord(ui->comboBox->currentIndex(), record);
                QMessageBox::information(this, "Success", "Успешно добавлен тип и возможные значения признака " + ui->comboBox->currentText());
            } else {
                QMessageBox::critical(this, "Возможные значения", "Возможные значения признака не должны повторяться");
            }
        }
    }
}
//ПРОВЕРКА ЗНАЧЕНИЙ КАЧЕСТВЕННОГО ТИПА
bool attribute_possible_values::checkQualVal(QString s)
{
    int ind = s.indexOf(";");
    int ind2;
    QString val = s.left(ind);
//    QMessageBox::information(this, "Success", s);
//    QMessageBox::information(this, "Success", val);
    if (s.indexOf(";"+val, ind+1) != -1) {
        return false;
    }
    while (ind < s.length()) {
        ind2 = s.indexOf(";", ind+1);
        if (ind2 == -1)
            break;
        val = s.mid(ind, ind2-ind+1);
//        QMessageBox::information(this, "Success", val + " " + QString::number(ind) + " "+QString::number(ind2));
        if (s.indexOf(val, ind2) != -1) {
            return false;
        }
        ind = ind2 + 1;
    }
    return true;

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
            ui->plainTextEdit->setPlainText(values.replace(';', '\n'));//+";");
        }
    }
}
