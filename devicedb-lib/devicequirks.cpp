#include "devicequirks.h"
#include <QFile>
#include <QDebug>
#include <QMessageBox>
#include <QDir>


bool parseNumberList(QString str, QList<uint8_t> &list) {
    size_t pos = 0;
    size_t start = 0;
    bool parsing = true;
    while( parsing )
    {
        std::string ccStr;

        pos = str.toStdString().find_first_of( ",", start );
        if( std::string::npos == pos )
        {
            ccStr = str.toStdString().substr( start );
            parsing = false;
        }
        else
        {
            ccStr = str.toStdString().substr( start, pos-start );
            start = pos + 1;
        }
        QString result = QString::fromStdString(ccStr);
        list.push_back(result.toInt());
    }
    return true;
}

DeviceQuirks::DeviceQuirks(QObject *parent) : QObject(parent)
{
    QString errorStr;
    int errorLine;
    int errorColumn;

    qDebug() << QDir(".").absolutePath();
    QFile mfxml("Quirks.xml");
    if (!mfxml.open(QFile::ReadOnly | QFile::Text)) {
            QMessageBox::warning(nullptr, tr("QXmlStream Quirks"),
                                 tr("Cannot read file %1:\n%2.")
                                 .arg(mfxml.fileName())
                                 .arg(mfxml.errorString()));
            this->setReady(false);
            return;
    }
    if (!domDocument.setContent(&mfxml, false,&errorStr, &errorLine, &errorColumn)) {
        QMessageBox::information(nullptr, tr("Quirks Database"), tr("Parse Error at line %1, column %2:\n%3")
                                 .arg(errorLine)
                                 .arg(errorColumn)
                                 .arg(errorStr));
        return;
    }

    QDomElement root = domDocument.documentElement();
    if (root.tagName() != "QuirkData") {
        QMessageBox::information(nullptr, tr("Quirks Database"),
                                 tr("The file is not an Quirk file."));
        return;
    }

    QDomElement child = root.firstChildElement("Quirk");
    while (!child.isNull()) {
        QString name = child.attribute("name");
        QuirksEntry *entry = new QuirksEntry();
        if (child.hasAttribute("CommandClass"))
            parseNumberList(child.attribute("CommandClass"), entry->CommandClasses);
        if (child.hasChildNodes()) {
            QDomNode options = child.firstChild();
            while (!options.isNull()) {
                if (options.nodeName().toUpper() == "OPTION")
                    entry->options.push_back(options.toElement().attribute("name"));
                else if (options.nodeName().toUpper() == "HELP")
                    entry->Help = options.firstChild().toText().data();
                options = options.nextSibling();
            }
        }
        this->m_quirks.insert(name, entry);
        child = child.nextSiblingElement();
    }

    mfxml.close();
    this->setReady(true);
    this->dump();
}

DeviceQuirks &DeviceQuirks::GetInstance() {
    static DeviceQuirks instance;
    return instance;
}

void DeviceQuirks::setReady(bool ready) {
    this->m_Ready = ready;
}
bool DeviceQuirks::isReady() const {
    return this->m_Ready;
}

void DeviceQuirks::dump() {
    QHash<QString, QuirksEntry*>::iterator it;
    for (it = this->m_quirks.begin(); it != this->m_quirks.end(); ++it) {
        qDebug() << it.key();
        qDebug() << "\tOptions:";
        for (int i = 0; i < it.value()->options.count(); ++i)
            qDebug() << "\t\t" << it.value()->options.at(i);
        qDebug() << "\tCommandClasses:";
        for (int i = 0; i < it.value()->CommandClasses.count(); ++i)
            qDebug() << "\t\t" << it.value()->CommandClasses.at(i);
        qDebug() << "\tHelp:";
        qDebug() << "\t\t" << it.value()->Help;
    }
}




