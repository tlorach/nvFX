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
/// include the enums we need to create the state groups
#include "OpenGL_enums.h"
#pragma warning(disable:4996)
using namespace nvFX;

/***************************************************************************************/
/* StateGroupRaster StateGroupRaster StateGroupRaster StateGroupRaster */
/***************************************************************************************/

/*************************************************************************/ /**
 ** 
 ** 
 **/ /*************************************************************************/ 
StateGroupRaster::~StateGroupRaster()
{
    StateVec::iterator it = m_states.begin();
    while(it != m_states.end())
    {
        if(*it)
            delete *it;
        ++it;
    }
    m_states.clear();
}

/*************************************************************************/ /**
 ** 
 ** 
 **/ /*************************************************************************/ 
StateGroupRaster::StateGroupRaster(Container* pCont, const char *name)
{
    m_container = pCont;
    if(name)
        m_name = name;
}

/*************************************************************************/ /**
 ** 
 ** 
 **/ /*************************************************************************/ 
StateGroupRaster::State* StateGroupRaster::createState(GLenum state, GLenum value1, GLenum value2)
{
#ifndef OGLES2
    switch(state)
    {
    case GL_POLYGON_MODE:
        break;
    default:
#ifdef WIN32
        OutputDebugStringA("WRONG STATE for StateGroupRaster::createState\n");
#endif
        return NULL;
    }
    m_states.push_back(new State(state, value1, value2) );
    return m_states.back();
#else
    return NULL;
#endif
}

/*************************************************************************/ /**
 ** 
 ** 
 **/ /*************************************************************************/ 
StateGroupRaster::State* StateGroupRaster::createState(GLenum state, GLenum value)
{
    switch(state)
    {
#ifndef OGLES2
    case GL_POINT_SMOOTH:
    case GL_POINT_SPRITE:
    case GL_PROGRAM_POINT_SIZE:
    case GL_POINT_SPRITE_COORD_ORIGIN:
    case GL_LINE_SMOOTH:
    case GL_LINE_STIPPLE:
    case GL_POLYGON_SMOOTH:
    case GL_POLYGON_OFFSET_POINT:
    case GL_POLYGON_OFFSET_LINE:
    case GL_POLYGON_STIPPLE:
    case GL_RASTERIZER_DISCARD:
    //case GL_COMBINED_LINE_STIPPLE_NV:
    case GL_LINE_STIPPLE_PATTERN:
    case GL_LINE_STIPPLE_REPEAT:
#endif
    case GL_CULL_FACE:
    case GL_CULL_FACE_MODE:
    case GL_FRONT_FACE:
    case GL_POLYGON_OFFSET_FILL:
        break;
    default:
#ifdef WIN32
        OutputDebugStringA("WRONG STATE for StateGroupRaster::createState\n");
#endif
        return NULL;
    }
    m_states.push_back(new State(state, value) );
    return m_states.back();
}

/*************************************************************************/ /**
 ** 
 ** 
 **/ /*************************************************************************/ 
StateGroupRaster::State* StateGroupRaster::createState(GLenum state, float value)
{
    switch(state)
    {
#ifndef OGLES2
    case GL_POINT_SIZE:
        break;
#endif
    case GL_LINE_WIDTH:
        break;
    case GL_POLYGON_OFFSET_FACTOR:
        {
            State*ps = findState(state);
            if(!ps) {
                ps = new State(state);
                m_states.push_back(ps);
            }
            ps->fvals[0] = value;
            return ps;
        }
        break;
    case GL_POLYGON_OFFSET_UNITS:
        {
            State*ps = findState(GL_POLYGON_OFFSET_FACTOR);
            if(!ps) {
                ps = new State(GL_POLYGON_OFFSET_FACTOR);
                m_states.push_back(ps);
            }
            ps->fvals[1] = value;
            return ps;
        }
        break;
    default:
#ifdef WIN32
        OutputDebugStringA("WRONG STATE for StateGroupRaster::createState\n");
#endif
        return NULL;
    }
    m_states.push_back(new State(state, value) );
    return m_states.back();
}
/*************************************************************************/ /**
 ** 
 ** 
 **/ /*************************************************************************/ 
StateGroupRaster::State* StateGroupRaster::findState(GLenum state)
{
    StateVec::iterator it = m_states.begin();
    while(it != m_states.end())
    {
        if((*it)->state == state)
            return *it;
        ++it;
    }
    return NULL;
}
/*************************************************************************/ /**
 ** 
 ** 
 **/ /*************************************************************************/ 
