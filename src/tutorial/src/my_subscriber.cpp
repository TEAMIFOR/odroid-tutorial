#include <ros/ros.h>
#include <image_transport/image_transport.h>
#include <opencv2/highgui/highgui.hpp>
#include <cv_bridge/cv_bridge.h>
#include <opencv2/opencv.hpp>
#include<vector>
#include<stdio.h>
#include<stdlib.h>


void imageCallback(const sensor_msgs::ImageConstPtr& msg)
{
  try
  {
        cv::Mat edges;
	cv::Mat frame;
	cv::Mat res;
	//cv::namedWindow("dst",1);
	cv::namedWindow("res",1);	 
	cv::namedWindow("NORMAL",1);

	frame = cv_bridge::toCvShare(msg, "bgr8")->image;

	cvtColor(frame, edges, CV_BGR2GRAY);
	 //cv::imshow("NORMAL", edges);
    
	cv::vector< cv::vector<cv::Point> > contours;
	cv::Mat dst;

        
	medianBlur(edges, edges, 5);
	threshold(edges,dst,127,255,cv::THRESH_BINARY);
	//cv::imshow("dst", dst);
	medianBlur(dst, res, 5);
	//cv::imshow("res", res);
    findContours(res, contours, cv::RETR_LIST, cv::CHAIN_APPROX_NONE);
    cv::Mat cimage = cv::Mat::zeros(dst.size(), CV_8UC3);
    for(size_t i = 0; i < contours.size(); i++)
    {
        size_t count = contours[i].size();
        if( count < 6 )
            continue;
        cv::Mat pointsf;
        cv::Mat(contours[i]).convertTo(pointsf, CV_32F);
        cv::RotatedRect box = cv::fitEllipse(pointsf);

	float Ma = box.size.width;
	float ma = box.size.height;
	ROS_INFO("%f\n , %f",Ma,ma);
	//printf("%f\n",ma); 
	
	
	
        if(ma>50 && Ma>75 && Ma<200 )
         {
	
	// drawContours(cimage, contours, (int)i, cv::Scalar::all(255), 1, 8);
       cv::ellipse(cimage, box, cv::Scalar(0,0,255),  1);
        cv::ellipse(cimage, box.center, box.size*0.5f, box.angle, 0, 360, cv::Scalar(0,255,255),  1);

        
        cv::Point2f vtx[4];
        box.points(vtx);
        for( int j = 0; j < 4; j++ )
            line(cimage, vtx[j], vtx[(j+1)%4], cv::Scalar(0,255,0), 1);
    	}
	}
    //cv::imshow("view", cimage);


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

return 0;
}
