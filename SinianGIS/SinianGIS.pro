QT       += core gui
QT       += opengl

#设置图标
RC_ICONS = Res/main.ico

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11
CONFIG += console

#使得QT可以调试
QMAKE_CXXFLAGS_RELEASE = $$QMAKE_CFLAGS_RELEASE_WITH_DEBUGINFO
QMAKE_LFLAGS_RELEASE = $$QMAKE_LFLAGS_RELEASE_WITH_DEBUGINFO
#QMAKE_CXXFLAGS_RELEASE = -Od -ZI -MD
#QMAKE_LFLAGS_RELEASE = /DEBUG /INCREMENTAL:NO

# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    ExceptionDmp.cpp \
    ViewWidget.cpp \
    main.cpp \
    mainwindow.cpp \
    osgshowwidget.cpp \
    sceneproject3d.cpp \
    sceneprojectbase.cpp

HEADERS += \
    ExceptionDmp.h \
    ViewWidget \
    mainwindow.h \
    osgshowwidget.h \
    pathref.hpp \
    sceneproject3d.h \
    sceneprojectbase.h

FORMS += \
    mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

INCLUDEPATH += D:/Work/OSGNewBuild/OpenSceneGraph-3.6.4/include \
    D:/Work/OSGNewBuild/osgearth-2.10.1/src

LIBS += -LD:/Work/OSGNewBuild/OpenSceneGraph-3.6.4/build/lib -losg -lOpenThreads -losgDB -losgUtil -losgGA -losgViewer -losgQt5\
    -LD:/Work/OSGNewBuild/osgearth-2.10.1/build/lib/Release -losgEarth -losgEarthUtil

DESTDIR = $$PWD/../bin

DISTFILES +=

RESOURCES += \
    Res.qrc
