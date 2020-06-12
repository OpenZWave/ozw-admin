#include <QCheckBox>
#include <QDebug>
#include <QSvgWidget>
#include <QLabel>
#include <QPixmap>

#include "nodeflagswidget.h"
#include "ui_nodeflagswidget.h"

NodeFlagsWidget::NodeFlagsWidget(QWidget *parent) :
    QFrame(parent),
    ui(new Ui::NodeFlagsWidget),
    p_awake(QStringLiteral(":/res/nodeflags/Awake.svg")),
    p_flirs(QStringLiteral(":/res/nodeflags/Flirs.svg")),
    p_failed(QStringLiteral(":/res/nodeflags/Dead.svg")),
    p_beaming(QStringLiteral(":/res/nodeflags/Beaming.svg")),
    p_routing(QStringLiteral(":/res/nodeflags/Routing.svg")),
    p_listening(QStringLiteral(":/res/nodeflags/Listening.svg")),
    p_zwplus(QStringLiteral(":/res/nodeflags/zwaveplus.png"))
{
    ui->setupUi(this);

    ui->awake->setPixmap(p_awake.scaled(ui->awake->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation));
    QSizePolicy ap_retain = ui->awake->sizePolicy();
    ui->awake->setSizePolicy(ap_retain);

    ui->flirs->setPixmap(p_flirs.scaled(ui->flirs->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation));
    QSizePolicy fp_retain = ui->flirs->sizePolicy();
    ui->flirs->setSizePolicy(fp_retain);

    ui->failed->setPixmap(p_failed.scaled(ui->failed->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation));
    QSizePolicy f1p_retain = ui->failed->sizePolicy();
    ui->failed->setSizePolicy(f1p_retain);

    ui->zwplus->setPixmap(p_zwplus.scaled(ui->zwplus->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation));
    QSizePolicy zp_retain = ui->zwplus->sizePolicy();
    ui->zwplus->setSizePolicy(zp_retain);

    ui->beaming->setPixmap(p_beaming.scaled(ui->beaming->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation));
    QSizePolicy bp_retain = ui->beaming->sizePolicy();
    ui->beaming->setSizePolicy(bp_retain);

    ui->routing->setPixmap(p_routing.scaled(ui->routing->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation));
    QSizePolicy rp_retain = ui->routing->sizePolicy();
    ui->routing->setSizePolicy(rp_retain);

    ui->listening->setPixmap(p_listening.scaled(ui->listening->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation));
    QSizePolicy lp_retain = ui->listening->sizePolicy();
    ui->listening->setSizePolicy(lp_retain);
}

NodeFlagsWidget::~NodeFlagsWidget()
{
    delete ui;
}

void NodeFlagsWidget::setValue(QBitArray value) {
    this->m_value = value;
    for (int i = 0; i < this->m_value.size(); i++) {
        switch (i) {
            case QTOZW_Nodes::nodeFlags::isAwake:
                {
                    this->ui->awake->setVisible(value.at(i));
                    break;
                }
            case QTOZW_Nodes::nodeFlags::isFlirs:
                {
                    this->ui->flirs->setVisible(value.at(i));
                    break;
                }
            case QTOZW_Nodes::nodeFlags::isFailed:
                {
                    this->ui->failed->setVisible(value.at(i));
                    break;
                }
            case QTOZW_Nodes::nodeFlags::isBeaming:
                {
                    this->ui->beaming->setVisible(value.at(i));
                    break;
                }
            case QTOZW_Nodes::nodeFlags::isRouting:
                {
                    this->ui->routing->setVisible(value.at(i));
                    break;
                }
            case QTOZW_Nodes::nodeFlags::isListening:
                {
                    this->ui->listening->setVisible(value.at(i));
                    break;
                }
            case QTOZW_Nodes::nodeFlags::isZWavePlus:
                {
                    this->ui->zwplus->setVisible(value.at(i));
                    break;
                }
            default:
            {
                break;
            }
        }

    }
}

