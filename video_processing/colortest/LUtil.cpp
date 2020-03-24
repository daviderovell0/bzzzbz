/*This source code copyrighted by Lazy Foo' Productions (2004-2013)
and may not be redistributed without written permission.*/
//Version: 001

//modes are GL_*mode*, MODELVIEW PROJECTION TEXTURE COLOR

#include "LUtil.h"

//-the current color rendering mode, cyan or multi-color triangle
int gColorMode = COLOR_MODE_CYAN;

//The projection scale, how large render
GLfloat gProjectionScale = 1.f;

bool initGL()
{
    //Initialize Projection Matrix
    glMatrixMode( GL_PROJECTION ); //specify current matrix {operation are done on this matrix}
    glLoadIdentity(); //replace current matrix w Identity matrix
    glOrtho( 0.0, SCREEN_WIDTH, SCREEN_HEIGHT, 0.0, 1.0, -1.0 ); //projection matrix multiplied by this

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

    //Reset modelview matrix
    glMatrixMode( GL_MODELVIEW );
    glLoadIdentity();

    //Move to center of the screen
    glTranslatef( SCREEN_WIDTH / 2.f , SCREEN_HEIGHT / 2.f, 0.f );


    //Render triangle
    if( gColorMode == COLOR_MODE_CYAN )
    {
        //Solid cyan
        glBegin( GL_TRIANGLES ); //delimit vertices of primitive(s), specify type, points, lines
            glColor3f( 0.f, 1.f, 1.f ); //last color is used always            
            glVertex2f( -50.f, -50.f ); //glVertex2f sets z=0, glVertex3f is 3D
            glVertex2f(  50.f, -50.f );
            glVertex2f(  50.f,  50.f );
        glEnd();
    }
    else
    {
       //Multi
        glBegin( GL_TRIANGLES ); 
            glColor3f( 1.f, 0.f, 0.f ); glVertex2f( -50.f, -50.f );        
            glColor3f( 0.f, 1.f, 0.f ); glVertex2f(  50.f, -50.f );
            glColor3f( 0.f, 0.f, 1.f );  glVertex2f(  50.f,  50.f );
        glEnd();
    }


    //Update screen
    glutSwapBuffers();
}

void handleKeys( unsigned char key, int x, int y )
{
    //If the user presses Q
    if( key == 'q' )
    {
        //Toggle color mode
        if( gColorMode == COLOR_MODE_CYAN )
        {
            gColorMode = COLOR_MODE_MULTI;
        }
        else
        {
            gColorMode = COLOR_MODE_CYAN;
        }
    }
    else if ( key == 'e' || key == 'r' )
    {
        if( key == 'e' )
        {
            //Cycle through projection scales
            if( gProjectionScale == 1.f )
            {
                //Zoom out
                gProjectionScale = 2.f;
            }
            else if( gProjectionScale = 2.f)
            {
                //Zoom out more
                gProjectionScale = 3.f;
            }
            else if( gProjectionScale = 0.5f)
            {
                //Zoom out 
                gProjectionScale = 1.f;
            }
        }
        else if( key == 'r' )
        {
            //Cycle through projection scales
            if( gProjectionScale == 3.f )
            {
                //Zoom in
                gProjectionScale = 2.f;
            }
            else if( gProjectionScale = 2.f)
            {
                //Zoom in more
                gProjectionScale = 1.f;
            }
            else if ( gProjectionScale = 1.f)
            {
                //Zoom in more
                gProjectionScale = 0.5f;
            }
        }
    //Update projection matrix
    glMatrixMode( GL_PROJECTION );
    glLoadIdentity();
    glOrtho( 0.0, SCREEN_WIDTH * gProjectionScale, SCREEN_HEIGHT * gProjectionScale, 0.0, 1.0, -1.0 ); 
    }
 }
