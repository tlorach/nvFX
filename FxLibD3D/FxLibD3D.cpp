/*
    Copyright (c) 2013, NVIDIA CORPORATION. All rights reserved.
    Copyright (c) 2013, Tristan Lorach. All rights reserved.

    Redistribution and use in source and binary forms, with or without
    modification, are permitted provided that the following conditions
    are met:
     * Redistributions of source code must retain the above copyright
       notice, this list of conditions and the following disclaimer.
     * Neither the name of NVIDIA CORPORATION nor the names of its
       contributors may be used to endorse or promote products derived
       from this software without specific prior written permission.

    THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS ``AS IS'' AND ANY
    EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
    IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
    PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL THE COPYRIGHT OWNER OR
    CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
    EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
    PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
    PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY
    OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
    (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
    OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

    Please direct any questions to tlorach@nvidia.com (Tristan Lorach)
*/
#   define  LOGI(...)  { printf(__VA_ARGS__); printf("\n"); }
#   define  LOGE(...)  { printf(">>Error : "); printf(__VA_ARGS__); printf("\n"); }
#ifdef WIN32
#include <windows.h> // for OutputDebugString
#endif
#include <fstream>
#include <assert.h>
#include "Fx.h"
#include "FxD3D.h"
#include "TexQuad2D.inl"
#pragma warning(disable:4996)
using namespace nvFX;

#ifdef USE_OLDPROGRAM
#error("D3D Cannot run with the USE_OLDPROGRAM define : we need the separate shader design !")
#endif

