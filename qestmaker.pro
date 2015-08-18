#-------------------------------------------------
#
# Project created by QtCreator 2015-05-01T19:05:50
#
#-------------------------------------------------

QT       += core gui declarative

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
    questitemnotifier.cpp \
    questnamedialog.cpp \
    visualizermodel.cpp \
    makerlite.cpp

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
    questitemnotifier.h \
    questnamedialog.h \
    visualizermodel.h \
    makerlite.h

FORMS    += mainwindow.ui \
    itemdialog.ui \
    questnamedialog.ui \
    makerlite.ui

LIBS += -lqjson

OTHER_FILES += \
    quest.json \
    TODO.txt \
    list.qml

RESOURCES += \
    res.qrc
