#ifndef NODEFLAGSWIDGET_H
#define NODEFLAGSWIDGET_H

#include <QFrame>
#include <QBitArray>
#include <QStringLiteral>
#include <qt-openzwave/qtozwnodemodel.h>

namespace Ui {
class NodeFlagsWidget;
}

class NodeFlagsWidget : public QFrame
{
    Q_OBJECT

public:
    explicit NodeFlagsWidget(QWidget *parent = nullptr);
    ~NodeFlagsWidget();
    void setValue(QBitArray);

private:
    Ui::NodeFlagsWidget *ui;
    QBitArray m_value;
    QPixmap p_awake;
    QPixmap p_flirs;
    QPixmap p_failed;
    QPixmap p_beaming;
    QPixmap p_routing;
    QPixmap p_listening;
    QPixmap p_zwplus;
};

#endif // NODEFLAGSWIDGET_H