StateGroupRaster::State* StateGroupRaster::getState(int i)
{
    if(i >= (int)m_states.size())
        return NULL;
    return m_states[i];
}
/*************************************************************************/ /**
 ** In OpenGL, validate is not mandatory.
 ** But it will be used when we will use the extension for state-groups...
 **/ /*************************************************************************/ 
bool StateGroupRaster::validate()
{
    return false;
}

/***************************************************************************/
/* StateGroupCS StateGroupCS StateGroupCS StateGroupCS StateGroupCS        */
/***************************************************************************/

/*************************************************************************/ /**
 ** 
 ** 
 **/ /*************************************************************************/ 
StateGroupCS::~StateGroupCS()
{
    StateVec::iterator it = m_states.begin();
    while(it != m_states.end())
    {
        if(*it)
            delete *it;
        ++it;
    }
    m_states.clear();
}

/*************************************************************************/ /**
 ** 
 ** 
 **/ /*************************************************************************/ 
StateGroupCS::StateGroupCS(Container* pCont, const char *name)
{
    m_container = pCont;
    if(name)
        m_name = name;
}

/*************************************************************************/ /**
 ** 
 ** 
 **/ /*************************************************************************/ 
StateGroupCS::State* StateGroupCS::createState(GLenum state, GLenum value)
{
    switch(state)
    {
#ifndef OGLES2
    case GL_ALPHA_TEST:
        break;
    case GL_ALPHA_TEST_FUNC:
        {
            State*ps = findState(state);
            if(ps) return ps;
            m_states.push_back(ps = new State(state, value) );
            return ps;
        }
    case GL_BLEND_SRC:
#endif
    case GL_BLEND_SRC_RGB:
        {
            State*ps = findState(state);
            if(!ps) {
                ps = new State(state);
                m_states.push_back(ps);
            }
            ps->values[0] = value;
            return ps;
        }
        break;
    case GL_BLEND_SRC_ALPHA:
        {
            State*ps = findState(GL_BLEND_SRC_RGB);
            if(!ps) {
                ps = new State(GL_BLEND_SRC_RGB);
                m_states.push_back(ps);
            }
            ps->values[2] = value;
            m_states.push_back(new State(state, value) ); // still adding although not used later (except for serialization)
            return ps;
        }
        break;
#ifndef OGLES2
    case GL_BLEND_DST:
        {
            State*ps = findState(GL_BLEND_SRC);
            if(!ps) {
                ps = new State(GL_BLEND_SRC);
                m_states.push_back(ps);
            }
            ps->values[1] = value;
            m_states.push_back(new State(state, value) ); // still adding although not used later (except for serialization)
            return ps;
        }
        break;
#endif
    case GL_BLEND_DST_RGB:
        {
            State*ps = findState(GL_BLEND_SRC_RGB);
            if(!ps) {
                ps = new State(GL_BLEND_SRC_RGB);
                m_states.push_back(ps);
            }
            ps->values[1] = value;
            return ps;
        }
        break;
    case GL_BLEND_DST_ALPHA:
        {
            State*ps = findState(GL_BLEND_SRC_RGB);
            if(!ps) {
                ps = new State(GL_BLEND_SRC_RGB);
                m_states.push_back(ps);
            }
            ps->values[3] = value;
            m_states.push_back(new State(state, value) ); // still adding although not used later (except for serialization)
            return ps;
        }
        break;
    //case GL_BLEND_EQUATION_RGBA_NV:
    case GL_BLEND_EQUATION_RGB:
        {
            State*ps = findState(state);
            if(!ps) {
                ps = new State(state);
                m_states.push_back(ps);
            }
            ps->values[0] = value;
            return ps;
        }
        break;
    case GL_BLEND_EQUATION_ALPHA:
        {
            State*ps = findState(GL_BLEND_EQUATION_RGB);
            if(!ps) {
                ps = new State(GL_BLEND_EQUATION_RGB);
                m_states.push_back(ps);
            }
            ps->values[1] = value;
            m_states.push_back(new State(state, value) ); // still adding although not used later (except for serialization)
            return ps;
        }
        break;
    //case GL_PER_DRAW_BUFFER_BLEND_FUNCTION_NV:
    //case GL_PER_DRAW_BUFFER_BLEND_EQUATION_NV:
    //case GL_PER_DRAW_BUFFER_BLEND_ENABLE_NV:
    //case GL_PER_DRAW_BUFFER_COLOR_WRITEMASK_NV:
#ifndef OGLES2
    case GL_FRAMEBUFFER_SRGB:
    case GL_COLOR_LOGIC_OP:
    case GL_LOGIC_OP_MODE:
    case GL_MULTISAMPLE:
    case GL_SAMPLE_MASK:
    case GL_SAMPLE_ALPHA_TO_ONE:
#endif
    case GL_DITHER:
    case GL_SAMPLE_ALPHA_TO_COVERAGE:
        break;
    case GL_BLEND:
    //case GL_COLOR_WRITEMASK:
#ifndef OGLES2
    case GL_SAMPLE_MASK_VALUE:
#endif
    //case GL_POLYGON_STIPPLE_BIT:
        break;
    default:
#ifdef WIN32
        OutputDebugStringA("WRONG STATE for StateGroupCS::createState\n");
#endif
        return NULL;
    }
    m_states.push_back(new State(state, value) );
    return m_states.back();
}

