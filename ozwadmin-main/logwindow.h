#ifndef LOGWINDOW_H
#define LOGWINDOW_H

#include <QDialog>
#include <QAbstractItemModel>

namespace Ui {
class LogWindow;
}

class LogWindow : public QDialog
{
    Q_OBJECT

public:
    explicit LogWindow(QWidget *parent = nullptr);
    ~LogWindow();
    void setModel(QAbstractItemModel *model);
private:
    Ui::LogWindow *ui;
    bool viewAtBottom = true;
};

#endif // LOGWINDOW_H
