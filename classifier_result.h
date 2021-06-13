#ifndef CLASSIFIER_RESULT_H
#define CLASSIFIER_RESULT_H

#include <QTableWidget>
#include <QWidget>

namespace Ui {
class classifier_result;
}

class classifier_result : public QWidget
{
    Q_OBJECT

public:
    explicit classifier_result(QWidget *parent = nullptr);
    ~classifier_result();

    QTableWidget* getResultTable();

    void cleanPanels();

    void addDiagnosistoList(QString d);

    void addMsgtoList(QString s);

    void addDiagnosistoList2(QString d);

signals:
    void attrValWindow();

private slots:
    void on_pushButton_clicked();

private:
    Ui::classifier_result *ui;
};

#endif // CLASSIFIER_RESULT_H
