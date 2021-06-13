#ifndef ATTRIBUTE_NORMAL_VALUES_H
#define ATTRIBUTE_NORMAL_VALUES_H

#include <QWidget>
#include <QSqlTableModel>
#include <QList>
#include <QListWidget>

namespace Ui {
class attribute_normal_values;
}

class attribute_normal_values : public QWidget
{
    Q_OBJECT

public:
    explicit attribute_normal_values(QWidget *parent = nullptr);
    ~attribute_normal_values();
    void update_mModel();

    void set_tabsUnenable();

    void clean_lineEdits();

    QString listToString(QList<QListWidgetItem*> l);

signals:
    void mainEditor();

private slots:
    void on_pushButtonBack_clicked();

    void on_pushButtonNext_clicked();

    void on_comboBox_currentIndexChanged(int index);

private:
    Ui::attribute_normal_values *ui;
    QSqlTableModel *mModel;
    QString val_1, val_2;
    float fval_1, fval_2;
    QString log_val;
};

#endif // ATTRIBUTE_NORMAL_VALUES_H
