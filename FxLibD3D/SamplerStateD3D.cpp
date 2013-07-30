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
#include <fstream>
#include <assert.h>
#include "Fx.h"
#include "FxD3D.h"
#include "OpenGL_enums.h"
#pragma warning(disable:4996)
using namespace nvFX;

#pragma MESSAGE("TODO TODO TODO TODO : Implement ALL the D3D SamplerState parameters")

/***************************************************************************/
/*  SamplerStateD3D SamplerStateD3D SamplerStateD3D SamplerStateD3D    */
/***************************************************************************/

inline D3D1X_TEXTURE_ADDRESS_MODE TexAddrModeGL2D3D1X(GLenum e)
{
    switch(e)
    {
    case GL_REPEAT: return D3D1X(TEXTURE_ADDRESS_WRAP);
    case GL_MIRRORED_REPEAT: return D3D1X(TEXTURE_ADDRESS_MIRROR);
    case GL_CLAMP: return D3D1X(TEXTURE_ADDRESS_CLAMP);
    case GL_CLAMP_TO_BORDER: return D3D1X(TEXTURE_ADDRESS_BORDER);
    //case : return D3D1X(TEXTURE_ADDRESS_MIRROR_ONCE);
    }
    return D3D1X(TEXTURE_ADDRESS_CLAMP);
}

/*************************************************************************/ /**
 ** 
 ** 
 **/ /*************************************************************************/ 
SamplerStateD3D::SamplerStateD3D(Container* pCont, const char* name) :
    SamplerState(pCont, name)
{
}
SamplerStateD3D::~SamplerStateD3D()
{
    ID3D1XSamplerState *samplerState = static_cast<ID3D1XSamplerState *>(m_data);
    if(!samplerState)
        samplerState->Release();
    m_data = NULL;
}

/*************************************************************************/ /**
 ** 
 ** see http://www.opengl.org/sdk/docs/man/xhtml/glTexParameter.xml
 ** \todo arguments need unit # in order to work with glBindSampler.
 ** or: create a new method for the case of >= OGL 3.3
 **/ /*************************************************************************/ 
void SamplerStateD3D::update(GLenum target, GLint tex, bool bindTexture)
{
    // Nothing to do : OpenGL-specific call
    // return false; ?
}

/*************************************************************************/ /**
 ** 
 ** 
 **/ /*************************************************************************/ 
SamplerState*  SamplerStateD3D::update2(void *data, Pass *pass, bool bBindProgram, bool bCreateIfNeeded)
{
    int id;
    for(int i=0; (id=pass->getLayerId(i)) >= 0; i++)
        update(data, pass, i, bBindProgram, bCreateIfNeeded);
    return this;
}

/*************************************************************************/ /**
 ** \brief this update method is called internally to perform the "update"
 ** i.e. the activation of the created D3D sampler state.for the target
 **/ /*************************************************************************/ 
