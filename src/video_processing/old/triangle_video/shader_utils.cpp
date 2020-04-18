//Utility for reading, compiling, debugging shaders

#include <stdio.h>
#include <stdlib.h>
#include <GL/glew.h>

//Load file "filename" into a string in memory (this can be passed on glShaderSource)
char* file_read(const char* filename)
{
  FILE* in = fopen(filename, "rb");
  if (in == NULL) return NULL;

  int res_size = BUFSIZ;
  char* res = (char*)malloc(res_size);
  int nb_read_total = 0;

  while (!feof(in) && !ferror(in)) {
    if (nb_read_total + BUFSIZ > res_size) {
      if (res_size > 10*1024*1024) break;
      res_size = res_size * 2;
      res = (char*)realloc(res, res_size);
    }
    char* p_res = res + nb_read_total;
    nb_read_total += fread(p_res, 1, BUFSIZ, in);
  }
  
  fclose(in);
  res = (char*)realloc(res, nb_read_total + 1);
  res[nb_read_total] = '\0';
  return res;
}

//
void print_log(GLuint object)
{
  GLint log_length = 0;
  if (glIsShader(object)) 
  {
    glGetShaderiv(object, GL_INFO_LOG_LENGTH, &log_length);
  }
  else if (glIsProgram(object))
  {
    glGetProgramiv(object, GL_INFO_LOG_LENGTH, &log_length);
  }
  else
  {
    fprintf(stderr, "printlog: Not a shader or a program\n");
    return;
  }

  char* log = (char*)malloc(log_length);

  if (glIsShader(object))
  {
    glGetShaderInfoLog(object, log_length, NULL, log);
  }
  else if (glIsProgram(object))
  {
    glGetProgramInfoLog(object, log_length, NULL, log);
  }

  fprintf(stderr, "%s", log);
  free(log);
}

//Create and compile shaders from "filename" using create_shader function
GLuint create_shader(const char* filename, GLenum type)
{
  //read strings from "filename" and make new variable "source"
  const GLchar* source = file_read(filename);
  //error handling
  if (source == NULL) {
    fprintf(stderr, "Error opening %s: ", filename); 
    perror("");
    return 0;
  }
  GLuint res = glCreateShader(type); //create empty shader "res" of type, "type"
  const GLchar* sources[] = {
    // Define GLSL version (Open GL ES 2.0 > version 100)
    "#version 100\n"  // OpenGL ES 2.0
    ,
    // Define default float precision for fragment shaders:
    //if type=fragment ? * : "" (empty string) ... * is precision declarations.
    (type == GL_FRAGMENT_SHADER) ?
    "#ifdef GL_FRAGMENT_PRECISION_HIGH\n" //if high can be used use high
    "precision highp float;           \n"
    "#else                            \n"
    "precision mediump float;         \n"
    "#endif                           \n"
     : ""
     ,
    source }; //append "source" >> basically just dynamically finishes the shader src

  glShaderSource(res, 3, sources, NULL); //3 bc version, precision stuff, source
  free((void*)source); //free memory allocated to source void* pointer to unknown datatype
  glCompileShader(res);
  //some error handling
  GLint compile_ok = GL_FALSE;
  glGetShaderiv(res, GL_COMPILE_STATUS, &compile_ok); 
  if (compile_ok == GL_FALSE) 
  {
    fprintf(stderr, "%s:", filename);
    print_log(res);
    glDeleteShader(res); //deletes it if error in compilation
    return 0;
  }
  return res; //returns "res" object which is the compiled shader coming from "filename"
  //need to return as it is defined within function, is of type GLuint
}

GLuint create_program(const char *vertexfile, const char *fragmentfile)
//takes the two filenames as inputs and creates, attaches to and links w program object
{
	GLuint program = glCreateProgram();
	GLuint shader;

	if(vertexfile) {
		shader = create_shader(vertexfile, GL_VERTEX_SHADER);
		if(!shader)
			return 0;
		glAttachShader(program, shader);
	}

	if(fragmentfile) 
    {
		shader = create_shader(fragmentfile, GL_FRAGMENT_SHADER);
		if(!shader)
			return 0;
		glAttachShader(program, shader);
	}

	glLinkProgram(program);
	GLint link_ok = GL_FALSE;
	glGetProgramiv(program, GL_LINK_STATUS, &link_ok);
	if (!link_ok) 
    {
		fprintf(stderr, "glLinkProgram:");
		print_log(program);
		glDeleteProgram(program);
		return 0;
	}
	return program;
}

//Geometry shaders could be used eg for turning POINTS into TRIANGLES or even more complex objects eg. 3D cube etc.

/*
#ifdef GL_GEOMETRY_SHADER
GLuint create_gs_program(const char *vertexfile, const char *geometryfile, const char *fragmentfile, GLint input, GLint output, GLint vertices) {
	GLuint program = glCreateProgram();
	GLuint shader;

	if(vertexfile) {
		shader = create_shader(vertexfile, GL_VERTEX_SHADER);
		if(!shader)
			return 0;
		glAttachShader(program, shader);
	}

	if(geometryfile) {
		shader = create_shader(geometryfile, GL_GEOMETRY_SHADER);
		if(!shader)
			return 0;
		glAttachShader(program, shader);

		glProgramParameteriEXT(program, GL_GEOMETRY_INPUT_TYPE_EXT, input);
		glProgramParameteriEXT(program, GL_GEOMETRY_OUTPUT_TYPE_EXT, output);
		glProgramParameteriEXT(program, GL_GEOMETRY_VERTICES_OUT_EXT, vertices);
	}

	if(fragmentfile) {
		shader = create_shader(fragmentfile, GL_FRAGMENT_SHADER);
		if(!shader)
			return 0;
		glAttachShader(program, shader);
	}

	glLinkProgram(program);
	GLint link_ok = GL_FALSE;
	glGetProgramiv(program, GL_LINK_STATUS, &link_ok);
	if (!link_ok) {
		fprintf(stderr, "glLinkProgram:");
		print_log(program);
		glDeleteProgram(program);
		return 0;
	}

	return program;
}
#else
GLuint create_gs_program(const char *vertexfile, const char *geometryfile, const char *fragmentfile, GLint input, GLint output, GLint vertices) {
	fprintf(stderr, "Missing support for geometry shaders.\n");
	return 0;
}
#endif

GLint get_attrib(GLuint program, const char *name) {
	GLint attribute = glGetAttribLocation(program, name);
	if(attribute == -1)
		fprintf(stderr, "Could not bind attribute %s\n", name);
	return attribute;
}

GLint get_uniform(GLuint program, const char *name) {
	GLint uniform = glGetUniformLocation(program, name);
	if(uniform == -1)
		fprintf(stderr, "Could not bind uniform %s\n", name);
	return uniform;
}/*/
