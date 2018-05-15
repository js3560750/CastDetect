#include "MyAlgorithm.h"
using namespace std;


MyAlgorithm::MyAlgorithm()
{
	//�޲εĹ��캯��
}

MyAlgorithm::~MyAlgorithm()
{
	//������������������֮ǰ�����Զ�������������
}


//�˲�
Mat MyAlgorithm::myBlur(const Mat &image) {
	Mat out;
	//blur(image, out, Size(3, 3));
	GaussianBlur(image, out, Size(3, 3), 0, 0);	//Size=3��7��15
	//boxFilter(image, out, -1, Size(5, 5));
	//medianBlur(image, out, 7);
	
	return out;
}

//��ֵ��
Mat MyAlgorithm::myThreshold(const Mat &image) {
	
	/*
	//��ʼ��adaptiveThreshold����Ҫ�õĸ������
	Mat dstImage;			//�ȶ���ö�ֵ��ͼ
	int blockSize = 5;		//��ʾ������С����������������ֵ��һ��Ϊ3��5��7......
	int constValue = 10;	//�̳���д��10��ûдΪëѡ10
	const int maxVal = 255;	//Ԥ��Ҷ����ֵ
	int adaptiveMethod = 0;	//����Ӧ��ֵ�㷨,0��һ�֣�1����һ��
	int thresholdType = 1;	//��ֵ���ͣ�0�Ƕ�������ֵ������������ֵ��Ϊ255��1�Ƿ���������ֵ������������ֵ��Ϊ0
							//ͼ������Ӧ��ֵ����
	adaptiveThreshold(image, dstImage, maxVal, adaptiveMethod, thresholdType, blockSize, constValue);
	*/

	
	Mat dstImage;	//�ȶ���ö�ֵ��ͼ
	int thresh = 155;	//��ֵ
	
	//��ʼ����ֵ�����������
	//0: ��������ֵ��		������ֵ��Ϊ255��<=��ֵ��Ϊ0
	//1������������ֵ��	������ֵ��Ϊ0��<=��ֵ��Ϊ255
	//2���ض���ֵ;		������
	//3: 0��ֵ��			������
	//4����0��ֵ			������
	
	int threshType = 1;
	//Ԥ�����ֵ
	const int maxVal = 255;
	//�̶���ֵ������
	threshold(image, dstImage, thresh, maxVal, threshType);
	
	return dstImage;
}

//��������
Mat MyAlgorithm::myMorph(const Mat &image) {
	Mat out;
	//�����  
	Mat element = getStructuringElement(MORPH_RECT, Size(25,25));
	/*
	��һ��������ԭͼ���ڶ������������ͼ�񣬵��ĸ������Ǻ�
	�ؼ����������������벻ͬ��ֵ��ִ�в�ͬ�Ĳ���
	MORPH_OPEN �C �����㣨Opening operation��
	MORPH_CLOSE �C �����㣨Closing operation��
	MORPH_GRADIENT -��̬ѧ�ݶȣ�Morphological gradient��
	MORPH_TOPHAT - ����ñ������Top hat����
	MORPH_BLACKHAT - ����ñ������Black hat����
	*/
	morphologyEx(image, out, MORPH_CLOSE, element);
	return out;
}

//��ʴ
Mat MyAlgorithm::myErode(const Mat &image) {
	Mat element = getStructuringElement(MORPH_RECT, Size(9, 9));
	Mat out;
	erode(image, out, element);
	return out;
}

//�������ͻ������Ӿ���
Mat MyAlgorithm::myFindContours(const Mat &image) {
	Canny(image, image, 100, 250);	//��Canny��Ե��⴦����ͼ���Ѿ�����һ��������
	//findContoursҪ�õĲ��ֲ���
	vector<vector<Point>> contours;
	//vector<Vec4i> hierarchy;	//hierachy������ʾ�����ĸ��ӹ�ϵ
	//��������Point�����modeȡֵ��RETR_EXTERNAL����ֻ������������
	findContours(image, contours, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE, Point());

	//�޳�����Ҫ������������̫С������
	vector<vector<Point>>::const_iterator itc = contours.begin();	//���������׼��������������
	while (itc!=contours.end())	//����
	{
		RotatedRect rect = minAreaRect(*itc);
		if (rect.size.height<=50||rect.size.width<150)	//�޳����Ϳ�̫С�ľ���
		{
			itc=contours.erase(itc);
		}
		else
		{
			++itc;	//C++�� ++i��i++��һ����
		}
	}

	Mat Contours = Mat::zeros(image.size(), CV_8UC1);		//��������������findContours�����ҵ���Point��
	Mat imgContours = Mat::zeros(image.size(), CV_8UC1);	//���������������ҵ���Point���������
	//��������
	for (int i = 0; i<contours.size(); i++)
	{
		//contours[i]������ǵ�i��������contours[i].size()������ǵ�i�����������е����ص���  
		for (int j = 0; j<contours[i].size(); j++)
		{
			//���Ƴ�contours���������е����ص�
			Point p = Point(contours[i][j].x, contours[i][j].y);
			Contours.at<uchar>(p) = 255;	//ͼContours�����з���p����ĵ㶼��Ϊ��ɫ
		}

		//���hierarchy�������� ��
		//cout << "����hierarchy�ĵ�" << i << " ��Ԫ������Ϊ��" << endl << hierarchy[i] << endl << endl;


		/*
		��һ��������	Ҫ����������ͼ�񣬼��ڴ�ͼ�ϻ���
		�ڶ���������	������Point�������ϣ���findContours()�ҵ���contours
		������������	Ҫ���Ƶ������ı�ţ����Ϊ�������������������
		���ĸ�������	��ɫScalar(255)=��ɫ
		�����������	������ϸ������Ǹ������������ڲ�����䣬Ĭ��1
		������������	�����������ߵ���ͨ�ԣ�Ĭ��8
		���߸�������	hierarchy
		*/
		//��������
		drawContours(imgContours, contours, i, Scalar(255), 2, 8);
		//ʹ��hierarchy����ʱ,hierarchy.size�������contours.size������ᱨ�쳣
		//drawContours(imgContours, contours, i, Scalar(255), 2, 8,hierarchy);
		
		//������С��Ӿ���
		//RotatedRect minAreaRect( InputArray points )������㼯����С��Ӿ���
		RotatedRect rect = minAreaRect(contours[i]);//contours[i]������ǵ�i�������ĵ㼯
		Point2f P[4];	//Point2f��Point_<float>�����ﶨ����һ��float�͵ĵ������
		rect.points(P);
		for (int j = 0; j <= 3; j++)
		{
			line(imgContours, P[j], P[(j + 1) % 4], Scalar(255), 2);
		}
	}
	return imgContours;
};