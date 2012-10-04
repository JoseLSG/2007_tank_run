
/********************************************************************************/
/*   header																		*/
/********************************************************************************/

#include <GL/glut.h>	
#include <stdio.h>	
#include <math.h>


#include <stdlib.h>
#include<gl\glut.h>

#include "Terrain_Arch.h"
#include "globalVar.h"

#include <stdlib.h>
#include <time.h>
#include <sys/types.h>

#include "Sphere_Func.h"


/********************************************************************************/
/*   Constant Variables															*/
/********************************************************************************/

	#define NONE_TEXTURE 0		// to set not texture the object
	#define APPLY_TEXTURE 1		// to set texture the object

	#define NUM_OF_TEXTURES 10	// The number of textures that are going to load in this program.

	#define SIZE 1000			// To deal with the view volume
	#define NEAR_Z 1.0			// for perspective / othor projection
	#define FAR_Z 700.0			// for perspective / othor projection

	#define FORWARD_MAX 0.5   //to limit forward camera
	#define BACKWARD_MAX 50	  //to limit backward camera

	#define PI 3.14159265


#define grass 1

#define NUM_SMOKE 40


/********************************************************************************/
/*   global variable															*/
/********************************************************************************/
	int H,W; //window size

	bool bAxis;			// To show or not to show the reference axis.

	bool bWire;			// To wireframe/not.	

	int person = 3;	// 1st or 3rd person view => initilize to the 3rd person

	float fovy = 90.0;			// For Perspective projections
	float fViewVol = 25.0;		// For Orthographic projections
	enum {PERSPECTIVE, ORTHOGRAPHIC} eProjection = PERSPECTIVE;


	float gun_Angle = 0;		// The rotation angle for the machine gun
	float turret_Angle = 0; 	// The rotation angle for the turret
	float barrel_Angle = 0;		//The rotation angle for the barrel
	float wheel_Left_Angle = 0; //The rotation angle for left wheel
	float wheel_Right_Angle = 0;//The rotation angle for right wheel
	float tank_Angle = 0;		//The rotation angle for the whole tank
	float distance = 0;			//The distance moving the tank forward/backward	


	int track = 0;	// to keep track the moving of the caterpilar


	GLfloat M[16];	        // using for Math calculation to keep the tank move/rotate properly
	GLfloat previousM[16];	// using for Math calculation to keep the tank move/rotate properly
	GLfloat temp[16];	    // using for Math calculation to keep the tank move/rotate properly
	bool first_time = true;	    // using to keep the tank move/rotate properly


	bool f1 = true;	//turn on/off light 0
	bool f2 = true;	//turn on/off light 1
	bool f3 = true; //turn on/off light 2
	bool f4 = true; //turn shininess/rusty mode

	bool shiny = false; //check the shininess/rusty mode


	//set property of shininess/rusty mode
	GLfloat no_mat[] = {0.0f, 0.0f, 0.0f, 1.0f};
	GLfloat mat_ambient[] = {0.7f, 0.7f, 0.7f, 1.0f};
	GLfloat mat_ambient_color[] = {0.8f, 0.8f, 0.2f, 1.0f};
	GLfloat mat_diffuse[] = {0.1f, 0.5f, 0.8f, 1.0f};
	GLfloat mat_specular[] = {1.0f, 1.0f, 1.0f, 1.0f};
	GLfloat no_shininess = 0.0f;
	GLfloat low_shininess = 5.0f;
	GLfloat high_shininess = 100.0f;
	GLfloat mat_emission[] = {0.3f, 0.2f, 0.2f, 0.0f};


	//set property for global ambient light
	GLfloat  ambientLight[] = { 0.3f, 0.3f, 0.3f, 1.0f};

	//set property for light 0 => front light (white)
	GLfloat	lightFrontPos[] = { 6.5f, 0.0f, 0.0f, 1.0f };		
	GLfloat spotFrontDir[] = { 20.0f, -5.3f, 0.0f };	
	GLfloat colorFront[4] = { 1.0, 1.0, 1.0, 1.0 };
	GLfloat colorFrontAmbient[4] = { 0.1, 0.1, 0.1, 1.0 };		
	float low_spot_angle = 35; //the angle for spot cut off => low beam
	float high_spot_angle = 52;//the angle for spot cut off => high beam

	//set property for light 1 => top light (blue)
	GLfloat	lightTopPos[] = { 5.0f, 10.0f, 0.0f, 1.0f };
	GLfloat spotTopDir[] = { 0.0f, -1.0f, 0.0f };
	GLfloat colorTop[4] = { 0, 0, 1, 1.0 };
	GLfloat colorTopAmbient[4] = { 0.0, 0.0, 0.3, 1.0 };
	float top_spot_angle = 15; //the angle for spot cut off

	//set property for light 2 => view light (yellow)
	GLfloat	lightViewPos[] = { 20.0f, 20.0f, 20.0f, 1.0f };
	GLfloat spotViewDir[] = { -1.0f, -1.0f, -1.0f };
	GLfloat colorView[4] = { 1.0, 1.0, 0.0, 1.0 };
	GLfloat colorViewAmbient[4] = { 0.1, 0.1, 0.0, 1.0 };
	float view_spot_angle = 35; //the angle for spot cut off


	double rad_light = 10;	//the radius of the circle that top light (light 1, blue) moves around
	double angle_light = 0; //the angle that top light (light 1, blue) makes in next move
	
	// Definition of the vertices of the cube.
	typedef GLfloat Point[3];
	Point pt[] = {
	{ -1.0, -1.0, 1.0 },
	{ -1.0, 1.0, 1.0 },
	{ 1.0, 1.0, 1.0 },
	{ 1.0, -1.0, 1.0 },
	{ -1.0, -1.0, -1.0 },
	{ -1.0, 1.0, -1.0 },
	{ 1.0, 1.0, -1.0 },
	{ 1.0, -1.0, -1.0 } };	

	// Definition of the face normal for each side of the cube.
	typedef GLfloat Normal[3];
	Normal normal[] = {
	{ 0.0, 0.0, 1.0 },
	{ 1.0, 0.0, 0.0 },
	{ 0.0, -1.0, 0.0 },
	{ 0.0, 1.0, 0.0 },
	{ 0.0, 0.0, -1.0 },
	{ -1.0, 0.0, 0.0 }
	};						


	// Definition of texturing for each side of the cube.
	Point tCube[]={
		{0.0 ,0.0},
		{0.0 ,0.0},
		{0.0 ,0.0},
		{0.0 ,0.0},
		{0.0 ,0.0},
		{0.0 ,0.0},
		{0.0 ,0.0},
		{0.0 ,0.0}
	};

	
	GLuint  tank_texture[NUM_OF_TEXTURES]; // array for texturing

	GLfloat fogColor[] = {0.8f, 0.8f, 1.0f, 0.0f}; // set fog color

	bool start_blue_light = true;

	float cuttingPos = 3.5;
	bool enableCutPlane_Thuc = false;
//	bool is_collapse_Thuc = false;
	float gun_fall_angle = 0;

/******************************************************************************/


/****************************************************************************************/
/* init texture                                                                                                                                                 */
/****************************************************************************************/
       void initTexture_Thuc(void){
               CreateTexture("texture/camouflage.bmp", tank_texture, 1);
               CreateTexture("texture/tanktrack_blur.bmp", tank_texture, 2);
               CreateTexture("texture/metal.bmp", tank_texture, 3);
               CreateTexture("texture/metal1.bmp", tank_texture, 6);
       }

/****************************************************************************************/
/* Draws one side of the cube.                                                                                                                  */
/****************************************************************************************/

void drawSide_Thuc (int v0, int v1, int v2, int v3, int n, int texture, int t0, int t1, int t2, int t3)
       {

               if(texture == NONE_TEXTURE){
                       glBegin(GL_QUADS);
                                       // All primitive drawings happen between glBegin / glEnd calls.
                               glNormal3fv(normal[n]);
                               glVertex3fv(pt[v0]);
                                       // OpenGL call to provide a vertex.
                               glVertex3fv(pt[v1]);
                               glVertex3fv(pt[v2]);
                               glVertex3fv(pt[v3]);
                       glEnd();
               }
               else if (texture == APPLY_TEXTURE){
                       glBegin(GL_QUADS);
                       // All primitive drawings happen between glBegin / glEnd calls.
                               glNormal3fv(normal[n]);
                               glTexCoord2fv(tCube[t0]); glVertex3fv(pt[v0]);// OpenGL call to provide a vertex.
                               glTexCoord2fv(tCube[t1]); glVertex3fv(pt[v1]);
                               glTexCoord2fv(tCube[t2]); glVertex3fv(pt[v2]);
                               glTexCoord2fv(tCube[t3]); glVertex3fv(pt[v3]);
                       glEnd();
               }
       }


/****************************************************************************************/
/* Draws the cube.                                                                                                                                              */
/****************************************************************************************/
void drawCube_Thuc (unsigned int texture)
       {
               drawSide_Thuc(0, 3, 2, 1, 0, texture, 0, 3, 2, 1);
               drawSide_Thuc(3, 7, 6, 2, 1, texture, 0, 4, 5, 1);
               drawSide_Thuc(0, 4, 7, 3, 2, texture, 0, 4, 7, 6);
               drawSide_Thuc(1, 2, 6, 5, 3, texture, 0, 4, 5, 1);
               drawSide_Thuc(7, 4, 5, 6, 4, texture, 0, 3, 2, 1);
               drawSide_Thuc(4, 0, 1, 5, 5, texture, 0, 4, 5, 1);
}

