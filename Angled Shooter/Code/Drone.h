#ifndef DRONE_H
#define DRONE_H

// Unmanned aerial vehicle(drone). Until I thought of much more cooler term for that, I'm gonna stick with this. REMIND!!!

class Drone
{
public:
	Drone() = default;
	~Drone() { Kill(); };

	void _Init_();
	void Spawn();
	void Move();

	void Shift_Collider_Rect();
	void Kill();

	virtual void User_Function();

	virtual void Follow();
	virtual void Update();
	virtual void Render();

private:
	_Frect rect;
	_Circle collider_rect;

	float velx, vely;

	float angle;
	Uint8 alpha;

	int move_delay;
	int spin_delay;
	Uint32 last_move;

private:
	// Create your own Texture!
};

void Drone::_Init_(Uint8 _alpha, int _move_delay, int _spin_delay)
{
	rect = { 0, 0, 0, 0 };

	velx = 0;
	vely = 0;

	move_delay = _move_delay;
	spin_delay = _spin_delay;

	angle = (float)Rand(1, 360);
	alpha = _alpha;

	last_move = 0;

	Shift_Collider_Rect();
}

void Drone::Spawn(Frect position)
{
	rect = position; // actually includes size.
}

void Drone::Move()
{
	angle = _angle;
	velx = Sin_Degree(angle) * SHIP_BASIC_SPEED;
	vely = Cos_Degree(angle) * SHIP_BASIC_SPEED;

	if (SDL_GetTicks() > last_move + move_delay) {
		last_move = SDL_GetTicks();
		rect.x += velx;
		rect.y -= vely;

		velx = 0.0;
		vely = 0.0;

		Shift_Collider_Rect();
	}
}

void Drone::Shift_Collider_Rect()
{
	collider_rect.x = rect.Center_X();
	collider_rect.y = rect.Center_Y();

	collider_rect.r = rect.w / 2;
}


Texture* Drone::sprite_body = nullptr;
Texture* Drone::sprite_wing = nullptr;