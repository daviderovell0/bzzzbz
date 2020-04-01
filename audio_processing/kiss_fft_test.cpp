#include "kiss_fftr.h"
#include <stdio.h>
#include <iostream>
#include <fstream>



int main(){
    char line[10];
    // number of samples
    int nfft = 1024;
    // Open file with data from the ADC 
    std::ifstream f;
    f.open("samples/sample_sines.txt", std::ifstream::in);
    // Open output FFT file
    std::ofstream out;
    out.open("samples/sine_phases.txt", std::ofstream::out);


    // Real valued FFT data structures init. 
    kiss_fftr_cfg cfg = kiss_fftr_alloc(nfft, 0, 0, 0);
    kiss_fft_scalar *cx_in = new kiss_fft_scalar[nfft];
    kiss_fft_cpx *cx_out = new kiss_fft_cpx[nfft/2+1];
 
    while (f.getline(line, 10))
    {
        for (int i = 0; i < nfft; i++){
            cx_in[i] = std::stod(line);
            std::cout << cx_in[i];
            if (!f.getline(line, 10)) {
                break;
            }
        }

        //fft here
        kiss_fftr( cfg , cx_in , cx_out );

        // print it to file here
        for (int k = 0; k < nfft/2+1; k++){
            out << cx_out[k].i;
            out << "\n";
        }
        


    }
    out.close();
    free(cfg);
    delete[] cx_in;
    delete[] cx_out;
}