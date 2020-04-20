/** @file AudioProcessing.h
 *
 * @brief Class for incoming audio and FFT processing.
 * 
 * @author Davide Rovelli (daviderovell0)
 */
#define kiss_fft_scalar float

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
     * Process the incoming audio.
     * 
     * @arg in: the input buffer from one of the ports
     * @arg out: the output buffer to one of the ports
     * 
     * @note Ports represent physical inputs and outputs of 
     * a the system's soundcard. As of release v1.0, only two 
     * ports are used since most systems have at least 2 (i.e. 
     * microphone in and LEFT channel of line out for a laptop)
     */
    virtual void process(float* in_buffer, float* out_buffer) = 0;
};


class AudioProcessing {
public:
    void setCallback(AudioProcessingCallback* cb);
    void start();
    int stop();
    /**
     * Real signal to complex freq. FFT using a mixed-radix library
     * (https://github.com/berndporr/kiss-fft). 
     * 
     * @returns a buffer with magnitudes of each complex pair
     * 
     * @param kiss_fft_scalar *buffer - the input buffer with real samples (double).
     * 
     */
    void runFFT(kiss_fft_scalar *buffer, float *fft_magnitudes, int nfft);

private:
    const char **ports;
	const char *client_name = "bzzzbz";
	const char *server_name = "jserver";
    jack_options_t options = JackNullOption;
	jack_status_t status;

    AudioProcessingCallback *apcallback  = NULL;
    /**
     * The physical audio input and the output ports specific to the system
     * the soundcard.
     * For WM8731: input_port is either LLINEIN or RLINEIN
     * output_port is either LLINEOUT or RLINEOUT 
     * 
     * @TODO add extra port for stereo input
     */
    jack_port_t *input_port;
    jack_port_t *output_portL;
    jack_port_t *output_portR;
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