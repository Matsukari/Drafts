#ifndef GAME_H
#define GAME_H
#define MUSIC_INFINITE_LOOP -1
#define MUSIC_NO_REPEAT 0

#include <Base.h>
#include <Graphics.h>

#include "../Character.h"
#include "../Effect.h"
#include "../Meteor.h"
#include "../Bullet.h"
#include "../Ship.h"

// Eror proni'es de programa.
class Game
{
public:
	Game();
	~Game();

public:
	void Run();

private:
	void Handle_Inputs();
	void Update();
	void Render();

	void Render_Menu_Screen();

	void Render_FPS();

	void Load_Window();
	void Load_Fonts();
	void Load_Textures();
	void Load_Audio();


	void Destroy();

private:
	SDL_Event 	   m_event;
	_Circle 	   m_mouse;
	const Uint8*   m_keypressed;

	bool 	m_is_running;
	bool  	m_is_playing;
	bool  	m_forced_move;

	int m_max_meteors;
	int m_max_enemy_ships;

private: // Startups Objects.
	Engine::Text_Type   m_font_message;

	Engine::Window*  	m_window;
	Engine::Texture* 	m_spritesheet_texture;
	Engine::Texture*    m_shield_texture;
	Engine::Texture*    m_boost_texture;
	Engine::Texture*    m_meteor_destroyed_texture;
	Engine::Texture 	m_circle_texture;

private:
	Mix_Music* 	m_bgm;
	Mix_Chunk* 	m_shoot_sound;
	Mix_Chunk*  m_shield_sound;
	Mix_Chunk* 	m_meteor_destroy_sound;


private: // Characters.
	std::vector<Meteor*>   m_meteors;
	std::vector<Ship*> 	   m_ships;

	Ship* 				   m_player_ship;
};


Game::Game()
{
	Load_Window();
	Load_Fonts();
	Load_Textures();
	Load_Audio();

	m_is_running  = m_window->is_running;
	m_is_playing  = false;
	m_forced_move = false;
	m_mouse		  = {0, 0, 5};

	m_max_meteors     = 10;
	m_max_enemy_ships =  3;

	Angled_Character::Set_Rendering_Screen(m_window->width, m_window->height);

	// Load Meteors
	for (int i = 0; i < m_max_meteors; i++)
	{
		m_meteors.push_back(new Meteor());
		m_meteors[i]->Spawn();
	}


	// Load Player
	int 	 player_level       = 1;
	_Speed   player_move_speed  = { .speed = 1.0, .delay = 20 };
	_Speed   player_shoot_speed = { .speed = 2.5, .delay = 120 };
	_FRect   player_rect        = { .x  = 300, .y = 500, .w = 40, .h = 40 };
	Bullet   player_bullet        { laserBlue01, 30, player_shoot_speed}; // sprite, damage, speed

	m_player_ship = new Ship(m_window->renderer, m_spritesheet_texture,
		playerShip2_orange, player_level, player_bullet, player_move_speed, player_shoot_speed);

	m_player_ship->Spawn(player_rect, 0.0, SDL_ALPHA_OPAQUE);

}
Game::~Game() // Destructor.
{
	if (Mix_PlayingMusic())
		Mix_HaltMusic();

	Mix_FreeMusic(m_bgm);

	delete m_spritesheet_texture;
	delete m_shield_texture;
	delete m_meteor_destroyed_texture;
	delete m_boost_texture;
	delete m_window;

	m_bgm = nullptr;

	m_spritesheet_texture 		= nullptr;
	m_shield_texture      		= nullptr;
	m_boost_texture       		= nullptr;
	m_meteor_destroyed_texture  = nullptr;
	m_window 			 		= nullptr;

	m_keypressed		 		= NULL;
}


