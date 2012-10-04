
/********************************************************************************/
/*   header																		*/
/********************************************************************************/

#include <GL/glut.h>	
#include <stdio.h>	
#include <math.h>

#include <stdlib.h>
#include<gl\glut.h>

//#include "Terrain_Arch.h"

#include <stdlib.h>
#include <sys/types.h>

#include "Sphere_Func.h"
#include "globalVar.h"


	#define PI 3.14159265		//for creating a disk		


	GLdouble xy_Tiff[4] = {0.0, 0.0, 1.0, 0.0};
	GLdouble yz_Tiff[4] = {1.0, 0.0, 0.0, 0.0};
	GLdouble zx_Tiff[4] = {0.0, 1.0, 0.0, 0.0};
	// Clipping planes for perusal.

	float x_AngleTiff ;		// The rotation angles about x, y and z axis.
	float y_AngleTiff ;
	float z_AngleTiff ;
	typedef GLfloat Point[3];
	Point ptTiff[] = {
	{ -1.0, -1.0, 1.0 },
	{ -1.0, 1.0, 1.0 },
	{ 1.0, 1.0, 1.0 },
	{ 1.0, -1.0, 1.0 },
	{ -1.0, -1.0, -1.0 },
	{ -1.0, 1.0, -1.0 },
	{ 1.0, 1.0, -1.0 },
	{ 1.0, -1.0, -1.0 } };	// Definition of the vertices of the cube.

	typedef GLfloat NormalTiff[3];
	NormalTiff normalTiff[] = {
	{ 0.0, 0.0, 1.0 },
	{ 1.0, 0.0, 0.0 },
	{ 0.0, -1.0, 0.0 },
	{ 0.0, 1.0, 0.0 },
	{ 0.0, 0.0, -1.0 },
	{ -1.0, 0.0, 0.0 }
	};						// Definition of the face normal for each side of the cube.



	GLdouble eqnTiff[4] = {1.0, 1.0, 1.0, -2.0};	// Clipping planes ..

	bool enableCutPlane_Tiff = false;
	float cuttingPos_Tiff = 3.5;
	float gun_fall_angle_Tiff = 0;
/****************************************************************************************/
void drawSide_Tiff (int v0, int v1, int v2, int v3, int n)
				
	{

			glBegin(GL_QUADS);
					// All primitive drawings happen between glBegin / glEnd calls.	
				glNormal3fv(normalTiff[n]);			
				glVertex3fv(ptTiff[v0]);
					// OpenGL call to provide a vertex.
				glVertex3fv(ptTiff[v1]);
				glVertex3fv(ptTiff[v2]);
				glVertex3fv(ptTiff[v3]);
			glEnd();
	}
/****************************************************************************************/
void drawCube_Tiff(unsigned int key)

		
	{
		drawSide_Tiff(0, 3, 2, 1, 0);
		drawSide_Tiff(3, 7, 6, 2, 1);
		drawSide_Tiff(0, 4, 7, 3, 2);
		drawSide_Tiff(1, 2, 6, 5, 3);
		drawSide_Tiff(7, 4, 5, 6, 4);
		drawSide_Tiff(4, 0, 1, 5, 5);
	}
/****************************************************************************************/
void drawDisk_Tiff(GLdouble innerRadius, GLdouble outerRadius, GLint slices, GLint rings,
GLdouble startAngle, GLdouble sweepAngle)

							// Used to generate a cylinder shape.
{
	GLUquadricObj* pObj;

	pObj = gluNewQuadric();
							// Creates a new quadrics object and returns a pointer to it.
	gluQuadricDrawStyle(pObj, GLU_FILL);

	gluPartialDisk(pObj, innerRadius, outerRadius, slices, rings, startAngle, sweepAngle);
						// Draw the cylinder with a radius : fRadius.
	gluDeleteQuadric(pObj);
							// Free the Quadric
}
/****************************************************************************************/
//Used to generate a pyramid without the bottom square for further use
void drawTriangle_Tiff(void)
{
	glPushMatrix();

			glBegin(GL_TRIANGLE_FAN);
				glVertex3f(0,0,0);
				glVertex3f(-15,1,2.5);
				glVertex3f(-15,1,-2.5);
				glVertex3f(-15,-1,-2.5);
				glVertex3f(-15,-1,2.5);
				glVertex3f(-15,1,2.5);
			glEnd();

	glPopMatrix();
}
/****************************************************************************************/
void drawSphere_Tiff(float fRadius, GLint slices, GLint rings)
							// Used to generate a Sphere shape.
{
	GLUquadricObj* pObj;
	pObj =  gluNewQuadric();
							// Creates a new quadrics object and returns a pointer to it.

	gluQuadricDrawStyle(pObj, GLU_FILL);

	gluSphere(pObj, fRadius,slices, rings);
							// Draw the sphere with a radius : fRadius.
	gluDeleteQuadric(pObj);
							// Free the Quadric

}
/****************************************************************************************/
void drawCylinder_Tiff(float fTopR, float fBottomR, float fHeight)
							// Used to generate a cylinder shape.
{
	GLUquadricObj* pObj;
							// To keep the original texture intact we need to set the current color to WHITE.

	pObj = gluNewQuadric();
							// Creates a new quadrics object and returns a pointer to it.
	gluQuadricDrawStyle(pObj, GLU_FILL);

	gluCylinder(pObj, fTopR, fBottomR, fHeight, 20, 20);
							// Draw the cylinder with a radius : fRadius.
	gluDeleteQuadric(pObj);
							// Free the Quadric

}

