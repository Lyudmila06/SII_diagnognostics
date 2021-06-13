#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "knowledge_editor.h"
#include "classifier_attr.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();


private slots:

    void on_editButton_clicked();

    void on_commandLinkButton_clicked();


    void on_classButton_clicked();

private:
    Ui::MainWindow *ui;
    knowledge_editor *editWindow;
    classifier_attr *classWindow;
};
#endif // MAINWINDOW_H
