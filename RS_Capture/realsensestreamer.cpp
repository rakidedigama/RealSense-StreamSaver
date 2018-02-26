#include <rs.hpp>
#include "example.hpp"
#include "realsensestreamer.h"
#include "depthconverter.h"
#include "QThread"
#include "QDateTime"


#define STREAM          RS2_STREAM_DEPTH  // rs2_stream is a types of data provided by RealSense device           //
#define FORMAT          RS2_FORMAT_Z16    // rs2_format is identifies how binary data is encoded within a frame   //
#define WIDTH           640               // Defines the number of columns for each frame                         //
#define HEIGHT          480               // Defines the number of lines for each frame                           //
#define FPS             30                // Defines the rate of frames per second                                //
#define STREAM_INDEX    0                 // Defines the stream index, used for multiple streams of the same type //
#define HEIGHT_RATIO    20                // Defines the height ratio between the original frame to the new frame //
#define WIDTH_RATIO     10                // Defines the width ratio between the original frame to the new frame  //
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// \brief RealSenseStreamer::RealSenseStreamer
/// \param parent
///

const String  WINDOW_NAME_1("Camera video");
const String  WINDOW_NAME_2("RGB video");
    Size frameSizeG(static_cast<int>(640), static_cast<int>(480));
    Size frameSizeC(static_cast<int>(640), static_cast<int>(480));

const int frameRate = 5;
QString fps = "fps_" + QString::number(frameRate) + "_";
const QDateTime videoName = QDateTime::currentDateTime();
  const QString timestamp = videoName.toString(QLatin1String("yyyyMMdd-hhmmss"));
const QString fileNameG =  QString::fromLatin1("C:/GITROOT/RealSense/Builds/build-RS_Capture-Desktop_Qt_5_8_0_MSVC2015_64bit-Debug/clipG-%1.avi").arg(timestamp);
const QString fileNameC =  QString::fromLatin1("C:/GITROOT/RealSense/Builds/build-RS_Capture-Desktop_Qt_5_8_0_MSVC2015_64bit-Debug/clipC-%1.avi").arg(timestamp);


RealSenseStreamer::RealSenseStreamer(QObject *parent):QObject(parent)
{
    depthConverterThread = new QThread(this);
    dConverter = new DepthConverter();
    dConverter->moveToThread(depthConverterThread);
    depthConverterThread->start();

    //pMOG = createBackgroundSubtractorMOG2(200,16,false);

    m_bProcessByDistance = true ; // select

  processed_depth_frame = 0;
  m_bDepthFrameUpdated = false;
   m_bDistFrameUpdated = false;


    qRegisterMetaType< cv::Mat >("cv::Mat");
    qRegisterMetaType< rs2::frame >("rs2::depth_frame");

    QObject::connect(this,SIGNAL(gotDepthFrame(cv::Mat)),dConverter,SLOT(processDepthFrame(cv::Mat)));
    QObject::connect(dConverter,SIGNAL(depthFrameProcessed(cv::Mat)),this,SLOT(updateDepthFrame(cv::Mat)));
    QObject::connect(this,SIGNAL(destroyed(QObject*)),depthConverterThread,SLOT(quit()));

    // by distance
    QObject::connect(this,SIGNAL(convertDepthFrameToDistance(rs2::depth_frame)),dConverter,SLOT(processDepthByDistance(rs2::depth_frame)));
    QObject::connect(dConverter,SIGNAL(depthFrameProcessedByDistance(cv::Mat)),this,SLOT(updateDistanceFrame(cv::Mat)));
}

int RealSenseStreamer::runStream() {

    window app(1280, 720, "RealSense Capture Example");
    texture depth_image, color_image;
    rs2::colorizer color_map;

    rs2::pipeline pipe;
     rs2::config config;
     config.enable_stream(RS2_STREAM_ANY, STREAM_INDEX, WIDTH, HEIGHT, FORMAT, FPS);
    pipe.start();

        cv::namedWindow(WINDOW_NAME_1, cv::WINDOW_AUTOSIZE);
        cv::namedWindow(WINDOW_NAME_2, cv::WINDOW_AUTOSIZE);

        cv::VideoWriter oVideoWriterG(fileNameG.toStdString(), CV_FOURCC('F','M','P','4'), 10, frameSizeG,false);
        cv::VideoWriter oVideoWriterC(fileNameC.toStdString(), CV_FOURCC('F','M','P','4'), 10, frameSizeC,true);

     std::cout<<"Start streaming" << std::endl;
    while(app) // Application still alive?
    {
        if ( !oVideoWriterG.isOpened() && !oVideoWriterC.isOpened() )
        {
            std::cout << "ERROR: Failed to write the video" << std::endl;
         }

       rs2::frameset data = pipe.wait_for_frames(); // Wait for next set of frames from the camera
       rs2::depth_frame depth_frame = data.get_depth_frame();
       rs2::frame depth = data.get_depth_frame();
       rs2::frame depthC = color_map(data.get_depth_frame()); // Find and colorize the depth data
       rs2::frame color = data.get_color_frame();            // Find the color data

       int width = depth_frame.get_width();
       int height = depth_frame.get_height();

       std::cout<<"Depth Frame Width : " << width << "Height : " << height << std::endl;



       //Mat grayDepthFrameResized(480,640,CV_8UC3);
       if(m_bProcessByDistance){
            emit convertDepthFrameToDistance(depth_frame);
           std::cout<<"Emit frame : Get distance Frame" << std::endl;
           if(m_bDistFrameUpdated){
                 m_bDistFrameUpdated = false; // reset frame update flag
                std::cout<<"Got processed depth frame in streamer" << std::endl;
                 cv::imshow(WINDOW_NAME_1,processed_dist_frame);
           }
           else{
                 //std::cout<<"Waiting for processed depth frame" << std::endl;
            }


       }
       else{
           Mat openDepth((Size(width, height)), CV_16UC1, (void*)depth.get_data(), Mat::AUTO_STEP);
           std::cout<<"Emit frame" << std::endl;
           emit gotDepthFrame(openDepth);
//           while(!m_bDepthFrameUpdated ){
               if(m_bDepthFrameUpdated){
                     m_bDepthFrameUpdated = false; // reset frame update flag
                    std::cout<<"Got processed depth frame in streamer" << std::endl;
                    cv::imshow(WINDOW_NAME_1,processed_depth_frame);
               }
               else{
                     //std::cout<<"Waiting for processed depth frame" << std::endl;
                }
//           }


           //Mat oDepthOut = dConverter->processDepthFrame(openDepth);

       }

       Mat openColor((Size(640, 480)), CV_8UC3, (void*)color.get_data(), Mat::AUTO_STEP);
       Mat openColorCvt(480,640,CV_8UC3);
       cv::cvtColor(openColor,openColorCvt,COLOR_BGR2RGB);


       cv::imshow(WINDOW_NAME_2,openColorCvt);

       cv::waitKey(50);
    }
    pipe.stop();
    oVideoWriterC.release();
     return EXIT_SUCCESS;

}

void RealSenseStreamer::updateDepthFrame(cv::Mat depthFrame){
    processed_depth_frame = depthFrame;
    m_bDepthFrameUpdated = true;
    std::cout<<"got Processed depth frame in SLOT" << std::endl;
}

void RealSenseStreamer::updateDistanceFrame(cv::Mat newDistFrame){
    processed_dist_frame = newDistFrame;
    m_bDistFrameUpdated = true;
    std::cout<<"got Processed DISTANCE frame in SLOT" << std::endl;
}
