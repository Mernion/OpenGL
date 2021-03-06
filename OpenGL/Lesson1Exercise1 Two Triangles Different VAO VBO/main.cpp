#include <iostream>

#define GLEW_STATIC
#include <GL/glew.h>

#include <GLFW\glfw3.h>

const GLint WIDTH = 800;
const GLint HEIGHT = 600;

const GLchar* vertexShaderSource = "#version 330 core\n"
"layout (location = 0) in vec3 position;\n"
"void main()\n"
"{\n"
"gl_Position = vec4(position.x, position.y, position.z, 1.0f)\n;"
"}\n";

const GLchar* fragmentShaderSource = "#version 330 core\n"
"out vec4 color;\n"
"void main()\n"
"{\n"
"color = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
"}\n";

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

	/* shader part */
	GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
	glCompileShader(vertexShader);
	GLint success;
	GLchar infoLog[512];
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
		std::cout << "Vertex shader compilation failed" << std::endl;
		std::cout << infoLog << std::endl;
		glfwTerminate();
		return -1;
	}

	GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, nullptr);
	glCompileShader(fragmentShader);
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
		std::cout << "Fragment shader compilation failed" << std::endl;
		std::cout << infoLog << std::endl;
		glfwTerminate();
		return -1;
	}

	GLuint shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);
	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
	if (!success)
	{
		glGetProgramInfoLog(shaderProgram, 512, nullptr, infoLog);
		std::cout << "Shader link failed" << std::endl;
		std::cout << infoLog << std::endl;
		glfwTerminate();
		return -1;
	}

	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	/* end of shader part */

	GLfloat vertices[2][9] = {
		{
		-0.5f, -0.5f, 0.0f,
		0.0f, -0.5f, 0.0f,
		0.0f, 0.5f, 0.0f
		},
		{
		0.0f, -0.5f, 0.0f,
		0.5f, -0.5f, 0.0f ,
		0.0f, 0.5f, 0.5f
		}
	};

	GLuint VBO[2];
	GLuint VAO[2];

	glGenVertexArrays(2, VAO);
	glGenBuffers(2, VBO);

	for (int i = 0; i < 2; i++)
	{
		glBindVertexArray(VAO[i]);

		glBindBuffer(GL_ARRAY_BUFFER, VBO[i]);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices[i]), vertices[i], GL_STATIC_DRAW);

		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
		glEnableVertexAttribArray(0);

		glBindBuffer(GL_ARRAY_BUFFER, 0);

		glBindVertexArray(0);
	}

	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	while (!glfwWindowShouldClose(window))
	{

		glfwPollEvents();

		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		glUseProgram(shaderProgram);
		for (int i = 0; i < 2; i++)
		{
			glBindVertexArray(VAO[i]);
			glDrawArrays(GL_TRIANGLES, 0, 3);
			glBindVertexArray(0);
		}

		glfwSwapBuffers(window);

	}

	glfwTerminate();

	return 0;

}