#pragma once
#include "Component.h"

class ResourceTexture;

class ComponentMaterial : public Component
{
public:
    ComponentMaterial();
    ComponentMaterial(unsigned int resourceID);
    ComponentMaterial(ResourceTexture* resource);
    ~ComponentMaterial();

    const unsigned int GetID() const;
    const ResourceTexture* GetResource()const;

    void Save(JSON_Array* componentsArry) const override;
    void Load(JSON_Object* componentObj) override;

private:
    unsigned int resourceID;
    ResourceTexture* resource = nullptr;
};