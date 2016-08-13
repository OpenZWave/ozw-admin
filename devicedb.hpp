#ifndef DEVICEDB_HPP
#define DEVICEDB_HPP

#include <QMainWindow>

#include "devicedbxmlreader.hpp"
#include "deviceconfigxmlreader.h"

namespace Ui {
class DeviceDB;
}

class DeviceDB : public QMainWindow
{
    Q_OBJECT

public:
    explicit DeviceDB(QWidget *parent = 0);
    ~DeviceDB();
public slots:
    void doProductPage(QTreeWidgetItem *);
private:
    Ui::DeviceDB *ui;
    DeviceDBXMLReader *deviceTree;
    DeviceConfigXMLReader *deviceDetails;
};

#endif // DEVICEDB_HPP
