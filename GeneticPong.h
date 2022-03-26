#pragma once

#include "effolkronium/random.hpp"
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <iostream>
#include <string>
#include <algorithm>
#include <thread>

#include "NeuroNet.h"


using Random = effolkronium::random_static;
using namespace sf;


#define MAX_SPEED (10000)

#define START_BALL_SPEED_X (300)
#define START_BALL_SPEED_Y (100)

#define RACKET_SPEED (400)

#define RACKET_SIZE_X (10)
#define RACKET_SIZE_Y (50)

#define BALL_SIZE (10)

#define UP_BUT_1 (Keyboard::W)
#define DN_BUT_1 (Keyboard::S)
#define UP_BUT_2 (Keyboard::Up)
#define DN_BUT_2 (Keyboard::Down)



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


class Osob: public RectangleShape
{
public:
	float m_fitness = 0;
	size_t m_score = 0;

	NeuroNet m_net;

	bool isAlive = true;

private:
	bool m_isUp = false;
	bool m_isDn = false;

	float m_speedSec = RACKET_SPEED;
public:
	Keyboard::Key m_upBut = Keyboard::W;
	Keyboard::Key m_dnBut = Keyboard::S;

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


//One window, one ball, many rackets
class GeneticPong
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

	//--------------------------------------------------------------------------------
	// Genetic
	size_t m_osobNum = 0;

	size_t m_genNum = 0;
	size_t m_alive = 0;
	size_t m_currentScore = 0;
	size_t m_maxScore = 0;

	// Ball
	Ball m_ball;

	// RacketArr
	std::vector<Osob> m_osobArr;

	//-------------------------------------------------------------------------------

	// Texts
	Font m_font;

	Text m_genText;
	Text m_aliveText;
	Text m_currentScoreText;
	Text m_maxScoreText;

	//-------------------------------------------------------------------------------

	// Sounds
	SoundBuffer m_hitSoundBuffer;
	Sound m_hitSound;

public:

	GeneticPong(Vector2u winSize, Vector2i winPos, size_t osobNum)
	{
		//--------------------------------------------------------------------------------

		// Window
		m_winSize = winSize;
		m_winPos = winPos;

		//--------------------------------------------------------------------------------
		// GameObjects

		m_alive = osobNum;
		m_osobNum = osobNum;
		m_osobArr.resize(osobNum);

		// Ball
		m_toDraw.push_back(&m_ball);
		m_ball.setSize(Vector2f(BALL_SIZE, BALL_SIZE));
		m_ball.setOrigin(BALL_SIZE / 2, BALL_SIZE / 2);
		m_ball.setPosition(static_cast<float>(winSize.x) / 50, static_cast<float>(winSize.y) / 2);

		// Rackets
		for (size_t osobI = 0; osobI < m_osobArr.size(); osobI++)
		{
			m_osobArr[osobI].setSize(Vector2f(RACKET_SIZE_X, RACKET_SIZE_Y));
			m_osobArr[osobI].setOrigin(RACKET_SIZE_X / 2, RACKET_SIZE_Y / 2);
			m_osobArr[osobI].setPosition(static_cast<float>(winSize.x) * 4 / 5, static_cast<float>(winSize.y) / 2);

			m_osobArr[osobI].m_net.SetLayersNum(3);
			m_osobArr[osobI].m_net.SetNeuronsNumInLayer(0, 5);
			m_osobArr[osobI].m_net.SetNeuronsNumInLayer(1, 5);
			m_osobArr[osobI].m_net.SetNeuronsNumInLayer(2, 3);
			m_osobArr[osobI].m_net.Init();
		}

		//-------------------------------------------------------------------------------
		// Sounds
		m_hitSoundBuffer.loadFromFile("Ping.wav");
		m_hitSound.setBuffer(m_hitSoundBuffer);

		//-------------------------------------------------------------------------------
		// Texts
		m_font.loadFromFile("BebasNeue-Regular.ttf");

		m_genText.setString("Generation: " + std::to_string(m_genNum));
		m_aliveText.setString("Alive: " + std::to_string(m_alive));
		m_currentScoreText.setString("Current Score: " + std::to_string(m_currentScore));
		m_maxScoreText.setString("Max Score: " + std::to_string(m_maxScore));

		m_genText.setFont(m_font);
		m_aliveText.setFont(m_font);
		m_currentScoreText.setFont(m_font);
		m_maxScoreText.setFont(m_font);

		m_genText.setPosition(0, 0);
		m_aliveText.setPosition(0, 30);
		m_currentScoreText.setPosition(0, 60);
		m_maxScoreText.setPosition(0, 90);

		m_toDraw.push_back(&m_genText);
		m_toDraw.push_back(&m_aliveText);
		m_toDraw.push_back(&m_currentScoreText);
		m_toDraw.push_back(&m_maxScoreText);
		//-------------------------------------------------------------------------------
	}


