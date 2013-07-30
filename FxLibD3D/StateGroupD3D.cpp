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
#ifdef WIN32
#include <windows.h> // for OutputDebugString
#endif
#include <fstream>
#include <assert.h>
#include "Fx.h"
#include "FxD3D.h"
#include "OpenGL_enums.h"
#pragma warning(disable:4996)
using namespace nvFX;

#pragma MESSAGE("TODO TODO TODO TODO TODO : Incomplete management of render states. Need to do all and test them all...")
#pragma MESSAGE("TODO TODO TODO TODO TODO : add D3D states (names and possible specific behavior)...")

D3D1X_FILL_MODE FillModeGL2D3D(GLenum e)
{
    switch(e)
    {
    //case GL_POINT: return D3D1X(FILL_WIREFRAME;
    case GL_LINE: return D3D1X(FILL_WIREFRAME);
	case GL_FILL: return D3D1X(FILL_SOLID);
    }
    return D3D1X(FILL_WIREFRAME);
}

D3D1X_CULL_MODE CullModeGL2D3D(GLenum e)
{
    switch(e)
    {
    //case GL_: return D3D1X(CULL_NONE);
    case GL_FRONT: return D3D1X(CULL_FRONT);
    case GL_BACK: return D3D1X(CULL_BACK);
    //case GL_FRONT_AND_BACK: ... ?
    }
    return D3D1X(CULL_NONE);
}

D3D1X_COMPARISON_FUNC DepthFuncGL2D3D(GLenum s)
{
    switch(s)
    {
    case GL_NEVER: return D3D1X(COMPARISON_NEVER);
	case GL_LESS: return D3D1X(COMPARISON_LESS);
	case GL_EQUAL: return D3D1X(COMPARISON_EQUAL);
	case GL_LEQUAL: return D3D1X(COMPARISON_LESS_EQUAL);
	case GL_GREATER: return D3D1X(COMPARISON_GREATER);
	case GL_NOTEQUAL: return D3D1X(COMPARISON_NOT_EQUAL);
	case GL_GEQUAL: return D3D1X(COMPARISON_GREATER_EQUAL);
	case GL_ALWAYS: return D3D1X(COMPARISON_ALWAYS);
    }
    return D3D1X(COMPARISON_NEVER);
}
D3D1X_COMPARISON_FUNC ComparisonFuncGL2D3D(GLenum e)
{
    switch(e)
    {
    case GL_NEVER: return D3D1X(COMPARISON_NEVER);
	case GL_LESS: return D3D1X(COMPARISON_LESS);
	case GL_EQUAL: return D3D1X(COMPARISON_EQUAL);
	case GL_LEQUAL: return D3D1X(COMPARISON_LESS_EQUAL);
	case GL_GREATER: return D3D1X(COMPARISON_GREATER);
	case GL_NOTEQUAL: return D3D1X(COMPARISON_NOT_EQUAL);
	case GL_GEQUAL: return D3D1X(COMPARISON_GREATER_EQUAL);
	case GL_ALWAYS: return D3D1X(COMPARISON_ALWAYS);
    };
    return D3D1X(COMPARISON_NEVER);
}
D3D1X_STENCIL_OP StencilOpGL2D3D(GLenum e)
{
    switch(e)
    {
    case GL_KEEP: return D3D1X(STENCIL_OP_KEEP);
    case GL_ZERO: return D3D1X(STENCIL_OP_ZERO);
    case GL_REPLACE: return D3D1X(STENCIL_OP_REPLACE);
    //case GL_INCR: return D3D1X(STENCIL_OP_INCR_SAT);
    //case GL_DECR: return D3D1X(STENCIL_OP_DECR_SAT);
    case GL_INVERT: return D3D1X(STENCIL_OP_INVERT);
    case GL_INCR: return D3D1X(STENCIL_OP_INCR);
    case GL_DECR: return D3D1X(STENCIL_OP_DECR);
    }
    return D3D1X(STENCIL_OP_KEEP);
}

D3D1X_BLEND_OP BlendOpGL2D3D(GLenum e)
{
    switch(e)
    {
    case GL_FUNC_ADD: return D3D1X(BLEND_OP_ADD);
	case GL_FUNC_SUBTRACT: return D3D1X(BLEND_OP_SUBTRACT);
	case GL_FUNC_REVERSE_SUBTRACT: return D3D1X(BLEND_OP_REV_SUBTRACT);
	case GL_MIN: return D3D1X(BLEND_OP_MIN);
	case GL_MAX: return D3D1X(BLEND_OP_MAX);
    }
    return D3D1X(BLEND_OP_ADD);
};

