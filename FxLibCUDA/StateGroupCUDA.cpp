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
#include "FxGL.h"
#pragma warning(disable:4996)
using namespace nvFX;

/***************************************************************************************/
/* StateGroupRasterGL StateGroupRasterGL StateGroupRasterGL StateGroupRasterGL */
/***************************************************************************************/

/*************************************************************************/ /**
 ** 
 ** 
 **/ /*************************************************************************/ 
void StateGroupRasterGL::apply()
{
    StateVec::iterator it = m_states.begin();
    while(it != m_states.end())
    {
        State *s = *it;
        if(s)
        {
            switch(s->state)
            {
        #ifndef OGLES2
            case GL_POINT_SMOOTH:
            case GL_POINT_SPRITE:
            case GL_LINE_SMOOTH:
            case GL_LINE_STIPPLE:
            case GL_POLYGON_SMOOTH:
            case GL_POLYGON_OFFSET_POINT:
            case GL_POLYGON_OFFSET_LINE:
            case GL_POLYGON_STIPPLE:
            case GL_RASTERIZER_DISCARD:
        #endif
            case GL_POLYGON_OFFSET_FILL:
            case GL_CULL_FACE:
                if(s->values[0] == GL_TRUE) glEnable(s->state);
                else glDisable(s->state);
                break;
        #ifndef OGLES2
            case GL_POINT_SPRITE_COORD_ORIGIN:
                break;
            case GL_POLYGON_MODE:
                glPolygonMode(s->values[0], s->values[1]);
                break;
            //case GL_COMBINED_LINE_STIPPLE_NV:
                break;
            case GL_LINE_STIPPLE_PATTERN:
            case GL_LINE_STIPPLE_REPEAT:
                assert(0);
                break;
            case GL_POINT_SIZE:
                glPointSize(s->fvals[0]);
                break;
        #endif
            case GL_CULL_FACE_MODE:
                glCullFace(s->values[0]);
                break;
            case GL_FRONT_FACE:
                glFrontFace(s->values[0]);
                break;
            case GL_LINE_WIDTH:
                glLineWidth(s->fvals[0]);
                break;
            case GL_POLYGON_OFFSET_FACTOR:
                glPolygonOffset(s->fvals[0], s->fvals[1]);
                break;
            case GL_POLYGON_OFFSET_UNITS:// in GL_POLYGON_OFFSET_FACTOR
                break;
            }
        }
        ++it;
    }
}
/*************************************************************************/ /**
 ** In OpenGL, validate is not mandatory.
 ** But it will be used when we will use the extension for state-groups...
 **/ /*************************************************************************/ 
bool StateGroupRasterGL::validate()
{
    return true;
}

/***************************************************************************/
/* StateGroupCSGL StateGroupCSGL StateGroupCSGL StateGroupCSGL StateGroupCSGL        */
/***************************************************************************/

/*************************************************************************/ /**
 ** 
 ** 
 **/ /*************************************************************************/ 
void StateGroupCSGL::apply()
{
    StateVec::iterator it = m_states.begin();
    while(it != m_states.end())
    {
        State *s = *it;
        if(s)
        {
            switch(s->state)
            {
            //case GL_PER_DRAW_BUFFER_BLEND_FUNCTION_NV:
            //case GL_PER_DRAW_BUFFER_BLEND_EQUATION_NV:
            //case GL_PER_DRAW_BUFFER_BLEND_ENABLE_NV:
            //case GL_PER_DRAW_BUFFER_COLOR_WRITEMASK_NV:
        #ifndef OGLES2
            case GL_FRAMEBUFFER_SRGB:
            case GL_COLOR_LOGIC_OP:
            case GL_ALPHA_TEST:
            case GL_MULTISAMPLE:
            case GL_SAMPLE_MASK:
            case GL_SAMPLE_ALPHA_TO_ONE:
        #endif
            case GL_DITHER:
            case GL_SAMPLE_ALPHA_TO_COVERAGE:
            case GL_BLEND:
                if(s->value[0] == GL_TRUE) glEnable(s->state);
                else glDisable(s->state);
                break;
        #ifndef OGLES2
            case GL_ALPHA_TEST_REF: // in GL_ALPHA_TEST_FUNC
                break;
            case GL_ALPHA_TEST_FUNC:
                glAlphaFunc(s->value[0], s->fvalue);
                break;
            case GL_BLEND_SRC:
                glBlendFunc(s->value[0], s->value[1]);
                break;
            case GL_BLEND_DST:// in GL_BLEND_SRC
                break;
        #endif

            case GL_BLEND_SRC_RGB:
                glBlendFuncSeparate(s->value[0], s->value[1], s->value[2], s->value[3]);
                break;
            case GL_BLEND_SRC_ALPHA:// in GL_BLEND_SRC_RGB
                break;
            case GL_BLEND_DST_RGB:// in GL_BLEND_SRC_RGB
                break;
            case GL_BLEND_DST_ALPHA:// in GL_BLEND_SRC_RGB
                break;

            //case GL_BLEND_EQUATION_RGBA_NV:
                break;
            case GL_BLEND_EQUATION_RGB:
        #ifndef OGLES2
                glBlendEquationSeparateEXT(s->value[0], s->value[1]);
        #else
                glBlendEquationSeparate(s->value[0], s->value[1]);
        #endif
                break;
            case GL_BLEND_EQUATION_ALPHA: // in GL_BLEND_EQUATION_RGB
                break;
        #ifndef OGLES2
            case GL_LOGIC_OP_MODE:
                glLogicOp(s->value[0]);
                break;
            case GL_SAMPLE_MASK_VALUE:
                assert(0);
                break;
        #endif
            case GL_COLOR_WRITEMASK:
                glColorMask(s->bvals[0], s->bvals[1], s->bvals[2],s-> bvals[3]);
                break;
            case GL_BLEND_COLOR:
                glBlendColor(s->fvals[0], s->fvals[1], s->fvals[2], s->fvals[3]);
                break;
            }
        }
        ++it;
    }
}
/*************************************************************************/ /**
 ** In OpenGL, validate is not mandatory.
 ** But it will be used when we will use the extension for state-groups...
 **/ /*************************************************************************/ 
