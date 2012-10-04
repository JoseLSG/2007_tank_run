
#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <GL/glut.h>

#include <time.h>
#include <sys/types.h>

#include <windows.h>
#include <mmsystem.h> 

#include "Terrain_Arch.h"
#include "Thuc_Tank.h"
#include "Jose_Tank.h"
#include "thaiTank.h"
#include "tiffTank.h"
#include "globalVar.h"
#include "Explosion.h"

#include "Sphere_Func.h"

#define POWER  20.0
#define GRAVITY 9.8 


float max_pitch =20;	//max 20 degree, lift up the barrel
float min_pitch = 0;

GLuint  texture1[10]; 

using namespace std;


float NEAR_z = 1.0, FAR_z = 100.0;

float _angle = 60.0f;

bool scene_Wired; // wireframe/shaded toggle variable
bool camera;

//Tank movement vectors
float z_Dmotion;

float z_Eye;	// variable to move foward and backwards

// camera view angle variables
float y_Angle;
float x_Angle;
float person_y_angle;

int pH;	// perspective height variable
int pW;	// perspective width variable
float oH;	// ortho height variable
float oW;	// ortho width variable

/**********************************TIFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF**************************************/
float tiffx = -50.0;
float tiffy = 2.5;
float tiffz = 0.0;

float tiffAngle = 0.0;

float timer=10000.0;

int timeCount =0;

/*****************************Josey*******************************/
/*float josex = 200.0;
float josey = -1.5;
float josez = 50.0;*/

float josex = -30.0;
float josey = -1.5;
float josez = 30.0;


float joseAngle = 0.0;

/*****************************Thuc*******************************/
float thucx = 0.0;
float thucy = 2.5;
float thucz = -60.0;

float thucAngle = 0.0;

bool tank_thuc_appear = true;
bool tank_tiff_appear = true;
bool tank_jose_appear = true;

//bool gameStart = false;
/**********************************TIFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF**************************************/


//--------------Data variables for Movement and collision-------------//

int USER = 0;

GLfloat Object_Collection[4][4] = 
{	
	{0.0,0.0,0.0,2.0}, // Main tank
	{thucx,thucy,thucz,9.0}, // thuc tank
	{josex,josey, josez,8.0}, // Jose Tank
	{tiffx, tiffy, tiffz, 8.0}	// Tiff Tank
};


GLfloat save_x_pos, save_z_pos;
float HeightMap_Y_Level = 296;
float Ground = -(HeightMap_Y_Level-293);

//--------------Ballistics -------------//

float angle_H;
float angle_V;
bool Fire;

GLfloat Bullet[2][4]= 
{
	{4.0 , 4.0, 0.0, 1.0}, // initial position
	{4.0, 0.0, 0.0, 1.0}  // final position
};

//--------------Ballistics END-------------//


GLfloat ObjectHits[4]= {0,	//	null
						0,	//	Thuc hit 
						0,	//	Jose hit
						0};	//	Tiff hit


//-------------- Functions for Movement and collision-------------//


void print_Menu(void)
{
	printf("This is a menu. Please consult the following for in-game functions.\n");
	printf("Press t/8 to move the tank forward.\n");
	printf("Press T/2 to move the tank backward.\n");
	printf("Press w/W to toggle between wired or solid frame. \n");
	printf("Press f/b to move the camera forward/backward.\n");
	printf("Press o/p to toggle between orthographic/perspective view.\n");
	printf("Press k/K to rotate the tank's machine gun clock/counter clock wise\n");
	printf("Press r/6 to rotate the tank clockwise.\n");
	printf("Press R/4 to rotate the tank counter clock wise.\n");
	printf("Press q/7 to pitch the tank barrel up/down.\n");
	printf("Press Q/9 to pitch the tank barrel up/down.\n");
	printf("Press y/1 to yaw the tank machine turret clockwise\n");
	printf("Press y/3 to yaw the tank machine turret counter-clockwise\n");
	printf("Press a/5 to fire.\n");
	printf("Press s/S to toggle collision spheres view on/off.\n");
	printf("Press m to display the menu again\n"); //This need to be work on//COMMENTED BY TIFF 1207
	printf("Press M to display the \"about\" of the menu.\n");//This need to be work on//COMMENTED BY TIFF 1207


	printf("******************************************************************\n");
	printf("*This game is implemented by:                                    *\n");
	printf("*NHUT THAI LE                                                    *\n"); 
	printf("*JOSE LUIS SALDIVAR                                              *\n"); 
	printf("*NGO ANH THUC                                                    *\n"); 
	printf("*YI-CHING LU                                                     *\n"); 
	printf("*Ressources:                                                     *\n");
	printf("*http://www.videotutorialsrock.com/                              *\n");
	printf("*http://nehe.gamedev.net/                                        *\n\n");

	printf("*Special Thanks to:                                              *\n");
	printf("*Our professor, Dr. Adam Krzyzak.                                *\n");
	printf("*Our tutors, Lukasz Jelen and Rokita Joanne                      *\n");

}


