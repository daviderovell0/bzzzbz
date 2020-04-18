/** @file  shader_utils.h
 *
 *  @brief Header for functions to read and create shaders and program object.
 *
 *  @author Peter Nagy (deetrone)
 */

char* file_read(const char* filename);
void print_log(GLuint object);
GLuint create_shader(const char* filename, GLenum type);
GLuint create_program(GLuint program, GLuint vert, GLuint frag);
