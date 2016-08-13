#ifndef DEVICECONFIGXMLREADER_H
#define DEVICECONFIGXMLREADER_H

#include <QIcon>
#include <QDomDocument>
#include <QTabWidget>

class DeviceConfigXMLReader : public QObject
{
    Q_OBJECT
public:
    DeviceConfigXMLReader(QTabWidget *, QWidget *parent = 0);


public slots:
    void setupManufacturerPage(const QDomElement &element);
    void setupProductPage(const QDomElement &element);

private:
    bool read(QIODevice *device);
    bool write(QIODevice *device);
    bool setFieldsFromElement(const QDomElement &, QString, QString, QString);
    void doConfigurationParams(const QDomElement &element);

    QDomDocument domDocument;
    //QHash<QTreeWidgetItem *, QDomElement> domElementForItem;
    QIcon folderIcon;
    QIcon bookmarkIcon;
    QTabWidget *tabWidget;


};

#endif // DEVICECONFIGXMLREADER_H
