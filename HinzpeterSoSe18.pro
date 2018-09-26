TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
QT += widgets
QT += gui
QT += core

SOURCES += main.cpp \
    widget.cpp \
    spielfeld.cpp \
    objekte.cpp

HEADERS += \
    widget.h \
    spielfeld.h \
    objekte.h

DISTFILES +=
