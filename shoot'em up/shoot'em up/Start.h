#pragma once
#include <SDL3/SDL.h>
#include "Button.h"

class Start
{
public:
	Start();
	bool initializeSDL();
	bool CreateWindowAndRenderer(SDL_Window*& window, SDL_Renderer*& renderer);

	Button startButton;
	Button leaveButton;
};