float relPos(float P2, float P1)
{
	return P2 - P1;
}

bool Sphere_Collision(GLfloat ObjB[][4], GLfloat ObjA[][4], int B, int A )
{
	float rel_x = relPos(ObjB[B][0],ObjA[A][0]);
	float rel_y = relPos(ObjB[B][1],ObjA[A][1]);
	float rel_z = relPos(ObjB[B][2],ObjA[A][2]);

	float dist_sqr =  rel_x * rel_x + rel_y * rel_y + rel_z* rel_z;

	float r_dist_sqr = ObjB[B][3]+ ObjA[A][3];

	return dist_sqr <= r_dist_sqr * r_dist_sqr;
}

bool Collision_Tests(GLfloat Objects[][4], GLfloat MainObjs[][4], int main_obj)
{

	return Sphere_Collision(Objects, MainObjs, 1, main_obj) || Sphere_Collision(Objects, MainObjs, 2, main_obj)
		|| Sphere_Collision(Objects, MainObjs, 3, main_obj) ;
}





//***************new collision tests for bullet  



// Overloaded collision function, only one more parameter for array of Hits

bool Collision_Tests(GLfloat Objects[][4], GLfloat MainObjs[][4], int main_obj, GLfloat ObjHits[])
{	

	bool collide_1;
	bool collide_2;
	bool collide_3;

	collide_1 = Sphere_Collision(Objects, MainObjs, 1, main_obj);
	collide_2 = Sphere_Collision(Objects, MainObjs, 2, main_obj);
	collide_3 = Sphere_Collision(Objects, MainObjs, 3, main_obj);

	if( collide_1 ) // Thuc tank vs main Obj
			ObjHits[1] = 1;

	if( collide_2 ) // Jose tank vs main Obj
			ObjHits[2] = 1;

	if( collide_3 ) // Tiff tank vs main Obj
			ObjHits[3] = 1;

		return collide_1 || collide_2 || collide_3 ;

} 

// 0 means not hit, 
// 1 means there is a Hit

//in Thuc's Explosion algorithm checks ObjectHits array to see which one was hit and enable animation for that object
// AND MAYBE reset it's hit flag from 1 back to 0 so future hit animations are not enabled... but might mean that if user tank shoots again same object
// then it's animation restarts



//*********************colli for bullet end





float DegToRad(float degrees)
{
	return degrees*(PI/180.0);
}

//-----------------------------------------------------------------------//
void drawAxis()
{
        glPushAttrib(GL_ALL_ATTRIB_BITS);        // Pushes attributes like current color to attribute stack.
        glBegin(GL_LINES);

               // X axis red
     glColor3f(1, 0, 0);
     glVertex3f(0, 0, 0);
     glVertex3f(10, 0, 0);

               // Y axis green
     glColor3f(0, 1, 0);
     glVertex3f(0, 0, 0);
     glVertex3f(0, 10, 0);

               // Z axis blue
     glColor3f(0, 0, 1);
     glVertex3f(0, 0, 0);
     glVertex3f(0, 0, 10);
         glEnd();
         glPopAttrib();
                                              
}

//--------------Ballistics -------------//

	// Bullet_Ballistics_FDest needs initial velocity, gravitational acceleration, angles of fire, and a array of 2 coord. sets. ( set1: initial pos, set2: final pos (hit coord.) )
