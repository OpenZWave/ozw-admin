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
#ifndef DEVICEDBXMLREADER_HPP
#define DEVICEDBXMLREADER_HPP


#include <QIcon>
#include <QDomDocument>
#include <QTreeWidget>

class QTreeWidgetItem;
class QTabWidget;


enum UserDataRoles {
    ItemType = Qt::UserRole,
    Manufacturer_ID,
    Manufacturer_Name,
    Product_Type,
    Product_ID,
    Product_Name,
    Product_Config
};

class DeviceDBXMLReader: public QTreeWidget
{
    Q_OBJECT
public:
    DeviceDBXMLReader(QWidget *parent = 0);
    bool read(QIODevice *device);
    bool write(QIODevice *device);
    void dump();
signals:
    void setupManufacturerPage(QDomNode &node);
    void setupProductPage(QDomNode &node);
private slots:
    void updateDomElement(QTreeWidgetItem *item, int column);
    void updateSelection();

private:
    void readManufacturer(const QDomNode &node, QTreeWidgetItem *parentItem  =0);
    QTreeWidgetItem *createItem(const QDomNode &node,
                                QTreeWidgetItem *parentItem = 0);

    QDomDocument domDocument;
    QHash<QTreeWidgetItem *, QDomNode> domNodeForItem;
    QIcon folderIcon;
    QIcon bookmarkIcon;
};

#endif // DEVICEDBXMLREADER_HPP
