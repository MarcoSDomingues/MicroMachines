#pragma once

#include <GL/glew.h>
#include <GL/freeglut.h>

#include <stdio.h>
#include <stdlib.h>
#include <memory.h>
#include <math.h>
#include <string.h>

#include "Entity.h"
#include "Vector3.h"
#include "VSShaderlib.h"
#include "basic_geometry.h"
#include "AVTmathLib.h"
#include "Vector4.h"

/* --- Defines --- */
#define FLARE_MAXELEMENTSPERFLARE 15
#define FLARE_RANGE(A,B) ( (rand()%((B)-(A)+1)) + (A) )
#define FLARE_FRANGE(A,B) ( ((float)(rand()&0xffffff)/(float)0xfffffe)*((B)-(A)) + (A) )
#define MAKEID(a,b,c,d) (((a)<<24) | ((b)<<16) | ((c)<<8) | ((d)<<0))
#define SUNWIDTH 50
#define SUNHEIGHT HEIGHTFROMWIDTH(SUNWIDTH)

// Get height from width, assuming certain aspect ratio.
#define HEIGHTFROMWIDTH(w)  ((320*(w)*SCREENheight)/(240*SCREENwidth))
#define isqrt(x)        (int)((double)(x))

/* --- Types --- */
typedef struct FLARE_ELEMENT_DEF
{
	GLuint *texture;

	float           fDistance;  // Distance along ray from source (0.0-1.0)
	float           fSize;      // Size relative to flare envelope (0.0-1.0)
	unsigned int    argb;       // ARGB for intensity mapping
} FLARE_ELEMENT_DEF;

typedef struct FLARE_DEF
{
	float           fScale;     // Scale factor for adjusting overall size of flare elements.
	float           fMaxSize;   // Max size of largest element, as proportion of screen width (0.0-1.0)
	int             nPieces;    // Number of elements in use


	FLARE_ELEMENT_DEF element[FLARE_MAXELEMENTSPERFLARE];
} FLARE_DEF;

class Flare{
	/*Global variables*/
	int     xFlare = 10;
	int     yFlare = 10;

	FLARE_DEF renderFlare;    // Flare to render.
	GLuint *texBack = NULL;
	GLuint *texSun = NULL;
	int bShowSun = 0;

	int SCREENwidth = 640;
	int SCREENheight = 480;

	VSShaderLib shader; struct MyMesh mesh; GLint pvm_uniformId; GLint vm_uniformId; GLint normal_uniformId;

public:
	void drawQuad(float x, float y, float width, float height, GLuint *tex, unsigned int colour);
	void render(FLARE_DEF *flare, float lx, float ly, float cx, float cy, VSShaderLib shader, struct MyMesh mesh, GLint pvm_uniformId, GLint vm_uniformId, GLint normal_uniformId);
};