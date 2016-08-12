#ifndef DEVICEDB_HPP
#define DEVICEDB_HPP

#include <QMainWindow>

namespace Ui {
class DeviceDB;
}

class DeviceDB : public QMainWindow
{
    Q_OBJECT

public:
    explicit DeviceDB(QWidget *parent = 0);
    ~DeviceDB();

private:
    Ui::DeviceDB *ui;
};

#endif // DEVICEDB_HPP
