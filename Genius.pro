#-------------------------------------------------
#
# Project created by QtCreator 2016-03-16T21:13:05
#
#-------------------------------------------------

QT       += core gui
CONFIG += c++11
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Genius
TEMPLATE = app

win32 :RC_ICONS =genius.ico


SOURCES += main.cpp \
    views/manager.cpp \
    controllers/controller.cpp \
    models/clipboardhistory.cpp \
    views/trayicon.cpp \
    tools/toolkit.cpp \
    resources/resources.cpp \
    controllers/gsettings.cpp \
    views/selector.cpp \
    controllers/fakekey.cpp \
    views/settingswindow.cpp \
    controllers/tempfolder.cpp \
    models/filing/datafile.cpp \
    models/clipboardentity.cpp \
    views/contentviewer.cpp \
    tools/rtimer.cpp \
    models/filing/fragmentframe.cpp \
    views/about.cpp \
    views/license.cpp \
    GUI/menuitem.cpp \
    controllers/runguard.cpp \
    GUI/menu.cpp \
    views/help.cpp

HEADERS  += \
    views/manager.h \
    controllers/controller.h \
    models/clipboardhistory.h \
    views/trayicon.h \
    tools/toolkit.h \
    resources/resources.h \
    controllers/gsettings.h \
    views/selector.h \
    controllers/fakekey.h \
    views/settingswindow.h \
    controllers/tempfolder.h \
    models/filing/datafile.h \
    models/clipboardentity.h \
    views/contentviewer.h \
    tools/rtimer.h \
    models/filing/fragmentframe.h \
    views/about.h \
    views/license.h \
    GUI/menuitem.h \
    controllers/runguard.h \
    GUI/menu.h \
    views/help.h

FORMS    += \
    views/manager.ui \
    views/selector.ui \
    views/settingswindow.ui \
    views/contentviewer.ui \
    views/about.ui \
    views/license.ui \
    views/help.ui

DISTFILES +=

RESOURCES += \
    resources/resources.qrc

include(./plugins/QHotkey/qhotkey.pri)
