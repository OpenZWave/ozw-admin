#include <QTableWidgetItem>
#include <QTime>
#include <QTimer>

#include "eventwindow.h"
#include "ui_eventwindow.h"


EventWindow::EventWindow(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::EventWindow)
{
    ui->setupUi(this);
    ui->eventTable->horizontalHeader()->setStretchLastSection(true);
    ui->eventTable->setHorizontalHeaderItem(0, new QTableWidgetItem("Time"));
    ui->eventTable->setHorizontalHeaderItem(1, new QTableWidgetItem("Message"));
    ui->eventTable->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);

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
    QTimer::singleShot(10, this->ui->eventTable, &QTableWidget::scrollToBottom);
}
