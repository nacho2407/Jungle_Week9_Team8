#include "UIPanelComponent.h"

UIPanelComponent::UIPanelComponent()
{
    bIsVisible = false;
}

void UIPanelComponent::UpdateWorldAABB() const
{
    FBox2D NewBounds;

    for (USceneComponent* Child : ChildComponents)
    {
        if (auto* UIComp = dynamic_cast<UPrimitiveComponent*>(Child))
        {
            // 각 컴포넌트가 가진 고유의 영역을 가져와 합칩니다.
            // (이미지/텍스트 컴포넌트에 이 함수가 있다고 가정)
            // FBox2D ChildBox = UIComp->GetUIBounds(); 
            // NewBounds += ChildBox;
        }
    }

    CachedBounds = NewBounds;

    Super::UpdateWorldAABB();
}

void UIPanelComponent::SetHovered(bool bInHovered)
{
    if (bIsHovered == bInHovered) return;
    bIsHovered = bInHovered;

    for (USceneComponent* Child : ChildComponents)
    {
    }

}