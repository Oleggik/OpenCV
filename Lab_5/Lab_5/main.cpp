#include <iostream>
#include <opencv2/opencv.hpp>
#include "opencv2/highgui/highgui.hpp"
#include "stdio.h"
#include "conio.h"
#include  <stdlib.h>
#include <math.h>
#include <windows.h>

using namespace std;
using namespace cv;

//1
Mat src, src_gray;
int thresh = 200;
int max_thresh = 255;
RNG rng(12345);

void printMenu()
{
	SetConsoleCP(1251);
	SetConsoleOutputCP(1251);
	cout << "\t\t\t[1] - Детектора углов Харриса" << endl;
	cout << "\t\t\t[2] - Детектора углов Ши Томаси" << endl;
	cout << "\t\t\t[3] - Функция перспективных преобразований getPerspectiveTransform()" << endl;
	cout << "\t\t\t[4] - Выход " << endl;
}

void harrisAngleDetector()
{
	Mat src, gray, gray1;
	int blockSize = 3;
	int apertureSize = 7;
	double k = 0.1;

	// Load source image and convert it to gray
	src = imread("books.jpg", 1);
	cvtColor(src, gray, CV_BGR2GRAY);
	Mat dst, dst_norm, dst_norm_scaled;
	dst = Mat::zeros(src.size(), CV_32FC1);
	// Detecting corners
	cornerHarris(gray, dst, blockSize, apertureSize, k, BORDER_DEFAULT);
	//cornerHarris(gray, dst, 7, 5, 0.5, BORDER_DEFAULT);

	normalize(dst, dst_norm, 0, 255, NORM_MINMAX, CV_32FC3, Mat());
	convertScaleAbs(dst_norm, dst_norm_scaled);
	// Drawing a circle around corners
	for (int j = 0; j < dst_norm.rows; j++)
	{
		for (int i = 0; i < dst_norm.cols; i++)
		{
			if ((int)dst_norm.at<float>(j, i) > thresh)
			{
				circle(dst_norm_scaled, Point(i, j), 5, Scalar(0), 2, 8, 0);
			}
		}
	}
	// Showing the result
	namedWindow("corners_window", CV_WINDOW_AUTOSIZE);
	imshow("corners_window", dst_norm_scaled);
	namedWindow("first", CV_WINDOW_AUTOSIZE);
	imshow("first", src);
}

void shiTomasiAngleDetector()
{
	Mat src, gray, gray1;
	int blockSize = 3, gradientSize = 3;;
	int apertureSize = 3;
	double k = 0.04;
	vector<Point2f> corners;
	int maxCorners = 23;
	double qualityLevel = 0.01;
	double minDistance = 10;

	// Load source image and convert it to gray
	src = imread("books.jpg", 1);
	Mat copy = src.clone();
	cvtColor(src, gray, CV_BGR2GRAY);
	int radius = 4;
	goodFeaturesToTrack(gray, corners, maxCorners, qualityLevel, minDistance, Mat(), blockSize, gradientSize, false, 0.04);
	for (size_t i = 0; i < corners.size(); i++) {
		circle(copy, corners[i], radius, Scalar(rng.uniform(0, 255), rng.uniform(0, 256), rng.uniform(0, 256)), FILLED);
	}

	namedWindow("goodFeaturesToTrack");
	imshow("goodFeaturesToTrack", copy);
}

void perspectiveTransform()
{
	// Input Quadilateral or Image plane coordinates
	Point2f inputQuad[4];
	// Output Quadilateral or World plane coordinates
	Point2f outputQuad[4];

	// Lambda Matrix
	Mat lambda(2, 4, CV_32FC1);
	//Input and Output Image;
	Mat input, output;

	//Load the image
	input = imread("books.jpg", 1);
	// Set the lambda matrix the same type and size as input
	lambda = Mat::zeros(input.rows, input.cols, input.type());

	// The 4 points that select quadilateral on the input , from top-left in clockwise order
	// These four pts are the sides of the rect box used as input 
	inputQuad[0] = Point2f(-30, -60);
	inputQuad[1] = Point2f(input.cols + 50, -50);
	inputQuad[2] = Point2f(input.cols + 100, input.rows + 50);
	inputQuad[3] = Point2f(-50, input.rows + 50);
	// The 4 points where the mapping is to be done , from top-left in clockwise order
	outputQuad[0] = Point2f(0, 0);
	outputQuad[1] = Point2f(input.cols - 100, 100);
	outputQuad[2] = Point2f(input.cols - 100, input.rows - 100);
	outputQuad[3] = Point2f(0, input.rows - 100);

	// Get the Perspective Transform Matrix i.e. lambda 
	lambda = getPerspectiveTransform(inputQuad, outputQuad);
	// Apply the Perspective Transform just found to the src image
	warpPerspective(input, output, lambda, output.size());

	//Display input and output
	imshow("Input", input);
	imshow("Output", output);
}

int main()
{
	int choose = 0;
	printMenu();
	for (;;)
	{
		cout << "\t\t\tВвод: "; cin >> choose;
		switch (choose)
		{
		case 1:
		{
			harrisAngleDetector();
			break;
		}
		case 2:
		{
			shiTomasiAngleDetector();
			break;
		}
		case 3:
		{
			perspectiveTransform();
			break;
		}
		case 4:
		{
			return (0);
		}
		}
		waitKey();
		destroyAllWindows();
	}
}