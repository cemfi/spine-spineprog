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


#include "codegen.h"
#include "spinewindow.h"
#include <QXmlStreamReader>
#include <QFile>
#include <QTextStream>
#include <QJsonArray>
#include <QJsonObject>
#include <QJSonDocument>
#include <QDebug>

using namespace Qt;

#include "spineosc.h"

Codegen::Codegen()
{
    loadModules();
    temporaryDir = new QTemporaryDir();
    qDebug() << temporaryDir->path();
    QDir dir;
    dir.mkpath(temporaryDir->path() + "/spine-sketch");

    // copy arduino code to temporary dir
    QFile::copy(":/code/code/accelerometer.h",   temporaryDir->path() + "/spine-sketch/accelerometer.h");
    QFile::copy(":/code/code/analog-generic.h",  temporaryDir->path() + "/spine-sketch/analog-generic.h");
    QFile::copy(":/code/code/digital-generic.h", temporaryDir->path() + "/spine-sketch/digital-generic.h");
    QFile::copy(":/code/code/dummy.h",           temporaryDir->path() + "/spine-sketch/dummy.h");
    QFile::copy(":/code/code/gps.h",             temporaryDir->path() + "/spine-sketch/gps.h");
    QFile::copy(":/code/code/imu-9dof.h",        temporaryDir->path() + "/spine-sketch/imu-9dof.h");
    QFile::copy(":/code/code/us-ranger.h",       temporaryDir->path() + "/spine-sketch/us-ranger.h");
    QFile::copy(":/code/code/Spine.h",           temporaryDir->path() + "/spine-sketch/Spine.h");
    QFile::copy(":/code/code/Spine.cpp",         temporaryDir->path() + "/spine-sketch/Spine.cpp");

}

void Codegen::loadModules()
{
    QFile xmlFile(":/xml/xml/modules.xml");
    xmlFile.open(QIODevice::ReadOnly);
    QXmlStreamReader reader(&xmlFile);

    while(!reader.atEnd()) {
        reader.readNext();
        if(reader.isStartElement()) {
            QString tag(reader.name().toString());
            if (QString::compare(tag, QString("device"), Qt::CaseInsensitive) == 0) {
                modules.append(Module());
            }
            if (QString::compare(tag, QString("channels"), Qt::CaseInsensitive) == 0) {
                while(!reader.atEnd()) {
                    reader.readNext();
                    if(reader.isCharacters() && !reader.isWhitespace()) {
                        QString text(reader.text().toString());
                        modules.last().channels = text.toInt();
                        break;
                    }
                }
            }
            if (QString::compare(tag, QString("code"), Qt::CaseInsensitive) == 0) {
                while(!reader.atEnd()) {
                    reader.readNext();
                    if(reader.isCharacters() && !reader.isWhitespace()) {
                        QString text(reader.text().toString());
                        modules.last().code = text;
                        break;
                    }
                }
            }
            if (QString::compare(tag, QString("type"), Qt::CaseInsensitive) == 0) {
                while(!reader.atEnd()) {
                    reader.readNext();
                    if(reader.isCharacters() && !reader.isWhitespace()) {
                        QString text(reader.text().toString());
                        modules.last().type = text;
                        break;
                    }
                }
            }
            if (QString::compare(tag, QString("name"), Qt::CaseInsensitive) == 0) {
                while(!reader.atEnd()) {
                    reader.readNext();
                    if(reader.isCharacters() && !reader.isWhitespace()) {
                        QString text(reader.text().toString());
                        modules.last().name = text;
                        break;
                    }
                }
            }
            if (QString::compare(tag, QString("shortname"), Qt::CaseInsensitive) == 0) {
                while(!reader.atEnd()) {
                    reader.readNext();
                    if(reader.isCharacters() && !reader.isWhitespace()) {
                        QString text(reader.text().toString());
                        modules.last().shortname = text;
                        break;
                    }
                }
            }

        }

    }
}

QVarLengthArray<QString> Codegen::analogSensors()
{
    QVarLengthArray<QString> result;

    int i;
    for (i = 0; i < modules.size(); i++) {
        if (QString::compare(modules[i].type, QString("analog"), Qt::CaseInsensitive) == 0) {
            result.append(modules[i].name);
        }
    }

    return result;
}

