//"Creation" by Danilo Guanabara
//Original: https://www.shadertoy.com/view/XsXXDn ; last accessed 08/04/2020
//Port: Peter Nagy

uniform float W;
uniform float H;
uniform float c_A; //c_A is GLUT_ELAPSED_TIME and as such its value is increasing, taking up more and more memory
const float pi=3.141596;

void main(){

    float X=gl_FragCoord.x/W;
    float Y=gl_FragCoord.y/H;
    vec2 pos=vec2(X-0.45,Y);
    vec2 u, p=pos;
    vec3 c;
    float l, z=c_A; //z==dynamic
    for(int i=0;i<3;i++){
         p=pos;
         u=p;
         p -= 0.5;
         p.x *= W/H;
         l = length(p);
         z += 0.07;
         u += p/l*(sin(z)+1.0)*abs(sin(l*9.0-z*2.0));
         c[i] = 0.01/length(fract(u) - 0.5);
    }
    gl_FragColor = vec4(c, 1.0);
}       

