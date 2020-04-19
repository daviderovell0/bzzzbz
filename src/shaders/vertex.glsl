/** @file vertex.glsl
 * 
 * @brief Vertex shader. 
 *
 * @author Peter Nagy (deetrone)
 * 
 */

attribute vec3 coord3d;

void main(void) {
 /**In the context of BZZZBZ the vertex shader is fixed, only a rectangle is drawn on the screen, while all the "colouring" is done by the fragment shader.
  */
  gl_Position = vec4(coord3d, 1.0);
}
