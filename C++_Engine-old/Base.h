#ifndef LIBRARY
	#define LIBRARY
#endif

#pragma once

#define BASE_H

#include <iostream>
#include <cstdlib>
#include <vector>
#include <fstream>
#include <string>
#include <iterator>
#include <map>
#include <cmath>

#include <sys/time.h>
#include <time.h>

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <SDL_mixer.h>

#define PI 3.1415

#define DIRECTIONS

enum ENUM_DIRECTION 
{
	DIRECTION_LEFT, DIRECTION_RIGHT, DIRECTION_TOP, DIRECTION_BOTTOM,
	DIRECTION_TOPLEFT, DIRECTION_TOPRIGHT, DIRECTION_BOTTOMLEFT, DIRECTION_BOTTOMRIGHT,
	DIRECTION_NOT_COLLIDED, DIRECTION_2 = 2, DIRECTION_4 = 4, DIRECTION_8 = 8  
};

#define MATH_EXTEND

int Point_Distance(int x1, int y1, int x2, int y2);
double Distance_Squared(int x1, int y1, int x2, int y2);

double Sin_Degree(double angle);
double Cos_Degree(double angle);

float Calc_Angle(float cur_x, float cur_y, float dst_x, float dst_y);
int Rand(int min, int max);

void Force_Exit();
void Force_Exit(std::string error);


struct _Color
{
	Uint8 r;  
	Uint8 g;  
	Uint8 b;

	explicit operator SDL_Color() 
	{
		static SDL_Color color;

		color = {r, g, b};
		return color;
	}
};
struct _Text
{
	const char* texts;

	SDL_Rect rect;
	_Color   color;
	int      space_margin;

	unsigned int Len() const
	{
		return sizeof(texts) + 1;
	}
};
struct _Rect
{
	int x;  int y;
	int w;  int h;
	SDL_Rect rect = {0, 0, 0, 0};

	SDL_Rect& Get_SDL_Rect() { rect = {x, y, w, h}; return rect; }

	// Returning a reference from referenced function apparently does't work. (return &Get_SDL_Rect(). )
	explicit operator SDL_Rect*() { rect = {x, y, w, h};  return &rect; }
	explicit operator SDL_FRect() { return SDL_FRect{ (float)x, (float)y, (float)w, (float)h }; };

	int Top() const { return y; }
	int Bottom() const { return y + h; }
	int Left() const { return x; }
	int Right() const { return x + w; }

	float Center_X() const { return (float)(x + (w / 2)); }
	float Center_Y() const { return (float)(y + (h / 2)); }

	// Return non negative if true, return direction of collision
	int Collided(_Rect obj, int colliding_dir);
}; 

struct _FRect
{
	float x;  float y;
	float w;  float h;
	SDL_FRect frect = {0.0f, 0.0f, 0.0f, 0.0f};

	SDL_FRect& Get_SDL_FRect() { frect = {x, y, w, h}; return frect; }

	// Returning a reference from referenced function apparently does't work. (return &Get_SDL_Rect(). )
	explicit operator SDL_FRect*() { frect = {x, y, w, h};  return &frect; }
	explicit operator SDL_Rect() { return SDL_Rect{ (int)x, (int)y, (int)w, (int)h }; };

	float Top() const { return y; }
	float Bottom() const { return y + h; }
	float Left() const { return x; }
	float Right() const { return x + w; }

	// Move the center of (*this rect) to the (center of provided rect).
	SDL_FRect& To_Center(_FRect _rect) { 
		return frect = {_rect.Center_X() - (w/2),  _rect.Center_Y() - (h/2), w, h};  
	}

	float Center_X() const { return (float)(x + (w / 2)); }
	float Center_Y() const { return (float)(y + (h / 2)); }

	// Return non negative if true, return direction of collision
	int Collided(_FRect obj, int colliding_dir);

}; 

struct _Circle
{
	// center x, y
	int x;  int y;
	int r;

	int TopLeft_X() const { return x - r; }
	int TopLeft_Y() const { return y - r; }

	int Diameter() const { return (r + r ); }

	bool Precise_Collided(_Circle obj);
	bool Collided(_Circle obj);
	bool Collided(_Rect obj);
	bool Collided(_FRect obj);
};


namespace Engine
{

	class Window 
	{
	public:
		Window(std::string t, unsigned int _tilesize, unsigned int w, unsigned int h, 
			Uint32 window_flags, Uint32 redererer_flags);
		~Window();

		void Test_Fps();
		void Show_Fps();

		void Set_Icon(const char *icon);
		void Destroy();

	protected:
		bool Init_SDL();

		void Create_Window(Uint32 flags);
		void Create_Renderer(Uint32 flags);

	public:
		// COLORS I have stealed from javidx9, ...apparently seeming convenient thing I haven't done before.
		static const _Color 
			GREY, 		DARK_GREY, 		VERY_DARK_GREY,
			RED, 		DARK_RED, 	 	VERY_DARK_RED,
			YELLOW, 	DARK_YELLOW, 	VERY_DARK_YELLOW ,
			GREEN,		DARK_GREEN,	 	VERY_DARK_GREEN,
			CYAN,		DARK_CYAN,	 	VERY_DARK_CYAN ,
			BLUE,		DARK_BLUE,	 	VERY_DARK_BLUE,
			MAGENTA,	DARK_MAGENTA,	VERY_DARK_MAGENTA ,
			BROWN, 		WHITE,			BLACK;


		SDL_Window* window;
		SDL_Renderer* renderer;

		const unsigned int width;
		const unsigned int height;
		const unsigned int tilesize;
		const std::string title;

		bool is_running;
		int fps;

	private:
		Uint32 m_last_counted_frames;
		int m_counted_frames;

	};

}



