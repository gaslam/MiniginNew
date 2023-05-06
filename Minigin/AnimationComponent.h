#pragma once
#include <SDL_rect.h>

#include "Component.h"
#include <string>
namespace dae {
	class RenderComponent;
	class AnimationComponent : public Component
	{
	public:
		AnimationComponent(GameObject* object, const std::string& textureFile, float framesSec,float scale = 1,int rows = 1, int cols = 1,int startRow = 0, int startCol = 0,int count = 1,bool canRepeat = false);
		void Update(float elapsedSec) override;
		void Render() const override {};
		bool IsFinished() const { return m_IsFinished; }
		void ChangeAnimation(int rowIdx, int colIdx, int count = 1, bool canRepeat = false, bool xFlipped = false, bool yFlipped = false);
		SDL_Rect GetCell() const;
	private:
		bool m_CanRepeat;
		bool m_IsFinished{ false };
		const int m_Rows;
		const int m_Cols;
		int m_StartFrame;
		int m_EndFrame;
		int m_CurrentFrame;
		int m_LockedFrame{ -1 };
		float m_FrameSec;
		float m_AccuSec{};

		int GetColIdx(int index, int nrCols) const;
		int GetRowIdx(int index, int nrCols) const;
		int GetGridIdx(int rowIndex, int nrCols, int collIndex) const;
		RenderComponent* m_pRenderComponent;
	};
}

