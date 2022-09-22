#ifndef CORRELATION_H
#define CORRELATION_H
#include <vector>
#include <fftw3.h>
#include <complex>

class Correlation{
    public:
    void fft(fftw_complex *in, fftw_complex *out,int N);
    void ifft(fftw_complex *in, fftw_complex *out,int N);
    std::vector<double> correlation(std::vector<double> x1, std::vector<double> x2);
};

#endif // CORRELATION_H
