uniform float H;
uniform float W;
uniform float fft[33];


void main( void ) {
	
	float X=gl_FragCoord.x/W;
    float Y=gl_FragCoord.y/H;
	
	float color = 0.0;
	float c= 0.0;
	
	
	for(int i=0; i<=32; i++){
	
		if( mod(X,1.0/32.0) < 0.02 && X < c/32.0 && X > (c-1.0)/32.0 ){
		
			if( Y < fft[i] ){
			
				color+=0.5;
				
			}
			
		}
		
		c+=1.0;
	}
	

	gl_FragColor = vec4( vec3( color, color,color ), 1.0 );

}