#pragma once
#ifndef WINDOW_H
#define WINDOW_H

#include <glew/GL/glew.h>
#include <SDL/SDL.h>
#include <SDL/SDL_opengl.h>
#include <SDL/SDL_mixer.h>
#include <iostream>

//Set up the GL3_Prototypes
#define GL3_PROTOTYPES 1

namespace ENGINE {

	class Window
	{
	public:
		// Constructor
		Window();
		~Window();

		// Initailize window
		bool Initialize(std::string windowName, int initWidth, int initHeight);
		// Clean Up
		void Shutdown();

		// Getters and Setters
		SDL_Window* GetWindow() const;
		int GetWidth() const;
		int GetHeight() const;
		void SetWindowSize(const int setWidth, const int setHeight);
		void SetFullScreen(bool setFullscreen);

		// Will print opengl information
		void GetInstalledOpenGLInfo();

	private:
		// This will set all the openGL attributes for the window
		void SetAttributes();

		// SDL window and context
		SDL_Window* SDLWindow; 
		SDL_GLContext SDLGLContext;

		// Window properties
		int width;
		int height;
		bool isFullScreen = false;
	};
}

#endif
