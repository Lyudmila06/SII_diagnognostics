#ifndef ATTRIBUTE_NORMAL_VALUES_H
#define ATTRIBUTE_NORMAL_VALUES_H

#include <QWidget>

namespace Ui {
class attribute_normal_values;
}

class attribute_normal_values : public QWidget
{
    Q_OBJECT

public:
    explicit attribute_normal_values(QWidget *parent = nullptr);
    ~attribute_normal_values();

private:
    Ui::attribute_normal_values *ui;
};

#endif // ATTRIBUTE_NORMAL_VALUES_H
