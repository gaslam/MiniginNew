#include "ImGuiTempRenderer.h"
#include <chrono>
#include <imgui_plot.h>

using namespace std;
using namespace chrono;

void GenerateDataTrashCash(int& size, int steps, std::vector<float>& timesFl)
{
	const int bufferSize{ 100000 };
	vector<int>buffer{};
	buffer.resize(bufferSize);
	fill_n(buffer.begin(), bufferSize, 100);
	long stepSize = 1;
	std::vector<long long> times{};
	auto start = high_resolution_clock::now();
	int stepSizeMax{ static_cast<int>(pow(2, steps)) };
	for (stepSize = 1; stepSize <= stepSizeMax; stepSize *= 2)
	{
		for (int i{}; i < bufferSize; i += stepSize)
		{
			buffer[i] *= 2;
		}
		auto end = high_resolution_clock::now();
		auto time = duration_cast<microseconds>(end - start).count();
		times.emplace_back(time);
		start = high_resolution_clock::now();
	}
	size = static_cast<int>(times.size());
	for (int i{}; i < size; ++i)
	{
		timesFl.emplace_back(static_cast<float>(times[i]));
	}
}

void GenerateDataTrashCashGameObject3D(int& size, int steps, std::vector<float>& timesFl)
{
	static float data[10]{};
	const int bufferSize{ 1000000 };
	vector<GameObject3D*>buffer{};
	buffer.resize(bufferSize);
	std::vector<long long> times{};
	int index{};
	do
	{
		buffer[index] = new GameObject3D();
		buffer[index]->ID = 100;
		++index;
	} while (index < bufferSize);

	int stepSize{};
	int stepSizeMax{ static_cast<int>(pow(2, steps)) };
	for (stepSize = 1; stepSize <= stepSizeMax; stepSize *= 2)
	{
		auto start = high_resolution_clock::now();
		for (int i{}; i < bufferSize; i += stepSize)
		{
			buffer[i]->ID *= 2;
		}
		auto end = high_resolution_clock::now();
		auto elapsedTime = duration_cast<microseconds>(end - start).count();
		times.emplace_back(elapsedTime);
	}
	size = static_cast<int>(times.size());
	for (int i{}; i < size; ++i)
	{
		timesFl.emplace_back(static_cast<float>(times[i]));
	}

	for (auto object : buffer)
	{
		delete object;
	}
}

void GenerateDataTrashCashGameObject3DAlt(int& size, int steps, std::vector<float>& timesFl)
{
	static float data[10]{};
	const int bufferSize{ 1000000 };
	vector<GameObject3DAlt*>buffer{};
	buffer.resize(bufferSize);
	std::vector<long long> times{};
	int index{};
	do
	{
		buffer[index] = new GameObject3DAlt();
		buffer[index]->ID = 100;
		++index;
	} while (index < bufferSize);

	int stepSize{};
	int stepSizeMax{ static_cast<int>(pow(2, steps)) };
	for (stepSize = 1; stepSize <= stepSizeMax; stepSize *= 2)
	{
		auto start = high_resolution_clock::now();
		for (int i{}; i < bufferSize; i += stepSize)
		{
			buffer[i]->ID *= 2;
		}
		auto end = high_resolution_clock::now();
		auto elapsedTime = duration_cast<microseconds>(end - start).count();
		times.emplace_back(elapsedTime);
	}

	size = static_cast<int>(times.size());
	for (int i{}; i < size; ++i)
	{
		data[i] = static_cast<float>(times[i]);
	}

	for (int i{}; i < size; ++i)
	{
		timesFl.emplace_back(static_cast<float>(times[i]));
	}

	for (auto object : buffer)
	{
		delete object;
	}
}

void RenderStats(float min, float max, const float** data, int values, ImU32* colors, int count = 1)
{
	// Draw first plot with multiple sources
	ImGui::PlotConfig conf;
	conf.values.ys_list = data;
	conf.values.ys_count = count;
	conf.values.count = values;
	conf.line_thickness = 2.f;
	conf.values.colors = colors;
	conf.scale.min = min;
	conf.scale.max = max;
	conf.tooltip.show = true;
	conf.frame_size = ImVec2(250, 100.f);
	conf.line_thickness = 2.f;
	conf.tooltip.format = "step=%.f\nmeasurement=%.f ms";
	conf.grid_y.show = true;
	conf.grid_y.subticks = 5;
	conf.skip_small_lines = true;

	ImGui::Plot("plot", conf);
}

void RenderStats(std::vector<float> data, int values, ImU32& color)
{
	auto minIt = std::min_element(data.begin(), data.end());
	auto maxIt = std::max_element(data.begin(), data.end());
	float min = *minIt;
	float max = *maxIt;

	const float* dataFl[]{ {&data[0]} };
	ImU32 colors[1]{ {color} };

	RenderStats(min, max, dataFl, values, colors);
}

