#include "Miscellaneous.h"



namespace Engine {

	std::string Get_LastLocation(std::string str) 
	{
		std::string last;
		for (int i = str.size()-1; i > 0; i--) {
			if (str[i] == '/') {
				last = (str.substr(i, str.size()-1));
				printf("\nLast Location(%s)", last.c_str());
			}
		}
		return last;
	}

	SDL_Rect Centered(SDL_Rect& rect, bool reference, bool resize) 
	{
		//if (referece) {
		//	SDL_Rect temprect = {	(rect.x + (rect.w/2)),  (rect.y + (rect.h/2)), 	(rect.w)};
		//}
		return SDL_Rect{0, 0, 0, 0};
	}
	SDL_Rect Centered(SDL_Rect& rect, SDL_Rect bg, bool reference, bool resize)
	{
		return SDL_Rect{0, 0, 0, 0};
	}

	void Extract_Spritesheet(std::vector<SDL_Rect>& vector, short frames, int size_w, int size_h, short cut_w, short cut_h,
		const char* extract_order) 
	{
		SDL_Rect frame_box;
		short extracted = 0;

		for (short col = 0; col <= (short)(cut_h * (size_h / cut_h)); col += cut_h) {
			for (short row = 0; row <= (short)(cut_w * (size_w / cut_w)); row += cut_w) {

				if (strcmp(extract_order, "left_to_right") == 0)
					frame_box = {row, col, cut_w, cut_h};
				else if (strcmp(extract_order, "top_to_bottom") == 0)
					frame_box = {col, row, cut_w, cut_h};
				else {
					printf("\nProvided argument for (Extract Order) unidentified.");
					return;
				}

				vector.push_back(frame_box);
				extracted++;

				if (extracted >= frames)
					return;
			}
		}
	}

	// For 2 Sides.
	void Extract_Spritesheet(std::vector<std::vector<SDL_Rect>>& vector, std::vector<int> frames, 
		short sheetw, short sheeth, short cut_w, short cut_h, const char* extract_order) 
	{
		SDL_Rect frame_box;
		short current_state = 0; //index
		short extracted = 0;

		for (short col = 0; col <= (short)(cut_h * (sheeth/cut_h)); col += cut_h) {
			for (short row = 0; row <= (short)(cut_w * (sheetw/cut_w)); row += cut_w) {

				if (strcmp(extract_order, "left_to_right") == 0)
					frame_box = {row, col, cut_w, cut_h};
				else if (strcmp(extract_order, "top_to_bottom") == 0)
					frame_box = {col, row, cut_w, cut_h};
				else {
					printf("\nProvided argument for (Extract Order) unidentified.");
					return;
				}


				vector[current_state].push_back(frame_box);
				extracted++;

				if (extracted >= frames[current_state]){
					current_state++;
					extracted = 0;

					// If the stated extracted go beyond provided index argument: either FINISHED or FAILED
					if (current_state >= frames.size()) {
						printf("\nExtracted Result: %i, Original: %i", current_state, frames.size());
						return;
					}
				}

			}
		}
	}

	// For 4 Sides.
	void Extract_Spritesheet(std::vector<std::vector<std::vector<SDL_Rect>>>& statevec, 
		std::vector<std::vector<int>> stateplace_index, short sheetw, short sheeth, short cut_w, short cut_h, const char* extract_order) 
	{
		SDL_Rect frame_box;
		short current_state = 0; //index
		short extracted = 0;


		// For equal extraction
		for (short col = 0; col <= (short)(cut_h * (sheeth/cut_h)); col += cut_h) {
			for (short row = 0; row <= (short)(cut_w * (sheetw/cut_w)); row += cut_w) {

				if (strcmp(extract_order, "left_to_right") == 0)
					frame_box = {row, col, cut_w, cut_h};
				else if (strcmp(extract_order, "top_to_bottom") == 0)
					frame_box = {col, row, cut_w, cut_h};
				else {
					printf("\nProvided argument for (Extract Order) unidentified.");
					return;
				}

				statevec[stateplace_index[current_state][0]][stateplace_index[current_state][1]].push_back(frame_box);
				extracted++;

				if (extracted >= stateplace_index[current_state][2]){
					current_state++;
					extracted = 0;
					// If the stated extracted go beyond provided index argument: either FINISHED or FAILED
					if (current_state >= stateplace_index.size()-1) {
						printf("\nExtracted Result: %i");
						return;
					}
				}

			}
		}
		
	}

}