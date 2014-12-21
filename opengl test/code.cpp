

#include<stdio.h>
#include<stdlib.h>
#include<gl/glut.h>
#include<gl/glu.h>
#include "imageloader.h"
#include "stdafx.h"
#include <windows.h>
#include "glm.h"
#include<cxcore.h>
#include<highgui.h>
#include<cv.h>
#include "text3d.h"

unsigned int xot=0,enterfl=0,bitsy=0;
int titsy=0;
//////////////////////////////////////////////////////////////////////
#define CAM_MEMORY_LIMIT 650
float i=0,j=0,k=0,xp=0,xj=0,xavg=0,yavg=0,
			xb=0,yb=0,
			xt=0,yt=0,
			xl=0,yl=0,
			xc=0,yc=0,
			x0=0,ydef=480,
			inclination=0,
			thresh1=0,thresh2=0,thresh3=0,acceleration,c=0,bend=0;

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
/////////////////////////////////////////////////////////////////


#define OBJECT_SIZE 5
#define OBJECT_SPEED .4
#define UPPER_SPEED_LIMIT	3

GLMmodel* pmodel1 = NULL;
GLMmodel* pmodel2 = NULL;
GLMmodel* pmodel3 = NULL;
GLMmodel* pmodel4 = NULL;
GLMmodel* pmodel5 = NULL;

void initObjects(void);
int collisionTest(float x,float y,float z,int front_collision);

struct vertices
{
    float x;
    float y;
    float z;
};

struct object
{
    float size;
    int check;
    struct vertices pos;
    struct vertices min;
    struct vertices max;
};

struct boundingBox
{
    struct vertices pos;
    struct vertices min;
    struct vertices max;
};

float up=0,front=0,lr=0,acc=.09,dec=.01;
int flag=0;
float incX=.01;
int keyPressed=0,lrKeyPressed=0,defaultKey;
int mouseX,mouseY;                  // These variables are used to hold the postion of the mouse when right or left key is pressed

int boxNumber=0;
int numberOfObjects=100;
int lowerBound=0;
int upperBound=numberOfObjects;
struct boundingBox box[100];
struct object objects[100];

GLuint loadTexture(Image* image) {
	GLuint textureId;
	glGenTextures(1, &textureId); //Make room for our texture
	glBindTexture(GL_TEXTURE_2D, textureId); //Tell OpenGL which texture to edit
	//Map the image to the texture
	glTexImage2D(GL_TEXTURE_2D,                //Always GL_TEXTURE_2D
				 0,                            //0 for now
				 GL_RGB,                       //Format OpenGL uses for image
				 image->width, image->height,  //Width and height
				 0,                            //The border of the image
				 GL_RGB, //GL_RGB, because pixels are stored in RGB format
				 GL_UNSIGNED_BYTE, //GL_UNSIGNED_BYTE, because pixels are stored
				                   //as unsigned numbers
				 image->pixels);               //The actual pixel data
	return textureId; //Returns the id of the texture
}

GLuint _textureId; //The id of the texture
GLuint _textureIdroad;
GLuint _textureIdgrass;
GLuint _textureIdbox;
GLuint _textureIdbike; 

float computeScale(const char* strs[4]) {
	float maxWidth = 0;
	for(int i = 0; i < 4; i++) {
		float width = t3dDrawWidth(strs[i]);
		if (width > maxWidth) {
			maxWidth = width;
		}
	}

	return 2.6f / maxWidth;
}

float _angle = -30.0f;
float _scale;
//The four strings that are drawn
const char* STRS[5] = {"New Game", "Scores", "Credits","Quit", "RoadBash 3"};

void cleanup() {
	t3dCleanup();
}


void init(void){
    glEnable(GL_DEPTH_TEST);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glEnable(GL_LIGHT1);
	glEnable(GL_LIGHT2);
	glEnable(GL_LIGHT3);
	glEnable(GL_NORMALIZE);
	glEnable(GL_COLOR_MATERIAL);
	glShadeModel(GL_SMOOTH);
	t3dInit();

Image* image = loadBMP("24 (3).bmp");
	_textureId = loadTexture(image);
	delete image;

image = loadBMP("Italy Ann 154.bmp");
	_textureIdroad = loadTexture(image);
	delete image;

image = loadBMP("4740894.bmp");
	_textureIdgrass = loadTexture(image);
	delete image;

image = loadBMP("5021576700_94025ece98.bmp");
	_textureIdbox = loadTexture(image);
	delete image;

image = loadBMP("vectorstock-61263-motorbike-vector.bmp");
	_textureIdbike = loadTexture(image);
	delete image;
}


