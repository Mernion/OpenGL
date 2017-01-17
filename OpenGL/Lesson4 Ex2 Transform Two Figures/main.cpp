#include <iostream>

#define GLEW_STATIC
#include <GL/glew.h>

#include <GLFW\glfw3.h>

#include "Shader.h"
#define STB_IMAGE_IMPLEMENTATION
#include "STB\stb_image.h"

#include <glm\glm.hpp>
#include <glm\gtc\matrix_transform.hpp>
#include <glm\gtc\type_ptr.hpp>

const GLint WIDTH = 800;
const GLint HEIGHT = 600;

GLfloat mixStr = 0.2f;
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
	if (action == GLFW_PRESS)
	{
		switch (key)
		{
		case GLFW_KEY_UP:
		{
			mixStr += 0.1f;
			if (mixStr > 1.0f)
				mixStr = 1.0f;
			break;
		}
		case GLFW_KEY_DOWN:
		{
			mixStr -= 0.1f;
			if (mixStr < 0.f)
				mixStr = 0.f;
			break;
		}

		default:
			break;
		}
	}
}


void PrepareTexture(GLuint& texture, const char* imagePath, GLint imageFormat, GLint textureWrapMode)
{
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture); // All upcoming GL_TEXTURE_2D operations now have effect on this texture object
										   // Set the texture wrapping parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	// Set texture wrapping to GL_REPEAT (usually basic wrapping method)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// Set texture filtering parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	// Load image, create texture and generate mipmaps
	int width, height, n;
	unsigned char* image = stbi_load(imagePath, &width, &height, &n, 0);
	glTexImage2D(GL_TEXTURE_2D, 0, imageFormat, width, height, 0, imageFormat, GL_UNSIGNED_BYTE, image);
	glGenerateMipmap(GL_TEXTURE_2D);
	STBI_FREE(image);
	glBindTexture(GL_TEXTURE_2D, 0); // Unbind texture when done, so we won't accidentily mess up our texture.
}

int main()
{
	glfwInit();
	// Set all the required options for GLFW
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

	// Create a GLFWwindow object that we can use for GLFW's functions
	GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "LearnOpenGL", nullptr, nullptr);
	glfwMakeContextCurrent(window);

	glfwSetKeyCallback(window, key_callback);

	// Set this to true so GLEW knows to use a modern approach to retrieving function pointers and extensions
	glewExperimental = GL_TRUE;
	// Initialize GLEW to setup the OpenGL Function pointers
	glewInit();

	// Define the viewport dimensions
	glViewport(0, 0, WIDTH, HEIGHT);

	// Build and compile our shader program
	Shader ourShader("VertexShader.txt", "FragmentShader.txt");

	// Set up vertex data (and buffer(s)) and attribute pointers
	GLfloat vertices[] = {
		// Positions          // Colors           // Texture Coords
		0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   2.0f, 2.0f, // Top Right
		0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   2.0f, 0.0f, // Bottom Right
		-0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f, // Bottom Left
		-0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 2.0f  // Top Left 
	};
	GLuint indices[] = {
		0, 1, 3, // First Triangle
		1, 2, 3  // Second Triangle
	};
	GLuint VBO, VAO, EBO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	// Position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);
	// Color attribute
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);
	// TexCoord attribute
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(6 * sizeof(GLfloat)));
	glEnableVertexAttribArray(2);

	glBindVertexArray(0); // Unbind VAO

						  // Load and create a texture 
	GLuint textureContainer, textureSmile;
	PrepareTexture(textureContainer, "container.jpg", GL_RGB, GL_REPEAT);
	PrepareTexture(textureSmile, "awesomeface.jpg", GL_RGB, GL_CLAMP_TO_EDGE);

	// Game loop
	while (!glfwWindowShouldClose(window))
	{
		// Check if any events have been activiated (key pressed, mouse moved etc.) and call corresponding response functions
		glfwPollEvents();

		// Render
		// Clear the colorbuffer
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		// transformation stuff
		glm::mat4 trans;
		trans = glm::translate(trans, glm::vec3(0.5f, -0.5f, 0.0f));
		trans = glm::rotate(trans, (glm::radians((GLfloat)glfwGetTime() * 50.0f)), glm::vec3(0.0f, 0.0f, 1.0f));

		// Bind Texture
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, textureContainer);
		glUniform1i(glGetUniformLocation(ourShader.Program, "ourTexture1"), 0);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, textureSmile);
		glUniform1i(glGetUniformLocation(ourShader.Program, "ourTexture2"), 1);

		// Activate shader
		ourShader.Use();
		glUniformMatrix4fv(glGetUniformLocation(ourShader.Program, "transform"), 1, GL_FALSE, glm::value_ptr(trans));
		glUniform1f(glGetUniformLocation(ourShader.Program, "blendStr"), mixStr);
		// Draw container
		glBindVertexArray(VAO);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

		trans = glm::mat4();
		trans = glm::translate(trans, glm::vec3(-0.5f, 0.5f, 0.0f));
		GLfloat scale = glm::sin(glfwGetTime());
		trans = glm::scale(trans, glm::vec3(scale, scale, 0.0f));
		glUniformMatrix4fv(glGetUniformLocation(ourShader.Program, "transform"), 1, GL_FALSE, glm::value_ptr(trans));
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

		glBindVertexArray(0);

		// Swap the screen buffers
		glfwSwapBuffers(window);
	}
	// Properly de-allocate all resources once they've outlived their purpose
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);
	// Terminate GLFW, clearing any resources allocated by GLFW.
	glfwTerminate();
	return 0;

}