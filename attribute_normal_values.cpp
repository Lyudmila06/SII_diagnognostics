#include "attribute_normal_values.h"
#include "ui_attribute_normal_values.h"

attribute_normal_values::attribute_normal_values(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::attribute_normal_values)
{
    ui->setupUi(this);
}

attribute_normal_values::~attribute_normal_values()
{
    delete ui;
}