/****************************************************************************************/
/* draw disc                                                                                                                                            */
/****************************************************************************************/
void drawDisc_Thuc(int texture){

       if(texture == NONE_TEXTURE){
               glBegin(GL_TRIANGLE_FAN);
                       glVertex3f(0,0,0);
                               for(float f=0; f <=360; f+=10)
                                       glVertex3f(2*cos(PI * f/180.0),2*sin(PI * f/180.0),0);
               glEnd();
       }
       else if (texture == APPLY_TEXTURE){

               glBegin(GL_TRIANGLE_FAN);
                       glTexCoord2f(0.3f, 0.3f); glVertex3f(0,0,0);
                               for(float f=0; f <=360; f+=10){
                                       glTexCoord2f(0.3*2*cos(PI * f/180.0), 0.3*2*sin(PI * f/180.0)); glVertex3f(2*cos(PI * f/180.0),2*sin(PI * f/180.0),0);
                               }
               glEnd();
       }
}


/****************************************************************************************/
/* Used to generate a Sphere shape.                                                                                                             */
/****************************************************************************************/
void drawSphere_Thuc(float fRadius, GLint slices, GLint rings)
{
       GLUquadricObj* quatric;
       quatric =  gluNewQuadric();     // Creates a new quadrics object and returns a pointer to it.

       gluQuadricNormals(quatric, GL_SMOOTH);
       gluQuadricTexture(quatric, GL_TRUE);


       gluQuadricDrawStyle(quatric, GLU_FILL);

       gluSphere(quatric, fRadius,slices, rings);      // Draw the sphere with a radius : fRadius.
       gluDeleteQuadric(quatric);                                      // Free the Quadric

}

/****************************************************************************************/
/* generate a cylinder shape.                                                                                                                   */
/****************************************************************************************/
void drawCylinder_Thuc(float fTopR, float fBottomR, float fHeight, GLint slices, GLint stacks)

{
       GLUquadricObj* quatric; // To keep the original texture intact we need to set the current color to WHITE.

       quatric = gluNewQuadric();      // Creates a new quadrics object and returns a pointer to it.

       gluQuadricNormals(quatric, GL_SMOOTH);
       gluQuadricTexture(quatric, GL_TRUE);

       gluQuadricDrawStyle(quatric, GLU_FILL);

       gluCylinder(quatric, fTopR, fBottomR, fHeight, slices, stacks); // Draw the cylinder with a radius : fRadius.
       gluDeleteQuadric(quatric);      // Free the Quadric
}


/****************************************************************************************/
/* draw 2 lines in the back of the tank bodybase                                                                                */
/****************************************************************************************/
void draw3Lines_Thuc(void){
       for (int i=0; i<3; i++){
                       glTranslatef(-0.2, 0.0, 0.0);
                       glPushMatrix();
                       glScalef(0.05, 0.1, 0.6);
                       drawCube_Thuc(NONE_TEXTURE);
                       glPopMatrix();
               }
}


/****************************************************************************************/
/* draw tank body                                                                                                                                               */
/****************************************************************************************/
void drawBodyBase_Thuc(void){

       glPushMatrix();
               glScalef(1.0, 0.07, 0.3);

               //glColor3f(0.6f, 0.53f, 0.44f);
               glColor3f(1.0f, 1.0f, 1.0f);
               glEnable(GL_TEXTURE_2D);
               glBindTexture(GL_TEXTURE_2D, tank_texture[1]);
               glBegin(GL_POLYGON);
                       glTexCoord2f(0.0f, 0.0f); glVertex3f(5.0, 5.0, -5.0);
                       glTexCoord2f(1.0f, 0.0f); glVertex3f(5.0, 5.0, 5.0);
                       glTexCoord2f(1.0f, 1.0f); glVertex3f(6.5, -5.0, 8.0);
                       glTexCoord2f(0.0f, 1.0f); glVertex3f(6.5, -5.0, -8.0);
               glEnd();

               glBegin(GL_POLYGON);
                       glTexCoord2f(0.0f, 0.0f); glVertex3f(5.0, 5.0, 5.0);
                       glTexCoord2f(3.0f, 0.0f); glVertex3f(-6.0, 5.0, 5.0);
                       glTexCoord2f(3.0f, 1.0f); glVertex3f(-5.0, -5.0, 8.0);
                       glTexCoord2f(0.0f, 1.0f); glVertex3f(6.5, -5.0, 8.0);
               glEnd();

               glBegin(GL_POLYGON);
                       glTexCoord2f(0.0f, 0.0f); glVertex3f(-5.0, -5.0, -8.0);
                       glTexCoord2f(1.0f, 0.0f); glVertex3f(-6.0, 5.0, -5.0);
                       glTexCoord2f(1.0f, 1.0f); glVertex3f(-6.0, 5.0, 5.0);
                       glTexCoord2f(0.0f, 1.0f); glVertex3f(-5.0, -5.0, 8.0);
               glEnd();

               glBegin(GL_POLYGON);
                       glTexCoord2f(0.0f, 0.0f); glVertex3f(5.0, 5.0, -5.0);
                       glTexCoord2f(3.0f, 0.0f); glVertex3f(-6.0, 5.0, -5.0);
                       glTexCoord2f(3.0f, 1.0f); glVertex3f(-5.0, -5.0, -8.0);
                       glTexCoord2f(0.0f, 1.0f); glVertex3f(6.5, -5.0, -8.0);
               glEnd();

               //glColor3f(0.3f, 0.25f, 0.19f);
               glBegin(GL_POLYGON);
                       glBegin(GL_POLYGON);
                       glVertex3f(6.5, -5.0, -8.0);
                       glVertex3f(-5.0, -5.0, -8.0);
                       glVertex3f(-5.0, -5.0, 8.0);
                       glVertex3f(6.5, -5.0, 8.0);
               glEnd();
               glEnd();

               //glColor3f(0.73f, 0.68f, 0.55f);
               glBegin(GL_POLYGON);
                       glTexCoord2f(0.0f, 0.0f); glVertex3f(5.0, 5.0, -5.0);
                       glTexCoord2f(3.0f, 0.0f); glVertex3f(-6.0, 5.0, -5.0);
                       glTexCoord2f(3.0f, 1.0f); glVertex3f(-6.0, 5.0, 5.0);
                       glTexCoord2f(0.0f, 1.0f); glVertex3f(5.0, 5.0, 5.0);
               glEnd();

               //draw small piece front left & right => de`
               //glColor3f(0.56f, 0.56f, 0.34f);
               glBegin(GL_POLYGON);
                       glTexCoord2f(0.0f, 0.0f); glVertex3f(6.5, -5.0, 8.0);
                       glTexCoord2f(0.5f, 0.0f); glVertex3f(7.5, -14.0, 8.0);
                       glTexCoord2f(0.5f, 0.5f); glVertex3f(7.5, -14.0, 3.5);
                       glTexCoord2f(0.0f, 0.5f); glVertex3f(6.5, -5.0, 3.5);
               glEnd();

               glBegin(GL_POLYGON);
                       glTexCoord2f(0.0f, 0.0f); glVertex3f(6.5, -5.0, -8.0);
                       glTexCoord2f(0.5f, 0.0f); glVertex3f(7.5, -14.0, -8.0);
                       glTexCoord2f(0.5f, 0.5f); glVertex3f(7.5, -14.0, -3.5);
                       glTexCoord2f(0.0f, 0.5f); glVertex3f(6.5, -5.0, -3.5);
               glEnd();
       glPopMatrix();
       glDisable(GL_TEXTURE_2D);

       //////draw small left and right cubes///////////////////

       //change mapping points for texture
       tCube[0][0] = 0.0;      tCube[0][1] = 0.0;      tCube[1][0] = 0.0;      tCube[1][1] = 1.0;
       tCube[2][0] = 5.0;      tCube[2][1] = 1.0;      tCube[3][0] = 5.0;      tCube[3][1] = 0.0;
       tCube[4][0] = 1.0;      tCube[4][1] = 0.0;  tCube[5][0] = 1.0;  tCube[5][1] = 1.0;
       tCube[6][0] = 5.0;      tCube[6][1] = 0.0;  tCube[7][0] = 1.0;  tCube[7][1] = 5.0;

       //glColor3f(0.3f, 0.25f, 0.19f);
       glColor3f(1.0f, 1.0f, 1.0f);
       glEnable(GL_TEXTURE_2D);
       glBindTexture(GL_TEXTURE_2D, tank_texture[1]);
       glPushMatrix();
               glTranslatef(0.5, -0.6, 1.9);
               glScalef(5.3, 0.3, 0.6);
               drawCube_Thuc(APPLY_TEXTURE);
       glPopMatrix();

       glPushMatrix();
               glTranslatef(0.5, -0.6, -1.9);
               glScalef(5.3, 0.3, 0.6);
               drawCube_Thuc(APPLY_TEXTURE);
       glPopMatrix();

       glDisable(GL_TEXTURE_2D);

       //draw 2 spheres in the bodybase
       glColor3f(0.57f, 0.58f, 0.49f);
       glPushMatrix();
               glTranslatef(-3.5, -0.03, 0.8);
               glScalef(1.0, 0.5, 1.0);
               drawSphere_Thuc(1.0, 30, 30);
       glPopMatrix();

       glPushMatrix();
               glTranslatef(-3.5, -0.03, -0.8);
               glScalef(1.0, 0.5, 1.0);
               drawSphere_Thuc(1.0, 30, 30);
       glPopMatrix();

       // draw lines in the end of bodybase
       glColor3f(0.67f, 0.49f, 0.32f);
       glPushMatrix();
               glTranslatef(-4.3, 0.3, 0.8);//left down 1
               draw3Lines_Thuc();
       glPopMatrix();

       glPushMatrix();
               glTranslatef(-4.9, 0.3, 0.8);//left down 2
               draw3Lines_Thuc();
       glPopMatrix();

       glPushMatrix();
               glTranslatef(-4.3, 0.3, -0.8);//right down 1
               draw3Lines_Thuc();
       glPopMatrix();

       glPushMatrix();
               glTranslatef(-4.9, 0.3, -0.8);//right down 2
               draw3Lines_Thuc();
       glPopMatrix();

       glPushMatrix();
               glTranslatef(-1.9, 0.3, 0.8);//left up
               draw3Lines_Thuc();
       glPopMatrix();

       glPushMatrix();
               glTranslatef(-1.9, 0.3, -0.8);//right up
               draw3Lines_Thuc();
       glPopMatrix();

       //center line
       glPushMatrix();
       glTranslatef(-3.9, 0.35, 0.0);
       glScalef(2.0, 0.05, 0.02);
       drawCube_Thuc(NONE_TEXTURE);
       glPopMatrix();

       //draw center sphere right behind the turret
       glColor3f(0.57f, 0.58f, 0.49f);
       glPushMatrix();
               glTranslatef(-1.6, 0.25, 0.0);
               glScalef(0.4, 0.2, 0.4);
               drawSphere_Thuc(1.0, 10, 10);
       glPopMatrix();

       // draw antena
       glColor3f(0.3f, 0.25f, 0.19f);
       glPushMatrix();
               glTranslatef(-1.6, 0.20, 1.0);
               glScalef(0.3, 0.3, 0.3);
               drawSphere_Thuc(1.0, 10, 10);
       glPopMatrix();

       glColor3f(0.0f, 0.0f, 0.0f);
       glPushMatrix();
       glTranslatef(-1.6, 0.20, 1.0);
       glBegin(GL_LINES);
     glVertex3f(0, 0, 0);
     glVertex3f(0, 8, 0);
       glEnd();
       glPopMatrix();

       //draw lines decoration in front
       glColor3f(0.67f, 0.49f, 0.32f);//left
       glPushMatrix();
       glTranslatef(4.8, 0.2, 0.5);
       glScalef(0.05, 0.2, 0.2);
       drawCube_Thuc(NONE_TEXTURE);
       glPopMatrix();

       glPushMatrix();
       glTranslatef(4.67, 0.2, 1.0);
       glRotatef(-30, 0.0, 1.0, 0.0);
       glScalef(0.05, 0.2, 0.2);
       drawCube_Thuc(NONE_TEXTURE);
       glPopMatrix();

       glPushMatrix();//right
       glTranslatef(4.8, 0.2, -0.5);
       glScalef(0.05, 0.2, 0.2);
       drawCube_Thuc(NONE_TEXTURE);
       glPopMatrix();

       glPushMatrix();
       glTranslatef(4.67, 0.2, -1.0);
       glRotatef(30, 0.0, 1.0, 0.0);
       glScalef(0.05, 0.2, 0.2);
       drawCube_Thuc(NONE_TEXTURE);
       glPopMatrix();

       glColor3f(0.53f, 0.42f, 0.32f);
       glPushMatrix();//center front
       glTranslatef(4.1, 0.2, 0.0);
       glScalef(0.4, 0.2, 0.9);
       drawCube_Thuc(NONE_TEXTURE);
       glPopMatrix();

       //center circles front
       glColor3f(0.57f, 0.58f, 0.49f);
       glPushMatrix();
               glTranslatef(4.1, 0.2, 0.0);
               glScalef(0.3, 0.3, 0.3);
               drawSphere_Thuc(1.0, 10, 10);
       glPopMatrix();

       glPushMatrix();
               glTranslatef(4.1, 0.2, 0.55);
               glScalef(0.3, 0.3, 0.3);
               drawSphere_Thuc(1.0, 10, 10);
       glPopMatrix();

       glPushMatrix();
               glTranslatef(4.1, 0.2, -0.55);
               glScalef(0.3, 0.3, 0.3);
               drawSphere_Thuc(1.0, 10, 10);
       glPopMatrix();
}

