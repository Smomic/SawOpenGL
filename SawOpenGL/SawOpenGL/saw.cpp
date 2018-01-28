#pragma once
#include "saw.h"

Saw::Saw(GLuint teethNumber, GLfloat thickness, GLfloat teethHeight, GLfloat width, GLfloat height) :
teethNumber(teethNumber), thickness(thickness), teethHeight(teethHeight), width(width), height(height) {
	create();
	bindVAO();
}

void Saw::create() {
	createTeeth();
	createBlade();
}

void Saw::createTeeth() {
	GLfloat sawBeginning = -width / 2.0f;
	GLfloat sawEnd = width / 2.0f;
	GLfloat lengthBetweenTeeth = -2.0f * sawBeginning / teethNumber;
	GLfloat topOfTooth = sawBeginning + lengthBetweenTeeth / 2;
	GLfloat halfWidth = width / 2.0f;
	GLfloat thickness = this->thickness / 2.0f;
	
	GLuint i = 0;
	GLfloat j, k;
	for (j = sawBeginning, k = topOfTooth; i < 4 * (teethNumber - 1); i += 4, j += lengthBetweenTeeth, k += lengthBetweenTeeth) {

		saveVertexToVector(glm::vec3(thickness, teethHeight, j), glm::vec3(1.0f, -1.0f, 1.0f), glm::vec2(j + halfWidth, teethHeight));
		saveVertexToVector(glm::vec3(thickness, 0.0f, k), glm::vec3(0.0f, -1.0f, 1.0f), glm::vec2(k + halfWidth, 0.0f));

		saveVertexToVector(glm::vec3(-thickness, teethHeight, j), glm::vec3(-1.0f, -1.0f, -1.0f), glm::vec2(j + halfWidth, teethHeight));
		saveVertexToVector(glm::vec3(-thickness, 0.0f, k), glm::vec3(0.0f, -1.0f, -1.0f), glm::vec2(k + halfWidth, 0.0f));

		saveIndicesToVector(i, i + 1, i + 4);
		saveIndicesToVector(i + 2, i + 3, i + 6);
	}

	saveVertexToVector(glm::vec3(thickness, teethHeight, j), glm::vec3(1.0f, -1.0f, 1.0f), glm::vec2(j + halfWidth, teethHeight));
	saveVertexToVector(glm::vec3(thickness, 0.0f, k), glm::vec3(0.0f, -1.0f, 1.0f), glm::vec2(k + halfWidth, 0.0f));
	saveVertexToVector(glm::vec3(-thickness, teethHeight, j), glm::vec3(-1.0f, -1.0f, -1.0f), glm::vec2(j + halfWidth, teethHeight));
	saveVertexToVector(glm::vec3(-thickness, 0.0f, k), glm::vec3(0.0f, -1.0f, -1.0f), glm::vec2(k + halfWidth, 0.0f));

	
	saveVertexToVector(glm::vec3(thickness, teethHeight, sawEnd), glm::vec3(1.0f, -1.0f, 1.0f), glm::vec2(1.0f, teethHeight));
	saveVertexToVector(glm::vec3(-thickness, teethHeight, sawEnd), glm::vec3(-1.0f, -1.0f, -1.0f), glm::vec2(1.0f, teethHeight));

	saveIndicesToVector(i, i + 1, i + 4);
	saveIndicesToVector(i + 2, i + 3, i + 5);
	
	for (i = 0; i < 4 * (teethNumber - 1); i += 4) {
		saveIndicesToVector(i, i + 1, i + 3);
		saveIndicesToVector(i, i + 2, i + 3);
		saveIndicesToVector(i + 1, i + 3, i + 6);
		saveIndicesToVector(i + 1, i + 4, i + 6);
	}
	saveIndicesToVector(i + 1, i + 3, i + 5);
	saveIndicesToVector(i + 1, i + 4, i + 5);

}

void Saw::createBlade() {
	GLfloat sawEnd = width / 2.0f;
	saveVertexToVector(glm::vec3(thickness, teethHeight + height, -sawEnd), glm::vec3(-1.0f, 1.0f, 1.0f), glm::vec2(0.0f, 0.0f));
	saveVertexToVector(glm::vec3(thickness, teethHeight + height, sawEnd), glm::vec3(0.0f, 1.0f, 1.0f), glm::vec2(1.0f, 0.0f));
	saveVertexToVector(glm::vec3(-thickness, teethHeight + height, -sawEnd), glm::vec3(1.0f, 1.0f, -1.0f), glm::vec2(0.0f, 0.0f));
	saveVertexToVector(glm::vec3(-thickness, teethHeight + height, sawEnd), glm::vec3(0.0f, 1.0f, -1.0f), glm::vec2(1.0f, 0.0f));

	saveIndicesToVector(0, 4 * teethNumber, 4 * teethNumber + 3);
	saveIndicesToVector(4 * teethNumber, 4 * teethNumber + 2, 4 * teethNumber + 3);
	saveIndicesToVector(2, 4 * teethNumber + 1, 4 * teethNumber + 4);
	saveIndicesToVector(4 * teethNumber + 1, 4 * teethNumber + 4, 4 * teethNumber + 5);

	saveIndicesToVector(0, 2, 4 * teethNumber + 2);
	saveIndicesToVector(2, 4 * teethNumber + 2, 4 * teethNumber + 4);
	saveIndicesToVector(4 * teethNumber, 4 * teethNumber + 1, 4 * teethNumber + 5);
	saveIndicesToVector(4 * teethNumber, 4 * teethNumber + 3, 4 * teethNumber + 5);
	saveIndicesToVector(4 * teethNumber + 2, 4 * teethNumber + 3, 4 * teethNumber + 5);
	saveIndicesToVector(4 * teethNumber + 2, 4 * teethNumber + 4, 4 * teethNumber + 5);

}
