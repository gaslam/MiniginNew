#pragma once
#include <glm/vec2.hpp>

#include "Component.h"
namespace dae
{
    class Transform;
    class Shape;
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
private:
    std::unique_ptr<Shape> m_Shape{};
    Transform* m_pTransform;
    };
}

