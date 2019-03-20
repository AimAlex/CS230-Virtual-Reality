/*
**** DO NOT MODIFY THIS FILE ****

-This file contains the SDL wrapper class.
-It is just a convenient wrapper around SDL to minimize interaction with it.
-You can take a look at how to initialize a SDL and OpenGL context here, but do not modify the file.
*/

#include "sdlWrapper.h"
#include "utils.h"

sdlWrapper::sdlWrapper(int width, int height, std::string windowName, int screenNum, bool bFullScreen)
{
	_width = width;
	_height = height;
	_windowName = windowName;
	
	initOpenGL(screenNum, bFullScreen);
	getError("initSDL");
}

sdlWrapper::~sdlWrapper() {
	shutdown();
}

void sdlWrapper::initOpenGL(int screenNum, int bFullScreen) {
	// Initialize SDL's Video subsystem
	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		throw(std::runtime_error("Unable to initialize SDL."));
	}

	// Screen bounds
	SDL_Rect bounds[2];
	SDL_GetDisplayBounds(0, &bounds[0]);
	SDL_GetDisplayBounds(1, &bounds[1]);
    
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_ACCELERATED_VISUAL, 1);
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);	// Create our window
	
    Uint32 flags = SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE;
	if (bFullScreen) { flags = flags | SDL_WINDOW_FULLSCREEN_DESKTOP; }
	_mainwindow = SDL_CreateWindow(	_windowName.c_str(),
									bounds[screenNum].x + 100, bounds[screenNum].y + 100,
									_width, _height, flags);

	if (!_mainwindow) /* Die if creation failed */
	{
		std::cout << "SDL Error: " << SDL_GetError() << std::endl;
		SDL_Quit();
	}

	// Request opengl 4.1 context.
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
	SDL_GL_SetAttribute(SDL_GL_ACCELERATED_VISUAL, 1);
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);

	// Create our opengl context and attach it to our window
	_maincontext = SDL_GL_CreateContext(_mainwindow);

	GLenum rev;
	glewExperimental = GL_TRUE;
	rev = glewInit();
	if (GLEW_OK != rev)
	{
		std::cout << "Error: " << glewGetErrorString(rev) << std::endl;
		exit(1);
	}
	else
	{
		std::cout << "GLEW Init: Success!" << std::endl;
		glGetError(); // Gets rid of thrown GL_INVALID_ENUM after glewInit is called (bug in glew)
	}

	std::cout << "OpenGL Version : " << glGetString(GL_VERSION) << std::endl;
    std::cout << "GLSL Version : " << glGetString(GL_SHADING_LANGUAGE_VERSION) << std::endl;
	std::cout << "Renderer       : " << glGetString(GL_RENDERER) << std::endl;
	std::cout << "Vendor         : " << glGetString(GL_VENDOR) << std::endl;

	glEnable(GL_DEPTH_TEST);

	getError("setupOpenGL");
}

void sdlWrapper::shutdown() {
	SDL_GL_DeleteContext(_maincontext);
	SDL_DestroyWindow(_mainwindow);
	SDL_Quit();
}
