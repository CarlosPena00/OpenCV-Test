//g++ follow.cpp -o a `pkg-config --cflags opencv --libs`
#include <assert.h>

#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/opencv.hpp"
#include <string> 
#include <sstream> 
#include <ctime>
using namespace cv;
using namespace std;
 cv::Mat test;
void rectan(cv::Mat& src)
{
	cv::rectangle(src,cv::Point(45,45),cv::Point(125,125),cv::Scalar(255,255,255),CV_FILLED,8,0);
/*	cv::Point2f pt(src.cols/2., src.rows/2.);
	cv::Mat rota = cv::getRotationMatrix2D(pt, 40.0, 1.0);
	cv::warpAffine(src, src, rota, Size(src.cols, src.rows));
*/
}

cv::Point getPoint(cv::Point alfa, int N)
{

	N = N%8;
	switch (N)
	{
		case 0:
			return cv::Point(alfa) + cv::Point(-1,0);
			break;
		case 1:
			return cv::Point(alfa) + cv::Point(-1,-1);
			break;
		case 2:
			return cv::Point(alfa) + cv::Point(0,-1);
			break;
		case 3:
			return cv::Point(alfa) + cv::Point(1,-1);
			break;
		case 4:
			return cv::Point(alfa) + cv::Point(1,0);
			break;
		case 5:
			return cv::Point(alfa) + cv::Point(1,1);
			break;
		case 6:
			return cv::Point(alfa) + cv::Point(0,1);
			break;
		case 7:
			return cv::Point(alfa) + cv::Point(-1,1);
			break;


	}
}

int getN(cv::Point alfa, cv::Point beta)
{

	cv::Point var = beta-alfa;
	if( var == cv::Point(-1,0))return 0;
	if( var == cv::Point(-1,-1))return 1;
	if( var == cv::Point(0,-1))return 2;
	if( var == cv::Point(1,-1))return 3;
	if( var == cv::Point(1 ,0))return 4;
	if( var == cv::Point(1 ,1))return 5;
	if( var == cv::Point(0 ,1))return 6;
	if( var == cv::Point(-1,1))return 7;

	assert(false);
	std::cout<<")))))))))))))))"<<std::endl;
	return -1;
}


void findneighbors(cv::Mat& src,cv::Point B0, cv::Point C0)
{	
//	
	cv::Point B1, C1, B,C,var,varOld;

	for(int n = getN(B0,C0) ; n <= 8 ; n++)
	{
		var = getPoint(B0, n);
		
		if(src.at<cv::Vec3b>(var) == cv::Vec3b(255,255,255))
		{
		
			B1 = var;
			if(test.at<cv::Vec3b>(B1) == cv::Vec3b(255,0,0))return;
			test.at<cv::Vec3b>(var) = cv::Vec3b(255,0,0);
			findneighbors(src, B1, getPoint(B0,n-1));
			return;
		}
		if(n==8)n=0;


	}


}

void findBoard(cv::Mat& src, int x, int y)
{

	cv::Point B0 = cv::Point(x,y);
	cv::Point C0 = cv::Point(x-1,y-1);
	findneighbors(src,B0,C0);
		
}

void findObj(cv::Mat& src)
{
	int rows = src.rows;
	int cols = src.cols;

	for(int j = 0 ; j < rows; j++)
	{
		for(int i = 0; i < cols; i++)
		{
			if(src.at<cv::Vec3b>(j,i) == cv::Vec3b(255,255,255))
			{
				findBoard(src,i,j);	
				//src.at<cv::Vec3b>(j,i) = cv::Vec3b(255,0,0);	
				return;			
			}
			
		}
	}
}


int main()
{
	cv::Mat src = cv::Mat::zeros(200, 200, CV_8UC3);
	test = src.clone();
	rectan(src);

	findObj(src);
	cv::imshow("test",test);
	cv::waitKey(0);
	return 0;
}