/*************************************************************************/ /**
 ** 
 ** 
 **/ /*************************************************************************/ 
StateGroupCS::State* StateGroupCS::createState(GLenum state, float x, float y, float z, float w)
{
    switch(state)
    {
    case GL_BLEND_COLOR:
        break;
    default:
#ifdef WIN32
        OutputDebugStringA("WRONG STATE for StateGroupCS::createState\n");
#endif
        return NULL;
    }
    m_states.push_back(new State(state, x,y,z,w) );
    return m_states.back();
}

/*************************************************************************/ /**
 ** 
 ** 
 **/ /*************************************************************************/ 
StateGroupCS::State* StateGroupCS::createState(GLenum state, GLboolean x, GLboolean y, GLboolean z, GLboolean w)
{
    switch(state)
    {
    case GL_COLOR_WRITEMASK:
        break;
    default:
#ifdef WIN32
        OutputDebugStringA("WRONG STATE for StateGroupCS::createState\n");
#endif
        return NULL;
    }
    m_states.push_back(new State(state, x,y,z,w) );
    return m_states.back();
}

/*************************************************************************/ /**
 ** 
 ** 
 **/ /*************************************************************************/ 
StateGroupCS::State* StateGroupCS::createState(GLenum state, float value)
{
#ifndef OGLES2
    switch(state)
    {
    case GL_ALPHA_TEST_REF:
        {
            State*ps = findState(GL_ALPHA_TEST_FUNC);
            if(!ps) {
                ps = new State(GL_ALPHA_TEST_FUNC);
                m_states.push_back(ps);
            }
            ps->fvalue = value;
            // let's still create it, although it won't be executed (serialization will need it)
            State* ps2 = new State(state);
            ps2->fvalue = value;
            m_states.push_back(ps2);
            return ps;
        }
        break;
    default:
#ifdef WIN32
        OutputDebugStringA("WRONG STATE for StateGroupCS::createState\n");
#endif
        return NULL;
    }
    State*ps = new State(state);
    ps->fvalue = value;
    m_states.push_back(ps);
    return m_states.back();
#else
    return NULL;
#endif
}
/*************************************************************************/ /**
 ** 
 ** 
 **/ /*************************************************************************/ 
StateGroupCS::State* StateGroupCS::findState(GLenum state)
{
    StateVec::iterator it = m_states.begin();
    while(it != m_states.end())
    {
        if((*it)->state == state)
            return *it;
        ++it;
    }
    return NULL;
}
/*************************************************************************/ /**
 ** 
 ** 
 **/ /*************************************************************************/ 
StateGroupCS::State* StateGroupCS::getState(int i)
{
    if(i >= (int)m_states.size())
        return NULL;
    return m_states[i];
}
/*************************************************************************/ /**
 ** In OpenGL, validate is not mandatory.
 ** But it will be used when we will use the extension for state-groups...
 **/ /*************************************************************************/ 
bool StateGroupCS::validate()
{
    return false;
}

/***************************************************************************/
/*  StateGroupDST StateGroupDST StateGroupDST StateGroupDST StateGroupDST  */
/***************************************************************************/

