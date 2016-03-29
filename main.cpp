// GLEW
#define GLEW_STATIC
#include <GL/glew.h>
// GLFW
#include <GLFW/glfw3.h>

#include <iostream>
#include "shader.h"

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);

int main() {
	const GLuint width = 1280, height = 1024;
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

	GLFWwindow* window = glfwCreateWindow(width, height, "LearnOpenGL", nullptr, nullptr);
	if (!window) {
		std::cout << "Couldn't create window" << std::endl;
		glfwTerminate();
		return -1;
	}

	glfwMakeContextCurrent(window);

	glewExperimental = GL_TRUE;
	if (glewInit() != GLEW_OK) {
		std::cout << "Couldn't init glew" << std::endl;
		return -1;
	}

	// Load our shaders
	Shader ourShader("shaders/basic_vertex.vs", "shaders/basic_frag.frag");

	// Vertices and setting them up to draw
	GLfloat vertices[] = {
		// positions		// Colors
		-0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, // Left  
		0.5f, -0.5f, 0.0f,  0.0f, 1.0f, 0.0f, // Right 
		0.0f,  0.5f, 0.0f,  0.0f, 0.0f, 1.0f, // Top
	};

	GLuint VBO, VAO;
	//glGenBuffers(1, &EBO);
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	// Bind the Vertex Array Object first, then bind and set vertex buffer(s) and attribute pointer(s).
	glBindVertexArray(VAO);

	// Bind VBO here
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	
	// Position, adjust for color and vertex in same array
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);
	// Color
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);

	glBindBuffer(GL_ARRAY_BUFFER, 0); // Note that this is allowed, the call to glVertexAttribPointer registered VBO as the currently bound vertex buffer object so afterwards we can safely unbind
	glBindVertexArray(0); // Unbind VAO

	glViewport(0, 0, width, height);
	glfwSetKeyCallback(window, key_callback);
	while (!glfwWindowShouldClose(window)) {
		glfwPollEvents(); // input

		glClearColor(0.9f, 0.3f, 0.3f, 1.0f); // clear and draw color
		glClear(GL_COLOR_BUFFER_BIT); // clear color buffer

		// Drawing triangle
		ourShader.Use();
		glBindVertexArray(VAO);
		glDrawArrays(GL_TRIANGLES, 0, 3);
		glBindVertexArray(0);

		glfwSwapBuffers(window);
	}

	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glfwTerminate();
	return 0;
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode) {
	// When a user presses the escape key, we set the WindowShouldClose property to true, 
	// closing the application
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);
}