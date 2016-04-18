#-------------------------------------------------
#
# Project created by QtCreator 2016-04-18T15:54:32
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = JuringvayVLC
TEMPLATE = app

INCLUDEPATH += $$PWD/vlc/include

SOURCES += main.cpp\
        widget.cpp \
    qvlc.cpp

HEADERS  += widget.h \
    qvlc.h

FORMS    += widget.ui

win32 {
LIBS += -L$$PWD/vlc/lib/ -llibvlc
LIBS += -L$$PWD/vlc/lib/ -llibvlccore
}

unix {
LIBS += $$PWD/vlc/lib/libvlc.so
}

MOC_DIR       = temp/moc
RCC_DIR       = temp/rcc
UI_DIR        = temp/ui
OBJECTS_DIR   = temp/obj
DESTDIR       = bin
