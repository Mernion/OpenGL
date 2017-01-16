#pragma once
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

#include <GL\glew.h>

class Shader
{
public:
	GLuint Program;

	Shader(const GLchar* vertexPath, const GLchar* fragmentPath)
	{
		std::string vertexCode;
		std::string fragmentCode;
		std::ifstream vertexShaderFile;
		std::ifstream fragmentShaderFile;

		vertexShaderFile.exceptions(std::ifstream::badbit);
		fragmentShaderFile.exceptions(std::ifstream::badbit);

		try
		{
			vertexShaderFile.open(vertexPath);
			fragmentShaderFile.open(fragmentPath);
			std::stringstream vertexShaderStream, fragmentShaderStream;

			vertexShaderStream << vertexShaderFile.rdbuf();
			fragmentShaderStream << fragmentShaderFile.rdbuf();

			vertexShaderFile.close();
			fragmentShaderFile.close();

			vertexCode = vertexShaderStream.str();
			fragmentCode = fragmentShaderStream.str();
		}
		catch (std::ifstream::failure e)
		{
			std::cout << "Error shader file reading" << std::endl;
		}

		const GLchar* vertexShaderCode = vertexCode.c_str();
		const GLchar* fragmentShaderCode = fragmentCode.c_str();

		GLuint vertex, fragment;

		vertex = glCreateShader(GL_VERTEX_SHADER);
		CompileShader(vertex, vertexShaderCode);

		fragment = glCreateShader(GL_FRAGMENT_SHADER);
		CompileShader(fragment, fragmentShaderCode);

		Program = glCreateProgram();
		CompileShaderProgram(Program, vertex, fragment);

		glDeleteShader(vertex);
		glDeleteShader(fragment);

	}

	void Use()
	{
		glUseProgram(Program);
	}

private:
	GLint success;
	GLchar infoLog[512];

	void CompileShader(GLuint& shader, const GLchar* shaderCode)
	{
		glShaderSource(shader, 1, &shaderCode, nullptr);
		glCompileShader(shader);
		glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
		if (!success)
		{
			glGetShaderInfoLog(shader, 512, nullptr, infoLog);
			std::cout << "Error vertex shader compilation " << std::endl;
			std::cout << infoLog << std::endl;
		}
	}

	void CompileShaderProgram(GLuint& program, GLuint& vertexShader, GLuint& fragmentShader)
	{
		glAttachShader(program, vertexShader);
		glAttachShader(program, fragmentShader);
		glLinkProgram(program);
		glGetProgramiv(program, GL_LINK_STATUS, &success);
		if (!success)
		{
			glGetProgramInfoLog(program, 512, nullptr, infoLog);
			std::cout << "Error shader program link" << std::endl;
			std::cout << infoLog << std::endl;
		}
	}

};