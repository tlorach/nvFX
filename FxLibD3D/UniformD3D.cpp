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

/*************************************************************************
 **
 ** UniformD3D UniformD3D UniformD3D UniformD3D UniformD3D UniformD3D
 ** UniformD3D UniformD3D UniformD3D UniformD3D UniformD3D UniformD3D
 **
 *************************************************************************/ 
UniformD3D::UniformD3D(const char* name, const char* groupname, const char* semantic) :
    Uniform(name, groupname, semantic)
{
}
/*************************************************************************/ /**
 ** 
 ** 
 **/ /*************************************************************************/ 
UniformD3D::~UniformD3D()
{
    Uniform::~Uniform();
    if(m_data) 
    {
#ifndef OGLES2
        if(m_type == TSubroutineUniform)
            for(int i=0; i<16; i++)
                if(m_data->subroutineNames[i]) delete [] m_data->subroutineNames[i];
#endif
    }
}
/*************************************************************************
 **
 **  method invoked by update()
 **
 *************************************************************************/ 
void UniformD3D::updateD3D(ShadowedData *pData, STarget &t, bool bBindProgram)
{
    if(t.pass)
    {
        // Here we take into account either the single-program case, or the separable shader case
        D3DShaderProgram *program = static_cast<D3DShaderProgram*>(t.pass->getProgram(t.passLayerId));
        IProgramPipeline* programPipeline = t.pass->getProgramPipeline(t.passLayerId);
        if(programPipeline)
            program = static_cast<D3DShaderProgram*>(programPipeline->getShaderProgram(t.shaderProgramNumber));
        if(!t.valid)
        {
            //
            // parameter not validated, yet : use Reflection interface to get the location in the shaders
            //
            t.bufferIndex = -1;
            ID3D1XShaderReflectionConstantBuffer *pCst = NULL;
            int i;
            // Because I couldn't use IID_ID3D1XShaderReflection, I used D3D11 instead...
            // TODO: fix this. See D3DShaderProgram::bind()
            D3D11_SHADER_DESC sd;
            if(program->m_data.reflector)
            {
                program->m_data.reflector->GetDesc((D3D1X(SHADER_DESC)*)&sd);
                for(i=0; i<(int)sd.ConstantBuffers; i++)
                {
                    pCst = program->m_data.reflector->GetConstantBufferByIndex(i);
                    D3D1X(SHADER_BUFFER_DESC) bufDesc;
                    pCst->GetDesc(&bufDesc);
                    if(m_name == std::string(bufDesc.Name))
                        break;
                    pCst = NULL;
                }
                if(pCst)
                {
                    t.bufferIndex = i;
                    m_type = Uniform::TCB; // force the type because it obviously is a constant buffer
                    t.valid = true;
                }
            }
            pCst = NULL;
        } //if(!t.valid)
        //
        // Now let's try with textures (we take textures as part of the 'uniforms' family)
        //
        if(!t.valid)
        {
            //
            // Use reflection to gather information
            //
            D3D11_SHADER_DESC sd;
            D3D1X_SHADER_INPUT_BIND_DESC r;
            r.Name = NULL;
            if(program->m_data.reflector)
            {
                int i;
                program->m_data.reflector->GetDesc((D3D1X_SHADER_DESC*)&sd);
                for(i=0; i<(int)sd.BoundResources; i++)
                {
                    program->m_data.reflector->GetResourceBindingDesc(i, &r);
                    if(r.Type == D3D10_SIT_TEXTURE) // type is still D3D10_SHADER_INPUT_TYPE
                    {
                        if(!strcmp(r.Name, m_name.c_str()))
                            break;
                    }
                    r.Name = NULL;
                }
                if(r.Name)
                {
                    t.uniformLocation = r.BindPoint;
                    // TODO: check consistency with the uniform type
                    //m_type = ... ?
                    t.valid = true;
                }
            }
        }
#ifdef USE_D3D11
        ID3D11DeviceContext *pd3d1X;
        ((ID3D1XDevice *)nvFX::getDevice())->GetImmediateContext(&pd3d1X);
#else
        // need to find a way to access the device !
        ID3D1XDevice *pd3d1X = (ID3D1XDevice *)nvFX::getDevice();
#endif
        if(t.valid && pData) switch(m_type)
        {
            case Uniform::TFloat:
                assert(1);
                break;
            case Uniform::TInt:
                assert(1);
            //case Uniform::TUInt:
            //case Uniform::TBool:
                break;
            case Uniform::TVec2:
                assert(1);
                break;
            case Uniform::TVec3:
                assert(1);
                break;
            case Uniform::TVec4:
                assert(1);
                break;
            case Uniform::TMat4:
                assert(1);
                break;
            case Uniform::TSubroutineUniform:
                assert(1);
                break;
            case Uniform::TTexture:
            case IUniform::TTexture1D:
            case IUniform::TTexture2D:
            case IUniform::TTexture2DRect:
            case IUniform::TTexture3D:
            case IUniform::TTextureCube:
                {
                ID3D1XShaderResourceView *pShaderResourceView = NULL;
                // case where we attached 'by hand' a resource view directly to the uniform
                if(pData->tex.m_d3dResourceView)
                    pShaderResourceView = (ID3D1XShaderResourceView *)pData->tex.m_d3dResourceView;
                // case where the resource is managed by nvFX::Resource class
                else if(pData->tex.pRes)
                    pShaderResourceView = (ID3D1XShaderResourceView *)static_cast<ResourceD3D*>(pData->tex.pRes)->m_pTextureView;
                if(pShaderResourceView)
                {
                    switch(t.ttype)
                    {
                    case THLSL_VTX: pd3d1X->VSSetShaderResources(t.uniformLocation, 1, &pShaderResourceView); break;
                    case THLSL_PIX: pd3d1X->PSSetShaderResources(t.uniformLocation, 1, &pShaderResourceView); break;
                    case THLSL_GS:  pd3d1X->GSSetShaderResources(t.uniformLocation, 1, &pShaderResourceView); break;
                    //case THLSL_HS: pd3d1X->xxSetShaderResources(t.index, 1, &pShaderResourceView); break;
                    //case THLSL_DS: pd3d1X->xxSetShaderResources(t.index, 1, &pShaderResourceView); break;
                    }
                    break;
                }
                }
                break;
            case Uniform::TUBO:
                assert(1);
                break;
            case Uniform::TCB:
                {
                    switch(t.ttype)
                    {
                    case THLSL_VTX: pd3d1X->VSSetConstantBuffers(t.bufferIndex, 1, (ID3D1XBuffer**)&pData->pVoid); break;
                    case THLSL_PIX: pd3d1X->PSSetConstantBuffers(t.bufferIndex, 1, (ID3D1XBuffer**)&pData->pVoid); break;
                    //case THLSL_GS: pd3d1X->xxSetConstantBuffers(t.bufferIndex, 1, (ID3D1XBuffer**)&pData->pVoid); break;
                    //case THLSL_HS: pd3d1X->xxSetConstantBuffers(t.bufferIndex, 1, (ID3D1XBuffer**)&pData->pVoid); break;
                    //case THLSL_DS: pd3d1X->xxSetConstantBuffers(t.bufferIndex, 1, (ID3D1XBuffer**)&pData->pVoid); break;
                    }
                }
                break;
            case Uniform::TUniform:
            default:
                assert(1);
                break;
        }
        //if(bBindProgram) glslProgram->unbind();
        t.dirty = false;
    }
}

