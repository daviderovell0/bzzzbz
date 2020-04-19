/** @file spectrum_slow.glsl
 * 
 * @brief An earlier implementation of spectrum.glsl, that caused performance issues, demonstrates the limitations of loops in GLSL.
 *
 * @author Marcell Illyes (marcellillyes)
 * 
 */

uniform float H;
uniform float W;
uniform float fft[513];


void main( void ) {
   /**Shows the audio spectrum. The 'for' loop used here leads to serious performance issues on the Pi but generally runs fine on more powerful desktop environments. 
    *
    *
    */
	
	float X=gl_FragCoord.x/W;
    float Y=gl_FragCoord.y/H;
	
	float color = 0.0;
	float c= 0.0;
	
	for(int i=0; i<=512;i+=16){
	
		if(mod(X,1.0/32.0)<0.02 && X<c/32.0 && X > (c-1.0)/32.0){
			if(Y<fft[i]){
				color+=0.5;	
			}	
		}
		c+=1.0;
	}
	gl_FragColor = vec4( vec3( color, color,color ), 1.0 );

}
