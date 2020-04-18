/* HEADER: Utility for reading, compiling, debugging shaders
 *
 */

char* file_read(const char* filename);
void print_log(GLuint object);
GLuint create_shader(const char* filename, GLenum type);
GLuint create_program(GLuint program, GLuint vert, GLuint frag);

