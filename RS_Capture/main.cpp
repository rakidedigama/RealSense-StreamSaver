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

//using namespace cv;
//using namespace std;

//Mat fgMaskMOG;
//Ptr<cv::BackgroundSubtractor> pMOG;

//const String  WINDOW_NAME_1("Camera video");
//const String  WINDOW_NAME_2("RGB video");

//const int frameRate = 5;
//QString fps = "fps_" + QString::number(frameRate) + "_";
//const QDateTime videoName = QDateTime::currentDateTime();
//  const QString timestamp = videoName.toString(QLatin1String("yyyyMMdd-hhmmss"));
//const QString fileNameG =  QString::fromLatin1("C:/GITROOT/RealSense/Builds/build-RS_Capture-Desktop_Qt_5_8_0_MSVC2015_64bit-Debug/clipG-%1.avi").arg(timestamp);
//const QString fileNameC =  QString::fromLatin1("C:/GITROOT/RealSense/Builds/build-RS_Capture-Desktop_Qt_5_8_0_MSVC2015_64bit-Debug/clipC-%1.avi").arg(timestamp);

//// Morphological operations

//int morph_elem = 2;
//int morph_size = 5;





//int distance_to_gray(float dist){
//    int gray_depth;
//    if(dist<=0){
//        gray_depth = 0;
//    }
//    if(dist>10){
//        gray_depth = 0;
//    }
//    else{
//        gray_depth = floor((10 - dist)/10 * 255);
//    }
//    return gray_depth;

//}

//cv::Mat invert_image(cv::Mat const& input)
//{
//    return 255 - input;
//}

int main(int argc, char * argv[])
{
    QApplication a(argc, argv);

    RealSenseStreamer r;
    r.runStream();


    return a.exec();

}

/*
int main(int argc, char * argv[]) try
{

    QThread* depthConverterThread = new QThread();
    DepthConverter* dConverter = new DepthConverter();
    dConverter->moveToThread(depthConverterThread);
    depthConverterThread->start();

    pMOG = createBackgroundSubtractorMOG2(200,16,false);

    Mat morphElement = getStructuringElement( morph_elem, Size( 2*morph_size + 1, 2*morph_size+1 ), Point( morph_size, morph_size ) );


    cv::namedWindow(WINDOW_NAME_1, cv::WINDOW_AUTOSIZE);
    cv::namedWindow(WINDOW_NAME_2, cv::WINDOW_AUTOSIZE);
    Size frameSizeG(static_cast<int>(640), static_cast<int>(480));
    Size frameSizeC(static_cast<int>(640), static_cast<int>(480));

     VideoWriter oVideoWriterG (fileNameG.toStdString(), CV_FOURCC('F','M','P','4'), 10, frameSizeG,false);
     VideoWriter oVideoWriterC (fileNameC.toStdString(), CV_FOURCC('F','M','P','4'), 10, frameSizeC,true);

    // Create a simple OpenGL window for rendering:
    window app(1280, 720, "RealSense Capture Example");
    texture depth_image, color_image;
    rs2::colorizer color_map;

    // Declare RealSense pipeline, encapsulating the actual device and sensors
    rs2::pipeline pipe;
     rs2::config config;
     config.enable_stream(RS2_STREAM_ANY, STREAM_INDEX, WIDTH, HEIGHT, FORMAT, FPS);
    // Start streaming with default recommended configuration
    pipe.start();

    while(app) // Application still alive?
    {
        if ( !oVideoWriterG.isOpened() && !oVideoWriterC.isOpened() )
        {
            cout << "ERROR: Failed to write the video" << endl;
         }

       rs2::frameset data = pipe.wait_for_frames(); // Wait for next set of frames from the camera
       rs2::depth_frame depth_frame = data.get_depth_frame();
       rs2::frame depth = data.get_depth_frame();
       rs2::frame depthC = color_map(data.get_depth_frame()); // Find and colorize the depth data
       rs2::frame color = data.get_color_frame();            // Find the color data

       int width = depth_frame.get_width();
       int height = depth_frame.get_height();

       cout<<"Depth Frame Width : " << width << "Height : " << height << endl;


       Mat openDepth((Size(width, height)), CV_16UC1, (void*)depth.get_data(), Mat::AUTO_STEP);
       //Mat oDepthOut = dConverter->processDepthFrame(openDepth);
/*
       Mat openDepthResized = Mat(480,640,CV_16UC1);
       Mat openDepthResized8 = Mat(480,640,CV_8UC1);
       cv::resize(openDepth,openDepthResized,Size(640,480));
       imwrite("depthO.jpg",openDepthResized);
       openDepthResized.convertTo(openDepthResized8,CV_8UC1,0.00390625);
       openDepthResized8 = 255-openDepthResized8;
       imwrite("depthO8.jpg",openDepthResized8);
       Mat oDepthHistEqualized;
       equalizeHist(openDepthResized8,oDepthHistEqualized);
       imwrite("depthO8H.jpg",oDepthHistEqualized);


       // Remove 255 values in foreground by thresholding
       Mat threshMask;
       double maxGrayLevel = 254;
       threshold(oDepthHistEqualized,threshMask,maxGrayLevel,255,THRESH_BINARY);
       oDepthHistEqualized.setTo(0,threshMask);

       // Morphology to remove noise
       Mat oDepthOut;
       morphologyEx(oDepthHistEqualized, oDepthOut, 2, morphElement ); // opening
        morphologyEx(oDepthOut, oDepthOut, 3, morphElement ); // closing

*/

