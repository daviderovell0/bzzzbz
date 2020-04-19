/** @file ex_SPI.cpp
 *
 *  @brief Implementation file to demonstrate external control using potentiometers. Uses the fragment shader wavepatterns.glsl 
 *
 * @author BZZZBZ
 *
 */

// Standard libraries
#include <iostream>
#include <fstream>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
// Use glew.h or gl.h but first declares all prototypes
#include <GL/glew.h>
#include <GL/freeglut.h>

// For SPI
#include <stdint.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <linux/types.h>
#include <linux/spi/spidev.h>

// Custom classes
#include "../MCP3008Comm.h"
#include "../shader_utils.h"


/***** OpenGL globals *****/
GLuint vbo_window;
GLuint program;
GLint attribute_coord3d;
GLint uniform_width;
GLint uniform_height;
GLint uniform_pA;
GLint uniform_pB;
GLint uniform_pC;
GLint uniform_time;

struct attributes {
  GLfloat coord3d[3];
};

/***** Control interface *****/
float pot_A=0.0;
float pot_B=0.0;
float pot_C=0.0;


/**
 * SPI sample callback class
 * Processing the output samples coming from the MCP3008 ADC 
 **/
class MCP3008printSampleCallback : public MCP3008callback {
	virtual void hasSample(int value, int channel) {
		switch (channel)
    {
    case 0:
      pot_A = value/1024.0;
      break;
    
    case 1:
      pot_B = value/1024.0;
      break;
    
    case 2:
      pot_C = value/1024.0;
      break;
    
    default:
      break;
    };
    printf("value: %d, channel: %d\n", value, channel);
	}
};


// OPENGL FUNCTIONS
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

  GLuint vs, fs;
  if ((vs = create_shader("../shaders/vertex.glsl", GL_VERTEX_SHADER))   == 0) return 0;
  if ((fs = create_shader("../shaders/wavepatterns.glsl", GL_FRAGMENT_SHADER)) == 0) return 0; //must be set manually for correct shader
  
  program = create_program(program, vs, fs);

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
  uniform_name = "time";
  uniform_time = glGetUniformLocation(program, uniform_name);
  if (uniform_time == -1) {
    fprintf(stderr, "Could not bind uniform_time %s\n", uniform_name);
    return 0;
  }
  uniform_name = "A";
  uniform_pA = glGetUniformLocation(program, uniform_name);
  if (uniform_pA == -1) {
    fprintf(stderr, "Could not bind uniform_pA %s\n", uniform_name);
    return 0;
  }
  uniform_name = "B";
  uniform_pB = glGetUniformLocation(program, uniform_name);
  if (uniform_pB == -1) {
    fprintf(stderr, "Could not bind uniform_pB %s\n", uniform_name);
    return 0;
  }
  uniform_name = "C";
  uniform_pC = glGetUniformLocation(program, uniform_name);
  if (uniform_pC == -1) {
    fprintf(stderr, "Could not bind uniform_pC %s\n", uniform_name);
    return 0;
  }
  return 1;
}

void onIdle() {
  
  float window_width=glutGet(GLUT_WINDOW_HEIGHT); //fix viewport for correct division and no stretching
  float window_height=glutGet(GLUT_WINDOW_HEIGHT);
  float t=glutGet(GLUT_ELAPSED_TIME)/1000.0/2.0;

  // Pass values to shader
  //when switching modes change program accordingly
  glUseProgram(program);
  glUniform1f(uniform_width, window_width);
  glUniform1f(uniform_height, window_height);
  glUniform1f(uniform_time, t);
  glUniform1f(uniform_pA, pot_A);
  glUniform1f(uniform_pB, pot_B);
  glUniform1f(uniform_pC, pot_C);
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

int main(int argc, char *argv[]){
    

    //Instantiate SPI related classes and start readouts
    MCP3008Comm* m = new MCP3008Comm();
    MCP3008printSampleCallback print_cb;
    m->setCallback(&print_cb);
    m->start();


    // opengl
    //init context
    glutInit(&argc, argv);
    glutInitContextVersion(2,0);
    glutInitDisplayMode(GLUT_RGBA|GLUT_ALPHA|GLUT_DOUBLE|GLUT_DEPTH);
    glutInitWindowSize(glutGet(GLUT_SCREEN_WIDTH), glutGet(GLUT_SCREEN_HEIGHT));
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

    // Terminate threads, free resources
    m->stop();
    delete m;
    free_resources();
    return 0;
}
