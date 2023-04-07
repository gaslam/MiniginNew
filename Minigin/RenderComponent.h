#pragma once
#include "Component.h"
#include <memory>
#include "string"
#include "Transform.h"
namespace dae {
	class Texture2D;
	class RenderComponent : public Component
	{
	public:
		RenderComponent(GameObject* pOwner, bool display = true) : Component(pOwner), m_CanRender{display} {};
		RenderComponent(GameObject* pOwner,std::shared_ptr<Texture2D>texture, bool display = true);
		RenderComponent(GameObject* pOwner,const std::string& filename , bool display = true);
		virtual void Render() const override;
		virtual void Update(float) override {};
		void SetTexture(const std::string& filename);
		void SetTexture(const std::shared_ptr<Texture2D> texture);
		std::shared_ptr<Texture2D> GetTexture() const { return m_Texture; }
		void SetPosition(float x, float y);
		void SetWidthAndHeight(float w, float h);
		void SetCanRender(bool render) { m_CanRender = render; }
	private:
		std::shared_ptr<Texture2D> m_Texture{};
		float m_Width{};
		float m_Height{};
		bool m_CanRender{};
	};
}

