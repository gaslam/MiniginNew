#include "RenderComponent.h"
#include "Renderer.h"
#include "Texture2D.h"
#include "ResourceManager.h"
#include <string>
#include "Texture2D.h"
#include "GameObject.h"

dae::RenderComponent::RenderComponent(GameObject* owner,const std::string& filename) : Component(owner)
{
	SetTexture(filename);
}

dae::RenderComponent::RenderComponent(GameObject* owner, std::shared_ptr<Texture2D> texture) : Component{ owner }
{
	m_Texture = texture;
}

void dae::RenderComponent::SetTexture(const std::string& filename)
{
	auto pTexture = ResourceManager::GetInstance().LoadTexture(filename);
	SetTexture(pTexture);
}

void dae::RenderComponent::SetTexture(const std::shared_ptr<Texture2D> texture)
{
	m_Texture = texture;
	glm::vec2 size = m_Texture->GetSize();
	SetWidthAndHeight(size.x,size.y);
	if (m_FrameHeight == 0.f || m_FrameWidth == 0.f)
	{
		SetFrameWidthAndHeight(size.x, size.y);
	}
}

void dae::RenderComponent::Render() const
{
	auto owner = GetOwner();
	if (owner == nullptr)
	{
		return;
	}
	auto transform = owner->GetComponent<Transform>();
	if (transform == nullptr)
	{
		return;
	}

	glm::vec3 pos = transform->GetWorldPosition();

	SDL_Rect srcRect{};
	const glm::ivec2 textureSize = m_Texture->GetSize();
	srcRect.h = static_cast<int>(m_FrameHeight);
	srcRect.w = static_cast<int>(m_FrameWidth);
	srcRect.x = static_cast<int>(m_TextureOffset.x);
	srcRect.y = static_cast<int>(m_TextureOffset.y);

	SDL_Rect desRect{};
	desRect.x = static_cast<int>(pos.x);
	desRect.y = static_cast<int>(pos.y);
	desRect.w = static_cast<int>(srcRect.w * m_Scale);
	desRect.h = static_cast<int>(srcRect.h * m_Scale);

	dae::Renderer::GetInstance().RenderTexture(*m_Texture,srcRect,desRect,m_xFlipped,m_yFlipped);
}

void dae::RenderComponent::SetPosition(float x, float y)
{
	const auto owner = GetOwner();
	if (owner == nullptr)
	{
		return;
	}
	auto transform = owner->GetComponent<Transform>();
	if (transform == nullptr)
	{
		return;
	}

	glm::vec3 pos{ x,y,0.f };
	transform->SetLocalPosition(pos);
}

void dae::RenderComponent::SetWidthAndHeight(float w, float h)
{
	m_Width = w;
	m_Height = h;
}

void dae::RenderComponent::SetFrameWidthAndHeight(float w, float h)
{
	m_FrameWidth = w;
	m_FrameHeight = h;
}
