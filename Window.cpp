#include "window.h"
#include <list>
#include <time.h>
const char* window_title = "GLFW Starter Project";
int counter;
int bit;
int walk;
int ball;
int cull;
glm::vec3 r_position = glm::vec3(0);
glm::vec3 gaxis;
glm::vec3 gxaxis;
glm::vec3 gyaxis;
unsigned char res[4];
GLfloat gvel;
GLfloat scale;
GLfloat zoom;
Group* satellite;
Skybox * box;
GLint shaderProgram, shaderProgram_sky, shaderProgram_control;
std::vector<glm::vec4> points;
std::vector<Points*> c_points;
// On some systems you need to change this to the absolute path
#define VERTEX_SHADER_PATH "../shader.vert"
#define FRAGMENT_SHADER_PATH "../shader.frag"
#define VERTEX_SHADER_SKY "../shader-skybox.vert"
#define FRAGMENT_SHADER_SKY "../shader-skybox.frag"
#define VERTEX_SHADER_CONTROL "../shader-control.vert"
#define FRAGMENT_SHADER_CONTROL "../shader-control.frag"

// Default camera parameters
glm::vec3 cam_pos(0.0f, 0.0f, 100.0f);		// e  | Position of camera
glm::vec3 cam_look_at(0.0f, 0.0f, 0.0f);	// d  | This is where the camera looks at
glm::vec3 cam_up(0.0f, 1.0f, 0.0f);			// up | What orientation "up" is
int Window::hold = 0;
int Window::mode = 0;
int Window::light_mode;
int Window::width;
int Window::height;
GLdouble Window::prevx;
GLdouble Window::prevy;
GLdouble Window::currx;
GLdouble Window::curry;
glm::mat4 Window::P;
glm::mat4 Window::V;
Curve* curve1;
Curve* curve2;
Curve* curve3;
Curve* curve4;
Curve* curve5;
Curve* curve6;
Curve* curve7;
Curve* curve8;
Curve* curve;
GLfloat t = 0;
GLfloat v = 0;
GLfloat a = 2.0f;
GLfloat c = 0.0001f;
GLfloat d = 0;
glm::vec3 maxPoint;
OBJObject* sphere;
void Window::initialize_objects()
{
	sphere = new OBJObject("sphere.obj");
	
	points.push_back(glm::vec4(-40, 20, 10, 1));//1/
	points.push_back(glm::vec4(-50, 0, 0, 1));//2/
	points.push_back(glm::vec4(-60, -20, -10, 1));//3/
	points.push_back(glm::vec4(-35, 20, -15, 1));//4/
	points.push_back(glm::vec4(-25, 0, -25, 1));//5/
	points.push_back(glm::vec4(-15, -20, -35, 1));//6/
	points.push_back(glm::vec4(-10, 20, -40, 1));//7/
	points.push_back(glm::vec4(0, 0, -50, 1));//8/
	points.push_back(glm::vec4(10, -20, -60, 1));//9/
	points.push_back(glm::vec4(15, 20, -35, 1));//10/
	points.push_back(glm::vec4(25, 0, -25, 1));//11/
	points.push_back(glm::vec4(35, -20, -15, 1));//12/
	points.push_back(glm::vec4(40, 20, -10, 1));//13/
	points.push_back(glm::vec4(50, 0, 0, 1));//14/
	points.push_back(glm::vec4(60, -20, 10, 1));//15/
	points.push_back(glm::vec4(35, 20, 15, 1));//16/
	points.push_back(glm::vec4(25, 0, 25, 1));//17/
	points.push_back(glm::vec4(15, -20, 35, 1));//18/
	points.push_back(glm::vec4(10, 20, 40, 1));//19/
	points.push_back(glm::vec4(0, 0, 50, 1));//20/
	points.push_back(glm::vec4(-10, -20, 60, 1));//21/
	points.push_back(glm::vec4(-15, 20, 35, 1));//22/
	points.push_back(glm::vec4(-25, 0, 25, 1));//23/
	points.push_back(glm::vec4(-35, -20, 15, 1));//24/
	r_position = glm::vec3(points[1]);
	curve1 = new Curve(points[1], points[2], points[3], points[4]);
	
	curve2 = new Curve(points[4], points[5], points[6], points[7]);
	
	curve3 = new Curve(points[7], points[8], points[9], points[10]);
	
	curve4 = new Curve(points[10], points[11], points[12], points[13]);
	
	curve5 = new Curve(points[13], points[14], points[15], points[16]);
	
	curve6 = new Curve(points[16], points[17], points[18], points[19]);
	
	curve7 = new Curve(points[19], points[20], points[21], points[22]);
	
	curve8 = new Curve(points[22], points[23], points[0], points[1]);
	
	for (GLuint i = 0; i < 24; i += 3) {
		Points* temp1 = new Points(points[i], points[i], 1);
		Points* temp2 = new Points(points[i+1], points[i+1], 0);
		Points* temp3 = new Points(points[i+2], points[i+2], 1);
		Points* temp4 = new Points(points[i], points[i + 2], 2);
		c_points.push_back(temp1);
		c_points.push_back(temp2);
		c_points.push_back(temp3);
		c_points.push_back(temp4);
	}
	std::vector<char*> faces = std::vector<char*>{
		"city_rt.ppm",
		"city_lf.ppm",
		"city_up.ppm",
		"city_dn.ppm",
		"city_bk.ppm",
		"city_ft.ppm"
	};
	
	box = new Skybox(faces);
	walk = 0;
	counter = 0;
	bit = 0;
	ball = 0;
	scale = 1.1f;
	zoom = 1.1f;
	cull = 0;
	gxaxis = glm::vec3(1, 0, 0);
	gyaxis = glm::vec3(0, 1, 0);

	r_position = points[1];

	shaderProgram = LoadShaders(VERTEX_SHADER_PATH, FRAGMENT_SHADER_PATH);
	shaderProgram_sky = LoadShaders(VERTEX_SHADER_SKY, FRAGMENT_SHADER_SKY);
	shaderProgram_control = LoadShaders(VERTEX_SHADER_CONTROL, FRAGMENT_SHADER_CONTROL);
	//light_mode = 0;
	//Material init
}

