#-------------------------------------------------
#
# Project created by QtCreator 2015-10-23T06:33:43
#
#-------------------------------------------------

QT       += core gui serialport

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = SPINEprog
TEMPLATE = app

SOURCES += main.cpp \
           spinewindow.cpp \
           codegen.cpp \
           spineosc.cpp \

HEADERS += spinewindow.h \
           codegen.h \
           hidapi.h \
           hidapi_darwin.h \
           oscpkt.hh \
           udp.hh \
           spineosc.h

FORMS   += spinewindow.ui

DISTFILES += modules.xml \
    enumerate_hid.py

ICON = gfx/spine.icns

RC_ICONS = gfx/spine.ico

RESOURCES += spineprog.qrc


macx {
    LIBS += -framework CoreFoundation \
            -framework Foundation
    CONFIG += objective_c
    SOURCES += macx/hid.c
}

win32|win64 {
    LIBS += -lWS2_32 -lsetupapi
    SOURCES += win32/hid.c
}
