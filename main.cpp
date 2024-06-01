#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <stb/stb_image.h>

#include "classes/Shader.h"

#define WINDOW_WIDTH	800
#define WINDOW_HEIGHT	600


// SOME HELPER FUNCTIONS -----------------------------------------------------
// Prototypes
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);

// Bodies
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

void processInput(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
}

// MAIN ---------------------------------------------------------------------
int main()
{
	// Initiate and define some library definitions/primitives
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	
	// Window definitions
	GLFWwindow* window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "LearnOpenGL", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);

	// Checking errors
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) 
	{ 
		std::cout << "Failed to initialize GLAD" << std::endl; return -1;  
	}

	// Set the viewport and rearange it if needed
	glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT); 
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	// Defining some vertex data
	float vertices[] = {
		// POSITION         // Colors         // texture coord 
		 0.5f,  0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.f, 1.f,
		 0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 1.f, 0.f,
		-0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 0.f, 0.f,
		-0.5f,  0.5f, 0.0f, 1.0f, 1.0f, 0.0f, 0.f, 1.f
	};
	unsigned int indices[] = {
		0,1,2,  // First triangle
		0,2,3
	};

	Shader ourShader("./shaders/shader.vert", "./shaders/shader.frag");

	// GPU Buffers configuration
	// VBO
	unsigned int VBO;
	glGenBuffers(1, &VBO);  // Generates a VBO 
	glBindBuffer(GL_ARRAY_BUFFER, VBO);		  // Assign this VBO to an array
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);  // Copia os dados para o buffer na GPU

	// VAO
	unsigned int VAO; glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	// EBO
	unsigned int EBO; glGenBuffers(1, &EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	// Vertex Attributes pointers
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);

	// Texture thingys
	stbi_set_flip_vertically_on_load(true);
	int width, height, nrChannels;
	unsigned char* data = stbi_load("./textures/container.jpg", &width, &height, &nrChannels, 0);

	unsigned int texture1;
	unsigned int texture2;
	glGenTextures(1, &texture1);
	glBindTexture(GL_TEXTURE_2D, texture1);
	if (data)
	{
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
		
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else std::cout << "FAILED TO LOAD TEXTURE 1!!" << std::endl;
	stbi_image_free(data);

	data = stbi_load("./textures/awesomeface.png", &width, &height, &nrChannels, 0);
	glGenTextures(1, &texture2);
	glBindTexture(GL_TEXTURE_2D, texture2);
	if (data)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else std::cout << "FAILED TO LOAD TEXTURE 2!!" << std::endl;
	stbi_image_free(data);

	ourShader.use();
	ourShader.setInt("texture1", 0);
	ourShader.setInt("texture2", 1);

	glm::mat4 model = glm::mat4(1.0f);
	model = glm::rotate(model, glm::radians(-55.f), glm::vec3(1.f, 0.f, 0.f));
	glm::mat4 view = glm::mat4(1.0f);
	view = glm::translate(view, glm::vec3(0.0f, 0.0f, -3.f));
	glm::mat4 projc = glm::mat4(1.f);
	projc = glm::perspective(glm::radians(45.0f), (float)WINDOW_WIDTH/WINDOW_HEIGHT , 0.1f, 100.f);

	ourShader.setMatrix4("model", model);
	ourShader.setMatrix4("view", view);
	ourShader.setMatrix4("projc", projc);
	// The general loop
	float scale = 0.20;
	while (!glfwWindowShouldClose(window))
	{
		// Input
		processInput(window);
		if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS && scale <= 1.0)
			scale += 0.05; 
		if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS && scale >= 0.0)
			scale -= 0.05;
		ourShader.setFloat("scale", scale);

		// Rendering Commands
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		// Ativação e binding das texturas
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture1);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, texture2);

		// Criação dos vértices
		glBindVertexArray(VAO);

		// Desenho dos vértices
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);
		
		// Check and call events and swap the buffers
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	// End application
	glfwTerminate();
	return 0;
} 