SamplerState*  SamplerStateD3D::update(void *data, Pass *pass, int layerID, bool bBindProgram, bool bCreateIfNeeded)
{
    if(layerID < 0)
        return update2(data, pass, bBindProgram, bCreateIfNeeded);
#ifdef USE_D3D11
    ID3D11DeviceContext *pd3d1X;
    ((ID3D1XDevice *)nvFX::getDevice())->GetImmediateContext(&pd3d1X);
#else
    ID3D1XDevice *pd3d1X = (ID3D1XDevice *)nvFX::getDevice();
#endif
    ID3D1XSamplerState *samplerState = (ID3D1XSamplerState *)data;
    if(samplerState == NULL)
    {
        if(!validate())
            return NULL;
        samplerState = static_cast<ID3D1XSamplerState *>(m_data);
    }
    for(int i=0; i<(int)m_targets.size(); i++)
    {
        // NOTE: I do realize here that the D3D device should be located in the target itself
        // for later use : when a samplerstate contains data for many targets (CUDA + D3D etc.)
        // for now this is Ok : we don't do funky mixes
        STarget &t = m_targets[i];
        if((pass == NULL) || ((t.pass == pass) && (t.passLayerId == layerID)))
        {
            // The fact we can have vtx or pix (and others) in one program is for the case of USE_OLDPROGRAM (to align with OpenGL approach)
            // but in non USE_OLDPROGRAM, a program will always have ONLY ONE of these guys at a time
            switch(t.ttype)
            {
            case THLSL_VTX: pd3d1X->VSSetSamplers(t.index, 1, &samplerState); break;
            case THLSL_PIX: pd3d1X->PSSetSamplers(t.index, 1, &samplerState); break;
            //case THLSL_GS: pd3d1X->GSSetSamplers(t.index, 1, &samplerState); break;
            //case THLSL_HS: pd3d1X->HSSetSamplers(t.index, 1, &samplerState); break;
            //case THLSL_DS: pd3d1X->DSSetSamplers(t.index, 1, &samplerState); break;
            }
        }
    }
    //
    // a pass ptr was specified and nothing found, we shall try to find the uniform in the pass program
    //
    if(pass && bCreateIfNeeded)
    {
        STarget t;
        int p = 0;
        D3DShaderProgram *program = static_cast<D3DShaderProgram*>(pass->getProgram(layerID));
        IProgramPipeline* programPipeline = pass->getProgramPipeline(layerID);
        if(programPipeline)
            program = static_cast<D3DShaderProgram*>(programPipeline->getShaderProgram(p++));
        while(program)
        {
            t.index = -1;
            t.dirty = false;
            t.valid = false;
            t.pass = pass;
            t.passLayerId = layerID;
            t.ttype = ShaderFlags2Type(program->getProgramShaderFlags());
            t.shaderProgramNumber = p-1; // we store the shader program number : corresponding to a separable vertex shader, frag. shader or other...
            //
            // Use reflection to gather information
            //
            D3D11_SHADER_DESC sd;
            D3D1X_SHADER_INPUT_BIND_DESC r;
            if(program->m_data.reflector)
            {
                program->m_data.reflector->GetDesc((D3D1X_SHADER_DESC*)&sd);
                for(int i=0; i<(int)sd.BoundResources; i++)
                {
                    program->m_data.reflector->GetResourceBindingDesc(i, &r);
                    if(r.Type == D3D10_SIT_SAMPLER) // type is still D3D10_SHADER_INPUT_TYPE
                    {
                        if(!strcmp(r.Name, m_name.c_str()))
                        {
                            t.index = r.BindPoint;
                            switch(t.ttype)
                            {
                            case THLSL_VTX: pd3d1X->VSSetSamplers(t.index, 1, &samplerState); break;
                            case THLSL_PIX: pd3d1X->PSSetSamplers(t.index, 1, &samplerState); break;
                            //case THLSL_GS: pd3d1X->GSSetSamplers(t.index, 1, &samplerState); break;
                            //case THLSL_HS: pd3d1X->HSSetSamplers(t.index, 1, &samplerState); break;
                            //case THLSL_DS: pd3d1X->DSSetSamplers(t.index, 1, &samplerState); break;
                            }
                            break;
                        }
                    }
                }
            }
            // Store the target if the shader needed it
            if(t.index >= 0)
            {
                t.valid = true;
                // NOTE: maybe we should use a std::map<pass,target> for targets...
                int i = 0;
                for(i=0; i<(int)m_targets.size(); i++)
                    if((m_targets[i].pass == pass)
                    && (m_targets[i].passLayerId == layerID)
                    && (m_targets[i].ttype == t.ttype)
                    && (m_targets[i].shaderProgramNumber == t.shaderProgramNumber))
                    {
                        m_targets[i] = t;
                        break;
                    }
                if(i == m_targets.size())
                {
                    m_targets.push_back(t);
                    pass->addSamplerState(this, i);
                }
            }
            //
            // Move to the next program is available
            //
            if(programPipeline)
                program = static_cast<D3DShaderProgram*>(programPipeline->getShaderProgram(p++));
            else
                program = NULL;
        } //while(program)

    }
    return this;
}
SamplerState*  SamplerStateD3D::updateForTarget(void *data, STarget &t, bool bBindProgram)
{
    if(!t.valid)
        return this;
#ifdef USE_D3D11
    ID3D11DeviceContext *pd3d1X;
    ((ID3D1XDevice *)nvFX::getDevice())->GetImmediateContext(&pd3d1X);
#else
    ID3D1XDevice *pd3d1X = (ID3D1XDevice *)nvFX::getDevice();
#endif
    ID3D1XSamplerState *samplerState = (ID3D1XSamplerState *)data;
    if(samplerState == NULL)
    {
        if(!validate())
            return NULL;
        samplerState = static_cast<ID3D1XSamplerState *>(m_data);
    }
    // The fact we can have vtx or pix (and others) in one program is for the case of USE_OLDPROGRAM (to align with OpenGL approach)
    // but in non USE_OLDPROGRAM, a program will always have ONLY ONE of these guys at a time
    switch(t.ttype)
    {
    case THLSL_VTX: pd3d1X->VSSetSamplers(t.index, 1, &samplerState); break;
    case THLSL_PIX: pd3d1X->PSSetSamplers(t.index, 1, &samplerState); break;
    //case THLSL_GS: pd3d1X->GSSetSamplers(t.index, 1, &samplerState); break;
    //case THLSL_HS: pd3d1X->HSSetSamplers(t.index, 1, &samplerState); break;
    //case THLSL_DS: pd3d1X->DSSetSamplers(t.index, 1, &samplerState); break;
    }
    return this;
}

