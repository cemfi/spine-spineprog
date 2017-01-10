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


class HIDThread : public QThread {
    Q_OBJECT

protected:
    void run();

private:
    void initSettings();
    bool allSettingsReceived();
    bool settingsWereSent = false;
    bool receivedSetting[16];
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
    static int settings[16];
    static void initialize();
    static void sendOSC(float value, int connector, int channel);

private:
    static HIDThread *gThread;
    static bool portChanged;
};

#endif
