//
// AVT demo light 
// based on demos from GLSL Core Tutorial in Lighthouse3D.com   
//
// This demo was built for learning purposes only.
// Some code could be severely optimised, but I tried to
// keep as simple and clear as possible.
//
// The code comes with no warranties, use it at your own risk.
// You may use it, or parts of it, wherever you want.
//

#include <math.h>
#include <iostream>
#include <sstream>

#include <string>

// include GLEW to access OpenGL 3.3 functions
#include <GL/glew.h>

// GLUT is the toolkit to interface with the OS
#include <GL/freeglut.h>

// Use Very Simple Libs
#include "VSShaderlib.h"
#include "AVTmathLib.h"
#include "VertexAttrDef.h"
#include "Vector3.h"
#include "Camera.h"
#include "PerspectiveCamera.h"
#include "OrtogonalCamera.h"
#include "basic_geometry.h"
#include <cstdlib>
#include <ctime>

//include objects
#include "GameObject.h"
#include "Car.h"

#define CAPTION "MicroMachines"
int WindowHandle = 0;
int WinX = 640, WinY = 480;

unsigned int FrameCount = 0;

VSShaderLib shader;

struct MyMesh mesh[8];
int objId=0; //id of the object mesh - to be used as index of mesh: mesh[objID] means the current mesh


//External array storage defined in AVTmathLib.cpp

/// The storage for matrices
extern float mMatrix[COUNT_MATRICES][16];
extern float mCompMatrix[COUNT_COMPUTED_MATRICES][16];

/// The normal matrix
extern float mNormal3x3[9];

bool clicking;

Vector3 speed;

GLint pvm_uniformId;
GLint vm_uniformId;
GLint normal_uniformId;
GLint lPos_uniformId;

//incrementar velocidade do jogo
double speed_timer = 0;
double speedInc = 1;
double auxtimer = 0;
double delayDraw[3] = {false, false, false }; //orange delay

double currentTime = 0;
double previousTime = 0;

//Car position
float carX, carY, carZ;

//Orange properties
float orangeX[3], orangeY, orangeZ[3];
float orangeYRot[3];
float orangeVelocity[3];

float tableSize = 5;

// Camera Position
float camX, camY, camZ;

// Mouse Tracking Variables
int startX, startY, tracking = 0;

// Camera Spherical Coordinates
float alpha = 90.0f, beta = -30.0f;
float r = 10.0f;

// Frame counting and FPS computation
long myTime,timebase = 0,frame = 0;
char s[32];
float lightPos[4] = {4.0f, 6.0f, 2.0f, 1.0f};

// cameras
std::vector<Camera*> _cameras;
int _current_camera = 0;

int iteration = 0;
// objects
Car car;

void timer(int value)
{
	std::ostringstream oss;
	oss << CAPTION << ": " << FrameCount << " FPS @ (" << WinX << "x" << WinY << ")";
	std::string s = oss.str();
	glutSetWindow(WindowHandle);
	glutSetWindowTitle(s.c_str());
    FrameCount = 0;
    glutTimerFunc(1000, timer, 0);
}

void refresh(int value)
{
	glutPostRedisplay();
	glutTimerFunc(1000/60, refresh, 0);
}

// ------------------------------------------------------------
//
// Reshape Callback Function
//

void changeSize(int w, int h) {

	float ratio;
	// Prevent a divide by zero, when window is too short
	if(h == 0)
		h = 1;
	// set the viewport to be the entire window
	glViewport(0, 0, w, h);
	// set the projection matrix
	ratio = (1.0f * w) / h;
	loadIdentity(PROJECTION);
	_cameras[_current_camera]->update(ratio);
	
}

//Tem de ficar aqui para o update conhecer a funcao 
float orangeRandomVel() {
	float auxV = ((float(rand()) / float(RAND_MAX)) * (0.001 - (-0.001))) + (-0.001);
	if (auxV == 0) {
		orangeRandomVel();
	}
	else
		return auxV;
}

void initOrange(int i) {
	orangeX[i] = rand() % 9 - 4;
	orangeZ[i] = rand() % 9 - 4;
	orangeVelocity[i] = orangeRandomVel();
}

