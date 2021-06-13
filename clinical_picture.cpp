#include "clinical_picture.h"
#include "ui_clinical_picture.h"
#include "QCommonStyle"
#include <QSqlRecord>
#include <QMessageBox>
#include <QtCore>
#include <Qt>
#include <QMessageBox>

clinical_picture::clinical_picture(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::clinical_picture)
{
    ui->setupUi(this);

    mModel2 = new QSqlTableModel(this);
    mModel2->setTable("Attributes");
    mModel2->select();

    fill_leftList();

    mModel1 = new QSqlTableModel(this);
    mModel1->setTable("Diseases");
    mModel1->select();
    ui->comboBox->setModel(mModel1);
    ui->comboBox->setModelColumn(0);

    //ui->listWidget_2->setModel(mModel2);

    //ui->listView->setModel(mModel2);
    //ui->listView->setModelColumn(0);

    QCommonStyle style;
    //ui->pushButtonLeft->setIcon(style.standardIcon(QStyle::SP_ArrowBack));
    //ui->pushButtonRight->setIcon(style.standardIcon((QStyle::SP_ArrowForward)));
}

clinical_picture::~clinical_picture()
{
    delete ui;
}

void clinical_picture::update_mModel1()
{
   this->mModel1->select();
}

void clinical_picture::update_mModel2()
{
   this->mModel2->select();
}

void clinical_picture::changeCurrentIndex()
{
    ui->comboBox->currentIndexChanged(0);
}

void clinical_picture::fill_leftList() {
    ui->listWidget_Left->clear();
    QString str;
    for (int i = 0; i < mModel2->rowCount(); i++) {
        //str = mModel2->record().value(0).toString();
        ui->listWidget_Left->addItem(mModel2->record(i).value(0).toString());
    }
}

void clinical_picture::on_pushButtonBack_clicked()
{
    emit mainEditor();
    this->close();
}

void clinical_picture::clean_lines()
{
    ui->listWidget_Right->clear();
    //ui->listView->setText(nullptr);
    //ui->listWidget->
}

void clinical_picture::on_pushButtonNext_clicked()
{
    //QMessageBox::critical(this, "значения", listToString(ui->listWidget_Right->items());
    //MIME
    QSqlRecord record = mModel1->record(ui->comboBox->currentIndex());
    QString val = listToString(ui->listWidget_Right->findItems("*", Qt::MatchWildcard));
    if(val.isEmpty()) {
        QMessageBox::critical(this, "Клиническая картина заболевания", "Не выбраны признаки заболевания " + ui->comboBox->currentText());
    } else {
        record.setValue("Клиническая картина", val);
        mModel1->setRecord(ui->comboBox->currentIndex(), record);
        QMessageBox::information(this, "Success", "Клиническая картина заболевания " + ui->comboBox->currentText() + " успешно изменена");
        update_mModel1();
        update_mModel2();
    }

}

QString clinical_picture::listToString(QList<QListWidgetItem*> l)
{
    QString str;
    //QList<QListWidgetItem*>::iterator lpoint = l.begin();
    while (!l.isEmpty()) {
        str += l.takeFirst()->text();
        str += ";";
    }
    return str;
}

void clinical_picture::on_comboBox_currentIndexChanged(int index)
{
    clean_lines();
    fill_leftList();
    QSqlRecord record = mModel1->record(index);
    QString values = record.value(1).toString();
    if (values != nullptr) {
        int l = 0, r = 0;
        while (l < values.size()) {
            r = values.indexOf(';', l);
            if (r == -1) r = values.size();
            ui->listWidget_Right->addItem(values.mid(l, r-l));
            l = r + 1;
        }
        //ui->listWidget->addItem()
        //ui->plainTextEdit->setPlainText(values.replace(';', '\n'));
    }
    int item_num = ui->listWidget_Right->count();
    for (int i = 0; i < item_num; i++) {
        qDeleteAll(ui->listWidget_Left->findItems(QString::fromStdString(ui->listWidget_Right->item(i)->text().toStdString()), Qt::MatchFixedString));
    }
}

void clinical_picture::on_pushButtonRight_clicked()
{
    ui->listWidget_Left->update();
    int list_num = ui->listWidget_Left->selectedItems().count();
    //QMessageBox::critical(this, "значения", QString::number(list_num));
    for (int i = 0; i < list_num; i++){
        //QMessageBox::critical(this, "значения", QString::number(ui->listWidget_Left->row(ui->listWidget_Left->selectedItems()[0])));
        ui->listWidget_Right->addItem(ui->listWidget_Left->takeItem(ui->listWidget_Left->row(ui->listWidget_Left->selectedItems()[0])));
    }
}

void clinical_picture::on_pushButtonLeft_clicked()
{
    ui->listWidget_Left->update();
    int list_num = ui->listWidget_Right->selectedItems().count();
    //QMessageBox::critical(this, "значения", QString::number(list_num));
    for (int i = 0; i < list_num; i++){
        ui->listWidget_Left->addItem(ui->listWidget_Right->takeItem(ui->listWidget_Right->row(ui->listWidget_Right->selectedItems()[0])));
    }
}

void clinical_picture::on_pushButtonRight_2_clicked()
{
    ui->listWidget_Left->update();
    int list_num = ui->listWidget_Left->count();
    for (int i = 0; i < list_num; i++){
        ui->listWidget_Right->addItem(ui->listWidget_Left->takeItem(0));
    }
}

void clinical_picture::on_pushButtonLeft_2_clicked()
{
    ui->listWidget_Right->update();
    int list_num = ui->listWidget_Right->count();
    for (int i = 0; i < list_num; i++){
        ui->listWidget_Left->addItem(ui->listWidget_Right->takeItem(0));
    }
}


