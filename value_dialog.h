#ifndef VALUE_DIALOG_H
#define VALUE_DIALOG_H

#include <QListWidget>
#include <QSqlTableModel>
#include <QWidget>

namespace Ui {
class value_dialog;
}

class value_dialog : public QWidget
{
    Q_OBJECT

public:
    explicit value_dialog(QWidget *parent = nullptr);
    ~value_dialog();

    bool setDialog(int attr_ind, QString attr_name, QString attr_val);

    void clean_lineEdits();

    QString listToString(QList<QListWidgetItem*> l);

signals:
    void attrWindow();

    void setVal(int ind, QString val);

private slots:
    void on_pushButtonBack_clicked();

    void on_pushButtonOK_clicked();

private:
    Ui::value_dialog *ui;
    QSqlTableModel *mModel;
    int attr_index;
};

#endif // VALUE_DIALOG_H
