/*
**** DO NOT MODIFY THIS FILE ****

- Helpful, general OpenGL function
*/

#pragma once

#include <string>
#include <SDL/SDL_opengl.h>

void getError(const char * event);
std::string errorToString(GLenum err);