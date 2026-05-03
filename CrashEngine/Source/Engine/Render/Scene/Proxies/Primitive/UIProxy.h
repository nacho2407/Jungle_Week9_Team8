#pragma once

#include "Render/Scene/Proxies/Primitive/PrimitiveProxy.h"

class UUIImageComponent;

class FUIProxy : public FPrimitiveProxy
{
public:
    FUIProxy(UUIImageComponent* InComponent);

    virtual void UpdateMesh() override;
    virtual void UpdateTransform() override;
    virtual void UpdateMaterial() override;
};