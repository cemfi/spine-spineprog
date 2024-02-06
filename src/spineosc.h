// Copyright 2017 Aristotelis Hadjakos
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
    along with SPINEprog.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef SPINEOSC_H
#define SPINEOSC_H

#include "spinewindow.h"
#include <QThread>
#include <QObject>
#include "hidapi.h"

#define SEND_VALUES 0
#define SEND_INFO 1

class HIDThread : public QThread {
    Q_OBJECT

protected:
    void run();

private:
    void processBuffer(unsigned char *buf, int bytesRead);
    bool shouldRun;
    hid_device *handle = NULL;
    void openHID();

signals:
    void signal();
};

class SpineOSC {
public:
    static int port;
    static void startHidOsc(SpineWindow *win);
    static void setPort(int number);
    static void initialize();
    static void sendOSC(int connector);
    static QString paths[16];
    static QVector<float> values[16];
    static QString connectorToText(int connector);

private:
    static HIDThread *gThread;
    static bool portChanged;
};

#endif