// Treat this as a destructor function. Delete dynamically allocated memory here.
void Window::clean_up()
{
	//delete(cube);
	delete(box);
	delete(sphere);
	delete(curve1);
	delete(curve2);
	delete(curve3);
	delete(curve4);
	delete(curve5);
	delete(curve6);
	delete(curve7);
	delete(curve8);

	for (GLuint i = 0; i < 32; i++) {
		delete(c_points[i]);
	}

	glDeleteProgram(shaderProgram);
	glDeleteProgram(shaderProgram_sky);
	glDeleteProgram(shaderProgram_control);
}

GLFWwindow* Window::create_window(int width, int height)
{
	// Initialize GLFW
	if (!glfwInit())
	{
		fprintf(stderr, "Failed to initialize GLFW\n");
		return NULL;
	}

	// 4x antialiasing
	glfwWindowHint(GLFW_SAMPLES, 4);

#ifdef __APPLE__ // Because Apple hates comforming to standards
	// Ensure that minimum OpenGL version is 3.3
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	// Enable forward compatibility and allow a modern OpenGL context
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

	// Create the GLFW window
	GLFWwindow* window = glfwCreateWindow(width, height, window_title, NULL, NULL);
	
	// Check if the window could not be created
	if (!window)
	{
		fprintf(stderr, "Failed to open GLFW window.\n");
		fprintf(stderr, "Either GLFW is not installed or your graphics card does not support modern OpenGL.\n");
		glfwTerminate();
		return NULL;
	}

	// Make the context of the window
	glfwMakeContextCurrent(window);

	// Set swap interval to 1
	glfwSwapInterval(1);

	// Get the width and height of the framebuffer to properly resize the window
	glfwGetFramebufferSize(window, &width, &height);
	// Call the resize callback to make sure things get drawn immediately
	Window::resize_callback(window, width, height);

	return window;
}

