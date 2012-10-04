
#ifndef TERRAIN_ARCH_H
#define TERRAIN_ARCH_H

void Terrain_func_1(void);

void func_set_2(void);

void text_Creation(void);

bool CreateTexture(char* strFileName, GLuint* textureArray, GLint textureID);

void drawGround(void);

extern GLuint  texture[10];

#endif