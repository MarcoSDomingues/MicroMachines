#pragma once
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
#include "HUD.h"
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
#include "Pause.h"
#include "Broccoli.h"

#define CAPTION "MicroMachines"
#define M_PI       3.14159265358979323846f

int WindowHandle = 0;
int WinX = 640, WinY = 480;

unsigned int FrameCount = 0;

VSShaderLib shader;

struct MyMesh mesh[9];
int objId = 0; //id of the object mesh - to be used as index of mesh: mesh[objID] means the current mesh

			   //External array storage defined in AVTmathLib.cpp

			   /// The storage for matrices
extern float mMatrix[COUNT_MATRICES][16];
extern float mCompMatrix[COUNT_COMPUTED_MATRICES][16];

/// The normal matrix
extern float mNormal3x3[9];

bool clicking;

bool paused;

Vector3 speed;

GLint pvm_uniformId;
GLint vm_uniformId;
GLint normal_uniformId;
GLint lPos_uniformId[9];
GLint local_uniformId[9];
GLint enabled_uniformId[9];
GLint spot_uniformId[9];
GLint spotDir_uniformId[9];
GLint spotCutOff_uniformId[9];
GLint spotExp_uniformId[9];

GLint tex_loc1, tex_loc2;
GLint texMode_uniformId;
GLint lightsOff_uniformId;

GLuint textureArray[5];

//incrementar velocidade do jogo
double speed_timer = 0;
double speedInc = 1;
double auxtimer = 0;
double delayDraw[3] = { false, false, false }; //orange delay

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
OrtogonalCamera* _hudCamera;

//HUD
int remainingLives = 5;
std::vector<Car*> _lives;
HUD HUDbg;

// Mouse Tracking Variables
int startX, startY, tracking = 0;

// Camera Spherical Coordinates
float alpha = 90.0f, beta = -30.0f;
float r = 10.0f;

// Frame counting and FPS computation
long myTime, timebase = 0, frame = 0;
char s[32];
//float lightPos[4] = {0.0f, 1.0f, 0.0f, 0.0f};

// cameras
std::vector<Camera*> _cameras;
int _current_camera = 0;

// Directional light
LightSource _directional_light = LightSource();
std::vector<LightSource*> _lamps;
std::vector<LightSource*> _spotLights;

int iteration = 0;
// objects
Car car;
Orange orange1;
Orange orange2;
Orange orange3;
std::vector<Orange> orangeArray;
std::vector<Cheerio> cheerioArray;
Butter butter1;
Butter butter2;
Table table;
Road road;
Broccoli broccoli;
Pause pauseScreen;
Pause deathScreen;

std::vector<StaticObject*> staticObjects;

//FUNCTIONS

void timer(int value);
void refresh(int value);
void changeSize(int w, int h);
void update(double delta_t);
void idle();
void pause();
void drawLights();
void renderScene(void);
void keyPressed(unsigned char key, int xx, int yy);
void keyReleased(unsigned char key, int x, int y);
void processMouseButtons(int button, int state, int xx, int yy);
void processMouseMotion(int xx, int yy);
void mouseWheel(int wheel, int direction, int x, int y);
GLuint setupShaders();
void init();
int main(int argc, char **argv);