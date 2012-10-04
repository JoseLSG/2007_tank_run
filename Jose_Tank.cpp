
#include <GL/glut.h>
#include <stdio.h>
#include <math.h>

#include "globalVar.h"

float turretAngle = 0.0; // angle of turret rotation
float barrelAngle = 0.0; // angle of barrel rotation
float Cam_View_Turret = 0.0; // angle of camera rotation with turret

float CubeX = 5.0, CubeY = 2.5, CubeZ = 10.0; // Boddy cube base dimensions 
float useX = CubeX/2, useY = CubeY/2.0, useZ = CubeZ/2.0; // Corners of the base cube
float useXadd = 1.5;

float TurretX = CubeX + CubeX/5.0 , TurretY = CubeY*2.0/3.0, TurretZ = CubeZ + CubeZ/8.0 ; 
float useTX = TurretX/2, useTY = TurretY/2.0, useTZ = TurretZ/2.0; // Corners of the turret cube

float bRadius = 0.2; // Barrel radius
float bInnnerRadius = 0.02 ; // Barrel inner radius

float wRadius = 0.70; // Wheel radius
float wInnerRadius = 0.1; // Wheel inner radius

bool enableCutPlane_Jose = false;
float cuttingPos_Jose = 3.5;
float gun_fall_angle_Jose = 0;


void drawWheel(void)
{
	glPushMatrix();
		glColor3f(1.0, 0.0, 0.0);
		glTranslatef(0.4,0.0,0.0);
		glScalef(0.0, 1.0, 1.0);
		glutSolidSphere(wRadius, 10.0, 10.0);
	glPopMatrix();

	glPushMatrix();
		glColor3f(1.0, 0.0, 0.0);
		glTranslatef(-0.4,0.0,0.0);
		glScalef(0.0, 1.0, 1.0);
		glutSolidSphere(wRadius, 10.0, 10.0);
	glPopMatrix();

	glPushMatrix();
		glColor3f(0.0, 1.0, 1.0);
		glScalef(5.0, 1.0, 1.0);
		glRotatef(90.0, 0.0, 1.0, 0.0);
		glutSolidTorus(wInnerRadius,wRadius,10.0,10.0);
	glPopMatrix();
}


void Wheel_System(void)
{
	drawWheel();
	float z = wRadius * 2.0 + 0.3; 
	for(int i = 2; i<=5 ; i++)
		{
			if(i<5)
			{
				glPushMatrix();
					glTranslatef(0.0, 0.0, z);
					drawWheel();
				glPopMatrix();
				z = wRadius * 2.0 + 0.3;
				z *= i;
			}
			else
			{
				glPushMatrix();
					glTranslatef(0.0, 0.8, z+0.5);
					drawWheel();
				glPopMatrix();
			}
		}

	z = -(wRadius * 2.0 + 0.3); 
	for(int j = 2; j<=5 ; j++)
		{
			if(j<5)
			{
				glPushMatrix();
					glTranslatef(0.0, 0.0, z);
					drawWheel();
				glPopMatrix();
				z = -(wRadius * 2.0 + 0.2); 
				z *= j;
			}
			else
			{
				glPushMatrix();
					glTranslatef(0.0, 0.8, z ); // back sprocket
					drawWheel();
				glPopMatrix();
			}
		}
	

}

void drawTrackSystem(void)
{
	Wheel_System();
}

void drawBarrel(void)
{
	glPushMatrix();
		glColor3f(0.0, 1.0, 1.0); 
		glScalef(1.0, 1.0, 250.0);
		glutSolidTorus(bInnnerRadius,bRadius,50.0,50.0);
	glPopMatrix();
}

void drawMachineGun(void)
{
	glPushMatrix();
		glColor3f(0.0, 0.0, 0.8);
		glTranslatef(0.0,-0.2,-1.0);
		glScalef(4.0, 50.0, 4.0);
		glRotatef(90.0,1.0,0.0,0.0);
		glutSolidTorus(0.005,0.2,50.0,10.0);
	glPopMatrix();

	glPushMatrix();
		glColor3f(0.0, 1.0, 1.0); 
		glScalef(1.0, 1.0, 30.0);
		glutSolidTorus(0.015,0.05,50.0,10.0);
	glPopMatrix();
}

