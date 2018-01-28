#pragma once
#include "shape.h"

class Saw : public Shape {

	GLuint teethNumber;
	GLfloat thickness;
	GLfloat teethHeight;
	GLfloat width;
	GLfloat height;

	void createTeeth();
	void createBlade();
	void create();
	
public:
	Saw(GLuint, GLfloat, GLfloat, GLfloat, GLfloat);
};