#include "mainwindow.h"
#include "attribute_editor.h"
#include <QApplication>
#include <QSqlDatabase>
#include <QSqlError>
#include <QMessageBox>
#include <QSortFilterProxyModel>

#define ACCESS "DRIVER={Microsoft Access Driver (*.mdb, *.accdb)};FIL={MS Access};" \
    "DBQ=C:\\Users\\Lyudmila\\Documents\\SII_diagnostics\\SII_medical_DB.accdb"
QSqlDatabase mDatabase;

int main(int argc, char *argv[])
{

    QApplication a(argc, argv);
    a.setStyle("fusion");
    mDatabase = QSqlDatabase::addDatabase("QODBC");
    mDatabase.setDatabaseName(ACCESS);
    if (!mDatabase.open()) {
            //QMessageBox::critical("Error", mDatabase.lastError().text());
            return a.exec();
        }
    MainWindow w;
    w.show();
    return a.exec();
}