void Bullet_Ballistics_FDest(float vi, float g, float V_angle, float H_angle, GLfloat Pos [][4])
{
	//re-initialize final destination with initial position
		Pos[1][0] = Pos[0][0]; 
		Pos[1][1] = Pos[0][1];
		Pos[1][2] = Pos[0][2];
	
	bool noCol = true;
	for(float t = 0.01; (Pos[1][1] > Ground) && noCol; t+= 0.01 )
	{
		
		Pos[1][0] += vi * cos(DegToRad(V_angle)) * cos(-DegToRad(H_angle)) * t;		// new X bullet position
		Pos[1][1] += vi * sin(-DegToRad(V_angle)) * t - g * t * t / 2;				// new Y bullet position
		Pos[1][2] += vi * cos(DegToRad(V_angle)) * sin(-DegToRad(H_angle)) * t;		// new Z bullet position

		
		noCol = !Collision_Tests(Object_Collection, Pos, 1, ObjectHits) ;
	}

	return;
}

//--------------Ballistics END-------------//


void initRendering() {
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_COLOR_MATERIAL);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glEnable(GL_NORMALIZE);
	glShadeModel(GL_SMOOTH);

	y_Angle = 0.0;
	x_Angle = 0.0;
	person_y_angle = 0.0;

	pH = 1.0;
	pW = 1.0;

	oH = 10.5;
	oW = 10.5;

	z_Eye = 15.0;

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(90, pW/pH, 1.0, 2000.0); // (fovy, aspect ratio, near, far)
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glClearColor(0.0274, 0.4745, 0.9764, 0.0);
	scene_Wired = false;
	camera = true;

	/*	LOOK FROM BEHIND THE ORIGIN*/
	phi_camera = PI/2.5;
	theta_camera = -PI/4;///0.0* (PI/180);
	rad_camera = 30.0;

	text_Creation();

	 initTexture_Thuc();
	 loadTexture_thai();

	 print_Menu();

//--------------Ballistics -------------//
	angle_H = 0.0;
	angle_V = 0.0;
	Fire = false;
//--------------Ballistics END-------------//

}

void handleResize(int w, int h) {
	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(90, pW/pH, 1.0, 2000.0);
}

void Explosion_Mech(void)
{

	
		glEnable(GL_BLEND);							// Enable Blending
		glBlendFunc(GL_SRC_ALPHA, GL_ONE);
		glDepthMask(GL_FALSE);							// Disable Depth Buffer Writes
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, texture[3]);
		//glEnable(GL_DEPTH_TEST);
		
if (do_explosion){


		//Thuc tank
		if (cuttingPos <= 1.0){
			cuttingPos = 1.0;
			//is_collapse_Thuc = true;
		}
		else {
			cuttingPos -= 0.1;
		}

		if (gun_fall_angle >= 30){
			gun_fall_angle = 30;
		}
		else{
			gun_fall_angle ++;
		}


		//Jose tank
		if (cuttingPos_Jose <= 1.0){
			cuttingPos_Jose = 1.0;
			//is_collapse_Thuc = true;
		}
		else {
			cuttingPos_Jose -= 0.1;
		}


		// Tiff tank
		if (cuttingPos_Tiff <= 1.0){
			cuttingPos_Tiff = 1.0;
			//is_collapse_Thuc = true;
		}
		else {
			cuttingPos_Tiff -= 0.1;
		}

		if (SparkObject._Alpha > 2)
			SparkObject.reach_max_alpha = true;
		if (SparkObject._Alpha < -1){
			do_explosion = false;		
		}

		if (SparkObject._Alpha < 0.5){
				do_smoke = true;
		}
				
		if (SparkObject.reach_max_alpha)
			SparkObject._Alpha -= 0.05f;			// Update Alpha
		else 
			SparkObject._Alpha += 0.03f;			// Update Alpha
		
		SparkObject._Scale += 0.1f;//0.03			// Update Scale

		glColor4f(1,1,1, SparkObject._Alpha);		// Scale
		
		glPushMatrix();
		glTranslatef((float)SparkObject._PositionX,
					(float)SparkObject._PositionY,
					(float)SparkObject._PositionZ);
		glScalef(SparkObject._Scale,SparkObject._Scale,SparkObject._Scale);
	
		drawSquadSpark();
		glPopMatrix();

		glPushMatrix();
		glColor3f(0.0, 0.0, 1.0);
		glTranslatef(10, 10, 10);

	glPopMatrix();
	} 
	
		glDepthMask(GL_TRUE);
		glDisable(GL_BLEND);
		glDisable(GL_TEXTURE_2D);
		

//for smoke

	glEnable(GL_BLEND);							// Enable Blending
	glBlendFunc(GL_SRC_ALPHA, GL_ONE);
	glDepthMask(GL_FALSE);							// Disable Depth Buffer Writes
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, texture[4]);

