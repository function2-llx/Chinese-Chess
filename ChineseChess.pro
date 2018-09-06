#-------------------------------------------------
#
# Project created by QtCreator 2018-09-04T11:53:39
#
#-------------------------------------------------

QT       += core gui
QT += network
QT += multimedia multimediawidgets

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = ChineseChess
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


SOURCES += \
        main.cpp \
        mainwindow.cpp \
    newdialog.cpp \
    waitdialog.cpp \
    joindialog.cpp \
    chessboard.cpp \
    chesspiece.cpp

HEADERS += \
        mainwindow.h \
    newdialog.h \
    waitdialog.h \
    joindialog.h \
    chessboard.h \
    chesspiece.h

FORMS += \
        mainwindow.ui \
    newdialog.ui \
    waitdialog.ui \
    joindialog.ui

RESOURCES += \
    image.qrc