QVarLengthArray<QString> Codegen::digitalSensors()
{
    QVarLengthArray<QString> result;

    int i;
    for (i = 0; i < modules.size(); i++) {
        if (QString::compare(modules[i].type, QString("digital"), Qt::CaseInsensitive) == 0) {
            result.append(modules[i].name);
        }
    }

    return result;
}

QVarLengthArray<QString> Codegen::i2cSensors()
{
    QVarLengthArray<QString> result;

    int i;
    for (i = 0; i < modules.size(); i++) {
        if (QString::compare(modules[i].type, QString("i2c"), Qt::CaseInsensitive) == 0) {
            result.append(modules[i].name);
        }
    }

    return result;
}

QVarLengthArray<QString> Codegen::uartSensors()
{
    QVarLengthArray<QString> result;

    int i;
    for (i = 0; i < modules.size(); i++) {
        if (QString::compare(modules[i].type, QString("uart"), Qt::CaseInsensitive) == 0) {
            result.append(modules[i].name);
        }
    }

    return result;
}

void Codegen::generateInclude(QTextStream& stream, const char *spacename, const QString& deviceName, int connector, int pinA, int pinB)
{
    QString toInclude;
    int i;
    for (i = 0; i < modules.size(); i++) {
        if (QString::compare(deviceName, modules[i].name, Qt::CaseInsensitive) == 0) {
            toInclude = modules[i].code;
            break;
        }
    }
    if (i == modules.size()) {
        return;
    }

    stream << "namespace " << spacename << " {" << endl;
    stream << "    uint8_t connector = " << connector << ";" << endl;
    stream << "    uint8_t PIN_A = " << pinA << ";" << endl;
    stream << "    uint8_t PIN_B = " << pinB << ";" << endl;
    stream << "    #include \"" << toInclude << "\"" << endl;
    stream << "}" << endl << endl;
}

void Codegen::generateSetup(QTextStream& stream, const char *spacename, const QString& deviceName)
{
    if (QString::compare(deviceName, QString("No Device"), Qt::CaseInsensitive) != 0) {
        QString sn = moduleForName(deviceName).shortname.replace(" ", "-");
        stream << "    " << "Spine.configSensor(" << spacename << "::connector, \"" << sn << "\", \"f\");";
        stream << "    " << spacename << "::setup();" << endl;
    }
}

void Codegen::generateLoop(QTextStream& stream, const char *spacename, const QString& deviceName)
{
    if (QString::compare(deviceName, QString("No Device"), Qt::CaseInsensitive) != 0) {
        stream << "    " << spacename << "::loop();" << endl;
    }
}

