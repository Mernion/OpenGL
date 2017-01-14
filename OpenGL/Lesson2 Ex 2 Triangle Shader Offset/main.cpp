#include <iostream>

#define GLEW_STATIC
#include <GL/glew.h>

#include <GLFW\glfw3.h>

#include "Shader.h"
const GLint WIDTH = 800;
const GLint HEIGHT = 600;

int main()
{
	/* init part */
	if (glfwInit() != GLFW_TRUE)
	{
		std::cout << "GLFW init failed" << std::endl;
		return -1;
	}

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

	GLFWwindow *window = glfwCreateWindow(WIDTH, HEIGHT, "Open GL Lesson 1 Ex. 1", nullptr, nullptr);
	glfwMakeContextCurrent(window);

	glewExperimental = GL_TRUE;

	GLenum err = glewInit();

	if (err != GLEW_OK)
	{
		std::cout << "GLEW init failed " << std::endl;
		std::cout << glewGetErrorString(err);
		glfwTerminate();
		return -1;
	}

	int width, height;
	glfwGetFramebufferSize(window, &width, &height);
	glViewport(0, 0, width, height);

	/* end of init part */

	GLfloat vertices[] =
	{
		-0.5f, -0.5f, 0.0f,
		0.5f, -0.5f, 0.0f,
		0.0f, 0.5f, 0.0f
	};

	Shader myShader("vertexOffsetShader.txt", "fragmentShader.txt");

	GLuint VAO, VBO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);

	glBindVertexArray(0);
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	GLfloat xOffset = 0.5f;
	GLint vertexOffsetVar = glGetUniformLocation(myShader.Program, "offset");
	if (vertexOffsetVar == -1)
	{
		std::cout << "Failed to find uniform var location " << std::endl;
	}


	while (!glfwWindowShouldClose(window))
	{

		glfwPollEvents();

		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		myShader.Use();
		glUniform1f(vertexOffsetVar, xOffset);
		glBindVertexArray(VAO);
		glDrawArrays(GL_TRIANGLES, 0, 3);
		glBindVertexArray(0);

		glfwSwapBuffers(window);

	}

	glfwTerminate();

	return 0;

}