/****************************************************************************************/
/* draw body bottom                                                                                                                                             */
/****************************************************************************************/
void drawBodyBottom_Thuc(void){

       glColor3f(0.3f, 0.25f, 0.19f);
       glPushMatrix();
               glScalef(6.0, 1.0, 1.5);
               drawCube_Thuc(NONE_TEXTURE);
       glPopMatrix();

       //draw 2 cylinders at the end of the body
       glColor3f(0.67f, 0.49f, 0.32f);
       glPushMatrix();
               glTranslatef(-6.0, 1.0, -0.8);
               glRotatef(90, 1.0, 0.0, 0.0);
               drawCylinder_Thuc(0.1, 0.1, 1.0, 15, 20);
       glPopMatrix();

       glPushMatrix();
               glTranslatef(-6.0, 1.0, 0.8);
               glRotatef(90, 1.0, 0.0, 0.0);
               drawCylinder_Thuc(0.1, 0.1, 1.0, 15, 20);
       glPopMatrix();

       //draw 2 spleres at the end of the body
       glPushMatrix();
               glTranslatef(-6.0, 0.1, -0.8);
               drawSphere_Thuc(0.23, 10, 10);
       glPopMatrix();

       glPushMatrix();
               glTranslatef(-6.0, 0.1, 0.8);
               drawSphere_Thuc(0.23, 10, 10);
       glPopMatrix();

       //draw center splere at the end of the body
       glColor3f(0.57f, 0.58f, 0.49f);
       glPushMatrix();
               glTranslatef(-6.0, -0.2, 0.0);
               drawSphere_Thuc(0.3, 10, 10);
       glPopMatrix();

       //draw small spleres (next to the center sphere) at the end of the body
       //left
       glPushMatrix();
               glTranslatef(-6.0, -0.4, -0.5);
               drawSphere_Thuc(0.13, 10, 10);
       glPopMatrix();

       glPushMatrix();
               glTranslatef(-6.0, -0.5, -1.2);
               drawSphere_Thuc(0.2, 10, 10);
       glPopMatrix();

       //right
       glPushMatrix();
               glTranslatef(-6.0, -0.4, 0.5);
               drawSphere_Thuc(0.13, 10, 10);
       glPopMatrix();

       glPushMatrix();
               glTranslatef(-6.0, -0.5, 1.2);
               drawSphere_Thuc(0.2, 10, 10);
       glPopMatrix();

       //draw 2 cubes at the front of the body
       glPushMatrix();
       glTranslatef(6.0, 0.1, -0.5);
       glScalef(0.1, 0.2, 0.07);
       drawCube_Thuc(NONE_TEXTURE);
       glPopMatrix();

       glPushMatrix();
       glTranslatef(6.0, 0.1, 0.5);
       glScalef(0.1, 0.07, 0.2);
       drawCube_Thuc(NONE_TEXTURE);
       glPopMatrix();
}

/****************************************************************************************/
/* draw Gun                                                                                                                                                             */
/****************************************************************************************/
void drawGun_Thuc(void){
       glColor3f(1.0f, 1.0f, 1.0f);
       glEnable(GL_TEXTURE_2D);
       glBindTexture(GL_TEXTURE_2D, tank_texture[1]);

       //drawCylinder_Thuc(0.15, 0.15, 8.0, 15, 10);
       drawCylinder_Thuc(0.15, 0.15, 2.0, 15, 10);
       glPushMatrix();
       for(int i1=0; i1<3; i1++){
               glTranslatef(0.0, 0.0, 2.0);
               drawCylinder_Thuc(0.15, 0.15, 2.0, 15, 10);
       }
       glPopMatrix();
       glDisable(GL_TEXTURE_2D);

       glColor3f(0.53f, 0.42f, 0.32f);
       glPushMatrix();
       glTranslatef(0.0, 0.0, 0.3);
       for (int i=0; i<8; i++){
               glTranslatef(0.0, 0.0, 0.05);
               glutSolidTorus(0.10, 0.25, 10, 10);
       }

       for (int j=0; j<5; j++){
               glTranslatef(0.0, 0.0, 0.05);
               glutSolidTorus(0.05, 0.20, 10, 10);
       }

       glColor3f(0.53f, 0.42f, 0.32f);
       glTranslatef(0.0, 0.0, 6.8);
       for (int t=0; t<5; t++){
               glTranslatef(0.0, 0.0, 0.05);
               glutSolidTorus(0.05, 0.20, 10, 10);
       }
       glPopMatrix();

       glColor3f(0.6f, 0.53f, 0.44f);
       glPushMatrix();
       glTranslatef(0.0, 0.0, 8.15);
       glScalef(0.25, 0.25, 0.25);
       drawSphere_Thuc(1.0, 10, 10);
       glPopMatrix();

}