/****************************************************************************************/
void drawOneWheel_Tiff(void)
{

	int f;
	int t=0;
	//Draws a pair of frames inside the wheel
		glPushMatrix();
			glColor3f(0.8, 0.8, 0.8);
			glTranslatef(4.2, -4.8, 0.5);
			glBegin(GL_TRIANGLE_FAN);
				glVertex3f(0,0,0);
				for(f=0; f <=360; f+=10)
					glVertex3f(2*cos(PI * f/180.0),2*sin(PI * f/180.0),0);
			glEnd();
			glTranslatef(0.0, 0.0, -1.0);
		glPopMatrix();
		glPushMatrix();
			glColor3f(0.8, 0.8, 0.8);
			glTranslatef(4.2, -4.8, -0.5);
			glBegin(GL_TRIANGLE_FAN);
				glVertex3f(0,0,0);
				for(f=0; f <=360; f+=10)
					glVertex3f(2*cos(PI * f/180.0),2*sin(PI * f/180.0),0);
			glEnd();
			glTranslatef(0.0, 0.0, -1.0);
		glPopMatrix();

// to complete the frame
			glPushMatrix();
				glColor3f(0.8, 0.8, 0.8);
				glTranslatef(4.2, -4.8, -0.5);
				drawCylinder_Tiff(2.1, 2.1, 1.0);
			glPopMatrix();

//the "wheel"
			glPushMatrix();
				glColor3f(0.1, 0.1, 0.1);
				glTranslatef(4.2, -4.8, 0.0);
				glutSolidTorus(0.8, 1.6, 50, 50);
			glPopMatrix();

//draw bumps on the wheel
			glPushMatrix();
				glTranslatef(5.3, -2.9, -0.65);
				drawCylinder_Tiff(0.3, 0.3, 1.4);
			glPopMatrix();
			glPushMatrix();
				glTranslatef(3.1, -2.9, -0.65);
				drawCylinder_Tiff(0.3, 0.3, 1.4);
			glPopMatrix();
			glPushMatrix();
				glTranslatef(6.4, -4.8, -0.65);
				drawCylinder_Tiff(0.3, 0.3, 1.4);
			glPopMatrix();
			glPushMatrix();
				glTranslatef(2.0, -4.8, -0.65);
				drawCylinder_Tiff(0.3, 0.3, 1.4);
			glPopMatrix();
			glPushMatrix();
				glTranslatef(5.3, -6.7, -0.65);
				drawCylinder_Tiff(0.3, 0.3, 1.4);
			glPopMatrix();
			glPushMatrix();
				glTranslatef(3.1, -6.7, -0.65);
				drawCylinder_Tiff(0.3, 0.3, 1.4);
			glPopMatrix();

}
/****************************************************************************************/
//Draws one complete Gear Wheel
void drawGearWheel_Tiff(void)
{
	int f;
	//sides of the gear whell
	glPushMatrix();
		glTranslatef(4.2, -4.8, 0.5);
		glTranslatef(3.0, 2.5, 0.0);
		glColor3f(0.2, 0.2, 0.2);
		glScalef(0.8,0.8,0.8);
		glBegin(GL_TRIANGLE_FAN);
			glVertex3f(0,0,0);
			for(f=0; f <=360; f+=10)
				glVertex3f(2*cos(PI * f/180.0),2*sin(PI * f/180.0),0);
		glEnd();
	glPopMatrix();
	glPushMatrix();
		glTranslatef(4.2, -4.8, 0.5);
		glTranslatef(3.0, 2.5, -1.0);
		glColor3f(0.2, 0.2, 0.2);
		glScalef(0.8,0.8,0.8);
		glBegin(GL_TRIANGLE_FAN);
			glVertex3f(0,0,0);
			for(f=0; f <=360; f+=10)
				glVertex3f(2*cos(PI * f/180.0),2*sin(PI * f/180.0),0);
		glEnd();
		
	glPopMatrix();

	//Central cylinder
	glPushMatrix();
		glColor3f(0.4,0.4,0.4);
		glTranslatef(4.2, -4.8, -0.5);
		glTranslatef(3.0, 2.5, 0.0);
		drawCylinder_Tiff(1.0,1.0,0.9);
	glPopMatrix();
	//5 Cube Gears
		glPushMatrix();
			glColor3f(0.3, 0.3, 0.2);
			glTranslatef(3.0, 2.5, 0.0);
			glPushMatrix();
				glTranslatef(5.3, -3.9, 0.0);
				glScalef(0.2, 0.2, 0.50);
				drawCube_Tiff(0);
			glPopMatrix();
			glPushMatrix();
				glTranslatef(3.1, -3.9, 0.0);
				glScalef(0.2, 0.2, 0.50);
				drawCube_Tiff(0);
			glPopMatrix();
			glPushMatrix();
				glTranslatef(5.7, -4.8, 0.0);
				glScalef(0.2, 0.2, 0.50);
				drawCube_Tiff(0);
			glPopMatrix();
			glPushMatrix();
				glTranslatef(2.6, -4.8, 0.0);
				glScalef(0.2, 0.2, 0.50);
				drawCube_Tiff(0);
			glPopMatrix();
			glPushMatrix();
				glTranslatef(5.3, -5.9, 0.0);
				glScalef(0.2, 0.2, 0.50);
				drawCube_Tiff(0);
			glPopMatrix();
			glPushMatrix();
				glTranslatef(3.1, -5.9, 0.0);
				glScalef(0.2, 0.2, 0.50);
				drawCube_Tiff(0);
			glPopMatrix();
		glPopMatrix();

}
/****************************************************************************************/


