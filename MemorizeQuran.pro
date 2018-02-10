#-------------------------------------------------
#
# Project created by QtCreator 2018-01-31T10:58:23
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets multimedia network

TARGET = MemorizeQuran
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    mylabel.cpp \
    surainfo.cpp \
    quraninfo.cpp \
    qurantextinfo.cpp \
    audioplayer.cpp \
    downloadmanager.cpp \
    cachingplayer.cpp \
    recitationinfo.cpp

HEADERS  += mainwindow.h \
    mylabel.h \
    surainfo.h \
    quraninfo.h \
    qurantextinfo.h \
    audioplayer.h \
    downloadmanager.h \
    cachingplayer.h \
    recitationinfo.h

FORMS    +=

RESOURCES += \
    fonts.qrc \
    quran.qrc \
    icons.qrc

RC_FILE = app.rc
