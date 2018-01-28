#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <map>

#include "shprogram.h"
#include "box.h"
#include "camera.h"
#include "halfLog.h"
#include "log.h"
#include "saw.h"
#include "scene.h"

void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);
void do_movement();

bool keys[1024];

const GLuint WIDTH = 1200, HEIGHT = 900;

const GLfloat sceneX = 10.0f;
const GLfloat sceneY = 5.0f;
const GLfloat sceneZ = 10.0f;

const GLfloat baseX = 3.2f;
const GLfloat baseY = 0.1f;
const GLfloat baseZ = 1.2f;

const GLfloat floorShift = -sceneY / 2.0f + baseY / 2.0f;

const GLfloat legX = 0.1f;
const GLfloat legY = 1.0f;
const GLfloat legZ = 0.05f;
const GLfloat legsDistance = 1.0f;

const GLfloat crossbeamX = 1.0f;
const GLfloat crossbeamY = 0.15f;
const GLfloat crossbeamZ = 0.05f;

const GLfloat logLength = 1.5f;
const GLfloat logRadius = 0.2f;
const GLuint logSlices = 64;
const GLfloat logSpeed = 0.001f;
GLfloat logPos = -baseX / 2.0f;

const GLfloat halfLogSpeed = 0.001f;
GLfloat firstHalfLogMoving = 0.0f;
GLfloat secondHalfLogMoving = 0.0f;
GLfloat gapBetweenHalfs = 0.1f;
GLfloat gap = 0.0f;

const GLfloat sawWidth = 1.5f;
const GLfloat sawHeight = 0.15f;
const GLuint sawTeethNumber = 50;
const GLfloat sawTeethHeight = 0.1f;
const GLfloat sawThickness = 0.01f;
const GLfloat sawSpeed = 10.0f;
const GLfloat sawShift = 0.001f; // level of changing position of saw
const GLfloat deltaDeepness = 0.0005f;
GLfloat sawDeepness = 0.0f;
GLfloat sawMoving = 0.0f;
GLfloat sawPosition = 0.0f;
GLdouble time;

Camera camera(glm::vec3(sceneX / 2.0f - 0.5f, floorShift / 2.0f, sceneZ / 2.0f - 0.5f));
GLfloat deltaTime = 0.0f;
GLfloat lastTime = 0.0f;
GLfloat posX = WIDTH / 2.0f;
GLfloat posY = HEIGHT / 2.0f;

GLfloat ambientLightIntensity = 0.8f;
GLfloat specularSawLightIntensity = 3.0f;
GLfloat specularStandLightIntensity = 2.0f;
GLfloat ligthPosX = 1.0f;
GLfloat ligthPosY = 1.0f + floorShift;
GLfloat ligthPosZ = 1.0f;
GLfloat lighPosShift = 0.05f;

enum FLAG {
	startSaw,
	showLog,
	showHalfLogs,
	createHalfLogs,
	changeSawPosition,
	beCut,
	fallFirstHalfLog,
	startMouse
};
std::map <FLAG, GLboolean> flagMap;


void drawElement(Box* element, glm::mat4 matrix, GLboolean specLight, ShaderProgram* shader) {
	shader->setAnimation(matrix);
	if (specLight == GL_TRUE) {
		shader->setSpecularLightIntensity(specularStandLightIntensity);
		element->draw();
		shader->setSpecularLightIntensity(0.0f);
	}
	else element->draw();
}

void animateLegs(Box* leg, ShaderProgram* shader) {
	glm::mat4 legMatrix, legMatrix2, legMatrix3, legMatrix4;
	legMatrix = glm::translate(legMatrix, glm::vec3(crossbeamX / 2.0f - legX / 2.0f, legY / 2.0f + baseY / 2.0f + floorShift, legZ - legsDistance / 2.0f));
	drawElement(leg, legMatrix, GL_TRUE, shader);
	legMatrix2 = glm::translate(legMatrix2, glm::vec3(-crossbeamX / 2.0f + legX / 2.0f, legY / 2.0f + baseY / 2.0f + floorShift, legZ - legsDistance / 2.0f));
	drawElement(leg, legMatrix2, GL_TRUE, shader);
	legMatrix3 = glm::translate(legMatrix3, glm::vec3(crossbeamX / 2.0f - legX / 2.0f, legY / 2.0f + baseY / 2.0f + floorShift, -legZ + legsDistance / 2.0f));
	drawElement(leg, legMatrix3, GL_TRUE, shader);
	legMatrix4 = glm::translate(legMatrix4, glm::vec3(-crossbeamX / 2.0f + legX / 2.0f, legY / 2.0f + baseY / 2.0f + floorShift, -legZ + legsDistance / 2.0f));
	drawElement(leg, legMatrix4, GL_TRUE, shader);
}

