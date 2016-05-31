// Copyright 2015 Aristotelis Hadjakos
// Contact: thadjakos@gmail.com

/*
    This file is part of SPINEprog.

    SPINEprog is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    SPINEprog is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with Foobar.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef SPINEWINDOW_H
#define SPINEWINDOW_H

#include <QMainWindow>
#include <QXmlStreamWriter>
#include <QComboBox>
#include <QProcess>
#include "codegen.h"
#include <QVarLengthArray>
#include <QSplashScreen>

namespace Ui {
class SpineWindow;
}

class SpineWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit SpineWindow(QWidget *parent = 0);
    ~SpineWindow();

private:
    Ui::SpineWindow *ui;
    void createXml();
    void addConnector(QXmlStreamWriter& xmlWriter, const char *name, QString device);
    void setupCombo(QComboBox * combo, QVarLengthArray<QString> names);
    QProcess *makespineProcess;
    Codegen *codegen;
    QString arduinoPath();
    bool uniqueI2cModules();

private slots:
    void programClicked();
    void readyReadStandardOutput();
    void readyReadStandardError();
    void finished(int, QProcess::ExitStatus);
    void actionArduino();
    void actionClipboard();
    void actionPath();
    void actionDocumentation();
    void actionAbout();
};

#endif // SPINEWINDOW_H
