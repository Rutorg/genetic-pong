#pragma once
#include "NeuroNet.h"
#include "GeneticPong.h"


class Osob
{
public:
	NeuroNet m_net;
	Osob m_racket;
	GeneticPong* m_game;

public:
	Osob(GeneticPong* game)
	{
		m_game = game;

		m_racket = (*m_game).

		m_net.SetLayersNum(3);
		m_net.SetNeuronsNumInLayer(0, 5);
		m_net.SetNeuronsNumInLayer(1, 5);
		m_net.SetNeuronsNumInLayer(2, 3);
		m_net.Init();

	}

	void update(Time elTime)
	{
		float ballSpeedX = (*m_game).m_ball.GetSpeed().x;
		float ballSpeedY = (*m_game).m_ball.GetSpeed().y;
		float ballPosX = (*m_game).m_ball.getPosition().x;
		float ballPosY = (*m_game).m_ball.getPosition().y;
		float racketY = m_racket.getPosition().y;

		vector<float> out(3);
		out = m_net.Activate(vector<float>{ballSpeedX, ballSpeedX, ballPosX, ballPosY, racketY});

		//Up
		if (out[0] > 0.9)
		{
			m_racket.SetIsUp(true);
			m_racket.SetIsDn(false);
		}
		//Down
		if (out[1] > 0.9)
		{
			m_racket.SetIsUp(true);
			m_racket.SetIsDn(false);
		}
		//Nothing
		if (out[2] > 0.9)
		{
			m_racket.SetIsUp(false);
			m_racket.SetIsDn(false);
		}
	}
};


class GeneticAlg
{
public:
	GeneticPong* m_game;

	size_t m_genNum = 100;
	size_t m_numInGen = 10;

	vector<Osob> osobArr;
	
public:

	GeneticAlg(GeneticPong* game)
	{
		m_game = game;

		for (size_t osobI = 0; osobI < m_numInGen; osobI++)
		{
			osobArr.push_back(Osob(m_game));
		}
	}

	void playOneFrame(Time elTime)
	{
		for (size_t osobI = 0; osobI < m_numInGen; osobI++)
		{
			osobArr[osobI].update(elTime);
		}



	}
	




};

