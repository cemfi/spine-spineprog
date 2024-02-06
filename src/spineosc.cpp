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

#include "spineosc.h"
#include <QtGlobal>
#include <QThread>
#include <QDebug>



#include "hidapi.h"
#include <stdio.h>
#include "udp.hh"
#include "oscpkt.hh"
using namespace oscpkt;

#include "spinewindow.h"
#include <QApplication>
#include <QObject>

int SpineOSC::port = 9876;
HIDThread *SpineOSC::gThread = 0;
bool SpineOSC::portChanged = false;
QString SpineOSC::paths[16];
QVector<float> SpineOSC::values[16];

void HIDThread::openHID()
{
    handle = hid_open(0x2341, 0x8036, NULL);
    if (!handle) {
        return;
    }
    hid_set_nonblocking(handle, 1);
}

void SpineOSC::setPort(int portnumber) {
    SpineOSC::port = portnumber;
    SpineOSC::portChanged = true;
}

void SpineOSC::sendOSC(int connector)
{
    static UdpSocket *sock = NULL;
    if (!sock || !sock->isOk() || SpineOSC::portChanged) {
        if (sock) {
            sock->close();
            delete sock;
        }

        sock = new UdpSocket;
        sock->connectTo("localhost", port);
        SpineOSC::portChanged = false;
    }
    if (sock->isOk()) {
        QString path = "/spine/";
        path = path + connectorToText(connector) + paths[connector];
        Message msg(path.toUtf8().constData());
        for (float value : SpineOSC::values[connector]) {
            msg.pushFloat(value);
        }

        PacketWriter pw;
        pw.addMessage(msg);
        sock->sendPacket(pw.packetData(), pw.packetSize());
    }
}

QString SpineOSC::connectorToText(int connector)
{
    if (connector == 1) return QString("A0-");
    if (connector == 2) return QString("A1-");
    if (connector == 3) return QString("A2-");
    if (connector == 4) return QString("A3-");

    if (connector == 5) return QString("D2-");
    if (connector == 6) return QString("D3-");
    if (connector == 7) return QString("D4-");
    if (connector == 8) return QString("D5-");
    if (connector == 9) return QString("D6-");
    if (connector == 10) return QString("D7-");
    if (connector == 11) return QString("D8-");

    if (connector >= 12 and connector <= 15) return QString("I2C-");

    if (connector == 16) return QString("UART-");

    return "";
}

void HIDThread::processBuffer(unsigned char *buf, int bytesRead)
{
    if (buf[1] == SEND_INFO) {
        qDebug() << "buf[0] =" << buf[0] << "buf[1..12] =" << buf[1] << buf[2] << buf[3] << buf[4] << buf[5] << buf[6] << buf[7] << buf[8] << buf[9] << buf[10] << buf[11] << buf[12] ;

        int connector = buf[2];
        QString shortName = QString::fromUtf8(buf + 3);
        QString format = QString::fromUtf8(buf + 3 + shortName.size() + 1); // +1 to skip the null terminator

        SpineOSC::paths[connector] = shortName;
        if (SpineOSC::values[connector].size() != format.length()) {
            SpineOSC::values[connector] = QVector<float>(format.length());
        }

        qDebug() << "SEND_INFO: " << "connector =" << connector << ", shortName =" << shortName << ", format =" << format;
    }
    else if (buf[1] == SEND_VALUES) {
        const int NUM_MESSAGES = 10;
        QSet<int> connectors;
        for (int i=0; i < NUM_MESSAGES; i++) {
            int connector = buf[2 + 6 * i];
            int valueId = buf[3 + 6 * i];
            union {
                float fvalue;
                unsigned char bvalue[4];
            } fbConversion;
            fbConversion.bvalue[0] = buf[4 + 6 * i];
            fbConversion.bvalue[1] = buf[5 + 6 * i];
            fbConversion.bvalue[2] = buf[6 + 6 * i];
            fbConversion.bvalue[3] = buf[7 + 6 * i];
            // qDebug() << "connector =" << connector << ", valueId =" << valueId << ", value =" << fbConversion.fvalue;
            if (SpineOSC::values[connector].size() > valueId) {
                SpineOSC::values[connector][valueId] = fbConversion.fvalue;
                connectors.insert(connector);
            }
        }
        for (int connector : connectors) {
            SpineOSC::sendOSC(connector);
        }
    }
}

void HIDThread::run()
{
    shouldRun = true;
    while (shouldRun) {
        if (!handle) {
            openHID();
            if (!handle) {
                QThread::usleep(100);
            }
        }
        if (handle) {
            unsigned char buf[256];
            memset(buf, 0, sizeof(buf));
            buf[0] = 0x3;
            int res = hid_read(handle, buf, sizeof(buf));
            if (res == 0) {
                // waiting
                QThread::usleep(10);
            }
            else if (res < 0) {
                // unable to read
                hid_close(handle);
                handle = NULL;
            }
            else {
                // OK
                if (buf[0] == 3) {
                    processBuffer(buf, res);
                }
            }
        }
    }
}

void update()
{
    // Nothing to do
}

void SpineOSC::startHidOsc(SpineWindow *win) {
    if (gThread == NULL) {
        gThread = new HIDThread();
        QObject::connect(gThread, SIGNAL(signal()), win, SLOT(update()));
        gThread->start();
    }
}

