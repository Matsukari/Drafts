#include "Base.h"
#include "Graphics.h"

int main(int argc, char** argv)
{
	Engine::Window* window = new Engine::Window("Sample", 32, 720, 480, SDL_WINDOW_SHOWN, SDL_RENDERER_ACCELERATED);

	SDL_Event e;
	bool quit = false;
	while(!quit)
	{
		while(SDL_PollEvent(&e)) {
			if(e.type == SDL_QUIT)
				quit = true;
		}

		SDL_RenderPresent(window->renderer);
		window->Show_Fps();

		SDL_SetRenderDrawColor(window->renderer, 255, 0, 0, SDL_ALPHA_OPAQUE);
		SDL_RenderClear(window->renderer);

	}

	delete window;
	getchar();
	return 0;
}