void Window::resize_callback(GLFWwindow* window, int width, int height)
{
#ifdef __APPLE__
	glfwGetFramebufferSize(window, &width, &height); // In case your Mac has a retina display
#endif
	Window::width = width;
	Window::height = height;
	// Set the viewport size. This is the only matrix that OpenGL maintains for us in modern OpenGL!
	glViewport(0, 0, width, height);

	if (height > 0)
	{
		P = glm::perspective(45.0f, (float)width / (float)height, 0.1f, 10000.0f);
		V = glm::lookAt(cam_pos, cam_look_at, cam_up);
	}
}

void Window::idle_callback()
{ 
	
	curve1->p1 = points[1];
	curve1->p2 = points[2];
	curve1->p3 = points[3];
	curve1->p4 = points[4];

	curve2->p1 = points[4];
	curve2->p2 = points[5];
	curve2->p3 = points[6];
	curve2->p4 = points[7];

	curve3->p1 = points[7];
	curve3->p2 = points[8];
	curve3->p3 = points[9];
	curve3->p4 = points[10];

	curve4->p1 = points[10];
	curve4->p2 = points[11];
	curve4->p3 = points[12];
	curve4->p4 = points[13];

	curve5->p1 = points[13];
	curve5->p2 = points[14];
	curve5->p3 = points[15];
	curve5->p4 = points[16];

	curve6->p1 = points[16];
	curve6->p2 = points[17];
	curve6->p3 = points[18];
	curve6->p4 = points[19];

	curve7->p1 = points[19];
	curve7->p2 = points[20];
	curve7->p3 = points[21];
	curve7->p4 = points[22];

	curve8->p1 = points[22];
	curve8->p2 = points[23];
	curve8->p3 = points[0];
	curve8->p4 = points[1];

	GLfloat maxheight = 0;
	GLfloat minv = 1000;
	for (int i = 1; i < 450; i+=3) {
		if (curve1->points[i] > maxheight) {
			maxheight = curve1->points[i];
			maxPoint = glm::vec3(curve1->points[i - 1], curve1->points[i], curve1->points[i+1]);
			d = 1.0f + (i / 3)*0.006667f;
		}
		if (curve2->points[i] > maxheight) {
			maxheight = curve2->points[i];
			maxPoint = glm::vec3(curve2->points[i - 1], curve2->points[i], curve2->points[i + 1]);
			d = 2.0f + (i / 3)*0.006667f;
		}
		if (curve3->points[i] > maxheight) {
			maxheight = curve3->points[i];
			maxPoint = glm::vec3(curve3->points[i - 1], curve3->points[i], curve3->points[i + 1]);
			d = 3.0f + (i / 3)*0.006667f;
		}
		if (curve4->points[i] > maxheight) {
			maxheight = curve4->points[i];
			maxPoint = glm::vec3(curve4->points[i - 1], curve4->points[i], curve4->points[i + 1]);
			d = 4.0f + (i / 3)*0.006667f;

		}
		if (curve5->points[i] > maxheight) {
			maxheight = curve5->points[i];
			maxPoint = glm::vec3(curve5->points[i - 1], curve5->points[i], curve5->points[i + 1]);
			d = 5.0f + (i / 3)*0.006667f;
		}
		if (curve6->points[i] > maxheight) {
			maxheight = curve6->points[i];
			maxPoint = glm::vec3(curve6->points[i - 1], curve6->points[i], curve6->points[i + 1]);
			d = 6.0f + (i / 3)*0.006667f;
		}
		if (curve7->points[i] > maxheight) {
			maxheight = curve7->points[i];
			maxPoint = glm::vec3(curve7->points[i - 1], curve7->points[i], curve7->points[i + 1]);
			d = 7.0f + (i / 3)*0.006667f;
		}
		if (curve8->points[i] > maxheight) {
			maxheight = curve8->points[i];
			maxPoint = glm::vec3(curve8->points[i - 1], curve8->points[i], curve8->points[i + 1]);
			d = 8.0f + (i / 3)*0.006667f;
		}
	}
	
	if (t < 1) {
		v = sqrt(abs(a * 0.00003f * (maxheight - r_position.y))) + c;
		t = t + (50.0f/(curve1->length))*v;
		r_position = curve1->position(t);
	}
	if (t >= 1 && t < 2) {
		v = sqrt(abs(a * 0.00003f * (maxheight - r_position.y))) + c;
	    t = t + (50.0f / (curve2->length))*v;
		r_position = curve2->position(t - 1);
	}
	if (t >= 2 && t < 3) {
		v = sqrt(abs(a * 0.00003f * (maxheight - r_position.y))) + c;
		t = t + (50.0f / (curve3->length))*v;
		r_position = curve3->position(t - 2);
	}
	if (t >= 3 && t < 4) {
		v = sqrt(abs(a * 0.00003f * (maxheight - r_position.y))) + c;
		t = t + (50.0f / (curve4->length))*v;
		r_position = curve4->position(t - 3);
	}
	if (t >= 4 && t < 5) {
		v = sqrt(abs(a * 0.00003f * (maxheight - r_position.y))) + c;
		t = t + (50.0f / (curve5->length))*v;
		r_position = curve5->position(t - 4);
	}
	if (t >= 5 && t < 6) {
		v = sqrt(abs(a * 0.00003f * (maxheight - r_position.y))) + c;
		t = t + (50.0f / (curve6->length))*v;
		r_position = curve6->position(t - 5);
	}
	if (t >= 6 && t < 7) {
		v = sqrt(abs(a * 0.00003f * (maxheight - r_position.y))) + c;
		t = t + (50.0f / (curve7->length))*v;
		r_position = curve7->position(t - 6);
	}
	if (t >= 7 && t < 8) {
		v = sqrt(abs(a * 0.00003f * (maxheight - r_position.y))) + c;
		t = t + (50.0f / (curve8->length))*v;
		r_position = curve8->position(t - 7);
	}
	if (t >= 8) t = 0;
	sphere->toWorld = glm::translate(glm::mat4(1.0f), r_position)*sphere->init();
	
}

