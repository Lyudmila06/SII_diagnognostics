#include "knowledge_editor.h"
#include "ui_knowledge_editor.h"


knowledge_editor::knowledge_editor(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::knowledge_editor)
{
    ui->setupUi(this);


    attr_editWindow = new attribute_editor();
    connect(attr_editWindow, &attribute_editor::mainEditor, this, &knowledge_editor::show);

    dis_editWindow = new disease_editor();
    connect(dis_editWindow, &disease_editor::mainEditor, this, &knowledge_editor::show);

    attr_possWindow = new attribute_possible_values();
    connect(attr_possWindow, &attribute_possible_values::mainEditor, this, &knowledge_editor::show);


}

knowledge_editor::~knowledge_editor()
{
    delete ui;
}

void knowledge_editor::on_pushButton_clicked()
{
    this->close();
    emit mainMenu();
}

void knowledge_editor::on_listWidget_itemDoubleClicked(QListWidgetItem *item)
{
    if (item->text() == "Признаки") {
        attr_editWindow->show();
        this->close();
    }
    if (item->text() == "Заболевания") {
        dis_editWindow->show();
        this->close();
    }
    if (item->text() == "Возможные значения признаков") {
        attr_possWindow->show();
        attr_possWindow->update_mModel();
        this->close();
    }
}
