#ifndef SPLASHDIALOG_H
#define SPLASHDIALOG_H

#include <QDialog>
#include <qt-openzwave/qtopenzwave.h>

namespace Ui {
class SplashDialog;
}

class SplashDialog : public QDialog
{
    Q_OBJECT

public:
    explicit SplashDialog(QTOpenZwave *qtozw, QWidget *parent = nullptr);
    ~SplashDialog();

private:
    Ui::SplashDialog *ui;
};

#endif // SPLASHDIALOG_H
