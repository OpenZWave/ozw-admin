#include <QScrollBar>

#include "logwindow.h"
#include "ui_logwindow.h"

LogWindow::LogWindow(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::LogWindow)
{
    ui->setupUi(this);
    this->ui->logview->verticalHeader()->hide();
    this->ui->logview->horizontalHeader()->setStretchLastSection(true);
    this->ui->logview->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
//    this->ui->val_system_tbl->verticalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
//    this->ui->logview->resizeColumnsToContents();


}

LogWindow::~LogWindow()
{
    delete ui;
}

void LogWindow::setModel(QAbstractItemModel *model) {
    this->ui->logview->setModel(model);
//    this->ui->logview->resizeColumnsToContents();
    connect(this->ui->logview->model(), &QAbstractItemModel::rowsAboutToBeInserted,
            this->ui->logview, [&] {
      auto bar = this->ui->logview->verticalScrollBar();
      viewAtBottom = bar ? (bar->value() == bar->maximum()) : false;
    });
    connect(this->ui->logview->model(), &QAbstractItemModel::rowsInserted,
            this->ui->logview, [&]{ if (viewAtBottom) this->ui->logview->scrollToBottom(); });
}
