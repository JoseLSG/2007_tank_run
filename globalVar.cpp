
#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <GL/glut.h>

float PI =3.141592654;

//MOVEMENT OF THE TANK COMPONENT	
float pitch=0;
float yaw=0;		//how many degrees, rotate the turret
float roll=0;		//how many degree, the machine gun rotate
float jump = 0;		//how far each caterpilar of the track shifted everytime the tank move (unit)
				
//LIGHT
	bool beam = true;		//lowbeam = true, high beam = false
	bool disableLight=true;	//enable/disable light feature
	bool allLights = false;	//on/off 3 lights 
	bool glossy = false;
	bool light1 = false;

//color of light and fog
	GLfloat globalAmbient[] = { 0.5, 0.5, 0.5, 1.0};
	GLfloat specref[]	= {1.0f, 1.0f, 1.0f, 1.0f};		//white
	GLfloat black[]	= {0.0 , 0.0 , 0.0 , 1.0};		//black
	GLfloat gray[]		= {0.5 , 0.5 , 0.5 , 1.0};		//gray
	GLfloat white[4]	= {1.0 , 1.0 , 1.0 , 0.5};		//spotlight color: white
	GLfloat smoke[4]	= {0.3 , 0.3 , 0.3 , 0.5};		//spotlight color: smoke
	GLfloat blue[4]	= {0.0 , 0.0 , 1.0 , 0.5};		//spotlight color, blue
	GLfloat sky[4]		= {0.0 , 0.0 , 0.3 , 0.5};		//spotlight color, sky
	GLfloat red[4]		= {1.0 , 0.0 , 0.0 , 0.5};		//spotlight color, red
	GLfloat pink[4]	= {0.2 , 0.0 , 0.0 , 0.5};		//spotlight color, pink
	GLfloat rock[]		= {0.5 , 0.5 , 0.2 , 0.0};

// Effects
	#define MODE_FLAT 1
	#define MODE_SMOOTH  2
	#define MODE_VERYLOW 3
	#define MODE_MEDIUM  4
	#define MODE_VERYHIGH 5
	int iShade = MODE_SMOOTH;
	int iTess = MODE_MEDIUM;
	
//current position of main tank 
	float tank_x=0;
	float tank_y=0;
	float tank_z=0;
	float disp_y=0; //displacement of tank on the y-axis caused by going up and down hill
	float beta;
//HILL COLLISION
	bool accumMove = false;		//accumulate the move distance
	bool goDown = false;		//true since the tank touch the top of the hill


	float turn = 0;			//how many degree to turn the tank
	float sumMove = 0;		//accumulate some number of move

//TANK LIST
	GLfloat tankSet[2][4]=
	{
		{0, 0, 0, 10},
		{0, 0, 0, 1}
	};

//EYE PARAMETERS
	float eye_X=0;		 
	float eye_Y=0;
	float eye_Z=0;
	float look_X = 0.0;
	float look_Y = 0.0;
	float look_Z = 0.0;
	float view_up_X = 0.0;
	float view_up_Y = 1.0;
	float view_up_Z = 0.0;
//CAMERA PARAMETERS, CONTROL THE MOVE OF THE CAMERA
	double theta_camera=0;		// Polar coordinates for the camera.
	double phi_camera=0;
	double rad_camera=10;			//camera rotate on the sphere whose center is at the object
	int viewMode = 3;		//default is 3rd person view

	//for sound
	bool sound = false;

	//for sky
	float cloud_angle = 0;


	// Sphere display
	float Spheres = false;

	bool gameStart= false;

	bool is_collapse_Thuc = false;
	bool is_collapse_Tiff = false;
	bool is_collapse_Jose = false;