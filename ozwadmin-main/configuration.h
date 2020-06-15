#ifndef CONFIGURATION_H
#define CONFIGURATION_H

#include <QDialog>
#include <QSettings>
#include "qt-openzwave/qtozwoptions.h"
#include "propertybrowser/qttreepropertybrowser.h"
#include "propertybrowser/qtvariantproperty.h"
#include "propertybrowser/qtpropertymanager.h"


namespace Ui {
class Configuration;
}

class Configuration : public QDialog
{
    Q_OBJECT

public:
    explicit Configuration(QWidget *parent = nullptr);
    ~Configuration();
    void saveConfiguration();

private:
    QtVariantProperty *addStringProperty(QString name, QString value, bool readonly);
    QtVariantProperty *addBoolProperty(QString name, bool value, bool readonly);
    QtVariantProperty *addIntProperty(QString name, int value, bool readonly);
    QtProperty *addEnumProperty(QString name, QStringList options, int selected, bool readonly);

private:
    Ui::Configuration *ui;
    QSettings settings;
    QtVariantPropertyManager *m_variantManager;
    QtEnumPropertyManager *m_enumManager;

    QtTreePropertyBrowser *m_OZWEditor;
    QtTreePropertyBrowser *m_NetworkEditor;


    QMap<QString, QtProperty *> m_variantToProperty;
    QTOZWOptions *m_options;
};

#endif // CONFIGURATION_H
