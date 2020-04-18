#include <stdio.h>
#include <stdlib.h>
#include <math.h>
// Use glew.h or gl.h but first declares all prototypes
#include <GL/glew.h>

#include <GL/freeglut.h>

// #define GLM_MESSAGES
#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "shader_utils.h"

GLuint vbo_triangle;
GLuint program;
GLint attribute_coord3d, attribute_v_color;
GLint uniform_m_transform;
GLint uniform_fade;
GLint uniform_red;
GLint uniform_green;
GLint uniform_blue;

GLfloat BPM=112.0;

struct attributes {
  GLfloat coord3d[3];
  GLfloat v_color[3];
};

int init_resources()
{
  /*struct attributes triangle_attributes[] = {
    {{ 0.0,  0.4, 0.0}, {red * 1.0, green * 0.6, blue * 0.5}},
    {{-0.4, -0.4, 0.0}, {red * 0.5, green * 1.0, blue * 0.6}},
    {{ 0.4, -0.4, 0.0}, {red * 0.6, green * 0.5, blue * 1.0}}
  };/*/

  struct attributes triangle_attributes[] = {
    {{ 0.0,  0.4, 0.0}, {1.0, 0.6, 0.5}},
    {{-0.4, -0.4, 0.0}, {0.5, 1.0, 0.6}},
    {{ 0.4, -0.4, 0.0}, {0.6, 0.5, 1.0}}
  };

  glGenBuffers(1, &vbo_triangle);
  glBindBuffer(GL_ARRAY_BUFFER, vbo_triangle);
  glBufferData(GL_ARRAY_BUFFER, sizeof(triangle_attributes), triangle_attributes, GL_STATIC_DRAW);

  GLint link_ok = GL_FALSE;

  GLuint vs, fs;
  if ((vs = create_shader("triangle.v.glsl", GL_VERTEX_SHADER))   == 0) return 0;
  if ((fs = create_shader("triangle.f.glsl", GL_FRAGMENT_SHADER)) == 0) return 0;

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

  const char* attribute_name;
  attribute_name = "coord3d";
  attribute_coord3d = glGetAttribLocation(program, attribute_name);
  if (attribute_coord3d == -1) {
    fprintf(stderr, "Could not bind attribute %s\n", attribute_name);
    return 0;
  }
  attribute_name = "v_color";
  attribute_v_color = glGetAttribLocation(program, attribute_name);
  if (attribute_v_color == -1) {
    fprintf(stderr, "Could not bind attribute %s\n", attribute_name);
    return 0;
  }
  const char* uniform_name;
  uniform_name = "m_transform";
  uniform_m_transform = glGetUniformLocation(program, uniform_name);
  if (uniform_m_transform == -1) {
    fprintf(stderr, "Could not bind uniform_m_transform %s\n", uniform_name);
    return 0;
  }
  uniform_name = "fade";
  uniform_fade = glGetUniformLocation(program, uniform_name);
  if (uniform_fade == -1) {
    fprintf(stderr, "Could not bind uniform_fade %s\n", uniform_name);
    return 0;
  }
  uniform_name = "red";
  uniform_red = glGetUniformLocation(program, uniform_name);
  if (uniform_red == -1) {
    fprintf(stderr, "Could not bind uniform_red %s\n", uniform_name);
    return 0;
  }
  uniform_name = "green";
  uniform_green = glGetUniformLocation(program, uniform_name);
  if (uniform_green == -1) {
    fprintf(stderr, "Could not bind uniform_green %s\n", uniform_name);
    return 0;
  }
  uniform_name = "blue";
  uniform_blue = glGetUniformLocation(program, uniform_name);
  if (uniform_blue == -1) {
    fprintf(stderr, "Could not bind uniform_blue %s\n", uniform_name);
    return 0;
  }

  return 1;
}

