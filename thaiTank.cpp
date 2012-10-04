#include <GL/glut.h>	// This is assuming that you have glut.h set in your include path.
#include <stdio.h>		// Other necessary program related includes.
#include <math.h>
#include <iostream>
#include "Terrain_Arch.h" //we are using the same texture
#include "mountain.h"
#include "globalVar.h"
#include "Thuc_Tank.h"


//MOVEMENT OF THE TANK
	bool firstmove = true;	//flag to tell if this is the first move
//TEXTURE
	#define BODY_TEXTURE_thai 4
	#define TRACK_TEXTURE_thai 4
	#define textureNum_thai 6	//index 0 is not used
	GLuint texture_thai[textureNum_thai]; //aray to contain the texture

//PARAMETER TO DRAW PRIMITIVE FROM SCRATCH
	typedef GLfloat Point_thai[3];// Definition of the vertices of the cube.
	Point_thai pt_thai[] = {
	{ -1.0, -1.0, 1.0 },
	{ -1.0, 1.0, 1.0 },
	{ 1.0, 1.0, 1.0 },
	{ 1.0, -1.0, 1.0 },
	{ -1.0, -1.0, -1.0 },
	{ -1.0, 1.0, -1.0 },
	{ 1.0, 1.0, -1.0 },
	{ 1.0, -1.0, -1.0 } };
	typedef GLfloat Normal_thai[3];// Definition of the face normal for each side of the cube.
	Normal_thai normal_thai[] = {
	{ 0.0, 0.0, 1.0 },
	{ 1.0, 0.0, 0.0 },
	{ 0.0, -1.0, 0.0 },
	{ 0.0, 1.0, 0.0 },
	{ 0.0, 0.0, -1.0 },
	{ -1.0, 0.0, 0.0 }
	};
//OBJECT LIST: each object has 4 coordinate: x,y,z,radius
GLfloat bottomSet[2][4] = 
{	
	{80.0, -10, 0.0, 40.0},
	{80.0, -10, 0.0, 35.0}
};
GLfloat topSet[1][4] = 
{
	{80.0, 20, 0.0, 10.0}
};

//HILL PARAMETER
#define hillAngle 0.5 //0.5 radian


/****************************************************************************************/
void loadTexture_thai(void)
{
	CreateTexture("./textureThai/sand18.bmp", texture_thai, 1);	//ground
	CreateTexture("./textureThai/CamoMono.bmp", texture_thai, 2);
	CreateTexture("./textureThai/Camo2.bmp", texture_thai, 3);
	CreateTexture("./textureThai/metal2.bmp", texture_thai, 4);	//tank body
	CreateTexture("./textureThai/graydirt.bmp", texture_thai, 5);	//tank body
}
/****************************************************************************************/


void setSpotLight1(void)
{

	GLfloat	 lightPos1[] = { 9.5, 1, 0, 1 };		//in front of the tank
	glLightf(GL_LIGHT6,GL_SPOT_CUTOFF,15);			//30' in total
	GLfloat  spotDir1[] = { 1, -tan(30*PI/180), 0};	//30 degree clock-wise from horizontal line
    glLightf(GL_LIGHT6, GL_LINEAR_ATTENUATION, 0.2f);
    glLightf(GL_LIGHT6, GL_QUADRATIC_ATTENUATION, 0.02f);

	if(beam == false) //high beam
	{
		glLightf(GL_LIGHT6,GL_SPOT_CUTOFF,20);	//40' in total
		spotDir1[0] = 1;
		spotDir1[1] = -tan(5*PI/180);//10 degree clock-wise from horizontal line
		spotDir1[2]	= 0;
		glLightf(GL_LIGHT6, GL_LINEAR_ATTENUATION, 0.01f);
		glLightf(GL_LIGHT6, GL_QUADRATIC_ATTENUATION, 0.0f);
	}
	glLightfv(GL_LIGHT6,GL_POSITION,lightPos1); 
	glLightfv(GL_LIGHT6,GL_SPOT_DIRECTION,spotDir1);//downward 30' to the ground

	if(light1 == true)
		glEnable(GL_LIGHT6);// Enabling a light
	else
		glDisable(GL_LIGHT6);
}


void drawSide_thai (int v0, int v1, int v2, int v3, int n, int key)
// Draws one side of the cube and colors it.
{
	if(key == 0)	//do not bind
	{
		glBegin(GL_QUADS);
			glNormal3fv(normal_thai[n]);			
			glVertex3fv(pt_thai[v0]);
			glVertex3fv(pt_thai[v1]);
			glVertex3fv(pt_thai[v2]);
			glVertex3fv(pt_thai[v3]);
		glEnd();
	}
	else//bind the texture on the texture array with index key 
	{
		glBindTexture(GL_TEXTURE_2D, texture_thai[key]);
		glBegin(GL_QUADS);
			glNormal3fv(normal_thai[n]);			
			glTexCoord2f( 0.0f, 0.0f);	glVertex3fv(pt_thai[v0]);
			glTexCoord2f( 5.0f, 0.0f);	glVertex3fv(pt_thai[v1]);
			glTexCoord2f( 5.0f, 1.0f);	glVertex3fv(pt_thai[v2]);
			glTexCoord2f( 0.0f, 1.0f);	glVertex3fv(pt_thai[v3]);
		glEnd();
	}
}
/****************************************************************************************/
void drawCube_thai (unsigned int key)
// Draws the cube.
/*key is the index of the texture in the texture array*/
{
	drawSide_thai(0, 3, 2, 1, 0, key);
	drawSide_thai(3, 7, 6, 2, 1, key);
	drawSide_thai(0, 4, 7, 3, 2, key);
	drawSide_thai(1, 2, 6, 5, 3, key);
	drawSide_thai(7, 4, 5, 6, 4, key);
	drawSide_thai(4, 0, 1, 5, 5, key);
}
/****************************************************************************************/
void drawSphere_thai(float fRadius, GLint slices, GLint rings)	// Used to generate a Sphere shape.
{
	GLUquadricObj* pObj;
	pObj =  gluNewQuadric();// Creates a new quadrics object and returns a pointer to it.
	gluQuadricDrawStyle(pObj, GLU_FILL);
	gluSphere(pObj, fRadius,slices, rings);	// Draw the sphere with a radius : fRadius.
	gluDeleteQuadric(pObj);	// Free the Quadric
}

