#define WIN32_LEAN_AND_MEAN

#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <sstream>


// Need to link with Ws2_32.lib, Mswsock.lib, and Advapi32.lib
#pragma comment (lib, "Ws2_32.lib")
#pragma comment (lib, "Mswsock.lib")
#pragma comment (lib, "AdvApi32.lib")

#include "BFEngine.h"

using namespace ENGINE;

//Initialize static variables
std::unique_ptr<BFEngine> BFEngine::BFEngineInstance(nullptr);
Window* BFEngine::window(nullptr);

//Initialize member variables
BFEngine::BFEngine() : isRunning(false), firstLoad(true), sceneManager(nullptr), windowName("Blue Flame Engine"), width(1280), height(720)
{

}

BFEngine::~BFEngine() 
{
	cout << "Shutting down..." << endl;

	closeNetwork();

	delete sceneManager;
	sceneManager = nullptr;
	
	TerminateGame();
}

void BFEngine::TerminateGame() 
{
	exit(0);

	window->Shutdown();
	window = nullptr;	

	SDL_Quit();
}

BFEngine* BFEngine::GetInstance() 
{
	if (BFEngineInstance.get() == nullptr) {
		BFEngineInstance.reset(new BFEngine);
	}
	return BFEngineInstance.get();
}

bool BFEngine::Initialize() 
{
	// Create a new window
	window = new Window();
	// Iniitalize the window with the window name and size
	window->Initialize(windowName, width, height);
	cout << "Window initialized to " << window->GetWidth() << " by " << window->GetHeight() << endl;
	
	// Initialize Scene Manager
	sceneManager = new SceneManager();
	sceneManager->Initialize(window);
	cout << "Scene Manager Initialized" << endl;

	// Initialize clock
	Clock::GetInstance()->init();
	cout << "Clock Initialized" << endl;

	// Set up controllers
	InputManager::GetInstance()->initalizeControllers();

	cout << "BFEngine Initialized" << endl;

	Settings::getInstance()->loadSettings("settings.txt");

	if (Settings::getInstance()->networkedGame) {
		if (Settings::getInstance()->isServer) {
			return setUpNetworkAsServer();
		}
		else {
			return setUpNetworkAsClient();
		}
	}

	return true;
}

void BFEngine::Run() 
{
	// Set the engine to running
	isRunning = true;

	// Set up timer
	Timer::GetInstance().Start();

	// Number of fixed ticks per second
	const int ticksPerSecond = 100;

	// The amount of ticks to skip per frame
	const int skipTicks = 1000 / ticksPerSecond;

	// The amount of frames the physics step can skip before slowing down
	const int maxFrameSkip = 5;

	// The next game tick the fixed loop will take place at
	DWORD nextGameTick = GetTickCount();

	// Number of loops the fixed update has between the rendered frames
	int loops;

	// Set the fixed update to run 120 times per second
	// This keeps it in the multiples of common refresh rates
	double fixedDeltaTime = 8.3333333333333333;

	// Current time since the system has started
	double currentTime = GetTickCount();

	// Accumulator that keeps track of the time between fixed updates and rendered frames 
	double accumulator = 0.0;

	while (isRunning)
	{
		// Check if the scene manager has quit
		if (sceneManager->IsQuit()) {
			isRunning = false;
		}

		// Calculate frame time between rendering
		double newTime = GetTickCount();
		double frameTime = newTime - currentTime;

		// Clamp the frame time so in cases where the rendered frames suffer,
		// slowing down the fixed update to help adjust
		if (frameTime > 250.0)
			frameTime = 250.0;

		currentTime = newTime;

		accumulator += frameTime;

		// This loop is called dependent on the fixedDeltaTime
		while (accumulator >= fixedDeltaTime) {

			// Game and physics logic that rely on a constant behaviour 
			// between systems should be done in this update
			UpdateState();
			FixedUpdate(fixedDeltaTime * 0.001);
			accumulator -= fixedDeltaTime;
		}

		// Update timer
		Timer::GetInstance().Update();

		// Because a rendering frame can occur between two fixed updates, the renderer
		// can assume based on input and velocity from the previous frame
		// where the objects in a scene will be in the next rendering frame
		// This interpolation represents that inbetween render state of the objects
		// and is passed to the renderer
		interpolation = accumulator / fixedDeltaTime;

		// Other graphic logic that is not physics and game dependent should
		// be done as desired in this update

		Update(Timer::GetInstance().GetDeltaTime());
		PreRender();
		Render();
		PostRender();

		// Only display window after the first render has been called
		if (firstLoad) {
			cout << "Displaying window..." << endl;
			SDL_ShowWindow(window->GetWindow());
			firstLoad = false;
		}
	}
}

