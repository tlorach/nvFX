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

CstBufferD3D::CstBufferD3D(const char* name) :
    CstBuffer(name)
{
}

CstBufferD3D::~CstBufferD3D()
{
    ID3D1XBuffer* pBuf = (ID3D1XBuffer*)m_pBufferInterface;
    if(pBuf)
        pBuf->Release();
    pBuf = NULL;
}

void CstBufferD3D::updateD3D(STarget &t)
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
            program->m_data.reflector->GetDesc((D3D1X_SHADER_DESC*)&sd);
            for(i=0; i<(int)sd.ConstantBuffers; i++)
            {
                D3D1X_SHADER_BUFFER_DESC bufDesc;
                pCst = program->m_data.reflector->GetConstantBufferByIndex(i);
                pCst->GetDesc(&bufDesc);
                if(m_name == std::string(bufDesc.Name))
                    break;
                pCst = NULL;
            }
            if(pCst)
            {
                t.bufferIndex = i;
                t.valid = true;
            }
            pCst = NULL;
        } //if(!t.valid)
        if(t.valid)
        {
            //
            // First, make sure that the buffer is correctly updated : sub-uniforms may have changed
            // dirty here means that the buffer may require data update for this target.
            // Note that once it got updated, all the targets of this CstBuffer must be dirty=false
            // Which is hacky : we will have to propagage this flag to other targets. To avoid redundancy
            // Why is that ? Because the original target design wasn't made for CstBuffer + sub-uniforms...
            //
            if(m_pBufferInterface && t.dirty) // a sub-uniform would have set it to true...
            {
                // this method will set dirty flags to false. So we expect the update to happen once
                // even if we have > 1 targets (when many passes)
                updateFromUniforms();
            }
            //
            // then set the buffer to the shaders
            //
#ifdef USE_D3D11
            ID3D11DeviceContext *pd3d1X;
            ((ID3D1XDevice *)nvFX::getDevice())->GetImmediateContext(&pd3d1X);
#else
            ID3D1XDevice *pd3d1X = (ID3D1XDevice *)nvFX::getDevice();
#endif
            switch(t.ttype)
            {
            case THLSL_VTX: pd3d1X->VSSetConstantBuffers(t.bufferIndex, 1, (ID3D1XBuffer**)&m_pBufferInterface); break;
            case THLSL_PIX: pd3d1X->PSSetConstantBuffers(t.bufferIndex, 1, (ID3D1XBuffer**)&m_pBufferInterface); break;
            //case THLSL_GS: pd3d1X->GSSetConstantBuffers(t.bufferIndex, 1, (ID3D1XBuffer**)&m_pBufferInterface); break;
            //case THLSL_HS: pd3d1X->HSSetConstantBuffers(t.bufferIndex, 1, (ID3D1XBuffer**)&m_pBufferInterface); break;
            //case THLSL_DS: pd3d1X->DSSetConstantBuffers(t.bufferIndex, 1, (ID3D1XBuffer**)&m_pBufferInterface); break;
            }
        }
        t.dirty = false;
    }
}

CstBuffer*    CstBufferD3D::update2(Pass *pass, bool bBindProgram, bool bCreateIfNeeded, bool bCreateBufferIfNeeded)
{
    int id;
    for(int i=0; (id=pass->getLayerId(i)) >= 0; i++)
        update(pass, id, bBindProgram, bCreateIfNeeded, bCreateBufferIfNeeded);
    return this;
}

CstBuffer*    CstBufferD3D::update(Pass *pass, int layerID, bool bBindProgram, bool bCreateIfNeeded, bool bCreateBufferIfNeeded)
{
    if(layerID < 0)
        return update2(pass, bBindProgram, bCreateIfNeeded, bCreateBufferIfNeeded);
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
            // At execution time, if a Cst Buffer is missing, we take the liberty to create one for you
            if(bCreateBufferIfNeeded && (m_pBufferInterface == NULL))
                buildD3DBuffer(ICstBuffer::D3D_USAGE_IMMUTABLE);
            if(pass)
            { // In this case we only update matching pass and return
                if((t.pass == pass) && (t.passLayerId == layerID))
                {
                    updateD3D(t);
                    return this;
                }
            } else {
                updateD3D(t);
                LOGI(">Warning : uniform update for many targets at the same time... normal ?");
            }
        case TDXCOMPUTE:
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
// REMOVE THIS !!!! SET to NULL (everywhere else, too)
        D3DShaderProgram *program = static_cast<D3DShaderProgram*>(pass->getProgram(layerID));
        IProgramPipeline* programPipeline = pass->getProgramPipeline(layerID);
        if(programPipeline)
            program = static_cast<D3DShaderProgram*>(programPipeline->getShaderProgram(p++));
        while(program)
        {
            t.valid = false;
            t.pass = pass;
            t.passLayerId = layerID;
            t.ttype = ShaderFlags2Type(program->getProgramShaderFlags());
            t.shaderProgramNumber = p-1; // we store the shader program number : corresponding to a separable vertex shader, frag. shader or other...
            updateD3D(t);
            // add this target only if it is needed
            if(t.valid)
            {
                // NOTE: maybe we should use a std::map<pass,target> for targets...
                int i = 0;
                for(i=0; i<(int)m_targets.size(); i++)
                    if((m_targets[i].pass == pass) 
                    && (m_targets[i].ttype == t.ttype)
                    && (m_targets[i].passLayerId == t.passLayerId)
                    && (m_targets[i].shaderProgramNumber == t.shaderProgramNumber))
                    {
                        m_targets[i] = t;
                        break;
                    }
                if(i == m_targets.size())
                {
                    m_targets.push_back(t);
                    pass->addCstBuffer(this, i);
                }
            }
            //
            // Move to the next program is available
            //
            if(programPipeline)
                program = static_cast<D3DShaderProgram*>(programPipeline->getShaderProgram(p++));
            else
                program = NULL;
        }

    }
    return this;
}

