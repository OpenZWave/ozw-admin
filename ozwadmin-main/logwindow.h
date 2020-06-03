#ifndef LOGWINDOW_H
#define LOGWINDOW_H

#include <QDialog>
#include <QAbstractItemModel>
#include <qt-openzwave/qtozwlog.h>

namespace Ui {
class LogWindow;
}

class LogWindow : public QWidget
{
    Q_OBJECT

public:
    explicit LogWindow(QWidget *parent = nullptr);
    ~LogWindow();
    void setModel(QAbstractItemModel *model);
private slots:
    void newMsg(QDateTime time, LogLevels::Level level, quint8 s_node, QString s_msg);
private:
    Ui::LogWindow *ui;
    bool viewAtBottom = true;
    QTOZWLog *m_log;
};

#endif // LOGWINDOW_H
