QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11
#QMAKE_CXXFLAGS += -O3

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    color.cpp \
    column.cpp \
    cube.cpp \
    face.cpp \
    main.cpp \
    mainwindow.cpp \
    matrix.cpp \
    object.cpp \
    objectinfo.cpp \
    plane.cpp \
    point2d.cpp \
    point3d.cpp \
    scene.cpp \
    settings.cpp \
    slab.cpp \
    wall.cpp

HEADERS += \
    color.h \
    column.h \
    cube.h \
    face.h \
    mainwindow.h \
    matrix.h \
    object.h \
    objectinfo.h \
    plane.h \
    point2d.h \
    point3d.h \
    scene.h \
    settings.h \
    slab.h \
    wall.h

FORMS += \
    mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