/*************************************************************************
 **
 **  update a uniform
 **
 *************************************************************************/ 
Uniform*    UniformD3D::update2(ShadowedData* pData, Pass *pass, bool bBindProgram, bool bCreateIfNeeded)
{
    int id;
    for(int i=0; (id=pass->getLayerId(i)) >= 0; i++)
        update(pData, pass, id, bBindProgram, bCreateIfNeeded);
    return this;
}
/*************************************************************************
 **
 **  update a uniform
 **
 *************************************************************************/ 
Uniform*    UniformD3D::update(ShadowedData* pData, Pass *pass, int layerID, bool bBindProgram, bool bCreateIfNeeded)
{
    static bool msgOnceCstBuffer = true;
    if(layerID < 0)
        return update2(pData, pass, bBindProgram, bCreateIfNeeded);
    for(int i=0; i<(int)m_targets.size(); i++)
    {
        STarget &t = m_targets[i];
        if(!t.valid)
            continue;
        switch(t.ttype)
        {
        case THLSL_VTX:
        case THLSL_PIX:
        case THLSL_GS:
        case THLSL_HS:
        case THLSL_DS:
            if(pass)
            { // In this case we only update matching pass and return
                if((t.pass == pass)&&(t.passLayerId == layerID))
                {
                    updateD3D(pData, t, bBindProgram);
                    return this;
                }
            } else {
                updateD3D(pData, t, true/*bBindProgram*/); // if updating all, we need to bind programs in any case
                LOGI(">Warning : uniform update for many targets at the same time... normal ?");
            }
        case TDXCOMPUTE:
            break;
        case TCSTBUFFER:
            assert(t.pBuffer);
            // special case of target being an intermediate constant buffer need shadowed data anyways
            if(!m_data)
                m_data = new ShadowedData;
            memcpy(m_data, pData, sizeof(ShadowedData));
            t.pBuffer->setDirty(true, pass);
            // this case is not ideal. So let's tell it
            if(msgOnceCstBuffer)
            {
                DebugBreak();
                LOGI(">Warning : update on a uniform that belongs to a Constant buffer is not so efficient (many map/unmap could occur)\n");
                msgOnceCstBuffer = false;
            }
            t.pBuffer->updateFromUniforms(true);
            break;
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
            t.dirty = false;
            t.shadertype = 0;
            t.valid = false;
            t.pass = pass;
            t.passLayerId = layerID;
            t.ttype = ShaderFlags2Type(program->getProgramShaderFlags());
            t.shaderProgramNumber = p-1;
            t.pBuffer = NULL;
            t.uniformLocation = -1;
            t.bufferIndex = 0;
            updateD3D(pData, t, false);
            // add this target only if it is needed
            if(t.valid)
            {
                // NOTE: maybe we should use a std::map<pass,target> for targets...
                int i = 0;
                for(i=0; i<(int)m_targets.size(); i++)
                    if((m_targets[i].pass == pass) && (m_targets[i].ttype == t.ttype) && (m_targets[i].passLayerId == t.passLayerId))
                    {
                        m_targets[i] = t;
                        break;
                    }
                if(i == m_targets.size())
                {
                    m_targets.push_back(t);
                    pass->addUniform(this, i);
                }
            }
            //
            // Move to the next program is available
            //
            if(programPipeline)
                program = static_cast<D3DShaderProgram*>(programPipeline->getShaderProgram(p++));
            else
                program = NULL;
        }//while(program)
    }
    return this;
}

Uniform*    UniformD3D::updateForTarget(ShadowedData* pData, STarget &t, bool bBindProgram)
{
    static bool msgOnceCstBuffer = true;
    if(!t.valid)
        return this;
    switch(t.ttype)
    {
    case THLSL_VTX:
    case THLSL_PIX:
    case THLSL_GS:
    case THLSL_HS:
    case THLSL_DS:
        updateD3D(pData, t, bBindProgram);
        break;
    case TDXCOMPUTE:
        break;
    case TCSTBUFFER:
        assert(t.pBuffer);
        // special case of target being an intermediate constant buffer need shadowed data anyways
        if(!m_data)
            m_data = new ShadowedData;
        memcpy(m_data, pData, sizeof(ShadowedData));
        t.pBuffer->setDirty(true, static_cast<Pass*>(t.pass));
        // this case is not ideal. So let's tell it
        if(msgOnceCstBuffer)
        {
            DebugBreak();
            LOGI(">Warning : update on a uniform that belongs to a Constant buffer is not so efficient (many map/unmap could occur)\n");
            msgOnceCstBuffer = false;
        }
        t.pBuffer->updateFromUniforms(true);
        break;
    }
    return this;
}

