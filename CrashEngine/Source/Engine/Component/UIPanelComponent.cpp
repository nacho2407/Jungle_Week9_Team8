#include "UIPanelComponent.h"

IMPLEMENT_COMPONENT_CLASS(UUIPanelComponent, UUIComponent, EEditorComponentCategory::Visual)

UUIPanelComponent::UUIPanelComponent()
{
}

FBox2D UUIPanelComponent::GetUIBounds() const
{
    FBox2D CombinedBounds;
    for (USceneComponent* Child : ChildComponents)
    {
        if (auto* UIComp = dynamic_cast<UUIComponent*>(Child))
        {
            CombinedBounds += UIComp->GetUIBounds();
        }
    }
    return CombinedBounds;
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
