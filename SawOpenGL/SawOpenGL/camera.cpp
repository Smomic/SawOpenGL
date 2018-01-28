#pragma once
#include "camera.h"
Camera::Camera(glm::vec3 position, glm::vec3 up, float yaw, float pitch) :
	front(glm::vec3(0.0f, 0.0f, -1.0f)), movementSpeed(SPEED), mouseSensitivity(SENSITIVTY) {

	this->position = position;
	this->worldUp = up;
	this->yaw = yaw;
	this->pitch = pitch;
	update();
}

Camera::Camera(float posX, float posY, float posZ, float upX, float upY, float upZ, float yaw, float pitch) :
	front(glm::vec3(0.0f, 0.0f, -1.0f)), movementSpeed(SPEED), mouseSensitivity(SENSITIVTY) {

	this->position = glm::vec3(posX, posY, posZ);
	this->worldUp = glm::vec3(upX, upY, upZ);
	this->yaw = yaw;
	this->pitch = pitch;
	update();
}

void Camera::setMaxX(GLfloat maxX) { 
	this->maxX = maxX / 2.0f; 
}

void Camera::setMaxY(GLfloat maxY) {
	this->maxY = maxY / 2.0f; 
}

void Camera::setMaxZ(GLfloat maxZ) { 
	this->maxZ = maxZ / 2.0f; 
}

void Camera::update() {
	glm::vec3 front;
	front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
	front.y = sin(glm::radians(pitch));
	front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
	this->front = glm::normalize(front);
	right = glm::normalize(glm::cross(this->front, worldUp));
	up = glm::normalize(glm::cross(right, this->front));
}

void Camera::processKeyboard(int direction, GLfloat deltaTime) {
	float velocity = movementSpeed * deltaTime;
	switch(direction) {
		
	case 1 :
		if (checkPosition()) position += front * velocity; // FORWARD
		break;

	case 2: 
		if (checkPosition()) position -= front * velocity; // BACK
		break;

	case 3:
		if (checkPosition()) position -= right * velocity; // LEFT
		break;

	case 4:
		if (checkPosition()) position += right * velocity; // RIGHT
		break;

	default: 
		break;
	}

}

bool Camera::checkPosition() {
	if (fabs(position.x) > maxX - positionOffset) { 
		position.x < 0 ? position.x = -maxX + positionOffset :
						 position.x = maxX - positionOffset; 
		return false;
	}

	else if (fabs(position.y) > maxY - positionOffset) {
		position.y < 0 ? position.y = -maxY + positionOffset :
						 position.y = maxY - positionOffset; 
		return false;
	}

	else if (fabs(position.z) > maxZ - positionOffset) { 
		position.z < 0 ? position.z = -maxZ + positionOffset :
						 position.z = maxZ - positionOffset; 
		return false;
	}

	else return true;
}

void Camera::processMouseMovement(GLfloat xoffset, GLfloat yoffset, GLboolean constrainPitch) {
	xoffset *= mouseSensitivity;
	yoffset *= mouseSensitivity;

	yaw += xoffset;
	pitch += yoffset;

	if (constrainPitch) {
		if (pitch > 89.0f)
			pitch = 89.0f;
		if (pitch < -89.0f)
			pitch = -89.0f;
	}
	update();
}

glm::mat4 Camera::getView() const {
	return glm::lookAt(position, position + front, up);
}