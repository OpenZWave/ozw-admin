#ifndef BITSETWIDGET_H
#define BITSETWIDGET_H

#include <QWidget>
#include <QGroupBox>
#include "valueid.h"

class BitSetWidget : public QGroupBox
{
    Q_OBJECT
public:
    BitSetWidget(QWidget *parent = NULL);
    void setValue(QtValueID *);
};

#endif // BITSETWIDGET_H