/*************************************************************************/ /**
 ** 
 ** 
 **/ /*************************************************************************/ 
StateGroupDST::~StateGroupDST()
{
    StateVec::iterator it = m_states.begin();
    while(it != m_states.end())
    {
        if(*it)
            delete *it;
        ++it;
    }
    m_states.clear();
}

/*************************************************************************/ /**
 ** 
 ** 
 **/ /*************************************************************************/ 
StateGroupDST::StateGroupDST(Container* pCont, const char *name)
{
    m_container = pCont;
    if(name)
        m_name = name;
}

/*************************************************************************/ /**
 ** 
 ** 
 **/ /*************************************************************************/ 
StateGroupDST::State* StateGroupDST::createState(GLenum state, GLenum value)
{
    switch(state)
    {
    case GL_DEPTH_TEST:
    case GL_DEPTH_FUNC:
    case GL_DEPTH_WRITEMASK:
#ifndef OGLES2
    case GL_DEPTH_CLAMP:
    case GL_DEPTH_BOUNDS_TEST_EXT:
    case GL_DEPTH_BOUNDS_EXT:
#endif
    case GL_STENCIL_TEST:
        break;

    case GL_STENCIL_FAIL:
        {
            State*ps = findState(state);
            if(ps) return ps;
            m_states.push_back(ps = new State(state, value) );
            return ps;
        }
    case GL_STENCIL_PASS_DEPTH_FAIL:
        {
            State*ps = StateGroupDST::findState(GL_STENCIL_FAIL);
            if(!ps){
                ps = new State(GL_STENCIL_FAIL);
                m_states.push_back(ps);
            }
            ps->values[0] = value;
            m_states.push_back(new State(state, value) );
            return ps;
        }
    case GL_STENCIL_PASS_DEPTH_PASS:
        {
            State*ps = StateGroupDST::findState(GL_STENCIL_FAIL);
            if(!ps){
                ps = new State(GL_STENCIL_FAIL);
                m_states.push_back(ps);
            }
            ps->values[1] = value;
            m_states.push_back(new State(state, value) );
            return ps;
        }

    //case GL_STENCIL_FRONT_FUNC:
    //case GL_STENCIL_FRONT_FAIL:
    //case GL_STENCIL_FRONT_PASS_DEPTH_FAIL:
    //case GL_STENCIL_FRONT_PASS_DEPTH_PASS:

    case GL_STENCIL_BACK_FAIL:
        {
            State*ps = findState(state);
            if(ps) return ps;
            m_states.push_back(ps = new State(state, value) );
            return ps;
        }
    case GL_STENCIL_BACK_PASS_DEPTH_FAIL:
        {
            State*ps = StateGroupDST::findState(GL_STENCIL_BACK_FAIL);
            if(!ps){
                ps = new State(GL_STENCIL_BACK_FAIL);
                m_states.push_back(ps);
            }
            ps->values[0] = value;
            m_states.push_back(new State(state, value) );
            return ps;
        }
    case GL_STENCIL_BACK_PASS_DEPTH_PASS:
        {
            State*ps = StateGroupDST::findState(GL_STENCIL_BACK_FAIL);
            if(!ps){
                ps = new State(GL_STENCIL_BACK_FAIL);
                m_states.push_back(ps);
            }
            ps->values[1] = value;
            m_states.push_back(new State(state, value) );
            return ps;
        }

    case GL_STENCIL_WRITEMASK:
    case GL_STENCIL_BACK_WRITEMASK:
    //case GL_STENCIL_FRONT_VALUE_MASK:
    //case GL_STENCIL_FRONT_WRITEMASK:
    //case GL_STENCIL_FRONT_REF:
        break;
    case GL_STENCIL_BACK_FUNC:
        {
            State*ps = findState(state);
            if(ps) return ps;
            ps = new State(state, value);
            m_states.push_back(ps);
            break;
        }
    case GL_STENCIL_BACK_VALUE_MASK:
        {
            State*ps = StateGroupDST::findState(GL_STENCIL_BACK_FUNC);
            if(!ps){
                ps = new State(GL_STENCIL_BACK_FUNC);
                m_states.push_back(ps);
            }
            ps->values[1] = value;
            m_states.push_back(ps);
            m_states.push_back(new State(state, value) );
            return ps;
        }
    case GL_STENCIL_BACK_REF:
        {
            State*ps = StateGroupDST::findState(GL_STENCIL_BACK_FUNC);
            if(!ps){
                ps = new State(GL_STENCIL_BACK_FUNC);
                m_states.push_back(ps);
            }
            ps->values[0] = value;
            m_states.push_back(new State(state, value) );
            return ps;
        }
    case GL_STENCIL_FUNC:
        {
            State*ps = findState(state);
            if(ps) return ps;
            ps = new State(state, value);
            m_states.push_back(ps);
            break;
        }
    case GL_STENCIL_REF:
        {
            State*ps = StateGroupDST::findState(GL_STENCIL_FUNC);
            if(!ps){
                ps = new State(GL_STENCIL_FUNC);
                m_states.push_back(ps);
            }
            ps->values[0] = value;
            m_states.push_back(new State(state, value) );
            return ps;
        }
    case GL_STENCIL_VALUE_MASK:
        {
            State*ps = StateGroupDST::findState(GL_STENCIL_FUNC);
            if(!ps){
                ps = new State(GL_STENCIL_FUNC);
                m_states.push_back(ps);
            }
            ps->values[1] = value;
            m_states.push_back(ps);
            m_states.push_back(new State(state, value) );
            return ps;
        }
    default:
#ifdef WIN32
        OutputDebugStringA("WRONG STATE for StateGroupDST::createState\n");
#endif
        return NULL;
    }
    m_states.push_back(new State(state, value) );
    return m_states.back();
}
/*************************************************************************/ /**
 ** 
 ** 
 **/ /*************************************************************************/ 
