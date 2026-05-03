#include "UITextComponent.h"
#include "Render/Scene/Proxies/Primitive/UITextProxy.h"
#include "Serialization/Archive.h"
#include "Runtime/Engine.h"

IMPLEMENT_COMPONENT_CLASS(UUITextComponent, UUIComponent, EEditorComponentCategory::Visual)

UUITextComponent::UUITextComponent()
{
    ReacquireDefaultFont();
}

FBox2D UUITextComponent::GetUIBounds() const
{
    // 텍스트 길이에 따른 대략적인 영역 계산 (UIProxy의 로직과 유사하게 맞추는 것이 좋음)
    float Width = Text.length() * FontSize * 10.0f;
    float Height = FontSize * 10.0f;
    return FBox2D(Position, Position + FVector2(Width, Height));
}

FPrimitiveProxy* UUITextComponent::CreateSceneProxy()
{
    return new FUITextProxy(this);
}

bool UUITextComponent::ReacquireDefaultFont()
{
    CachedFont = FResourceManager::Get().FindFont(FontName);
    if (CachedFont && CachedFont->IsLoaded())
    {
        return true;
    }

    FontName = FName("Default");
    CachedFont = FResourceManager::Get().FindFont(FontName);
    return CachedFont && CachedFont->IsLoaded();
}

void UUITextComponent::SetText(const FString& InText)
{
    if (Text == InText) return;
    Text = InText;
    MarkProxyDirty(ESceneProxyDirtyFlag::Transform);
    MarkWorldBoundsDirty();
}

void UUITextComponent::SetFont(const FName& InFontName)
{
    if (FontName == InFontName) return;
    FontName = InFontName;
    ReacquireDefaultFont();
    MarkProxyDirty(ESceneProxyDirtyFlag::Mesh);
}

void UUITextComponent::SetFontSize(float InSize)
{
    FontSize = InSize;
    MarkRenderTransformDirty();
    MarkWorldBoundsDirty();
}

void UUITextComponent::GetEditableProperties(TArray<FPropertyDescriptor>& OutProps)
{
    UUIComponent::GetEditableProperties(OutProps);
    OutProps.push_back({ "UI Text", EPropertyType::String, &Text });
    OutProps.push_back({ "Font", EPropertyType::Name, &FontName });
    OutProps.push_back({ "UI Position", EPropertyType::Vec2, &Position });
    OutProps.push_back({ "Font Size", EPropertyType::Float, &FontSize, 0.1f, 100.0f, 0.1f });
    OutProps.push_back({ "Z-Order", EPropertyType::Int,  &ZOrder });
    OutProps.push_back({ "Color", EPropertyType::Color4,  &Color });
}

void UUITextComponent::PostEditProperty(const char* PropertyName)
{
    UUIComponent::PostEditProperty(PropertyName);

    if (strcmp(PropertyName, "UI Text") == 0 ||
        strcmp(PropertyName, "UI Position") == 0 ||
        strcmp(PropertyName, "Font Size") == 0 ||
        strcmp(PropertyName, "Z-Order") == 0)
    {
        MarkRenderTransformDirty();
        MarkWorldBoundsDirty();
    }
    else if (strcmp(PropertyName, "Color") == 0)
    {
        MarkRenderTransformDirty();
    }
    else if (strcmp(PropertyName, "Font") == 0)
    {
        ReacquireDefaultFont();
        MarkProxyDirty(ESceneProxyDirtyFlag::Mesh);
    }
}

void UUITextComponent::Serialize(FArchive& Ar)
{
    UUIComponent::Serialize(Ar);
    Ar << Text;
    Ar << FontName;
    Ar << Position.X << Position.Y;
    Ar << FontSize;
    Ar << Color.X << Color.Y << Color.Z << Color.W;
    Ar << ZOrder;

    if (Ar.IsLoading())
    {
        ReacquireDefaultFont();
    }
}

FMeshBuffer* UUITextComponent::GetMeshBuffer() const
{
    return &FMeshBufferManager::Get().GetMeshBuffer(EPrimitiveMeshShape::Quad);
}

FMeshDataView UUITextComponent::GetMeshDataView() const
{
    return FMeshDataView::FromMeshData(FMeshBufferManager::Get().GetMeshData(EPrimitiveMeshShape::Quad));
}