//draws one single caterpiller for further use
void drawOneChain_Tiff(void)
{
	glPushMatrix();
		glColor3f(0.4, 0.0, 0.0);
		glPushMatrix();
			glScalef(0.6, 0.1, 0.8);
				glTranslatef(-2.65, 0.0, 0.0);
				drawCube_Tiff(0);
		glPopMatrix();

		glPushMatrix();
			glColor3f(0.7, 0.2, 0.5);
			glTranslatef(-0.8,0.0, -0.80);

					glTranslatef(-1.589, 0.0, 0.0);
					drawCylinder_Tiff(0.2, 0.2, 1.6);
		glPopMatrix();
	glPopMatrix();
}

/****************************************************************************************/

void drawTwoChain_Tiff(void)
{
	int s;
	glPushMatrix();

		glColor3f(0.4, 0.0, 0.0);


		glPushMatrix();
			glScalef(0.6, 0.1, 0.8);
			for(s=0; s<=1; s+=1)	
			{
		
				glTranslatef(-2.65, 0.0, 0.0);
				drawCube_Tiff(0);
			}
		glPopMatrix();

		glPushMatrix();
			glColor3f(0.7, 0.2, 0.5);
			glTranslatef(-0.8,0.0, -0.80);
				for (s=0; s<=1; s+=1)
				{
					glTranslatef(-1.589, 0.0, 0.0);
					drawCylinder_Tiff(0.2, 0.2, 1.6);
				}
		glPopMatrix();

	glPopMatrix();

}
/****************************************************************************************/
/****************************************************************************************/
//Draws the full caterpillar set using the function drawOneChain and drawTwoChain
void drawTrack_Tiff(void)
{
	int s=0;
	//lower parallel track
	glPushMatrix();

	glPushMatrix();
		glColor3f(0.4, 0.0, 0.0);
		glTranslatef(4.4, -7.3, 0.0);
		glScalef(0.6, 0.1, 0.8);
		drawCube_Tiff(0);
	//Draws a sequence of caterpillar track with repeatition
	for(s=0; s<=9; s+=1)	
	{
		
		glTranslatef(-2.65, 0.0, 0.0);
		drawCube_Tiff(0);
	}
	glPopMatrix();

	glPushMatrix();
		glColor3f(0.7, 0.2, 0.5);
		glTranslatef(5.2, -7.3, -0.80);
		drawCylinder_Tiff(0.2, 0.2, 1.6);
	glPopMatrix();

	glPushMatrix();
		glColor3f(0.7, 0.2, 0.5);
		glTranslatef(3.6, -7.3, -0.80);
		drawCylinder_Tiff(0.2, 0.2, 1.6);
	for (s=0; s<=9; s+=1)
	{
		glTranslatef(-1.589, 0.0, 0.0);
		drawCylinder_Tiff(0.2, 0.2, 1.6);
	}
	glPopMatrix();
	// higher parallel track
	glPushMatrix();
		glColor3f(0.4, 0.0, 0.0);
		glTranslatef(4.0, -2.2, 0.0);
		glScalef(0.6, 0.1, 0.8);
		drawCube_Tiff(0);

	for(s=0; s<=9; s+=1)	
	{
		
		glTranslatef(-2.65, 0.0, 0.0);
		drawCube_Tiff(0);
	}
	glPopMatrix();

	glPushMatrix();
		glColor3f(0.7, 0.2, 0.5);
		glTranslatef(4.8, -2.2, -0.80);
		drawCylinder_Tiff(0.2, 0.2, 1.6);
	glPopMatrix();

	glPushMatrix();
		glColor3f(0.7, 0.2, 0.5);
		glTranslatef(3.2, -2.2, -0.80);
		drawCylinder_Tiff(0.2, 0.2, 1.6);
	for (s=0; s<=8; s+=1)
	{
		glTranslatef(-1.589, 0.0, 0.0);
		drawCylinder_Tiff(0.2, 0.2, 1.6);
	}
	glPopMatrix();

	//draw caterpillar one by one to make it fit the gear wheel
	glPushMatrix();
		glTranslatef(4.4, -7.5, 0.0);
		glTranslatef(3.9,2.8,0.0);
		glRotatef(40,0.0,0.0,1.0);
		drawTwoChain_Tiff();
	glPopMatrix();

	glPushMatrix();
		glTranslatef(4.5, -7.0, 0.0);
		glTranslatef(5.2,5.2,0.0);
		glRotatef(60,0.0,0.0,1.0);
		drawTwoChain_Tiff();
	glPopMatrix();

	glPushMatrix();
		glTranslatef(4.3, -2.8, 0.0);
		glRotatef(230,0.0,0.0,1.0);
		drawOneChain_Tiff();
	glPopMatrix();

	glPushMatrix();
		glTranslatef(5.2, -1.7, 0.0);
		glRotatef(215,0.0,0.0,1.0);
		drawOneChain_Tiff();
	glPopMatrix();

	glPushMatrix();
		glTranslatef(8.1, -0.4, 0.0);
		glRotatef(120,0.0,0.0,1.0);
		drawOneChain_Tiff();
	glPopMatrix();

	glPushMatrix();
		glTranslatef(6.5, 0.2, 0.0);
		glRotatef(150,0.0,0.0,1.0);
		drawOneChain_Tiff();
	glPopMatrix();

//do the same thing with the back side
	glPushMatrix();
		glRotatef(180, 0.0,1.0,0.0);

		glPushMatrix();
			glTranslatef(15.3, -4.7, 0.0);
			glRotatef(40,0.0,0.0,1.0);
			drawTwoChain_Tiff();
		glPopMatrix();

		glPushMatrix();
			glTranslatef(16.7, -1.8, 0.0);
			glRotatef(60,0.0,0.0,1.0);
			drawTwoChain_Tiff();
		glPopMatrix();

		glPushMatrix();
			glTranslatef(15.1, -0.4, 0.0);
			glRotatef(120,0.0,0.0,1.0);
			drawOneChain_Tiff();
		glPopMatrix();

		glPushMatrix();
			glTranslatef(13.5, 0.2, 0.0);
			glRotatef(150,0.0,0.0,1.0);
			drawOneChain_Tiff();
		glPopMatrix();

		glPushMatrix();
			glScalef(0.5,1.0,1.0);
			glTranslatef(24.3, -3.0, 0.0);
			glRotatef(230,0.0,0.0,1.0);
			drawOneChain_Tiff();
		glPopMatrix();

		glPushMatrix();
			glTranslatef(12.2, -1.7, 0.0);
			glRotatef(215,0.0,0.0,1.0);
			drawOneChain_Tiff();
		glPopMatrix();
	glPopMatrix();
	glPopMatrix();

}
/****************************************************************************************/
//Assemble the wheels and the track
void drawWheelSet_Tiff(void)
{
	glPushMatrix();
		drawTrack_Tiff();
		//first wheel
		drawOneWheel_Tiff();
		//following wheels
		glPushMatrix();
			glTranslatef(-5.0,0.0,0.0);
			drawOneWheel_Tiff();
			glTranslatef(-5.0,0.0,0.0);
			drawOneWheel_Tiff();
			glTranslatef(-5.0,0.0,0.0);
			drawOneWheel_Tiff();
		glPopMatrix();
	//first Gearwheel
		drawGearWheel_Tiff();
	//following Gearwheel
		glPushMatrix();
			glTranslatef(-21.4,-0.1,0.0);
		//	glRotatef(2.5, 0.0,0.0,1.0);
			drawGearWheel_Tiff();
		glPopMatrix();
	glPopMatrix();		


}