void drawScene() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	glPushMatrix();

	 glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, _textureIdbike);

	//Bottom
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glColor3f(0.2f, 0.2f, 0.2f);
	glTranslatef(-400,-300,0);
	glBegin(GL_POLYGON);

	glNormal3f(0.0, 0.0f, 1.0f);

	glTexCoord2f(0.0f, 0.0f);
glVertex3f(0,0,-500);
	glTexCoord2f(1.0f, 0.0f);
glVertex3f(800,0,-500);
	glTexCoord2f(1.0f, 1.0f);
glVertex3f(800,600,-500);
	glTexCoord2f(0.0f, 1.0f);
glVertex3f(0,600,-500);

    //glColor3f(0.75,0.75,0.75); //light grey
    //glColor3f(.68,.56,.2);//mud brown

    glEnd();
    glPopMatrix();
    glColor3f(1,1,1);

	glTranslatef(0.0f, 0.0f, -8.0f);

	GLfloat ambientColor[] = {0.5f, 0.5f, 0.5f, 1.0f};
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, ambientColor);
	GLfloat lightColor0[] = {0.6f, 0.6f, 1.0f, 1.0f};
	GLfloat lightPos0[] = {-0.5f, 0.5f, 1.0f, 0.0f};

	glPushMatrix();
	glRotatef(-_angle, 0.0f, 1.0f, 0.0f);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, lightColor0);
	glLightfv(GL_LIGHT0, GL_POSITION, lightPos0);
    glPopMatrix();

    GLfloat lightColor1[] = {1.0f, 0.6f, 0.6f, 1.0f};
	GLfloat lightPos1[] = {-0.5f, 0.5f, 1.0f, 0.0f};

	glPushMatrix();
	glRotatef(-_angle+120, 0.0f, 1.0f, 0.0f);
	glLightfv(GL_LIGHT1, GL_DIFFUSE, lightColor1);
	glLightfv(GL_LIGHT1, GL_POSITION, lightPos1);
    glPopMatrix();

    GLfloat lightColor2[] = {0.6f, 1.0f, 0.6f, 1.0f};

	GLfloat lightPos2[] = {-0.5f, 0.5f, 1.0f, 0.0f};

	glPushMatrix();
	glRotatef(-_angle+240, 0.0f, 1.0f, 0.0f);
	glLightfv(GL_LIGHT2, GL_DIFFUSE, lightColor2);
	glLightfv(GL_LIGHT2, GL_POSITION, lightPos2);
    glPopMatrix();


	glRotatef(20.0f, 1.0f, 0.0f, 0.0f);
	//glRotatef(-_angle, 0.0f, 1.0f, 0.0f);

	//Draw the strings along the sides of a square
	glScalef(_scale, _scale, _scale);
	glColor3f(1.0f, 1.0f, 1.0f);

	glRotatef(-45,0,1,0);
	glTranslatef(1,-2,0);
	if(xot%4==0){
	glTranslatef(5,0,0);
	glColor3f(0.3,0.3,0.3);
	t3dDraw3D(STRS[3], 0, 0, 0.2f);

		glTranslatef(0,2,0);
		glColor3f(0.3,0.3,0.3);
	t3dDraw3D(STRS[2], 0, 0, 0.2f);

		glTranslatef(0,2,0);
		glColor3f(0.3,0.3,0.3);
	t3dDraw3D(STRS[1], 0, 0, 0.2f);


		glTranslatef(0,2,0);
		glPushMatrix();
      glRotatef(titsy,0,1,0);
      glColor3f(0.6,0.6,0.6);
    t3dDraw3D(STRS[0], 0, 0, 0.2f);
    glPopMatrix();
	}

	if(xot%4==1){

	glTranslatef(5,0,0);
	glColor3f(0.3,0.3,0.3);
	t3dDraw3D(STRS[3], 0, 0, 0.2f);

		glTranslatef(0,2,0);
		glColor3f(0.3,0.3,0.3);
	t3dDraw3D(STRS[2], 0, 0, 0.2f);



		glTranslatef(0,2,0);
		glPushMatrix();
		glRotatef(titsy,0,1,0);
		glColor3f(0.6,0.6,0.6);
	t3dDraw3D(STRS[1], 0, 0, 0.2f);
	glPopMatrix();


		glTranslatef(0,2,0);
		glColor3f(0.3,0.3,0.3);
    t3dDraw3D(STRS[0], 0, 0, 0.2f);
	}

	if(xot%4==2){

	glTranslatef(5,0,0);
	glColor3f(0.3,0.3,0.3);
	t3dDraw3D(STRS[3], 0, 0, 0.2f);


		glTranslatef(0,2,0);
		glPushMatrix();
		glRotatef(titsy,0,1,0);
		glColor3f(0.6,0.6,0.6);
	t3dDraw3D(STRS[2], 0, 0, 0.2f);
glPopMatrix();

		glTranslatef(0,2,0);
glColor3f(0.3,0.3,0.3);
	t3dDraw3D(STRS[1], 0, 0, 0.2f);


		glTranslatef(0,2,0);
		glColor3f(0.3,0.3,0.3);
    t3dDraw3D(STRS[0], 0, 0, 0.2f);
	}

	if(xot%4==3){
	glTranslatef(5,0,0);
	glPushMatrix();
	 glRotatef(titsy,0,1,0);
	 glColor3f(0.6,0.6,0.6);
	t3dDraw3D(STRS[3], 0, 0, 0.2f);
	glPopMatrix();


		glTranslatef(0,2,0);
		glColor3f(0.3,0.3,0.3);
	t3dDraw3D(STRS[2], 0, 0, 0.2f);


		glTranslatef(0,2,0);
		glColor3f(0.3,0.3,0.3);
	t3dDraw3D(STRS[1], 0, 0, 0.2f);


		glTranslatef(0,2,0);
		glColor3f(0.3,0.3,0.3);
    t3dDraw3D(STRS[0], 0, 0, 0.2f);
	}

