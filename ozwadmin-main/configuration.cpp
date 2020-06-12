#include <QDate>
#include <QDebug>

#include "configuration.h"
#include "ui_configuration.h"
#include "ozwcore.h"
#include "util.h"

#include "propertybrowser/qtvariantproperty.h"
#include "propertybrowser/qtpropertymanager.h"
#include "propertybrowser/qttreepropertybrowser.h"
#include "propertybrowser/qteditorfactory.h"


Configuration::Configuration(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Configuration)
{
    ui->setupUi(this);
    connect(this->ui->buttonBox, &QDialogButtonBox::accepted, this, &Configuration::saveConfiguration);

    QTOZWManager *manager = OZWCore::get()->getQTOZWManager();
    m_options = manager->getOptions();
    QCoreApplication::processEvents();
    if (m_options)
        m_options->setReady(true);
    else
        qCDebug(ozwadmin) << "No m_options";

        QtVariantEditorFactory *variantFactory = new QtVariantEditorFactory(this);
        QtEnumEditorFactory *enumFactory = new QtEnumEditorFactory(this);
        m_OZWvariantManager = new QtVariantPropertyManager(this);
        m_OZWenumManager = new QtEnumPropertyManager(this);

    { 
        /* OZW Pane */
        this->m_OZWEditor = new QtTreePropertyBrowser(this);
        m_OZWEditor->setFactoryForManager(m_OZWvariantManager, variantFactory);
        m_OZWEditor->setFactoryForManager(m_OZWenumManager, enumFactory);
        m_OZWEditor->setPropertiesWithoutValueMarked(true);
        m_OZWEditor->setRootIsDecorated(false);

        QtProperty *topItem = m_OZWvariantManager->addProperty(QtVariantPropertyManager::groupTypeId(),
                    QLatin1String(" Paths"));
        topItem->addSubProperty(this->addStringProperty("ConfigPath", m_options->ConfigPath(), m_options->isLocked()));
        topItem->addSubProperty(this->addStringProperty("UserPath", m_options->UserPath(), m_options->isLocked()));
        m_OZWEditor->addProperty(topItem);


        topItem = m_OZWvariantManager->addProperty(QtVariantPropertyManager::groupTypeId(),
                                            QLatin1String("Driver"));
        topItem->addSubProperty(this->addBoolProperty("NotifyTransactions", m_options->NotifyTransactions(), m_options->isLocked()));
        topItem->addSubProperty(this->addBoolProperty("SaveConfiguration", m_options->SaveConfiguration(), m_options->isLocked()));
        topItem->addSubProperty(this->addIntProperty("DriverMaxAttempts", m_options->DriverMaxAttempts(), m_options->isLocked()));
        topItem->addSubProperty(this->addBoolProperty("SuppressValueRefresh", m_options->SuppressValueRefresh(), m_options->isLocked()));
        topItem->addSubProperty(this->addIntProperty("RetryTimeout", m_options->RetryTimeout(), m_options->isLocked()));
        topItem->addSubProperty(this->addBoolProperty("EnableSIS", m_options->EnableSIS(), m_options->isLocked()));
        topItem->addSubProperty(this->addBoolProperty("NotifyOnDriverUnload", m_options->NotifyOnDriverUnload(), m_options->isLocked()));
        m_OZWEditor->addProperty(topItem);

        topItem = m_OZWvariantManager->addProperty(QtVariantPropertyManager::groupTypeId(),
                                            QLatin1String("Associations"));
        topItem->addSubProperty(this->addBoolProperty("Associate", m_options->Associate(), m_options->isLocked()));
        topItem->addSubProperty(this->addBoolProperty("PerformReturnRoutes", m_options->PerformReturnRoutes(), m_options->isLocked()));
        m_OZWEditor->addProperty(topItem);

        topItem = m_OZWvariantManager->addProperty(QtVariantPropertyManager::groupTypeId(),
                                            QLatin1String("Polling"));
        topItem->addSubProperty(this->addIntProperty("PollInterval", m_options->PollInterval(), m_options->isLocked()));
        topItem->addSubProperty(this->addBoolProperty("IntervalBetweenPolls", m_options->IntervalBetweenPolls(), m_options->isLocked()));
        m_OZWEditor->addProperty(topItem);

        topItem = m_OZWvariantManager->addProperty(QtVariantPropertyManager::groupTypeId(),
                                            QLatin1String("CommandClasses"));
        topItem->addSubProperty(this->addStringProperty("Exclude", m_options->Exclude(), m_options->isLocked()));
        topItem->addSubProperty(this->addStringProperty("Include", m_options->Include(), m_options->isLocked()));
        m_OZWEditor->addProperty(topItem);

        topItem = m_OZWvariantManager->addProperty(QtVariantPropertyManager::groupTypeId(),
                                            QLatin1String("Security"));
        topItem->addSubProperty(this->addEnumProperty("SecurityStrategy", m_options->SecurityStrategy().getEnums(), m_options->SecurityStrategy().getSelected(), m_options->isLocked()));
        topItem->addSubProperty(this->addStringProperty("CustomSecuredCC", m_options->CustomSecuredCC(), m_options->isLocked())); 
        topItem->addSubProperty(this->addBoolProperty("EnforceSecureReception", m_options->EnforceSecureReception(), m_options->isLocked()));
        topItem->addSubProperty(this->addStringProperty("networkKey", m_options->NetworkKey(), m_options->isLocked()));
        m_OZWEditor->addProperty(topItem);

        topItem = m_OZWvariantManager->addProperty(QtVariantPropertyManager::groupTypeId(),
                                            QLatin1String("Sleeping Devices"));
        topItem->addSubProperty(this->addBoolProperty("AssumeAwake", m_options->AssumeAwake(), m_options->isLocked()));
        m_OZWEditor->addProperty(topItem);

        topItem = m_OZWvariantManager->addProperty(QtVariantPropertyManager::groupTypeId(),
                                            QLatin1String("UserCode CommandClass"));
        topItem->addSubProperty(this->addBoolProperty("RefreshAllUserCodes", m_options->RefreshAllUserCodes(), m_options->isLocked()));
        m_OZWEditor->addProperty(topItem);

        topItem = m_OZWvariantManager->addProperty(QtVariantPropertyManager::groupTypeId(),
                                            QLatin1String("Device Database"));
        topItem->addSubProperty(this->addBoolProperty("AutoUpdateConfigFile", m_options->AutoUpdateConfigFile(), m_options->isLocked()));
        topItem->addSubProperty(this->addEnumProperty("ReloadAfterUpdate", m_options->ReloadAfterUpdate().getEnums(), m_options->ReloadAfterUpdate().getSelected(), m_options->isLocked()));
        m_OZWEditor->addProperty(topItem);

        topItem = m_OZWvariantManager->addProperty(QtVariantPropertyManager::groupTypeId(),
                                            QLatin1String("Language"));
        topItem->addSubProperty(this->addStringProperty("Language", m_options->Language(), m_options->isLocked()));
        m_OZWEditor->addProperty(topItem);

        topItem = m_OZWvariantManager->addProperty(QtVariantPropertyManager::groupTypeId(),
                                            QLatin1String("Multi Channel Devices"));
        topItem->addSubProperty(this->addBoolProperty("IncludeInstanceLabels", m_options->IncludeInstanceLabels(), m_options->isLocked()));
        m_OZWEditor->addProperty(topItem);

        //m_OZWEditor->setEnabled(!m_options->isLocked());
        this->ui->config_ozw->layout()->addWidget(m_OZWEditor);
    }
    {
        /* Network Pane */
        m_NetvariantManager = new QtVariantPropertyManager(this);
        m_NetenumManager = new QtEnumPropertyManager(this);


        this->m_NetworkEditor = new QtTreePropertyBrowser(this);
        m_NetworkEditor->setFactoryForManager(m_NetvariantManager, variantFactory);
        m_NetworkEditor->setFactoryForManager(m_NetenumManager, enumFactory);
        m_NetworkEditor->setPropertiesWithoutValueMarked(true);
        m_NetworkEditor->setRootIsDecorated(false);

        QtProperty *topItem = m_OZWvariantManager->addProperty(QtVariantPropertyManager::groupTypeId(),
                    QLatin1String(" Paths"));
        m_NetworkEditor->addProperty(topItem);

        this->ui->config_net->layout()->addWidget(m_NetworkEditor);
    }

}

