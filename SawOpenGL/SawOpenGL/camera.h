#pragma once
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

const float YAW = -90.0f;
const float PITCH = 0.0f;
const float SPEED = 2.5f;
const float SENSITIVTY = 0.1f;

const float positionOffset = 0.5f;

class Camera {

	void update();

public:
	glm::vec3 position;
	glm::vec3 front;
	glm::vec3 up;
	glm::vec3 right;
	glm::vec3 worldUp;

	GLfloat maxX;
	GLfloat maxY;
	GLfloat maxZ;

	float yaw;
	float pitch;

	float movementSpeed;
	float mouseSensitivity;

	Camera(glm::vec3 = glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3 = glm::vec3(0.0f, 1.0f, 0.0f), float = YAW, float = PITCH);
	Camera(float posX, float posY, float posZ, float upX, float upY, float upZ, float yaw, float pitch);
	void processKeyboard(int, GLfloat);
	void processMouseMovement(GLfloat, GLfloat, GLboolean = GL_TRUE);

	glm::mat4 getView() const;
	void setMaxX(GLfloat);
	void setMaxY(GLfloat);
	void setMaxZ(GLfloat);
	bool checkPosition();
};