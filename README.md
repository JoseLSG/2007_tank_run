COMP 471 - FALL 2007 TERM PROJECT - TANK RUN
============================================

Group 4
JOSE LUIS SALDIVAR
NHUT THAI LE
YI-CHING LU 
ANH THUC NGO


1. Introduction
---------------
1.1-Goal
In this project, we create a game that allows a user tank moving around in a valley surrounded by mountains and shooting random objects, such as other tanks and objects. As planned, we have focused on both functional and visual aspects to solve 5 problems:

1) Moving the tank over an obstacle (e.g. a small hill).
2) Shooting targets within a fixed range.
3) Preventing the tank from going through obstacles (e.g. other tanks and objects) 
4) Creating explosion effects on the objects.
5) Creating terrain and landscape.

1.2-Approach
We have done a lot of researches and referred some examples from previous semesters to find the effective solutions for our games. We have also dealt with a lot of mathematical and physical problems to resolve the collision avoidance problem use to move the tank over an obstacle, shooting targets within a fixed range and preventing the tank from going through the obstacles. 

As for research, the Internet and the examples from previous semesters were very helpful for us. We went through the codes, online tutorials and forums to find the general idea of how to solve our problems. With these general ideas, we build our own solutions or apply them directly to our game. However, for some specific functionality such as collision avoidance, moving up and down an obstacle, and finding the position of the bullet, we had to come up with our own algorithm. By doing researches and build our own algorithm, we have learned a lot from others and pushes ourselves to think and to find our own solutions.

In order to work effectively, we assigned specific work for each member. We work individually in our own part, yet set up frequent meetings to discuss and exchange the ideas so that we follow the same direction and measure the progress. When each member finishes their part, we combine the codes together and produce the final version of the original game design. 

1.3-Discussion
Following the proposal, we think that we meet our promise.  
By implementing this, we added realism to the game. Though our game still contains some bugs, we were somehow successful in the first attempt, it is just a matter of time constrain. If we had more time, we would have improved it in terms of functionality and visual component. 


2. Methodology
--------------
2.1-Implementation of tank movement over a fixed obstacle (small hill)
We are trying to find a simple way to mimic the movement of the tank over a fixed hill. For the tank to move over the hill, we need to know the following parameters:
1.	Angles of the hill with respect to the flat ground: hill angle when the tank goes up and when the tank goes down. For simplicity the angle of the up hill and down hill is set to be the same.  In our implementation the variable hillAngle represents this angle.
2.	The length of the tank body: the tank will not be translated along the y-axis until it is completely on the edge of the hill.
3.	Top and bottom position of the hill: when the tank moves around the world, it must know whether it hit the base of the hill or the top of the hill. To do so, we check if the tank collide with the spheres represents the top and the bottom of the hill.
The arrangement of the 2 spheres for the hill is shown as below:
 

From the position of the 2 spheres of the tank and the 2 spheres of the hill we can tell whether the tank is outside, on the edges, or on top of the hill.
The screenshot below shows 2 spheres of the tank:
Outside sphere: black
Inside sphere: yellow
 

Moving the tank over the hill is done by rotating the tank according to the hill angle, translating the tank along x, y, z axis properly. The angle of the tank with respect to the ground while moving up/down hill is represented by beta in our program.
When the tank is moving up the hill but no completely on the edge of the hill, beta is:
Beta = arcsin(x*sin(hillAngle))/tankLength
The translation of the tank along the y-axis is:
tank_y = xsin(hillAngle)-tankLength*sin(hillAngle)
Where x is the distance the tank has moved along the edge of the hill up to the moment. Downhill movement is nearly the reverse of up hill movement.

2.2-Shooting targets within a fixed range
With this objective, we decided to apply physics to our tank shooting functionality. We also could just implement a straight line equation. We did not choose to do it with straight line equation because the ballistic equation represents reality better. 

Borrowing from physics the equation of moving objects according to velocity and gravity, we implemented that along with other trigonometry needed to separate the one physics equation into 3 parts.

Ballistic Trajectory:
S = So + Vo * T – G * T * T / 2,	
Where:
S = final position,
So = initial position, 
Vo = initial velocity, 
G = acceleration due to gravity, and 
T = time.

