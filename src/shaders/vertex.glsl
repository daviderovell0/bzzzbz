/** @file vertex.glsl
 * 
 * @brief Vertex shader. Fixed for BZZZBZ.
 *
 * @author Peter Nagy (deetrone)
 * 
 */

attribute vec3 coord3d;

void main(void) {
  gl_Position = vec4(coord3d, 1.0);
}
