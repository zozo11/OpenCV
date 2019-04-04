#include "BlurbCounter.h"
int looptimes = 0;
double colortime =0,colortime1 = 0;

int BlobCounter(Mat img)
{
    system_clock::time_point starttime, endtime;

    int counter = -1, s1, s2;
    set<pair<int,int>> ctSet[10000];
    vector<vector<int>> Avct(img.cols, vector<int>(img.rows, -1));
    set<pair<int,int>>::iterator itr;


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
                    if((s1!=s2) && (s1!=-1) && (s2!=-1))
                    {
                        GETSTART_TIME
                        for(itr=ctSet[s2].begin(); itr!=ctSet[s2].end(); itr++)
                        {
                            int cl = itr->first;
                            int rw = itr->second;
                            ctSet[s1].insert(make_pair(cl, rw));
                            Avct[cl][rw] = s1;
                        }
                        ctSet[s2].clear();
                        GETENDING_TIME(colortime1)
                    }
                }else{
                    counter = counter + 1;
                    if(counter >= 10000)
                        return  -1;
                    ctSet[counter].insert(make_pair(x,y));

                    Avct[x][y] = counter;
                }
            }

        }
    }

    int cont = 0;
    for(int i = 1; i<counter; i++)
    {
        if(ctSet[i].size() > 50){
            cont += 1;
        }

    }

    if(looptimes++ > 50 ){
        cout << "colortime: " << colortime << "colortime1:" << colortime1 << endl;
    }
    return cont;
}
