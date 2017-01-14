#include <iostream>

#define GLEW_STATIC
#include <GL/glew.h>

#include <GLFW\glfw3.h>

#include "Shader.h"
#define STB_IMAGE_IMPLEMENTATION
#include "STB\stb_image.h"

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
		// Positions		// Colors			// Texture Coords
		0.5f, 0.5f, 0.0f,	1.0f, 0.0f, 0.0f,	1.0f, 1.0f,
		0.5f, -0.5f, 0.0f,	0.0f, 1.0f, 0.0f,	1.0f, 0.0f,
		-0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f,	0.0f, 0.0f,
		-0.5f, 0.5f, 0.0f,  1.0f, 1.0f, 0.0f,   0.0f, 1.0f
	};
	GLuint indices[] =
	{
		0, 1, 3,
		1, 2, 3
	};

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
	GLfloat borderColor[] = { 1.0f, 1.0f, 0.0f, 1.0f };
	glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	int txtWidth, txtHeight, txtComp;
	unsigned char *txtImage = stbi_load("container.png", &txtWidth, &txtHeight, &txtComp, 0);

	GLuint texture;
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, txtWidth, txtHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, txtImage);
	glGenerateMipmap(GL_TEXTURE_2D);
	STBI_FREE(txtImage);
	glBindTexture(GL_TEXTURE_2D, 0);

	Shader myShader("VertexShader.txt", "FragmentShader.txt");

	GLuint VAO, VBO, EBO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &EBO);
	glGenBuffers(1, &VBO);

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GL_FLOAT), (GLvoid*)0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GL_FLOAT), (GLvoid*)(3 * sizeof(vertices)));
	glEnableVertexAttribArray(1);

	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GL_FLOAT), (GLvoid*)(6 * sizeof(vertices)));
	glEnableVertexAttribArray(2);

	glBindVertexArray(0);
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);


	while (!glfwWindowShouldClose(window))
	{

		glfwPollEvents();

		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
		
		glBindTexture(GL_TEXTURE_2D, texture);
		myShader.Use();
		glBindVertexArray(VAO);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);

		glfwSwapBuffers(window);

	}

	glfwTerminate();

	return 0;

}