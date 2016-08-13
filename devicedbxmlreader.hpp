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

signals:
    void setupManufacturerPage(const QDomElement &element);
    void setupProductPage(const QDomElement &element);
private slots:
    void updateDomElement(QTreeWidgetItem *item, int column);
    void updateSelection();

private:
    void readManufacturer(const QDomElement &element, QTreeWidgetItem *parentItem  =0);
    QTreeWidgetItem *createItem(const QDomElement &element,
                                QTreeWidgetItem *parentItem = 0);

    QDomDocument domDocument;
    QHash<QTreeWidgetItem *, QDomElement> domElementForItem;
    QIcon folderIcon;
    QIcon bookmarkIcon;
};

#endif // DEVICEDBXMLREADER_HPP
