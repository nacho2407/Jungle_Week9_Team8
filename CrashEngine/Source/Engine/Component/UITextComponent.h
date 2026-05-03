#pragma once

#include "UIComponent.h"
#include "Render/Resources/Buffers/MeshBufferManager.h"
#include "Resource/ResourceManager.h"

class UUITextComponent : public UUIComponent
{
public:
    DECLARE_CLASS(UUITextComponent, UUIComponent)

    UUITextComponent();
    virtual ~UUITextComponent() override = default;

    // --- UI 인터페이스 ---
    virtual FBox2D GetUIBounds() const override;

    virtual FPrimitiveProxy* CreateSceneProxy() override;

    virtual void GetEditableProperties(TArray<FPropertyDescriptor>& OutProps) override;
    virtual void PostEditProperty(const char* PropertyName) override;
    virtual void Serialize(FArchive& Ar) override;

    virtual FMeshBuffer* GetMeshBuffer() const override;
    virtual FMeshDataView GetMeshDataView() const override;

    bool ReacquireDefaultFont();

    const FString& GetText() const { return Text; }
    const FName& GetFontName() const { return FontName; }
    float GetFontSize() const { return FontSize; }
    const FFontResource* GetFont() const { return CachedFont; }

    // --- Setter ---
    void SetText(const FString& InText);
    void SetFont(const FName& InFontName);
    void SetFontSize(float InSize);

private:
    FString Text;
    FName FontName = FName("Default");
    FFontResource* CachedFont = nullptr;

    float FontSize = 1.0f;
};