void Codegen::generateCode()
{
    QFile inoFile(temporaryDir->path() + "/spine-sketch/spine-sketch.ino");
    inoFile.open(QIODevice::WriteOnly);
    QTextStream stream(&inoFile);

    stream << "// This code was generated by spine-codegen" << endl << endl;
    stream << "#include \"Spine.h\"" << endl;
    stream << "#include <Wire.h>" << endl << endl;

    generateInclude(stream, "ConA0", deviceA0, 1, 18, 18);
    generateInclude(stream, "ConA1", deviceA1, 2, 19, 19);
    generateInclude(stream, "ConA2", deviceA2, 3, 20, 20);
    generateInclude(stream, "ConA3", deviceA3, 4, 21, 21);

    generateInclude(stream, "ConD2", deviceD2, 5, 2, 3);
    generateInclude(stream, "ConD3", deviceD3, 6, 3, 4);
    generateInclude(stream, "ConD4", deviceD4, 7, 4, 5);
    generateInclude(stream, "ConD5", deviceD5, 8, 5, 6);
    generateInclude(stream, "ConD6", deviceD6, 9, 6, 7);
    generateInclude(stream, "ConD7", deviceD7, 10, 7, 8);
    generateInclude(stream, "ConD8", deviceD8, 11, 8, 9);

    generateInclude(stream, "ConI2C1", deviceI2c1, 12, 2, 3);
    generateInclude(stream, "ConI2C2", deviceI2c2, 13, 2, 3);
    generateInclude(stream, "ConI2C3", deviceI2c3, 14, 2, 3);
    generateInclude(stream, "ConI2C4", deviceI2c4, 15, 2, 3);

    generateInclude(stream, "ConUART", deviceUart, 16, 0, 1);

    // setup
    stream << "void setup() {" << endl;
    stream << "    Spine.begin();" << endl;
    generateSetup(stream, "ConA0", deviceA0);
    generateSetup(stream, "ConA1", deviceA1);
    generateSetup(stream, "ConA2", deviceA2);
    generateSetup(stream, "ConA3", deviceA3);

    generateSetup(stream, "ConD2", deviceD2);
    generateSetup(stream, "ConD3", deviceD3);
    generateSetup(stream, "ConD4", deviceD4);
    generateSetup(stream, "ConD5", deviceD5);
    generateSetup(stream, "ConD6", deviceD6);
    generateSetup(stream, "ConD7", deviceD7);
    generateSetup(stream, "ConD8", deviceD8);

    generateSetup(stream, "ConI2C1", deviceI2c1);
    generateSetup(stream, "ConI2C2", deviceI2c2);
    generateSetup(stream, "ConI2C3", deviceI2c3);
    generateSetup(stream, "ConI2C4", deviceI2c4);

    generateSetup(stream, "ConUART", deviceUart);
    stream << "}" << endl << endl;

    // loop
    stream << "void loop() {" << endl;
    generateLoop(stream, "ConA0", deviceA0);
    generateLoop(stream, "ConA1", deviceA1);
    generateLoop(stream, "ConA2", deviceA2);
    generateLoop(stream, "ConA3", deviceA3);

    generateLoop(stream, "ConD2", deviceD2);
    generateLoop(stream, "ConD3", deviceD3);
    generateLoop(stream, "ConD4", deviceD4);
    generateLoop(stream, "ConD5", deviceD5);
    generateLoop(stream, "ConD6", deviceD6);
    generateLoop(stream, "ConD7", deviceD7);
    generateLoop(stream, "ConD8", deviceD8);

    generateLoop(stream, "ConI2C1", deviceI2c1);
    generateLoop(stream, "ConI2C2", deviceI2c2);
    generateLoop(stream, "ConI2C3", deviceI2c3);
    generateLoop(stream, "ConI2C4", deviceI2c4);

    generateLoop(stream, "ConUART", deviceUart);
    stream << "    wait10ms();" << endl;
    stream << "}" << endl << endl;

    stream << "void wait10ms() {" << endl;
    stream << "  static unsigned long lastMillis = millis();" << endl;
    stream << "  unsigned long elapsed = millis() - lastMillis;" << endl;
    stream << "  if (elapsed < 10) {" << endl;
    stream << "      delay(10 - elapsed);" << endl;
    stream << "  }" << endl;
    stream << "}" << endl << endl;

    inoFile.close();
}

bool Codegen::isDevice(const QString& name)
{
    return QString::compare(name, QString("No Device"), Qt::CaseInsensitive) != 0;
}

Module Codegen::moduleForName(const QString& name)
{
    int i;
    for (i = 0; i < modules.size(); i++) {
        if (QString::compare(name, modules[i].name, Qt::CaseInsensitive) == 0) {
            return modules[i];
        } 
    }
    return Module();
}

