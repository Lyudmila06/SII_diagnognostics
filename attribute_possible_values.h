#ifndef ATTRIBUTE_POSSIBLE_VALUES_H
#define ATTRIBUTE_POSSIBLE_VALUES_H

#include <QWidget>
#include <QSqlTableModel>

namespace Ui {
class attribute_possible_values;
}

class attribute_possible_values : public QWidget
{
    Q_OBJECT

public:
    explicit attribute_possible_values(QWidget *parent = nullptr);
    ~attribute_possible_values();
    void update_mModel();

    void clean_lineEdits();

    bool checkQualVal(QString s);

    void deleteDisAttr(QString name);

signals:
    void mainEditor();

private slots:

    void on_pushButtonBack_clicked();

    void on_pushButtonNext_clicked();


    void on_comboBox_currentIndexChanged(int index);

private:
    Ui::attribute_possible_values *ui;
    QSqlTableModel *mModel;
    QSqlTableModel *mModel2;
};

#endif // ATTRIBUTE_POSSIBLE_VALUES_H