Trigonometry used:
Y = sin(angle_v) * Vo,
 r = cos(angle_v) * Vo,
X = r * cos(angle_h),
Z = r * sin(angle_h)

   
Each part represents one of the 3 dimensions (x,y,z coordinates) needed to determine the position of the projectile bullet at any interval time. In our algorithm, our main goal was to determine the final position of the bullet once it hits a target (based on our collision detection algorithm) or it reaches ground level. 

Object Hit
 

Ground Hit
 

After we understand the equation, we have to implement it into our code. As the tank moves and changes the angle of the barrel, the variables of the equation also have to change. The effect of the variable change must represent an approximation to the real world shooting. 

We only considered gravity as the variable most important in calculating a realistic ballistics trajectory. We could have implemented air resistance, friction, or magnetic fields to try to approximate real bullet motion but it would result in much more calculations. We feel that for this game, gravity would be enough approximate real ballistics trajectory.

2.3-Preventing the tank from clipping through objects (Collision Detection)
An implementation of collision detection is simply based on the concept of 2 spheres moving in space. If the 2 spheres collide, the distance between their 2 centers is less than the sum of their respective radius. To implement this concept, we calculate the distance between the center of the tank and the center of the static object whenever the position of the tank is changed. The distance is then compared to the sum of the imaginary radius of the tank and the static object to determine if there is a collision. If there is a collision, the previous position of the tank is used instead of the new one; thus the tank stays at the same position, it can not advance. We have an array of static objects, each has 4 parameters: center coordinate x, y, z and the last one is the imaginary radius.
All the objects in this array are checked for collision every time the tank move. In our project, there is a small gap between where the collision spheres and the objects inside the sphere.

 

Instead of using sphere collision, we could have used a rectangle shape that would fit more properly for some objects but it would require more calculations with ray to plane equations to find an intersecting points. This method could be used in conjunctions with sphere collision to create a more realistic level of detail between objects. Another method could be use just simple increase the number of spheres and reduce the size of their radius. It would require more work to maintain variable updated and checked but the result would be a more realistic collision detection system. Our team felt our implementation would satisfy our need for collision. We would spend more time on other aspects of the project yet use our collision implementation as an important base for other functionality aspects of this game.

2.4-Creating explosion effect on the objects
In conjunction with the ballistic trajectory, we also implement an explosion at the hitting point. The explosion itself is composed of 4 components: Particles, Light, Smoke and Sound.

Particles resulting from the explosion: To mimic the explosion effect, we first draw 70 triangles with the same size and 1000 points using glpoints function. During the explosion, the size of the triangles and points will become bigger and also be displaced continuously. With the displacement and size changing effect of 70 triangles and 1000 particles, the explosion can be mimicked to a reasonable level of realistic. 
Sound is played each time when an explosion occurs.

Smoke must come out of the exploded object to make the explosion look more real. The smoke is done with alpha blending.  We first place two square objects crossing each other. Its alpha blending value will be increased continuously until reaching a point after the explosion effect with the particles. There are a total of four “smoke” objects. Each of them will be displaced upward to mimic the moving of the smoke. The smoke objects are textured.

Light effect of the explosion is the same idea as the smoke. It consists of alpha blending of two square objects crossing each other. The square objects for light are bigger in size than the smoke. The objects are textured.

2.5-Creating terrain and landscape
The terrain is made of a mesh of triangles (triangle strip). The variation in height of the terrain is generated using a height map. We make a grey-scale bitmap file by PhotoShop and use a function to read each pixel on the height map and generate a number according to the color of each pixel (the whither the pixel the larger the number). Another function will read this number and convert into a vector representing the position of the vertex of the triangle it must be in our 3D world. The normal vector at each vertex will also be calculated.
     

3. Result
---------

3.1-Tank movement over a fixed obstacle (small hill)
With the implementation above, when we move the tank over the hill such that the direction of the move is straight to the center of the hill, the tank goes up and down as expected.
One easy way to test is when the game have just started, the tank is at (0, 0, 0) and ready to go along the positive x-axis. The hill is at (80, 0, 0). When the tank moves forward, it will go on the small hill.
There are a few bugs with this approach: after the tank goes on the hill on its rear, the tank goes into the ground. The reason is that the angle of rotation is no longer positive.  Another problem is the position of the tank when it is not passing through the top of the hill but just go around the edge of the hill. Because of the translation, the tank looks like it is “dragged” around the edge of the hill.