Configuration::~Configuration()
{
    delete ui;
}

void Configuration::saveConfiguration() {

    QtVariantProperty *property;
    QtProperty *property1 = dynamic_cast<QtProperty *>(this->m_variantToProperty["SaveLogLevel"]);
    OptionList SaveLogLevel = m_options->SaveLogLevel();
    if (property1->valueText() != SaveLogLevel.getSelectedName()) {
        SaveLogLevel.setSelected(property1->valueText());
        m_options->setSaveLogLevel(SaveLogLevel);
        settings.setValue("openzwave/SaveLogLevel", SaveLogLevel.getSelectedName());
        qDebug() << "Saved SaveLogLevel Param with " << SaveLogLevel.getSelectedName();
    }

    property1 = dynamic_cast<QtProperty *>(this->m_variantToProperty["QueueLogLevel"]);
    OptionList QueueLogLevel = m_options->SaveLogLevel();
    if (property1->valueText() != QueueLogLevel.getSelectedName()) {
        QueueLogLevel.setSelected(property1->valueText());
        m_options->setQueueLogLevel(QueueLogLevel);
        settings.setValue("openzwave/QueueLogLevel", QueueLogLevel.getSelectedName());
        qDebug() << "Saved QueueLogLevel Param with " << QueueLogLevel.getSelectedName();
    }

    property1 = dynamic_cast<QtProperty *>(this->m_variantToProperty["DumpTriggerLevel"]);
    OptionList DumpTriggerLevel = m_options->DumpTriggerLevel();
    if (property1->valueText() != DumpTriggerLevel.getSelectedName()) {
        DumpTriggerLevel.setSelected(property1->valueText());
        m_options->setQueueLogLevel(DumpTriggerLevel);
        settings.setValue("openzwave/DumpTriggerLevel", DumpTriggerLevel.getSelectedName());
        qDebug() << "Saved DumpTriggerLevel Param with " << DumpTriggerLevel.getSelectedName();
    }


    property = dynamic_cast<QtVariantProperty *>(this->m_variantToProperty["Associate"]);
    if (property->value() != m_options->Associate()) {
        m_options->setAssociate(property->value().toBool());
        settings.setValue("openzwave/Associate", property->value().toBool());
        qDebug() << "Saved Associate Param with " << property->value();
    }

    property = dynamic_cast<QtVariantProperty *>(this->m_variantToProperty["Exclude"]);
    if (property->value() != m_options->Exclude()) {
        m_options->setExclude(property->value().toString());
        settings.setValue("openzwave/Exclude", property->value().toString());
        qDebug() << "Saved Exclude Param with " << property->value();
    }

    property = dynamic_cast<QtVariantProperty *>(this->m_variantToProperty["Include"]);
    if (property->value() != m_options->Include()) {
        m_options->setInclude(property->value().toString());
        settings.setValue("openzwave/Include", property->value().toString());
        qDebug() << "Saved Include Param with " << property->value();
    }

    property = dynamic_cast<QtVariantProperty *>(this->m_variantToProperty["NotifyTransactions"]);
    if (property->value() != m_options->NotifyTransactions()) {
        m_options->setNotifyTransactions(property->value().toBool());
        settings.setValue("openzwave/NotifyTransactions", property->value().toBool());
        qDebug() << "Saved NotifyTransactions Param with " << property->value();
    }

    property = dynamic_cast<QtVariantProperty *>(this->m_variantToProperty["SaveConfiguration"]);
    if (property->value() != m_options->SaveConfiguration()) {
        m_options->setSaveConfiguration(property->value().toBool());
        settings.setValue("openzwave/SaveConfiguration", property->value().toBool());
        qDebug() << "Saved SaveConfiguration Param with " << property->value();
    }

    property = dynamic_cast<QtVariantProperty *>(this->m_variantToProperty["DriverMaxAttempts"]);
    if (property->value() != m_options->DriverMaxAttempts()) {
        m_options->setDriverMaxAttempts(property->value().toInt());
        settings.setValue("openzwave/DriverMaxAttempts", property->value().toInt());
        qDebug() << "Saved DriverMaxAttempts Param with " << property->value();
    }

    property = dynamic_cast<QtVariantProperty *>(this->m_variantToProperty["PollInterval"]);
    if (property->value()  != m_options->PollInterval()) {
        m_options->setPollInterval(property->value().toInt());
        settings.setValue("openzwave/PollInterval", property->value().toInt());
        qDebug() << "Saved PollInterval Param with " << property->value();
    }

    property = dynamic_cast<QtVariantProperty *>(this->m_variantToProperty["IntervalBetweenPolls"]);
    if (property->value()  != m_options->IntervalBetweenPolls()) {
        m_options->setIntervalBetweenPolls(property->value().toBool());
        settings.setValue("openzwave/IntervalBetweenPolls", property->value().toBool());
        qDebug() << "Saved IntervalBetweenPolls Param with " << property->value();
    }

    property = dynamic_cast<QtVariantProperty *>(this->m_variantToProperty["SuppressValueRefresh"]);
    if (property->value()  != m_options->SuppressValueRefresh()) {
        m_options->setSuppressValueRefresh(property->value().toBool());
        settings.setValue("openzwave/SuppressValueRefresh", property->value().toBool());
        qDebug() << "Saved SuppressValueRefresh Param with " << property->value();
    }

    property = dynamic_cast<QtVariantProperty *>(this->m_variantToProperty["PerformReturnRoutes"]);
    if (property->value() != m_options->PerformReturnRoutes()) {
        m_options->setPerformReturnRoutes(property->value().toBool());
        settings.setValue("openzwave/PerformReturnRoutes", property->value().toBool());
        qDebug() << "Saved PerformReturnRoutes Param with " << property->value();
    }

    property = dynamic_cast<QtVariantProperty *>(this->m_variantToProperty["IntervalBetweenPolls"]);
    if (property->value() != m_options->IntervalBetweenPolls()) {
        m_options->setIntervalBetweenPolls(property->value().toBool());
        settings.setValue("openzwave/IntervalBetweenPolls", property->value().toBool());
        qDebug() << "Saved IntervalBetweenPolls Param with " << property->value();
    }

    property = dynamic_cast<QtVariantProperty *>(this->m_variantToProperty["NetworkKey"]);
    if (property->value() != m_options->NetworkKey()) {
        m_options->setNetworkKey(property->value().toString());
        /* we don't save the Network Key */
        qDebug() << "Not Saving Network Key";
    }

    property = dynamic_cast<QtVariantProperty *>(this->m_variantToProperty["RefreshAllUserCodes"]);
    if (property->value() != m_options->RefreshAllUserCodes()) {
        m_options->setRefreshAllUserCodes(property->value().toBool());
        settings.setValue("openzwave/RefreshAllUserCodes", property->value().toBool());
        qDebug() << "Saved RefreshAllUserCodes Param with " << property->value();
    }

    property = dynamic_cast<QtVariantProperty *>(this->m_variantToProperty["RetryTimeout"]);
    if (property->value() != m_options->RetryTimeout()) {
        m_options->setRetryTimeout(property->value().toInt());
        settings.setValue("openzwave/RetryTimeout", property->value().toInt());
        qDebug() << "Saved RetryTimeout Param with " << property->value();
    }

    property = dynamic_cast<QtVariantProperty *>(this->m_variantToProperty["EnableSIS"]);
    if (property->value()  != m_options->EnableSIS()) {
        m_options->setEnableSIS(property->value().toBool());
        settings.setValue("openzwave/EnableSIS", property->value().toBool());
        qDebug() << "Saved EnableSIS Param with " << property->value();
    }

    property = dynamic_cast<QtVariantProperty *>(this->m_variantToProperty["AssumeAwake"]);
    if (property->value() != m_options->AssumeAwake()) {
        m_options->setAssumeAwake(property->value().toBool());
        settings.setValue("openzwave/AssumeAwake", property->value().toBool());
        qDebug() << "Saved AssumeAwake Param with " << property->value();
    }

    property = dynamic_cast<QtVariantProperty *>(this->m_variantToProperty["NotifyOnDriverUnload"]);
    if (property->value() != m_options->NotifyOnDriverUnload()) {
        m_options->setNotifyOnDriverUnload(property->value().toBool());
        settings.setValue("openzwave/NotifyOnDriverUnload", property->value().toBool());
        qDebug() << "Saved NotifyOnDriverUnload Param with " << property->value();
    }

    property1 = dynamic_cast<QtProperty *>(this->m_variantToProperty["SecurityStrategy"]);
    OptionList SecurityStrategy = m_options->SecurityStrategy();
    if (property1->valueText() != SecurityStrategy.getSelectedName()) {
        SecurityStrategy.setSelected(property1->valueText());
        m_options->setSecurityStrategy(SecurityStrategy);
        settings.setValue("openzwave/SecurityStrategy", SecurityStrategy.getSelectedName());
        qDebug() << "Saved SecurityStrategy Param with " << SecurityStrategy.getSelectedName();
    }

    property = dynamic_cast<QtVariantProperty *>(this->m_variantToProperty["CustomSecuredCC"]);
    if (property->value() != m_options->CustomSecuredCC()) {
        m_options->setCustomSecuredCC(property->value().toString());
        settings.setValue("openzwave/CustomSecuredCC", property->value().toString());
        qDebug() << "Saved CustomSecuredCC Param with " << property->value();
    }

    property = dynamic_cast<QtVariantProperty *>(this->m_variantToProperty["EnforceSecureReception"]);
    if (property->value() != m_options->EnforceSecureReception()) {
        m_options->setEnforceSecureReception(property->value().toBool());
        settings.setValue("openzwave/EnforceSecureReception", property->value().toBool());
        qDebug() << "Saved EnforceSecureReception Param with " << property->value();
    }

    property = dynamic_cast<QtVariantProperty *>(this->m_variantToProperty["AutoUpdateConfigFile"]);
    if (property->value() != m_options->AutoUpdateConfigFile()) {
        m_options->setAutoUpdateConfigFile(property->value().toBool());
        settings.setValue("openzwave/AutoUpdateConfigFile", property->value().toBool());
        qDebug() << "Saved AutoUpdateConfigFile Param with " << property->value();
    }

    property1 = dynamic_cast<QtProperty *>(this->m_variantToProperty["ReloadAfterUpdate"]);
    OptionList ReloadAfterUpdate = m_options->ReloadAfterUpdate();
    if (property1->valueText() != ReloadAfterUpdate.getSelectedName()) {
        ReloadAfterUpdate.setSelected(property1->valueText());
        m_options->setReloadAfterUpdate(ReloadAfterUpdate);
        settings.setValue("openzwave/SecurityStrategy", ReloadAfterUpdate.getSelectedName());
        qDebug() << "Saved ReloadAfterUpdate Param with " << ReloadAfterUpdate.getSelectedName();
    }

    property = dynamic_cast<QtVariantProperty *>(this->m_variantToProperty["Language"]);
    if (property->value() != m_options->Language()) {
        m_options->setLanguage(property->value().toString());
        settings.setValue("openzwave/Language", property->value().toString());
        qDebug() << "Saved Language Param with " << property->value();
    }

    property = dynamic_cast<QtVariantProperty *>(this->m_variantToProperty["IncludeInstanceLabels"]);
    if (property->value() != m_options->IncludeInstanceLabels()) {
        m_options->setIncludeInstanceLabels(property->value().toBool());
        settings.setValue("openzwave/IncludeInstanceLabels", property->value().toBool());
        qDebug() << "Saved IncludeInstanceLabels Param with " << property->value();
    }
}

