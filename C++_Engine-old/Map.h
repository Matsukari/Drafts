#pragma once
#define MAP_H

#include <SDL.h>
#include <iostream>
#include <cstdlib>

#include <string>
#include <vector>

namespace Engine 
{
	class Map_Structure 
	{
	public:
		Map_Structure(void);
		~Map_Structure(void);
		
		std::vector<std::string> Texture_Map(void);
		std::vector<std::string> Text_Map(void);

		// Only once
		void Set_TextureMap(std::vector<std::string>& map);
		void Set_TextMap(std::vector<std::string>& map);

		int Last_Texture(void);

		void Clear(void);

		// Add new build in map: String case : 
		void Build_Text(char build_code, std::string text, SDL_Color color, SDL_Rect rect, Uint8 alpha = SDL_ALPHA_OPAQUE);
		void Build_Text(char build_code, std::string text, SDL_Color color, unsigned int w = Engine::game->Tilesize(), unsigned int h = Engine::game->Tilesize(), Uint8 alpha = SDL_ALPHA_OPAQUE);

		void Build_Texture(char build_code, std::string path, unsigned int w = Engine::game->Tilesize(), unsigned int h = Engine::game->Tilesize(), Uint8 alpha = SDL_ALPHA_OPAQUE, SDL_Color* col = NULL);
		void Build_Texture(char build_code, std::string path, SDL_Rect rect, Uint8 alpha = SDL_ALPHA_OPAQUE, SDL_Color* col = NULL);

		void Build(unsigned int x_margin = Engine::game->Tilesize(), unsigned int y_margin = Engine::game->Tilesize());

		void Render_Text(TTF_Font* font);
		void Render_Texture(void);

		void Destroy_Texture(void);


		void Camera(Character& target, unsigned int min = 250, unsigned int max = Engine::game->Screen_W()-250);
		void CameraPos_Centered(Character& entity, bool animate = false, int speed = 3);

		int Collide_GetPosition(char build_code, Character& obj, int dir);

	private:

		// Self Arrays: All of grouping will take place in initilizin
		std::vector<std::string> m_text_map;
		std::vector<std::string> m_texture_map;

		std::vector<char> m_build_code;
		std::vector<char> m_build_type;

		std::vector<std::string> m_build_text;
		std::vector<Texture> m_build_texture;

		std::vector<SDL_Rect> m_text_rect;
		// 2 dimension: 1- type of texture 2 - ammount of texture; separate rect 
		std::vector<std::vector<SDL_Rect>> m_texture_rect; 

		std::vector<SDL_Color> m_text_color;
		std::vector<Uint8> m_text_alpha;

		bool once = true;

	};


	class Map 
	{
	public:
		Map(void);
		~Map(void);

		virtual void Init(void);
		virtual void Create_Layer(unsigned short layers);

		virtual void Create(void);

		virtual void Render(unsigned short layer);
		virtual void Render_All(void);

		virtual void Camera(Character& character, unsigned short layer);
		virtual void Camera_All(Character& character);

		virtual void Events(void);
		virtual void Update(Character& character);

		virtual void Clear(unsigned short stage);
		virtual void Clear_All(void);

		virtual void Destroy(void);

	private:
		
		std::vector<Map_Structure> m_structure;
		unsigned short m_layers;
	};

};

