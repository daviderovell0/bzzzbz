uniform float H;
uniform float W;
uniform float fft[513];
// this "cuts down" the fft. 513 (MAX) for full resolution
// 340 bins will display the first 16KHz that is a good 
// spectrum for most music applications
const float n_of_bins = 340.0;

void main( void ) {
	// normalise
	float X=gl_FragCoord.x/W;
    float Y=gl_FragCoord.y/H;
	
	float level = 0.0;
	float xstep=1.0/n_of_bins;

	float rel_pos=X/xstep;

	// Casting creates the bin. i.e. all rel_pos values between 1.0 and 1.99 
	// (and therefore their corresponding X values) will have a Y value of fft[1]
	int bin=int(rel_pos);

	if (Y <= fft[bin]){
		level += 1.0;
	}
		

	gl_FragColor = vec4( vec3(level), 1.0 );

}