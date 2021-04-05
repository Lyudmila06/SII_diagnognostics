#include "mainwindow.h"
#include "./ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    editWindow = new knowledge_editor();
    connect(editWindow, &knowledge_editor::mainMenu, this, &MainWindow::show);
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_editButton_clicked()
{
    editWindow->show();
    this->close();
}

void MainWindow::on_commandLinkButton_clicked()
{
    QApplication::quit();
}
