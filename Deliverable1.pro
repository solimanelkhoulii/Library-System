QT       += core gui sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    adminlogin.cpp \
    browsecatalogue.cpp \
    librarianlogin.cpp \
    login.cpp \
    main.cpp \
    mainwindow.cpp \
    pagehome.cpp \
    viewaccount.cpp \
    user.cpp \
    datastore.cpp \
    catalogueitem.cpp \
    patron.cpp \
    librarianpatronmanager.cpp \
    librariancataloguemanager.cpp \
    databasemanager.cpp

HEADERS += \
    adminlogin.h \
    browsecatalogue.h \
    librarianlogin.h \
    login.h \
    mainwindow.h \
    pagehome.h \
    viewaccount.h \
    user.h \
    datastore.h \
    catalogueitem.h \
    patron.h \
    librarianpatronmanager.h \
    librariancataloguemanager.h \
    databasemanager.h

FORMS += \
    adminlogin.ui \
    browsecatalogue.ui \
    librarianlogin.ui \
    login.ui \
    mainwindow.ui \
    pagehome.ui \
    viewaccount.ui \
    librarianpatronmanager.ui \
    librariancataloguemanager.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
