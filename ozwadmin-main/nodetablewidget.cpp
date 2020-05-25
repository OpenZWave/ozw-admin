
#include <QMenu>

#include <qt-openzwave/qtozwnodemodel.h>
#include <qt-openzwave/qtozwproxymodels.h>

#include "nodetablewidget.h"
#include "ui_nodetablewidget.h"
#include "node_delegate.h"


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
	//    this->ui->nodeList->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
	//    this->ui->nodeList->verticalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);

	this->ui->nodeList->setContextMenuPolicy(Qt::CustomContextMenu);
	connect(this->ui->nodeList, SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(rightClickMenu(QPoint)));
}

nodeTableWidget::~nodeTableWidget()
{
    delete ui;
}

void nodeTableWidget::setModel(QAbstractItemModel *model) 
{
	QTOZW_proxyNodeModel *nodeList = new QTOZW_proxyNodeModel(this);
	nodeList->setSourceModel(model);
	this->ui->nodeList->setModel(nodeList);
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
	QObject::connect(ui->nodeList->selectionModel(), &QItemSelectionModel::currentRowChanged, this, &nodeTableWidget::currentRowChanged);
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
    QModelIndex index=this->ui->nodeList->indexAt(pos);

    QMenu *menu=new QMenu(this);
    menu->addAction(new QAction("Action 1", this));
    menu->addAction(new QAction("Action 2", this));
    menu->addAction(new QAction("Action 3", this));
    menu->popup(this->ui->nodeList->viewport()->mapToGlobal(pos));
}