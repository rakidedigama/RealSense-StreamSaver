#include "depthconverter.h"
#include "iostream"


using namespace std;

DepthConverter::DepthConverter(QObject* parent):QObject(parent)
{
    m_bDepthFrameProcessed = false;
}

void DepthConverter::createMorphElement(){
    morphElement = getStructuringElement( morph_elem, Size( 2*morph_size + 1, 2*morph_size+1 ), Point( morph_size, morph_size ) );

}

void DepthConverter::processDepthFrame(cv::Mat depthFrame){

    Mat openDepthResized = Mat(480,640,CV_16UC1);
    Mat openDepthResized8 = Mat(480,640,CV_8UC1);
    cv::resize(depthFrame,openDepthResized,Size(640,480));
    //imwrite("depthO.jpg",openDepthResized);
    openDepthResized.convertTo(openDepthResized8,CV_8UC1,0.00390625);
    openDepthResized8 = 255-openDepthResized8;
    //imwrite("depthO8.jpg",openDepthResized8);
    Mat oDepthHistEqualized;
    equalizeHist(openDepthResized8,oDepthHistEqualized);
    //imwrite("depthO8H.jpg",oDepthHistEqualized);


    // Remove 255 values in foreground by thresholding
    Mat threshMask;
    double maxGrayLevel = 254;
    threshold(oDepthHistEqualized,threshMask,maxGrayLevel,255,THRESH_BINARY);
    oDepthHistEqualized.setTo(0,threshMask);

    // Morphology to remove noise
    Mat oDepthOut;
    morphologyEx(oDepthHistEqualized, oDepthOut, 2, morphElement ); // opening
     morphologyEx(oDepthOut, oDepthOut, 3, morphElement ); // closing
    std::cout<<"Depth Frame Processed in Converter" << std::endl;
     emit depthFrameProcessed(oDepthOut);
}

void DepthConverter::processDepthByDistance(rs2::depth_frame depth_frame){

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
     std::cout<<"Distance Frame Processed in Converter" << std::endl;
    emit depthFrameProcessedByDistance(grayDepthFrameResized);
}

int DepthConverter::distance_to_gray(float dist){
    int gray_depth;
    if(dist<=0){
        gray_depth = 0;
    }
    if(dist>10){
        gray_depth = 0;
    }
    else{
        gray_depth = floor((10 - dist)/10 * 255);
    }
    return gray_depth;

}
