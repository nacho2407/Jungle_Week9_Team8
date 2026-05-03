#include "UIPass.h"

#include <algorithm>

#include "Component/UIComponent.h"
#include "Render/Execute/Context/RenderPipelineContext.h"
#include "Render/Execute/Context/Scene/SceneView.h"
#include "Render/Scene/Proxies/Primitive/PrimitiveProxy.h"
#include "Render/Resources/Bindings/RenderBindingSlots.h"
#include "Render/Resources/Buffers/ConstantBufferData.h"
#include "Render/Resources/FrameResources.h"
#include "Render/Submission/Command/BuildDrawCommand.h"
#include "Render/Submission/Command/DrawCommandList.h"

namespace
{
uint8 EncodeUIZOrderBits(int32 ZOrder)
{
    const int32 BiasedZOrder = std::clamp(ZOrder + 128, 0, 255);
    return static_cast<uint8>(BiasedZOrder);
}
} // namespace

void FUIPass::PrepareTargets(FRenderPipelineContext& Context)
{
    ID3D11RenderTargetView* RTV = Context.GetViewportRTV();
    ID3D11DepthStencilView* DSV = Context.StateCache->DSV;
    Context.Context->OMSetRenderTargets(1, &RTV, DSV);
}

void FUIPass::BuildDrawCommands(FRenderPipelineContext& Context, const FPrimitiveProxy& Proxy)
{
    if (!Proxy.bFontBatched)
    {
        uint8 UserBits = 0;
        if (const UUIComponent* UIComponent = dynamic_cast<const UUIComponent*>(Proxy.Owner))
        {
            UserBits = EncodeUIZOrderBits(UIComponent->GetZOrder());
        }

        DrawCommandBuild::BuildMeshDrawCommand(Proxy, ERenderPass::UI, Context, *Context.DrawCommandList, UserBits);
    }
}

void FUIPass::SubmitDrawCommands(FRenderPipelineContext& Context)
{
    uint32 GlobalStart = 0, GlobalEnd = 0;
    Context.DrawCommandList->GetPassRange(ERenderPass::UI, GlobalStart, GlobalEnd);

    if (GlobalStart >= GlobalEnd)
    {
        return;
    }

    ID3D11RenderTargetView* RTV = Context.GetViewportRTV();
    ID3D11DepthStencilView* DSV = Context.GetViewportDSV();
    Context.Context->OMSetRenderTargets(1, &RTV, DSV);

    SubmitPassRange(Context, ERenderPass::UI);
}