/****************************************************************************************/
/* draw         Barrel                                                                                                                                          */
/****************************************************************************************/
void drawSmallBarrel_Thuc(void){
       glColor3f(0.73f, 0.68f, 0.55f);
       glPushMatrix();
       glRotatef(90, 0.0, 1.0, 0.0);
       drawCylinder_Thuc(0.1, 0.1, 2.0, 15, 20);
       glPopMatrix();

       //draw the aim
       glColor3f(0.3f, 0.25f, 0.19f);
       glPushMatrix();
       glTranslatef(1.95, 0.1, 0.0);
       glBegin(GL_LINES);
     glVertex3f(0, 0, 0);
     glVertex3f(0, 0.1, 0);
       glEnd();
       glPopMatrix();
}

/****************************************************************************************/
/* draw Machine gun                                                                                                                                             */
/****************************************************************************************/
void drawMachineGun_Thuc(void){

       //draw gia' do*~ su'ng
       glColor3f(0.53f, 0.42f, 0.32f);
       glPushMatrix();
       glTranslatef(0.8, 1.0, -0.5);
       glRotatef(-90, 1.0, 0.0, 0.0);
       drawCylinder_Thuc(0.05, 0.05, 1.7, 15, 20);
       glPopMatrix();

       glPushMatrix();
       glTranslatef(0.8, 2.7, -0.5);
       glScalef(0.05, 0.05, 0.05);
       drawSphere_Thuc(1, 20, 20);
       glPopMatrix();

       //draw the join of barrel and gia' do~ su'ng
       glColor3f(0.53f, 0.42f, 0.32f);
       glPushMatrix();
       glTranslatef(0.8, 2.3, -0.5);
       glScalef(0.2, 0.2, 0.2);
       drawSphere_Thuc(1, 20, 20);
       glPopMatrix();

       //draw the barrel
       glPushMatrix();
       glTranslatef(0.35, 2.3, -0.5);
       glRotatef(barrel_Angle, 1.0, 0.0, 0.0);//rotate machine gun when any action
       drawSmallBarrel_Thuc();
       glPopMatrix();
}

/****************************************************************************************/
/* draw gun in the side                                                                                                                                 */
/****************************************************************************************/
void drawSideGun_Thuc(void){

       //draw the base
       glColor3f(0.3f, 0.25f, 0.19f);
       glPushMatrix();
       glScalef(0.2, 0.2, 0.2);
       drawSphere_Thuc(1, 20, 20);
       glPopMatrix();

       //draw the gun
       glColor3f(0.53f, 0.42f, 0.32f);
       glPushMatrix();
       glRotatef(50, 0.0, 1.0, 1.0);
       drawCylinder_Thuc(0.1, 0.1, 0.7, 15, 20);
       glPopMatrix();
}

/****************************************************************************************/
/* draw Turret                                                                                                                                                  */
/****************************************************************************************/
void drawTurret_Thuc(void){

/*      GLdouble cutPlane[4] = {0, -1, 0, 1};
       glClipPlane (GL_CLIP_PLANE0, cutPlane);
               glEnable(GL_CLIP_PLANE0);
*/
       //draw turret
       glColor3f(0.8f, 0.6f, 0.44f);
       glPushMatrix();
       glRotatef(90, 1.0, 0.0, 0.0);
       glScalef(1.5, 1.0, 2.0);
       glutSolidTorus(0.7, 0.7, 30, 30);

       glColor3f(1.0f, 1.0f, 1.0f);
       glEnable(GL_TEXTURE_2D);
       glBindTexture(GL_TEXTURE_2D, tank_texture[1]);

       glTranslatef(0.0, 0.0, -0.7);
       glScalef(0.42, 0.42, 0.42);
       drawDisc_Thuc(APPLY_TEXTURE);

       glPopMatrix();
//glDisable(GL_CLIP_PLANE0);

       //change mapping points for texture
       tCube[0][0] = 0.0;      tCube[0][1] = 0.0;      tCube[1][0] = 0.0;      tCube[1][1] = 1.5;
       tCube[2][0] = 1.5;      tCube[2][1] = 1.5;      tCube[3][0] = 1.5;      tCube[3][1] = 0.0;
       tCube[4][0] = 1.0;      tCube[4][1] = 0.0;  tCube[5][0] = 1.0;  tCube[5][1] = 1.0;
       tCube[6][0] = 1.0;      tCube[6][1] = 0.0;  tCube[7][0] = 1.0;  tCube[7][1] = 1.0;

       glColor3f(1.0f, 1.0f, 1.0f);
       glEnable(GL_TEXTURE_2D);
       glBindTexture(GL_TEXTURE_2D, tank_texture[1]);

       glPushMatrix();
       glTranslatef(1.7, 0.0, 0.0);
       glScalef(0.8, 1.4, 0.7);
       drawCube_Thuc(APPLY_TEXTURE);
       glPopMatrix();

       glDisable(GL_TEXTURE_2D);

       glPushMatrix();
       glColor3f(0.53f, 0.42f, 0.32f);
       glTranslatef(2.5, 0.6, 0.0);
       glScalef(0.5, 0.8, 0.7);
       drawSphere_Thuc(1.0,10,10);
       glPopMatrix();

       //set shininess property
   /*    if (shiny){
               setPropertyShiny();
       }
*/
       if (enableCutPlane_Thuc)// && !is_collapse_Thuc)
       {
       //draw gun
       glPushMatrix();
       glTranslatef(2.5, 0.6, 0.0);
       glRotatef(90, 0.0, 1.0, 0.0);
       glRotatef(-5, 1.0, 0.0, 0.0);
       //glRotatef(gun_Angle, 1.0, 0.0, 0.0);//rotate machine gun when any action
       glRotatef(gun_fall_angle, 1.0, 0.0, 0.0);
       drawGun_Thuc();
       glPopMatrix();
	   }
       else
       {
		   if (is_collapse_Thuc){
			   glPushMatrix();
				glTranslatef(2.5, 0.6, 0.0);
				glRotatef(90, 0.0, 1.0, 0.0);
				glRotatef(-5, 1.0, 0.0, 0.0);
				//glRotatef(gun_Angle, 1.0, 0.0, 0.0);//rotate machine gun when any action
				glRotatef(30, 1.0, 0.0, 0.0);
				drawGun_Thuc();
				glPopMatrix();
		   }
		   else{
			//draw gun
			glPushMatrix();
			glTranslatef(2.5, 0.6, 0.0);
			glRotatef(90, 0.0, 1.0, 0.0);
			//glRotatef(gun_fall_angle, 1.0, 0.0, 0.0);
			glRotatef(gun_Angle, 1.0, 0.0, 0.0);//rotate machine gun when any action
			drawGun_Thuc();
			glPopMatrix();
			}
	   }

 //      setPropertyRusty();

       glColor3f(1.0f, 1.0f, 1.0f);

       glPushMatrix();

       glEnable(GL_TEXTURE_2D);
       glBindTexture(GL_TEXTURE_2D, tank_texture[1]);

       glTranslatef(0.0, 1.6, -0.5);
       glRotatef(90, 1.0, 0.0, 0.0);
       drawCylinder_Thuc(0.5, 0.5, 0.4, 15, 30);

       glDisable(GL_TEXTURE_2D);

       glColor3f(0.67f, 0.49f, 0.32f);
       glScalef(0.25, 0.25, 0.25);
       drawDisc_Thuc(NONE_TEXTURE);

       glPopMatrix();

       //draw the small lock door
       glColor3f(1.0f, 1.0f, 1.0f);
       glPushMatrix();

       glEnable(GL_TEXTURE_2D);
       glBindTexture(GL_TEXTURE_2D, tank_texture[1]);

       glTranslatef(0.4, 1.45, 0.5);
       glRotatef(90, 1.0, 0.0, 0.0);
       drawCylinder_Thuc(0.2, 0.2, 0.2, 15, 30);

       glDisable(GL_TEXTURE_2D);


       glColor3f(0.67f, 0.49f, 0.32f);
       glScalef(0.1, 0.1, 0.15);
       drawDisc_Thuc(NONE_TEXTURE);
       glPopMatrix();

       //glDisable(GL_CLIP_PLANE0);

       //draw side gun left
       glPushMatrix();
       glTranslatef(0.5, 1.0, 1.2);
       drawSideGun_Thuc();
       glPopMatrix();

       glPushMatrix();
       glTranslatef(0.1, 1.0, 1.3);
       drawSideGun_Thuc();
       glPopMatrix();

       //draw side gun right
       glPushMatrix();
       glTranslatef(0.5, 1.0, -1.2);
       glRotatef(100, 0.0, 1.0, 0.0);
       drawSideGun_Thuc();
       glPopMatrix();

       glPushMatrix();
       glTranslatef(0.1, 1.0, -1.3);
       glRotatef(100, 0.0, 1.0, 0.0);
       drawSideGun_Thuc();
       glPopMatrix();

//      glDisable(GL_CLIP_PLANE0);

       drawMachineGun_Thuc();
}

/****************************************************************************************/
/* draw body                                                                                                                                                    */
/****************************************************************************************/
void drawBody_Thuc(void){
       drawBodyBase_Thuc();

       glPushMatrix();
       glTranslatef(0.5, -1.35, 0.0);
       drawBodyBottom_Thuc();
       glPopMatrix();
}

