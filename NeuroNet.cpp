#include "NeuroNet.h"

#define W_D 5.0f
#define B_D 1000.0f

using Random = effolkronium::random_static;

//=========================================================================

float Neuron::Sig(float x)
{
	return (1 / (1 + exp(-x)));
}

float Neuron::Th(float x)
{
	if (x > 100)
	{
		return 1;
	}
	else if (x < -100)
	{
		return -1;
	}

	float result =  (exp(x) - exp(-x));

	result /= exp(x) + exp(-x);

	return (result);
}

void Neuron::SetBias(float X) { m_bias = X; }

float Neuron::GetBias() { return m_bias; }

float Neuron::Output(std::vector<float> inputs, std::vector<float> weights)
{
	float sum = 0;

	for (unsigned int i = 0; i < inputs.size(); i++)
	{
		sum += inputs[i] * weights[i];
	}

	sum += m_bias;

	return Th(sum);
}

//=========================================================================

void NeuroNet::SetLayersNum(unsigned int size)
{
	m_neuronLayers.resize(size);
	m_weightLayers.resize(size - 1);
}

void NeuroNet::SetNeuronsNumInLayer(unsigned int layerIndex, unsigned int Amount)
{
	m_neuronLayers[layerIndex].resize(Amount);
}

void NeuroNet::Init()
{
	//������������� ������

	//�� ����� ��������
	for (unsigned int layerI = 0; layerI < m_neuronLayers.size(); layerI++)
	{
		//�� ����� �����
		for (unsigned int neuronI = 0; neuronI < m_neuronLayers[layerI].size(); neuronI++)
		{
			m_neuronLayers[layerI][neuronI].SetBias(Random::get<Random::common>(-B_D, B_D));
		}
	}


	// ������������� ����� ���������� �������

	// ����� �� ����� �����.
	for (unsigned int layerI = 0; layerI < m_weightLayers.size(); layerI++)
	{
		unsigned int clastersNum = m_neuronLayers[layerI + 1].size();
		m_weightLayers[layerI].resize(clastersNum);


		// ����� �� ��������� �����.
		unsigned int weightsNum = m_neuronLayers[layerI].size();
		for (unsigned int clasterI = 0; clasterI < m_weightLayers[layerI].size(); clasterI++)
		{
			m_weightLayers[layerI][clasterI].resize(weightsNum);


			// ����� �� ����� �����.
			for (unsigned int weightI = 0; weightI < weightsNum; weightI++)
			{
				m_weightLayers[layerI][clasterI][weightI] = Random::get<Random::common>(-W_D, W_D);
			}
		}
	}
}

std::vector<float> NeuroNet::Activate(std::vector<float> input)
{
	std::vector<float> inputs = input;
	std::vector<float> outputs;
	for (unsigned int layerIndex = 1; layerIndex < m_neuronLayers.size(); layerIndex++)
	{
		outputs.clear();

		for (unsigned int neuronIndex = 0; neuronIndex < m_neuronLayers[layerIndex].size(); neuronIndex++)
		{
			std::vector<float> weights1 = m_weightLayers[layerIndex - 1][neuronIndex];

			float output1 = m_neuronLayers[layerIndex][neuronIndex].Output(inputs, weights1);

			outputs.push_back(output1);
		}

		if (layerIndex == m_neuronLayers.size() - 1)
		{
			return outputs;
		}

		inputs = outputs;
	}

	//������ ���� ���-�� �� ���!
	return std::vector<float>(10, 0);
}

void NeuroNet::Activate(std::vector<float> input, std::vector<float> &output)
{
	std::vector<float> inputs = input;
	std::vector<float> outputs;

	for (unsigned int layerIndex = 1; layerIndex < m_neuronLayers.size(); layerIndex++)
	{
		outputs.clear();

		for (unsigned int neuronIndex = 0; neuronIndex < m_neuronLayers[layerIndex].size(); neuronIndex++)
		{
			std::vector<float> weights1 = m_weightLayers[layerIndex - 1][neuronIndex];

			float output1 = m_neuronLayers[layerIndex][neuronIndex].Output(inputs, weights1);

			outputs.push_back(output1);
		}

		if (layerIndex == m_neuronLayers.size() - 1)
		{
			output = outputs;
			return;
		}

		inputs = outputs;
	}
}

