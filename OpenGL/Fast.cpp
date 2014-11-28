// Fast.cpp : Defines the entry point for the console application.
//
#define GLFW_DLL

#include <stdio.h>
#include </usr/local/include/GL/glew.h>
#include </usr/local/include/GLFW/glfw3.h>
//#include <glfw3.h>
#include <random>
#include <time.h>
#include <iostream>
#include "ModelLoader.h"

#define WIDTH  640
#define HEIGHT 480
GLFWwindow* window;
ModelLoader modelLoader;

void initModel(const char *filename) {
	modelLoader.read_from_file(filename);
}
int glInit() {
    glewExperimental = GL_TRUE;
	// start GL context and O/S window using the GLFW helper library
	if (!glfwInit()) {
		fprintf(stderr, "ERROR: could not start GLFW3\n");
		return 1;
	}
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
	initModel("Patrick.obj");
	window = glfwCreateWindow(WIDTH, HEIGHT, "FastnSlow", NULL, NULL);
	if (!window) {
		fprintf(stderr, "ERROR: could not open window with GLFW3\n");
		glfwTerminate();
		return 1;
	}
	glfwMakeContextCurrent(window);
    GLenum err = glewInit();
    if (GLEW_OK != err)
    {
        /* Problem: glewInit failed, something is seriously wrong. */
        fprintf(stderr, "Error: %s\n", glewGetErrorString(err));
    }
	// tell GL to only draw onto a pixel if the shape is closer to the viewer
	glEnable(GL_DEPTH_TEST); // enable depth-testing
	glDepthFunc(GL_LESS); // depth-testing interprets a smaller value as "closer"
    return 1;
}
/*

Your 2D transformation then is the combination of the Rotation, Scale, and Translation matrices:

|1, 0, tx|   |cos(theta), -sin(theta), 0|   |sx, 0,  0|
|0, 1, ty| * |sin(theta), cos(theta),  0| * |0,  sy, 0|
|0, 0, 1 |   |0,          0,           1|   |0,  0,  1|

*/

