// Fast.cpp : Defines the entry point for the console application.
//
#define GLFW_DLL

#include <stdio.h>
#include </usr/local/include/GL/glew.h>
#include </usr/local/include/GLFW/glfw3.h>
#include <random>
#include <time.h>
#include <iostream>
#include "Object.h"

#define WIDTH  200
#define HEIGHT 200
GLFWwindow* window;
Object patrick;

void initModel(const char *filename) {
    patrick.readFromFile(filename);
    patrick.init(45.0f, (float)WIDTH/(float)HEIGHT, 1.0f, 10.0f, 0, 0, -4);
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
        "uniform mat4 modelViewProjectionMatrix;"
		"uniform vec3 vertex_colour;"
		"in vec3 position;"
		"smooth out vec3 colour;"
		"void main () {"
		"	colour = vertex_colour;"
		"	gl_Position = modelViewProjectionMatrix * vec4(position, 1.0);"
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
	glLinkProgram(shader_programme);
	glUseProgram(shader_programme);

    GLuint uiVAO[1], uiVBO[5];

	glGenVertexArrays(1, uiVAO);
	glGenBuffers(5, uiVBO); 

	// Setup VAO
	glBindVertexArray(uiVAO[0]);\
    
	// main vertexes
    GLuint positionLocation = glGetAttribLocation(shader_programme, "position");
	float *fQuad_new = new float[patrick.vertices_counter];
	fQuad_new = patrick.vertices_array;
	glBindBuffer(GL_ARRAY_BUFFER, uiVBO[0]);
	glBufferData(GL_ARRAY_BUFFER, patrick.vertices_counter * sizeof(float), &fQuad_new[0], GL_STATIC_DRAW); // ok
    glVertexAttribPointer(positionLocation, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(positionLocation);


//	//color
//	float *fQuadColor = new float[modelLoader.vertexs_counter];
//	for (int i = 0; i < modelLoader.vertexs_counter; i++) {
//		fQuadColor[i] = 0;
//	}
//	glBindBuffer(GL_ARRAY_BUFFER, uiVBO[1]);
//	glBufferData(GL_ARRAY_BUFFER, modelLoader.vertexs_counter * sizeof(float), fQuadColor, GL_STATIC_DRAW);
//	glEnableVertexAttribArray(1);
//	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);

	// indexes
	int *indexes_new = new int[patrick.indices_counter];
	indexes_new = patrick.indices_array; // ok
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, uiVBO[2]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, patrick.indices_counter * sizeof(int), &indexes_new[0], GL_STATIC_DRAW);
    
    
    GLuint matrixLocation = glGetUniformLocation(shader_programme, "modelViewProjectionMatrix");
    glUniformMatrix4fv(matrixLocation, 1, GL_TRUE, patrick.MVPmatrix.x);
    
	
	srand((unsigned int)time(NULL));

	double xPos, yPos;
    float anglex=0, angley=0, anglez=0;

	glClearColor(0.5, 1, 1, 1);
	glViewport(0, 0, WIDTH, HEIGHT);
    time_t time0;
	while (!glfwWindowShouldClose(window)) {
		time0 = clock();
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glBindVertexArray(uiVAO[0]);
		glDrawElements(GL_TRIANGLES, patrick.indices_counter, GL_UNSIGNED_INT, 0);

		glfwPollEvents();
		glfwSwapBuffers(window);
//		if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_1)) {
//			std::cout << "detected left btn" << std::endl;
//			for (int i = 0; i < modelLoader.vertexs_counter; i++) {
//				fQuadColor[i] = rand() % 10 / 10.0;
//			}
//			glBindBuffer(GL_ARRAY_BUFFER, uiVBO[1]);
//			glBufferData(GL_ARRAY_BUFFER, modelLoader.vertexs_counter * sizeof(float), fQuadColor, GL_STATIC_DRAW);
//			glEnableVertexAttribArray(1);
//			glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
//		}
//
		if (glfwGetKey(window, GLFW_KEY_X)) {
            anglex += 0.1;
            patrick.rotate(anglex, angley, anglez);
            glUniformMatrix4fv(matrixLocation, 1, GL_TRUE, patrick.MVPmatrix.x);
		}
        if (glfwGetKey(window, GLFW_KEY_Y)) {
            angley += 0.1;
            patrick.rotate(anglex, angley, anglez);
            glUniformMatrix4fv(matrixLocation, 1, GL_TRUE, patrick.MVPmatrix.x);
        }
        if (glfwGetKey(window, GLFW_KEY_Z)) {
            anglez += 0.1;
            patrick.rotate(anglex, angley, anglez);
            glUniformMatrix4fv(matrixLocation, 1, GL_TRUE, patrick.MVPmatrix.x);
        }
//
//		// œÂÂÏÂ˘ÂÌËÂ Ì‡ ‚ÂÎË˜ËÌÛ
//		glfwGetCursorPos(window, &xPos, &yPos);
//		//std::cout << xPos << ' ' << yPos << std::endl;
		if (glfwGetKey(window, GLFW_KEY_RIGHT)) {
            patrick.move(0.01, 0, 0);
            glUniformMatrix4fv(matrixLocation, 1, GL_TRUE, patrick.MVPmatrix.x);
		}
		if (glfwGetKey(window, GLFW_KEY_LEFT)) {
            patrick.move(-0.01, 0, 0);
            glUniformMatrix4fv(matrixLocation, 1, GL_TRUE, patrick.MVPmatrix.x);
		}
		if (glfwGetKey(window, GLFW_KEY_UP)) {
            patrick.move(0, 0.01, 0);
            glUniformMatrix4fv(matrixLocation, 1, GL_TRUE, patrick.MVPmatrix.x);
		}
		if (glfwGetKey(window, GLFW_KEY_DOWN)) {
            patrick.move(0, -0.01, 0);
            glUniformMatrix4fv(matrixLocation, 1, GL_TRUE, patrick.MVPmatrix.x);
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
