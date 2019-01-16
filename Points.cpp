#include "Points.h"
#include "Window.h"
static GLuint id_g = 1;
Points::Points(glm::vec4 position1, glm::vec4 position2,GLuint type)
{
	if (type == 2) id = 0;
	else {
		id = id_g;
		id_g++;
	}
	this->type = type;
	this->position1 = position1;
	this->position2 = position2;
	if (type == 0 || type == 1) {//0 for inter points, 1 for handle points
		vertices.push_back(position1.x);
		vertices.push_back(position1.y);
		vertices.push_back(position1.z);
	}
	else {//2 for lines
		vertices.push_back(position1.x);
		vertices.push_back(position1.y);
		vertices.push_back(position1.z);
		vertices.push_back(position2.x);
		vertices.push_back(position2.y);
		vertices.push_back(position2.z);
	}
	toWorld = glm::mat4(1.0f);
	temp = glm:: mat4(1.0f);
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*(vertices.size()), &vertices[0], GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_TRUE, 3 * sizeof(GLfloat), (GLvoid*)0);

	glBindVertexArray(0);
}

void Points::draw(GLuint shaderProgram) {
	glm::mat4 modelview = Window::V * toWorld;
	glUniform1i(glGetUniformLocation(shaderProgram, "id"), id);
	if (type == 0) glUniform1i(glGetUniformLocation(shaderProgram, "colormode"), 1);
	else if (type == 1) glUniform1i(glGetUniformLocation(shaderProgram, "colormode"), 2);
	else glUniform1i(glGetUniformLocation(shaderProgram, "colormode"), 3);
	uProjection = glGetUniformLocation(shaderProgram, "projection");
	uModelview = glGetUniformLocation(shaderProgram, "modelview");
	uModel = glGetUniformLocation(shaderProgram, "model");

	glUniformMatrix4fv(uProjection, 1, GL_FALSE, &Window::P[0][0]);
	glUniformMatrix4fv(uModelview, 1, GL_FALSE, &modelview[0][0]);
	glUniformMatrix4fv(uModel, 1, GL_FALSE, &toWorld[0][0]);

	glBindVertexArray(VAO);
	glLineWidth(2.0f);
	glPointSize(20.0f);
	if (type == 0 || type == 1) glDrawArrays(GL_POINTS, 0, 1);
	else glDrawArrays(GL_LINES, 0, 2);

	glBindVertexArray(0);
}

void Points::update() {
	
	if (type == 0 || type == 1) {//0 for inter points, 1 for handle points
		vertices[0] = position1.x;
		vertices[1] = position1.y;
		vertices[2] = position1.z;
	}
	
	else {//2 for lines
		vertices[0] = position1.x;
		vertices[1] = position1.y;
		vertices[2] = position1.z;
		vertices[3] = position2.x;
		vertices[4] = position2.y;
		vertices[5] = position2.z;
	}

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*(vertices.size()), &vertices[0], GL_STATIC_DRAW);
}

Points::~Points()
{
}
