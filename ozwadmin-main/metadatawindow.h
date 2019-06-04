#ifndef METADATAWINDOW_H
#define METADATAWINDOW_H

#include <QDialog>
#include <qt-openzwave/qtozwmanager.h>

namespace Ui {
class MetaDataWindow;
}

class MetaDataWindow : public QDialog
{
    Q_OBJECT

public:
    explicit MetaDataWindow(QWidget *parent = nullptr);
    ~MetaDataWindow();
    void populate(QTOZWManager *manager, quint8 node);

private:
    QTOZWManager *m_manager;
    quint8 node;
    Ui::MetaDataWindow *ui;
};

#endif // METADATAWINDOW_H
