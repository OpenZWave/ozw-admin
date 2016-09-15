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
    void setPath(QString path);


public slots:
    void setupManufacturerPage(const QDomElement &element);
    void setupProductPage(const QDomElement &element);

private:
    bool read(QIODevice *device);
    bool write(QIODevice *device);
    bool setFieldsFromElement(const QDomElement &, QString, QString, QString);
    void doConfigurationParams(const QDomElement &element);
    void doAssociations(const QDomElement &element);
    void doQuirks(const QDomElement &element);

    QDomDocument domDocument;
    //QHash<QTreeWidgetItem *, QDomElement> domElementForItem;
    QIcon folderIcon;
    QIcon bookmarkIcon;
    QTabWidget *tabWidget;
    QString m_Path;


};

#endif // DEVICECONFIGXMLREADER_H
