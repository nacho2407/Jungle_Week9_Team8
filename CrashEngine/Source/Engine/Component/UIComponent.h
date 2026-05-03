#pragma once

#include "Component/PrimitiveComponent.h"
#include "Math/Vector.h"
#include <algorithm>

struct FBox2D
{
    FVector2 Min;
    FVector2 Max;

    FBox2D() : Min(1e6f, 1e6f), Max(-1e6f, -1e6f) {}
    FBox2D(FVector2 InMin, FVector2 InMax) : Min(InMin), Max(InMax) {}

    void operator+=(const FBox2D& Other)
    {
        Min.X = (std::min)(Min.X, Other.Min.X);
        Min.Y = (std::min)(Min.Y, Other.Min.Y);
        Max.X = (std::max)(Max.X, Other.Max.X);
        Max.Y = (std::max)(Max.Y, Other.Max.Y);
    }

    bool IsInside(FVector2 P) const
    {
        return (P.X >= Min.X && P.X <= Max.X && P.Y >= Min.Y && P.Y <= Max.Y);
    }
};

class UUIComponent : public UPrimitiveComponent
{
public:
    DECLARE_CLASS(UUIComponent, UPrimitiveComponent)

    UUIComponent();
    virtual ~UUIComponent() override = default;

    virtual FBox2D GetUIBounds() const { return FBox2D(); }
    virtual void UpdateWorldAABB() const override;

    virtual void SetHovered(bool bInHovered);
    bool IsHovered() const { return bIsHovered; }

    FVector2 GetPosition() const { return Position; }
    virtual void SetPosition(const FVector2& InPos);

    FVector2 GetSize() const { return Size; }
    virtual void SetSize(const FVector2& InSize);

    FVector4 GetColor() const { return Color; }
    virtual void SetColor(const FVector4& InColor);

    int32 GetZOrder() const { return ZOrder; }
    virtual void SetZOrder(int32 InZOrder);

    DECLARE_DELEGATE(FOnHoverChanged, bool);
    FOnHoverChanged OnHoverChanged;

protected:
    FVector2 Position = FVector2(0, 0);
    FVector2 Size = FVector2(100, 100);
    FVector4 Color = FVector4(1, 1, 1, 1);
    int32 ZOrder = 0;
    bool bIsHovered = false;
};
