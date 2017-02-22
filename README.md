# OpenCV-Test
This repository will contain a several test and comparations with the OpenCV (Version 2.4.11) library

### Table of contents

1. [Repository Tree](#Repository Tree)
2. [Copy Mat](#Copy Mat)

### Repository Tree

~~~
OpenCV-Test
|
*--> CopyMat
	|
	+--> lena.bmp
	|
	+--> CopyMat.cpp
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