QtVariantProperty *Configuration::addStringProperty(QString name, QString value, bool locked) {
            QtVariantProperty *variantitem = m_OZWvariantManager->addProperty(QVariant::String, name);
            variantitem->setValue(value);
            variantitem->setBold(settings.contains("openzwave/"+name));
            variantitem->setEnabled(!locked);
            this->m_variantToProperty.insert(name, variantitem);
            return variantitem;
}

QtVariantProperty *Configuration::addBoolProperty(QString name, bool value, bool locked) {
            QtVariantProperty *variantitem = m_OZWvariantManager->addProperty(QVariant::Bool, name);
            variantitem->setValue(value);
            variantitem->setBold(settings.contains("openzwave/"+name));
            variantitem->setEnabled(!locked);
            this->m_variantToProperty.insert(name, variantitem);
            return variantitem;
}

QtVariantProperty *Configuration::addIntProperty(QString name, int value, bool locked) {
            QtVariantProperty *variantitem = m_OZWvariantManager->addProperty(QVariant::Int, name);
            variantitem->setValue(value);
            variantitem->setBold(settings.contains("openzwave/"+name));
            variantitem->setEnabled(!locked);
            this->m_variantToProperty.insert(name, variantitem);
            return variantitem;
}

QtProperty *Configuration::addEnumProperty(QString name, QStringList options, int selected, bool readonly) {
            QtProperty *enumitem = m_OZWenumManager->addProperty(name);
            m_OZWenumManager->setEnumNames(enumitem, options);
            m_OZWenumManager->setValue(enumitem, selected);
            enumitem->setBold(settings.contains("openzwave/"+name));
            enumitem->setEnabled(!readonly);
            this->m_variantToProperty.insert(name, enumitem);
            return enumitem;
}
