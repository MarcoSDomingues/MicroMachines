#include "main.h"

void timer(int value)
{
	//players get 5 points for each second he is able to be on the table
	if (!paused)
		gameScore += 5;

	std::ostringstream oss;
	oss << CAPTION << ": " << FrameCount << " FPS @ (" << WinX << "x" << WinY << ")" 
		<< "              SCORE: " << gameScore;
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
	loadIdentity(PROJECTION);
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

void iterate(int value)
{

	particles.iterate(value);
	glutTimerFunc(33, iterate, 0);


}

// ------------------------------------------------------------
//
// Time Functions
//

void idle() {
	currentTime = glutGet(GLUT_ELAPSED_TIME);
	update(currentTime - previousTime);
	previousTime = currentTime;
	//glutPostRedisplay();
}

void update(double delta_t) {
	if (!paused) {
		car.update(delta_t);

		for (int i = 0; i < orangeArray.size(); i++) {

			if (orangeArray[i].getPosition().getX() >= tableSize || orangeArray[i].getPosition().getX() <= -tableSize) {
				orangeArray[i].setDelayDraw(true);
				if (glutGet(GLUT_ELAPSED_TIME) > auxtimer + 10000) {
					auxtimer = glutGet(GLUT_ELAPSED_TIME);
					orangeArray[i].init();

				}

			}
			orangeArray[i].update(delta_t);
			if (orangeArray[i].checkCollisions(&car)) {
				car.kill();
				remainingLives--;
				if (remainingLives <= 0) {
					pause();
				}
			}
		}

		for (int i = 0; i < staticObjects.size(); i++) {
			if (staticObjects[i]->checkCollisions(&car)) {
				car.setAcceleration(0);
				car.setSpeed(0);
				fireworks = 1;
				particles.init(car.getPosition().getX() - car.getDirection().getX(), 1, car.getPosition().getZ() - car.getDirection().getZ());

			}
		}
		for (int i = 0; i < cheerioArray.size(); i++) {
			if (cheerioArray[i].checkCollisions(&car)) {
				car.setAcceleration(0);
				car.setSpeed(0);
			}
		}

		//Car fall from table
		if (car.getPosition().getX() >= tableSize || car.getPosition().getX() <= -tableSize || car.getPosition().getZ() >= tableSize || car.getPosition().getZ() <= -tableSize) {
			car.kill();
			remainingLives--;
			if (remainingLives <= 0) {
				pause();
			}
		}
	}
}

void pause() {
	paused = !paused;
}

// ------------------------------------------------------------
//
// Render stufff
//

void drawLights() {

	float res[4], resDir[4];
	//Directional Light
	multMatrixPoint(VIEW, _directional_light.getPosition(), res);   //lightPos definido em World Coord so is converted to eye space

	lPos_uniformId[0] = glGetUniformLocation(shader.getProgramIndex(), "Lights[0].l_pos");
	local_uniformId[0] = glGetUniformLocation(shader.getProgramIndex(), "Lights[0].isLocal");
	enabled_uniformId[0] = glGetUniformLocation(shader.getProgramIndex(), "Lights[0].isEnabled");
	spot_uniformId[0] = glGetUniformLocation(shader.getProgramIndex(), "Lights[0].isSpot");
	glUniform1i(local_uniformId[0], _directional_light.isLocal());
	glUniform1i(enabled_uniformId[0], _directional_light.isEnabled());
	glUniform1i(spot_uniformId[0], _directional_light.isSpot());
	glUniform4fv(lPos_uniformId[0], 1, res);

	lPos_uniformId[1] = glGetUniformLocation(shader.getProgramIndex(), "Lights[1].l_pos");
	local_uniformId[1] = glGetUniformLocation(shader.getProgramIndex(), "Lights[1].isLocal");
	enabled_uniformId[1] = glGetUniformLocation(shader.getProgramIndex(), "Lights[1].isEnabled");
	spot_uniformId[1] = glGetUniformLocation(shader.getProgramIndex(), "Lights[1].isSpot");
	multMatrixPoint(VIEW, _lamps[0]->getPosition(), res);   //lightPos definido em World Coord so is converted to eye space
	glUniform1i(local_uniformId[1], _lamps[0]->isLocal());
	glUniform1i(enabled_uniformId[1], _lamps[0]->isEnabled());
	glUniform1i(spot_uniformId[1], _lamps[0]->isSpot());
	glUniform4fv(lPos_uniformId[1], 1, res);

	lPos_uniformId[2] = glGetUniformLocation(shader.getProgramIndex(), "Lights[2].l_pos");
	local_uniformId[2] = glGetUniformLocation(shader.getProgramIndex(), "Lights[2].isLocal");
	enabled_uniformId[2] = glGetUniformLocation(shader.getProgramIndex(), "Lights[2].isEnabled");
	spot_uniformId[2] = glGetUniformLocation(shader.getProgramIndex(), "Lights[2].isSpot");
	multMatrixPoint(VIEW, _lamps[1]->getPosition(), res);   //lightPos definido em World Coord so is converted to eye space
	glUniform1i(local_uniformId[2], _lamps[1]->isLocal());
	glUniform1i(enabled_uniformId[2], _lamps[1]->isEnabled());
	glUniform1i(spot_uniformId[2], _lamps[1]->isSpot());
	glUniform4fv(lPos_uniformId[2], 1, res);

	lPos_uniformId[3] = glGetUniformLocation(shader.getProgramIndex(), "Lights[3].l_pos");
	local_uniformId[3] = glGetUniformLocation(shader.getProgramIndex(), "Lights[3].isLocal");
	enabled_uniformId[3] = glGetUniformLocation(shader.getProgramIndex(), "Lights[3].isEnabled");
	spot_uniformId[3] = glGetUniformLocation(shader.getProgramIndex(), "Lights[3].isSpot");
	multMatrixPoint(VIEW, _lamps[2]->getPosition(), res);   //lightPos definido em World Coord so is converted to eye space
	glUniform1i(local_uniformId[3], _lamps[2]->isLocal());
	glUniform1i(enabled_uniformId[3], _lamps[2]->isEnabled());
	glUniform1i(spot_uniformId[3], _lamps[2]->isSpot());
	glUniform4fv(lPos_uniformId[3], 1, res);

	lPos_uniformId[4] = glGetUniformLocation(shader.getProgramIndex(), "Lights[4].l_pos");
	local_uniformId[4] = glGetUniformLocation(shader.getProgramIndex(), "Lights[4].isLocal");
	enabled_uniformId[4] = glGetUniformLocation(shader.getProgramIndex(), "Lights[4].isEnabled");
	spot_uniformId[4] = glGetUniformLocation(shader.getProgramIndex(), "Lights[4].isSpot");
	multMatrixPoint(VIEW, _lamps[3]->getPosition(), res);   //lightPos definido em World Coord so is converted to eye space
	glUniform1i(local_uniformId[4], _lamps[3]->isLocal());
	glUniform1i(enabled_uniformId[4], _lamps[3]->isEnabled());
	glUniform1i(spot_uniformId[4], _lamps[3]->isSpot());
	glUniform4fv(lPos_uniformId[4], 1, res);

	lPos_uniformId[5] = glGetUniformLocation(shader.getProgramIndex(), "Lights[5].l_pos");
	local_uniformId[5] = glGetUniformLocation(shader.getProgramIndex(), "Lights[5].isLocal");
	enabled_uniformId[5] = glGetUniformLocation(shader.getProgramIndex(), "Lights[5].isEnabled");
	spot_uniformId[5] = glGetUniformLocation(shader.getProgramIndex(), "Lights[5].isSpot");
	multMatrixPoint(VIEW, _lamps[4]->getPosition(), res);   //lightPos definido em World Coord so is converted to eye space
	glUniform1i(local_uniformId[5], _lamps[4]->isLocal());
	glUniform1i(enabled_uniformId[5], _lamps[4]->isEnabled());
	glUniform1i(spot_uniformId[5], _lamps[4]->isSpot());
	glUniform4fv(lPos_uniformId[5], 1, res);

	lPos_uniformId[6] = glGetUniformLocation(shader.getProgramIndex(), "Lights[6].l_pos");
	local_uniformId[6] = glGetUniformLocation(shader.getProgramIndex(), "Lights[6].isLocal");
	enabled_uniformId[6] = glGetUniformLocation(shader.getProgramIndex(), "Lights[6].isEnabled");
	spot_uniformId[6] = glGetUniformLocation(shader.getProgramIndex(), "Lights[6].isSpot");
	multMatrixPoint(VIEW, _lamps[5]->getPosition(), res);   //lightPos definido em World Coord so is converted to eye space
	glUniform1i(local_uniformId[6], _lamps[5]->isLocal());
	glUniform1i(enabled_uniformId[6], _lamps[5]->isEnabled());
	glUniform1i(spot_uniformId[6], _lamps[5]->isSpot());
	glUniform4fv(lPos_uniformId[6], 1, res);

	float dir[4];
	float dir1[4] = { car.getPosition().getX() + car.getDirection().getX(), -1, car.getPosition().getZ() + car.getDirection().getZ(), 1.0f }; //camera3
	float dir2[4] = { car.getDirection().getX(), 8, car.getDirection().getZ(), 1.0f }; //outras cameras
	if (_current_camera == 2) {
		std::copy(dir1, dir1 + 4, dir);
	}
	else {
		std::copy(dir2, dir2 + 4, dir);
	}
	lPos_uniformId[7] = glGetUniformLocation(shader.getProgramIndex(), "Lights[7].l_pos");
	local_uniformId[7] = glGetUniformLocation(shader.getProgramIndex(), "Lights[7].isLocal");
	enabled_uniformId[7] = glGetUniformLocation(shader.getProgramIndex(), "Lights[7].isEnabled");
	spot_uniformId[7] = glGetUniformLocation(shader.getProgramIndex(), "Lights[7].isSpot");
	spotDir_uniformId[7] = glGetUniformLocation(shader.getProgramIndex(), "Lights[7].coneDirection");
	spotCutOff_uniformId[7] = glGetUniformLocation(shader.getProgramIndex(), "Lights[7].spotCutOff");
	spotExp_uniformId[7] = glGetUniformLocation(shader.getProgramIndex(), "Lights[7].spotExponent");
	multMatrixPoint(VIEW, _spotLights[0]->getPosition(), res);   //lightPos definido em World Coord so is converted to eye space
	multMatrixPoint(VIEW, dir, resDir);
	glUniform1i(local_uniformId[7], _spotLights[0]->isLocal());
	glUniform1i(enabled_uniformId[7], _spotLights[0]->isEnabled());
	glUniform1i(spot_uniformId[7], _spotLights[0]->isSpot());
	//glUniform3f(spotDir_uniformId[7], car.getDirection().getX(), car.getDirection().getY() - 1.0f, car.getDirection().getZ());
	glUniform1f(spotCutOff_uniformId[7], _spotLights[0]->getCutOff());
	glUniform1f(spotExp_uniformId[7], _spotLights[0]->getExponent());
	glUniform3fv(spotDir_uniformId[7], 1, resDir);
	glUniform4fv(lPos_uniformId[7], 1, res);

	lPos_uniformId[8] = glGetUniformLocation(shader.getProgramIndex(), "Lights[8].l_pos");
	local_uniformId[8] = glGetUniformLocation(shader.getProgramIndex(), "Lights[8].isLocal");
	enabled_uniformId[8] = glGetUniformLocation(shader.getProgramIndex(), "Lights[8].isEnabled");
	spot_uniformId[8] = glGetUniformLocation(shader.getProgramIndex(), "Lights[8].isSpot");
	spotDir_uniformId[8] = glGetUniformLocation(shader.getProgramIndex(), "Lights[8].coneDirection");
	spotCutOff_uniformId[8] = glGetUniformLocation(shader.getProgramIndex(), "Lights[8].spotCutOff");
	spotExp_uniformId[8] = glGetUniformLocation(shader.getProgramIndex(), "Lights[8].spotExponent");
	multMatrixPoint(VIEW, _spotLights[1]->getPosition(), res);   //lightPos definido em World Coord so is converted to eye space
	multMatrixPoint(VIEW, dir, resDir);
	glUniform1i(local_uniformId[8], _spotLights[1]->isLocal());
	glUniform1i(enabled_uniformId[8], _spotLights[1]->isEnabled());
	glUniform1i(spot_uniformId[8], _spotLights[1]->isSpot());
	//glUniform3f(spotDir_uniformId[8], car.getDirection().getX(), car.getDirection().getY(), car.getDirection().getZ());
	glUniform1f(spotCutOff_uniformId[8], _spotLights[1]->getCutOff());
	glUniform1f(spotExp_uniformId[8], _spotLights[1]->getExponent());
	glUniform3fv(spotDir_uniformId[8], 1, resDir);
	glUniform4fv(lPos_uniformId[8], 1, res);

}

void drawBroccoli() {
	float lookAt[3], objToCamProj[3], upAux[3];
	float angleCosine;

	// objToCamProj is the vector in world coordinates from the 
	// local origin to the camera projected in the XZ plane
	Vector3 direction = car.getDirection();
	float xcamX = 0.0f, xcamZ = 0.0f;

	if (_current_camera == 2) {
		xcamX = car.getPosition().getX() - 2 * direction.getX();
		xcamZ = car.getPosition().getZ() - 2 * direction.getZ();
	}

	glAlphaFunc(GL_GREATER, 0.1);
	glEnable(GL_ALPHA_TEST);
	for (Broccoli b : broccoli) {
		objToCamProj[0] = xcamX - b.getPosition().getX();
		objToCamProj[1] = 0;
		objToCamProj[2] = xcamZ - b.getPosition().getZ();

		lookAt[0] = 0.0f;
		lookAt[1] = 0.0f;
		lookAt[2] = 1.0f;

		normalize(objToCamProj);

		crossProduct(lookAt, objToCamProj, upAux);

		angleCosine = dotProduct(lookAt, objToCamProj);

		if ((angleCosine < 0.99990) && (angleCosine > -0.9999))
		{
			b.setAngle(angleCosine);
			b.setDirection(upAux[0], upAux[1], upAux[2]);
		}

		//draw the BROCCOLI!!!

		glUniform1i(texMode_uniformId, true);

		b.draw(shader, pvm_uniformId, vm_uniformId, normal_uniformId);

		glUniform1i(texMode_uniformId, false);

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
	//glUniform4fv(lPos_uniformId, 1, lightPos); //efeito capacete do mineiro, ou seja lighPos foi definido em eye coord 
	
	_spotLights[0]->setPosition(car.getPosition().getX() + (0.1f * car.getDirection().getX()),
		car.getPosition().getY() + 1.0f,
		car.getPosition().getZ()  + (0.1f * car.getDirection().getZ()),
		1.0f);
	_spotLights[1]->setPosition(car.getPosition().getX() + (0.1f * car.getDirection().getX()),
		car.getPosition().getY() + 1.0f,
		car.getPosition().getZ() + (0.1f * car.getDirection().getZ()),
		1.0f);
	//LIGHTS
	drawLights();

	//Associar os Texture Units aos Objects Texture
	//stone.tga loaded in TU0; checker.tga loaded in TU1;  lightwood.tga loaded in TU2

	//Indicar aos tres samplers do GLSL quais os Texture Units a serem usados
	glUniform1i(tex_loc1, 0);
	glUniform1i(tex_loc2, 1);
	
	for (int i = 0; i < 128; i++) {
		cheerioArray[i].draw(shader, pvm_uniformId, vm_uniformId, normal_uniformId);
	}

	car.draw(shader, pvm_uniformId, vm_uniformId, normal_uniformId);

	butter2.draw(shader, pvm_uniformId, vm_uniformId, normal_uniformId);
	butter1.draw(shader, pvm_uniformId, vm_uniformId, normal_uniformId);

	glEnable(GL_STENCIL_TEST);

	// Draw floor
	glStencilFunc(GL_ALWAYS, 1, 0xFF); // Set any stencil to 1
	glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
	glStencilMask(0xFF); // Write to stencil buffer
	glDepthMask(GL_FALSE); // Don't write to depth buffer
	glClear(GL_STENCIL_BUFFER_BIT); // Clear stencil buffer (0 by default)

	table.draw(shader, pvm_uniformId, vm_uniformId, normal_uniformId);

	// Draw cube reflection
	glStencilFunc(GL_EQUAL, 1, 0xFF); // Pass test if stencil value is 1
	glStencilMask(0x00); // Don't write anything to stencil buffer
	glDepthMask(GL_TRUE); // Write to depth buffer

	glUniform1i(darken_uniformId, true);
	butter1ref.draw(shader, pvm_uniformId, vm_uniformId, normal_uniformId);
	butter2ref.draw(shader, pvm_uniformId, vm_uniformId, normal_uniformId);
	glUniform1i(darken_uniformId, false);

	glDisable(GL_STENCIL_TEST);

	for (int i = 0; i < orangeArray.size(); i++) {
		if (!orangeArray[i].getDelayDraw()) {
			orangeArray[i].draw(shader, pvm_uniformId, vm_uniformId, normal_uniformId);
		}
	}

	glUniform1i(texMode_uniformId, true);
	road.draw(shader, pvm_uniformId, vm_uniformId, normal_uniformId);
	glUniform1i(texMode_uniformId, false);

	drawBroccoli();

	glUniform1i(lightsOff_uniformId, true);
	glUniform1i(texMode_uniformId, true);
	sun.draw(shader, pvm_uniformId, vm_uniformId, normal_uniformId);
	glUniform1i(texMode_uniformId, false);
	glUniform1i(lightsOff_uniformId, false);
	

	if (fireworks) {
		glUniform1i(texMode_uniformId, true);
		glUniform1i(lightsOff_uniformId, true);
		particles.render(textureArray[5], mesh[11], vm_uniformId, pvm_uniformId, normal_uniformId, shader, car.getAngle() - 180);
		glUniform1i(texMode_uniformId, false);
		glUniform1i(lightsOff_uniformId, false);
	}

	cup.draw(shader, pvm_uniformId, vm_uniformId, normal_uniformId);

	FLARE_DEF renderFlare;
	renderFlare.fScale = 1;
	renderFlare.fMaxSize = 1.0;
	renderFlare.nPieces = 4;
	renderFlare.element[0].texture = &textureArray[6];
	renderFlare.element[0].fDistance = 0;
	renderFlare.element[0].fSize = 1.0;
	renderFlare.element[0].argb = 0xff6060ff;

	renderFlare.element[1].texture = &textureArray[7];
	renderFlare.element[1].fDistance = 1.0;
	renderFlare.element[1].fSize = 1.0;
	renderFlare.element[1].argb = 0xff6060ff;

	renderFlare.element[2].texture = &textureArray[8];
	renderFlare.element[2].fDistance = 2.0;
	renderFlare.element[2].fSize = 1.0;
	renderFlare.element[2].argb = 0xff6060ff;

	renderFlare.element[3].texture = &textureArray[9];
	renderFlare.element[3].fDistance = 3.0;
	renderFlare.element[3].fSize = 1.0;
	renderFlare.element[3].argb = 0xff6060ff;

	float sunp[4] = { sun.getPosition().getX(), sun.getPosition().getY(), sun.getPosition().getZ(), 1.0f };
	float view[16];
	float projection[16];
	double viewd[16];
	double projd[16];
	double winx, winy, winz;
	int viewp[4];

	memcpy(view, mMatrix[VIEW], 16 * sizeof(float));
	memcpy(projection, mMatrix[PROJECTION], 16 * sizeof(float));

	for (int i = 0; i < 16; i++) {
		viewd[i] = (double)view[i];
		projd[i] = (double)projection[i];
	}
	glGetIntegerv(GL_VIEWPORT, viewp);
	gluProject(sunp[0], sunp[1], sunp[2], viewd, projd, viewp, &winx, &winy, &winz);
	float sun_pos_x = winx;
	float sun_pos_y = winy;
	if (sun_pos_y <= 400)
		sun_pos_y = -500;

	//HUD stuff
	float ratio = (1.0f * glutGet(GLUT_WINDOW_WIDTH)) / glutGet(GLUT_WINDOW_HEIGHT);
	pushMatrix(PROJECTION); // Save the current matrix
	loadIdentity(PROJECTION); // We initialize the projection matrix as identity
	_hudCamera->update(ratio);
	pushMatrix(VIEW); // Save the current matrix
	loadIdentity(VIEW); // Initialize the model matrix as identity

	glUniform1i(lightsOff_uniformId, true);
	glUniform1i(texMode_uniformId, true);
	std::cout << "angle: " << car.getAngle() << std::endl;
	if (70 < car.getAngle() && car.getAngle() < 230) flare.render(&renderFlare, sun_pos_x, sun_pos_y, WinX / 2, WinY / 2,
				 shader, mesh[11], pvm_uniformId, vm_uniformId, normal_uniformId);
	glUniform1i(texMode_uniformId, false);
	glUniform1i(lightsOff_uniformId, false);

	if (paused) {

		glDepthMask(GL_FALSE);

		glUniform1i(lightsOff_uniformId, true);
		glUniform1i(texMode_uniformId, true);
		if (remainingLives > 0) {
			pauseScreen.draw(shader, pvm_uniformId, vm_uniformId, normal_uniformId);
		}
		else {
			deathScreen.draw(shader, pvm_uniformId, vm_uniformId, normal_uniformId);
		}
		glUniform1i(lightsOff_uniformId, false);
		glUniform1i(texMode_uniformId, false);

		glDepthMask(GL_TRUE);
	}

	glUniform1i(lightsOff_uniformId, true);
	HUDbg.draw(shader, pvm_uniformId, vm_uniformId, normal_uniformId);
	for (int i = 0; i < remainingLives; i++) {
		_lives.at(i)->draw(shader, pvm_uniformId, vm_uniformId, normal_uniformId);
	}
	glUniform1i(lightsOff_uniformId, false);

	popMatrix(VIEW); // Restore the previous matrix
	popMatrix(PROJECTION); // Restore the previous matrix

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

		case 'c': case 'C':
			for (int i = 0; i < _lamps.size(); i++) {
				if (_lamps[i]->isEnabled())
					_lamps[i]->setEnabled(false);
				else
					_lamps[i]->setEnabled(true);
			}
			break;

		case 'h': case 'H':
			for (int i = 0; i < _spotLights.size(); i++) {
				if (_spotLights[i]->isEnabled())
					_spotLights[i]->setEnabled(false);
				else
					_spotLights[i]->setEnabled(true);
			}
			break;
		
		case 'O': case 'o':
			car.left();
			break;

		case 'P': case 'p':
			car.right();
			break;

		case 'Q': case 'q':
			if (!paused) {
				car.accelerate();
			}
			break;
	
		case 'A': case 'a':
			if (!paused) {
				car.reverse();
			}
			break;

		case 'S': case 's':
			if (remainingLives > 0) {
				pause();
			}
			break;
		case 'R': case 'r':
			if (remainingLives <= 0) {
				pause();
				remainingLives = 5;
				gameScore = 0;
			}
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

	darken_uniformId = glGetUniformLocation(shader.getProgramIndex(), "darken");
	texMode_uniformId = glGetUniformLocation(shader.getProgramIndex(), "texMode");
	lightsOff_uniformId = glGetUniformLocation(shader.getProgramIndex(), "lightsOff");
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

	gameScore = 0;

	//Texture Object definition
	glGenTextures(12, textureArray);
	TGA_Texture(textureArray, "stone.tga", 0);
	TGA_Texture(textureArray, "checker.tga", 1);
	TGA_Texture(textureArray, "pause.tga", 2);
	TGA_Texture(textureArray, "death.tga", 3);
	TGA_Texture(textureArray, "tree.tga", 4);
	TGA_Texture(textureArray, "particle.tga", 5);
	TGA_Texture(textureArray, "lensFlare/Flare1.tga", 6);
	TGA_Texture(textureArray, "lensFlare/Flare2.tga", 7);
	TGA_Texture(textureArray, "lensFlare/Flare3.tga", 8);
	TGA_Texture(textureArray, "lensFlare/Flare4.tga", 9);
	TGA_Texture(textureArray, "lensFlare/Sun.tga", 10);
	TGA_Texture(textureArray, "lensFlare/SunLight.tga", 11);

	srand(time(NULL));

	for (int i = 0; i < 10; i++) {
		Car* life = new Car();
		life->setPosition(-5.0f + 0.5f*i, -4.0, 5.0f);
		_lives.push_back(life);
	}
	
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

	staticObjects.push_back(&butter1);
	staticObjects.push_back(&butter2);
	staticObjects.push_back(&butter1ref);
	staticObjects.push_back(&butter2ref);

	for (int i = 0; i < 128; i++) {
		Cheerio* cheerio = new Cheerio();
		cheerioArray.push_back(*cheerio);
	}

	float xBot = 2.4f;		float yBot = 2.5f;
	float xTop = -2.4f;		float yTop = -3.6f;
	float xRight = 2.4f;	float yRight = -2.1f;
	float xLeft = -2.4f;	float yLeft = -2.1f;

	for (int i = 0; i < 17; i++) {
		cheerioArray[i].setPosition(xBot - 0.3f * i, 0.52f, yBot);
		cheerioArray[i+17].setPosition(xBot - 0.3f * i, 0.52f, yBot + 1.2f);
		cheerioArray[i+34].setPosition(xTop + 0.3f * i, 0.52f, yTop);
		cheerioArray[i+51].setPosition(xTop + 0.3f * i, 0.52f, yTop + 1.2f);
	}
	for (int i = 0; i < 15; i++) {
		cheerioArray[i+68].setPosition(xRight, 0.52f, yRight + 0.3f * i);
		cheerioArray[i+83].setPosition(xRight + 1.2f, 0.52f, yRight + 0.3f * i);
		cheerioArray[i+98].setPosition(xLeft, 0.52f, yLeft + 0.3f * i);
		cheerioArray[i+113].setPosition(xLeft - 1.2f, 0.52f, yLeft + 0.3f * i);
	}

	butter1.setPosition(3.6f, 0.5f, 3.0f);
	butter2.setPosition(-3.4f, 0.5f, -4.0f);
	butter1ref.setPosition(3.6f, 0.5f, 3.0f);
	butter2ref.setPosition(-3.4f, 0.5f, -4.0f);

	table.setPosition(-4.5, 0.5f, -4.5f);
	pauseScreen.setPosition(-2.5, -2.5, 5.0f);
	deathScreen.setPosition(-2.5, -2.5, 5.0f);
	HUDbg.setPosition(-5.5, -4.0, 0.0f);

	broccoli[0].setPosition(1.5f, 0.5f, 1.5f);
	broccoli[1].setPosition(1.5f, 0.5f, -1.5f);
	broccoli[2].setPosition(-1.5f, 0.5f, -1.5f);
	broccoli[3].setPosition(-1.5f, 0.5f, 1.5f);

	sun.setPosition(5.0f, 2.0f, -5.0f);

	cup.setPosition(-1.5f, 1.0f, 0.0f);

	speed = Vector3(0.0f, 0.0f, 0.0f);

	// set the camera position based on its spherical coordinates
	camX = r * sin(alpha * 3.14f / 180.0f) * cos(beta * 3.14f / 180.0f);
	camZ = r * cos(alpha * 3.14f / 180.0f) * cos(beta * 3.14f / 180.0f);
	camY = r *   						     sin(beta * 3.14f / 180.0f);

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
	createCube();

	float amb1[] = { 0.1f, 0.1f, 0.1f, 1.0f };
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

	float amb2[] = { 0.2f, 0.2f, 0.2f, 1.0f };
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

	float amb3[] = { 0.1f, 0.075f, 0.0f, 1.0f };
	float diff3[] = { 0.8f, 0.6f, 0.0f, 1.0f };
	float spec3[] = { 0.3f, 0.3f, 0.3f, 1.0f };
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
	float diff4[] = { 0.3f, 0.3f, 0.3f, 1.0f };
	float spec4[] = { 0.5f, 0.5f, 0.5f, 1.0f };
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

	float amb5[] = { 0.025f, 0.025f, 0.1f, 1.0f };
	float diff5[] = { 0.1f, 0.1f, 0.4f, 1.0f };
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

	float amb6[] = { 0.0f, 0.2f, 0.0f, 1.0f };
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

	for (Car* life : _lives) {
		life->addMesh(&mesh[4]);
		life->addMesh(&mesh[5]);
		life->addMesh(&mesh[6]);
	}

	butter1.addMesh(&mesh[5]);
	butter2.addMesh(&mesh[5]);
	butter1ref.addMesh(&mesh[5]);
	butter2ref.addMesh(&mesh[5]);
	butter1ref.reflect();
	butter2ref.reflect();

	table.addMesh(&mesh[0]);

	pauseScreen.addMesh(&mesh[0]);
	pauseScreen.addTexture(textureArray[2]);
	pauseScreen.addTexture(textureArray[2]);

	deathScreen.addMesh(&mesh[0]);
	deathScreen.addTexture(textureArray[3]);
	deathScreen.addTexture(textureArray[3]);

	HUDbg.addMesh(&mesh[2]);

	road.addMesh(&mesh[1]);
	road.addMesh(&mesh[2]);
	road.addTexture(textureArray[0]);
	road.addTexture(textureArray[1]);
	
	for (int i = 0; i < 128; i++) {
		cheerioArray[i].addMesh(&mesh[3]);
	}

	//Orange
	objId = 7;

	float amb7[] = { 0.2f, 0.1f, 0.025f, 1.0f };
	float diff7[] = { 0.8f, 0.4f, 0.1f, 1.0f };
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

	//Quad
	objId = 8;

	float amb8[] = { 0.1f, 0.1f, 0.1f, 1.0f };
	float diff8[] = { 0.6f, 0.6f, 0.6f, 1.0f };

	memcpy(mesh[objId].mat.ambient, amb8, 4 * sizeof(float));
	memcpy(mesh[objId].mat.diffuse, diff8, 4 * sizeof(float));
	createCube();

	for (int i = 0; i < 4; i++) {
		broccoli[i].addMesh(&mesh[8]);
		broccoli[i].addTexture(textureArray[4]);
	}

	//Cup
	objId = 9;

	float amb9[] = { 0.1f, 0.1f, 0.7f, 0.3f };
	float diff9[] = { 0.1f, 0.1f, 0.4f, 0.1f };
	float spec9[] = { 0.9f, 0.9f, 0.9f, 0.3f };
	shininess = 500.0;

	memcpy(mesh[objId].mat.ambient, amb9, 4 * sizeof(float));
	memcpy(mesh[objId].mat.diffuse, diff9, 4 * sizeof(float));
	memcpy(mesh[objId].mat.specular, spec9, 4 * sizeof(float));
	memcpy(mesh[objId].mat.emissive, emissive, 4 * sizeof(float));
	mesh[objId].mat.shininess = shininess;
	mesh[objId].mat.texCount = texcount;
	createTorus(0.9f, 1.0f, 32, 12);

	objId = 10;

	memcpy(mesh[objId].mat.ambient, amb9, 4 * sizeof(float));
	memcpy(mesh[objId].mat.diffuse, diff9, 4 * sizeof(float));
	memcpy(mesh[objId].mat.specular, spec9, 4 * sizeof(float));
	memcpy(mesh[objId].mat.emissive, emissive, 4 * sizeof(float));
	mesh[objId].mat.shininess = shininess;
	mesh[objId].mat.texCount = texcount;
	createSphere(1.0, 9);

	cup.addMesh(&mesh[9]);
	cup.addMesh(&mesh[10]);

	//SUN
	objId = 11;
	float amb11[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	memcpy(mesh[objId].mat.ambient, amb11, 4 * sizeof(float));
	createCube();

	sun.addMesh(&mesh[11]);
	sun.addTexture(textureArray[10]);
	sun.addTexture(textureArray[10]);

	// some GL settings
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glEnable(GL_MULTISAMPLE);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glClearColor(0.5f, 0.5f, 0.5f, 1.0f);

	//lights
	_directional_light.setPosition(0.0f, 1.0f, 0.0f, 0.0f); //4th parameter == 0 => directional light
	_directional_light.setEnabled(true);

	LightSource *l1 = new LightSource();
	l1->setPosition(2.5f, 1.0f, 2.5f, 1.0f);
	l1->setEnabled(true);
	l1->setLocal(true);
	_lamps.push_back(l1);
	
	LightSource *l2 = new LightSource();
	l2->setPosition(2.5f, 1.0f, -2.5f, 1.0f);
	l2->setEnabled(true);
	l2->setLocal(true);
	_lamps.push_back(l2);

	LightSource *l3 = new LightSource();
	l3->setPosition(0.0f, 1.0f, 2.5f, 1.0f);
	l3->setEnabled(true);
	l3->setLocal(true);
	_lamps.push_back(l3);

	LightSource *l4 = new LightSource();
	l4->setPosition(0.0f, 1.0f, -2.5f, 1.0f);
	l4->setEnabled(true);
	l4->setLocal(true);
	_lamps.push_back(l4);

	LightSource *l5 = new LightSource();
	l5->setPosition(-2.5f, 1.0f, 2.5f, 1.0f);
	l5->setEnabled(true);
	l5->setLocal(true);
	_lamps.push_back(l5);

	LightSource *l6 = new LightSource();
	l6->setPosition(-2.5f, 1.0f, -2.5f, 1.0f);
	l6->setEnabled(true);
	l6->setLocal(true);
	_lamps.push_back(l6);

	LightSource *spot1 = new LightSource();
	spot1->setEnabled(true);
	spot1->setLocal(true);
	spot1->setSpot(true);
	float cutOff = 2.0f;
	spot1->setCutOff(cutOff);
	spot1->setExponent(0.2f);
	_spotLights.push_back(spot1);

	LightSource *spot2 = new LightSource();
	spot2->setEnabled(true);
	spot2->setLocal(true);
	spot2->setSpot(true);
	cutOff = 2.0f;
	spot2->setCutOff(cutOff);
	spot2->setExponent(0.2f);
	_spotLights.push_back(spot2);

	// create cameras
	PerspectiveCamera* p1 = new PerspectiveCamera(53.13f, 0.1f, 1000.0f);
	_cameras.push_back(p1);
	OrtogonalCamera* ortho = new OrtogonalCamera(-5, 5, -5, 5, -100, 100);
	_cameras.push_back(ortho);
	PerspectiveCamera* p2 = new PerspectiveCamera(53.13f, 0.1f, 1000.0f);
	_cameras.push_back(p2);
	_hudCamera = new OrtogonalCamera(-5, 5, -5, 5, -100, 100);

	// create geometry and VAO of the quad for particles
	objId = 11;
	mesh[objId].mat.texCount = texcount;
	createQuad(2, 2);

	//particles.init();
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
	glutTimerFunc(0, iterate, 0);

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