/****************************************************************************************/
void drawCylinder_thai(float fTopR, float fBottomR, float fHeight)
// Used to generate a cylinder shape.
{
	GLUquadricObj* pObj;	// To keep the original texture intact we need to set the current color to WHITE.
	pObj = gluNewQuadric();	// Creates a new quadrics object and returns a pointer to it.

	gluQuadricNormals(pObj, GL_SMOOTH);
	gluQuadricTexture(pObj, GL_TRUE);

	gluQuadricDrawStyle(pObj, GLU_FILL);

	gluCylinder(pObj, fTopR, fBottomR, fHeight, 20, 20);// Draw the cylinder with a radius : fRadius.
	gluDeleteQuadric(pObj);	// Free the Quadric
}

/****************************************************************************************/
void drawHexCylinder_thai(float fTopR, float fBottomR, float fHeight)
// Used to generate a Hexagon cylinder shape.
{
	GLUquadricObj* pObj;// To keep the original texture intact we need to set the current color to WHITE.
	pObj = gluNewQuadric();// Creates a new quadrics object and returns a pointer to it.
	
	gluQuadricNormals(pObj, GL_SMOOTH);
	gluQuadricTexture(pObj, GL_TRUE);
	
	gluQuadricDrawStyle(pObj, GLU_FILL);
	gluCylinder(pObj, fTopR, fBottomR, fHeight, 8, 10);// Draw the cylinder with a radius : fRadius.
	gluDeleteQuadric(pObj);// Free the Quadric
}
/****************************************************************************************/
void drawDisk_thai(GLdouble innerRadius, GLdouble outerRadius, GLint slices, GLint rings,
GLdouble startAngle, GLdouble sweepAngle)
// Used to generate a cylinder shape.
{
	GLUquadricObj* pObj;
	pObj = gluNewQuadric();// Creates a new quadrics object and returns a pointer to it.
	gluQuadricNormals(pObj, GL_SMOOTH);
	gluQuadricTexture(pObj, GL_TRUE);
	gluQuadricDrawStyle(pObj, GLU_FILL);
	gluPartialDisk(pObj, innerRadius, outerRadius, slices, rings, startAngle, sweepAngle);// Draw the cylinder with a radius : fRadius.						
	gluDeleteQuadric(pObj);// Free the Quadric
}


/**************************************************************************************/
void drawMountain_thai(GLfloat bottom[][4], GLfloat top[][4])
{
	glColor3f(1.0f, 1.0f, 1.0f);
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, texture[2]);

	glPushMatrix();

		glPushMatrix();	//mountain
			glTranslatef(topSet[0][0], 13, 0);
			glRotatef(90, 1, 0, 0);	//concave down
			//drawDisk_thai(0,topSet[0][3],10,10,0,360);//top cover
			glPushMatrix();
				glScalef(5,5,5);
				drawDisc_Thuc(1);
			glPopMatrix();
			drawCylinder_thai(topSet[0][3], bottomSet[0][3], (bottomSet[0][3] - topSet[0][3])*tan(hillAngle));			//sides
		glPopMatrix();	//end mountain

	glDisable(GL_TEXTURE_2D);
	glPopMatrix();
}
/*SHOW ENVIRONMENT*********************************************************************/
void showEnvironment_thai(void)
{
glColor3f(0.8,0.8,0.5);
	glPushMatrix();	
		drawMountain_thai(bottomSet, topSet);
	glPopMatrix();
}