if (do_smoke){

	if (sparkTime > 200) {
		sparkTime = 100;

		if (is_collapse_Thuc)
			tank_thuc_appear = false;

		if (is_collapse_Tiff)
			tank_tiff_appear = false;

		if (is_collapse_Jose)
			tank_jose_appear = false;
	}

	if (sparkTime < 20){
		drawOneSmoke(0, SparkObject._PositionX, SparkObject._PositionY-3, SparkObject._PositionZ);
	}
	else if (sparkTime < 40){
		drawOneSmoke(0, SparkObject._PositionX, SparkObject._PositionY-3, SparkObject._PositionZ);
		drawOneSmoke(1, SparkObject._PositionX, SparkObject._PositionY-3, SparkObject._PositionZ);
	}
	else if (sparkTime < 60){
		drawOneSmoke(0, SparkObject._PositionX, SparkObject._PositionY-3, SparkObject._PositionZ);
		drawOneSmoke(1, SparkObject._PositionX, SparkObject._PositionY-3, SparkObject._PositionZ);
		drawOneSmoke(2, SparkObject._PositionX, SparkObject._PositionY-3, SparkObject._PositionZ);
	}
	else if (sparkTime < 80){
		drawOneSmoke(0, SparkObject._PositionX, SparkObject._PositionY-3, SparkObject._PositionZ);
		drawOneSmoke(1, SparkObject._PositionX, SparkObject._PositionY-3, SparkObject._PositionZ);
		drawOneSmoke(2, SparkObject._PositionX, SparkObject._PositionY-3, SparkObject._PositionZ);
		drawOneSmoke(3, SparkObject._PositionX, SparkObject._PositionY-3, SparkObject._PositionZ);
	}
	else {
		drawOneSmoke(0, SparkObject._PositionX, SparkObject._PositionY-3, SparkObject._PositionZ);
		drawOneSmoke(1, SparkObject._PositionX, SparkObject._PositionY-3, SparkObject._PositionZ);
		drawOneSmoke(2, SparkObject._PositionX, SparkObject._PositionY-3, SparkObject._PositionZ);
		drawOneSmoke(3, SparkObject._PositionX, SparkObject._PositionY-3, SparkObject._PositionZ);
		drawOneSmoke(4, SparkObject._PositionX, SparkObject._PositionY-3, SparkObject._PositionZ);
	}
	

	sparkTime ++;
}
		glDepthMask(GL_TRUE);
		glDisable(GL_BLEND);
		glDisable(GL_TEXTURE_2D);


		if (fuel > 0){			
			explosionChange();
			particleExplosion();			
		}

		if (sound){
			PlaySound("sound/Explode.wav",NULL,SND_FILENAME|SND_ASYNC);
			sound = false;
		}


		//for sky
       glColor3f(1.0f, 1.0f, 1.0f);
       glEnable(GL_TEXTURE_2D);
       glBindTexture(GL_TEXTURE_2D, texture[6]);
       glPushMatrix();
       glRotatef(90, 1, 0, 0);       
	   drawSphere(700, 50, 50);
       glPopMatrix();


       glEnable(GL_BLEND);                                                        // Enable Blending
       glBlendFunc(GL_SRC_ALPHA, GL_ONE);
       glDepthMask(GL_FALSE);                                                        // Disable Depth Buffer Writes

       glColor3f(1.0f, 1.0f, 1.0f);
       glEnable(GL_TEXTURE_2D);
       glBindTexture(GL_TEXTURE_2D, texture[5]);
       glPushMatrix();
       glRotatef (cloud_angle, 0, 1, 0);
       glRotatef(90, 1, 0, 0);
       drawSphere(690, 50, 50);
       glPopMatrix();

       glDepthMask(GL_TRUE);
       glDisable(GL_BLEND);
       glDisable(GL_TEXTURE_2D);

       cloud_angle += 0.1;
 

}
/****************************************TIFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF************************************/
//Called every 1000 milliseconds -> 1 sec
void update(int value) {
	timeCount++;

	if (tank_thuc_appear){
		 if (thucx <=150.0 && thucz <= 150.0){

			if (timeCount%5 ==0){
				if (thucAngle <=330){
				thucAngle = thucAngle + 30;
				}
				else
				{
				thucAngle = thucAngle -30;
				
				}
			 }

		}
		else if (thucx==151 || thucz==151)
		{
			thucAngle += 15;
		}
		else if (thucx >= -150.0 && thucz >= -150.0){

			if (timeCount%5 ==0){
			if (thucAngle >=-330){
				thucAngle = thucAngle - 30;
			}
			else{
			thucAngle = thucAngle +30;
			}
			}
			thucx = -(thucx-30);
			thucz = -(thucz+30);
		}
	 Object_Collection[1][0] = thucx;
	 Object_Collection[1][2] = thucz;
	}
	/****************************************TIFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF************************************/
	if(tank_tiff_appear ){
	 if ((timeCount%2) ==0)
	 {
		 
		 if (tiffx <=150.0 && tiffz <= 150.0){
			if (tiffAngle <=330){
				tiffAngle = tiffAngle + 30;
			}
			else
			{
				tiffAngle = tiffAngle -30;
			}
			tiffx = -(tiffx)+50;
			tiffz = -(tiffz+40);
		}
		else if (tiffx >= -150.0 && tiffz >= -150.0){
			if (tiffAngle >=-330){
				tiffAngle = tiffAngle - 30;
			}
			else{
			tiffAngle = tiffAngle +30;
			}
			tiffx = -(tiffx)-50;
			tiffz = (tiffz+50);
		}
	 Object_Collection[3][0] = tiffx;
	 Object_Collection[3][2] = tiffz;
	
	 }
	}
	 	/****************************************TIFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF************************************/
	if (tank_jose_appear){
	 if ((timeCount%3) ==0)
	 {
		 if (josex <=140.0 && josez <= 140.0){
			if (joseAngle <=330){
				joseAngle = joseAngle + 30;
			}
			else
			{
				joseAngle = joseAngle -30;

			}
			josex = (josex+5);
			josez = (-josez+10);
		}
		else if (josex >= -140.0 && josez >= -140.0){
			if (joseAngle >=-330){
				joseAngle = joseAngle - 30;
			}
			else{
			joseAngle = joseAngle +30;

			}
			josex = (josex-10);
			josez = (-josez-10);
		}
	 Object_Collection[2][0] = josex;
	 Object_Collection[2][2] = josez;
		 
	
	 }
	}
	glutPostRedisplay();
	glutTimerFunc(timer, update, 0);
}



