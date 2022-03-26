#pragma once

#include "effolkronium/random.hpp"
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <iostream>
#include <string>


#define MAX_SPEED (10000)

#define START_BALL_SPEED_X (250)
#define START_BALL_SPEED_Y (0)

#define RACKET_SPEED (400)

#define RACKET_SIZE_X (10)
#define RACKET_SIZE_Y (50)

#define BALL_SIZE (10)

#define UP_BUT_1 (sf::Keyboard::W)
#define DN_BUT_1 (sf::Keyboard::S)
#define UP_BUT_2 (sf::Keyboard::Up)
#define DN_BUT_2 (sf::Keyboard::Down)


using namespace sf;
using Random = effolkronium::random_static;


class Ball :public RectangleShape
{
private:
	//per second
	Vector2f m_speedSec = Vector2f(START_BALL_SPEED_X, START_BALL_SPEED_Y);

public:
	Vector2f GetSpeed() { return m_speedSec; }

	void SetSpeed(Vector2f speed) { m_speedSec = speed; }
	void SetSpeedX(float speedX) { m_speedSec.x = speedX; }
	void SetSpeedY(float speedY) { m_speedSec.y = speedY; }


	void MoveByTime(Time elTime)
	{
		float deltaX = m_speedSec.x * elTime.asSeconds();
		float deltaY = m_speedSec.y * elTime.asSeconds();

		move(deltaX, deltaY);
	}
};


class Osob :public RectangleShape
{
private:
	bool m_isUp = false;
	bool m_isDn = false;

	float m_speedSec = RACKET_SPEED;

public:
	//Keyboard::Key m_upBut = Keyboard::W;
	//Keyboard::Key m_dnBut = Keyboard::S;

public:

	void SetIsUp(bool IsUp) { m_isUp = IsUp; }
	void SetIsDn(bool IsDn) { m_isDn = IsDn; }

	bool IsUp() { return m_isUp; }
	bool IsDn() { return m_isDn; }

	void MoveByTime(Time elTime)
	{
		int direction = 0;
		if (m_isUp)
			direction -= 1;
		if (m_isDn)
			direction += 1;

		float deltaY = direction * m_speedSec * elTime.asSeconds();

		move(0, deltaY);
	}
};

class Pong
{
public:
	//--------------------------------------------------------------------------------

	// Window
	RenderWindow m_window;
	Vector2u m_winSize;
	Vector2i m_winPos;

	//--------------------------------------------------------------------------------

	// GameObjects
	std::vector<Drawable*> m_toDraw;

	// Ball
	Ball m_ball;

	// Rackets
	Osob m_racket1;
	Osob m_racket2;

	//-------------------------------------------------------------------------------

	// Texts
	Font m_font;

	Text m_scoreText1;
	Text m_scoreText2;

	Text m_speedText;


	//-------------------------------------------------------------------------------

	// Sounds
	SoundBuffer m_hitSoundBuffer;
	Sound m_hitSound;

	//-------------------------------------------------------------------------------

	// Service

	// Score
	int m_score1 = 0;
	int m_score2 = 0;

	//--------------------------------------------------------------------------------

public:
	Pong(Vector2u winSize, Vector2i winPos);

	void winCreate(String winTitle);

	void PlayOneFrame_OLD(Time elTime);

	void PlayContinous_OLD();
};