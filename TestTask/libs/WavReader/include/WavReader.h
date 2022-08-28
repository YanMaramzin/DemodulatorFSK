#ifndef WAVREADER_H
#define WAVREADER_H
#include "vector"
#include "string"
#include "fstream"
#include "iostream"

class WavReader{


    public:
        int ReadFreqSampl(std::string fileName);
    template <typename T>
        std::vector<T> ReadWavData(const std::string &fileName)
        {
            std::ifstream file(fileName,std::ios::binary);
            std::vector<T> data;

            if(!file.is_open())
            {
                std::cout<<"Read error"<<std::endl;
            }
            else
            {
                file.seekg (0, file.end);


                int length = file.tellg();
                length-=44;

                data.reserve(length/(2*sizeof (T)));
                //file.seekg(0,file.beg);
                file.seekg (44);
                T * buffer = new T [length];
                // read data as a block:
                file.read ((char *)buffer,length);
                for(size_t i=0;i<length/sizeof(T);i++)
                 {
                    data.push_back(buffer[i]);
                 }
                file.close();

                delete[] buffer;
            }
            return data;
        }

};




#endif // WAVREADER_H
