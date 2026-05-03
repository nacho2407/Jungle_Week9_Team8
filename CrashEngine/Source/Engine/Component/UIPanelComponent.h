#pragma once

#include "UIComponent.h"

class UUIPanelComponent : public UUIComponent
{
public:
    DECLARE_CLASS(UUIPanelComponent, UUIComponent)

    UUIPanelComponent();

    virtual FBox2D GetUIBounds() const override;
    virtual bool HitTest(const FVector2& Point) const override;

    virtual void SetHovered(bool bInHovered) override;

private:
};