QString Codegen::makePatcher(int port)
{
    QVarLengthArray<int> connectors;
    QVarLengthArray<Module> usedModules;
    
    if (isDevice(deviceA0)) {
        usedModules.append(moduleForName(deviceA0));
        connectors.append(1);
    }
    if (isDevice(deviceA1)) {
        usedModules.append(moduleForName(deviceA1));
        connectors.append(2);
    }
    if (isDevice(deviceA2)) {
        usedModules.append(moduleForName(deviceA2));
        connectors.append(3);
    }
    if (isDevice(deviceA3)) {
        usedModules.append(moduleForName(deviceA3));
        connectors.append(4);
    }

    if (isDevice(deviceD2)) {
        usedModules.append(moduleForName(deviceD2));
        connectors.append(5);
    }
    if (isDevice(deviceD3)) {
        usedModules.append(moduleForName(deviceD3));
        connectors.append(6);
    }
    if (isDevice(deviceD4)) {
        usedModules.append(moduleForName(deviceD4));
        connectors.append(7);
    }
    if (isDevice(deviceD5)) {
        usedModules.append(moduleForName(deviceD5));
        connectors.append(8);
    }
    if (isDevice(deviceD6)) {
        usedModules.append(moduleForName(deviceD6));
        connectors.append(9);
    }
    if (isDevice(deviceD7)) {
        usedModules.append(moduleForName(deviceD7));
        connectors.append(10);
    }
    if (isDevice(deviceD8)) {
        usedModules.append(moduleForName(deviceD8));
        connectors.append(11);
    }

    if (isDevice(deviceI2c1)) {
        usedModules.append(moduleForName(deviceI2c1));
        connectors.append(12);
    }
    if (isDevice(deviceI2c2)) {
        usedModules.append(moduleForName(deviceI2c2));
        connectors.append(13);
    }
    if (isDevice(deviceI2c3)) {
        usedModules.append(moduleForName(deviceI2c3));
        connectors.append(14);
    }
    if (isDevice(deviceI2c4)) {
        usedModules.append(moduleForName(deviceI2c4));
        connectors.append(15);
    }

    if (isDevice(deviceUart)) {
        usedModules.append(moduleForName(deviceUart));
        connectors.append(16);
    }

    return buildPatcher(port, connectors, usedModules);
}

QString Codegen::buildPatcher(int port, QVarLengthArray<int> connectors, QVarLengthArray<Module> usedModules)
{
    boxesArray = new QJsonArray();
    linesArray = new QJsonArray();
    maxX = 0;

    QVarLengthArray<int> channels;
    QVarLengthArray<QString> names;
    QVarLengthArray<QString> types;
    for (int i = 0; i < usedModules.size(); i++) {
        channels.append(usedModules[i].channels);
        names.append(usedModules[i].shortname);
        types.append(usedModules[i].type);
    }

    QString udpreceive = "udpreceive ";
    udpreceive.append(QString::asprintf("%d", port));
    QJsonObject box;
    QJsonObject boxEntry;
    boxEntry["maxclass"] = "newobj";
    boxEntry["text"] = udpreceive;
    boxEntry["fontname"] = "Arial";
    QJsonArray patchingRect;
    patchingRect.append(32.0);
    patchingRect.append(30.0);
    patchingRect.append(80.0);
    patchingRect.append(20.0);
    boxEntry["patching_rect"] = patchingRect;
    boxEntry["numinlets"] =  1;
    boxEntry["id"] = "udpreceive";
    boxEntry["fontsize"] = 12.0;
    boxEntry["numoutlets"] = 1;
    QJsonArray outlettype;
    boxEntry["outlettype"] = outlettype;
    box["box"] = boxEntry;
    boxesArray->append(box);

    boxEntry["maxclass"] = "newobj";
    boxEntry["text"] = "regexp /spine/ @substitute \" \"";
    boxEntry["fontname"] = "Arial";
    patchingRect = QJsonArray();
    patchingRect.append(32.0);
    patchingRect.append(70.0);
    patchingRect.append(80.0);
    patchingRect.append(20.0);
    boxEntry["patching_rect"] = patchingRect;
    boxEntry["numinlets"] =  2;
    boxEntry["id"] = "firstroute";
    boxEntry["fontsize"] = 12.0;
    boxEntry["numoutlets"] = 2;
    boxEntry["outlettype"] = outlettype;
    box["box"] = boxEntry;
    boxesArray->append(box);

    boxEntry["maxclass"] = "newobj";
    QString routeText = "route ";
    for (int i = 0; i < usedModules.size(); i++) {
        QString target = "";
        target.append(SpineOSC::connectorToText(connectors[i]));
        target.append(usedModules[i].shortname);
        routeText.append(target);
        routeText.append(" ");
    }
    boxEntry["text"] = routeText.toUtf8().constData();
    boxEntry["fontname"] = "Arial";
    patchingRect = QJsonArray();
    patchingRect.append(32.0);
    patchingRect.append(110.0);
    patchingRect.append(80.0);
    patchingRect.append(20.0);
    boxEntry["patching_rect"] = patchingRect;
    boxEntry["numinlets"] =  2;
    boxEntry["id"] = "secondroute";
    boxEntry["fontsize"] = 12.0;
    boxEntry["numoutlets"] = usedModules.size() + 1;
    boxEntry["outlettype"] = outlettype;
    box["box"] = boxEntry;
    boxesArray->append(box);

    connect("udpreceive", 0, "firstroute", 0);
    connect("firstroute", 0, "secondroute", 0);

    int numSensors = connectors.size();
    int horizontalDisplacement = 0;
    int horizontalDisplacementLevel2 = 0;
    for (int i = 1; i <= numSensors; i++) {
        if (channels[i - 1] == 1) {
            makeComment(32 + horizontalDisplacement, 170 - 14, names[i - 1]);
            if (QString::compare(types[i - 1], "digital", Qt::CaseInsensitive) != 0) {
                makeFlonum(QString("flonum-%1").arg(i), 32 + horizontalDisplacement, 170);
                connect("secondroute", connectors[i - 1] - 1, QString("flonum-%1").arg(i), 0);
            }
            else {
                makeToggle(QString("toggle-%1").arg(i), 32 + horizontalDisplacement, 170);
                connect("secondroute", connectors[i - 1] - 1, QString("toggle-%1").arg(i), 0);
            }
            horizontalDisplacement += 60;
        } else {
            makeComment(32 + horizontalDisplacement, 170 - 14, names[i - 1]);
            makeSelect(QString("sub-route-%1").arg(i), 32 + horizontalDisplacement, 170, channels[i - 1]);
            connect("secondroute", i - 1, QString("sub-route-%1").arg(i), 0);
            horizontalDisplacement += sizeOfSelect(channels[i - 1]) + 10;
            for (int j = 1; j <= channels[i - 1]; j++) {
                if (QString::compare(types[i - 1], "digital", Qt::CaseInsensitive) != 0) {
                    QString flonumID = QString("flonum-%1-%2").arg(i).arg(j);
                    makeFlonum(flonumID, 32 + horizontalDisplacementLevel2, 240);
                    connect(QString("sub-route-%1").arg(i), j - 1, flonumID, 0);
                }
                else {
                    QString toggleID = QString("toggle-%1-%2").arg(i).arg(j);
                    makeToggle(toggleID, 32 + horizontalDisplacementLevel2, 240);
                    connect(QString("sub-route-%1").arg(i), j - 1, toggleID, 0);
                }
                horizontalDisplacementLevel2 += 60;
            }
        }
    }

    QJsonObject result;
    result["boxes"] = *boxesArray;
    result["lines"] = *linesArray;
    QJsonArray rect;
    rect.append(20.0);
    rect.append(20.0);
    rect.append(maxX + 3);
    rect.append(230.0);
    result["rect"] = rect;
    
    QJsonObject version;
    version["major"] = 6;
    version["minor"] = 1;
    version["revision"] = 5,
    version["architecture"] = "x86";
    result["appversion"] = version;

    QJsonDocument document;
    document.setObject(result);
    QString text =  QString(document.toJson());
    return text;
}