namespace nvFX
{

Resource* new_Resource(ResourceRepository* pCont, const char* name)
{
    return new ResourceD3D(pCont, name);
}
void      delete_Resource(Resource *p)
{
    delete p;
}
ProgramPipeline*    new_ProgramPipeline(Container *pCont)
{
    return new D3DProgramPipeline(pCont);
}
void                delete_ProgramPipeline(ProgramPipeline *p)
{
    delete p;
}
CstBuffer *new_CstBuffer(const char* name)
{
    return new CstBufferD3D(name);
}
void delete_CstBuffer(CstBuffer *p)
{
    CstBufferD3D *pC = static_cast<CstBufferD3D *>(p);
    delete pC;
}

SamplerState *new_SamplerState(Container *pCont, const char* name)
{
    return new SamplerStateD3D(pCont, name);
}
void delete_SamplerState(SamplerState *p)
{
    SamplerStateD3D *pS = static_cast<SamplerStateD3D *>(p);
    delete pS;
}

StateGroupRaster *new_StateGroupRaster(Container *pCont, const char* name)
{
    return new StateGroupRasterD3D(pCont, name);
}
void delete_StateGroupRaster(StateGroupRaster *p)
{
    delete p;
}

StateGroupCS *new_StateGroupCS(Container *pCont, const char* name)
{
    return new StateGroupCSD3D(pCont, name);
}
void delete_StateGroupCS(StateGroupCS *p)
{
    delete p;
}

StateGroupDST *new_StateGroupDST(Container *pCont, const char* name)
{
    return new StateGroupDSTD3D(pCont, name);
}
void delete_StateGroupDST(StateGroupDST *p)
{
    delete p;
}

// empty function : no path rendering in D3D
StateGroupPath *new_StateGroupPR(Container *pCont, const char* name)
{
    return NULL;
}
void delete_StateGroupPR(StateGroupPath *p)
{
}
Shader* new_ShaderPath(const char* name, bool bPostscript)
{
    return NULL;
}
Program* new_ProgramPath(Container *pCont)
{
    return NULL;
}


Program* new_Program(Container *pCont)
{
    return new D3DShaderProgram(pCont);
}
void delete_Program(IProgram *pProg)
{
    D3DShaderProgram *p = static_cast<D3DShaderProgram *>(pProg);
    delete p;
}

Shader* new_GLSLShader(const char* name)
{
    return NULL;
}
Shader* new_HLSL10Shader(const char* name)
{
	// TODO avoroshilov: trace where new_HLSL10Shader is called, and deal with containers there
    return new D3DShader(name/*, pCont*/);
}
Shader* new_HLSL11Shader(const char* name)
{
	// TODO avoroshilov: trace where new_HLSL11Shader is called, and deal with containers there
	return new D3DShader(name/*, pCont*/);
}
void     delete_Shader(IShader *pShd)
{
    D3DShader *p = static_cast<D3DShader *>(pShd);
    delete p;
}


Uniform* new_Uniform(const char* name, const char* groupname, const char* semantic)
{
    UniformD3D *pUniform = new UniformD3D(name, groupname, semantic);
    return pUniform;
}
void delete_Uniform(Uniform *pUniform)
{
    UniformD3D *p = static_cast<UniformD3D *>(pUniform);
    delete p;
}

std::string buildCodeForThisUniform(IContainer* pCont, Uniform* pUniform)
{
    std::string code;
    const char * name = pUniform->getName();
    switch(pUniform->getType())
    {
    case IUniform::TInt: code += "int "; break;
    case IUniform::TInt2: code += "int2 "; break;
    case IUniform::TInt3: code += "int3 "; break;
    case IUniform::TInt4: code += "int4 "; break;
    case IUniform::TBool: code += "bool "; break;
    case IUniform::TBool2: code += "bool2 "; break;
    case IUniform::TBool3: code += "bool3 "; break;
    case IUniform::TBool4: code += "bool4 "; break;
    case IUniform::TFloat: code += "float "; break;
    case IUniform::TVec2: code += "float2 "; break;
    case IUniform::TVec3: code += "float3 "; break;
    case IUniform::TVec4: code += "float4 "; break;
    case IUniform::TMat2: code += "float2x2 "; break;
    case IUniform::TMat3: code += "float3x3 "; break;
    case IUniform::TMat4: code += "float4x4 "; break;
    // texture part. Obviously we don't expect them in constant buffers but let's use buildCodeForThisUniform for this anyways
    case IUniform::TTexture1D: code += "Texture1D "; break;
    case IUniform::TTexture2D: code += "Texture2D "; break;
    case IUniform::TTexture2DRect: code += "Texture2DRect "; break;
    case IUniform::TTexture3D: code += "Texture3D "; break;
    case IUniform::TTextureCube: code += "TextureCube "; break;
    }
    code += name;
    int sz = pUniform->getArraySz();
    if(sz > 1)
    {
        char tmp[10]; sprintf(tmp, "[%d]", sz);
        code += tmp;
    }
    // NO need to append the default value : this will be done through this Uniform anyways
    code += ";\n";
    return code;
}

bool buildCodeFromUniforms(IContainer* pCont)
{
    std::string code;
    Uniform *pUniform;
    code = std::string("//nvFX-Generated\n");
    //
    // Walk through uniform parameters in CstBuffers
    //
    ICstBuffer *pCstBuffer;
    for(int i=0; pCstBuffer = pCont->findCstBuffer(i); i++)
    {
        code += "cbuffer ";
        code += pCstBuffer->getName();
        code += " {\n";
        for(int i=0; pUniform = static_cast<Uniform*>(pCstBuffer->findUniform(i)); i++)
        {
            code += buildCodeForThisUniform(pCont, pUniform);
        }
        code += " };\n";
    }
    //
    // Walk through uniform parameters in GLOBAL space
    // and add them to a special cbuffer named nvfx_global
    // NOTE: uniforms that aren't in any cbuffer are put by default to a cbuffer called $Globals
    // However I decided to not use it here by default to avoid problems when some effects didn't
    // put all global parameters outside of HLSL source code.
    //
    code += "cbuffer nvfx_globals {\n";
    for(int i=0; pUniform = static_cast<Uniform*>(pCont->findUniform(i)); i++)
    {
        const char * name = pUniform->getName();
        // IF the uniform has '.' : we declared a member of a struct. We can't append it to GLSL !
        if(strchr(name, '.'))
            continue;
        code += buildCodeForThisUniform(pCont, pUniform);
    }
    code += "};\n";
    //
    // here is a special case : HLSL can't work without cbuffers
    // even parameters outside of any cbuffer is in fact in a cbuffer called $Global
    // We need to copy these uniforms into this $Global cbuffer
    // On the other hand we kept the HLSL generated code without cbuffer
    //
    Container *pContainer = static_cast<Container*>(pCont);
    if(pContainer->findUniform(0)) // if at least one uniform outside of any cstbuffer
    {
        CstBuffer* pGlobalBuffer = static_cast<CstBuffer*>(pContainer->createCstBuffer("nvfx_globals"));
        pContainer->moveGlobalUniformsToBuffer(pGlobalBuffer);
    }
    //
    // Look for texture resources
    // resources are potentially created for scene-level management when a render target was needed for intermediate work
    // WARNING: this will run through ALL resources. Including ones that aren't declared in the current effect
    // Idea: look for resources; check if texture binding point is there. If not, create it
    // FOR NOW: NOT NECESSARY... Texture associated with resource can be created from nvFX
    //
    //IResourceRepository* pRep = getResourceRepositorySingleton();
    //IResource *pRes;
    //for(int i=0; pRes = pRep->get(i); i++)
    //{
    //    const char * name = pRes->getName();
    //    #pragma MESSAGE("SHALL we check if the texture resource belongs to this effect ???")
    //    // If not, this could allow to easily share rendered textures among effect...
    //    //if(pRes->getContainerCreator() == pCont)
    //    switch(pRes->getType())
    //    {
    //    case RESTEX_1D :  code += "Texture1D ";
    //        code += name;
    //        code += ";\n";
    //        break;
    //    case RESTEX_2D :  code += "Texture2D ";
    //        code += name;
    //        code += ";\n";
    //        break;
    //    //case RESTEX_3D :  code += "Texture3D ";
    //    //case RESTEX_CUBE_MAP : code += "TextureCube ";
    //    }
    //}
    //
    // Look for sampler states
    //
    ISamplerState *pSS;
    for(int i=0; pSS = pCont->findSamplerState(i); i++)
    {
        const char * name = pSS->getName();
        code += "SamplerState ";
        code += name;
        code += ";\n";
        // No need to append any state : they will be handled by the runtime
    }
    //
    // Now add this generated code as header to every Shader
    //
    Shader *pShader = static_cast<Shader*>(pContainer->createHLSL10Shader(NULL));
    return pShader->loadShaderCode(code.c_str());//"#define INFX\n");
}

/*************************************************************************
 **
 ** Pass Pass Pass Pass Pass Pass Pass Pass Pass Pass Pass Pass Pass Pass
 **
 *************************************************************************/ 

/**
 **
 **  NA on OpenGL
 **
 **/ 
void* Pass::getD3DIASignature()
{
    if(m_pActiveStatesLayer == NULL)
        return NULL;
    if(m_pActiveStatesLayer->program)
        return m_pActiveStatesLayer->program->getD3DIASignature();
    if(m_pActiveStatesLayer->programPipeline)
    {
        int i=0;
        IProgram* prg = m_pActiveStatesLayer->programPipeline->getShaderProgram(i++);
        while(prg)
        {
            int f = prg->getProgramShaderFlags();
            if(f & FX_VERTEX_SHADER_BIT)
                return prg->getD3DIASignature();
            prg = m_pActiveStatesLayer->programPipeline->getShaderProgram(i++);
        }
    }
    return NULL;
}
int Pass::getD3DIASignatureSize()
{
    if(m_pActiveStatesLayer == NULL)
        return 0;
    if(m_pActiveStatesLayer->program)
        return m_pActiveStatesLayer->program->getD3DIASignatureSize();
    if(m_pActiveStatesLayer->programPipeline)
    {
        int i=0;
        IProgram* prg = m_pActiveStatesLayer->programPipeline->getShaderProgram(i++);
        while(prg)
        {
            int f = prg->getProgramShaderFlags();
            if(f & FX_VERTEX_SHADER_BIT)
                return prg->getD3DIASignatureSize();
            prg = m_pActiveStatesLayer->programPipeline->getShaderProgram(i++);
        }
    }
    return 0;
}

void Pass::setViewPort(int *viewport, float *viewportZ)
{
#ifdef USE_D3D11
	ID3D11DeviceContext *pd3d1X;
	((ID3D1XDevice *)nvFX::getDevice())->GetImmediateContext(&pd3d1X);
#else
	ID3D1XDevice *pd3d1X = (ID3D1XDevice *)nvFX::getDevice();
#endif
	D3D1X_VIEWPORT vp;
    vp.TopLeftX = viewport[0];
    vp.TopLeftY = viewport[1];
    vp.Width = viewport[2];
    vp.Height = viewport[3];
    vp.MinDepth = viewportZ ? viewportZ[0] : 0.0f;
    vp.MaxDepth = viewportZ ? viewportZ[1] : 1.0f;
	pd3d1X->RSSetViewports(1, &vp);
}

/// \brief clears the viewport and eventually sets the clear color
/// \arg numRTs : 0 means we are in the backbuffer. 1 to N means we address a FBO... yeah... lame
void Pass::clearRenderTargets(PassInfo *pi)
{
#ifdef USE_D3D11
	ID3D11DeviceContext *pd3d1X;
	((ID3D1XDevice *)nvFX::getDevice())->GetImmediateContext(&pd3d1X);
#else
	ID3D1XDevice *pd3d1X = (ID3D1XDevice *)nvFX::getDevice();
#endif
	int n=1;
    switch(pi->clearMode)
    {
    case Clear_all:
    case Clear_color:
    case Clear_color_depth:
        if(pi->currentFBO)
        {
            int n = pi->currentFBO->getNumRenderTargets();
            for(int i=0; i<(int)n; i++)
            {
                ResourceD3D* pRes = static_cast<ResourceD3D*>(pi->currentFBO->getColorResource(i));
                ID3D1XRenderTargetView* pRT = pRes->m_pTextureRTView;
				pd3d1X->ClearRenderTargetView( pRT, pi->clearColor[i]);
            }
        } else {
            IFrameBufferObjectsRepository* pRep = getFrameBufferObjectsRepositorySingleton();
            ID3D1XRenderTargetView* pRT = static_cast<ID3D1XRenderTargetView*>(pRep->getBackBuffer());
			pd3d1X->ClearRenderTargetView( pRT, pi->clearColor[0]);
        }
        break;
    }
    UINT ClearFlags;
    switch(pi->clearMode)
    {
    case Clear_depth:
        ClearFlags = D3D1X(CLEAR_DEPTH);
        break;
    case Clear_stencil:
        ClearFlags = D3D1X(CLEAR_STENCIL);
        break;
    case Clear_all:
    case Clear_depth_stencil:
        ClearFlags = D3D1X(CLEAR_DEPTH)|D3D1X(CLEAR_STENCIL);
        break;
    }
    if(ClearFlags == 0)
        return;
    if(pi->currentFBO)
    {
        ResourceD3D* pRes = static_cast<ResourceD3D*>(pi->currentFBO->getDSTResource());
        ID3D1XDepthStencilView* pDSTV = pRes->m_pTextureDSTView;
		pd3d1X->ClearDepthStencilView( pDSTV, ClearFlags, 1.0f, 0);
    } else {
        IFrameBufferObjectsRepository* pRep = getFrameBufferObjectsRepositorySingleton();
        ID3D1XDepthStencilView* pDSTV = static_cast<ID3D1XDepthStencilView*>(pRep->getBackBufferDST());
		pd3d1X->ClearDepthStencilView( pDSTV, ClearFlags, 1.0f, 0);
    }
}

/*************************************************************************/ /**

 */ /*********************************************************************/
void Pass::drawFullscreenQuad(float size, float offsetx, float offsety)
{
    ID3D1XDevice* pd3dDevice = (ID3D1XDevice*)nvFX::getDevice();
    if(TexQuad2D::g_VtxBufferStatic == NULL)
        TexQuad2D::Init(pd3dDevice, getD3DIASignature(), getD3DIASignatureSize());
    TexQuad2D::Draw(pd3dDevice, size, offsetx, offsety);
}

//
// D3DProgramPipeline D3DProgramPipeline D3DProgramPipeline D3DProgramPipeline
// D3DProgramPipeline D3DProgramPipeline D3DProgramPipeline D3DProgramPipeline
// D3DProgramPipeline D3DProgramPipeline D3DProgramPipeline D3DProgramPipeline
//

D3DProgramPipeline::D3DProgramPipeline(Container *pCont) : ProgramPipeline(pCont)
{
}

bool D3DProgramPipeline::bind()
{
    ID3D1XDevice *pd3d1X = (ID3D1XDevice *)nvFX::getDevice();
    // reset the shader stages
#ifdef USE_D3D11
    ID3D11DeviceContext *pd3dDC;
    pd3d1X->GetImmediateContext(&pd3dDC);
    pd3dDC->DSSetShader(NULL, NULL, 0);
    pd3dDC->HSSetShader(NULL, NULL, 0);
    pd3dDC->VSSetShader(NULL, NULL, 0);
    pd3dDC->PSSetShader(NULL, NULL, 0);
    pd3dDC->GSSetShader(NULL, NULL, 0);
#else
    pd3d1X->VSSetShader(NULL);
    pd3d1X->PSSetShader(NULL);
    pd3d1X->GSSetShader(NULL);
#endif
    return ProgramPipeline::bind();
}

} //namespace nvFX
