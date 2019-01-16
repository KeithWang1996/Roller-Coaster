#include "Curve.h"
#include "Window.h"

Curve::Curve(glm::vec4 p1, glm::vec4 p2, glm::vec4 p3, glm::vec4 p4)
{
	glm::mat4 P = glm::mat4(p1, p2, p3, p4);
	this->p1 = p1;
	this->p2 = p2;
	this->p3 = p3;
	this->p4 = p4;
	for (GLuint i = 0; i <= 150; i++) {
		if (i < 150) {
			indices.push_back(i);
			indices.push_back(i + 1);
		}
		T = glm::vec4(pow(t, 3), pow(t, 2), t, 1);
		t = t + (1.0f / 150.0f);
		glm::vec3 q = P*B*T;
		
		points.push_back(q.x);
		points.push_back(q.y);
		points.push_back(q.z);
		if (i > 0) {
			glm::vec3 p = glm::vec3(points[3*i - 3], points[3*i - 2], points[3*i - 1]);
			length = length + glm::length(q - p);
		}
	}

	toWorld = glm::mat4(1.0f);
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);
	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*(points.size()), &points[0], GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_TRUE, 3 * sizeof(GLfloat), (GLvoid*)0);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint)*indices.size(), &indices[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glBindVertexArray(0);

}

void Curve::draw(GLuint shaderProgram) {
	glm::mat4 modelview = Window::V * toWorld;
	glUniform1i(glGetUniformLocation(shaderProgram, "colormode"), 0);
	glUniform1i(glGetUniformLocation(shaderProgram, "id"), 0);
	uProjection = glGetUniformLocation(shaderProgram, "projection");
	uModelview = glGetUniformLocation(shaderProgram, "modelview");
	uModel = glGetUniformLocation(shaderProgram, "model");

	glUniformMatrix4fv(uProjection, 1, GL_FALSE, &Window::P[0][0]);
	glUniformMatrix4fv(uModelview, 1, GL_FALSE, &modelview[0][0]);
	glUniformMatrix4fv(uModel, 1, GL_FALSE, &toWorld[0][0]);

	glBindVertexArray(VAO);
	glLineWidth(5.0f);
	glDrawElements(GL_LINES , (GLsizei)indices.size(), GL_UNSIGNED_INT, 0);

	glBindVertexArray(0);
}

void Curve::update() {
	t = 0;
	GLfloat l = 0;
	glm::mat4 P = glm::mat4(p1, p2, p3, p4);
	for (GLuint i = 0; i <= 450; i+=3) {
		T = glm::vec4(pow(t, 3), pow(t, 2), t, 1);
		t = t + (1.0f / 150.0f);
		glm::vec3 q = P*B*T;
		points[i] = (q.x);
		points[i+1] = (q.y);
		points[i+2] = (q.z);
		if (i > 0) {
			glm::vec3 p = glm::vec3(points[i - 3], points[i - 2], points[i - 1]);
			l = l + glm::length(q - p);
		}
	}
	length = l;
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*(points.size()), &points[0], GL_STATIC_DRAW);
}

glm::vec3 Curve::position(GLfloat t) {
	T = glm::vec4(pow(t, 3), pow(t, 2), t, 1);
	glm::mat4 P = glm::mat4(p1, p2, p3, p4);
	return P*B*T;
}

Curve::~Curve()
{
}
