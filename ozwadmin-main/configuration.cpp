#include <QDate>
#include <QDebug>

#include "configuration.h"
#include "ui_configuration.h"

#include "propertybrowser/qtvariantproperty.h"
#include "propertybrowser/qttreepropertybrowser.h"


Configuration::Configuration(QTOZWOptions *options, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Configuration)
{
    ui->setupUi(this);

    QtVariantEditorFactory *variantFactory = new QtVariantEditorFactory();
    QtVariantPropertyManager *variantManager = new QtVariantPropertyManager();


    QtTreePropertyBrowser *variantEditor = new QtTreePropertyBrowser();
    variantEditor->setFactoryForManager(variantManager, variantFactory);
    variantEditor->setPropertiesWithoutValueMarked(true);
    variantEditor->setRootIsDecorated(false);

    QtProperty *topItem = variantManager->addProperty(QtVariantPropertyManager::groupTypeId(),
                QLatin1String(" Paths"));

    QtVariantProperty *item = variantManager->addProperty(QVariant::String, "ConfigPath");
    item->setValue(options->ConfigPath());
    topItem->addSubProperty(item);

    item = variantManager->addProperty(QVariant::String, "UserPath");
    item->setValue(options->UserPath());
    topItem->addSubProperty(item);

    variantEditor->addProperty(topItem);

    topItem = variantManager->addProperty(QtVariantPropertyManager::groupTypeId(),
                                          QLatin1String(" Blah"));


    variantEditor->addProperty(topItem);


    this->ui->config_ozw->layout()->addWidget(variantEditor);

}

Configuration::~Configuration()
{
    delete ui;
}