/****************************************************************************************/
/* draw a big wheel                                                                                                                                             */
/****************************************************************************************/
void drawBigWheel_Thuc(void){

       //draw the outer circle
       //glColor3f(0.37f, 0.37f, 0.36f);
       glColor3f(0.2f, 0.2f, 0.2f);
       drawCylinder_Thuc(1, 1, 0.5, 15, 10);
       drawCylinder_Thuc(0.85, 0.85, 0.5, 15, 10);
       drawCylinder_Thuc(0.8, 0.8, 0.5, 15, 10);
       drawCylinder_Thuc(0.2, 0.2, 0.5, 15, 10);

       //draw details in
       glPushMatrix();
       glTranslatef(0.0, 0.0, 0.25);
       drawSphere_Thuc(0.17, 10, 10);
       glPopMatrix();

       glPushMatrix();
       glTranslatef(0.3, 0.0, 0.25);
       glScalef(0.5, 0.5, 0.5);
       drawSphere_Thuc(0.17, 10, 10);
       glPopMatrix();

       glPushMatrix();
       glTranslatef(-0.3, 0.0, 0.25);
       glScalef(0.5, 0.5, 0.5);
       drawSphere_Thuc(0.17, 10, 10);
       glPopMatrix();

       glColor3f(0.49f, 0.44f, 0.34f);
       glPushMatrix();
       glTranslatef(0.0, 0.0, 0.25);
       glScalef(0.49, 0.49, 1.0);
       drawDisc_Thuc(NONE_TEXTURE);
       glPopMatrix();
}

/****************************************************************************************/
/* draw a small wheel                                                                                                                                   */
/****************************************************************************************/
void drawSmallWheel_Thuc(void){

       //draw the outer circle
       glColor3f(0.2f, 0.2f, 0.2f);
       drawCylinder_Thuc(1, 1, 0.5, 15, 10);
       drawCylinder_Thuc(0.65, 0.65, 0.5, 15, 10);

       glPushMatrix();
       glTranslatef(0.0, 0.0, 0.25);
       drawSphere_Thuc(0.3, 10, 10);
       glPopMatrix();

       //draw details
       glPushMatrix();
       glTranslatef(0.0, 0.75, 0.25);
       drawSphere_Thuc(0.17, 10, 10);
       glPopMatrix();

       glPushMatrix();
       glTranslatef(0.0, -0.75, 0.25);
       drawSphere_Thuc(0.17, 10, 10);
       glPopMatrix();

       glPushMatrix();
       glTranslatef(0.75, 0.0, 0.25);
       drawSphere_Thuc(0.17, 10, 10);
       glPopMatrix();

       glPushMatrix();
       glTranslatef(-0.75, 0.0, 0.25);
       drawSphere_Thuc(0.17, 10, 10);
       glPopMatrix();

       glPushMatrix();
       glTranslatef(0.4, 0.7, 0.25);
       drawSphere_Thuc(0.17, 10, 10);
       glPopMatrix();

       glPushMatrix();
       glTranslatef(0.7, 0.4, 0.25);
       drawSphere_Thuc(0.17, 10, 10);
       glPopMatrix();

       glPushMatrix();
       glTranslatef(-0.4, -0.7, 0.25);
       drawSphere_Thuc(0.17, 10, 10);
       glPopMatrix();

       glPushMatrix();
       glTranslatef(-0.7, -0.4, 0.25);
       drawSphere_Thuc(0.17, 10, 10);
       glPopMatrix();

       glPushMatrix();
       glTranslatef(-0.4, 0.7, 0.25);
       drawSphere_Thuc(0.17, 10, 10);
       glPopMatrix();

       glPushMatrix();
       glTranslatef(-0.7, 0.4, 0.25);
       drawSphere_Thuc(0.17, 10, 10);
       glPopMatrix();

       glPushMatrix();
       glTranslatef(0.4, -0.7, 0.25);
       drawSphere_Thuc(0.17, 10, 10);
       glPopMatrix();

       glPushMatrix();
       glTranslatef(0.7, -0.4, 0.25);
       drawSphere_Thuc(0.17, 10, 10);
       glPopMatrix();

       glColor3f(0.49f, 0.44f, 0.34f);
       glPushMatrix();
       glTranslatef(0.0, 0.0, 0.25);
       glScalef(0.49, 0.49, 1.0);
       drawDisc_Thuc(NONE_TEXTURE);
       glPopMatrix();
}

/****************************************************************************************/
/* draw 1 track                                                                                                                                                 */
/****************************************************************************************/
void drawOneTrack_Thuc(bool odd_track){

       glEnable(GL_TEXTURE_2D);
       glBindTexture(GL_TEXTURE_2D, tank_texture[2]);

       if (odd_track){
               //glColor3f(0.0f, 0.0f, 0.0f);
               glBegin(GL_POLYGON);
                       glNormal3f(0.0, 1.0, 0.0);
                       glTexCoord2f(0.0f, 0.0f); glVertex3f(0.0, 1.0, -0.5);
                       glTexCoord2f(1.0f, 0.0f); glVertex3f(0.0, 1.0, 0.5);
                       glTexCoord2f(1.0f, 0.5f); glVertex3f(0.5, 1.0, 0.5);
                       glTexCoord2f(0.0f, 0.5f); glVertex3f(0.5, 1.0, -0.5);
               glEnd();

       }
       else{
               //glColor3f(0.2f, 0.2f, 0.2f);
               glBegin(GL_POLYGON);
                       glNormal3f(0.0, 1.0, 0.0);
                       glTexCoord2f(0.0f, 1.0f); glVertex3f(0.0, 1.0, -0.5);
                       glTexCoord2f(1.0f, 1.0f); glVertex3f(0.0, 1.0, 0.5);
                       glTexCoord2f(1.0f, 1.1f); glVertex3f(0.5, 1.0, 0.5);
                       glTexCoord2f(0.0f, 1.1f); glVertex3f(0.5, 1.0, -0.5);
               glEnd();
       }

       glDisable(GL_TEXTURE_2D);
}

/****************************************************************************************/
/* draw half of the track                                                                                                                               */
/****************************************************************************************/
void drawHalfTrack_Thuc(void){

       drawOneTrack_Thuc(true);

       //straight tracks
       glPushMatrix();

       for (int i=0; i<17; i++){
               glTranslatef(0.5, 0.0, 0.0);

               if (track%2 == 0){
                       if (i%2 == 0)
                               drawOneTrack_Thuc(false);

                       else
                               drawOneTrack_Thuc(true);
               }

               else {
                       if (i%2 == 0)
                               drawOneTrack_Thuc(true);

                       else
                               drawOneTrack_Thuc(false);

               }
       }

       glPopMatrix();
}

