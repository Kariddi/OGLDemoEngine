/*
 Code taken from an online source
*/

#ifndef __TGA_HEADER__
#define __TGA_HEADER__

//#include "MainHeader.h"
//#include "TextureHeader.h"

#include <stdlib.h>
#include <stdio.h>
#include <OGL.h>

#include <math.h>

struct TextureData	
{
	GLubyte	* imageData;									// Image Data (Up To 32 Bits)
	GLuint	bpp;											// Image Color Depth In Bits Per Pixel
	GLuint	width;											// Image Width
	GLuint	height;											// Image Height
        GLuint	format;										// Image Type (GL_RGB, GL_RGBA)
	GLenum  type;
        ~TextureData() { free(imageData); }
};	

typedef struct
{
	GLubyte Header[12];									// TGA File Header
} TGAHeader;


typedef struct
{
	GLubyte		header[6];								// First 6 Useful Bytes From The Header
	GLuint		bytesPerPixel;							// Holds Number Of Bytes Per Pixel Used In The TGA File
	GLuint		imageSize;								// Used To Store The Image Size When Setting Aside Ram
	GLuint		temp;									// Temporary Variable
	GLuint		type;	
	GLuint		Height;									//Height of Image
	GLuint		Width;									//Width ofImage
	GLuint		Bpp;									// Bits Per Pixel
} TGA;

bool LoadTGA(TextureData * texture, const char * filename);
bool LoadCompressedTGA(TextureData * texture, const char * filename, FILE * fTGA);
bool LoadUncompressedTGA(TextureData * texture, const char * filename, FILE * fTGA);
bool LoadPNG(TextureData *texture, const char *filename);
bool LoadJPG(TextureData *texture, const char *filename);

#endif