int main() {
	glInit();

	const char* vertex_shader =
		"#version 400\n"
		"layout(location = 0) in vec3 vertex_position;"
		"layout(location = 1) in vec3 vertex_colour;"
		"layout(location = 2) in vec3 modifier_pos;"
		"layout(location = 3) in int angle;"
		"in vec3 vp;"
		"smooth out vec3 colour;"
		"void main () {"
		"	colour = vertex_colour;"
		"	vec3 rot1 = vec3(cos(radians(angle)), sin(radians(angle)), 0);"
		"	vec3 rot2 = vec3(-sin(radians(angle)), cos(radians(angle)), 0);"
		"	vec3 rot3 = vec3(0, 0, 1);"
		"	mat3 rot = mat3(rot1, rot2, rot3);"
		"	gl_Position = vec4 ( ((vertex_position + modifier_pos) * rot), 1.0);"
		"}";

	const char* fragment_shader =
		"#version 400\n"
		"in vec3 colour;"
		"out vec4 frag_colour;"
		"void main () {"
		"	frag_colour = vec4(colour, 1.0);"
		"}";

	GLuint vs = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vs, 1, &vertex_shader, NULL);
	glCompileShader(vs);
    GLint out;
    glGetShaderiv(vs, GL_COMPILE_STATUS, &out);
    GLsizei bufsize = 1024;
    GLsizei length = 1024;
    GLchar infolog;
    glGetShaderInfoLog(vs, bufsize, &length, &infolog);
    for(int i=0; i<length; i++) {
        cout << *(&infolog+i);
    }
	GLuint fs = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fs, 1, &fragment_shader, NULL);
	glCompileShader(fs);

	GLuint shader_programme = glCreateProgram();
	glAttachShader(shader_programme, fs);
	glAttachShader(shader_programme, vs);
	glBindAttribLocation(shader_programme, 0, "vertex_position");
	glBindAttribLocation(shader_programme, 1, "vertex_colour");
    glBindAttribLocation(shader_programme, 2, "modifier_pos");
    glBindAttribLocation(shader_programme, 3, "angle");
	glLinkProgram(shader_programme);
	glUseProgram(shader_programme);

    GLuint uiVAO[1], uiVBO[5];

	glGenVertexArrays(1, uiVAO);
	glGenBuffers(5, uiVBO); 

	// Setup VAO
	glBindVertexArray(uiVAO[0]);
    
	// main vertexes
	float *fQuad_new = new float[modelLoader.vertexs_counter];
	fQuad_new = modelLoader.vertexes_out;
	glBindBuffer(GL_ARRAY_BUFFER, uiVBO[0]);
	glBufferData(GL_ARRAY_BUFFER, modelLoader.vertexs_counter * sizeof(float), &fQuad_new[0], GL_STATIC_DRAW); // ok
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);


	//color
	float *fQuadColor = new float[modelLoader.vertexs_counter];
	for (int i = 0; i < modelLoader.vertexs_counter; i++) {
		fQuadColor[i] = 0;
	}
	glBindBuffer(GL_ARRAY_BUFFER, uiVBO[1]);
	glBufferData(GL_ARRAY_BUFFER, modelLoader.vertexs_counter * sizeof(float), fQuadColor, GL_STATIC_DRAW);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);

	// indexes
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, uiVBO[2]);
	int *indexes_new = new int[modelLoader.indexes_counter];
	indexes_new = modelLoader.indexes; // ok
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, modelLoader.indexes_counter * sizeof(int), &indexes_new[0], GL_STATIC_DRAW);

	// offset array
	float *deltaArray = new float[modelLoader.vertexs_counter];
	for (int i = 0; i < modelLoader.vertexs_counter; i++) {
		deltaArray[i] = 0;
	}
	glBindBuffer(GL_ARRAY_BUFFER, uiVBO[3]);
	glBufferData(GL_ARRAY_BUFFER, modelLoader.vertexs_counter * sizeof(float), &deltaArray[0], GL_STATIC_DRAW);
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, 0);

	//rotate
	float *rotArray = new float[modelLoader.vertexs_counter];
	for (int i = 0; i < modelLoader.vertexs_counter; i++) {
		rotArray[i] = 0;
	}
	glBindBuffer(GL_ARRAY_BUFFER, uiVBO[4]);
	glBufferData(GL_ARRAY_BUFFER, modelLoader.vertexs_counter * sizeof(float), &rotArray[0], GL_STATIC_DRAW);
	glEnableVertexAttribArray(3);
	glVertexAttribPointer(3, 1, GL_FLOAT, GL_FALSE, 0, 0);
	
	srand((unsigned int)time(NULL));

	double xPos, yPos;

	glClearColor(0.5, 1, 1, 1);
	glViewport(0, 0, WIDTH, HEIGHT);
    time_t time0;
	while (!glfwWindowShouldClose(window)) {
		time0 = clock();
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glBindVertexArray(uiVAO[0]);
		glDrawElements(GL_TRIANGLES, modelLoader.indexes_counter, GL_UNSIGNED_INT, 0);

		glfwPollEvents();
		glfwSwapBuffers(window);
		if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_1)) {
			std::cout << "detected left btn" << std::endl;
			for (int i = 0; i < modelLoader.vertexs_counter; i++) {
				fQuadColor[i] = rand() % 10 / 10.0;
			}
			glBindBuffer(GL_ARRAY_BUFFER, uiVBO[1]);
			glBufferData(GL_ARRAY_BUFFER, modelLoader.vertexs_counter * sizeof(float), fQuadColor, GL_STATIC_DRAW);
			glEnableVertexAttribArray(1);
			glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
		}

		if (glfwGetKey(window, GLFW_KEY_R)) {
			std::cout << "detected R btn" << std::endl;
			for (int i = 0; i < modelLoader.vertexs_counter; i++) {	
				if (rotArray[i] == 360) {
					rotArray[i] = 0;
				}
				rotArray[i] += 1;
			}
			cout << "WARNING!!!" << rotArray[0] << "WARNING!!!" << endl;
			glBindBuffer(GL_ARRAY_BUFFER, uiVBO[4]);
			glBufferData(GL_ARRAY_BUFFER, modelLoader.vertexs_counter * sizeof(float), &rotArray[0], GL_STATIC_DRAW);
			glEnableVertexAttribArray(3);
			glVertexAttribPointer(3, 1, GL_FLOAT, GL_FALSE, 0, 0);
		}

		// Перемещение на величину
		glfwGetCursorPos(window, &xPos, &yPos);
		//std::cout << xPos << ' ' << yPos << std::endl;
		if (glfwGetKey(window, GLFW_KEY_RIGHT)) {
			for (int i = 0; i < modelLoader.vertexs_counter; i += 3) {
				deltaArray[i] += 0.01;
			}
			glBindBuffer(GL_ARRAY_BUFFER, uiVBO[3]);
			glBufferData(GL_ARRAY_BUFFER, modelLoader.vertexs_counter * sizeof(float), &deltaArray[0], GL_STATIC_DRAW);
			glEnableVertexAttribArray(2);
			glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, 0);

		}
		if (glfwGetKey(window, GLFW_KEY_LEFT)) {
			for (int i = 0; i < modelLoader.vertexs_counter; i += 3) {
				deltaArray[i] -= 0.01;
			}
			glBindBuffer(GL_ARRAY_BUFFER, uiVBO[3]);
			glBufferData(GL_ARRAY_BUFFER, modelLoader.vertexs_counter * sizeof(float), &deltaArray[0], GL_STATIC_DRAW);
			glEnableVertexAttribArray(2);
			glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, 0);
		}
		if (glfwGetKey(window, GLFW_KEY_UP)) {
			for (int i = 1; i < modelLoader.vertexs_counter; i += 3) {
				deltaArray[i] += 0.01;
			}
			glBindBuffer(GL_ARRAY_BUFFER, uiVBO[3]);
			glBufferData(GL_ARRAY_BUFFER, modelLoader.vertexs_counter * sizeof(float), &deltaArray[0], GL_STATIC_DRAW);
			glEnableVertexAttribArray(2);
			glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, 0);
		}
		if (glfwGetKey(window, GLFW_KEY_DOWN)) {
			for (int i = 1; i < modelLoader.vertexs_counter; i += 3) {
				deltaArray[i] -= 0.01;
			}
			glPushMatrix();	
			glBindBuffer(GL_ARRAY_BUFFER, uiVBO[3]);
			glBufferData(GL_ARRAY_BUFFER, modelLoader.vertexs_counter * sizeof(float), &deltaArray[0], GL_STATIC_DRAW);
			glEnableVertexAttribArray(2);
			glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, 0);

		}
#ifdef __APPLE__
        clock_t start = clock();
        float dif_sec = ((float)start-time0)/CLOCKS_PER_SEC;
        printf("%s: %f\n", "FPS", 1/dif_sec);
#elif _WIN32
        if (clock() - time0 != 0) {
            cout << 1000.0 / (float)(clock() - time0) << endl;
        }
#elif _WIN64
        if (clock() - time0 != 0) {
            cout << 1000.0 / (float)(clock() - time0) << endl;
        }
#endif
	}

	glfwTerminate();
	return 0;
}