2.3-Preventing the tank from going through the obstacles (e.g other tanks and trees)
For the tank, when the tank moves toward an object, it may hit an object at its barrel or at its front bumper. The collide point between 2 object is not precisely calculated because the collision is base on assumption that the 2 object are spheres.

2.4-Creating explosion effect on the objects
In general, the explosion that we implemented satisfied our intention. We can make the explosion happen at any position, we have particles, smoke, sound, light synchronized properly and they mimic the explosion at an acceptable level of realistic.

However, due to the way we implement explosion light and smoke, when viewed from some angle, the explosion is not real.

3.5-Creating terrain and landscape
We are successful in creating the landscape, we have a flat area for the tank to run and around that area we have high mountains surrounded.

The only problem we encountered is that the same texture is applied to the whole terrain. We want to change texture depend on the height of the terrain.

4. Discussion 
-------------

4.1-Tank movement over a fixed obstacle (small hill)
We think our implementation is pretty much promising. To fix the tank colliding to the ground, we just have to implement a collision detection of the front of the tank and the ground. While the tank is going down, if the front touch the ground, the tank should stop translate down but start rotating back to normal.
Through this project we have learned a valuable lesion: “by wasting time on this project, we screwed up our exam and other curses”

2.2-Shooting targets within a fixed range
Implementing physics and trigonometry equations into our project was the correct approach to our game. Straight line trajectory for a bullet wouldn’t make much sense at certain shooting angles (aiming straight in front and keep moving without stopping). To further improve this method, we would consider the addition forces applied to real objects in the real world. This will make effects, such as wind applied positively on a bullet, increasing the reach of the bullet. We learned how to implement other study fields, such physics, into a program to increase realism and in the process have a better idea of these other fields. 

2.3-Preventing the tank from going through the obstacles (e.g other tanks and trees)
To remedy the problem of the precision of the collision above, we can increase the number of spheres attached to the object and try to make the spheres have a more refined fit to the objects. However, calculation is more complex.
 
2.4-Creating explosion effect on the objects
We was thinking about increasing the number of plane used for explosion light and smoke but after a few tried, we realize that it make the probability of seeing unreal light increase. Thus, at this moment, we don’t know how to fix this problem.

2.5-Creating terrain and landscape
We think if we replace triangle strip simple primitive triangles or quads built one vertex by one vertex then we can map different texture on different piece. Thus it is possible to map different texture at different height. However, this approach will take a lot of time and memory.
User Manual


Instructions to run Demo_Tank project:
======================================

1) Extract files
2) Open workspace Demo_Tank.dsw on Visual C++ 6
3) Compile all files and run the .exe file

Or without compilation, run the \Demo_Tank_submission\Demo_Tank\Demo_Tank.exe file

Note: The main file is Tank_Game.cpp .

 This is a menu. Please consult the following for in-game functions.  
	 Press t/8 to move the tank forward.  
	 Press T/2 to move the tank backward.  
	 Press w/W to toggle between wired or solid frame.   
	 Press f/b to move the camera forward/backward.  
	 Press o/p to toggle between orthographic/perspective view.  
	 Press k/K to rotate the tank's machine gun clock/counter clock wise  
	 Press r/6 to rotate the tank clockwise.  
	 Press R/4 to rotate the tank counter clock wise.  
	 Press q/7 to pitch the tank barrel up/down.  
	 Press Q/9 to pitch the tank barrel up/down.  
	 Press y/1 to yaw the tank machine turret clockwise  
	 Press y/3 to yaw the tank machine turret counter-clockwise  
	 Press a/5 to fire.  
	Press s/S to toggle collision spheres view on/off.  


This game is implemented by:
	NHUT THAI LE  
	JOSE LUIS SALDIVAR      	                                        
	NGO ANH THUC
	YI-CHING LU 
	Ressources:  
	http://www.videotutorialsrock.com/                 
	http://nehe.gamedev.net/                                   

	Special Thanks to:                                             
	Our professor, Dr. Adam Krzyzak.                  
	 *Our tutors, Lukasz Jelen and Rokita Joanne            
