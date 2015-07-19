
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
int deltaMove = 0,h=400,w=400, border=6;
int font=(int)GLUT_BITMAP_8_BY_13;
static GLint snowman_display_list;
int bitmapHeight=13;

int frame,time,timebase=0;
char s[30];

int mainWindow, subWindow1,subWindow2,subWindow3;

void initWindow();

void changeSize2(int w1, int h1)
{

	// Prevent a divide by zero, when window is too short
	// (you cant make a window of zero width).
	ratio = 1.0f * w1 / h1;
	// Reset the coordinate system before modifying
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	
	// Set the viewport to be the entire window
    glViewport(0, 0, w1, h1);

	// Set the clipping volume
	gluPerspective(45,ratio,0.1,1000);
	glMatrixMode(GL_MODELVIEW);
}

void changeSize(int w1,int h1) {
	if(h1 == 0)
		h1 = 1;

	w = w1;
	h = h1;

	glutSetWindow(subWindow1);
	glutPositionWindow(border,border);
	glutReshapeWindow(w-2*border, h/2 - border*3/2);
	changeSize2(w-2*border, h/2 - border*3/2);

	glutSetWindow(subWindow2);
	glutPositionWindow(border,(h+border)/2);
	glutReshapeWindow(w/2-border*3/2, h/2 - border*3/2);
	changeSize2(w/2-border*3/2,h/2 - border*3/2);

	glutSetWindow(subWindow3);
	glutPositionWindow((w+border)/2,(h+border)/2);
	glutReshapeWindow(w/2-border*3/2,h/2 - border*3/2);
	changeSize2(w/2-border*3/2,h/2 - border*3/2);
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


void setOrthographicProjection() {

	// switch to projection mode
	glMatrixMode(GL_PROJECTION);
	// save previous matrix which contains the 
	//settings for the perspective projection
	glPushMatrix();
	// reset matrix
	glLoadIdentity();
	// set a 2D orthographic projection
	gluOrtho2D(0, w, 0, h/2);
	// invert the y axis, down is positive
	glScalef(1, -1, 1);
	// mover the origin from the bottom left corner
	// to the upper left corner
	glTranslatef(0, -h/2, 0);
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



void initScene() {

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);

	snowman_display_list = createDL();
}

void orientMe(float ang) {


	lx = sin(ang);
	lz = -cos(ang);
}


void moveMeFlat(int i) {
	x = x + i*(lx)*0.1;
	z = z + i*(lz)*0.1;
}


void renderScene2(int currentWindow) {


	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	


	glPushMatrix();
		glColor3f(1.0,0.0,0.0);
		glTranslatef(x,y,z);
		glRotatef(180-angle*180.0/3.14,0.0,1.0,0.0);
		glutSolidCone(0.2,0.8f,4,4);
	glPopMatrix();

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
	
	if (currentWindow == subWindow1)
	{
		frame++;
		time=glutGet(GLUT_ELAPSED_TIME);
		if (time - timebase > 1000) {
			sprintf(s,"FPS: %4.2f",frame*1000.0/(time-timebase));
			timebase = time;		
			frame = 0;
		}
		glColor3f(0.0,1.0,1.0);
		setOrthographicProjection();
		glPushMatrix();
		glLoadIdentity();
		renderBitmapString(30,15,(void *)font,"GLUT Tutorial @ 3D Tech"); 
		renderBitmapString(30,35,(void *)font,s);
		renderBitmapString(30,55,(void *)font,"Esc - Quit");		
		glPopMatrix();
		resetPerspectiveProjection();
	}
	glutSwapBuffers();
}

void renderScene() {
	glutSetWindow(mainWindow);
	glClear(GL_COLOR_BUFFER_BIT);
	glutSwapBuffers();
}

void renderScenesw1() {
	glutSetWindow(subWindow1);
	glLoadIdentity();
	gluLookAt(x, y, z, 
		      x + lx,y + ly,z + lz,
			  0.0f,1.0f,0.0f);

	renderScene2(subWindow1);
}

void renderScenesw2() {
	glutSetWindow(subWindow2);
	glLoadIdentity();
	gluLookAt(x, y+15, z, 
		      x ,y - 1,z,
			  lx,0,lz);
	renderScene2(subWindow2);
}

void renderScenesw3() {
	glutSetWindow(subWindow3);
	glLoadIdentity();
	gluLookAt(x-lz*10 , y, z+lx*10, 
		      x ,y ,z ,
			  0.0f,1.0f,0.0f);
	renderScene2(subWindow3);
}

void renderSceneAll() {
	if (deltaMove)
		moveMeFlat(deltaMove);
	if (deltaAngle) {
		angle += deltaAngle;
		orientMe(angle);
	}
//	renderScene();
	renderScenesw1();
	renderScenesw2();
	renderScenesw3();
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
	}

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



int main(int argc, char **argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowPosition(100,100);
	glutInitWindowSize(w,h);
	mainWindow = glutCreateWindow("SnowMen from 3D-Tech");
	glutIgnoreKeyRepeat(1);
	glutKeyboardFunc(processNormalKeys);
	glutSpecialFunc(pressKey);
	glutSpecialUpFunc(releaseKey);
	glutReshapeFunc(changeSize);
	glutDisplayFunc(renderScene);
	glutIdleFunc(renderSceneAll);

	subWindow1 = glutCreateSubWindow(mainWindow, border,border,w-2*border, h/2 - border*3/2);
	glutDisplayFunc(renderScene);
	initScene();

	subWindow2 = glutCreateSubWindow(mainWindow, border,(h+border)/2,w/2-border*3/2, h/2 - border*3/2);
	glutDisplayFunc(renderScenesw2);
	initScene();

	subWindow3 = glutCreateSubWindow(mainWindow, (w+border)/2,(h+border)/2,w/2-border*3/2,h/2 - border*3/2);
	glutDisplayFunc(renderScenesw3);
	initScene();

	glutMainLoop();

	return(0);
}
