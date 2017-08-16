//g++ CopyMat.cpp -o a `pkg-config --cflags opencv --libs`

#include <iostream>
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/opencv.hpp"
#include <string>
#include <sstream>
#include <ctime>
using namespace cv;
using namespace std;

double FullRedMatrix_OpenCV(cv::Mat &src){

    int rows = src.rows;
    int cols = src.cols;
    cv::Vec3b holder;

    clock_t begin = clock();
    for (int j = 0; j < cols; ++j)
    {
        for (int i = 0; i < rows; ++i)
        {
            holder = src.at<cv::Vec3b>(j,i);
            holder = cv::Vec3b(holder[0], holder[1], 255);
            src.at<cv::Vec3b>(j,i) = holder;
        }
    }
    clock_t end = clock();
    double diff = double(end - begin) / CLOCKS_PER_SEC;
    return diff;
}


double FullRedMatrix_OpenCV_2(cv::Mat &src){

    int rows = src.rows;
    int cols = src.cols;
    cv::Vec3b holder;
    clock_t begin = clock();
    for (int j = 0; j < cols; ++j)
    {
        cv::Vec3b* row = src.ptr<cv::Vec3b>(j);

        for (int i = 0; i < rows; ++i)
        {

            row[i] = cv::Vec3b(row[i].val[0], row[i].val[1], 255);
        }
    }

    clock_t end = clock();
    double diff = double(end - begin) / CLOCKS_PER_SEC;
    return diff;
}




int DIM = 500;
int main()
{
    cv::Mat src = cv::Mat::zeros(DIM,DIM,CV_8UC3);
    cv::randu(src, Vec3b::all(0), Vec3b::all(255));
    cv::Mat original = src.clone();

    cv::Mat FullRedOpenCV_1 = src.clone();
    double sum = 0;
    for(int i = 0 ; i < 1000 ; i ++)
        sum += FullRedMatrix_OpenCV(FullRedOpenCV_1);
    std::cout<<"FullRedMatrix_OpenCV: "<<sum<<std::endl;

    cv::Mat FullRedOpenCV_2 = src.clone();
    double sum2 = 0;
    for(int i = 0 ; i < 1000 ; i ++)
        sum2 += FullRedMatrix_OpenCV_2(FullRedOpenCV_2);
    std::cout<<"FullRedMatrix_OpenCV: "<<sum2<<std::endl;



    cv::imshow("Ori",original);
    cv::imshow("FullRedMatrix_OpenCV",FullRedOpenCV_1);
    cv::imshow("FullRedMatrix_OpenCV_2",FullRedOpenCV_2);

    cv::waitKey(0);


    return 0;
}
