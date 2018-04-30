#include "Window.h"

using namespace ENGINE;

Window::Window() : SDLWindow(nullptr) 
{

}

Window::~Window()
{
	Shutdown();
}

bool Window::Initialize(std::string windowName, int initWidth, int initHeight) 
{
	// Initialize SDL
	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_JOYSTICK | SDL_INIT_AUDIO | SDL_INIT_HAPTIC) > 0) {
		std::cout << "Failed to init SDL!" << std::endl;
		return false;
	}
	std::cout << "SDL Initialized" << std::endl;

	width = initWidth;
	height = initHeight;

	// Create Window
	SDLWindow = SDL_CreateWindow(windowName.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, initWidth, initHeight, SDL_WINDOW_OPENGL | SDL_WINDOW_HIDDEN);

	// V-Sync
	SDL_GL_SetSwapInterval(1);

	// Check to make sure the window was created properly
	if (!SDLWindow) {
		std::cout << "Unable to create Window" << std::endl;
		Shutdown();
		return false;
	}

	// Create an SDL/GL context=
	SDLGLContext = SDL_GL_CreateContext(SDLWindow);

	// Set the Attributes that will be used within the window
	SetAttributes();

	//Initialize glew and get the error message from the initialize function
	GLenum err = glewInit();
	if (err != GLEW_OK) {
		std::cout << "glewInit failed, aborting." << std::endl;
		std::cout << "Error: " << glewGetErrorString(err) << std::endl;
		return false;
	}
	std::cout << "GLEW Initialized" << std::endl;

	// Print OpenGl Information
	GetInstalledOpenGLInfo();

	// Initialize Audio
	if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 512) == -1)


	{
		std::cout << "Audio was not initialized" << std::endl;
		return false;
	}
	std::cout << "Audio initialized" << std::endl;

	return true;
}

void Window::Shutdown() 
{
	//Clean up the window
	Mix_CloseAudio();
	Mix_Quit();
	SDL_GL_DeleteContext(SDLGLContext);
	SDL_DestroyWindow(SDLWindow);
	SDLWindow = nullptr;	
}

void Window::SetAttributes() 
{
	//Ignore any depricated functions
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

	//Set up the version of OpenGL that will be used (3.3 covers MOST computers)
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
	//Creates a double buffer within the game
	//This allows one surface to render in the background
	//The buffers then swap when the back on is filled,
	//and the process repeats
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	//This sets up the depth buffer
	//This will help the GPU determine which objects are in front of others (perspective)
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 32);
	// This is used for anti-aliasing
	//SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS, 1);
	SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, 4);
}

SDL_Window* Window::GetWindow() const {
	return SDLWindow;
}

int Window::GetWidth() const {
	return width;
}

int Window::GetHeight() const {
	return height;
}

void Window::SetWindowSize(const int setWidth, const int setHeight) 
{
	width = setWidth;
	height = setHeight;
}

void Window::SetFullScreen(bool setFullscreen) 
{
	isFullScreen = setFullscreen;
	SDL_SetWindowFullscreen(SDLWindow, (isFullScreen ? SDL_WINDOW_FULLSCREEN_DESKTOP : SDL_WINDOW_SHOWN));
}

void Window::GetInstalledOpenGLInfo() 
{
	// You can to get some info regarding versions and manufacturer
	const GLubyte *version = glGetString(GL_VERSION);
	// You can also get the version as ints	
	const GLubyte *vendor = glGetString(GL_VENDOR);
	const GLubyte *renderer = glGetString(GL_RENDERER);
	const GLubyte *glslVersion = glGetString(GL_SHADING_LANGUAGE_VERSION);

	std::cout << "GL Version " << version << std::endl;
	std::cout << "Graphics card vendor " << vendor << std::endl;
	std::cout << "Graphics card name " << renderer << std::endl;
	std::cout << "GLSL Version " << glslVersion << std::endl;
}