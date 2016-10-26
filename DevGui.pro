## Qt project file
QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = DevGui
TEMPLATE = app

# library linking (lib{name}.a -> -l{name} -L{path})
LIBS += -L$$PWD/ -ldevControl
INCLUDEPATH += $$PWD/
## compiler flags
QMAKE_CXXFLAGS += -Wno-deprecated

SOURCES += main.cpp \
        mainwindow.cpp \
        mythread.cpp

HEADERS  += mainwindow.h \
        led-test.h \
        mythread.h \


FORMS    += mainwindow.ui


