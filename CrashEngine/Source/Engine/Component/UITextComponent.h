#pragma once

#include "Component/PrimitiveComponent.h"
#include "Math/Vector.h" 
#include "Render/Resources/Buffers/MeshBufferManager.h"
#include "Resource/ResourceManager.h"

class UUITextComponent : public UPrimitiveComponent
{
public:
    DECLARE_CLASS(UUITextComponent, UPrimitiveComponent)

    UUITextComponent();
    virtual ~UUITextComponent() override = default;

    virtual FPrimitiveProxy* CreateSceneProxy() override;
    virtual void UpdateWorldAABB() const override;

    virtual void GetEditableProperties(TArray<FPropertyDescriptor>& OutProps) override;
    virtual void PostEditProperty(const char* PropertyName) override;
    virtual void Serialize(FArchive& Ar) override;

    virtual FMeshBuffer* GetMeshBuffer() const override;
    virtual FMeshDataView GetMeshDataView() const override;

    bool ReacquireDefaultFont();

    const FString& GetText() const { return Text; }
    const FName& GetFontName() const { return FontName; }
    FVector2 GetPosition() const { return Position; }
    float GetFontSize() const { return FontSize; }
    const FFontResource* GetFont() const { return CachedFont; }
    FVector4 GetColor() const { return Color; }
    int32 GetZOrder() const { return ZOrder; }

    // --- Setter ---
    void SetText(const FString& InText);
    void SetFont(const FName& InFontName);
    void SetPosition(const FVector2& InPosition);
    void SetFontSize(float InSize);
    void SetColor(const FVector4& InColor);
    void SetZOrder(int32 InZOrder);

private:
    FString Text;
    FName FontName = FName("Default");
    FFontResource* CachedFont = nullptr;

    FVector2 Position = FVector2(0.0f, 0.0f);
    float FontSize = 1.0f;
    FVector4 Color = FVector4(1.0f, 1.0f, 1.0f, 1.0f);
    int32 ZOrder = 0;
};