	bool compOsobsas(Osob osob1, Osob osob2)
	{
		return true;
	}
	void Selection_old()
	{
		//����� �� ���������� �������
		//sort(bots.begin(), bots.end(), comp);

		//�������� �����
		//��������
		std::vector<Osob> bufferArr(static_cast<int>(m_osobNum / 2));
		for (size_t i = 0; i < bufferArr.size(); i++)
		{
			//bufferArr[i] = m_osobArr[i];
		}

		//�����������
		//for (size_t i = 0; i < bots.size(); i++)
		//{
		//	size_t num1 = Random::get(0, static_cast<int>(m_osobNum / 2) - 1);
		//	size_t num2 = Random::get(0, static_cast<int>(m_osobNum / 2) - 1);

		//	bots[i] = NeuroNet::Crossing(bufferArr[num1], bufferArr[num2]);
		//}
	}


	static bool compOsobs(Osob osob1, Osob osob2)
	{
		return osob1.m_fitness > osob2.m_fitness;
	}

	// ����� ��������
	void Selection()
	{
		// �������������� ������ ��� ����, ����� �������� ������ � ������ ������
		sort(m_osobArr.begin(), m_osobArr.end(), GeneticPong::compOsobs);

		// ����� ������ ���� ������
		float sum = 0;
		for (size_t osobI = 0; osobI < m_osobNum; osobI++)
		{
			sum += m_osobArr[osobI].m_fitness;
		}

		sum /= m_osobNum;


		//// ����������� ������ ��� �����������
		//vector <float> VerSkr;
		//for (size_t osobI = 0; osobI < m_osobNum; osobI++)
		//{
		//	float a = Random::get<Random::common>(1, 2);
		//	float b = 2 - a;
		//	VerSkr.push_back( ( a - (a - b) * (osobI) / (m_osobNum)  ) / m_osobNum);
		//}



		// ����������� ������ ��� �����������
		std::vector <float> VerSkr;
		for (size_t osobI = 0; osobI < m_osobNum; osobI++)
		{
			VerSkr.push_back(m_osobArr[osobI].m_fitness / (sum + m_osobArr[osobI].m_fitness));
		}

		//��������� ����
		std::vector<Osob> secBufferArr(m_osobNum);
		for (size_t osobI = 0; osobI < m_osobNum; osobI++)
		{
			secBufferArr[osobI].m_net = m_osobArr[osobI].m_net;
		}


		//��������������� �����������
		size_t countSkr = static_cast<size_t>(m_osobNum * 0.95f);	//������� ���������� ����� ������� ������������
		size_t count = m_osobNum - countSkr;;
		for (size_t osob1 = 0; count != countSkr; osob1++)
		{
			float a = Random::get<Random::common>(0, 1.0f);

			//���� ���� ������� ��� �����������
			if (a < VerSkr[osob1]){

				//����� ������ ��� �����������
				for (size_t osob2 = 0; count != countSkr; osob2++){

					float b = Random::get<Random::common>(0, 1.0f);

					if (b < VerSkr[osob2]){
						m_osobArr[count].m_net = NeuroNet::Crossing(secBufferArr[osob1].m_net, secBufferArr[osob2].m_net);

						count++;
						break;
					}

					if (osob2 == countSkr - 1)
						osob2 = 0;
				}
			}

			if (osob1 == countSkr - 1)
				osob1 = 0;
		}
	}

