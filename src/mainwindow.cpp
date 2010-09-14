/***************************************************************************
 *   Copyright (C) 2006 by Vladimir Kuznetsov                              *
 *   vovanec@gmail.com                                                     *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/

#include <QtGui>

#include "mainwindow.h"
#include "tabwidget.h"
#include "config.h"
#include "version.h"
#include "properties.h"


MainWindow::MainWindow(const QString& work_dir, const QString& command, QWidget * parent, Qt::WindowFlags f) : QMainWindow(parent,f)
{
    setupUi(this);
    restoreGeometry(Properties::Instance()->mainWindowGeometry);
    restoreState(Properties::Instance()->mainWindowState);

    connect(consoleTabulator, SIGNAL(quit_notification()), SLOT(quit()));
    consoleTabulator->setWorkDirectory(work_dir);
    //consoleTabulator->setShellProgram(command);
    consoleTabulator->addTerminal(command);
    setWindowTitle(STR_VERSION);
    setWindowIcon(QIcon(":/icons/main.png"));
    this->addActions();
}


void MainWindow::addActions()
{
    QSettings settings(QDir::homePath()+"/.qterminal", QSettings::IniFormat);
    settings.beginGroup("Shortcuts");

    QAction* act = new QAction(this);

    act->setShortcut(Properties::Instance()->shortcuts[ADD_TAB]);
    connect(act, SIGNAL(triggered()), consoleTabulator, SLOT(addTerminal()));
    addAction(act);

    act = new QAction(this);
    act->setShortcut(Properties::Instance()->shortcuts[TAB_RIGHT]);
    connect(act, SIGNAL(triggered()), consoleTabulator, SLOT(traverseRight()));
    addAction(act);

    act = new QAction(this);
    act->setShortcut(Properties::Instance()->shortcuts[TAB_LEFT]);
    connect(act, SIGNAL(triggered()), consoleTabulator, SLOT(traverseLeft()));
    addAction(act);

    act = new QAction(this);
    act->setShortcut(Properties::Instance()->shortcuts[MOVE_LEFT]);
    connect(act, SIGNAL(triggered()), consoleTabulator, SLOT(moveLeft()));
    addAction(act);

    act = new QAction(this);
    act->setShortcut(Properties::Instance()->shortcuts[MOVE_RIGHT]);
    connect(act, SIGNAL(triggered()), consoleTabulator, SLOT(moveRight()));
    addAction(act);

    settings.endGroup();
}

MainWindow::~MainWindow()
{
}

void MainWindow::on_consoleTabulator_currentChanged(int)
{
}

void MainWindow::closeEvent(QCloseEvent* ev)
{
    if(QMessageBox::question(this,
                                tr("Close qterminal"),
                                tr("Are you sure you want to exit?"),
                                QMessageBox::Yes | QMessageBox::No) == QMessageBox::Yes)
    {
        Properties::Instance()->mainWindowGeometry = saveGeometry();
        Properties::Instance()->mainWindowState = saveState();
        Properties::Instance()->saveSettings();
        ev->accept();
    }
    else
    {
        ev->ignore();
    }
}

void MainWindow::quit()
{
    Properties::Instance()->mainWindowGeometry = saveGeometry();
    Properties::Instance()->mainWindowState = saveState();
    Properties::Instance()->saveSettings();
    QApplication::exit(0);
}

