
/****************************************************************************************/
/* 
/* Name: Nhut Thai LE 
/* ID : 4655346
/* Term project - 2007/12/04
/****************************************************************************************/

#ifndef __GLOBALVAR_H
#define __GLOBALVAR_H

extern float PI;
//MOVEMENT OF THE TANK COMPONENT	
extern float pitch;
extern float yaw;		//how many degrees, rotate the turret
extern float roll;		//how many degree, the machine gun rotate
extern float jump ;		//how far each caterpilar of the track shifted everytime the tank move (unit)
				
//LIGHT
	extern bool beam;		//lowbeam = true, high beam = false
	extern bool disableLight;	//enable/disable light feature
	extern bool allLights ;	//on/off 3 lights 
	extern bool glossy ;
	extern bool light1 ;
	extern bool light2 ;
	extern bool light3 ;

//color of light and fog
	extern GLfloat  globalAmbient[] ;
	extern GLfloat  specref[]	;		//white
	extern GLfloat  black[]	;		//black
	extern GLfloat  gray[]		;		//gray
	extern GLfloat  white[4]	;		//spotlight color: white
	extern GLfloat  smoke[4]	;		//spotlight color: smoke
	extern GLfloat	 blue[4]	;		//spotlight color, sky
	extern GLfloat	 red[4]		;		//spotlight color, red
	extern GLfloat	 pink[4]	;		//spotlight color, pink

	extern GLfloat  rock[]		;


// Effects
	#define MODE_FLAT 1
	#define MODE_SMOOTH  2
	#define MODE_VERYLOW 3
	#define MODE_MEDIUM  4
	#define MODE_VERYHIGH 5
	extern int iShade;
	extern	int iTess;
	
//current position of main tank 
	extern float tank_x;
	extern float tank_y;
	extern float tank_z;
	extern float disp_y; //displacement of tank on the y-axis caused by going up and down hill
	extern float beta;

//HILL COLLISION
	extern bool accumMove;		//accumulate the move distance
	extern bool goDown;		//true since the tank touch the top of the hill


	extern float turn;			//how many degree to turn the tank
	extern float sumMove;		//accumulate some number of move

//TANK LIST
	extern GLfloat tankSet[2][4];
	extern GLfloat bottomSet[2][4];
	extern GLfloat topSet[1][4];
 
//EYE PARAMETERS
	extern	float eye_X;		 
	extern	float eye_Y;
	extern	float eye_Z;
	extern	float look_X;
	extern	float look_Y;
	extern	float look_Z;
	extern	float view_up_X;
	extern	float view_up_Y;
	extern	float view_up_Z;
//CAMERA PARAMETERS, CONTROL THE MOVE OF THE CAMERA
	extern	double theta_camera;		// Polar coordinates for the camera.
	extern	double phi_camera;
	extern	double rad_camera;			//camera rotate on the sphere whose center is at the object
	extern int viewMode;		//default is 3rd person view

//for sky
	extern float cloud_angle;

// Sphere display
	extern float Spheres;
	extern bool gameStart;
	extern bool is_collapse_Thuc;
	extern bool is_collapse_Tiff;
	extern bool is_collapse_Jose;

#endif