#include "completeness_check.h"
#include "ui_completeness_check.h"

#include <QMessageBox>
#include <QSqlRecord>
#include <QStyle>
#include "QCommonStyle"

#define SUCCESS_ style.standardIcon(QStyle::SP_DialogApplyButton)
#define FAIL_ style.standardIcon(QStyle::SP_DialogCancelButton)


completeness_check::completeness_check(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::completeness_check)
{
    ui->setupUi(this);
//    status_success = new QTableWidgetItem();
//    status_success->setIcon(style.standardIcon(QStyle::SP_DialogApplyButton));
//    status_fail = new QTableWidgetItem();
//    status_fail->setIcon(style.standardIcon(QStyle::SP_DialogAbortButton));

//    status_success = new QIcon(style.standardIcon(QStyle::SP_DialogApplyButton));
//    status_fail = new QIcon(style.standardIcon(QStyle::SP_DialogAbortButton));

    mModel3 = new QSqlTableModel(this);
    mModel3->setTable("Disease_attributes");
    mModel3->select();
    mModel2 = new QSqlTableModel(this);
    mModel2->setTable("Attributes");
    mModel2->select();

    mModel1 = new QSqlTableModel(this);
    mModel1->setTable("Diseases");
    mModel1->select();

    //QSqlRecord record = mModel1->;
    //QString values = record.value(1).toString();
    ui->tableWidget_dis->insertColumn(0);
    ui->tableWidget_dis->setHorizontalHeaderItem(0, new QTableWidgetItem("Заболевание"));
    ui->tableWidget_dis->insertColumn(1);
    ui->tableWidget_dis->setHorizontalHeaderItem(1, new QTableWidgetItem("Результат"));
    ui->tableWidget_dis->setColumnWidth(0, 400);

    ui->tableWidget_attr->insertColumn(0);
    ui->tableWidget_attr->setHorizontalHeaderItem(0, new QTableWidgetItem("Признак"));
    //ui->tableWidget_attr->setColumnWidth(0, 250);

    ui->tableWidget_attr->insertColumn(1);
    ui->tableWidget_attr->setHorizontalHeaderItem(1, new QTableWidgetItem("Результат"));
    ui->tableWidget_attr->setColumnWidth(0, 400);

    ui->tabWidget->setCurrentIndex(0);
    //ui->tableWidget_attr->insertColumn(1);
    //ui->tableWidget_attr->setHorizontalHeaderItem(1, new QTableWidgetItem("Клиническая картина"));
}

completeness_check::~completeness_check()
{
    delete ui;
}

QStyleOptionViewItem completeness_check::viewOptions() const
{
    QStyleOptionViewItem option = completeness_check::viewOptions();
    option.decorationAlignment = Qt::AlignHCenter | Qt::AlignCenter;
    option.decorationPosition = QStyleOptionViewItem::Top;

    return option;
}

void completeness_check::update_Models()
{
    mModel1->select();
    mModel2->select();
    mModel3->select();
}

void completeness_check::clean_Tables()
{
    //ui->tableWidget_attr->clear();
    //ui->tableWidget_dis->clear();
    ui->tableWidget_dis->setRowCount(0);
    ui->tableWidget_attr->setRowCount(0);
}

//Преобразует строку с признаками(рзделитель ;) в список признаков
QList<QString> completeness_check::StringToList(QString s)
{
    QList<QString> list_;
    int l = 0, r = 0;
    while (l < s.size()) {
        r = s.indexOf(';', l);
        if (r == -1) r = s.size();
        list_.push_back(s.mid(l, r-l));
        l = r + 1;
    }

    return list_;
}