void Game::Load_Window()
{
	std::ifstream window_file("Configures/Window_Configure.txt");
	std::string   name;  
	std::string   title; 
	int 		  width;
	int 		  height;

	window_file >> name >> title;
	window_file >> name >> width >> height;
	window_file.close();

	m_window = new Engine::Window(title, 40, width, height, SDL_WINDOW_SHOWN, SDL_RENDERER_ACCELERATED);
}
void Game::Load_Textures()
{

	m_spritesheet_texture	 	= new Engine::Texture(m_window->renderer, "Assets/Images/sheet.png");
	m_shield_texture    	    = new Engine::Texture(m_window->renderer, "Assets/Images/shield.png");
	m_meteor_destroyed_texture  = new Engine::Texture(m_window->renderer, "Assets/Images/meteor_destroy.png");
	m_boost_texture       		= new Engine::Texture(m_window->renderer, "Assets/Images/move_fire.png");
	m_circle_texture.Load(m_window->renderer, "Assets/Images/sample_circle.png");

	// Point the texture of player animation effects.
	Ship::  shield_animation.texture    	= m_shield_texture;
	Ship::  boost_animation.texture     	= m_boost_texture;
	Ship::  destroyed_animation.texture 	= m_spritesheet_texture;
	Meteor::destroyed_animation.texture     = m_meteor_destroyed_texture;
}
void Game::Load_Fonts()
{
	std::ifstream 			  text_placing_file("Configures/Text_Placing.txt");
	std::vector<std::string>  text_placing;
	std::string 			  text_line;
	int 					  letter_w;
	int 				      letter_h;
	bool 					  once = true;

 	// The resolution of each letter. First in file.
	text_placing_file >> letter_w >> letter_h;

	// Since blank( ) is ignored in text, be sure to add _ at the beginning.
	text_placing_file >> text_line;
						 text_line = " " + text_line;
						 text_placing.push_back(text_line);


	// Extract a full textline separated by newline(\n).
	while (text_placing_file >> text_line)
	{
		text_placing.push_back(text_line);
	}
	text_placing_file.close();


	m_font_message.Font(m_window->renderer, "Assets/Fonts/font_message.png");
	m_font_message.Config_Placement(text_placing, letter_w, letter_h);
}
void Game::Load_Audio()
{
	m_bgm 		  			= nullptr;
	m_shoot_sound 			= Mix_LoadWAV("Assets/Sounds/sfx_laser1.ogg");
	m_shield_sound 			= Mix_LoadWAV("Assets/Sounds/sfx_zap.ogg");
	m_meteor_destroy_sound  = Mix_LoadWAV("Assets/Sounds/sfx_lose.ogg");
}
void Game::Render_Menu_Screen()
{
	Handle_Inputs();
	
	// Updates
	SDL_RenderPresent(m_window->renderer);

	// Renders
	SDL_SetRenderDrawColor(m_window->renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
	SDL_RenderClear(m_window->renderer);
}
void Game::Render_FPS()
{
	const char*         fps_text           = (std::to_string(m_window->fps)).c_str(); 
	static SDL_Rect  	fps_rect           = {0, 0, 10, 10};
	static _Color    	fps_text_color     = {255, 255, 255};
	static int 	     	fps_space_margin   = 100;

	m_font_message.Render(m_window->renderer, _Text{fps_text, fps_rect, fps_text_color, fps_space_margin}, 255);
}

void Game::Handle_Inputs()
{
	m_keypressed = SDL_GetKeyboardState(NULL);

	while(SDL_PollEvent(&m_event)) {

		if (m_event.type == SDL_QUIT) 
		{
			m_is_running = false;
			m_is_playing = false;
		}
		// Proceeed to ingame screen
		if ((m_event.type == SDL_KEYDOWN && m_event.key.keysym.sym == SDLK_j)) 
		{
			m_is_playing = true;
		}
		else if ((m_event.type == SDL_KEYDOWN && m_event.key.keysym.sym == SDLK_SPACE) && !m_forced_move)
		{
			_Size    size  = { .w     = 8,    .h     = 15 };
			_Speed   speed = { .speed = 4.0, .delay  = 20 };

			m_player_ship->Shoot(size, speed, m_player_ship->angle);
			Mix_PlayChannel(-1, m_shoot_sound, MUSIC_NO_REPEAT);
		}
		else if (m_event.type == SDL_MOUSEBUTTONDOWN) 
		{
			SDL_GetMouseState(&m_mouse.x, &m_mouse.y);
			m_forced_move = true;

			m_player_ship->angle = Calc_Angle(m_player_ship->rect.Center_X(), m_player_ship->rect.Center_Y(), m_mouse.x,  m_mouse.y);
		}
	}
}
// Main Process ------------------------------------------------------------------------------------------------------------------
void Game::Update()
{
	SDL_RenderPresent(m_window->renderer);
	m_window->Test_Fps();

	// If user click mouse, then dont stop until it reach the last clicked mouse destination.
	// If not, get the angle of player_ship from the mouse.
	m_player_ship->boost_animation.texture->angle = m_player_ship->angle;
	if (m_forced_move)
	{
		m_player_ship->boost_animation.Play(m_player_ship->boost_status, 100);

		if (!m_player_ship->collider_circle.Collided(m_mouse)) 
		{
			_Speed              player_speed = { .speed = 3.0, .delay = 10 };
			m_player_ship->Move(player_speed);
		}
		else
			m_forced_move = false;
	}
	else 
	{
		SDL_GetMouseState(&m_mouse.x, &m_mouse.y);
		m_player_ship->angle = Calc_Angle(m_player_ship->rect.Center_X(), m_player_ship->rect.Center_Y(), m_mouse.x,  m_mouse.y);
	}


	// Interactions with meteors.
	for (int any_meteor = 0; any_meteor < m_max_meteors; any_meteor++)
	{
		// Check if player collided any with meteors.
		if (m_player_ship->collider_circle.Collided(m_meteors[any_meteor]->collider_circle)) 
		{
			m_player_ship->shield_animation.Play(m_player_ship->shield_status, 100);
			Mix_PlayChannel(-1, m_shield_sound, MUSIC_NO_REPEAT);
		}
		// Check if any bullets collided with any meteors.
		for (int any_bullet = 0; any_bullet < m_player_ship->bullets.size(); any_bullet++) 
		{
			if (m_player_ship->bullets[any_bullet].collider_circle.Collided(m_meteors[any_meteor]->collider_circle))
			{
				m_player_ship->bullets[any_bullet].has_hit = true;
				if (m_meteors[any_meteor]->health > 0) {
					m_meteors[any_meteor]->health -= m_player_ship->bullets[any_bullet].damage;
				}
				else {
					m_meteors[any_meteor]->is_dead = true;
					m_meteors[any_meteor]->health = 100;
					m_meteors[any_meteor]->Kill();
					Mix_PlayChannel(-1, m_meteor_destroy_sound, MUSIC_NO_REPEAT);
				}
			}
		}
		m_meteors[any_meteor]->Update();
	}
	m_player_ship->Update();


}
void Game::Render()
{
	// First draw - background. Dont draw brefore this one
	SDL_SetRenderDrawColor(m_window->renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
	SDL_RenderClear(m_window->renderer);
	Render_FPS();


	for (int any_meteor = 0; any_meteor < m_max_meteors; any_meteor++)
	{
		m_meteors[any_meteor]->Render(m_window->renderer, m_spritesheet_texture);
	}
	m_player_ship->Render();

}

void Game::Run()
{
	while(m_is_running)
	{
		// Will not proceed until player enters.
		Render_Menu_Screen();

		while(m_is_playing)
		{
			Handle_Inputs();
			Update();
			Render();
		}
	}
}


#endif


/*
#define TRANSITIONS
#define TRANSITION_FINISHED true

class Transitions
{
public:
	Transitions();
	~Transitions();

	bool Transition_Fade(Uint8& alpha, Uint8 last_alpha, int speed);
	bool Transition_Glow(Uint8& alpha, Uint8 last_alpha, int speed);

	bool Transition_Move();

protected:
	int last_transition;
};

bool Transitions::Transition_Fade(Uint8& alpha, Uint8 last_alpha, int speed)
{
	if (alpha >= last_alpha) {
		if (SDL_GetTicks() >= last_transition + speed) {
			alpha -= speed;		
		}
		return false;
	}

	return true;
}
bool Transitions::Transition_Glow(Uint8& alpha, Uint8 last_alpha, int speed)
{
	if (alpha >= last_alpha) {
		if (SDL_GetTicks() <= last_transition + speed) {
			alpha += speed;		
		}
		return false;
	}

	return true;
}

bool Transitions::Transition_Move()
{
	return false;
}

*/


































/*

	 Partial_Sprite  name = "beamLong1", { .x = 828, .y = 943, .w = 15, .h = 67
	 Partial_Sprite  name = "beamLong2", { .x = 307, .y = 309, .w = 25, .h = 64
	 Partial_Sprite  name = "bold_silver", { .x = 810, .y = 837, .w = 19, .h = 30
	 Partial_Sprite  name = "bolt_bronze", { .x = 810, .y = 467, .w = 19, .h = 30
	 Partial_Sprite  name = "bolt_gold", { .x = 809, .y = 437, .w = 19, .h = 30
	 Partial_Sprite  name = "buttonBlue", { .x = 0, .y = 78, .w = 222, .h = 39
	 Partial_Sprite  name = "buttonGreen", { .x = 0, .y = 117, .w = 222, .h = 39
	 Partial_Sprite  name = "buttonRed", { .x = 0, .y = 0, .w = 222, .h = 39
	 Partial_Sprite  name = "buttonYellow", { .x = 0, .y = 39, .w = 222, .h = 39

	 Partial_Sprite  name = "cursor", { .x = 797, .y = 173, .w = 30, .h = 33



	 Partial_Sprite  name = "numeral0", { .x = 367, .y = 644, .w = 19, .h = 19
	 Partial_Sprite  name = "numeral1", { .x = 205, .y = 688, .w = 19, .h = 19
	 Partial_Sprite  name = "numeral2", { .x = 406, .y = 290, .w = 19, .h = 19
	 Partial_Sprite  name = "numeral3", { .x = 580, .y = 707, .w = 19, .h = 19
	 Partial_Sprite  name = "numeral4", { .x = 386, .y = 644, .w = 19, .h = 19
	 Partial_Sprite  name = "numeral5", { .x = 628, .y = 646, .w = 19, .h = 19
	 Partial_Sprite  name = "numeral6", { .x = 671, .y = 1002, .w = 19, .h = 19
	 Partial_Sprite  name = "numeral7", { .x = 690, .y = 1004, .w = 19, .h = 19
	 Partial_Sprite  name = "numeral8", { .x = 709, .y = 1004, .w = 19, .h = 19
	 Partial_Sprite  name = "numeral9", { .x = 491, .y = 215, .w = 19, .h = 19
	 Partial_Sprite  name = "numeralX", { .x = 382, .y = 814, .w = 17, .h = 17
	 Partial_Sprite  name = "pill_blue", { .x = 674, .y = 262, .w = 22, .h = 21
	 Partial_Sprite  name = "pill_green", { .x = 573, .y = 989, .w = 22, .h = 21
	 Partial_Sprite  name = "pill_red", { .x = 222, .y = 108, .w = 22, .h = 21
	 Partial_Sprite  name = "pill_yellow", { .x = 222, .y = 129, .w = 22, .h = 21
	 Partial_Sprite  name = "playerLife1_blue", { .x = 482, .y = 358, .w = 33, .h = 26
	 Partial_Sprite  name = "playerLife1_green", { .x = 535, .y = 150, .w = 33, .h = 26
	 Partial_Sprite  name = "playerLife1_orange", { .x = 777, .y = 327, .w = 33, .h = 26
	 Partial_Sprite  name = "playerLife1_red", { .x = 775, .y = 301, .w = 33, .h = 26
	 Partial_Sprite  name = "playerLife2_blue", { .x = 465, .y = 991, .w = 37, .h = 26
	 Partial_Sprite  name = "playerLife2_green", { .x = 391, .y = 991, .w = 37, .h = 26
	 Partial_Sprite  name = "playerLife2_orange", { .x = 428, .y = 991, .w = 37, .h = 26
	 Partial_Sprite  name = "playerLife2_red", { .x = 502, .y = 991, .w = 37, .h = 26
	 Partial_Sprite  name = "playerLife3_blue", { .x = 777, .y = 385, .w = 32, .h = 26
	 Partial_Sprite  name = "playerLife3_green", { .x = 778, .y = 469, .w = 32, .h = 26
	 Partial_Sprite  name = "playerLife3_orange", { .x = 777, .y = 712, .w = 32, .h = 26
	 Partial_Sprite  name = "playerLife3_red", { .x = 777, .y = 443, .w = 32, .h = 26
	 Partial_Sprite  name = "playerShip1_blue", { .x = 211, .y = 941, .w = 99, .h = 75
	 Partial_Sprite  name = "playerShip1_damage1", { .x = 112, .y = 941, .w = 99, .h = 75
	 Partial_Sprite  name = "playerShip1_damage2", { .x = 247, .y = 234, .w = 99, .h = 75
	 Partial_Sprite  name = "playerShip1_damage3", { .x = 247, .y = 159, .w = 99, .h = 75


	 Partial_Sprite  name = "playerShip2_damage1", { .x = 0, .y = 866, .w = 112, .h = 75
	 Partial_Sprite  name = "playerShip2_damage2", { .x = 0, .y = 791, .w = 112, .h = 75
	 Partial_Sprite  name = "playerShip2_damage3", { .x = 0, .y = 716, .w = 112, .h = 75

	 Partial_Sprite  name = "playerShip3_blue", { .x = 325, .y = 739, .w = 98, .h = 75
	 Partial_Sprite  name = "playerShip3_damage1", { .x = 323, .y = 832, .w = 98, .h = 75
	 Partial_Sprite  name = "playerShip3_damage2", { .x = 310, .y = 907, .w = 98, .h = 75
	 Partial_Sprite  name = "playerShip3_damage3", { .x = 325, .y = 664, .w = 98, .h = 75
	 Partial_Sprite  name = "playerShip3_green", { .x = 346, .y = 75, .w = 98, .h = 75
	 Partial_Sprite  name = "playerShip3_orange", { .x = 336, .y = 309, .w = 98, .h = 75
	 Partial_Sprite  name = "playerShip3_red", { .x = 325, .y = 0, .w = 98, .h = 75
	 Partial_Sprite  name = "powerupBlue", { .x = 696, .y = 329, .w = 34, .h = 33
	 Partial_Sprite  name = "powerupBlue_bolt", { .x = 539, .y = 989, .w = 34, .h = 33
	 Partial_Sprite  name = "powerupBlue_shield", { .x = 777, .y = 679, .w = 34, .h = 33
	 Partial_Sprite  name = "powerupBlue_star", { .x = 776, .y = 895, .w = 34, .h = 33
	 Partial_Sprite  name = "powerupGreen", { .x = 774, .y = 613, .w = 34, .h = 33
	 Partial_Sprite  name = "powerupGreen_bolt", { .x = 766, .y = 80, .w = 34, .h = 33
	 Partial_Sprite  name = "powerupGreen_shield", { .x = 776, .y = 862, .w = 34, .h = 33
	 Partial_Sprite  name = "powerupGreen_star", { .x = 651, .y = 490, .w = 34, .h = 33
	 Partial_Sprite  name = "powerupRed", { .x = 491, .y = 182, .w = 34, .h = 33
	 Partial_Sprite  name = "powerupRed_bolt", { .x = 775, .y = 646, .w = 34, .h = 33
	 Partial_Sprite  name = "powerupRed_shield", { .x = 776, .y = 928, .w = 34, .h = 33
	 Partial_Sprite  name = "powerupRed_star", { .x = 774, .y = 977, .w = 34, .h = 33
	 Partial_Sprite  name = "powerupYellow", { .x = 774, .y = 761, .w = 34, .h = 33
	 Partial_Sprite  name = "powerupYellow_bolt", { .x = 740, .y = 761, .w = 34, .h = 33
	 Partial_Sprite  name = "powerupYellow_shield", { .x = 482, .y = 325, .w = 34, .h = 33
	 Partial_Sprite  name = "powerupYellow_star", { .x = 607, .y = 857, .w = 34, .h = 33
	 Partial_Sprite  name = "scratch1", { .x = 325, .y = 814, .w = 21, .h = 16
	 Partial_Sprite  name = "scratch2", { .x = 423, .y = 41, .w = 21, .h = 16
	 Partial_Sprite  name = "scratch3", { .x = 346, .y = 295, .w = 16, .h = 12
	 Partial_Sprite  name = "shield_bronze", { .x = 797, .y = 143, .w = 30, .h = 30
	 Partial_Sprite  name = "shield_gold", { .x = 797, .y = 113, .w = 30, .h = 30
	 Partial_Sprite  name = "shield_silver", { .x = 778, .y = 824, .w = 30, .h = 30
	 Partial_Sprite  name = "speed", { .x = 858, .y = 284, .w = 7, .h = 108
	 Partial_Sprite  name = "star1", { .x = 628, .y = 681, .w = 25, .h = 24
	 Partial_Sprite  name = "star2", { .x = 222, .y = 84, .w = 25, .h = 24
	 Partial_Sprite  name = "star3", { .x = 576, .y = 300, .w = 24, .h = 24
	 Partial_Sprite  name = "star_bronze", { .x = 778, .y = 794, .w = 31, .h = 30
	 Partial_Sprite  name = "star_gold", { .x = 778, .y = 557, .w = 31, .h = 30
	 Partial_Sprite  name = "star_silver", { .x = 778, .y = 527, .w = 31, .h = 30
	 Partial_Sprite  name = "things_bronze", { .x = 778, .y = 495, .w = 32, .h = 32
	 Partial_Sprite  name = "things_gold", { .x = 777, .y = 411, .w = 32, .h = 32
	 Partial_Sprite  name = "things_silver", { .x = 777, .y = 353, .w = 32, .h = 32
	 Partial_Sprite  name = "turretBase_big", { .x = 310, .y = 982, .w = 41, .h = 41
	 Partial_Sprite  name = "turretBase_small", { .x = 808, .y = 301, .w = 26, .h = 26

	 Partial_Sprite  name = "wingBlue_0", { .x = 647, .y = 924, .w = 45, .h = 78
	 Partial_Sprite  name = "wingBlue_1", { .x = 738, .y = 253, .w = 37, .h = 72
	 Partial_Sprite  name = "wingBlue_2", { .x = 805, .y = 0, .w = 26, .h = 84
	 Partial_Sprite  name = "wingBlue_3", { .x = 600, .y = 450, .w = 51, .h = 75
	 Partial_Sprite  name = "wingBlue_4", { .x = 692, .y = 924, .w = 42, .h = 80
	 Partial_Sprite  name = "wingBlue_5", { .x = 596, .y = 892, .w = 51, .h = 69
	 Partial_Sprite  name = "wingBlue_6", { .x = 694, .y = 847, .w = 42, .h = 74
	 Partial_Sprite  name = "wingBlue_7", { .x = 675, .y = 134, .w = 43, .h = 83
	 Partial_Sprite  name = "wingGreen_0", { .x = 650, .y = 525, .w = 45, .h = 78
	 Partial_Sprite  name = "wingGreen_1", { .x = 775, .y = 229, .w = 37, .h = 72
	 Partial_Sprite  name = "wingGreen_2", { .x = 809, .y = 527, .w = 26, .h = 84
	 Partial_Sprite  name = "wingGreen_3", { .x = 535, .y = 0, .w = 51, .h = 75
	 Partial_Sprite  name = "wingGreen_4", { .x = 694, .y = 431, .w = 42, .h = 80
	 Partial_Sprite  name = "wingGreen_5", { .x = 525, .y = 251, .w = 51, .h = 69
	 Partial_Sprite  name = "wingGreen_6", { .x = 695, .y = 511, .w = 42, .h = 74
	 Partial_Sprite  name = "wingGreen_7", { .x = 655, .y = 764, .w = 43, .h = 83
	 Partial_Sprite  name = "wingRed_0", { .x = 809, .y = 712, .w = 26, .h = 84
	 Partial_Sprite  name = "wingRed_1", { .x = 768, .y = 0, .w = 37, .h = 72
	 Partial_Sprite  name = "wingRed_2", { .x = 600, .y = 300, .w = 51, .h = 75
	 Partial_Sprite  name = "wingRed_3", { .x = 698, .y = 715, .w = 42, .h = 80
	 Partial_Sprite  name = "wingRed_4", { .x = 586, .y = 75, .w = 51, .h = 69
	 Partial_Sprite  name = "wingRed_5", { .x = 718, .y = 123, .w = 42, .h = 74
	 Partial_Sprite  name = "wingRed_6", { .x = 653, .y = 681, .w = 43, .h = 83
	 Partial_Sprite  name = "wingRed_7", { .x = 651, .y = 286, .w = 45, .h = 78
	 Partial_Sprite  name = "wingYellow_0", { .x = 650, .y = 603, .w = 45, .h = 78
	 Partial_Sprite  name = "wingYellow_1", { .x = 760, .y = 120, .w = 37, .h = 72
	 Partial_Sprite  name = "wingYellow_2", { .x = 809, .y = 353, .w = 26, .h = 84
	 Partial_Sprite  name = "wingYellow_3", { .x = 576, .y = 150, .w = 51, .h = 75
	 Partial_Sprite  name = "wingYellow_4", { .x = 726, .y = 0, .w = 42, .h = 80
	 Partial_Sprite  name = "wingYellow_5", { .x = 525, .y = 182, .w = 51, .h = 69
	 Partial_Sprite  name = "wingYellow_6", { .x = 695, .y = 585, .w = 42, .h = 74
	 Partial_Sprite  name = "wingYellow_7", { .x = 651, .y = 364, .w = 43, .h = 83
*/

