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
const QString fileNameG =  QString::fromLatin1("C:/GITROOT/RealSense/Clips/clipG-%1.avi").arg(timestamp);
const QString fileNameC =  QString::fromLatin1("C:/GITROOT/RealSense/Clips/clipC-%1.avi").arg(timestamp);


RealSenseStreamer::RealSenseStreamer(QObject *parent):QObject(parent)
{
    depthConverterThread = new QThread(this);
    dConverter = new DepthConverter();
    dConverter->moveToThread(depthConverterThread);
    depthConverterThread->start();

    pMOG = createBackgroundSubtractorMOG2(200,16,false);

    m_bProcessByDistance = false ; // select

  processed_depth_frame = 0;
  m_bDepthFrameUpdated = false;
   m_bDistFrameUpdated = false;
    m_BGsubtraction = false;


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
      rs2::pipeline_profile profile = pipe.start();


      // Each depth camera might have different units for depth pixels, so we get it here
      // Using the pipeline's profile, we can retrieve the device that the pipeline uses
      float depth_scale = get_depth_scale(profile.get_device());

      //Pipeline could choose a device that does not have a color stream
      //If there is no color stream, choose to align depth to another stream
      rs2_stream align_to = find_stream_to_align(profile.get_streams());

      // Create a rs2::align object.
      // rs2::align allows us to perform alignment of depth frames to others frames
      //The "align_to" is the stream type to which we plan to align depth frames.
      rs2::align align(align_to);


//     rs2::config config;
//     config.enable_stream(RS2_STREAM_ANY, STREAM_INDEX, WIDTH, HEIGHT, FORMAT, FPS);
    //pipe.start();

        cv::namedWindow(WINDOW_NAME_1, cv::WINDOW_AUTOSIZE);
        cv::namedWindow(WINDOW_NAME_2, cv::WINDOW_AUTOSIZE);

        cv::VideoWriter oVideoWriterG(fileNameG.toStdString(), CV_FOURCC('F','M','P','4'), 10, frameSizeG,false);
        cv::VideoWriter oVideoWriterC(fileNameC.toStdString(), CV_FOURCC('F','M','P','4'), 10, frameSizeC,true);

     std::cout<<"Start streaming" << std::endl;
     std::string input;
     std::cout <<"Enter b for BG subtraction, or o for not" << std::endl;
     std::cin>>input ;
     if(input=="b"){
         std::cout << "Do background subtraction" <<std::endl;
         m_BGsubtraction = true;
     }
     if(input=="o"){
         m_BGsubtraction = false;
     }
     else{
         exit(0);
     }

    while(app) // Application still alive?
    {
        if ( !oVideoWriterG.isOpened() && !oVideoWriterC.isOpened() )
        {
            std::cout << "ERROR: Failed to write the video" << std::endl;
        }


        // Alignement
        // Using the align object, we block the application until a frameset is available
        rs2::frameset frameset = pipe.wait_for_frames();

        // rs2::pipeline::wait_for_frames() can replace the device it uses in case of device error or disconnection.
        // Since rs2::align is aligning depth to some other stream, we need to make sure that the stream was not changed
        //  after the call to wait_for_frames();
        if (profile_changed(pipe.get_active_profile().get_streams(), profile.get_streams()))
        {
            //If the profile was changed, update the align object, and also get the new device's depth scale
            profile = pipe.get_active_profile();
            align_to = find_stream_to_align(profile.get_streams());
            align = rs2::align(align_to);
            depth_scale = get_depth_scale(profile.get_device());
        }

        //Get processed aligned frame
        auto processed = align.process(frameset);

        // Trying to get both other and aligned depth frames
        rs2::video_frame other_frame = processed.first(align_to);
        rs2::depth_frame aligned_depth_frame = processed.get_depth_frame();

        int width = aligned_depth_frame.get_width();
        int height = aligned_depth_frame.get_height();
        /*

       rs2::frameset data = pipe.wait_for_frames(); // Wait for next set of frames from the camera
       rs2::depth_frame depth_frame = data.get_depth_frame();
       rs2::frame depth = data.get_depth_frame();
       rs2::frame depthC = color_map(data.get_depth_frame()); // Find and colorize the depth data
       rs2::frame color = data.get_color_frame();            // Find the color data

       int width = depth_frame.get_width();
       int height = depth_frame.get_height();

      // std::cout<<"Depth Frame Width : " << width << "Height : " << height << std::endl;


      */


       QTime timer;



       // Depth Stream

       if(m_bProcessByDistance){
            emit convertDepthFrameToDistance(aligned_depth_frame);
            timer.start();
           std::cout<<"Emit frame : Get distance Frame" << std::endl;
           if(m_bDistFrameUpdated){
                std::cout<<"Got processed depth frame in streamer : " << timer.elapsed() << " ms"  << std::endl;
                 cv::imshow(WINDOW_NAME_1,processed_dist_frame);
                 m_bDistFrameUpdated = false; // reset frame update flag
           }
           else{
               if(!processed_dist_frame.empty())
                    cv::imshow(WINDOW_NAME_1,processed_dist_frame);
            }


       }


       else{
           Mat openDepth((Size(width, height)), CV_16UC1, (void*)aligned_depth_frame.get_data(), Mat::AUTO_STEP);
           std::cout<<"Emit frame" << std::endl;
           emit gotDepthFrame(openDepth);
//           while(!m_bDepthFrameUpdated ){
               if(m_bDepthFrameUpdated){
                     m_bDepthFrameUpdated = false; // reset frame update flag
                    std::cout<<"Got processed depth frame in streamer" << std::endl;
                    if(m_BGsubtraction){
                         Mat fgMaskMOG2;
                         pMOG->apply(processed_depth_frame, fgMaskMOG2);
         cv::imshow(WINDOW_NAME_1,fgMaskMOG2);
                    }
                    else{
                        cv::imshow(WINDOW_NAME_1,processed_depth_frame);
                    }

               }
               else{
                     //std::cout<<"Waiting for processed depth frame" << std::endl;
                }

       }

       // Color Stream



       Mat openColor((Size(640, 480)), CV_8UC3, (void*)other_frame.get_data(), Mat::AUTO_STEP);
       Mat openColorCvt(480,640,CV_8UC3);
       cv::cvtColor(openColor,openColorCvt,COLOR_BGR2RGB);
       cv::imshow(WINDOW_NAME_2,openColorCvt);
       cv::waitKey(50);

       oVideoWriterC<<openColorCvt;
       oVideoWriterG<<processed_depth_frame;
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

float RealSenseStreamer::get_depth_scale(rs2::device dev)
{
    // Go over the device's sensors
    for (rs2::sensor& sensor : dev.query_sensors())
    {
        // Check if the sensor if a depth sensor
        if (rs2::depth_sensor dpt = sensor.as<rs2::depth_sensor>())
        {
            return dpt.get_depth_scale();
        }
    }
    throw std::runtime_error("Device does not have a depth sensor");
}

rs2_stream RealSenseStreamer::find_stream_to_align(const std::vector<rs2::stream_profile>& streams)
{
    //Given a vector of streams, we try to find a depth stream and another stream to align depth with.
    //We prioritize color streams to make the view look better.
    //If color is not available, we take another stream that (other than depth)
    rs2_stream align_to = RS2_STREAM_ANY;
    bool depth_stream_found = false;
    bool color_stream_found = false;
    for (rs2::stream_profile sp : streams)
    {
        rs2_stream profile_stream = sp.stream_type();
        if (profile_stream != RS2_STREAM_DEPTH)
        {
            if (!color_stream_found)         //Prefer color
                align_to = profile_stream;

            if (profile_stream == RS2_STREAM_COLOR)
            {
                color_stream_found = true;
            }
        }
        else
        {
            depth_stream_found = true;
        }
    }

    if(!depth_stream_found)
        throw std::runtime_error("No Depth stream available");

    if (align_to == RS2_STREAM_ANY)
        throw std::runtime_error("No stream found to align with Depth");

    return align_to;
}


bool RealSenseStreamer::profile_changed(const std::vector<rs2::stream_profile>& current, const std::vector<rs2::stream_profile>& prev)
{
    for (auto&& sp : prev)
    {
        //If previous profile is in current (maybe just added another)
        auto itr = std::find_if(std::begin(current), std::end(current), [&sp](const rs2::stream_profile& current_sp) { return sp.unique_id() == current_sp.unique_id(); });
        if (itr == std::end(current)) //If it previous stream wasn't found in current
        {
            return true;
        }
    }
    return false;
}
