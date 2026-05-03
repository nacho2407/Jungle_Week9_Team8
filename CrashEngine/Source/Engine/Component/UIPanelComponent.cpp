#include "UIPanelComponent.h"

IMPLEMENT_COMPONENT_CLASS(UUIPanelComponent, UUIComponent, EEditorComponentCategory::Visual)

UUIPanelComponent::UUIPanelComponent()
{
}

FBox2D UUIPanelComponent::GetUIBounds() const
{
    FBox2D CombinedBounds(Position, Position + Size);
    for (USceneComponent* Child : ChildComponents)
    {
        if (auto* UIComp = dynamic_cast<UUIComponent*>(Child))
        {
            CombinedBounds += UIComp->GetUIBounds();
        }
    }
    return CombinedBounds;
}

bool UUIPanelComponent::HitTest(const FVector2& Point) const
{
    for (USceneComponent* Child : ChildComponents)
    {
        if (auto* UIComp = dynamic_cast<UUIComponent*>(Child))
        {
            if (UIComp->HitTest(Point))
            {
                return true;
            }
        }
    }

    return UUIComponent::HitTest(Point);
}

void UUIPanelComponent::SetHovered(bool bInHovered)
{
    if (bIsHovered == bInHovered) return;
    
    UUIComponent::SetHovered(bInHovered);

    for (USceneComponent* Child : ChildComponents)
    {
        if (auto* UIComp = dynamic_cast<UUIComponent*>(Child))
        {
            UIComp->SetHovered(bInHovered);
        }
    }
}
