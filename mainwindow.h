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
#include "logwindow.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    Q_PROPERTY(QString SerialPort MEMBER m_serialport)
public slots:
    void OpenSerialPort();
    void newNode(qint8 nodeID);
    void saveCache();
    void resizeColumns();
    void NodeSelected(QModelIndex,QModelIndex);
    void openLogWindow();
    void updateGroups(qint8, qint8);



private:
    Ui::MainWindow *ui;
    QString m_serialport;
    QSettings settings;
    LogBrowser *logBrowser;
};

#endif // MAINWINDOW_H
