#pragma once

#include "UIComponent.h"
#include "Render/Resources/Buffers/MeshBufferManager.h"

class UTexture2D;

class UUIImageComponent : public UUIComponent
{
public:
    DECLARE_CLASS(UUIImageComponent, UUIComponent)

    UUIImageComponent();
    virtual ~UUIImageComponent() override = default;

    // --- UI 인터페이스 ---
    virtual FBox2D GetUIBounds() const override { return FBox2D(Position, Position + Size); }

    // --- 렌더링 및 엔진 기본 오버라이드 ---
    virtual FPrimitiveProxy* CreateSceneProxy() override;

    // --- 에디터 및 직렬화 ---
    virtual void GetEditableProperties(TArray<FPropertyDescriptor>& OutProps) override;
    virtual void PostEditProperty(const char* PropertyName) override;
    virtual void Serialize(FArchive& Ar) override;

    virtual FMeshBuffer* GetMeshBuffer() const override;
    virtual FMeshDataView GetMeshDataView() const override;

    // --- Getter ---
    UMaterial* GetMaterial() const { return Material; }
    float GetRotation() const { return Rotation; }

    // --- Setter ---
    void SetMaterial(UMaterial* InMaterial);
    void SetRotation(float InRotation);

private:
    float Rotation = 0.0f;

    FMaterialSlot MaterialSlot;
    UMaterial* Material = nullptr;
};
