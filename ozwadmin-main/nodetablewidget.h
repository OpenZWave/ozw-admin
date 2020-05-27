#ifndef NODETABLEWIDGET_H
#define NODETABLEWIDGET_H

#include <QWidget>
#include <QPoint>
#include <qabstractitemmodel.h>
#include <qitemselectionmodel.h>


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
public Q_SLOTS:
    void rightClickMenu(QPoint pos);
private slots:
	void resizeContents();

private:
    Ui::nodeTableWidget *ui;
};

#endif // NODETABLEWIDGET_H