/****************************************************************************************/
//Assemble the 2 pairs of sets of wheels
void wheelAssemble_Tiff(void)
{
			glPushMatrix();
			drawWheelSet_Tiff();
			glTranslatef(0.0,0.0,-10.0);
			drawWheelSet_Tiff();
			glPopMatrix();
}
/****************************************************************************************/
//Draw the cover on the wheel
void drawCover_Tiff(void)
{
	glPushMatrix();
		glTranslatef(5.5, -0.5, 0.1);
		glRotatef(45, 0.0,0.0,1.0);
		glRotatef(90, 0.0, 1.0, 0.0);
		glScalef(1.0, 0.08, 1.8);
		glColor3f(0.1, 0.1, 0.1);
		drawCube_Tiff(0);
	glPopMatrix();

	glPushMatrix();
		glTranslatef(7.7, 0.7, 0.1);
	//	glRotatef(-45, 0.0,0.0,1.0);
		glRotatef(90, 0.0, 1.0, 0.0);
		glScalef(1.0, 0.08, 0.9);
		glColor3f(0.1, 0.1, 0.1);
		drawCube_Tiff(0);
	glPopMatrix();

	glPushMatrix();
		glTranslatef(10.2, -0.9, 0.1);
		glRotatef(-45, 0.0,0.0,1.0);
		glRotatef(90, 0.0, 1.0, 0.0);
		glScalef(1.0, 0.08, 2.3);
		glColor3f(0.1, 0.1, 0.1);
		drawCube_Tiff(0);
	glPopMatrix();
}
/****************************************************************************************/
//Draw light
void drawLight_Tiff(void)
{
	glPushMatrix();
	glRotatef(20, 0.0,0.0,1.0);
	glPushMatrix();
		glColor3f(0.1,0.1,0.1);
		glRotatef(90, 0.0, 1.0, 0.0);
		drawCylinder_Tiff(0.0,0.5,1.0);
	glPopMatrix();
	//light bulb
	glPushMatrix();
		glColor3f(1.1,1.1,1.1);
		glTranslatef(1.0,0.0,0.0);
		glScalef(0.8,1.3,1.0);
		drawSphere_Tiff(0.35,8,8);
	glPopMatrix();
	glPopMatrix();
}
/****************************************************************************************/
//Draw the body
void drawBody_Tiff(void)
{
	//central lower body
	glPushMatrix();
		glColor3f(0.3,0.3,0.2);
		glTranslatef(-3.5,-2.0,-5.02);
		glScalef(13.5,1.5,4.20);
		drawCube_Tiff(0);
	glPopMatrix();

	//front on-the-wheel body
	glPushMatrix();
		glColor3f(0.3,0.3,0.2);
		glTranslatef(-3.5,-1.25,0.0);
		glScalef(8.0,0.75,0.8);
		drawCube_Tiff(0);
	glPopMatrix();

	//back on-the-wheel body
	glPushMatrix();
		glColor3f(0.3,0.3,0.2);
		glTranslatef(-3.5,-1.25,0.0);
		glTranslatef(0.0,0.0,-10.0);
		glScalef(8.0,0.75,0.8);
		drawCube_Tiff(0);
	glPopMatrix();
	//triangular body short
	glPushMatrix();	
		glTranslatef(-3.48,2.5,-5.0);
		glRotatef(90, 0.0, 0.0, 1.0);
		glRotatef(90, 1.0, 0.0, 0.0);
		glColor3f(0.3, 0.3, 0.2);
		glScalef(0.2,5.78,3.2);
		drawTriangle_Tiff();
	glPopMatrix();
	//triangular body long
	glPushMatrix();	
		glTranslatef(-3.45,2.5,-5.0);
		glRotatef(90, 0.0, 0.0, 1.0);
		glRotatef(90, 1.0, 0.0, 0.0);
		glColor3f(0.3, 0.3, 0.2);
		glScalef(0.2,4.2,5.3);
		drawTriangle_Tiff();
	glPopMatrix();
	//cover
	drawCover_Tiff();
	
	glPushMatrix();
		glTranslatef(0.0,0.0,-10.3);
		drawCover_Tiff();
	glPopMatrix();

	glPushMatrix();
		glTranslatef(-6.5,0.0,0);
		glScalef(-1.0,1.0,1.0);
		drawCover_Tiff();
	glPopMatrix();

	glPushMatrix();
		glTranslatef(-6.5,0.0,-10.3);
		glScalef(-1.0,1.0,1.0);
		drawCover_Tiff();
	glPopMatrix();

	//front end
	glPushMatrix();
		glTranslatef(9.2,-2.0,-5.0);
		glScalef(0.5,0.65,0.9);
		glColor3f(0.4,0.4,0.2);
		glPushMatrix();
			glTranslatef(1.2,0.0,0.0);
			glClipPlane (GL_CLIP_PLANE0, yz_Tiff);	
			glEnable (GL_CLIP_PLANE0);
		
			glPushMatrix();
				glTranslatef(-1.2,0.0,0.0);
				glRotatef(90, 1.0, 0.0, 0.0);
				glutSolidTorus(2.5, 3.0, 8, 6);
			glPopMatrix();
			glDisable (GL_CLIP_PLANE0);
		glPopMatrix();
	glPopMatrix();
	//lights
	glPushMatrix();
		glTranslatef(6.5,0.0,-2.0);
		drawLight_Tiff();
	glPopMatrix();
	glPushMatrix();
		glTranslatef(6.5,0.0,-8.0);
		drawLight_Tiff();
	glPopMatrix();
	//put wheels as part of the tank		
	wheelAssemble_Tiff();
}
/****************************************************************************************/
void drawGun_Tiff(void)
{

	glPushMatrix();
		glColor3f(0.4,0.4,0.4);
		glScalef(0.8,0.6,1.7);
		drawCube_Tiff(0);
	glPopMatrix();
	
	//outer rotater
	glPushMatrix();
		glColor3f(0.6,0.6,0.6);
		glTranslatef(0.6,0.0,0.5);
		drawCylinder_Tiff(0.6,0.6,1.0);
	glPopMatrix();
	//inner rotater
	glPushMatrix();
		glColor3f(0.3,0.4,0.7);
		glTranslatef(0.6,0.0,0.5);
		drawCylinder_Tiff(0.4,0.4,1.0);
	glPopMatrix();
	//gun
	glPushMatrix();
		glColor3f(0.3,0.3,0.3);
		glTranslatef(1.0,0.0,1.0);
		glRotatef(90, 0.0, 1.0, 0.0);
		drawCylinder_Tiff(0.2,0.2,3.0);
	glPopMatrix();

	//second set
	glPushMatrix();
		glTranslatef(0.0,0.0,-2.0);
	//outer rotater
	glPushMatrix();
		glColor3f(0.6,0.6,0.6);
		glTranslatef(0.6,0.0,0.5);
		drawCylinder_Tiff(0.6,0.6,1.0);
	glPopMatrix();
	//inner rotater
	glPushMatrix();
		glColor3f(0.3,0.4,0.7);
		glTranslatef(0.6,0.0,0.5);
		drawCylinder_Tiff(0.4,0.4,1.0);
	glPopMatrix();
	//gun
	glPushMatrix();
		glColor3f(0.3,0.3,0.3);
		glTranslatef(1.0,0.0,1.0);
		glRotatef(90, 0.0, 1.0, 0.0);
		drawCylinder_Tiff(0.2,0.2,3.0);
	glPopMatrix();
	glPopMatrix();




}