/****************************************************************************************/
/*      draw Caterpillar                                                                                                                                        */
/****************************************************************************************/
void drawCaterpillar_Thuc(void){

       // draw up and down straight tracks
       glPushMatrix();
       glTranslatef(-4.5, 0.0, 0.0);

       drawHalfTrack_Thuc();//draw up straight tracks

       glPushMatrix();
       glTranslatef(0.0, -2.0, 0.0);
       drawHalfTrack_Thuc();//draw down straight tracks
       glPopMatrix();
       glPopMatrix();

       //draw tracks running circle in the right side
       glEnable(GL_TEXTURE_2D);
       glBindTexture(GL_TEXTURE_2D, tank_texture[2]);
       if (track%2 ==0){

               glBegin(GL_POLYGON);
               //      glNormal3f(-2.5, 1.0, 0.0);
                       glTexCoord2f(0.0f, 0.0f); glVertex3f(4.5, 1.0, -0.5);
                       glTexCoord2f(1.0f, 0.0f); glVertex3f(4.5, 1.0, 0.5);
                       glTexCoord2f(1.0f, 0.5f); glVertex3f(5.0, 1.2, 0.5);
                       glTexCoord2f(0.0f, 0.5f); glVertex3f(5.0, 1.2, -0.5);
               glEnd();
       }
       else{
               glBegin(GL_POLYGON);
               //      glNormal3f(-2.5, 1.0, 0.0);
                       glTexCoord2f(0.0f, 1.0f); glVertex3f(4.5, 1.0, -0.5);
                       glTexCoord2f(1.0f, 1.0f); glVertex3f(4.5, 1.0, 0.5);
                       glTexCoord2f(1.0f, 1.1f); glVertex3f(5.0, 1.2, 0.5);
                       glTexCoord2f(0.0f, 1.1f); glVertex3f(5.0, 1.2, -0.5);
               glEnd();
       }

       if (track%2 ==0){
               glBegin(GL_POLYGON);
               //      glNormal3f(-0.5, 1.0, 0.0);
                       glTexCoord2f(0.0f, 1.0f); glVertex3f(5.0, 1.2, -0.5);
                       glTexCoord2f(1.0f, 1.0f); glVertex3f(5.0, 1.2, 0.5);
                       glTexCoord2f(1.0f, 1.1f); glVertex3f(5.5, 1.3, 0.5);
                       glTexCoord2f(0.0f, 1.1f); glVertex3f(5.5, 1.3, -0.5);
               glEnd();
       }
       else{
               glBegin(GL_POLYGON);
               //      glNormal3f(-0.5, 1.0, 0.0);
                       glTexCoord2f(0.0f, 0.0f); glVertex3f(5.0, 1.2, -0.5);
                       glTexCoord2f(1.0f, 0.0f); glVertex3f(5.0, 1.2, 0.5);
                       glTexCoord2f(1.0f, 0.5f); glVertex3f(5.5, 1.3, 0.5);
                       glTexCoord2f(0.0f, 0.5f); glVertex3f(5.5, 1.3, -0.5);
               glEnd();
       }

       if (track%2 ==0){
               glBegin(GL_POLYGON);
               //      glNormal3f(0.0, 1.0, 0.0);
                       glTexCoord2f(0.0f, 0.0f); glVertex3f(5.5, 1.3, -0.5);
                       glTexCoord2f(1.0f, 0.0f); glVertex3f(5.5, 1.3, 0.5);
                       glTexCoord2f(1.0f, 0.5f); glVertex3f(5.9, 1.3, 0.5);
                       glTexCoord2f(0.0f, 0.5f); glVertex3f(5.9, 1.3, -0.5);
               glEnd();
       }
       else{
               glBegin(GL_POLYGON);
               //      glNormal3f(0.0, 1.0, 0.0);
                       glTexCoord2f(0.0f, 1.0f); glVertex3f(5.5, 1.3, -0.5);
                       glTexCoord2f(1.0f, 1.0f); glVertex3f(5.5, 1.3, 0.5);
                       glTexCoord2f(1.0f, 1.1f); glVertex3f(5.9, 1.3, 0.5);
                       glTexCoord2f(0.0f, 1.1f); glVertex3f(5.9, 1.3, -0.5);
               glEnd();
       }

       if (track%2 ==0){
               glBegin(GL_POLYGON);
               //      glNormal3f(float(4/3), 1.0, 0.0);
                       glTexCoord2f(0.0f, 1.0f); glVertex3f(5.9, 1.3, -0.5);
                       glTexCoord2f(1.0f, 1.0f); glVertex3f(5.9, 1.3, 0.5);
                       glTexCoord2f(1.0f, 1.1f); glVertex3f(6.3, 1.0, 0.5);
                       glTexCoord2f(0.0f, 1.1f); glVertex3f(6.3, 1.0, -0.5);
               glEnd();
       }
       else{
               glBegin(GL_POLYGON);
               //      glNormal3f(float(4/3), 1.0, 0.0);
                       glTexCoord2f(0.0f, 0.0f); glVertex3f(5.9, 1.3, -0.5);
                       glTexCoord2f(1.0f, 0.0f); glVertex3f(5.9, 1.3, 0.5);
                       glTexCoord2f(1.0f, 0.5f); glVertex3f(6.3, 1.0, 0.5);
                       glTexCoord2f(0.0f, 0.5f); glVertex3f(6.3, 1.0, -0.5);
               glEnd();
       }

       if (track%2 ==0){
               glBegin(GL_POLYGON);
               //      glNormal3f(0.4, 1.0, 0.0);
                       glTexCoord2f(0.0f, 0.0f); glVertex3f(6.3, 1.0, -0.5);
                       glTexCoord2f(1.0f, 0.0f); glVertex3f(6.3, 1.0, 0.5);
                       glTexCoord2f(1.0f, 0.5f); glVertex3f(6.5, 0.5, 0.5);
                       glTexCoord2f(0.0f, 0.5f); glVertex3f(6.5, 0.5, -0.5);
               glEnd();
       }
       else{
               glBegin(GL_POLYGON);
               //      glNormal3f(0.4, 1.0, 0.0);
                       glTexCoord2f(0.0f, 1.0f); glVertex3f(6.3, 1.0, -0.5);
                       glTexCoord2f(1.0f, 1.0f); glVertex3f(6.3, 1.0, 0.5);
                       glTexCoord2f(1.0f, 1.1f); glVertex3f(6.5, 0.5, 0.5);
                       glTexCoord2f(0.0f, 1.1f); glVertex3f(6.5, 0.5, -0.5);
               glEnd();
       }

       if (track%2 ==0){
               glBegin(GL_POLYGON);
               //      glNormal3f(-0.333333, 1.0, 0.0);
                       glTexCoord2f(0.0f, 1.0f); glVertex3f(6.3, -0.1, -0.5);
                       glTexCoord2f(1.0f, 1.0f); glVertex3f(6.3, -0.1, 0.5);
                       glTexCoord2f(1.0f, 1.1f); glVertex3f(6.5, 0.5, 0.5);
                       glTexCoord2f(0.0f, 1.1f); glVertex3f(6.5, 0.5, -0.5);
               glEnd();
       }
       else{
               glBegin(GL_POLYGON);
               //      glNormal3f(-0.333333, 1.0, 0.0);
                       glTexCoord2f(0.0f, 0.0f); glVertex3f(6.3, -0.1, -0.5);
                       glTexCoord2f(1.0f, 0.0f); glVertex3f(6.3, -0.1, 0.5);
                       glTexCoord2f(1.0f, 0.5f); glVertex3f(6.5, 0.5, 0.5);
                       glTexCoord2f(0.0f, 0.5f); glVertex3f(6.5, 0.5, -0.5);
               glEnd();
       }

       if (track%2 ==0){
               glBegin(GL_POLYGON);
                       glTexCoord2f(0.0f, 0.0f); glVertex3f(5.7, -0.5, -0.5);
                       glTexCoord2f(1.0f, 0.0f); glVertex3f(5.7, -0.5, 0.5);
                       glTexCoord2f(1.0f, 0.5f); glVertex3f(6.3, -0.1, 0.5);
                       glTexCoord2f(0.0f, 0.5f); glVertex3f(6.3, -0.1, -0.5);
               glEnd();
       }
       else{
               glBegin(GL_POLYGON);
                       glTexCoord2f(0.0f, 1.0f); glVertex3f(5.7, -0.5, -0.5);
                       glTexCoord2f(1.0f, 1.0f); glVertex3f(5.7, -0.5, 0.5);
                       glTexCoord2f(1.0f, 1.1f); glVertex3f(6.3, -0.1, 0.5);
                       glTexCoord2f(0.0f, 1.1f); glVertex3f(6.3, -0.1, -0.5);
               glEnd();
       }

       if (track%2 ==0){
               glBegin(GL_POLYGON);
                       glTexCoord2f(0.0f, 1.0f); glVertex3f(5.3, -0.7, -0.5);
                       glTexCoord2f(1.0f, 1.0f); glVertex3f(5.3, -0.7, 0.5);
                       glTexCoord2f(1.0f, 1.1f); glVertex3f(5.7, -0.5, 0.5);
                       glTexCoord2f(0.0f, 1.1f); glVertex3f(5.7, -0.5, -0.5);
               glEnd();
       }
       else{
               glBegin(GL_POLYGON);
                       glTexCoord2f(0.0f, 0.0f); glVertex3f(5.3, -0.7, -0.5);
                       glTexCoord2f(1.0f, 0.0f); glVertex3f(5.3, -0.7, 0.5);
                       glTexCoord2f(1.0f, 0.5f); glVertex3f(5.7, -0.5, 0.5);
                       glTexCoord2f(0.0f, 0.5f); glVertex3f(5.7, -0.5, -0.5);
               glEnd();
       }

       if (track%2 ==0){
               glBegin(GL_POLYGON);
                       glTexCoord2f(0.0f, 0.0f); glVertex3f(4.9, -0.9, -0.5);
                       glTexCoord2f(1.0f, 0.0f); glVertex3f(4.9, -0.9, 0.5);
                       glTexCoord2f(1.0f, 0.5f); glVertex3f(5.3, -0.7, 0.5);
                       glTexCoord2f(0.0f, 0.5f); glVertex3f(5.3, -0.7, -0.5);
               glEnd();
       }
       else{
               glBegin(GL_POLYGON);
                       glTexCoord2f(0.0f, 1.0f); glVertex3f(4.9, -0.9, -0.5);
                       glTexCoord2f(1.0f, 1.0f); glVertex3f(4.9, -0.9, 0.5);
                       glTexCoord2f(1.0f, 1.1f); glVertex3f(5.3, -0.7, 0.5);
                       glTexCoord2f(0.0f, 1.1f); glVertex3f(5.3, -0.7, -0.5);
               glEnd();
       }

       if (track%2 ==0){
               glBegin(GL_POLYGON);
                       glTexCoord2f(0.0f, 1.0f); glVertex3f(4.5, -1.0, -0.5);
                       glTexCoord2f(1.0f, 1.0f); glVertex3f(4.5, -1.0, 0.5);
                       glTexCoord2f(1.0f, 1.1f); glVertex3f(4.9, -0.9, 0.5);
                       glTexCoord2f(0.0f, 1.1f); glVertex3f(4.9, -0.9, -0.5);
               glEnd();
       }
       else{
               glBegin(GL_POLYGON);
                       glTexCoord2f(0.0f, 0.0f); glVertex3f(4.5, -1.0, -0.5);
                       glTexCoord2f(1.0f, 0.0f); glVertex3f(4.5, -1.0, 0.5);
                       glTexCoord2f(1.0f, 0.5f); glVertex3f(4.9, -0.9, 0.5);
                       glTexCoord2f(0.0f, 0.5f); glVertex3f(4.9, -0.9, -0.5);
               glEnd();
       }

       //draw tracks running circle in the left side
       if (track%2 ==0){
               glBegin(GL_POLYGON);
                       glTexCoord2f(0.0f, 0.0f); glVertex3f(-4.5, 1.0, -0.5);
                       glTexCoord2f(1.0f, 0.0f); glVertex3f(-4.5, 1.0, 0.5);
                       glTexCoord2f(1.0f, 0.5f); glVertex3f(-5.0, 1.2, 0.5);
                       glTexCoord2f(0.0f, 0.5f); glVertex3f(-5.0, 1.2, -0.5);
               glEnd();
       }
       else{
               glBegin(GL_POLYGON);
                       glTexCoord2f(0.0f, 1.0f); glVertex3f(-4.5, 1.0, -0.5);
                       glTexCoord2f(1.0f, 1.0f); glVertex3f(-4.5, 1.0, 0.5);
                       glTexCoord2f(1.0f, 1.1f); glVertex3f(-5.0, 1.2, 0.5);
                       glTexCoord2f(0.0f, 1.1f); glVertex3f(-5.0, 1.2, -0.5);
               glEnd();
       }

       if (track%2 ==0){
               glBegin(GL_POLYGON);
                       glTexCoord2f(0.0f, 1.0f); glVertex3f(-5.0, 1.2, -0.5);
                       glTexCoord2f(1.0f, 1.0f); glVertex3f(-5.0, 1.2, 0.5);
                       glTexCoord2f(1.0f, 1.1f); glVertex3f(-5.5, 1.3, 0.5);
                       glTexCoord2f(0.0f, 1.1f); glVertex3f(-5.5, 1.3, -0.5);
               glEnd();
       }
       else{
               glBegin(GL_POLYGON);
                       glTexCoord2f(0.0f, 0.0f); glVertex3f(-5.0, 1.2, -0.5);
                       glTexCoord2f(1.0f, 0.0f); glVertex3f(-5.0, 1.2, 0.5);
                       glTexCoord2f(1.0f, 0.5f); glVertex3f(-5.5, 1.3, 0.5);
                       glTexCoord2f(0.0f, 0.5f); glVertex3f(-5.5, 1.3, -0.5);
               glEnd();
       }

       if (track%2 ==0){
               glBegin(GL_POLYGON);
                       glTexCoord2f(0.0f, 0.0f); glVertex3f(-5.5, 1.3, -0.5);
                       glTexCoord2f(1.0f, 0.0f); glVertex3f(-5.5, 1.3, 0.5);
                       glTexCoord2f(1.0f, 0.5f); glVertex3f(-5.9, 1.1, 0.5);
                       glTexCoord2f(0.0f, 0.5f); glVertex3f(-5.9, 1.1, -0.5);
               glEnd();
       }
       else{
               glBegin(GL_POLYGON);
                       glTexCoord2f(0.0f, 1.0f); glVertex3f(-5.5, 1.3, -0.5);
                       glTexCoord2f(1.0f, 1.0f); glVertex3f(-5.5, 1.3, 0.5);
                       glTexCoord2f(1.0f, 1.1f); glVertex3f(-5.9, 1.1, 0.5);
                       glTexCoord2f(0.0f, 1.1f); glVertex3f(-5.9, 1.1, -0.5);
               glEnd();
       }

       if (track%2 ==0){
               glBegin(GL_POLYGON);
                       glTexCoord2f(0.0f, 1.0f); glVertex3f(-5.9, 1.1, -0.5);
                       glTexCoord2f(1.0f, 1.0f); glVertex3f(-5.9, 1.1, 0.5);
                       glTexCoord2f(1.0f, 1.1f); glVertex3f(-6.2, 0.6, 0.5);
                       glTexCoord2f(0.0f, 1.1f); glVertex3f(-6.2, 0.6, -0.5);
               glEnd();
       }
       else{
               glBegin(GL_POLYGON);
                       glTexCoord2f(0.0f, 0.0f); glVertex3f(-5.9, 1.1, -0.5);
                       glTexCoord2f(1.0f, 0.0f); glVertex3f(-5.9, 1.1, 0.5);
                       glTexCoord2f(1.0f, 0.5f); glVertex3f(-6.2, 0.6, 0.5);
                       glTexCoord2f(0.0f, 0.5f); glVertex3f(-6.2, 0.6, -0.5);
               glEnd();
       }

       if (track%2 ==0){
               glBegin(GL_POLYGON);
                       glTexCoord2f(0.0f, 0.0f); glVertex3f(-6.2, 0.6, -0.5);
                       glTexCoord2f(1.0f, 0.0f); glVertex3f(-6.2, 0.6, 0.5);
                       glTexCoord2f(1.0f, 0.5f); glVertex3f(-6.05, 0.1, 0.5);
                       glTexCoord2f(0.0f, 0.5f); glVertex3f(-6.05, 0.1, -0.5);
               glEnd();
       }
       else{
               glBegin(GL_POLYGON);
                       glTexCoord2f(0.0f, 1.0f); glVertex3f(-6.2, 0.6, -0.5);
                       glTexCoord2f(1.0f, 1.0f); glVertex3f(-6.2, 0.6, 0.5);
                       glTexCoord2f(1.0f, 1.1f); glVertex3f(-6.05, 0.1, 0.5);
                       glTexCoord2f(0.0f, 1.1f); glVertex3f(-6.05, 0.1, -0.5);
               glEnd();
       }

       if (track%2 ==0){
               glBegin(GL_POLYGON);
                       glTexCoord2f(0.0f, 1.0f); glVertex3f(-6.05, 0.1, -0.5);
                       glTexCoord2f(1.0f, 1.0f); glVertex3f(-6.05, 0.1, 0.5);
                       glTexCoord2f(1.0f, 1.1f); glVertex3f(-5.7, -0.3, 0.5);
                       glTexCoord2f(0.0f, 1.1f); glVertex3f(-5.7, -0.3, -0.5);
               glEnd();
       }
       else{
               glBegin(GL_POLYGON);
                       glTexCoord2f(0.0f, 0.0f); glVertex3f(-6.05, 0.1, -0.5);
                       glTexCoord2f(1.0f, 0.0f); glVertex3f(-6.05, 0.1, 0.5);
                       glTexCoord2f(1.0f, 0.5f); glVertex3f(-5.7, -0.3, 0.5);
                       glTexCoord2f(0.0f, 0.5f); glVertex3f(-5.7, -0.3, -0.5);
               glEnd();
       }

       if (track%2 ==0){
               glBegin(GL_POLYGON);
                       glTexCoord2f(0.0f, 0.0f); glVertex3f(-5.7, -0.3, -0.5);
                       glTexCoord2f(1.0f, 0.0f); glVertex3f(-5.7, -0.3, 0.5);
                       glTexCoord2f(1.0f, 0.5f); glVertex3f(-5.0, -0.75, 0.5);
                       glTexCoord2f(0.0f, 0.5f); glVertex3f(-5.0, -0.75, -0.5);
               glEnd();
       }
       else{
               glBegin(GL_POLYGON);
                       glTexCoord2f(0.0f, 1.0f); glVertex3f(-5.7, -0.3, -0.5);
                       glTexCoord2f(1.0f, 1.0f); glVertex3f(-5.7, -0.3, 0.5);
                       glTexCoord2f(1.0f, 1.1f); glVertex3f(-5.0, -0.75, 0.5);
                       glTexCoord2f(0.0f, 1.1f); glVertex3f(-5.0, -0.75, -0.5);
               glEnd();
       }

       if (track%2 ==0){
               glBegin(GL_POLYGON);
                       glTexCoord2f(0.0f, 1.0f); glVertex3f(-5.0, -0.75, -0.5);
                       glTexCoord2f(1.0f, 1.0f); glVertex3f(-5.0, -0.75, 0.5);
                       glTexCoord2f(1.0f, 1.1f); glVertex3f(-4.5, -1.0, 0.5);
                       glTexCoord2f(0.0f, 1.1f); glVertex3f(-4.5, -1.0, -0.5);
               glEnd();
       }
       else{
               glBegin(GL_POLYGON);
                       glTexCoord2f(0.0f, 0.0f); glVertex3f(-5.0, -0.75, -0.5);
                       glTexCoord2f(1.0f, 0.0f); glVertex3f(-5.0, -0.75, 0.5);
                       glTexCoord2f(1.0f, 0.5f); glVertex3f(-4.5, -1.0, 0.5);
                       glTexCoord2f(0.0f, 0.5f); glVertex3f(-4.5, -1.0, -0.5);
               glEnd();
       }

       glDisable(GL_TEXTURE_2D);

}


