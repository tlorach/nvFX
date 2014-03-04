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

#ifdef USECUDA
std::map<int, CUDABuffer>	buffers;
#endif

CstBufferGL::CstBufferGL(const char* name) :
#ifdef USECUDA
    CstBufferCUDA(name)
#else
    CstBuffer(name)
#endif
{
}

CstBufferGL::~CstBufferGL()
{
    removeGLBuffer();
}

/*************************************************************************
 **
 **  method invoked by update()
 **
 *************************************************************************/ 
void CstBufferGL::updateGLSL(STarget &t, bool bBindProgram)
{
    NXPROFILEFUNCCOL2(m_name.c_str(), COLOR_ORANGE, 11);
    if(t.pass)
    {
        // Here we take into account either the single-program case, or the separable shader case
        GLSLProgram *glslProgram = static_cast<GLSLProgram*>(t.pass->getProgram(t.passLayerId));
        GLSLProgramPipeline* glslProgramPipeline = static_cast<GLSLProgramPipeline*>(t.pass->getProgramPipeline(t.passLayerId));
        if(glslProgramPipeline)
            glslProgram = static_cast<GLSLProgram*>(glslProgramPipeline->getShaderProgram(t.shaderProgramNumber));
        int prog = glslProgram->getProgram();
//#ifdef USE_OLDPROGRAM
        if(bBindProgram && !glslProgramPipeline)
            glslProgram->bind(t.pass->m_container);
        GLint dbgProg = 0;
        //glGetIntegerv(GL_CURRENT_PROGRAM, &dbgProg);
        //assert(prog == dbgProg);
//#endif
        if(!t.valid)
        {
#ifndef OGLES2
            if(glGetUniformBlockIndex)
                t.uniformLocation = glGetUniformBlockIndex(glslProgram->getProgram(), m_name.c_str());
#endif
            t.valid = t.uniformLocation < 0 ? false : true;
        }
        if(t.valid)
        {
            //
            // First, make sure that the buffer is correctly updated : sub-uniforms may have changed
            // dirty here means that the buffer may require data update for this target.
            // Note that once it got updated, all the targets of this CstBuffer must be dirty=false
            // Which is hacky : we will have to propagage this flag to other targets. To avoid redundancy
            // Why is that ? Because the original target design wasn't made for CstBuffer + sub-uniforms...
            //
            if((m_bufferId) && t.dirty) // a sub-uniform would have set it to true...
            {
                // this method will set dirty flags to false. So we expect the update to happen once
                // even if we have > 1 targets (when many passes)
                updateFromUniforms();
            }
#ifndef OGLES2
            if(m_bufferId)
            {
                //GLint res = 111;
                //glGetActiveUniformBlockiv(prog,t.uniformLocation, GL_UNIFORM_BLOCK_BINDING, &res);
                //glUniformBlockBinding(prog, t.uniformLocation, t.uniformLocation);
                if(m_bufferOffset > 0)
                    glBindBufferRange(GL_UNIFORM_BUFFER, t.uniformLocation, m_bufferId, m_bufferOffset*m_sizeOfCstBuffer, m_sizeOfCstBuffer);
                else
                    glBindBufferBase(GL_UNIFORM_BUFFER, t.uniformLocation, m_bufferId);
                CHECKGLERRORS("Uniform::TUBO");
            }
#endif
        }
//#ifdef USE_OLDPROGRAM
        if(bBindProgram && !glslProgramPipeline) glslProgram->unbind(t.pass->m_container);
//#endif
        t.dirty = false;
    }
}

CstBuffer*    CstBufferGL::update2(Pass *pass, bool bBindProgram, bool bCreateIfNeeded, bool bCreateBufferIfNeeded)
{
    if (NULL == pass)
      return this;
    int id;
    for(int i=0; (id=pass->getLayerId(i)) >= 0; i++)
        update(pass, id, bBindProgram, bCreateIfNeeded, bCreateBufferIfNeeded);
    return this;
}

