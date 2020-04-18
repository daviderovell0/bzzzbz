/**
 * From the OpenGL Programming wikibook: http://en.wikibooks.org/wiki/OpenGL_Programming
 * This file is in the public domain.
 * Contributors: Sylvain Beucler
 */
#ifndef _CREATE_SHADER_H
#define _CREATE_SHADER_H
#include <GL/glew.h>
#include <stdio.h>
#include <stdlib.h>
char* file_read(const char* filename);
void print_log(GLuint object);
GLuint create_shader(const char* filename, GLenum type);
#endif
