l#-------------------------------------------------
#
# Project created by QtCreator 2019-08-10T15:51:38
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = distort-desktop-client
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

CONFIG += c++11

SOURCES += \
        about.cpp \
        atomicstate.cpp \
        authparams.cpp \
        backgroundworker.cpp \
        conversation.cpp \
        conversationwidget.cpp \
        distortexception.cpp \
        group.cpp \
        groupwidget.cpp \
        inmessage.cpp \
        json.yasl/json.c \
        main.cpp \
        mainwindow.cpp \
        message.cpp \
        messagedelegate.cpp \
        messagelistmodel.cpp \
        messagewidget.cpp \
        outmessage.cpp \
        peer.cpp \
        restclient.cpp \
        signindialog.cpp \
        signinworker.cpp

HEADERS += \
        about.h \
        atomicstate.h \
        authparams.h \
        backgroundworker.h \
        conversation.h \
        conversationwidget.h \
        distortexception.h \
        group.h \
        groupwidget.h \
        inmessage.h \
        json.yasl/json.h \
        mainwindow.h \
        message.h \
        messagedelegate.h \
        messagelistmodel.h \
        messagewidget.h \
        outmessage.h \
        peer.h \
        restclient.h \
        signindialog.h \
        signinworker.h

FORMS += \
        about.ui \
        conversationwidget.ui \
        groupitem.ui \
        mainwindow.ui \
        messagewidget.ui \
        signindialog.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

unix|win32: LIBS += -lcurlpp -lcurl

unix|win32: LIBS += -lcryptopp

DISTFILES += \
    res/alert_bell.wav \
    res/contact.png \
    res/conv_single.png \
    res/icon_512.png \
    scripts/background.yasl \
    scripts/drawmessage.yasl \
    scripts/geterror.yasl \
    scripts/getmessage.yasl \
    scripts/init.yasl \
    scripts/restclient.yasl \
    scripts/switchconv.yasl

## Include YASL
INCLUDEPATH += $$PWD/yasl
DEPENDPATH += $$PWD/yasl

yaslTarget.target = $$shell_path($$PWD/yasl/libyaslapi.a)
yaslTarget.depends = FORCE
win32: yaslTarget.commands = cd $$PWD/yasl ; cmake --configure . ; cmake --build . ; cd ..
else: yaslTarget.commands = cd $$PWD/yasl ; cmake --configure . ; cmake --build . ; cd ..

PRE_TARGETDEPS += $$shell_path($$PWD/yasl/libyaslapi.a)
QMAKE_EXTRA_TARGETS += yaslTarget

unix|win32: LIBS += -L$$PWD/yasl/ -lyaslapi


## Copy YASL scripts
# https://stackoverflow.com/a/39234363
copydata.commands = $(COPY_DIR) \"$$shell_path($$PWD/scripts)\" \"$$shell_path($$OUT_PWD)\"
first.depends = $(first) copydata
export(first.depends)
export(copydata.commands)
QMAKE_EXTRA_TARGETS += first copydata

RESOURCES += \
    resources.qrc
