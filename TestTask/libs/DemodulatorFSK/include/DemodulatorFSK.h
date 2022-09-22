#ifndef DEMODULATORFSK_H
#define DEMODULATORFSK_H
#include <vector>
#include <math.h>
#include <algorithm>
#include <numeric>
#include <iterator>
#include "../../Filter/include/Filter.h"
#include "../../Correlation/include/Correlation.h"

class DemodulatorFSK{

    public:
    template<typename T>
    std::vector<int> demodulate(std::vector<T>& in,double inputSampleRate,double f0, double f1,double baudRate,const double stopBitDuration)
    {
        std::vector<int> bin;       // вектор для последовательности 0 и 1
        std::vector<double> t;      // вектор времени для расчета значений cos и sin
        const double countPerSymbol=inputSampleRate/baudRate;     // число отсчётов, приходящихся на один символ
        t.resize(in.size());
        t[0]=0;
        for(int i=1;i<in.size();i++)
        {
            t[i]+=t[i-1]+1/inputSampleRate;
        }

        std::vector<double> cos0;  //вектор для значений косинуса с частотой f0
        std::vector<double> sin0;  //вектор для значений синуса с частотой f0
        std::vector<double> cos1;  //вектор для значений косинуса с частотой f1
        std::vector<double> sin1;  //вектор для значений синуса с частотой f0

        cos0.resize(in.size());
        sin0.resize(in.size());
        cos1.resize(in.size());
        sin1.resize(in.size());

        for(int i=0;i<in.size();i++)
        {
            cos0[i]=cos(2*M_PI*f0*t[i]);
            cos1[i]=cos(2*M_PI*f1*t[i]);
            sin0[i]=sin(2*M_PI*f0*t[i]);
            sin1[i]=sin(2*M_PI*f1*t[i]);
        }

        std::vector<double> Q0;     //квадратурная составляющая для частоты f0
        std::vector<double> I0;     //синфазная составляющая для частоты f0

        std::vector<double> Q1;     //квадратурная составляющая для частоты f1
        std::vector<double> I1;     //синфазная составляющая для частоты f1

        Q0.resize(in.size());
        I0.resize(in.size());
        Q1.resize(in.size());
        I1.resize(in.size());

        for(int i=0;i<in.size();i++)
        {
            I0[i]=in[i]*cos0[i]*sqrt(2*baudRate);
            I1[i]=in[i]*cos1[i]*sqrt(2*baudRate);

            Q0[i]=in[i]*sin0[i]*sqrt(2*baudRate);
            Q1[i]=in[i]*sin1[i]*sqrt(2*baudRate);
        }
        std::vector<double> summaI0;
        std::vector<double> summaI1;
        std::vector<double> summaQ0;
        std::vector<double> summaQ1;

        summaI0.reserve(in.size());
        summaI1.reserve(in.size());
        summaQ0.reserve(in.size());
        summaQ1.reserve(in.size());

        FilterMovingAverageNonRec filtMAV;

        summaI0=filtMAV.filtration(I0,countPerSymbol);
        summaI1=filtMAV.filtration(I1,countPerSymbol);
        summaQ0=filtMAV.filtration(Q0,countPerSymbol);
        summaQ1=filtMAV.filtration(Q1,countPerSymbol);


        std::vector<double> sqrtI0; //вектор для расчёта квадрата
        std::vector<double> sqrtI1; //вектор для расчёта квадрата
        std::vector<double> sqrtQ0; //вектор для расчёта квадрата
        std::vector<double> sqrtQ1; //вектор для расчёта квадрата



        sqrtI0.resize(summaI0.size());
        sqrtI1.resize(summaI1.size());
        sqrtQ0.resize(summaQ0.size());
        sqrtQ1.resize(summaQ1.size());


        //Возведение значений в квадрат
        for(int i=0;i<summaI0.size();i++)
        {
            sqrtI0[i]=pow(summaI0[i],2);
            sqrtI1[i]=pow(summaI1[i],2);
            sqrtQ0[i]=pow(summaQ0[i],2);
            sqrtQ1[i]=pow(summaQ1[i],2);
        }


        std::vector<double> z0;  // вектор для получения суммы квадратов на выходе ветви для частоты f0
        std::vector<double> z1;  // вектор для получения суммы квадратов на выходе ветви для частоты f1
        std::vector<double> difference; // разность значений на выходе
        z0.resize(sqrtI0.size());
        z1.resize(sqrtI0.size());

        for(int i=0;i<z1.size();i++)
        {
            z0[i]=sqrtI0[i]+sqrtQ0[i];
            z1[i]=sqrtI1[i]+sqrtQ1[i];
        }

        std::vector<double> startStop(round(z1.size()/(countPerSymbol*7.5))*countPerSymbol*7.5,0.0);

        for(int i=0;i<startStop.size();i+=5*countPerSymbol)
        {
            for(int j=0;j<2.5*countPerSymbol;j++)
            {
                if(j<stopBitDuration*countPerSymbol)
                    startStop[i+j]=1;
                else
                    startStop[j+i]=-1;
            }
        }

        Correlation corr;
        std::vector<double> r1;
        r1=corr.correlation(z1,startStop);

        std::vector<double>::const_iterator find_it = std::max_element(r1.begin(),r1.end());

        double maxCorr=*std::max_element(r1.begin(),r1.end());
        int ind=find_it-r1.begin()-1;

        int p=0;
        while (ind<z0.size()){
            if(p==0)
            {
              ind+=1.5*countPerSymbol/2;
              if(z1[ind]>z0[ind])
                  bin.push_back(1);
              else
                  bin.push_back(0);
              ind+=1.25*countPerSymbol;
              p=6;
            }
            else
            {
                if(z1[ind]>z0[ind])
                    bin.push_back(1);
                else
                    bin.push_back(0);
                ind+=countPerSymbol;
                --p;
            }
        }
        //Получение последовательности нулей и единиц
//        for(int i=0;i<difference.size();i+=countPerSymbol/2)
//        {
//            if(difference[i]>0)
//                bin.push_back(1);
//            else
//                bin.push_back(0);
//        }


        return bin;
}
};

#endif // DEMODULATORFSK_H