//=========================================================================

int NeuroNet::Save(std::string filePath)
{
	std::ofstream file;
	file.open(filePath);

	// ������ ���-�� ����� ��������.
	file << m_neuronLayers.size() << std::endl;

	// ������ ������
	file << std::endl;

	// ������ ���-�� �������� � ����.
	for (unsigned int layerI = 0; layerI < m_neuronLayers.size(); layerI++)
	{
		file << m_neuronLayers[layerI].size() << std::endl;
	}


	// ������ ������
	file << std::endl;


	// ����� �� ����� �����.
	for (unsigned int layerI = 0; layerI < m_weightLayers.size(); layerI++)
	{
		// ����� �� ��������� �����.
		for (unsigned int clasterI = 0; clasterI < m_weightLayers[layerI].size(); clasterI++)
		{
			// ����� �� ����� �����.
			for (unsigned int weightI = 0; weightI < m_weightLayers[layerI][clasterI].size(); weightI++)
			{
				file << m_weightLayers[layerI][clasterI][weightI] << ' ';
			}

			file << std::endl;
		}
	}


	// 2 ������ ������.
	file << std::endl << std::endl;


	// ���������� ������

	// ����� �� ����� ��������.
	for (unsigned int layerI = 0; layerI < m_neuronLayers.size(); layerI++)
	{
		// ����� �� ����� ������.
		for (unsigned int neuronI = 0; neuronI < m_neuronLayers[layerI].size(); neuronI++)
		{
			file << m_neuronLayers[layerI][neuronI].GetBias() << ' ';
		}

		file << std::endl;
	}


	// �������� ����� � �����.
	file.close();
	return 1;
}

int NeuroNet::Load(std::string filePath)
{
	std::ifstream file;
	file.open(filePath);

	unsigned int neuronLayersNum = 0;

	// ���-�� ����� ��������
	file >> neuronLayersNum;
	if (neuronLayersNum != m_neuronLayers.size())
	{
		return -1;
	}

	// �� ����� ��������
	for (unsigned int layerI = 0; layerI < neuronLayersNum; layerI++)
	{
		unsigned int neuronsNum = 0;

		file >> neuronsNum;
		if (neuronsNum != m_neuronLayers[layerI].size())
		{
			return -2;
		}
	}

	// ����� �� ����� �����.
	for (unsigned int layerI = 0; layerI < m_weightLayers.size(); layerI++)
	{
		// ����� �� ��������� �����.
		for (unsigned int clasterI = 0; clasterI < m_weightLayers[layerI].size(); clasterI++)
		{
			// ����� �� ����� �����.
			for (unsigned int weightI = 0; weightI < m_weightLayers[layerI][clasterI].size(); weightI++)
			{
				float weight;
				file >> weight;

				m_weightLayers[layerI][clasterI][weightI] = weight;
			}
		}
	}

	file.close();
	return 1;
}

//=========================================================================

void NeuroNet::SetWeight(float value, unsigned int layerI, unsigned int clasterI, unsigned int weightI)
{
	m_weightLayers[layerI][clasterI][weightI] = value;
}

float NeuroNet::GetWeight( unsigned int layerI, unsigned int clasterI, unsigned int weightI)
{
	return m_weightLayers[layerI][clasterI][weightI];
}

//=========================================================================

