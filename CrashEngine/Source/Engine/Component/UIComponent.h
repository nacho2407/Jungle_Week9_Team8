#pragma once

#include "Component/PrimitiveComponent.h"
#include "Math/Vector.h"
#include "UIPanelComponent.h"

class UUIComponent : public UPrimitiveComponent
{
public:
    DECLARE_CLASS(UUIComponent, UPrimitiveComponent)

    UUIComponent() : Position(0, 0), Color(1, 1, 1, 1), ZOrder(0) {}

    virtual FBox2D GetUIBounds() const = 0;
    virtual void OnHoverChanged(bool bInHovered) { bIsHovered = bInHovered; }

    FVector2 GetPosition() const { return Position; }
    void SetPosition(const FVector2& InPos) { Position = InPos; MarkWorldBoundsDirty(); }

    FVector4 GetColor() const { return Color; }
    void SetColor(const FVector4& InColor) { Color = InColor; MarkRenderTransformDirty(); }

protected:
    FVector2 Position;
    FVector4 Color;
    int32 ZOrder;
    bool bIsHovered = false;
};