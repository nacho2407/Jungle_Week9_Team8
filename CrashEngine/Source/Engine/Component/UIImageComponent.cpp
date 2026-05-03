#include "UIImageComponent.h"
#include "Serialization/Archive.h"
#include <cmath>
#include "Materials/Material.h"
#include "Materials/MaterialManager.h"
#include "Render/Scene/Proxies/Primitive/UIProxy.h"
#include "Runtime/Engine.h"
#include "Texture/Texture2D.h"

IMPLEMENT_COMPONENT_CLASS(UUIImageComponent, UUIComponent, EEditorComponentCategory::Visual)

UUIImageComponent::UUIImageComponent()
{
}

FPrimitiveProxy* UUIImageComponent::CreateSceneProxy()
{
    return new FUIProxy(this);
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

void UUIImageComponent::SetRotation(float InRotation)
{
    Rotation = InRotation;
    MarkRenderTransformDirty();
}

void UUIImageComponent::GetEditableProperties(TArray<FPropertyDescriptor>& OutProps)
{
    UUIComponent::GetEditableProperties(OutProps);
    OutProps.push_back({ "UI Position", EPropertyType::Vec2, &Position });
    OutProps.push_back({ "UI Size",     EPropertyType::Vec2, &Size });
    OutProps.push_back({ "UI Rotation",    EPropertyType::Float, &Rotation });
    OutProps.push_back({ "Z-Order",     EPropertyType::Int,  &ZOrder });
    OutProps.push_back({ "Color",     EPropertyType::Color4,  &Color });
    OutProps.push_back({ "Material", EPropertyType::MaterialSlot, &MaterialSlot });
}

void UUIImageComponent::PostEditProperty(const char* PropertyName)
{
    UUIComponent::PostEditProperty(PropertyName);

    if (strcmp(PropertyName, "UI Position") == 0 ||
        strcmp(PropertyName, "UI Size") == 0 ||
        strcmp(PropertyName, "Z-Order") == 0 ||
        strcmp(PropertyName, "UI Rotation") == 0)
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
    UUIComponent::Serialize(Ar);
    Ar << Position.X << Position.Y;
    Ar << Size.X << Size.Y;
    Ar << Rotation;
    Ar << Color.X << Color.Y << Color.Z << Color.W;
    Ar << ZOrder;
    Ar << MaterialSlot.Path;
}

FMeshBuffer* UUIImageComponent::GetMeshBuffer() const
{
    return &FMeshBufferManager::Get().GetMeshBuffer(EPrimitiveMeshShape::TexturedQuad);
}

FMeshDataView UUIImageComponent::GetMeshDataView() const
{
    return FMeshDataView::FromMeshData(FMeshBufferManager::Get().GetMeshData(EPrimitiveMeshShape::Quad));
}
