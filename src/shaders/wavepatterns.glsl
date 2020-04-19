/** @file wavepatterns.glsl
 * 
 * @brief Example shader to demonstrate SPI control of BZZZBZ.
 *
 * @author Marcell Illyes (marcellillyes)
 * 
 */


uniform float W;
uniform float H;
uniform float A;
uniform float B;
uniform float C;
uniform float time;

const float pi = 3.1415926;
const float THICK = 0.0015;
const float RES = 100.0;
const float FREQ = 5.0;


void main( void ) {
    /**The 'if' statement is used to draw the shapes*/
    	float X = gl_FragCoord.x/W;
    	float Y = gl_FragCoord.y/H;
	    float color = 0.0;
	    if(mod((1.5*A) * cos((0.2*time/24.0)+Y * (FREQ+B*0.2) * pi - 0.2*pi) + (1.5*A) * sin((0.2*time/16.0)+X * (FREQ+B*0.3) * pi - 0.2*pi) , 1.0 / RES) <= THICK*(4.9*C-0.5)){
	        color += 1.0;
		}
	    gl_FragColor = vec4(1.0*color, 0.5*Y*color+(0.2*B*Y*color), 0.1*color, 1.0 );
}
