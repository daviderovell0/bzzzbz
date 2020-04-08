/* OpenGL main, windowing for BZZZBZ
 *
 * Author: Peter Nagy
 * Based on code by: Sylvain Beucler
 */


/* TO DO!!!
 * fix/implement viewport
 * create different modes for the available shaders
 * make proper control variables
 */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
// Use glew.h or gl.h but first declares all prototypes
#include <GL/glew.h>
#include <GL/freeglut.h>
#include "shader_utils.h"

//all global variables
GLuint vbo_window;
GLuint program;
GLint attribute_coord3d;
GLint uniform_width;
GLint uniform_height;
GLint uniform_A;

struct attributes {
  GLfloat coord3d[3];
};

int init_resources()
{ 
  //vertices, z=0
  struct attributes window_attributes[] = {
    {{ -1.0,  1.0, 0.0}},
    {{-1.0, -1.0, 0.0}},
    {{ 1.0, -1.0, 0.0}},
    {{ 1.0, 1.0, 0.0}},
    {{ -1.0, 1.0, 0.0}}
  };
  
  glGenBuffers(1, &vbo_window);
  glBindBuffer(GL_ARRAY_BUFFER, vbo_window);
  glBufferData(GL_ARRAY_BUFFER, sizeof(window_attributes), window_attributes, GL_STATIC_DRAW);

  GLint link_ok = GL_FALSE;

  GLuint vs, fs;
  if ((vs = create_shader("shaders/vertex.glsl", GL_VERTEX_SHADER))   == 0) return 0;
  if ((fs = create_shader("shaders/creation.glsl", GL_FRAGMENT_SHADER)) == 0) return 0; //must be set manually for correct shader >> change to filename_read to be able to switch
  
  //unique program object for each shader or one program and shaders are changed...as this is done in init_resources, probably multiple programs
  program = glCreateProgram();
  glAttachShader(program, vs);
  glAttachShader(program, fs);
  glLinkProgram(program);
  glGetProgramiv(program, GL_LINK_STATUS, &link_ok);
  if (!link_ok) {
    fprintf(stderr, "glLinkProgram:");
    print_log(program);
    return 0;
  }

  const char* attribute_name = "coord3d";
  attribute_coord3d = glGetAttribLocation(program, attribute_name);
  if (attribute_coord3d == -1) {
    fprintf(stderr, "Could not bind attribute %s\n", attribute_name);
    return 0;
  }
  //if multiple program we can choose which uniforms are bound where
  const char* uniform_name;
  uniform_name = "H";
  uniform_height = glGetUniformLocation(program, uniform_name);
  if (uniform_height == -1) {
    fprintf(stderr, "Could not bind uniform_height %s\n", uniform_name);
    return 0;
  }
  uniform_name = "W";
  uniform_width = glGetUniformLocation(program, uniform_name);
  if (uniform_width == -1) {
    fprintf(stderr, "Could not bind uniform_width %s\n", uniform_name);
    return 0;
  }
  uniform_name = "c_A";
  uniform_A = glGetUniformLocation(program, uniform_name);
  if (uniform_A == -1) {
    fprintf(stderr, "Could not bind uniform_A %s\n", uniform_name);
    return 0;
  }
  return 1;
}

void onIdle() {
  float window_width=glutGet(GLUT_WINDOW_HEIGHT); //fix viewport for correct division and no stretching
  float window_height=glutGet(GLUT_WINDOW_HEIGHT);
  float dyn_A=0.0;
  //float dyn_A=sin(glutGet(GLUT_ELAPSED_TIME)/1000.0/4.0)/2.0 +0.5; //dummy dynamic variable, 4sec, 0.0-1.0
  

  //when switching modes change program accordingly
  glUseProgram(program);
  glUniform1f(uniform_width, window_width);
  glUniform1f(uniform_height, window_height);
  glUniform1f(uniform_A, dyn_A);
  glutPostRedisplay();
}

void onDisplay()
{
  glClearColor(1.0, 1.0, 1.0, 1.0); // empty == white
  glClear(GL_COLOR_BUFFER_BIT);
  glUseProgram(program); //must be changed from modes
  //same for all programs, as attribute_coord3d is always bound
  glEnableVertexAttribArray(attribute_coord3d);
  glBindBuffer(GL_ARRAY_BUFFER, vbo_window);
  glVertexAttribPointer(
    attribute_coord3d,   // attribute
    3,                   // number of elements per vertex, here (x,y,z)
    GL_FLOAT,            // the type of each element
    GL_FALSE,            // take our values as-is
    0,                  //every element appears...
    0                    // offset of first element
  );

  // Push each element in buffer_vertices to the vertex shader, create rectangle
  glDrawArrays(GL_TRIANGLES, 0, 3);
  glDrawArrays(GL_TRIANGLES, 2, 3);
  glDisableVertexAttribArray(attribute_coord3d);
  glutSwapBuffers();
}

void free_resources() //free up memory, all programs used should be deleted.
{   
  glDeleteProgram(program);
  glDeleteBuffers(1, &vbo_window);
}

int main(int argc, char* argv[])
{
  //init context
  glutInit(&argc, argv);
  glutInitContextVersion(2,0);
  glutInitDisplayMode(GLUT_RGBA|GLUT_ALPHA|GLUT_DOUBLE|GLUT_DEPTH);
  glutInitWindowSize(1200, 900);
  glutCreateWindow("BZZZBZ");
  //error handling, important for non-Pi implementations
  GLenum glew_status = glewInit();
  if (glew_status != GLEW_OK) {
    fprintf(stderr, "Error: %s\n", glewGetErrorString(glew_status));
    return 1;
  }
  if (!GLEW_VERSION_2_0) {
    fprintf(stderr, "Error: your graphic card does not support OpenGL 2.0\n");
    return 1;
  }
  //main loop if init_resources returns 1
  if (init_resources()) {
    glutDisplayFunc(onDisplay);
    glutIdleFunc(onIdle);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glutMainLoop();
  }

  free_resources();
  return 0;
}

  



