#pragma once

#include "Core/ResourceTypes.h"
#include "Render/Scene/Proxies/Primitive/PrimitiveProxy.h"
#include "Math/Matrix.h"

class UUITextComponent;

class FUITextProxy : public FPrimitiveProxy
{
public:
    FUITextProxy(UUITextComponent* InComponent);

    virtual void UpdateMesh() override;
    virtual void UpdateTransform() override;
    virtual void UpdatePerViewport(const FSceneView& SceneView) override;

    FString CachedText;
    float CachedFontScale = 1.0f;
    FVector2 CachedPosition;
    FVector4 CachedColor;
    ID3D11ShaderResourceView* CachedFontSRV = nullptr;
    const FFontResource* CachedFontResource = nullptr;
};