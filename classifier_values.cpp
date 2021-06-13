#include "classifier_values.h"
#include "ui_classifier_values.h"

#include <QMessageBox>
#include <QSqlRecord>
#include <QString>

classifier_values::classifier_values(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::classifier_values)
{
    ui->setupUi(this);

    ui->tableWidget_attr->insertColumn(0);
    ui->tableWidget_attr->setHorizontalHeaderItem(0, new QTableWidgetItem("Признак"));
    ui->tableWidget_attr->insertColumn(1);
    ui->tableWidget_attr->setHorizontalHeaderItem(1, new QTableWidgetItem("Значение"));

    v_dialogWindow = new value_dialog();
    connect(v_dialogWindow, &value_dialog::attrWindow, this, &classifier_values::show);
    connect(v_dialogWindow, &value_dialog::setVal, this, &classifier_values::setValue);

    mModel1 = new QSqlTableModel();
    mModel1->setTable("Diseases");
    mModel1->select();

    mModel2 = new QSqlTableModel();
    mModel2->setTable("Disease_attributes");
    mModel2->select();

    resultWindow = new classifier_result();
    connect(resultWindow, &classifier_result::attrValWindow, this, &classifier_values::show);

}

classifier_values::~classifier_values()
{
    delete ui;
}

void classifier_values::setValue(int ind, QString val)
{
    ui->tableWidget_attr->item(ind, 1)->setText(val);
}

bool classifier_values::isValComplete()
{
    for (int i = 0; i < ui->tableWidget_attr->rowCount(); i++) {
        if (ui->tableWidget_attr->item(i, 1)->text().isEmpty()) {
            QMessageBox::critical(this, "Error", "Не заполнены значения всех выбранных признаков");
            return false;
        }
    }
    return true;
}

void classifier_values::setDiagnosis()
{
    //resultWindow->cleanPanels();
    bool isfitting;
    bool ishealthy;
    //QSqlQuery query;
    QSqlTableModel *model = new QSqlTableModel();
    model->setTable("Disease_attributes");
    QSqlTableModel *modelAttr = new QSqlTableModel();
    modelAttr->setTable("Attributes");

    //добавляем выбранные признаки
    QTableWidget *result_table = resultWindow->getResultTable();
    for (int i = 0; i < ui->tableWidget_attr->rowCount(); i++) {
        result_table->insertRow(i);
        result_table->setVerticalHeaderItem(i, new QTableWidgetItem(ui->tableWidget_attr->item(i, 0)->text()));
    }
    int counAttr = result_table->rowCount();

    //проходим по каждому заболеванию
    for (int i = 0; i < mModel1->rowCount(); i++) {
        QString dis = mModel1->record(i).value(0).toString();
        result_table->insertColumn(i);
        result_table->setHorizontalHeaderItem(i, new QTableWidgetItem(dis));
        isfitting = true;

        for (int j = 0; j < counAttr; j++) {
            model->setFilter("Заболевание = '" + dis + "' AND Признак = '" + result_table->verticalHeaderItem(j)->text() + "'");
            model->select();
            if (model->rowCount() == 0) {
                isfitting = false;
                result_table->setItem(j, i, new QTableWidgetItem("-"));
                resultWindow->addMsgtoList("Значения признака "+result_table->verticalHeaderItem(j)->text() + " не входит в область возможных значений заболевания "+dis);
            } else {
                modelAttr->setFilter("Наименование = '" + result_table->verticalHeaderItem(j)->text() + "'");
                modelAttr->select();
                if (modelAttr->rowCount() != 0
                        && checkEntry(modelAttr->record(0).value(1).toString(), ui->tableWidget_attr->item(j, 1)->text(), model->record(0).value(2).toString())) {
                    result_table->setItem(j, i, new QTableWidgetItem("+"));
                } else {
                    isfitting = false;
                    result_table->setItem(j, i, new QTableWidgetItem("-"));
                    resultWindow->addMsgtoList("Значения признака "+result_table->verticalHeaderItem(j)->text() + " не входит в область возможных значений заболевания "+dis);
                }
            }
        }
        if (isfitting) {
            resultWindow->addDiagnosistoList(dis);
        } else {
            resultWindow->addDiagnosistoList2(dis);
        }
    }

    //если все признаки имеют нормальные показатели, то здоров
    ishealthy = true;
    int ind_health = result_table->columnCount();
    result_table->insertColumn(mModel1->rowCount());
    result_table->setHorizontalHeaderItem(mModel1->rowCount(), new QTableWidgetItem("Здоров"));
    for (int j = 0; j < counAttr; j++) {
        modelAttr->setFilter("Наименование = '" + result_table->verticalHeaderItem(j)->text() + "'");
        modelAttr->select();
        if (modelAttr->rowCount() != 0) {
            QString norm = modelAttr->record(0).value(3).toString();
            if(checkEntry(modelAttr->record(0).value(1).toString(), ui->tableWidget_attr->item(j, 1)->text(), norm)) {
                result_table->setItem(j, ind_health, new QTableWidgetItem("+"));
            } else {
                result_table->setItem(j, ind_health, new QTableWidgetItem("-"));
                ishealthy = false;
                resultWindow->addMsgtoList("Значения признака "+modelAttr->record(0).value(0).toString() + " не входит в область нормальных значений");
            }
        }
    }
    if (ishealthy) {
        resultWindow->addDiagnosistoList("Здоров");
    } else {
        resultWindow->addDiagnosistoList2("Здоров");
    }
}