/*DRAW TURRET BASE*********************************************************************/
void drawTurretBase_thai(void)
{
glColor3f(1.0f, 1.0f, 1.0f);
	glEnable(GL_TEXTURE_2D);
	glPushMatrix();	//base turret
		glScalef(1.5, 1, 1);			//lenghten the base
		glRotatef(22.5,0,1,0);			//align the side facing screen
		glRotatef(90, 1, 0, 0);			//concave down
		glBindTexture(GL_TEXTURE_2D, texture_thai[4]); //Camo
		drawDisk_thai(0,3,8,10,0,360);		//top cover
		drawHexCylinder_thai(3, 5, 2);		//sides
	glPopMatrix();	//end base turret
	glDisable(GL_TEXTURE_2D);
}
/*DRAW BARREL-THE CANON****************************************************************/
void drawBarrel_thai(void)
{
glColor3f(0.1,0.1,0.1);
	
	if(glossy)
	{
		glPushAttrib(GL_LIGHTING_BIT);	//the tank body has different material
		glMaterialfv(GL_FRONT, GL_AMBIENT, white);
		glMaterialfv(GL_FRONT, GL_DIFFUSE, gray);
		glMaterialfv(GL_FRONT, GL_SPECULAR, white);
		glMateriali(GL_FRONT, GL_SHININESS, 128);
	}
	glPushMatrix();	//canon
		glTranslatef(5, -1, 0);			//align to turret level
		glRotatef(pitch, 0, 0, 1);		//lift up
		glRotatef(90, 0, 1, 0);			//along x-axis
		drawCylinder_thai(0.5, 0.5, 15);
		drawCylinder_thai(0.3, 0.3, 15);
		drawDisk_thai(0.3,0.5,20,5,0,360);	//thickness disk
		glTranslatef(0,0,15);
		drawDisk_thai(0.3,0.5,20,5,0,360);	//thickness disk
	glPopMatrix();	//end canon
	if(glossy)
		glPopAttrib();
}
/*DRAW MACHINE GUN*********************************************************************/
void drawMachineGun_thai(void)
{
	float mgLeg = 2;	//length of machine gun leg
glColor3f(0.0,0.0,0.0);
	glPushMatrix();//machine gun
		glTranslatef(2, 0, 1.5);
		glPushMatrix();	//first leg
			glRotatef(-30, 0, 1, 0);
			glTranslatef(-mgLeg*cos(60*PI/180), 0, 0);
			glRotatef(-30, 0, 0, 1);
			glRotatef(-90, 1, 0, 0);
			drawCylinder_thai(0.05, 0.1, mgLeg);
		glPopMatrix();	//end first leg
		glPushMatrix();	//second leg
			glTranslatef(0, 0, mgLeg*cos(60*PI/180));
			glRotatef(-120, 1, 0, 0);
			drawCylinder_thai(0.05, 0.1, mgLeg);
		glPopMatrix();	//end second leg
		glPushMatrix();	//third leg
			glRotatef(30, 0, 1, 0);
			glTranslatef(mgLeg*cos(60*PI/180), 0, 0);
			glRotatef(120, 0, 0, 1);
			glRotatef(90, 0, 1, 0);
			drawCylinder_thai(0.05, 0.1, mgLeg);
		glPopMatrix();	//end third leg
			glColor3f(0.3,0.3,0.3);
		glPushMatrix(); //gun body
			glTranslatef(-1, mgLeg*sin(60*PI/180), 0);
			glRotatef(90, 0, 1, 0);
			glRotatef(roll,0,0,1);//roll around z-axis
			glColor3f(0,0,0);
			glPushMatrix();
				glTranslatef(0,0.2,0);//top
				drawCylinder_thai(0.1, 0.1, 5);
			glPopMatrix();
			glPushMatrix();
				glTranslatef(0,-0.2,0);//bottom
				drawCylinder_thai(0.1, 0.1, 5);
			glPopMatrix();
			glColor3f(0.3,0.3,0.3);
			glPushMatrix();
				glTranslatef(0.2,0,0);//right
				drawCylinder_thai(0.1, 0.1, 5);
			glPopMatrix();
			glPushMatrix();
				glTranslatef(-0.2,0,0);//left
				drawCylinder_thai(0.1, 0.1, 5);
			glPopMatrix();
		glPopMatrix();	//end gun body
	glPopMatrix();	//end machine gun
}
/*DRAW EXIT****************************************************************************/
void drawExit_thai(void)
{
	GLdouble exitPlane[4] = {0, 1, 0, 0};	//cutting plane for turret
glColor3f(0.5,0.5,0.5);
	glPushMatrix();		//exit door
		glTranslatef(-1, 0, 1.25);	//behind the machine gun
		glScalef(1, 0.5, 1);		//flaten
		glClipPlane (GL_CLIP_PLANE0, exitPlane);
		glEnable (GL_CLIP_PLANE0);
			glRotatef(30, 0, 0, 1);
			glutSolidDodecahedron();
		glDisable (GL_CLIP_PLANE0);
	glPopMatrix();	//end exit door
}
/*DRAW COMPLETE TURRET*****************************************************************/
void drawCompleteTurret_thai(void)
{
	glPushMatrix();
		glTranslatef(0, 4.5, 0);//whole turret is on top of body
		glRotatef(yaw, 0, 1, 0);//rotate by yaw degree about y-axis at origin
		drawTurretBase_thai();
		drawBarrel_thai();
		drawMachineGun_thai();
		drawExit_thai();
	glPopMatrix();
}
/*DRAW EXHAUSTER***********************************************************************/
void drawExhauster_thai(void)
{
glColor3f(0,0,0);//ancient chinese ink :P
	glPushMatrix();	
		glTranslatef(-7, -2, -2);
		glRotatef(-45, 0, 0, 1);
		glRotatef(90, 0, 1, 0);
		glTranslatef(0, 0, -5);
		drawCylinder_thai(0.5,0.2,3);
	glPopMatrix();
}
/*DRAW MAIN BODY***********************************************************************/
void drawMainBody_thai(void)
{
	int cutAngle0 = 10;
	//cutting plane for TOP FRONT of body
	GLdouble bodyPlane0[4] = {-tan(cutAngle0*PI/180), -1, 0, (2.5+4*tan(cutAngle0*PI/180))};
	int cutAngle1 = 20;
	//cutting plane for BOTTOM FRONT of body
	GLdouble bodyPlane1[4] = {-tan(cutAngle1*PI/180), 1, 0, (2.5+7*tan(cutAngle1*PI/180))};
	int cutAngle2 = 20;
	//cutting plane for BOTTOM REAR of body
	GLdouble bodyPlane2[4] = {tan(cutAngle2*PI/180), 1, 0, (2.5+7*tan(cutAngle2*PI/180))};
	int cutAngle3 = 45;
	//cutting plane for TOP FRONT of left and right body
	GLdouble miniPlane[4] = {-tan(cutAngle3*PI/180), -1, 0, (1+9*tan(cutAngle3*PI/180))};
	
	glPushMatrix();	//body
	if(glossy)
	{
		glPushAttrib(GL_LIGHTING_BIT);	//the tank body has different material
		glMaterialfv(GL_FRONT, GL_SPECULAR, white);
		glMateriali(GL_FRONT, GL_SHININESS, 128);
	}

	glEnable(GL_TEXTURE_2D);//apply texture to the whole body
		glPushMatrix();		//middle body

			glBindTexture(GL_TEXTURE_2D, texture_thai[BODY_TEXTURE_thai]);
			glBegin(GL_POLYGON);// close the TOP FRONT cut from middle body
				glNormal3f(tan(cutAngle0*PI/180), 1, 0);
				glTexCoord2f(0.0f, 1.0f);	glVertex3f( 4.0f, 2.5f, 3.0f);	// Top close
				glTexCoord2f(2.0f, 1.0f);	glVertex3f( 4.0f, 2.5f, -3.0f);	// Top far
				glTexCoord2f(2.0f, 0.0f);	glVertex3f( 10.0,(-10*tan(cutAngle0*PI/180) + 4*tan(cutAngle0*PI/180) + 2.5), -3.0);// Bottom far
				glTexCoord2f(0.0f, 0.0f);	glVertex3f( 10.0,(-10*tan(cutAngle0*PI/180) + 4*tan(cutAngle0*PI/180) + 2.5), 3.0);	// Bottom close
			glEnd();
			glBegin(GL_QUADS);	// close the BOTTOM FRONT cut from middle body
				glNormal3f(tan(cutAngle1*PI/180), -1, 0);
				glTexCoord2f(0.0f, 1.0f);	glVertex3f( 10.0,(-10*tan(-cutAngle1*PI/180) + 7*tan(-cutAngle1*PI/180) - 2.5), 3.0);// top close
				glTexCoord2f(2.0f, 1.0f);	glVertex3f( 10.0,(-10*tan(-cutAngle1*PI/180) + 7*tan(-cutAngle1*PI/180) - 2.5), -3.0);// top far
				glTexCoord2f(2.0f, 0.0f);	glVertex3f( 7.0f, -2.5f, -3.0f);	// bottom far
				glTexCoord2f(0.0f, 0.0f);	glVertex3f( 7.0f, -2.5f, 3.0f);		// bottom close
			glEnd();

			glBegin(GL_QUADS);	// close the BOTTOM REAR cut from middle body
				glNormal3f(-tan(cutAngle2*PI/180), -1, 0);
				glTexCoord2f(0.0f, 1.0f);	glVertex3f( -10.0,(10*tan(cutAngle2*PI/180) - 7*tan(cutAngle2*PI/180) - 2.5), 3.0);	// top close
				glTexCoord2f(2.0f, 1.0f);	glVertex3f( -10.0,(10*tan(cutAngle2*PI/180) - 7*tan(cutAngle2*PI/180) - 2.5), -3.0);// top far
				glTexCoord2f(2.0f, 0.0f);	glVertex3f( -7.0f, -2.5f, -3.0f);	// bottom far
				glTexCoord2f(0.0f, 0.0f);	glVertex3f( -7.0f, -2.5f, 3.0f);	// bottom close
			glEnd();

			glClipPlane (GL_CLIP_PLANE0, bodyPlane0);	//cut the TOP FRONT
			glEnable (GL_CLIP_PLANE0);
				glClipPlane (GL_CLIP_PLANE1, bodyPlane1);
				glEnable (GL_CLIP_PLANE1);		//cut the  BOTTOM FRONT
					glClipPlane (GL_CLIP_PLANE2, bodyPlane2);
					glEnable (GL_CLIP_PLANE2);	//cut the  BOTTOM REAR

						glScalef(10, 2.5, 3);	//middle body using custom draw

						drawCube_thai(BODY_TEXTURE_thai);	//replace this line by the 2 lines above to use glut
					glDisable (GL_CLIP_PLANE2);
				glDisable (GL_CLIP_PLANE1);
			glDisable (GL_CLIP_PLANE0);
		glPopMatrix();
		
		glPushMatrix();	//left body
			glTranslatef(0, 1.5 ,4);
			
			glBegin(GL_QUADS);			// close the cut
				glNormal3f(tan(cutAngle3*PI/180), 1, 0);
				glTexCoord2f(0.0f, 0.5f);	glVertex3f( 9.0f, 1.0f, 1.0f);	// Top close
				glTexCoord2f(0.5f, 0.5f);	glVertex3f( 9.0f, 1.0f, -1.0f);	// Top far
				glTexCoord2f(0.5f, 0.0f);	glVertex3f( 10.0,(-10*tan(cutAngle3*PI/180) + 9*tan(cutAngle3*PI/180) + 1), -1.0);	// Bottom far
				glTexCoord2f(0.0f, 0.0f);	glVertex3f( 10.0,(-10*tan(cutAngle3*PI/180) + 9*tan(cutAngle3*PI/180) + 1), 1.0);	// Bottom close
			glEnd();

			glClipPlane (GL_CLIP_PLANE0, miniPlane);
			glEnable (GL_CLIP_PLANE0);	//cut the TOP FRONT of the left body

				glScalef(10, 1, 1);	//middle body using custom draw

					drawCube_thai(BODY_TEXTURE_thai);	//replace this line by the 2 lines above to use glut
			glDisable (GL_CLIP_PLANE0);

		glPopMatrix();	//end left body

		glPushMatrix();	//right body
			glTranslatef(0, 1.5 ,-4);
			
			glBegin(GL_QUADS);			// close the cut
				glTexCoord2f(0.0f, 0.5f);	glVertex3f( 9.0f, 1.0f, 1.0f);		// Top close
				glTexCoord2f(0.5f, 0.5f);	glVertex3f( 9.0f, 1.0f, -1.0f);		// Top far
				glTexCoord2f(0.5f, 0.0f);	glVertex3f( 10.0,(-10*tan(cutAngle3*PI/180) + 9*tan(cutAngle3*PI/180) + 1), -1.0);	// Bottom far
				glTexCoord2f(0.0f, 0.0f);	glVertex3f( 10.0,(-10*tan(cutAngle3*PI/180) + 9*tan(cutAngle3*PI/180) + 1), 1.0);	// Bottom close
			glEnd();
			
			glEnable (GL_CLIP_PLANE0);	//cut the TOP FRONT of the right body

			glScalef(10, 1, 1);	//middle body using custom draw

					drawCube_thai(BODY_TEXTURE_thai);	//replace this line by the 2 lines above to use glut
			glDisable (GL_CLIP_PLANE0);

		glPopMatrix();	//end right body
	
	glDisable(GL_TEXTURE_2D);
	if(glossy)
		glPopAttrib();		//restore material properties to the rest of the world
	glPopMatrix();		//end body
}
/*draw left track**********************************************************************/
void drawLeftTrack_thai(void)
{
	GLdouble frontVerCut[4] = {cos(30*PI/180), sin(30*PI/180), 0, -9.5*cos(30*PI/180)};
	GLdouble frontHorCut[4] = {cos(90*PI/180-2*atan(0.5)), sin(90*PI/180-2*atan(0.5)),0, -9.5*cos(90*PI/180-2*atan(0.5))};
	GLdouble rearVerCut[4] = {-cos(30*PI/180),sin(30*PI/180), 0, -9.5*cos(30*PI/180)};
	GLdouble rearHorCut[4] = {-cos(90*PI/180-2*atan(0.5)), sin(90*PI/180-2*atan(0.5)),0, -9.5*cos(90*PI/180-2*atan(0.5))};
	GLdouble horCut[4] = {0,1,0,0};

	glEnable(GL_TEXTURE_2D);
	glPushMatrix();
		glTranslatef(0, -1, 4);//shift to the right
		float dt = -9 + jump;	//draw top, rear to front
		while(dt <= 9)
		{
			glPushMatrix();
				glTranslatef(dt,1,0);		

glColor3f(1.0, 1.0, 1.0);
				glScalef(0.4,0.1,0.75);	//each caterpilar is: 0.8 x 0.2 x 1.5
				drawCube_thai(TRACK_TEXTURE_thai);
			glPopMatrix();
			dt = dt + 1;
		}
		float db = 8 - jump;	//draw bottom, front to rear
		while(db >= -8.5)		
		{
			glPushMatrix();
				glTranslatef(db,-2,0);		

glColor3f(1.0, 1.0, 1.0);
				glScalef(0.4,0.1,0.75);
				drawCube_thai(TRACK_TEXTURE_thai);
			glPopMatrix();
			db = db - 1;
		}
		//draw front curve
		float da = 90 - jump*180/PI;	//start drawing at the top of the circle, clockwise, jump is used as radian, 1 rad is the angle which its arc has the same length as the radius
		glPushMatrix();
			glClipPlane (GL_CLIP_PLANE0, frontVerCut);	//cut off everything > -240
			glEnable (GL_CLIP_PLANE0);
				glClipPlane (GL_CLIP_PLANE1, frontHorCut);//cut off everything < 0
				glEnable (GL_CLIP_PLANE1);
					while(da >= -270)
					{//draw each caterpilar and tap it on the circle
						glPushMatrix();
							glTranslatef(9.5,0,0);	//move the whole circle of caterpilars to the front of the track
							glRotatef(da,0,0,1);	//move away from previous caterpilar
							glTranslatef(1,0,0);	//tap on the circle with radius =1
							glRotatef(90,0,0,1);	//vertical
							glScalef(0.4,0.1,0.75);
							drawCube_thai(TRACK_TEXTURE_thai);
						glPopMatrix();
						da = da - 45;
					}
				glDisable (GL_CLIP_PLANE1);
			glDisable (GL_CLIP_PLANE0);
		glPopMatrix();	
		//draw rear curve
		da = 90 - jump*180/PI;	//start drawing at the top of the circle, clockwise
		glPushMatrix();
			glClipPlane (GL_CLIP_PLANE0, rearVerCut);	//cut off everything > -240
			glEnable (GL_CLIP_PLANE0);
				glClipPlane (GL_CLIP_PLANE1, rearHorCut);//cut off everything < 0
				glEnable (GL_CLIP_PLANE1);
					while(da >= -270)
					{//draw each caterpilar and tap it on the circle
						glPushMatrix();
							glTranslatef(-9.5,0,0);	//move the whole circle of caterpilars to the back of the track
							glRotatef(da,0,0,1);	//move away from previous caterpilar
							glTranslatef(1,0,0);	//tap on the circle with radius =1
							glRotatef(90,0,0,1);	//vertical
							glScalef(0.4,0.1,0.75);
							drawCube_thai(TRACK_TEXTURE_thai);
						glPopMatrix();
						da = da - 45;
					}
				glDisable (GL_CLIP_PLANE1);
			glDisable (GL_CLIP_PLANE0);
		glPopMatrix();

		//draw front
		float df = 0.5 + jump;
		while(df <= 1.5)
		{
			glPushMatrix();
				glTranslatef(8.5,-2,0);
				glRotatef((90-180*2*atan(0.5)/PI),0, 0 , 1);
				glTranslatef(df, 0, 0);
				glScalef(0.4,0.1,0.75);
				drawCube_thai(TRACK_TEXTURE_thai);
			glPopMatrix();
			df = df + 1;
		}
		//draw rear
		df = -1.5 + jump;
		while(df <= -0.5)
		{
			glPushMatrix();
				glTranslatef(-8.5,-2,0);
				glRotatef(-(90-180*2*atan(0.5)/PI),0, 0 , 1);
				glTranslatef(df, 0, 0);
				glScalef(0.4,0.1,0.75);
				drawCube_thai(TRACK_TEXTURE_thai);
			glPopMatrix();
			df = df + 1;
		}
glColor3f(0,0,0);
		glPushMatrix();//top rear wheel
			glTranslatef(-9.5, 0, -0.75);
			drawCylinder_thai(1, 1, 1.5);
			drawDisk_thai(0.4, 1, 20, 20, 0, 360);
			glPushMatrix();
				glTranslatef(0, 0, 1.5);
				drawDisk_thai(0.4, 1, 20, 20, 0, 360);
			glPopMatrix();
		glPopMatrix();
		glPushMatrix();//top front wheel
			glTranslatef(9.5, 0, -0.75);
			drawCylinder_thai(1, 1, 1.5);
			drawDisk_thai(0.4, 1, 20, 20, 0, 360);
			glPushMatrix();
				glTranslatef(0, 0, 1.5);
				drawDisk_thai(0.4, 1, 20, 20, 0, 360);
			glPopMatrix();
		glPopMatrix();
	glPopMatrix();
	glDisable(GL_TEXTURE_2D);
}
/*draw right track*********************************************************************/
void drawRightTrack_thai(void)
{
	GLdouble frontVerCut[4] = {cos(30*PI/180), sin(30*PI/180), 0, -9.5*cos(30*PI/180)};
	GLdouble frontHorCut[4] = {cos(90*PI/180-2*atan(0.5)), sin(90*PI/180-2*atan(0.5)),0, -9.5*cos(90*PI/180-2*atan(0.5))};
	GLdouble rearVerCut[4] = {-cos(30*PI/180),sin(30*PI/180), 0, -9.5*cos(30*PI/180)};
	GLdouble rearHorCut[4] = {-cos(90*PI/180-2*atan(0.5)), sin(90*PI/180-2*atan(0.5)),0, -9.5*cos(90*PI/180-2*atan(0.5))};
	GLdouble horCut[4] = {0,1,0,0};

	glEnable(GL_TEXTURE_2D);
	glPushMatrix();
		glTranslatef(0, -1, -4);//shift to the right
		float dt = -9 + jump;	//draw top, rear to front
		while(dt <= 9)
		{
			glPushMatrix();
				glTranslatef(dt,1,0);		

glColor3f(1.0, 1.0, 1.0);
				glScalef(0.4,0.1,0.75);	//each caterpilar is: 0.8 x 0.2 x 1.5
				drawCube_thai(TRACK_TEXTURE_thai);
			glPopMatrix();
			dt = dt + 1;
		}
		float db = 8 - jump;	//draw bottom, front to rear
		while(db >= -8.5)		
		{
			glPushMatrix();
				glTranslatef(db,-2,0);		

glColor3f(1.0, 1.0, 1.0);
				glScalef(0.4,0.1,0.75);
				drawCube_thai(TRACK_TEXTURE_thai);
			glPopMatrix();
			db = db - 1;
		}
		//draw front curve
		float da = 90 - jump*180/PI;	//start drawing at the top of the circle, clockwise, jump is used as radian, 1 rad is the angle which its arc has the same length as the radius
		glPushMatrix();
			glClipPlane (GL_CLIP_PLANE0, frontVerCut);	//cut off everything > -240
			glEnable (GL_CLIP_PLANE0);
				glClipPlane (GL_CLIP_PLANE1, frontHorCut);//cut off everything < 0
				glEnable (GL_CLIP_PLANE1);
					while(da >= -270)
					{//draw each caterpilar and tap it on the circle
						glPushMatrix();
							glTranslatef(9.5,0,0);	//move the whole circle of caterpilars to the front of the track
							glRotatef(da,0,0,1);	//move away from previous caterpilar
							glTranslatef(1,0,0);	//tap on the circle with radius =1
							glRotatef(90,0,0,1);	//vertical
							glScalef(0.4,0.1,0.75);
							drawCube_thai(TRACK_TEXTURE_thai);
						glPopMatrix();
						da = da - 45;
					}
				glDisable (GL_CLIP_PLANE1);
			glDisable (GL_CLIP_PLANE0);
		glPopMatrix();	
		//draw rear curve
		da = 90 - jump*180/PI;	//start drawing at the top of the circle, clockwise
		glPushMatrix();
			glClipPlane (GL_CLIP_PLANE0, rearVerCut);	//cut off everything > -240
			glEnable (GL_CLIP_PLANE0);
				glClipPlane (GL_CLIP_PLANE1, rearHorCut);//cut off everything < 0
				glEnable (GL_CLIP_PLANE1);
					while(da >= -270)
					{//draw each caterpilar and tap it on the circle
						glPushMatrix();
							glTranslatef(-9.5,0,0);	//move the whole circle of caterpilars to the back of the track
							glRotatef(da,0,0,1);	//move away from previous caterpilar
							glTranslatef(1,0,0);	//tap on the circle with radius =1
							glRotatef(90,0,0,1);	//vertical
							glScalef(0.4,0.1,0.75);
							drawCube_thai(TRACK_TEXTURE_thai);
						glPopMatrix();
						da = da - 45;
					}
				glDisable (GL_CLIP_PLANE1);
			glDisable (GL_CLIP_PLANE0);
		glPopMatrix();

		//draw front
		float df = 0.5 + jump;
		while(df <= 1.5)
		{
			glPushMatrix();
				glTranslatef(8.5,-2,0);
				glRotatef((90-180*2*atan(0.5)/PI),0, 0 , 1);
				glTranslatef(df, 0, 0);
				glScalef(0.4,0.1,0.75);
				drawCube_thai(TRACK_TEXTURE_thai);
			glPopMatrix();
			df = df + 1;
		}
		//draw rear
		df = -1.5 + jump;
		while(df <= -0.5)
		{
			glPushMatrix();
				glTranslatef(-8.5,-2,0);
				glRotatef(-(90-180*2*atan(0.5)/PI),0, 0 , 1);
				glTranslatef(df, 0, 0);
				glScalef(0.4,0.1,0.75);
				drawCube_thai(TRACK_TEXTURE_thai);
			glPopMatrix();
			df = df + 1;
		}
		glColor3f(0,0,0);
		glPushMatrix();//top rear wheel
			glTranslatef(-9.5, 0, -0.75);
			drawCylinder_thai(1, 1, 1.5);
			drawDisk_thai(0.4, 1, 20, 20, 0, 360);
			glPushMatrix();
				glTranslatef(0, 0, 1.5);
				drawDisk_thai(0.4, 1, 20, 20, 0, 360);
			glPopMatrix();
		glPopMatrix();
		glPushMatrix();//top front wheel
			glTranslatef(9.5, 0, -0.75);
			drawCylinder_thai(1, 1, 1.5);
			drawDisk_thai(0.4, 1, 20, 20, 0, 360);
			glPushMatrix();
				glTranslatef(0, 0, 1.5);
				drawDisk_thai(0.4, 1, 20, 20, 0, 360);
			glPopMatrix();
		glPopMatrix();
	glPopMatrix();
	glDisable(GL_TEXTURE_2D);
}
/*DRAW CORE OF 2 TOP WHEELS************************************************************/
void drawTopWheelsCore_thai(void)
{
glColor3f(1.0, 0.5, 0.3);
	glPushMatrix();	//core of rear wheel
		glTranslatef(-9.5, -1, -4.75);
		drawCylinder_thai(0.4, 0.4, 9.5);
	glPopMatrix();

	glPushMatrix();	//core of front wheel
		glTranslatef(9.5, -1, -4.75);
		drawCylinder_thai(0.4, 0.4, 9.5);
	glPopMatrix();
}
/*DRAW WHEELS**************************************************************************/
void drawMiddleWheels_thai(void)
{
	glPushMatrix();//4 wheels on each side in the middle
glColor3f(1.0, 0.5, 0.3);
		glPushMatrix();	//core
			glTranslatef(5.1, -2, -5);
			drawCylinder_thai(0.4, 0.4, 9.5);
		glPopMatrix();
glColor3f(0,0,0);
		glPushMatrix();	//bottom left wheel, first from front
			glTranslatef(5.1, -2, 3.25);
			drawCylinder_thai(1, 1, 1.5);
			drawDisk_thai(0.4, 1, 20, 5, 0, 360);
			glTranslatef(0, 0, 0.75);
			drawDisk_thai(0.4, 1, 20, 5, 0, 360);
		glPopMatrix();
		glPushMatrix();	//bottom right wheel, first from front
			glTranslatef(5.1, -2, -4.75);
			drawCylinder_thai(1, 1, 1.5);
			drawDisk_thai(0.4, 1, 20, 5, 0, 360);
			glTranslatef(0, 0, 0.75);
			drawDisk_thai(0.4, 1, 20, 5, 0, 360);
		glPopMatrix();
glColor3f(1.0, 0.5, 0.3);
		glPushMatrix();	//core
			glTranslatef(1.7, -2, -5);
			drawCylinder_thai(0.4, 0.4, 9.5);
		glPopMatrix();
glColor3f(0,0,0);
		glPushMatrix();	//bottom left wheel, second from front
			glTranslatef(1.7, -2, 3.25);
			drawCylinder_thai(1, 1, 1.5);
			drawDisk_thai(0.4, 1, 20, 5, 0, 360);
			glTranslatef(0, 0, 0.75);
			drawDisk_thai(0.4, 1, 20, 5, 0, 360);
		glPopMatrix();
		glPushMatrix();	//bottom right wheel, second from front
			glTranslatef(1.7, -2, -4.75);
			drawCylinder_thai(1, 1, 1.5);
			drawDisk_thai(0.4, 1, 20, 5, 0, 360);
			glTranslatef(0, 0, 0.75);
			drawDisk_thai(0.4, 1, 20, 5, 0, 360);
		glPopMatrix();
glColor3f(1.0, 0.5, 0.3);
		glPushMatrix(); //core
			glTranslatef(-1.7, -2, -5);
			drawCylinder_thai(0.4, 0.4, 9.5);
		glPopMatrix();
glColor3f(0,0,0);
		glPushMatrix();	//bottom left wheel, third from front
			glTranslatef(-1.7, -2, 3.25);
			drawCylinder_thai(1, 1, 1.5);
			drawDisk_thai(0.4, 1, 20, 5, 0, 360);
			glTranslatef(0, 0, 0.75);
			drawDisk_thai(0.4, 1, 20, 5, 0, 360);
		glPopMatrix();
		glPushMatrix();	//bottom right wheel, third from front
			glTranslatef(-1.7, -2, -4.75);
			drawCylinder_thai(1, 1, 1.5);
			drawDisk_thai(0.4, 1, 20, 5, 0, 360);
			glTranslatef(0, 0, 0.75);
			drawDisk_thai(0.4, 1, 20, 5, 0, 360);
		glPopMatrix();
glColor3f(1.0, 0.5, 0.3);
		glPushMatrix();	//core
			glTranslatef(-5.1, -2, -5);
			drawCylinder_thai(0.4, 0.4, 9.5);
		glPopMatrix();
glColor3f(0,0,0);
		glPushMatrix();	//bottom left wheel, fourth from front
			glTranslatef(-5.1, -2, 3.25);
			drawCylinder_thai(1, 1, 1.5);
			drawDisk_thai(0.4, 1, 20, 5, 0, 360);
			glTranslatef(0, 0, 0.75);
			drawDisk_thai(0.4, 1, 20, 5, 0, 360);
		glPopMatrix();
		glPushMatrix();	//bottom right wheel, fourth from front
			glTranslatef(-5.1, -2, -4.75);
			drawCylinder_thai(1, 1, 1.5);
			drawDisk_thai(0.4, 1, 20, 5, 0, 360);
			glTranslatef(0, 0, 0.75);
			drawDisk_thai(0.4, 1, 20, 5, 0, 360);
		glPopMatrix();
	glPopMatrix();//end 4 wheels on each sides in the middle
}
/*DRAW MOVING MECHANISM****************************************************************/
void drawMovingMechanism_thai(void)
{
glColor3f(0.0,0.0,0.0);//ancient chinese ink :P
	
	glPushMatrix();
		drawLeftTrack_thai();
		drawRightTrack_thai();
		drawTopWheelsCore_thai();
		drawMiddleWheels_thai();
	glPopMatrix();
}
/*DRAW COMPLETE BODY*******************************************************************/
void drawCompleteBody_thai(void)
{
	glPushMatrix();//everything not the turet
		drawMainBody_thai();
		drawExhauster_thai();
		drawMovingMechanism_thai();	//tracks, wheels
	glPopMatrix();
}
/**************************************************************************************/
void drawTank_thai(void)
{
glColor3f(1.0,1.0,1.0);
	if(isInside(bottomSet, 0, tankSet, 0) && !isInside(topSet, 0, tankSet, 1)) //tank is inside the bottomSphere but outside the topSphere
	{
		accumMove = true;	//tell keyboard function to accumulate the distance the tank move within the hill
		if(goDown)
		{
			if(beta > -hillAngle)	//beta is the angle that the tank has to rotate to match the angle of the hill
			{	
				beta = calTankAngle(sumMove,-hillAngle,17);//17 is the length of the tank body

			}
			else
			{	
				tank_y = fabs((bottomSet[0][3]-topSet[0][3])*tan(hillAngle) - sumMove*sin(hillAngle));
			}
			glTranslatef(tank_x, tank_y+4, tank_z);
			glRotatef(turn, 0, 1, 0);
			glTranslatef(-8.5,0,0);				//move back to normal position
			glRotatef(beta*180/PI, 0, 0, 1);	//rotate about the end of the tank
			glTranslatef(8.5,0,0);
		}
		else
		{
			if(beta<hillAngle)	//beta is the angle that the tank has to rotate to match the angle of the hill
			{	
				beta = calTankAngle(sumMove,hillAngle,17);//17 is the length of the tank body

			}
			else
			{	
				tank_y = sumMove*sin(hillAngle)-17*sin(hillAngle);
			}
			glTranslatef(tank_x, tank_y, tank_z);
			glRotatef(turn, 0, 1, 0);
			glTranslatef(-8.5,0,0);				//move back to normal position
			glRotatef(beta*180/PI, 0, 0, 1);	//rotate about the end of the tank
			glTranslatef(8.5,0,0);				//end of the tank is now at origin
		}
	}
	else if(isInside(bottomSet, 0, tankSet, 0) && isInside(topSet, 0, tankSet, 1)) //when the tank is inside the bottomSphere and the topSphere
	{
		goDown = true;
		accumMove = false;
		beta = 0;
		glTranslatef(tank_x,tank_y+4,tank_z);
		glRotatef(turn, 0, 1, 0);
	}
	else
	{
		goDown=false;
		accumMove = false;
		beta = 0;
		tank_y = 0;

		glTranslatef(tank_x,tank_y,tank_z);
		glRotatef(turn, 0, 1, 0);
	}

	drawCompleteTurret_thai();	//turret at origins, all the moves has been accumulated from the matrix above
	setSpotLight1();		//set the spotlight in front the tank
	drawCompleteBody_thai();		//COMPLETE BODY AT ORIGIN

	if(Spheres){
	glutWireSphere(tankSet[0][3], 20, 20);	//black
	glColor3f(1,1,0);
	glutWireSphere(tankSet[1][3], 20, 20);	//yellow
	}

}
