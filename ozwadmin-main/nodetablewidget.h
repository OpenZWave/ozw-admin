#ifndef NODETABLEWIDGET_H
#define NODETABLEWIDGET_H

#include <QWidget>
#include <QPoint>
#include <qabstractitemmodel.h>
#include <qitemselectionmodel.h>
#include <QToolBar>


namespace Ui {
class nodeTableWidget;
}

class nodeTableWidget : public QWidget
{
    Q_OBJECT

public:
    explicit nodeTableWidget(QWidget *parent = nullptr);
    ~nodeTableWidget();
	void setModel(QAbstractItemModel *model);
	QModelIndex currentIndex();
	QItemSelectionModel *selectionModel();
Q_SIGNALS:
	void currentRowChanged(const QModelIndex &current, const QModelIndex &previous);
    void refreshNodeInfo(const quint8 node);
public Q_SLOTS:
    void rightClickMenu(QPoint pos);
private slots:
	void resizeContents();
    void updateSelectedNode(const QModelIndex &current, const QModelIndex &previous);
    void refreshNodeInfoTriggered();

private:
    Ui::nodeTableWidget *ui;
    QToolBar tb;
};

#endif // NODETABLEWIDGET_H
