#-------------------------------------------------
#
# Project created by QtCreator 2015-10-23T06:33:43
#
#-------------------------------------------------

QT       += core gui
QT       += serialport

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = SPINEprog
TEMPLATE = app

SOURCES += main.cpp\
        spinewindow.cpp \
    codegen.cpp

HEADERS  += spinewindow.h \
    codegen.h

FORMS    += spinewindow.ui

DISTFILES += modules.xml

ICON = gfx/spine.icns
RC_ICONS = gfx/spine.ico

RESOURCES += \
    spineprog.qrc