void update(double delta_t) {
	car.update(delta_t);

	//aumentar a velocidade depois de um certo tempo
	if (glutGet(GLUT_ELAPSED_TIME) > speed_timer + 20000) {
		speed_timer = glutGet(GLUT_ELAPSED_TIME);
		speedInc += 1;
	}

	for (int i = 0; i < 3; i++) {
		orangeX[i] = orangeX[i] + orangeVelocity[i] * speedInc * delta_t;
		orangeYRot[i] = orangeYRot[i] + (orangeVelocity[i] * 100 * speedInc ) * delta_t;
	}

	for (int i = 0; i < 3; i++) {
		if (orangeX[i] >= tableSize || orangeX[i] <= -tableSize) {
			delayDraw[i] = true;
			if (glutGet(GLUT_ELAPSED_TIME) > auxtimer + 5000) {
				auxtimer = glutGet(GLUT_ELAPSED_TIME);
				initOrange(i);
				delayDraw[i] = false;
			}
		}
	}
}

void idle() {
	currentTime = glutGet(GLUT_ELAPSED_TIME);
	update(currentTime - previousTime);
	previousTime = currentTime;
	//glutPostRedisplay();
}


// ------------------------------------------------------------
//
// Render stufff
//

void loadMesh() {
	GLint loc;
	loc = glGetUniformLocation(shader.getProgramIndex(), "mat.ambient");
	glUniform4fv(loc, 1, mesh[objId].mat.ambient);
	loc = glGetUniformLocation(shader.getProgramIndex(), "mat.diffuse");
	glUniform4fv(loc, 1, mesh[objId].mat.diffuse);
	loc = glGetUniformLocation(shader.getProgramIndex(), "mat.specular");
	glUniform4fv(loc, 1, mesh[objId].mat.specular);
	loc = glGetUniformLocation(shader.getProgramIndex(), "mat.shininess");
	glUniform1f(loc, mesh[objId].mat.shininess);
}

