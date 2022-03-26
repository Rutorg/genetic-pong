#pragma once

#include "effolkronium/random.hpp"
#include <vector>
#include <cmath>
#include <fstream>
#include <iostream>
#include <string>


class Neuron
{
private:
	float m_bias;
	float Sig(float x);
	float Th(float x);

public:
	void SetBias(float X);
	float GetBias();
	float Output(std::vector<float> inputs, std::vector<float> weights);
};

class NeuroNet
{
	//Структура:
	//Два вектора, один вектор слоев нейронов (слои -> сами нейроны), другой вектор кластеров весов (слои -> кластеры -> сами весы).
private: 
	std::vector< std::vector<Neuron> > m_neuronLayers;

	std::vector< std::vector < std::vector<float> > > m_weightLayers;

public:
	void SetLayersNum(unsigned int size);

	void SetNeuronsNumInLayer(unsigned int layerIndex, unsigned int Amount);

	void Init();

	int Load(std::string filePath);

	int Save(std::string filePath);

	void SetWeight(float value, unsigned int layerI, unsigned int clasterI, unsigned int neuronI);

	float GetWeight(unsigned int layerI, unsigned int clasterI, unsigned int weightI);

	static NeuroNet Crossing(NeuroNet net1, NeuroNet net2);

	void Mutate(size_t countToMutate);

	std::vector<float> Activate(std::vector<float> input);
	void Activate(std::vector<float> input, std::vector<float>& output);
};