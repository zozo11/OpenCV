/*********************************************************************************************
 * 159731 Assignment 1 Zoe Wang ID 18042783
 * compile with:
 * g++ -std=c++11 main.cpp -o main `pkg-config --cflags --libs opencv`
*********************************************************************************************/
#include <iostream>
#include <string.h>
#include <set>
#include <map>
#include <chrono>
#include <opencv2/core/core.hpp>
#include <opencv2/opencv.hpp>
#define Gpiexel(image,i,j)  ((uchar *) (((image).data)+(j)*((image).step)))[(i)*((image).channels())]
#define GETSTART_TIME    { starttime = system_clock::now();    }
#define GETENDING_TIME(y)     {  endtime = system_clock::now();  y = y + std::chrono::duration_cast<std::chrono::microseconds>(endtime - starttime).count();   }
#define PERFORMANCE_NEW_CODES
/*in(32)  use high 16bit means the total counter of pixels, the low 16bits means the object ID*/
#define     PIXELNUMADD(s1,s2, newnumber)   {  mergeset[s1] =  mergeset[s2] +  newnumber<<16 }
#define     THEVALIDE_NUMBER      50
#define     CACULATE(c,r)        ((c<< 16)+r)

using namespace cv;
using namespace std;
using namespace chrono;

/*Prototype*/
int BlobCounter(Mat img);
Mat MedianFilter (const Mat &src_img);

/*Attribute*/
int looptimes = 0;
double colortime =0,colortime1 = 0;
Mat src_img, grey_img, binary_img, median_img;

int main(int argc, char** argv)
{

    if(argc < 2 )
    {
        VideoCapture cap;
        cap.open(0);
        if (!cap.isOpened())
        if (!cap.isOpened())
            {
                cout << "Failed to open camera" << endl;
                return 0;
            }
           cout << "Opened camera" << endl;
        namedWindow("WebCam", 1);
        cap.set(CV_CAP_PROP_FRAME_WIDTH, 640);
        cap.set(CV_CAP_PROP_FRAME_HEIGHT, 480);

        float fps=0.0;
        /*add the debug codes*/
        double colortime =0, filtertime = 0, threhold_time =0, blobtime = 0;
        int looptimes = 0;
        system_clock::time_point starttime, endtime;
        while (waitKey(30)<0){

            system_clock::time_point start = system_clock::now();
            cap >> src_img;
            if( src_img.empty() )
                break;

            //Make grey image
            cvtColor(src_img, grey_img, CV_BGR2GRAY);
            //Median filter image
            median_img = MedianFilter(grey_img);
            //Make binary image
            threshold(median_img, binary_img, 0, 255, cv::THRESH_OTSU);
            //Counter number of elements on image
            int cont = 0;
            cont = BlobCounter(binary_img);
            char printit[100];
            sprintf(printit,"%d %2.1f",cont,fps);
            putText(binary_img, printit, cvPoint(10,30), FONT_HERSHEY_PLAIN, 2, cvScalar(255,255,255), 2, 8);
            imshow("WebCam", binary_img);

            system_clock::time_point end = system_clock::now();
            double seconds = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();

            fps = 1000000/seconds;
        }
    }
    else
    {
            src_img = imread(argv[1], CV_LOAD_IMAGE_COLOR);
            cvtColor(src_img, grey_img, CV_BGR2GRAY);

            namedWindow("Source image", WINDOW_AUTOSIZE);
            imshow("Source image", src_img);
            //Median filter image
            median_img = MedianFilter(grey_img);
            median_img = MedianFilter(median_img);
            namedWindow("Median filter image", WINDOW_AUTOSIZE);
            imshow("Median filter image", median_img);
            //Image binary segmentation
            threshold(median_img, binary_img, 0, 255, cv::THRESH_OTSU);
            //Image counter function
            int countr = 0;
            countr = BlobCounter(binary_img);
            //Show Text on image
            cout<<"There are "<<countr<<" items on image"<<endl;

            char text[100];
            sprintf(text, "%d", countr);
            putText(binary_img, text, cvPoint(10, 30), FONT_HERSHEY_PLAIN, 2, cvScalar(255, 255, 255), 2, 8);

            namedWindow("Binary image", WINDOW_AUTOSIZE);
            imshow("Binary image", binary_img);

            waitKey(0);

    }
    return 0;
}



