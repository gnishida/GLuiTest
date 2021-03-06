#ifdef _WIN32
#include <windows.h>
#endif
#include <stdio.h>
#include <GL/glut.h>
#include "OBJLoader.h"
#include <GL/glui.h>

static float FOV = 60.0;
static float nearZ = 0.1;
static float farZ = 100.0;
static int winWidth = 512;
static int winHeight = 512;
static int winId = -1;
static GLUI *glui;
static std::vector<Vertex> vertices;
float view_rotate[16] = { 1,0,0,0, 0,1,0,0, 0,0,1,0, 0,0,0,1 };
float trans_z[] = { 10 };

/*****************************************************************************
*****************************************************************************/
static void
leftButtonDownCB(void)
{
   printf("left down!\n");
}

/*****************************************************************************
*****************************************************************************/
static void
leftButtonUpCB(void)
{
   printf("left up!\n");
}

/*****************************************************************************
*****************************************************************************/
static void
middleButtonDownCB(void)
{
   printf("middle down!\n");
}


/*****************************************************************************
*****************************************************************************/
static void
middleButtonUpCB(void)
{
   printf("middle up!\n");
}

/*****************************************************************************
*****************************************************************************/
static void
rightButtonDownCB(void)
{
   printf("right down!\n");
}


/*****************************************************************************
*****************************************************************************/
static void
rightButtonUpCB(void)
{
   printf("right up!\n");
}

/*****************************************************************************
*****************************************************************************/
static void
mouseCB(int button, int state, int x, int y)
{
   /* invert y so that ymax is up */
   int y2 = (winHeight - y - 1);

   printf("mouse @ (%i, %i)\n", x, y2);

   if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
      leftButtonDownCB();
   else if (button == GLUT_LEFT_BUTTON && state == GLUT_UP)
      leftButtonUpCB();
   else if (button == GLUT_MIDDLE_BUTTON && state == GLUT_DOWN)
      middleButtonDownCB();
   else if (button == GLUT_MIDDLE_BUTTON && state == GLUT_UP)
      middleButtonUpCB();
   else if (button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN)
      rightButtonDownCB();
   else if (button == GLUT_RIGHT_BUTTON && state == GLUT_UP)
      rightButtonUpCB();
}


/*****************************************************************************
*****************************************************************************/
static void
motionCB(int x, int y)
{
   /* invert y so that ymax is up */
   int y2 = (winHeight - y - 1);

   printf("mouse moved to (%i, %i)\n", x, y2);
}


/*****************************************************************************
*****************************************************************************/
void
reshapeCB(int width, int height)
{
	float aspect = (float)width/(float)height;

	winWidth = width;
	winHeight = height;
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(FOV, aspect, nearZ, farZ);
	glMatrixMode(GL_MODELVIEW);

	printf("Window now of size %i x %i\n", winWidth, winHeight);
}

/*****************************************************************************
*****************************************************************************/
void
keyboardCB(unsigned char key, int x, int y)
{
	/* invert y so that ymax is up */
	int y2 = (winHeight - y - 1);

	printf("key '%c' pressed at (%i, %i)\n", key, x, y2);
}

/*****************************************************************************
*****************************************************************************/
void
idleFunc()
{		
	glutPostRedisplay();
}

/*****************************************************************************
*****************************************************************************/
void drawScene()
{
	glBegin(GL_TRIANGLES);
	for (int i = 0; i < vertices.size(); i+=3) {
		for (int j = 0; j < 3; ++j) {
			glColor3f(vertices[i + j].color[0],vertices[i + j].color[1], vertices[i + j].color[2]);
			glNormal3f(vertices[i + j].normal[0],vertices[i + j].normal[1], vertices[i + j].normal[2]);
			glVertex3f(vertices[i + j].position[0],vertices[i + j].position[1], vertices[i + j].position[2]);
		}
	}
	glEnd();
}

/*****************************************************************************
*****************************************************************************/
void
refreshCB()
{
	glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

	// render geometry
	glPushMatrix();
	glTranslatef(0, 0, -trans_z[0]);
	glMultMatrixf( view_rotate );

	drawScene();
	glPopMatrix();

	// let's see it!
	glutSwapBuffers();
}

/*****************************************************************************
*****************************************************************************/
char *
readFile(char *fileName)
{
	char *text = new char[10000];
	char line[256];
	FILE *fp;
	fopen_s(&fp, fileName, "r");
	if (!fp) return NULL;

	text[0] = '\0';
	while (fgets(line,256,fp)) {
		strcat_s(text, 10000, line);
	}
	
	fclose(fp);

	return (text);
}

/*****************************************************************************
*****************************************************************************/
void MakeGUI()
{
	glui = GLUI_Master.create_glui("GUI", 0, 0, 0);
	glui->add_statictext( "Simple GLUI Example" );
	glui->add_rotation("Rotation", view_rotate);
	glui->add_translation("Zoom in/out", GLUI_TRANSLATION_Z, trans_z);

	glui->set_main_gfx_window(winId);

	/* We register the idle callback with GLUI, *not* with GLUT */
	GLUI_Master.set_glutIdleFunc(idleFunc);
}

/*****************************************************************************
*****************************************************************************/
int
main(int argc, char *argv[])
{
	// init OpenGL/GLUT
	glutInit(&argc, argv);
	
	// create main window
	glutInitWindowPosition(0, 0);
	glutInitWindowSize(winWidth, winHeight);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
	winId = glutCreateWindow("MyWindow");
	
	glClearColor(0,0,0,1);

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glEnable(GL_COLOR_MATERIAL);

	static GLfloat lightPosition[4] = {0.0f, 0.0f, 100.0f, 0.0f};
	glLightfv(GL_LIGHT0, GL_POSITION, lightPosition);

	// setup callbacks
	glutDisplayFunc(refreshCB);
	glutReshapeFunc(reshapeCB);
	glutKeyboardFunc(keyboardCB);
	glutMouseFunc(mouseCB);
	glutMotionFunc(motionCB);

	// load OBJ
	OBJLoader::load("models/cow.obj", vertices);

	// force initial matrix setup
	reshapeCB(winWidth, winHeight);

	// set modelview matrix stack to identity
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	// make GLUI GUI
	MakeGUI();
	glutMainLoop();

	return (TRUE);
}
