#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

using namespace std;

void processInput(GLFWwindow* window);

const char* vertexShaderSource = "#version 330 core\n"
"layout (location = 0) in vec3 aPos;\n"
"void main()\n"
"{\n"
"   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
"}\0";

const char* fragmentShaderSource = "#version 330 core\n"
"out vec4 FragColor;\n"
"void main()\n"
"{\n"
"   FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
"}\n\0";

#define VERTICAL 1
#define HORIZONTAL 0

float vertices[] = {
	-0.5f, -0.5f, 0.0f,
	 0.5f, -0.5f, 0.0f,
	 0.0f,  0.5f, 0.0f
};

void updateTriangle(int axis, float direction) {
	cout << axis << " " << direction << endl;
	for (int i = axis; i < 9; i = i + 3) {
		cout << i << endl;
		vertices[i] = vertices[i] + direction;
	}
}

void update(float vertices[], GLFWwindow* window) {
	
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		updateTriangle(VERTICAL, .05f);
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		updateTriangle(HORIZONTAL, -.05f);
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		updateTriangle(VERTICAL, -.05f);
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		updateTriangle(HORIZONTAL, .05f);
}

int main() {

	//glfw: init and configure (tool that helps make windows)
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	//create window with GLFW
	GLFWwindow* window = glfwCreateWindow(800, 600, "learnOpenGl", NULL, NULL);
	if (window == NULL) {
		cout << "failed to create glfw window" << endl;
		glfwTerminate();
		return -1;
	}

	//set current window and load glad
	//GLAD abstracts away configuring opengl for your current system and version
	glfwMakeContextCurrent(window);
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		cout << "Failed to initialize GLAD" << endl;
		return -1;
	}

	//set size of viewport and declare vars
	glViewport(0, 0, 800, 600);

	// =======VERTEX SHADER COMPILATION
	unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER); 
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
	glCompileShader(vertexShader);


	// =======FRAGMENT SHADER COMPILATION
	unsigned int fragShader;
	fragShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragShader, 1, &fragmentShaderSource, NULL);
	glCompileShader(fragShader);


	//===========CREATE SHADER PROGRAM
	unsigned int shaderProgram;
	shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragShader);
	glLinkProgram(shaderProgram);


	//===========TEST TO MAKE SURE IT WORKS
	int success;
	char infoLog[512];

	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
		cout << "ERROR::PROGRAM::SHADER::LINKING_FAILED\n" << infoLog << endl;
	}


	glUseProgram(shaderProgram);
	//SHADERS NO LONGER NEEDED AFTER LINK
	glDeleteShader(vertexShader);
	glDeleteShader(fragShader);

	//===========MAKING BUFFERS
	unsigned int VAO, VBO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glBindVertexArray(VAO);

	//generate buffer from memory(VBO) and then bind to buffer and add triangle.
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	
	//set vertex attributes pointers (since vertexes are an array of numbers we tell the computer how to use them to draw)
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	//use our shader program to render


	//render loop
	while (!glfwWindowShouldClose(window)) {
		update(vertices, window);

		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		glUseProgram(shaderProgram);
		glBindVertexArray(VAO);
		glDrawArrays(GL_TRIANGLES, 0, 3);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}



	//terminate after loop
	glfwTerminate();
	return 0;
}

//takes input and sets window close if esc is pressed.
void processInput(GLFWwindow* window) {
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
}