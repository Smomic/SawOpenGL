#pragma once
#include "log.h"
#include <cmath>

static const GLfloat pi = 3.14159;

Log::Log(GLfloat radius, GLfloat length, GLuint slices) :
	radius(radius), length(length), slices(slices)
{
	crossSection = new Shape();
	bark = new Shape();
	create(length);
}

void Log::create() {
	createCrossSections();
	createBark();
}

void Log::create(GLfloat length) {
	crossSection->clear();
	bark->clear();
	this->length = length;
	create();
	crossSection->bindVAO();
	bark->bindVAO();
}

void Log::createCrossSections() {
	GLfloat x = length / 2.0f;
	GLfloat delta = (2.0f * pi) / slices;
	GLfloat round = 1.0f / (2.0f * radius);
	GLfloat alfa = -pi / 2.0f;

	for (GLuint i = 0; i < slices; ++i, alfa += delta) {
		crossSection->saveVertexToVector(glm::vec3(x, radius * sin(alfa), radius * cos(alfa)), 
										glm::normalize(glm::vec3(0, radius * sin(alfa), radius * cos(alfa))),
										glm::vec2(round * (radius * sin(alfa) + radius), round * (radius * cos(alfa) + radius)));

		crossSection->saveVertexToVector(glm::vec3(-x, radius * sin(alfa), radius * cos(alfa)),
										glm::normalize(glm::vec3(0, radius * sin(alfa), radius * cos(alfa))),
										glm::vec2(round * (radius * sin(alfa) + radius), round * (radius * cos(alfa) + radius)));

		crossSection->saveIndicesToVector(0, 2*i, 2*(i + 1));
		crossSection->saveIndicesToVector(1, 2 * i + 1, 2 * (i + 1) + 1);
	}
}

void Log::createBark() {
	GLfloat x = length / 2.0f;
	GLfloat delta = (2.0f * pi) / slices;
	GLfloat round = 1.0f / (2.0f * radius);
	GLfloat alfa = -pi / 2.0f;

	for (GLuint i = 0; i < slices; ++i, alfa += delta) {
		bark->saveVertexToVector(glm::vec3(x, radius * sin(alfa), radius * cos(alfa)), glm::vec3(1.0f, 0, 0.0f), glm::vec2(radius * alfa, 0.0f));
		bark->saveVertexToVector(glm::vec3(-x, radius * sin(alfa), radius * cos(alfa)), glm::vec3(-1.0f, 0, 0.0f), glm::vec2(radius * alfa, 1.0f));
	}

	for (GLuint i = 0; i < 2 * slices; ++i)
		bark->saveIndicesToVector(i, i + 1, i + 2);
}

void Log::draw() {
	crossSection->draw();
	bark->draw();
}

void Log::shiftBarkTexture(GLfloat shift, GLuint whichPart) {
	if (whichPart == 1) {
		for (int i = 2 * bark->getCoordinatesOfVertices() - 1; i < bark->getVerticesSize(); i += 2 * bark->getCoordinatesOfVertices())
			bark->getVertices()[i] = shift;
	}
	else {
		shift = 1.0f - shift;
		for (int i = bark->getCoordinatesOfVertices() - 1; i < bark->getVerticesSize(); i += 2 * bark->getCoordinatesOfVertices())
			bark->getVertices()[i] = shift;
	}
	bark->bindVAO();
}

void Log::setSectionTexture(std::string texture, GLuint shaderID) {
	crossSection->setTexture(texture, shaderID);
}

void Log::setBarkTexture(std::string texture, GLuint shaderID) {
	bark->setTexture(texture, shaderID);
}