void animateCrossbeams(Box* crossbeam, ShaderProgram* shader) {
	glm::mat4 crossbeamMatrix, crossbeamMatrix2;
	crossbeamMatrix = glm::translate(crossbeamMatrix, glm::vec3(0.0f, -crossbeamY / 2.0f + baseY / 2.0f + legY + floorShift, legZ - legsDistance / 2.0f));
	drawElement(crossbeam, crossbeamMatrix, GL_TRUE, shader);
	crossbeamMatrix2 = glm::translate(crossbeamMatrix2, glm::vec3(0.0f, -crossbeamY / 2.0f + baseY / 2.0f + legY + floorShift, -legZ + legsDistance / 2.0f));
	drawElement(crossbeam, crossbeamMatrix2, GL_TRUE, shader);
}


void animateScene(Box* base, Box* leg, Box* crossbeam, Scene* scene, glm::mat4* baseMove, glm::mat4* sceneMove, ShaderProgram* shader) {
	drawElement(scene, *sceneMove, GL_FALSE, shader);
	drawElement(base, *baseMove, GL_FALSE, shader);
	animateLegs(leg, shader);
	animateCrossbeams(crossbeam, shader);
}

void animateSaw(Saw* saw, ShaderProgram* shader) {
	glm::mat4 sawMoveMatrix;

	if (flagMap[startSaw] == GL_TRUE) {

		time = glfwGetTime();
		sawDeepness -= deltaDeepness;
		sawMoving = sawSpeed * (float)sin(20 * time) / 50.0f;
		sawMoveMatrix = glm::translate(sawMoveMatrix, glm::vec3(sawPosition, legY / 2.0f + sawHeight / 2.0f + crossbeamY / 2.0f + sawDeepness + floorShift, sawMoving));

		if (fabs(sawDeepness) >= legY / 2.0f) {

			flagMap[startSaw] = GL_FALSE;
			flagMap[showLog] = GL_FALSE;
			flagMap[beCut] = GL_TRUE;
			flagMap[changeSawPosition] = GL_FALSE;
			sawDeepness = 0.0f;
		}
	}
	else
		sawMoveMatrix = glm::translate(sawMoveMatrix, glm::vec3(sawPosition, legY / 2.0f + sawHeight / 2.0f + crossbeamY / 2.0f + floorShift, 0.0f));

	shader->setAnimation(sawMoveMatrix);
	shader->setSpecularLightIntensity(specularSawLightIntensity);
	saw->draw();
	shader->setSpecularLightIntensity(0.0f);
}

void animateLog(Log* log, ShaderProgram* shader) {
	glm::mat4 logMoveMatrix;

	if (logPos <= 0.0f)
		logMoveMatrix = glm::translate(logMoveMatrix, glm::vec3(logPos, logRadius + baseY / 2.0f + floorShift, 0.0f));
	else
		logPos = 0.0f;

	shader->setAnimation(logMoveMatrix);
	log->draw();

	if (logPos < 0.0f)
		logPos += logSpeed;
}

