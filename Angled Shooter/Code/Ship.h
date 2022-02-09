#ifndef SHIP_H
#define SHIP_H

enum SHIP_PARTS 
{
	PART_BODY, PART_GUN, PART_BEAM, PART_TOTAL
};
enum SHIP_TYPES
{
	playerShip1_green, playerShip1_orange, playerShip1_red,
	playerShip2_green, playerShip2_orange, playerShip2_red,

	enemyBlack1,  enemyBlack2,   enemyBlack3,  enemyBlack4,  enemyBlack5, 
	enemyBlue1,   enemyBlue2,    enemyBlue3,   enemyBlue4,   enemyBlue5, 
	enemyGreen1,  enemyGreen2,   enemyGreen3,  enemyGreen4,  enemyGreen5, 
	enemyRed1,    enemyRed2,     enemyRed3,    enemyRed4,    enemyRed5,

	ufoBlue,  ufoGreen,  ufoRed,  ufoYello
};
enum RANDOM_BUILDS
{
	RANDOM_WEAK = 5, RANDOM_NORMAL = 20, RANDOM_STRONG = 50
};


#define MAX_EXP 100.00f
#define SHIP_BASIC_SPEED 2.01f;
#define SHIP_BASIC_ROTATE_SPEED 1.01f;

// Because of how dependent this class is, you have to define the effects texture and constructor, and Bullets.
// All construcor arguments are defaulted to 0, that is if you will create types and then duplicate it 
class Ship : public Angled_Character
{
public:
	Ship(SDL_Renderer* _renderer, Engine::Texture* _texture, int _ship_type, int _level, Bullet _bullet, _Speed _move_speed, _Speed _shoot_speed);
	~Ship();

	void  Update_Attributes();
	void  Copy(const Ship& _ship);

	// Spawn a new bullet.
	void  Shoot(_Size _size, _Speed _speed, float _angle);
	// Update and render all Shooted bullets.
	void  Shoot_Update();
	void  Shoot_Render();

	virtual void  Update();
	virtual void  Render();

public: //  Basic out game character attributes.
	Engine::Texture*  texture;
	SDL_Renderer*     renderer;

	int         level;
	int         health;
	float       exp;

	SDL_Rect    ship_sprite;
	_Speed      shoot_speed;
	_Speed      reload_speed;

	std::vector<Bullet>    bullets;
	int 				   shooted_bullets;

public:
	static std::vector<SDL_Rect>  ship_sprite_clips;

	static Effect  				  shield_animation;
	static Effect  				  destroyed_animation;
	static Effect  				  boost_animation;

	Effect_Status 				  shield_status;
	Effect_Status 				  destroyed_status;
	Effect_Status 				  boost_status;

};


Ship::Ship(SDL_Renderer* _renderer, Engine::Texture* _texture, int _ship_type, int _level, Bullet _bullet,
		   _Speed _move_speed, _Speed _shoot_speed) : Angled_Character()
{
	renderer = _renderer;
	texture  = _texture;

	move_speed    = _move_speed; // Inherited
	shoot_speed   = _shoot_speed;

	for (int ammo = 200, i = 0; i < ammo; i++)
	{
		bullets.push_back(_bullet);
	}
	shooted_bullets = 0;

	level = _level;
	exp   =  0.0f;
	Update_Attributes();

	static bool has_loaded_data = false;
	if(!has_loaded_data)
	{
		std::ifstream  infile("Configures/Ship_Atlas.txt");
		std::string    _name;
		SDL_Rect 	   _rect;


		// Detroy all objects within vector.
		ship_sprite_clips.clear();

		// Sprite_Atlas:
		infile >> _name;

		while (infile >> _name)
		{
			infile >> _name >> _rect.x;
			infile >> _name >> _rect.y; 
			infile >> _name >> _rect.w;
			infile >> _name >> _rect.h;

			ship_sprite_clips.push_back(_rect);
		}

		infile.close();
		has_loaded_data = true;
	}

	// After loading data, ship_sprite_clips is cleared at first;
	if (_ship_type > ship_sprite_clips.size() || _ship_type < 0) 
	{
		std::cout << "\nShip::Ship(int, int, Uint32, Uint32) undefined ship_sprite argument; turned to Random()";
		ship_sprite = ship_sprite_clips[Rand(0, ship_sprite_clips.size())];
	}
	else
		ship_sprite = ship_sprite_clips[_ship_type];

}
Ship::~Ship()
{
	Kill();
	renderer = nullptr;
	texture  = nullptr;
}