CstBuffer*    CstBufferGL::update(Pass *pass, int layerID, bool bBindProgram, bool bCreateIfNeeded, bool bCreateBufferIfNeeded)
{
  if (NULL == pass)
      return this;
    NXPROFILEFUNCCOL2(__FUNCTION__, COLOR_ORANGE, 11);
#ifndef OGLES2
    if(layerID < 0)
        return update2(pass, bBindProgram, bCreateIfNeeded, bCreateBufferIfNeeded);
    bool bCreate = pass ? bCreateIfNeeded : false;
    int tsz = (int)m_targets.size();
    for(int i=0; i<tsz; i++)
    {
        STarget &t = m_targets[i];
        if(!t.valid)
            continue;
        switch(t.ttype)
        {
        case TCUDA:
#ifdef USECUDA
            CstBufferCUDA::update(pass, layerID, bBindProgram, bCreateIfNeeded, bCreateBufferIfNeeded);
#endif
            break;
        case TGLSL:
            // first, test if a buffer got allocated/assigned. If not we need one !
            if((m_bufferId == 0) && bCreateBufferIfNeeded)
                buildGLBuffer(ICstBuffer::STATIC_DRAW, 1);
            if(pass)
            { // In this case we only update matching pass and return
                if((t.pass == pass) && (t.passLayerId == layerID))
                {
                    updateGLSL(t, bBindProgram);
                    bCreate = false;
                    break;
                }
            } else {
                updateGLSL(t, true/*bBindProgram*/); // if updating all, we need to bind programs in any case
                LOGI(">Warning : uniform update for many targets at the same time... normal ?");
            }
        }
    }
    //
    // a pass ptr was specified and nothing found, we shall try to find the uniform in the pass program
    //
    if(bCreate)
    {
        STarget t;
        int p = 0;
        // Here we take into account either the single-program case, or the separable shader case
        GLSLProgram *glslProgram = static_cast<GLSLProgram*>(pass->getProgram(layerID));
        GLSLProgramPipeline* glslProgramPipeline = static_cast<GLSLProgramPipeline*>(pass->getProgramPipeline(layerID));
        if(glslProgramPipeline)
            glslProgram = static_cast<GLSLProgram*>(glslProgramPipeline->getShaderProgram(p++));
        //
        // Loops as many times as we have programs (when separable)
        //
        while(glslProgram)
        {
//#ifdef USE_OLDPROGRAM
            int prevProg = 0; //glGetIntegerv(GL_CURRENT_PROGRAM, &prevProg);
            if(bBindProgram && !glslProgramPipeline) // we may want to remove this : no need to bind anything for uniform blocks
                glslProgram->bind(pass->m_container);
//#endif
// PENDING ISSUE PENDING ISSUE PENDING ISSUE PENDING ISSUE PENDING ISSUE PENDING ISSUE
// PENDING ISSUE PENDING ISSUE PENDING ISSUE PENDING ISSUE PENDING ISSUE PENDING ISSUE
// I need to know how to only track uniform blocks that are really used by the source code !!!!
// related to http://nvbugs/910435 ??
#pragma MESSAGE("returns indices of uniform blocks *even* when not used at all... related to http://nvbugs/910435 ?")
            if(glGetUniformBlockIndex
                && ((t.uniformLocation = glGetUniformBlockIndex(glslProgram->getProgram(), m_name.c_str()))>=0))
            {
//int ii[40];
//int nn;
//char nam[40];
//int loc;
//int activeUbos[10];
//glGetProgramiv(glslProgram->getProgram(), GL_ACTIVE_UNIFORM_BLOCKS, activeUbos);
//glGetActiveUniformBlockiv(glslProgram->getProgram(), t.uniformLocation, GL_UNIFORM_BLOCK_ACTIVE_UNIFORMS, &nn);
//glGetActiveUniformBlockiv(glslProgram->getProgram(), t.uniformLocation, GL_UNIFORM_BLOCK_ACTIVE_UNIFORM_INDICES, ii);
//for(int i=0; i<nn; i++)
//{
//    GLsizei length;
//    GLint size;
//    GLenum type;
//    glGetActiveUniform(glslProgram->getProgram(), ii[i],
//                                  39, 
//                                  &length,
//                                  &size, 
//                                  &type, 
//                                  nam);
//    loc = glGetUniformLocation(glslProgram->getProgram(), nam);
//}
                // we must bind this block index to our own number... yeah... let's take the same
                // http://www.opengl.org/sdk/docs/man4/xhtml/glUniformBlockBinding.xml
                glUniformBlockBinding(glslProgram->getProgram(), t.uniformLocation, t.uniformLocation);
                CHECKGLERRORS("Uniform::TUBO");
                t.valid = t.uniformLocation < 0 ? false : true;
                t.pass = pass;
                t.passLayerId = layerID;
                t.ttype = TGLSL;
                t.shaderProgramNumber = p-1; // we store the shader program number : corresponding to a separable vertex shader, frag. shader or other...
                // NOTE: maybe we should use a std::map<pass,target> for targets...
                int i = 0;
                int tsz = (int)m_targets.size();
                for(i=0; i<tsz; i++)
                    if((m_targets[i].pass == pass) 
                        && (m_targets[i].ttype == t.ttype) 
                        && (m_targets[i].passLayerId == t.passLayerId)
                        && (m_targets[i].shaderProgramNumber == t.shaderProgramNumber))
                    {
                        m_targets[i] = t;
                        break;
                    }
                if(i == tsz)
                {
                    m_targets.push_back(t);
                    pass->addCstBuffer(this, i);
                }
                updateGLSL(t, false);
//#ifdef USE_OLDPROGRAM
                if(bBindProgram && !glslProgramPipeline)
                    glUseProgram(prevProg);//glslProgram->unbind();
//#endif
            }
//#ifdef USE_OLDPROGRAM
            if(bBindProgram && !glslProgramPipeline) // we may want to remove this : no need to bind anything for uniform blocks
                if(bBindProgram) glUseProgram(prevProg);//glslProgram->unbind();
//#endif
            //
            // Move to the next program is available
            //
            if(glslProgramPipeline)
                glslProgram = static_cast<GLSLProgram*>(glslProgramPipeline->getShaderProgram(p++));
            else
                glslProgram = NULL;
        } //while(glslProgram)
    }//if(pass && bCreateIfNeeded)
#else
    //
    // In the case of ES2 : we simply bypass the buffer and take the sub-uniforms to directly work with them in GLES2
    //
    UniformVec::iterator iU = m_uniforms.begin();
    while(iU != m_uniforms.end())
    {
        Uniform *pU = *iU;
        pU->update(pU->getShadowedData(), pass, layerID, bBindProgram, bCreateIfNeeded);
        ++iU;
    }
#endif
    return this;
}
/*************************************************************************
 **
 **  
 **
 *************************************************************************/ 
