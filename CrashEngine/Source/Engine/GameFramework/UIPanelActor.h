// 게임 프레임워크 영역에서 공유되는 타입과 인터페이스를 정의합니다.
#pragma once

#include "GameFramework/AActor.h"

class UUIPanelComponent;

// AUIPanelActor는 UI 패널 계층의 루트로 사용할 수 있는 액터입니다.
class AUIPanelActor : public AActor
{
public:
    DECLARE_CLASS(AUIPanelActor, AActor)

    AUIPanelActor();

    void InitDefaultComponents();

    UUIPanelComponent* GetUIPanelComponent() const { return UIPanelComponent; }

private:
    UUIPanelComponent* UIPanelComponent = nullptr;
};