D3D1X_BLEND BlendGL2D3D(GLenum e)
{
    switch(e)
    {
    case GL_ZERO: return D3D1X(BLEND_ZERO);
	case GL_ONE: return D3D1X(BLEND_ONE);
	case GL_SRC_COLOR: return D3D1X(BLEND_SRC_COLOR);
	case GL_ONE_MINUS_SRC_COLOR: return D3D1X(BLEND_INV_SRC_COLOR);
	case GL_SRC_ALPHA: return D3D1X(BLEND_SRC_ALPHA);
	case GL_ONE_MINUS_SRC_ALPHA: return D3D1X(BLEND_INV_SRC_ALPHA);
	case GL_DST_ALPHA: return D3D1X(BLEND_DEST_ALPHA);
	case GL_ONE_MINUS_DST_ALPHA: return D3D1X(BLEND_INV_DEST_ALPHA);
	case GL_DST_COLOR: return D3D1X(BLEND_DEST_COLOR);
	case GL_ONE_MINUS_DST_COLOR: return D3D1X(BLEND_INV_DEST_COLOR);
	case GL_SRC_ALPHA_SATURATE: return D3D1X(BLEND_SRC_ALPHA_SAT);
	//??case GL_: return D3D1X(BLEND_BLEND_FACTOR);
	//??case GL_: return D3D1X(BLEND_INV_BLEND_FACTOR);
	case GL_SRC1_COLOR: return D3D1X(BLEND_SRC1_COLOR);
	case GL_ONE_MINUS_SRC1_COLOR: return D3D1X(BLEND_INV_SRC1_COLOR);
	case GL_SRC1_ALPHA: return D3D1X(BLEND_SRC1_ALPHA);
	case GL_ONE_MINUS_SRC1_ALPHA: return D3D1X(BLEND_INV_SRC1_ALPHA);
    }
    return D3D1X(BLEND_ONE);
}
/***************************************************************************************/
/* StateGroupRasterGL StateGroupRasterGL StateGroupRasterGL StateGroupRasterGL */
/***************************************************************************************/

/*************************************************************************/ /**
 ** 
 ** 
 **/ /*************************************************************************/ 
StateGroupRasterD3D::~StateGroupRasterD3D()
{
}

/*************************************************************************/ /**
 ** 
 ** 
 **/ /*************************************************************************/ 
StateGroupRasterD3D::StateGroupRasterD3D(Container* pCont, const char *name) : StateGroupRaster(pCont, name)
{
    m_d3dStateGroup = NULL;
}
/*************************************************************************/ /**
 ** 
 ** 
 **/ /*************************************************************************/ 
void StateGroupRasterD3D::apply()
{
    if(!m_d3dStateGroup)
        if(!validate())
            return;
#ifdef USE_D3D11
    ID3D11DeviceContext *pd3d1X;
    ((ID3D1XDevice *)nvFX::getDevice())->GetImmediateContext(&pd3d1X);
#else
    ID3D1XDevice *pd3d1X = (ID3D1XDevice *)nvFX::getDevice();
#endif
    if(!pd3d1X)
        return;
    pd3d1X->RSSetState(m_d3dStateGroup);
}
/*************************************************************************/ /**
 ** \todo Not ready : need to check all the states
 ** 
 **/ /*************************************************************************/ 
