/** \file testAudioProcessing.cpp
 * 
 *  \brief Unit test suite for the AudioProcessing class
 * 
 *  \note Always run tests from the repository home "bzzzbz"
 * 
 *  \author Davide Rovelli (daviderovell0)
 * 
 */

#define BOOST_TEST_MODULE sampletest
#include <iostream>
#include <fstream>
#include <boost/test/unit_test.hpp>
#include "../src/AudioProcessing.h"

class PrintSampleCallBack : public AudioProcessingCallback {
     virtual void process(float* in_buffer, float* out_buffer) {
         printf("%f\n", in_buffer[0]);
     }
};

AudioProcessing *ap;
PrintSampleCallBack cb;

/** 
 * \test AudioProcessingThreadStarts
 * 
 * \brief Check that the class constructor does not throw any exception
 * 
 */ 
BOOST_AUTO_TEST_CASE(AudioProcessingCreate)
{
    BOOST_CHECK_NO_THROW(ap = new AudioProcessing());
}

/** 
 * \test AudioProcessingCallbackIsRegistered
 * 
 * \brief Check that the callback registration does not throw any exception
 * 
 */ 
BOOST_AUTO_TEST_CASE(AudioProcessingCallbackIsRegistered)
{
    BOOST_CHECK_NO_THROW(ap->setCallback(&cb));
}

/** 
 * \test AudioProcessingStartJackClient
 * 
 * \brief Check that the JACK client connects to the server and starts 
 * 
 */ 
BOOST_AUTO_TEST_CASE(AudioProcessingStartJackClient)
{
   
    BOOST_CHECK_NO_THROW(ap->start());
}

/**
 * \test FFTReturnsCorrectSpectrum
 * 
 * \brief Check that the FFT returns the correct bin from a 
 * pre recorded input: 432Hz sinewave. FFT buffer size is 1024 at 48KHz, we 
 * therefore expect to have a peak around bin 10 since 24KHz / (1024/2+1) * 10 = 467Hz 
 * +-46Hz(FFT resolution).
 */ 
BOOST_AUTO_TEST_CASE(FFTReturnsCorrectSpectrum)
{   
    // init fft data structures
    float *fft_buffer_in = new kiss_fft_scalar[1024];
    float *fft_frame_out = (float *) malloc(513*sizeof(float));

    std::ifstream f;
    std::string line;
    float n;
    f.open("test/inputs/sinewave_432Hz.txt");
    // put audio values in the fft buffer
    for(int i=0; i < 1024; i++){
        getline(f,line);
        line = line.substr(0,9);
        fft_buffer_in[i] = std::stof(line);
    }
    
    // run fft
    ap->runFFT(fft_buffer_in,fft_frame_out,1024);

    // find the peak bin
    float max = fft_frame_out[0];
    int peak_bin = 0;
    for(int i=0; i < 513; i++){
        if(fft_frame_out[i] > max){
            max = fft_frame_out[i];
            peak_bin = i;
        }
    }
    
    // check if peak is at bin 10 with 46Hz (1 bin) tolerance
    BOOST_CHECK(peak_bin >=9 && peak_bin <= 11);
}

/** 
 * \test AudioProcessingThreadStops
 * 
 * \brief Check that the JACK client is closed successfully
 * 
 */ 
BOOST_AUTO_TEST_CASE(AudioProcessingThreadStops)
{
    BOOST_CHECK_EQUAL(ap->stop(),0);
}