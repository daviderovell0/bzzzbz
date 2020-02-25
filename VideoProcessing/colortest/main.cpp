/*This source code copyrighted by Lazy Foo' Productions (2004-2013)
and may not be redistributed without written permission.*/
//Version: 001

#include "LUtil.h"

void runMainLoop( int val ); //this actually comes later...
/*
Pre Condition:
 -Initialized freeGLUT
Post Condition:
 -Calls the main loop functions and sets itself to be called back in 1000 / SCREEN_FPS milliseconds
Side Effects:
 -Sets glutTimerFunc
*/

int main( int argc, char* args[] )
{
	//Initialize FreeGLUT
	glutInit( &argc, args );

	//Create OpenGL 2.1 context
	glutInitContextVersion( 2, 1 ); //so that not modern OpenGL etc

	//Create Double Buffered Window
	glutInitDisplayMode( GLUT_DOUBLE ); //there are a few arguments available...
	glutInitWindowSize( SCREEN_WIDTH, SCREEN_HEIGHT ); //gets from header
	glutCreateWindow( "Second test" );

	//Do post window/context creation initialization
	if( !initGL() )
	{
		printf( "Unable to initialize graphics library!\n" );
		return 1;
	}

    //Set keyboard function
	glutKeyboardFunc( handleKeys );

	//Set rendering function
	glutDisplayFunc( render ); //not entirely sure, "render" here creates the triangle and swaps front/back

	//Set main loop
	glutTimerFunc( 1000 / SCREEN_FPS, runMainLoop, 0 ); //at least every 1000/FPS, runMainLoop is called

	//Start GLUT main loop
	glutMainLoop();

	return 0;
}

void runMainLoop( int val )
{
    //Frame logic
    update();
    render();

    //Run frame one more time
    glutTimerFunc( 1000 / SCREEN_FPS, runMainLoop, val );
}
