#include "Map.h"


namespace Engine 
{

	Map_Structure::Map_Structure(void)
	{
	}
	Map_Structure::~Map_Structure(void)
	{
	}

	// Only once
	void Map_Structure::Set_TextureMap(std::vector<std::string>& map) 
	{
		m_texture_map.clear();
		m_texture_map = map;
	}
	void Map_Structure::Set_TextMap(std::vector<std::string>& map) 
	{
		m_text_map.clear();
		m_text_map = map;
	}


	void Map_Structure::Clear(void) 
	{
		m_text_map.clear();
		m_texture_map.clear();
		m_build_code.clear();
		m_build_type.clear();
		m_build_text.clear();

		this->Destroy_Texture();

		m_build_texture.clear();
		m_text_rect.clear();
		m_texture_rect.clear();
		m_text_color.clear();
		m_text_alpha.clear();
	}

	void Map_Structure::Destroy_Texture(void) 
	{
		for (unsigned int i = 0; i < m_build_texture.size(); i++){
			m_build_texture[i].Destroy();
		}
	}


	// Add new build in map: String case : 
	void Map_Structure::Build_Text(char build_code, std::string text, SDL_Color color, SDL_Rect rect, Uint8 alpha)
	{
		m_build_code.push_back(build_code);
		m_build_text.push_back(text);

		m_text_alpha.push_back(alpha);
		m_text_color.push_back(color);
		m_build_type.push_back('S');

		m_text_rect.push_back(rect);
	}
	void Map_Structure::Build_Text(char build_code, std::string text, SDL_Color color, unsigned int w, unsigned int h, Uint8 alpha)
	{
		m_build_code.push_back(build_code);
		m_build_text.push_back(text);

		m_text_alpha.push_back(alpha);
		m_text_color.push_back(color);
		m_build_type.push_back('S');

		SDL_Rect rect = {0 ,0, w, h};
		m_text_rect.push_back(rect);
	}


	void Map_Structure::Build_Texture(char build_code, std::string path, unsigned int w, unsigned int h, Uint8 alpha, SDL_Color* col) 
	{
		m_build_code.push_back(build_code);

		Texture texture;
		texture.Load(path);
		texture.Opacity(alpha);
		if (col != NULL)
			texture.Color(col);


		m_build_texture.push_back(texture);
		m_build_type.push_back('T');

		SDL_Rect rect = {0, 0, w, h};
		std::vector<SDL_Rect> vec = {rect};
		m_texture_rect.push_back(vec);
	}

	void Map_Structure::Build_Texture(char build_code, std::string path, SDL_Rect rect, Uint8 alpha, SDL_Color* col) 
	{
		m_build_code.push_back(build_code);

		Texture texture;
		texture.Load(path);
		texture.Opacity(alpha);
		if (col != NULL)
			texture.Color(col);

		m_build_texture.push_back(texture);
		m_build_type.push_back('T');

		std::vector<SDL_Rect> vec = {rect};
		m_texture_rect.push_back(vec);
	}


	int Map_Structure::Last_Texture(void)
	{
		return (m_build_texture.size() - 1);
	}



	// Sets The Corresponding X and Y Axis for all SDL_Rect
	void Map_Structure::Build(unsigned int x_margin, unsigned int y_margin) 
	{
		// Carries 
		int x = 0;
		int y = 0;
		for (int row = 0; row < m_text_map.size(); row++){
			for (int individual = 0; individual < m_text_map[row].size(); individual++){
	
				for (int i = 0; i < m_build_code.size(); i++){
					if (m_text_map[row][individual] == m_build_code[i] && m_build_type[i] == 'S'){
						m_text_rect[i].x = x;
						m_text_rect[i].y = y;
					}
				}
				x += x_margin;
			}
			x = 0;
			y += y_margin;
		}

		x = 0;
		y = 0;

		for (int row = 0; row < m_texture_map.size(); row++){
			for (int individual = 0; individual < m_texture_map[row].size(); individual++){
	
				for (int i = 0; i < m_build_code.size(); i++){
					if (m_texture_map[row][individual] == m_build_code[i] && m_build_type[i] == 'T') {
						// Index 0 got the right w/h value;
						SDL_Rect rect = {x, y, m_texture_rect[i][0].w, m_texture_rect[i][0].h};

						m_texture_rect[i].push_back(rect);
						
					}
				}

				x += x_margin;
			}
			x = 0;
			y += y_margin;

		}

		// Clean Empty X n Y Shits
		for (unsigned int i = 0; i < m_texture_rect.size(); i++){
			m_texture_rect[i].erase(m_texture_rect[i].begin());
		}


	}