bool StateGroupRasterD3D::validate()
{
    if(m_d3dStateGroup)
        m_d3dStateGroup->Release();
    ID3D1XDevice *pd3d1X = (ID3D1XDevice *)nvFX::getDevice();
    if(!pd3d1X)
        return false;
    D3D1X_RASTERIZER_DESC rd = {
        D3D1X(FILL_SOLID),
        D3D1X(CULL_BACK),
        FALSE,
        0,
        0.0f,
        0.0f,
        TRUE,
        FALSE,
        FALSE,
        FALSE
    };
    StateVec::iterator it = m_states.begin();
    while(it != m_states.end())
    {
        State* pState = *it;
        if(pState)
        {
            switch(pState->state)
            {
            case GL_POINT_SMOOTH:
            case GL_POINT_SPRITE:
            case GL_LINE_SMOOTH:
            case GL_LINE_STIPPLE:
            case GL_POLYGON_SMOOTH:
            case GL_POLYGON_OFFSET_POINT:
            case GL_POLYGON_OFFSET_LINE:
            case GL_POLYGON_STIPPLE:
            case GL_RASTERIZER_DISCARD:
            case GL_POLYGON_OFFSET_FILL:
                DebugBreak();
                break;
            case GL_CULL_FACE:
                rd.CullMode = CullModeGL2D3D(pState->state);
                break;
            case GL_POINT_SPRITE_COORD_ORIGIN:
                DebugBreak();
                break;
            case GL_POLYGON_MODE:
                rd.FillMode = FillModeGL2D3D(pState->state);
                break;
            //case GL_COMBINED_LINE_STIPPLE_NV:
            case GL_LINE_STIPPLE_PATTERN:
            case GL_LINE_STIPPLE_REPEAT:
                DebugBreak();
                break;
            case GL_POINT_SIZE:
                DebugBreak();
                break;
            case GL_CULL_FACE_MODE:
                DebugBreak();
                break;
            case GL_FRONT_FACE: //GL_CW and GL_CCW
                DebugBreak();
                break;
            case GL_LINE_WIDTH:
                DebugBreak();
                break;
            case GL_POLYGON_OFFSET_FACTOR:
                DebugBreak();
                break;
            case GL_POLYGON_OFFSET_UNITS:// in GL_POLYGON_OFFSET_FACTOR
                DebugBreak();
                break;
            }
        }
        ++it;
    }
    HRESULT hr = pd3d1X->CreateRasterizerState(&rd, &m_d3dStateGroup);
    if(FAILED(hr))
        return false;
    return true;
}

/***************************************************************************/
/* StateGroupCSGL StateGroupCSGL StateGroupCSGL StateGroupCSGL StateGroupCSGL        */
/***************************************************************************/

/*************************************************************************/ /**
 ** 
 ** 
 **/ /*************************************************************************/ 
StateGroupCSD3D::~StateGroupCSD3D()
{
}
/*************************************************************************/ /**
 ** 
 ** 
 **/ /*************************************************************************/ 
StateGroupCSD3D::StateGroupCSD3D(Container* pCont, const char *name) : StateGroupCS(pCont, name)
{
    m_d3dStateGroup = NULL;
}


/*************************************************************************/ /**
 ** 
 ** 
 **/ /*************************************************************************/ 
void StateGroupCSD3D::apply()
{
    if(!m_d3dStateGroup)
        if(!validate())
            return;
#ifdef USE_D3D11
    ID3D11DeviceContext *pd3d1X;
    ((ID3D1XDevice *)nvFX::getDevice())->GetImmediateContext(&pd3d1X);
#else
    ID3D1XDevice *pd3d1X = (ID3D1XDevice *)nvFX::getDevice();
#endif
    if(!pd3d1X)
        return;
    pd3d1X->OMSetBlendState(m_d3dStateGroup, 0, 0xFFFFFFFF);// TODO: Blend factor and Sample Mask !!
}
/*************************************************************************/ /**
 ** \todo BlendEnable for many levels. For now, only for level 0 :-(
 ** \todo Not ready : need to check all the states
 ** 
 **/ /*************************************************************************/ 
