#ifndef DISEASE_DESCRIPTION_H
#define DISEASE_DESCRIPTION_H

#include <QWidget>
#include <QSqlTableModel>
#include <QList>
#include <QListWidget>

namespace Ui {
class disease_description;
}

class disease_description : public QWidget
{
    Q_OBJECT

public:
    explicit disease_description(QWidget *parent = nullptr);
    ~disease_description();

    void update_mModels();

    void set_tabsUnenable();

    void clean_lineEdits();

    void clean_lines();

    void changeCurrentIndex();

    QString listToString(QList<QListWidgetItem*> l);

signals:
    void mainEditor();

private slots:
    void on_comboBox_currentIndexChanged(int index);

    void on_pushButtonBack_clicked();

    void on_pushButtonNext_clicked();

    void on_listWidget_itemClicked(QListWidgetItem *item);

private:
    Ui::disease_description *ui;
    QSqlTableModel *mModel1, *mModel2, *mModel3;
    QString val_1, val_2;
    float fval_1, fval_2;
    QString log_val;
};

#endif // DISEASE_DESCRIPTION_H
