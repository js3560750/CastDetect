#include "MyAlgorithm.h"
using namespace std;


MyAlgorithm::MyAlgorithm()
{
	//无参的构造函数
}

MyAlgorithm::~MyAlgorithm()
{
	//析构函数，对象被销毁之前，会自动调用析构函数
}


//滤波
Mat MyAlgorithm::myBlur(const Mat &image) {
	Mat out;
	//blur(image, out, Size(3, 3));
	GaussianBlur(image, out, Size(3, 3), 0, 0);	//Size=3、7、15
	//boxFilter(image, out, -1, Size(5, 5));
	//medianBlur(image, out, 7);
	
	return out;
}

//二值化
Mat MyAlgorithm::myThreshold(const Mat &image) {
	
	/*
	//初始化adaptiveThreshold函数要用的各类参数
	Mat dstImage;			//先定义好二值化图
	int blockSize = 5;		//表示领域块大小，用来计算区域阈值，一般为3、5、7......
	int constValue = 10;	//教程中写的10，没写为毛选10
	const int maxVal = 255;	//预设灰度最大值
	int adaptiveMethod = 0;	//自适应阈值算法,0是一种，1是另一种
	int thresholdType = 1;	//阈值类型，0是二进制阈值化，即大于阈值的为255。1是反二进制阈值化，即大于阈值的为0
							//图像自适应阈值操作
	adaptiveThreshold(image, dstImage, maxVal, adaptiveMethod, thresholdType, blockSize, constValue);
	*/

	
	Mat dstImage;	//先定义好二值化图
	int thresh = 155;	//阈值
	
	//初始化阈值化处理的类型
	//0: 二进制阈值；		大于阈值的为255，<=阈值的为0
	//1：反二进制阈值；	大于阈值的为0，<=阈值的为255
	//2：截断阈值;		不常用
	//3: 0阈值；			不常用
	//4：反0阈值			不常用
	
	int threshType = 1;
	//预设最大值
	const int maxVal = 255;
	//固定阈值化操作
	threshold(image, dstImage, thresh, maxVal, threshType);
	
	return dstImage;
}

//开闭运算
Mat MyAlgorithm::myMorph(const Mat &image) {
	Mat out;
	//定义核  
	Mat element = getStructuringElement(MORPH_RECT, Size(25,25));
	/*
	第一个参数是原图，第二个参数是输出图像，第四个参数是核
	关键第三个参数，填入不同的值会执行不同的操作
	MORPH_OPEN – 开运算（Opening operation）
	MORPH_CLOSE – 闭运算（Closing operation）
	MORPH_GRADIENT -形态学梯度（Morphological gradient）
	MORPH_TOPHAT - “顶帽”（“Top hat”）
	MORPH_BLACKHAT - “黑帽”（“Black hat“）
	*/
	morphologyEx(image, out, MORPH_CLOSE, element);
	return out;
}

//腐蚀
Mat MyAlgorithm::myErode(const Mat &image) {
	Mat element = getStructuringElement(MORPH_RECT, Size(9, 9));
	Mat out;
	erode(image, out, element);
	return out;
}

//找轮廓和画最大外接矩形
Mat MyAlgorithm::myFindContours(const Mat &image) {
	Canny(image, image, 100, 250);	//经Canny边缘检测处理后的图就已经有了一定的轮廓
	//findContours要用的部分参数
	vector<vector<Point>> contours;
	//vector<Vec4i> hierarchy;	//hierachy向量表示轮廓的父子关系
	//找轮廓点Point，如果mode取值“RETR_EXTERNAL”即只检测最外层轮廓
	findContours(image, contours, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE, Point());

	//剔除不想要的轮廓，比如太小的轮廓
	vector<vector<Point>>::const_iterator itc = contours.begin();	//定义迭代器准备迭代轮廓向量
	while (itc!=contours.end())	//迭代
	{
		RotatedRect rect = minAreaRect(*itc);
		if (rect.size.height<=50||rect.size.width<150)	//剔除长和宽太小的矩形
		{
			itc=contours.erase(itc);
		}
		else
		{
			++itc;	//C++中 ++i比i++快一丢丢
		}
	}

	Mat Contours = Mat::zeros(image.size(), CV_8UC1);		//画布，用来绘制findContours函数找到的Point点
	Mat imgContours = Mat::zeros(image.size(), CV_8UC1);	//画布，用来根据找到的Point点绘制轮廓
	//绘制轮廓
	for (int i = 0; i<contours.size(); i++)
	{
		//contours[i]代表的是第i个轮廓，contours[i].size()代表的是第i个轮廓上所有的像素点数  
		for (int j = 0; j<contours[i].size(); j++)
		{
			//绘制出contours向量内所有的像素点
			Point p = Point(contours[i][j].x, contours[i][j].y);
			Contours.at<uchar>(p) = 255;	//图Contours中所有符合p坐标的点都设为白色
		}

		//输出hierarchy向量内容 ，
		//cout << "向量hierarchy的第" << i << " 个元素内容为：" << endl << hierarchy[i] << endl << endl;


		/*
		第一个参数：	要绘制轮廓的图像，即在此图上绘制
		第二个参数：	轮廓点Point向量集合，即findContours()找到的contours
		第三个参数：	要绘制的轮廓的编号，如果为负数，则绘制所有轮廓
		第四个参数：	颜色Scalar(255)=白色
		第五个参数：	线条粗细，如果是负数，则轮廓内部被填充，默认1
		第六个参数：	绘制轮廓的线的连通性，默认8
		第七个参数：	hierarchy
		*/
		//绘制轮廓
		drawContours(imgContours, contours, i, Scalar(255), 2, 8);
		//使用hierarchy参数时,hierarchy.size必须等于contours.size，否则会报异常
		//drawContours(imgContours, contours, i, Scalar(255), 2, 8,hierarchy);
		
		//绘制最小外接矩形
		//RotatedRect minAreaRect( InputArray points )函数求点集的最小外接矩形
		RotatedRect rect = minAreaRect(contours[i]);//contours[i]代表的是第i个轮廓的点集
		Point2f P[4];	//Point2f即Point_<float>，这里定义了一个float型的点的数组
		rect.points(P);
		for (int j = 0; j <= 3; j++)
		{
			line(imgContours, P[j], P[(j + 1) % 4], Scalar(255), 2);
		}
	}
	return imgContours;
};