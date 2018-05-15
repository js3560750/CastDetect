#include <iostream>  
#include <opencv2/core/core.hpp>  
#include <opencv2/highgui/highgui.hpp>  
#include <opencv2/imgproc/imgproc.hpp>
#include "MyAlgorithm.h"

using namespace cv;

//�������ԣ���ʹ��OpenCV
int baseTest()
{
	// ����һ��ͼƬ��opencvĬ����ΪBGR���ʹ���ͼƬ��������cvtColor��,,CV_BGR2RGB��ת��
	//Mat���ܹ��Զ������ڴ棬Mat���ɾ���ͷ��ָ��洢��������ֵ�ľ����ָ�빹��
	//ע���ȡ�Ҷ�ͼҪ����IMREAD_GRAYSCALE
	Mat img = imread("test.jpg");
	//���ͼ��δ���룬��img.data=null����-1
	if (img.empty()) {
		return -1;
	}


	//�½��Ҷ�ͼ
	Mat imgGray;
	//תΪ�Ҷ�ͼ
	cvtColor(img, imgGray, CV_BGR2GRAY);	//��Ҫ#include imgproc.hpp

											//����ͼƬ
											//imwrite("save.jpg", img);
											// ����һ����Ϊ "��Ϸԭ��"����  
	namedWindow("��Ϸԭ��");
	// �ڴ�������ʾ��Ϸԭ��  
	imshow("��Ϸԭ��", img);
	imshow("Gray", imgGray);	//ֱ�ӵ���imshow���Զ�����Gray���ڣ�����Ҫ����дnamedWindow("Gray")
								// �ȴ�6000 ms�󴰿��Զ��ر�  
	waitKey(6000);
}

int main() {
	//����ԭͼ
	Mat img = imread("./picture/cast/1.jpg");/*ת��Ļ�����\n ��б����\\ */
	if (!img.data) {
		std::cout << "��ͼ����" << std::endl;
	}
	//תΪ�Ҷ�ͼ
	Mat imgGray;
	cvtColor(img, imgGray, CV_BGR2GRAY);

	//�˲�
	MyAlgorithm myAlgorithm;
	Mat imgGaussain = myAlgorithm.myBlur(imgGray);

	//��ֵ��
	Mat imgThreshold = myAlgorithm.myThreshold(imgGaussain);

	//������
	Mat imgMorp = myAlgorithm.myMorph(imgThreshold);

	//��ʴ
	Mat imgErode = myAlgorithm.myErode(imgMorp);

	//�������ͻ������Ӿ���
	Mat imgContours = myAlgorithm.myFindContours(imgErode);

	//imshow("ԭͼ", img);
	//imshow("��ֵ��", imgThreshold);
	imwrite("./picture/result/result.jpg", imgContours);
	//waitKey(0);
	return 0;
}