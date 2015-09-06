#-------------------------------------------------
#
# Project created by QtCreator 2015-05-01T19:05:50
#
#-------------------------------------------------

QT       += core gui widgets

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
    subjectitem.cpp \
    interioritem.cpp
#    makerlite.cpp

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
    subjectitem.h \
    interioritem.h
#    makerlite.h

FORMS    += mainwindow.ui \
    itemdialog.ui \
    questnamedialog.ui \
    makerlite.ui

CONFIG(release, debug|release) {
    #This is a release build
    DEFINES += QT_NO_DEBUG_OUTPUT
} else {
    #This is a debug build
}

win32 {

DEFINES = QJSON_MAKEDLL

SOURCES +=  3rdparty/qjson/src/parser.cpp \
            3rdparty/qjson/src/serializer.cpp \
            3rdparty/qjson/src/json_scanner.cpp \
3rdparty/qjson/src/json_parser.cc \
#3rdparty/qjson/src/json_scanner.cc \
#3rdparty/qjson/src/serializerrunnable.cpp \
#3rdparty/qjson/src/qobjecthelper.cpp \
#3rdparty/qjson/src/parserrunnable.cpp

HEADERS +=  3rdparty/qjson/src/parser.h \
            3rdparty/qjson/src/serializer.h \
            3rdparty/qjson/src/json_scanner.h  \
#3rdparty/qjson/src/pasrerrunnable.h \
#3rdparty/qjson/src/serializer.h \
#3rdparty/qjson/src/FlexLexer.h \
3rdparty/qjson/src/position.hh \
3rdparty/qjson/src/location.hh \
3rdparty/qjson/src/qjson_debug.h \
#3rdparty/qjson/src/serializerrunnable.h \
3rdparty/qjson/src/json_parser.hh \
3rdparty/qjson/src/qjson_export.h \
#3rdparty/qjson/src/stack.hh \
3rdparty/qjson/src/parser.h \
3rdparty/qjson/src/parser_p.h \
#3rdparty/qjson/src/qobjecthelper.h

INCLUDEPATH += 3rdparty
INCLUDEPATH += 3rdparty/qjson/src
} else {
    LIBS += -lqjson
}

OTHER_FILES += \
    quest.json \
    TODO.txt \
    list.qml

RESOURCES += \
    res.qrc
