#include "Base.h"

int Point_Distance(int x1, int y1, int x2, int y2)
{
     return sqrt(  ((x1 - x2)*(x1 - x2)) + ((y1 - y2)*(y1 - y2))  );
}
double Distance_Squared(int x1, int y1, int x2, int y2)
{
	int delta_x = x2 - x1;
	int delta_y = y2 - y1;

	return (delta_x * delta_x) + (delta_y * delta_y);
}

double Sin_Degree(double angle)
{
	return sin((angle * PI) / 180);
}
double Cos_Degree(double angle)
{
	return cos((angle * PI) / 180);
}

float Calc_Angle(float cur_x, float cur_y, float dst_x, float dst_y)
{
	return ((atan2(dst_y - cur_y, dst_x - cur_x)) * 180 / PI) + 90;
}

int Rand(int min, int max)
{
	timeval t;

	SDL_Delay(1);

	gettimeofday(&t, NULL);
	srand(t.tv_usec * t.tv_sec);
	
	// if negative
	if (min < 0) {max *= 2;}

	return (rand() % max) + min;
}


void Force_Exit()
{
	exit(EXIT_FAILURE);
	getchar();
}
void Force_Exit(std::string error)
{
	// Since there is only one error to display. Later, should create error log which contain all error.
	std::fstream file{"Data/Error_Catched.txt", std::ios::out | std::ios::trunc};

	if(file.is_open()) {
		file << error;
	}
	else
		printf("Error_Catched_file not opened.");

	file.close();

	Force_Exit();
}


// Return non negative if true, return direction of collision
int _Rect::Collided(_Rect obj, int colliding_dir)
{
	// Well, just be careful not make the velocity greater than the width of object B cause
	// that will make the object A leap through object B and pass trigger the collision.
	switch(colliding_dir)
	{
		case DIRECTION_RIGHT:
			if ( (Right() >= obj.Left()	 &&  Right() <= obj.Right())  &&
				 (Bottom() >= obj.Top()  &&  Top() <= obj.Bottom()) )  { return DIRECTION_RIGHT; }

			break;
		case DIRECTION_LEFT:
			if (
				(Left() >= obj.Right() 	 &&  Left() <= obj.Right())  && 
				(Bottom() >= obj.Top()   &&  Top() <= obj.Bottom()) )  { return DIRECTION_LEFT; }

			break;
		case DIRECTION_TOP:
			if (
				(Right() >= obj.Left()   && Left() <= obj.Right())  &&
			 	(Top() >= obj.Bottom() 	 && Top() <= obj.Bottom()) )  {	return DIRECTION_TOP; }
			
			break;
		case DIRECTION_BOTTOM:
			if( (Right() >= obj.Left()	 && Left() <= obj.Bottom())  &&
			    (Bottom() >= obj.Top() 	 && Bottom() <= obj.Bottom() ))  { return DIRECTION_BOTTOM; }
			
			break;
		}

	return DIRECTION_NOT_COLLIDED;
}

// Return non negative if true, return direction of collision
int _FRect::Collided(_FRect obj, int colliding_dir)
{
	// Well, just be careful not make the velocity greater than the width of object B cause
	// that will make the object A leap through object B and pass trigger the collision.
	switch(colliding_dir)
	{
		case DIRECTION_RIGHT:
			if ( (Right() >= obj.Left()	 &&  Right() <= obj.Right())  &&
				 (Bottom() >= obj.Top()  &&  Top() <= obj.Bottom()) )  { return DIRECTION_RIGHT; }

			break;
		case DIRECTION_LEFT:
			if (
				(Left() >= obj.Right() 	 &&  Left() <= obj.Right())  && 
				(Bottom() >= obj.Top()   &&  Top() <= obj.Bottom()) )  { return DIRECTION_LEFT; }

			break;
		case DIRECTION_TOP:
			if (
				(Right() >= obj.Left()   && Left() <= obj.Right())  &&
			 	(Top() >= obj.Bottom() 	 && Top() <= obj.Bottom()) )  {	return DIRECTION_TOP; }
			
			break;
		case DIRECTION_BOTTOM:
			if( (Right() >= obj.Left()	 && Left() <= obj.Bottom())  &&
			    (Bottom() >= obj.Top() 	 && Bottom() <= obj.Bottom() ))  { return DIRECTION_BOTTOM; }
			
			break;
		}

	return DIRECTION_NOT_COLLIDED;
}


bool _Circle::Precise_Collided(_Circle obj)
{
	if(Point_Distance(x, y, obj.x, obj.y) < r + obj.r)
		return true;

	return false;
}
bool _Circle::Collided(_Circle obj)
{
	if (Distance_Squared(x, y, obj.x, obj.y) < (r + obj.r) * (r + obj.r))
		return true;

	return false;
}
bool _Circle::Collided(_Rect obj)
{
	int c_x = 0, c_y = 0;

	if (x < obj.Left()) {
		c_x = obj.Left();
	}
	else if (x > obj.Right()) {
		c_x = obj.Right();
	}
	else
		c_x = x; // in center;

	if (y < obj.Top()) {
		c_y = obj.Top();
	}
	else if (y > obj.Bottom()) {
		c_y = obj.Bottom();
	}
	else
		c_y = y; // in center;

	if (Distance_Squared(x, y, c_x, c_y)  <  (r * r))
		return true;

	return false;
}

