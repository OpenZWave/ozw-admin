#ifndef NODESTATUS_H
#define NODESTATUS_H

#include <QWidget>
#include <QModelIndex>
#include <QModelIndex>
#include <QTimer>

class QTOZWManager;

namespace Ui {
class NodeStatus;
}

class NodeStatus : public QWidget
{
    Q_OBJECT

public:
    explicit NodeStatus(QWidget *parent = nullptr);
    ~NodeStatus();

public Q_SLOTS:
	void NodeSelected(QModelIndex, QModelIndex);
	
private:
	void updateNodeStats();


    Ui::NodeStatus *ui;
	QTimer m_statTimer;
	QModelIndex currentNode;
};

#endif // NODESTATUS_H
