#pragma once
#include <GL/glew.h>
#include <SOIL.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <vector>

class Shape {
	
	GLuint VAO;
	GLuint VBO;
	GLuint EBO;

	GLuint texture;
	GLuint coordinatesOfVertices;

	std::vector<GLfloat> vertices;
	std::vector<GLuint> indices;

	void LoadMipmapTexture(GLuint, const char*);
	GLfloat* getVerticesAdress() const;
	GLuint* getIndicesAdress() const;

public:
	Shape();
	~Shape();
	void bindVAO();
	void setTexture(std::string, GLuint);
	void draw() const;

	std::vector<GLfloat>& getVertices();
	std::vector<GLuint>& getIndices();

	void saveVertexToVector(const glm::vec3&, const glm::vec3&, const glm::vec2&);
	void saveIndicesToVector(const GLuint&, const GLuint&, const GLuint&);
	void setVertices(const std::vector<GLfloat>&&);
	void setIndices(const std::vector<GLuint>&&);
	int getVerticesSize() const;
	void clear();
	glm::vec3 calculateSurfaceNormal(const glm::vec3&, const glm::vec3&, const glm::vec3&);
	GLuint getCoordinatesOfVertices() const;
};