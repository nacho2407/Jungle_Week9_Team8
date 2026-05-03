// 게임 프레임워크 영역의 세부 동작을 구현합니다.
#include "GameFramework/UIPanelActor.h"

#include "Component/UIPanelComponent.h"

IMPLEMENT_CLASS(AUIPanelActor, AActor)

AUIPanelActor::AUIPanelActor()
{
    bNeedsTick = false;
    bTickInEditor = true;
}

void AUIPanelActor::InitDefaultComponents()
{
    UIPanelComponent = AddComponent<UUIPanelComponent>();
    if (!UIPanelComponent)
    {
        return;
    }

    SetRootComponent(UIPanelComponent);

    // 기본 크기/위치를 둬서 생성 직후에도 Hover 영역이 바로 생기도록 설정.
    UIPanelComponent->SetPosition(FVector2(760.0f, 390.0f));
    UIPanelComponent->SetSize(FVector2(400.0f, 300.0f));
    UIPanelComponent->SetZOrder(0);
}
