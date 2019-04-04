
#include <cv.h>
#include <cxcore.h>
#include <iostream>
#include <stdio.h>
#include <opencv2/core/core.hpp>
#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgcodecs.hpp>

using namespace std;
using namespace cv;
//using namespace chrono;

//define  pixelGrey( image , x , y ) image.data[ image.step[0] ✯ y+image.step[1] ✯ x]


int main(int argc, char** argv)
{

     //声明IplImage指针
    IplImage* pImg = NULL;
    IplImage* pContourImg = NULL;
    CvMemStorage * storage = cvCreateMemStorage(0);
    CvSeq * contour = 0;
    int mode = CV_RETR_EXTERNAL;// CV_RETR_EXTERNAL
    unsigned th=0;
    if( argc == 3)
        if(strcmp(argv[2], "all") == 0)
            mode = CV_RETR_CCOMP; //内外轮廓都检测
    //创建窗口
    cvNamedWindow("src", 1);
    cvNamedWindow("contour",1);
    //载入图像，强制转化为Gray
    if( argc >= 2 && (pImg = cvLoadImage( argv[1], 0)) != 0 )
    {
        cvShowImage( "src", pImg );
        //为轮廓显示图像申请空间
        //3通道图像，以便用彩色显示
        pContourImg = cvCreateImage(cvGetSize(pImg),IPL_DEPTH_8U,3);
        //copy source image and convert it to BGR image
        cvCvtColor(pImg, pContourImg, CV_GRAY2BGR);
        cvCanny(pImg, pImg, 50, 150, 5);
        int Num=cvFindContours( pImg, storage, &contour, sizeof(CvContour), mode, CV_CHAIN_APPROX_SIMPLE);
        std::cout<<"The number of Contours is:"<<Num<<std::endl;
        for(;contour!=0;contour=contour->h_next)
        {
            printf("***************************************************\n");
            for(int i=0;i<contour->total;i++)
            {
                CvPoint* p=(CvPoint*)cvGetSeqElem(contour,i);
                printf("p->x=%d,p->y=%d\n",p->x,p->y);
            }
            //将轮廓画出
            cvDrawContours(pContourImg, contour, CV_RGB(255,0,0), CV_RGB(0, 255, 0), 0, 2, 0);
        }
    }
    else
    {
        //销毁窗口
        cvDestroyWindow( "src" );
        cvDestroyWindow( "contour" );
        cvReleaseMemStorage(&storage);
        return -1;
    }
    //显示图像
    cvShowImage( "contour", pContourImg );
    cvWaitKey(0);
    //销毁窗口
    cvDestroyWindow( "src" );
    cvDestroyWindow( "contour" );
    //释放图像
    cvReleaseImage( &pImg );
    cvReleaseImage( &pContourImg );
    cvReleaseMemStorage(&storage);
    return 0;
}