/****************************************TIFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF************************************/

void drawScene() {

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	if( Collision_Tests(Object_Collection, tankSet, USER) )
	{
		tankSet[USER][0] =  save_x_pos;
		tankSet[USER][2] =  save_z_pos;
		tankSet[USER+1][0] =  save_x_pos;
		tankSet[USER+1][2] =  save_z_pos;

		tank_x =  save_x_pos;
		tank_z =  save_z_pos;
	}


	eye_X = rad_camera * sin(phi_camera) * sin(theta_camera);
	eye_Y = rad_camera * cos(phi_camera);
	eye_Z = rad_camera * sin(phi_camera) * cos(theta_camera);
	view_up_X = rad_camera * sin(phi_camera -90) * sin(theta_camera);
	view_up_Y = rad_camera * cos(phi_camera -90);
	view_up_Z = rad_camera * sin(phi_camera -90) * cos(theta_camera);

	if(viewMode == 3)	
		gluLookAt(eye_X+tank_x, eye_Y+tank_y, eye_Z+tank_z, look_X+tank_x, look_Y+tank_y, look_Z+tank_z, view_up_X, view_up_Y, view_up_Z);
	else
	{
		glLoadIdentity();
		gluLookAt(tank_x,15+tank_y,tank_z, 100*cos(yaw*PI/180),0,-100*sin(yaw*PI/180), 0,1,0);
	}


	GLfloat ambientColor[] = {0.4f, 0.4f, 0.4f, 1.0f};
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, ambientColor);
	
	GLfloat lightColor0[] = {0.6f, 0.6f, 0.6f, 1.0f};
	GLfloat lightPos0[] = {-0.5f, 0.8f, 0.1f, 0.0f};
	glLightfv(GL_LIGHT0, GL_DIFFUSE, lightColor0);
	glLightfv(GL_LIGHT0, GL_POSITION, lightPos0);
	drawAxis();
	
	glPushMatrix();
	glTranslatef(0,296, 0);
	Terrain_func_1();
	glPopMatrix();
   
	if (tank_thuc_appear){
	   glPushMatrix();
		glTranslatef(Object_Collection[1][0], Object_Collection[1][1], Object_Collection[1][2]);
		glRotatef(thucAngle, 0.0,1.0 , 0.0);
		glPushMatrix();
			glScalef(2,2,2);
			drawTank_Thuc(0, 0, 0);
		glPopMatrix();
		if(Spheres)
			glutSolidSphere(Object_Collection[1][3],10,10);
 	   glPopMatrix();
	}

	if (tank_jose_appear){
		glPushMatrix();
		glTranslatef(Object_Collection[2][0], Object_Collection[2][1], Object_Collection[2][2]);

		/****************************************TIFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF************************************/
		glRotatef(joseAngle, 0.0,1.0 , 0.0);
		if(Spheres)
			glutSolidSphere(Object_Collection[2][3],10,10);
		drawTank_Jose();
		glPopMatrix();
	}

	if (tank_tiff_appear){
		glPushMatrix();
		glTranslatef(Object_Collection[3][0], Object_Collection[3][1], Object_Collection[3][2]);
		/****************************************TIFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF************************************/
		glRotatef(tiffAngle, 0.0,1.0 , 0.0);
		drawTank_Tiff(0,0,0);
		glTranslatef(0, 0, -5.5);

		if(Spheres)
			glutSolidSphere(Object_Collection[3][3],10,10);

		glPopMatrix();
	}

	glPushMatrix();
		drawTank_thai();
	glPopMatrix();

	glPushMatrix();
		drawMountain_thai( bottomSet,topSet);
	glPopMatrix();

	Explosion_Mech();

	glutSwapBuffers();

}


