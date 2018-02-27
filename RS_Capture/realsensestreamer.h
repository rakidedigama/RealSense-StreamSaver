


#include "depthconverter.h"




#ifndef REALSENSESTREAMER_H
#define REALSENSESTREAMER_H
#include "QObject"

class RealSenseStreamer : public QObject
{
    Q_OBJECT
public:
    RealSenseStreamer(QObject *parent=0);
    int runStream();

private:
    QThread* depthConverterThread ;
    DepthConverter* dConverter ;
    bool m_bProcessByDistance;
    cv::Mat processed_depth_frame;
     bool m_bDepthFrameUpdated ;

    cv::Mat processed_dist_frame;
    bool m_bDistFrameUpdated;

    Ptr<BackgroundSubtractor> pMOG;
    bool m_BGsubtraction;

signals:

    void gotDepthFrame(cv::Mat depth);
    void convertDepthFrameToDistance(rs2::depth_frame);
//    cv::VideoWriter oVideoWriterG;
//    cv::VideoWriter oVideoWriterC;

public slots:
   void updateDepthFrame(cv::Mat newDepthFrame);
   void updateDistanceFrame(cv::Mat newDistFrame);
};

#endif // REALSENSESTREAMER_H
