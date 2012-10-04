/********************************************************************************/

#include <stdlib.h>
#include<gl\glut.h>

#include "Terrain_Arch.h"

#include <stdlib.h>
#include <time.h>
#include <sys/types.h>

#include <GL/glut.h>        
#include <stdio.h>        
#include <math.h>

#define NUM_SMOKE 40
#define SIZE_SPARK 10
#define SIZE_SMOKE 4

	//for spark	
	struct Spark{
	   float _PositionX;
	   float _PositionY;
	   float _PositionZ;
	   GLfloat   _Alpha;
	   GLfloat   _Scale;

	   bool reach_max_alpha;
	   bool reach_min_alpha;
};

	Spark SparkObject;
	bool do_explosion;	

	struct Smoke{
	   float _PositionX;
	   float _PositionY;
	   float _PositionZ;
	   GLfloat   _Alpha;
	   GLfloat   _Scale;

	   bool reach_max_alpha;
	   bool reach_min_alpha;
};

	Smoke SmokeArray[NUM_SMOKE];
	bool do_smoke = false;

	int sparkTime = 0;

	#define NUM_PARTICLES    1000          /* Number of particles  */
	#define NUM_DEBRIS       70            /* Number of debris     */

	/* A particle */

	struct particleData
	{
	float   position[3];
	float   speed[3];
	float   color[3];
	};
	typedef struct particleData    particleData;

	/* A piece of debris */

	struct debrisData
	{
	float   position[3];
	float   speed[3];
	float   orientation[3];        /* Rotation angles around x, y, and z axes */
	float   orientationSpeed[3];
	float   color[3];
	float   scale[3];
	};
	typedef struct debrisData    debrisData;

	particleData  particles[NUM_PARTICLES];
	debrisData       debris[NUM_DEBRIS];

	int     fuel = 0;
	int      wantNormalize = 0;   /* Speed vector normalization flag */

/****************************************************************************************/
/* prototypes   																		*/
/****************************************************************************************/
void initSmoke(float x, float y, float z);
void initSpark(float x, float y, float z);
void drawOneSmoke(int num, float initX, float initY, float initZ);
void drawSquadSmoke(void);
void explosionChange(void);
void newSpeed (float dest[3]);
void particleExplosion(void);
void drawSquadSpark(void);
void newSpark (void);

/****************************************************************************************/
/* init Smooke   																		*/
/****************************************************************************************/
void initSmoke(float x, float y, float z){
	for(int i=0; i<NUM_SMOKE; i++){		

//		smokeElement[i] = false;

		SmokeArray[i]._PositionX = x;//i*10;
		SmokeArray[i]._PositionY = y;
		SmokeArray[i]._PositionZ = z;//i*10;
		SmokeArray[i]._Alpha = 0;
		SmokeArray[i]._Scale=1;

		SmokeArray[i].reach_max_alpha = false;
		SmokeArray[i].reach_min_alpha = false;
	}
}

/****************************************************************************************/
/* init Spark    																		*/
/****************************************************************************************/
void initSpark(float x, float y, float z){
	//for(int i=0; i<20; i++){		

		//spark = false;

		SparkObject._PositionX = x;//10
		SparkObject._PositionY = y;//3
		SparkObject._PositionZ = z;//10
		SparkObject._Alpha = 0;
		SparkObject._Scale=1;

		SparkObject.reach_max_alpha = false;
		SparkObject.reach_min_alpha = false;
//	}
}


