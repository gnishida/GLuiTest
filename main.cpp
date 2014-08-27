#ifdef _WIN32
#include <windows.h>
#endif
#include <iostream>
#include <math.h>
#include <stdio.h>
#include "GLee.h"
#include <GL/glut.h>
#include "SOIL.h"


static float FOV = 60.0;
static float nearZ = 0.1;
static float farZ = 100.0;
static int winWidth = 512;
static int winHeight = 512;
static int winId = -1;
static float angle = 0.0;
static GLchar *vertex_source = NULL;
static GLchar *fragment_source = NULL;
static GLuint program_object;
static GLuint vertex_shader, fragment_shader;
static GLuint colorTexId = -1;
static GLuint normalTexId = -1;


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
/// Print out the information log for a shader object 
/// @arg obj handle for a program object
*****************************************************************************/
static void printProgramInfoLog(GLuint obj)
{
	GLint infologLength = 0, charsWritten = 0;
	glGetProgramiv(obj, GL_INFO_LOG_LENGTH, &infologLength);
	if (infologLength > 2) {
		GLchar* infoLog = new GLchar [infologLength];
		glGetProgramInfoLog(obj, infologLength, &charsWritten, infoLog);
		fprintf(stderr, "%s\n", infoLog);
		delete infoLog;
	}
}


/*****************************************************************************
*****************************************************************************/
void
idleFunc()
{
	const float step = fabs(0.2);
	static float angleStep = step;
	
	// called when doing nothing
	if (angle > 45) {
		angleStep = -1 * step;
		angle = 45;
	} else if (angle < -45) {
		angleStep = 1 * step;
		angle = -45;
	} else
		angle += angleStep;
		
	glutPostRedisplay();
}



/*****************************************************************************
*****************************************************************************/
void
refreshCB()
{
	glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

	int colorLoc = glGetUniformLocation(program_object, "color_texture");  
	int normalLoc = glGetUniformLocation(program_object, "normal_texture");  

	// start program!
	glUseProgram(program_object);
	glUniform1i(colorLoc, 0);
	glUniform1i(normalLoc, 1);

	// set textures this frame
	glActiveTexture(GL_TEXTURE0);  
	glBindTexture(GL_TEXTURE_2D, colorTexId);  
	glEnable(GL_TEXTURE_2D);  
	glTexEnvf( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL ); 
	
	glActiveTexture(GL_TEXTURE1);  
	glBindTexture(GL_TEXTURE_2D, normalTexId);  
	glEnable(GL_TEXTURE_2D);  
	glTexEnvf( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL ); 
	
	// render geometry
	glPushMatrix();
	glTranslatef(0,0,-3.0);
	glRotatef(angle,0,1,0);
	glColor4f(1,1,1,1);
	glBegin(GL_QUADS);
	    glMultiTexCoord2f(GL_TEXTURE0, 0,0);   // texture coord
		glMultiTexCoord2f(GL_TEXTURE1, 1,0);   // tangent
		glMultiTexCoord2f(GL_TEXTURE2, 0,1);   // bitangent (or binormal)
		glVertex3f(-1,-1, 0.0);
		glMultiTexCoord2f(GL_TEXTURE0, 1,0);   // texture coord
		glMultiTexCoord2f(GL_TEXTURE1, 1,0);   // tangent
		glMultiTexCoord2f(GL_TEXTURE2, 0,1);   // bitangent (or binormal)
		glVertex3f( 1,-1, 0.0);
		glMultiTexCoord2f(GL_TEXTURE0, 1,1);   // texture coord
		glMultiTexCoord2f(GL_TEXTURE1, 1,0);   // tangent
		glMultiTexCoord2f(GL_TEXTURE2, 0,1);   // bitangent (or binormal)
		glVertex3f( 1, 1, 0.0);
		glMultiTexCoord2f(GL_TEXTURE0, 0,1);   // texture coord
		glMultiTexCoord2f(GL_TEXTURE1, 1,0);   // tangent
		glMultiTexCoord2f(GL_TEXTURE2, 0,1);   // bitangent (or binormal)
		glVertex3f(-1, 1, 0.0);
	glEnd();
	glPopMatrix();

	glActiveTexture(GL_TEXTURE1);  
	glBindTexture(GL_TEXTURE_2D, 0);  
	glDisable(GL_TEXTURE_2D);  
  
	glActiveTexture(GL_TEXTURE0);  
	glBindTexture(GL_TEXTURE_2D, 0);  
	glDisable(GL_TEXTURE_2D);

	// end program
	glUseProgram(0);

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
	
	glCullFace(GL_BACK);
	glDisable(GL_NORMALIZE);
	glDisable(GL_BLEND);
	glShadeModel(GL_SMOOTH);
	glReadBuffer(GL_BACK);
	glEnable(GL_SCISSOR_TEST);
	glEnable(GL_DEPTH_TEST);
	glClearColor(0,0,0,1);

	// setup callbacks
	glutDisplayFunc(refreshCB);
	glutReshapeFunc(reshapeCB);
	glutKeyboardFunc(keyboardCB);
	glutMouseFunc(mouseCB);
	glutMotionFunc(motionCB);

	// force initial matrix setup
	reshapeCB(winWidth, winHeight);

	// set modelview matrix stack to identity
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	// setup shaders
	program_object = glCreateProgram();
	vertex_shader = glCreateShader(GL_VERTEX_SHADER);
	fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);

	vertex_source = readFile("vertex.prg");
	fragment_source = readFile("fragment.prg");
	glShaderSource(vertex_shader, 1, (const GLchar **)&vertex_source, NULL);
	glShaderSource(fragment_shader, 1, (const GLchar **)&fragment_source, NULL);
	
	glCompileShader(vertex_shader);
	glAttachShader(program_object, vertex_shader);
	printProgramInfoLog(program_object);

	glCompileShader(fragment_shader);
	glAttachShader(program_object, fragment_shader);
	printProgramInfoLog(program_object);


	// setup texturing
	{
		int width, height, channels;
		unsigned char *colorData = SOIL_load_image("marble.png", &width, &height, &channels, 3);
		glGenTextures( 1, &colorTexId ); //generate the texture with the loaded data  
		glBindTexture( GL_TEXTURE_2D, colorTexId ); //bind the texture to it’s array  
		glTexEnvf( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL ); //set texture environment parameters  
		glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);  
		glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);  
		glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT );  
		glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT );  
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, colorData);  
	}

	{
		int width, height, channels;
		unsigned char *normalData = SOIL_load_image("normals2.png", &width, &height, &channels, 3);
		glGenTextures( 1, &normalTexId ); //generate the texture with the loaded data  
		glBindTexture( GL_TEXTURE_2D, normalTexId ); //bind the texture to it’s array  
		glTexEnvf( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL ); //set texture environment parameters  
		glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);  
		glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);  
		glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT );  
		glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT );  
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, normalData);  
	}


	// link program!
	glLinkProgram(program_object);

	// enter endless loop
	glutIdleFunc(idleFunc);
	glutMainLoop();

	return (TRUE);
}
