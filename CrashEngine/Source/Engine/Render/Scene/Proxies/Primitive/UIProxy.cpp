#include "UIProxy.h"
#include "Component/UIImageComponent.h"
#include "Render/Resources/Shaders/ShaderManager.h"
#include "Materials/Material.h"
#include "Texture/Texture2D.h"
#include "Render/Execute/Registry/RenderPassTypes.h"
#include "Render/Resources/Bindings/RenderBindingSlots.h"

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
    Shader = FShaderManager::Get().GetShader(EShaderType::UIImage);

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
    FMatrix UserRotMat = FMatrix::MakeRotationX(UIComp->GetRotation());
    FMatrix AlignRotMat = FMatrix::MakeRotationY(90.0f);
    FMatrix TransMat = FMatrix::MakeTranslationMatrix(FVector(
        UIComp->GetPosition().X + (UIComp->GetSize().X * 0.5f),
        UIComp->GetPosition().Y + (UIComp->GetSize().Y * 0.5f),
        0.5f));

    FMatrix WorldMat = ScaleMat * UserRotMat * AlignRotMat * TransMat;

    const float Width = UUIComponent::CanvasWidth;
    const float Height = UUIComponent::CanvasHeight;
    FMatrix OrthoProj = FMatrix::MakeOrthographicOffCenter(0.0f, Width, Height, 0.0f, 0.0f, 1.0f);

    FPerObjectCBData Data = {};
    Data.Model = WorldMat * OrthoProj;
    Data.Color = UIComp->GetColor();

    PerObjectConstants = Data;
    MarkPerObjectCBDirty();
}
