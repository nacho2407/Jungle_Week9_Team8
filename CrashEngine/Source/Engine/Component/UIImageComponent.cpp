#include "UIImageComponent.h"
#include "Serialization/Archive.h"
#include <cmath>
#include "Materials/Material.h"
#include "Materials/MaterialManager.h"
#include "Render/Scene/Proxies/Primitive/UIProxy.h"
#include "Runtime/Engine.h"
#include "Texture/Texture2D.h"

IMPLEMENT_COMPONENT_CLASS(UUIImageComponent, UPrimitiveComponent, EEditorComponentCategory::Visual)

UUIImageComponent::UUIImageComponent()
{
    bBlockComponent = false;
    bGenerateOverlapEvents = false;
}

FPrimitiveProxy* UUIImageComponent::CreateSceneProxy()
{
    return new FUIProxy(this);
}

void UUIImageComponent::UpdateWorldAABB() const
{
    FVector Min(Position.X, Position.Y, 0.0f);
    FVector Max(Position.X + Size.X, Position.Y + Size.Y, 0.1f);

    WorldAABBMinLocation = Min;
    WorldAABBMaxLocation = Max;
}

void UUIImageComponent::SetPosition(const FVector2& InPosition)
{
    Position = InPosition;
    MarkRenderTransformDirty();
    MarkWorldBoundsDirty();
}

void UUIImageComponent::SetSize(const FVector2& InSize)
{
    Size = InSize;
    MarkRenderTransformDirty();
    MarkWorldBoundsDirty();
}

void UUIImageComponent::SetColor(const FVector4& InColor)
{
    Color = InColor;
    MarkRenderTransformDirty();
}

void UUIImageComponent::SetZOrder(int32 InZOrder)
{
    ZOrder = InZOrder;
    MarkRenderTransformDirty();
}

void UUIImageComponent::SetMaterial(UMaterial* InMaterial)
{
    Material = InMaterial;
    if (Material)
    {
        MaterialSlot.Path = Material->GetAssetPathFileName();
    }
    else
    {
        MaterialSlot.Path = "None";
    }
    MarkProxyDirty(ESceneProxyDirtyFlag::Material);
}

void UUIImageComponent::GetEditableProperties(TArray<FPropertyDescriptor>& OutProps)
{
    UPrimitiveComponent::GetEditableProperties(OutProps);
    OutProps.push_back({ "UI Position", EPropertyType::Vec2, &Position });
    OutProps.push_back({ "UI Size",     EPropertyType::Vec2, &Size });
    OutProps.push_back({ "Z-Order",     EPropertyType::Int,  &ZOrder });
    OutProps.push_back({ "Color",     EPropertyType::Color4,  &Color });
    OutProps.push_back({ "Material", EPropertyType::MaterialSlot, &MaterialSlot });
}

void UUIImageComponent::PostEditProperty(const char* PropertyName)
{
    UPrimitiveComponent::PostEditProperty(PropertyName);

    if (strcmp(PropertyName, "UI Position") == 0 ||
        strcmp(PropertyName, "UI Size") == 0 ||
        strcmp(PropertyName, "Z-Order") == 0)
    {
        MarkRenderTransformDirty();
        MarkWorldBoundsDirty();
    }
    else if (strcmp(PropertyName, "Color") == 0)
    {
        MarkRenderTransformDirty();
    }
    else if (strcmp(PropertyName, "Material") == 0)
    {
        if (MaterialSlot.Path == "None" || MaterialSlot.Path.empty())
        {
            SetMaterial(nullptr);
        }
        else
        {
            UMaterial* LoadedMat = FMaterialManager::Get().GetOrCreateMaterial(MaterialSlot.Path);
            if (LoadedMat)
            {
                SetMaterial(LoadedMat);
            }
        }
    }
}

void UUIImageComponent::Serialize(FArchive& Ar)
{
    UPrimitiveComponent::Serialize(Ar);
    Ar << Position.X << Position.Y;
    Ar << Size.X << Size.Y;
    Ar << Color.X << Color.Y << Color.Z << Color.W;
    Ar << ZOrder;
    Ar << MaterialSlot.Path;
}

FMeshBuffer* UUIImageComponent::GetMeshBuffer() const
{
    return &FMeshBufferManager::Get().GetMeshBuffer(EPrimitiveMeshShape::Quad);
}

FMeshDataView UUIImageComponent::GetMeshDataView() const
{
    return FMeshDataView::FromMeshData(FMeshBufferManager::Get().GetMeshData(EPrimitiveMeshShape::Quad));
}