void Window::display_callback(GLFWwindow* window)
{
	// Clear the color and depth buffers
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Use the shader of programID
	glUseProgram(shaderProgram);
	
	for (GLuint i = 0; i < 32; i++) {
		c_points[i]->draw(shaderProgram);
		c_points[i]->update();
	}
	
	curve1->draw(shaderProgram);
	curve1->update();
	curve2->draw(shaderProgram);
	curve2->update();
	curve3->draw(shaderProgram);
	curve3->update();
	curve4->draw(shaderProgram);
	curve4->update();
	curve5->draw(shaderProgram);
	curve5->update();
	curve6->draw(shaderProgram);
	curve6->update();
	curve7->draw(shaderProgram);
	curve7->update();
	curve8->draw(shaderProgram);
	curve8->update();

	glUniform3f(glGetUniformLocation(shaderProgram, "viewPos"), cam_pos.x, cam_pos.y, cam_pos.z);
	sphere->draw(shaderProgram);

	glUseProgram(shaderProgram_sky);
	box->draw(shaderProgram_sky);
	glfwSwapBuffers(window);
	// Gets events, including input such as keyboard and mouse or window resizing
	glfwPollEvents();
	// Swap buffers
	
}

void Window::key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	// Check for a key press
	if (action == GLFW_PRESS || action == GLFW_REPEAT)
	{
		// Check if escape was pressed
		if (key == GLFW_KEY_ESCAPE)
		{
			// Close the window. This causes the program to also terminate.
			glfwSetWindowShouldClose(window, GL_TRUE);
		}

		if (key == GLFW_KEY_P) {
			
			if (a != 0) {
				a = 0;
				c = 0;
			}
			else {
				a = 2.0f;
				c = 0.0001f;
			}
			
		}

		if (key == GLFW_KEY_R) {
			t = d - 1;
		}
		if (key == GLFW_KEY_S) {

		}
		if (key == GLFW_KEY_W) {
		}
		if (key == GLFW_KEY_UP) {
			if (mods & GLFW_MOD_SHIFT) {
			}
			else {
				box->axis = box->xaxis;
				box->vel = 2.0f;
				cam_pos = glm::vec3(box->rotate()*glm::vec4(cam_pos, 0.0));
				cam_up = glm::normalize(glm::vec3(box->rotate()*glm::vec4(cam_up, 1.0)));
				V = glm::lookAt(cam_pos, cam_look_at, cam_up);
				box->yaxis = glm::normalize(glm::vec3(box->rotate()*glm::vec4(box->yaxis, 0.0)));
			}
		}
		if (key == GLFW_KEY_DOWN) {
			if (mods & GLFW_MOD_SHIFT) {
			}
			else {
				box->axis = box->xaxis;
				box->vel = -2.0f;
				cam_pos = glm::vec3(box->rotate()*glm::vec4(cam_pos, 0.0));
				cam_up = glm::normalize(glm::vec3(box->rotate()*glm::vec4(cam_up, 1.0)));
				V = glm::lookAt(cam_pos, cam_look_at, cam_up);
				box->yaxis = glm::normalize(glm::vec3(box->rotate()*glm::vec4(box->yaxis, 0.0)));
			}
		}
		if (key == GLFW_KEY_RIGHT) {
			if (mods & GLFW_MOD_SHIFT) {
			}
			else {
				box->axis = box->yaxis;
				box->vel = -2.0f;
				cam_pos = glm::vec3(box->rotate()*glm::vec4(cam_pos, 0.0));
				cam_up = glm::normalize(glm::vec3(box->rotate()*glm::vec4(cam_up, 1.0)));
				V = glm::lookAt(cam_pos, cam_look_at, cam_up);
				box->xaxis = glm::normalize(glm::vec3(box->rotate()*glm::vec4(box->xaxis, 0.0)));
			}
		}
		if (key == GLFW_KEY_LEFT) {
			if (mods & GLFW_MOD_SHIFT) {
			}
			else {
				box->axis = box->yaxis;
				box->vel = 2.0f;
				cam_pos = glm::vec3(box->rotate()*glm::vec4(cam_pos, 0.0));
				cam_up = glm::normalize(glm::vec3(box->rotate()*glm::vec4(cam_up, 1.0)));
				V = glm::lookAt(cam_pos, cam_look_at, cam_up);
				box->xaxis = glm::normalize(glm::vec3(box->rotate()*glm::vec4(box->xaxis, 0.0)));
			}
		}
		if (key == GLFW_KEY_M) {
			if (walk == 0) walk = 1;
			else walk = 0;
		}
		if (key == GLFW_KEY_Z) {
			if (mods & GLFW_MOD_SHIFT) zoom = 1.1;
			else zoom = 1 / 1.1;
			cam_pos = glm::vec3(glm::scale(glm::mat4(1.0f), glm::vec3(zoom, zoom, zoom))*glm::vec4(cam_pos, 0));
			V = glm::lookAt(cam_pos, cam_look_at, cam_up);
		}
		if (key == GLFW_KEY_C) {
			if (cull == 0) cull = 1;
			else cull = 0;
		}
		if (key == GLFW_KEY_3) {
		}
		if (key == GLFW_KEY_0) {
			mode = 0;
		}
		if (key == GLFW_KEY_N) {
		}
	
	}
}

