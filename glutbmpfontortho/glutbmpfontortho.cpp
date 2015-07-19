
#include <math.h>
#include <gl\glut.h>
#include <gl\gl.h>
#include <gl\glu.h>
#include <string.h>
#include <stdlib.h>


float angle=0.0,deltaAngle = 0.0,ratio;
float x=0.0f,y=1.75f,z=5.0f;
float lx=0.0f,ly=0.0f,lz=-1.0f;
int deltaMove = 0,h,w;
int font=(int)GLUT_BITMAP_8_BY_13;
static GLint snowman_display_list;
int bitmapHeight=13;




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
	snowManDL = glGenLists(1);

	// start list
	glNewList(snowManDL,GL_COMPILE);

	// call the function that contains the rendering commands
		drawSnowMan();

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
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);
}

void renderBitmapString(float x, float y, void *font,char *string)
{
  
  char *c;
  glRasterPos2f(x, y);
  for (c=string; *c != '\0'; c++) {
    glutBitmapCharacter(font, *c);
  }
}

void renderSpacedBitmapString(float x, float y,int spacing, void *font,char *string) {
  char *c;
  int x1=x;
  for (c=string; *c != '\0'; c++) {
	glRasterPos2f(x1,y);
    glutBitmapCharacter(font, *c);
	x1 = x1 + glutBitmapWidth(font,*c) + spacing;
  }
}


void renderVerticalBitmapString(float x, float y, int bitmapHeight, void *font,char *string)
{
  
  char *c;
  int i;
  for (c=string,i=0; *c != '\0'; i++,c++) {
	glRasterPos2f(x, y+bitmapHeight*i);
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
	glPushMatrix();
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

	for(int i = -3; i < 3; i++)
		for(int j=-3; j < 3; j++) {
			glPushMatrix();
			glTranslatef(i*10.0,0,j * 10.0);
			glCallList(snowman_display_list);;
			glPopMatrix();
		}


	glPushMatrix();
	glColor3f(0.0f,1.0f,1.0f);
	setOrthographicProjection();
	glLoadIdentity();
	renderSpacedBitmapString(30,30,10,(void *)font,"3D Tech");
	renderVerticalBitmapString(30,30,bitmapHeight,(void *)font,"3D Tech");
	renderBitmapString(60,60,(void *)font,"GLUT Tutorial");
	resetPerspectiveProjection();
	glPopMatrix();
	glPopMatrix();
	glutSwapBuffers();
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
		case GLUT_KEY_LEFT : 
		case GLUT_KEY_RIGHT : deltaAngle = 0.0f;break;
		case GLUT_KEY_UP : 
		case GLUT_KEY_DOWN : deltaMove = 0;break;
	}
}

void processMenuEvents(int option) {

		font = option;
		switch(font) {
			case GLUT_BITMAP_8_BY_13: bitmapHeight = 13; break;
			case GLUT_BITMAP_9_BY_15: bitmapHeight = 15; break;
			case GLUT_BITMAP_TIMES_ROMAN_10: bitmapHeight = 10; break;
			case GLUT_BITMAP_TIMES_ROMAN_24: bitmapHeight = 24; break;
			case GLUT_BITMAP_HELVETICA_10: bitmapHeight = 10; break;
			case GLUT_BITMAP_HELVETICA_12: bitmapHeight = 12; break;
			case GLUT_BITMAP_HELVETICA_18: bitmapHeight = 18; break;
		}
}


void createMenus() {
	int menu = glutCreateMenu(processMenuEvents);

	glutAddMenuEntry("8 by 13",(int)GLUT_BITMAP_8_BY_13);
	glutAddMenuEntry("9 by 15",(int)GLUT_BITMAP_9_BY_15);
	glutAddMenuEntry("Times Roman 10",(int)GLUT_BITMAP_TIMES_ROMAN_10);
	glutAddMenuEntry("Times Roman 24",(int)GLUT_BITMAP_TIMES_ROMAN_24);
	glutAddMenuEntry("Helvetica 10",(int)GLUT_BITMAP_HELVETICA_10);
	glutAddMenuEntry("Helvetica 12",(int)GLUT_BITMAP_HELVETICA_12);
	glutAddMenuEntry("Helvetica 18",(int)GLUT_BITMAP_HELVETICA_18);
	glutAttachMenu(GLUT_RIGHT_BUTTON);
}

void processNormalKeys(unsigned char key, int x, int y) {

	if (key == 27) 
		exit(0);
}

int main(int argc, char **argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowPosition(100,100);
	glutInitWindowSize(640,360);
	glutCreateWindow("SnowMen from 3D-Tech");

	initScene();

	glutKeyboardFunc(processNormalKeys);
	glutIgnoreKeyRepeat(1);
	glutSpecialFunc(pressKey);
	glutSpecialUpFunc(releaseKey);
	createMenus();
	glutDisplayFunc(renderScene);
	glutIdleFunc(renderScene);

	glutReshapeFunc(changeSize);

	glutMainLoop();

	return(0);
}