void renderMesh() {
	// send matrices to OGL
	computeDerivedMatrix(PROJ_VIEW_MODEL);
	glUniformMatrix4fv(vm_uniformId, 1, GL_FALSE, mCompMatrix[VIEW_MODEL]);
	glUniformMatrix4fv(pvm_uniformId, 1, GL_FALSE, mCompMatrix[PROJ_VIEW_MODEL]);
	computeNormalMatrix3x3();
	glUniformMatrix3fv(normal_uniformId, 1, GL_FALSE, mNormal3x3);

	// Render mesh
	glBindVertexArray(mesh[objId].vao);
	glDrawElements(mesh[objId].type, mesh[objId].numIndexes, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}

void drawOrange(float x, float y, float z, float orangeRot) {
	//DRAWORANGE
	objId = 7;

	loadMesh();
	pushMatrix(MODEL);
	translate(MODEL, x, y, z);
	scale(MODEL, 0.6f, 0.6f, 0.6f);
	rotate(MODEL, orangeRot, 0, 0, orangeRot);
	renderMesh();
	popMatrix(MODEL);
}


void drawButterBox(float x, float y, float z) {
	objId = 5;

	pushMatrix(MODEL);
	scale(MODEL, 0.7f, 0.7f, 0.7f);
	rotate(MODEL, -45, 0, 1, 0);

	loadMesh();
	pushMatrix(MODEL);
	translate(MODEL, x - 0.5, y + 0.1f, z - 0.5);
	scale(MODEL, 1.0f, 0.5f, 1.2f);
	//rotate(MODEL, 90, 90, 0, 0);
	renderMesh();
	popMatrix(MODEL);

	loadMesh();
	pushMatrix(MODEL);
	translate(MODEL, x - 0.6f, y + 0.6f, z - 0.6f);
	scale(MODEL, 1.2f, 0.1f, 1.4f);
	//rotate(MODEL, 90, 90, 0, 0);
	renderMesh();
	popMatrix(MODEL);
	popMatrix(MODEL);
}

void drawTable() {
	//Draw Table
	objId = 0;
	loadMesh();
	pushMatrix(MODEL);
	
	scale(MODEL, 9.0f, 0.5f, 9.0f);
	translate(MODEL, -0.5f, 0.0f, -0.5f);
	
	renderMesh();
	popMatrix(MODEL);
}

void drawRoad() {
	//Draw Road
	objId = 1;
	loadMesh();
	pushMatrix(MODEL);

	//top
	scale(MODEL, 7.0f, 0.51f, 1.0f);
	translate(MODEL, -0.5f, 0.0f, -3.5f);

	renderMesh();
	popMatrix(MODEL);

	//bottom
	loadMesh();
	pushMatrix(MODEL);

	scale(MODEL, 7.0f, 0.51f, 1.0f);
	translate(MODEL, -0.5f, 0.0f, 2.6f);

	renderMesh();
	popMatrix(MODEL);

	//left
	loadMesh();
	pushMatrix(MODEL);

	scale(MODEL, 1.0f, 0.51f, 7.0f);
	translate(MODEL, -3.5f, 0.0f, -0.5f);

	renderMesh();
	popMatrix(MODEL);

	//right
	loadMesh();
	pushMatrix(MODEL);

	scale(MODEL, 1.0f, 0.51f, 7.0f);
	translate(MODEL, 2.5f, 0.0f, -0.5f);

	renderMesh();
	popMatrix(MODEL);

	//Start Marker
	objId = 2;
	loadMesh();
	pushMatrix(MODEL);

	scale(MODEL, 0.1f, 0.5f, 1.0f);
	translate(MODEL, 0.0f, 0.03f, 2.6f);

	renderMesh();
	popMatrix(MODEL);
}

void cheerio(MatrixTypes aType, float x, float y, float z) {
	loadMesh();
	pushMatrix(aType);

	translate(aType, x, y, z);

	renderMesh();
	popMatrix(aType);
}

void drawCheerios() {
	objId = 3;
	
	float xBot = 2.4f;		float yBot = 2.5f;
	float xTop = -2.4f;		float yTop = -3.6f;
	float xRight = 2.4f;	float yRight = -2.1f;
	float xLeft = -2.4f;	float yLeft = -2.1f;

	for (int i = 0; i < 17; i++) {
		cheerio(MODEL, xBot - 0.3f * i, 0.52f, yBot);
		cheerio(MODEL, xBot - 0.3f * i, 0.52f, yBot + 1.2f);
		cheerio(MODEL, xTop + 0.3f * i, 0.52f, yTop);
		cheerio(MODEL, xTop + 0.3f * i, 0.52f, yTop + 1.2f);
	}

	for (int i = 0; i < 15; i++) {
		cheerio(MODEL, xRight, 0.52f, yRight + 0.3f * i);
		cheerio(MODEL, xRight + 1.2f, 0.52f, yRight + 0.3f * i);
		cheerio(MODEL, xLeft, 0.52f, yLeft + 0.3f * i);
		cheerio(MODEL, xLeft - 1.2f, 0.52f, yLeft + 0.3f * i);
	}
}

void renderScene(void) {

	GLint loc;
	FrameCount++;
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	// load identity matrices
	loadIdentity(VIEW);
	loadIdentity(MODEL);

	float carX = car.getPosition().getX();
	float carZ = car.getPosition().getZ();

	Vector3 dir = car.getDirection() + car.getPosition();
	float dirX = dir.getX();
	float dirZ = dir.getZ();

	Vector3 cam = car.getPosition() - car.getDirection();

	float cX = cam.getX();
	float cZ = cam.getZ();

	//if (_current_camera == 2) lookAt(cX, 2, cZ, dirX, 1, dirZ, 0, 1, 0);

		if (_current_camera == 2) {
			if (clicking) {
				lookAt(carX, 3, carZ, carX + camX, 1, carZ + camZ, 0, 1, 0);
			}
			else {
				lookAt(cX, 2, cZ, dirX, 1, dirZ, 0, 1, 0);
			}
		}
		else lookAt(0, 10, 0.1, 0, 0, 0, 0, 1, 0);
	// use our shader
	glUseProgram(shader.getProgramIndex());

	//send the light position in eye coordinates
	glUniform4fv(lPos_uniformId, 1, lightPos); //efeito capacete do mineiro, ou seja lighPos foi definido em eye coord 
	
	/*
	float res[4];
	multMatrixPoint(VIEW, lightPos,res);   //lightPos definido em World Coord so is converted to eye space
	glUniform4fv(lPos_uniformId, 1, res);
	*/	

	drawTable();
	drawRoad();
	drawCheerios();
	//drawCar(carX, carY, carZ);
	car.draw(car.getPosition().getX(), car.getPosition().getY(), car.getPosition().getZ(), shader, pvm_uniformId, vm_uniformId, normal_uniformId, lPos_uniformId);
	
	for (int i = 0; i < 3; i++) {
		if (!delayDraw[i]) {
			drawOrange(orangeX[i], 1.0, orangeZ[i], orangeYRot[i]);
		}
	}
	drawButterBox(7.0f, 0.5f, 0.0f);
	drawButterBox(-7.0f, 0.5f, 0.0f);

	glutSwapBuffers();
}

// ------------------------------------------------------------
//
// Events from the Keyboard
//

void keyPressed(unsigned char key, int xx, int yy)
{
	switch(key) {

		case 27:
			glutLeaveMainLoop();
			break;
		case '1':
			_current_camera = 0;
			changeSize(glutGet(GLUT_WINDOW_WIDTH), glutGet(GLUT_WINDOW_HEIGHT));
			break;
		case '2':
			_current_camera = 1;
			changeSize(glutGet(GLUT_WINDOW_WIDTH), glutGet(GLUT_WINDOW_HEIGHT));
			break;
		case '3':
			_current_camera = 2;
			changeSize(glutGet(GLUT_WINDOW_WIDTH), glutGet(GLUT_WINDOW_HEIGHT));
			break;
		case 'c': 
			printf("Camera Spherical Coordinates (%f, %f, %f)\n", alpha, beta, r);
			printf("Camera Cartesian Coordinates (%f, %f, %f)\n", camX, camY, camZ);
			break;
		case 'm': glEnable(GL_MULTISAMPLE); break;
		case 'n': glDisable(GL_MULTISAMPLE); break;
		
		case 'O': case 'o':
			car.left();
			break;

		case 'P': case 'p':
			car.right();
			break;

		case 'Q': case 'q':
			car.accelerate();
			break;
	
		case 'A': case 'a':
			car.reverse();
			break;
		default:
			break;
	}
}

void keyReleased(unsigned char key, int x, int y)
{
	switch (key) {
		case 'O': case 'o':
			car.stopLeft();
			break;

		case 'P': case 'p':
			car.stopRight();
			break;

		case 'Q': case 'q':
			car.stopForward();
			break;
		case 'A': case 'a':
			car.stopBack();
			break;
		default:
			break;
	}
}

// ------------------------------------------------------------
//
// Mouse Events
//

void processMouseButtons(int button, int state, int xx, int yy)
{
	// start tracking the mouse
	if (state == GLUT_DOWN)  {
		clicking = true;
		startX = xx;
		startY = yy;
		if (button == GLUT_LEFT_BUTTON)
			tracking = 1;
		else if (button == GLUT_RIGHT_BUTTON)
			tracking = 2;
	}

	//stop tracking the mouse
	else if (state == GLUT_UP) {
		clicking = false;
		if (tracking == 1) {
			alpha -= (xx - startX);
			beta += (yy - startY);
		}
		else if (tracking == 2) {
			r += (yy - startY) * 0.01f;
			if (r < 0.1f)
				r = 0.1f;
		}
		tracking = 0;
	}
}

// Track mouse motion while buttons are pressed

void processMouseMotion(int xx, int yy)
{

	int deltaX, deltaY;
	float alphaAux, betaAux;
	float rAux;

	deltaX =  - xx + startX;
	deltaY =    yy - startY;

	// left mouse button: move camera
	if (tracking == 1) {


		alphaAux = alpha + deltaX;
		betaAux = beta + deltaY;

		if (betaAux > 85.0f)
			betaAux = 85.0f;
		else if (betaAux < -85.0f)
			betaAux = -85.0f;
		rAux = r;
	}
	// right mouse button: zoom
	else if (tracking == 2) {

		alphaAux = alpha;
		betaAux = beta;
		rAux = r + (deltaY * 0.01f);
		if (rAux < 0.1f)
			rAux = 0.1f;
	}

	camX = rAux * sin(alphaAux * 3.14f / 180.0f) * cos(betaAux * 3.14f / 180.0f);
	camZ = rAux * cos(alphaAux * 3.14f / 180.0f) * cos(betaAux * 3.14f / 180.0f);
	camY = rAux *   						       sin(betaAux * 3.14f / 180.0f);

//  uncomment this if not using an idle func
//	glutPostRedisplay();
}


void mouseWheel(int wheel, int direction, int x, int y) {

	r += direction * 0.1f;
	if (r < 0.1f)
		r = 0.1f;

	camX = r * sin(alpha * 3.14f / 180.0f) * cos(beta * 3.14f / 180.0f);
	camZ = r * cos(alpha * 3.14f / 180.0f) * cos(beta * 3.14f / 180.0f);
	camY = r *   						     sin(beta * 3.14f / 180.0f);

//  uncomment this if not using an idle func
//	glutPostRedisplay();
}

// --------------------------------------------------------
//
// Shader Stuff
//


GLuint setupShaders() {

	// Shader for models
	shader.init();
	shader.loadShader(VSShaderLib::VERTEX_SHADER, "shaders/pointlight.vert");
	shader.loadShader(VSShaderLib::FRAGMENT_SHADER, "shaders/pointlight.frag");

	// set semantics for the shader variables
	glBindFragDataLocation(shader.getProgramIndex(), 0,"colorOut");
	glBindAttribLocation(shader.getProgramIndex(), VERTEX_COORD_ATTRIB, "position");
	glBindAttribLocation(shader.getProgramIndex(), NORMAL_ATTRIB, "normal");
	glBindAttribLocation(shader.getProgramIndex(), TEXTURE_COORD_ATTRIB, "texCoord");

	glLinkProgram(shader.getProgramIndex());

	pvm_uniformId = glGetUniformLocation(shader.getProgramIndex(), "m_pvm");
	vm_uniformId = glGetUniformLocation(shader.getProgramIndex(), "m_viewModel");
	normal_uniformId = glGetUniformLocation(shader.getProgramIndex(), "m_normal");
	lPos_uniformId = glGetUniformLocation(shader.getProgramIndex(), "l_pos");
	
	printf("InfoLog for Hello World Shader\n%s\n\n", shader.getAllInfoLogs().c_str());
	
	return(shader.isProgramValid());
}

// ------------------------------------------------------------
//
// Model loading and OpenGL setup
//

void init()
{
	car.setPosition(0.0f, 0.45f, 2.8f);

	// set the camera position based on its spherical coordinates
	camX = r * sin(alpha * 3.14f / 180.0f) * cos(beta * 3.14f / 180.0f);
	camZ = r * cos(alpha * 3.14f / 180.0f) * cos(beta * 3.14f / 180.0f);
	camY = r *   						     sin(beta * 3.14f / 180.0f);

	carX = car.getPosition().getX();
	carY = car.getPosition().getY();
	carZ = car.getPosition().getZ();

	
	std::vector<Car> cars;
	cars.push_back(car);

	srand(time(NULL));
	for (int i = 0; i < 3; i++) {
		initOrange(i);
		orangeYRot[i] = 0.0f;
	}
	orangeY = 2.0f;

	speed = Vector3(0.0f, 0.0f, 0.0f);

	float amb[]= {0.2f, 0.15f, 0.1f, 1.0f};
	float diff[] = {0.8f, 0.6f, 0.4f, 1.0f};
	float spec[] = {0.8f, 0.8f, 0.8f, 1.0f};
	float emissive[] = {0.0f, 0.0f, 0.0f, 1.0f};
	float shininess= 100.0f;
	int texcount = 0;

	// create geometry and VAO of the pawn
	objId=0;
	memcpy(mesh[objId].mat.ambient, amb,4*sizeof(float));
	memcpy(mesh[objId].mat.diffuse, diff,4*sizeof(float));
	memcpy(mesh[objId].mat.specular, spec,4*sizeof(float));
	memcpy(mesh[objId].mat.emissive, emissive,4*sizeof(float));
	mesh[objId].mat.shininess = shininess;
	mesh[objId].mat.texCount = texcount;
	createTable();

	float amb1[] = { 0.2f, 0.2f, 0.2f, 1.0f };
	float diff1[] = { 0.4f, 0.4f, 0.4f, 1.0f };
	float spec1[] = { 0.5f, 0.5f, 0.5f, 1.0f };
	shininess = 50.0f;

	// create geometry and VAO of the sphere
	objId=1;
	memcpy(mesh[objId].mat.ambient, amb1,4*sizeof(float));
	memcpy(mesh[objId].mat.diffuse, diff1,4*sizeof(float));
	memcpy(mesh[objId].mat.specular, spec1,4*sizeof(float));
	memcpy(mesh[objId].mat.emissive, emissive,4*sizeof(float));
	mesh[objId].mat.shininess = shininess;
	mesh[objId].mat.texCount = texcount;
	createCube();

	// create geometry and VAO of the cylinder

	float amb2[] = { 0.7f, 0.7f, 0.7f, 1.0f };
	float diff2[] = { 0.8f, 0.8f, 0.8f, 1.0f };
	float spec2[] = { 0.9f, 0.9f, 0.9f, 1.0f };
	shininess = 50.0f;

	objId=2;
	memcpy(mesh[objId].mat.ambient, amb2,4*sizeof(float));
	memcpy(mesh[objId].mat.diffuse, diff2,4*sizeof(float));
	memcpy(mesh[objId].mat.specular, spec2,4*sizeof(float));
	memcpy(mesh[objId].mat.emissive, emissive,4*sizeof(float));
	mesh[objId].mat.shininess = shininess;
	mesh[objId].mat.texCount = texcount;
	createCube();

	float amb3[] = { 0.7f, 0.5f, 0.0f, 1.0f };
	float diff3[] = { 0.8f, 0.6f, 0.0f, 1.0f };
	float spec3[] = { 0.0f, 0.0f, 0.0f, 1.0f };
	shininess = 50.0f;
	// create geometry and VAO of the 
	objId=3;
	memcpy(mesh[objId].mat.ambient, amb3,4*sizeof(float));
	memcpy(mesh[objId].mat.diffuse, diff3,4*sizeof(float));
	memcpy(mesh[objId].mat.specular, spec3,4*sizeof(float));
	memcpy(mesh[objId].mat.emissive, emissive,4*sizeof(float));
	mesh[objId].mat.shininess = shininess;
	mesh[objId].mat.texCount = texcount;
	createTorus(0.04f, 0.1f, 6, 10);

	//BlackWheels
	objId = 4;

	float amb4[] = { 0.0f, 0.0f, 0.0f, 1.0f };
	float diff4[] = { 0.3f, 0.3f, 0.4f, 1.0f };
	float spec4[] = { 0.9f, 0.9f, 0.9f, 1.0f };
	shininess = 500.0;

	memcpy(mesh[objId].mat.ambient, amb4, 4 * sizeof(float));
	memcpy(mesh[objId].mat.diffuse, diff4, 4 * sizeof(float));
	memcpy(mesh[objId].mat.specular, spec4, 4 * sizeof(float));
	memcpy(mesh[objId].mat.emissive, emissive, 4 * sizeof(float));
	mesh[objId].mat.shininess = shininess;
	mesh[objId].mat.texCount = texcount;
	createTorus(1.0f, 3.0f, 6, 12);

	//CarBiggerCube
	objId = 5;

	float amb5[] = { 0.0f, 0.0f, 0.9f, 1.0f };
	float diff5[] = { 0.3f, 0.3f, 0.4f, 1.0f };
	float spec5[] = { 0.9f, 0.9f, 0.9f, 1.0f };
	shininess = 500.0;

	memcpy(mesh[objId].mat.ambient, amb5, 4 * sizeof(float));
	memcpy(mesh[objId].mat.diffuse, diff5, 4 * sizeof(float));
	memcpy(mesh[objId].mat.specular, spec5, 4 * sizeof(float));
	memcpy(mesh[objId].mat.emissive, emissive, 4 * sizeof(float));
	mesh[objId].mat.shininess = shininess;
	mesh[objId].mat.texCount = texcount;
	createCube();

	//CarSmallerCube
	objId = 6;

	float amb6[] = { 0.0f, 0.4f, 0.0f, 1.0f };
	float diff6[] = { 0.0f, 0.8f, 0.0f, 1.0f };
	float spec6[] = { 0.9f, 0.9f, 0.9f, 1.0f };
	shininess = 500.0;

	memcpy(mesh[objId].mat.ambient, amb6, 4 * sizeof(float));
	memcpy(mesh[objId].mat.diffuse, diff6, 4 * sizeof(float));
	memcpy(mesh[objId].mat.specular, spec6, 4 * sizeof(float));
	memcpy(mesh[objId].mat.emissive, emissive, 4 * sizeof(float));
	mesh[objId].mat.shininess = shininess;
	mesh[objId].mat.texCount = texcount;
	createCube();

	car.addMesh(&mesh[4]);
	car.addMesh(&mesh[5]);
	car.addMesh(&mesh[6]);

	//Orange
	objId = 7;

	float amb7[] = { 0.7f, 0.3f, 0.0f, 1.0f };
	float diff7[] = { 0.7f, 0.4f, 0.0f, 1.0f };
	float spec7[] = { 0.0f, 0.0f, 0.0f, 1.0f };
	shininess = 500.0;

	memcpy(mesh[objId].mat.ambient, amb7, 4 * sizeof(float));
	memcpy(mesh[objId].mat.diffuse, diff7, 4 * sizeof(float));
	memcpy(mesh[objId].mat.specular, spec7, 4 * sizeof(float));
	memcpy(mesh[objId].mat.emissive, emissive, 4 * sizeof(float));
	mesh[objId].mat.shininess = shininess;
	mesh[objId].mat.texCount = texcount;
	createSphere(1.0, 9);
	

	// some GL settings
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glEnable(GL_MULTISAMPLE);
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);

	// create cameras
	PerspectiveCamera* p1 = new PerspectiveCamera(53.13f, 0.1f, 1000.0f);
	_cameras.push_back(p1);
	OrtogonalCamera* ortho = new OrtogonalCamera(-5, 5, -5, 5, -10, 10);
	_cameras.push_back(ortho);
	PerspectiveCamera* p2 = new PerspectiveCamera(53.13f, 0.1f, 1000.0f);
	_cameras.push_back(p2);
}

