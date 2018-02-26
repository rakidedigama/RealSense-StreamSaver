#ifndef DEPTHCONVERTER_H
#define DEPTHCONVERTER_H

#include "rs.hpp"
#include "QObject"
#include "opencv2/core.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/videoio.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/objdetect.hpp"
#include "opencv2/video/background_segm.hpp"





using namespace cv;

class DepthConverter : public QObject
{
    Q_OBJECT


public:
    explicit DepthConverter(QObject* parent=0);


    void createMorphElement();
public slots:
    void processDepthFrame(cv::Mat depthFrame);
    void processDepthByDistance(rs2::depth_frame depth_frame);

private:
    bool m_bDepthFrameProcessed;
    Mat morphElement;
    int morph_elem = 2;
    int morph_size = 5;
    static int distance_to_gray(float dist);

signals:
    void depthFrameProcessed(cv::Mat processedDepthFrame);
    void depthFrameProcessedByDistance(cv::Mat grayDepthFrameResized);

};

#endif // DEPTHCONVERTER_H
