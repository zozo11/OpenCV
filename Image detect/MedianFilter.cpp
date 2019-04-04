#include "MedianFilter.h"
#if 0
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
	{
        for(int j = 0;j < img.cols;j++){
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
        }
    }
	_median_img.copyTo(median_img);
    return median_img;
}

#endif

void MedianFilter (const Mat &src_img, Mat &median_img)
{
	Mat _median_img(src_img.size(), src_img.type());
	//_median_img.create(src_img.size(), CV_8UC3);
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

                    Gpiexel(_median_img,j,i) = arr[4];
                } else {
                    Gpiexel(_median_img,j,i) = 0;
                }
            }
        }
    }
//	_median_img.copyTo(median_img);
    return ;
}
