#include "classifier_result.h"
#include "ui_classifier_result.h"

classifier_result::classifier_result(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::classifier_result)
{
    ui->setupUi(this);
}

classifier_result::~classifier_result()
{
    delete ui;
}

void classifier_result::cleanPanels()
{
    ui->listWidget->clear();
    ui->listWidget_2->clear();
    ui->listWidget_3->clear();
    ui->tableWidget->setRowCount(0);
    ui->tableWidget->setColumnCount(0);
    ui->tableWidget->clear();
}

void classifier_result::on_pushButton_clicked()
{
    cleanPanels();
    emit attrValWindow();
    this->close();
}

QTableWidget *classifier_result::getResultTable()
{
    //ui->tableWidget = t;
    return ui->tableWidget;
}

void classifier_result::addDiagnosistoList(QString d)
{
    ui->listWidget->addItem(d);
}

void classifier_result::addDiagnosistoList2(QString d)
{
    ui->listWidget_3->addItem(d);
}

void classifier_result::addMsgtoList(QString s)
{
    ui->listWidget_2->addItem(s);
}
