#include "UIComponent.h"

#include "Core/Logging/LogMacros.h"
#include "GameFramework/AActor.h"

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

bool UUIComponent::HitTest(const FVector2& Point) const
{
    return GetUIBounds().IsInside(Point);
}

void UUIComponent::SetHovered(bool bInHovered)
{
    if (bIsHovered != bInHovered)
    {
        bIsHovered = bInHovered;

        const AActor* OwnerActor = GetOwner();
        const uint32 OwnerUUID = OwnerActor ? OwnerActor->GetUUID() : 0;
        const char* ClassName = GetClass() ? GetClass()->GetName() : "UnknownUIComponent";
        UE_LOG(UI, Debug, "Hover %s - Component=%s OwnerUUID=%u ZOrder=%d",
            bIsHovered ? "Enter" : "Leave",
            ClassName,
            OwnerUUID,
            GetZOrder());

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
