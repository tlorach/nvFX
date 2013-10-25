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
#include <windows.h> // for OutputDebugString()
#endif
#include <stdarg.h> // Container::eprintf()

#include <fstream>
#include <assert.h>
#include "Fx.h"
#pragma warning(disable:4996)
using namespace nvFX;

/*****************************************************************************/
/** Texture Texture Texture Texture Texture Texture Texture Texture Texture **/
/*****************************************************************************/
/*
Texture::Texture(Container* pCont, const char * name)
{
    //m_targets.clear();
    if(name)
        m_name = name;
    m_OGLTex = 0;
    m_resource = NULL;
    m_d3dresource = NULL;
    m_type = TEXTURE_UNKNOWN;
    m_samplerState = NULL;
    //m_annotations
    m_container = pCont;
}

Texture::~Texture()
{
}

ITexture* Texture::deleteTarget(IPass *pass, int layerID)
{
    std::vector<STarget>::iterator it = m_targets.begin();
    std::vector<STarget>::iterator it2;
    while(it != m_targets.end())
    {
        STarget &t = *it;
        if(!t.valid)
            continue;
        if((pass != NULL) &&(t.pass != pass))
            continue;
        if((layerID > 0)&&(t.passLayerId != layerID))
            continue;
        it2 = it;
        ++it;
        m_targets.erase(it2);
    }
    return this;
}

ITexture* Texture::invalidateTarget(IPass *pass, int layerID)
{
    int tsz = (int)m_targets.size();
    for(int i=0; i<tsz; i++)
    {
        STarget &t = m_targets[i];
        if(!t.valid)
            continue;
        if((pass != NULL) &&(t.pass != pass))
            continue;
        if((layerID > 0)&&(t.passLayerId != layerID))
            continue;
        t.valid = false;
        t.dirty = true;
        t.index = -1;
    }
    return this;
}

int         Texture::getNumTargets(nvFX::TargetType ttype)
{
    int n = 0;
    int tsz = (int)m_targets.size();
    if(ttype == TANY)
        return tsz;
    for(int i=0; i<tsz; i++)
    {
        STarget &t = m_targets[i];
        if(!t.valid)
            continue;
        if(t.ttype == ttype)
            n++;
    }
    return n;
}
*/
/*************************************************************************/ /**
 ** FrameBufferObject FrameBufferObject FrameBufferObject FrameBufferObject
 ** 
 **/ /*************************************************************************/ 
void FrameBufferObject::setDSTResource(IResource * pDST)
{ 
    // if ever a previous set of resource were assigned... need to remove them
    m_dst = static_cast<Resource*>(pDST);
    m_w = m_h = 0;
}
void FrameBufferObject::setColorResource(IResource ** pp, int n)
{ 
    // if ever a previous set of resource were assigned... need to remove them
    int i=0;
    for(; i<(int)m_colors.size(); i++)
    {
        m_colors[i] = NULL;
    }
    
    m_colors.clear();       
    for(i = 0; i<n;i++)
    { 
        m_colors.push_back(static_cast<Resource*>(pp[i]));
    } 
    m_w = m_h = 0;
}

/*************************************************************************/ /**
 ** FrameBufferObjectsRepository singleton
 ** 
 **/ /*************************************************************************/ 