	void Map_Structure::Render_Text(TTF_Font* font) 
	{
		for (unsigned int i = 0; i < m_build_text.size(); i++){
			Draw_Text(font, m_build_text[i], m_text_color[i], m_text_rect[i], m_text_alpha[i]);
		}
	}
	void Map_Structure::Render_Texture(void) 
	{
		for (unsigned int i = 0; i < m_build_texture.size(); i++){
			for (unsigned int j = 0; j < m_texture_rect[i].size(); j++){
				m_build_texture[i].Render(&m_texture_rect[i][j]);
			}
		}
	}
	void Map_Structure::Camera(Character& target, unsigned int min, unsigned int max) 
	{
		if (target.rect.x > max || target.rect.x < min)
			target.rect.x -= target.speedx * 2;

		// Adjust Camera only if the target's motion speed is not ZERO
		// and if the target's X is greater than first Rendered Texture's X and camera w and Target's X is lower than Last Texture's X+W
		if (target.speedx != 0) {
			for (unsigned int i = 0; i < m_texture_rect.size(); i++){
				for (unsigned int j = 0; j < m_texture_rect[i].size(); j++){
					m_texture_rect[i][j].x -= target.speedx;
				}
			}
		}
	}

	void Map_Structure::CameraPos_Centered(Character& entity, bool animate, int speed)
	{
		int timeleft = abs(entity.rect.x);
		speed = (entity.rect.x > 0) ? 3 : -3;

		for (unsigned int i = 0; i < m_texture_rect.size(); i++){
			for (unsigned int j = 0; j < m_texture_rect[i].size(); i++){
				if (animate && timeleft > 0){
					m_texture_rect[i][j].x -= speed;
					timeleft -= speed;
				}
				else if (!animate){
					m_texture_rect[i][j].x -= (entity.rect.x);
				}
			}
		}
	}

	int Map_Structure::Collide_GetPosition(char build_code, Character& obj, int dir) {
		int collideside = -1;

		for (unsigned int i = 0; i < m_build_code.size(); i++){
			if (m_build_code[i] == build_code){
				for (unsigned int j = 0; j < m_texture_rect[i].size(); j++){
					collideside = Check_Collision_Get_Position(obj.rect, m_texture_rect[i][j], dir);
				}
			}
		}

		return collideside;
	}





	void Map::Init(void)
	{
		m_layers = 0;
	}
	void Map::Create_Layer(unsigned short layers) 
	{
		m_structure.resize(layers);
	}
	void Map::Create(void) 
	{
		return;
	}


	void Map::Render(unsigned short layer) 
	{
		m_structure[layer].Render_Texture();
	}
	void Map::Render_All(void) {
		for (unsigned short i = 0; i < m_structure.size(); i++)
			m_structure[i].Render_Texture();
	}


	void Map::Update(Character& character) {
		return;
	}


	void Map::Camera(Character& character, unsigned short layer) 
	{
		m_structure[layer].Camera(character);
	}
	void Map::Camera_All(Character& character) {
		for (unsigned short i = 0; i < m_structure.size(); i++)
		{
			m_structure[i].Camera(character);
		}
	}


	void Map::Events(void) 
	{
		return;
	}

	void Map::Clear(unsigned short layer) 
	{ 
		m_structure[layer].Clear();
	}

	void Map::Clear_All(void) 
	{ 
		for (unsigned short i = 0; i < m_structure.size(); i++){
			m_structure[i].Clear();
		}
	}


	void Map::Destroy(void) 
	{
		for (unsigned short i = 0; i < m_structure.size(); i++){
			m_structure[i].Destroy_Texture();
		}
	}

};