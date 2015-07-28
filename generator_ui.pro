#-------------------------------------------------
#
# Project created by QtCreator 2015-07-17T13:25:18
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets serialport

TARGET = generator_ui
TEMPLATE = app


SOURCES += main.cpp\
        generator.cpp \
    protocol.cpp

HEADERS  += generator.h \
    protocol.h

FORMS    += generator.ui