IFrameBufferObjectsRepository* nvFX::getFrameBufferObjectsRepositorySingleton()
{
    static FrameBufferObjectsRepository repository;
    return &repository;
}
bool IFrameBufferObjectsRepositoryEx::swapResources(IFrameBufferObject *pIF1, IFrameBufferObject *pIF2)
{
#pragma MESSAGE(__FILE__ "(131) : Check the integrity of IFrameBufferObjectsRepository::swapResources !")
    FrameBufferObject* pF1 = static_cast<FrameBufferObject*>(pIF1);
    FrameBufferObject* pF2 = static_cast<FrameBufferObject*>(pIF2);
    if(pF1->m_w != pF2->m_w)
        return false;
    if(pF1->m_h != pF2->m_h)
        return false;
    if(pF1->m_dst && !pF2->m_dst)
        return false;
    if(pF2->m_dst && !pF1->m_dst)
        return false;
    if(pF1->m_colors.size() != pF2->m_colors.size())
        return false;
    if(pF1->m_dst)
    {
        GLuint dstGLID = pF1->m_dst->m_OGLId;
        pF1->m_dst->m_OGLId = pF2->m_dst->m_OGLId;
        pF2->m_dst->m_OGLId = dstGLID;
    }
    int sz = (int)pF1->m_colors.size();
    for(int i=0; i<sz; i++)
    {
        GLuint glID = pF1->m_colors[i]->m_OGLId;
        pF1->m_colors[i]->m_OGLId = pF2->m_colors[i]->m_OGLId;
        pF2->m_colors[i]->m_OGLId = glID;
    }
    GLuint          fboID = pF1->m_fboID;
    pF1->m_fboID = pF2->m_fboID;
    pF2->m_fboID = fboID;
    return false;
}
FrameBufferObjectsRepository::FrameBufferObjectsRepository()
{
    m_vp[0] = m_vp[1] = m_vp[2] = m_vp[3] = 0;
    m_backbuffer = NULL;
    m_backbufferDST = NULL;
    m_msaa[0] = 1;
    m_msaa[1] = 0;
}
FrameBufferObjectsRepository::~FrameBufferObjectsRepository()
{
    finish();
}
bool        FrameBufferObjectsRepository::finish()
{
    FboMap::iterator iR = m_FBOs.begin();
    for(; iR != m_FBOs.end(); iR++)
    {
        iR->second.refCnt--;
    }
    m_FBOs.clear();
    return true;
}
IFrameBufferObject* FrameBufferObjectsRepository::createFBO(const char * name)
{
    FrameBufferObject* p = NULL;
    FboMap::iterator iR = m_FBOs.find(name);
    if(iR != m_FBOs.end())
    {
        LOGD("FBO %s already declared. Using the existing one\n", name);
        iR->second.refCnt++;
        return iR->second.p;
    }
    p = new FrameBufferObject(this, name);
    m_FBOs[name] = Fbo(p,1);
    return p;
}
int        FrameBufferObjectsRepository::addFBO(IFrameBufferObject* p)
{
    if (p == NULL)
      return 0;
    FboMap::iterator iR = m_FBOs.find(p->getName());
    if(iR != m_FBOs.end())
    {
        if(iR->second.p != p)
        {
            LOGD("FBO %s already declared with a different object ptr failing...\n", p->getName());
            return -1;
        } else {
            return ++iR->second.refCnt;
        }
    }
    m_FBOs[p->getName()] = Fbo(static_cast<FrameBufferObject*>(p), 1);
    return 1;
}
int         FrameBufferObjectsRepository::releaseFbo(IFrameBufferObject* p)
{
    FrameBufferObject* pp = static_cast<FrameBufferObject*>(p);
    FboMap::iterator iR = m_FBOs.begin();
    while(iR != m_FBOs.end())
    {
        if(iR->second.p == pp)
        {
            iR->second.refCnt--;
            if(iR->second.refCnt == 0)
            {
                LOGD("Delete FBO %s and remove it from repository\n", pp->getName());
                pp->invalidate();
                delete pp;
                m_FBOs.erase(iR);
                return 0;
            } else {
                LOGD("Released FBO %s to refCnt=%d\n", pp->getName(), iR->second.refCnt);
                return iR->second.refCnt;
            }
        }
        ++iR;
    }
    return -1;
}

bool        FrameBufferObjectsRepository::setParams(int x, int y, int w, int h, int depthSamples, int coverageSamples, BufferHandle backbuffer, BufferHandle backbufferDST, void *pDev)
{
#ifdef WIN32
    m_pDevice = pDev; // Note: in D3D, we may want to 'AddRef' the device... should be done in a FrameBufferObjectsRepositoryD3D...
#endif
    m_vp[0] = x;
    m_vp[1] = y;
    m_vp[2] = w;
    m_vp[3] = h;
    m_backbuffer = backbuffer;
    m_backbufferDST = backbufferDST;
    m_msaa[0] = depthSamples;
    m_msaa[1] = coverageSamples;
    return true;
}

