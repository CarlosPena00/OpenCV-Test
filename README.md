# OpenCV-Test
This repository will contain a several test and comparations with the OpenCV (Version 2.4.11) library

### Table of contents

1. [Repository Tree](#Repository Tree)
2. [Copy Mat](#Copy Mat)
3. [MatAcess]

### Repository Tree

~~~
OpenCV-Test
|
*--> CopyMat
	|
	+--> lena.bmp
	|
	+--> CopyMat.cpp
|
*--> MatAcess
	|
	+--> MatAcess.cpp
~~~

### Copy Mat

For that I search 5 options

|- CopyTo

|- Clone

|- Operator =

|- cv::Mat foo()

|- cv::Mat& foo()

A) **CopyTo**

If it does not have a proper size or type before the operation, it is reallocated.
```c++
	src.copyTo(dst);
```
Time for 100k operations: 16.866

B) **Clone**

The method creates a full copy of the array
```c++
	dst = src.clone();
```
Time for 100k operations: 27.9904

C) **Operator =**

Matrix assignment is an O(1) operation. This means that no data is copied but the data is shared and the reference counter
```c++
	dst = src;
```
Time for 100k operations: 0.004868

D) **cv::Mat foo()**

```c++
	cv::Mat foo()return src;
```
Time for 100k operations: 0.009247


E) **cv::Mat &foo()**

```c++
	cv::Mat& foo()return src;
```
Time for 100k operations: 0.003784


### MatAcess

int Size = 500;
int Ninter = 10000;

1) src.at<cv::Vec3b>(j,i)
Time : 17.5593

2) cv::Vec3b* row = src.ptr<cv::Vec3b>(j)
Time: 7.76744

3) TTB - parallel_for
Time: 18.8249

4) OpenCV - parallel_for
Time: 8.47706

5) OpenMP_2
Time: 7.1739

6) Std::for_Each
Time: 39.0343

#### With -03

1) src.at<cv::Vec3b>(j,i)
Time : 2.38472

2) cv::Vec3b* row = src.ptr<cv::Vec3b>(j)
Time: 1.2344

3) TTB - parallel_for
Time: 3.4527

4) OpenCV - parallel_for
Time: 3.55384

5) OpenMP_2
Time: 1.23095

6) Std::for_Each
Time: 6.17241

