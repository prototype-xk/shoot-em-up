#include <SDL3/SDL.h>
#include <stdbool.h>
#include "select_level.h"


int main(int argc, char* argv[]) {
	if (!SDL_Init(SDL_INIT_VIDEO)) {
		SDL_Log("Erreur SDL_Init: %s", SDL_GetError());
		return 1;
	}

	SDL_Window* window = SDL_CreateWindow("Bouton SDL 3", 800, 600, 0);
	SDL_Renderer* renderer = SDL_CreateRenderer(window, nullptr);


	Button myButton = createButton(100, 25, 200, 100, "World 1");
	Button myButton2 = createButton(100, 150, 200, 100, "World 2");
	Button myButton3 = createButton(100, 275, 200, 100, "World 3");
	Button myButton4 = createButton(100, 400, 200, 100, "World 4");

	bool running = true;
	SDL_Event e;

	while (running) {
		while (SDL_PollEvent(&e)) {
			if (e.type == SDL_EVENT_QUIT) running = false;

			handleButtonEvent(&myButton, &e);
			handleButtonEvent(&myButton2, &e);
			handleButtonEvent(&myButton3, &e);
			handleButtonEvent(&myButton4, &e);

			if (isButtonClicked(&myButton, &e)) SDL_Log("Bouton 1 cliqué !");
			if (isButtonClicked(&myButton2, &e)) SDL_Log("Bouton 2 cliqué !");
			if (isButtonClicked(&myButton3, &e)) SDL_Log("Bouton 3 cliqué !");
			if (isButtonClicked(&myButton4, &e)) SDL_Log("Bouton 4 cliqué !");
		}

		SDL_SetRenderDrawColor(renderer, 240, 240, 240, 255);
		SDL_RenderClear(renderer);

		renderButton(renderer, &myButton);
		renderButton(renderer, &myButton2);
		renderButton(renderer, &myButton3);
		renderButton(renderer, &myButton4);

		SDL_RenderPresent(renderer);
	}

	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();
	return 0;
}