bool        FrameBufferObjectsRepository::validateAll()
{
    bool bRes = true;
    FboMap::iterator iR = m_FBOs.begin();
    for(; iR != m_FBOs.end(); iR++)
    {
        if(!iR->second.p->validate())
            bRes = false;
    }
    return bRes;
}
bool        FrameBufferObjectsRepository::updateValidated()
{
    bool bRes = true;
    FboMap::iterator iR = m_FBOs.begin();
    for(; iR != m_FBOs.end(); iR++)
    {
        // only update the ones already valid
        if((iR->second.refCnt > 0)&&(iR->second.p->m_fboID))
            if(!iR->second.p->validate())
                bRes = false;
    }
    return bRes;
}
IFrameBufferObject*      FrameBufferObjectsRepository::find(const char * fboName)
{
    FboMap::iterator iR = m_FBOs.find(fboName);
    if(iR != m_FBOs.end())
        return iR->second.p;
    return NULL;
}
IFrameBufferObject*      FrameBufferObjectsRepository::find(int i)
{
    if(i >= (int)m_FBOs.size())
        return NULL;
    FboMap::iterator iR = m_FBOs.begin();
    while(i > 0)
    {
        ++iR;
        --i;
    }
    return iR->second.p;
}

static FrameBufferObject* fboArray[100];

extern bool findMatchingAnnotation(IAnnotation* pAnnot, const char *annotationName, const char* value);

int FrameBufferObjectsRepository::gatherFromAnnotation(IFrameBufferObject** &pp, const char *annotationName, const char* str, int *ival, float *fval)
{
    memset(fboArray, 0, 100*sizeof(IShader*));
    pp = (IFrameBufferObject**)fboArray;
    int n = 0;
    if(!annotationName)
        return 0;
    FboMap::iterator i2 = m_FBOs.begin();
    while(i2 != m_FBOs.end())
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
                    nvFX::printf(">>Error : gathering of resources exceeded 100 !");
                    return n;
                }
                fboArray[n++] = i2->second.p;
            }
        }
        ++i2;
    }
    // warning : means that we cannot put in the same list some global shaders from here with
    // local shader previously found in this method...
    if(n == 0)
    {
        // Try in the global repository
        return static_cast<FrameBufferObjectsRepository*>(getFrameBufferObjectsRepositorySingleton())->gatherFromAnnotation(pp, annotationName, str, ival, fval);
    }
    return n;
}
int FrameBufferObjectsRepository::gatherFromAnnotation(IFrameBufferObject** &pp, const char *annotationName, const char* value)
{
    return gatherFromAnnotation(pp, annotationName, value, NULL, NULL);
}
int FrameBufferObjectsRepository::gatherFromAnnotation(IFrameBufferObject** &pp, const char *annotationName, int value)
{
    return gatherFromAnnotation(pp, annotationName, NULL, &value, NULL);
}
int FrameBufferObjectsRepository::gatherFromAnnotation(IFrameBufferObject** &pp, const char *annotationName, float value)
{
    return gatherFromAnnotation(pp, annotationName, NULL, NULL, &value);
}
int FrameBufferObjectsRepository::gatherFromNamespace(IFrameBufferObject** &pp, const char *ns)
{
    memset(fboArray, 0, 100*sizeof(IShader*));
    pp = (IFrameBufferObject**)fboArray;
    int n = 0;
    if(!ns)
        return 0;
    std::string strNS(ns);
    FboMap::iterator i2 = m_FBOs.begin();
    while(i2 != m_FBOs.end())
    {
        FrameBufferObject* p = i2->second.p;
        if(p && (p->m_name.ns() == strNS))
        {
            if(n >= 100)
            {
                nvFX::printf(">>Error : gathering of FBOs exceeded 100 !");
                return n;
            }
            fboArray[n++] = p;
        }
        ++i2;
    }
    return n;
}

