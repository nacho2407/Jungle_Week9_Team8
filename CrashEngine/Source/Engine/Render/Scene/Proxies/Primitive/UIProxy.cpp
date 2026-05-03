#include "UIProxy.h"
#include "Component/UIImageComponent.h"
#include "Render/Resources/Shaders/ShaderManager.h"
#include "Materials/Material.h"
#include "Texture/Texture2D.h"
#include "Render/Execute/Registry/RenderPassTypes.h"
#include "Render/Resources/Bindings/RenderBindingSlots.h"
// 엔진 환경에 맞게 쿼드 메쉬나 텍스처를 가져오는 헤더 인클루드 필요
// #include "Engine/Runtime/Engine.h"

FUIProxy::FUIProxy(UUIImageComponent* InComponent)
    : FPrimitiveProxy(InComponent)
{
    bSupportsOutline = false; 
    bShowAABB = false;        
    bNeverCull = true; 
    bAllowViewModeShaderOverride = false;
    bPerViewportUpdate = true;
}

void FUIProxy::UpdateMesh()
{
    Pass = ERenderPass::UI;
    Blend = EBlendState::AlphaBlend;
    DepthStencil = EDepthStencilState::NoDepth;
    Rasterizer = ERasterizerState::SolidNoCull;
    MeshBuffer = &FMeshBufferManager::Get().GetMeshBuffer(EPrimitiveMeshShape::TexturedQuad);
    Shader = FShaderManager::Get().GetShader(EShaderType::Billboard);

    UpdateMaterial();
}

void FUIProxy::UpdateMaterial()
{
    UUIImageComponent* UIComp = static_cast<UUIImageComponent*>(Owner);

    DiffuseSRV = nullptr;
    MaterialCB[0] = nullptr;
    MaterialCB[1] = nullptr;

    if (UIComp && UIComp->GetMaterial())
    {
        UMaterial* Mat = UIComp->GetMaterial();

        MaterialCB[0] = Mat->GetGPUBufferBySlot(ECBSlot::PerShader0);
        MaterialCB[1] = Mat->GetGPUBufferBySlot(ECBSlot::PerShader1);

        UTexture2D* DiffuseTex = Mat->GetDiffuseTexture();
        if (DiffuseTex)
        {
            DiffuseSRV = DiffuseTex->GetSRV();
        }
    }
}

void FUIProxy::UpdateTransform()
{
    UUIImageComponent* UIComp = static_cast<UUIImageComponent*>(Owner);
    if (!UIComp) return;

    FMatrix ScaleMat = FMatrix::MakeScaleMatrix(FVector(1.0f, UIComp->GetSize().X, UIComp->GetSize().Y));

    FMatrix RotMat = FMatrix::MakeRotationY(90.0f);

    float PosX = UIComp->GetPosition().X + (UIComp->GetSize().X * 0.5f);
    float PosY = UIComp->GetPosition().Y + (UIComp->GetSize().Y * 0.5f);

    FMatrix TransMat = FMatrix::MakeTranslationMatrix(FVector(PosX, PosY, 0.5f));

    PerObjectConstants = FPerObjectCBData::FromWorldMatrix(ScaleMat * RotMat * TransMat);

    CachedWorldPos = FVector(UIComp->GetPosition().X, UIComp->GetPosition().Y, 0.0f);
    CachedBounds = UIComp->GetWorldBoundingBox();

    MarkPerObjectCBDirty();
}