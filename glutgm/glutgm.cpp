
#include <math.h>
#include <gl\glut.h>
#include <gl\gl.h>
#include <gl\glu.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>


float angle=0.0,deltaAngle = 0.0,ratio;
float x=0.0f,y=1.75f,z=5.0f;
float lx=0.0f,ly=0.0f,lz=-1.0f;
int deltaMove = 0,h,w;
int font=(int)GLUT_BITMAP_8_BY_13;
static GLint snowman_display_list;
int bitmapHeight=13;

int frame,time,timebase=0;
char s[60];
char currentMode[80];

// this string keeps the last good setting 
// for the game mode
char gameModeString[40] = "640x480";

void init();

void changeSize(int w1, int h1)
	{

	// Prevent a divide by zero, when window is too short
	// (you cant make a window of zero width).
	if(h1 == 0)
		h1 = 1;

	w = w1;
	h = h1;
	ratio = 1.0f * w / h;
	// Reset the coordinate system before modifying
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	
	// Set the viewport to be the entire window
    glViewport(0, 0, w, h);

	// Set the clipping volume
	gluPerspective(45,ratio,0.1,1000);

	// setting the camera now
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(x, y, z, 
		      x + lx,y + ly,z + lz,
			  0.0f,1.0f,0.0f);
	}


void drawSnowMan() {


	glColor3f(1.0f, 1.0f, 1.0f);

// Draw Body	
	glTranslatef(0.0f ,0.75f, 0.0f);
	glutSolidSphere(0.75f,20,20);


// Draw Head
	glTranslatef(0.0f, 1.0f, 0.0f);
	glutSolidSphere(0.25f,20,20);

// Draw Eyes
	glPushMatrix();
	glColor3f(0.0f,0.0f,0.0f);
	glTranslatef(0.05f, 0.10f, 0.18f);
	glutSolidSphere(0.05f,10,10);
	glTranslatef(-0.1f, 0.0f, 0.0f);
	glutSolidSphere(0.05f,10,10);
	glPopMatrix();

// Draw Nose
	glColor3f(1.0f, 0.5f , 0.5f);
	glRotatef(0.0f,1.0f, 0.0f, 0.0f);
	glutSolidCone(0.08f,0.5f,10,2);
}



GLuint createDL() {
	GLuint snowManDL;

	// Create the id for the list
	snowManDL = glGenLists(2);

	glNewList(snowManDL+1,GL_COMPILE);
		drawSnowMan();
	glEndList();
	// start list
	glNewList(snowManDL,GL_COMPILE);

	// call the function that contains the rendering commands
	for(int i = -3; i < 3; i++)
		for(int j=-3; j < 3; j++) {
			glPushMatrix();
			glTranslatef(i*10.0,0,j * 10.0);
			glCallList(snowManDL+1);
			glPopMatrix();
		}

	// endList
	glEndList();

	return(snowManDL);
}

void initScene() {

	glEnable(GL_DEPTH_TEST);
	snowman_display_list = createDL();

}

void orientMe(float ang) {


	lx = sin(ang);
	lz = -cos(ang);
	glLoadIdentity();
	gluLookAt(x, y, z, 
		      x + lx,y + ly,z + lz,
			  0.0f,1.0f,0.0f);
}


void moveMeFlat(int i) {
	x = x + i*(lx)*0.1;
	z = z + i*(lz)*0.1;
	glLoadIdentity();
	gluLookAt(x, y, z, 
		      x + lx,y + ly,z + lz,
			  0.0f,1.0f,0.0f);
}

void setOrthographicProjection() {

	// switch to projection mode
	glMatrixMode(GL_PROJECTION);
	// save previous matrix which contains the 
	//settings for the perspective projection
	glPushMatrix();
	// reset matrix
	glLoadIdentity();
	// set a 2D orthographic projection
	gluOrtho2D(0, w, 0, h);
	// invert the y axis, down is positive
	glScalef(1, -1, 1);
	// mover the origin from the bottom left corner
	// to the upper left corner
	glTranslatef(0, -h, 0);
	glMatrixMode(GL_MODELVIEW);
}

void resetPerspectiveProjection() {
	// set the current matrix to GL_PROJECTION
	glMatrixMode(GL_PROJECTION);
	// restore previous settings
	glPopMatrix();
	// get back to GL_MODELVIEW matrix
	glMatrixMode(GL_MODELVIEW);
}

void renderBitmapString(float x, float y, void *font,char *string)
{
  
  char *c;
  // set position to start drawing fonts
  glRasterPos2f(x, y);
  // loop all the characters in the string
  for (c=string; *c != '\0'; c++) {
    glutBitmapCharacter(font, *c);
  }
}


