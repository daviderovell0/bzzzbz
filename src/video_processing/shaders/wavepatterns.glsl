//Author: Marcell Illyes


uniform float H;
uniform float W;
uniform float c_A; //c_A is GLUT_ELAPSED_TIME >> constanstly increasing, risk of memory overflow on pi
const float pi = 3.141596;

float BPM = 120.0;
float THICKK = 0.0015;
float RES = 100.0;
float AMP = 0.5;
float FREQ = 5.0;
float BIAS = 0.5 ;
float PHASE = 0.2 * pi;


void main( void ) {
   
    float X = gl_FragCoord.x/W;
    float Y = gl_FragCoord.y/H;
	float color = 0.0;
	
	if(mod(AMP * cos(Y * FREQ * pi - PHASE) + BIAS + AMP * sin(X * FREQ * pi - PHASE) + BIAS, 1.0 / RES) <= THICKK){
	
	        color += 1.0+c_A;
		
	}
	
	gl_FragColor = vec4(color, (0.6*Y)*color, 0.2*color, 1.0 );

}