void drawTurret(void)
{

	glPushMatrix();
		glTranslatef(-useTX + 1.0,useY ,useTZ - 0.1);
		drawMachineGun();
	glPopMatrix();


	glPushMatrix();
		glColor3f(0.0, 0.0, 1.0);
		glScalef(TurretX, TurretY, TurretZ);
		glutSolidCube(1);
	glPopMatrix();

	glPushMatrix();
		glBegin(GL_QUADS);
			glVertex3f(-useTX, useTY, useTZ);
			glVertex3f(-useTX/8.0, useTY, useTZ);
			glVertex3f(-useTX/8.0, 0.0, useTZ + useTZ/2.0);
			glVertex3f(-useTX, 0.0, useTZ+ useTZ/2.0);

			glVertex3f(-useTX, -useTY, useTZ);
			glVertex3f(-useTX/8.0, -useTY, useTZ);
			glVertex3f(-useTX/8.0, 0.0, useTZ + useTZ/2.0);
			glVertex3f(-useTX, 0.0, useTZ+ useTZ/2.0);

			glVertex3f(useTX, useTY, useTZ);
			glVertex3f(useTX/8.0, useTY, useTZ);
			glVertex3f(useTX/8.0, 0.0, useTZ + useTZ/2.0);
			glVertex3f(useTX, 0.0, useTZ+ useTZ/2.0);

			glVertex3f(useTX, -useTY, useTZ);
			glVertex3f(useTX/8.0, -useTY, useTZ);
			glVertex3f(useTX/8.0, 0.0, useTZ + useTZ/2.0);
			glVertex3f(useTX, 0.0, useTZ+ useTZ/2.0);
		glEnd();
	glPopMatrix();


	glPushMatrix();
		glTranslatef(0.0, 0.0, TurretZ - TurretZ/2.0);
		glRotatef(barrelAngle, 1.0, 0.0, 0.0);
		glTranslatef(0.0,0.0, TurretZ/2.0);
		drawBarrel();
	glPopMatrix();

}
void drawBody(void)
{
	glPushMatrix();
		glScalef(CubeX, CubeY, CubeZ); // Dimensions of base cube
		glutSolidCube(1); // Base cube 
	glPopMatrix();

	glPushMatrix();
		
		glBegin(GL_QUADS);
			glVertex3f( -useX-useXadd, useY, useZ ); //tops side nose
			glVertex3f(  useX+useXadd, useY, useZ );
			glVertex3f(  useX+useXadd, CubeY/6.0, CubeZ/3.0 +useZ );
			glVertex3f( -useX-useXadd, CubeY/6.0, CubeZ/3.0 +useZ );

			glVertex3f( -useX-useXadd, CubeY/6.0, CubeZ/3.0 +useZ ); // Top tip of nose
			glVertex3f(  useX+useXadd, CubeY/6.0, CubeZ/3.0 +useZ ); 
			glVertex3f(  useX+useXadd, 0.0, CubeZ/3.0 + useZ + 0.5 );
			glVertex3f( -useX-useXadd, 0.0, CubeZ/3.0 + useZ + 0.5 );
			
			glVertex3f( -useX , -useY, useZ ); // Bottom tip of nose
			glVertex3f(  useX , -useY, useZ );
			glVertex3f(  useX , -useY, CubeZ/6.0 +useZ );
			glVertex3f( -useX , -useY, CubeZ/6.0 +useZ );

			glVertex3f( -useX , -useY, CubeZ/6.0 +useZ ); // Bottom of nose
			glVertex3f(  useX , -useY, CubeZ/6.0 +useZ );
			glVertex3f(  useX, 0.0, CubeZ/3.0 + useZ + 0.5 );
			glVertex3f( -useX, 0.0, CubeZ/3.0 + useZ + 0.5 );

			glVertex3f( -useX-useXadd, useY, -CubeZ/4.0 -useZ );  // Base cube top cover
			glVertex3f(  useX+useXadd, useY, -CubeZ/4.0 -useZ );
			glVertex3f(  useX+useXadd, useY, useZ );
			glVertex3f( -useX-useXadd, useY, useZ );

			glVertex3f( -useX-useXadd, useY, -CubeZ/4.0 -useZ );  // Back cover
			glVertex3f(  useX+useXadd, useY, -CubeZ/4.0 -useZ );
			glVertex3f(  useX+useXadd, 0.0 , -CubeZ/4.0 -useZ );
			glVertex3f( -useX-useXadd, 0.0 , -CubeZ/4.0 -useZ );

		glEnd();

		glBegin(GL_POLYGON); // Front nose left side
		glColor3f(1.0,1.0,0.0);
			glVertex3f( -useX , useY, useZ );
			glVertex3f( -useX , -useY, useZ );
			glVertex3f( -useX , -useY, CubeZ/6.0 +useZ );
			glVertex3f( -useX , 0.0, CubeZ/3.0 + useZ + 0.5 );
			glVertex3f( -useX , CubeY/6.0, CubeZ/3.0 +useZ );		
		glEnd();

		glBegin(GL_POLYGON); // Front nose right sides
			glVertex3f( useX , useY, useZ );
			glVertex3f( useX , -useY, useZ );
			glVertex3f( useX , -useY, CubeZ/6.0 +useZ );
			glVertex3f( useX , 0.0, CubeZ/3.0 + useZ + 0.5 );
			glVertex3f( useX , CubeY/6.0, CubeZ/3.0 +useZ );		
		glEnd();

	glPopMatrix();

}