CstBuffer*    CstBufferGL::updateForTarget(CstBuffer::STarget &t, bool bBindProgram)
{
    //assert(t.valid);
    CHECK_TRUE(t.valid);
    //if(!t.valid)
    //    return this;
    switch(t.ttype)
    {
    case TCUDA:
        //TODO (and filter this if pass != Null
        break;
    case TGLSL:
        if((m_bufferId == 0))// && bCreateBufferIfNeeded)
            buildGLBuffer(ICstBuffer::STATIC_DRAW, 1);
        CHECK_TRUE(m_bufferId!=0);
        updateGLSL(t, bBindProgram);
    }
    return this;
}

int CstBufferGL::bufferSizeAndData(char *pData, Uniform* pU, int &szBytes, int &pk)
{
    if (NULL == pU)
      return 0;
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
            if(pData) pU->getValueRaw(pData + szBytes, 0, 0); // 0 means we don't care about any limit in pData...
            break;
        case nvFX::IUniform::TMat3:
            szType = 16 * 3;
            if(pData) pU->getValueRaw(pData + szBytes, 0, 0); // 0 means we don't care about any limit in pData...
            break;
        default:
            if(pData)
                pU->getValueRaw(pData + szBytes, 0, 16-szType); // 0 means we don't care about any limit in pData...
            if(szType < 16) // float, float2, float3...
                szType = 16;
            break;
        }
        szBytes += szType * szArray;
    } else {
        // NOTE: float3x3 to CHECK !!! And other cases...
        if(((szType == 12)&&(pk > 0)) // for vec3
        || ((pk > 0)&&(pk + szType > 16)) )
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
    return szBytes;// + (pk > 0 ? (16 - pk) : 0); we don't add to round to 16 bytes. Not in this function
}
/*************************************************************************/ /**
** \brief computes the size and eventually fills data in the memory at right offset
**
** \todo Shall we work on the various packing options that allow more 
** efficient alignment ?
**
** - similar to performing a #pragma pack 4 with Visual Studio
** - in arrays : every element in an array is stored in a four-component vector
**/ /*************************************************************************/
int CstBufferGL::bufferSizeAndData(char *pData, int *sz)
{
#pragma MESSAGE("TODO TODO TODO TODO : CstBufferGL::bufferSizeAndData : Check the integrity of this part. Came from D3D version and I am pretty sure something is wrong for tricky cases")
    //
    // Calculate the totaly size of the buffer to allocate
    // and/or eventually add values to the buffer
    //
    int szBytes = 0;
    int pk = 0;
    UniformVec::iterator iU = m_uniforms.begin();
    while(iU != m_uniforms.end())
    {
        bufferSizeAndData(pData, *iU, szBytes, pk);
        ++iU;
    }
    if(sz) *sz = szBytes; // needed for CstBuffer offset computation...
    return szBytes + (pk > 0 ? (16 - pk) : 0);
}
/*************************************************************************/ /**
** \brief build an OpenGL buffer for you
**
**/ /*************************************************************************/
int CstBufferGL::buildGLBuffer(CstBufferGL::BufferUsageGL usage, int sizeMultiplier)
{
    NXPROFILEFUNCCOL2(__FUNCTION__, COLOR_RED, 11);
	if(sizeMultiplier > 1)
		m_sizeMultiplier = sizeMultiplier;
	else
		m_sizeMultiplier = 1;
#ifndef OGLES2
    //
    // Calculate the totaly size of the buffer to allocate
    //
    m_sizeOfCstBuffer = bufferSizeAndData(NULL);
    //
    // setup the default values
    //
    if(m_stagingBuffer)
        free(m_stagingBuffer);
    m_stagingBuffer = malloc(m_sizeOfCstBuffer);
    bufferSizeAndData((char*)m_stagingBuffer);

    //
    // Create the buffer
    //
    unsigned int buf;
    glGenBuffers(1, &buf);
    glBindBuffer(GL_UNIFORM_BUFFER, buf);
#if 0
    glBufferData(GL_UNIFORM_BUFFER, m_sizeOfCstBuffer * m_sizeMultiplier, NULL, usage);
    void *pData = glMapBuffer(GL_UNIFORM_BUFFER, GL_WRITE_ONLY);
    memcpy(pData, pTemp, m_sizeOfCstBuffer * m_sizeMultiplier);
    glUnmapBuffer(GL_UNIFORM_BUFFER);
#else
	// the size of the buffer can be more (m_sizeMultiplier>1) so we can later shift into it
    glBufferData(GL_UNIFORM_BUFFER, m_sizeOfCstBuffer * m_sizeMultiplier, m_stagingBuffer, usage);
#endif
    if(CHECKGLERRORS("CstBufferGL::buildGLBuffer"))
    {
        //DebugBreak();
        assert(0);
        return NULL;
    }
    setGLBuffer(buf);
    m_ownBufferId = true;
    //
    // The creation did write existing values from the sub-uniforms
    // we need to set dirty flag to false
    //
    for(int u=0; u<(int)m_uniforms.size(); u++)
    {
        Uniform* pU = m_uniforms[u];
        pU->setDirty(false, NULL);
    }
    int tsz = (int)m_targets.size();
    for(int i=0; i<tsz; i++)
        m_targets[i].dirty = false;

    return buf;
#else
    return 0;
#endif
}

