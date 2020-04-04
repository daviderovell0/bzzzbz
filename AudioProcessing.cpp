#include "AudioProcessing.h"


void AudioProcessing::jack_shutdown(void *arg){
    exit (1);
}

double *AudioProcessing::runFFT(kiss_fft_scalar *buffer, double *fft_magnitudes){
	int nfft = sizeof(buffer)*8;
	// Real valued FFT data structures init. 
    kiss_fftr_cfg cfg = kiss_fftr_alloc(nfft, 0, 0, 0);
	kiss_fft_cpx *cx_out = new kiss_fft_cpx[nfft/2+1];

	//fft here
    kiss_fftr( cfg , buffer , cx_out );

	for (int k = 0; k < nfft/2+1; k++){
            // calculate magnitude of complex pair
            fft_magnitudes[k] = sqrt(pow(cx_out[k].i, 2) + pow(cx_out[k].r,2));
        }
	return fft_magnitudes;
}

void AudioProcessing::setCallback(AudioProcessingCallback* cb) {
    apcallback = cb;
}

int AudioProcessing::run(jack_nframes_t nframes, void *arg){
    AudioProcessing *ap = (AudioProcessing *) arg;
    jack_default_audio_sample_t *in;
    in = (jack_default_audio_sample_t*)jack_port_get_buffer (ap->input_port, nframes);
    unsigned int i;

    for( i=0; i<nframes; i++ )
	{
		if (ap->apcallback) {
				ap->apcallback->process(in[i]);
			}
	}
	return 0;      
}


void AudioProcessing::start(){
	printf("open client...\n");
    /* open a client connection to the JACK server */
	client = jack_client_open (client_name, options, &status, server_name);
	if (client == NULL) {
		fprintf (stderr, "jack_client_open() failed, "
			 "status = 0x%2.0x\n", status);
		if (status & JackServerFailed) {
			fprintf (stderr, "Unable to connect to JACK server\n");
		}
		exit (1);
	}
	if (status & JackServerStarted) {
		fprintf (stderr, "JACK server started\n");
	}
	if (status & JackNameNotUnique) {
		client_name = jack_get_client_name(client);
		fprintf (stderr, "unique name `%s' assigned\n", client_name);
	}

    /* tell the JACK server to call `run()' whenever
	   there is work to be done. Callback is automatically in a thread.
	*/

	jack_set_process_callback (client, this->run, this); // passing run only is not type compatible with JACK API

    /* tell the JACK server to call `jack_shutdown()' if
	   it ever shuts down, either entirely, or if it
	   just decides to stop calling us.
	*/

	jack_on_shutdown (client, this->jack_shutdown, 0);

    /* register the port */
    input_port = jack_port_register (client, "input",
					 JACK_DEFAULT_AUDIO_TYPE,
					 JackPortIsInput, 0);
    if (input_port == NULL) {
        fprintf(stderr, "no more JACK ports available\n");
		exit (1);
    }

    /* Start the client. AudioProcessing::run starts now. */
	if (jack_activate (client)) {
		fprintf (stderr, "cannot activate client");
		exit (1);
	}

    /**
     * Notes to avoid confusion:
     * - ports need to be connected before the client is started
	 * - the orientation of the driver backend ports is "flipped": playback ports are
	 * "input" to the backend, and capture ports are "output" from
	 * it.
     */

    /* Get the input port */
    ports = jack_get_ports (client, NULL, NULL,
				JackPortIsPhysical|JackPortIsOutput);
	if (ports == NULL) {
		fprintf(stderr, "no physical capture ports\n");
		exit (1);
	}
    /* Connect the input port (micIN) */
	if (jack_connect (client, ports[0], jack_port_name (input_port))) {
		fprintf (stderr, "cannot connect input ports\n");
	}
    /* Deallocate memory */
	free (ports);

    // handle termination inputs for the user
    /*signal(SIGQUIT, signal_h);
	signal(SIGTERM, signal_handler);
	signal(SIGHUP, signal_handler);
	signal(SIGINT, signal_handler);*/

}


void AudioProcessing::stop(){
    jack_client_close (client);
	exit (0);
}