/****************************************************************************************/
/* for spark																		*/
/****************************************************************************************/
void drawOneSmoke(int num, float initX, float initY, float initZ){
	if (SmokeArray[num]._Alpha > 1.2)
		SmokeArray[num].reach_max_alpha = true;
	else if(SmokeArray[num]._Alpha < 0)
		SmokeArray[num].reach_min_alpha = true;

	if (SmokeArray[num].reach_max_alpha){
		SmokeArray[num]._Alpha -= 0.02f;
		SmokeArray[num]._Scale -= 0.03f;//0.03			// Update Scale
	}
    else{
		SmokeArray[num]._Alpha += 0.02f;			// Update Alpha
		SmokeArray[num]._Scale += 0.03f;//0.03			// Update Scale
	}
			
	// Assign Vertices Colour Yellow With Alpha
	// Colour Tracks Ambient And Diffuse
	if(!SmokeArray[num].reach_min_alpha){
		glColor4f(0.5,0.5,0.5, SmokeArray[num]._Alpha);		// Scale
		
		SmokeArray[num]._PositionX += 0.05;//0.04;
		SmokeArray[num]._PositionY += 0.02;//0.15;
		SmokeArray[num]._PositionZ += 0.05;//0.05;

		glPushMatrix();
		glTranslatef((float)SmokeArray[num]._PositionX,
					(float)SmokeArray[num]._PositionY,
					(float)SmokeArray[num]._PositionZ);
		glScalef(SmokeArray[num]._Scale, SmokeArray[num]._Scale,SmokeArray[num]._Scale);

		drawSquadSmoke();
		glPopMatrix();
	}
	else{
		SmokeArray[num]._PositionX = initX;//i*10;
		SmokeArray[num]._PositionY = initY+2;
		SmokeArray[num]._PositionZ = initZ;//i*10;
		SmokeArray[num]._Alpha = 0;
		SmokeArray[num]._Scale=1;

		SmokeArray[num].reach_max_alpha = false;
		SmokeArray[num].reach_min_alpha = false;
	}
	
}
/****************************************************************************************/
/* for spark																		*/
/****************************************************************************************/
void drawSquadSmoke(void){

	glBegin(GL_POLYGON);			
			glTexCoord2f(1.0f, 1.0f); glVertex3f(SIZE_SMOKE, -SIZE_SMOKE/2, 0);
			glTexCoord2f(1.0f, 0.0f); glVertex3f(SIZE_SMOKE, SIZE_SMOKE/2, 0);
			glTexCoord2f(0.0f, 0.0f); glVertex3f(-SIZE_SMOKE, SIZE_SMOKE/2, 0);
			glTexCoord2f(0.0f, 1.0f); glVertex3f(-SIZE_SMOKE, -SIZE_SMOKE/2, 0);
	glEnd();

	glPushMatrix();	
	glRotatef(45, 0.0, 1.0, 0.0);
	glBegin(GL_POLYGON);			
			glTexCoord2f(1.0f, 1.0f); glVertex3f(SIZE_SMOKE, -SIZE_SMOKE/2, 0);
			glTexCoord2f(1.0f, 0.0f); glVertex3f(SIZE_SMOKE, SIZE_SMOKE/2, 0);
			glTexCoord2f(0.0f, 0.0f); glVertex3f(-SIZE_SMOKE, SIZE_SMOKE/2, 0);
			glTexCoord2f(0.0f, 1.0f); glVertex3f(-SIZE_SMOKE, -SIZE_SMOKE/2, 0);
	glEnd();
	glPopMatrix();
}

/****************************************************************************************/
/* for spark																		*/
/****************************************************************************************/
void explosionChange(void){
	if (fuel > 0)
	{
	  for (int i = 0; i < NUM_PARTICLES; i++)
	    {
	      particles[i].position[0] += particles[i].speed[0] * 0.2;
	      particles[i].position[1] += particles[i].speed[1] * 0.2;
	      particles[i].position[2] += particles[i].speed[2] * 0.2;
	      
	      particles[i].color[0] -= 1.0 / 500.0;
	      if (particles[i].color[0] < 0.0)
		  {
		  particles[i].color[0] = 0.0;
		  }
	      
	      particles[i].color[1] -= 1.0 / 100.0;
	      if (particles[i].color[1] < 0.0)
		  {
		  particles[i].color[1] = 0.0;
		  }
	      
	      particles[i].color[2] -= 1.0 / 50.0;
	      if (particles[i].color[2] < 0.0)
		  {
		  particles[i].color[2] = 0.0;
		  }
	    }
	  
	  for (i = 0; i < NUM_DEBRIS; i++)
	    {
	      debris[i].position[0] += debris[i].speed[0] * 0.1;
	      debris[i].position[1] += debris[i].speed[1] * 0.1;
	      debris[i].position[2] += debris[i].speed[2] * 0.1;
	      
	      debris[i].orientation[0] += debris[i].orientationSpeed[0] * 10;
	      debris[i].orientation[1] += debris[i].orientationSpeed[1] * 10;
	      debris[i].orientation[2] += debris[i].orientationSpeed[2] * 10;
	    }
	  
	  --fuel;
	}
}

/****************************************************************************************/
/* for spark																		*/
/****************************************************************************************/
void newSpeed (float dest[3])
{
  float    x;
  float    y;
  float    z;
  float    len;

  x = (2.0 * ((GLfloat) rand ()) / ((GLfloat) RAND_MAX)) - 1.0;
  y = (2.0 * ((GLfloat) rand ()) / ((GLfloat) RAND_MAX)) - 1.0;
  z = (2.0 * ((GLfloat) rand ()) / ((GLfloat) RAND_MAX)) - 1.0;

  /*
   * Normalizing the speed vectors gives a "fireball" effect
   *
   */

  if (wantNormalize)
    {
      len = sqrt (x * x + y * y + z * z);

      if (len)
	{
	  x = x / len;
	  y = y / len;
	  z = z / len;
	}
    }

  dest[0] = x;
  dest[1] = y;
  dest[2] = z;
}

