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

    attr_normWindow = new attribute_normal_values();
    connect(attr_normWindow, &attribute_normal_values::mainEditor, this, &knowledge_editor::show);

    clinic_picWindow = new clinical_picture();
    connect(clinic_picWindow, &clinical_picture::mainEditor, this, &knowledge_editor::show);

    dis_descrWindow = new disease_description();
    connect(dis_descrWindow, &disease_description::mainEditor, this, &knowledge_editor::show);

    compl_checkWindow = new completeness_check();
    connect(compl_checkWindow, &completeness_check::mainEditor, this, &knowledge_editor::show);
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
    if (item->text() == "Нормальные значения признаков") {
        attr_normWindow->show();
        attr_normWindow->update_mModel();
        this->close();
    }
    if (item->text() == "Признаковое описание заболеваний") {
        clinic_picWindow->show();
        clinic_picWindow->update_mModel1();
        clinic_picWindow->update_mModel2();
        clinic_picWindow->changeCurrentIndex();
        this->close();
    }
    if (item->text() == "Значения признаков при заболевании") {
        dis_descrWindow->show();
        //dis_descrWindow->update_mModel();
        dis_descrWindow->update_mModels();
        dis_descrWindow->changeCurrentIndex();
        this->close();
    }
    if (item->text() == "Проверка целостности базы знаний") {
        compl_checkWindow->show();
        compl_checkWindow->update_Tables();
        //dis_descrWindow->update_mModel();
        this->close();
    }
}
