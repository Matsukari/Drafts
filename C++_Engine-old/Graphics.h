#pragma once
#define GRAPHICS_H

#include "Base.h"

namespace Engine 
{
	SDL_Texture* Load_Texture(SDL_Renderer* renderer, const char* path);

	// Unstable render position.
	void Draw_Text(SDL_Renderer* renderer, TTF_Font* font, std::string text, SDL_Color color, int x, int y, short w = 0, short h = 0, Uint8 alpha = SDL_ALPHA_OPAQUE);
	void Draw_Text(SDL_Renderer* renderer, TTF_Font* font, std::string text, SDL_Color color, SDL_Rect rect, Uint8 alpha = SDL_ALPHA_OPAQUE);


	// Addon draws
	void Draw_Rect_Fill(SDL_Renderer* renderer, _Color color, int x, int y, int w, int h, Uint8 alpha = SDL_ALPHA_OPAQUE);
	void Draw_Rect_Fill(SDL_Renderer* renderer, _Color color, SDL_Rect *rect, Uint8 alpha = SDL_ALPHA_OPAQUE);

	void Draw_Rect_Outline(SDL_Renderer* renderer, _Color color, int x, int y, int w, int h, Uint8 alpha = SDL_ALPHA_OPAQUE);
	void Draw_Rect_Outline(SDL_Renderer* renderer, _Color color, SDL_Rect* rect, Uint8 alpha = SDL_ALPHA_OPAQUE);

	void Draw_Line(SDL_Renderer* renderer, _Color color, int x1, int y1, int x2, int y2, Uint8 alpha = SDL_ALPHA_OPAQUE);
	void Draw_Pixel_Dot(SDL_Renderer* renderer, _Color color, int x, int y, Uint8 alpha = SDL_ALPHA_OPAQUE);

	void Draw_Grid_Line(SDL_Renderer* renderer, _Color color, int start_x, int start_y, int end_x, int end_y, int margin_x, int margin_y);




	class Texture 
	{
	public:
		Texture(SDL_Renderer* renderer, const char* ppath);
		Texture();
		~Texture();

		std::string path;
		SDL_RendererFlip flip = SDL_FLIP_NONE;
		double angle = 360.0;

		explicit operator SDL_Texture*() { return m_texture; }

		SDL_Texture* Get_SDL_Texture() { return m_texture; }
		void Load(SDL_Renderer* renderer, std::string ppath);
		void Load(SDL_Renderer* renderer, const char* ppath);

		void Render(SDL_Renderer* renderer, SDL_Rect* rect, SDL_Rect* clip = NULL);
		void Render(SDL_Renderer* renderer, int x, int y, int w, int h, SDL_Rect* clip = NULL);

		void Opacity(Uint8 alpha);

		void Color(_Color color);
		void Color(SDL_Color* color);
		void Color(Uint8 r, Uint8 g, Uint8 b);

		void Destroy(void);

	private:
		SDL_Texture* m_texture;
		bool m_is_loaded {false};
	};



	class Text_Type 
	{
	public:
		Text_Type(SDL_Renderer* renderer, const char* path, std::vector<std::string> placement, int cutw, int cuth);
		Text_Type(void);
		~Text_Type();

		void Font(SDL_Renderer* renderer, const char* path);
		void Font(SDL_Renderer* renderer, std::string path);
		void Config_Placement(std::vector<std::string> placement, int cutw, int cuth);

		void Render(SDL_Renderer* renderer, _Text text, Uint8 alpha = SDL_ALPHA_OPAQUE);
		
		void Destroy_Font();

	private:
		Texture m_texture;
		SDL_Rect m_rect;

		std::map<char, SDL_Rect> m_alphabet;
		std::map<char, SDL_Rect>::iterator m_it;

		int m_cutw = 0;
		int m_cuth = 0;
	};

}
