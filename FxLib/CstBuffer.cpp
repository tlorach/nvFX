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

    $Id: //sw/devrel/Playpen/tristan/GitHub/nvFX/FxLib/CstBuffer.cpp#6 $
    $Date: 2013/06/20 $
    $Revision: #6 $
    $Author: tlorach $
    $Change: 16301413 $
*/
#include <fstream>
#include <assert.h>
#include "Fx.h"
#pragma warning(disable:4996)
using namespace nvFX;

/***************************************************************************/
/* CstBuffer CstBuffer CstBuffer CstBuffer CstBuffer CstBuffer CstBuffer   */
/***************************************************************************/

/*************************************************************************/ /**
 ** 
 ** 
 **/ /*************************************************************************/ 
CstBuffer::~CstBuffer()
{
}
CstBuffer::CstBuffer(const char* name) : 
ICstBufferEx()
{
	m_sizeMultiplier = 1;
	m_sizeOfCstBuffer = 0;
	m_bufferOffset = 0;
    m_bufferId = 0;
    m_pBufferInterface = NULL;
    if(name)
        m_name = name;
}

/*************************************************************************/ /**
** \brief sets the offset where to use the data for the uniform block
**
**/ /*************************************************************************/
bool		CstBuffer::offsetBufferBlock(int n)
{
	if(n >= m_sizeMultiplier)
		return false;
	if(m_bufferOffset != n)
	{
		m_bufferOffset = n;
		//setDirty(true);
	}
	return true;
}

