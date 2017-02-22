//g++ CopyMat.cpp -o a `pkg-config --cflags opencv --libs`

#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/opencv.hpp"
#include <string> 
#include <sstream> 
#include <ctime>
using namespace cv;
using namespace std;


cv::Mat src4,dst4,src5,dst5;

cv::Mat& foo(){
	return src4;
}
cv::Mat foo2(){
	return src4;
}

int main(){

	cv::Mat src1,src2,src3,dst1,dst2,dst3;
	src1 = imread("lena.bmp");
	src2 = imread("lena.bmp");
	src3 = imread("lena.bmp");
	src4 = imread("lena.bmp");
	src5 = imread("lena.bmp");

	clock_t begin3 = clock();
	for(int i = 0 ; i < 100000 ; i++)
	{
		src3.copyTo(dst3);	
	}
	clock_t end3 = clock(); 
	double diff3 = double(end3 - begin3) / CLOCKS_PER_SEC;
	std::cout<<"CopyTo "<<diff3<<std::endl;
	
	clock_t begin2 = clock();
	for(int i = 0 ; i < 100000 ; i++)
	{
		dst2 = src2.clone();
	}
	clock_t end2 = clock(); 
	double diff2 = double(end2 - begin2) / CLOCKS_PER_SEC;
	std::cout<<"Clone "<<diff2<<std::endl;
	
  	clock_t begin = clock();
	for(int i = 0 ; i < 100000 ; i++)
	{
		dst1 = src1;
	}
	clock_t end = clock(); 
	double diff = double(end - begin) / CLOCKS_PER_SEC;
	std::cout<<"Operator= "<<diff<<std::endl;
	
	clock_t begin5 = clock();	
	for(int i = 0 ; i < 100000 ; i++)
	{
		dst5 = foo2();
	}
	clock_t end5 = clock(); 
	double diff5 = double(end5 - begin5) / CLOCKS_PER_SEC;
	std::cout<<"Function "<<diff5<<std::endl;


	clock_t begin4 = clock();
	for(int i = 0 ; i < 100000 ; i++)
	{
		dst4 = foo();
	}
	clock_t end4 = clock(); 
	double diff4 = double(end4 - begin4) / CLOCKS_PER_SEC;
	std::cout<<"&Function "<<diff4<<std::endl;

	imshow("Operator=",dst1);
	imshow("Clone",dst2);
	imshow("CopyTo",dst3);
	imshow("&Function",dst4);
	imshow("Function",dst5);



	cv::waitKey(0);

	return 0 ;
}