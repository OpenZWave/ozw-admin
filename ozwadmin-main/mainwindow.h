/*  ozw-admin - Gui for OpenZWave
 *    Copyright (C) 2016  Justin Hammond <justin@dynam.ac>
 *
 *    This program is free software: you can redistribute it and/or modify
 *    it under the terms of the GNU General Public License as published by
 *    the Free Software Foundation, either version 3 of the License, or
 *    (at your option) any later version.
 *
 *    This program is distributed in the hope that it will be useful,
 *    but WITHOUT ANY WARRANTY; without even the implied warranty of
 *    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *    GNU General Public License for more details.
 *
 *    You should have received a copy of the GNU General Public License
 *    along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSettings>
#include <QModelIndex>
#include <QDir>
#include <QTimer>
#include <QMessageBox>

#include "qt-ads/DockManager.h"
#include "logwindow.h"
#include "statusbarmessages.h"
#include "nodetablewidget.h"
#include "controllercommands.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
public slots:
    void OpenConnection();
    void CloseConnection();
    void resizeColumns();
    void NodeSelected(QModelIndex,QModelIndex);
    void openLogWindow();
    void OpenDeviceDB();
    void QTOZW_Ready();
    void openAboutWindow();
    void openMetaDataWindow();
    void openConfigWindow();
    void addNode();
    void delNode();
    void healNetwork();
    
    QMessageBox::StandardButton openCriticalDialog(QString title, QString msg);

private:
    void connected(bool);
    ControllerCommands *m_controllerCommands;

    Ui::MainWindow *ui;
    statusBarMessages sbMsg;
	nodeTableWidget *ntw;

    LogWindow m_logWindow;
    ads::CDockManager* m_DockManager;
};

#endif // MAINWINDOW_H
