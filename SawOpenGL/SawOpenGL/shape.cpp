#include "shape.h"

Shape::Shape() : coordinatesOfVertices(8) {}

Shape::~Shape() {

	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);
}

void Shape::draw() const {

	glBindTexture(GL_TEXTURE_2D, texture);
	glBindVertexArray(VAO);
	glDrawElements(GL_TRIANGLES, indices.size() * 3, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}

void Shape::setTexture(std::string texture, GLuint shaderID) {

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	LoadMipmapTexture(GL_TEXTURE0, texture.c_str());
	glUniform1i(glGetUniformLocation(shaderID, "Texture0"), 0);

}

void Shape::LoadMipmapTexture(GLuint texId, const char* fname) {

	int width, height;
	unsigned char* image = SOIL_load_image(fname, &width, &height, 0, SOIL_LOAD_RGB);
	if (image == nullptr)
		throw std::exception("Failed to load texture file");

	glGenTextures(1, &texture);
	glActiveTexture(texId);
	glBindTexture(GL_TEXTURE_2D, texture);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
	glGenerateMipmap(GL_TEXTURE_2D);
	SOIL_free_image_data(image);
	glBindTexture(GL_TEXTURE_2D, 0);
}

void Shape::bindVAO() { 

	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(GLfloat), getVerticesAdress(), GL_STATIC_DRAW);
	
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLuint), getIndicesAdress(), GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, coordinatesOfVertices * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);
	
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, coordinatesOfVertices * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);
	
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, coordinatesOfVertices * sizeof(GLfloat), (GLvoid*)(6 * sizeof(GLfloat)));
	glEnableVertexAttribArray(2);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void Shape::saveVertexToVector(const glm::vec3& position, const glm::vec3& normal, const glm::vec2& texture) {
	
	vertices.emplace_back(position.x);
	vertices.emplace_back(position.y);
	vertices.emplace_back(position.z);

	vertices.emplace_back(normal.x);
	vertices.emplace_back(normal.y);
	vertices.emplace_back(normal.z);

	vertices.emplace_back(texture.x);
	vertices.emplace_back(texture.y);
}

void Shape::saveIndicesToVector(const GLuint& indice1, const GLuint& indice2, const GLuint& indice3) {
	
	indices.emplace_back(indice1);
	indices.emplace_back(indice2);
	indices.emplace_back(indice3);
}

void Shape::setVertices(const std::vector<GLfloat>&& v) {
	vertices = std::move(v);
}

void Shape::setIndices(const std::vector<GLuint>&& i) {
	indices = std::move(i);
}

GLfloat* Shape::getVerticesAdress() const {
	return (GLfloat*)&vertices[0];
}

GLuint* Shape::getIndicesAdress() const {
	return (GLuint*)&indices[0];
}

int Shape::getVerticesSize() const {
	return vertices.size();
}

std::vector<GLuint>& Shape::getIndices(){
	return indices;
}

std::vector<GLfloat>& Shape::getVertices() {
	return vertices;
}

void Shape::clear() {
	vertices.clear();
	indices.clear();
}

glm::vec3 Shape::calculateSurfaceNormal(const glm::vec3 & p1, const glm::vec3 & p2, const glm::vec3 & p3) {
	return glm::cross(p2 - p1, p3 - p1);
}

GLuint Shape::getCoordinatesOfVertices() const {
	return coordinatesOfVertices;
}
