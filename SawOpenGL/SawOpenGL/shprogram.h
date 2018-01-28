#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

class ShaderProgram{

	GLuint program_id;
	GLuint lightColorLoc;
	GLuint lightPosLoc;
	GLuint viewPosLoc;
	GLuint viewLoc;
	GLuint projectionLoc;
	GLuint ambientLoc;
	GLuint specularLoc;
	GLuint animationLoc;

public:
	// Constructor reads and builds the shader
	ShaderProgram(const GLchar*, const GLchar*);

	// Use the program
	void Use() const
	{
		glUseProgram(get_programID());
	}
	
	// returns program ID
	GLuint get_programID() const
	{
		return program_id;
	}

	void setLightColor(glm::vec3);
	void setLightPosition(glm::vec3);

	void setViewPosition(glm::vec3);
	void setView(glm::mat4);

	void setProjection(glm::mat4);

	void setAmbientLightIntensity(GLfloat);
	void setSpecularLightIntensity(GLfloat);

	void setAnimation(glm::mat4);
};
