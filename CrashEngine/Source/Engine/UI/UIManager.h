#pragma once

#include <limits>

#include "Component/UIComponent.h"
#include "Engine/Input/InputSystem.h"
#include "Engine/Runtime/WindowsWindow.h"
#include "GameFramework/AActor.h"
#include "GameFramework/World.h"
#include "GameFramework/WorldContext.h"
#include "UI/SWindow.h"
#include "Viewport/Viewport.h"

class FUIManager
{
public:
    static void UpdateHoverFromFullViewport(
        UWorld* World,
        const FWindowsWindow* Window,
        const InputSystem& Input,
        const FViewport* Viewport)
    {
        const bool bWindowForeground = Window && Window->IsForeground();
        HWND WindowHandle = Window ? Window->GetHWND() : nullptr;
        UpdateHoverFromFullViewport(World, bWindowForeground, WindowHandle, Input, Viewport);
    }

    static void UpdatePIEHoverFromViewport(
        const TArray<FWorldContext>& WorldList,
        bool bHasPIESession,
        const FWindowsWindow* Window,
        const InputSystem& Input,
        const FViewport* Viewport,
        const FRect& ViewportRect)
    {
        UWorld* PIEWorld = nullptr;
        if (bHasPIESession)
        {
            for (const FWorldContext& Context : WorldList)
            {
                if (Context.WorldType == EWorldType::PIE && Context.World)
                {
                    PIEWorld = Context.World;
                    break;
                }
            }
        }

        const bool bWindowForeground = Window && Window->IsForeground();
        HWND WindowHandle = Window ? Window->GetHWND() : nullptr;
        UpdateHoverFromViewport(PIEWorld, bWindowForeground, WindowHandle, Input, Viewport, ViewportRect);
    }

    static void UpdateHoverFromViewport(
        UWorld* World,
        bool bWindowForeground,
        HWND WindowHandle,
        const InputSystem& Input,
        const FViewport* Viewport,
        const FRect& ViewportRect)
    {
        if (!World)
        {
            return;
        }

        if (!bWindowForeground || !WindowHandle || !Viewport)
        {
            UpdateWorldHover(World, nullptr);
            return;
        }

        POINT MouseClientPos{};
        if (!Input.TryGetMouseClientPos(WindowHandle, MouseClientPos))
        {
            UpdateWorldHover(World, nullptr);
            return;
        }

        FVector2 MouseCanvasPos{};
        if (!TryProjectMouseToCanvas(MouseClientPos, ViewportRect, Viewport, MouseCanvasPos))
        {
            UpdateWorldHover(World, nullptr);
            return;
        }

        UpdateWorldHover(World, &MouseCanvasPos);
    }

    static void UpdateHoverFromFullViewport(
        UWorld* World,
        bool bWindowForeground,
        HWND WindowHandle,
        const InputSystem& Input,
        const FViewport* Viewport)
    {
        FRect ViewportRect{};
        if (Viewport)
        {
            ViewportRect.X = 0.0f;
            ViewportRect.Y = 0.0f;
            ViewportRect.Width = static_cast<float>(Viewport->GetWidth());
            ViewportRect.Height = static_cast<float>(Viewport->GetHeight());
        }

        UpdateHoverFromViewport(World, bWindowForeground, WindowHandle, Input, Viewport, ViewportRect);
    }

    static bool TryProjectMouseToCanvas(const POINT& MouseClientPos, const FRect& ViewportRect, const FViewport* Viewport, FVector2& OutCanvasPos)
    {
        if (!Viewport || ViewportRect.Width <= 0.0f || ViewportRect.Height <= 0.0f)
        {
            return false;
        }

        const bool bInsideViewportRect =
            MouseClientPos.x >= ViewportRect.X &&
            MouseClientPos.y >= ViewportRect.Y &&
            MouseClientPos.x < ViewportRect.X + ViewportRect.Width &&
            MouseClientPos.y < ViewportRect.Y + ViewportRect.Height;
        if (!bInsideViewportRect)
        {
            return false;
        }

        const float ViewportWidth = static_cast<float>(Viewport->GetWidth());
        const float ViewportHeight = static_cast<float>(Viewport->GetHeight());
        if (ViewportWidth <= 0.0f || ViewportHeight <= 0.0f)
        {
            return false;
        }

        const float LocalX = static_cast<float>(MouseClientPos.x) - ViewportRect.X;
        const float LocalY = static_cast<float>(MouseClientPos.y) - ViewportRect.Y;

        OutCanvasPos.X = (LocalX / ViewportWidth) * UUIComponent::CanvasWidth;
        OutCanvasPos.Y = (LocalY / ViewportHeight) * UUIComponent::CanvasHeight;
        return true;
    }

    static void UpdateWorldHover(UWorld* World, const FVector2* MouseCanvasPos)
    {
        if (!World)
        {
            return;
        }

        TArray<UUIComponent*> RootUIComponents;
        for (AActor* Actor : World->GetActors())
        {
            if (!Actor || !Actor->IsVisible())
            {
                continue;
            }

            for (UPrimitiveComponent* PrimitiveComponent : Actor->GetPrimitiveComponents())
            {
                UUIComponent* UIComponent = dynamic_cast<UUIComponent*>(PrimitiveComponent);
                if (!UIComponent || !UIComponent->ShouldRenderInCurrentWorld() || !IsUIRootComponent(UIComponent))
                {
                    continue;
                }

                RootUIComponents.push_back(UIComponent);
            }
        }

        UUIComponent* HoveredRoot = nullptr;
        if (MouseCanvasPos)
        {
            int32 BestZOrder = (std::numeric_limits<int32>::min)();
            int32 BestSerial = (std::numeric_limits<int32>::min)();
            int32 Serial = 0;

            for (UUIComponent* UIComponent : RootUIComponents)
            {
                if (!UIComponent->HitTest(*MouseCanvasPos))
                {
                    ++Serial;
                    continue;
                }

                const int32 ZOrder = UIComponent->GetZOrder();
                if (!HoveredRoot || ZOrder > BestZOrder || (ZOrder == BestZOrder && Serial > BestSerial))
                {
                    HoveredRoot = UIComponent;
                    BestZOrder = ZOrder;
                    BestSerial = Serial;
                }

                ++Serial;
            }
        }

        for (UUIComponent* UIComponent : RootUIComponents)
        {
            UIComponent->SetHovered(UIComponent == HoveredRoot);
        }
    }

private:
    static bool IsUIRootComponent(const UUIComponent* UIComponent)
    {
        if (!UIComponent) return false;

        UUIComponent* MutableUI = const_cast<UUIComponent*>(UIComponent);
        const USceneComponent* Parent = MutableUI->GetParent();

        return dynamic_cast<const UUIComponent*>(Parent) == nullptr;
    }
};
