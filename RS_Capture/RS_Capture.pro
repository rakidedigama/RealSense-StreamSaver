QT += core
QT -= gui

CONFIG += c++11

TARGET = RS_Capture
CONFIG += console
CONFIG -= app_bundle

TEMPLATE = app

SOURCES += main.cpp
LIBS += "C:/BUILDS/IntelRealSense_SDK/Debug/realsense2.lib"

LIBS += -lOpenGL32

LIBS += glu32.lib

LIBS +=  "C:/BUILDS/glfw-3.2.1/src/Debug/glfw3dll.lib"

LIBS += C:/BUILDS/openCV/lib/Debug/opencv_core330d.lib
LIBS += C:/BUILDS/openCV/lib/Debug/opencv_highgui330d.lib
LIBS +=C:/BUILDS/openCV/lib/Debug/opencv_imgcodecs330d.lib
LIBS += C:/BUILDS/openCV/lib/Debug/opencv_imgproc330d.lib
LIBS += C:/BUILDS/openCV/lib/Debug/opencv_calib3d330d.lib
LIBS += C:/BUILDS/openCV/lib/Debug/opencv_video330d.lib
LIBS += C:/BUILDS/openCV/lib/Debug/opencv_videoio330d.lib
LIBS += C:/BUILDS/openCV/lib/Debug/opencv_videostab330d.lib





INCLUDEPATH += "C:/dev/Intel RealSense SDK 2/include/librealsense2/" \
                "C:/dev/Intel RealSense SDK 2/examples" \
                C:/dev/glfw-3.2.1/include \
                 C:\BUILDS\openCV\install\include \

HEADERS += "C:/dev/Intel RealSense SDK 2/examples/example.hpp" \
         =  "C:/dev/glfw-3.2.1/include/GLFW/glfw3.h"

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0