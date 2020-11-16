/** @file cells.glsl
 * 
 * @brief Example shader used to demonstrate BZZZBZ. Audio reactive and can also be controlled with potentiometers. 
 * @author Peter Nagy (deetrone)
 * 
 */

uniform float H;
uniform float W;
uniform float A;
uniform float B;
uniform float C;
uniform float fft[513];

const float pi = 3.1415926;

void main( void ) {
    /**
     *   The pattern is based on five cells (cellular noise) created with the loop and the interaction of shaping functions. The two variables, 'color' and 'var' can be explored and modified to create new visuals while the hue can be changed by modifying the coefficients of the parameters of the gl_FragColor function.
     *
     */    
        //normalise screen coordinates and adjust position
        float X=gl_FragCoord.x/W -0.42;
    	float Y=gl_FragCoord.y/H + 0.2;
        //variables derived from controls
        float A_mod=175.0+25.0*(4.0*A-2.0);
        float C_fall=1.0-0.999*C;
        float EXPONENT=pow(B, 0.8);
    	//init variables
	float color = 0.0;
    	float var = 0.0;
        float m_dist = 1.0;  // minimum distance
        //create centres for cells
	    vec2 point[5];
	    point[0] = vec2(0.0,0.2);
	    point[1] = vec2(0.8,0.4);
	    point[2] = vec2(0.5,0.8);
	    point[3] = vec2(1.0,1.0);
	    point[4] = vec2(0.1,1.0);

        //5 variables based on incoming FFT, in current version single bins are selected manually, will be more robust in future versions
	float env16=fft[1];
        float env12=fft[4];
        float env1=fft[8];
        float env3=fft[16];
        float env01=fft[48];

        //static values that can be used for testing/debugging
        /*float env16 = 0.1;
        float env12= 0.3;
        float env1 = 0.4;
        float env3 = 0.6;
        float env01 = 0.1;*/
	    
        //find global minimum distance for each pixel from any point
       	for (int i = 0; i < 5; i++){
		float dist = distance(vec2(X,Y), point[i]);
            	m_dist = min(m_dist, dist);
        }
        //use two variables to color 
	color += sin(m_dist*X*(exp(C)*20.0-19.0*env3)*(cos((A_mod-env12+pi*A_mod)/(60.0*C_fall)))+sin(A_mod+env12));
	var +=0.3* sin(tan(env1+m_dist*Y*X*pow(A_mod+2.0*env1, EXPONENT*2.0)));
	    
        //output, set coefficients of each channel to change hue 
        gl_FragColor = vec4(0.0*(0.8*(var+color)+0.2*color/(var+color)), 0.72*(0.8*(var+color)+0.2*color/(var+color+env01)), 1.0*(0.8*(var+color+env01)+0.2*color/(var+color+env01)), 1.0);
}
