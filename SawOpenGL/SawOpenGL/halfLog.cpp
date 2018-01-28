#include "halfLog.h"

HalfLog::HalfLog(GLfloat radius, GLfloat length, GLuint slices, GLfloat cutSign, GLfloat cutThickness, GLuint whichPart) :
	Log(radius, afterCutLength, slices) {

	this->whichPart = whichPart;
	this->cutSign = cutSign;
	this->cutThickness = cutThickness;
	this->length = length;

	setAfterCutLength(this->cutSign);
}

void HalfLog::setCut(GLfloat cutSign) {
	this->cutSign = cutSign;
	setAfterCutLength(this->cutSign);
}

GLfloat HalfLog::setAfterCutLength(GLfloat cutSign) {
	 return this->whichPart == 1 ? afterCutLength = length / 2 - cutSign
		: afterCutLength = length / 2 + cutSign;
}

void HalfLog::create() {
	Log::create(afterCutLength);
}

GLfloat HalfLog::getAfterCutLength() const {
	return afterCutLength;
}

void HalfLog::shiftTexture() {
	shiftBarkTexture(afterCutLength / length, whichPart);
}
