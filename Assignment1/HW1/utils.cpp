/*
**** DO NOT MODIFY THIS FILE ****

- Helpful, general OpenGL function
*/

#include "utils.h"
#include <SDL/SDL_opengl.h>
#include <iostream>

void getError(const char * event)
{
	GLenum err = glGetError();
	if (err != GL_NO_ERROR)
	{
		std::string errorMsg = "OpenGL Error: " + errorToString(err) + ". After event " + event;
		throw std::runtime_error(errorMsg);
	}
}

std::string errorToString(GLenum err)
{
	switch (err)
	{
	case GL_INVALID_ENUM:
		return "GL_INVALID_ENUM";
	case GL_INVALID_VALUE:
		return "GL_INVALID_VALUE";
	case GL_INVALID_OPERATION:
		return "GL_INVALID_OPERATION";
	case GL_STACK_OVERFLOW:
		return "GL_STACK_OVERFLOW";
	case GL_STACK_UNDERFLOW:
		return "GL_STACK_UNDERFLOW";
	case GL_INVALID_FRAMEBUFFER_OPERATION:
		return "GL_INVALID_FRAMEBUFFER_OPERATION";
	case GL_OUT_OF_MEMORY:
		return "GL_OUT_OF_MEMORY";
	}

	return "";
}
