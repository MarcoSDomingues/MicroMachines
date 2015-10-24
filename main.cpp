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
#include "TGA.h"
#include "AVTmathLib.h"
#include "VertexAttrDef.h"
#include "Vector3.h"
#include "Camera.h"
#include "LightSource.h"
#include "PerspectiveCamera.h"
#include "OrtogonalCamera.h"
#include "basic_geometry.h"
#include <cstdlib>
#include <ctime>

//include objects
#include "GameObject.h"
#include "Car.h"
#include "Orange.h"
#include "Butter.h"
#include "Table.h"
#include "Road.h"
#include "Cheerio.h"

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
GLint local_uniformId[2];
GLint enabled_uniformId[2];
GLint spot_uniformId[2];
GLint tex_loc1, tex_loc2;
GLint texMode_uniformId;

GLuint textureArray[2];

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
//float lightPos[4] = {0.0f, 1.0f, 0.0f, 0.0f};

// cameras
std::vector<Camera*> _cameras;
int _current_camera = 0;

// Directional light
LightSource _directional_light = LightSource();
LightSource _point_Light = LightSource();

int iteration = 0;
// objects
Car car;
Orange orange1;
Orange orange2;
Orange orange3;
std::vector<Orange> orangeArray;
Butter butter1;
Butter butter2;
Table table;
Road road;
Cheerio cheerio;

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

void update(double delta_t) {
	car.update(delta_t);

	for (int i = 0; i < orangeArray.size(); i++) {

		if ((orangeArray[i].getPosition().getX() >= tableSize || orangeArray[i].getPosition().getX() <= -tableSize)) {
			orangeArray[i].setDelayDraw(true);
			if (glutGet(GLUT_ELAPSED_TIME) > auxtimer + 500) {
				auxtimer = glutGet(GLUT_ELAPSED_TIME);
				orangeArray[i].init();

			}

		}
		orangeArray[i].update(delta_t);
		car.checkCollisions(&orangeArray[i]);
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
	//glUniform4fv(lPos_uniformId, 1, lightPos); //efeito capacete do mineiro, ou seja lighPos foi definido em eye coord 
	
	//Directional Light
	float res[4];
	multMatrixPoint(VIEW, _directional_light.getPosition(), res);   //lightPos definido em World Coord so is converted to eye space
	
	glUniform1i(local_uniformId[0], _directional_light.isLocal());
	glUniform1i(enabled_uniformId[0], _directional_light.isEnabled());
	glUniform1i(spot_uniformId[0], _directional_light.isSpot());
	glUniform4fv(lPos_uniformId, 1, res);

	//Associar os Texture Units aos Objects Texture
	//stone.tga loaded in TU0; checker.tga loaded in TU1;  lightwood.tga loaded in TU2

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, textureArray[0]);

	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, textureArray[1]);

	//Indicar aos tres samplers do GLSL quais os Texture Units a serem usados
	glUniform1i(tex_loc1, 0);
	glUniform1i(tex_loc2, 1);
	
	

	cheerio.draw(shader, pvm_uniformId, vm_uniformId, normal_uniformId);
	glUniform1i(texMode_uniformId, true);
	road.draw(shader, pvm_uniformId, vm_uniformId, normal_uniformId);
	glUniform1i(texMode_uniformId, false);
	
	car.draw(shader, pvm_uniformId, vm_uniformId, normal_uniformId);

	butter1.draw(shader, pvm_uniformId, vm_uniformId, normal_uniformId);
	butter2.draw(shader, pvm_uniformId, vm_uniformId, normal_uniformId);
	
	table.draw(shader, pvm_uniformId, vm_uniformId, normal_uniformId);
	

	for (int i = 0; i < orangeArray.size(); i++) {
		if (!orangeArray[i].getDelayDraw()) {
			orangeArray[i].draw(shader, pvm_uniformId, vm_uniformId, normal_uniformId);
		}
	}
	glBindTexture(GL_TEXTURE_2D, 0);
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

		case 'n': case 'N':
			if (_directional_light.isEnabled())
				_directional_light.setEnabled(false);
			else
				_directional_light.setEnabled(true);
			break;
		
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
	local_uniformId[0] = glGetUniformLocation(shader.getProgramIndex(), "Lights[0].isLocal");
	enabled_uniformId[0] = glGetUniformLocation(shader.getProgramIndex(), "Lights[0].isEnabled");
	spot_uniformId[0] = glGetUniformLocation(shader.getProgramIndex(), "Lights[0].isSpot");

	texMode_uniformId = glGetUniformLocation(shader.getProgramIndex(), "texMode");
	tex_loc1 = glGetUniformLocation(shader.getProgramIndex(), "texmap1");
	tex_loc2 = glGetUniformLocation(shader.getProgramIndex(), "texmap2");
	
	printf("InfoLog for Hello World Shader\n%s\n\n", shader.getAllInfoLogs().c_str());
	
	return(shader.isProgramValid());
}

// ------------------------------------------------------------
//
// Model loading and OpenGL setup
//

void init()
{
	//Texture Object definition

	glGenTextures(2, textureArray);
	TGA_Texture(textureArray, "stone.tga", 0);
	TGA_Texture(textureArray, "checker.tga", 1);

	srand(time(NULL));

	car.setPosition(0.0f, 0.45f, 2.8f);

	carX = car.getPosition().getX();
	carY = car.getPosition().getY();
	carZ = car.getPosition().getZ();
	
	std::vector<Car> cars;
	cars.push_back(car);

	orangeArray.push_back(orange1);
	orangeArray.push_back(orange2);
	orangeArray.push_back(orange3);

	for (int i = 0; i < orangeArray.size(); i++) {
		orangeArray[i].init();
	}

	butter1.setPosition(3.6f, 0.5f, 3.0f);
	butter2.setPosition(-3.4f, 0.5f, -4.0f);

	table.setPosition(-0.5f, 0.0f, -0.5f);

	speed = Vector3(0.0f, 0.0f, 0.0f);

	// set the camera position based on its spherical coordinates
	camX = r * sin(alpha * 3.14f / 180.0f) * cos(beta * 3.14f / 180.0f);
	camZ = r * cos(alpha * 3.14f / 180.0f) * cos(beta * 3.14f / 180.0f);
	camY = r *   						     sin(beta * 3.14f / 180.0f);

	float amb[]= {0.4f, 0.3f, 0.2f, 1.0f};
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

	float amb2[] = { 0.4f, 0.4f, 0.4f, 1.0f };
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

	butter1.addMesh(&mesh[5]);
	butter2.addMesh(&mesh[5]);

	table.addMesh(&mesh[0]);

	road.addMesh(&mesh[1]);
	road.addMesh(&mesh[2]);

	cheerio.addMesh(&mesh[3]);

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

	for (int i = 0; i < orangeArray.size(); i++) {
		orangeArray[i].addMesh(&mesh[7]);
	}
	
	// some GL settings
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glEnable(GL_MULTISAMPLE);
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);

	//lights
	_directional_light.setPosition(0.0f, 1.0f, 0.0f, 0.0f); //4th parameter == 0 => directional light
	_directional_light.setEnabled(true);

	// create cameras
	PerspectiveCamera* p1 = new PerspectiveCamera(53.13f, 0.1f, 1000.0f);
	_cameras.push_back(p1);
	OrtogonalCamera* ortho = new OrtogonalCamera(-5, 5, -5, 5, -100, 100);
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

