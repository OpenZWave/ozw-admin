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

#include <qt-openzwave/qtopenzwave.h>
#include <qt-openzwave/qtozwmanager.h>

#include "logwindow.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    Q_PROPERTY(QString SerialPort MEMBER m_serialport)
public slots:
    void OpenSerialPort();
    void resizeColumns();
    void NodeSelected(QModelIndex,QModelIndex);
    void openLogWindow();
    void OpenDeviceDB();
    void QTOZW_Ready();


private:
    Ui::MainWindow *ui;
    QString m_serialport;
    QSettings settings;

    QTOpenZwave *m_openzwave;
    QTOZWManager *m_qtozwmanager;
    QDir m_configpath;
    QDir m_userpath;
};

#endif // MAINWINDOW_H