bool StateGroupCSD3D::validate()
{
    if(m_d3dStateGroup)
        m_d3dStateGroup->Release();
    ID3D1XDevice *pd3d1X = (ID3D1XDevice *)nvFX::getDevice();
    if(!pd3d1X)
        return false;
    D3D1X_BLEND_DESC bsd
#ifndef USE_D3D11
    = {
        FALSE,
        { FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE},
        D3D1X(BLEND_ONE),
        D3D1X(BLEND_ZERO),
        D3D1X(BLEND_OP_ADD),
        D3D1X(BLEND_ONE),
        D3D1X(BLEND_ZERO),
        D3D1X(BLEND_OP_ADD),
        { D3D1X(COLOR_WRITE_ENABLE_ALL), D3D1X(COLOR_WRITE_ENABLE_ALL), D3D1X(COLOR_WRITE_ENABLE_ALL), D3D1X(COLOR_WRITE_ENABLE_ALL), 
        D3D1X(COLOR_WRITE_ENABLE_ALL), D3D1X(COLOR_WRITE_ENABLE_ALL), D3D1X(COLOR_WRITE_ENABLE_ALL), D3D1X(COLOR_WRITE_ENABLE_ALL )}
    };
#else
    ; bsd.AlphaToCoverageEnable = FALSE;
    bsd.IndependentBlendEnable = FALSE;
    for(int i=0; i<8; i++)
    {
        //D3D11_RENDER_TARGET_BLEND_DESC
        bsd.RenderTarget[i].BlendEnable = FALSE;
        bsd.RenderTarget[i].SrcBlend = D3D1X(BLEND_ONE);
        bsd.RenderTarget[i].DestBlend = D3D1X(BLEND_ZERO);
        bsd.RenderTarget[i].BlendOp = D3D1X(BLEND_OP_ADD);
        bsd.RenderTarget[i].SrcBlendAlpha = D3D1X(BLEND_ONE);
        bsd.RenderTarget[i].DestBlendAlpha = D3D1X(BLEND_ZERO);
        bsd.RenderTarget[i].BlendOpAlpha = D3D1X(BLEND_OP_ADD);
        bsd.RenderTarget[i].RenderTargetWriteMask = D3D1X(COLOR_WRITE_ENABLE_ALL);
    }
#endif
    StateVec::iterator it = m_states.begin();
    while(it != m_states.end())
    {
        State* pState = *it;
        if(pState)
        {
            switch(pState->state)
            {
            //case GL_PER_DRAW_BUFFER_BLEND_FUNCTION_NV:
            //case GL_PER_DRAW_BUFFER_BLEND_EQUATION_NV:
            //case GL_PER_DRAW_BUFFER_BLEND_ENABLE_NV:
            //case GL_PER_DRAW_BUFFER_COLOR_WRITEMASK_NV:
            case GL_FRAMEBUFFER_SRGB:
                DebugBreak();
                break;
            case GL_COLOR_LOGIC_OP: // See http://www.opengl.org/sdk/docs/man/xhtml/glLogicOp.xml
                //bsd.BlendOp = BlendOpGL2D3D(pState->state);
                DebugBreak();
                break;
            case GL_ALPHA_TEST:
                DebugBreak(); // No more alpha Test in D3D
                break;
            case GL_MULTISAMPLE:
                DebugBreak();
                break;
            case GL_SAMPLE_MASK: // As arg of OMSetBlendState
                DebugBreak();
                break;
            case GL_SAMPLE_ALPHA_TO_ONE:
                DebugBreak();
                break;
            case GL_DITHER:
                DebugBreak();
                break;
            case GL_SAMPLE_ALPHA_TO_COVERAGE:
                bsd.AlphaToCoverageEnable = pState->state;
                break;
            case GL_BLEND:
                // WARNING: for now only same blending for all...
                // TODO: Fix this
                for(int i=0; i<8; i++)
#ifdef USE_D3D11
                    bsd.RenderTarget[i].BlendEnable = pState->state;
#else
                    bsd.BlendEnable[i] = pState->state;
#endif
                break;
            case GL_ALPHA_TEST_REF: // in GL_ALPHA_TEST_FUNC
                DebugBreak();
                break;
            case GL_ALPHA_TEST_FUNC:
                DebugBreak();
                break;
            case GL_BLEND_SRC:
#ifdef USE_D3D11
#pragma MESSAGE("TODO TODO TODO TODO : allow to assign separate values for each RT")
                for(int i=0; i<8; i++) {
                    bsd.RenderTarget[i].SrcBlend = BlendGL2D3D(pState->state);
                    bsd.RenderTarget[i].SrcBlendAlpha = BlendGL2D3D(pState->state);
                }
#else
                bsd.SrcBlend = BlendGL2D3D(pState->state);
                bsd.SrcBlendAlpha = BlendGL2D3D(pState->state);
#endif
                break;
            case GL_BLEND_DST:// in GL_BLEND_SRC
#ifdef USE_D3D11
                for(int i=0; i<8; i++) {
                    bsd.RenderTarget[i].DestBlend = BlendGL2D3D(pState->state);
                    bsd.RenderTarget[i].DestBlendAlpha = BlendGL2D3D(pState->state);
                }
#else
                bsd.DestBlend = BlendGL2D3D(pState->state);
                bsd.DestBlendAlpha = BlendGL2D3D(pState->state);
#endif
                break;

            case GL_BLEND_SRC_RGB:
#ifdef USE_D3D11
                for(int i=0; i<8; i++)
                    bsd.RenderTarget[i].SrcBlend = BlendGL2D3D(pState->state);
#else
                bsd.SrcBlend = BlendGL2D3D(pState->state);
#endif
                break;
            case GL_BLEND_SRC_ALPHA:// in GL_BLEND_SRC_RGB
#ifdef USE_D3D11
                for(int i=0; i<8; i++)
                    bsd.RenderTarget[i].SrcBlendAlpha = BlendGL2D3D(pState->state);
#else
                bsd.SrcBlendAlpha = BlendGL2D3D(pState->state);
#endif
                break;
            case GL_BLEND_DST_RGB:// in GL_BLEND_SRC_RGB
#ifdef USE_D3D11
                for(int i=0; i<8; i++)
                    bsd.RenderTarget[i].DestBlend = BlendGL2D3D(pState->state);
#else
                bsd.DestBlend = BlendGL2D3D(pState->state);
#endif
                break;
            case GL_BLEND_DST_ALPHA:// in GL_BLEND_SRC_RGB
#ifdef USE_D3D11
                for(int i=0; i<8; i++)
                    bsd.RenderTarget[i].DestBlendAlpha = BlendGL2D3D(pState->state);
#else
                bsd.DestBlendAlpha = BlendGL2D3D(pState->state);
#endif
                break;

            //case GL_BLEND_EQUATION_RGBA_NV:
                break;
            case GL_BLEND_EQUATION_RGB:
#ifdef USE_D3D11
                for(int i=0; i<8; i++)
                    bsd.RenderTarget[i].BlendOp = BlendOpGL2D3D(pState->state);
#else
                bsd.BlendOp = BlendOpGL2D3D(pState->state);
#endif
                break;
            case GL_BLEND_EQUATION_ALPHA: // in GL_BLEND_EQUATION_RGB
#ifdef USE_D3D11
                for(int i=0; i<8; i++)
                    bsd.RenderTarget[i].BlendOpAlpha = BlendOpGL2D3D(pState->state);
#else
                bsd.BlendOpAlpha = BlendOpGL2D3D(pState->state);
#endif
                break;
            case GL_LOGIC_OP_MODE:
                DebugBreak();
                //glLogicOp(value[0]);
                break;
            case GL_SAMPLE_MASK_VALUE: // As argument in the OMSetBlendState ?
                DebugBreak();
                break;
            case GL_COLOR_WRITEMASK:
                DebugBreak();
                break;
            case GL_BLEND_COLOR:
                // as argument of OMSetBlendState ?
                DebugBreak();
                break;
            }
        }
        ++it;
    }
    HRESULT hr = pd3d1X->CreateBlendState(&bsd, &m_d3dStateGroup);
    if(FAILED(hr))
        return false;
    return true;
}

