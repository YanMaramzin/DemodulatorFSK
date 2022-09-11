#include <Filter.h>
#include <algorithm>
#include <cmath>

std::vector<double> FilterMovingAverageNonRec::filtration(std::vector<double> &in,int windowSize){

    std::vector<double> filtSig;
    filtSig.reserve(in.size());
    double sum=0;
    for(int i=0; i<in.size();i++)
    {
        sum += in[i];
        if (i >= windowSize)
            sum -= in[i-windowSize];
        filtSig.push_back(sum/windowSize);
    }
    return filtSig;
}

std::vector<double> FilterMovingAverageRec::filtration(std::vector<double> &in,int windowSize)
{
    std::vector<double> filtSig;
    filtSig.reserve(in.size());
    filtSig.push_back(in[0]/windowSize);
    for(int i=1; i<in.size();i++)
    {
        if(i<windowSize)
        filtSig.push_back(filtSig[i-1]+in[i]/windowSize);
        else
        filtSig.push_back(filtSig[i-1]+(in[i]-in[i-windowSize])/windowSize);
    }
    return filtSig;
}

