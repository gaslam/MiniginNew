#include <fstream>

#include "Scene.h"

namespace Utils
{
	inline void ReadLevelData(std::string& file, dae::Scene* scene)
	{
		std::ifstream stream{ file };
		while(stream.is_open())
		{
			std::string line{};
			std::getline(stream, line);
			std::cout << line;
		}
	}
}
