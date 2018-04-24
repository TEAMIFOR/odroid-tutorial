#include <ros/ros.h>
#include <image_transport/image_transport.h>
#include <opencv2/highgui/highgui.hpp>
#include <cv_bridge/cv_bridge.h>
#include <opencv2/opencv.hpp>
#include<vector>
#include<stdio.h>
#include<stdlib.h>

int sliderPos = 70;
cv::Mat image;
void processImage(int /*h*/, void*)
{
	cv::vector< cv::vector<cv::Point> > contours;
	cv::Mat edges;
	cv::Mat
	threshold(edges,dst,127,255,THRESH_BINARY)
    findContours(dst, contours, RETR_LIST, CHAIN_APPROX_NONE);
    cv::Mat cimage = cv::Mat::zeros(bimage.size(), CV_8UC3);
    for(size_t i = 0; i < contours.size(); i++)
    {
        size_t count = contours[i].size();
        if( count < 6 )
            continue;
        cv::Mat pointsf;
        Mat(contours[i]).convertTo(pointsf, CV_32F);
        RotatedRect box = fitEllipse(pointsf);
        if( MAX(box.size.width, box.size.height) > MIN(box.size.width, box.size.height)*30 )
            continue;
        drawContours(cimage, contours, (int)i, Scalar::all(255), 1, 8);
        ellipse(cimage, box, Scalar(0,0,255), 1, LINE_AA);
        ellipse(cimage, box.center, box.size*0.5f, box.angle, 0, 360, Scalar(0,255,255), 1, LINE_AA);
        Point2f vtx[4];
        box.points(vtx);
        for( int j = 0; j < 4; j++ )
            line(cimage, vtx[j], vtx[(j+1)%4], Scalar(0,255,0), 1, LINE_AA);
    }
    cv::imshow("result", cimage);
}

void imageCallback(const sensor_msgs::ImageConstPtr& msg)
{
  try
  {
	cv::Mat frame;
	cv::Mat edges;
	frame = cv_bridge::toCvShare(msg, "bgr8")->image;
	cvtColor(frame, edges, CV_BGR2GRAY);
	cv::imshow("view", edges);
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
