#pragma once
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "scene.h"

Scene::Scene(GLfloat width, GLfloat height, GLfloat depth, GLuint densityTexture) :
	Box(width, height, depth, densityTexture) {

	changeSignOfNormals(getVertices());
}