CstBuffer* CstBufferD3D::updateForTarget(STarget &t, bool bBindProgram)
{
    if(!t.valid)
        return this;
    switch(t.ttype)
    {
    case THLSL_VTX:
    case THLSL_PIX:
    case THLSL_GS:
    case THLSL_HS:
    case THLSL_DS:
        // At execution time, if a Cst Buffer is missing, we take the liberty to create one for you
        if(m_pBufferInterface == NULL)
            buildD3DBuffer(ICstBuffer::D3D_USAGE_IMMUTABLE);
        updateD3D(t);
        break;
    case TDXCOMPUTE:
        break;
    }
    return this;
}

/*************************************************************************/ /**
** \brief computes the size and eventually fills data in the memory at right offset
**
**/ /*************************************************************************/
int CstBufferD3D::bufferSizeAndData(char *pData, Uniform* pU, int &szBytes, int &pk)
{
    int szType = pU->getSizeOfType();
    int szArray = pU->getArraySz();
    // special case : arrays are always 4 vecs aligned
    if(szArray > 1)
    {
        if(pk > 0)
            szBytes += 16 - pk;
        pk = 0;
        switch(pU->getType())
        {
        case nvFX::IUniform::TMat2:
            szType = 16 * 2;
            break;
        case nvFX::IUniform::TMat3:
            szType = 16 * 3;
            break;
        default:
            if(szType < 16) // float, float2, float3...
                szType = 16;
            break;
        }
        szBytes += szType * szArray;
    } else {
        // NOTE: float3x3 to CHECK !!!
        if((pk > 0)&&(pk + szType > 16))
        {
            szBytes += 16 - pk;
            pk = 0;
        }
        //
        // put the Uniform values
        //
        if(pData)
            pU->getValueRaw(pData + szBytes, 0); // 0 means we don't care about any limit in pData...

        pk += szType;
        pk = pk % 16;
        szBytes += szType;
    }
    return szBytes;
}
/*************************************************************************/ /**
** \brief computes the size and eventually fills data in the memory at right offset
**
** \todo Shall we work on the various packing options that allow more 
** efficient alignment ?
**
** for now http://msdn.microsoft.com/en-us/library/windows/desktop/bb509632(v=vs.85).aspx
** - similar to performing a #pragma pack 4 with Visual Studio
** - in arrays : every element in an array is stored in a four-component vector
**/ /*************************************************************************/
#pragma MESSAGE("TODO TODO TODO TODO : CstBufferD3D::bufferSizeAndData : Check all packing cases (Float3x3...)")
int CstBufferD3D::bufferSizeAndData(char *pData, int *sz)
{
    //
    // Calculate the totaly size of the buffer to allocate
    // and/or eventually add values to the buffer
    //
    int szBytes = 0;
    int pk = 0;
    UniformVec::iterator iU = m_uniforms.begin();
    while(iU != m_uniforms.end())
    {
        Uniform *pU = *iU;
        bufferSizeAndData(pData, pU, szBytes, pk);
        ++iU;
    }
    return szBytes + (16 - pk);
}
/*************************************************************************/ /**
** \brief build a D3D buffer for you
**
**/ /*************************************************************************/
void* CstBufferD3D::buildD3DBuffer(ICstBuffer::BufferUsageD3D usage)
{
    ID3D1XBuffer* pBuf = (ID3D1XBuffer*)m_pBufferInterface;
    if(pBuf)
        pBuf->Release();
    pBuf = NULL;
    //
    // Calculate the totaly size of the buffer to allocate
    //
    int szBytes = bufferSizeAndData(NULL);
    //
    // setup the default values
    //
    void * pTemp = malloc(szBytes);
    bufferSizeAndData((char*)pTemp);

    D3D1X_SUBRESOURCE_DATA defaultData;
    defaultData.pSysMem = pTemp;
    defaultData.SysMemPitch = 0;
    defaultData.SysMemSlicePitch = 0;
    //
    // Create the buffer
    //
    D3D1X_BUFFER_DESC cbDesc;
    cbDesc.ByteWidth = szBytes;
    cbDesc.Usage = (D3D1X_USAGE)usage;
    cbDesc.BindFlags = D3D1X(BIND_CONSTANT_BUFFER);
    cbDesc.CPUAccessFlags = D3D1X(CPU_ACCESS_WRITE);
    cbDesc.MiscFlags = 0;
    ID3D1XDevice *pd3d1X = (ID3D1XDevice *)nvFX::getDevice();
    HRESULT hr = pd3d1X->CreateBuffer( &cbDesc, &defaultData, &pBuf);
    free(pTemp);
    if(FAILED(hr))
    {
        DebugBreak();
        return NULL;
    }
    setD3DBuffer(pBuf);
    //
    // The creation did write existing values from the sub-uniforms
    // we need to set dirty flag to false
    //
    for(int u=0; u<(int)m_uniforms.size(); u++)
    {
        Uniform* pU = m_uniforms[u];
        pU->setDirty(false, NULL);
    }
    for(int i=0; i<(int)m_targets.size(); i++)
        m_targets[i].dirty = false;

    return m_pBufferInterface;
}