glRotatef(40,0,1,0);
glRotatef(30,0,0,1);
glTranslatef(-10.0,7,0);

glColor3f(1.0,1.0,1.0);
    t3dDraw2D(STRS[4], 0, 0, 0.2f);



	glutSwapBuffers();
}



void handleResize(int w, int h) {
	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45.0, (double)w / (double)h, 1.0, 700.0);
}

void bikeMovement(int up,int leftRight)
{
    if((up==1)&&(leftRight==0))
    {
        if(incX<UPPER_SPEED_LIMIT)
        {
            incX=incX+acc;
            front=front+incX;
        }
        else
        {
            incX=UPPER_SPEED_LIMIT;
            front=front+incX;
        }
        //printf("%f\t",incX);
        glutPostRedisplay();
    }
    else if((up==0)&&(leftRight==0))
    {
            if(incX>0)
			{
				incX=incX-acc;
				front=front+incX;
			}
			else
			{
				incX=0;
			}
    }
    else if((leftRight==-1))
    {
        lrKeyPressed=1;
        //mouseX=x;
        //mouseY=y;
        if(lr>=-50)
        {
        lr=lr-1;
        }
        else
        {
            lr=lr;
        }

        if(up==1)
        {
            if(incX<UPPER_SPEED_LIMIT)
            {
                incX=incX+acc;
                front=front+incX;
            }
            else
            {
                incX=UPPER_SPEED_LIMIT;
                front=front+incX;
            }
            //printf("%f\t",incX);
        }
        else
        {
            if(incX>0)
			{
				incX=incX-acc;
				front=front+incX;
			}
			else
			{
				incX=0;
			}
        }
        glutPostRedisplay();
    }
    else if((leftRight==-2))
    {
        lrKeyPressed=1;
        //mouseX=x;
        //mouseY=y;
        if(lr>=-50)
        {
        lr=lr-2;
        }
        else
        {
            lr=lr;
        }
        if(up==1)
        {
            if(incX<UPPER_SPEED_LIMIT)
            {
                incX=incX+acc;
                front=front+incX;
            }
            else
            {
                incX=UPPER_SPEED_LIMIT;
                front=front+incX;
            }
            //printf("%f\t",incX);
        }
        else
        {
            if(incX>0)
			{
				incX=incX-acc;
				front=front+incX;
			}
			else
			{
				incX=0;
			}
        }
        glutPostRedisplay();
    }
    else if((leftRight==0))
    {
        lr=lr;
        if(up==1)
        {
            if(incX<UPPER_SPEED_LIMIT)
            {
                incX=incX+acc;
                front=front+incX;
            }
            else
            {
                incX=UPPER_SPEED_LIMIT;
                front=front+incX;
            }
            //printf("%f\t",incX);
        }
        else
        {
            if(incX>0)
			{
				incX=incX-acc;
				front=front+incX;
			}
			else
			{
				incX=0;
			}
        }
    }
    else if((leftRight==1))
    {
        lrKeyPressed=1;
       //mouseX=x;
        //mouseY=y;
        if(lr<=50)
        {
            lr=lr+1;
            //front=front+incX;
        }
        else
        {
            lr=lr;
            //front=front+incX;
        }
        if(up==1)
        {
            if(incX<UPPER_SPEED_LIMIT)
            {
                incX=incX+acc;
                front=front+incX;
            }
            else
            {
                incX=UPPER_SPEED_LIMIT;
                front=front+incX;
            }
            //printf("%f\t",incX);
        }
        else
        {
            if(incX>0)
			{
				incX=incX-acc;
				front=front+incX;
			}
			else
			{
				incX=0;
			}
        }
        glutPostRedisplay();
    }
    else if((leftRight==2))
    {
        lrKeyPressed=1;
        //mouseX=x;
        //mouseY=y;
        if(lr<=50)
        {
            lr=lr+2;
            //front=front+incX;
        }
        else
        {
            lr=lr;
            //front=front+incX;
        }
        if(up==1)
        {
            if(incX<UPPER_SPEED_LIMIT)
            {
                incX=incX+acc;
                front=front+incX;
            }
            else
            {
                incX=UPPER_SPEED_LIMIT;
                front=front+incX;
            }
            //printf("%f\t",incX);
        }
        else
        {
            if(incX>0)
			{
				incX=incX-acc;
				front=front+incX;
			}
			else
			{
				incX=0;
			}
        }
        glutPostRedisplay();
    }
    else
    {
        incX=incX+acc;
        front=front+incX;
        glutPostRedisplay();
    }

}

