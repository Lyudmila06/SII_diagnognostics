#include "classifier_attr.h"
#include "ui_classifier_attr.h"
#include "QSqlRecord"

#include <QMessageBox>

classifier_attr::classifier_attr(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::classifier_attr)
{
    ui->setupUi(this);

    mModel1 = new QSqlTableModel(this);
    mModel1->setTable("Attributes");
    mModel1->select();

    fill_leftList();

    classvalWindow = new classifier_values();
    connect(classvalWindow, &classifier_values::attrWindow, this, &classifier_attr::show);
}

classifier_attr::~classifier_attr()
{
    delete ui;
}

void classifier_attr::clean_lines()
{
    ui->listWidget_Right->clear();
    ui->listWidget_Left->clear();
    ui->listWidget_Unavail->clear();
}

void classifier_attr::fill_leftList() {
    //ui->listWidget_Left->clear();
    clean_lines();
    QString str;
    mModel1->select();
    for (int i = 0; i < mModel1->rowCount(); i++) {
        //str = mModel2->record().value(0).toString();
        ui->listWidget_Left->addItem(mModel1->record(i).value(0).toString());
    }
    checkAttr();
}

void classifier_attr::on_pushButtonBack_clicked()
{
    clean_lines();
    emit mainMenu();
    this->close();
}

void classifier_attr::on_pushButtonNext_clicked()
{
    if (ui->listWidget_Right->count() == 0) {
        QMessageBox::critical(this, "Error", "Не выбран ни один признак");
    } else {
//        bool fine = true;
//        for (int i = 0; i < ui->listWidget_Right->count(); i++) {
//            if (!ui->listWidget_Right->item(i)->toolTip().isEmpty()) {
//                fine = false;
//                QMessageBox::critical(this, "Error", "Признак " + ui->listWidget_Right->item(i)->text() + ":\n" + ui->listWidget_Right->item(i)->toolTip());
//            }
//        }
//        if (fine) {
            classvalWindow->setAttrList(ui->listWidget_Right->findItems(QString("*"), Qt::MatchWrap | Qt::MatchWildcard));
            classvalWindow->show();
            this->close();
//        }
    }
}

void classifier_attr::on_pushButtonRight_2_clicked()
{
    ui->listWidget_Left->update();
    int list_num = ui->listWidget_Left->count();
    for (int i = 0; i < list_num; i++){
        ui->listWidget_Right->addItem(ui->listWidget_Left->takeItem(0));
    }
}

void classifier_attr::on_pushButtonRight_clicked()
{
    ui->listWidget_Left->update();
    int list_num = ui->listWidget_Left->selectedItems().count();
    //QMessageBox::critical(this, "значения", QString::number(list_num));
    for (int i = 0; i < list_num; i++){
        //QMessageBox::critical(this, "значения", QString::number(ui->listWidget_Left->row(ui->listWidget_Left->selectedItems()[0])));
        ui->listWidget_Right->addItem(ui->listWidget_Left->takeItem(ui->listWidget_Left->row(ui->listWidget_Left->selectedItems()[0])));
    }
}

void classifier_attr::on_pushButtonLeft_clicked()
{
    ui->listWidget_Left->update();
    int list_num = ui->listWidget_Right->selectedItems().count();
    //QMessageBox::critical(this, "значения", QString::number(list_num));
    for (int i = 0; i < list_num; i++){
        ui->listWidget_Left->addItem(ui->listWidget_Right->takeItem(ui->listWidget_Right->row(ui->listWidget_Right->selectedItems()[0])));
    }
}

void classifier_attr::on_pushButtonLeft_2_clicked()
{
    ui->listWidget_Right->update();
    int list_num = ui->listWidget_Right->count();
    for (int i = 0; i < list_num; i++){
        ui->listWidget_Left->addItem(ui->listWidget_Right->takeItem(0));
    }
}

bool classifier_attr::checkAttr()
{
    QBrush errbrush(QColor(255, 200, 200));
    QSqlTableModel *select_model = new QSqlTableModel();
    select_model->setTable("Attributes");
    bool isfull = true;
    QString msg;
    QString attr;
    for (int i = 0; i < ui->listWidget_Left->count(); i++) {
        select_model->setFilter("Наименование = '" + ui->listWidget_Left->item(i)->text() + "'");
        select_model->select();
        if (select_model->rowCount() != 0) {
            msg = "";
            QString attr_req = select_model->record(0).value(1).toString() +" "+ select_model->record(0).value(2).toString() + " "+ select_model->record(0).value(3).toString();
            if (select_model->record(0).value(1).toString().isEmpty()) {
                msg += "Отсутствует тип признака \n";
                isfull = false;
            }
            if (select_model->record(0).value(2).toString().isEmpty()) {
                msg += "Отсутствуют возможные значения признака \n";
                isfull = false;
            }
            if (select_model->record(0).value(3).toString().isEmpty()) {
                msg += "Отсутствуют нормальные значения признака \n";
                isfull = false;
            }
        }
        if (!msg.isEmpty()) {
            ui->listWidget_Left->item(i)->setToolTip(msg);
            ui->listWidget_Left->item(i)->setBackground(errbrush);
            ui->listWidget_Unavail->addItem(ui->listWidget_Left->takeItem(i));
            i--;
        }

    }
    return isfull;
}
