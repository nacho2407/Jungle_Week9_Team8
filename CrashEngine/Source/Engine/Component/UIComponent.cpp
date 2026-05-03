#include "UIComponent.h"

IMPLEMENT_ABSTRACT_COMPONENT_CLASS(UUIComponent, UPrimitiveComponent)

UUIComponent::UUIComponent()
{
    bBlockComponent = false;
    bGenerateOverlapEvents = false;
}

void UUIComponent::UpdateWorldAABB() const
{
    FBox2D Bounds = GetUIBounds();
    WorldAABBMinLocation = FVector(Bounds.Min.X, Bounds.Min.Y, 0.0f);
    WorldAABBMaxLocation = FVector(Bounds.Max.X, Bounds.Max.Y, 0.1f);
}

void UUIComponent::SetHovered(bool bInHovered)
{
    if (bIsHovered != bInHovered)
    {
        bIsHovered = bInHovered;
        OnHoverChanged.BroadCast(bIsHovered);
    }
}

void UUIComponent::SetPosition(const FVector2& InPos)
{
    Position = InPos;
    MarkRenderTransformDirty();
    MarkWorldBoundsDirty();
}

void UUIComponent::SetSize(const FVector2& InSize)
{
    Size = InSize;
    MarkRenderTransformDirty();
    MarkWorldBoundsDirty();
}

void UUIComponent::SetColor(const FVector4& InColor)
{
    Color = InColor;
    MarkRenderTransformDirty();
}

void UUIComponent::SetZOrder(int32 InZOrder)
{
    ZOrder = InZOrder;
    MarkRenderTransformDirty();
}
