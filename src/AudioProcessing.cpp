#include "AudioProcessing.h"


void AudioProcessing::jack_shutdown(void *arg){
    exit (1);
}



void AudioProcessing::runFFT(kiss_fft_scalar *buffer, float *fft_magnitudes, int nfft){
	// Real valued FFT data structures init. 
    kiss_fftr_cfg cfg = kiss_fftr_alloc(nfft, 0, 0, 0);
	kiss_fft_cpx *cx_out = new kiss_fft_cpx[nfft/2+1];

	//fft here
    kiss_fftr( cfg , buffer , cx_out );

	for (int k = 1; k < nfft/2+1; k++){
            // calculate magnitude of complex pair
            fft_magnitudes[k] = sqrt(pow(cx_out[k].i, 2) + pow(cx_out[k].r,2))/10; // /10 is a temp normalisation
        }
	fft_magnitudes[0] = 0;
	free(cfg);
}

void AudioProcessing::setCallback(AudioProcessingCallback* cb) {
    apcallback = cb;
}

int AudioProcessing::run(jack_nframes_t nframes, void *arg){
    AudioProcessing *ap = (AudioProcessing *) arg;
    jack_default_audio_sample_t *in, *out;

    in = (jack_default_audio_sample_t*)jack_port_get_buffer (ap->input_port, nframes);
	out = (jack_default_audio_sample_t*)jack_port_get_buffer (ap->output_portL, nframes);

	if (ap->apcallback) {
		ap->apcallback->process(in, out);
	}
	return 0;      
}


void AudioProcessing::start(){
	printf("opening Jack client...\n");
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

	jack_set_process_callback (client, this->run, this); // passing non-static run is not type compatible with JACK API

    /* tell the JACK server to call `jack_shutdown()' if
	   it ever shuts down, either entirely, or if it
	   just decides to stop calling us.
	*/

	jack_on_shutdown (client, this->jack_shutdown, 0);

    /* register the ports */
    input_port = jack_port_register (client, "input",
					 JACK_DEFAULT_AUDIO_TYPE,
					 JackPortIsInput, 0);

	output_portL = jack_port_register (client, "output1",
					  JACK_DEFAULT_AUDIO_TYPE,
					  JackPortIsOutput, 0);
	output_portR = jack_port_register (client, "output2",
					  JACK_DEFAULT_AUDIO_TYPE,
					  JackPortIsOutput, 0);		

    if (input_port == NULL || output_portL == NULL || output_portR == NULL){
        fprintf(stderr, "Error in ports registration\n");
		exit (1);
    }

	

    /* Start the client. AudioProcessing::run starts now. */
	if (jack_activate (client)) {
		fprintf (stderr, "cannot activate client");
		exit (1);
	}

    /**
     * \note to avoid confusion:
     * - ports need to be connected before the client is started
	 * - the orientation of the driver backend ports is "flipped": playback ports are
	 * "input" to the backend, and capture ports are "output" from
	 * it.
     */

    /* Get the input port */
    ports = jack_get_ports (client, NULL, NULL,JackPortIsPhysical|JackPortIsOutput);
	if (ports == NULL) {
		fprintf(stderr, "no physical capture ports\n");
		exit (1);
	}

	if (jack_connect (client, ports[0], jack_port_name (input_port))) {
		fprintf (stderr, "Cannot connect to input ports\n");
	}
	
	/* Get the output ports */
	ports = jack_get_ports (client, NULL, NULL,JackPortIsPhysical|JackPortIsInput);
	if (ports == NULL) {
		fprintf(stderr, "no physical capture ports\n");
		exit (1);
	}
	if (jack_connect (client, jack_port_name (output_portL),ports[0])) {
		fprintf (stderr, "Cannot connect to output port\n");
	}
	if (jack_connect (client, jack_port_name (output_portR),ports[1])) {
		fprintf (stderr, "Cannot connect to output port\n");
	}
	free (ports);
}

int AudioProcessing::stop(){
    return jack_client_close (client);
	exit (0);
}