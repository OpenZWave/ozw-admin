/*  ozw-admin - Gui for OpenZWave
 *    Copyright (C) 2016  Justin Hammond <justin@dynam.ac>
 *
 *    This program is free software: you can redistribute it and/or modify
 *    it under the terms of the GNU General Public License as published by
 *    the Free Software Foundation, either version 3 of the License, or
 *    (at your option) any later version.
 *
 *    This program is distributed in the hope that it will be useful,
 *    but WITHOUT ANY WARRANTY; without even the implied warranty of
 *    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *    GNU General Public License for more details.
 *
 *    You should have received a copy of the GNU General Public License
 *    along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
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
    Q_PROPERTY(bool ready READ isReady WRITE setReady)

public:
    explicit DeviceDB(QWidget *parent = 0);
    ~DeviceDB();
    bool isReady() const;
    void setReady(bool ready);

public slots:
    void doProductPage(QTreeWidgetItem *);
private:
    bool LoadXML();
    Ui::DeviceDB *ui;
    DeviceDBXMLReader *deviceTree;
    DeviceConfigXMLReader *deviceDetails;
    bool m_Ready;
    QString m_Path;
};

#endif // DEVICEDB_HPP
