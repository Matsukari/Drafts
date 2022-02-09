#pragma once
#define MISCELLANEOUS_H

#include <SDL.h>
#include <iostream>
#include <cstdlib>

#include <cstring>
#include <string>
#include <vector>


namespace Engine 
{

	std::string Get_LastLocation(std::string str);

	SDL_Rect Centered(SDL_Rect& rect, bool reference, bool resize);
	SDL_Rect Centered(SDL_Rect& rect, SDL_Rect bg, bool reference, bool resize);


	// For 1 side.
	void Extract_Spritesheet(std::vector<SDL_Rect>& vector, short frames, int size_w, int size_h, short cut_w, short cut_h,
		const char* extract_order);
	// For 2 Sides.
	void Extract_Spritesheet(std::vector<std::vector<SDL_Rect>>& vector, std::vector<int> frames, 
		short sheetw, short sheeth, short cut_w, short cut_h, const char* extract_order);
	// For 4 Sides.
	void Extract_Spritesheet(std::vector<std::vector<std::vector<SDL_Rect>>>& statevec, 
		std::vector<std::vector<int>> stateplace_index, short sheetw, short sheeth, short cut_w, short cut_h, const char* extract_order);
}