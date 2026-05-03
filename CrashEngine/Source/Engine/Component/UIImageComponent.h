#pragma once

#include "Component/PrimitiveComponent.h"
#include "Math/Vector.h" 
#include "Render/Resources/Buffers/MeshBufferManager.h"

class UTexture2D;

class UUIImageComponent : public UPrimitiveComponent
{
public:
    DECLARE_CLASS(UUIImageComponent, UPrimitiveComponent)

    UUIImageComponent();
    virtual ~UUIImageComponent() override = default;

    // --- 렌더링 및 엔진 기본 오버라이드 ---
    virtual FPrimitiveProxy* CreateSceneProxy() override;
    virtual void UpdateWorldAABB() const override;

    // --- 에디터 및 직렬화 ---
    virtual void GetEditableProperties(TArray<FPropertyDescriptor>& OutProps) override;
    virtual void PostEditProperty(const char* PropertyName) override;
    virtual void Serialize(FArchive& Ar) override;

    virtual FMeshBuffer* GetMeshBuffer() const override;
    virtual FMeshDataView GetMeshDataView() const override;

    // --- Getter ---
    FVector2 GetPosition() const { return Position; }
    FVector2 GetSize() const { return Size; }
    FVector4 GetColor() const { return Color; }
    int32 GetZOrder() const { return ZOrder; }
    UMaterial* GetMaterial() const { return Material; }

    // --- Setter ---
    void SetPosition(const FVector2& InPosition);
    void SetSize(const FVector2& InSize);
    void SetColor(const FVector4& InColor);
    void SetZOrder(int32 InZOrder);
    void SetMaterial(UMaterial* InMaterial);

private:
    FVector2 Position = FVector2(0.0f, 0.0f);
    FVector2 Size = FVector2(100.0f, 100.0f);
    FVector4 Color = FVector4(1.0f, 1.0f, 1.0f, 1.0f);
    int32 ZOrder = 0;

    FMaterialSlot MaterialSlot;
    UMaterial* Material = nullptr;
};