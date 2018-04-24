#include <ros/ros.h>
#include <image_transport/image_transport.h>
#include <opencv2/highgui/highgui.hpp>
#include <cv_bridge/cv_bridge.h>
#include <opencv2/opencv.hpp>

int sliderPos = 70;
cv::Mat image;
void processImage(int, void*);

void imageCallback(const sensor_msgs::ImageConstPtr& msg)
{
  try
  {
	cv::VideoCapture cap(0); 

    cv::Mat edges;
	
    cv::namedWindow("edges",1);
    for(;;)
    {   cv::Mat frame;
        cap >> frame; // get a new frame from camera
        cvtColor(frame, edges, CV_BGR2GRAY);
        cv::imshow("edges", edges);
    }
    processImage(0, 0);

  }
  catch (cv_bridge::Exception& e)
  {
    ROS_ERROR("Could not convert from '%s' to 'bgr8'.", msg->encoding.c_str());
  }
}

int main(int argc, char **argv)
{
  ros::init(argc, argv, "image_listener");
  ros::NodeHandle nh;


  cv::namedWindow("view");
  cv::startWindowThread();
  image_transport::ImageTransport it(nh);
  image_transport::Subscriber sub = it.subscribe("usb_cam/image_raw", 1, imageCallback);
  ros::spin();
  cv::destroyWindow("view");


}