void drawTank_Jose(void)
{	
	glPushMatrix();
		glColor3f(0.419, 0.556, 0.137);
		drawBody();
	glPopMatrix();

    if (enableCutPlane_Jose)
    {
		   GLdouble cutPlane2[4] = {0, -1, 0, cuttingPos_Jose};
		   glClipPlane (GL_CLIP_PLANE2, cutPlane2);
           glEnable(GL_CLIP_PLANE2);
				glPushMatrix();
				glColor3f(0.0, 0.0, 1.0); 
				glRotatef(turretAngle, 0.0, 1.0, 0.0);
				glTranslatef(0.0, useY + TurretY/2.0, -CubeZ/8.0);
				drawTurret();
				glPopMatrix();
			glDisable(GL_CLIP_PLANE2);
	}
	else{
		if (is_collapse_Jose){
			GLdouble cutPlane2[4] = {0, -1, 0, cuttingPos_Jose};
		   glClipPlane (GL_CLIP_PLANE2, cutPlane2);
           glEnable(GL_CLIP_PLANE2);
				glPushMatrix();
				glColor3f(0.0, 0.0, 1.0); 
				glRotatef(turretAngle, 0.0, 1.0, 0.0);
				glTranslatef(0.0, useY + TurretY/2.0, -CubeZ/8.0);
				drawTurret();
				glPopMatrix();
			glDisable(GL_CLIP_PLANE2);
		}
		else{

			glPushMatrix();
				glColor3f(0.0, 0.0, 1.0); 
				glRotatef(turretAngle, 0.0, 1.0, 0.0);
				glTranslatef(0.0, useY + TurretY/2.0, -CubeZ/8.0);
				drawTurret();
			glPopMatrix();
		}
	}

	glPushMatrix();
		glTranslatef(0.0, -useY - 0.4, 0.0); // lowers the wheels

		glPushMatrix();
			glTranslatef(-useX-useXadd/2.0, 0.0, 0.0); // move to right side of tank
			drawTrackSystem();
		glPopMatrix();

		glPushMatrix();
			glTranslatef( useX+useXadd/2.0, 0.0, 0.0);  // move to left side of tank
			drawTrackSystem();
		glPopMatrix();
	

	glPopMatrix();



}