
#include <qt-openzwave/qtozwmanager.h>
#include "splashdialog.h"
#include "ui_splashdialog.h"

SplashDialog::SplashDialog(QTOpenZwave *qtozw, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SplashDialog)
{
    ui->setupUi(this);
    QString info("<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\"> "
                 "<html><head><meta name=\"qrichtext\" content=\"1\" /><style type=\"text/css\">"
                 "p, li { white-space: pre-wrap; }"
                 "</style></head><body style=\" font-family:'.SF NS Text'; font-size:13pt; font-weight:400; font-style:normal;\">"
                 "<p align=\"center\" style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><a href=\"https://github.com/OpenZWave/ozw-admin\"><span style=\" text-decoration: underline; color:#0000ff;\">https://github.com/OpenZWave/ozw-admin</span></a></p>"
                 "<p align=\"center\" style=\"-qt-paragraph-type:empty; margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px; text-decoration: underline; color:#0000ff;\"><br /></p>"
                 "<p align=\"center\" style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><a href=\"https://github.com/sponsors/Fishwaldo\"><span style=\" text-decoration: underline; color:#0000ff;\">Support OpenZWave Development!</span></a></p>"
                 "<p style=\"-qt-paragraph-type:empty; margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><br /></p>"
                 "<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\">Versions:</p>"
                 "<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\">OpenZWave Version %1</p>"
                 "<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\">qt-openzwave Version: %2</p>"
                 "<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\">QT Version: %3</p>"
                 "<p style=\"-qt-paragraph-type:empty; margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><br /></p></body></html>");


    ui->InfoText->setText(info.arg(qtozw->GetManager()->getVersionAsString()).arg(qtozw->getVersion()).arg(qVersion()));
    setWindowFlags(Qt::SplashScreen|Qt::WindowStaysOnTopHint);
    QString ozwadminversion("Version %1");
    this->ui->OZWAdminVersionLbl->setText(ozwadminversion.arg(QCoreApplication::applicationVersion()));
}

SplashDialog::~SplashDialog()
{
    delete ui;
}