void BFEngine::UpdateState() {
	sceneManager->UpdateState();
}

void BFEngine::FixedUpdate(const float deltaTime) 
{
	sceneManager->FixedUpdate(deltaTime);
	sceneManager->HandleEvents();
}

void BFEngine::Update(const float deltaTime) 
{
	sceneManager->Update(deltaTime);
}

void BFEngine::PreRender() 
{
	sceneManager->PreRender();
}

void BFEngine::Render() 
{
	sceneManager->Render(interpolation);
}

void BFEngine::PostRender() 
{
	sceneManager->PostRender();
}

SceneManager* BFEngine::GetSceneManager() 
{
	return sceneManager;
}

void BFEngine::SetWindowName(std::string name)
{
	windowName = name;
}

void BFEngine::SetWindowDimensions(int width, int height)
{
	this->width = width;
	this->height = height;
}

// Networking

bool BFEngine::setUpNetworkAsServer() {
	///networking block
	struct addrinfo *result = NULL;
	struct addrinfo hints;

	// Initialize Winsock
	iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (iResult != 0) {
		printf("WSAStartup failed with error: %d\n", iResult);
		return false;
	}

	ZeroMemory(&hints, sizeof(hints));
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;
	hints.ai_flags = AI_PASSIVE;

	// Resolve the server address and port
	iResult = getaddrinfo(NULL, DEFAULT_PORT, &hints, &result);
	if (iResult != 0) {
		printf("getaddrinfo failed with error: %d\n", iResult);
		WSACleanup();
		return false;
	}

	// Create a SOCKET for connecting to server
	ListenSocket = socket(result->ai_family, result->ai_socktype, result->ai_protocol);
	if (ListenSocket == INVALID_SOCKET) {
		printf("socket failed with error: %ld\n", WSAGetLastError());
		freeaddrinfo(result);
		WSACleanup();
		return false;
	}

	u_long iMode = 1; //supposedly sets nonblocking i think
	if (ioctlsocket(ListenSocket, FIONBIO, &iMode) == SOCKET_ERROR) {
		printf("ioctlsocket() failed with error %d\n", WSAGetLastError());
		return 1;
	}
	iResult = bind(ListenSocket, result->ai_addr, (int)result->ai_addrlen);
	if (iResult == SOCKET_ERROR) {
		printf("bind failed with error: %d\n", WSAGetLastError());
		freeaddrinfo(result);
		closesocket(ListenSocket);
		WSACleanup();

	}

	freeaddrinfo(result);


	iResult = listen(ListenSocket, SOMAXCONN);
	if (iResult == SOCKET_ERROR) {
		printf("listen failed with error: %d\n", WSAGetLastError());
		closesocket(ListenSocket);
		WSACleanup();
	}
	int x = 0;
	do {
		// Accept a client socket
		ClientSocket = accept(ListenSocket, (struct sockaddr*)&client_info, &addrsize);
		if (ClientSocket == INVALID_SOCKET) {
			printf("accept failed with error: %d\n", WSAGetLastError());
			//closesocket(ListenSocket);
			//WSACleanup();
		}
		else {
			x = 1;
			printf("accept SUCCEEDED");

			//ufds[0].fd = ClientSocket;
			//ufds[0].events = POLLIN; // check for just normal data

									 // No longer need server socket
			closesocket(ListenSocket);

			clientTable.push_back(spacer);
			clientTable.push_back("===== Client IP Address ==== Gamer Tag =====");
			clientTable.push_back(spacer);

			cout << "Networking Initialized" << endl;
		}
	} while (x == 0);

	return true;
}

