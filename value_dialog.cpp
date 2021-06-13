#include "value_dialog.h"
#include "ui_value_dialog.h"

#include <QMessageBox>
#include <QSqlRecord>

value_dialog::value_dialog(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::value_dialog)
{
    ui->setupUi(this);

    mModel = new QSqlTableModel(this);
    mModel->setTable("Attributes");
    mModel->select();

}

value_dialog::~value_dialog()
{
    delete ui;
}

void value_dialog::clean_lineEdits()
{
    ui->radioButtonTrue->setChecked(false);
    ui->radioButtonFalse->setChecked(false);
    ui->radioButtonFalse->setText("False");
    ui->radioButtonTrue->setText("True");
    ui->lineEditNum->setText(nullptr);
    ui->label_num_poss->setText("Возможные значения: ");
    ui->listWidgetQual->clear();
}

bool value_dialog::setDialog(int attr_ind, QString attr_name, QString attr_val)
{
    bool isfull = true;
    attr_index = attr_ind;
    clean_lineEdits();
    ui->tabWidget->setVisible(true);
    //mModel->select();
    ui->label->setText("Выберите значение признака " + attr_name);
    mModel->setFilter("Наименование = '" + attr_name + "'");
    mModel->select();
    if (mModel->rowCount() != 1) {
        QMessageBox::critical(this, "Error", QString::number(mModel->rowCount()) + " Отсутствует признак " + attr_name);
        return false;
    }
    QSqlRecord record = mModel->record(0);

    if (record.value(2).isNull()) {
        QMessageBox::critical(this, "Error", "Отсутствует нормальное значение признака " + attr_name);
        return false;
    }

    if (record.value(1).toString() == "Логический") {
        ui->tabWidget->setCurrentIndex(0);
        ui->tabWidget->setTabEnabled(0, true);
        ui->tabWidget->setTabEnabled(1, false);
        ui->tabWidget->setTabEnabled(2, false);
        //ui->tab_log->setVisible(true);
        QString values = record.value(2).toString();
        if (values != nullptr) {
            QString val_1 = values.left(values.indexOf(';'));
            QString val_2 = values.right(values.size() - values.indexOf(';') - 1);
            ui->radioButtonTrue->setText(val_1);
            ui->radioButtonFalse->setText(val_2);
            //ui->lineEditFalse->setText(values.right(values.size() - values.indexOf(';') - 1));
        }
        //устанавливаются уже введенные значения
        if (!attr_val.isEmpty()) {
            if (attr_val == ui->radioButtonFalse->text())
                ui->radioButtonFalse->setChecked(true);
            if (attr_val == ui->radioButtonTrue->text())
                ui->radioButtonTrue->setChecked(true);
        }
    }

    else if (record.value(1).toString() == "Количественный") {
        ui->tabWidget->setCurrentIndex(1);
        ui->tabWidget->setTabEnabled(0, false);
        ui->tabWidget->setTabEnabled(1, true);
        ui->tabWidget->setTabEnabled(2, false);
//        ui->tab_num->setVisible(true);
        ui->label_num_poss->setText("Возможные значения: [" + mModel->record(0).value(2).toString() + "]");
        //устанавливаются уже введенные значения
        if (!attr_val.isEmpty()) {
            ui->lineEditNum->setText(attr_val);
        }
        ui->lineEditNum->setFocus();
    }

    else if (record.value(1).toString() == "Качественный") {
        ui->tabWidget->setCurrentIndex(2);
        ui->tabWidget->setTabEnabled(0, false);
        ui->tabWidget->setTabEnabled(1, false);
        ui->tabWidget->setTabEnabled(2, true);
//        ui->tab_qual->setVisible(true);
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
        //устанавливаются уже введенные значения
        if (!attr_val.isEmpty()) {
            ui->listWidgetQual->findItems(attr_val, Qt::MatchExactly).takeFirst()->setSelected(true);
        }

    }
    else {
//        ui->tabWidget->setTabEnabled(0, false);
//        ui->tabWidget->setTabEnabled(1, false);
//        ui->tabWidget->setTabEnabled(2, false);
//        ui->tabWidget->setVisible(false);
        QMessageBox::critical(this, "Error", "Отсутствует тип признака " + attr_name);
        isfull = false;
    }
    return isfull;
}

void value_dialog::on_pushButtonBack_clicked()
{
    emit attrWindow();
    this->close();
}

void value_dialog::on_pushButtonOK_clicked()
{
    QString result;
    if (ui->tabWidget->isTabEnabled(0)) {
        if (ui->radioButtonTrue->isChecked()) {
            result = ui->radioButtonTrue->text();
        } else if (ui->radioButtonFalse->isChecked()) {
            result = ui->radioButtonFalse->text();
        }
    } else if (ui->tabWidget->isTabEnabled(1)) {
        if (!ui->lineEditNum->text().isEmpty()) {
            bool isnum = new bool;
            float r_num = ui->lineEditNum->text().toFloat(&isnum);
            if (isnum){
                QString values = mModel->record(0).value(2).toString();
                float min_val = values.left(values.indexOf(';')).toFloat();
                float max_val = values.right(values.size() - values.indexOf(';') - 1).toFloat();
                if (r_num > max_val || r_num < min_val) {
                    QMessageBox::critical(this, "Количественные тип", "Введенное значение находится за пределами интервала допустипых значений");
                    return;
                } else {
                    result = ui->lineEditNum->text();
                }
            } else {
                QMessageBox::critical(this, "Количественные тип", "Введено не числовое значение");
                return;
            }
        }
    } else if (ui->tabWidget->isTabEnabled(2)) {
        if (!ui->listWidgetQual->selectedItems().isEmpty())
            result = ui->listWidgetQual->selectedItems().takeFirst()->text();
    }

    emit setVal(attr_index, result);
    emit attrWindow();
    clean_lineEdits();
    this->close();
}
