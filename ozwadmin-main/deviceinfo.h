#ifndef DEVICEINFO_H
#define DEVICEINFO_H

#include <QWidget>
#include <QModelIndex>

class QTOZWManager;

namespace Ui {
class DeviceInfo;
}

class DeviceInfo : public QWidget
{
    Q_OBJECT

public:
    explicit DeviceInfo(QWidget *parent = nullptr);
    ~DeviceInfo();

	void setQTOZWManager(QTOZWManager *manager);
public Q_SLOTS:
	void NodeSelected(QModelIndex, QModelIndex);

Q_SIGNALS:
	void openMetaDataWindow();
private:
    Ui::DeviceInfo *ui;
	QTOZWManager *m_qtozwmanager;
};

#endif // DEVICEINFO_H