void renderScene(void) {

	if (deltaMove)
		moveMeFlat(deltaMove);
	if (deltaAngle) {
		angle += deltaAngle;
		orientMe(angle);
	}

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

// Draw ground

	glColor3f(0.9f, 0.9f, 0.9f);
	glBegin(GL_QUADS);
		glVertex3f(-100.0f, 0.0f, -100.0f);
		glVertex3f(-100.0f, 0.0f,  100.0f);
		glVertex3f( 100.0f, 0.0f,  100.0f);
		glVertex3f( 100.0f, 0.0f, -100.0f);
	glEnd();

// Draw 36 SnowMen

	glCallList(snowman_display_list);
	
	frame++;
	time=glutGet(GLUT_ELAPSED_TIME);
	if (time - timebase > 1000) {
		sprintf(s,"FPS:%4.2f",frame*1000.0/(time-timebase));
		timebase = time;		
		frame = 0;
	}

	glColor3f(0.0f,1.0f,1.0f);
	setOrthographicProjection();
	glPushMatrix();
	glLoadIdentity();
	renderBitmapString(30,15,(void *)font,"GLUT Tutorial @ 3D Tech"); 
	renderBitmapString(30,30,(void *)font,s);
	renderBitmapString(30,45,(void *)font,"F1 - Game Mode  640x480 32 bits");
	renderBitmapString(30,60,(void *)font,"F2 - Game Mode  800x600 32 bits");
	renderBitmapString(30,75,(void *)font,"F3 - Game Mode 1024x768 32 bits");
	renderBitmapString(30,90,(void *)font,"F4 - Game Mode 1280x1024 32 bits");
	renderBitmapString(30,105,(void *)font,"F5 - Game Mode 1600x1200 32 bits");
	renderBitmapString(30,120,(void *)font,"F6 - Window Mode");
	renderBitmapString(30,135,(void *)font,"Esc - Quit");
	renderBitmapString(30,150,(void *)font,currentMode);
	glPopMatrix();
	resetPerspectiveProjection();

	glutSwapBuffers();
}

void processNormalKeys(unsigned char key, int x, int y) {

	if (key == 27) 
		exit(0);
}

void pressKey(int key, int x, int y) {

	switch (key) {
		case GLUT_KEY_LEFT : deltaAngle = -0.01f;break;
		case GLUT_KEY_RIGHT : deltaAngle = 0.01f;break;
		case GLUT_KEY_UP : deltaMove = 1;break;
		case GLUT_KEY_DOWN : deltaMove = -1;break;
		case GLUT_KEY_F1:  
			
			// define resolution, color depth
			glutGameModeString("640x480:32");
			// enter full screen
			if (glutGameModeGet(GLUT_GAME_MODE_POSSIBLE)) {
				glutEnterGameMode();
				sprintf(gameModeString,"640x480:32");
				// register callbacks again 
				// and init OpenGL context
				init();
			}
			else
				glutGameModeString(gameModeString);
			break;
		case GLUT_KEY_F2:     
			// define resolution, color depth
			glutGameModeString("800x600:32");
			// enter full screen
			if (glutGameModeGet(GLUT_GAME_MODE_POSSIBLE)) {
				glutEnterGameMode();
				sprintf(gameModeString,"800x600:32");
				// register callbacks again 
				// and init OpenGL context
				init();
			}
			else
				glutGameModeString(gameModeString);
			break;
		case GLUT_KEY_F3:  
			// define resolution, color depth
			glutGameModeString("1024x768:32");
			// enter full screen
			if (glutGameModeGet(GLUT_GAME_MODE_POSSIBLE)) {
				glutEnterGameMode();
				sprintf(gameModeString,"1024x768:32");
				// register callbacks again 
				// and init OpenGL context
				init();
			}
			else
				glutGameModeString(gameModeString);
			break;
		case GLUT_KEY_F4:  
			// define resolution, color depth
			glutGameModeString("1280x1024:32");
			// enter full screen
			if (glutGameModeGet(GLUT_GAME_MODE_POSSIBLE)) {
				glutEnterGameMode();
				sprintf(gameModeString,"1280x1024:32");
				// register callbacks again 
				// and init OpenGL context
				init();
			}
			else
				glutGameModeString(gameModeString);
			break;
		case GLUT_KEY_F5:  
			// define resolution, color depth
			glutGameModeString("1600x1200");
			// enter full screen
			if (glutGameModeGet(GLUT_GAME_MODE_POSSIBLE)) {
				glutEnterGameMode();
				sprintf(gameModeString,"1600x1200");
				// register callbacks again 
				// and init OpenGL context
				init();
			}
			else
				glutGameModeString(gameModeString);
			break;
		case GLUT_KEY_F6: 
			// return to default window
			w = 640;h = 360;
			if (glutGameModeGet(GLUT_GAME_MODE_ACTIVE) != 0)
				glutLeaveGameMode();
			break;
	}
	if (glutGameModeGet(GLUT_GAME_MODE_ACTIVE) == 0)
		sprintf(currentMode,"Current Mode: Window");
	else
		sprintf(currentMode,
			"Current Mode: Game Mode %dx%d at %d hertz, %d bpp",
			glutGameModeGet(GLUT_GAME_MODE_WIDTH),
			glutGameModeGet(GLUT_GAME_MODE_HEIGHT),
			glutGameModeGet(GLUT_GAME_MODE_REFRESH_RATE),
			glutGameModeGet(GLUT_GAME_MODE_PIXEL_DEPTH));
}

void releaseKey(int key, int x, int y) {

	switch (key) {
		case GLUT_KEY_LEFT : if (deltaAngle < 0.0f) 
								 deltaAngle = 0.0f;
							 break;
		case GLUT_KEY_RIGHT : if (deltaAngle > 0.0f) 
								 deltaAngle = 0.0f;
							 break;
		case GLUT_KEY_UP :	 if (deltaMove > 0) 
								 deltaMove = 0;
							 break;
		case GLUT_KEY_DOWN : if (deltaMove < 0) 
								 deltaMove = 0;
							 break;
	}
}

void init() {
	glutIgnoreKeyRepeat(1);
	glutKeyboardFunc(processNormalKeys);
	glutSpecialFunc(pressKey);
	glutSpecialUpFunc(releaseKey);
	glutDisplayFunc(renderScene);
	glutIdleFunc(renderScene);
	glutReshapeFunc(changeSize);
	initScene();

}

int main(int argc, char **argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowPosition(100,100);
	glutInitWindowSize(640,360);
	glutCreateWindow("SnowMen from 3D-Tech");

	// register all callbacks and
	// create display lists
	init();

	glutMainLoop();

	return(0);
}
