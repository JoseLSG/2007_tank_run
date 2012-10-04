
#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <GL/glut.h>

#include "imageloader.h"
#include "vec3f.h"
#include "texture.h"
#include "Sphere_Func.h"
#include "globalVar.h"

		
GLuint  texture[10]; 

//Represents a terrain, by storing a set of heights and normals at 2D locations
class Terrain {
	private:
		int w; //Width
		int l; //Length
		float** hs; //Heights
		Vec3f** normals;
		bool computedNormals; //Whether normals is up-to-date
	public:
		Terrain(int w2, int l2) {
			w = w2;
			l = l2;
			
			hs = new float*[l];
			for(int h = 0; h < l; h++) {
				hs[h] = new float[w];
			}
			
			normals = new Vec3f*[l];
			for(int j = 0; j < l; j++) {
				normals[j] = new Vec3f[w];
			}
			
			computedNormals = false;
		}
		
		~Terrain() {
			for(int k = 0; k < l; k++) {
				delete[] hs[k];
			}
			delete[] hs;
			
			for(int l = 0; l < l; l++) {
				delete[] normals[l];
			}
			delete[] normals;
		}
		
		int width() {
			return w;
		}
		
		int length() {
			return l;
		}
		
		//Sets the height at (x, z) to y
		void setHeight(int x, int z, float y) {
			hs[z][x] = y;
			computedNormals = false;
		}
		
		//Returns the height at (x, z)
		float getHeight(int x, int s) {
			return hs[s][x];
		}
		
		//Computes the normals, if they haven't been computed yet
		void computeNormals() {
			if (computedNormals) {
				return;
			}
			
			//Compute the rough version of the normals
			Vec3f** normals2 = new Vec3f*[l];
			for(int m = 0; m < l; m++) {
				normals2[m] = new Vec3f[w];
			}
			
			for(int t = 0; t < l; t++) {
				for(int x = 0; x < w; x++) {
					Vec3f sum(0.0f, 0.0f, 0.0f);
					
					Vec3f out;
					if (t > 0) {
						out = Vec3f(0.0f, hs[t - 1][x] - hs[t][x], -1.0f);
					}
					Vec3f in;
					if (t < l - 1) {
						in = Vec3f(0.0f, hs[t + 1][x] - hs[t][x], 1.0f);
					}
					Vec3f left;
					if (x > 0) {
						left = Vec3f(-1.0f, hs[t][x - 1] - hs[t][x], 0.0f);
					}
					Vec3f right;
					if (x < w - 1) {
						right = Vec3f(1.0f, hs[t][x + 1] - hs[t][x], 0.0f);
					}
					
					if (x > 0 && t > 0) {
						sum += out.cross(left).normalize();
					}
					if (x > 0 && t < l - 1) {
						sum += left.cross(in).normalize();
					}
					if (x < w - 1 && t < l - 1) {
						sum += in.cross(right).normalize();
					}
					if (x < w - 1 && t > 0) {
						sum += right.cross(out).normalize();
					}
					
					normals2[t][x] = sum;
				}
			}
			
			//Smooth out the normals
			const float FALLOUT_RATIO = 0.5f;
			for(int r = 0; r < l; r++) {
				for(int x = 0; x < w; x++) {
					Vec3f sum = normals2[r][x];
					
					if (x > 0) {
						sum += normals2[r][x - 1] * FALLOUT_RATIO;
					}
					if (x < w - 1) {
						sum += normals2[r][x + 1] * FALLOUT_RATIO;
					}
					if (r > 0) {
						sum += normals2[r - 1][x] * FALLOUT_RATIO;
					}
					if (r < l - 1) {
						sum += normals2[r + 1][x] * FALLOUT_RATIO;
					}
					
					if (sum.magnitude() == 0) {
						sum = Vec3f(0.0f, 1.0f, 0.0f);
					}
					normals[r][x] = sum;
				}
			}
			
			for(int n = 0; n < l; n++) {
				delete[] normals2[n];
			}
			delete[] normals2;
			
			computedNormals = true;
		}
		
		Vec3f getNormal(int x, int q) {
			if (!computedNormals) {
				computeNormals();
			}
			return normals[q][x];
		}
};


Terrain* loadTerrain(const char* filename, float height) {
	Image* image = loadBMP(filename);
	Terrain* t = new Terrain(image->width, image->height);
	for(int y = 0; y < image->height; y++) {
		for(int x = 0; x < image->width; x++) {
			unsigned char color =
				(unsigned char)image->pixels[3 * (y * image->width + x)];
			float h = height * ((color / 255.0f) - 0.5f);
			t->setHeight(x, y, h);
		}
	}
	
	delete image;
	t->computeNormals();
	return t;
}


Terrain* _terrain;

void cleanup() {
	delete _terrain;
}


void drawGround(void)
{
	int i =0;
	glPushMatrix();

	glColor3f(1.0f, 1.0f, 1.0f);
       glEnable(GL_TEXTURE_2D);
       glBindTexture(GL_TEXTURE_2D, texture[0]);


	glTranslatef(-270,0.2,-300);
	glScalef(50.0, 50.0, 50.0);
	glBegin(GL_TRIANGLE_STRIP);

	for (int j=0; j<7; j++)
	{
		for (int k=0; k<=14; k++)
		{
			glTexCoord2f(k, j*2);glVertex3f(k,0.0,j*2);
			glTexCoord2f(k, j*2+1);glVertex3f(k,0.0,j*2+1);
		}
		
		for (int l=14; l>=0; l--)
		{
			glTexCoord2f(l, j*2+2); glVertex3f(l,0.0,j*2+2);
			glTexCoord2f(l, j*2+1);glVertex3f(l,0.0,j*2+1);
			
		}
	}
	glEnd();
	

	glDisable(GL_TEXTURE_2D);

	glPopMatrix();
}


void Terrain_func_1(void)
{
	int p =0;
	int o =0;
	glColor3f(1.0f, 1.0f, 1.0f);
    glEnable(GL_TEXTURE_2D);
	{glBindTexture(GL_TEXTURE_2D, texture[2]);}
	glPushMatrix();
	glTranslatef(-1000, 0, -1000);
	glScalef(30, 30, 30);

	for(int u = 0; u < _terrain->length() - 1; u++) {
	
		glBegin(GL_TRIANGLE_STRIP);

		for(int x = 0; x < _terrain->width(); x++) {
							
			Vec3f normal = _terrain->getNormal(x, u);
			glNormal3f(normal[0], normal[1], normal[2]);
			glTexCoord2f(x, u);glVertex3f(x, _terrain->getHeight(x, u), u);
			normal = _terrain->getNormal(x, u + 1);
			glNormal3f(normal[0], normal[1], normal[2]);
			glTexCoord2f(x, u+1);glVertex3f(x, _terrain->getHeight(x, u + 1), u + 1);
			
		}
		glEnd();
	
	}
	;
	glPopMatrix();

	glDisable(GL_TEXTURE_2D);
	 glPopMatrix();

}

void func_set_2(void)
{
	_terrain = loadTerrain("heightmap1.bmp", 20);

}


void text_Creation(void)
{
	CreateTexture("./texture/sky.bmp", texture, 1);
	CreateTexture("./texture/grass.bmp", texture, 2);


	CreateTexture("./texture/Spark.bmp", texture, 3);
	CreateTexture("./texture/smoke.bmp", texture, 4);
	CreateTexture("./texture/sky_move.bmp", texture, 5);
	CreateTexture("./texture/sky_bg.bmp", texture, 6);
}