StateGroupDST::State* StateGroupDST::findState(GLenum state)
{
    StateVec::iterator it = m_states.begin();
    while(it != m_states.end())
    {
        if((*it)->state == state)
            return *it;
        ++it;
    }
    return NULL;
}
/*************************************************************************/ /**
 ** 
 ** 
 **/ /*************************************************************************/ 
StateGroupDST::State* StateGroupDST::getState(int i)
{
    if(i >= (int)m_states.size())
        return NULL;
    return m_states[i];
}
/*************************************************************************/ /**
 ** 
 ** 
 **/ /*************************************************************************/ 
StateGroupDST::State* StateGroupDST::createState(GLenum state, float val1, float val2)
{
#ifndef OGLES2
    switch(state)
    {
    case GL_DEPTH_BOUNDS_EXT:
        break;
    default:
#ifdef WIN32
        OutputDebugStringA("WRONG STATE for StateGroupDST::createState\n");
#endif
        return NULL;
    }
    m_states.push_back(new State(state, val1, val2) );
    return m_states.back();
#else
    return NULL;
#endif
}

/*************************************************************************/ /**
 ** In OpenGL, validate is not mandatory.
 ** But it will be used when we will use the extension for state-groups...
 **/ /*************************************************************************/ 
bool StateGroupDST::validate()
{
    return false;
}

#ifndef OGLES2
/********************************************************************************/
/*  StateGroupPath StateGroupPath StateGroupPath StateGroupPath StateGroupPath  */
/********************************************************************************/

/*************************************************************************/ /**
 ** 
 ** 
 **/ /*************************************************************************/ 
StateGroupPath::~StateGroupPath()
{
    StateVec::iterator it = m_states.begin();
    while(it != m_states.end())
    {
        if(*it)
            delete *it;
        ++it;
    }
    m_states.clear();
}

/*************************************************************************/ /**
 ** 
 ** 
 **/ /*************************************************************************/ 
StateGroupPath::StateGroupPath(Container* pCont, const char *name)
{
    m_container = pCont;
    if(name)
        m_name = name;
}

/*************************************************************************/ /**
 ** 
 ** 
 **/ /*************************************************************************/ 
StateGroupPath::State* StateGroupPath::findState(GLenum state)
{
    StateVec::iterator it = m_states.begin();
    while(it != m_states.end())
    {
        if((*it)->state == state)
            return *it;
        ++it;
    }
    return NULL;
}
/*************************************************************************/ /**
 ** 
 ** 
 **/ /*************************************************************************/ 
