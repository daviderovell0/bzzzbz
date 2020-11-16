/** \file AudioProcessing.h
 *
 * \brief Classes for incoming audio and FFT processing.
 * 
 * \author Davide Rovelli (daviderovell0)
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

/**
 * \class AudioProcessingCallback
 * 
 * \brief Callback attached to the AudioProcessing class running in a separate 
 * real-time priority thread.
 * 
 */ 
class AudioProcessingCallback {
public:
    /**
     * Process the incoming audio.
     * 
     * \param in_buffer: the input buffer from one of the ports
     * \param out_buffer: the output buffer to one of the ports
     * 
     * \note Ports represent physical inputs and outputs of 
     * a the system's soundcard. As of release v1.0, only two 
     * ports are used since most systems have at least 2 (i.e. 
     * microphone in and LEFT channel of line out for a laptop)
     */
    virtual void process(float* in_buffer, float* out_buffer) = 0;
};


/**
 * \class AudioProcessing
 * 
 * \brief Class for audio data acquisition and processing. It uses JACK audio
 * client to connect with the underlying hardware. 
 */ 
class AudioProcessing {
public:
    /**
     * \brief Set the callback of the AudioProcessing class.
     * 
     * \param AudioProcessingCallback* cb - Pointer to the callback instance.
     */ 
    void setCallback(AudioProcessingCallback* cb);
    /**
     * 
     * \brief Initialise the JACK client and start the callback thread.  
     */ 
    void start();
    /**
     * \brief close the JACK client and free resources. 
     * 
     */
    int stop();
    /**
     * Real signal to complex freq. FFT using a mixed-radix library
     * (https://github.com/berndporr/kiss-fft). 
     * 
     * \returns a buffer with magnitudes of each complex pair
     * 
     * \param kiss_fft_scalar *buffer - the input buffer with real samples (float).
     * \param float *fft_magnitudes - the output buffer that will be filled complex frequencies.
     * \param int nfft - FFT buffer size
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