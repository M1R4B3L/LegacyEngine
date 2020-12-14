#pragma once

#include "Component.h"

class Component;

class ComponentMaterial : public Component
{
public:
    ComponentMaterial();
    ComponentMaterial(unsigned int resourceID);
    ~ComponentMaterial();

    const unsigned int GetID() const;

    void Save(JSON_Array* componentsArry) const override;
    void Load(JSON_Object* componentObj) override;

private:
    unsigned int resourceID;
};