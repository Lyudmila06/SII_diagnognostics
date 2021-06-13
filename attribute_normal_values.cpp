#include "attribute_normal_values.h"
#include "ui_attribute_normal_values.h"
#include <QSqlRecord>
#include <QMessageBox>

#define logtype "Логический"
#define numtype "Количественный"
#define qualtype "Качественный"

attribute_normal_values::attribute_normal_values(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::attribute_normal_values)
{
    ui->setupUi(this);

    set_tabsUnenable();

    mModel = new QSqlTableModel(this);
    mModel->setTable("Attributes");
    mModel->select();
    ui->comboBox->setModel(mModel);
    ui->comboBox->setModelColumn(0);
}

attribute_normal_values::~attribute_normal_values()
{
    delete ui;
}

void attribute_normal_values::update_mModel()
{
   this->mModel->select();
}

void attribute_normal_values::on_pushButtonBack_clicked()
{
    emit mainEditor();
    this->close();
}

void attribute_normal_values::set_tabsUnenable()
{
    ui->tabWidget->setTabEnabled(0, false);
    ui->tabWidget->setTabEnabled(1, false);
    ui->tabWidget->setTabEnabled(2, false);
    log_val = nullptr;

}

void attribute_normal_values::clean_lineEdits()
{
    ui->radioButtonTrue->setChecked(false);
    ui->radioButtonFalse->setChecked(false);
    ui->lineEditMax->setText(nullptr);
    ui->lineEditMin->setText(nullptr);
    ui->listWidget->clear();

}

QString attribute_normal_values::listToString(QList<QListWidgetItem*> l)
{
    QString str;
    QList<QListWidgetItem*>::iterator lpoint = l.begin();
    while (!l.isEmpty()) {
        str += l.takeFirst()->text();
        str += ";";
    }
    return str;
}

void attribute_normal_values::on_pushButtonNext_clicked()
{
    QSqlRecord record = mModel->record(ui->comboBox->currentIndex());

    if (ui->tab_log->isVisible()){
        //mModel2->selectRow(ui->comboBox->currentIndex());
        if (ui->radioButtonTrue->isChecked()){
            record.setValue("Нормальные значения", ui->radioButtonTrue->text());
            mModel->setRecord(ui->comboBox->currentIndex(), record);
            QMessageBox::information(this, "Success", "Успешно добавлено нормальное значение к "+ui->comboBox->currentText());
        } else if (ui->radioButtonFalse->isChecked()) {
            record.setValue("Нормальные значения", ui->radioButtonFalse->text());
            mModel->setRecord(ui->comboBox->currentIndex(), record);
            QMessageBox::information(this, "Success", "Успешно добавлено нормальное значение к "+ui->comboBox->currentText());
            update_mModel();
        } else {
            QMessageBox::critical(this, "Нормальные значения", "Не выбрано нормальное значение логического признака");
        }
    }
    else if (ui->tab_num->isVisible()){
        bool isnum1 = new bool(true), isnum2 = new bool(true);
        QString min_val = ui->lineEditMin->text();
        QString max_val = ui->lineEditMax->text();
        min_val.toFloat(&isnum1);
        max_val.toFloat(&isnum2);
        if(min_val.isEmpty() | max_val.isEmpty()) {
            QMessageBox::critical(this, "Нормальные значения", "Не введены нормальные значения количественного признака");
        } else if(min_val.toFloat() > max_val.toFloat()) {
             QMessageBox::critical(this, "Нормальные значения", "Минимальное значение признака не может быть больше максимального");
        } else if (min_val.toFloat() > fval_2 | min_val.toFloat() < fval_1  | max_val.toFloat() < fval_1 | max_val.toFloat() > fval_2) {
            QMessageBox::critical(this, "Нормальные значения", "Интервал нормальных значений не может находиться за пределами интервала возможных значений");
        } else {
            record.setValue("Нормальные значения", min_val+";"+max_val);
            mModel->setRecord(ui->comboBox->currentIndex(), record);
            QMessageBox::information(this, "Success", "Успешно добавлен интервал нормальных значений к "+ui->comboBox->currentText());
            update_mModel();
        }
    }
    else if (ui->tab_qual->isVisible()){
        QString val = ui->listWidget->selectedItems().takeFirst()->text(); //listToString(ui->listWidget->selectedItems().toList());
        if(val.isEmpty()) {
            QMessageBox::critical(this, "Нормальные значения", "Не выбраны нормальные значения качественного признака");
        } else {
            //QMessageBox::information(this, "Success", val);
            record.setValue("Нормальные значения", val);
            mModel->setRecord(ui->comboBox->currentIndex(), record);
            QMessageBox::information(this, "Success", "Успешно добавлено нормальное значение к "+ui->comboBox->currentText());
            update_mModel();
        }
    }
}

void attribute_normal_values::on_comboBox_currentIndexChanged(int index)
{
    clean_lineEdits();
    set_tabsUnenable();
    QSqlRecord record = mModel->record(index);
    if (record.value(1).toString() == logtype) {
        ui->tabWidget->setTabEnabled(0, true);
        ui->tabWidget->setCurrentIndex(0);
        QString values = record.value(2).toString();
        if (values != nullptr) {
            val_1 = values.left(values.indexOf(';'));
            val_2 = values.right(values.size() - values.indexOf(';') - 1);
            ui->radioButtonTrue->setText(val_1);
            ui->radioButtonFalse->setText(val_2);
            //ui->lineEditFalse->setText(values.right(values.size() - values.indexOf(';') - 1));
        }
    }
    if (record.value(1).toString() == numtype) {
        ui->tabWidget->setTabEnabled(1, true);
        ui->tabWidget->setCurrentIndex(1);
        QString values = record.value(2).toString();
        if (values != nullptr) {
            fval_1 = values.left(values.indexOf(';')).toFloat();
            fval_2 = values.right(values.size() - values.indexOf(';') - 1).toFloat();
            ui->label_num_poss->setText("Интервал допустимых значений: ["+values+"]");
        }
    }
    if (record.value(1).toString() == qualtype) {
        ui->tabWidget->setTabEnabled(2, true);
        ui->tabWidget->setCurrentIndex(2);
        QString values = record.value(2).toString();
        if (values != nullptr) {
            int l = 0, r = 0;
            while (l < values.size()) {
                r = values.indexOf(';', l);
                if (r == -1) r = values.size();
                ui->listWidget->addItem(values.mid(l, r-l));
                l = r + 1;
            }
            //ui->listWidget->addItem()
            //ui->plainTextEdit->setPlainText(values.replace(';', '\n'));
        }
    }
}

