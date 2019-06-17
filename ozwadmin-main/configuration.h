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
    explicit Configuration(QTOZWOptions *options, QWidget *parent = nullptr);
    ~Configuration();
    void saveConfiguration();

private:
    Ui::Configuration *ui;
    QSettings settings;
    QtTreePropertyBrowser *m_variantEditor;
    QtVariantPropertyManager *variantManager;
    QtEnumPropertyManager *enumManager;
    QTOZWOptions *m_options;
    QMap<QString, QtProperty *> m_variantToProperty;
};

#endif // CONFIGURATION_H