/***************************************************************************/
/*  StateGroupDSTGL StateGroupDSTGL StateGroupDSTGL StateGroupDSTGL StateGroupDSTGL  */
/***************************************************************************/

/*************************************************************************/ /**
 ** 
 ** 
 **/ /*************************************************************************/ 
StateGroupDSTD3D::~StateGroupDSTD3D()
{
}

/*************************************************************************/ /**
 ** 
 ** 
 **/ /*************************************************************************/ 
StateGroupDSTD3D::StateGroupDSTD3D(Container* pCont, const char *name) : StateGroupDST(pCont, name)
{
    m_d3dStateGroup = NULL;
}

/*************************************************************************/ /**
 ** 
 ** 
 **/ /*************************************************************************/ 
void StateGroupDSTD3D::apply()
{
    if(!m_d3dStateGroup)
        if(!validate())
            return;
#ifdef USE_D3D11
    ID3D11DeviceContext *pd3d1X;
    ((ID3D1XDevice *)nvFX::getDevice())->GetImmediateContext(&pd3d1X);
#else
    ID3D1XDevice *pd3d1X = (ID3D1XDevice *)nvFX::getDevice();
#endif
    if(!pd3d1X)
        return;
    pd3d1X->OMSetDepthStencilState(m_d3dStateGroup, 0);// TODO: stencil Ref !!
}
/*************************************************************************/ /**
 ** \todo Check this whole setup : OGL 2 D3D is rather tricky... must test!
 ** 
 **/ /*************************************************************************/ 
