/** @file ex_opengl.cpp
 * 
 * @brief TO WRITE demonstrate video w/o any inputs
 *
 * @author Peter Nagy (deetrone)
 * 
 */


#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <GL/glew.h>
#include <GL/freeglut.h>
#include "../shader_utils.h"

// Global variables
GLuint vbo_window;
GLuint program;
//fix attribute for vertex shader
GLint attribute_coord3d;
// window dimensions
GLint uniform_width;
GLint uniform_height;
// "time" uniform to change render
GLint uniform_time;


struct attributes {
  GLfloat coord3d[3];
};


int init_resources()
{ 
  // vertices to be stored in VBO, to create rectangle in vertex shader
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

  GLuint vs, fs;
  if ((vs = create_shader("../shaders/vertex.glsl", GL_VERTEX_SHADER))   == 0) return 0;
  if ((fs = create_shader("../shaders/creation.glsl", GL_FRAGMENT_SHADER)) == 0) return 0; //must be set manually for correct shader
  
  program = create_program(program, vs, fs);

  //bind attribute (coordinates)
  const char* attribute_name = "coord3d";
  attribute_coord3d = glGetAttribLocation(program, attribute_name);
  if (attribute_coord3d == -1) {
    fprintf(stderr, "Could not bind attribute %s\n", attribute_name);
    return 0;
  }
  //bind all uniforms from fragment shaders (associated program object)
  const char* uniform_name;
  //width and height, should be same
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
  // time control uniform
  uniform_name = "time";
  uniform_time = glGetUniformLocation(program, uniform_name);
  if (uniform_time == -1) {
    fprintf(stderr, "Could not bind uniform_time %s\n", uniform_name);
    return 0;
  }
  return 1;
}

void onIdle() {
  float window_width=glutGet(GLUT_WINDOW_HEIGHT);
  float window_height=glutGet(GLUT_WINDOW_HEIGHT);
  float t=glutGet(GLUT_ELAPSED_TIME)/1000.0/2.0;
  

  //when switching modes change program accordingly
  glUseProgram(program);
  glUniform1f(uniform_width, window_width);
  glUniform1f(uniform_height, window_height);
  glUniform1f(uniform_time, t);

  glutPostRedisplay();
}

void onDisplay()
{
  glClearColor(1.0, 1.0, 1.0, 1.0); // empty == white
  glClear(GL_COLOR_BUFFER_BIT);
  glUseProgram(program);
  glEnableVertexAttribArray(attribute_coord3d);
  glBindBuffer(GL_ARRAY_BUFFER, vbo_window);
  glVertexAttribPointer(
    attribute_coord3d,  // attribute
    3,                  // number of elements per vertex, here (x,y,z)
    GL_FLOAT,           // the type of each element
    GL_FALSE,           // take values as-is
    0,                  // every element appears...
    0                   // offset of first element
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
  glutInitWindowSize(glutGet(GLUT_SCREEN_WIDTH), glutGet(GLUT_SCREEN_HEIGHT));
  glutCreateWindow("BZZZBZ");
  //error handling, for non-Pi implementations
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

  



