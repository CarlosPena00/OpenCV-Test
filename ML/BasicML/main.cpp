#include "mainwindow.h"
#include <QApplication>

#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/opencv.hpp"
#include "opencv2/ml/ml.hpp"
#include "opencv2/core/core.hpp"
#include <opencv2/ml/ml.hpp>
//using namespace ml
using namespace cv;

int main(int argc, char *argv[])
{
    cv::Mat normCsvContent,csvContent,response;
    CvMLData file;
    const char* filename = "data.csv";
    file.read_csv(filename);
    file.set_response_idx(2);

    CvTrainTestSplit cvtts(0.75f, true);
    //file.set_train_test_split(&cvtts);
    csvContent = file.get_values();
    csvContent = csvContent.colRange(0,csvContent.cols-1);
    response = file.get_responses();
    cv::Mat zzero = cv::Mat::zeros(response.rows,response.cols,response.type());
    int width = 512, height = 512;
    Mat image = Mat::zeros(height, width, CV_8UC3);

    Vec3b green(0,255,0), blue (255,0,0);
    CvKNearest knn;
    knn.train(csvContent,response);

    for (int i = 0; i < image.rows; ++i)
            for (int j = 0; j < image.cols; ++j)
            {
                Mat sampleMat = (Mat_<float>(1,2) << j,i);
                float response = knn.find_nearest(sampleMat,5);

                if (response == 1)
                    image.at<Vec3b>(i,j)  = green;
                else if (response == -1)
                     image.at<Vec3b>(i,j)  = blue;
            }
    // Show the training data
    int thickness = -1;
    int lineType = 8;
    circle( image, Point(501,  10), 5, Scalar(  0,   0,   0), thickness, lineType);
    circle( image, Point(255,  10), 5, Scalar(255, 255, 255), thickness, lineType);
    circle( image, Point(501, 255), 5, Scalar(255, 255, 255), thickness, lineType);
    circle( image, Point( 10, 501), 5, Scalar(255, 255, 255), thickness, lineType);


    imshow("Knn", image); // show it to the user
     waitKey(0);



    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    return a.exec();
}

//    csvContent = file.get_values();
//    csvContent.rowRange(1,csvContent.rows).convertTo(csvContent,CV_64FC1);
//    csvContent = csvContent.colRange(0,csvContent.cols-1);
//    response = csvContent.col(csvContent.cols-1);

//Mat trainingDataMat(4, 2, CV_32FC1, trainingData);
//  Mat labelsMat(4, 1, CV_32FC1, labels);
//Both input and output vectors/values are passed as matrices
//the input feature vectors are stored as train_data rows
// tflag=CV_ROW_SAMPLE The feature vectors are stored as rows.
//all the components (features) of a training vector are stored continuously
// float labels[4] = {1.0, -1.0, -1.0, -1.0};
// float trainingData[4][2] = { {501, 10}, {255, 10}, {501, 255}, {10, 501} };

// Mat labelsMat = file.get_values();
// Mat trainingDataMat = file.get_responses();

