

#include "stdafx.h"
#include <stdlib.h>
#include <windows.h>
#include <glut.h>
#include "glm.h"
static float ypoz = 0, zpoz = 0;

// You need to declare your self a pointer to the model
// This is where the data will be loaded
GLMmodel* pmodel1 = NULL;
GLMmodel* pmodel2 = NULL;
GLMmodel* pmodel3 = NULL;
GLMmodel* pmodel4 = NULL;
GLMmodel* pmodel5 = NULL;
long count=0;
void init(void)
{
   glClearColor (1.0, 1.0, 1.0, 1.0);
   glEnable(GL_DEPTH_TEST);
   glShadeModel (GL_SMOOTH);
}




void drawmodel_box(void)
{
	if (!pmodel1)
	{
	    pmodel1 = glmReadOBJ("bike1.obj");
        if (!pmodel1) exit(0);
	    glmUnitize(pmodel1);
		glmFacetNormals(pmodel1);
		glmVertexNormals(pmodel1, 90.0);
    }
    glmDraw(pmodel1, GLM_SMOOTH| GLM_TEXTURE);
}
void drawmodel_box2(void)
{
	// Load the model only if it hasn't been loaded before
	// If it's been loaded then pmodel1 should be a pointer to the model geometry data...otherwise it's null
    if (!pmodel2)
	{
		// this is the call that actualy reads the OBJ and creates the model object
        pmodel2 = glmReadOBJ("bike2.obj");
        if (!pmodel2) exit(0);
		// This will rescale the object to fit into the unity matrix
		// Depending on your project you might want to keep the original size and positions you had in 3DS Max or GMAX so you may have to comment this.
        glmUnitize(pmodel2);
		// These 2 functions calculate triangle and vertex normals from the geometry data.
		// To be honest I had some problem with very complex models that didn't look to good because of how vertex normals were calculated
		// So if you can export these directly from you modeling tool do it and comment these line
		// 3DS Max can calculate these for you and GLM is perfectly capable of loading them
        glmFacetNormals(pmodel2);
		glmVertexNormals(pmodel2, 90.0);
    }
    // This is the call that will actualy draw the model
	// Don't forget to tell it if you want textures or not :))
    glmDraw(pmodel2, GLM_SMOOTH| GLM_TEXTURE);

}

void drawmodel_box3(void)
{
	if (!pmodel3)
	{
	    pmodel3 = glmReadOBJ("bike3.obj");
        if (!pmodel3) exit(0);
	    glmUnitize(pmodel3);
		glmFacetNormals(pmodel3);
		glmVertexNormals(pmodel3, 90.0);
    }
    glmDraw(pmodel3, GLM_SMOOTH| GLM_TEXTURE);
}
void drawmodel_box4(void)
{
	if (!pmodel4)
	{
	    pmodel4 = glmReadOBJ("bike4.obj");
        if (!pmodel4) exit(0);
	    glmUnitize(pmodel4);
		glmFacetNormals(pmodel4);
		glmVertexNormals(pmodel4, 90.0);
    }
    glmDraw(pmodel4, GLM_SMOOTH| GLM_TEXTURE);
}
void drawmodel_box5(void)
{
	if (!pmodel5)
	{
	    pmodel5 = glmReadOBJ("bike5.obj");
        if (!pmodel5) exit(0);
	    glmUnitize(pmodel5);
		glmFacetNormals(pmodel5);
		glmVertexNormals(pmodel5, 90.0);
    }
    glmDraw(pmodel5, GLM_SMOOTH| GLM_TEXTURE);
}
void display(void)
{

	glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity ();

	glTranslatef(0,0,-5);

	glPushMatrix();
	glColor3f(0.0f,0.0f,0.0f);
	glScalef(100,100,100);
	// I added these to be able to rotate the whole scene so you can see the box and textures
		//glRotatef(ypoz,0,1,0);
		//glRotatef(zpoz,0,0,1);
/*		drawmodel_box();
		glColor3f(1.0f,0.0f,0.0f);
		drawmodel_box2();
		glColor3f(0.0f,1.0f,0.0f);
		drawmodel_box3();
		glColor3f(0.0f,0.0f,1.0f);
		drawmodel_box4();
		glColor3f(1.0f,1.0f,1.0f);
		drawmodel_box5();	*/

				drawmodel_box();
		glColor3f(1.0f,1.0f,1.0f);
		drawmodel_box2();
		glColor3f(1.0f,1.0f,1.0f);
		drawmodel_box3();
		glColor3f(1.0f,1.0f,1.0f);
		drawmodel_box4();
		glColor3f(0.2f,0.2f,1.0f);
		drawmodel_box5();
	glPopMatrix();


	Sleep(5);
	glutSwapBuffers();


}

void reshape (int w, int h)
{
   glViewport (0, 0, (GLsizei) w, (GLsizei) h);
   glMatrixMode (GL_PROJECTION);
   glLoadIdentity ();
   gluPerspective(60.0, (GLfloat) w/(GLfloat) h, 1.0, 20.0);
   glMatrixMode (GL_MODELVIEW);
}

void keyboard(unsigned char key, int x, int y)
{
   switch (key) {
      case 27:
         exit(0);
         break;
	  case 'y':
		  ypoz=ypoz+5;
		  if (ypoz>360) ypoz=0;
         glutPostRedisplay();
		 printf("count-%ld",count);
         break;
	 case 'z':
		 zpoz = zpoz+5;
		 if (zpoz>360) zpoz=0;
         glutPostRedisplay();
         break;
   }
}

void animate()
{
	ypoz+=0.5;
	if (ypoz>360) ypoz=0;
	glutPostRedisplay();
}

int main(int argc, char** argv)
{
   glutInit(&argc, argv);
   glutInitDisplayMode (GLUT_RGB | GLUT_DEPTH | GLUT_DOUBLE);
   glutInitWindowSize (500, 500);
   glutInitWindowPosition (100, 100);
   glutCreateWindow (argv[0]);
   init ();
   glutDisplayFunc(display);
   glutReshapeFunc(reshape);
   glutKeyboardFunc(keyboard);
   // Do a little animation - rotate the object a bit so we can see it better
   glutIdleFunc(animate);
   glutMainLoop();
   return 0;
}

