#include <QGroupBox>
#include <QCheckBox>
#include <QBitArray>
#include <QVBoxLayout>
#include <QVariant>
#include <QDebug>
#include "bitsetwidget.h"

BitSetWidget::BitSetWidget(QWidget *parent) :
    QGroupBox(parent)
{
    setAutoFillBackground(true);
    //QGroupBox *groupBox = new QGroupBox;
    QVBoxLayout *vbox = new QVBoxLayout;
    vbox->setSpacing(0);
    vbox->setMargin(0);

    //groupBox->setSpacing(0);
    //groupBox->setMargin(0);
    this->setLayout(vbox);
    //this->setCentralWidget(groupBox);
}

void BitSetWidget::setValue(QtValueID *vid) {
    QBitArray ba = vid->getValue().toBitArray();
    this->setTitle(vid->GetLabel());
    for (int i = 0; i < ba.count(); ++i) {
        QCheckBox *cb = new QCheckBox(vid->GetLabel(i), this);
        cb->setChecked(ba.at(i));
        cb->setProperty("index", i);
        this->layout()->addWidget(cb);
        qDebug() <<"done " << i;
    }
}
