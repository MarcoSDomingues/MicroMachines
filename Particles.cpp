#include "Particles.h"

extern float mCompMatrix[COUNT_COMPUTED_MATRICES][16];
extern float mNormal3x3[9];

void Particles::Particle() {

}

void Particles::iterate(int value)
{
	int i;
	float h;

	/* Método de Euler de integração de eq. diferenciais ordinárias
	h representa o step de tempo; dv/dt = a; dx/dt = v; e conhecem-se os valores iniciais de x e v */

	h = 0.125f;
	for (i = 0; i<MAX_PARTICULAS; i++)
	{
		particula[i].x += (h*particula[i].vx);
		particula[i].y += (h*particula[i].vy);
		particula[i].z += (h*particula[i].vz);
		particula[i].vx += (h*particula[i].ax);
		particula[i].vy += (h*particula[i].ay);
		particula[i].vz += (h*particula[i].az);
		particula[i].life -= particula[i].fade;
	}
}

void Particles::init(GLfloat x, GLfloat y, GLfloat z)
{
	GLfloat v, theta, phi;
	int i;

	for (i = 0; i<MAX_PARTICULAS; i++)
	{
		v = 0.8*frand() + 0.2;
		phi = frand()*M_PI;
		theta = 2.0*frand()*M_PI;

		particula[i].x = x;
		particula[i].y = y;
		particula[i].z = z;
		particula[i].vx = v * cos(theta) * sin(phi);
		particula[i].vy = v * cos(phi);
		particula[i].vz = v * sin(theta) * sin(phi);
		particula[i].ax = 0.1f; /* simular um pouco de vento */
		particula[i].ay = -0.15f; /* simular a aceleração da gravidade */
		particula[i].az = 0.0f;

		/* tom amarelado que vai ser multiplicado pela textura que varia entre branco e preto */
		particula[i].r = 0.882f;
		particula[i].g = 0.552f;
		particula[i].b = 0.211f;

		particula[i].life = 1.0f;		/* vida inicial */
		particula[i].fade = 0.005f;	    /* step de decréscimo da vida para cada iteração */
	}
}

void Particles::render(GLint texture, struct MyMesh mesh, GLint pvm_uniformId, GLint vm_uniformId, GLint normal_uniformId, VSShaderLib shader, float angle)
{
	float particle_color[4];
	// draw fireworks particles

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, texture);
	glDisable(GL_DEPTH_TEST); /* não interessa o z-buffer: as partículas podem ser desenhadas umas por cima das outras sem problemas de ordenação */

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE);

	for (int i = 0; i < MAX_PARTICULAS; i++)
	{
		if (particula[i].life > 0.0f) /* só desenha as que ainda estão vivas */
		{

			/* A vida da partícula representa o canal alpha da cor. Como o blend está activo a cor final é a soma da cor rgb do fragmento multiplicada pelo
			alpha com a cor do pixel destino */

			particle_color[0] = particula[i].r;
			particle_color[1] = particula[i].g;
			particle_color[2] = particula[i].b;
			particle_color[3] = particula[i].life;

			// send the material - diffuse color modulated with texture
			GLint loc = glGetUniformLocation(shader.getProgramIndex(), "mat.diffuse");
			glUniform4fv(loc, 1, particle_color);

			pushMatrix(MODEL);
			
			translate(MODEL, particula[i].x, particula[i].y, particula[i].z);
			rotate(MODEL, angle, 0, 1, 0);
			scale(MODEL, 0.3, 0.3, 0.3);
			//rotate(MODEL, 180, 0, 0, 1);

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
	}
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glBindVertexArray(0);
	glBindTexture(GL_TEXTURE_2D, 0);
	glEnable(GL_DEPTH_TEST);
}