StateGroupPath::State* StateGroupPath::createState(GLenum state, float val)
{
    switch(state)
    {
    case GL_PATH_STENCIL_DEPTH_OFFSET_FACTOR_NV:
      {
        State* ps = findState(GL_PATH_STENCIL_DEPTH_OFFSET_FACTOR_NV);
        if(ps) {
            ps->fval[0] = val;
            return ps;
        }
        m_states.push_back(new State(state, val, 0.0) );
        return m_states.back();
      }
    case GL_PATH_STENCIL_DEPTH_OFFSET_UNITS_NV:
      {
        State* ps = findState(GL_PATH_STENCIL_DEPTH_OFFSET_FACTOR_NV);
        if(ps) {
            ps->fval[1] = val;
            return ps;
        }
        m_states.push_back(new State(state, 0.0, val) );
        return m_states.back();
      }
    case GL_PATH_STROKE_WIDTH_NV:
    case GL_PATH_MITER_LIMIT_NV:
    case GL_PATH_DASH_OFFSET_NV:
    case GL_PATH_CLIENT_LENGTH_NV:
        break;
    default:
#ifdef WIN32
        OutputDebugStringA("WRONG STATE for StateGroupPath::createState\n");
#endif
        return NULL;
    }
    State* ps = findState(state);
    if(ps) {
        ps->fval[0] = val;
        return ps;
    }
    m_states.push_back(new State(state, val) );
    return m_states.back();
}

/*************************************************************************/ /**
 ** 
 ** 
 **/ /*************************************************************************/ 
StateGroupPath::State* StateGroupPath::createState(GLenum state, int val)
{
    switch(state)
    {
    case GL_PATH_FILL_MASK_NV:
        break;
    default:
#ifdef WIN32
        OutputDebugStringA("WRONG STATE for StateGroupPath::createState\n");
#endif
        return NULL;
    }
    State* ps = findState(state);
    if(ps) {
        ps->ival = val;
        return ps;
    }
    m_states.push_back(new State(state, val) );
    return m_states.back();
}

/*************************************************************************/ /**
 ** 
 ** 
 **/ /*************************************************************************/ 
StateGroupPath::State* StateGroupPath::createState(GLenum state, GLenum val)
{
    switch(state)
    {
    case GL_PATH_STENCIL_FUNC_NV:
    case GL_PATH_FOG_GEN_MODE_NV://             FOG_COORDINATE|FRAGMENT_DEPTH
    case GL_PATH_COVER_DEPTH_FUNC_NV://    depth func enums

    case GL_PATH_INITIAL_END_CAP_NV://     GL_FLAT_NV|GL_SQUARE_NV|GL_ROUND_NV|GL_TRIANGULAR_NV
    case GL_PATH_TERMINAL_END_CAP_NV://    GL_FLAT_NV|GL_SQUARE_NV|GL_ROUND_NV|GL_TRIANGULAR_NV
    case GL_PATH_INITIAL_DASH_CAP_NV://    GL_FLAT_NV|GL_SQUARE_NV|GL_ROUND_NV|GL_TRIANGULAR_NV
    case GL_PATH_TERMINAL_DASH_CAP_NV://   GL_FLAT_NV|GL_SQUARE_NV|GL_ROUND_NV|GL_TRIANGULAR_NV
    case GL_PATH_JOIN_STYLE_NV://          GL_MITER_REVERT_NV|GL_MITER_TRUNCATE_NV|GL_BEVEL_NV|GL_ROUND_NV|GL_NONE
    case GL_PATH_DASH_OFFSET_RESET_NV://   GL_MOVE_TO_RESET_NV|GL_MOVE_TO_CONTINUES_NV

    case GL_PATH_FILL_MODE_NV://           GL_COUNT_UP_NV|GL_INVERT| GL_COUNT_DOWN_NV
    case GL_PATH_FILL_COVER_MODE_NV://     GL_BOUNDING_BOX_NV|GL_CONVEX_HULL_NV
    case GL_PATH_STROKE_COVER_MODE_NV://   GL_BOUNDING_BOX_NV|GL_CONVEX_HULL_NV
        break;
    default:
#ifdef WIN32
        OutputDebugStringA("WRONG STATE for StateGroupPath::createState\n");
#endif
        return NULL;
    }
    State* ps = findState(state);
    if(ps) {
        ps->value = val;
        return ps;
    }
    m_states.push_back(new State(state, val) );
    return m_states.back();
}

/*************************************************************************/ /**
 ** In OpenGL, validate is not mandatory.
 ** But it will be used when we will use the extension for state-groups...
 **/ /*************************************************************************/ 
bool StateGroupPath::validate()
{
    return false;
}
#endif