void Ship::Shoot(_Size _size, _Speed _speed, float _angle)
{
	static Uint32 last_update = 0;   
				  last_update = SDL_GetTicks();

	if (last_update > shoot_speed.last_update + shoot_speed.delay) {

		if (shooted_bullets < bullets.size() - 1) {
			shoot_speed.last_update = last_update;

			shooted_bullets++;

			_FRect  position = 
			{
				.x = rect.Center_X() + (Sin_Degree(_angle) * (rect.w/2)), // from center of the ship to designated angle to shoot.
				.y = rect.Center_Y() - (Cos_Degree(_angle) * (rect.h/2)), // Window screen have opposites vertical side.
				.w = _size.w,
				.h = _size.h
			};

			// Since the position when rendering is TOPLEFT to *this center, make it center to itself.
			position.x = (position.x - (position.w/2));
			position.y = (position.y - (position.h/2));

			bullets[shooted_bullets].speed = _speed;

			bullets[shooted_bullets].Spawn(position, angle, SDL_ALPHA_OPAQUE);
		}
		else {
			printf("\nReload first!! Press Q!!");
		}
	}
}
void Ship::Shoot_Update()
{
	if (shooted_bullets > 0)
	{
		for (int i = 0; i < shooted_bullets; i++) {
			if (not(bullets[i].has_hit))
			{
				bullets[i].Update();
			}
			if (bullets[i].has_hit)
			{
				// Vanish it
				bullets[i].Kill();
			}
		}
	}
}
void Ship::Shoot_Render()
{
	if (shooted_bullets > 0) 
	{
		for (int i = 0; i < shooted_bullets; i++) {
			if (not(bullets[i].has_hit))
			{
				bullets[i].Render(renderer, texture);
			}
		}
	}
}
void Ship::Copy(const Ship& _ship)
{
	level  = _ship.level;
	health = _ship.health;

	bullets = _ship.bullets;

	ship_sprite  = _ship.ship_sprite;
	shoot_speed  = _ship.shoot_speed;
	reload_speed = _ship.reload_speed;

	Update_Attributes();
}

void Ship::Update_Attributes()
{
	health             = Rand(level*100, (level*100)+20);
	bullets[0].damage  = Rand(level*2,   (level*2)+4); // temp
}
void Ship::Update()
{
	rect.x += velx;
	rect.y -= vely;

	Shift_Collider_Circle();

	Shoot_Update();
	shield_animation.Update(shield_status);
	boost_animation.Update(boost_status);

	// Make sure to Move() first before updating;
	// If vel x or y is not 0, then update rect, after, reset vel.
	velx = 0.0;
	vely = 0.0;
}
void Ship::Render()
{
	SDL_RenderCopyExF(renderer, texture->Get_SDL_Texture(), &ship_sprite, (SDL_FRect*)rect,
		angle, NULL, SDL_FLIP_NONE);

	Shoot_Render();


	// To Center of player ship.
	_FRect  shield_rect = {
		.x = rect.Center_X() - 40, 
		.y = rect.Center_Y() - 40, 
		.w = 80, .h = 80
	};
	// To Center of back of player ship.
	_FRect  boost_rect = {
		.x = rect.Center_X() - (Sin_Degree(angle) * (rect.w/2) ),
		.y = rect.Center_Y() + (Cos_Degree(angle) * (rect.h/2) ),
		.w = 13, .h = 18
	};
		boost_rect.x = (boost_rect.x - (boost_rect.w/2));
		boost_rect.y = (boost_rect.y - (boost_rect.h/2));
	
	boost_animation.Render(renderer, boost_status, boost_rect);
	shield_animation.Render(renderer, shield_status, shield_rect);
}





std::vector<SDL_Rect>   Ship::ship_sprite_clips = {};

Effect  Ship::shield_animation    {  NULL,
	{
		{ .x = 64*0, .y = 0, .w = 64, .h = 64 },
		{ .x = 64*1, .y = 0, .w = 64, .h = 64 },
		{ .x = 64*2, .y = 0, .w = 64, .h = 64 },
		{ .x = 64*3, .y = 0, .w = 64, .h = 64 },
		{ .x = 64*4, .y = 0, .w = 64, .h = 64 }
	}

};
Effect  Ship::destroyed_animation { NULL,
	{ 
		{ .x = 112, .y = 941, .w = 99, .h = 75 },
		{ .x = 247, .y = 234, .w = 99, .h = 75 },
		{ .x = 247, .y = 159, .w = 99, .h = 75 }
	}
};
Effect Ship::boost_animation     { NULL,
	{
		{ .x = 16*0, .y = 0, .w = 16, .h = 32 },
		{ .x = 16*1, .y = 0, .w = 16, .h = 32 },
		{ .x = 16*2, .y = 0, .w = 16, .h = 32 },
		{ .x = 16*3, .y = 0, .w = 16, .h = 32 }
	}
};



#endif