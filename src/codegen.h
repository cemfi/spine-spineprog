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


#ifndef CODEGEN_H
#define CODEGEN_H

#include <QVarLengthArray>
#include <QString>
#include <QTextStream>
#include <QJsonArray>
#include <QTemporaryDir>

class Module
{
public:
    int channels;
    QString code;
    QString type;
    QString name;
    QString shortname;
    int connector;
};

class Codegen
{
public:
    Codegen();

    void generateCode();
    QString
        deviceA0, deviceA1, deviceA2, deviceA3,
        deviceD2, deviceD3, deviceD4, deviceD5, deviceD6, deviceD7, deviceD8,
        deviceI2c1, deviceI2c2, deviceI2c3, deviceI2c4,
        deviceUart;

    QVarLengthArray<QString> analogSensors();
    QVarLengthArray<QString> digitalSensors();
    QVarLengthArray<QString> i2cSensors();
    QVarLengthArray<QString> uartSensors();

    QString makePatcher();
    void copySketchTo(const QString& dest);
    QTemporaryDir *getTemporaryDir();

private:
    void loadModules();
    QVarLengthArray<Module> modules;
    void generateInclude(QTextStream& stream, const char *spacename, const QString& deviceName, int connector, int pinA, int pinB);
    void generateSetup(QTextStream& stream, const char *spacename, const QString& deviceName);
    void generateLoop(QTextStream& stream, const char *spacename, const QString& deviceName);
    bool isDevice(const QString& name);
    QString buildPatcher(QVarLengthArray<int> connectors, QVarLengthArray<Module> modules);
    QJsonArray *boxesArray;
    QJsonArray *linesArray;
    int maxX;
    Module moduleForName(const QString& name);

    void connect(const QString& id1, int outlet, const QString& id2, int inlet);
    void makeSelect(const QString& id, int x, int y, int numOutlets);
    void makeFlonum(const QString& id, int x, int y);
    void makeToggle(const QString& id, int x, int y);
    void makeComment(int x, int y, const QString& text);
    int sizeOfSelect(int numOutlets);

    QTemporaryDir *temporaryDir;
    bool copyDir(const QString &src, const QString &dest);

};


#endif // CODEGEN_H