	void Mutation()
	{
		//������� % �� ������
		size_t countToMutate = static_cast<size_t>(0.2 * m_osobNum);
		for (size_t count = 0; count < countToMutate; count++)
		{
			size_t osobI = Random::get(0, static_cast<int>(m_osobNum) - 1);

			m_osobArr[osobI].m_net.Mutate(25);
		}
	}

	void EvolutionStep()
	{
		Selection();
		Mutation();
	}

	void reset()
	{
		m_alive = m_osobNum;
		m_currentScore = 0;
		m_genNum++;
		//--------------------------------------------------------------------------------

		// Ball
		m_ball.setPosition(static_cast<float>(m_winSize.x / 50), static_cast<float>(m_winSize.y) / 2);


		if (Random::get<bool>())
		{
			m_ball.SetSpeed(Vector2f(START_BALL_SPEED_X, Random::get<float>(300, 500)));
		}
		else
		{
			m_ball.SetSpeed(Vector2f(START_BALL_SPEED_X, Random::get<float>(-500, -300)));
		}

		// Osobi
		for (size_t osobI = 0; osobI < m_osobArr.size(); osobI++)
		{
			m_osobArr[osobI].setPosition(static_cast<float>(m_winSize.x) * 4 / 5, static_cast<float>(m_winSize.y) / 2);

			m_osobArr[osobI].m_fitness = 0;

			m_osobArr[osobI].isAlive = true;
		}

		//-------------------------------------------------------------------------------
		// Texts

		m_genText.setString("Generation: " + std::to_string(m_genNum));
		m_aliveText.setString("Alive: " + std::to_string(m_alive));
		m_currentScoreText.setString("Current Score: " + std::to_string(m_currentScore));
		m_maxScoreText.setString("Max Score: " + std::to_string(m_maxScore));

		//-------------------------------------------------------------------------------
	}

	void winCreate(String winTitle)
	{
		m_window.create(VideoMode(m_winSize.x, m_winSize.y), winTitle);
		m_window.setPosition(m_winPos);
		m_window.setFramerateLimit(60);
	}

	void netUpdate()
	{

		float ballPosX = m_ball.getPosition().x;
		float ballPosY = m_ball.getPosition().y;
		float ballSpeedX = m_ball.GetSpeed().x;
		float ballSpeedY = m_ball.GetSpeed().y;

		for (size_t osobI = 0; osobI < m_osobNum; osobI++)
		{
			if (!m_osobArr[osobI].isAlive)
			{
				continue;
			}

			float osobPosY = m_osobArr[osobI].getPosition().y;

			std::vector <float> input { ballPosX, ballPosY, ballSpeedX, ballSpeedY, osobPosY };
			std::vector <float> output(3);

			output = m_osobArr[osobI].m_net.Activate(input);

			// Up
			if (output[0] > output[1] && output[0] > output[2])
			{
				m_osobArr[osobI].SetIsUp(true);
				m_osobArr[osobI].SetIsDn(false);
			}
			// Dn
			else if (output[1] > output[0] && output[1] > output[2])
			{
				m_osobArr[osobI].SetIsUp(false);
				m_osobArr[osobI].SetIsDn(true);
			}
			// Nothing
			else if (output[2] > output[0] && output[2] > output[1])
			{
				m_osobArr[osobI].SetIsUp(false);
				m_osobArr[osobI].SetIsDn(false);
			}
		}
	}

