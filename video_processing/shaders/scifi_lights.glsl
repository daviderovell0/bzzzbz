// Original shader: https://www.shadertoy.com/view/4ljXRR

uniform float W;
uniform float H;
uniform float c_A;
const float pi=3.141596;

float hash (float v) {
  return smoothstep(0.3, 0.8, abs(sin(v))) * 20.0;
}

void main(void) {
    float X=gl_FragCoord.x/W;
    float Y=gl_FragCoord.y/H;
    vec2 pos=vec2(X,Y);

    vec2 p = -1.5 + 2.5 * pos;		
    p.x *= X / Y;	
	
    //float v = p.x + cos(0.9 + p.y);
    	
    vec3 col = vec3(0.2, 0.3, 0.4);
    vec3 c = vec3(0.0);	
    c += col / (abs(tan(hash(p.x) + cos(1.5 + p.y)))); 
    c += col / (abs(tan(hash(p.y) + cos(1.5 + p.x))));
    c /= 5.0+c_A;	
	
    gl_FragColor = vec4(c, 1.0);
}
