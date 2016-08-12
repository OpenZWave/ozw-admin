#ifndef DEVICEDBXMLREADER_HPP
#define DEVICEDBXMLREADER_HPP


#include <QIcon>
#include <QXmlStreamReader>

class QTreeWidget;
class QTreeWidgetItem;

class DeviceDBXMLReader
{
public:
    DeviceDBXMLReader(QTreeWidget *treeWidget);

    bool read(QIODevice *device);

    QString errorString() const;

private:
    void readXBEL();
    void readProduct(QTreeWidgetItem *item);
    void readSeparator(QTreeWidgetItem *item);
    void readManufacturer(QTreeWidgetItem *item);
    void readBookmark(QTreeWidgetItem *item);

    QTreeWidgetItem *createChildItem(QTreeWidgetItem *item);

    QXmlStreamReader xml;
    QTreeWidget *treeWidget;

    QIcon folderIcon;
    QIcon bookmarkIcon;
};

#endif // DEVICEDBXMLREADER_HPP
