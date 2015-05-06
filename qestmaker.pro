#-------------------------------------------------
#
# Project created by QtCreator 2015-05-01T19:05:50
#
#-------------------------------------------------

QT       += core gui

TARGET = qestmaker
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    questscene.cpp \
    sceneitem.cpp \
    questitem.cpp \
    episodeitem.cpp \
    actitem.cpp \
    itemdialog.cpp \
    itemitem.cpp \
    itemcreator.cpp \
    questtree.cpp \
    itembackground.cpp \
    scenevisualizer.cpp \
    propertywidget.cpp \
    questitemnotifier.cpp \
    questnamedialog.cpp

HEADERS  += mainwindow.h \
    questscene.h \
    sceneitem.h \
    questitem.h \
    episodeitem.h \
    actitem.h \
    itemdialog.h \
    itemitem.h \
    itemcreator.h \
    questtree.h \
    itembackground.h \
    scenevisualizer.h \
    propertywidget.h \
    questitemnotifier.h \
    questnamedialog.h

FORMS    += mainwindow.ui \
    itemdialog.ui \
    questnamedialog.ui

LIBS += -lqjson

OTHER_FILES += \
    quest.json
