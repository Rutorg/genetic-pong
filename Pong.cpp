#include "Pong.h"

Pong::Pong(Vector2u winSize, Vector2i winPos)
{
	//--------------------------------------------------------------------------------
	// Window
	m_winSize = winSize;
	m_winPos = winPos;

	//--------------------------------------------------------------------------------
	// GameObjects

	// Ball
	m_toDraw.push_back(&m_ball);
	m_ball.setSize(Vector2f(BALL_SIZE, BALL_SIZE));
	m_ball.setOrigin(BALL_SIZE / 2, BALL_SIZE / 2);
	m_ball.setPosition(static_cast<float>(winSize.x) / 2, static_cast<float>(winSize.y) / 2);

	// Racket1
	m_racket1.m_upBut = UP_BUT_1;
	m_racket1.m_dnBut = DN_BUT_1;

	m_racket1.setSize(Vector2f(RACKET_SIZE_X, RACKET_SIZE_Y));
	m_racket1.setOrigin(RACKET_SIZE_X / 2, RACKET_SIZE_Y / 2);
	m_racket1.setPosition(static_cast<float>(winSize.x) * 1 / 5, static_cast<float>(winSize.y) / 2);
	m_toDraw.push_back(&m_racket1);
	// Racket2
	m_racket2.m_upBut = UP_BUT_2;
	m_racket2.m_dnBut = DN_BUT_2;

	m_racket2.setSize(Vector2f(RACKET_SIZE_X, RACKET_SIZE_Y));
	m_racket2.setOrigin(RACKET_SIZE_X / 2, RACKET_SIZE_Y / 2);
	m_racket2.setPosition(static_cast<float>(winSize.x) * 4 / 5, static_cast<float>(winSize.y) / 2);
	m_toDraw.push_back(&m_racket2);

	//-------------------------------------------------------------------------------

	// Sounds
	m_hitSoundBuffer.loadFromFile("Ping.wav");
	m_hitSound.setBuffer(m_hitSoundBuffer);

	//-------------------------------------------------------------------------------

	// Texts
	m_font.loadFromFile("BebasNeue-Regular.ttf");

	m_scoreText1.setString("0");
	m_scoreText1.setFont(m_font);
	m_scoreText1.setPosition(static_cast<float>(winSize.x) * 3 / 4, 0);
	m_toDraw.push_back(&m_scoreText1);

	m_scoreText2.setString("0");
	m_scoreText2.setFont(m_font);
	m_scoreText2.move(static_cast<float>(winSize.x) / 4, 0);
	m_toDraw.push_back(&m_scoreText2);


	m_speedText.setString("0");
	m_speedText.setFont(m_font);
	m_speedText.move(static_cast<float>(winSize.x) / 2 - 10, 0);
	m_toDraw.push_back(&m_speedText);

	//--------------------------------------------------------------------------------
}

void Pong::winCreate(String winTitle)
{
	m_window.create(VideoMode(m_winSize.x, m_winSize.y), winTitle);
	m_window.setPosition(m_winPos);
	m_window.setFramerateLimit(60);
}

