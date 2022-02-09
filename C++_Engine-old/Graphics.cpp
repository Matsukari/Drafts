#include "Graphics.h"


namespace Engine {

	// Early Functions --------------------------------------------------------------------------------------------------------------
	SDL_Texture* Load_Texture(SDL_Renderer* renderer, const char* path) 
	{
		SDL_Surface* surface = IMG_Load(path);
		SDL_Texture* texture = NULL;

		printf("\n Loaded Path: (%s)", path);


		if (surface != NULL) {
			texture = SDL_CreateTextureFromSurface(renderer, surface);
		}
		else {
			Force_Exit("Loaded Surface is NULL.");
		}

		SDL_FreeSurface(surface);

		return texture;
	}


	void Draw_Text(SDL_Renderer* renderer, TTF_Font* font, std::string text, SDL_Color color, int x, int y, short w, short h, Uint8 alpha) 
	{
		SDL_Surface* surface = TTF_RenderText_Solid(font, text.c_str(), color);
		if (surface == NULL) { Force_Exit("Loaded Surface is NULL."); }

		SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);


		SDL_Rect rect = {x, y, (int)((text.size() * 13) + (w)), h};
		SDL_RenderCopy(renderer, texture, NULL, &rect);

		SDL_FreeSurface(surface);
		SDL_DestroyTexture(texture);
	}
	void Draw_Text(SDL_Renderer* renderer, TTF_Font* font, std::string text, SDL_Color color, SDL_Rect rect, Uint8 alpha) 
	{
		SDL_Surface* surface = TTF_RenderText_Solid(font, text.c_str(), color);
		if (surface == NULL) { Force_Exit("Loaded Surface is NULL."); }

		SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);

		SDL_SetTextureAlphaMod(texture, alpha);

		SDL_Rect temprect = {rect.x, rect.y, (int)((text.size() * 13) + (rect.w)), rect.h};
		SDL_RenderCopy(renderer, texture, NULL, &temprect);

		SDL_FreeSurface(surface);
		SDL_DestroyTexture(texture);
	}



	// ADdon Draws ------------------------------------------------------------------------------------------------------------------
	void Draw_Rect_Fill(SDL_Renderer* renderer, _Color color, int x, int y, int w, int h, Uint8 alpha) 
	{
		SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, alpha);

		SDL_Rect rect = {x, y, w, h};
		SDL_RenderFillRect(renderer, &rect);
	}
	void Draw_Rect_Fill(SDL_Renderer* renderer, _Color color, SDL_Rect *rect, Uint8 alpha) 
	{
		SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, alpha);

		SDL_RenderFillRect(renderer, rect);
	}

	void Draw_Rect_Outline(SDL_Renderer* renderer, _Color color, int x, int y, int w, int h, Uint8 alpha) 
	{
		SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, alpha);

		SDL_Rect rect = {x, y, w, h};
		SDL_RenderDrawRect(renderer, &rect);
	}


	void Draw_Rect_Outline(SDL_Renderer* renderer, _Color color, SDL_Rect* rect, Uint8 alpha) 
	{
		SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, alpha);

		SDL_RenderDrawRect(renderer, rect);
	}

	void Draw_Line(SDL_Renderer* renderer, _Color color, int x1, int y1, int x2, int y2, Uint8 alpha) 
	{
		SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, alpha);

		SDL_RenderDrawLine(renderer, x1, y1, x2, y2);
	}

	void Draw_Pixel_Dot(SDL_Renderer* renderer, _Color color, int x, int y, Uint8 alpha) 
	{
		SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, alpha);

		SDL_RenderDrawPoint(renderer, x, y);
	}

	void Draw_Grid_Line(SDL_Renderer* renderer, _Color color, int start_x, int start_y, int end_x, int end_y, int margin_x, int margin_y)
	{
		// Vertical Lines.
		for (unsigned short i = start_x; i <= end_x; i += margin_x) {
			Draw_Line(renderer, color, i, 0, i, end_y);
		}

		// Horizontal Lines.
		for (unsigned short i = start_y; i <= end_y; i += margin_y) {
			Draw_Line(renderer, color, 0, i, end_x, i);
		}
	}





	// Texture class Functions -----------------------------------------------------------------------------------------------------
	Texture::Texture(SDL_Renderer* renderer, const char* ppath) 
	{
		Load(renderer, ppath);
	}
	Texture::Texture() 
	{
	
	}
	Texture::~Texture()
	{
		Destroy();
		m_texture = NULL;
	}

	void Texture::Load(SDL_Renderer* renderer, std::string ppath) 
	{
		Load(renderer, ppath.c_str());
	}
	void Texture::Load(SDL_Renderer* renderer, const char* ppath) 
	{
		if (m_is_loaded)
			Destroy();

		// fortunately, it works.  const char* -> std::string conrversion.
		path = ppath;

		// Packed with error suppression(not actually suppressing but countermeasure showing the error to fix it manually.)
		m_texture = Load_Texture(renderer, ppath);
		m_is_loaded = true;
	}

	// The Center parameter for rendercopyex is left NULL, i dont know how it works.
	void Texture::Render(SDL_Renderer* renderer, SDL_Rect* rect, SDL_Rect* clip) 
	{
		SDL_RenderCopyEx(renderer, m_texture, clip, rect, angle, NULL, flip);
	}
	void Texture::Render(SDL_Renderer* renderer, int x, int y, int w, int h, SDL_Rect* clip) 
	{
		SDL_Rect rect {x, y, w, h};
		SDL_RenderCopyEx(renderer, m_texture, clip, &rect, angle, NULL, flip);
	}


	void Texture::Opacity(Uint8 alpha) 
	{
		SDL_SetTextureAlphaMod(m_texture, alpha);
	}

	void Texture::Color(_Color color) 
	{
		SDL_SetTextureColorMod(m_texture, color.r, color.g, color.b);
	}
	void Texture::Color(SDL_Color* color) 
	{
		SDL_SetTextureColorMod(m_texture, color->r, color->g, color->b);
	}
	void Texture::Color(Uint8 r, Uint8 g, Uint8 b) 
	{
		SDL_SetTextureColorMod(m_texture, r, g, b);
	}


	void Texture::Destroy() 
	{
		printf("\nDestroying Texture: %s...", path.c_str());

		SDL_DestroyTexture(m_texture); 
		m_texture = NULL;
	}





	// Text_Type class Functions ---------------------------------------------------------------------------------------------------------
	Text_Type::Text_Type(SDL_Renderer* renderer, const char* path, std::vector<std::string> placement, int cutw, int cuth)
	{
		Font(renderer, path);
		Config_Placement(placement, cutw, cuth);
	}
	Text_Type::Text_Type() 
	{
		
	}
	Text_Type::~Text_Type() 
	{
		// Destroy_Font();
		// Since the texture has destructor that automatically destroy texture once out of scope. But, Carefull.
	}

	void Text_Type::Font(SDL_Renderer* renderer, const char* path) 
	{
		m_texture.Load(renderer, path);
	}
	void Text_Type::Font(SDL_Renderer* renderer, std::string path) 
	{
		m_texture.Load(renderer, path);
	}
	void Text_Type::Config_Placement(std::vector<std::string> placement, int cutw, int cuth) 
	{
		m_cutw = cutw;
		m_cuth = cuth;

		for (int i = 0; i < placement.size(); i++) {
			for (int j = 0; j < placement[i].size(); j++) {
				m_alphabet[placement[i][j]] = {j*cutw, i*cuth, cutw, cuth};
			}
		}
	}
	void Text_Type::Render(SDL_Renderer* renderer, _Text text, Uint8 alpha) 
	{
		m_texture.Color(text.color);
		m_texture.Opacity(alpha);

		for (int i = 0; i < text.Len(); i++) {
			m_rect = {text.rect.x + ( (((text.rect.w*2)/2)+m_cutw) * i) + (text.space_margin), 
				text.rect.y, (text.rect.w * 2), (text.rect.h * 2)};

			m_it = m_alphabet.find(text.texts[i]);
			m_texture.Render(renderer, &m_rect, &m_it->second); // it->second = SDL_Rect(rect of of placement of alphabet).
		}
	}
	void Text_Type::Destroy_Font() 
	{
		m_texture.Destroy();
	}

}
