
#include<stdio.h>
#include<stdlib.h>
#include<glut.h>
//#include<glu.h>
#include "imageloader.h"

#define OBJECT_SIZE 5
#define OBJECT_SPEED .2

void initObjects(void);

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

float up=0,front=0,lr=0,acc=.04,dec=.001;
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


void init(void){
    glEnable(GL_DEPTH_TEST);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glEnable(GL_NORMALIZE);
	glEnable(GL_COLOR_MATERIAL);


Image* image = loadBMP("24 (3).bmp");
	_textureId = loadTexture(image);
	delete image;

image = loadBMP("Italy Ann 154.bmp");
	_textureIdroad = loadTexture(image);
	delete image;

image = loadBMP("4740894.bmp");
	_textureIdgrass = loadTexture(image);
	delete image;
}
void specialKeys(int key,int x,int y)
{
    keyPressed=1;

    if(key==GLUT_KEY_UP)
    {
        if(incX<1.5)
        {
            incX=incX+acc;
            front=front+incX;
        }
        else
        {
            incX=1.5;
            front=front+incX;
        }
        //printf("%f\t",incX);
        glutPostRedisplay();
    }
    else if(key==GLUT_KEY_RIGHT)
    {
        lrKeyPressed=1;
        mouseX=x;
        mouseY=y;
        if(lr<=50)
        {
            lr=lr+1.5;
            //front=front+incX;
        }
        else
        {
            lr=lr;
            //front=front+incX;
        }
        glutPostRedisplay();
    }
    else if(key==GLUT_KEY_LEFT)
    {
        lrKeyPressed=1;
        mouseX=x;
        mouseY=y;
        if(lr>=-50)
        {
        lr=lr-1.5;
        }
        else
        {
            lr=lr;
        }
        glutPostRedisplay();
    }
    else if(key==GLUT_KEY_DOWN)
    {
		incX=0;
        glutPostRedisplay();
    }
    else
    {
        incX=incX+acc;
        front=front+incX;
        glutPostRedisplay();
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
            break;
        case ' ':

			if(incX>0)
			{
				incX=incX-acc;
				front=front+incX;
			}
			else
			{
				incX=0;
			}
			break;
        case 'n':
            incX=1.5;
            front=front+1.5;
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
}
void moveObjectsH(void)
{
    int i;
    static int count=0;
    int temp;
    int temp1=-1;
    if(count==0)
    {
        for(i=lowerBound;i<numberOfObjects;i++)
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
        for(i=lowerBound;i<numberOfObjects;i++)
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
     for(i=0;i<numberOfObjects;i++)
     {
         objects[i].pos.x=i*50;
         objects[i].pos.y=1+(objects[i].size/2.0);
         //objects[i].pos.z=j+10;
     }
     for(i=0;i<numberOfObjects;i++)
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
    for(i=lowerBound;i<numberOfObjects;i++)
    {
        glPushMatrix();
        glTranslatef(objects[i].pos.x,objects[i].pos.y,objects[i].pos.z);
        glutSolidCube(OBJECT_SIZE);
        glPopMatrix();
    }
}
void objectsWithinBallsRange(float x,float y,float z)
{
    int i;
    int temp=0;
    for(i=0;i<numberOfObjects;i++)
    {
        if(x<=objects[i].pos.x)
        {
            lowerBound=i-2;
            break;
        }
    }
    if(lowerBound<0)
        lowerBound=0;
    upperBound=i+2;

}
int collisionTest(float x,float y,float z)
{
    //float maxX=0,maxY=12.0,maxZ=10.0,minX=0.0,minY=-10.0,minZ=-10;
    int valueReturn=0;
    int i;
    objectsWithinBallsRange(x,y,z);
    //for(i=lowerBound;i<=upperBound;i++)
    for(i=lowerBound;i<=upperBound;i++)
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

    ifCollided=collisionTest(ballX+5,ballY,ballZ);
    //printf("%f\t%f\t%f\n",ballX,ballY,ballZ);
    if(ifCollided==1)
    {
       front=front-1;
       incX=0;
    }
    ifCollided=collisionTest(ballX,ballY,ballZ-5);
    if(ifCollided==1)
    {
        lr=lr+1;
        incX=0;
    }
    ifCollided=collisionTest(ballX,ballY,ballZ+5);
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
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    //glEnable(GL_DEPTH_TEST);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    GLfloat ambientLight[] = {1.0f, 1.0f, 1.0f, 1.0f};
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, ambientLight);

    GLfloat directedLight[] = {0.0f, 1.0f, 1.0f, 1.0f};
	GLfloat directedLightPos[] = {1.0f, 0.3f, 0.9f, 1.0f};
	glLightfv(GL_LIGHT0, GL_DIFFUSE, directedLight);
	glLightfv(GL_LIGHT0, GL_POSITION, directedLightPos);


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
    drawObject();
    grid();
    road();
    ball();
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
int main()
{
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
    glutInitWindowSize(700,700);
    glutInitWindowPosition(400,0);
    glutCreateWindow("RoadBash");
    init();
    glutDisplayFunc(display);
    glutReshapeFunc(changeSize);
    glutKeyboardFunc(keyboard);
    glutSpecialFunc(specialKeys);
    setup();
    initObjects();
    glutTimerFunc(40,movingObject,1);
    glutMainLoop();
    return 0;
}


