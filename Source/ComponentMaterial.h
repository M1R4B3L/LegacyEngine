#pragma once

#include "Component.h"

class Component;

class ComponentMaterial : public Component
{
public:
    ComponentMaterial(unsigned int texture);
    ~ComponentMaterial();

    const unsigned int GetID() const;

private:
    unsigned int textureID;
};