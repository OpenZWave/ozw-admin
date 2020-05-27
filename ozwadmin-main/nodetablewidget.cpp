
#include <QMenu>

#include <qt-openzwave/qtozwnodemodel.h>
#include <qt-openzwave/qtozwproxymodels.h>

#include "nodetablewidget.h"
#include "ui_nodetablewidget.h"
#include "node_delegate.h"
#include "util.h"


nodeTableWidget::nodeTableWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::nodeTableWidget)
{
    this->ui->setupUi(this);
	Node_Delegate *nodeflagdelegate = new Node_Delegate(this);

	this->ui->nodeList->setItemDelegateForColumn(QTOZW_Nodes::NodeColumns::NodeFlags, nodeflagdelegate);
	this->ui->nodeList->setSelectionBehavior(QAbstractItemView::SelectRows);
	this->ui->nodeList->resizeColumnsToContents();
	this->ui->nodeList->verticalHeader()->hide();
	this->ui->nodeList->setSelectionMode(QAbstractItemView::SingleSelection);
	this->ui->nodeList->setSortingEnabled(true);
	this->ui->nodeList->horizontalHeader()->setSectionsMovable(true);

	this->ui->nodeList->setContextMenuPolicy(Qt::CustomContextMenu);
	connect(this->ui->nodeList, SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(rightClickMenu(QPoint)));
}

nodeTableWidget::~nodeTableWidget()
{
    delete ui;
}

void nodeTableWidget::setModel(QAbstractItemModel *model) 
{
	QTOZW_proxyNodeModel *proxyNodeList = new QTOZW_proxyNodeModel(this);
	proxyNodeList->setSourceModel(model);
	this->ui->nodeList->setModel(proxyNodeList);
	for (int i = 0; i <= QTOZW_Nodes::NodeColumns::NodeCount; i++) {
		switch (i) {
		case QTOZW_Nodes::NodeColumns::NodeID:
		case QTOZW_Nodes::NodeColumns::NodeName:
		case QTOZW_Nodes::NodeColumns::NodeProductName:
		case QTOZW_Nodes::NodeColumns::NodeFlags:
			break;
		default:
			this->ui->nodeList->horizontalHeader()->hideSection(i);
		}
	}
	connect(ui->nodeList->selectionModel(), &QItemSelectionModel::currentRowChanged, this, &nodeTableWidget::currentRowChanged);
	connect(proxyNodeList, &QTOZW_proxyNodeModel::rowsInserted, this, &nodeTableWidget::resizeContents);
	connect(proxyNodeList, &QTOZW_proxyNodeModel::rowsRemoved, this, &nodeTableWidget::resizeContents);
	QTimer::singleShot(100, this, &nodeTableWidget::resizeContents);
}

QModelIndex nodeTableWidget::currentIndex()
{
	return this->ui->nodeList->currentIndex();
}
QItemSelectionModel *nodeTableWidget::selectionModel()
{
	return this->ui->nodeList->selectionModel();
}

void nodeTableWidget::rightClickMenu(QPoint pos) 
{
    //QModelIndex index=this->ui->nodeList->indexAt(pos);

    QMenu *menu=new QMenu(this);
    menu->addAction(new QAction("Action 1", this));
    menu->addAction(new QAction("Action 2", this));
    menu->addAction(new QAction("Action 3", this));
    menu->popup(this->ui->nodeList->viewport()->mapToGlobal(pos));
}

void nodeTableWidget::resizeContents() {
	this->ui->nodeList->resizeColumnsToContents();
}