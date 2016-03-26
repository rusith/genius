#-------------------------------------------------
#
# Project created by QtCreator 2016-03-16T21:13:05
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Genius
TEMPLATE = app


SOURCES += main.cpp \
    views/manager.cpp \
    controllers/controller.cpp \
    models/clipboarditem.cpp \
    models/clipboardhistory.cpp \
    views/trayicon.cpp \
    tools/toolkit.cpp \
    resources/resources.cpp \
    views/texteditor.cpp \
    views/imageeditor.cpp \
    controllers/gsettings.cpp \
    views/selector.cpp \
    controllers/fakekey.cpp \
    views/settingswindow.cpp \
    controllers/tempfolder.cpp \
    models/clipboardimageitem.cpp \
    models/clipboardtextitem.cpp

HEADERS  += \
    views/manager.h \
    controllers/controller.h \
    models/clipboarditem.h \
    models/clipboardhistory.h \
    views/trayicon.h \
    tools/toolkit.h \
    resources/resources.h \
    views/texteditor.h \
    views/imageeditor.h \
    controllers/gsettings.h \
    views/selector.h \
    controllers/fakekey.h \
    views/settingswindow.h \
    controllers/tempfolder.h \
    models/clipboardimageitem.h \
    models/clipboardtextitem.h

FORMS    += \
    views/manager.ui \
    views/texteditor.ui \
    views/imageeditor.ui \
    views/selector.ui \
    views/settingswindow.ui

DISTFILES +=

RESOURCES += \
    resources/resources.qrc

include(./plugins/QHotkey/qhotkey.pri)