bool BFEngine::setUpNetworkAsClient() {
	struct addrinfo *result = NULL,
		*ptr = NULL,
		hints;

	// Initialize Winsock
	iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (iResult != 0) {
		printf("WSAStartup failed with error: %d\n", iResult);
		return false;
	}

	ZeroMemory(&hints, sizeof(hints));
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;

	// Resolve the server address and port
	if (Settings::getInstance()->serverIPAddress == "") {
		iResult = getaddrinfo(NULL, DEFAULT_PORT, &hints, &result);
	}
	else {
		iResult = getaddrinfo(Settings::getInstance()->serverIPAddress.c_str(), DEFAULT_PORT, &hints, &result);
	}

	if (iResult != 0) {
		printf("getaddrinfo failed with error: %d\n", iResult);
		WSACleanup();
		return false;
	}
	else {
		printf("getaddrinfo succeeded: %d\n", iResult);
	}

	// Attempt to connect to an address until one succeeds
	for (ptr = result; ptr != NULL; ptr = ptr->ai_next) {
		// Create a SOCKET for connecting to server

		//u_long iMode = 1; //supposedly sets nonblocking i think
		//ioctlsocket(ClientSocket, FIONBIO, &iMode);


		ClientSocket = socket(ptr->ai_family, ptr->ai_socktype,
			ptr->ai_protocol);
		if (ClientSocket == INVALID_SOCKET) {
			printf("socket failed with error: %ld\n", WSAGetLastError());
			WSACleanup();
			return false;
		}
		else {
			//ufds[0].fd = ClientSocket;
			//ufds[0].events = POLLIN; // check for just normal data
		}

		// Connect to server.
		iResult = connect(ClientSocket, ptr->ai_addr, (int)ptr->ai_addrlen);
		if (iResult == SOCKET_ERROR) {
			closesocket(ClientSocket);
			ClientSocket = INVALID_SOCKET;
			continue;
		}
		break;
	}
	return true;
}

std::string BFEngine::receiveData() {
		iResult = recv(ClientSocket, recvbuf, recvbuflen, 0);

		//printf("%i", sizeof(recvbuf), "\n");
		memcpy(connectedClientName, recvbuf, 256);

		if (iResult > 0) {
			char ipstr[INET6_ADDRSTRLEN];
			inet_ntop(AF_INET, &client_info.sin_addr, (PSTR)ipstr, sizeof(ipstr));
			return connectedClientName;
		}
		else if (iResult == 0)
			printf("Connection closing...\n");
		else {
			//printf("recv failed with error: %d\n", WSAGetLastError());
			//closesocket(ClientSocket);
			//WSACleanup();
			return "";
		}
}

void BFEngine::sendData(std::string data) {
	const char* sendbuf = data.c_str();
	//printf("\n\n", sendbuf);

	// Send an initial buffer
	iResult = send(ClientSocket, sendbuf, (int)strlen(sendbuf) + 1, 0);
	if (iSendResult == SOCKET_ERROR) {
		printf("send failed with error: %d\n", WSAGetLastError());
		//closesocket(ClientSocket);
		//WSACleanup();
	}
	else {
		//printf("sending buffer back to sender\n\n");
	}
}

int BFEngine::closeNetwork() {
	iResult = shutdown(ClientSocket, SD_SEND);
	if (iResult == SOCKET_ERROR) {
		printf("shutdown failed with error: %d\n", WSAGetLastError());
		closesocket(ClientSocket);
		WSACleanup();
		return 1;
	}

	// cleanup
	closesocket(ClientSocket);
	WSACleanup();
	return 0;
}