	void PlayOneFrame(Time elTime)
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
			}
		}

		//--------------------------------------------------------------------------------

		size_t osobArrSize = m_osobArr.size();

		// Racket processing and walls for racket.
		for (size_t osobI = 0; osobI < osobArrSize; osobI++)
		{
			// ���� ��� ������
			if (!m_osobArr[osobI].isAlive)
			{
				continue;
			}


			if (m_osobArr[osobI].IsUp() && !(m_osobArr[osobI].getPosition().y - RACKET_SIZE_Y / 2 < 0))
			{
				m_osobArr[osobI].MoveByTime(elTime);
			}
			else if (m_osobArr[osobI].IsDn() && !(m_osobArr[osobI].getPosition().y + RACKET_SIZE_Y / 2 > m_winSize.y))
			{
				m_osobArr[osobI].MoveByTime(elTime);
			}
		}


		//--------------------------------------------------------------------------------
		// Walls for ball.

		float ballPosX = m_ball.getPosition().x;
		float ballPosY = m_ball.getPosition().y;
		float ballSpeedX = m_ball.GetSpeed().x;
		float ballSpeedY = m_ball.GetSpeed().y;

		//Vertical
		if ((ballPosX + BALL_SIZE > m_winSize.x && ballSpeedX > 0)
			|| (ballPosX - BALL_SIZE < 0 && ballSpeedX < 0))
		{
			m_ball.SetSpeedX(-ballSpeedX);
			ballSpeedX = m_ball.GetSpeed().x;
		}

		//Horizontal
		if ((ballPosY + BALL_SIZE > m_winSize.y && ballSpeedY > 0)
			|| (ballPosY - BALL_SIZE < 0 && ballSpeedY < 0))
		{
			m_ball.SetSpeedY(-ballSpeedY);
			ballSpeedY = m_ball.GetSpeed().y;
		}

		//--------------------------------------------------------------------------------
		// Collisions
		// �������
		if (ballPosX + BALL_SIZE > 0.8 * m_winSize.x && ballSpeedX > 0)
		{
			for (size_t osobI = 0; osobI < osobArrSize; osobI++)
			{
				// ���� ��� ������
				if (!m_osobArr[osobI].isAlive)
				{
					continue;
				}

				float upperY = m_osobArr[osobI].getPosition().y + RACKET_SIZE_Y;
				float bottomY = m_osobArr[osobI].getPosition().y - RACKET_SIZE_Y;

				if (bottomY <= ballPosY - BALL_SIZE && ballPosY + BALL_SIZE <= upperY) // �����
				{
					//fitness
					m_osobArr[osobI].m_fitness++;
				}
				else // �� �����
				{
					m_osobArr[osobI].isAlive = false;
					m_osobArr[osobI].m_fitness +=  50 / abs(ballPosY - m_osobArr[osobI].getPosition().y);
					m_alive--;
				}
			}

			if (m_alive > 0)
			{
				m_currentScore++;

				m_hitSound.play();

				m_ball.SetSpeedX(-ballSpeedX - 30);
				ballSpeedX = m_ball.GetSpeed().x;
			}

			// Max score
			if (m_currentScore > m_maxScore)
			{
				m_maxScore = m_currentScore;
			}

		}

		//--------------------------------------------------------------------------------

		m_ball.MoveByTime(elTime);

		//--------------------------------------------------------------------------------
		//Text
		m_genText.setString("Generation: " + std::to_string(m_genNum));
		m_aliveText.setString("Alive: " + std::to_string(m_alive));
		m_currentScoreText.setString("Current Score: " + std::to_string(m_currentScore));
		m_maxScoreText.setString("Max Score: " + std::to_string(m_maxScore));

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

			// Rackets render
			for (size_t osobI = 0; osobI < osobArrSize; osobI++)
			{
				if (m_osobArr[osobI].isAlive)
				{
					m_window.draw(m_osobArr[osobI]);
				}
			}

			m_window.display();
		}
	}
};