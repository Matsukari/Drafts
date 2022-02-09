#ifndef BULLET_H
#define BULLET_H

enum BULLET_TYPES
{
	laserBlue01,  laserBlue02,  laserBlue03,  laserBlue04,  laserBlue05,  laserBlue06,  laserBlue07,  laserBlue08,
	laserBlue09,  laserBlue10,  laserBlue11,  laserBlue12,  laserBlue13,  laserBlue14,  laserBlue15,  laserBlue16,  
	laserGreen01, laserGreen02, laserGreen03, laserGreen04, laserGreen05, laserGreen06, laserGreen07, laserGreen08, 
	laserGreen09, laserGreen10, laserGreen11, laserGreen12, laserGreen13, laserGreen14, laserGreen15, laserGreen16, 
	laserRed01,   laserRed02,   laserRed03,   laserRed04,   laserRed05,   laserRed06,   laserRed07,   laserRed08,   
	laserRed09,   laserRed10,   laserRed11,   laserRed12,   laserRed13,   laserRed14,   laserRed15,   laserRed16, 
};

class Bullet : public Angled_Character
{
public:
	Bullet(int _bullet_sprite, int _damage, _Speed _speed);
	~Bullet() { };

	virtual void Update();
	virtual void Render(SDL_Renderer* renderer, Engine::Texture* texture);

public:
	int       sprite;
	int       damage;
	_Speed    speed;

	bool 	  has_hit;

public:
	static std::vector<SDL_Rect> sprite_clips;
};

std::vector<SDL_Rect> Bullet::sprite_clips = {};

Bullet::Bullet(int _bullet_sprite, int _damage, _Speed _speed) : Angled_Character()
{
	sprite       = _bullet_sprite;
	move_speed   = _speed;
	speed        = _speed;
	damage       = _damage;

	has_hit		 = false;

	static bool first_instanciated = false;
	if (!first_instanciated)
	{
		std::ifstream  infile("Configures/Bullet_Atlas.txt");
		std::string    _name;
		SDL_Rect 	   _rect;

		// Destroy all objects in vector.
		sprite_clips.clear();

		// Spriteatlas
		infile >> _name;

		while (infile >> _name)
		{
			infile >> _name >> _rect.x;
			infile >> _name >> _rect.y;
			infile >> _name >> _rect.w;
			infile >> _name >> _rect.h;

			sprite_clips.push_back(_rect);
		}

		infile.close();
		first_instanciated = true;
	}
}

void Bullet::Update()
{
	// Call inherited Spawn() before this.
	Move(speed, angle);

	rect.x += velx;
	rect.y -= vely;
	Shift_Collider_Circle();

	velx = 0.0;
	vely = 0.0;
}
void Bullet::Render(SDL_Renderer* renderer, Engine::Texture* texture)
{
	SDL_RenderCopyExF(renderer, texture->Get_SDL_Texture(), &sprite_clips[sprite], (SDL_FRect*)rect,
		angle, NULL, flip);
}


#endif



