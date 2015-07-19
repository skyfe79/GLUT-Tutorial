#include<gl/glut.h>
#include<gl/glu.h>
#include<gl/gl.h>
#include<stdio.h>


#define RED 11
#define GREEN 12
#define BLUE 13
#define WHITE 14
#define CHANGE_TO_1 15
#define CHANGE_TO_2 16


float angle = 0.0;
float red=1.0, blue=1.0, green=1.0;
int menu1,menu2,menu,flag;

void changeSize(int w, int h) {

	// Prevent a divide by zero, when window is too short
	// (you cant make a window of zero width).
	if(h == 0)
		h = 1;

	float ratio = 1.0* w / h;

	// Reset the coordinate system before modifying
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	
	// Set the viewport to be the entire window
    glViewport(0, 0, w, h);

	// Set the correct perspective.
	gluPerspective(45,ratio,1,1000);
	gluLookAt(0.0,0.0,5.0, 
		      0.0,0.0,-1.0,
			  0.0f,1.0f,0.0f);
	glMatrixMode(GL_MODELVIEW);


}


void renderScene(void) {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();
	glRotatef(angle,0.0,1.0,0.0);

	glColor3f(red,green,blue);

	glBegin(GL_TRIANGLES);
		glVertex3f(-0.5,-0.5,0.0);
		glVertex3f(0.5,0.0,0.0);
		glVertex3f(0.0,0.5,0.0);
	glEnd();
	angle++;
	glutSwapBuffers();
}



void processMenuEvents(int option) {

	red = 0.0;
	green = 0.0;
	blue = 0.0;
	switch (option) {
		case RED : red = 1.0; break;
		case GREEN : green = 1.0; break;
		case BLUE : blue = 1.0; break;
		case WHITE : red = 1.0; 
				green = 1.0; 
				blue = 1.0; break;
	}
}

void processSpecialKeys(int c, int x, int y) {
	
	if (!flag) {
		if (c == GLUT_KEY_F1) {
			int x = glutGetMenu();
			if (x == menu1)
				glutSetMenu(menu2);
			else
				glutSetMenu(menu1);
			glutAttachMenu(GLUT_RIGHT_BUTTON);
		}
	}
}

void createGLUTMenus() {

	menu2 = glutCreateMenu(processMenuEvents);
	glutAddMenuEntry("Blue",BLUE);
	glutAddMenuEntry("Green",GREEN);
	glutAddMenuEntry("Red",RED);

	menu1 = glutCreateMenu(processMenuEvents);
	glutAddMenuEntry("Red",RED);
	glutAddMenuEntry("Green",GREEN);
	glutAddMenuEntry("Blue",BLUE);
	glutAttachMenu(GLUT_RIGHT_BUTTON);
}

void processMenuStatus(int status, int x, int y) {

	if (status == GLUT_MENU_IN_USE)
		flag = 1;
	else
		flag = 0;
}

void main(int argc, char **argv) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowPosition(100,100);
	glutInitWindowSize(320,320);
	glutCreateWindow("SnowMen");
	glutDisplayFunc(renderScene);
	glutIdleFunc(renderScene);
	glutReshapeFunc(changeSize);


	createGLUTMenus();

	glutMenuStatusFunc(processMenuStatus);

	glutSpecialFunc(processSpecialKeys);

	glutMainLoop();
}

