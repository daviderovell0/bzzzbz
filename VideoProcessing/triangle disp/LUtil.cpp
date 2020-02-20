/*This source code copyrighted by Lazy Foo' Productions (2004-2013)
and may not be redistributed without written permission.*/
//Version: 001

//modes are GL_mode, MODELVIEW PROJECTION TEXTURE COLOR

#include "LUtil.h"

bool initGL()
{
    //Initialize Projection Matrix
    glMatrixMode( GL_PROJECTION ); //specify current matrix {operation are done on this matrix}
    glLoadIdentity(); //replace current matrix w Identity matrix

    //Initialize Modelview Matrix
    glMatrixMode( GL_MODELVIEW );
    glLoadIdentity();

    //Initialize clear color
    glClearColor( 0.f, 0.f, 0.f, 1.f ); //init the clear values for buffer, RGBA

    //Check for error
    GLenum error = glGetError(); //if glGetError = 0 than that generates error
    if( error != GL_NO_ERROR ) //if error =/= 0 (GL_NO_ERROR)
    {
        printf( "Error initializing OpenGL! %s\n", gluErrorString( error ) ); //produces string from error code in error
        return false;
    }

    return true;
}

void update() //this will be needed for moving image (buffer update)
{

}

void render()
{
    //Clear color buffer
    glClear( GL_COLOR_BUFFER_BIT ); //clear the indicated buffer, COLOR, DEPTH, STENCIL

    //Render triangle
    glBegin( GL_TRIANGLES ); //delimit vertices of primitive(s), specify type, points, lines
        glVertex2f( -0.5f, -0.5f ); //glVertex2f sets z=0, glVertex3f is 3D
        glVertex2f(  0.5f, -0.5f );
        glVertex2f(  0.5f,  0.5f );
    glEnd();

    //Update screen
    glutSwapBuffers();
}
