#include "AudioProcessing.h"

/* When receiving ctrl-C */
void AudioProcessing::signal_handler(int sig, jack_client_t *client)
{
	jack_client_close(client);
	fprintf(stderr, "signal received, exiting ...\n");
	exit(0);
}

void AudioProcessing::jack_shutdown(){
    exit (1);
}


void AudioProcessing::setCallback(AudioProcessingCallback* cb) {
    apcallback = cb;
}

int AudioProcessing::run(jack_nframes_t nframes, void *arg){
    jack_default_audio_sample_t *in;
    in = (jack_default_audio_sample_t*)jack_port_get_buffer (input_port, nframes);
    int i;

    for( i=0; i<nframes; i++ )
	{
		if (this->apcallback) {
				this->apcallback->process(in[i]);
			}
	}
	return 0;      
}


void AudioProcessing::start(){
    /* open a client connection to the JACK server */
	this->client = jack_client_open (client_name, options, &status, server_name);
	if (this->client == NULL) {
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
	   there is work to be done.
	*/

	jack_set_process_callback (client, this->run, 0); // passing run only is not type compatible with JACK API

    /* tell the JACK server to call `jack_shutdown()' if
	   it ever shuts down, either entirely, or if it
	   just decides to stop calling us.
	*/

	jack_on_shutdown (client, this->jack_shutdown, 0);

    /* register the port */
    input_port = jack_port_register (this->client, "input",
					 JACK_DEFAULT_AUDIO_TYPE,
					 JackPortIsInput, 0);
    if (input_port == NULL) {
        fprintf(stderr, "no more JACK ports available\n");
		exit (1);
    }

    /* Start the client. AudioProcessing::run starts now. */
	if (jack_activate (this->client)) {
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
    ports = jack_get_ports (this->client, NULL, NULL,
				JackPortIsPhysical|JackPortIsOutput);
	if (ports == NULL) {
		fprintf(stderr, "no physical capture ports\n");
		exit (1);
	}
    /* Connect the input port (micIN) */
	if (jack_connect (this->client, ports[0], jack_port_name (input_port))) {
		fprintf (stderr, "cannot connect input ports\n");
	}
    /* Deallocate memory */
	free (ports);


    // handle termination inputs for the user
    signal(SIGQUIT, this->signal_handler);
	signal(SIGTERM, this->signal_handler);
	signal(SIGHUP, this->signal_handler);
	signal(SIGINT, this->signal_handler);

}


void AudioProcessing::stop(){
    jack_client_close (this->client);
	exit (0);
}