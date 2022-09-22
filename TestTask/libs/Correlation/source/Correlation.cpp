#include <Correlation.h>

const int REAL=0;
const int IMAG=1;
//Функция для прямого преобразовния Фурье
void Correlation::fft(fftw_complex *in, fftw_complex *out,int N)
{
     fftw_plan plan=fftw_plan_dft_1d(N,in,out,FFTW_FORWARD,FFTW_ESTIMATE);
     fftw_execute(plan);
     fftw_destroy_plan(plan);
     fftw_cleanup();

}

// Функция для обратного преобразовния Фурье
void Correlation::ifft(fftw_complex *in, fftw_complex *out,int N)
{
    fftw_plan plan=fftw_plan_dft_1d(N,in,out,FFTW_BACKWARD,FFTW_ESTIMATE);
    fftw_execute(plan);
    fftw_destroy_plan(plan);
    fftw_cleanup();
    for(int i=0; i<N; ++i)
    {
        out[i][REAL]/=N;
        out[i][IMAG]/=N;
    }
}


std::vector<double> Correlation::correlation(std::vector<double> x1,std::vector<double> x2)
{

    std::vector<double> r;
    std::vector<std::complex<double>> sig1(x1.size());
    std::vector<std::complex<double>> sig2(x2.size());

    for(int i=0;i<sig1.size();i++)
    {
        sig1[i]=x1[i];
    }

    for(int i=0;i<sig2.size();i++)
    {
        sig2[i]=x2[i];
    }

    std::vector<std::complex<double>> X3;

    if(sig1.size()!=sig2.size())
    {
        int sizeX1=sig1.size();
        int sizeX2=sig2.size();
        sig1.resize(sizeX1+sizeX2-1);
        sig2.resize(sizeX1+sizeX2-1);
    }

    //Быстрая корреляция
    fft((fftw_complex*)&sig1[0],(fftw_complex*)&sig1[0],sig1.size());
    fft((fftw_complex*)&sig2[0],(fftw_complex*)&sig2[0],sig2.size());

    //Комплесное сопряжение
    for(int i=0; i<sig1.size();++i)
    {
      sig1[i]=std::conj(sig1[i]);
    }

    for(int i=0; i<sig1.size();++i)
    {
        double re=sig1[i].real()*sig2[i].real()-sig1[i].imag()*sig2[i].imag();
        double im=sig1[i].real()*sig2[i].imag()+sig2[i].real()*sig1[i].imag();
        X3.push_back(std::complex<double>(re,im));
    }


    ifft((fftw_complex*)&X3[0],(fftw_complex*)&X3[0],X3.size());
    r.resize(X3.size());
    for(int i=0; i<x1.size();++i)
    {
        X3[i]/=X3.size();
        r[i]=X3[i].real();
    }


    return r;
}