bool StateGroupCSGL::validate()
{
    return true;
}

/***************************************************************************/
/*  StateGroupDSTGL StateGroupDSTGL StateGroupDSTGL StateGroupDSTGL StateGroupDSTGL  */
/***************************************************************************/

/*************************************************************************/ /**
 ** 
 ** 
 **/ /*************************************************************************/ 
void StateGroupDSTGL::apply()
{
    StateVec::iterator it = m_states.begin();
    while(it != m_states.end())
    {
        State *s = *it;
        if(s)
        {
            switch(s->state)
            {
            case GL_DEPTH_TEST:
        #ifndef OGLES2
            case GL_DEPTH_CLAMP:
            case GL_DEPTH_BOUNDS_TEST_EXT:
        #endif
            case GL_STENCIL_TEST:
                if(s->value == GL_TRUE) glEnable(s->state);
                else glDisable(s->state);
                break;
            case GL_DEPTH_FUNC:
                glDepthFunc(s->value);
                break;
            case GL_DEPTH_WRITEMASK:
                glDepthMask(s->value);
                break;
        #ifndef OGLES2
            case GL_DEPTH_BOUNDS_EXT:
                assert(0);
        #endif
                break;

            //case GL_STENCIL_FRONT_REF:
            case GL_STENCIL_BACK_REF://GL_STENCIL_BACK_FUNC
            case GL_STENCIL_REF: // in GL_STENCIL_FUNC
            case GL_STENCIL_BACK_VALUE_MASK://GL_STENCIL_BACK_FUNC
            case GL_STENCIL_VALUE_MASK: // in GL_STENCIL_FUNC
                break;
            //case GL_STENCIL_FRONT_FUNC:
            case GL_STENCIL_FUNC:
                if(s->value)
                    glStencilFuncSeparate(GL_FRONT, s->value, s->values[0], s->values[1]);
                break;
            case GL_STENCIL_BACK_FUNC:
                if(s->value)
                    glStencilFuncSeparate(GL_BACK, s->value, s->values[0], s->values[1]);
                break;

            case GL_STENCIL_PASS_DEPTH_PASS:// in GL_STENCIL_FAIL
            case GL_STENCIL_PASS_DEPTH_FAIL:// in GL_STENCIL_FAIL
                break;
            //case GL_STENCIL_FRONT_FAIL:
            case GL_STENCIL_FAIL:
                glStencilOpSeparate(GL_FRONT, s->value, s->values[0], s->values[1]);
                break;

            case GL_STENCIL_BACK_PASS_DEPTH_PASS:
                break;
            case GL_STENCIL_BACK_FAIL:
                glStencilOpSeparate(GL_BACK, s->value, s->values[0], s->values[1]);
                break;
            //case GL_STENCIL_FRONT_PASS_DEPTH_FAIL:
            case GL_STENCIL_BACK_PASS_DEPTH_FAIL:
                break;
            //case GL_STENCIL_FRONT_PASS_DEPTH_PASS:

            case GL_STENCIL_BACK_WRITEMASK:
                break;
            case GL_STENCIL_WRITEMASK:
                break;
            //case GL_STENCIL_FRONT_VALUE_MASK:
                break;
            //case GL_STENCIL_FRONT_WRITEMASK:
                break;
            }
        }
        ++it;
    }
}
/*************************************************************************/ /**
 ** In OpenGL, validate is not mandatory.
 ** But it will be used when we will use the extension for state-groups...
 **/ /*************************************************************************/ 
bool StateGroupDSTGL::validate()
{
    return true;
}

