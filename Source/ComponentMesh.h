#pragma once
#include "Component.h"

class ResourceMesh;

class ComponentMesh : public Component
{
public:
    ComponentMesh();
    ComponentMesh(unsigned int uid);
    ~ComponentMesh();

    const unsigned int GetID() const;
    const ResourceMesh* GetResource()const;

    bool IsActive()const override;
    void SetActive(bool active) override;

    void Save(JSON_Array* componentsArry)const override;
    void Load(JSON_Object* componentObj) override;
    
private:
    unsigned int resourceID;
    ResourceMesh* resource = nullptr;

public:
    bool activeMesh = true;

};