//Median Filter function
Mat MedianFilter (const Mat &src_img)
{
    Mat median_img(src_img.size(), src_img.type());
    uchar arr[9] = { 0 };
	for(int i=0;i<src_img.rows;++i) {
        int indexoffset = 0;
        for (int j=0; j < src_img.cols; ++j) {
            indexoffset = j%3;
            if ((i - 1) > 0 && (i + 1) < src_img.rows && (j - 1) > 0 && (j + 1) < src_img.cols) {
/*
col 0
Gpiexel(src_img,j-1,i-1)
Gpiexel(src_img,j-1,i)
Gpiexel(src_img,j-1,i+1)

col 1

Gpiexel(src_img,j,i)
Gpiexel(src_img,j,i+1)

col 2
Gpiexel(src_img,j+1,i-1)
Gpiexel(src_img,j+1,i)
Gpiexel(src_img,j+1,i+1)

*/
                    if(j == 2) {
                        arr[0] =  Gpiexel(src_img,j-1,i-1);
                        arr[3] =  Gpiexel(src_img,j-1,i);
                        arr[6] =  Gpiexel(src_img,j-1,i+1);

                        arr[1] =  Gpiexel(src_img,j,i-1);
                        arr[4] =  Gpiexel(src_img,j,i);
                        arr[7] =  Gpiexel(src_img,j,i+1);

                        arr[2] = Gpiexel(src_img,j+1,i-1);
                        arr[5] = Gpiexel(src_img,j+1,i);
                        arr[8] = Gpiexel(src_img,j+1,i+1);
                    }else {
                        arr[indexoffset + 0] =  Gpiexel(src_img,j+1,i-1);
                        arr[indexoffset + 3] =  Gpiexel(src_img,j+1,i);
                        arr[indexoffset + 6] =  Gpiexel(src_img,j+1,i+1);
                    }
                    /*need to update*/
                    for (int gap = 9 / 2; gap > 0; gap /= 2)//Shell's Sort
                        for (int i = gap; i < 9; ++i)
                            for (int j = i - gap; j >= 0 && arr[j] > arr[j + gap]; j -= gap)
                                swap(arr[j], arr[j + gap]);

                    Gpiexel(median_img,j,i) = arr[4];
                } else {
                    Gpiexel(median_img,j,i) = 0;
                }
            }
        }
	//_median_img.copyTo(median_img);
    return median_img;
}

int BlobCounter(Mat img)
{
    int counter = -1, s1, s2;
    array<set<int>,10000> ctSet;
    set<int>::iterator itr;
    vector<vector<int>> Avct(img.cols, vector<int>(img.rows, -1));
    //system_clock::time_point starttime, endtime;

    for(int y=1; y<img.rows; ++y)
    {
        for(int x=1; x<img.cols; ++x)
        {
            if(Gpiexel(img,x,y) !=0)
            {
                if((Gpiexel(img,x-1,y)!=0) || (Gpiexel(img,x,y-1)!=0))
                {
                    s1 = Avct[x-1][y];
                    s2 = Avct[x][y-1];

                    if(s1!=-1)
                    {
                        ctSet[s1].insert(CACULATE(x,y));
                        Avct[x][y] = s1;
                    } else {
                        if(s2!=-1)
                        {
                            ctSet[s2].insert(CACULATE(x,y));
                            Avct[x][y] = s2;
                        }
                    }

                    if((s1!=s2) && (s1!=-1) && (s2!=-1))
                    {
                        //GETSTART_TIME
                        for(itr=ctSet[s2].begin(); itr!=ctSet[s2].end(); itr++)
                        {
                            int i =  *itr;
                            int cl = (i&0xffff0000) >> 16;
							int rw = (i&0xffff) ;
                            ctSet[s1].insert(CACULATE(cl,rw));
                            Avct[cl][rw] = s1;
                        }
                        ctSet[s2].clear();
                        //GETENDING_TIME(colortime1)
                    }
                }else{
                    //GETSTART_TIME
                    counter = counter + 1;
                    if(counter >= 10000)
                        return  -1;
                    ctSet[counter].insert(CACULATE(x,y));
                    Avct[x][y] = counter;
                    //GETENDING_TIME(colortime1)
                }
            }
        }
    }
    int cont = 0;
    for(int i = 1; i<=counter; i++)
    {
        if(ctSet[i].size() > 50){
            cont += 1;
        }
    }

    /*if(looptimes++ > 50 ){
        cout << "colortime: " << colortime << "colortime1:" << colortime1 << endl;
    }*/
    return cont;
}