void Codegen::connect(const QString& id1, int outlet, const QString& id2, int inlet) {

    QJsonObject patchline;
    QJsonArray source;
    source.append(id1);
    source.append(outlet);
    QJsonArray dest;
    dest.append(id2);
    dest.append(inlet);
    patchline["source"] = source;
    patchline["destination"] = dest;
    patchline["hidden"] = 0;
    patchline["disabled"] = 0;
    QJsonObject line;
    line["patchline"] = patchline;
    linesArray->append(line);
}

void Codegen::makeSelect(const QString& id, int x, int y, int numOutlets) {
    QString routeString = QString("unpack ");
    for (int i = 1; i <= numOutlets; i++) {
        routeString = routeString.append(" 0.");
    }
    QJsonObject boxElement;
    boxElement["maxclass"] = "newobj";
    boxElement["text"] = routeString;
    boxElement["fontname"] = "Arial";
    QJsonArray patchingRect;
    patchingRect.append(x);
    patchingRect.append(y);
    patchingRect.append(sizeOfSelect(numOutlets));
    patchingRect.append(20.0);
    boxElement["patching_rect"] = patchingRect; //
    boxElement["numinlets"] = 5;
    boxElement["id"] = id;
    boxElement["fontsize"] = 12.0;
    boxElement["numoutlets"] = 5;
    QJsonArray outlettype;
    outlettype.append("bang");
    outlettype.append("bang");
    outlettype.append("bang");
    outlettype.append("bang");
    outlettype.append("");
    boxElement["outlettype"] = outlettype;

    QJsonObject box;
    box["box"] = boxElement;
    if (x + sizeOfSelect(numOutlets) > maxX) {
        maxX = x + sizeOfSelect(numOutlets);
    }
    boxesArray->append(box);
}

