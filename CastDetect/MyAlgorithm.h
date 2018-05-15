#pragma once
#include<iostream>  
#include <opencv2/core/core.hpp>  
#include <opencv2/highgui/highgui.hpp>  
#include<opencv2/imgproc/imgproc.hpp>

using namespace cv;

class MyAlgorithm
{
public:
	MyAlgorithm();
	~MyAlgorithm();
	
	Mat myBlur(const Mat &image);
	Mat myThreshold(const Mat &image);
	Mat myMorph(const Mat &image);
	Mat myErode(const Mat &image);
	Mat myFindContours(const Mat &image);
};