void Window::mouse_button_callback(GLFWwindow* window, int button, int action, int mods) {

	if (button == GLFW_MOUSE_BUTTON_LEFT) {
		if (action == GLFW_PRESS) {
			glfwGetCursorPos(window, &prevx, &prevy);
			glUseProgram(shaderProgram_control);
			box->draw(shaderProgram_control);
			
			for (GLuint i = 0; i < 32; i++) {
				c_points[i]->draw(shaderProgram_control);
			}

			curve1->draw(shaderProgram_control);
			curve2->draw(shaderProgram_control);
			curve3->draw(shaderProgram_control);
			curve4->draw(shaderProgram_control);
			curve5->draw(shaderProgram_control);
			curve6->draw(shaderProgram_control);
			curve7->draw(shaderProgram_control);
			curve8->draw(shaderProgram_control);

			glReadPixels(prevx, height-prevy, 1, 1, GL_RGBA, GL_UNSIGNED_BYTE, &res);
			hold = res[0];
			cout << hold << endl;
			/*
			switch (res[0]) {
			case 0: hold = 0; break;
			case 1: hold = 1; break;
			case 2: hold = 2; break;
			case 3: hold = 3; break;
			case 4: hold = 4; break;
			case 5: hold = 5; break;
			case 6: hold = 6; break;
			case 7: hold = 7; break;
			case 8: hold = 8; break;
			case 9: hold = 9; break;
			case 10: hold = 10; break;
			case 11: hold = 11; break;
			case 12: hold = 12; break;
			case 13: hold = 13; break;
			case 14: hold = 14; break;
			case 15: printf("Picked 15\n"); break;
			case 16: printf("Picked 16\n"); break;
			case 17: printf("Picked 17\n"); break;
			case 18: printf("Picked 18\n"); break;
			case 19: printf("Picked 19\n"); break;
			case 20: printf("Picked 20\n"); break; 
			case 21: printf("Picked 21\n"); break;
			case 22: printf("Picked 22\n"); break;
			case 23: printf("Picked 23\n"); break;
			case 24: printf("Picked 24\n"); break;
			default:printf("Res: %d\n", res[0]);
			
			}
			*/
		}
		else if (action == GLFW_RELEASE) {
			prevx = 0;
			prevy = 0;
			currx = 0;
			curry = 0;
	
			box->vel = 0;
			if (mode == 1) {
				c_points[(hold / 3) * 4]->temp = c_points[(hold / 3) * 4]->toWorld;
				c_points[(hold / 3) * 4 + 2]->temp = c_points[(hold / 3) * 4 + 2]->toWorld;
			}
			else if (mode == 2) c_points[(hold / 3) * 4 + 1]->temp = c_points[(hold / 3) * 4 + 1]->toWorld;
			else if (mode == 3) {
				c_points[(hold / 3) * 4 - 2]->temp = c_points[(hold / 3) * 4 - 2]->toWorld;
				c_points[(hold / 3) * 4 - 4]->temp = c_points[(hold / 3) * 4 - 4]->toWorld;
			}
			hold = 0;
			mode = 0;
		}
	}
	
}


