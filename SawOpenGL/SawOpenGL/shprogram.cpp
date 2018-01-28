#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
using namespace std;

#define GLEW_STATIC
#include <GL/glew.h>
#include "shprogram.h"

string read_shader_code(const GLchar* shaderPath)
{
	ifstream shader_file;
	shader_file.exceptions(ifstream::badbit);

	shader_file.open(shaderPath);
	stringstream shader_stream;
	shader_stream << shader_file.rdbuf();
	shader_file.close();
	return shader_stream.str();
}

GLuint compile_shader(const GLchar* shaderCode, GLenum shaderType)
{
	GLuint shader_id = glCreateShader(shaderType);
	glShaderSource(shader_id, 1, &shaderCode, NULL);
	glCompileShader(shader_id);

	// Print compile errors if any
	GLint success = 0;
	glGetShaderiv(shader_id, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		GLchar infoLog[512];
		glGetShaderInfoLog(shader_id, sizeof(infoLog), NULL, infoLog);
		string msg = string("Shader compilation: ") + infoLog;
		throw exception(msg.c_str());
	}
	return shader_id;
}

ShaderProgram::ShaderProgram(const GLchar* vertexPath, const GLchar* fragmentPath)
{
	// prepare vertex and fragment shaders
	string vertex_code = read_shader_code(vertexPath);
	GLuint vertex_id = compile_shader(vertex_code.c_str(), GL_VERTEX_SHADER);

	string fragment_code = read_shader_code(fragmentPath);
	GLuint fragment_id = compile_shader(fragment_code.c_str(), GL_FRAGMENT_SHADER);

	// link shader program
	program_id = glCreateProgram();
	glAttachShader(program_id, vertex_id);
	glAttachShader(program_id, fragment_id);
	glLinkProgram(program_id);

	// Print linking errors if any
	GLint success;
	glGetProgramiv(program_id, GL_LINK_STATUS, &success);
	if (!success)
	{
		GLchar infoLog[512];
		glGetProgramInfoLog(program_id, sizeof(infoLog), NULL, infoLog);
		string msg = string("Shader program linking: ") + infoLog;
		throw exception(msg.c_str());
	}

	lightColorLoc = glGetUniformLocation(program_id, "lightColor");
	lightPosLoc = glGetUniformLocation(program_id, "lightPosition");

	viewPosLoc = glGetUniformLocation(program_id, "viewPosition");
	viewLoc = glGetUniformLocation(program_id, "view");
	projectionLoc = glGetUniformLocation(program_id, "projection");

	ambientLoc = glGetUniformLocation(program_id, "ambientLightIntensity");
	specularLoc = glGetUniformLocation(program_id, "specularLightIntensity");

	animationLoc = glGetUniformLocation(program_id, "animation");

	// Delete the shaders as they're linked into our program now and no longer necessery
	glDeleteShader(vertex_id);
	glDeleteShader(fragment_id);
}


void ShaderProgram::setLightColor(glm::vec3 lightColor) {
	glUniform3fv(lightColorLoc, 1, &lightColor[0]);
}

void ShaderProgram::setLightPosition(glm::vec3 lightPosition) {
	glUniform3fv(lightPosLoc, 1, &lightPosition[0]);
}

void ShaderProgram::setViewPosition(glm::vec3 viewPosition) {
	glUniform3fv(viewPosLoc, 1, &viewPosition[0]);
}

void ShaderProgram::setView(glm::mat4 view) {
	glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
}

void ShaderProgram::setProjection(glm::mat4 projection) {
	glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));
}

void ShaderProgram::setAmbientLightIntensity(GLfloat ambientLightIntensity) {
	glUniform1f(ambientLoc, ambientLightIntensity);
}

void ShaderProgram::setSpecularLightIntensity(GLfloat specularLightIntensity) {
	glUniform1f(specularLoc, specularLightIntensity);
}

void ShaderProgram::setAnimation(glm::mat4 animation) {
	glUniformMatrix4fv(animationLoc, 1, GL_FALSE, glm::value_ptr(animation));
}
