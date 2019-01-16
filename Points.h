#ifndef POINTS_H
#define POINTS_H

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
class Points
{
public:
    GLuint id;
	GLuint type;
	glm::vec4 position1, position2;
	glm::mat4 toWorld;
	glm::mat4 temp;
	std::vector<GLfloat> vertices;
	GLuint VBO, VAO;
	GLuint uProjection, uModelview, uModel;
	void draw(GLuint);
	void update();
	Points(glm::vec4 position1, glm::vec4 position2, GLuint type);
	~Points();
};
#endif

