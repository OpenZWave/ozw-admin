#include "metadatawindow.h"
#include "ui_metadatawindow.h"

MetaDataWindow::MetaDataWindow(QWidget *parent) :
    QDialog(parent),
    m_manager(nullptr),
    ui(new Ui::MetaDataWindow)
{
    ui->setupUi(this);
}

MetaDataWindow::~MetaDataWindow()
{
    delete ui;
}

void MetaDataWindow::populate(QTOZWManager *manager, quint8 node) {
    this->m_manager = manager;
    QString value = this->m_manager->GetMetaData(node, QTOZWManagerSource::QTOZWMetaDataField::Description);
    this->ui->md_description->setText(value);
    value = this->m_manager->GetMetaData(node, QTOZWManagerSource::QTOZWMetaDataField::ResetHelp);
    this->ui->md_reset->setText(value);
    value = this->m_manager->GetMetaData(node, QTOZWManagerSource::QTOZWMetaDataField::WakeupHelp);
    this->ui->md_wakeup->setText(value);
    value = this->m_manager->GetMetaData(node, QTOZWManagerSource::QTOZWMetaDataField::ExclusionHelp);
    this->ui->md_exclusion->setText(value);
    value = this->m_manager->GetMetaData(node, QTOZWManagerSource::QTOZWMetaDataField::InclusionHelp);
    this->ui->md_inclusion->setText(value);
}