void completeness_check::check_Attributes()
{
    QString tip;
    for (int i = 0; i < mModel2->rowCount(); i++) {
        tip = "";
        QTableWidgetItem *status = new QTableWidgetItem();
        status->setIcon(SUCCESS_);
        if (mModel2->record(i).value(1).toString().isEmpty()) {
            status->setIcon(FAIL_);
            tip = "Тип признака отсутствует\n";

        }
        if (mModel2->record(i).value(2).toString().isEmpty()) {
            status->setIcon(FAIL_);
            tip += "Возможные значения признака отсутствуют\n";

        }
        if (mModel2->record(i).value(3).toString().isEmpty()) {
            status->setIcon(FAIL_);
            tip += "Нормальные значения признака отсутствуют\n";

        }
        if (tip.isEmpty())
            tip = "Проверка успешно пройдена";

        ui->tableWidget_attr->setItem(i, 1, status);
        ui->tableWidget_attr->item(i, 1)->setToolTip(tip);

    }
}
void completeness_check::check_Diseases()
{
    QList<QString> clin_pic;
    QSqlTableModel *select_model = new QSqlTableModel();
    QString tip;
    bool bstatus;
    int a = 0;
    for (int i = 0; i < mModel1->rowCount(); i++) {
        tip = "";
        bstatus = true;
        QTableWidgetItem *status = new QTableWidgetItem();

        if (mModel1->record(i).value(1).toString().isEmpty()) {
            tip += "Клиническая картина заболевания пуста\n";
//            status->setIcon(FAIL_);
            bstatus = false;
        } else {
            clin_pic = StringToList(mModel1->record(i).value(1).toString());
//            status->setIcon(SUCCESS_);
            for (int j = 0; j < clin_pic.size(); j++) {
                select_model->setTable("Disease_attributes");
                //select_model->select();
                select_model->setFilter("Заболевание = '" + mModel1->record(i).value(0).toString() + "' AND Признак = '" + clin_pic[j] + "'");
                select_model->select();
                a = select_model->rowCount();
                if(select_model->rowCount() == 0 || select_model->record(0).value(2).toString().isEmpty()) {
                    tip += "Не установлено значение признака " + clin_pic[j] + " при заболевании\n";
//                    status->setIcon(FAIL_);
                    bstatus = false;
                }
            }
        }
        if (bstatus) {
            tip = "Проверка успешно пройдена";
            status->setIcon(SUCCESS_);
        } else
            status->setIcon(FAIL_);
        ui->tableWidget_dis->setItem(i, 1, status);
        ui->tableWidget_dis->item(i, 1)->setToolTip(tip);
    }
}



void completeness_check::update_Tables() {
    update_Models();
    int j = mModel1->rowCount();
    QString s;

    for (int i = 0; i < mModel1->rowCount(); i++) {
        ui->tableWidget_dis->insertRow(i);//->addItem(values.mid(l, r-l));
        s = mModel1->record(i).value(0).toString();
        QTableWidgetItem *qti = new QTableWidgetItem(mModel1->record(i).value(0).toString(), 0);
        ui->tableWidget_dis->setItem(i, 0, qti);
        //ui->listWidget->addItem()
        //ui->plainTextEdit->setPlainText(values.replace(';', '\n'));
    }

    for (int i = 0; i < mModel2->rowCount(); i++) {
        ui->tableWidget_attr->insertRow(i);//->addItem(values.mid(l, r-l));
        s = mModel2->record(i).value(0).toString();
        QTableWidgetItem *qti = new QTableWidgetItem(mModel2->record(i).value(0).toString(), 0);
        //QTableWidgetItem *status = new QTableWidgetItem();
        //status->setIcon(FAIL_);
//        new QTableWidgetItem()
        ui->tableWidget_attr->setItem(i, 0, qti);
        //ui->tableWidget_attr->setItem(i, 1, status);
        //ui->listWidget->addItem()
        //ui->plainTextEdit->setPlainText(values.replace(';', '\n'));
    }
}

void completeness_check::on_pushButtonBack_clicked()
{
    clean_Tables();
    ui->tab->setFocus();
    ui->tabWidget->setCurrentIndex(0);
    emit mainEditor();
    this->close();
}

void completeness_check::on_pushButtonCheck_clicked()
{
    check_Attributes();
    check_Diseases();
}
