uniform float H;
uniform float W;
uniform float c_A;
uniform float FREQ;
uniform float PERIOD;
uniform float EXPONENT;
const float pi = 3.141596;



void main( void ) {

    float X=gl_FragCoord.x/W - 0.42;
    float Y=gl_FragCoord.y/H + 0.2;
    
	float color = 0.0;
    float var = 0.0;

	vec2 point[5];
	point[0] = vec2(0.0,0.2);
	point[1] = vec2(0.8,0.4);
	point[2] = vec2(0.5,0.8);
	point[3] = vec2(1.0,1.0);
	point[4] = vec2(0.1,1.0);
	
	float m_dist = 100.0;  // minimum distance
	float AMP = 0.4;
	
   	for (int i = 0; i < 5; i++) 
	{
		float dist = distance(vec2(X,Y), point[i]);
                m_dist = min(m_dist, dist);
    }

    //ZOOM: frequency, PERIOD: evolution speed
	color += sin(m_dist*X*(exp(FREQ)*50.0-49.0)*(cos((c_A+pi*c_A)/PERIOD))+sin(c_A));
	var +=0.3* sin(tan(m_dist*Y*X*pow(c_A, EXPONENT*5.0)));

    //gl_FragColor = vec4( vec3(0.8*(var+color)+0.2*color/(var+color)), 1.0 );
    gl_FragColor = vec4( 0.0*(0.8*(var+color)+0.2*color/(var+color)), 0.72*(0.8*(var+color)+0.2*color/(var+color)), 1.0*(0.8*(var+color)+0.2*color/(var+color)), 1.0);
}
