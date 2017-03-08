#include "mainwindow.h"
#include <QApplication>

#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/opencv.hpp"
#include "opencv2/ml/ml.hpp"
#include "opencv2/core/core.hpp"
#include <opencv2/ml/ml.hpp>

using namespace cv;
/*
    cv::normalize(csvContent.colRange(0,1), csvContent.colRange(0,1), 0, 255, NORM_MINMAX, CV_8UC1);
    cv::normalize(csvContent.colRange(1,2), csvContent.colRange(1,2), 0, 255, NORM_MINMAX, CV_8UC1);
*/
int main(int argc, char *argv[])
{
    const char* filename = "data.csv";

    cv::TrainData data;
    data.read_csv(filename);
    CvTrainTestSplit cvtts(0.5f, true);

    data.set_train_test_split(&cvtts);
    data.set_response_idx(2);
    cv::Mat X,Y, X_Test,Y_Test, X_Norm,X_Norm1,X_Norm2;

    X = data.get_values();
    X = X.colRange(0,X.cols-1);
    X_Test = data.get_test_sample_idx();
//    cv::normalize(X.colRange(0,1), X_Norm1, 0, 255, NORM_MINMAX, CV_8UC1);
//    cv::normalize(X.colRange(1,2), X_Norm2, 0, 255, NORM_MINMAX, CV_8UC1);
//    cv::hconcat(X_Norm1,X_Norm2,X_Norm);
    Y = data.get_responses();


    CvKNearest KNN;
    KNN.train(X,Y);

        int width = 512, height = 512;
        Mat image = Mat::zeros(height, width, CV_8UC3);

        Vec3b green(0,255,0), blue (255,0,0);
        for (int i = 0; i < image.rows; ++i)
                for (int j = 0; j < image.cols; ++j)
                {

                    Mat sampleMat = (Mat_<float>(1,2) << j,i);
                    float response = KNN.find_nearest(sampleMat,5);

                    if (response == 1)
                        image.at<Vec3b>(i,j)  = green;
                    else if (response == 0)
                         image.at<Vec3b>(i,j)  = blue;
                }


            imshow("Knn", image); // show it to the user
             waitKey(0);


    //file.set_train_test_split(&cvtts);

//    cv::Mat zzero = cv::Mat::zeros(response.rows,response.cols,response.type());
//    int width = 512, height = 512;
//    Mat image = Mat::zeros(height, width, CV_8UC3);

//    Vec3b green(0,255,0), blue (255,0,0);
//    CvKNearest knn;
//    knn.train(csvContent,response);

//    for (int i = 0; i < image.rows; ++i)
//            for (int j = 0; j < image.cols; ++j)
//            {
//                Mat sampleMat = (Mat_<float>(1,2) << j,i);
//                float response = knn.find_nearest(sampleMat,5);

//                if (response == 1)
//                    image.at<Vec3b>(i,j)  = green;
//                else if (response == -1)
//                     image.at<Vec3b>(i,j)  = blue;
//            }
//    // Show the training data
//    int thickness = -1;
//    int lineType = 8;

////    circle( image, Point(501,  10), 5, Scalar(  0,   0,   0), thickness, lineType);
////    circle( image, Point(255,  10), 5, Scalar(255, 255, 255), thickness, lineType);
////    circle( image, Point(501, 255), 5, Scalar(255, 255, 255), thickness, lineType);
////    circle( image, Point( 10, 501), 5, Scalar(255, 255, 255), thickness, lineType);


////    imshow("Knn", image); // show it to the user
////     waitKey(0);



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

