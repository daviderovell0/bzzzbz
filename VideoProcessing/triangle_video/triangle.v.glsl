attribute vec3 coord3d;
attribute vec3 v_color;
varying vec3 f_color;
uniform mat4 m_transform;

void main(void) {
  gl_Position = m_transform * vec4(coord3d, 1.0);
  f_color = v_color;
}
