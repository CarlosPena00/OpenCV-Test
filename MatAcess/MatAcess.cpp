//g++ MatAcess.cpp -o a `pkg-config --cflags opencv --libs`


#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/opencv.hpp"
#include <string>
#include <sstream>
#include <ctime>
#include <tbb/tbb.h>
using namespace cv;
using namespace std;
using namespace tbb;

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
            src.at<cv::Vec3b>(j,i) = cv::Vec3b(holder[0], holder[1], 255);
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


class parallel_pixel
{
private:
    uchar *p ;
public:
    parallel_pixel(uchar *ptr ) : p(ptr) { }

    void operator() ( const blocked_range<int>& r ) const
    {
        for ( int i = r.begin(); i != r.end(); ++i ) {
            p[(i*3)+2] = (uchar)255  ;
        }
    }
} ;
// Thanks to http://answers.opencv.org/question/22115/best-way-to-apply-a-function-to-each-element-of-mat/

int DIM = 2000;
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

    task_scheduler_init init;
    uchar* p2 = src.data ;

    int nElements = src.cols*src.rows;
    clock_t begin = clock();

    for(int i = 0 ; i < 1000 ; i ++)
        parallel_for(blocked_range<int>(0, nElements, 800), parallel_pixel(p2) ) ;

    clock_t end = clock();
    double diff = double(end - begin) / CLOCKS_PER_SEC;
    std::cout<<"TTB: "<<diff<<std::endl;

    cv::imshow("Ori",original);
    cv::imshow("FullRedMatrix_OpenCV",FullRedOpenCV_1);
    cv::imshow("FullRedMatrix_OpenCV_2",FullRedOpenCV_2);
    cv::imshow("TBB",src);

    cv::waitKey(0);


    return 0;
}


/*
Matrix 2000 x 2000

1) src.at<cv::Vec3b>(j,i)

Time: 88.9919

2)cv::Vec3b* row = src.ptr<cv::Vec3b>(j)

Time: 58.9217

3) TTB - parallel_for

Time: 28.1236

*/