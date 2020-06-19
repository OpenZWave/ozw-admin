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
    void init();
private slots:
    void newMsg(QDateTime time, LogLevels::Level level, quint8 s_node, QString s_msg);
    void scrollWindow();
    void logsReady(bool ready);
    void syncStatus(quint32 size, bool finsihed);
private:
    Ui::LogWindow *ui;
    bool viewAtBottom = true;
    QTOZWLog *m_log;
    QTOZWLogModel *m_logModel;
    QTimer m_scrollTimer;
};

#endif // LOGWINDOW_H