bool FrameBufferObjectsRepository::removeFBO(FrameBufferObject *p)
{
    FboMap::iterator iR = m_FBOs.begin();
    for(; iR != m_FBOs.end(); iR++)
    {
        if(iR->second.p == p)
        {
            int c = iR->second.refCnt--;
            // NOTE: maybe we should test with (c <= 1) : after all c==1 means only this
            // was held by the singleton() and one more guy outside
            if(c == 0)
            {
                LOGD("FBO not used anymore, removing from singleton repository\n");
                m_FBOs.erase(iR);
            }
            // TEST. Should be good.
            else if(c == 1)
            {
                LOGD("FBO used only ONCE (refCnt=1), removing from singleton repository\n");
                m_FBOs.erase(iR);
            }
            return true;
        }
    }
    return false;
}

/*************************************************************************/ /**
 **  singleton
 ** 
 **/ /*************************************************************************/ 
IResourceRepository* nvFX::getResourceRepositorySingleton()
{
    static ResourceRepository repository;
    return &repository;
}
//bool		createRenderBuffer(int w, int h, GLenum format, int depthSamples, int coverageSamples);
//bool		remove(LPCSTR name);
//bool		remove(Resource* pTex);
//Resource* 	loadTextureFromFile(LPCSTR fname/*, CGtype type*/);

ResourceRepository::ResourceRepository()
{
    m_vp[0] = m_vp[1] = m_vp[2] = m_vp[3] = 0;
    m_backbuffer = NULL;
    m_backbufferDST = NULL;
    m_msaa[0] = 1;
    m_msaa[1] = 0;
    m_pDevice = NULL;
}

ResourceRepository::~ResourceRepository()
{
    finish();
}

bool        ResourceRepository::finish()
{
    ResourceMap::iterator iR = m_resources.begin();
    for(; iR != m_resources.end(); iR++)
    {
        int cnt = 0;
        cnt = --iR->second.refCnt;
    }
    m_resources.clear();
    return true;
}
IResource*  ResourceRepository::createResource(const char * name, ResourceType type)
{
    Resource* p = NULL;
    ResourceMap::iterator iR = m_resources.find(name);
    if(iR != m_resources.end())
    {
        LOGD("ResourceRepository::createResource : WARNING Resource %s already declared\n", name);
        iR->second.refCnt++;
        return iR->second.p;
    }
    p = new_Resource(this, name);
    if (NULL == p)
      return NULL;
    p->setType(type);
    m_resources[name] = Res(p,1);
    return p;
}
int        ResourceRepository::addResource(IResource* p)
{
  if (NULL == p)
    return 0;

    ResourceMap::iterator iR = m_resources.find(p->getName());
    if(iR != m_resources.end())
    {
        if(iR->second.p != p)
        {
            LOGD("Resource %s already declared. failing...\n", p->getName());
            return -1;
        } else {
            return ++iR->second.refCnt;
        }
    }
    m_resources[p->getName()] = Res(static_cast<Resource*>(p),1);
    return 1;
}

int         ResourceRepository::releaseResource(IResource* p)
{
    Resource* pp = static_cast<Resource*>(p);
    ResourceMap::iterator iR = m_resources.begin();
    while(iR != m_resources.end())
    {
        if(iR->second.p == pp)
        {
            iR->second.refCnt--;
            if(iR->second.refCnt == 0)
            {
                LOGD("Delete Resource %s and remove it from repository\n", pp->getName());
                pp->invalidate();
                delete_Resource(pp);
                m_resources.erase(iR);
                return 0;
            } else {
                LOGD("Released Resource %s to refCnt=%d\n", pp->getName(), iR->second.refCnt);
                return iR->second.refCnt;
            }
        }
        ++iR;
    }
    return -1;
}

IResource*  ResourceRepository::loadTexture(const char * texName/*, CGtype samplerType*/)
{
    CHECK_TRUE(false);
    return NULL;
}
IResource*  ResourceRepository::loadTexture3DSlices(const char * texName, const char * *fileNames, int nFiles)
{
    CHECK_TRUE(false);
    return NULL;
}
void        ResourceRepository::releaseTextureID(GLuint texID)
{
    CHECK_TRUE(false);
}