bool  SamplerStateD3D::validate()
{
    static D3D1X_FILTER filterD3D1X[] = {
        // MIN as NEAREST (POINT) :
          // MAG as NEAREST:
          D3D1X(FILTER_MIN_MAG_MIP_POINT),           // No MIP (we don't have this under D3D)
          D3D1X(FILTER_MIN_MAG_MIP_POINT),           // MIP NEAREST
          D3D1X(FILTER_MIN_MAG_POINT_MIP_LINEAR),    // MIP LINEAR
          // MAG as LINEAR:
          D3D1X(FILTER_MIN_POINT_MAG_LINEAR_MIP_POINT), // No MIP (we don't have this under D3D)
          D3D1X(FILTER_MIN_POINT_MAG_LINEAR_MIP_POINT),
          D3D1X(FILTER_MIN_POINT_MAG_MIP_LINEAR),
        // MIN as LINEAR
          // MAG as NEAREST
          D3D1X(FILTER_MIN_LINEAR_MAG_MIP_POINT),
          D3D1X(FILTER_MIN_LINEAR_MAG_MIP_POINT),
          D3D1X(FILTER_MIN_LINEAR_MAG_POINT_MIP_LINEAR),
          // MAG as LINEAR
          D3D1X(FILTER_MIN_MAG_LINEAR_MIP_POINT), // No MIP (well...)
          D3D1X(FILTER_MIN_MAG_LINEAR_MIP_POINT), // MIP NEAREST
          D3D1X(FILTER_MIN_MAG_MIP_LINEAR),       // MIP LINEAR
        /*D3D1X(FILTER_ANISOTROPIC),
        D3D1X(FILTER_COMPARISON_MIN_MAG_MIP_POINT),
        D3D1X(FILTER_COMPARISON_MIN_MAG_POINT_MIP_LINEAR),
        D3D1X(FILTER_COMPARISON_MIN_POINT_MAG_LINEAR_MIP_POINT),
        D3D1X(FILTER_COMPARISON_MIN_POINT_MAG_MIP_LINEAR),
        D3D1X(FILTER_COMPARISON_MIN_LINEAR_MAG_MIP_POINT),
        D3D1X(FILTER_COMPARISON_MIN_LINEAR_MAG_POINT_MIP_LINEAR),
        D3D1X(FILTER_COMPARISON_MIN_MAG_LINEAR_MIP_POINT),
        D3D1X(FILTER_COMPARISON_MIN_MAG_MIP_LINEAR),
        D3D1X(FILTER_COMPARISON_ANISOTROPIC),
        D3D1X(FILTER_TEXT_1BIT)*/
    };
    ID3D1XDevice *pd3d1X = (ID3D1XDevice *)nvFX::getDevice();
    ID3D1XSamplerState *samplerState = static_cast<ID3D1XSamplerState *>(m_data);
    if(!samplerState)
    {
        HRESULT hr;
        int minoffset = 0;
        int magoffset = 0;
        int mipoffset = 0;
        //
        // NOTE: so far we drive filter mode and Address Mode... ONLY... TODO: handle the rest
        //
        D3D1X_SAMPLER_DESC ssdesc = {
            D3D1X(FILTER_MIN_MAG_MIP_POINT), //D3D1X(FILTER )Filter;
            D3D1X(TEXTURE_ADDRESS_CLAMP), //D3D1X(TEXTURE_ADDRESS_MODE )AddressU;
            D3D1X(TEXTURE_ADDRESS_CLAMP), //D3D1X(TEXTURE_ADDRESS_MODE )AddressV;
            D3D1X(TEXTURE_ADDRESS_CLAMP), //D3D1X(TEXTURE_ADDRESS_MODE )AddressW;
            0.0f, //FLOAT MipLODBias;
            16,//UINT MaxAnisotropy;
            D3D1X(COMPARISON_NEVER),//D3D1X(COMPARISON_FUNC )ComparisonFunc;
            {0.0f, 0.0f, 0.0f, 0.0f},//FLOAT BorderColor[ 4 ];
            0,//FLOAT MinLOD;
            D3D1X(FLOAT32_MAX)//FLOAT MaxLOD;
        };
        StateVec::iterator iState = m_states.begin();
        //
        // WARNING: COMPARISON & ANISOTROPIC *NOT* handled, yet !
        //
        while(iState != m_states.end())
        {
            switch((*iState)->sstate)
            {
            case GL_TEXTURE_MAG_FILTER:
                {
                    switch((*iState)->value)
                    {
                    case GL_NEAREST_MIPMAP_LINEAR: // shall we raise a warning ?
                    case GL_NEAREST_MIPMAP_NEAREST:
                    case GL_NEAREST:
                        magoffset = 0;
                        break;
                    case GL_LINEAR_MIPMAP_NEAREST:
                    case GL_LINEAR_MIPMAP_LINEAR:
                    case GL_LINEAR:
                        magoffset = 1;
                        break;
                    default:
                        nvFX::printf("Error in SamplerState : Wrong MIN_FILTER assignment\n");
                        return false;
                    }
                }
                break;
            case GL_TEXTURE_MIN_FILTER:
                {
                    switch((*iState)->value)
                    {
                    case GL_NEAREST:
                        minoffset = 0;
                        mipoffset = 0;
                        break;
                    case GL_NEAREST_MIPMAP_NEAREST:
                        minoffset = 0;
                        mipoffset = 1;
                        break;
                    case GL_NEAREST_MIPMAP_LINEAR:
                        minoffset = 0;
                        mipoffset = 2;
                        break;
                    case GL_LINEAR:
                        minoffset = 1;
                        mipoffset = 0;
                        break;
                    case GL_LINEAR_MIPMAP_NEAREST:
                        minoffset = 1;
                        mipoffset = 1;
                        break;
                    case GL_LINEAR_MIPMAP_LINEAR:
                        minoffset = 1;
                        mipoffset = 2;
                        break;
                    default:
                        nvFX::printf("Error in SamplerState : Wrong MAG_FILTER assignment\n");
                        return false;
                    }
                }
                break;
            case GL_TEXTURE_WRAP_S:
                ssdesc.AddressU = TexAddrModeGL2D3D1X((*iState)->value);
                break;
            case GL_TEXTURE_WRAP_T:
                ssdesc.AddressV = TexAddrModeGL2D3D1X((*iState)->value);
                break;
            case GL_TEXTURE_WRAP_R:
                ssdesc.AddressW = TexAddrModeGL2D3D1X((*iState)->value);
                break;
            }
            ++iState;
        }
        ssdesc.Filter = filterD3D1X[minoffset*6 + magoffset*3 + mipoffset];
        hr = pd3d1X->CreateSamplerState(&ssdesc, &samplerState);
        if(FAILED(hr))
            return false;
        m_data = samplerState;
    }
    return true;
}

bool SamplerStateD3D::setD3DSamplerState(void *p)
{
    ID3D1XDevice *pd3d1X = (ID3D1XDevice *)nvFX::getDevice();
    ID3D1XSamplerState *pSS = (ID3D1XSamplerState *)p;
    if(m_data)
    {
        ID3D1XSamplerState *samplerState = static_cast<ID3D1XSamplerState *>(m_data);
        samplerState->Release();
        m_data = NULL;
    }
    m_data = pSS;
    if(pSS)
        pSS->AddRef();
    return true;
}