/*************************************************************************/ /**
** \brief 
**
**/ /*************************************************************************/
ICstBuffer*    CstBuffer::setGLBuffer(int buffer)
{
    if(m_bufferId == buffer)
        return this;
    m_bufferId = buffer;
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
** \brief  assigns the ptr... BUT does not increase the reference count (D3D overidden part will do) !
**
**/ /*************************************************************************/
ICstBuffer*    CstBuffer::setD3DBuffer(void* buffer)
{
    if(m_pBufferInterface == buffer)
        return this;
    m_pBufferInterface = buffer;
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
 ** \brief create a uniform object that can be used to update uniforms in GLSL
 ** (or others like Cg or CUDA...).
 **/ /*************************************************************************/ 
IUniform*      CstBuffer::createUniform(const char *name, const char *annotation)
{
    // get the current total size before we add a new item
    int offsetBytes;
    bufferSizeAndData(NULL, &offsetBytes);
    Uniform *p = NULL;
    /*for(int i=0; i<(int)m_uniforms.size(); i++)
    {
        if(m_uniforms[i] && (!strcmp(m_uniforms[i]->getName(), name)))
        {
            p = m_uniforms[i];
            break;
        }
    }
    if(p == NULL)*/
    {
        p = new_Uniform(name, NULL, annotation);
        m_uniforms.push_back(p);
    }
    //
    // let's create a target for no specific pass : the CstBuffer will have
    // the knowledge of which pass needs it.
    // uniforms attached to a CstBuffer are further away and not directly related
    // to the shaders
    //
    p->m_targets.clear(); // if ever...
    Uniform::STarget t;
    t.ttype = TCSTBUFFER;
    t.valid = true;
    t.dirty = false;
    // there is no way to identify the target, at this stage. So let's create a target for no Pass. 
    // Later one target for a pass will be created
    t.pass = NULL; 
    // put the ptr to allow the uniform to interact with the owner, in this case
    // NOTE: only one owner for a uniform is allowed : if we have many targets, the same CstBuffer ptr will be used (or it's a bug)
    t.pBuffer = this;
    // compute the overall size of the current buffer, after we added
    // TODO : Warning Warning Warning Warning offset could be wrong in special cases")
    // I think we will have to test the alignment depending on the format to come
    t.offsetBytes = offsetBytes;
    t.size = 0; // to compute later when we know the type of this uniform
    p->m_targets.push_back(t);
    return p;
}

/*************************************************************************/ /**
 ** 
 ** 
 **/ /*************************************************************************/ 
IUniform*            CstBuffer::findUniform(const char * name)
{
    UniformVec::iterator it = m_uniforms.begin();
    while(it != m_uniforms.end())
    {
        if(!strcmp((*it)->getName(), name))
            break;
        ++it;
    }
    if(it == m_uniforms.end())
        return NULL;
    else return (*it);
}
/*************************************************************************/ /**
 ** 
 ** 
 **/ /*************************************************************************/ 
IUniform*            CstBuffer::findUniform(int i)
{
    if(i >= (int)m_uniforms.size())
        return NULL;
    return m_uniforms[i];
}


/**************************************************************************************************
 **
 ** 
 **
 **************************************************************************************************/
bool CstBuffer::getDirty(Pass *pass)
{
    bool bDirty = false;
    int tsz = (int)m_targets.size();
    for(int i=0; i<tsz; i++)
    {
        STarget &t = m_targets[i];
        if((pass && (t.pass == pass)) || (pass == NULL))
            if(t.dirty)
                return true;
    }
    return false;
}

/**************************************************************************************************
 **
 ** \todo there is an \e oversimplification in the case of having a component of a CstBuffer dirty :
 ** when a uniform of a CstBuffer changed, we will invoke this setDirty(). Which is not so good.
 ** This dirty flag should be used when the buffer reference changed. Not when the inside changed.
 ** so we should create a distinct dirty set/get for when the CstBuffer's children got dirty.
 ** See nvFX::Uniform::setTargetDirtyFlag
 **
 **************************************************************************************************/
void CstBuffer::setDirty(bool bYes, Pass *pass)
{
    int tsz = (int)m_targets.size();
    for(int i=0; i<tsz; i++)
    {
        STarget &t = m_targets[i];
        if((pass && (t.pass == pass)) || (pass == NULL))
            t.dirty = bYes;
    }
}

/*************************************************************************/ /**
 ** \brief invalidates the target for pass
 **
 **/ /*************************************************************************/ 
CstBuffer*    CstBuffer::invalidateTarget(IPass *pass, int layerID)
{
    std::vector<STarget>::iterator it = m_targets.begin();
    while(it != m_targets.end())
    {
        if((it->pass == pass)&&(it->passLayerId == layerID))
            it->valid = false;
        ++it;
    }
    return this;
}

/**************************************************************************************************
 **
 ** 
 **
 **************************************************************************************************/
bool CstBufferRepository::remove(CstBuffer *p)
{
    ItemMap::iterator iR = m_Items.begin();
    for(; iR != m_Items.end(); iR++)
    {
        if(iR->second.p == p)
        {
            int c = --iR->second.refCnt;
            if(c == 0) {
                LOGD("Resource not used anymore, removing from singleton repository\n");
                m_Items.erase(iR);
            }
            return true;
        }
    }
    return false;
}
CstBufferRepository::CstBufferRepository()
{
}
CstBufferRepository::~CstBufferRepository()
{
    ItemMap::iterator iR = m_Items.begin();
    for(; iR != m_Items.end(); iR++)
    {
        int cnt = 0;
        cnt = --iR->second.refCnt;
    }
    m_Items.clear();
}
ICstBuffer*   CstBufferRepository::createCstBuffer(const char *name)
{
    CstBuffer* p = NULL;
    ItemMap::iterator iR = m_Items.find(name);
    if(iR != m_Items.end())
    {
        // Let's not re-use any existing one from this method... confusing
        //LOGD("CstBufferRepository::createCstBuffer : WARNING CstBuffer %s already declared\n", name);
        //iR->second.refCnt++;
        //return iR->second.p;
        LOGD("CstBufferRepository::createCstBuffer : WARNING : CstBuffer %s already declared\n", name);
        return NULL;
    }
    p = new_CstBuffer(name);
    m_Items[name] = Item(p,1);
    return p;
}
int         CstBufferRepository::addCstBuffer(ICstBuffer* p)
{
    if (!p)
      return 0;

    ItemMap::iterator iR = m_Items.find(p->getName());

    if(iR != m_Items.end())
    {
        if(iR->second.p != p)
        {
            LOGD("CstBuffer %s already declared. failing...\n", p->getName());
            return -1;
        } else {
            return ++iR->second.refCnt;
        }
    }
    m_Items[p->getName()] = Item(static_cast<CstBuffer*>(p),1);
    return 1;
}
int         CstBufferRepository::releaseCstBuffer(ICstBuffer* p)
{
    CstBuffer* pp = static_cast<CstBuffer*>(p);
    ItemMap::iterator iR = m_Items.begin();
    while(iR != m_Items.end())
    {
        if(iR->second.p == pp)
        {
            iR->second.refCnt--;
            if(iR->second.refCnt == 0)
            {
                LOGI("Delete CstBuffer %s and remove it from repository\n", pp->getName());
                delete_CstBuffer(pp);
                m_Items.erase(iR);
                return 0;
            } else {
                LOGI("Released CstBuffer %s to refCnt=%d\n", pp->getName(), iR->second.refCnt);
                return iR->second.refCnt;
            }
        }
        ++iR;
    }
    return -1;
}

ICstBuffer*  CstBufferRepository::find(const char * Name)
{
    ItemMap::iterator iR = m_Items.find(Name);
    if(iR != m_Items.end())
        return iR->second.p;
    return NULL;
}
ICstBuffer*  CstBufferRepository::find(ICstBuffer * p)
{
    ItemMap::iterator iR = m_Items.begin();
    for(; iR != m_Items.end(); iR++)
    {
        if(iR->second.p == p)
        {
            return iR->second.p;
        }
    }
    return NULL;
}
ICstBuffer*  CstBufferRepository::findFromSemantic(const char * sem)
{
    return NULL;
}
ICstBuffer*  CstBufferRepository::get(int i)
{
    if(i >= (int)m_Items.size())
        return NULL;
    ItemMap::iterator iR = m_Items.begin();
    while(i > 0)
    {
        ++iR;
        --i;
    }
    return iR->second.p;
}

static CstBuffer* cbArray[100];

extern bool findMatchingAnnotation(IAnnotation* pAnnot, const char *annotationName, const char* value);

int CstBufferRepository::gatherFromAnnotation(ICstBuffer** &pp, const char *annotationName, const char* str, int *ival, float *fval)
{
    memset(cbArray, 0, 100*sizeof(ICstBuffer*));
    pp = (ICstBuffer**)cbArray;
    int n = 0;
    if(!annotationName)
        return 0;
    ItemMap::iterator i2 = m_Items.begin();
    while(i2 != m_Items.end())
    {
        if(i2->second.p)
        {
            IAnnotation* pAnnot = i2->second.p->annotations();
            if((str && findMatchingAnnotation(pAnnot, annotationName, str))
             ||(ival && (pAnnot->getAnnotationInt(annotationName)==ival[0]))
             ||(fval && (pAnnot->getAnnotationFloat(annotationName)==fval[0])))
            {
                if(n >= 100)
                {
                    nvFX::printf(">>Error : gathering of uniforms exceeded 100 !");
                    return n;
                }
                cbArray[n++] = i2->second.p;
            }
        }
        ++i2;
    }
    return n;
}

int CstBufferRepository::gatherFromAnnotation(ICstBuffer** &pp, const char *annotationName, const char* value)
{
    return gatherFromAnnotation(pp, annotationName, value, NULL, NULL);
}
int CstBufferRepository::gatherFromAnnotation(ICstBuffer** &pp, const char *annotationName, int value)
{
    return gatherFromAnnotation(pp, annotationName, NULL, &value, NULL);
}
int CstBufferRepository::gatherFromAnnotation(ICstBuffer** &pp, const char *annotationName, float value)
{
    return gatherFromAnnotation(pp, annotationName, NULL, NULL, &value);
}
int CstBufferRepository::gatherFromNamespace(ICstBuffer** &pp, const char *ns)
{
    memset(cbArray, 0, 100*sizeof(IShader*));
    pp = (ICstBuffer**)cbArray;
    int n = 0;
    if(!ns)
        return 0;
    std::string strNS(ns);
    ItemMap::iterator i2 = m_Items.begin();
    while(i2 != m_Items.end())
    {
        CstBuffer* p = i2->second.p;
        if(p && (p->m_name.ns() == strNS))
        {
            if(n >= 100)
            {
                nvFX::printf(">>Error : gathering of CstBuffers exceeded 100 !");
                return n;
            }
            cbArray[n++] = p;
        }
        ++i2;
    }
    return n;
}