void KeyEvent(unsigned char key, int x, int y)
{


	switch (key){

	case 'g':
	case 'G':
		gameStart=true;
		break;

		case 's':
				Spheres = true;
				break;
		case 'S':
				Spheres = false;
				break;

		case 'w':
		case 'W':
			if(scene_Wired == false)
			{
				scene_Wired = true; // sets flag to wireframe mode
				glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); // Displays scene objects in wiredframe mode
			}

			else
			{
				scene_Wired = false; // sets flag back to non-wireframe mode
				glPolygonMode(GL_FRONT_AND_BACK, GL_FILL); // Displays objects filled
			}

			break;

		case 'o':
		case 'O':
				glMatrixMode(GL_PROJECTION);
				glLoadIdentity();
				glOrtho(-oW, oW, -oH, oH, NEAR_z, FAR_z);

			break; 

		case 'p':
		case 'P':
				glMatrixMode(GL_PROJECTION);
				glLoadIdentity();
				gluPerspective(90, pW/pH, 1.0, 2000.0);
			break;
		
		case 'f':
				rad_camera -= 2.5; // move forward on the Z axis
			break;
		
		case 'b':
				rad_camera += 2.5; // move backwards on the Z axis
			break;


		case 'k':
			roll -= 30;
			break;
		case 'K':
			roll += 30;
			break;
		case '6':
		case 'r':
			turn -= 2;
			angle_H -= 2;
			if(jump > 0)
			{	jump += -0.5;}
			else 
			{	jump = 0.5;}
			break;
		case '4':
		case 'R':
			turn += 2;
			angle_H += 2;
			if(jump < 1)
			{	jump += 0.5;}
			else
			{	jump = 0.5;}
			break;
		case '2':
		case 'T':
			tank_x -= cos(-turn*PI/180)*0.5;
			tank_z -= sin(-turn*PI/180)*0.5;

		//Save of old position
			save_x_pos = tankSet[0][0];
			save_z_pos = tankSet[0][2] ;
			
			tankSet[0][0] = tank_x;
			tankSet[0][1] = tank_y;
			tankSet[0][2] = tank_z;
			tankSet[1][0] = tank_x;
			tankSet[1][1] = tank_y;
			tankSet[1][2] = tank_z;

			Bullet[0][0] = tankSet[0][0];
			Bullet[0][2] = tankSet[0][2];
			
			if(accumMove)
				sumMove = sumMove - 0.5;
			else
				sumMove = 0;
			if(jump > 0)	//redraw the caterpilar at different position
			{	jump += -0.5;}
			else 
			{	jump = 0.5;}
			break;
		case '8':
		case 't':
			tank_x += cos(-turn*PI/180)*0.5;
			tank_z += sin(-turn*PI/180)*0.5;

					//Save of old position
			save_x_pos = tankSet[0][0];
			save_z_pos = tankSet[0][2] ;

			tankSet[0][0] = tank_x;
			tankSet[0][1] = tank_y;
			tankSet[0][2] = tank_z;
			tankSet[1][0] = tank_x;
			tankSet[1][1] = tank_y;
			tankSet[1][2] = tank_z;

			Bullet[0][0] = tankSet[0][0];
			Bullet[0][2] = tankSet[0][2];

			if(accumMove)
				sumMove = sumMove + 0.5;
			else
				sumMove = 0;
			if(jump < 1)
			{	jump += 0.5;}
			else
			{	jump = 0.5;}
			break;
		case '7':
		case 'Q':
			if(pitch < max_pitch)
				pitch += 2 ;
			break;
		case '9':
		case 'q':
			if(pitch > min_pitch)
				pitch -= 2;
			break;
		case '1':
		case 'y':
			yaw -= 5;
			angle_H -= 5;
			break;
		case '3':
		case 'Y':
			yaw += 5;
			angle_H += 5;
			break;


		case '5':
		case 'a': //Fire Cano
				Bullet_Ballistics_FDest( POWER, GRAVITY, -pitch, angle_H, Bullet );
				Fire = true;

				
				initSpark(Bullet[1][0], Bullet[1][1], Bullet[1][2]);
				initSmoke(Bullet[1][0], Bullet[1][1], Bullet[1][2]);
				newSpark ();
				sparkTime = 0;
			
				//Thuc tank is shot
				if(ObjectHits[1] == 1){
					cuttingPos = 3.5;
					enableCutPlane_Thuc = true;	
					is_collapse_Thuc = true;
					ObjectHits[1] = 0;
				}				
				else{				
					cuttingPos = 1;
					gun_fall_angle = 30;					
					enableCutPlane_Thuc = false;
					
				}

				//Jose tank is shot
				if(ObjectHits[2] == 1){
					cuttingPos_Jose = 3.5;
					enableCutPlane_Jose = true;	
					is_collapse_Jose = true;
					ObjectHits[2] = 0;
				}				
				else{				
					cuttingPos_Jose = 1;
					gun_fall_angle_Jose = 30;					
					enableCutPlane_Jose = false;
					
				}

				//Tiff tank is shot
				if(ObjectHits[3] == 1){
					cuttingPos_Tiff = 3.5;
					enableCutPlane_Tiff = true;	
					is_collapse_Tiff = true;
					ObjectHits[3] = 0;
				}				
				else{				
					cuttingPos_Tiff = 1;
					gun_fall_angle_Tiff = 30;					
					enableCutPlane_Tiff = false;
					
				}
			
				sound = true;					  
							
				do_explosion = true;

			break;
//--------------Ballistics END-------------//
		case 'm':
			//TO DISPLAY THE MENU AGAIN //COMMENTED BY TIFF1207
			break;
		case 'M':
			//TO DISPLAY THE About//COMMENTED BY TIFF1207
			break;
		case 27:
				exit(1);

	}


	glutPostRedisplay(); // Redisplays the screen
}


