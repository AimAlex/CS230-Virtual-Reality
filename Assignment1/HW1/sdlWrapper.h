/*
**** DO NOT MODIFY THIS FILE ****

-This file contains the SDL wrapper class.
-It is just a convenient wrapper around SDL to minimize interaction with it.
-You can take a look at how to initialize a SDL and OpenGL context here, but do not modify the file.
*/

#pragma once

#include <SDL/SDL.h>
#include <GL/glew.h>
#include <SDL/SDL_opengl.h>
#include <iostream>

class sdlWrapper
{
public:
	sdlWrapper(int width, int height, std::string windowName, int screenNum, bool bFullScreen);
	~sdlWrapper();

	void initOpenGL(int screenNum, int bFullScreen);
	void shutdown();
		
	void toggleFullScreen() {
		Uint32 FullscreenFlag = SDL_WINDOW_FULLSCREEN_DESKTOP;
		bool IsFullscreen = SDL_GetWindowFlags(_mainwindow) & FullscreenFlag;
		SDL_SetWindowFullscreen(_mainwindow, IsFullscreen ? 0 : FullscreenFlag);
	}

	void swapbuffer() {
		SDL_GL_SwapWindow(_mainwindow);
	}

	SDL_GLContext	getContext()	  { return & _maincontext; }
	SDL_Window *	getWindow()		  { return _mainwindow; }
	
	int	getWidth()  {
		SDL_GetWindowSize(_mainwindow, &_width, &_height);
		return _width;
	}
	int	getHeight() {
		SDL_GetWindowSize(_mainwindow, &_width, &_height);
		return _height;
	}

private:
	// Window width, height, and name
	int _width; 
	int _height;
	std::string _windowName;
	
	SDL_Window * _mainwindow; /* Our window handle */
	SDL_GLContext _maincontext; /* Our opengl context handle */
};

