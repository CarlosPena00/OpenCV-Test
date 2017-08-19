//g++ MatAcess.cpp -o a `pkg-config --cflags opencv --libs`
// Maybe -O3, -fopenmp

#include <opencv2/opencv.hpp>
#include <iostream>

#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/opencv.hpp"
#include <string>
#include <sstream>
#include <ctime>
#include <tbb/tbb.h>
#include <omp.h>

using namespace cv;
using namespace std;
using namespace tbb ;

double RedMatrix_OpenCV(cv::Mat &src){

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


double RedMatrix_OpenCV_2(cv::Mat &src){

    int rows = src.rows;
    int cols = src.cols;
    clock_t begin = clock();
    cv::Vec3b* row;
    for (int j = 0; j < cols; ++j)
    {
        row = src.ptr<cv::Vec3b>(j);

        for (int i = 0; i < rows; ++i)
            row[i].val[2] = 255;

    }

    clock_t end = clock();
    double diff = double(end - begin) / CLOCKS_PER_SEC;
    return diff;
}


double OpenMP_2(cv::Mat &src){

    int rows = src.rows;
    int cols = src.cols;
    clock_t begin = clock();
    cv::Vec3b* row;
    for (int j = 0; j < cols; ++j)
    {
        row = src.ptr<cv::Vec3b>(j);
        #pragma omp for nowait
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


int SIZE = 500;
int Ninter = 10000;
int main()
{

    cv::Mat original  = cv::Mat::zeros(SIZE,SIZE,CV_8UC3);
    cv::Mat src = original.clone();
    cv::Mat src2 = src.clone();
    cv::Mat src3 = src.clone();
    cv::Mat src4 = src.clone();
    cv::Mat FullRedOpenCV_1 = src.clone();
    cv::Mat FullRedOpenCV_2 = src.clone();
    uchar* p2 = src.data;
    uchar* p3 = src.data;

    int nElements = src.cols*src.rows;

    double sum = 0, sum2 = 0, sum3 = 0;

    for(int i = 0 ; i < Ninter ; i ++)
        sum += RedMatrix_OpenCV(FullRedOpenCV_1);
    std::cout<<"1) src.at<cv::Vec3b>(j,i)"<<std::endl<<"Time : "<<sum<<std::endl;

    for(int i = 0 ; i < Ninter ; i ++)
        sum2 += RedMatrix_OpenCV_2(FullRedOpenCV_2);
    std::cout<<"2) cv::Vec3b* row = src.ptr<cv::Vec3b>(j)"<<std::endl<<"Time: "<<sum2<<std::endl;

    task_scheduler_init init;

    clock_t begin = clock();

    for(int i = 0 ; i < Ninter ; i ++)
        parallel_for(blocked_range<int>(0, nElements, 800), parallel_pixel(p2) ) ;

    clock_t end = clock();
    double diff = double(end - begin) / CLOCKS_PER_SEC;
    std::cout<<"3) TTB - parallel_for"<<std::endl<<"Time: "<<diff<<std::endl;

    begin = clock();
    for(int i = 0 ; i < Ninter ; i++)
        parallel_for_(cv::Range(0, nElements), Parallel_clipBufferValues(p3));
    end = clock();
    diff = double(end - begin) / CLOCKS_PER_SEC;
    std::cout<<"4) OpenCV - parallel_for"<<std::endl<<"Time: "<<diff<<std::endl;

    for(int i = 0 ; i < Ninter ; i++)
        sum3 += OpenMP_2(src3);
    std::cout<<"5) OpenMP_2"<<std::endl<<"Time: "<<sum3<<std::endl;

    begin = clock();
    for(int i = 0 ; i < Ninter ; i++)
        std::for_each(src4.begin<cv::Vec3b>(), src4.end<cv::Vec3b>(), [](cv::Vec3b& pixel)
        {
            pixel.val[2] = 255;
        });
    end = clock();
    diff = double(end - begin) / CLOCKS_PER_SEC;
    std::cout<<"6) Std::for_Each"<<std::endl<<"Time: "<<diff<<std::endl;

/*
    cv::imshow("Ori",original);
    cv::imshow("RedMatrix_OpenCV",FullRedOpenCV_1);
    cv::imshow("RedMatrix_OpenCV_2",FullRedOpenCV_2);
    cv::imshow("TBB",src);
    cv::imshow("Parallel_for_",src2);
    cv::imshow("OpenMP_2",src3);
    cv::imshow("Std::ForEach",src4);
*/

    cv::waitKey(0);


    return 0;
}
