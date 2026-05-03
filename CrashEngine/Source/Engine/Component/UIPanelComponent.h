#pragma once

#include "Component/PrimitiveComponent.h"
#include "Math/Vector.h"

class UPrimitiveComponent;

struct FBox2D
{
    FVector2 Min;
    FVector2 Max;

    FBox2D() : Min(1e6f, 1e6f), Max(-1e6f, -1e6f) {}
    FBox2D(FVector2 InMin, FVector2 InMax) : Min(InMin), Max(InMax) {}

    void operator+=(const FBox2D& Other)
    {
        Min.X = (Min.X < Other.Min.X) ? Min.X : Other.Min.X;
        Min.Y = (Min.Y < Other.Min.Y) ? Min.Y : Other.Min.Y;
        Max.X = (Max.X > Other.Max.X) ? Max.X : Other.Max.X;
        Max.Y = (Max.Y > Other.Max.Y) ? Max.Y : Other.Max.Y;
    }

    bool IsInside(FVector2 P) const
    {
        return (P.X >= Min.X && P.X <= Max.X && P.Y >= Min.Y && P.Y <= Max.Y);
    }
};

class UIPanelComponent : public UPrimitiveComponent
{
public:
    DECLARE_CLASS(UIPanelComponent, UPrimitiveComponent)

    UIPanelComponent();

    virtual void UpdateWorldAABB() const override;

    void SetHovered(bool bInHovered);
    const FBox2D& GetBounds() const { return CachedBounds; }

private:
    mutable FBox2D CachedBounds; 
    bool bIsHovered = false;
};