/*************************************************************************/ /**
** \brief  destroys the buffer if owned
**
**/ /*************************************************************************/
bool CstBufferGL::removeGLBuffer()
{
    NXPROFILEFUNCCOL2(__FUNCTION__, COLOR_RED, 11);
	m_sizeMultiplier = 1;
#ifndef OGLES2
    m_sizeOfCstBuffer = 0;
    if(m_bufferId && m_ownBufferId)
        glDeleteBuffers(1, &m_bufferId);
    m_bufferId = 0;
    m_ownBufferId = false;
    for(int u=0; u<(int)m_uniforms.size(); u++)
    {
        Uniform* pU = m_uniforms[u];
        pU->setDirty(true, NULL);
    }
    int tsz = (int)m_targets.size();
    for(int i=0; i<tsz; i++)
        m_targets[i].dirty = true;
    return true;
#else
    return false;
#endif
}
/*************************************************************************/ /**
** \brief  assigns the ptr... BUT does not increase the reference count (D3D overidden part will do) !
**
**/ /*************************************************************************/
ICstBuffer*    CstBufferGL::setGLBuffer(int buffer)
{
    if(m_bufferId == buffer)
        return this;
    if(m_bufferId)
        removeGLBuffer();

    m_bufferId = buffer;
    m_ownBufferId = false;
    int tsz = (int)m_targets.size();
    for(int i=0; i<tsz;i++)
        m_targets[i].dirty = true;
    //update(NULL, true);
    //
    // NOTE: subsequent uniforms of this buffer are then dirty for this target
    // TODO: walk through them and mark them
    //
    return this;
}

