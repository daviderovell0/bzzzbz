attribute vec3 coord3d;

void main(void) {
  gl_Position = vec4(coord3d, 1.0);
}