void animateHalfLogs(HalfLog* firstHalfLog, HalfLog* secondHalfLog, ShaderProgram* shader) {
	glm::mat4 firstHalfLogMoveMatrix;
	glm::mat4 secondHalfLogMoveMatrix;

	if (flagMap[createHalfLogs] == GL_FALSE) {

		firstHalfLog->setCut(sawPosition);
		firstHalfLog->create();
		firstHalfLog->shiftTexture();
		secondHalfLog->setCut(sawPosition);
		secondHalfLog->create();
		secondHalfLog->shiftTexture();
		flagMap[createHalfLogs] = GL_TRUE;
	}

	if (flagMap[fallFirstHalfLog] == GL_FALSE) {

		firstHalfLogMoveMatrix = glm::translate(firstHalfLogMoveMatrix,
			glm::vec3(firstHalfLogMoving + sawPosition + (logLength / 2 - sawPosition) / 2.0f, logRadius + baseY / 2.0f + floorShift, 0.0f));
		shader->setAnimation(firstHalfLogMoveMatrix);
		firstHalfLog->draw();
	}

	if (gap >= gapBetweenHalfs) {
		secondHalfLogMoving += halfLogSpeed;
		secondHalfLogMoveMatrix = glm::translate(secondHalfLogMoveMatrix,
			glm::vec3(secondHalfLogMoving + sawPosition - (logLength / 2 + sawPosition) / 2.0f, logRadius + baseY / 2.0f + floorShift, 0.0f));
	}
	else
		secondHalfLogMoveMatrix = glm::translate(secondHalfLogMoveMatrix,
		glm::vec3(sawPosition - (logLength / 2 + sawPosition) / 2.0f, logRadius + baseY / 2.0f + floorShift, 0.0f));

	shader->setAnimation(secondHalfLogMoveMatrix);
	secondHalfLog->draw();


	gap += halfLogSpeed;
	firstHalfLogMoving += halfLogSpeed;

	if (firstHalfLogMoving > baseX / 2.0f - firstHalfLog->getAfterCutLength() / 2.0f)
		flagMap[fallFirstHalfLog] = GL_TRUE;

	if (secondHalfLogMoving > baseX / 2.0f + firstHalfLog->getAfterCutLength() / 2.0f + gapBetweenHalfs) {

		logPos = -baseX / 2.0f;
		firstHalfLogMoving = 0.0f;
		secondHalfLogMoving = 0.0f;
		gap = 0.0f;
		flagMap[showLog] = GL_TRUE;
		flagMap[beCut] = GL_FALSE;
		flagMap[changeSawPosition] = GL_TRUE;
		flagMap[createHalfLogs] = GL_FALSE;
		flagMap[fallFirstHalfLog] = GL_FALSE;
	}
}


int main() {
	if (glfwInit() != GL_TRUE) {
		std::cout << "GLFW initialization failed" << std::endl;
		return -1;
	}
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

	try {

		GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "SawOpenGL", nullptr, nullptr);
		if (window == nullptr)
			throw std::exception("GLFW window not created");
		glfwMakeContextCurrent(window);

		glfwSetKeyCallback(window, key_callback);
		glfwSetCursorPosCallback(window, mouse_callback);
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

		glewExperimental = GL_TRUE;
		if (glewInit() != GLEW_OK)
			throw std::exception("GLEW Initialization failed");

		glViewport(0, 0, WIDTH, HEIGHT);
		glEnable(GL_DEPTH_TEST);
		glEnable(GL_LIGHTING);

		ShaderProgram theProgram("shader.vert", "shader.frag");
		Scene scene(sceneX, sceneY, sceneZ);
		scene.setTexture("textures/sceneTexture.png", theProgram.get_programID());
		Saw saw(sawTeethNumber, sawThickness, sawTeethHeight, sawWidth, sawHeight);
		saw.setTexture("textures/metalTexture.png", theProgram.get_programID());
		Log log(logRadius, logLength, logSlices);
		log.setSectionTexture("textures/sectionTexture.png", theProgram.get_programID());
		log.setBarkTexture("textures/barkTexture.png", theProgram.get_programID());
		HalfLog firstHalfLog(logRadius, logLength, logSlices, sawPosition, sawThickness, 1);
		firstHalfLog.setSectionTexture("textures/sectionTexture.png", theProgram.get_programID());
		firstHalfLog.setBarkTexture("textures/barkTexture.png", theProgram.get_programID());
		HalfLog secondHalfLog(logRadius, logLength, logSlices, sawPosition, sawThickness, 2);
		secondHalfLog.setSectionTexture("textures/sectionTexture.png", theProgram.get_programID());
		secondHalfLog.setBarkTexture("textures/barkTexture.png", theProgram.get_programID());
		Box base(baseX, baseY, baseZ);
		base.setTexture("textures/deskWood.png", theProgram.get_programID());
		Box leg(legX, legY, legZ);
		leg.setTexture("textures/redMetalTexture.png", theProgram.get_programID());
		Box crossbeam(crossbeamX, crossbeamY, crossbeamZ);
		crossbeam.setTexture("textures/redMetalTexture.png", theProgram.get_programID());

		glm::mat4 baseMoveMatrix;
		glm::mat4 sceneMoveMatrix;
		baseMoveMatrix = glm::translate(baseMoveMatrix, glm::vec3(0.0f,floorShift, 0.0f));
		sceneMoveMatrix = glm::translate(sceneMoveMatrix, glm::vec3(0.0f, 0.0f, 0.0f));

		flagMap[showLog] = GL_TRUE;
		flagMap[changeSawPosition] = GL_TRUE;
		flagMap[startMouse] = GL_TRUE;
		camera.setMaxX(sceneX);
		camera.setMaxY(sceneY);
		camera.setMaxZ(sceneZ);

		glm::vec3 lightColor(1.0f, 1.0f, 1.0f);


		while (!glfwWindowShouldClose(window)) {

			GLfloat currentTime = (float)glfwGetTime();
			deltaTime = currentTime - lastTime;
			lastTime = currentTime;
			do_movement();
			glClearColor(0.1f, 0.2f, 0.3f, 1.0f);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

			glm::mat4 view, projection;
			view = camera.getView();
			projection = glm::perspective(glm::radians(45.0f), (GLfloat)WIDTH / (GLfloat)HEIGHT, 0.1f, 100.0f);
			glm::vec3 lightPosition(ligthPosX, ligthPosY, ligthPosZ);
			theProgram.Use();
			theProgram.setLightColor(lightColor);
			theProgram.setLightPosition(lightPosition);
			theProgram.setViewPosition(camera.position);
			theProgram.setView(view);
			theProgram.setProjection(projection);

			animateScene(&base, &leg, &crossbeam, &scene, &baseMoveMatrix, &sceneMoveMatrix, &theProgram);
			animateSaw(&saw, &theProgram);
			if (flagMap[showLog] == GL_TRUE) animateLog(&log, &theProgram);
			if (flagMap[beCut] == GL_TRUE) animateHalfLogs(&firstHalfLog, &secondHalfLog, &theProgram);

			theProgram.setAmbientLightIntensity(ambientLightIntensity);

			glBindVertexArray(0);
			glUseProgram(0);
			glfwSwapBuffers(window);
			glfwPollEvents();
		}
		glfwTerminate();
	}
	catch (std::exception ex) {
		std::cout << ex.what() << std::endl;
	}
	glfwTerminate();

	return 0;
}


