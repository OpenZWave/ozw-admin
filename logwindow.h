#ifndef LOGWINDOW_H
#define LOGWINDOW_H

#include <QObject>
#include <QDialog>
#include <QMdiArea>
#include <platform/Log.h>
#include <string>

/* our Custom Logging Class */

class ozwAdminLog : public QObject, public OpenZWave::i_LogImpl
{
        Q_OBJECT
public:
        ozwAdminLog();
        virtual ~ozwAdminLog();
        virtual void Write( OpenZWave::LogLevel _level, uint8 const _nodeId, char const* _format, va_list _args );
        virtual void QueueDump();
        virtual void QueueClear();
        virtual void SetLoggingState( OpenZWave::LogLevel _saveLevel, OpenZWave::LogLevel _queueLevel, OpenZWave::LogLevel _dumpTrigger );
        virtual void SetLogFileName( const std::string &_filename );
signals:
        void newLogMsg(OpenZWave::LogLevel, uint8, const QString);
};




class LogBrowserDialog;

class LogBrowser : public QObject
{
    Q_OBJECT
public:
    explicit LogBrowser(QObject *parent = 0);
    ~LogBrowser();
    ozwAdminLog *logAdapter;

public slots:
    void outputMessage( OpenZWave::LogLevel Level, uint8, const QString &msg );
    void show();

signals:
    void sendMessage( OpenZWave::LogLevel Level, const QString &msg );

private:
    LogBrowserDialog *browserDialog;

};




class QTextBrowser;
class QPushButton;

class LogBrowserDialog : public QDialog
{
    Q_OBJECT

public:
    LogBrowserDialog(QWidget *parent = 0);
    ~LogBrowserDialog();

public slots:
    void outputMessage( OpenZWave::LogLevel type, const QString &msg );

protected slots:
    void save();

protected:
    virtual void keyPressEvent( QKeyEvent *e );
    virtual void closeEvent( QCloseEvent *e );

    QTextBrowser *browser;
    QPushButton *clearButton;
    QPushButton *saveButton;
};

#endif // LOGWINDOW_H
