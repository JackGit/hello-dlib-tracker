QT += core
QT -= gui

CONFIG += c++11

TARGET = hello-dlib-tracker
CONFIG += console
CONFIG -= app_bundle

TEMPLATE = app

SOURCES += main.cpp

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


# opencv
INCLUDEPATH += D:\opencv\mingw-build\install\include
LIBS += -L"D:\opencv\mingw-build\install\x86\mingw\bin"
#LIBS += -lopencv_core2413 -lopencv_highgui2413 -lopencv_imgproc2413
LIBS += -lopencv_core2413 -lopencv_highgui2413 -lopencv_imgproc2413 -lopencv_features2d2413 -lopencv_objdetect2413

# dlib
# speed up dlib
QMAKE_CXXFLAGS_RELEASE += -mavx

INCLUDEPATH += D:\dlib-19.2\include
LIBS += -L"D:\dlib-19.2\lib"
LIBS += -ldlib


LIBS += -lcomctl32
LIBS += -lgdi32

LIBS += -lwinmm
LIBS += -limm32
