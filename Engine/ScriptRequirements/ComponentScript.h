#pragma once
#include "Component.h"

class ResourceScript;

class __declspec(dllimport) ComponentScript : public Component
{
public:
    //ComponentScript();
    ComponentScript(unsigned int resourceID);
    ~ComponentScript();

    const unsigned int GetID() const;
    const ResourceScript* GetResource()const;

    void Start() override;
    void Update(float dt)override;
    const char* GetName();

    //bool ChangeResource(unsigned int id);

    void Save(JSON_Array* componentsArry) const override;
    void Load(JSON_Object* componentObj) override;

private:
    unsigned int resourceID;
    ResourceScript* resource = nullptr;
    //void scriptPtr;
};