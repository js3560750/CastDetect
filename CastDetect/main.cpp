#include <iostream>  
#include <opencv2/core/core.hpp>  
#include <opencv2/highgui/highgui.hpp>  
#include <opencv2/imgproc/imgproc.hpp>
#include "MyAlgorithm.h"

using namespace cv;

//基本测试，能使用OpenCV
int baseTest()
{
	// 读入一张图片，opencv默认作为BGR类型处理图片，可以用cvtColor（,,CV_BGR2RGB）转换
	//Mat类能够自动管理内存，Mat类由矩阵头和指向存储所有像素值的矩阵的指针构成
	//注意读取灰度图要增加IMREAD_GRAYSCALE
	Mat img = imread("test.jpg");
	//如果图像未读入，即img.data=null返回-1
	if (img.empty()) {
		return -1;
	}


	//新建灰度图
	Mat imgGray;
	//转为灰度图
	cvtColor(img, imgGray, CV_BGR2GRAY);	//需要#include imgproc.hpp

											//保存图片
											//imwrite("save.jpg", img);
											// 创建一个名为 "游戏原画"窗口  
	namedWindow("游戏原画");
	// 在窗口中显示游戏原画  
	imshow("游戏原画", img);
	imshow("Gray", imgGray);	//直接调用imshow，自动创建Gray窗口，不需要额外写namedWindow("Gray")
								// 等待6000 ms后窗口自动关闭  
	waitKey(6000);
}

int main() {
	//读入原图
	Mat img = imread("./picture/cast/1.jpg");/*转义的换行是\n 反斜杠是\\ */
	if (!img.data) {
		std::cout << "读图错误" << std::endl;
	}
	//转为灰度图
	Mat imgGray;
	cvtColor(img, imgGray, CV_BGR2GRAY);

	//滤波
	MyAlgorithm myAlgorithm;
	Mat imgGaussain = myAlgorithm.myBlur(imgGray);

	//二值化
	Mat imgThreshold = myAlgorithm.myThreshold(imgGaussain);

	//闭运算
	Mat imgMorp = myAlgorithm.myMorph(imgThreshold);

	//腐蚀
	Mat imgErode = myAlgorithm.myErode(imgMorp);

	//找轮廓和画最大外接矩形
	Mat imgContours = myAlgorithm.myFindContours(imgErode);

	//imshow("原图", img);
	//imshow("二值化", imgThreshold);
	imwrite("./picture/result/result.jpg", imgContours);
	//waitKey(0);
	return 0;
}