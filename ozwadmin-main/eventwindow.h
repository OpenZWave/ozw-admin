#ifndef EVENTWINDOW_H
#define EVENTWINDOW_H

#include <QWidget>
#include <QTimer>


namespace Ui {
class EventWindow;
}

class EventWindow : public QWidget
{
    Q_OBJECT

public:
    explicit EventWindow(QWidget *parent = nullptr);
    ~EventWindow();

public slots:
    void newEvent(QString);
private slots:
    void scrollWindow();
private:
    Ui::EventWindow *ui;
    QTimer m_scrollTimer;
};

#endif // EVENTWINDOW_H
