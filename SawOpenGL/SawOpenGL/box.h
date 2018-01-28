#pragma once
#include "shape.h"

class Box : public Shape {

	GLfloat halfX;
	GLfloat halfY;
	GLfloat halfZ;

	void create(GLuint);
	void calculateSurfaceNormals(std::vector<GLfloat>&);

public:
	Box(GLfloat, GLfloat, GLfloat, GLuint = 1);
	void changeSignOfNormals(std::vector<GLfloat>&);
};