// ------------------------------------------------------------
//
// Main function
//


int main(int argc, char **argv) {

//  GLUT initialization
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DEPTH|GLUT_DOUBLE|GLUT_RGBA|GLUT_MULTISAMPLE);

	glutInitContextVersion (3, 3);
	glutInitContextProfile (GLUT_CORE_PROFILE );
	glutInitContextFlags(GLUT_FORWARD_COMPATIBLE | GLUT_DEBUG);

	glutInitWindowPosition(100,100);
	glutInitWindowSize(WinX, WinY);
	WindowHandle = glutCreateWindow(CAPTION);


//  Callback Registration
	glutDisplayFunc(renderScene);
	glutReshapeFunc(changeSize);
	glutIdleFunc(idle);

//	Mouse and Keyboard Callbacks
	glutKeyboardFunc(keyPressed);	//Funcao a ser chamada ao pressionar uma tecla 
	glutIgnoreKeyRepeat(1);		//Impedir que, ao pressionar uma tecla continuamente, seja chamada a funcao keyPressed repetidamente
	glutKeyboardUpFunc(keyReleased);	//Funcao a ser chamada ao libertar uma tecla
	glutMouseFunc(processMouseButtons);
	glutMotionFunc(processMouseMotion);
	glutMouseWheelFunc ( mouseWheel ) ;
	glutTimerFunc(0,timer,0);
	glutTimerFunc(0, refresh, 0);


//	return from main loop
	glutSetOption(GLUT_ACTION_ON_WINDOW_CLOSE, GLUT_ACTION_GLUTMAINLOOP_RETURNS);

//	Init GLEW
	glewExperimental = GL_TRUE;
	glewInit();

	printf ("Vendor: %s\n", glGetString (GL_VENDOR));
	printf ("Renderer: %s\n", glGetString (GL_RENDERER));
	printf ("Version: %s\n", glGetString (GL_VERSION));
	printf ("GLSL: %s\n", glGetString (GL_SHADING_LANGUAGE_VERSION));

	if (!setupShaders()) return(1);

 

	init();

	//  GLUT main loop
	glutMainLoop();

	return(0);

}