NeuroNet NeuroNet::Crossing(NeuroNet net1, NeuroNet net2)
{
	unsigned int neuronLayersNum = net1.m_neuronLayers.size();


	// ������������� �������� ��.
	NeuroNet child;
	child.SetLayersNum(neuronLayersNum);

	for (unsigned int layerI = 0; layerI < neuronLayersNum; layerI++)
	{
		child.SetNeuronsNumInLayer(layerI, net1.m_neuronLayers[layerI].size());
	}

	child.Init();


	//����������� ������.

	//�� ����� ��������
	for (unsigned int layerI = 0; layerI < neuronLayersNum; layerI++)
	{
		//�� ����� �����
		for (unsigned int neuronI = 0; neuronI < child.m_neuronLayers[layerI].size(); neuronI++)
		{
			if (Random::get<bool>())
				child.m_neuronLayers[layerI][neuronI].SetBias(net1.m_neuronLayers[layerI][neuronI].GetBias());
			else
				child.m_neuronLayers[layerI][neuronI].SetBias(net2.m_neuronLayers[layerI][neuronI].GetBias());
		}
	}


	// ����������� �����.

	// ����� �� ����� �����.
	for (unsigned int layerI = 0; layerI < child.m_weightLayers.size(); layerI++)
	{
		// ����� �� ��������� �����.
		for (unsigned int clasterI = 0; clasterI < child.m_weightLayers[layerI].size(); clasterI++)
		{
			// ����� �� ����� �����.
			for (unsigned int weightI = 0; weightI < child.m_weightLayers[layerI][clasterI].size(); weightI++)
			{
				if (Random::get<bool>())
					child.m_weightLayers[layerI][clasterI][weightI] = net1.m_weightLayers[layerI][clasterI][weightI];
				else
					child.m_weightLayers[layerI][clasterI][weightI] = net2.m_weightLayers[layerI][clasterI][weightI];
			}
		}
	}

	return child;
}

void NeuroNet::Mutate(size_t countToMutate)
{
	std::vector <float> genotype;

	// ������� ����� � ������ � ���� ���������.

	// ����� �� ����� �����.
	size_t weightsCount = 0;
	for (unsigned int layerI = 0; layerI < m_weightLayers.size(); layerI++)
	{
		// ����� �� ��������� �����.
		for (unsigned int clasterI = 0; clasterI < m_weightLayers[layerI].size(); clasterI++)
		{
			// ����� �� ����� �����.
			for (unsigned int weightI = 0; weightI < m_weightLayers[layerI][clasterI].size(); weightI++)
			{
				genotype.push_back(m_weightLayers[layerI][clasterI][weightI]);
				weightsCount++;
			}
		}
	}

	// ����� �� ����� ��������.
	size_t biasesCount = 0;
	for (unsigned int layerI = 0; layerI < m_neuronLayers.size(); layerI++)
	{
		// ����� �� ����� ��������.
		for (unsigned int neuronI = 0; neuronI < m_neuronLayers[layerI].size(); neuronI++)
		{
			genotype.push_back(m_neuronLayers[layerI][neuronI].GetBias());
			biasesCount++;
		}
	}
	//=============================================================================================

	// ��������������� ������� �����
	for (size_t i = 0; i < weightsCount; i++)
	{
		size_t genToMutate = Random::get(0, static_cast<int>(weightsCount) - 1);
		float mutatedGen = genotype[genToMutate] * Random::get<Random::common>(-1.0f, 2.0f);

		genotype[genToMutate] = mutatedGen;
	}


	// ��������������� ������� ������
	for (size_t i = weightsCount; i < countToMutate; i++)
	{
		size_t genToMutate = Random::get(static_cast<int>(weightsCount), static_cast<int>(countToMutate) - 1);
		float mutatedGen = genotype[genToMutate] * Random::get<Random::common>(-1.0f, 2.0f);

		genotype[genToMutate] = mutatedGen;
	}


	//=============================================================================================
	//������� ������� � ���������� ������

	// ����� �� ����� �����.
	size_t count = 0;
	for (unsigned int layerI = 0; layerI < m_weightLayers.size(); layerI++)
	{
		// ����� �� ��������� �����.
		for (unsigned int clasterI = 0; clasterI < m_weightLayers[layerI].size(); clasterI++)
		{
			// ����� �� ����� �����.
			for (unsigned int weightI = 0; weightI < m_weightLayers[layerI][clasterI].size(); weightI++)
			{
				m_weightLayers[layerI][clasterI][weightI] = genotype[count];

				count++;
				if (count == weightsCount)
				{
					break;
				}
			}
		}
	}

	// ����� �� ����� ��������.
	count = 0;
	for (unsigned int layerI = 0; layerI < m_neuronLayers.size(); layerI++)
	{
		// ����� �� ����� ��������.
		for (unsigned int neuronI = 0; neuronI < m_neuronLayers[layerI].size(); neuronI++)
		{
			m_neuronLayers[layerI][neuronI].SetBias(genotype[count]);

			count++;
			if (count == biasesCount)
			{
				break;
			}
		}
	}

}
