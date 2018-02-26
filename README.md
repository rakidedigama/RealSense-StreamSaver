# RealSense-StreamSaver
This is an C++ OpenCV application that captures Depth/Color streams from RealSense D435 using the RealSenseSDK 2.0 and writes them in FFMPG 
format as seperate video files. The RGB stream is saved as 3 channel CV_8UC3 frames while the 16 bit unsigned depth stream is converted & scaled
to CV_8UC1 and saved as a grayscale video. 

The program provides TWO options for depth Mapping. By distance (in meters) or by the direct scaling of 16 bit unsigned values. The mapping by distance is done by considering the range as [0,10] meters. 