/****************************************************************************************/

void drawBarrel_Tiff(void)
{
	glPushMatrix();
		glTranslatef(7.0,2.5,-5.0);
		glColor3f(0.6,0.6,0.6);
		glRotatef(90, 0.0, 1.0, 0.0);
	//inner gun
		drawCylinder_Tiff(0.4, 0.3, 13.5);

		glPushMatrix();
		glTranslatef(0.0,0.0,0.0);
	//right support
		glPushMatrix();
			glColor3f(0.4,0.4,0.4);
			glScalef(1.0,1.0,-1.0);
			glRotatef(35, 0.0, 1.0, 0.0);
			drawCylinder_Tiff(0.2, 0.2, 5.0);
		glPopMatrix();
	//left support
		glPushMatrix();
				glScalef(1.0,1.0,-1.0);
				glRotatef(-35, 0.0, 1.0, 0.0);
				drawCylinder_Tiff(0.2, 0.2, 5.0);
		glPopMatrix();

		glPopMatrix();
	//base of barrel
		glPushMatrix();
			glColor3f(0.3,0.3,0.3);
			glScalef(1.0,1.0,5.0);
			glutSolidTorus(0.4, 0.5, 10, 10);
		glPopMatrix();
	//outer gun
		glColor3f(0.4,0.4,0.4);
		drawCylinder_Tiff(0.5, 0.4, 13.0);
	glPopMatrix();
}

