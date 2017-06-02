#-------------------------------------------------
#
# Project created by QtCreator 2017-03-27T15:43:42
#
#-------------------------------------------------

QT       += core gui printsupport serialport quickwidgets webenginewidgets

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Air-E205_Baloon_Project_GUI
TEMPLATE = app


SOURCES += main.cpp\
        utama.cpp \
    serialthread.cpp \
    vincentythread.cpp \
    savethread.cpp \
    haversinethread.cpp

HEADERS  += utama.h \
    serialthread.h \
    vincentythread.h \
    savethread.h \
    haversinethread.h

FORMS    += utama.ui

RESOURCES += \
    konten.qrc

unix:!macx: LIBS += -L$$PWD/../../../../usr/lib/ -lqwt-qt5

INCLUDEPATH += $$PWD/../../../../usr/include/qwt
DEPENDPATH += $$PWD/../../../../usr/include/qwt

DISTFILES +=
