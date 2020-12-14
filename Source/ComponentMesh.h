#pragma once

#include "Component.h"
#include "Globals.h"

class Component;

class ComponentMesh : public Component
{
public:
    ComponentMesh();
    ComponentMesh(unsigned int uid);
    ~ComponentMesh();

    const unsigned int GetID() const;

 
    bool IsActive()const override;
    void SetActive(bool active) override;

    void Save(JSON_Array* componentsArry)const override;
    void Load(JSON_Object* componentObj) override;
    
private:
    unsigned int resourceID;

public:
    bool activeMesh = true;

};