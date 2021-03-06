#include <iostream>
#include <windows.h>
#include <stdlib.h> 
#include <stdio.h> 
#include <opencv2/opencv.hpp> 

using namespace std;
using namespace cv;

IplImage* image = 0;
IplImage* gray = 0;
IplImage* bin = 0;
IplImage* dst = 0;
IplImage* color_dst = 0;

void printMenu()
{
	SetConsoleCP(1251);
	SetConsoleOutputCP(1251);
	cout << endl; cout << "\t======================= МЕНЮ =======================" << endl;
	cout << "\t\t\t" << "[1]" << " - Контуры на изображении — cvFindContours()" << endl;
	cout << "\t\t\t" << "[2]" << " - Прямые линии на изображении — cvHoughLines2()" << endl;
	cout << "\t\t\t" << "[3]" << " - Окружности на изображении — cvHoughCircles()" << endl;
	cout << "\t\t\t" << "[0]" << " - ВЫХОД" << endl;
}

void findContours()
{
	image = cvLoadImage("img.jpg", 1);
	// клонируем
	dst = cvCloneImage(image);

	// создаём одноканальные картинки
	gray = cvCreateImage(cvGetSize(image), IPL_DEPTH_8U, 1);
	bin = cvCreateImage(cvGetSize(image), IPL_DEPTH_8U, 1);
	// клонируем
	dst = cvCloneImage(image);
	// окно для отображения картинки
	cvNamedWindow("original", CV_WINDOW_NORMAL);
	cvNamedWindow("binary", CV_WINDOW_NORMAL);
	cvNamedWindow("contours", CV_WINDOW_NORMAL);

	// преобразуем в градации серого
	cvCvtColor(image, gray, CV_RGB2GRAY);

	// преобразуем в двоичное
	cvInRangeS(gray, cvScalar(40), cvScalar(150), bin); // atoi(argv[2])

	CvMemStorage* storage = cvCreateMemStorage(0);
	CvSeq* contours = 0;

	// находим контуры
	int contoursCont = cvFindContours(bin, storage, &contours, sizeof(CvContour), CV_RETR_LIST, CV_CHAIN_APPROX_SIMPLE, cvPoint(0, 0));

	cout << "Количество контуров: " << contoursCont << endl;

	// нарисуем контуры
	for (CvSeq* seq0 = contours; seq0 != 0; seq0 = seq0->h_next) {
		cvDrawContours(dst, seq0, CV_RGB(255, 216, 0), CV_RGB(0, 0, 250), 0, 1, 8); // рисуем контур
	}

	// показываем картинки
	cvShowImage("original", image);
	cvShowImage("binary", bin);
	cvShowImage("contours", dst);
}

void findLines()
{
	// получаем картинку (в градациях серого)
	image = cvLoadImage("img.jpg", CV_LOAD_IMAGE_GRAYSCALE);

	// хранилище памяти для хранения найденных линий
	CvMemStorage* storage = cvCreateMemStorage(0);
	CvSeq* lines = 0;
	int i = 0;

	dst = cvCreateImage(cvGetSize(image), 8, 1);
	color_dst = cvCreateImage(cvGetSize(image), 8, 3);

	// детектирование границ
	cvCanny(image, dst, 50, 200, 3);

	// конвертируем в цветное изображение
	cvCvtColor(dst, color_dst, CV_GRAY2BGR);

	// нахождение линий
	lines = cvHoughLines2(dst, storage, CV_HOUGH_PROBABILISTIC, 1, CV_PI / 180, 50, 50, 10);

	// нарисуем найденные линии
	for (i = 0; i < lines->total; i++) {
		CvPoint* line = (CvPoint*)cvGetSeqElem(lines, i);
		cvLine(color_dst, line[0], line[1], CV_RGB(255, 0, 0), 3, CV_AA, 0);
	}

	// показываем
	cvNamedWindow("original", CV_WINDOW_NORMAL);
	cvShowImage("original", image);

	cvNamedWindow("lines", CV_WINDOW_NORMAL);
	cvShowImage("lines", color_dst);
}

void findCircles()
{
	// получаем картинку (в градациях серого)
	image = cvLoadImage("eritrocit.jpg", CV_LOAD_IMAGE_GRAYSCALE);
	// клонируем
	dst = cvCloneImage(image);

	// хранилище памяти для кругов
	CvMemStorage* storage = cvCreateMemStorage(0);
	// сглаживаем изображение
	cvSmooth(image, image, CV_GAUSSIAN, 5, 5);
	// поиск кругов
	CvSeq* results = cvHoughCircles(
		image,
		storage,
		CV_HOUGH_GRADIENT,
		4,
		image->width / 25
	);
	// пробегаемся по кругам и рисуем их на оригинальном изображении
	for (int i = 0; i < results->total; i++) {
		float* p = (float*)cvGetSeqElem(results, i);
		CvPoint pt = cvPoint(cvRound(p[0]), cvRound(p[1]));
		cvCircle(dst, pt, cvRound(p[2]), CV_RGB(0xff, 0, 0));
	}
	
	cout << "Количество кругов: " << results->total << endl;

	// показываем
	cvNamedWindow("original", CV_WINDOW_NORMAL);
	cvShowImage("original", image);

	cvNamedWindow("circles", CV_WINDOW_NORMAL);
	cvShowImage("circles", dst);
}

int main()
{
	printMenu();
	int choose = 0;
	for (;;)
	{
		cout << "\t\t\tВвод: "; cin >> choose;
		switch (choose)
		{
		case 0: return 0;
		case 1:
		{
			findContours();
		} break;
		case 2:
		{
			findLines();
		} break;
		case 3:
		{
			findCircles();
		} break;
		default:
			cout << "\t\t\t|| [" << choose << "] - неизвестная команда ||" << endl; break;
		}
		waitKey();
		destroyAllWindows();
	}
	return 0;
}