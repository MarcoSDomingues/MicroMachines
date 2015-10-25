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
int objId = 0; //id of the object mesh - to be used as index of mesh: mesh[objID] means the current mesh

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