void Pong::PlayOneFrame_OLD(Time elTime)
{
	//--------------------------------------------------------------------------------
	Event ev;
	if (m_window.isOpen())
	{
		while (m_window.pollEvent(ev))
		{
			//--------------------------------------------------------------------------------
			// Handling events.

			// Closing window.
			if (ev.type == Event::Closed)
			{
				m_window.close();
			}
			else if (ev.type == Event::KeyPressed)
			{
				switch (ev.key.code)
				{
				case UP_BUT_1:
					m_racket1.SetIsUp(true);
					break;
				case DN_BUT_1:
					m_racket1.SetIsDn(true);
					break;


				case UP_BUT_2:
					m_racket2.SetIsUp(true);
					break;
				case DN_BUT_2:
					m_racket2.SetIsDn(true);
					break;
				}
			}
			else if (ev.type == Event::KeyReleased)
			{
				switch (ev.key.code)
				{
				case UP_BUT_1:
					m_racket1.SetIsUp(false);
					break;
				case DN_BUT_1:
					m_racket1.SetIsDn(false);
					break;


				case UP_BUT_2:
					m_racket2.SetIsUp(false);
					break;
				case DN_BUT_2:
					m_racket2.SetIsDn(false);
					break;
				}
			}
		}
	}

	//--------------------------------------------------------------------------------
	// Processing events.


	//bool isFitnesed = 1;
	//// Обновление функции оценки
	//if (isFitnesed && (m_ball.getPosition().x > winSize.x * 4 / 5 && m_ball.GetSpeed().x > 0))
	//{
	//	isFitnesed = 0;

	//	m_fitness = Fitness();
	//}
	//else if (m_ball.getPosition().x < winSize.x * 4 / 5 && m_ball.GetSpeed().x < 0)
	//{
	//	isFitnesed = 1;
	//}


	Vector2u winSize = m_window.getSize();


	// Racket1 processing and walls for racket1.
	if (m_racket1.IsUp() && !(m_racket1.getPosition().y - RACKET_SIZE_Y/2 < 0))
	{
		m_racket1.MoveByTime(elTime);
	}
	else if (m_racket1.IsDn() && !(m_racket1.getPosition().y + RACKET_SIZE_Y/2 > winSize.y))
	{
		m_racket1.MoveByTime(elTime);
	}
	//--------------------------------------------------------------------------------

	// Racket2 processing and walls for racket2.
	if (m_racket2.IsUp() && !(m_racket2.getPosition().y - RACKET_SIZE_Y/2 < 0))
	{
		m_racket2.MoveByTime(elTime);
	}
	else if (m_racket2.IsDn() && !(m_racket2.getPosition().y + RACKET_SIZE_Y/2 > winSize.y))
	{
		m_racket2.MoveByTime(elTime);
	}
	//--------------------------------------------------------------------------------
	// Walls for ball.

	//Verticals
	if ((m_ball.getPosition().x + BALL_SIZE > winSize.x && m_ball.GetSpeed().x > 0)
		|| (m_ball.getPosition().x - BALL_SIZE < 0 && m_ball.GetSpeed().x < 0))
	{
		//--------------------------------------------------------------------------------

		// Score processing and speed rollback.
		if (m_ball.GetSpeed().x > 0)
		{
			m_score1++;
			m_scoreText1.setString(std::to_string(m_score1));
			m_ball.SetSpeedX(START_BALL_SPEED_X);
		}
		else
		{
			m_score2++;
			m_scoreText2.setString(std::to_string(m_score2));
			m_ball.SetSpeedX(-START_BALL_SPEED_X);
		}

		//--------------------------------------------------------------------------------

		m_ball.SetSpeedX(-m_ball.GetSpeed().x);
	}
	
	//Horizontal
	if ((m_ball.getPosition().y + BALL_SIZE > winSize.y&& m_ball.GetSpeed().y > 0)
		|| (m_ball.getPosition().y - BALL_SIZE < 0 && m_ball.GetSpeed().y < 0))
	{
		m_ball.SetSpeedY(-m_ball.GetSpeed().y);
	}

	//--------------------------------------------------------------------------------
	//Collisions
	if (m_ball.GetSpeed().x < 0 && m_ball.getGlobalBounds().intersects(m_racket1.getGlobalBounds()))
	{
		m_ball.SetSpeedX(-m_ball.GetSpeed().x);

		if (m_racket1.IsUp())
		{
			m_ball.SetSpeedY(m_ball.GetSpeed().y - 20);
		}
		else if (m_racket1.IsDn())
		{
			m_ball.SetSpeedY(m_ball.GetSpeed().y + 20);
		}
		//acceleration
		if (abs(m_ball.GetSpeed().x) < MAX_SPEED)
			m_ball.SetSpeedX(m_ball.GetSpeed().x + 30 * 1.1f);


		if (m_window.isOpen())
		{
			m_hitSound.play();
		}
	}
	else if (m_ball.GetSpeed().x > 0 && m_ball.getGlobalBounds().intersects(m_racket2.getGlobalBounds()))
	{


		m_ball.SetSpeedX(-m_ball.GetSpeed().x);

		if (m_racket2.IsUp())
		{
			m_ball.SetSpeedY(m_ball.GetSpeed().y - 20);
		}
		else if (m_racket2.IsDn())
		{
			m_ball.SetSpeedY(m_ball.GetSpeed().y + 20);
		}
		//acceleration
		if (abs(m_ball.GetSpeed().x) < MAX_SPEED)
			m_ball.SetSpeedX(m_ball.GetSpeed().x - 30 * 1.1f);

		if (m_window.isOpen())
		{
			m_hitSound.play();
		}
	}
	//--------------------------------------------------------------------------------

	//Speed of ball to m_window
	m_speedText.setString(std::to_string(abs(static_cast<int>(m_ball.GetSpeed().x))));

	m_ball.MoveByTime(elTime);

	//--------------------------------------------------------------------------------

	// Render
	if (m_window.isOpen())
	{
		m_window.clear();

		//Render
		for (unsigned int i = 0; i < m_toDraw.size(); i++)
		{
			m_window.draw(*m_toDraw[i]);
		}

		m_window.display();
	}
}

void Pong::PlayContinous_OLD()
{
	Clock clock;
	while (m_window.isOpen())
	{
		PlayOneFrame_OLD(clock.restart());
	}
}

//float Pong::Fitness()
//{
//	float radiusY = abs(m_racket2.getPosition().y - m_ball.getPosition().y);
//	
//	return 1 / radiusY;
//}