#ifndef MEDIANFILTER_H_INCLUDED
#define MEDIANFILTER_H_INCLUDED
#define Gpiexel(image,i,j)  ((uchar *) (((image).data)+(j)*((image).step)))[(i)*((image).channels())]

#include <iostream>
#include <string.h>
#include <set>
#include <chrono>
#include <opencv2/core/core.hpp>
#include <opencv2/opencv.hpp>
//#include <opencv2/highgui/highgui.hpp>

using namespace cv;
using namespace std;
using namespace chrono;

Mat MedianFilter (const Mat &src_img, Mat &dst);
#endif // MEDIANFILTER_H_INCLUDED

#define GETSTART_TIME    { starttime = system_clock::now();    }

#define GETENDING_TIME(y)     {  endtime = system_clock::now();  y = y + std::chrono::duration_cast<std::chrono::microseconds>(endtime - starttime).count();   }
