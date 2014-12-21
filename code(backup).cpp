#include "stdafx.h"
#include<stdio.h>
#include<stdlib.h>
#include <windows.h>
#include<cxcore.h>
#include<highgui.h>
#include<cv.h>
//#include <gl.h>
#include <glut.h>
//#include<glu.h>
#include"glm.h"
#include<math.h>

static float ypoz = 0, zpoz = 0;

// You need to declare your self a pointer to the model
// This is where the data will be loaded
GLMmodel* pmodel1 = NULL;
GLMmodel* pmodel2 = NULL;
GLMmodel* pmodel3 = NULL;
GLMmodel* pmodel4 = NULL;
GLMmodel* pmodel5 = NULL;
long count=0;

float i=0.0,j=0.0,k=0.0,xp=0.0,xj=0.0,xavg=0.0,yavg=0.0,
			xb=0.0,yb=0.0,
			xt=0.0,yt=0.0,
			xl=0.0,yl=0.0,
			xr=0.0,yr=0.0,
			inclination=0.0,
			thresh1=0.0,thresh2=0.0,thresh3=0.0,inclination0=0;

	IplImage* captured;
	IplImage* dilated;
	IplImage* eroded;
	CvCapture* video1;
	IplImage* buff;
	IplImage* disp;
	IplImage* smooth;
	IplImage* scaled;
	int trans=0,d=1;
	CvScalar data;


void idle(int val)
{	
	glutPostRedisplay();
	glutTimerFunc(33,idle,2);
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

void display()
{
	glClearColor(0.0, 0.0, 0.0, 0.0);
    	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);


/*	glPushMatrix();
	glTranslatef(xavg,yavg,0);
	glutSolidCube(200);
	glPopMatrix();
/*

	glBegin(GL_QUADS);
		glVertex3f(xr,xb,0);	
		glVertex3f(xb,yb,0);	
		glVertex3f(xl,yl,0);
		glVertex3f(xt,yt,0);
	glEnd();
*/
///////////////////////////////////////////////////////////nishanthprakash20///////////////////////////////////////////////////
	captured=cvQueryFrame(video1);
	disp=cvCreateImage(cvGetSize(captured),IPL_DEPTH_8U,3);	
	eroded=cvCreateImage(cvGetSize(captured),IPL_DEPTH_8U,3);	
	dilated=cvCreateImage(cvGetSize(captured),IPL_DEPTH_8U,3);

//	data=cvGet2D(captured,240,320);
//	printf("%f,%f,%f\n",data.val[0],data.val[1],data.val[2]);
	

	thresh1=150;
	thresh2=100;
	thresh3=100;


	for(i=0;i<disp->height;i++)
	for(j=0;j<disp->width;j++)
		{
			data=cvGet2D(captured,i,j);
			
			if(data.val[1]>thresh1&&data.val[2]<thresh2&&data.val[0]<thresh3)
			{	
				cvSet2D(disp,i,j,data);
			}
		}

	cvErode(disp,eroded,NULL,1);
	cvDilate(eroded,dilated,NULL,4);
	for(i=0;i<disp->height;i++)
	for(j=0;j<disp->width;j++)
		{
			data=cvGet2D(dilated,i,j);
			
			if(data.val[1]>thresh1&&data.val[2]<thresh2&&data.val[0]<thresh3)
			{	goto donetop;
				
			}
		}
	donetop:
	xt=j;
	yt=i;
	
	for(i=479;i>0;i--)
	for(j=0;j<disp->width;j++)
		{
			data=cvGet2D(dilated,i,j);
			
			if(data.val[1]>thresh1&&data.val[2]<thresh2&&data.val[0]<thresh3)
			{	goto doneleft;
				
			}
		}
	doneleft:
	xb=j;
	yb=i;
	
	inclination=((float)atan((yt-yb)/(xt-xb))-(float)atan(10.0/21))*180/3.14;
	if(inclination<0)	inclination+=60;
	printf("%f\n",inclination);
	
	cvNamedWindow("Cap");
	cvShowImage("Cap",dilated);
	cvWaitKey(3);

//*/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    	glColor3f(1.0, 1.0, 1.0);
	glPushMatrix();
	glTranslatef(0,0,-5);
	glRotatef(inclination,0,0,1);

	glScalef(100,100,100);
	glColor3f(0.0f,0.0f,0.0f);
	drawmodel_box();
		glColor3f(1.0f,1.0f,1.0f);
		drawmodel_box2();
		glColor3f(1.0f,1.0f,1.0f);
		drawmodel_box3();
		glColor3f(1.0f,1.0f,1.0f);
		drawmodel_box4();
		glColor3f(0.2f,0.2f,1.0f);
		drawmodel_box5();									//remove this
	//glScalef(0.01,0.01,0.01);
//glTranslatef(0,0,5);
	glPopMatrix();
	
  	glutSwapBuffers();
}


int main(int argc, char** argv)
{
    	glutInit(&argc, argv);
    	glutInitDisplayMode (GLUT_DOUBLE | GLUT_RGBA|GLUT_DEPTH);
    	glutInitWindowSize (800, 400); 
    	glutCreateWindow("hello");
  
		  glClearColor (1.0, 1.0, 1.0, 1.0);
	glEnable(GL_DEPTH_TEST);
   glShadeModel (GL_SMOOTH);
   
    glViewport (0, 0, 800, 400);
   glMatrixMode (GL_PROJECTION);
   glLoadIdentity ();
   gluPerspective(60.0, 800/400, 1.0, 220.0);
	//glOrtho(-1000,1000,-1000,1000,1000,-1000);
glMatrixMode(GL_MODELVIEW);
	glutTimerFunc(33,idle,2);
    	glutDisplayFunc(display); 

	video1=cvCaptureFromCAM(-1);

    	glutMainLoop();
    	return 0;  
}