/****************************************************************************************/
/* draw left side wheel                                                                                                                                 */
/****************************************************************************************/
void drawLeftWheel_Thuc(){

       //draw big wheels
       glPushMatrix();
       glTranslatef(0.0, 0.0, 0.5);
       glRotatef(wheel_Left_Angle, 0.0, 0.0, 1.0); // rotate if turn the tank
       glRotatef(180, 0.0, 1.0, 0.0);
       drawBigWheel_Thuc();
       glPopMatrix();

       glPushMatrix();
       glTranslatef(1.3, 0.0, 0.0);
       glRotatef(wheel_Left_Angle, 0.0, 0.0, 1.0); // rotate if turn the tank
       glRotatef(180, 0.0, 1.0, 0.0);
       drawBigWheel_Thuc();
       glPopMatrix();

       glPushMatrix();
       glTranslatef(-1.3, 0.0, 0.0);
       glRotatef(wheel_Left_Angle, 0.0, 0.0, 1.0); // rotate if turn the tank
       glRotatef(180, 0.0, 1.0, 0.0);
       drawBigWheel_Thuc();
       glPopMatrix();

       glPushMatrix();
       glTranslatef(2.6, 0.0, 0.5);
       glRotatef(wheel_Left_Angle, 0.0, 0.0, 1.0); // rotate if turn the tank
       glRotatef(180, 0.0, 1.0, 0.0);
       drawBigWheel_Thuc();
       glPopMatrix();

       glPushMatrix();
       glTranslatef(-2.6, 0.0, 0.5);
       glRotatef(wheel_Left_Angle, 0.0, 0.0, 1.0); // rotate if turn the tank
       glRotatef(180, 0.0, 1.0, 0.0);
       drawBigWheel_Thuc();
       glPopMatrix();

       glPushMatrix();
       glTranslatef(3.9, 0.0, 0.0);
       glRotatef(wheel_Left_Angle, 0.0, 0.0, 1.0); // rotate if turn the tank
       glRotatef(180, 0.0, 1.0, 0.0);
       drawBigWheel_Thuc();
       glPopMatrix();

       glPushMatrix();
       glTranslatef(-3.9, 0.0, 0.0);
       glRotatef(wheel_Left_Angle, 0.0, 0.0, 1.0); // rotate if turn the tank
       glRotatef(180, 0.0, 1.0, 0.0);
       drawBigWheel_Thuc();
       glPopMatrix();

       //draw small wheels
       glPushMatrix();
       glTranslatef(-5.4, 0.5, 0.0);
       glScalef(0.7, 0.7, 0.7);
       glRotatef(wheel_Left_Angle, 0.0, 0.0, 1.0); // rotate if turn the tank
       glRotatef(180, 0.0, 1.0, 0.0);
       drawSmallWheel_Thuc();
       glPopMatrix();

       glPushMatrix();
       glTranslatef(5.6, 0.5, 0.0);
       glScalef(0.8, 0.8, 0.8);
       glRotatef(wheel_Left_Angle, 0.0, 0.0, 1.0); // rotate if turn the tank
       glRotatef(180, 0.0, 1.0, 0.0);
       drawSmallWheel_Thuc();
       glPopMatrix();

//      glColor3f(1.0f, 1.0f, 1.0f);
       glColor3f(0.49f, 0.44f, 0.34f);
       //glColor4f(1.0f, 1.0f, 1.0f, 0.0f);
       glPushMatrix();
       glTranslatef(0.0, 0.0, 0.0);
       drawCaterpillar_Thuc();
       glPopMatrix();


}

