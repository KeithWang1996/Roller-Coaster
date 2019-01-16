#ifndef CURVE_H
#define CURVE_H

#define GLFW_INCLUDE_GLEXT
#ifdef __APPLE__
#define GLFW_INCLUDE_GLCOREARB
#else
#include <GL/glew.h>
#endif
#include <GLFW/glfw3.h>
// Use of degrees is deprecated. Use radians instead.
#ifndef GLM_FORCE_RADIANS
#define GLM_FORCE_RADIANS
#endif
#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <vector>
#include <math.h>
class Curve
{
public:
	glm::mat4 toWorld;
	GLuint VBO, VAO, EBO, VBO2;
	GLuint uProjection, uModelview, uModel;
	GLfloat t = 0.0f;
	GLfloat length = 0.0f;
	glm::vec4 p1;
	glm::vec4 p2;
	glm::vec4 p3;
	glm::vec4 p4;
	std::vector<GLfloat> points;
	std::vector<GLuint> indices;
	glm::vec4 T = glm::vec4(pow(t, 3), pow(t, 2), t, 1);
	glm::mat4 B = glm::mat4(-1, 3, -3, 1,
	                         3, -6, 3, 0,
	                        -3, 3, 0, 0, 
		                     1, 0, 0, 0 );
	Curve(glm::vec4 p1, glm::vec4 p2, glm::vec4 p3, glm::vec4 p4);
	~Curve();
	void draw(GLuint shaderProgram);
	glm::vec3 position(GLfloat t);
	void update();
};
#endif

