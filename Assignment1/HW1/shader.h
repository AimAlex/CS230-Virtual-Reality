/*
**** DO NOT MODIFY THIS FILE ****

- Contains shader class which handles shader compilation, and attaching uniforms
- You will not need to look here for the homework1. 

*/

#pragma once

#include <GL/glew.h>
#include "utils.h"

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

class Shader
{
public:
    GLuint Program;

	Shader(const GLchar* vertexPath, const GLchar* fragmentPath) {
		loadProgramFromFile(vertexPath, fragmentPath);
	}
	
	// Uses the current shader
    void Use() { glUseProgram(this->Program); }

	// Attach to Uniform
	template<typename T>
	void attachToUniform(std::string uniform, const T & value) {
		if (Program == 0) {
			throw(std::runtime_error("No program object found. Cannot attach to uniform."));
		}
		Use();

		GLint loc = glGetUniformLocation(Program, uniform.c_str());
		if (loc == -1) {
			std::string msg = "Uniform " + uniform + " could not be found. Could not attach to uniform";
			throw std::runtime_error(msg);
		}
		attachToLoc(loc, value);
	}

private:
	template<typename T>
	void attachToLoc(const GLint & loc, const T & value);

    void checkCompileErrors(GLuint shader, std::string type)
	{
		GLint success;
		GLchar infoLog[1024];
		if(type != "PROGRAM")
		{
			glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
			if(!success)
			{
				glGetShaderInfoLog(shader, 1024, NULL, infoLog);
                std::cout << "| ERROR::::SHADER-COMPILATION-ERROR of type: " << type << "|\n" << infoLog << "\n| -- --------------------------------------------------- -- |" << std::endl;
			}
		}
		else
		{
			glGetProgramiv(shader, GL_LINK_STATUS, &success);
			if(!success)
			{
				glGetProgramInfoLog(shader, 1024, NULL, infoLog);
                std::cout << "| ERROR::::PROGRAM-LINKING-ERROR of type: " << type << "|\n" << infoLog << "\n| -- --------------------------------------------------- -- |" << std::endl;
			}
		}
	}

	// Assumes the shader paths are relative to the Visual Studio Solution path
	GLuint loadProgramFromFile(const char * vertShaderPath, const char * fragShaderPath)
	{
		std::string vertShaderSrc = readShaderFile(vertShaderPath);
		std::string fragShaderSrc = readShaderFile(fragShaderPath);

		return loadProgram(vertShaderSrc.c_str(), fragShaderSrc.c_str());

	}



	GLuint loadProgram(const char * vertShaderSrc, const char * fragShaderSrc)
	{
		GLuint vertexShader, fragmentShader, programObject;
		GLint linked;

		// Load the vertex/fragment shaders
		vertexShader = loadShader(GL_VERTEX_SHADER, vertShaderSrc);
		fragmentShader = loadShader(GL_FRAGMENT_SHADER, fragShaderSrc);

		// Create the program object
		programObject = glCreateProgram();
		if (programObject == 0)
		{
			throw std::runtime_error("Could not create glProgram");
		}
        GLuint VertexArrayID;
        glGenVertexArrays(1, &VertexArrayID);
        glBindVertexArray(VertexArrayID);
		glAttachShader(programObject, vertexShader);
		glAttachShader(programObject, fragmentShader);

		// Link the program
		glLinkProgram(programObject);

		// Check the link status
		glGetProgramiv(programObject, GL_LINK_STATUS, &linked);

		if (!linked)
		{
			GLint infoLen = 0;

			glGetProgramiv(programObject, GL_INFO_LOG_LENGTH, &infoLen);

			if (infoLen > 1)
			{
				char * infoLog = (char *)malloc(sizeof(char) * infoLen);
				glGetProgramInfoLog(programObject, infoLen, NULL, infoLog);
				std::cout << "Error linking program: \n" << infoLog << std::endl;
				free(infoLog);
			}
			glDeleteProgram(programObject);
			throw (std::runtime_error("Error linking program"));
		}

		GLint status = GL_FALSE;
		glValidateProgram(programObject);

		glGetProgramiv(programObject, GL_VALIDATE_STATUS, &status); // Find out if the shader program validated correctly

		if (status != GL_TRUE)  // If there was a problem validating
		{
			GLint infoLen = 0;
			glGetProgramiv(programObject, GL_INFO_LOG_LENGTH, &infoLen);

			if (infoLen > 1)
			{
				char * infoLog = (char *)malloc(sizeof(char) * infoLen);

				glGetProgramInfoLog(programObject, infoLen, NULL, infoLog);
				std::cout << "Error validating program: \n" << infoLog << std::endl;
				free(infoLog);
			}
			glDeleteProgram(programObject);
			throw(std::runtime_error("Error validating program"));
		}

		// Free up no longer needed shader resources
		glDetachShader(programObject, vertexShader);
		glDetachShader(programObject, fragmentShader);

		glDeleteShader(vertexShader);
		glDeleteShader(fragmentShader);

		Program = programObject;
		return programObject;
	}




	GLuint loadShader(GLenum type, const char * src)
	{
		GLuint shader;
		GLint compiled;

		// Create the shader object
		shader = glCreateShader(type);
		getError("glCreateShader()");

		// Load the shader source
		glShaderSource(shader, 1, &src, NULL);

		// Compile the shader
		glCompileShader(shader);

		// Check the compile status
		glGetShaderiv(shader, GL_COMPILE_STATUS, &compiled);

		if (!compiled)
		{
			GLint infoLen = 0;

			glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &infoLen);

			if (infoLen > 1)
			{
				char * infoLog = (char *)malloc(sizeof(char) * infoLen);
				std::string typeStr;

				if (type == GL_VERTEX_SHADER)
				{
					typeStr = "vertex shader";
				}
				else if (type == GL_FRAGMENT_SHADER)
				{
					typeStr = "fragment shader";
				}
				else {
					throw std::runtime_error("neither fragment nor vertex shader!");
				}

				glGetShaderInfoLog(shader, infoLen, NULL, infoLog);
				std::cout << "Error compiling program: " << typeStr << "\n" << infoLog << std::endl;
				free(infoLog);
			}
			glDeleteShader(shader);
            throw(std::runtime_error("Shader compilation error"));
			return 0;
		}

		return shader;
	}




	std::string readShaderFile(const char * filePath)
	{
		std::string content;
		std::fstream fileStream(filePath, std::fstream::in);

		if (!fileStream.is_open())
		{
			std::string error(filePath);
			error += ". Could not be read.";
			throw(std::runtime_error(error));
			return "";
		}

		std::string line = "";
		while (!fileStream.eof())
		{
			getline(fileStream, line);
			content.append(line + "\n");
		}

		fileStream.close();
		return content;
	}

};