void update(int value) {
	_angle += 1.5f;
	if (_angle > 360) {
		_angle -= 360;
	}

	if(titsy==40)
        bitsy=1;
    if(titsy==-20)
        bitsy=0;

        if(!bitsy)
            titsy+=1;
        if(bitsy)
            titsy-=1;
	glutPostRedisplay();
	glutTimerFunc(25, update, 0);
}


void specialKeys(int key,int x,int y)
{
        keyPressed=1;

    if(key==GLUT_KEY_UP)
    {
        bikeMovement(1,0);
    }
    else if(key==GLUT_KEY_RIGHT)
    {
        bikeMovement(0,1);
    }
    else if(key==GLUT_KEY_LEFT)
    {
        bikeMovement(0,-1);
    }
    else if(key==GLUT_KEY_DOWN)
    {
        bikeMovement(0,0);

    }
    else
    {
        bikeMovement(3,3);
    }
}

void keyboard(unsigned char key,int x,int y)
{
    int temp;
    switch(key)
    {
        case 'w':
            flag=1;
            up=up+.1;
            break;
        case 's':
            flag=1;
            up=up-.1;
            break;
        case 'r':
            flag=1;
            up=0;
            front=0;
            incX=0;
            lowerBound=0;
            break;
        case ' ':
            incX=0;
            glutPostRedisplay();
			break;
        case 'n':
            incX=UPPER_SPEED_LIMIT;
            front=front+UPPER_SPEED_LIMIT;
            break;

    }
    glutPostRedisplay();
}

