#include "UIPass.h"

#include "Render/Execute/Context/RenderPipelineContext.h"
#include "Render/Execute/Context/Scene/SceneView.h"
#include "Render/Scene/Proxies/Primitive/PrimitiveProxy.h"
#include "Render/Resources/Bindings/RenderBindingSlots.h"
#include "Render/Resources/Buffers/ConstantBufferData.h"
#include "Render/Resources/FrameResources.h"
#include "Render/Submission/Command/BuildDrawCommand.h"
#include "Render/Submission/Command/DrawCommandList.h"

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
        DrawCommandBuild::BuildMeshDrawCommand(Proxy, ERenderPass::UI, Context, *Context.DrawCommandList);
    }
}

void FUIPass::SubmitDrawCommands(FRenderPipelineContext& Context)
{
    DrawCommandBuild::BuildBatchedUITextDrawCommands(Context, *Context.DrawCommandList);

    uint32 GlobalStart = 0, GlobalEnd = 0;
    Context.DrawCommandList->GetPassRange(ERenderPass::UI, GlobalStart, GlobalEnd);

    if (GlobalStart >= GlobalEnd) return;

    ID3D11RenderTargetView* RTV = Context.GetViewportRTV();
    ID3D11DepthStencilView* DSV = Context.GetViewportDSV();
    Context.Context->OMSetRenderTargets(1, &RTV, DSV);

    SubmitPassRange(Context, ERenderPass::UI);
}