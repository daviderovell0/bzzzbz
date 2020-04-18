/** @file creation.glsl
 * 
 * @brief Example shader that runs without any inputs, perfect for the desktop environment. Original: https://www.shadertoy.com/view/XsXXDn 
 *
 * @author Danilo Guanabara, Port: Peter Nagy (deetrone)
 * 
 */

uniform float W;
uniform float H;
uniform float time;
const float pi=3.1415926;

void main(){

    float X=gl_FragCoord.x/W;
    float Y=gl_FragCoord.y/H;
    vec2 pos=vec2(X-0.46,Y);
    vec2 u, p=pos;
    vec3 c;
    float l, z=time; 
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