bool _Circle::Collided(_FRect obj)
{
	int c_x = 0, c_y = 0;

	if (x < obj.Left()) {
		c_x = obj.Left();
	}
	else if (x > obj.Right()) {
		c_x = obj.Right();
	}
	else
		c_x = x; // in center;

	if (y < obj.Top()) {
		c_y = obj.Top();
	}
	else if (y > obj.Bottom()) {
		c_y = obj.Bottom();
	}
	else
		c_y = y; // in center;

	if (Distance_Squared(x, y, c_x, c_y)  <  (r * r))
		return true;

	return false;
}




namespace Engine
{
	const _Color 
		Window::GREY = {192, 192, 192}, 	Window::DARK_GREY = {128, 128, 128}, 	Window::VERY_DARK_GREY = {64, 64, 64},
		Window::RED = {255, 0, 0}, 			Window::DARK_RED = {128, 0, 0}, 	 	Window::VERY_DARK_RED = {64, 0, 0},
		Window::YELLOW = {255, 255, 0}, 	Window::DARK_YELLOW = {128, 128, 0}, 	Window::VERY_DARK_YELLOW = {64, 64, 0},
		Window::GREEN = {0, 255, 0},		Window::DARK_GREEN = {0, 128, 0},	 	Window::VERY_DARK_GREEN = {0, 64, 0},
		Window::CYAN = {0, 255, 255},		Window::DARK_CYAN = {0, 128, 128},		Window::VERY_DARK_CYAN = {0, 64, 64},
		Window::BLUE = {0, 0, 255},			Window::DARK_BLUE = {0, 0, 128},	 	Window::VERY_DARK_BLUE = {0, 0, 64},
		Window::MAGENTA = {255, 0, 255},	Window::DARK_MAGENTA = {128, 0, 128},	Window::VERY_DARK_MAGENTA = {64, 0, 64},
		Window::BROWN = {205, 133, 63}, 	Window::WHITE = {255, 255, 255},		Window::BLACK = {0, 0, 0};


	Window::Window(std::string t, unsigned int _tilesize, unsigned int w, unsigned int h, 
		Uint32 window_flags, Uint32 renderer_flags) : title(t), width(w), height(h), tilesize(_tilesize)
	{
		fps 	   = 0;
		is_running = false;

		m_last_counted_frames = 0;
		m_counted_frames      = 0;

		if (Init_SDL()) {
			Create_Window(window_flags);
			Create_Renderer(renderer_flags);

			is_running = true;
		}
		else {
			is_running = false;

			if (!is_running)
				Force_Exit({"SDL Failed to initialize"});
		}
	}

	// Initialize it once. This is common sense, initialize it twice or three and let's see what will happen.
	bool Window::Init_SDL()
	{
		if (SDL_Init(SDL_INIT_EVERYTHING) < 0) {
			printf("\nSDL failed to Initialize. SDL_Error: %s.", SDL_GetError());

			return false;
		}

		int img_flag = IMG_INIT_PNG | IMG_INIT_JPG;
		if (!IMG_Init(img_flag) & img_flag) {
			printf("\nSDL_Image Failed to initialize. IMG_Error: %s.", IMG_GetError());
			
			return false;
		}

		if (TTF_Init() == -1) {
			printf("\nSDL_ttf failed to initialize. TTF_Error: %s.", TTF_GetError());

			return false;
		}
		if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 4096) < 0) {
			printf("\nSDL_Mixer Failed to initalize. Mix_Error: %s.", Mix_GetError());
			
			return false;
		}
		

		return true;
	}

	Window::~Window() 
	{
		Destroy();
		printf("\nDestroyed Game.");
	}

	void Window::Create_Window(Uint32 flags) 
	{
		window = SDL_CreateWindow(title.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, flags);
	}
	void Window::Create_Renderer(Uint32 flags)  
	{
		if (window != NULL) {
			renderer = SDL_CreateRenderer(window, -1, flags);
		}
		else {
			printf("\nCreate Window First.");
		}
	}
	void Window::Set_Icon(const char *icon)
	{
		SDL_Surface *surface = IMG_Load(icon);

		if (surface != NULL) {
			SDL_SetWindowIcon(window, surface);
		}
		else
			printf("\nLoaded Surface is NULL.");

		SDL_FreeSurface(surface);
	}

	void Window::Test_Fps()
	{
		// Count how many time the game looped within a second(1000 ms).
		if (SDL_GetTicks() > m_last_counted_frames + 1000) {
			m_last_counted_frames = SDL_GetTicks();

			fps = m_counted_frames;
			m_counted_frames = 0;
		}

		m_counted_frames++;
	}
	void Window::Show_Fps()
	{
		Test_Fps();
		printf("\nFps (%i)", fps);
	}

	// Call it only once. 
	void Window::Destroy()
	{
		SDL_DestroyRenderer(renderer);
		SDL_DestroyWindow(window);

		renderer = nullptr;
		window = nullptr;

		SDL_Quit();
		TTF_Quit();
		IMG_Quit();
		Mix_Quit();
	}
}