void Codegen::makeFlonum(const QString& id, int x, int y) {
    QJsonObject boxElement;
    boxElement["maxclass"] = "flonum";
    boxElement["fontname"] = "Arial";
    QJsonArray patchingRect;
    patchingRect.append(x);
    patchingRect.append(y);
    patchingRect.append(50.0);
    patchingRect.append(20.0);
    boxElement["patching_rect"] = patchingRect;
    boxElement["numinlets"] = 1;
    boxElement["id"] = id;
    boxElement["parameter_enable"] = 0;
    boxElement["fontsize"] = 12.0;
    boxElement["numoutlets"] = 2;
    QJsonArray outlettype;
    outlettype.append("float");
    outlettype.append("bang");
    boxElement["outlettype"] = outlettype; // [ "float", "bang" ]

    QJsonObject box;
    box["box"] = boxElement;
    if (x + 50 > maxX) {
        maxX = x + 50;
    }
    boxesArray->append(box);
}

void Codegen::makeToggle(const QString& id, int x, int y) {
    QJsonObject boxElement;
    boxElement["maxclass"] = "toggle";
    boxElement["numoutlets"] = 1;
    QJsonArray patchingRect;
    patchingRect.append(x);
    patchingRect.append(y);
    patchingRect.append(24.0);
    patchingRect.append(24.0);
    boxElement["patching_rect"] = patchingRect;
    boxElement["id"] = id;
    QJsonArray outlettype;
    outlettype.append("int");
    boxElement["outlettype"] = outlettype; // [ "int" ],
    boxElement["parameter_enable"] = 0;
    boxElement["style"] = "";
    boxElement["numinlets"] = 1;

    QJsonObject box;
    box["box"] = boxElement;

    if (x + 50 > maxX) {
        maxX = x + 50;
    }
    boxesArray->append(box);
}

void Codegen::makeComment(int x, int y, const QString& text) {
    QJsonObject boxElement;
    boxElement["maxclass"] = "comment";
    boxElement["text"] = text;
    boxElement["numoutlets"] = 0;
    QJsonArray patchingRect;
    patchingRect.append(x);
    patchingRect.append(y);
    patchingRect.append(text.length() * 4);
    patchingRect.append(20.0);
    boxElement["patching_rect"] = patchingRect;
    boxElement["id"] = "comment";
    boxElement["fontsize"] = 8.0;
    boxElement["style"] = "";
    boxElement["numinlets"] = 1;

    QJsonObject box;
    box["box"] = boxElement;
    if (x + 50 > maxX) {
        maxX = x + 50;
    }
    boxesArray->append(box);
}

int Codegen::sizeOfSelect(int numOutlets) {
    return 48 + 10 * numOutlets;
}

void Codegen::copySketchTo(const QString& dest)
{
    QDir().mkdir(dest + "/spine-sketch");
    copyDir(temporaryDir->path() + "/spine-sketch", dest + "/spine-sketch");
}

bool Codegen::copyDir(const QString &src, const QString &dest) {
    QDir dirsrc(src);
    QDir dirdest(dest);
    if(!dirsrc.isReadable()) {
        return false;
    }
    QFileInfoList entries = dirsrc.entryInfoList();
    for(QList<QFileInfo>::iterator it = entries.begin(); it!=entries.end(); ++it) {
        QFileInfo &finfo = *it;
        if(finfo.fileName() == "." || finfo.fileName() == "..") {
            continue;
        }
        if(finfo.isFile()) {
            qDebug(finfo.filePath().toLocal8Bit());
            qDebug(dirdest.absoluteFilePath(finfo.fileName()).toLocal8Bit());
            QFile::copy(finfo.filePath(), dirdest.absoluteFilePath(finfo.fileName()));
        } else {
            return false;
        }
    }
    return true;
}

QTemporaryDir *Codegen::getTemporaryDir()
{
    return temporaryDir;
}


