#include "NeuroNet.h"
#include "GeneticPong.h"

#include "effolkronium/random.hpp"
#include <algorithm>
#include <filesystem>

using Random = effolkronium::random_static;


// Просто смотреть на работу
void Test1()
{
	GeneticPong game(Vector2u(1000, 600), Vector2i(100, 50), 2);

	game.winCreate("Firts");

	Clock clock;
	while (game.m_window.isOpen())
	{
		game.PlayOneFrame(clock.restart());
	}
}


// Нейросети просто работают
void Work1()
{
	GeneticPong game(Vector2u(1000, 600), Vector2i(100, 50), 30);

	game.winCreate("Firts");

	Clock clock;
	while (game.m_window.isOpen())
	{
		game.netUpdate();
		game.PlayOneFrame(clock.restart());
	}
}


void Evolution()
{
	std::ofstream file;
	file.open("logNN.txt");

	GeneticPong game(Vector2u(1000, 600), Vector2i(100, 50), 1000);

	game.winCreate("First");

	Clock clock;
	while (game.m_window.isOpen() || game.m_genNum > 100)
	{
		game.netUpdate();
		game.PlayOneFrame(clock.restart());
		if (game.m_alive == 0 || game.m_currentScore > 150)
		{
			file << game.m_genNum << ' ' << game.m_currentScore << std::endl << std::endl;
			game.EvolutionStep();
			clock.restart();
			game.reset();
		}
	}

	game.m_osobArr[0].m_net.Save("Neuronet.txt");


	file.close();
}



int main()
{
	Evolution();

	return 1;
}