IResource*  ResourceRepository::find(const char * texName)
{
    ResourceMap::iterator iR = m_resources.find(texName);
    if(iR != m_resources.end())
        return iR->second.p;
    return NULL;
}
IResource*  ResourceRepository::find(GLuint glID)
{
    ResourceMap::iterator iR = m_resources.begin();
    while(iR != m_resources.end())
    {
        if(iR->second.p->m_OGLId == glID)
            return iR->second.p;
        ++iR;
    }
    return NULL;
}
IResource*  ResourceRepository::get(int i)
{
    if(i >= (int)m_resources.size())
        return NULL;
    ResourceMap::iterator iR = m_resources.begin();
    while(i > 0)
    {
        ++iR;
        --i;
    }
    return iR->second.p;
}
IResource*  ResourceRepository::find(void *p)
{
    ResourceMap::iterator iR = m_resources.begin();
    for(; iR != m_resources.end(); iR++)
    {
        if(iR->second.p == p)
        {
            return iR->second.p;
        }
    }
    return NULL;
}

static Resource* resArray[100];
int ResourceRepository::gatherFromAnnotation(IResource** &pp, const char *annotationName, const char* str, int *ival, float *fval)
{
    memset(resArray, 0, 100*sizeof(IShader*));
    pp = (IResource**)resArray;
    int n = 0;
    if(!annotationName)
        return 0;
    ResourceMap::iterator i2 = m_resources.begin();
    while(i2 != m_resources.end())
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
                    nvFX::printf(">>Error : gathering of resources exceeded 100 !");
                    return n;
                }
                resArray[n++] = i2->second.p;
            }
        }
        ++i2;
    }
    // warning : means that we cannot put in the same list some global shaders from here with
    // local shader previously found in this method...
    if(n == 0)
    {
        // Try in the global repository
        return static_cast<ResourceRepository*>(getResourceRepositorySingleton())->gatherFromAnnotation(pp, annotationName, str, ival, fval);
    }
    return n;
}
int ResourceRepository::gatherFromAnnotation(IResource** &pp, const char *annotationName, const char* value)
{
    return gatherFromAnnotation(pp, annotationName, value, NULL, NULL);
}
int ResourceRepository::gatherFromAnnotation(IResource** &pp, const char *annotationName, int value)
{
    return gatherFromAnnotation(pp, annotationName, NULL, &value, NULL);
}
int ResourceRepository::gatherFromAnnotation(IResource** &pp, const char *annotationName, float value)
{
    return gatherFromAnnotation(pp, annotationName, NULL, NULL, &value);
}
int ResourceRepository::gatherFromNamespace(IResource** &pp, const char *ns)
{
    memset(resArray, 0, 100*sizeof(IShader*));
    pp = (IResource**)resArray;
    int n = 0;
    if(!ns)
        return 0;
    std::string strNS(ns);
    ResourceMap::iterator i2 = m_resources.begin();
    while(i2 != m_resources.end())
    {
        Resource* p = i2->second.p;
        if(p && (p->m_name.ns() == strNS))
        {
            if(n >= 100)
            {
                nvFX::printf(">>Error : gathering of resources exceeded 100 !");
                return n;
            }
            resArray[n++] = p;
        }
        ++i2;
    }
    return n;
}


bool ResourceRepository::removeResource(Resource *p)
{
    ResourceMap::iterator iR = m_resources.begin();
    for(; iR != m_resources.end(); iR++)
    {
        if(iR->second.p == p)
        {
            int c = --iR->second.refCnt;
            if(c == 0) {
                LOGD("Resource not used anymore, removing from singleton repository\n");
                m_resources.erase(iR);
            }
            return true;
        }
    }
    return false;
}

bool ResourceRepository::setParams(int x, int y, int w, int h, int depthSamples, int coverageSamples, BufferHandle backbuffer, BufferHandle backbufferDST, void *pDev)
{
#ifdef WIN32
    m_pDevice = pDev; // AddRef ?
#endif
    m_vp[0] = x;
    m_vp[1] = y;
    m_vp[2] = w;
    m_vp[3] = h;
    m_backbuffer = backbuffer;
    m_backbufferDST = backbufferDST;
    m_msaa[0] = depthSamples;
    m_msaa[1] = coverageSamples;
    //
    // FBOs repository must be in-sync with the resource. So let's do it here
    //
    nvFX::getFrameBufferObjectsRepositorySingleton()->setParams(x,y,w,h,depthSamples,coverageSamples, backbuffer, backbufferDST, pDev );
    return true;
}

