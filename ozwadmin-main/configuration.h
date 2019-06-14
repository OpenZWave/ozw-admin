#ifndef CONFIGURATION_H
#define CONFIGURATION_H

#include <QDialog>
#include "qt-openzwave/qtozwoptions.h"

namespace Ui {
class Configuration;
}

class Configuration : public QDialog
{
    Q_OBJECT

public:
    explicit Configuration(QTOZWOptions *options, QWidget *parent = nullptr);
    ~Configuration();

private:
    Ui::Configuration *ui;
};

#endif // CONFIGURATION_H
