#pragma once
#include "Log.h"

class HalfLog : public Log {

	GLuint whichPart;
	GLfloat cutThickness;
	GLfloat cutSign;
	GLfloat afterCutLength;
	GLfloat length;
	
	GLfloat setAfterCutLength(GLfloat);

public:
	HalfLog(GLfloat, GLfloat, GLuint, GLfloat, GLfloat, GLuint);
	void setCut(GLfloat);
	void create();
	GLfloat getAfterCutLength() const;
	void shiftTexture();
};