void SpecialKeysEvent(int key, int x, int y)
{
	switch (key){

			case GLUT_KEY_LEFT:
				theta_camera -= 5.0 * (PI/180);
				break;

			case GLUT_KEY_RIGHT:
				theta_camera += 5.0 * (PI/180);
				break;

			case GLUT_KEY_UP:
				phi_camera -= 5.0 * (PI/180);
				break;

			case GLUT_KEY_DOWN:
				phi_camera += 5.0 * (PI/180);
				break;

			case GLUT_KEY_F4:
					glossy = glossy ^ true;
				break;
	}

	glutPostRedisplay();
}


void WindowResize(int w, int h)
{
	glViewport(0, 0, w, h);
						// Set the viewport to current window size.
	pW = w;
	pH = h;

	glMatrixMode(GL_PROJECTION); 
	glLoadIdentity();

	gluPerspective(90, pW/pH, 1.0, 2000.0);

	
	glutPostRedisplay();
}


int main(int argc, char** argv) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(900, 700);
	/*******************TIffffff****************************/
	glutTimerFunc(timer, update, 0);
	/*******************TIffffff****************************/
	glutCreateWindow("Tank_Game");
	initRendering();

	func_set_2();


	glutDisplayFunc(drawScene);
	glutKeyboardFunc(KeyEvent);
	glutReshapeFunc(handleResize);
	glutSpecialFunc(SpecialKeysEvent);
	glutIdleFunc(drawScene);
	
	glutMainLoop();
	return 0;
}









