#pragma once
#include <vector>

//This is a class that is just there to split the workload into two tasks. It also gives the Renderer a more Cohesive responsibility, although it's not necessary
namespace dae {
	class ImGuiTempRenderer final
	{
		void DrawPlotForTranshCash();
		void DrawPlotForGameObject3D();
		std::vector<float> m_TrashCashData{};
		int m_TrashCashDataSize{};
		int m_TrashCashSteps{};
		std::vector<float> m_TrashCashDataObject3D{};
		std::vector<float> m_TrashCashDataObject3DAlt{};
		int m_TrashCashDataSizeObject3D{};
		int m_TrashCashDataSizeObject3DAlt{};
		int m_TrashCashStepsObject3D{};

	public:
		void DrawGUI();
	};
}

struct Transform
{
	float matrix[16] = {
		1,0,0,0,
		0,1,0,0,
		0,0,1,0,
		0,0,0,1
	};
};

class GameObject3D
{
public:
	Transform transform{};
	int ID;
};

class GameObject3DAlt
{
public:
	GameObject3DAlt() = default;
	~GameObject3DAlt()
	{
		if (transform)
		{
			delete transform;
		}
	};

	GameObject3DAlt(const GameObject3DAlt& other) = delete;
	GameObject3DAlt(GameObject3DAlt&& other) = delete;
	GameObject3DAlt& operator=(const GameObject3DAlt& other) = delete;
	GameObject3DAlt& operator=(GameObject3DAlt&& other) = delete;

	Transform* transform;
	int ID;
};

