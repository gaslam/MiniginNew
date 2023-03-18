#include <stdexcept>
#include "Renderer.h"
#include "SceneManager.h"
#include "Texture2D.h"
#include <imgui.h>
#include <backends/imgui_impl_sdl.h>
#include <backends/imgui_impl_opengl2.h>
#include <imgui_plot.h>
#include <chrono>

using namespace std;
using namespace chrono;

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
	Transform transform;
	int ID;
};

int GetOpenGLDriverIndex()
{
	auto openglIndex = -1;
	const auto driverCount = SDL_GetNumRenderDrivers();
	for (auto i = 0; i < driverCount; i++)
	{
		SDL_RendererInfo info;
		if (!SDL_GetRenderDriverInfo(i, &info))
			if (!strcmp(info.name, "opengl"))
				openglIndex = i;
	}
	return openglIndex;
}

float* GenerateData()
{
	vector<GameObject3D*>buffer{};
	const int bufferSize{ 1000000 };
	static float data[10]{};
	buffer.resize(bufferSize);
	int index{};
	do
	{
		buffer[index] = new GameObject3D{};
		buffer[index]->ID = 100;
		++index;
	} while (index < bufferSize);

	int stepSize{};
	int stepsPassed{};
	for (stepSize = 1; stepSize <= 1024; stepSize *= 2)
	{
		auto start = high_resolution_clock::now();
		for (int i{}; i < bufferSize; i += stepSize)
		{
			buffer[i]->ID *= 2;
		}
		auto end = high_resolution_clock::now();
		auto elapsedTime = duration_cast<microseconds>(end - start).count();
		const float timeInFloat = static_cast<float>(elapsedTime);
		data[stepsPassed] = timeInFloat;
		++stepsPassed;
	}

	for (auto object3D : buffer)
	{
		delete object3D;
	}
	return data;
}

void RenderStats()
{
	float min{}, max{};
	static const float* y_data[] = { GenerateData()};
	for (int i{}; i < 10; ++i)
	{
		if (y_data[0][i] < min)
		{
			min = y_data[0][i];
		}
		if (y_data[0][i] > max)
		{
			max = y_data[0][i];
		}
	}
	static ImU32 colors[3] = { ImColor(0, 255, 0), ImColor(255, 0, 0), ImColor(0, 0, 255) };
	static uint32_t selection_start = 0, selection_length = 0;

	ImGui::Begin("Exercise 1", nullptr, ImGuiWindowFlags_None);
	static char   u8_v = 10;
	static char   u8_one = 1;
	static ImU8   u8_min = 0;
	static ImU8   u8_max = 255;
	ImGui::InputScalar("input u8", ImGuiDataType_U8, &u8_v, &u8_one, NULL, "%u");

	// Draw first plot with multiple sources
	ImGui::PlotConfig conf;
	conf.values.ys = y_data[0];
	conf.values.count = 10;
	conf.grid_y.size = 0.5f;
	conf.line_thickness = 2.f;
	conf.values.color = colors[0];
	conf.scale.min = min;
	conf.scale.max = max;
	conf.tooltip.show = true;
	conf.frame_size = ImVec2(250, 100.f);
	conf.line_thickness = 2.f;

	ImGui::Plot("plot", conf);

	ImGui::End();
}



void dae::Renderer::Init(SDL_Window* window)
{
	m_window = window;
	m_renderer = SDL_CreateRenderer(window, GetOpenGLDriverIndex(), SDL_RENDERER_ACCELERATED);
	if (m_renderer == nullptr) 
	{
		throw std::runtime_error(std::string("SDL_CreateRenderer Error: ") + SDL_GetError());
	}
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGui_ImplSDL2_InitForOpenGL(window, SDL_GL_GetCurrentContext());
	ImGui_ImplOpenGL2_Init();
}

void dae::Renderer::Render()
{
	const auto& color = GetBackgroundColor();
	SDL_SetRenderDrawColor(m_renderer, color.r, color.g, color.b, color.a);
	SDL_RenderClear(m_renderer);

	SceneManager::GetInstance().Render();

	ImGui_ImplOpenGL2_NewFrame();
	ImGui_ImplSDL2_NewFrame(m_window);
	ImGui::NewFrame();

	//Draw code here
	RenderStats();
	if (m_ShowDemo)
	{
		//ImGui::ShowDemoWindow(&m_ShowDemo);
	}
	ImGui::Render();
	ImGui_ImplOpenGL2_RenderDrawData(ImGui::GetDrawData());
	SDL_RenderPresent(m_renderer);
}

void dae::Renderer::Destroy()
{
	ImGui_ImplOpenGL2_Shutdown();
	ImGui_ImplSDL2_Shutdown();
	ImGui::DestroyContext();
	if (m_renderer != nullptr)
	{
		SDL_DestroyRenderer(m_renderer);
		m_renderer = nullptr;
	}
}

void dae::Renderer::RenderTexture(const Texture2D& texture, const float x, const float y) const
{
	SDL_Rect dst{};
	dst.x = static_cast<int>(x);
	dst.y = static_cast<int>(y);
	SDL_QueryTexture(texture.GetSDLTexture(), nullptr, nullptr, &dst.w, &dst.h);
	SDL_RenderCopy(GetSDLRenderer(), texture.GetSDLTexture(), nullptr, &dst);
}

void dae::Renderer::RenderTexture(const Texture2D& texture, const float x, const float y, const float width, const float height) const
{
	SDL_Rect dst{};
	dst.x = static_cast<int>(x);
	dst.y = static_cast<int>(y);
	dst.w = static_cast<int>(width);
	dst.h = static_cast<int>(height);
	SDL_RenderCopy(GetSDLRenderer(), texture.GetSDLTexture(), nullptr, &dst);
}

inline SDL_Renderer* dae::Renderer::GetSDLRenderer() const { return m_renderer; }
