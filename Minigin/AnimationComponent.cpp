#include "AnimationComponent.h"
#include "GameObject.h"
#include "RenderComponent.h"

using namespace dae;

dae::AnimationComponent::AnimationComponent(GameObject* object, const std::string& textureFile, float framesSec, float scale, int rows, int cols, int startRow, int startCol, int count, bool canRepeat) :
	Component(object), m_Rows{ rows }, m_Cols{ cols }, m_FrameSec{ framesSec }, m_StartFrame{ GetGridIdx(startRow,cols,startCol) }, m_EndFrame{ m_StartFrame + count }, m_CanRepeat{ canRepeat }
{
	auto owner = GetOwner();
	if (!owner)
	{
		return;
	}
	m_pRenderComponent = owner->AddComponent<RenderComponent>(textureFile);
	if (m_pRenderComponent)
	{
		m_pRenderComponent->SetScale(scale);
		float textureCellWidth = m_pRenderComponent->GetWidth() / m_Cols;
		float textureCellHeight = m_pRenderComponent->GetHeight() / m_Rows;
		m_pRenderComponent->SetFrameWidthAndHeight(textureCellWidth, textureCellHeight);
	}
}

void dae::AnimationComponent::Update(float elapsedSec)
{
	m_AccuSec += elapsedSec;
	if (m_AccuSec < m_FrameSec)
	{
		return;
	}

	if (m_CurrentFrame == m_EndFrame - 1 && !m_CanRepeat)
	{
		m_IsFinished = true;
		return;
	}

	if (m_EndFrame != 0)
	{
		if (m_LockedFrame > -1)
		{
			if (m_CurrentFrame < m_LockedFrame - 1)
			{
				++m_CurrentFrame;
			}
		}
		else
		{
			++m_CurrentFrame %= m_EndFrame;
		}
	}
	if (m_CurrentFrame < m_StartFrame)
	{
		m_CurrentFrame = m_StartFrame;
	}
	m_AccuSec -= m_FrameSec;

	int rowIndex{ GetRowIdx(m_CurrentFrame, m_Cols) };
	const float textureFrameWidth = m_pRenderComponent->GetFrameWidth();
	const float textureFrameHeight = m_pRenderComponent->GetFrameHeight();
	glm::vec2 offset{};
	offset.x = m_Rows == 1 ? m_CurrentFrame : static_cast<float>(m_CurrentFrame % m_Cols) * textureFrameWidth;
	offset.y = rowIndex * textureFrameHeight;
	m_pRenderComponent->SetTextureOffset(offset);

}

void dae::AnimationComponent::ChangeAnimation(int rowIdx,int colIdx, int count, bool canRepeat, bool xFlipped, bool yFlipped)
{
	m_StartFrame = GetGridIdx(rowIdx, m_Cols, colIdx);
	m_EndFrame = m_StartFrame + count;
	m_CanRepeat = canRepeat;
	m_AccuSec = 0.f;
	m_CurrentFrame = m_StartFrame;
	m_pRenderComponent->SetXandYFlip(xFlipped, yFlipped);
}

int AnimationComponent::GetColIdx(int index, int nrCols)
{
	int result{ index % nrCols };
	return result;
}

int AnimationComponent::GetRowIdx(int index, int nrCols)
{
	int result{ index / nrCols };
	return result;
}

int AnimationComponent::GetGridIdx(int rowIndex, int nrCols, int collIndex)
{
	int result{ rowIndex * nrCols + collIndex };
	return result;
}