#ifndef KNOWLEDGE_EDITOR_H
#define KNOWLEDGE_EDITOR_H

#include <QWidget>
#include <QListWidgetItem>
#include  "attribute_editor.h"
#include "disease_editor.h"
#include "attribute_possible_values.h"



namespace Ui {
class knowledge_editor;
}

class knowledge_editor : public QWidget
{
    Q_OBJECT

signals:
    void mainMenu();

public:
    explicit knowledge_editor(QWidget *parent = nullptr);
    ~knowledge_editor();

private slots:
    void on_pushButton_clicked();

    void on_listWidget_itemDoubleClicked(QListWidgetItem *item);

private:
    Ui::knowledge_editor *ui;
    attribute_editor *attr_editWindow;
    disease_editor *dis_editWindow;
    attribute_possible_values *attr_possWindow;
};

#endif // KNOWLEDGE_EDITOR_H
