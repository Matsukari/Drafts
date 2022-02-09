#ifndef METEOR_H
#define METEOR_H

enum METEOR_TYPES 
{
	meteorBrown_big1, 	meteorBrown_big2,  meteorBrown_big3,   meteorBrown_big4,
	meteorBrown_med1, 	meteorBrown_med3,  meteorBrown_small1, meteorBrown_small2,
	meteorBrown_tiny1, 	meteorBrown_tiny2,

	meteorGrey_big1,  meteorGrey_big2,	 meteorGrey_big3, 	meteorGrey_big4,
	meteorGrey_med1,  meteorGrey_med2, 	 meteorGrey_small1, meteorGrey_small2,
	meteorGrey_tiny1, meteorGrey_tiny2,  TOTAL_METEOR_TYPES
};

enum METEOR_ATTRIBUTES
{
	AT_SIZE, 		  AT_TYPE, 			 AT_HEALTH, 
	AT_VELX, 		  AT_VELY,			 AT_ROTATE_SPEED,  
	AT_ROTATE_DELAY,  AT_MOVE_DELAY,	 AT_TOTAL_SPRITES
};


struct Int_Range
{
	int min;
	int max;
};


// Closed off member variables. Access through file.
class Meteor : public Angled_Character
{
public:
	Meteor();
	~Meteor();

	virtual void Spawn();
	virtual void Update();
	virtual void Render(SDL_Renderer* renderer, Engine::Texture* texture);

	bool	is_dead;
	bool 	once   = true;
	int 	id;

protected:
	virtual void Move();
	void 	  	 Rotate();

private:
	int 	m_type;
	_Speed  m_rotate_speed;

	static std::vector<Int_Range> attr; 			// included rot speed, move_speed etc.
	static std::vector<SDL_Rect>  sprite_clips; 	// rendering clips.

public:
	Effect_Status   destroyed_status;
	static Effect  	destroyed_animation;
};


Meteor::Meteor() : Angled_Character()
{
	static int instanciated_times = 0;
			   instanciated_times++;

	id 			   = instanciated_times;
	// class member init
	m_type 		   = 0;
	is_dead        = false;
	m_rotate_speed = { 0.0, 0};

	static bool has_loaded_data = false;
	if (!has_loaded_data)
	{
		std::ifstream  infile("Configures/Meteor_Configure.txt");
		std::string    _name;
		int     	   _min;
		int 		   _max;
		int 		   _number;
		SDL_Rect 	   _rect;

		// Attributes
		for (int i = 0; i < 8; i++) 
		{
			infile >> _name >> _min >> _max;

			attr.push_back(Int_Range{_min, _max});
		} 

 		// total sprites
		infile >> _name >> _number;

		// some naming text.
		infile >> _name;

		// sprite clips
		while (infile >> _name)
		{
			infile >> _rect.x;
			infile >> _name >> _rect.y;
			infile >> _name >> _rect.w;
			infile >> _name >> _rect.h;

			sprite_clips.push_back(_rect);
		}
		infile.close();

		has_loaded_data = true;
	}
}
Meteor::~Meteor() // Destructor.
{
}

void Meteor::Spawn()
{
	int _size = Rand(attr[AT_SIZE].min, attr[AT_SIZE].max);

	// Inherited varibales.
	rect = { (float)Rand(0, rendering_screen_w), -40.0f, (float)_size, (float)_size };
	velx = 	 (float)Rand(attr[AT_VELX].min, attr[AT_VELX].max);
	vely = 	 (float)Rand(attr[AT_VELY].min, attr[AT_VELY].max);

	move_speed.delay = Rand(attr[AT_MOVE_DELAY].min, attr[AT_MOVE_DELAY].max);

	// Member variables.
	m_type = Rand(attr[AT_TYPE].min, attr[AT_TYPE].max);
	health = Rand(attr[AT_HEALTH].min, attr[AT_HEALTH].max);
	
	m_rotate_speed.speed = Rand(attr[AT_ROTATE_SPEED].min, attr[AT_ROTATE_SPEED].max);
	m_rotate_speed.delay = Rand(attr[AT_ROTATE_DELAY].min, attr[AT_ROTATE_DELAY].max);

	printf("\nType (%i): Health:%i,  %.f %.f %.f %.f  | %.f | %.f |", m_type, health, rect.x, rect.y, rect.w, rect.h,  velx, m_rotate_speed.speed);
}
void Meteor::Update()
{
	Move();
	Rotate();

	// if the meteor's image is not within rendering sight. Respawn
	if (!is_dead && (rect.Right() < 0 || rect.Left() > rendering_screen_w || rect.Top() > rendering_screen_h)) {
		Kill();
		Spawn();
	}

	destroyed_animation.Update(destroyed_status);
}
void Meteor::Render(SDL_Renderer* renderer, Engine::Texture* texture)
{
	SDL_RenderCopyExF(renderer, texture->Get_SDL_Texture(), &sprite_clips[m_type], 
		((SDL_FRect*)rect), angle, NULL, SDL_FLIP_NONE);

	_FRect destroy_rect = rect;
	destroy_rect.w *= 2;
	destroy_rect.h *= 2;

	destroyed_animation.Render(renderer, destroyed_status, rect);
}

void Meteor::Rotate()
{
	if (SDL_GetTicks() > m_rotate_speed.last_update + m_rotate_speed.delay) {
		m_rotate_speed.last_update = SDL_GetTicks();

		angle += m_rotate_speed.speed;
	}
}
void Meteor::Move()
{
	if (SDL_GetTicks() > move_speed.last_update + move_speed.delay) {
		move_speed.last_update = SDL_GetTicks();

		rect.x += velx;
		rect.y += vely;

		Shift_Collider_Circle();
	}
}
std::vector<Int_Range> Meteor::attr 			   = {};
std::vector<SDL_Rect>  Meteor::sprite_clips		   = {};
Effect 				   Meteor::destroyed_animation { nullptr,
	{
		
		{ .x = 101*0, .y = 0, .w = 101, .h = 84 },
		{ .x = 101*1, .y = 0, .w = 101, .h = 84 },
		{ .x = 101*2, .y = 0, .w = 101, .h = 84 },
		{ .x = 101*3, .y = 0, .w = 101, .h = 84 },
		{ .x = 101*4, .y = 0, .w = 101, .h = 84 },
		{ .x = 101*5, .y = 0, .w = 101, .h = 84 }

	}

};


#endif
