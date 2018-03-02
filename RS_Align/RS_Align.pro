QT += core
QT -= gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets
CONFIG += c++11

TARGET = RS_Align
CONFIG += console
CONFIG -= app_bundle

TEMPLATE = app



SOURCES += main.cpp \
            "C:/dev/IntelRealSenseSDK2/third-party/imgui/imgui_impl_glfw.cpp" \
            "C:/dev/IntelRealSenseSDK2/third-party/imgui/imgui.cpp" \
                   "C:/dev/IntelRealSenseSDK2/third-party/imgui/imgui_draw.cpp" \
            ../RS_Capture/depthconverter.cpp \


LIBS += "C:/BUILDS/IntelRealSense_SDK/Debug/realsense2.lib"

LIBS += -lOpenGL32

LIBS += glu32.lib

LIBS +=  "C:/BUILDS/glfw-3.2.1/src/Debug/glfw3dll.lib"

debug {
LIBS += C:/BUILDS/openCV/lib/Debug/opencv_core330d.lib
LIBS += C:/BUILDS/openCV/lib/Debug/opencv_highgui330d.lib
LIBS +=C:/BUILDS/openCV/lib/Debug/opencv_imgcodecs330d.lib
LIBS += C:/BUILDS/openCV/lib/Debug/opencv_imgproc330d.lib
LIBS += C:/BUILDS/openCV/lib/Debug/opencv_calib3d330d.lib
LIBS += C:/BUILDS/openCV/lib/Debug/opencv_video330d.lib
LIBS += C:/BUILDS/openCV/lib/Debug/opencv_videoio330d.lib
LIBS += C:/BUILDS/openCV/lib/Debug/opencv_videostab330d.lib
}


# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.


# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

INCLUDEPATH += "C:/dev/IntelRealSenseSDK2/include/librealsense2/" \
                "C:/dev/IntelRealSenseSDK2/examples" \
                C:/dev/glfw-3.2.1/include \
                "C:/dev/IntelRealSenseSDK2/third-party/imgui" \
                       C:\BUILDS\openCV\install\include \
                ../RS_Capture

HEADERS += "C:/dev/IntelRealSenseSDK2examples/example.hpp" \
             "C:/dev/glfw-3.2.1/include/GLFW/glfw3.h" \
            "C:/dev/IntelRealSenseSDK2/include/librealsense2/rs.hpp" \
             "C:/dev/IntelRealSenseSDK2/third-party/imgui/imgui_impl_glfw.h" \
            "C:/dev/IntelRealSenseSDK2/third-party/imgui/imgui.h" \
             "C:/dev/IntelRealSenseSDK2/third-party/imgui/imgui_draw.h" \
                 ../RS_Capture/depthconverter.h \



#DEFINES += QT_DEPRECATED_WARNINGS
