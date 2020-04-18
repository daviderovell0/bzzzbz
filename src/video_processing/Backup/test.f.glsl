uniform float H;
uniform float W;
uniform float c_A;
float color;

void main( void ) {
    //vec2 resolution = vec2(H, W); //here
	float X = gl_FragCoord.x/W;
    float Y = gl_FragCoord.y/H;
	
	color += sin(30.0*Y);
    color += sin(30.0*X);
    float var=sin(c_A);

	gl_FragColor = vec4( vec3(color+var), 1.0);
}
