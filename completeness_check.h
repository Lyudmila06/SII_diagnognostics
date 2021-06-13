#ifndef COMPLETENESS_CHECK_H
#define COMPLETENESS_CHECK_H

#include <QCommonStyle>
#include <QList>
#include <QListWidget>
#include <QSqlTableModel>
#include <QTableWidgetItem>
#include <QWidget>

namespace Ui {
class completeness_check;
}

class completeness_check : public QWidget
{
    Q_OBJECT

public:
    explicit completeness_check(QWidget *parent = nullptr);
    ~completeness_check();

    QString listToString(QList<QListWidgetItem*> l);

    QList<QString> StringToList(QString s);

    void clean_Tables();

    void update_Tables();

    void check_Attributes();

    void check_Diseases();

    void update_Models();

    QCommonStyle style;
    //QTableWidgetItem *status_success, *status_fail;
    //QIcon *status_success, *status_fail;

    QStyleOptionViewItem viewOptions() const;

signals:
    void mainEditor();

private slots:
    void on_pushButtonBack_clicked();

    void on_pushButtonCheck_clicked();

private:
    Ui::completeness_check *ui;
    QSqlTableModel *mModel1, *mModel2, *mModel3;
};

#endif // COMPLETENESS_CHECK_H