/****************************************************************************************/
/* for spark																		*/
/****************************************************************************************/
void particleExplosion(void){


	glPushMatrix ();

      glDisable (GL_LIGHTING);
      glDisable (GL_DEPTH_TEST);

      glBegin (GL_POINTS);

      for (int i = 0; i < NUM_PARTICLES; i++)
	{
	  glColor3fv (particles[i].color);
	  glVertex3fv (particles[i].position);
	}
  
      glEnd ();

      glPopMatrix ();

      glEnable (GL_LIGHTING); 
      glEnable (GL_DEPTH_TEST);

      glNormal3f (0.0, 0.0, 1.0);

      for (i = 0; i < NUM_DEBRIS; i++)
	{
	  glColor3fv (debris[i].color);

	  glPushMatrix ();
      	  
	  glTranslatef (debris[i].position[0],
			debris[i].position[1],
			debris[i].position[2]);

	  glRotatef (debris[i].orientation[0], 1.0, 0.0, 0.0);
	  glRotatef (debris[i].orientation[1], 0.0, 1.0, 0.0);
	  glRotatef (debris[i].orientation[2], 0.0, 0.0, 1.0);

	  glScalef (debris[i].scale[0],
		    debris[i].scale[1],
		    debris[i].scale[2]);

	  glBegin (GL_TRIANGLES);
	  glVertex3f (0.0, 1.0, 0.0);
	  glVertex3f (-0.5, 0.0, 0.0);
	  glVertex3f (0.5, 0.0, 0.0);
	  glEnd ();
	  
	  glPopMatrix ();
	}
//} 
}

/****************************************************************************************/
/* for spark																		*/
/****************************************************************************************/
void drawSquadSpark(void){

	glBegin(GL_POLYGON);			
			glTexCoord2f(1.0f, 1.0f); glVertex3f(SIZE_SPARK, -SIZE_SPARK/2, 0);
			glTexCoord2f(1.0f, 0.0f); glVertex3f(SIZE_SPARK, SIZE_SPARK/2, 0);
			glTexCoord2f(0.0f, 0.0f); glVertex3f(-SIZE_SPARK, SIZE_SPARK/2, 0);
			glTexCoord2f(0.0f, 1.0f); glVertex3f(-SIZE_SPARK, -SIZE_SPARK/2, 0);
	glEnd();

	glBegin(GL_POLYGON);			
			glTexCoord2f(1.0f, 1.0f); glVertex3f(0, SIZE_SPARK/2, -SIZE_SPARK/2);
			glTexCoord2f(1.0f, 0.0f); glVertex3f(0, SIZE_SPARK/2, SIZE_SPARK/2);
			glTexCoord2f(0.0f, 0.0f); glVertex3f(0, -SIZE_SPARK/2, SIZE_SPARK/2);
			glTexCoord2f(0.0f, 1.0f); glVertex3f(0, -SIZE_SPARK/2, -SIZE_SPARK/2);
	glEnd();
}


/****************************************************************************************/
/* for spark																		*/
/****************************************************************************************/

void newSpark (void)
{
  int    i;

  for (i = 0; i < NUM_PARTICLES; i++)
    {
      particles[i].position[0] = SparkObject._PositionX;		
      particles[i].position[1] = SparkObject._PositionY;
      particles[i].position[2] = SparkObject._PositionZ;

      particles[i].color[0] = 1.0;
      particles[i].color[1] = 1.0;
      particles[i].color[2] = 0.5;

      newSpeed (particles[i].speed);
    }

  for (i = 0; i < NUM_DEBRIS; i++)
    {
      debris[i].position[0] = SparkObject._PositionX;
      debris[i].position[1] = SparkObject._PositionY;
      debris[i].position[2] = SparkObject._PositionZ;

      debris[i].orientation[0] = 0.0;
      debris[i].orientation[1] = 0.0;
      debris[i].orientation[2] = 0.0;

      debris[i].color[0] = 0.7;
      debris[i].color[1] = 0.7;
      debris[i].color[2] = 0.7;

      debris[i].scale[0] = (2.0 * 
			    ((GLfloat) rand ()) / ((GLfloat) RAND_MAX)) - 1.0;
      debris[i].scale[1] = (2.0 * 
			    ((GLfloat) rand ()) / ((GLfloat) RAND_MAX)) - 1.0;
      debris[i].scale[2] = (2.0 * 
			    ((GLfloat) rand ()) / ((GLfloat) RAND_MAX)) - 1.0;

      newSpeed (debris[i].speed);
      newSpeed (debris[i].orientationSpeed);
    }
  
  fuel = 100;
}

