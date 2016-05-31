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


#include "spinewindow.h"
#include "ui_spinewindow.h"
#include <QtGlobal>
#include <QFile>
#include <QSerialPortInfo>
#include <QGraphicsPixmapItem>
#include <QClipboard>
#include <QVarLengthArray>
#include <QFileDialog>
#include <QFileInfoList>
#include <QDir>
#include <QDebug>
#include <QSettings>
#include <QMessageBox>
#include <QToolBar>
#include <QDesktopServices>

SpineWindow::SpineWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::SpineWindow)
{
    ui->setupUi(this);
    connect(ui->pushButtonProgram, SIGNAL(clicked()), this, SLOT(programClicked()));

    codegen = new Codegen();

    setupCombo(ui->comboBoxA0, codegen->analogSensors());
    setupCombo(ui->comboBoxA1, codegen->analogSensors());
    setupCombo(ui->comboBoxA2, codegen->analogSensors());
    setupCombo(ui->comboBoxA3, codegen->analogSensors());


    setupCombo(ui->comboBoxD2, codegen->digitalSensors());
    setupCombo(ui->comboBoxD3, codegen->digitalSensors());
    setupCombo(ui->comboBoxD4, codegen->digitalSensors());
    setupCombo(ui->comboBoxD5, codegen->digitalSensors());
    setupCombo(ui->comboBoxD6, codegen->digitalSensors());
    setupCombo(ui->comboBoxD7, codegen->digitalSensors());
    setupCombo(ui->comboBoxD8, codegen->digitalSensors());

    setupCombo(ui->comboBoxI2C1, codegen->i2cSensors());
    setupCombo(ui->comboBoxI2C2, codegen->i2cSensors());
    setupCombo(ui->comboBoxI2C3, codegen->i2cSensors());
    setupCombo(ui->comboBoxI2C4, codegen->i2cSensors());

    setupCombo(ui->comboBoxUART, codegen->uartSensors());

    ui->textBrowser->setStyleSheet(QString::fromUtf8("background-color: rgb(200, 200, 200);"));

    ui->graphicsView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    ui->graphicsView->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    ui->graphicsView->setEnabled(false);
    QGraphicsScene* scene = new QGraphicsScene(this);
    QPixmap pixmap = QPixmap(QString(":/gfx/gfx/base_shield.png"));
    qDebug() << ui->graphicsView->geometry();
    pixmap = pixmap.scaledToWidth(300, Qt::SmoothTransformation);
    scene->addPixmap(pixmap);
    ui->graphicsView->setScene(scene);
    // ui->graphicsView->scale(0.3, 0.3);

    connect(ui->actionArduino, SIGNAL(triggered()), this, SLOT(actionArduino()));
    connect(ui->actionClipboard, SIGNAL(triggered()), this, SLOT(actionClipboard()));
    connect(ui->actionPath, SIGNAL(triggered()), this, SLOT(actionPath()));
    connect(ui->actionDocumentation, SIGNAL(triggered()), this, SLOT(actionDocumentation()));
    connect(ui->actionAbout, SIGNAL(triggered()), this, SLOT(actionAbout()));

    statusBar()->hide();
}


void SpineWindow::actionDocumentation()
{
    QDesktopServices::openUrl(QUrl("http://www.cemfi.de/research/spine/"));
}

void SpineWindow::actionAbout()
{

    QSplashScreen *splash = new QSplashScreen(QPixmap(":/gfx/gfx/spineprog-about.png"));
    splash->show();

}

void SpineWindow::actionArduino()
{
    codegen->deviceA0 = ui->comboBoxA0->currentText();
    codegen->deviceA1 = ui->comboBoxA1->currentText();
    codegen->deviceA2 = ui->comboBoxA2->currentText();
    codegen->deviceA3 = ui->comboBoxA3->currentText();  
    codegen->deviceD2 = ui->comboBoxD2->currentText();
    codegen->deviceD3 = ui->comboBoxD3->currentText();
    codegen->deviceD4 = ui->comboBoxD4->currentText();
    codegen->deviceD5 = ui->comboBoxD5->currentText();
    codegen->deviceD6 = ui->comboBoxD6->currentText();
    codegen->deviceD7 = ui->comboBoxD7->currentText();
    codegen->deviceD8 = ui->comboBoxD8->currentText();
    codegen->deviceI2c1 = ui->comboBoxI2C1->currentText();
    codegen->deviceI2c2 = ui->comboBoxI2C2->currentText();
    codegen->deviceI2c3 = ui->comboBoxI2C3->currentText();
    codegen->deviceI2c4 = ui->comboBoxI2C4->currentText();
    codegen->deviceUart = ui->comboBoxUART->currentText();
    codegen->generateCode();

    QFileDialog dialog(this, "Save Arduino Code", QDir::homePath());
    QString dest = dialog.getExistingDirectory();
    if (!dest.isEmpty()) {
        codegen->generateCode();
        codegen->copySketchTo(dest);
    }
}

