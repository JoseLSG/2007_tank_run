/***************************************************************************************************************/
/* HEADER FILE FOR THE TUTORIAL ON TEXTURE MAPPING.						
/*												Author: Ramgopal R,
/*												Computer Graphics and Visualization Lab. Concordia University
/*												
/*Notes and Acknowledgements:					Thanks to DigiBen from GameTutorials.com for a significant 
/*												portion of the write up.
/**************************************************************************************************************/

#ifndef __TEXTURE_H
#define __TEXTURE_H

#include <GL\glaux.h>


// This function creates a texture from a BMP file 'strFileName'. It further stores the OpenGL handle to the generated texture 
// in the 'textureID' location of 'textureArray'.
bool CreateTexture(char* strFileName, GLuint* textureArray, GLint textureID)
{
	AUX_RGBImageRec *pBitmap = NULL;					// The glaux data structure to store bitmap.

	
	if(!strFileName)									// Return from the function if no file name was passed in
		return false;
	
	// We need to load the texture data, so we use an API that the glaux.lib.
	
	pBitmap = auxDIBImageLoad(strFileName);				// Load the bitmap and store the data
	
	if(pBitmap == NULL)									// If we can't load the file, quit!
		return false;

	// Now that we have the texture data, we need to register our texture with OpenGL
	// To do this we need to call glGenTextures().  The 1 for the first parameter is
	// how many texture we want to register this time (we could do a bunch in a row).
	// The second parameter is the array index that will hold the reference to this texture.

	// Generate a texture with the associative texture ID stored in the array
	glGenTextures(1, &textureArray[textureID]);

	// Now that we have a reference for the texture, we need to bind the texture
	// to tell OpenGL this is the reference that we are assigning the bitmap data too.
	// The first parameter tells OpenGL we want are using a 2D texture, while the
	// second parameter passes in the reference we are going to assign the texture too.
	// We will use this function later to tell OpenGL we want to use this texture to texture map.

	// Bind the texture to the texture arrays index and init the texture
	glBindTexture(GL_TEXTURE_2D, textureArray[textureID]);

	// Now comes the important part, we actually pass in all the data from the bitmap to
	// create the texture. Here is what the parameters mean in gluBuild2DMipmaps():
	// (We want a 2D texture, 3 channels (RGB), bitmap width, bitmap height, It's an RGB format,
	//  the data is stored as unsigned bytes, and the actuall pixel data);

	// What is a Mip map?  Mip maps are a bunch of scaled pictures from the original.  This makes
	// it look better when we are near and farther away from the texture map.  It chooses the
	// best looking scaled size depending on where the camera is according to the texture map.
	// Otherwise, if we didn't use mip maps, it would scale the original UP and down which would
	// look not so good when we got far away or up close, it would look pixelated.

	// Build Mipmaps (builds different versions of the picture for distances - looks better)
	gluBuild2DMipmaps(GL_TEXTURE_2D, 3, pBitmap->sizeX, pBitmap->sizeY, GL_RGB, GL_UNSIGNED_BYTE, pBitmap->data);

	// Lastly, we need to tell OpenGL the quality of our texture map.  GL_LINEAR_MIPMAP_LINEAR
	// is the smoothest.  GL_LINEAR_MIPMAP_NEAREST is faster than GL_LINEAR_MIPMAP_LINEAR, 
	// but looks blochy and pixilated.  Good for slower computers though.  Read more about 
	// the MIN and MAG filters in the texture.cpp.
		
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR_MIPMAP_NEAREST);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR_MIPMAP_LINEAR);

	// Now we need to free the bitmap data that we loaded since openGL stored it as a texture

	if (pBitmap)										// If we loaded the bitmap
	{
		if (pBitmap->data)								// If there is texture data
		{
			free(pBitmap->data);						// Free the texture data, we don't need it anymore
		}

		free(pBitmap);									// Free the bitmap structure
	}
	return true;
}
#endif //	__TEXTURE_H definition ends here