void changeSize(GLsizei w, GLsizei h)
{
   float aspect;

   if(h==0)
        h=1;
    aspect=(float)w/(float)h;
    glViewport(0,0,w,h);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    gluPerspective(120,aspect,.1,200);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

void setup()
{
    glClearColor(0,1,1,1);
	glEnable(GL_DEPTH_TEST);
	glShadeModel (GL_SMOOTH);
}

void moveObjectsH(void)
{
    int i;
    static int count=0;
    int temp;
    int temp1=-1;
    if(count==0)
    {
        for(i=0;i<numberOfObjects;i++)
        {
            temp1=rand()%2;
            if(temp1==0)
                temp1=-1;
            //printf("%d\n",temp1);
            objects[i].check=temp1;
            temp=rand()%50;
            //printf("%d\n",temp);
            objects[i].pos.z=temp;
        }
        count++;
    }
    else
    {
        for(i=lowerBound;i<upperBound;i++)
        {
            if(objects[i].pos.z>=50)
            {
                objects[i].check=1;
            }
            else if(objects[i].pos.z<=-50)
            {
                objects[i].check=-1;
            }
            else if((objects[i].pos.z<=50)&&(objects[i].pos.z>=-50))
            {
                objects[i].check=objects[i].check;
            }
            else
            {

            }

            if(objects[i].check==1)
                objects[i].pos.z-=OBJECT_SPEED;
            else
                objects[i].pos.z+=OBJECT_SPEED;
        }
    }
}

void movingObject(int value)
{
    initObjects();
    glutPostRedisplay();
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

void Bike()
{
	float ballX,ballY,ballZ;
	int ifCollided=0;
	glPushMatrix();
			ballX=-15+front;
			ballY=6;
			ballZ=0+lr;
			if(ballZ>25)
			{
				ballZ=25;
				ballX=ballX+OBJECT_SIZE;
			}
			else if(ballZ<-25)
			{
				ballZ=-25;
				ballX=ballX+OBJECT_SIZE;
			}
			else
			{
				//Do Nothing
			}

			glTranslatef(ballX,ballY,ballZ);

			glPushMatrix();
					glTranslatef(0,1,0);
					glRotatef(-90,0,1,0);
					glRotatef(-20,1,0,0);
					glRotatef(-ballZ*1.75,0,0,1);
						glPushMatrix();
									glColor3f(0.0f,0.0f,0.0f);
									glScalef(500,500,500);
									// I added these to be able to rotate the whole scene so you can see the box and textures
										//glRotatef(ypoz,0,1,0);
										//glRotatef(zpoz,0,0,1);
									/*	drawmodel_box();
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

				glPopMatrix();

		glPopMatrix();

	ifCollided=collisionTest(ballX+7,ballY,ballZ,1);
    //printf("%f\t%f\t%f\n",ballX,ballY,ballZ);
    if(ifCollided==1)
    {
       front=front-1;
       incX=0;
    }
    ifCollided=collisionTest(ballX,ballY,ballZ-2,0);
    if(ifCollided==1)
    {
        lr=lr+1;
        incX=0;
    }
    ifCollided=collisionTest(ballX,ballY,ballZ+2,0);
    if(ifCollided==1)
    {
        lr=lr-1;
        incX=0;
    }
}

void initObjects(void)
{
     /*//float maxX=0,maxY=12.0,maxZ=10.0,minX=0.0,minY=-10.0,minZ=-10;
     box[boxNumber].max.x=0.0;
     box[boxNumber].max.y=12.0;
     box[boxNumber].max.z=10.0;
     box[boxNumber].min.x=0.0;
     box[boxNumber].min.y=-10.0;
     box[boxNumber].min.z=-10.0;

     //printf("%f\n%f\n%f\n%f\n%f\n%f\n",
     box[boxNumber].max.x,
     box[boxNumber].max.y,
     box[boxNumber].max.z,
     box[boxNumber].min.x,
     box[boxNumber].min.y,
     box[boxNumber].min.z);*/

     int i;
     int temp=-1;
     moveObjectsH();
     for(i=lowerBound;i<upperBound;i++)
     {
         objects[i].pos.x=i*50;
         objects[i].pos.y=1+(objects[i].size/2.0);
         //objects[i].pos.z=j+10;
     }
     for(i=lowerBound;i<upperBound;i++)
     {
         objects[i].size=OBJECT_SIZE;
         box[i].max.x=(objects[i].pos.x + (objects[i].size/2.0));
         box[i].max.y=(objects[i].pos.y + (objects[i].size/2.0));
         box[i].max.z=(objects[i].pos.z + (objects[i].size/2.0));
         box[i].min.x=(objects[i].pos.x - (objects[i].size/2.0));
         box[i].min.y=(objects[i].pos.y - (objects[i].size/2.0));
         box[i].min.z=(objects[i].pos.z - (objects[i].size/2.0));
         /*printf("%f\n%f\n%f\n%f\n%f\n%f\n",
         box[i].max.x,
         box[i].max.y,
         box[i].max.z,
         box[i].min.x,
         box[i].min.y,
         box[i].min.z);*/
     }

}

void drawObject(void)
{
    int i;
    glColor3f(0,0,0);
    for(i=lowerBound;i<upperBound;i++)
    {
        glPushMatrix();
        glTranslatef(objects[i].pos.x,objects[i].pos.y,objects[i].pos.z);

    glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, _textureIdbox);

	//Bottom
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glColor3f(1.0f, 1.0f, 1.0f);

        glBegin(GL_QUADS);

        glNormal3f(0.0, 0.0f, -1.0f);
        glTexCoord2f(0.0f, 1.0f);
        glVertex3f(-2.5,2.5,-2.5);
        glTexCoord2f(0.0f, 0.0f);
        glVertex3f(-2.5,-2.5,-2.5);
        glTexCoord2f(1.0f, 0.0f);
        glVertex3f(2.5,-2.5,-2.5);
        glTexCoord2f(1.0f, 1.0f);
        glVertex3f(2.5,2.5,-2.5);

        glNormal3f(0.0, 0.0f, 1.0f);
        glTexCoord2f(0.0f, 1.0f);
        glVertex3f(-2.5,2.5,2.5);
        glTexCoord2f(0.0f, 0.0f);
        glVertex3f(-2.5,-2.5,2.5);
        glTexCoord2f(1.0f, 0.0f);
        glVertex3f(2.5,-2.5,2.5);
        glTexCoord2f(1.0f, 1.0f);
        glVertex3f(2.5,2.5,2.5);

        /*glNormal3f(1.0, 0.0f, 0.0f);
        glTexCoord2f(0.0f, 1.0f);
        glVertex3f(2.5,2.5,-2.5);
        glTexCoord2f(0.0f, 0.0f);
        glVertex3f(2.5,-2.5,-2.5);
        glTexCoord2f(1.0f, 0.0f);
        glVertex3f(2.5,-2.5,2.5);
        glTexCoord2f(1.0f, 1.0f);
        glVertex3f(2.5,2.5,2.5);
*/
        glNormal3f(-1.0, 0.0f, 0.0f);
        glTexCoord2f(0.0f, 1.0f);
        glVertex3f(-2.5,2.5,-2.5);
        glTexCoord2f(0.0f, 0.0f);
        glVertex3f(-2.5,-2.5,-2.5);
        glTexCoord2f(1.0f, 0.0f);
        glVertex3f(-2.5,-2.5,2.5);
        glTexCoord2f(1.0f, 1.0f);
        glVertex3f(-2.5,2.5,2.5);

        glNormal3f(0.0, 1.0f, 0.0f);
        glTexCoord2f(0.0f, 1.0f);
        glVertex3f(-2.5,2.5,2.5);
        glTexCoord2f(0.0f, 0.0f);
        glVertex3f(-2.5,2.5,-2.5);
        glTexCoord2f(1.0f, 0.0f);
        glVertex3f(2.5,2.5,-2.5);
        glTexCoord2f(1.0f, 1.0f);
        glVertex3f(2.5,2.5,2.5);

        glEnd();

        glPopMatrix();
    }
}