/// \brief  assigns the ptr... BUT does not increase the reference count (D3D overidden part will do) !
ICstBuffer*    CstBufferD3D::setD3DBuffer(void* buffer)
{
    ID3D1XBuffer* pBuf = (ID3D1XBuffer*)m_pBufferInterface;
    if(pBuf)
        pBuf->Release();

    CstBuffer::setD3DBuffer(buffer);

    pBuf = (ID3D1XBuffer*)buffer;
    if(pBuf)
        pBuf->AddRef();
    return this;
}

/*************************************************************************/ /**
** \brief update a buffer from a uniform that may have changed.
** as opposed to OpenGL, there is no way map a sub-range of the buffer.
** so it could be slow... but new versions of DX11 should solve this. To see...
**/ /*************************************************************************/
bool CstBufferD3D::updateFromUniforms(bool bForceUpdating)
{
    ID3D1XBuffer* pBuf = (ID3D1XBuffer*)m_pBufferInterface;
    if(!pBuf)
        return false;
    //
    // gather the dirty flags of uniforms and reset them
    //
    bool bDirty = false;
    for(int u=0; u<(int)m_uniforms.size(); u++)
    {
        Uniform* pU = m_uniforms[u];
        if((!bDirty) && pU->getDirty(NULL))
            bDirty = true;
        pU->setDirty(false, NULL);
    }
    if(!bDirty)
        return true;
    //
    // Now map the buffer and do the updates
    //
    char *cb;
#ifdef USE_D3D11
    ID3D11DeviceContext *pd3d1X;
    D3D11_MAPPED_SUBRESOURCE mappedSR;
    ((ID3D1XDevice *)nvFX::getDevice())->GetImmediateContext(&pd3d1X);
    HRESULT hr = pd3d1X->Map(pBuf, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedSR);
    cb = (char*)mappedSR.pData;
#else
    HRESULT hr = pBuf->Map(D3D1X(MAP_WRITE_DISCARD), 0, (void**)&cb);
#endif
    if(FAILED(hr))
        return false;
    int sz = bufferSizeAndData(cb);
#ifdef USE_D3D11
    pd3d1X->Unmap(pBuf, 0);
#else
    pBuf->Unmap();
#endif
    //
    // The buffer is clean
    //
    for(int i=0; i<(int)m_targets.size(); i++)
    {
        m_targets[i].dirty = false;
    }
    return true;
}

bool        CstBufferD3D::mapBuffer(void** ppData, bool bForceUpdating)
{
    ID3D1XBuffer* pBuf = (ID3D1XBuffer*)m_pBufferInterface;
    if(!pBuf)
        return false;
    //
    // gather the dirty flags and reset them
    //
    bool bDirty = false;
    for(int u=0; u<(int)m_uniforms.size(); u++)
    {
        Uniform* pU = m_uniforms[u];
        if((!bDirty) && pU->getDirty(NULL))
            bDirty = true;
        pU->setDirty(false, NULL);
    }
    //
    // Map the buffer anyways : for users-purpose
    //
#ifdef USE_D3D11
    ID3D11DeviceContext *pd3d1X;
    D3D11_MAPPED_SUBRESOURCE mappedSR;
    ((ID3D1XDevice *)nvFX::getDevice())->GetImmediateContext(&pd3d1X);
    HRESULT hr = pd3d1X->Map(pBuf, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedSR);
    *ppData = mappedSR.pData;
#else
    HRESULT hr = pBuf->Map(D3D1X(MAP_WRITE_DISCARD), 0, ppData);
#endif
    if(FAILED(hr))
        return false;
    //
    // Update from uniforms only if dirty flag is true
    //
    if(bDirty)
        bufferSizeAndData((char*)*ppData);
    return true;
}
void        CstBufferD3D::unmapBuffer()
{
    ID3D1XBuffer* pBuf = (ID3D1XBuffer*)m_pBufferInterface;
    if(!pBuf)
        return;
#ifdef USE_D3D11
    ID3D11DeviceContext *pd3d1X;
    ((ID3D1XDevice *)nvFX::getDevice())->GetImmediateContext(&pd3d1X);
    pd3d1X->Unmap(pBuf, 0);
#else
    pBuf->Unmap();
#endif
}

