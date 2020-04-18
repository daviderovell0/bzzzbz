/* HEADER: Utility for reading, compiling, debugging shaders
 *
 * Author: Peter Nagy
 * Based on code by: Sylvain Beucler
 * Original from the OpenGL Programming wikibook. This file is in the public domain.
 */

#ifndef _CREATE_SHADER_H
#define _CREATE_SHADER_H
char* file_read(const char* filename);
void print_log(GLuint object);
GLuint create_shader(const char* filename, GLenum type);
#endif
