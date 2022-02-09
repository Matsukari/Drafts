#ifndef EFFECT_H
#define EFFECT_H

#define EFFECT_ON   1
#define EFFECT_OFF  0

struct Effect_Status
{
	int    status 		= EFFECT_OFF;

	int     encores 	= 0;
	int     cur_replays = 0;
	int 	cur_frame 	= 0;
	int     speed 		= 0;
	Uint32  lasttime 	= 0;
};


class Effect
{
public:
	Effect(Engine::Texture* _texture, std::vector<SDL_Rect> _clips);
	Effect();
	~Effect();

	void _Init_(Engine::Texture* _texture, std::vector<SDL_Rect> _clips);

	void Play(Effect_Status& e, int _speed, int _encores = 0);
	void Update(Effect_Status& e);
	void Render(SDL_Renderer* renderer, Effect_Status& e, _FRect rect);

public:
	// Originally just a pointing object.
	Engine::Texture* 	   texture;
	std::vector<SDL_Rect>  clips;
};

Effect::Effect(Engine::Texture* _texture, std::vector<SDL_Rect> _clips)
{
	_Init_(_texture, _clips);
}
Effect::Effect()
{
	texture = nullptr;
	clips = {};
};
Effect::~Effect()
{
	texture = nullptr;
}

void Effect::_Init_(Engine::Texture* _texture, std::vector<SDL_Rect> _clips)
{
	texture     = _texture;
	clips       = _clips;
}
void Effect::Play(Effect_Status& e, int _speed, int _encores)
{
	// Only play after finishing current one.
	if (e.status == EFFECT_OFF) 
	{
		e.status  =  EFFECT_ON;

		e.encores = _encores;
		e.speed   = _speed;
	}
}
void Effect::Update(Effect_Status& e)
{
	if (e.status == EFFECT_ON && (SDL_GetTicks() > e.lasttime + e.speed))
	{
		e.lasttime = SDL_GetTicks();

		e.cur_frame++;

		if (e.cur_frame >= clips.size())
		{
			e.cur_frame = 0;
			e.cur_replays++;

			// If the number of replays is greater than or equal the provided times of replays. Turn if off.
			if (e.cur_replays > e.encores) 
			{
				e.cur_replays = 0;
				e.status      = EFFECT_OFF;
			}
		}
	}
}

void Effect::Render(SDL_Renderer* renderer, Effect_Status& e, _FRect rect)
{
	if (e.status == EFFECT_ON) 
	{
		SDL_Rect temp_rect = (SDL_Rect)rect;
		texture->Render(renderer, &temp_rect, &clips[e.cur_frame]);
	}
}

#endif