QString SpineWindow::arduinoPath()
{
    QSettings settings("de.cemfi", "SPINEprog");
    QVariant value = settings.value("Arduino Path");
    if (!value.isNull()) {
        return value.toString();
    }
    else {
        return "";
    }
}


void SpineWindow::actionPath() {
    QFileDialog dialog(this, "Set Arduino Path");
#ifdef Q_OS_MAC
    QString dest = dialog.getOpenFileName(this, "Set Arduino Path", "", "Arduino.app");
#elif defined(Q_OS_WIN)
    QString dest = dialog.getOpenFileName(this, "Set Path to Arduino Executable", "", "arduino_debug.exe");
#endif
    if (!dest.isEmpty()) {
        QSettings settings("de.cemfi", "SPINEprog");
        settings.setValue(QString("Arduino Path"), dest);
    }

    qDebug() << arduinoPath();
}

void SpineWindow::actionClipboard()
{
    codegen->deviceA0 = ui->comboBoxA0->currentText();
    codegen->deviceA1 = ui->comboBoxA1->currentText();
    codegen->deviceA2 = ui->comboBoxA2->currentText();
    codegen->deviceA3 = ui->comboBoxA3->currentText();

    codegen->deviceD2 = ui->comboBoxD2->currentText();
    codegen->deviceD3 = ui->comboBoxD3->currentText();
    codegen->deviceD4 = ui->comboBoxD4->currentText();
    codegen->deviceD5 = ui->comboBoxD5->currentText();
    codegen->deviceD6 = ui->comboBoxD6->currentText();
    codegen->deviceD7 = ui->comboBoxD7->currentText();
    codegen->deviceD8 = ui->comboBoxD8->currentText();

    codegen->deviceI2c1 = ui->comboBoxI2C1->currentText();
    codegen->deviceI2c2 = ui->comboBoxI2C2->currentText();
    codegen->deviceI2c3 = ui->comboBoxI2C3->currentText();
    codegen->deviceI2c4 = ui->comboBoxI2C4->currentText();

    codegen->deviceUart = ui->comboBoxUART->currentText();

    QClipboard *clipboard = QApplication::clipboard();
    clipboard->setText(codegen->makePatcher());
}

void SpineWindow::setupCombo(QComboBox * combo, QVarLengthArray<QString> names)
{
   combo->addItem("No Device");
   int i;
   for (i = 0; i < names.size(); i++) {
       combo->addItem(names[i]);
   }
}

SpineWindow::~SpineWindow()
{
    delete ui;
}

bool SpineWindow::uniqueI2cModules() {
    bool isUnique = true;
    if (QString::compare(ui->comboBoxI2C1->currentText(), QString("No Device")) != 0) {
        isUnique = isUnique && (QString::compare(ui->comboBoxI2C1->currentText(), ui->comboBoxI2C2->currentText()) != 0);
        isUnique = isUnique && (QString::compare(ui->comboBoxI2C1->currentText(), ui->comboBoxI2C3->currentText()) != 0);
        isUnique = isUnique && (QString::compare(ui->comboBoxI2C1->currentText(), ui->comboBoxI2C4->currentText()) != 0);
    }
    if (QString::compare(ui->comboBoxI2C2->currentText(), QString("No Device")) != 0) {
        isUnique = isUnique && (QString::compare(ui->comboBoxI2C2->currentText(), ui->comboBoxI2C3->currentText()) != 0);
        isUnique = isUnique && (QString::compare(ui->comboBoxI2C2->currentText(), ui->comboBoxI2C4->currentText()) != 0);
    }
    if (QString::compare(ui->comboBoxI2C3->currentText(), QString("No Device")) != 0) {
        isUnique = isUnique && (QString::compare(ui->comboBoxI2C3->currentText(), ui->comboBoxI2C4->currentText()) != 0);
    }
    return isUnique;
}

