#ifndef PLAYER_H
#define PLAYER_H

#define PLAYER 1


class Player : public Ship
{
public:
	Player();
	~Player();

public: // virtual functions

	void User_Function();
	void Update();
	void Render();

	void Save_Data();
	void Load_Data();
};


void Update()
{

}