void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode) {
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);

	if (key == GLFW_KEY_SPACE && action == GLFW_PRESS) {
		if (flagMap[startSaw] == GL_FALSE && flagMap[beCut] == GL_FALSE && legY / 2.0f + sawWidth / 2.0f >= 2.0f * logRadius + baseY / 2.0f && logPos == 0.0f)
			flagMap[startSaw] = GL_TRUE;
	}

	if (key >= 0 && key < 1024) {
		if (action == GLFW_PRESS)
			keys[key] = true;
		else if (action == GLFW_RELEASE)
			keys[key] = false;
	}
}

void do_movement() {
	if (keys[GLFW_KEY_W])
		camera.processKeyboard(1, deltaTime);
	if (keys[GLFW_KEY_S])
		camera.processKeyboard(2, deltaTime);
	if (keys[GLFW_KEY_A])
		camera.processKeyboard(3, deltaTime);
	if (keys[GLFW_KEY_D])
		camera.processKeyboard(4, deltaTime);
	if (keys[GLFW_KEY_Z] && flagMap[changeSawPosition] == GL_TRUE && flagMap[startSaw] == GL_FALSE) {
		if (sawPosition > -logLength / 2.0f + 0.01f && sawPosition > -crossbeamX / 2.0f + legX + sawThickness / 2.0f + 0.01f)
			sawPosition -= sawShift;
	}
	if (keys[GLFW_KEY_X] && flagMap[changeSawPosition] == GL_TRUE && flagMap[startSaw] == GL_FALSE) {
		if (sawPosition < logLength / 2.0f - 0.01f && sawPosition < crossbeamX / 2.0f - legX - sawThickness / 2.0f - 0.01f)
			sawPosition += sawShift;
	}
	if (keys[GLFW_KEY_UP])
		ligthPosZ += lighPosShift;
	if (keys[GLFW_KEY_DOWN])
		ligthPosZ -= lighPosShift;
	if (keys[GLFW_KEY_LEFT])
		ligthPosX -= lighPosShift;
	if (keys[GLFW_KEY_RIGHT])
		ligthPosX += lighPosShift;
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos) {
	if (flagMap[startMouse]) {
		posX = (float)xpos;
		posY = (float)ypos;
		flagMap[startMouse] = false;
	}

	GLfloat xoffset = (float)(xpos - posX);
	GLfloat yoffset = (float)(posY - ypos);

	posX = (float)xpos;
	posY = (float)ypos;

	camera.processMouseMovement(xoffset, yoffset);
}