#include "AudioProcessing.h"

// define the audio process routine 
class TriggerFFT : public AudioProcessingCallback {
	virtual void process(float sample) {
		printf("%f\n",sample);
	}
};

int main(int argc, char *argv[]){
    AudioProcessing *ap;
    TriggerFFT cb;

    ap->setCallback(&cb);
    ap->start();
    getchar();
    ap->stop();

}