#ifndef _EXPLOSION_H
#define _EXPLOSION_H

	void initSpark(float x, float y, float z);
	void initSmoke(float x, float y, float z);
	void newSpark (void);
	void drawOneSmoke(int num, float initX, float initY, float initZ);
	void explosionChange(void);
	void particleExplosion(void);
	void drawSquadSpark(void);


#define NUM_SMOKE 40


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

extern	Spark SparkObject;
extern	bool do_explosion;	

struct Smoke{
	   float _PositionX;
	   float _PositionY;
	   float _PositionZ;
	   GLfloat   _Alpha;
	   GLfloat   _Scale;

	   bool reach_max_alpha;
	   bool reach_min_alpha;
};

extern	Smoke SmokeArray[NUM_SMOKE];
extern	bool do_smoke ;

extern	int sparkTime;

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

extern	particleData  particles[NUM_PARTICLES];
extern	debrisData       debris[NUM_DEBRIS];

extern	int     fuel;
extern	int      wantNormalize;   /* Speed vector normalization flag */

extern	bool sound;
extern	float cuttingPos ;
extern	bool enableCutPlane_Thuc;
extern	bool is_collapse_Thuc;
extern	float gun_fall_angle;


#endif