#pragma once
#include "Component.h"
#include <memory>
#include "string"
#include "Transform.h"
#include <glm/glm.hpp>
namespace dae {
	class Texture2D;
	class RenderComponent : public Component
	{
	public:
		RenderComponent(GameObject* pOwner) : Component(pOwner) {};
		RenderComponent(GameObject* pOwner,std::shared_ptr<Texture2D>texture);
		RenderComponent(GameObject* pOwner,const std::string& filename);
		virtual void Render() const override;
		virtual void Update(float) override {};
		void SetTexture(const std::string& filename);
		void SetTexture(const std::shared_ptr<Texture2D> texture);
		std::shared_ptr<Texture2D> GetTexture() const { return m_Texture; }
		void SetPosition(float x, float y);
		void SetWidthAndHeight(float w, float h);
		void SetFrameWidthAndHeight(float w, float h);
		float GetWidth() const { return m_Width; }
		float GetHeight() const { return m_Height; }
		float GetWidthScaled() const { return m_Width * m_Scale; }
		float GetHeightScaled() const { return m_Height * m_Scale; }
		float GetFrameWidth() const { return m_FrameWidth; }
		float GetFrameHeight() const { return m_FrameHeight; }
		void SetScale(float scale) { m_Scale = scale; }
		void SetTextureOffset(glm::vec2& offset) { m_TextureOffset = offset; }
		void SetXFlip(bool isFlipped) { m_xFlipped = isFlipped; }
		void SetYFlip(bool isFlipped) { m_xFlipped = isFlipped; }
		void SetXandYFlip(bool isXFlipped, bool isYFlipped) {
			m_xFlipped = isXFlipped;
			m_yFlipped = isYFlipped;
		}
	private:
		bool m_xFlipped{ false };
		bool m_yFlipped{ false };
		float m_Width{};
		float m_Height{};
		float m_FrameWidth{};
		float m_FrameHeight{};
		float m_Scale{1.f};
		glm::vec2 m_TextureOffset{};
		std::shared_ptr<Texture2D> m_Texture{};
	};
}

