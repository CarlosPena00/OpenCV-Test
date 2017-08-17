//g++ MatAcess.cpp -o a `pkg-config --cflags opencv --libs`


#include "mainwindow.h"
#include <QApplication>
#include <opencv2/opencv.hpp>
#include <iostream>

#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/opencv.hpp"
#include <string>
#include <sstream>
#include <ctime>
#include <tbb/tbb.h>
using namespace cv;
using namespace std;
using namespace tbb ;

double FullRedMatrix_OpenCV(cv::Mat &src){

    int rows = src.rows;
    int cols = src.cols;


    clock_t begin = clock();
    for (int j = 0; j < cols; ++j)
    {
        for (int i = 0; i < rows; ++i)
        {
            src.at<cv::Vec3b>(j,i)[2] = 255;
        }
    }
    clock_t end = clock();
    double diff = double(end - begin) / CLOCKS_PER_SEC;
    return diff;
}


double FullRedMatrix_OpenCV_2(cv::Mat &src){

    int rows = src.rows;
    int cols = src.cols;
    clock_t begin = clock();
    for (int j = 0; j < cols; ++j)
    {
        cv::Vec3b* row = src.ptr<cv::Vec3b>(j);

        for (int i = 0; i < rows; ++i)
            row[i].val[2] = 255;

    }

    clock_t end = clock();
    double diff = double(end - begin) / CLOCKS_PER_SEC;
    return diff;
}


class parallel_pixel
{
private:
    uchar *p ;
public:
    parallel_pixel(uchar *ptr ) : p(ptr) { }

    void operator() ( const blocked_range<int>& r ) const
    {
        for ( int i = r.begin(); i != r.end(); ++i ) {
            p[(i*3)+2] = 255  ;
        }
    }
};



class Parallel_clipBufferValues: public cv::ParallelLoopBody
{
private:
  uchar *bufferToClip;
  //uchar minValue, maxValue;

public:
  Parallel_clipBufferValues(uchar* bufferToProcess)
    : bufferToClip(bufferToProcess){}

  virtual void operator()( const cv::Range &r ) const {
    register uchar *inputOutputBufferPTR=bufferToClip+r.start;
    for (register int jf = r.start; jf != r.end; ++jf, ++inputOutputBufferPTR)
    {
        bufferToClip[(jf*3)+2] = 255;
    }
  }
};


int DIM = 2000;
int main()
{

    cv::Mat src = cv::Mat::zeros(DIM,DIM,CV_8UC3);
    //cv::randu(src, Vec3b::all(0), Vec3b::all(255));
    cv::Mat original = src.clone();
    cv::Mat src2 = src.clone();
    cv::Mat FullRedOpenCV_1 = src.clone();
    double sum = 0;
    for(int i = 0 ; i < 1000 ; i ++)
        sum += FullRedMatrix_OpenCV(FullRedOpenCV_1);
    std::cout<<"FullRedMatrix_OpenCV: "<<sum<<std::endl;

    cv::Mat FullRedOpenCV_2 = src.clone();
    double sum2 = 0;
    for(int i = 0 ; i < 1000 ; i ++)
        sum2 += FullRedMatrix_OpenCV_2(FullRedOpenCV_2);
    std::cout<<"FullRedMatrix_OpenCV_2: "<<sum2<<std::endl;

    task_scheduler_init init;
    uchar* p2 = src.data;
    uchar* p3 = src2.data;

    int nElements = src.cols*src.rows;
    clock_t begin = clock();

    for(int i = 0 ; i < 1000 ; i ++)
        parallel_for(blocked_range<int>(0, nElements, 800), parallel_pixel(p2) ) ;

    clock_t end = clock();
    double diff = double(end - begin) / CLOCKS_PER_SEC;
    std::cout<<"TTB: "<<diff<<std::endl;

    begin = clock();
    for(int i = 0 ; i < 1000 ; i++)
        parallel_for_(cv::Range(0, nElements), Parallel_clipBufferValues(p3));
    end = clock();
    diff = double(end - begin) / CLOCKS_PER_SEC;
    std::cout<<"Parallel_for_: "<<diff<<std::endl;



    cv::imshow("Ori",original);
    cv::imshow("FullRedMatrix_OpenCV",FullRedOpenCV_1);
    cv::imshow("FullRedMatrix_OpenCV_2",FullRedOpenCV_2);
    cv::imshow("TBB",src);
    cv::imshow("Parallel_for_",src2);

    cv::waitKey(0);


    return 0;
}


/*
Matrix 2000 x 2000
inter = 1000

1) src.at<cv::Vec3b>(j,i)

Time: 47.0361

2)cv::Vec3b* row = src.ptr<cv::Vec3b>(j)

Time: 12.3938

3) TTB - parallel_for

Time: 28.0166

4) OpenCV- parallel_for_

Time: 13.0653

*/