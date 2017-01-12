#define GLEW_STATIC
#include "GL\glew.h"

#include "GLFW\glfw3.h"

#include <iostream>

GLint WIDTH = 800;
GLint HEIGHT = 600;

int main()
{

	if (glfwInit() != GLFW_TRUE)
	{
		std::cout << "GLFW init failed " << std::endl;
		return -1;
	}

	// 3.3 opengl
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

	GLFWwindow *window = glfwCreateWindow(WIDTH, HEIGHT, "Open GL L1 E1", nullptr, nullptr);
	glfwMakeContextCurrent(window);

	glewExperimental = GL_TRUE;
	if (glewInit() != GLEW_OK)
	{
		std::cout << "Glew init failed" << std::endl;
		glfwTerminate();
		return -1;
	}

	int width, height; // viewport
	glfwGetFramebufferSize(window, &width, &height);
	glViewport(0, 0, width, height);

	while (!glfwWindowShouldClose(window))
	{
		glfwPollEvents();

		glfwSwapBuffers(window);


	}

	glfwTerminate();

	return 0;

}