void objectsWithinBallsRange(float x,float y,float z)
{
    int i;
    int temp=0;
    for(i=lowerBound;i<numberOfObjects;i++)
    {
        if(x<=objects[i].pos.x)
        {
            lowerBound=i-2;
            //printf("lowerBound=%d\n",lowerBound);
            temp=1;
            break;
        }
    }
    for(i=lowerBound;i<numberOfObjects;i++)
    {
        if((x+150)<=objects[i].pos.x)   ///////////////////////////////////This depends on the space between two objects
        {
            upperBound=i;
            //printf("upperBound=%d\n",upperBound);
            break;
        }
    }
    if(lowerBound<0)
    {
        lowerBound=0;
    }
    if(lowerBound>=numberOfObjects)
    {
        lowerBound=upperBound=numberOfObjects;
    }
    if(temp==0)
    {
        lowerBound=upperBound=numberOfObjects;
    }


}

int collisionTest(float x,float y,float z, int front_collision)
{
    //float maxX=0,maxY=12.0,maxZ=10.0,minX=0.0,minY=-10.0,minZ=-10;
    int valueReturn=0;
    int i;
    if(front_collision==1)
        objectsWithinBallsRange(x,y,z);

    //for(i=lowerBound;i<=upperBound;i++)

    for(i=lowerBound;i<upperBound;i++)
    {
         /*printf("%d\n",i);
         printf("%f\t%f\t%f\n%f\t%f\t%f\n",
         box[i].max.x,
         box[i].max.y,
         box[i].max.z,
         box[i].min.x,
         box[i].min.y,
         box[i].min.z);
         printf("\n%f\t%f\t%f\n",x,y,z);
         printf("\n\n\n\n\n");*/


        //if(x>box[i].min.x && x<box[i].max.x && y>box[i].min.y && y<box[i].max.y && z>box[i].min.z && z<box[i].max.z )

        if(x>box[i].min.x && x<box[i].max.x && z>box[i].min.z && z<box[i].max.z )
        //if(x>minX && x<maxX && y>minY && y<maxY && z>minZ && z<maxZ)
        {
            valueReturn=valueReturn || 1;
        }
        else
        {
            valueReturn=valueReturn || 0;
        }
    }
    return(valueReturn);
}


void ball()
{
    float ballX,ballY,ballZ;
    int ifCollided=0;

    glPushMatrix();
    ballX=-15+front;
    ballY=6;
    ballZ=0+lr;

    if(ballZ>25)
    {
        ballZ=25;
        ballX=ballX+OBJECT_SIZE;
    }
    else if(ballZ<-25)
    {
        ballZ=-25;
        ballX=ballX+OBJECT_SIZE;
    }
    else
    {
        //Do Nothing
    }
    glTranslatef(ballX,ballY,ballZ);
    glColor3f(1,0,0);
    glutWireSphere(5,100,10);
    glPopMatrix();

    ifCollided=collisionTest(ballX+5,ballY,ballZ,1);
    //printf("%f\t%f\t%f\n",ballX,ballY,ballZ);
    if(ifCollided==1)
    {
       front=front-1;
       incX=0;
    }
    ifCollided=collisionTest(ballX,ballY,ballZ-5,0);
    if(ifCollided==1)
    {
        lr=lr+1;
        incX=0;
    }
    ifCollided=collisionTest(ballX,ballY,ballZ+5,0);
    if(ifCollided==1)
    {
        lr=lr-1;
        incX=0;
    }

}

void road()
{
    glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, _textureIdroad);

	//Bottom
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glColor3f(1.0f, 1.0f, 1.0f);
	glBegin(GL_POLYGON);

	glNormal3f(0.0, 0.0f, -1.0f);
	glTexCoord2f(0.0f, 0.0f);
