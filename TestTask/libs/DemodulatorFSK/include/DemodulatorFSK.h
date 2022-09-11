#ifndef DEMODULATORFSK_H
#define DEMODULATORFSK_H
#include <vector>
#include <math.h>
#include <algorithm>
#include <numeric>
#include "../../Filter/include/Filter.h"

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

//        int j=0;        //переменная для определения шага
//        int step=0;
//        double sumI0;   // временная переменная для кореляционных сумм
//        double sumQ0;   // временная переменная для кореляционных сумм
//        double sumI1;   // временная переменная для кореляционных сумм
//        double sumQ1;   // временная переменная для кореляционных сумм

//        // Цикл для вычисления корреляционных сумм
//        while (step<in.size())
//        {
//            //если мы прошли 6 символов, то мы получили стоп бит, который в 1.5 раза длинее обычной посылки
//            if (j==6)
//            {
//               sumI0=std::accumulate(begin(I0)+step,begin(I0)+step+stopBitDuration*countPerSymbol,0.0);
//               sumI1=std::accumulate(begin(I1)+step,begin(I1)+step+stopBitDuration*countPerSymbol,0.0);
//               sumQ0=std::accumulate(begin(Q0)+step,begin(Q0)+step+stopBitDuration*countPerSymbol,0.0);
//               sumQ1=std::accumulate(begin(Q1)+step,begin(Q1)+step+stopBitDuration*countPerSymbol,0.0);

//               step+=stopBitDuration*countPerSymbol;    // увеличение шага на длину стопового бита
//               j=0;
//            }
//            // вычисление сумм для обычных посылок
//            else
//            {
//               sumI0=std::accumulate(begin(I0)+step,begin(I0)+step+countPerSymbol,0.0);
//               sumI1=std::accumulate(begin(I1)+step,begin(I1)+step+countPerSymbol,0.0);
//               sumQ0=std::accumulate(begin(Q0)+step,begin(Q0)+step+countPerSymbol,0.0);
//               sumQ1=std::accumulate(begin(Q1)+step,begin(Q1)+step+countPerSymbol,0.0);

//               step+=countPerSymbol;    // увеличение шага на длину обычного бита
//               j++;
//            }
//            summaI0.push_back(sumI0);
//            summaI1.push_back(sumI1);
//            summaQ0.push_back(sumQ0);
//            summaQ1.push_back(sumQ1);
//        }


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

        for(int i=0;i<difference.size();i++)
        {
            z0[i]=sqrtI0[i]+sqrtQ0[i];
            z1[i]=sqrtI1[i]+sqrtQ1[i];
        }



//        //Получение последовательности нулей и единиц
//        for(int i=0;i<difference.size();i++)
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
