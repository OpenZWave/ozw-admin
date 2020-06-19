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

    this->m_log = OZWCore::get()->getQTOZWManager()->getLog();
    connect(this->m_log, &QTOZWLog::newLogLine, this, &LogWindow::newMsg);
    connect(this->m_log, &QTOZWLog::syncStatus, this, &LogWindow::syncStatus);

    this->m_scrollTimer.setInterval(500);
    connect(&this->m_scrollTimer, &QTimer::timeout, this, &LogWindow::scrollWindow);
}

LogWindow::~LogWindow()
{
    this->m_scrollTimer.stop();
    delete ui;
}

void LogWindow::newMsg(QDateTime time, LogLevels::Level level, quint8 s_node, QString s_msg) {
    Q_UNUSED(time);
    Q_UNUSED(level);
    Q_UNUSED(s_node);
    Q_UNUSED(s_msg);
    if (!this->m_scrollTimer.isActive())
        this->m_scrollTimer.start();
}

void LogWindow::scrollWindow()
{
    if (this->ui->pauseScroll->isChecked())
        this->ui->logview->scrollToBottom();

    this->m_scrollTimer.stop();
}

void LogWindow::init() {
    connect(&OZWCore::get()->settings, &SettingsManager::logBufferSizeChanged, this->m_log, &QTOZWLog::setLogBufSize);
    this->m_log->setLogBufSize(OZWCore::get()->settings.logBufferSize());
    this->m_logModel = new QTOZWLogModel(this->m_log, this);
    this->ui->logview->setModel(this->m_logModel);
    connect(this->m_log, &QTOZWLog::readyChanged, this, &LogWindow::logsReady);
    if (this->m_log->isReady()) 
        this->logsReady(true);
}

void LogWindow::logsReady(bool ready) {
    if (ready) {
        if (OZWCore::get()->settings.retriveLogBuffer() == true) 
            this->m_log->syncroniseLogs();
    }
}

void LogWindow::syncStatus(quint32 size, bool finsihed) {
    qCDebug(ozwadmin) << "Sync Status: " << size << "Finsihed:" << finsihed;
}
