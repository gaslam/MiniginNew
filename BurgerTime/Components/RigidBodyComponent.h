#pragma once
#include <glm/vec2.hpp>
#include "Shape.h"
#include <Component.h>
namespace dae
{
    class Transform;
    class RigidBodyComponent :
        public Component
    {

    public:
        RigidBodyComponent(GameObject* pOwner, Transform* pTransform) : Component(pOwner), m_pTransform(pTransform){}
        void Render() const override;
        void Update(float) override;
        void SetShape(Shape* shape);
        Shape* GetShape() const { return m_Shape.get(); }
        bool IsPointInRect(glm::ivec2& point) const;
        bool IsOverlapping(RigidBodyComponent* pComponent) const;
        bool IsStatic() const { return m_IsStatic; }
        void SetIsStatic(bool isStatic)
        {
	        m_IsStatic = isStatic;
            if(!m_IsStatic)
            {
                m_TotalTimePassed = 0;
            }
            else
            {
                m_AllowTroughGround = true;
            }
        }

        void SetAllowThroughGround(bool isAllowed) { m_AllowTroughGround = isAllowed; }
        bool IsAllowedThroughGround() const { return m_AllowTroughGround; }
private:
    bool m_AllowTroughGround{false};
    bool m_IsStatic{true};
    float m_Gravity{ -9.81f };
    float m_TotalTimePassed{};
    std::unique_ptr<Shape> m_Shape{};
    Transform* m_pTransform;
    glm::vec2 m_Velocity{};
    };
}