/*************************************************************************/ /**
**
**/ /*************************************************************************/
bool CstBufferGL::updateFromUniforms(bool bForceUpdating)
{
#ifndef OGLES2
    if(m_bufferId <= 0)
        return false;
    bool bDidBind = false;
#ifdef USEBUFFERSUBDATA
    NXPROFILEFUNCCOL2("upd.BufferSubData", COLOR_RED2, 11);
    for(int u=0; u<(int)m_uniforms.size(); u++)
    {
        Uniform* pU = m_uniforms[u];
        if(pU->getDirty(NULL))
        {
            if(!bDidBind) {
                glBindBuffer(GL_UNIFORM_BUFFER, m_bufferId);
                bDidBind = true;
            }
            // we expect ONE target
            int offset = pU->m_targets[0].offsetBytes;
            int szBytes = 0;
            int pk = 0;
            static char pData[4*4*sizeof(float)*16]; // TODO: allocate what is needed
            int sz = bufferSizeAndData(pData, pU, szBytes, pk);
            glBufferSubData(GL_UNIFORM_BUFFER, offset, sz, pData);
        }
        pU->setDirty(false, NULL);
    }
    if(bDidBind)
        glBindBuffer(GL_UNIFORM_BUFFER, 0);
#elif defined(USEMAPRANGE)
    NXPROFILEFUNCCOL2("upd.BufferSubData", COLOR_RED2, 11);
    for(int u=0; u<(int)m_uniforms.size(); u++)
    {
        Uniform* pU = m_uniforms[u];
        if(pU->getDirty(NULL))
        {
            if(!bDidBind) {
                glBindBuffer(GL_UNIFORM_BUFFER, m_bufferId);
                bDidBind = true;
            }
            // we expect ONE target
#pragma MESSAGE(__FILE__"(416) : TODO TODO TODO TODO : Warning... offset could be wrong in special cases. Need to test more")
            int offset = pU->m_targets[0].offsetBytes;
            int szBytes = 0;
            int pk = 0;
            int &sz = pU->m_targets[0].size;
            if(sz == 0)
            {
                sz = bufferSizeAndData(NULL, pU, szBytes, pk);
                // correction of the offset (when at setup, we didn't know how to align better)
                if((offset%16)&&((offset%16) + sz > 16))
                    offset += 16 - (offset%16);
            }
            char *pData = (char*)glMapBufferRange(GL_UNIFORM_BUFFER, offset, sz, GL_MAP_UNSYNCHRONIZED_BIT|GL_MAP_WRITE_BIT|GL_MAP_INVALIDATE_RANGE_BIT);
            szBytes = pk = 0;
            bufferSizeAndData(pData, pU, szBytes, pk);
            glUnmapBuffer(GL_UNIFORM_BUFFER);
            CHECKGLERRORS("CstBufferGL::updateFromUniforms");
        }
        pU->setDirty(false, NULL);
    }
    if(bDidBind)
        glBindBuffer(GL_UNIFORM_BUFFER, 0);
#else
    //NXPROFILEFUNCCOL2("upd.MapBuffer", COLOR_RED2, 11);
    NXPROFILEFUNCCOL2("upd.BufferSubData", COLOR_RED2, 11);
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
    glBindBuffer(GL_UNIFORM_BUFFER, m_bufferId);
#if 0
    cb = (char*)glMapBuffer(GL_UNIFORM_BUFFER, GL_WRITE_ONLY);
    if(!cb)
        return false;
    int sz = bufferSizeAndData(cb);
    glUnmapBuffer(GL_UNIFORM_BUFFER);
#else
    assert(m_stagingBuffer);
    int sz = bufferSizeAndData((char*)m_stagingBuffer);
    glBufferSubData(GL_UNIFORM_BUFFER, 0, m_sizeOfCstBuffer * m_sizeMultiplier, m_stagingBuffer);
#endif
    //
    // The buffer is clean
    //
    int tsz = (int)m_targets.size();
    for(int i=0; i<tsz; i++)
    {
        m_targets[i].dirty = false;
    }
#endif
    return true;
#else
    LOGE("OGL ES2 doesn't have constant buffers. Should never happen\n");
    assert(0);
    return false;
#endif
}

bool        CstBufferGL::mapBuffer(void** ppData, bool bForceUpdating)
{
#ifndef OGLES2
    if(m_bufferId <= 0)
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
    glBindBuffer(GL_UNIFORM_BUFFER, m_bufferId);
    *ppData = glMapBuffer(GL_UNIFORM_BUFFER, GL_WRITE_ONLY);
    if(!(*ppData))
        return false;
    //
    // Update from uniforms only if dirty flag is true
    //
    if(bDirty)
        bufferSizeAndData((char*)*ppData);
    return true;
#else
    return false;
#endif
}
void        CstBufferGL::unmapBuffer()
{
#ifndef OGLES2
    glBindBuffer(GL_UNIFORM_BUFFER, m_bufferId);
    glUnmapBuffer(GL_UNIFORM_BUFFER);
#endif
}

bool CstBufferGL::update()
{
    return updateFromUniforms();
}
