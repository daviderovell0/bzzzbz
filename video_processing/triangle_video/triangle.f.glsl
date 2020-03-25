varying vec3 f_color;
uniform float fade;
uniform float red;
uniform float green;
uniform float blue;

void main(void)
{
  gl_FragColor[0] = red * f_color.r;
  gl_FragColor[1] = green * f_color.g;
  gl_FragColor[2] = blue * f_color.b;
  gl_FragColor[3] = 0.1 + fade;
}
