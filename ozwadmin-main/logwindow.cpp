#include <QScrollBar>

#include "logwindow.h"
#include "ui_logwindow.h"
#include "ozwcore.h"
#include "util.h"

LogWindow::LogWindow(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::LogWindow)
{
    ui->setupUi(this);
    this->ui->logview->verticalHeader()->hide();
    this->ui->logview->horizontalHeader()->setStretchLastSection(true);
    this->ui->logview->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    this->ui->logview->verticalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
//    this->ui->logview->resizeColumnsToContents();

    this->m_log = OZWCore::get()->getQTOZWManager()->getLog();
    connect(this->m_log, &QTOZWLog::newLogLine, this, &LogWindow::newMsg);

}

LogWindow::~LogWindow()
{
    delete ui;
}

void LogWindow::newMsg(QDateTime time, LogLevels::Level level, quint8 s_node, QString s_msg) {
            qCDebug(ozwadmin) << time << level << s_node << s_msg;
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
