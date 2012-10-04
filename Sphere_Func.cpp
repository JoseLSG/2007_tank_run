

#include <GL/glut.h>	
#include <stdio.h>	
#include <math.h>


#include <stdlib.h>
#include<gl\glut.h>


/**************************************************************************************
Used to generate a Sphere shape.                                                                                                                /
****************************************************************************************/
void drawSphere(float fRadius, GLint slices, GLint rings)                                                        
{
       GLUquadricObj* quatric;
       quatric =  gluNewQuadric();        // Creates a new quadrics object and returns a pointer to it.

       gluQuadricNormals(quatric, GL_SMOOTH);
       gluQuadricTexture(quatric, GL_TRUE);


       gluQuadricDrawStyle(quatric, GLU_FILL);

       gluSphere(quatric, fRadius,slices, rings);        // Draw the sphere with a radius : fRadius.
       gluDeleteQuadric(quatric);                                        // Free the Quadric

}

 