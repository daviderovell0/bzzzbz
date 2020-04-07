#include "AudioProcessing.h"
#include <stdio.h>
#include <iostream>
#include <fstream>



// GLOBAL VARIABLES
// Initalise audio processing instance with default constructor 
AudioProcessing *ap = new AudioProcessing(); 
// Initialise FFT buffer as global variable for access during video mapping
int test_counter=0;
int nfft = 64;
double *fft_frame = (double *) malloc((nfft/2+1)*sizeof(double));



/**
 *  Terminate the JACK client when exiting the program, to avoid errors in the 
 * following executions.
*/
void signal_handler(int sig)
{
	fprintf(stderr, "signal received, closing JACK client...\n");
    ap->stop();
    printf("recorded number of fft frames:%d\n",test_counter);
    free(fft_frame);
}


/**
 * Define the audio process routine callback:
 * Run the FFT method every time the chosen buffer 
 * is filled. Buffer size/2+1 = number of bins.
 * Take FFT accuracy vs. execution speed into account when choosing the buffer size. 
 */
class TriggerFFT : public AudioProcessingCallback {
    int i = 0;
    // initalise the buffer to 64
    kiss_fft_scalar *buffer = new kiss_fft_scalar[nfft];
    


	virtual void process(float sample) {
		
        // run the fft when the buffer is full
        if (i == nfft -1 ) {
            
            ap->runFFT(buffer, fft_frame);
            //printf("\nfft frame 0: %f\n", fft_frame[32]);
            //printf("fft frame size: %lu\n\n", sizeof(fft_frame));
            i = 0;
            test_counter += 1;

            for(int j=0;j< nfft/2+1;j++){
                fprintf(stdout,"%f\n",fft_frame[j]);
            }
            printf("--\n");
        }
        // fill it otherwise
        else {
            //printf("buf fill. sample:%f\n",sample);
            buffer[i] = sample;
            i++;
        }


	}
};

int main(int argc, char *argv[]){

    // Set the exit routine: Keep running until exit signal (ctrl+C) received.
    signal(SIGQUIT, signal_handler);
	signal(SIGTERM, signal_handler);
	signal(SIGHUP, signal_handler);
	signal(SIGINT, signal_handler);

    TriggerFFT cb;
    ap->setCallback(&cb);
    ap->start();
    getchar();
    printf("recorded number of fft frames:%d\n",test_counter);
    ap->stop();

    free(fft_frame);
}