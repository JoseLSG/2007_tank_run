
#include <GL/glut.h>	// This is assuming that you have glut.h set in your include path.
#include <stdio.h>		// Other necessary program related includes.
#include <math.h>
#include <iostream>

using namespace std;


/**************************************************************************************/
float relPosMt(float P2, float P1)
{
	return P2 - P1;
}
/**************************************************************************************/
bool isInside(GLfloat staticList[][4], int staticObject, GLfloat dynamicList[][4], int dynamicObject)
{	//check if the given moving object in the dynamicList collide with any static object in the staticList
	float rel_x = relPosMt(staticList[staticObject][0],dynamicList[dynamicObject][0]); //calculate x-distance between 2 object
	float rel_y = relPosMt(staticList[staticObject][1],dynamicList[dynamicObject][1]); //y-distance
	float rel_z = relPosMt(staticList[staticObject][2],dynamicList[dynamicObject][2]); //z-distance
	
	float dist_sqr =  rel_x * rel_x + rel_y * rel_y + rel_z* rel_z; //3d distance

	float r_dist_sqr = dynamicList[dynamicObject][3] + staticList[staticObject][3];

	return dist_sqr <= r_dist_sqr * r_dist_sqr;
}
/**************************************************************************************/
float calTankAngle(float distance, float angle, float tankLength)
{//calculate the angle(radian) in which the tank has to rotate about z axis
//to go up the hill base on the distance of the tank from the base of the hill,
//the angle of the hill(radian), and the length of the tank body
	return asin(distance*sin(angle)/tankLength);
}
