
#include <math.h>
#include <gl\glut.h>
#include <gl\gl.h>
#include <gl\glu.h>
#include <string.h>
#include <stdlib.h>


float angle=0.0,deltaAngle = 0.0,ratio;
float x=0.0f,y=1.75f,z=30.0f;
float lx=0.0f,ly=0.0f,lz=-1.0f;
int deltaMove = 0;
int font=(int)GLUT_STROKE_ROMAN;


void changeSize(int w, int h)
	{

	// Prevent a divide by zero, when window is too short
	// (you cant make a window of zero width).
	if(h == 0)
		h = 1;

	ratio = 1.0f * w / h;
	// Reset the coordinate system before modifying
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	
	// Set the viewport to be the entire window
    glViewport(0, 0, w, h);

	// Set the clipping volume
	gluPerspective(45,ratio,1,10000);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(x, y, z, 
		      x + lx,y + ly,z + lz,
			  0.0f,1.0f,0.0f);


	}



void initScene() {

	glEnable(GL_DEPTH_TEST);
	glLineWidth(4.0);

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


void renderStrokeCharacter(float x, float y, float z, void *font,char *string)
{
  
  char *c;
  glPushMatrix();
  glTranslatef(x, y, z);
  for (c=string; *c != '\0'; c++) {
    glutStrokeCharacter(font, *c);
  }
  glPopMatrix();
}

void renderScene(void) {

	if (deltaMove)
		moveMeFlat(deltaMove);
	if (deltaAngle) {
		angle += deltaAngle;
		orientMe(angle);
	}

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glPushMatrix();

	renderStrokeCharacter(-400,150,-800,(void *)font,"3D Tech");
	renderStrokeCharacter(-400,0,  -800,(void *)font,"GLUT Tutorial");

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
}


void createMenus() {
	int menu = glutCreateMenu(processMenuEvents);

	glutAddMenuEntry("Roman",(int)GLUT_STROKE_ROMAN);
	glutAddMenuEntry("Mono Roman",(int)GLUT_STROKE_MONO_ROMAN);
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