void onIdle() {
  float cur_fade = cosf(glutGet(GLUT_ELAPSED_TIME) / 1000.0 * (2*3.14) * BPM / 60 ) / 4 + 0.5; // 0->1->0 every 5 seconds
  float move = sinf(glutGet(GLUT_ELAPSED_TIME) / 1000.0 * (2*3.14) * BPM / 60 / 8); // -1<->+1 every 10 seconds
  float angle = glutGet(GLUT_ELAPSED_TIME) / 1000.0 * 30;  // 30° per second
  glm::vec3 axis_z(0, 0, 1);
  
  /*glm::mat4 m_transform = glm::translate(glm::mat4(1.0f), glm::vec3(0.4 * move, 0.6 * sinf(move), 0.0))
     * glm::rotate(glm::mat4(1.0f), glm::radians(angle), axis_z);*/

  glm::mat4 m_transform;
  float direction=sinf(glutGet(GLUT_ELAPSED_TIME) / 1000.0 * (2*3.14) * BPM / 60 / 8);
  if (direction >= 0 )
  {
   m_transform = glm::translate(glm::mat4(1.0f), glm::vec3(0.4 * move, 0.6 * sinf(move), 0.0))
     * glm::rotate(glm::mat4(1.0f), glm::radians(angle), axis_z);
   }
   else 
   {
    m_transform = glm::translate(glm::mat4(1.0f), glm::vec3(0.4 * move, 0.6 * sinf(move), 0.0))
     * glm::rotate(glm::mat4(1.0f), -1.0f * glm::radians(angle), axis_z);
   }

  float red = sinf(glutGet(GLUT_ELAPSED_TIME) / 1000.0 * (2*3.14) * BPM / 60 / 2 ) / 4 + 0.5;
  float green = sinf(glutGet(GLUT_ELAPSED_TIME) / 1000.0 * (2*3.14) * BPM / 60 / 4) / 4 + 0.5;
  float blue = sinf(glutGet(GLUT_ELAPSED_TIME) / 1000.0 * (2*3.14) * BPM / 60 / 8 ) / 4 + 0.5;

  glUseProgram(program);

  glUniform1f(uniform_fade, cur_fade);
  glUniform1f(uniform_red, red);
  glUniform1f(uniform_green, green);
  glUniform1f(uniform_blue, blue);
  glUniformMatrix4fv(uniform_m_transform, 1, GL_FALSE, glm::value_ptr(m_transform));
  glutPostRedisplay();
}

void onDisplay()
{
  glClearColor(1.0, 1.0, 1.0, 1.0);
  glClear(GL_COLOR_BUFFER_BIT);
  
  glUseProgram(program);
  glEnableVertexAttribArray(attribute_coord3d);
  glEnableVertexAttribArray(attribute_v_color);
  glBindBuffer(GL_ARRAY_BUFFER, vbo_triangle);
  glVertexAttribPointer(
    attribute_coord3d,   // attribute
    3,                   // number of elements per vertex, here (x,y,z)
    GL_FLOAT,            // the type of each element
    GL_FALSE,            // take our values as-is
    sizeof(struct attributes),  // next coord3d appears every 5 floats
    0                    // offset of first element
  );
  glVertexAttribPointer(
    attribute_v_color,      // attribute
    3,                      // number of elements per vertex, here (r,g,b)
    GL_FLOAT,               // the type of each element
    GL_FALSE,               // take our values as-is
    sizeof(struct attributes),  // stride
    (void*) offsetof(struct attributes, v_color)  // offset
  );

  /* Push each element in buffer_vertices to the vertex shader */
  glDrawArrays(GL_TRIANGLES, 0, 3);

  glDisableVertexAttribArray(attribute_coord3d);
  glDisableVertexAttribArray(attribute_v_color);
  glutSwapBuffers();
}

void free_resources()
{
  glDeleteProgram(program);
  glDeleteBuffers(1, &vbo_triangle);
}


int main(int argc, char* argv[]) {
  glutInit(&argc, argv);
  glutInitContextVersion(2,0);
  glutInitDisplayMode(GLUT_RGBA|GLUT_ALPHA|GLUT_DOUBLE|GLUT_DEPTH);
  glutInitWindowSize(900, 900);
  glutCreateWindow("Fade, rotate, move Triangle");

  GLenum glew_status = glewInit();
  if (glew_status != GLEW_OK) {
    fprintf(stderr, "Error: %s\n", glewGetErrorString(glew_status));
    return 1;
  }

  if (!GLEW_VERSION_2_0) {
    fprintf(stderr, "Error: your graphic card does not support OpenGL 2.0\n");
    return 1;
  }

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