bool classifier_values::checkEntry(QString type, QString val, QString poss_val)
{
    //QMessageBox::information(this, "Error", val + " " + poss_val);
    if (type == "Логический" && val == poss_val) {
        return true;
    }
    if (type == "Количественный") {
        float min_val = poss_val.left(poss_val.indexOf(';')).toFloat();
        float max_val = poss_val.right(poss_val.size() - poss_val.indexOf(';') - 1).toFloat();
        float num = val.toFloat();
        if (min_val <= num && num <= max_val)
            return true;
        else return false;
    }
    if (type == "Качественный") {
        QRegularExpression regex(val+";*");
        if (poss_val.contains(";" + val + ";") || regex.match(poss_val).hasMatch()) {
            return true;
        } else return false;
    }
    return false;
}

void classifier_values::on_pushButtonBack_clicked()
{
    emit attrWindow();
    this->close();
}

void classifier_values::setAttrList(QList<QListWidgetItem *> l)
{
    //ui->tableWidget_attr->clear();
    ui->tableWidget_attr->setRowCount(0);
    QTableWidgetItem *twi;
    for (int i = 0; i < l.count(); i++) {
        twi = new QTableWidgetItem();
        twi->setText(l[i]->text());
        ui->tableWidget_attr->insertRow(i);
        ui->tableWidget_attr->setItem(i, 0, twi);
        ui->tableWidget_attr->setItem(i, 1, new QTableWidgetItem);
    }
}



void classifier_values::on_tableWidget_attr_cellDoubleClicked(int row, int column)
{
    if (v_dialogWindow->setDialog(row, ui->tableWidget_attr->item(row, 0)->text(), ui->tableWidget_attr->item(row, 1)->text())) {
        v_dialogWindow->show();
        this->close();
    }
}

void classifier_values::on_pushButtonNext_clicked()
{
    if (isValComplete()) {
        setDiagnosis();
        resultWindow->show();
        this->close();
    }
}

void classifier_values::on_pushButtonEdit_clicked()
{
    if (!ui->tableWidget_attr->selectedItems().isEmpty()) {
        int row = ui->tableWidget_attr->selectedItems().first()->row();
        if (v_dialogWindow->setDialog(row, ui->tableWidget_attr->item(row, 0)->text(), ui->tableWidget_attr->item(row, 1)->text())) {
            v_dialogWindow->show();
            this->close();
        }
    }
}
