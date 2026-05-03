#pragma once

#include "Render/Execute/Passes/Base/MeshPassBase.h"

struct FRenderPipelineContext;
class FPrimitiveProxy;

class FUIPass : public FMeshPassBase
{
public:
    void PrepareTargets(FRenderPipelineContext& Context) override;
    void BuildDrawCommands(FRenderPipelineContext& Context) override { (void)Context; }
    void BuildDrawCommands(FRenderPipelineContext& Context, const FPrimitiveProxy& Proxy) override;
    void SubmitDrawCommands(FRenderPipelineContext& Context) override;
};