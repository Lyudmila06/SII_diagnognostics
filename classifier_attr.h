#ifndef CLASSIFIER_ATTR_H
#define CLASSIFIER_ATTR_H

#include <QList>
#include <QListWidget>
#include <QSqlTableModel>
#include <QString>
#include <QWidget>
#include "classifier_values.h"

namespace Ui {
class classifier_attr;
}

class classifier_attr : public QWidget
{
    Q_OBJECT

public:
    explicit classifier_attr(QWidget *parent = nullptr);
    ~classifier_attr();

    void update_mModel();

    void clean_lines();

    void fill_leftList();

    QString listToString(QList<QListWidgetItem*> l);

    bool checkAttr();

signals:
    void mainMenu();

private slots:
    void on_pushButtonBack_clicked();

    void on_pushButtonNext_clicked();

    void on_pushButtonRight_2_clicked();

    void on_pushButtonRight_clicked();

    void on_pushButtonLeft_clicked();

    void on_pushButtonLeft_2_clicked();

private:
    Ui::classifier_attr *ui;

    QSqlTableModel *mModel1;
    classifier_values *classvalWindow;
};

#endif // CLASSIFIER_ATTR_H