/****************************************************************************************/
void drawTurret_Tiff(void)
{
	//turret
	glPushMatrix();
		glScalef(1.6,0.7,1.05);
		glTranslatef(-2.0,3.5,-4.8);
		glColor3f(0.4,0.4,0.2);
		glRotatef(90, 1.0, 0.0, 0.0);
		glutSolidTorus(2.5, 3.0, 8, 6);
	glPopMatrix();
	drawBarrel_Tiff();

	glPushMatrix();
		glColor3f(0.3,0.3,0.3);
		glScalef(1.0,0.3,0.8);
		glTranslatef(-3.0,13.5,-6.0);
	//	glRotatef(90, 1.0, 0.0, 0.0);
		glClipPlane (GL_CLIP_PLANE0, zx_Tiff);	
		glEnable (GL_CLIP_PLANE0);
		drawSphere_Tiff(5.0,7,6);
		glDisable (GL_CLIP_PLANE0);
	glPopMatrix();

	glPushMatrix();
		glTranslatef(0.0, 4.9,-5.0);
		glRotatef(90,0.0,1.0,0.0);
		glScalef(0.7,0.3,0.3);
		glColor3f(0.1,0.1,0.1);
	//	glRotatef(90, 1.0, 0.0, 0.0);
		glClipPlane (GL_CLIP_PLANE0, zx_Tiff);	
		glEnable (GL_CLIP_PLANE0);
		glutSolidTorus(0.4, 2.0, 10, 10);
		glDisable (GL_CLIP_PLANE0);
	glPopMatrix();
	//gunSet
	glPushMatrix();
		glTranslatef(3.2,3.8,-5.0);
		drawGun_Tiff();
	glPopMatrix();
	
}

/****************************************************************************************/
//Assemble the tank and draw it
void drawTank_Tiff(float x, float y, float z)
{
	if (enableCutPlane_Tiff)
     {
		   GLdouble cutPlane1[4] = {0, -1, 0, cuttingPos_Tiff};
		   glClipPlane (GL_CLIP_PLANE1, cutPlane1);
           glEnable(GL_CLIP_PLANE1);

			glPushMatrix();
				glTranslatef(x,y,z);
				drawTurret_Tiff();
				drawBody_Tiff();
			glPopMatrix();
		   glDisable(GL_CLIP_PLANE1);
	}
	else{
		    if (is_collapse_Tiff){
				GLdouble cutPlane1[4] = {0, -1, 0, 1};
				glClipPlane (GL_CLIP_PLANE1, cutPlane1);
				glEnable(GL_CLIP_PLANE1);

				glPushMatrix();
					glTranslatef(x,y,z);
					drawTurret_Tiff();
					drawBody_Tiff();
				glPopMatrix();
				glDisable(GL_CLIP_PLANE1);
			}
			else{
				glPushMatrix();
					glTranslatef(x,y,z);
					drawTurret_Tiff();
					drawBody_Tiff();
				glPopMatrix();
			}
	}
}