//     Mat grayDepthFrameResized(480,640,CV_8UC3);
//     grayDepthFrameResized = dConverter->processDepthByDistance(depth_frame);
//      pMOG->apply(oDepthOut,fgMaskMOG);
/*
       Mat grayDepthFrame = Mat(1280,720,CV_8U);

        float sum = 0;
        for(int i=0;i<1280;i++){
            for(int j=0;j<720;j++){
                float dist = depth_frame.get_distance(1280-i,j);
                int intensity = distance_to_gray(dist);
                //cout<< i << " " << j << endl;
                sum = sum + intensity;
                //cout<<"distance:" << dist << "intensity: " <<distance_to_gray(dist)<<endl;
                grayDepthFrame.at<uchar>(i,j) =distance_to_gray(dist);

            }
        }

        std::cout<<" Depth Intensity Mapping done  :" <<endl;

        // Depth Rotation and Channels

        Mat grayDepthFrameR = Mat(720,1280,CV_8U);
        Mat grayDepthFrameR_3 = Mat(720,1280,CV_8UC3);
        Mat grayDepthFrameResized = Mat(480,640,CV_8UC3);
        cv::rotate(grayDepthFrame,grayDepthFrameR, ROTATE_90_CLOCKWISE);
//        cv::cvtColor(grayDepthFrameR,grayDepthFrameR_3,CV_GRAY2RGB);
        cv::resize(grayDepthFrameR,grayDepthFrameResized,Size(640,480));

           //imwrite("depthGR.jpg",grayDepthFrameResized);
//            imwrite("depthGR3.jpg",grayDepthFrameR_3);
              // oVideoWriterG<<grayDepthFrameResized;

        */

        // Color

/*
        Mat openColor((Size(640, 480)), CV_8UC3, (void*)color.get_data(), Mat::AUTO_STEP);
        Mat openColorCvt(480,640,CV_8UC3);
        cv::cvtColor(openColor,openColorCvt,COLOR_BGR2RGB);

        // Display

        cv::imshow(WINDOW_NAME_1,grayDepthFrameResized);
        cv::imshow(WINDOW_NAME_2,openColorCvt);
        cv::waitKey(50);

        // Save Video

        //oVideoWriterG<<oDepthOut;
        oVideoWriterC<<openColorCvt;

    }

   pipe.stop();
   oVideoWriterC.release();


    return EXIT_SUCCESS;
} */
//catch (const rs2::error & e)
//{
//    std::cerr << "RealSense error calling " << e.get_failed_function() << "(" << e.get_failed_args() << "):\n    " << e.what() << std::endl;
//    return EXIT_FAILURE;
//}
//catch (const std::exception& e)
//{
//    std::cerr << e.what() << std::endl;
//    return EXIT_FAILURE;
//}
