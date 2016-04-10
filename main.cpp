// GLEW
#define GLEW_STATIC
#include <GL/glew.h>
// GLFW
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <SOIL.h>

#include <iostream>
#include <cmath>
#include "shader.h"

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void doMovement();
void runSample4(GLFWwindow* window);

const GLuint WIDTH = 1280, HEIGHT = 1024;

// to get top down perspective
//glm::vec3 cameraPos = glm::vec3(0.0f, 2.0f, 3.0f);
//glm::vec3 cameraFront = glm::vec3(0.0f, -1.0f, -1.0f);

glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 6.0f);
glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);

glm::vec3 pPos(200.0f, 200.0f, 0.0f);

bool keys[1024];

GLfloat deltaTime = 0.0f;	// Time between current frame and last frame
GLfloat lastFrame = 0.0f;  	// Time of last frame

// Used for fps camera
GLfloat lastX = WIDTH/2.0f, lastY = HEIGHT/2.0f;
GLfloat pitch = 0.0f, yaw = -90.0f;
bool firstMouse = true;

int main() {
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

	GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "LearnOpenGL", nullptr, nullptr);
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
	runSample4(window);
}

void runSample4(GLFWwindow* window) {
	// Setup mouse for fps style camera, will not need for game

	// Load our shaders
	Shader ourShader("shaders/default3d.vs", "shaders/basic_frag.frag");
	Shader textureShader("shaders/text_simp.vs", "shaders/text_simp.frag");

	// Vertices and setting them up to draw
	//GLfloat vertices[] = {
	//	-0.5f, -0.5f, -0.5f,
	//	0.5f, -0.5f, -0.5f,
	//	0.5f,  0.5f, -0.5f,
	//	0.5f,  0.5f, -0.5f,
	//	-0.5f,  0.5f, -0.5f,
	//	-0.5f, -0.5f, -0.5f,

	//	-0.5f, -0.5f,  0.5f,
	//	0.5f, -0.5f,  0.5f,
	//	0.5f,  0.5f,  0.5f,
	//	0.5f,  0.5f,  0.5f,
	//	-0.5f,  0.5f,  0.5f,
	//	-0.5f, -0.5f,  0.5f,

	//	-0.5f,  0.5f,  0.5f,
	//	-0.5f,  0.5f, -0.5f,
	//	-0.5f, -0.5f, -0.5f,
	//	-0.5f, -0.5f, -0.5f,
	//	-0.5f, -0.5f,  0.5f,
	//	-0.5f,  0.5f,  0.5f,

	//	0.5f,  0.5f,  0.5f,
	//	0.5f,  0.5f, -0.5f,
	//	0.5f, -0.5f, -0.5f,
	//	0.5f, -0.5f, -0.5f,
	//	0.5f, -0.5f,  0.5f,
	//	0.5f,  0.5f,  0.5f,

	//	-0.5f, -0.5f, -0.5f,
	//	0.5f, -0.5f, -0.5f,
	//	0.5f, -0.5f,  0.5f,
	//	0.5f, -0.5f,  0.5f,
	//	-0.5f, -0.5f,  0.5f,
	//	-0.5f, -0.5f, -0.5f,

	//	-0.5f,  0.5f, -0.5f,
	//	0.5f,  0.5f, -0.5f,
	//	0.5f,  0.5f,  0.5f,
	//	0.5f,  0.5f,  0.5f,
	//	-0.5f,  0.5f,  0.5f,
	//	-0.5f,  0.5f, -0.5f,
	//};

	//GLuint VBO, VAO;
	//glGenVertexArrays(1, &VAO);
	//glGenBuffers(1, &VBO);
	//glBindVertexArray(VAO);

	//// Bind VBO here
	//glBindBuffer(GL_ARRAY_BUFFER, VBO);
	//glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	//glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
	//glEnableVertexAttribArray(0);

	//glBindVertexArray(0); // Unbind VAO

	glViewport(0, 0, WIDTH, HEIGHT);
	glfwSetKeyCallback(window, key_callback);


	glm::mat4 view;

	// Texture stuff here
	GLfloat box[] = {
		// Pos      // Tex
		0.0f, 1.0f, 0.0f, 1.0f,
		1.0f, 0.0f, 1.0f, 0.0f,
		0.0f, 0.0f, 0.0f, 0.0f,

		0.0f, 1.0f, 0.0f, 1.0f,
		1.0f, 1.0f, 1.0f, 1.0f,
		1.0f, 0.0f, 1.0f, 0.0f
	};

	GLuint tVBO, tVAO;
	glGenVertexArrays(1, &tVAO);
	glGenBuffers(1, &tVBO);

	glBindVertexArray(tVAO);

	glBindBuffer(GL_ARRAY_BUFFER, tVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(box), box, GL_STATIC_DRAW);

	// Position attribute
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), (GLvoid*)0);

	glBindVertexArray(0); // Unbind VAO

	GLuint texture;
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);

	// Set the texture wrapping parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	// Set texture wrapping to GL_REPEAT (usually basic wrapping method)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// Set texture filtering parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	int width, height;
	unsigned char *image = SOIL_load_image("imgs/wall.jpg", &width, &height, 0, SOIL_LOAD_RGB);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
	glGenerateMipmap(GL_TEXTURE_2D);
	SOIL_free_image_data(image);
	glBindTexture(GL_TEXTURE_2D, 0); // unbind


	glm::mat4 projection;
	projection = glm::ortho(0.0f, (GLfloat)WIDTH, (GLfloat)HEIGHT, 0.0f, -1.0f, 1.0f);

	textureShader.Use();
	glUniformMatrix4fv(glGetUniformLocation(textureShader.Program, "projection"), 1, GL_FALSE, glm::value_ptr(projection));

	glm::vec2 size(100.0f, 100.f);
	while (!glfwWindowShouldClose(window)) {
		GLfloat currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;
		glfwPollEvents(); // input
		doMovement();

		//glEnable(GL_DEPTH_TEST);
		glClearColor(0.9f, 0.3f, 0.3f, 1.0f); // clear and draw color
		//glClearColor(0.4f, 0.2f, 0.4f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);// | GL_DEPTH_BUFFER_BIT);

		//ourShader.Use();

		glm::mat4 model;
		model = glm::translate(model, pPos);
		model = glm::scale(model, glm::vec3(size, 1.0f)); // Last scale
		//view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);

		//GLint modelLoc = glGetUniformLocation(ourShader.Program, "model");
		//GLint viewLoc = glGetUniformLocation(ourShader.Program, "view");
		//GLint projectionLoc = glGetUniformLocation(ourShader.Program, "projection");

		//glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		//glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
		//glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));

		//glBindVertexArray(VAO);
		//glDrawArrays(GL_TRIANGLES, 0, 36);
		//glBindVertexArray(0);

		// texture obj
		glBindTexture(GL_TEXTURE_2D, texture);

		textureShader.Use();
		glUniformMatrix4fv(glGetUniformLocation(textureShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
		glUniform3f(glGetUniformLocation(textureShader.Program, "textColor"), 1.0f, 1.0f, 0.7f);

		glBindVertexArray(tVAO);
		glDrawArrays(GL_TRIANGLES, 0, 6);
		glBindVertexArray(0);

		glfwSwapBuffers(window);
	}

	//glDeleteVertexArrays(1, &VAO);
	glDeleteVertexArrays(1, &tVAO);
	glDeleteBuffers(1, &tVBO);
	glfwTerminate();
}

void doMovement() {
	// Camera controls
	//GLfloat cameraSpeed = 3.0f * deltaTime;
	if (keys[GLFW_KEY_W])
		pPos.y += 0.1f;
	if (keys[GLFW_KEY_S])
		pPos.y -= 0.1f;
	if (keys[GLFW_KEY_A])
		pPos.x -= 0.1f;
	if (keys[GLFW_KEY_D])
		pPos.x += 0.1f;
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode) {
	// When a user presses the escape key, we set the WindowShouldClose property to true, 
	// closing the application
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);

	if (action == GLFW_PRESS)
		keys[key] = true;
	else if (action == GLFW_RELEASE)
		keys[key] = false;
}

void mouse_callback(GLFWwindow * window, double xpos, double ypos) {
	//if (firstMouse) {
	//	lastX = xpos;
	//	lastY = ypos;
	//	firstMouse = false;
	//}
	//GLfloat xoffset = xpos - lastX;
	//GLfloat yoffset = lastY - ypos; // Reversed since y-coordinates range from bottom to top
	//lastX = xpos;
	//lastY = ypos;

	//GLfloat sensitivity = 0.11f;
	//xoffset *= sensitivity;
	//yoffset *= sensitivity;

	//yaw += xoffset;
	//pitch += yoffset;

	//if (pitch > 89.0f)
	//	pitch = 89.0f;
	//if (pitch < -89.0f)
	//	pitch = -89.0f;

	//glm::vec3 front;
	//front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
	//front.y = sin(glm::radians(pitch));
	//front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
	//cameraFront = glm::normalize(front);
}

