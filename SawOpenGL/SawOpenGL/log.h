#pragma once
#include "shape.h"

class Log {

	GLfloat radius;
	GLfloat length;
	GLuint slices;

	Shape* crossSection;
	Shape* bark;

	void createCrossSections();
	void createBark();
	void create();
	
public:
	Log(GLfloat, GLfloat, GLuint);
	void create(GLfloat);
	void draw();
	void shiftBarkTexture(GLfloat, GLuint);
	void setSectionTexture(std::string, GLuint);
	void setBarkTexture(std::string, GLuint);

};
