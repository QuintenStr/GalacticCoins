#pragma once
#include "Component.h"
#include "IController.h"

class MovementComponent : public Component
{
    std::shared_ptr<IController> controller;

public:
    MovementComponent(const GameObject& parent);
    virtual ~MovementComponent();

    void setStrategy(std::shared_ptr<IController> newController);
    void update(float deltaTime) override;
    bool init() override;

    std::shared_ptr<IController> getController();
};
