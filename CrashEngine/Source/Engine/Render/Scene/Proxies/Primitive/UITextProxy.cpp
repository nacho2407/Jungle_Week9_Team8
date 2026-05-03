#include "UITextProxy.h"
#include "Component/UITextComponent.h"
#include "Render/Resources/Shaders/ShaderManager.h"
#include "Render/Execute/Context/Scene/SceneView.h"
#include "Render/Resources/Buffers/ConstantBufferData.h"

FUITextProxy::FUITextProxy(UUITextComponent* InComponent)
    : FPrimitiveProxy(InComponent)
{
    bSupportsOutline = false;
    bShowAABB = false;
    bNeverCull = true;
    bAllowViewModeShaderOverride = false;
    bPerViewportUpdate = true;
}

void FUITextProxy::UpdateMesh()
{
    Pass = ERenderPass::UI;
    Blend = EBlendState::AlphaBlend;
    DepthStencil = EDepthStencilState::NoDepth;
    Rasterizer = ERasterizerState::SolidNoCull;

    MeshBuffer = Owner ? Owner->GetMeshBuffer() : nullptr;
    Shader = FShaderManager::Get().GetShader(EShaderType::UIText);

    bFontBatched = true;
}

void FUITextProxy::UpdateTransform()
{
    UUITextComponent* UITextComp = static_cast<UUITextComponent*>(Owner);
    if (!UITextComp) return;

    FPerObjectCBData Data = {};
    Data.Model = FMatrix::Identity;
    Data.Color = UITextComp->GetColor();

    PerObjectConstants = Data;
    MarkPerObjectCBDirty();
}

void FUITextProxy::UpdatePerViewport(const FSceneView& SceneView)
{
    UUITextComponent* UITextComp = static_cast<UUITextComponent*>(Owner);
    if (!UITextComp)
    {
        bVisible = false;
        return;
    }

    if (UITextComp->GetText().empty() || !UITextComp->ReacquireDefaultFont())
    {
        bVisible = false;
        return;
    }

    bVisible = UITextComp->ShouldRenderInCurrentWorld();
    if (!bVisible) return;

    CachedText = UITextComp->GetText();
    CachedFontScale = UITextComp->GetFontSize();
    CachedPosition = UITextComp->GetPosition();
    CachedColor = UITextComp->GetColor();

    CachedFontResource = UITextComp->GetFont();
    if (CachedFontResource && CachedFontResource->SRV)
    {
        CachedFontSRV = CachedFontResource->SRV;
    }
    else
    {
        CachedFontSRV = nullptr;
    }
}