#include "disease_description.h"
#include "ui_disease_description.h"

#include <QMessageBox>
#include <QSqlQuery>
#include <QSqlRecord>
#include <QSqlError>

#define logtype "Логический"
#define numtype "Количественный"
#define qualtype "Качественный"

disease_description::disease_description(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::disease_description)
{
    ui->setupUi(this);
    mModel3 = new QSqlTableModel(this);
    mModel3->setTable("Disease_attributes");
    mModel3->select();

    mModel2 = new QSqlTableModel(this);
    mModel2->setTable("Attributes");
    mModel2->select();

    mModel1 = new QSqlTableModel(this);
    mModel1->setTable("Diseases");
    mModel1->select();
    ui->comboBox->setModel(mModel1);
    ui->comboBox->setModelColumn(0);
}

disease_description::~disease_description()
{
    delete ui;
}

void disease_description::changeCurrentIndex()
{
    ui->comboBox->currentIndexChanged(0);
}

void disease_description::on_comboBox_currentIndexChanged(int index)
{
    ui->listWidget->clear();
    QSqlRecord record = mModel1->record(index);
    QString values = record.value(1).toString();
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

void disease_description::update_mModels()
{
    mModel1->select();
    mModel2->select();
    mModel3->select();
}

QString disease_description::listToString(QList<QListWidgetItem*> l)
{
    QString str;
    //QList<QListWidgetItem*>::iterator lpoint = l.begin();
    while (!l.isEmpty()) {
        str += l.takeFirst()->text();
        str += ";";
    }
    return str;
}

void disease_description::set_tabsUnenable()
{
    ui->tabWidget->setTabEnabled(0, false);
    ui->tabWidget->setTabEnabled(1, false);
    ui->tabWidget->setTabEnabled(2, false);
    log_val = nullptr;

}

void disease_description::clean_lineEdits()
{
    ui->radioButtonTrue->setChecked(false);
    ui->radioButtonFalse->setChecked(false);
    ui->lineEditMax->setText(nullptr);
    ui->lineEditMin->setText(nullptr);
    ui->listWidgetQual->clear();

}

void disease_description::on_pushButtonBack_clicked()
{
    clean_lineEdits();
    set_tabsUnenable();
    emit mainEditor();
    this->close();
}

void disease_description::on_pushButtonNext_clicked()
{
    QString result_str = nullptr;
    QSqlQuery query;
    QSqlRecord record = mModel3->record();
    query.exec("SELECT * FROM Disease_attributes WHERE Заболевание = '" + ui->comboBox->currentText() + "' and Признак = '"
                + ui->listWidget->selectedItems().first()->text() + "';");
    record.setValue("Заболевание", ui->comboBox->currentText());
    record.setValue("Признак", ui->listWidget->selectedItems().first()->text());
    QString attr = ui->listWidget->selectedItems().first()->text();

    if (ui->tab_log->isVisible()){
        //mModel2->selectRow(ui->comboBox->currentIndex());
        if (ui->radioButtonTrue->isChecked()){
            result_str = ui->radioButtonTrue->text();
            QMessageBox::information(this, "Success", "Успешно добавлено нормальное значение признака "
                                        + attr + " при заболевании " + ui->comboBox->currentText());
        } else if (ui->radioButtonFalse->isChecked()) {
            result_str = ui->radioButtonFalse->text();
        } else {
            QMessageBox::critical(this, "Значение признака заболевания", "Не выбрано нормальное значение логического признака");
            return;
        }
    }
    else if (ui->tab_num->isVisible()){
        QString min_val = ui->lineEditMin->text();
        QString max_val = ui->lineEditMax->text();
        if(min_val.isEmpty() | max_val.isEmpty()) {
            QMessageBox::critical(this, "Значение признака заболевания", "Не введены нормальные значения количественного признака");
            return;
        } else if(min_val.toFloat() > max_val.toFloat()) {
             QMessageBox::critical(this, "Значение признака заболевания", "Минимальное значение признака не может быть больше максимального");
             return;
        } else if (min_val.toFloat() > fval_2 | min_val.toFloat() < fval_1  | max_val.toFloat() < fval_1 | max_val.toFloat() > fval_2) {
            QMessageBox::critical(this, "Значение признака заболевания", "Интервал не может находиться за пределами интервала возможных значений");
            return;
        } else {
            result_str = min_val+";"+max_val;
        }
    }
    else if (ui->tab_qual->isVisible()){
        QString val = listToString(ui->listWidgetQual->selectedItems()); //listToString(ui->listWidget->selectedItems().toList());
        if(val.isEmpty()) {
            QMessageBox::critical(this, "Значение признака заболевания", "Не выбраны нормальные значения качественного признака");
            return;
        } else {
            result_str = val;
        }
    }
    if (query.next()){
        if (query.exec("UPDATE Disease_attributes SET [Возможные значения] = '" + result_str
                   + "' WHERE Заболевание = '" + ui->comboBox->currentText() + "' and Признак = '" + ui->listWidget->selectedItems().first()->text() + "';"))
            QMessageBox::information(this, "Success", "Успешно добавлено нормальное значение к "+ui->comboBox->currentText());
        else QMessageBox::critical(this, "Значение признака заболевания", query.lastError().text());
    } else {
        record.setValue("Возможные значения", result_str);
        mModel3->insertRecord(-1, record);
        //QMessageBox::information(this, "Success", "Успешно добавлено нормальное значение признака "
        //                            + attr + " при заболевании " + ui->comboBox->currentText());
    }
    mModel3->select();
}

void disease_description::on_listWidget_itemClicked(QListWidgetItem *item)
{
    clean_lineEdits();
    set_tabsUnenable();
    //QSqlRecord record = mModel2->record(index);
    //THERE
    int i;
    for (i = 0; i < mModel2->rowCount(); i++) {
        if (mModel2->record(i).value(0) == item->text()){
            break;
        }
    }
    if (i > mModel2->rowCount())
        return;
    QSqlRecord record = mModel2->record(i);
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
        clean_lineEdits();
        ui->tabWidget->setTabEnabled(2, true);
        ui->tabWidget->setCurrentIndex(2);
        QString values = record.value(2).toString();
        if (values != nullptr) {
            int l = 0, r = 0;
            while (l < values.size()) {
                r = values.indexOf(';', l);
                if (r == -1) r = values.size();
                ui->listWidgetQual->addItem(values.mid(l, r-l));
                l = r + 1;
            }
            //ui->listWidget->addItem()
            //ui->plainTextEdit->setPlainText(values.replace(';', '\n'));
        }
    }
}
