#include "UIPass.h"

#include "Render/Execute/Context/RenderPipelineContext.h"
#include "Render/Execute/Context/Scene/SceneView.h"
#include "Render/Resources/Bindings/RenderBindingSlots.h"
#include "Render/Resources/Buffers/ConstantBufferData.h"
#include "Render/Resources/FrameResources.h"
#include "Render/Submission/Command/BuildDrawCommand.h"
#include "Render/Submission/Command/DrawCommandList.h"


void FUIPass::PrepareTargets(FRenderPipelineContext& Context)
{
    ID3D11RenderTargetView* RTV = Context.GetViewportRTV();

    Context.Context->OMSetRenderTargets(1, &RTV, nullptr);
}

void FUIPass::BuildDrawCommands(FRenderPipelineContext& Context, const FPrimitiveProxy& Proxy)
{
    DrawCommandBuild::BuildMeshDrawCommand(Proxy, ERenderPass::UI, Context, *Context.DrawCommandList);
}

void FUIPass::SubmitDrawCommands(FRenderPipelineContext& Context)
{
    if (!Context.DrawCommandList)
    {
        return;
    }

    uint32 GlobalStart = 0;
    uint32 GlobalEnd = 0;
    Context.DrawCommandList->GetPassRange(ERenderPass::UI, GlobalStart, GlobalEnd);
    if (GlobalStart >= GlobalEnd)
    {
        return;
    }

    float Width = Context.SceneView ? Context.SceneView->ViewportWidth : 1920.0f;
    float Height = Context.SceneView ? Context.SceneView->ViewportHeight : 1080.0f;

    FMatrix OrthoProj = FMatrix::MakeOrthographicOffCenter(0.0f, Width, Height, 0.0f, 0.0f, 1.0f);

    FFrameCBData UIFrameData = {};
    UIFrameData.View = FMatrix::Identity;
    UIFrameData.Projection = OrthoProj;
    UIFrameData.InvViewProj = (UIFrameData.View * UIFrameData.Projection).GetInverse();

    Context.Resources->FrameBuffer.Update(Context.Context, &UIFrameData, sizeof(FFrameCBData));

    ID3D11Buffer* FrameCB = Context.Resources->FrameBuffer.GetBuffer();
    Context.Context->VSSetConstantBuffers(ECBSlot::Frame, 1, &FrameCB);
    Context.Context->PSSetConstantBuffers(ECBSlot::Frame, 1, &FrameCB);

    SubmitPassRange(Context, ERenderPass::UI);

    if (Context.SceneView)
    {
        FFrameCBData MainFrameData = {};
        MainFrameData.View = Context.SceneView->View;
        MainFrameData.Projection = Context.SceneView->Proj;
        MainFrameData.InvViewProj = (Context.SceneView->View * Context.SceneView->Proj).GetInverse();
        MainFrameData.CameraWorldPos = Context.SceneView->CameraPosition;
        MainFrameData.bIsWireframe = (Context.SceneView->ViewMode == EViewMode::Wireframe);
        MainFrameData.WireframeColor = Context.SceneView->WireframeColor;

        Context.Resources->FrameBuffer.Update(Context.Context, &MainFrameData, sizeof(FFrameCBData));
    }
}