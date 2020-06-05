#include <QTableWidgetItem>
#include <QTime>

#include "eventwindow.h"
#include "ui_eventwindow.h"
#include "util.h"


EventWindow::EventWindow(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::EventWindow)
{
    ui->setupUi(this);
    ui->eventTable->verticalHeader()->hide();
    ui->eventTable->horizontalHeader()->setStretchLastSection(true);
    ui->eventTable->setHorizontalHeaderItem(0, new QTableWidgetItem("Time"));
    ui->eventTable->setHorizontalHeaderItem(1, new QTableWidgetItem("Message"));
    //ui->eventTable->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    //ui->eventTable->verticalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    this->m_scrollTimer.setInterval(500);
    connect(&this->m_scrollTimer, &QTimer::timeout, this, &EventWindow::scrollWindow);
}

EventWindow::~EventWindow()
{
    delete ui;
}

void EventWindow::newEvent(QString Msg) {
    QTableWidgetItem *timeStamp = new QTableWidgetItem(QTime::currentTime().toString());
    timeStamp->setFlags(timeStamp->flags() ^ Qt::ItemIsEditable);
    QTableWidgetItem *message = new QTableWidgetItem(Msg);
    message->setFlags(message->flags() ^ Qt::ItemIsEditable);

    this->ui->eventTable->insertRow ( this->ui->eventTable->rowCount() );
    this->ui->eventTable->setItem(this->ui->eventTable->rowCount() -1, 
                                  0,
                                  timeStamp);
    this->ui->eventTable->setItem(this->ui->eventTable->rowCount() -1, 
                                  1,
                                  message);
    if (!this->m_scrollTimer.isActive())
        this->m_scrollTimer.start();

                            
}

void EventWindow::scrollWindow()
{
    if (this->ui->pauseScroll->isChecked())
        this->ui->eventTable->scrollToBottom();

    this->m_scrollTimer.stop();
}
