#pragma once
#include "box.h"

Box::Box(GLfloat width, GLfloat height, GLfloat depth, GLuint densityTexture) :
halfX(width / 2.0f), halfY(height / 2.0f), halfZ(depth / 2.0f) {
	create(densityTexture);
	calculateSurfaceNormals(getVertices());
	bindVAO();
}

void Box::create(GLuint densityTexture) {
	GLfloat d = densityTexture * 1.0f;

	setVertices(std::vector<GLfloat> {
			-halfX, -halfY, halfZ,		0.0f, 0.0f, 0.0f,		0.0f, 0.0f,	// front
			halfX, -halfY, halfZ,		0.0f, 0.0f, 0.0f,		1.0f, 0.0f,
			-halfX, halfY, halfZ,		0.0f, 0.0f, 0.0f,		0.0f, 1.0f,
			halfX, halfY, halfZ,		0.0f, 0.0f, 0.0f,		1.0f, 1.0f,

			-halfX, halfY, -halfZ,		0.0f, 0.0f, 0.0f,		1.0f, 1.0f,	// back
			halfX, halfY, -halfZ,		0.0f, 0.0f, 0.0f,		0.0f, 1.0f,
			-halfX, -halfY, -halfZ,		0.0f, 0.0f, 0.0f,		1.0f, 0.0f,
			halfX, -halfY, -halfZ,		0.0f, 0.0f, 0.0f,		0.0f, 0.0f,

			-halfX, halfY, halfZ,		0.0f, 0.0f, 0.0f,		0.0f, 0.0f,	// top
			halfX, halfY, halfZ,		0.0f, 0.0f, 0.0f,		1.0f, 0.0f,
			-halfX, halfY, -halfZ,		0.0f, 0.0f, 0.0f,		0.0f, 1.0f,
			halfX, halfY, -halfZ,		0.0f, 0.0f, 0.0f,		1.0f, 1.0f,

			-halfX, -halfY, -halfZ,		0.0f, 0.0f, 0.0f,		0.0f, 0.0f,	// bottom
			halfX, -halfY, -halfZ,		0.0f, 0.0f, 0.0f,		1.0f, 0.0f,
			-halfX, -halfY, halfZ,		0.0f, 0.0f, 0.0f,		0.0f, 1.0f,
			halfX, -halfY, halfZ,		0.0f, 0.0f, 0.0f,		1.0f, 1.0f,

			halfX, -halfY, halfZ,		0.0f, 0.0f, 0.0f,		0.0f, 0.0f,	// right
			halfX, -halfY, -halfZ,		0.0f, 0.0f, 0.0f,		1.0f, 0.0f,
			halfX, halfY, halfZ,		0.0f, 0.0f, 0.0f,		0.0f, 1.0f,
			halfX, halfY, -halfZ,		0.0f, 0.0f, 0.0f,		1.0f, 1.0f,

			-halfX, -halfY, -halfZ,		0.0f, 0.0f, 0.0f,		0.0f, 0.0f,	// left
			-halfX, -halfY, halfZ,		0.0f, 0.0f, 0.0f,		1.0f, 0.0f,
			-halfX, halfY, -halfZ,		0.0f, 0.0f, 0.0f,		0.0f, 1.0f,
			-halfX, halfY, halfZ,		0.0f, 0.0f, 0.0f,		1.0f, 1.0f
	});

	setIndices(std::vector<GLuint> {

			0, 1, 2,
			2, 1, 3,
			4, 5, 6,
			6, 5, 7,
			8, 9, 10,
			10, 9, 11,
			12, 13, 14,
			14, 13, 15,
			16, 17, 18,
			18, 17, 19,
			20, 21, 22,
			22, 21, 23
	});
}

void Box::calculateSurfaceNormals(std::vector<GLfloat>& v) {
	int coords = getCoordinatesOfVertices();
	int coords4 = 4 * coords;
	for (int i = 0; i < getVerticesSize(); i += coords4) {
		glm::vec3 p1 = glm::make_vec3(&v[i]);
		glm::vec3 p2 = glm::make_vec3(&v[i + coords]);
		glm::vec3 p3 = glm::make_vec3(&v[i + 2 * coords]);

		glm::vec3 normal = calculateSurfaceNormal(p1, p2, p3);

		for (int j = 0; j < coords4; j += coords) {
			v[i + 3 + j] = normal.x;
			v[i + 4 + j] = normal.y;
			v[i + 5 + j] = normal.z;
		}
	}
}

void Box::changeSignOfNormals(std::vector<GLfloat>& v) {
	for (int i = 0; i < getVerticesSize(); i += getCoordinatesOfVertices()) {
		v[i + 3] *= -1;
		v[i + 4] *= -1;
		v[i + 5] *= -1;
	}
}