#-------------------------------------------------
#
# Project created by QtCreator 2020-04-26T00:03:31
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = CPUPowerGUI
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

CONFIG += c++11

SOURCES += \
        src/CPUCore.cpp \
        src/CPUInfo.cpp \
        src/CPUPolicy.cpp \
        src/DisplayCorePage.cpp \
        src/MainPage.cpp \
        src/MainWindow.cpp \
        src/PolicyDisplayWidget.cpp \
        src/PolicyEditorWidget.cpp \
        src/SwitchButton.cpp \
        src/imageswitch.cpp \
        src/main.cpp

HEADERS += \
        src/CPUCore.h \
        src/CPUInfo.h \
        src/CPUPolicy.h \
        src/DisplayCorePage.h \
        src/MainPage.h \
        src/MainWindow.h \
        src/PolicyDisplayWidget.h \
        src/PolicyEditorWidget.h \
        src/SwitchButton.h \
        src/UpdateInfoInterface.h \
        src/imageswitch.h

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    resource.qrc
