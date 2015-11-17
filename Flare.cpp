#include "Flare.h"

extern float mCompMatrix[COUNT_COMPUTED_MATRICES][16];
extern float mNormal3x3[9];

/*Draw lens flare with specified(lx, ly) light position
in screen coordinates, for given(cx, cy) position of
center of screen.*/

void Flare::render(FLARE_DEF *flare, int lx, int ly, int cx, int cy, VSShaderLib s, struct MyMesh m, GLint pvmId, GLint vmId, GLint normalId)
{
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glDisable(GL_DEPTH_TEST);
	glDisable(GL_LIGHTING);
	glDisable(GL_CULL_FACE);

	int dx, dy; // Screen coordinates of "destination"
	int px, py; // Screen coordinates of flare element
	int width, height, alpha; // Piece parameters;
	FLARE_ELEMENT_DEF *element;

	shader = s; 
	mesh = m;
	pvm_uniformId = pvmId;
	vm_uniformId = vmId;
	normal_uniformId = normalId;

	// Compute how far off-center the flare source is.
	int maxflaredist = isqrt(cx*cx + cy*cy);
	int flaredist = isqrt((lx - cx)*(lx - cx) + (ly - cy)*(ly - cy));
	flaredist = (maxflaredist - flaredist);
	int flaremaxsize = (int)(SCREENwidth * flare->fMaxSize);
	int flarescale = (int)(SCREENwidth * flare->fScale);

	// Destination is opposite side of centre from source
	dx = cx + (cx - lx);
	dy = cy + (cy - ly);

	// Render each element.
	for (int i = 0; i < flare->nPieces; ++i)
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

		if (/*width > 1*/true)
		{
			unsigned int    argb = (alpha << 24) | (element->argb & 0x00ffffff);
			//drawQuad(i, i, width, height, element->texture, argb);

			drawQuad((px - width / 2)/500, (py - height / 2)/500, width, height, element->texture, argb);
			std::cout << "x: " << px - width / 2 << std::endl;
			std::cout << "y: " << py - height / 2 << std::endl;
		}
	}
	glBindVertexArray(0);
	glBindTexture(GL_TEXTURE_2D, 0);

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_LIGHTING);
	glEnable(GL_CULL_FACE);
}

void Flare::drawQuad(int x, int y, int width, int height, GLuint *tex, unsigned int colour)
{

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, *tex);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, *tex);
	
	pushMatrix(MODEL);
	translate(MODEL, x, 1, y);
	//scale(MODEL, width, height, 0.3);

	// send matrices to OGL
	computeDerivedMatrix(PROJ_VIEW_MODEL);
	glUniformMatrix4fv(vm_uniformId, 1, GL_FALSE, mCompMatrix[VIEW_MODEL]);
	glUniformMatrix4fv(pvm_uniformId, 1, GL_FALSE, mCompMatrix[PROJ_VIEW_MODEL]);
	computeNormalMatrix3x3();
	glUniformMatrix3fv(normal_uniformId, 1, GL_FALSE, mNormal3x3);

	glBindVertexArray(mesh.vao);
	glDrawElements(mesh.type, mesh.numIndexes, GL_UNSIGNED_INT, 0);
	popMatrix(MODEL);
}