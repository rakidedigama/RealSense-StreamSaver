// License: Apache 2.0. See LICENSE file in root directory.
// Copyright(c) 2017 Intel Corporation. All Rights Reserved.
#include <rs.hpp>

#include "realsensestreamer.h"
#include "QApplication"


//#define STREAM          RS2_STREAM_DEPTH  // rs2_stream is a types of data provided by RealSense device           //
//#define FORMAT          RS2_FORMAT_Z16    // rs2_format is identifies how binary data is encoded within a frame   //
//#define WIDTH           640               // Defines the number of columns for each frame                         //
//#define HEIGHT          480               // Defines the number of lines for each frame                           //
//#define FPS             30                // Defines the rate of frames per second                                //
//#define STREAM_INDEX    0                 // Defines the stream index, used for multiple streams of the same type //
//#define HEIGHT_RATIO    20                // Defines the height ratio between the original frame to the new frame //
//#define WIDTH_RATIO     10                // Defines the width ratio between the original frame to the new frame  //
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////





int main(int argc, char * argv[])
{
    QApplication a(argc, argv);

    RealSenseStreamer r;
    r.runStream();


    return a.exec();

}

