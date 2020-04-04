/** @file AudioProcessing.h
 *
 * @brief Class for incoming audio and FFT processing.
 * 
 * @author Davide Rovelli (daviderovell0)
 */
#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <signal.h>

#include <jack/jack.h>
#include "kiss_fftr.h"

class AudioProcessingCallback {
public:
    /**
     * Process the incoming audio
     */
    virtual void process(float sample) = 0;
};


class AudioProcessing {
public:
    void setCallback(AudioProcessingCallback* cb);
    void start();
    void stop();
    /**
     * Real signal to complex freq. FFT using a mixed-radix library
     * (https://github.com/berndporr/kiss-fft). 
     * 
     * @returns a buffer with magnitudes of each complex pair
     * 
     * @param kiss_fft_scalar *buffer - the input buffer with real samples (double).
     * 
     */
    double *runFFT(kiss_fft_scalar *buffer, double *fft_magnitudes);

private:
    const char **ports;
	const char *client_name = "bzzzbz";
	const char *server_name = NULL;
    jack_options_t options = JackNullOption;
	jack_status_t status;

    AudioProcessingCallback *apcallback  = NULL;
    /**
     * Set the input and the output ports for the audio card.
     * 
     * For our current chip WM8731 we only need the LineIn inputs, since the audio output
     * is provided by the chip built in BYPASS mode
     * 
     * @TODO add extra port for stereo input
     */
    jack_port_t *input_port;

    /**
     * JACK client data structure init. 
     */
    jack_client_t *client;
	
    /**
     * The input read process (JACK application callback) that is called in a
     * special realtime thread once for each audio cycle.
     * 
     */
    static int run(jack_nframes_t nframes, void *arg);

    /**
     * TODO: 
     * Handle termination signal input from the user (i.e. ctrl-C)
     * Very tricky to use since it has to be static and also
     * */
    static void signal_handler(int sig);
    /**
     * Defines the shutdown routine when the JACK server is terminated.
     */
    static void jack_shutdown(void *arg);
};