bool ResourceRepository::validateAll()
{
    bool bRes = true;
    ResourceMap::iterator iR = m_resources.begin();
    for(; iR != m_resources.end(); iR++)
    {
        LOGD("Validating Resource %s (userCnt=%d)\n", iR->second.p->getName(), iR->second.p->getUserCnt());
        if(!iR->second.p->validate())
            bRes = false;
    }
    //
    // The validation of FBOs is mandatory anyways to be consistent. So let's do it here
    //
    if(!nvFX::getFrameBufferObjectsRepositorySingleton()->validateAll() )
        bRes = false;
    return bRes;
}

bool ResourceRepository::updateValidated()
{
    bool bRes = true;
    ResourceMap::iterator iR = m_resources.begin();
    for(; iR != m_resources.end(); iR++)
    {
        // re-validate only those that got previously explicitly validated
        if(iR->second.p->isValidated() )
        {
            LOGD("Updating Resource %s\n", iR->second.p->getName());
            if(!iR->second.p->validate())
                bRes = false;
        } else {
            LOGD("Resource %s not referenced as active resource. Skipping it...\n", iR->second.p->getName());
        }
    }
    //
    // The validation of FBOs is mandatory anyways to be consistent. So let's do it here
    //
    if(!nvFX::getFrameBufferObjectsRepositorySingleton()->updateValidated() )
        bRes = false;
    return bRes;
}

bool ResourceRepository::invalidateUnusedResources()
{
    bool bRes = true;
    ResourceMap::iterator iR = m_resources.begin();
    for(; iR != m_resources.end(); iR++)
    {
        if(iR->second.p->getUserCnt() == 0)
        {
            LOGD("Resource %s NOT referenced as active resource. invalidating it...\n", iR->second.p->getName());
            if(!iR->second.p->invalidate())
                bRes = false;
        } else {
            LOGD("Resource %s still referenced as active resource. Skipping it...\n", iR->second.p->getName());
        }
    }
    return bRes;
}

/*************************************************************************/ /**
 ** \brief
 **
 **/ /*************************************************************************/ 
Resource::Resource(ResourceRepository* pCont, const char *name)
{
    if(name)
        m_name = name;
    m_type = RESOURCE_UNKNOWN;
    m_samplerState = NULL;
    m_pOwner = pCont;

    m_OGLId = 0;
    //m_d3dresource = NULL;
    m_pContainer = NULL;
    m_userCnt = 0;
}
Resource::~Resource()
{
}
void Resource::setDimensions(int w, int h, int d)
{
    m_creationData.sz[0] = w;
    m_creationData.sz[1] = h;
    m_creationData.sz[2] = d;
}
void Resource::setDimensionsApplicationDependent(bool bYes)
{
    m_creationData.appDepSz = bYes;
}
void Resource::setMSAA(int d, int s)
{
    m_creationData.msaa[0] = d;
    m_creationData.msaa[1] = s;
}
void Resource::setMSAAApplicationDependent(bool bYes)
{
    m_creationData.appDepMSAA = bYes;
}
void Resource::setDimensionFactor(float w,float h)
{
    m_creationData.szFact[0] = w;
    m_creationData.szFact[1] = h;
}
void Resource::setResolveTarget(IResource* p)
{
    m_creationData.resolveTarget = static_cast<Resource*>(p);
}
bool Resource::setFormat(ResourceFormat f)
{
#ifdef OGLES2
    switch(f)
    {
    case FMT_ALPHA:
    case FMT_LUMINANCE:
    case FMT_LUMINANCE_ALPHA:
    case FMT_RGB:
    case FMT_RGBA:
        m_creationData.fmt = f;
        break;
    default:
        m_creationData.fmt = FMT_RGBA;
        return false;
    }
#else
    m_creationData.fmt = f;
#endif
    return true;
}