void SpineWindow::programClicked()
{
    if (!uniqueI2cModules()) {
        QMessageBox box(this);
        box.setText("Duplicate I2C modules. Please only attach one sensor module of a kind to the I2C slots.");
        box.exec();
        return;
    }

    codegen->deviceA0 = ui->comboBoxA0->currentText();
    codegen->deviceA1 = ui->comboBoxA1->currentText();
    codegen->deviceA2 = ui->comboBoxA2->currentText();
    codegen->deviceA3 = ui->comboBoxA3->currentText();

    codegen->deviceD2 = ui->comboBoxD2->currentText();
    codegen->deviceD3 = ui->comboBoxD3->currentText();
    codegen->deviceD4 = ui->comboBoxD4->currentText();
    codegen->deviceD5 = ui->comboBoxD5->currentText();
    codegen->deviceD6 = ui->comboBoxD6->currentText();
    codegen->deviceD7 = ui->comboBoxD7->currentText();
    codegen->deviceD8 = ui->comboBoxD8->currentText();

    codegen->deviceI2c1 = ui->comboBoxI2C1->currentText();
    codegen->deviceI2c2 = ui->comboBoxI2C2->currentText();
    codegen->deviceI2c3 = ui->comboBoxI2C3->currentText();
    codegen->deviceI2c4 = ui->comboBoxI2C4->currentText();

    codegen->deviceUart = ui->comboBoxUART->currentText();

    codegen->generateCode();

    QString portStr = QString("");
    Q_FOREACH(QSerialPortInfo port, QSerialPortInfo::availablePorts()) {
        if (((port.vendorIdentifier() == 0x2341) || (port.vendorIdentifier() == 0x2A03)) && (port.productIdentifier() == 0x8036 || port.productIdentifier() == 0x0036)) {
          portStr = port.portName();
        }
    }
    if (portStr.isEmpty()) {
        ui->textBrowser->setText(QString("No Arduino Leonardo detected. Please plug it into the USB connector and hit \"Program\" again."));
        QMessageBox box(this);
        box.setText("No Arduino Leonardo detected. Please plug it into the USB connector and hit \"Program\" again.");
        box.exec();
        return;
    }

    makespineProcess = new QProcess();
    makespineProcess->setProcessChannelMode(QProcess::MergedChannels);
    connect(makespineProcess, SIGNAL(readyReadStandardOutput()), this, SLOT(readyReadStandardOutput()));
    connect(makespineProcess, SIGNAL(readyReadStandardError()), this, SLOT(readyReadStandardError()));
    connect(makespineProcess, SIGNAL(finished(int, QProcess::ExitStatus)), SLOT(finished(int, QProcess::ExitStatus)));

#ifdef Q_OS_MAC
    QString path = arduinoPath();
    path.append(QString("/Contents/MacOS/Arduino"));
    QFile executable(path);
    if (!executable.exists()) {
        QMessageBox box(this);
        box.setText("Plesase set path of the Arduino IDE with \"Settings - Arduino Path ...\". SPINEprog needs a current version of the Arduino IDE. You can download the Arduino IDE from http://arduino.cc ");
        box.exec();
    }
    QString sketchPath = codegen->getTemporaryDir()->path();
    sketchPath.append("/spine-sketch/spine-sketch.ino");
    QString port = QString("--port /dev/").append(portStr);
    QString arduinoCall = "\"" + path + "\"" + " --board arduino:avr:leonardo --upload " +  port +  " --verbose-build " + " --verbose-upload " +  "\"" + sketchPath + "\"";
    qDebug(arduinoCall.toLocal8Bit());
    makespineProcess->start("/bin/bash", QStringList() << "-c" << arduinoCall);
#elif defined(Q_OS_WIN)
    QString path = arduinoPath();
    QFile executable(path);
    if (!executable.exists()) {
        QMessageBox box(this);
        box.setText("Plesase set path of the Arduino IDE with \"Settings - Arduino Path ...\". SPINEprog needs a current version of the Arduino IDE. You can download the Arduino IDE from http://arduino.cc ");
        box.exec();
    }
    QString sketchPath = codegen->getTemporaryDir()->path();
    sketchPath.append("/spine-sketch/spine-sketch.ino");
    QString port = QString("--port ").append(portStr);
    QString arduinoArguments = "--board arduino:avr:leonardo --upload " +  port +  " --verbose-build --verbose-upload \"" + sketchPath + "\"";
    QString arduinoCall = "\"" + path + "\"" + " " + arduinoArguments;
    QFile batFile(codegen->getTemporaryDir()->path() + "/spine-sketch/run-arduino.bat");
    batFile.open(QIODevice::WriteOnly);
    QTextStream stream(&batFile);
    stream << arduinoCall;
    batFile.close();
    makespineProcess->start(codegen->getTemporaryDir()->path() + "/spine-sketch/run-arduino.bat");
#endif
    ui->textBrowser->setText(QString("executing..."));
}

void SpineWindow::addConnector(QXmlStreamWriter& xmlWriter, const char *name, QString device)
{
    xmlWriter.writeStartElement("Connector");
        xmlWriter.writeTextElement("Name", name);
        xmlWriter.writeTextElement("Device", device);
    xmlWriter.writeEndElement();
}

void SpineWindow::readyReadStandardOutput()
{
    ui->textBrowser->append(makespineProcess->readAllStandardOutput());
}

void SpineWindow::readyReadStandardError()
{
    ui->textBrowser->append(makespineProcess->readAllStandardError());
}

void SpineWindow::finished(int, QProcess::ExitStatus)
{
    ui->textBrowser->append("done!");
    ui->progressBar->setMinimum(0);
    ui->progressBar->setMaximum(100);
}