bool StateGroupDSTD3D::validate()
{
    if(m_d3dStateGroup)
        m_d3dStateGroup->Release();
    ID3D1XDevice *pd3d1X = (ID3D1XDevice *)nvFX::getDevice();
    if(!pd3d1X)
        return false;
    D3D1X_DEPTH_STENCIL_DESC dstd = {
      TRUE,//BOOL                       DepthEnable;
      D3D1X(DEPTH_WRITE_MASK_ZERO),//D3D1X(DEPTH_WRITE_MASK_ALL),//D3D1X(DEPTH_WRITE_MASK) DepthWriteMask;
      D3D1X(COMPARISON_LESS),//D3D1X(COMPARISON_FUNC) DepthFunc;
      FALSE,//BOOL                       StencilEnable;
      D3D1X(DEFAULT_STENCIL_READ_MASK),//UINT8                      StencilReadMask;
      D3D1X(DEFAULT_STENCIL_WRITE_MASK),//UINT8                      StencilWriteMask;
      {
        D3D1X(STENCIL_OP_KEEP),
        D3D1X(STENCIL_OP_KEEP),
        D3D1X(STENCIL_OP_KEEP),
        D3D1X(COMPARISON_ALWAYS)
      },
      { //D3D1X(DEPTH_STENCILOP_DESC )BackFace;
        D3D1X(STENCIL_OP_KEEP),
        D3D1X(STENCIL_OP_KEEP),
        D3D1X(STENCIL_OP_KEEP),
        D3D1X(COMPARISON_ALWAYS)
      },
    };
    StateVec::iterator it = m_states.begin();
    while(it != m_states.end())
    {
        State* pState = *it;
        if(pState)
        {
            switch(pState->state)
            {
            case GL_DEPTH_TEST:
                dstd.DepthEnable = pState->value;
                break;
            case GL_DEPTH_CLAMP:
                DebugBreak();
                break;
            case GL_DEPTH_BOUNDS_TEST_EXT:
                DebugBreak();
                break;
            case GL_STENCIL_TEST:
                dstd.StencilEnable = pState->value;
                break;
            case GL_DEPTH_FUNC:
                dstd.DepthFunc = DepthFuncGL2D3D(pState->state);
                break;
            case GL_DEPTH_WRITEMASK:
                dstd.DepthWriteMask = (D3D1X(DEPTH_WRITE_MASK))pState->value;
                break;
            case GL_DEPTH_BOUNDS_EXT:
                DebugBreak();
                break;
            //case GL_STENCIL_FRONT_REF:
            case GL_STENCIL_BACK_REF://GL_STENCIL_BACK_FUNC
                // This part is not even passed as argument of OMSetDepthStencilState
                DebugBreak();
            case GL_STENCIL_REF: // in GL_STENCIL_FUNC
                // This part is passed as argument of OMSetDepthStencilState
                DebugBreak();
            case GL_STENCIL_BACK_VALUE_MASK://GL_STENCIL_BACK_FUNC
            case GL_STENCIL_VALUE_MASK: // in GL_STENCIL_FUNC
                DebugBreak();
                break;
            //case GL_STENCIL_FRONT_FUNC:
            case GL_STENCIL_FUNC:
                dstd.FrontFace.StencilFunc = ComparisonFuncGL2D3D(pState->state);
                break;
            case GL_STENCIL_BACK_FUNC:
                dstd.BackFace.StencilFunc = ComparisonFuncGL2D3D(pState->state);
                break;
            case GL_STENCIL_PASS_DEPTH_PASS:// in GL_STENCIL_FAIL
                DebugBreak();
                dstd.FrontFace.StencilPassOp = StencilOpGL2D3D(pState->state);
                break;
            case GL_STENCIL_PASS_DEPTH_FAIL:// in GL_STENCIL_FAIL
                DebugBreak();
                dstd.FrontFace.StencilFailOp = StencilOpGL2D3D(pState->state);
                break;
            //case GL_STENCIL_FRONT_FAIL:
            case GL_STENCIL_FAIL:
                dstd.FrontFace.StencilFailOp = StencilOpGL2D3D(pState->state);
                break;
            case GL_STENCIL_BACK_PASS_DEPTH_PASS:
                DebugBreak();
                break;
            case GL_STENCIL_BACK_FAIL:
                dstd.BackFace.StencilFailOp = StencilOpGL2D3D(pState->state);
                break;
            //case GL_STENCIL_FRONT_PASS_DEPTH_FAIL:
            case GL_STENCIL_BACK_PASS_DEPTH_FAIL:
                DebugBreak();
                break;
            //case GL_STENCIL_FRONT_PASS_DEPTH_PASS:
            case GL_STENCIL_BACK_WRITEMASK:
                DebugBreak();
                dstd.StencilWriteMask = pState->value;
                break;
            case GL_STENCIL_WRITEMASK:
                dstd.StencilWriteMask = pState->value;
                break;
            //case GL_STENCIL_FRONT_VALUE_MASK:
            //case GL_STENCIL_FRONT_WRITEMASK:
                break;
            }
        }
        ++it;
    }
    HRESULT hr = pd3d1X->CreateDepthStencilState(&dstd, &m_d3dStateGroup);
    if(FAILED(hr))
        return false;
    return true;
}

