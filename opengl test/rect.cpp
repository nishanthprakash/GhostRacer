#include<stdio.h>
#include<stdlib.h>
#include<cxcore.h>
#include<highgui.h>
#include<cv.h>
#include <GL/gl.h>
#include <GL/glut.h>
#include<GL/glu.h>
#include<math.h>



float i=0,j=0,k=0,xp=0,xj=0,xavg=0,yavg=0,
			xb=0,yb=0,
			xt=0,yt=0,
			xl=0,yl=0,
			xr=0,yr=0,
			inclination=0,
			thresh1=0,thresh2=0,thresh3=0;

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
	
	inclination=(atan((yt-yb)/(xt-xb))-atan(21/10))*180/3.14;

	printf("%f\n",inclination);

/*	for(i=600;i>0;i--)
		for(j=0;j<disp->width;j++)
		{
			data=cvGet2D(dilated,j,i);
			
			if(data.val[2]>120&&data.val[1]<100&&data.val[0]<100)
			{	goto doneright;
				
			}
		}
	doneright:
	xr=i;
	yr=j;
	for(i=400;i>0;i--)
		for(j=0;j<disp->width;j++)
		{
			data=cvGet2D(dilated,i,j);
			
			if(data.val[2]>120&&data.val[1]<100&&data.val[0]<100)
			{	goto donebottom;
				
			}
		}
	donebottom:
	xb=j;
	yb=i;
	
	
*/	
	cvNamedWindow("Cap");
	cvShowImage("Cap",dilated);
	cvWaitKey(3);

//*/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    	glColor3f(1.0, 1.0, 1.0);
	glPushMatrix();
	glTranslatef(0,0,0);
	glRotatef(inclination,0,0,1);
	glutSolidCube(400);
	glPopMatrix();
	
  	glutSwapBuffers();
}


int main(int argc, char** argv)
{
    	glutInit(&argc, argv);
    	glutInitDisplayMode (GLUT_DOUBLE | GLUT_RGBA|GLUT_DEPTH);
    	glutInitWindowSize (500, 500); 
    	glutCreateWindow("hello");
  
	
	glOrtho(-1000,1000,-1000,1000,1000,-1000);

	glutTimerFunc(33,idle,2);
    	glutDisplayFunc(display); 

	video1=cvCaptureFromCAM(-1);

    	glutMainLoop();
    	return 0;  
}
