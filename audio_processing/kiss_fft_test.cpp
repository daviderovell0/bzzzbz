#include "kiss_fftr.h"
#include <stdio.h>
#include <iostream>
#include <fstream>



int main(){
    char line[100];
    // number of samples
    int nfft = 1024000;
    std::ifstream f;
    f.open("samples/sample_sines.txt", std::ifstream::in);

    /*kiss_fftr_cfg cfg = kiss_fftr_alloc(nfft, 0, 0, 0);
    kiss_fft_scalar *cx_in = new kiss_fft_scalar[nfft];
    kiss_fft_cpx *cx_out = new kiss_fft_cpx[nfft/2+1];*/

    f.getline(line, 100); 
    for (int k = 0; k < nfft; ++k)
    {
        std::cout << line;
        f.getline(line, 100); 
    }

    /*kiss_fftr(cfg, cx_in, cx_out);

    printf(cx_out);

    free(cfg);
    delete[] cx_in;
    delete[] cx_out;*/
}