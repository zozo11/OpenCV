#include <iostream>
#include <string.h>
#include <set>
#include <chrono>
#include <opencv2/core/core.hpp>
#include <opencv2/opencv.hpp>
//#include <opencv2/highgui/highgui.hpp>
#define Gpiexel(image,i,j)  ((uchar *) (((image).data)+(j)*((image).step)))[(i)*((image).channels())]
using namespace cv;
using namespace std;
using namespace chrono;

//#include "MedianFilter.h"
//#include "BlurbCounter.h"

/*********************************************************************************************
 * compile with:
 * g++ -std=c++11 main.cpp MedianFilter.cpp BlurbCounter.cpp -o main `pkg-config --cflags --libs opencv`
 * g++ -std=c++11 main.cpp -o main `pkg-config --cflags --libs opencv`
*********************************************************************************************/
int BlobCounter(Mat img);
Mat MedianFilter (const Mat &src_img, Mat &dst);

int main(int argc, char** argv)
{
    Mat src_img, grey_img, binary_img, Counter_img, median_img, median_img2;
    if(argc < 2 )
    {
        VideoCapture cap;
        cap.open(0);
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
        //double colortime =0, filtertime = 0, threhold_time =0, blobtime = 0;
        //int looptimes = 0;
        //system_clock::time_point starttime, endtime;
        while (waitKey(30)<0){

            system_clock::time_point start = system_clock::now();
            cap >> src_img;
            if( src_img.empty() )
                break;



          //  GETSTART_TIME
            //Make grey image
            cvtColor(src_img, grey_img, CV_BGR2GRAY);
            //GETENDING_TIME(colortime)
            //Median filter image
            //GETSTART_TIME
            median_img = MedianFilter(grey_img, median_img);
            //GETENDING_TIME(filtertime)
            //Make binary image
            //GETSTART_TIME
            threshold(median_img, binary_img, 0, 255, cv::THRESH_OTSU);
            //GETENDING_TIME(threhold_time)
            //Counter number of elements on image
            int cont = 0;
            //GETSTART_TIME
            cont = BlobCounter(binary_img);
            //GETENDING_TIME(blobtime)
            char printit[100];
            sprintf(printit,"%d %2.1f",cont,fps);
            putText(binary_img, printit, cvPoint(10,30), FONT_HERSHEY_PLAIN, 2, cvScalar(255,255,255), 2, 8);
            imshow("WebCam", binary_img);
            /*if(looptimes++ > 50 ){
                cout << "colortime: " << colortime << " filtertime: " << filtertime << "  threhold_time: " << threhold_time << " blobtime: " << blobtime << endl;
            }*/
            system_clock::time_point end = system_clock::now();
            double seconds = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();

            fps = 1000000/seconds;
           // cout << "frames " << fps << " seconds " << seconds << endl;
        }
    }
    else
    {
            src_img = imread(argv[1], CV_LOAD_IMAGE_COLOR);
            cvtColor(src_img, grey_img, CV_BGR2GRAY);

            namedWindow("Source image", WINDOW_AUTOSIZE);
            imshow("Source image", src_img);
            //Median filter image
            MedianFilter(grey_img, median_img);
            MedianFilter(median_img, median_img2);

            namedWindow("Median filter image", WINDOW_AUTOSIZE);
            imshow("Median filter image", median_img2);
            //Image binary segmentation
            threshold(median_img2, binary_img, 0, 255, cv::THRESH_OTSU);
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
uchar Median(uchar n1, uchar n2, uchar n3, uchar n4, uchar n5,
	uchar n6, uchar n7, uchar n8, uchar n9) {
	uchar arr[9];
	arr[0] = n1;
	arr[1] = n2;
	arr[2] = n3;
	arr[3] = n4;
	arr[4] = n5;
	arr[5] = n6;
	arr[6] = n7;
	arr[7] = n8;
	arr[8] = n9;
	for (int gap = 9 / 2; gap > 0; gap /= 2)//Shell's Sort
		for (int i = gap; i < 9; ++i)
			for (int j = i - gap; j >= 0 && arr[j] > arr[j + gap]; j -= gap)
				swap(arr[j], arr[j + gap]);
	return arr[4];//Return median value
}

Mat MedianFilter (const Mat &src_img, Mat &median_img)
{
	Mat _median_img(src_img.size(), src_img.type());
	//_median_img.create(src_img.size(), CV_8UC3);
	for(int i=0;i<src_img.rows;++i)
		for (int j=0; j < src_img.cols; ++j) {
            if ((i - 1) > 0 && (i + 1) < src_img.rows && (j - 1) > 0 && (j + 1) < src_img.cols) {

                Gpiexel(_median_img,j,i)=Median(Gpiexel(src_img,j,i), Gpiexel(src_img,j+1,i+1),
                        Gpiexel(src_img,j+1,i), Gpiexel(src_img,j,i+1), Gpiexel(src_img,j+1,i-1),
                        Gpiexel(src_img,j-1,i+1), Gpiexel(src_img,j-1,i), Gpiexel(src_img,j,i-1),
                        Gpiexel(src_img,j-1,i-1));
			}
			else
			{
                Gpiexel(_median_img,j,i)=0;

			}
		}
	_median_img.copyTo(median_img);
    return median_img;
}

//Blurb Counter
/*int looptimes = 0;
double colortime =0,colortime1 = 0;
struct compareSet
{
	int x,y;
	bool operator ()  (const ) const
	{
		return x<a.x;
	}
};
priority_queue <node> q;*/
int counter = -1, s1, s2;
set<pair<int,int>> ctSet[10000];
bool CheckPointer(int, int);


int BlobCounter(Mat img)
{
    vector<vector<int>> Avct(img.cols, vector<int>(img.rows, -1));
    //system_clock::time_point starttime, endtime;
    for(int y=1; y<img.rows; ++y)
    {
        for(int x=1; x<img.cols; ++x)
        {
            if(Gpiexel(img,x,y) !=0)
            {
                if((Gpiexel(img,x-1,y)!=0) && (Gpiexel(img,x,y-1)!=0))
                {
                    s1 = Avct[x-1][y];
                    s2 = Avct[x][y-1];

                    if(s1!=-1)
                    {
                        ctSet[s1].insert(make_pair(x,y));
                        Avct[x][y] = s1;
                    }
                    if(s2!=-1)
                    {
                        ctSet[s2].insert(make_pair(x,y));
                        Avct[x][y] = s2;
                    }
                    if(CheckPointer(s1,s2) == true){
                        blocksidmerge(s1,s2);
                    }
                    /*if((s1!=s2) && (s1!=-1) && (s2!=-1))
                    {
                        //compare s1 and s2
                        for(itr=ctSet[s1].begin(); itr!=ctSet[s1].end(); itr++)
                        {
                            for(itr=ctSet[s2].begin(); itr!=ctSet[s2].end(); itr++)
                            {
                                if(s1 == s2)
                                {
                                    cout<<"1"<<endl;
                                }else
                                {
                                    for(itr=ctSet[s2].begin(); itr!=ctSet[s2].end(); itr++)
                                    {
                                        int cl = itr->first;
                                        int rw = itr->second;
                                        ctSet[s1].insert(make_pair(cl, rw));
                                        Avct[cl][rw] = s1;
                                    }
                                    ctSet[s2].clear();
                                }

                            }
                        }
                        //GETSTART_TIME
                    for(itr=ctSet[s2].begin(); itr!=ctSet[s2].end(); itr++)
                    {
                        int cl = itr->first;
                        int rw = itr->second;
                        ctSet[s1].insert(make_pair(cl, rw));
                        Avct[cl][rw] = s1;
                    }
                    ctSet[s2].clear();
                    //GETENDING_TIME(colortime1)
                    }*/
                }/*else{
                    counter = counter + 1;
                    if(counter >= 10000)
                        return  -1;
                    ctSet[counter].insert(make_pair(x,y));

                    Avct[x][y] = counter;
                }*/
            }

        }
    }

    /*int cont = 0;
    for(int i = 1; i<counter; i++)
    {
        if(ctSet[i].size() > 50){
            cont += 1;
        }

    }*/

    /*if(looptimes++ > 50 ){
        cout << "colortime: " << colortime << "colortime1:" << colortime1 << endl;
    }*/
    //return cont;
    return 0;
}

bool CheckPointer(int s1, int s2)
{
    if((s1!=s2) && (s1!=-1) && (s2!=-1)){
        return true;
    }else {
        return false;
    }
}

bool blocksidmerge(int s1, int s2){


#if 1
    cout<<"get it "<<endl;
    //compare s1 and s2
    set<pair<int,int>>::iterator itr;

    for(itr=ctSet[s2].begin(); itr!=ctSet[s2].end(); itr++){
                int cl = itr->first;
                int rw = itr->second;
                ctSet[s1].insert(make_pair(cl, rw));
                Avct[cl][rw] = s1;
    }
    ctSet[s2].clear();
#else
/*new codes for performance*/
    static vect

    cout<<"get it "<<endl;
    //compare s1 and s2
    set<pair<int,int>>::iterator itr;
    for(itr=ctSet[s2].begin(); itr!=ctSet[s2].end(); itr++){
                int cl = itr->first;
                int rw = itr->second;
                ctSet[s1].insert(make_pair(cl, rw));
                Avct[cl][rw] = s1;
    }
    ctSet[s2].clear();
#endif
    return true;
}