glVertex3f(-30,1,-50);
	glTexCoord2f(600.0f, 0.0f);
glVertex3f(100000,1,-50);
	glTexCoord2f(600.0f, 0.4f);
glVertex3f(100000,1,50);
	glTexCoord2f(0.0f, 0.4f);
glVertex3f(-30,1,50);

    //glColor3f(0.75,0.75,0.75); //light grey
    //glColor3f(.68,.56,.2);//mud brown




    glEnd();
    //glDisable(GL_TEXTURE_ID);
}

void grid()
{
    float x=0,z=0;
     glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, _textureIdgrass);

	//Bottom
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glColor3f(1.0f, 1.0f, 1.0f);
	glBegin(GL_POLYGON);


	glNormal3f(0.0, 0.0f, -1.0f);
	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(-30,-1,-25);
	glTexCoord2f(6000.0f, 0.0f);
	glVertex3f(100000,-1,-25);
	glTexCoord2f(6000.0f, 10000.0f);
	glVertex3f(100000,-1,-100000);
	glTexCoord2f(0.0f, 10000.0f);
	glVertex3f(-30,-1,-100000);

	glNormal3f(0.0, 0.0f, -1.0f);
	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(-30,-1,25);
	glTexCoord2f(6000.0f, 0.0f);
	glVertex3f(100000,-1,25);
	glTexCoord2f(6000.0f, 10000.0f);
	glVertex3f(100000,-1,100000);
	glTexCoord2f(0.0f, 10000.0f);
	glVertex3f(-30,-1,100000);


    //glColor3f(0.75,0.75,0.75); //light grey
    //glColor3f(.68,.56,.2);//mud brown


   glEnd();
 /*   glColor3f(0,0,.25);
    glLineWidth(1);
    glBegin(GL_LINES);
    for(x=-100000;x<=100000;x=x+15)
    {
        glVertex3f(x,0,-100000);
        glVertex3f(x,0,100000);
    }
    glEnd();
    glBegin(GL_LINES);
    for(z=-100000;z<=100000;z=z+15)
    {
        glVertex3f(-100000,0,z);
        glVertex3f(100000,0,z);
    }
    glEnd();
    glLineWidth(1);
    glColor3f(.086,.56,.06);
    glBegin(GL_POLYGON);
    glVertex3f(-100000,-2,-1000);
    glVertex3f(-100000,-2,1000);
    glVertex3f(100000,-2,1000);
    glVertex3f(100000,-2,-1000);
    glEnd();*/

}
void changelookat()//change lookat functions changes the view by moving it either up or down
{
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(-25+front,15+up,0,0+front,0,0,0,1,0);
}


void idle(int value)
{

        if(incX>0)
        {
            incX=incX-dec;
            front=front+incX;
            keyPressed=-1;
        }
        else
        {
            keyPressed=0;
        }
        glutPostRedisplay();


}

void object(void)
{
    glColor3f(0,0,0);
    glPushMatrix();
    glTranslatef(10,2,0);
    glutSolidCube(20);

    glPopMatrix();
}