void dae::ImGuiTempRenderer::DrawGUI()
{
	DrawPlotForTranshCash();
	DrawPlotForGameObject3D();
}

void dae::ImGuiTempRenderer::DrawPlotForTranshCash()
{
	ImGui::Begin("Exercise 1", nullptr, ImGuiWindowFlags_None);
	static char   u8_v = 100;
	static char   u8_one = 1;
	static ImU8   u8_min = 0;
	static ImU8   u8_max = 10;
	ImGui::InputScalar("samples (0 to 10)", ImGuiDataType_U8, &u8_v, &u8_one, NULL, "%u");
	m_TrashCashSteps = static_cast<int>(u8_v);
	const char* buttonText = "Trash the cache";
	if (u8_v > u8_max)
	{
		u8_v = u8_max;
	}
	if (u8_v < u8_min)
	{
		u8_v = u8_min;
	}

	if (ImGui::Button(buttonText))
	{
		ImGui::Text("Generating data ...");
		m_TrashCashDataSize = 0;
		GenerateDataTrashCash(m_TrashCashDataSize, m_TrashCashSteps, m_TrashCashData);
	}

	if (m_TrashCashDataSize > 0)
	{
		ImU32 colorOrange = { ImColor{195,165,0} };
		RenderStats(m_TrashCashData, m_TrashCashDataSize, colorOrange);
	}
	ImGui::End();
}

void dae::ImGuiTempRenderer::DrawPlotForGameObject3D()
{
	static char   u8_v = 10;
	static char   u8_one = 1;
	static ImU8   u8_min = 0;
	static ImU8   u8_max = 10;
	ImU32 colorGreen = { ImColor{0,255,0} };
	ImU32 colorAliceBlue = { ImColor{94,97,100} };
	const bool size3DBiggerThan0 = m_TrashCashDataSizeObject3D > 0;
	const bool size3DAltBiggerThan0 = m_TrashCashDataSizeObject3DAlt > 0;


	ImGui::Begin("Exercise 2", nullptr, ImGuiWindowFlags_None);
	ImGui::InputScalar("samples(0 to 10)", ImGuiDataType_U8, &u8_v, &u8_one, NULL, "%u");
	m_TrashCashStepsObject3D = static_cast<int>(u8_v);

	if (u8_v > u8_max)
	{
		u8_v = u8_max;
	}
	if (u8_v < u8_min)
	{
		u8_v = u8_min;
	}

	const char* buttonText = "Trash the cache with GameObject3D";
	if (ImGui::Button(buttonText))
	{
		ImGui::Text("Generating data ...");
		m_TrashCashDataSizeObject3D = 0;
		GenerateDataTrashCashGameObject3D(m_TrashCashDataSizeObject3D, m_TrashCashStepsObject3D, m_TrashCashDataObject3D);
	}

	if (size3DBiggerThan0)
	{
		RenderStats(m_TrashCashDataObject3D, m_TrashCashDataSizeObject3D, colorGreen);
	}

	buttonText = "Trash the cache With GameObject3DAlt";
	if (ImGui::Button(buttonText))
	{
		ImGui::Text("Generating data ...");
		m_TrashCashDataSizeObject3DAlt = 0;
		GenerateDataTrashCashGameObject3DAlt(m_TrashCashDataSizeObject3DAlt, m_TrashCashStepsObject3D, m_TrashCashDataObject3DAlt);
	}

	if (size3DAltBiggerThan0)
	{
		RenderStats(m_TrashCashDataObject3DAlt, m_TrashCashDataSizeObject3DAlt, colorAliceBlue);
	}

	if (size3DBiggerThan0 && size3DAltBiggerThan0 && m_TrashCashDataSizeObject3D == m_TrashCashDataSizeObject3DAlt)
	{
		float min3D = *std::min_element(m_TrashCashDataObject3D.begin(), m_TrashCashDataObject3D.end());
		float max3D = *std::max_element(m_TrashCashDataObject3D.begin(), m_TrashCashDataObject3D.end());
		float min3DAlt = *std::min_element(m_TrashCashDataObject3DAlt.begin(), m_TrashCashDataObject3DAlt.end());
		float max3DAlt = *std::max_element(m_TrashCashDataObject3DAlt.begin(), m_TrashCashDataObject3DAlt.end());

		float min = min3D < min3DAlt ? min3D : min3DAlt;
		float max = max3D > max3DAlt ? max3D : max3DAlt;
		const int totalAllowdArrayValues{ 2 };
		const float* dataFl[totalAllowdArrayValues]{ &m_TrashCashDataObject3D[0],&m_TrashCashDataObject3DAlt[0] };
		ImU32 colors[totalAllowdArrayValues]{ colorGreen,colorAliceBlue };

		RenderStats(min, max, dataFl, 10, colors, 2);
	}
	ImGui::End();
}