glm::vec3 Window::trackBallMapping(GLfloat x, GLfloat y) {
	glm::vec3 v;
	GLfloat d;
	v.x = (2.0f*x - width) / width;
	v.y = (height - 2.0f*y) / height;
	v.z = 0.0f;
	d = glm::length(v);
	d = (d<1.0f) ? d : 1.0f;
	v.z = sqrt(1.001f - d*d);
	glm::normalize(v);
	return v;
}


void Window::cursor_position_callback(GLFWwindow* window, GLdouble xpos, GLdouble ypos){
	glfwGetCursorPos(window, &currx, &curry);
	//glm::mat4 rotate = glm::mat4(1.0f);
	//if (cam_pos != glm::vec3(0.0f, 0.0f, 100.0f)) {
		glm::vec3 xdir = glm::normalize(glm::cross(-cam_pos, cam_up));
		glm::vec3 ydir = glm::normalize(cam_up);
	//}
		glm::vec3 tomove = glm::vec3(xdir.x*(currx - prevx) / 10.0f, xdir.y*(currx - prevx) / 10.0f, xdir.z*(currx - prevx) / 10.0f) 
			+ glm::vec3(ydir.x*(prevy - curry) / 10.0f, ydir.y*(prevy - curry) / 10.0f, ydir.z*(prevy - curry) / 10.0f);
	if (hold > 0) {
		if (hold % 3 == 1) {
			mode = 1;
			c_points[(hold / 3) * 4]->position1 = glm::translate(glm::mat4(1.0f), tomove)*c_points[(hold / 3) * 4]->position1;
			c_points[(hold / 3) * 4 + 2]->position1 = glm::translate(glm::mat4(1.0f), -tomove)*c_points[(hold / 3) * 4 + 2]->position1;
			c_points[(hold / 3) * 4 + 3]->position1 = c_points[(hold / 3) * 4]->position1;
			c_points[(hold / 3) * 4 + 3]->position2 = c_points[(hold / 3) * 4 + 2]->position1;
			points[hold - 1] = glm::translate(glm::mat4(1.0f), tomove)*points[hold - 1];
			points[hold + 1] = glm::translate(glm::mat4(1.0f), -tomove)*points[hold + 1];

		}
		else if (hold % 3 == 2) {
			mode = 2;
			c_points[(hold / 3) * 4 + 1]->position1 = glm::translate(glm::mat4(1.0f), tomove)*c_points[(hold / 3) * 4+1]->position1;
			c_points[(hold / 3) * 4]->position1 = glm::translate(glm::mat4(1.0f), tomove)*c_points[(hold / 3) * 4]->position1;
			c_points[(hold / 3) * 4 + 2]->position1 = glm::translate(glm::mat4(1.0f), tomove)*c_points[(hold / 3) * 4 + 2]->position1;
			c_points[(hold / 3) * 4 + 3]->position1 = c_points[(hold / 3) * 4]->position1;
			c_points[(hold / 3) * 4 + 3]->position2 = c_points[(hold / 3) * 4 + 2]->position1;
			points[hold - 1] = glm::translate(glm::mat4(1.0f), tomove)*points[hold - 1];
			points[hold - 2] = glm::translate(glm::mat4(1.0f), tomove)*points[hold - 2];
			points[hold] = glm::translate(glm::mat4(1.0f), tomove)*points[hold];
		}
		else {
			mode = 3;
			c_points[(hold / 3) * 4 - 2]->position1 = glm::translate(glm::mat4(1.0f), tomove)*c_points[(hold / 3) * 4 - 2]->position1;
			c_points[(hold / 3) * 4 - 4]->position1 = glm::translate(glm::mat4(1.0f), -tomove)*c_points[(hold / 3) * 4 - 4]->position1;
			c_points[(hold / 3) * 4 - 1]->position1 = c_points[(hold / 3) * 4 - 4]->position1;
			c_points[(hold / 3) * 4 - 1]->position2 = c_points[(hold / 3) * 4 - 2]->position1;
			points[hold - 1] = glm::translate(glm::mat4(1.0f), tomove)*points[hold - 1];
			points[hold - 3] = glm::translate(glm::mat4(1.0f), -tomove)*points[hold - 3];
		}
	}
	prevx = currx;
	prevy = curry;

	/*
	if (hold == 1) {
		box->prev = trackBallMapping(prevx, prevy);
		glfwGetCursorPos(window, &currx, &curry);
		box->curr = trackBallMapping(currx, curry);

		float temp = glm::acos(glm::dot(box->curr, box->prev));
		if (temp > 0.0001f) box->vel = 30*temp;
		box->axis = glm::normalize(glm::cross(box->curr, box->prev));
		cam_pos = glm::vec3(box->rotate()*glm::vec4(cam_pos, 0.0));
		cam_up = glm::normalize(glm::vec3(box->rotate()*glm::vec4(cam_up, 1.0)));
		V = glm::lookAt(cam_pos, cam_look_at, cam_up);
		prevx = currx;
		prevy = curry;
	}
	*/

}
