#include "WavReader.h"
#include "DemodulatorFSK.h"


int main()
{
    WavReader reader;
    DemodulatorFSK demodul;
    std::vector<short> data=reader.ReadWavData<short>("/home/yan/FSK2_8000_50Bd_7_5st5.wav");
    std::vector<int> dem=demodul.demodulate(data,8000,787,1237,50,1.5);

    for(int i=0;i<dem.size();i++)
    {
        std::cout<<dem[i]<<" ";
    }

    return 0;
}
