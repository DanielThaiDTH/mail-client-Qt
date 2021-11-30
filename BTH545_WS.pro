QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    DateTime.cpp \
    Email.cpp \
    MailParser.cpp \
    collapsablesection.cpp \
    errorwindow.cpp \
    filterdialog.cpp \
    forwarddialog.cpp \
    inbox.cpp \
    inboxdisplay.cpp \
    inboxitem.cpp \
    mailcontent.cpp \
    mailframe.cpp \
    main.cpp \
    mainwindow.cpp \
    searchfilter.cpp \
    senddialog.cpp

HEADERS += \
    DateTime.h \
    Email.h \
    MailParser.h \
    collapsablesection.h \
    errorwindow.h \
    filterdialog.h \
    forwarddialog.h \
    inbox.h \
    inboxdisplay.h \
    inboxitem.h \
    mailcontent.h \
    mailframe.h \
    mainwindow.h \
    searchfilter.h \
    senddialog.h

FORMS += \
    errorwindow.ui \
    filterdialog.ui \
    forwarddialog.ui \
    mainwindow.ui \
    senddialog.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    resources.qrc