void display()
{
/////////////////////////////////////////////////////////////////////////////////
captured=cvQueryFrame(video1);
	disp=cvCreateImage(cvGetSize(captured),IPL_DEPTH_8U,3);	
	eroded=cvCreateImage(cvGetSize(captured),IPL_DEPTH_8U,3);	
	dilated=cvCreateImage(cvGetSize(captured),IPL_DEPTH_8U,3);


	

	thresh1=150;
	thresh2=120;
	thresh3=120;


	for(i=0;i<disp->height;i++)
	for(j=0;j<disp->width;j++)
		{
			data=cvGet2D(captured,i,j);
			
			if((data.val[1]>thresh1&&data.val[2]<thresh2&&data.val[0]<thresh3)||(data.val[2]>thresh1&&data.val[1]<thresh2&&data.val[0]<thresh3))
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
			
			if((data.val[1]>thresh1&&data.val[2]<thresh2&&data.val[0]<thresh3)||(data.val[2]>thresh1&&data.val[1]<thresh2&&data.val[0]<thresh3))
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
			
			if((data.val[1]>thresh1&&data.val[2]<thresh2&&data.val[0]<thresh3))
			{	acceleration=0;
				goto doneleft;	
			}
			else if(data.val[2]>thresh1&&data.val[1]<thresh2&&data.val[0]<thresh3)
			{	acceleration=1;
				goto doneleft;
			}
			
		}
	doneleft:
	xb=j;
	yb=i;
	
	inclination=(atan((yt-yb)/(xt-xb))-atan(1/2.1))*180/3.14;
	//bend=inclination;
	
	if(inclination<-70&&inclination>-85)
	
		inclination=-2;
	
	else if ((inclination>-70)&&(inclination<-55))
		
		inclination=-1;
	
	
	else if(inclination>5&&inclination<20)
	
		inclination=1;
	
	else if(inclination>20&&inclination<50)

		inclination=2;
	else 
		inclination=0;
	

	printf("%f    %f (%f)\n",inclination,acceleration,c++);

/*	
	xc=(xt-xb)/2;
	yc=(yb-yt)/2;

	printf("%f\n",ydef-yc);
*/
	cvNamedWindow("Cap");
	cvShowImage("Cap",dilated);
	cvWaitKey(3);
	
	
///////////////////////////////////////////////////////////////////////////////////	


    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    //glEnable(GL_DEPTH_TEST);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    GLfloat ambientLight[] = {1.0f, 1.0f, 1.0f, 1.0f};
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, ambientLight);

    GLfloat directedLight[] = {0.0f, 1.0f, 1.0f, 1.0f};
	GLfloat directedLightPos[] = {1.0f, 0.3f, 0.9f, 1.0f};
	glLightfv(GL_LIGHT3, GL_DIFFUSE, directedLight);
	glLightfv(GL_LIGHT3, GL_POSITION, directedLightPos);


	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, _textureId);

	//Bottom
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glColor3f(1.0f, 1.0f,1.0f);
	glBegin(GL_QUADS);
	glNormal3f(0.0, 0.0f, -1.0f);
	glTexCoord2f(0.0f, 0.5f);
	glVertex3f(-99355.5f, 50.5f, -100.9f);
	glTexCoord2f(280.0f, 0.5f);
	glVertex3f(100000.5f, 50.5f, -100.9f);
	glTexCoord2f(280.0f, 1.0f);
	glVertex3f(100000.5f, 200.5f, -100.9f);
	glTexCoord2f(0.0f, 1.0f);
	glVertex3f(-99355.5f, 200.5f, -100.9f);


	glEnd();
	//glDisable(GL_TEXTURE_ID);

    gluLookAt(-25+front,15+up,0,0+front,0,0,0,1,0);
    if(flag==1)
    {
        changelookat();
    }
    //object();

    grid();
    road();
    //ball();
	//glPushMatrix();
	//glTranslatef(-15+front,6,0+lr);
	//glPushMatrix();
	//glTranslatef(10,1,0);
	//glRotatef(-90,0,1,0);
	//glRotatef(-20,1,0,0);
	bikeMovement(acceleration,-inclination);
	Bike();
	drawObject();
	//glPopMatrix();
	//glPopMatrix();
    if(keyPressed==1)
    {
        glutTimerFunc(1000,idle,1);
    }
    else if(keyPressed==-1)
    {
        glutTimerFunc(4,idle,1);
    }
    else
    {
        // Do Nothing
    }
    /*if(lrKeyPressed==1)
    {
        //specialKeys(GLUT_KEY_UP,mouseX,mouseY);
        lrKeyPressed=0;
        specialKeys(GLUT_KEY_REPEAT_DEFAULT,mouseX,mouseY);
    }*/
    glutTimerFunc(40,movingObject,1);
    glutSwapBuffers();
}

void handleKeypress(unsigned char key, int x, int y) {
	switch (key) {
	    case 's' : xot++;
	    titsy=0;
        glutPostRedisplay();
        break;

        case 'w' : xot--;
        titsy=0;
        glutPostRedisplay();
        break;

        case 'y' : enterfl=xot;
			if(xot%4==3){
				cleanup();
                   exit(0) ;
					}
                else if(xot%4==0)
				{
					glViewport(0,0,1280,720);

				   glMatrixMode(GL_PROJECTION);
				   glLoadIdentity();

				   gluPerspective(120,1280.0/720,.1,200);
					glutDisplayFunc(display);
					glutReshapeFunc(changeSize);
					glutKeyboardFunc(keyboard);
					glutTimerFunc(40,movingObject,1);
					glutSpecialFunc(specialKeys);
					glutPostRedisplay();
				}
				else    glutPostRedisplay();
                break;

		//case 27: //Escape key
		//	cleanup();
			//exit(0);

	}
}


int main()
{
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
    glutInitWindowSize(1280,720);
    glutInitWindowPosition(0,0);
    glutCreateWindow("RoadBash");
    init();
	_scale = computeScale(STRS);
    glutDisplayFunc(drawScene);
	//glutDisplayFunc(display);
    //glutReshapeFunc(changeSize);
	glutReshapeFunc(handleResize);
    //glutKeyboardFunc(keyboard);
	glutKeyboardFunc(handleKeypress);
    
    setup();
		video1=cvCaptureFromCAM(-1);
    initObjects();
    //glutTimerFunc(40,movingObject,1);
	glutTimerFunc(25, update, 0);
    glutMainLoop();
    return 0;
}