/****************************************************************************************/
/* draw right side wheel                                                                                                                                        */
/****************************************************************************************/
void drawRightWheel_Thuc(){

       //draw big wheels
       glPushMatrix();
       glTranslatef(0.0, 0.0, 0.0);
       glRotatef(wheel_Right_Angle, 0.0, 0.0, 1.0); // rotate if turn the tank
       drawBigWheel_Thuc();
       glPopMatrix();

       glPushMatrix();
       glTranslatef(1.3, 0.0, -0.5);
       glRotatef(wheel_Right_Angle, 0.0, 0.0, 1.0); // rotate if turn the tank
       drawBigWheel_Thuc();
       glPopMatrix();

       glPushMatrix();
       glTranslatef(-1.3, 0.0, -0.5);
       glRotatef(wheel_Right_Angle, 0.0, 0.0, 1.0); // rotate if turn the tank
       drawBigWheel_Thuc();
       glPopMatrix();

       glPushMatrix();
       glTranslatef(2.6, 0.0, 0.0);
       glRotatef(wheel_Right_Angle, 0.0, 0.0, 1.0); // rotate if turn the tank
       drawBigWheel_Thuc();
       glPopMatrix();

       glPushMatrix();
       glTranslatef(-2.6, 0.0, 0.0);
       glRotatef(wheel_Right_Angle, 0.0, 0.0, 1.0); // rotate if turn the tank
       drawBigWheel_Thuc();
       glPopMatrix();

       glPushMatrix();
       glTranslatef(3.9, 0.0, -0.5);
       glRotatef(wheel_Right_Angle, 0.0, 0.0, 1.0); // rotate if turn the tank
       drawBigWheel_Thuc();
       glPopMatrix();

       glPushMatrix();
       glTranslatef(-3.9, 0.0, -0.5);
       glRotatef(wheel_Right_Angle, 0.0, 0.0, 1.0); // rotate if turn the tank
       drawBigWheel_Thuc();
       glPopMatrix();

       //draw small wheels
       glPushMatrix();
       glTranslatef(-5.4, 0.5, -0.2);
       glScalef(0.7, 0.7, 0.7);
       glRotatef(wheel_Right_Angle, 0.0, 0.0, 1.0); // rotate if turn the tank
       drawSmallWheel_Thuc();
       glPopMatrix();

       glPushMatrix();
       glTranslatef(5.6, 0.5, -0.2);
       glScalef(0.8, 0.8, 0.8);
       glRotatef(wheel_Right_Angle, 0.0, 0.0, 1.0); // rotate if turn the tank
       drawSmallWheel_Thuc();
       glPopMatrix();

       //glColor3f(0.0f, 0.0f, 0.0f);
       glPushMatrix();
       glTranslatef(0.0, 0.0, 0.0);
       drawCaterpillar_Thuc();
       glPopMatrix();
}


/****************************************************************************************/
/* draw 2 wheels                                                                                                                                                */
/****************************************************************************************/
void drawWheels_Thuc(void){

       //wheel left
       glPushMatrix();
       glTranslatef(0.0, 0.0, 2.0);
       drawLeftWheel_Thuc();
       glPopMatrix();

       //wheel right
       glPushMatrix();
       glTranslatef(0.0, 0.0, -2.0);
       drawRightWheel_Thuc();
       glPopMatrix();
}

/****************************************************************************************/
/* draw Tank                                                                                                                                                    */
/****************************************************************************************/
void drawTank_Thuc(float x, float y, float z)
{
       // set shininess mode
     /*  if(shiny) {
               setPropertyShiny();
       }
*/
	glPushMatrix();
		glTranslatef(x, y, z);
       drawBody_Thuc();

       //for explosion
	  // if(!is_collapse_Thuc){
			
       if (enableCutPlane_Thuc)
       {
		   GLdouble cutPlane[4] = {0, -1, 0, cuttingPos};
       glClipPlane (GL_CLIP_PLANE0, cutPlane);
               glEnable(GL_CLIP_PLANE0);
                       glPushMatrix();
                       glTranslatef(1.0, 0.6, 0.0);
                       glRotatef(turret_Angle, 0.0, 1.0, 0.0); // rotate turret if any action
                       drawTurret_Thuc();
                       glPopMatrix();
               glDisable(GL_CLIP_PLANE0);

       glColor3f(1.0f, 1.0f, 1.0f);
       glEnable(GL_TEXTURE_2D);
       glBindTexture(GL_TEXTURE_2D, tank_texture[6]);

         if (cuttingPos <= 1.0){
               glPushMatrix();
               glTranslatef(1.0, 0.5, 0.0);
               glRotatef(90, 1.0, 0.0, 0.0);
               glScalef(1.0, 0.7, 0.42);
               drawDisc_Thuc(APPLY_TEXTURE);
               glPopMatrix();
         }
       }

       else{
		    if (is_collapse_Thuc){
				GLdouble cutPlane[4] = {0, -1, 0, 1};
				glClipPlane (GL_CLIP_PLANE0, cutPlane);
               glEnable(GL_CLIP_PLANE0);
                       glPushMatrix();
                       glTranslatef(1.0, 0.6, 0.0);
                       glRotatef(turret_Angle, 0.0, 1.0, 0.0); // rotate turret if any action
                       drawTurret_Thuc();
                       glPopMatrix();
               glDisable(GL_CLIP_PLANE0);

				glColor3f(1.0f, 1.0f, 1.0f);
				glEnable(GL_TEXTURE_2D);
				glBindTexture(GL_TEXTURE_2D, tank_texture[6]);

				if (cuttingPos <= 1.0){
					glPushMatrix();
					glTranslatef(1.0, 0.5, 0.0);
					glRotatef(90, 1.0, 0.0, 0.0);
					glScalef(1.0, 0.7, 0.42);
					drawDisc_Thuc(APPLY_TEXTURE);
					glPopMatrix();
				}

			}
			else{
               glPushMatrix();
               glTranslatef(1.0, 0.6, 0.0);
               glRotatef(turret_Angle, 0.0, 1.0, 0.0); // rotate turret if any action
               drawTurret_Thuc();
               glPopMatrix();
			}
       }
	/*   }
	   else{
		   GLdouble cutPlane[4] = {0, -1, 0, 1};
       glClipPlane (GL_CLIP_PLANE0, cutPlane);
               glEnable(GL_CLIP_PLANE0);
                       glPushMatrix();
                       glTranslatef(1.0, 0.6, 0.0);
                       glRotatef(20, 0.0, 1.0, 0.0); // rotate turret if any action
                       drawTurret_Thuc();
                       glPopMatrix();
               glDisable(GL_CLIP_PLANE0);

       glColor3f(1.0f, 1.0f, 1.0f);
       glEnable(GL_TEXTURE_2D);
       glBindTexture(GL_TEXTURE_2D, tank_texture[6]);
	   }
*/
       glPushMatrix();
       glTranslatef(0.3, -2.2, 0.0);
       drawWheels_Thuc();
       glPopMatrix();

	glPopMatrix();

}
