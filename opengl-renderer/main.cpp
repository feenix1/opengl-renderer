#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 600;

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
	glViewport(0, 0, width, height);
}

/// @brief Reads data from file and returns a string object.
/// @param fileName 
/// @return 
std::string readFile(std::string fileName) {
	std::ifstream file;
	file.open(fileName, std::ios::in);
	if (!file.is_open()) {
		std::cerr << "Failed to open file: " << fileName << std::endl;
		return "";
	}
	std::stringstream buffer;
	buffer << file.rdbuf();
	file.close();
	return buffer.str();
}

void processInput(GLFWwindow* window) {
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, true);
	}
}

int main() {
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow* window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "renderer", NULL, NULL);
	if (window == NULL) {
		std::cout << "Could not create GLFW window!" << std::endl;
		glfwTerminate();
		return -1;
	}

	glfwMakeContextCurrent(window);
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		std::cout << "Could not initialize GLAD" << std::endl;
		return -1;
	}

	glViewport(0, 0, SCREEN_HEIGHT, SCREEN_WIDTH);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	// Create VAO (stores vert buffer setup/settings)
	unsigned int VAO;
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	// make verts, vert buffer, and send to GPU
	float verts[] = {
		 0.5f,  0.5f, 0.0f,  // top right
		 0.5f, -0.5f, 0.0f,  // bottom right
		-0.5f, -0.5f, 0.0f,  // bottom left
		-0.5f,  0.5f, 0.0f   // top left
	}; 
	unsigned int VBO;
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(verts), verts, GL_STATIC_DRAW);

	// made tri indicies and same as last
	unsigned int triIndices[] = {
		0, 1, 3,
		1, 2, 3,
	};
	unsigned int EBO;
	glGenBuffers(1, &EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(triIndices), triIndices, GL_STATIC_DRAW);

	// Map vert buffer to attriubte 0 in vertex shader
	// vert has position which is 3 floats large
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	// error logging stuff
	int success;
	char infoLog[512];

	// Vertex compile
	const std::string vertSource = readFile("vert.glsl");
	const char* vertSourceC = vertSource.c_str();
	unsigned int vertShaderHdl = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertShaderHdl, 1, &vertSourceC, NULL);
	glCompileShader(vertShaderHdl);
	glGetShaderiv(vertShaderHdl, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(vertShaderHdl, 512, NULL, infoLog);
		std::cout << "vertex shader compilation failed: \n" << infoLog << std::endl;
	}

	// Fragment compile
	const std::string fragSource = readFile("frag.glsl");
	const char* fragSourceC = fragSource.c_str();
	unsigned int fragShaderHdl = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragShaderHdl, 1, &fragSourceC, NULL);
	glCompileShader(fragShaderHdl);
	glGetShaderiv(fragShaderHdl, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(fragShaderHdl, 512, NULL, infoLog);
		std::cout << "fragment shader compilation failed: \n" << infoLog << std::endl;
	}

	// Link
	unsigned int shaderProgram;
	shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertShaderHdl);
	glAttachShader(shaderProgram, fragShaderHdl);
	glLinkProgram(shaderProgram);
	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
		std::cout << "shader program linking failed: \n" << infoLog << std::endl;
	}

	// Clean up shader objects
	glDeleteShader(vertShaderHdl);
	glDeleteShader(fragShaderHdl);

	// wireframe if we want to
	// glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	while (!glfwWindowShouldClose(window)) {
		processInput(window);

		glClearColor(0.3f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		glUseProgram(shaderProgram);
		glBindVertexArray(VAO);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

		glfwPollEvents();
		glfwSwapBuffers(window);
	}

	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);
	glDeleteProgram(shaderProgram);

	glfwTerminate();
	return 0;
}
