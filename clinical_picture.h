#ifndef CLINICAL_PICTURE_H
#define CLINICAL_PICTURE_H

#include <QWidget>
#include <QSqlTableModel>
#include <QListWidget>

namespace Ui {
class clinical_picture;
}

class clinical_picture : public QWidget
{
    Q_OBJECT

public:
    explicit clinical_picture(QWidget *parent = nullptr);

    void update_mModel1();

    void update_mModel2();

    void clean_lines();

    void fill_leftList();

    void changeCurrentIndex();

    QString listToString(QList<QListWidgetItem*> l);

    ~clinical_picture();

signals:
    void mainEditor();

private slots:
    void on_pushButtonBack_clicked();

    void on_pushButtonNext_clicked();

    void on_comboBox_currentIndexChanged(int index);

    void on_pushButtonLeft_clicked();

    void on_pushButtonRight_2_clicked();

    void on_pushButtonLeft_2_clicked();

    void on_pushButtonRight_clicked();

private:
    Ui::clinical_picture *ui;
    QSqlTableModel *mModel1;
    QSqlTableModel *mModel2;

};

#endif // CLINICAL_PICTURE_H
