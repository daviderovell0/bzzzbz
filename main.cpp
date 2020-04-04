#include "AudioProcessing.h"

// Initalise audio processing instance with default constructor 
AudioProcessing *ap = new AudioProcessing(); 

/**
 *  Terminate the JACK client when exiting the program, to avoid errors in the 
 * following executions.
*/
void signal_handler(int sig)
{
	fprintf(stderr, "signal received, closing JACK client...\n");
    ap->stop();
}


// Define the audio process routine 
class TriggerFFT : public AudioProcessingCallback {
	virtual void process(float sample) {
		printf("%f\n",sample);
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
    ap->stop();

}