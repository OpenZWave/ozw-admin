#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QTextBrowser>
#include <QPushButton>
#include <QFileDialog>
#include <QDir>
#include <QFile>
#include <QMessageBox>
#include <QTextStream>
#include <QCloseEvent>
#include <QKeyEvent>
#include <QMainWindow>
#include <QDateTime>
#include <iostream>
#include "logwindow.h"

ozwAdminLog::ozwAdminLog()
{

}
ozwAdminLog::~ozwAdminLog()
{
}
void ozwAdminLog::Write( OpenZWave::LogLevel _level, uint8 const _nodeId, char const* _format, va_list _args ) {
    char lineBuf[1024] = {0};
    //int lineLen = 0;
    if( _format != NULL && _format[0] != '\0' )
    {
        va_list saveargs;
        va_copy( saveargs, _args );

        vsnprintf( lineBuf, sizeof(lineBuf), _format, _args );
        va_end( saveargs );
    }
//    printf("Node %d: %s\n", _nodeId, lineBuf);
    QString msg("Node: ");
    msg.append(QString::number(_nodeId) + ": ");
    msg.append(lineBuf);
    emit newLogMsg(_level, _nodeId, msg);

}

void ozwAdminLog::QueueDump() {

}

void ozwAdminLog::QueueClear() {

}

void ozwAdminLog::SetLoggingState( OpenZWave::LogLevel _saveLevel, OpenZWave::LogLevel _queueLevel, OpenZWave::LogLevel _dumpTrigger ) {
    Q_UNUSED(_saveLevel);
    Q_UNUSED(_queueLevel);
    Q_UNUSED(_dumpTrigger);
}

void ozwAdminLog::SetLogFileName( const std::string &_filename ) {
    Q_UNUSED(_filename);
}



LogBrowserDialog::LogBrowserDialog(QWidget *parent)
    : QDialog(parent)
{
    QVBoxLayout *layout = new QVBoxLayout;
    setLayout(layout);

    browser = new QTextBrowser(this);
    browser->setWordWrapMode(QTextOption::NoWrap);
    layout->addWidget(browser);

    QHBoxLayout *buttonLayout = new QHBoxLayout;
    buttonLayout->setContentsMargins(0, 0, 0, 0);
    layout->addLayout(buttonLayout);

    buttonLayout->addStretch(10);

    clearButton = new QPushButton(this);
    clearButton->setText("clear");
    buttonLayout->addWidget(clearButton);
    connect(clearButton, SIGNAL(clicked()), browser, SLOT(clear()));

    saveButton = new QPushButton(this);
    saveButton->setText("save output");
    buttonLayout->addWidget(saveButton);
    connect(saveButton, SIGNAL(clicked()), this, SLOT(save()));

    //resize(200, 400);
    this->setWindowTitle("Log Window");
    this->setAttribute(Qt::WA_DeleteOnClose);
}


LogBrowserDialog::~LogBrowserDialog()
{

}


QString GetLogLevelString
(
        OpenZWave::LogLevel _level
        )
{
    if ((_level >= OpenZWave::LogLevel_None) && (_level <= OpenZWave::LogLevel_Internal)) {
        char buf[20];
        snprintf( buf, sizeof(buf), "%s, ", OpenZWave::LogLevelString[_level] );
        return buf;
    }
    else
        return "Unknown, ";
}



void LogBrowserDialog::outputMessage(OpenZWave::LogLevel type, const QString &msg)
{

    QDateTime dateTime = QDateTime::currentDateTime();
    QString output(dateTime.toString() + " : " + GetLogLevelString(type)  + msg);
    switch (type) {
    case OpenZWave::LogLevel_Debug:
    case OpenZWave::LogLevel_Detail:
    case OpenZWave::LogLevel_Always:
        browser->setTextColor(Qt::blue);
        break;
    case OpenZWave::LogLevel_Alert:
    case OpenZWave::LogLevel_Warning: {
        QColor orangeColor(255,165,0);
        browser->setTextColor(orangeColor);
        break;
    }
    case OpenZWave::LogLevel_Fatal:
    case OpenZWave::LogLevel_Error:
        browser->setTextColor(Qt::red);
        break;
    default:
        browser->setTextColor(Qt::black);
    }

    browser->append(output);
    return;

}


void LogBrowserDialog::save()
{
    QString saveFileName = QFileDialog::getSaveFileName(
                this,
                tr("Save Log Output"),
                tr("%1/logfile.txt").arg(QDir::homePath()),
                tr("Text Files (*.txt);;All Files (*)")
                );

    if(saveFileName.isEmpty())
        return;

    QFile file(saveFileName);
    if(!file.open(QIODevice::WriteOnly)) {
        QMessageBox::warning(
                    this,
                    tr("Error"),
                    QString(tr("<nobr>File '%1'<br/>cannot be opened for writing.<br/><br/>"
                               "The log output could <b>not</b> be saved!</nobr>"))
                    .arg(saveFileName));
        return;
    }

    QTextStream stream(&file);
    stream << browser->toPlainText();
    file.close();
}


void LogBrowserDialog::closeEvent(QCloseEvent *e)
{
    QMessageBox::StandardButton answer = QMessageBox::question(
                this,
                tr("Close Log Browser?"),
                tr("Do you really want to close the log browser?"),
                QMessageBox::Yes | QMessageBox::No
                );

    if (answer == QMessageBox::Yes)
        e->accept();
    else
        e->ignore();
}


void LogBrowserDialog::keyPressEvent(QKeyEvent *e)
{
    // ignore all keyboard events
    // protects against accidentally closing of the dialog
    // without asking the user
    e->ignore();
}




LogBrowser::LogBrowser(QObject *parent) :
    QObject(parent)
{
    //    qRegisterMetaType<QtMsgType>("QtMsgType");
    qRegisterMetaType<OpenZWave::LogLevel>("OpenZWave::LogLevel");
    this->logAdapter = new ozwAdminLog();
    this->browserDialog = new LogBrowserDialog;

    connect(this, SIGNAL(sendMessage(OpenZWave::LogLevel,QString)), browserDialog, SLOT(outputMessage(OpenZWave::LogLevel,QString)), Qt::QueuedConnection);
    connect(this->logAdapter, SIGNAL(newLogMsg(OpenZWave::LogLevel,uint8,QString)), this, SLOT(outputMessage(OpenZWave::LogLevel,uint8,QString)));

    OpenZWave::Log::SetLoggingClass(this->logAdapter);

}


LogBrowser::~LogBrowser()
{
    delete browserDialog;
}


void LogBrowser::outputMessage(OpenZWave::LogLevel Level, uint8 node, const QString &msg)
{
    //std::cout << msg.toStdString() << std::endl;
    emit sendMessage( Level, msg );
}

void LogBrowser::show() {
    //qobject_cast<QMainWindow *>(this->parent)->mdiArea->addSubWindow(browserDialog);
    //win->addSubWindow(browserDialog);
    browserDialog->show();
}
