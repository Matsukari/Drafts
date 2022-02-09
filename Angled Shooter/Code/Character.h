#ifndef CHARACTER_H
#define CHARACTER_H

#define DEFAULT_MOVE_SPEED 1.50f
#define SELF_SPEED true

struct _Speed
{
	float   speed;
	int     delay;

	Uint32  last_update = 0;
};
struct _Size
{
	int w;
	int h;
};

// next time, use unary 'b' to indicate one is from base class.
class Angled_Character
{
public:
	Angled_Character();
	virtual ~Angled_Character();
	// Create your own Sprite!

	int health;

	SDL_RendererFlip  flip;
	_Color			  color;

	_FRect   rect;
	_Circle  collider_circle;

	float  velx;
	float  vely;

	float  angle;
	Uint8  alpha;

	_Speed move_speed;

public:
	virtual void Update();
	virtual void Render(SDL_Renderer* renderer);
	virtual void Render(SDL_Renderer* renderer, Engine::Texture* texture);

	virtual void Spawn();
	virtual void Spawn(_FRect _rect, float _angle, Uint8 _alpha);
	virtual void Kill();

	virtual void Move(_Speed speed, float _angle);
	virtual void Move(_Speed speed);
	virtual void Shift_Collider_Circle();

protected:

	virtual void Save_Data();
	virtual void Load_Data();

public:
	static int rendering_screen_w;
	static int rendering_screen_h;

	static void Set_Rendering_Screen(int screen_w, int screen_h);
};

// Static members.
int Angled_Character::rendering_screen_w = 0;
int Angled_Character::rendering_screen_h = 0;
void Angled_Character::Set_Rendering_Screen(int screen_w, int screen_h)
{
	rendering_screen_w = screen_w;
	rendering_screen_h = screen_h;
}


Angled_Character::Angled_Character() // Constructor.
{
	flip = SDL_FLIP_NONE;

	health = 100;

	rect = {0, 0, 0, 0};
	Shift_Collider_Circle();

	velx  = 0.0f; 
	vely  = 0.0f;
	angle = (float)Rand(1, 360);

	alpha = SDL_ALPHA_OPAQUE;

	move_speed.speed = 0;
	move_speed.delay = 0;
}
Angled_Character::~Angled_Character() // Destructor.
{
	this->Kill();
}
void Angled_Character::Spawn()
{
	std::cout << "\nNot defined Spawn(void).";
}
void Angled_Character::Spawn(_FRect _rect, float _angle, Uint8 _alpha) // Actual position for rendering. Call After _Init_()
{
	rect  = _rect;
	angle = _angle;
	alpha = _alpha;
}
void Angled_Character::Kill()
{
	rect.x = -1000;
	rect.y = -1000;

	Shift_Collider_Circle();
}
void Angled_Character::Move(_Speed speed, float _angle) // Only change velx and y, Call update to see the changes and remeber to set to 0 thereafter.
{
	if (speed.speed < 0) {
		std::cout << "\nWarning: Character::Move(float, float) bad argument; turned to default";

		speed.speed = DEFAULT_MOVE_SPEED;
	}

	if (SDL_GetTicks() > move_speed.last_update + speed.delay) {
		move_speed.last_update = SDL_GetTicks();

		angle = _angle;

		velx = Sin_Degree(angle) * speed.speed;
		vely = Cos_Degree(angle) * speed.speed;
	}
}
void Angled_Character::Move(_Speed speed)
{
	Move(speed, this->angle);
}
void Angled_Character::Shift_Collider_Circle() // Follow rect position. Overide if the sprite's w and h are not of the same size.
{
	collider_circle.x = rect.Center_X();
	collider_circle.y = rect.Center_Y();

	collider_circle.r = rect.w / 2;
}
void Angled_Character::Update()
{
	std::cout << "\nNot defined Update.";
}
void Angled_Character::Render(SDL_Renderer* renderer)
{
	std::cout << "\nNot defined Render.";
}
void Angled_Character::Render(SDL_Renderer* renderer, Engine::Texture* texture)
{
	std::cout << "\nNot defined Render(Texture)";
}

void Angled_Character::Load_Data()
{
	std::cout << "\nNot defined Load_Data.";
}
void Angled_Character::Save_Data()
{
	std::cout << "\nNot defined Save_Data.";
}



#endif