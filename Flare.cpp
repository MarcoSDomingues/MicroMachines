#include "Flare.h"

/*Draw lens flare with specified(lx, ly) light position
in screen coordinates, for given(cx, cy) position of
center of screen.*/

void Flare::render(FLARE_DEF *flare, int lx, int ly, int cx, int cy)
{
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glDisable(GL_DEPTH_TEST);
	glDisable(GL_LIGHTING);
	glDisable(GL_CULL_FACE);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	drawQuad(xFlare - SUNWIDTH / 2, yFlare - SUNHEIGHT / 2, SUNWIDTH, SUNHEIGHT, texSun, 0xffffffe0);

	int     dx, dy;          // Screen coordinates of "destination"
	int     px, py;          // Screen coordinates of flare element
	int     maxflaredist, flaredist, flaremaxsize, flarescale;
	int     width, height, alpha;    // Piece parameters;
	int     i;
	FLARE_ELEMENT_DEF    *element;

	// Compute how far off-center the flare source is.
	maxflaredist = isqrt(cx*cx + cy*cy);
	flaredist = isqrt((lx - cx)*(lx - cx) +
		(ly - cy)*(ly - cy));
	flaredist = (maxflaredist - flaredist);
	flaremaxsize = (int)(SCREENwidth * flare->fMaxSize);
	flarescale = (int)(SCREENwidth * flare->fScale);

	// Destination is opposite side of centre from source
	dx = cx + (cx - lx);
	dy = cy + (cy - ly);

	// Render each element.
	for (i = 0; i < flare->nPieces; ++i)
	{
		element = &flare->element[i];

		// Position is interpolated along line between start and destination.
		px = (int)((1.0f - element->fDistance)*lx + element->fDistance*dx);
		py = (int)((1.0f - element->fDistance)*ly + element->fDistance*dy);

		// Piece size are 0 to 1; flare size is proportion of
		// screen width; scale by flaredist/maxflaredist.
		width = (int)((flaredist*flarescale*element->fSize) / maxflaredist);

		// Width gets clamped, to allows the off-axis flares
		// to keep a good size without letting the elements get
		// too big when centered.
		if (width > flaremaxsize)
		{
			width = flaremaxsize;
		}

		// Flare elements are square (round) so height is just
		// width scaled by aspect ratio.
		height = HEIGHTFROMWIDTH(width);
		alpha = (flaredist*(element->argb >> 24)) / maxflaredist;

		if (width > 1)
		{
			unsigned int    argb = (alpha << 24) | (element->argb & 0x00ffffff);

			drawQuad(px - width / 2, py - height / 2, width, height, element->texture, argb); 
		}
	}

	glutSwapBuffers();
}

void Flare::drawQuad(int x, int y, int width, int height, GLuint *tex, unsigned int colour)
{

	//fazer set da textura
	//TM_setTexture(tex);

	glBegin(GL_QUADS);

	glColor4ub((GLbyte)(colour >> 16 & 0xff),
		(GLbyte)(colour >> 8 & 0xff),
		(GLbyte)(colour >> 0 & 0xff),
		(GLbyte)(colour >> 24 & 0xff));

	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(x, y, 1.0f);
	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(x + width, y, 1.0f);
	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(x + width, y + height, 1.0f);
	glTexCoord2f(0.0f, 1.0f);
	glVertex3f(x, y + height, 1.0f);

	glEnd();
}