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
int SpineOSC::settings[16];

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

void SpineOSC::sendOSC(float value, int connector, int channel)
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
		Message msg("/spine");
		msg.pushInt32(connector);
        if (channel != 0) {
            msg.pushInt32(channel);
        }
		msg.pushFloat(value);
		PacketWriter pw;
		pw.addMessage(msg);
		sock->sendPacket(pw.packetData(), pw.packetSize());
	}
}




void HIDThread::initSettings() {
    int i;
    for (i = 0; i < 16; i++) {
        SpineOSC::settings[i] = 0;
        receivedSetting[i] = false;
    }
}

bool HIDThread::allSettingsReceived() {
        int i;
        for (i = 0; i < 16; i++) {
            if (receivedSetting[i] == false) {
                return false;
            }
        }
        return true;
    }


void HIDThread::run()
{
    initSettings();

	shouldRun = true;

    int connector_id[10];
    int channel_id[10];
    double last_value[10];


	while (shouldRun) {
		int numMessages = 0;
		int spineID = 0;

		if (!handle) {
			openHID();
			if (!handle) {
                QThread::usleep(100);
                initSettings();
                settingsWereSent = false;
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
				if ((res == 62) && (buf[0] == 3)) {
					union {
						float fvalue;
						unsigned char bvalue[4];
					} fbConversion;

					numMessages = buf[1] >> 4;
					spineID = buf[1] & 0xF;

					int i;
					for (i = 0; i < numMessages; i++) {
                        // third byte: 5 bits value id (1F - no compund), 3 bits slot-ID
                        connector_id[i] = buf[2 + 6 * i];
                        channel_id[i] = buf[3 + 6 * i];
                        fbConversion.bvalue[0] = buf[4 + 6 * i];
                        fbConversion.bvalue[1] = buf[5 + 6 * i];
                        fbConversion.bvalue[2] = buf[6 + 6 * i];
                        fbConversion.bvalue[3] = buf[7 + 6 * i];
                        last_value[i] = fbConversion.fvalue;
					}
				}
			}
		}
		int i;
		for (i = 0; i < numMessages; i++) {
			float value = last_value[i];
            int connector = connector_id[i];
            int channel = channel_id[i];
            if (connector != 255) {
                SpineOSC::sendOSC(value, connector, channel);
            }
            else {
                SpineOSC::settings[channel] = value;
                receivedSetting[channel] = true;
                if (!settingsWereSent && allSettingsReceived()) {
                    emit signal();
                    settingsWereSent = true;
                }
            }
		}
	}
}



void SpineOSC::startHidOsc(SpineWindow *win) {
    if (gThread == NULL) {
        gThread = new HIDThread();
        QObject::connect(gThread, SIGNAL(signal()), win, SLOT(update()));
        gThread->start();
    }
}

