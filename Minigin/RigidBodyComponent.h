#pragma once
#include "Component.h"
#include "Shape.h"
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

private:
    std::unique_ptr<Shape> m_Shape{};
    Transform* m_pTransform;
    };
}

