#ifndef CLASSIFIER_VALUES_H
#define CLASSIFIER_VALUES_H

#include <QList>
#include <QListWidgetItem>
#include <QTableWidget>
#include <QWidget>
#include "value_dialog.h"
#include "classifier_result.h"

namespace Ui {
class classifier_values;
}

class classifier_values : public QWidget
{
    Q_OBJECT

signals:
    void attrWindow();

public:
    explicit classifier_values(QWidget *parent = nullptr);
    ~classifier_values();

    void setAttrList(QList<QListWidgetItem *> l);

    void setValue(int ind, QString val);

    void setDiagnosis();

    bool isValComplete();

    bool checkEntry(QString type, QString val, QString poss_val);

private slots:
    void on_pushButtonBack_clicked();

    void on_tableWidget_attr_cellDoubleClicked(int row, int column);

    void on_pushButtonNext_clicked();

    void on_pushButtonEdit_clicked();

private:
    Ui::classifier_values *ui;
    value_dialog *v_dialogWindow;
    QSqlTableModel *mModel1, *mModel2, *mModel3;
    //QTableWidget *result_table;
    classifier_result *resultWindow;
};

#endif // CLASSIFIER_VALUES_H
