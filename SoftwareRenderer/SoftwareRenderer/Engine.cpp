#include "App.h"

int main(int argc, char* argv[])
{
	// Start SDL2
	SDL_Init(SDL_INIT_EVERYTHING);

	// Initialize the engine and user code from app
	App engine;
	engine.EngineInitialize();
	engine.EngineLoad();


	// Main game loop
	SDL_Event e;
	while (engine.IsRunning())
	{
		engine.Tick();
		SDL_SetWindowTitle(engine.GetSDLWindow(), std::to_string(engine.GetFrameTime()).c_str());

		// Clear Input
		// previousKeyState = currentKeyState
		// currentKeyState = Nothing
		engine.PushKeyState();

		// Fill currentKeyState
		while (SDL_PollEvent(&e) != 0)
		{
			// Call input to notify
			engine.Input(e);
			if (e.type == SDL_QUIT)
			engine.Quit();
		}

		// Return control to programmer for implementation
		engine.EngineUpdate();     

		// Draw a pretty picture
		engine.EngineDraw();
	}

	engine.EngineUnLoad();

	SDL_Quit();

	return 0;
}


Engine::Engine() : mRunning(true), fTime(0.0f), prevTime(0), currTime(0)
{
	UINT width = 1600;
	UINT height = 900;
	sDevice.InitDevice(width, height);

	mWindow = SDL_CreateWindow("Demo", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, SDL_WINDOW_SHOWN);

	if (mWindow == NULL)
		throw "Could not create SDL Window";

	mRenderer = SDL_CreateRenderer(mWindow, -1, SDL_RENDERER_SOFTWARE);
	mTexture = SDL_CreateTexture(mRenderer, SDL_PIXELFORMAT_RGB24, SDL_TEXTUREACCESS_STREAMING, sDevice.screen.GetScreenWidth(), sDevice.screen.GetScreenHeight());
}

Engine::~Engine()
{
   
}


void Engine::EngineInitialize()
{
	Initialize();
}

// Virtual Method Implemented in App
void Engine::Initialize()
{

}


void Engine::EngineLoad()
{
	Load();
}

// Virtual Method Implemented in App
void Engine::Load()
{

}

void Engine::Tick()
{
	prevTime = currTime;
	currTime = SDL_GetTicks();
	fTime = (float)(currTime - prevTime) / 1000.0f;
}

void Engine::EngineUpdate()
{
	Update();
}

void Engine::Update()
{

}

void Engine::Input(SDL_Event e)
{
	switch (e.type)
	{
	case SDL_KEYDOWN:
		KeyIsDown(e.key.keysym.sym);
		break;

	case SDL_KEYUP:
		KeyIsUp(e.key.keysym.sym);
		break;  

	default:
		break;
	}
}

inline void Engine::PushKeyState()
{
   // Put previousKeyState in currentKeyState
	memcpy(&mPreviousKeyState, mCurrentKeyState, INPUT_SIZE);
}


void Engine::EngineUnLoad()
{
   // Cleanup and Quit
	UnLoad();
	SDL_DestroyWindow(mWindow);
	SDL_DestroyRenderer(mRenderer);
	SDL_DestroyTexture(mTexture);
}

// Virtual Method Implemented in App
void Engine::UnLoad()
{
	//delete 
}


void Engine::EngineDraw()
{
	// Prepare contents on screen buffer
	Draw();

	// Display texture
	unsigned char* frameBuffer = sDevice.screen.GetBuffer();
	SDL_UpdateTexture(mTexture, NULL, frameBuffer, sDevice.screen.GetScreenWidth() * 3 * sizeof(unsigned char));
	SDL_RenderClear(mRenderer);
	SDL_RenderCopyEx(mRenderer, mTexture, NULL, NULL, 0, 0, SDL_FLIP_VERTICAL);
	SDL_RenderPresent(mRenderer);
}

// Virtual Method Implemented in App
void Engine::Draw()
{

}