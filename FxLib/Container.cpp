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
//#include <assert.h>
#include "Fx.h"
#pragma warning(disable:4996)
using namespace nvFX;

/***************************************************************************/
/** Container Container Container Container Container Container Container **/
/***************************************************************************/

Container::Container()
{
    // this flag has no impact out of GLSL
    m_separateShaders = false;
}

/*************************************************************************/ /**
 ** We need to check how possible it is to switch to separate shaders on/off
 ** 
 **/ /*************************************************************************/ 
void Container::separateShadersEnable(bool bYes)
{
    // TODO: check how possible it is to change the mode, here.
    // - if the current HW allows it
    // - if the shaders fail/succeed in on of these modes
    // for example : walk through the existing programs ?
    // would we want to trigger a re-compilation/validation of compiled programs ?
    // I don't think so : this value should be set prior to start using OpenGL
    #ifdef USE_OLDPROGRAM
    m_separateShaders = false;
    #else
    m_separateShaders = bYes;
    #endif
}

/*************************************************************************/ /**
 ** 
 ** 
 **/ /*************************************************************************/ 
Container::~Container()
{
    ShaderModuleRepository* pShdRep = 
        static_cast<ShaderModuleRepository*>(getShaderModuleRepositorySingleton());
    TechVec::iterator i1 = m_techniques.begin();
    //
    // Techniques
    //
    while(i1 != m_techniques.end())
    {
        if(*i1)
            delete *i1;
        ++i1;
    }
    m_techniques.clear();
    //
    // SamplerStates
    //
    SStateVec::iterator i3 = m_samplerStates.begin();
    while(i3 != m_samplerStates.end())
    {
        if(*i3)
            delete_SamplerState(*i3);
        ++i3;
    }
    m_samplerStates.clear();
    //
    // Constant buffers
    //
    ICstBufferRepositoryEx* pCRep = getCstBufferRepositorySingleton()->getExInterface();
    CstBufferVec::iterator i4 = m_cstBuffers.begin();
    while(i4 != m_cstBuffers.end())
    {
        if(*i4)
        {
            if(pCRep->find(*i4)) // if in global area, could be still used by some other effects
            {
                if(pCRep->releaseCstBuffer(*i4) > 0)
                    LOGE(__FILE__"(111): TODO (*i4)->deleteTarget(...);\n");
                    //CHECK_TRUE_MSG(false,"TODO: (*i4)->deleteTarget(...);");
            } else
                delete_CstBuffer(*i4);
        }
        ++i4;
    }
    //
    // Uniforms
    //
    IUniformRepositoryEx* pURep = getUniformRepositorySingleton()->getExInterface();
    UniformMap::iterator i5 = m_uniforms.begin();
    while(i5 != m_uniforms.end())
    {
        if(i5->second)
        {
            if(pURep->find(i5->second))
            {
                if(pURep->releaseUniform(i5->second) > 0)
                    LOGE(__FILE__"(130): TODO (*i4)->deleteTarget(...);\n");
                    //CHECK_TRUE_MSG(false,"TODO: (*i4)->deleteTarget(...);");
            } else
                delete_Uniform(i5->second);
        }
        ++i5;
    }
    //
    // m_stateGroups
    //
    StateGroupRasterVec::iterator i6 = m_stateGroupRasters.begin();
    while(i6 != m_stateGroupRasters.end())
    {
        if(*i6)
            delete_StateGroupRaster(static_cast<StateGroupRaster*>(*i6));
        ++i6;
    }
    StateGroupCSVec::iterator i7 = m_stateGroupCSs.begin();
    while(i7 != m_stateGroupCSs.end())
    {
        if(*i7)
            delete_StateGroupCS(static_cast<StateGroupCS*>(*i7));
        ++i7;
    }
    StateGroupDSTVec::iterator i8 = m_stateGroupDSTs.begin();
    while(i8 != m_stateGroupDSTs.end())
    {
        if(*i8)
            delete_StateGroupDST(static_cast<StateGroupDST*>(*i8));
        ++i8;
    }
#ifndef OGLES2
    StateGroupPRVec::iterator i11 = m_stateGroupPRs.begin();
    while(i11 != m_stateGroupPRs.end())
    {
        if(*i11)
            delete_StateGroupPR(static_cast<StateGroupPath*>(*i11));
        ++i11;
    }
#endif
    //
    //m_shaderprograms;
    //
    ShaderProgramVec progs = m_shaderprograms;
    ShaderProgramVec::iterator i10 = progs.begin();
    while(i10 != progs.end())
    {
        if(*i10)
        {
            int c = pShdRep->releaseProgram(*i10); // remove it from the global pool if existed there
            if(c <= 0)
            {
#ifdef _DEBUG
                LOGD("Program %d (", (*i10)->getProgram());
                IShader* pshd;
                for(int ii=0; pshd = (*i10)->getShader(ii); ii++)
                    LOGD("%s ", pshd->getName());
                LOGD(") no more used... DELETING it\n");
#endif
                // good enough : query the first shader in the program
                switch((*i10)->getShader(0)->getType())
                {
                case TGLSL:
                case THLSL:
                    delete_Program(*i10);
                    break;
                case TCUDA:
                    delete_ProgramCUDA(*i10);
                    break;
                }
            }
        }
        ++i10;
    }
    m_shaderprograms.clear();
    //
    // Shaders
    //
    ShaderVec shaders = m_shaders;
    ShaderVec::iterator i2 = shaders.begin();
    while(i2 != shaders.end())
    {
        if(*i2)
        {
            // Note: the global repository gets updated when the container adds/removes a shader.
            // and not when a program adds/removes a shader. Reason: a program always belongs to a container
            int c = pShdRep->releaseShader(*i2);
            // the container is a 'user' of this shader code: it might have created it from scratch
            // or referenced it and found it from the global repository
            // removing as a user might lead to destroying this shader if no more used by anyone
            (*i2)->removeUser(this);
        }
        ++i2;
    }
    m_shaders.clear();
    //
    // Resources
    //
    FrameBufferObjectsRepository* pFboRep = 
        static_cast<FrameBufferObjectsRepository*>(nvFX::getFrameBufferObjectsRepositorySingleton());
    ResourceRepository* pResRep = 
        static_cast<ResourceRepository*>(nvFX::getResourceRepositorySingleton());
    ResourceVec::iterator it = m_resources.begin();
    while(it != m_resources.end())
    {
        pResRep->releaseResource(*it);
        ++it;
    }
    FboVec::iterator it2 = m_fbos.begin();
    while(it2 != m_fbos.end())
    {
        pFboRep->releaseFbo(*it2);
        ++it2;
    }
}

/*************************************************************************/ /**
 ** 
 ** 
 **/ /*************************************************************************/ 
IContainer*      IContainer::create(const char *name)
{
#ifdef USE_NSIGHT
    initNSight();
#endif
    IContainer* p = new Container;
    return p;
}
/*************************************************************************/ /**
 ** 
 ** 
 **/ /*************************************************************************/ 
bool IContainer::destroy(IContainer* p)
{
    // for now, we just delete it... no more.
    delete static_cast<Container*>(p);
    return true;
}
/*************************************************************************/ /**
 ** 
 ** 
 **/ /*************************************************************************/ 
ITechnique*      Container::createTechnique(const char *name)
{
    Technique *pTech = new Technique(this, name);
    m_techniques.push_back(pTech);
    return pTech;
}
/*************************************************************************/ /**
 ** 
 ** 
 **/ /*************************************************************************/ 
IShader*     Container::createPathShader(const char *name, bool bGlobal, bool bPostscript)
{
    Shader *p;
    ShaderVec::iterator i2 = m_shaders.begin();
    while(i2 != m_shaders.end())
    {
        if(name && (!strcmp((*i2)->getName(), name)))
        {
            LOGD("Error: Container::createPathShader can't create a new shader with the same name of one already there");
            return NULL;
        }
        ++i2;
    }
    if(bGlobal)
    {
        ShaderModuleRepository* pShdRep = 
            static_cast<ShaderModuleRepository*>(nvFX::getShaderModuleRepositorySingleton());
        if(p = static_cast<Shader*>(pShdRep->findShader(name)))
        {
            LOGD("Container::createPathShader shader %s already there. Add RefCnt...\n", name ? name : "<noname>");
            addPathShader(p);
            pShdRep->addShader(p); // increments the reference counter
        } else {
            LOGD("Container::createPathShader shader %s created and added as Global...\n", name ? name : "<noname>");
            p = new_ShaderPath(name, bPostscript);
            p->addUser(this);
            if(!p)
                return NULL;
            m_shaders.push_back(p);
            pShdRep->addShader(p);
        }
    } else {
        LOGD("Container::createPathShader shader %s created locally only...\n", name ? name : "<noname>");
        p = new_ShaderPath(name, bPostscript);
        p->addUser(this);
        if(!p)
            return NULL;
        m_shaders.push_back(p);
    }
    return p;
}

#pragma MESSAGE(__FILE__"(203):TODO TODO TODO TODO Check validity of shaders GLSL vs. DX10 vs DX11")
IShader*     Container::createGLSLShader(const char *name, bool bGlobal)
{
    Shader *p;
    ShaderVec::iterator i2 = m_shaders.begin();
    while(i2 != m_shaders.end())
    {
        if(name && (!strcmp((*i2)->getName(), name)))
        {
            LOGD("Error: Container::createGLSLShader can't create a new shader with the same name of one already there");
            return NULL;
        }
        ++i2;
    }
    if(bGlobal)
    {
        ShaderModuleRepository* pShdRep = 
            static_cast<ShaderModuleRepository*>(nvFX::getShaderModuleRepositorySingleton());
        if(p = static_cast<Shader*>(pShdRep->findShader(name)))
        {
            LOGD("Container::createGLSLShader shader %s already there. Add RefCnt...\n", name ? name : "<noname>");
            addGLSLShader(p);
            pShdRep->addShader(p); // increments the reference counter
        } else {
            LOGD("Container::createGLSLShader shader %s created and added as Global...\n", name ? name : "<noname>");
            p = new_GLSLShader(name);
            p->addUser(this);
            if(!p)
                return NULL;
            m_shaders.push_back(p);
            pShdRep->addShader(p);
        }
    } else {
        LOGD("Container::createGLSLShader shader %s created locally only...\n", name ? name : "<noname>");
        p = new_GLSLShader(name);
        p->addUser(this);
        if(!p)
            return NULL;
        m_shaders.push_back(p);
    }
    return p;
}
IShader* Container::createHLSL10Shader(const char *name, bool bGlobal)
{
    Shader *p;
    ShaderVec::iterator i2 = m_shaders.begin();
    while(i2 != m_shaders.end())
    {
        if(name && (!strcmp((*i2)->getName(), name)))
        {
            LOGD("Error: Container::createGLSLShader can't create a new shader with the same name of one already there");
            return NULL;
        }
        ++i2;
    }
    if(bGlobal)
    {
        ShaderModuleRepository* pShdRep = 
            static_cast<ShaderModuleRepository*>(nvFX::getShaderModuleRepositorySingleton());
        if(p = static_cast<Shader*>(pShdRep->findShader(name)))
        {
            addGLSLShader(p);
            pShdRep->addShader(p); // increments the reference counter
        } else {
            p = new_HLSL10Shader(name);
            p->addUser(this);
            if(!p)
                return NULL;
            m_shaders.push_back(p);
            pShdRep->addShader(p);
        }
    } else {
        p = new_HLSL10Shader(name);
        p->addUser(this);
        if(!p)
            return NULL;
        m_shaders.push_back(p);
    }
    return p;
}
IShader* Container::createHLSL11Shader(const char *name, bool bGlobal)
{
    Shader *p;
    ShaderVec::iterator i2 = m_shaders.begin();
    while(i2 != m_shaders.end())
    {
        if(name && (!strcmp((*i2)->getName(), name)))
        {
            LOGD("Error: Container::createGLSLShader can't create a new shader with the same name of one already there");
            return NULL;
        }
        ++i2;
    }
    if(bGlobal)
    {
        ShaderModuleRepository* pShdRep = 
            static_cast<ShaderModuleRepository*>(nvFX::getShaderModuleRepositorySingleton());
        if(p = static_cast<Shader*>(pShdRep->findShader(name)))
        {
            addGLSLShader(p);
            pShdRep->addShader(p); // increments the reference counter
        } else {
            p = new_HLSL11Shader(name);
            p->addUser(this);
            if(!p)
                return NULL;
            m_shaders.push_back(p);
            pShdRep->addShader(p);
        }
    } else {
        p = new_HLSL11Shader(name);
        p->addUser(this);
        if(!p)
            return NULL;
        m_shaders.push_back(p);
    }
    return p;
}
IShader* Container::createCUDAShader(const char *name, bool bGlobal)
{
#ifdef USECUDA
    Shader *p;
    ShaderVec::iterator i2 = m_shaders.begin();
    while(i2 != m_shaders.end())
    {
        if(name && (!strcmp((*i2)->getName(), name)))
        {
            LOGD("Error: Container::createGLSLShader can't create a new shader with the same name of one already there");
            return NULL;
        }
        ++i2;
    }
    if(bGlobal)
    {
        ShaderModuleRepository* pShdRep = 
            static_cast<ShaderModuleRepository*>(nvFX::getShaderModuleRepositorySingleton());
        if(p = static_cast<Shader*>(pShdRep->findShader(name, TCUDA)))
        {
            addCUDAShader(p);
            pShdRep->addShader(p); // increments the reference counter
        } else {
            p = new_ShaderCUDA(name);
            p->addUser(this);
            if(!p)
                return NULL;
            m_shaders.push_back(p);
            pShdRep->addShader(p);
        }
    } else {
        p = new_ShaderCUDA(name);
        p->addUser(this);
        if(!p)
            return NULL;
        m_shaders.push_back(p);
    }
    return p;
#else
    return NULL;
#endif
}

bool Container::addCUDAShader(IShader* p)
{
    Shader *p2 = static_cast<Shader*>(p);
    ShaderVec::iterator i2 = m_shaders.begin();
    while(i2 != m_shaders.end())
    {
        if((*i2)== p2)
            return false;
        ++i2;
    }
    m_shaders.push_back(p2);
    p2->addUser(this);
    return true;
}

bool    Container::addGLSLShader(IShader* p)
{
    Shader *p2 = static_cast<Shader*>(p);
    ShaderVec::iterator i2 = m_shaders.begin();
    while(i2 != m_shaders.end())
    {
        if((*i2)== p2)
            return false;
        ++i2;
    }
    m_shaders.push_back(p2);
    p2->addUser(this);
    return true;
}
bool    Container::addPathShader(IShader* p)
{
    Shader *p2 = static_cast<Shader*>(p);
    ShaderVec::iterator i2 = m_shaders.begin();
    while(i2 != m_shaders.end())
    {
        if((*i2)== p2)
            return false;
        ++i2;
    }
    m_shaders.push_back(p2);
    p2->addUser(this);
    return true;
}
bool    Container::addHLSL10Shader(IShader* p)
{
    Shader *p2 = static_cast<Shader*>(p);
    ShaderVec::iterator i2 = m_shaders.begin();
    while(i2 != m_shaders.end())
    {
        if((*i2)== p2)
            return false;
        ++i2;
    }
    m_shaders.push_back(p2);
    p2->addUser(this);
    return true;
}
bool    Container::addHLSL11Shader(IShader* p)
{
    Shader *p2 = static_cast<Shader*>(p);
    ShaderVec::iterator i2 = m_shaders.begin();
    while(i2 != m_shaders.end())
    {
        if((*i2)== p2)
            return false;
        ++i2;
    }
    m_shaders.push_back(p2);
    p2->addUser(this);
    return true;
}

/*************************************************************************/ /**
 ** 
 ** 
 **/ /*************************************************************************/ 
IShader*     Container::findShader(const char * name, TargetType t)
{
    if(!name)
        return NULL;
    StringName sname(name);
    ShaderVec::iterator i2 = m_shaders.begin();
    while(i2 != m_shaders.end())
    {
        Shader* pShd = static_cast<Shader*>(*i2);
        if(pShd->m_name == sname)
        {
            if(t == TANY)
                return *i2;
            else if(t == (*i2)->getType())
                return *i2;
        }
        ++i2;
    }
    // Try in the global repository
    IShader *p = getShaderModuleRepositorySingleton()->findShader(name);
    return p;
}
/*************************************************************************/ /**
 ** 
 ** 
 **/ /*************************************************************************/ 
IShader*     Container::findShader(int i)
{
    if(i < (int)m_shaders.size())
        return m_shaders[i];
    return NULL;
}


/*************************************************************************/ /**
 **
 ** 
 **
 **/ /*************************************************************************/ 
// TODO: check if this is clean enough...
static IShader* shdArray[100];

bool findMatchingAnnotation(IAnnotation* pAnnot, const char *annotationName, const char* value)
{
    const char *str;
    bool found = false;
    if(value) {
        str = pAnnot->getAnnotationString(annotationName);
        if(str) found = strcmp(str, value) == 0;
        return found;
    }
    if(pAnnot->getAnnotationInt(annotationName) != 0)
        return true;
    if(pAnnot->getAnnotationFloat(annotationName) != 0)
        return true;
    return false;
}

int Container::gatherShadersFromAnnotation(IShader** &pp, const char *annotationName, const char* str, int *ival, float *fval)
{
    memset(shdArray, 0, 100*sizeof(IShader*));
    pp = (IShader**)shdArray;
    int n = 0;
    if(!annotationName)
        return 0;
    ShaderVec::iterator i2 = m_shaders.begin();
    while(i2 != m_shaders.end())
    {
        IAnnotation* pAnnot = (*i2)->annotations();
        if((str && findMatchingAnnotation(pAnnot, annotationName, str))
         ||(ival && (pAnnot->getAnnotationInt(annotationName)==ival[0]))
         ||(fval && (pAnnot->getAnnotationFloat(annotationName)==fval[0])))
        {
            if(n >= 100)
            {
                nvFX::printf(">>Error : gathering of shader exceeded 100 !");
                return n;
            }
            shdArray[n++] = (*i2);
        }
        ++i2;
    }
    // warning : means that we cannot put in the same list some global shaders from here with
    // local shader previously found in this method...
    if(n == 0)
    {
        // Try in the global repository
        return static_cast<ShaderModuleRepository*>(getShaderModuleRepositorySingleton())->gatherFromAnnotation(pp, annotationName, str, ival, fval);
    }
    return n;
}
static IResource* resArray[100];
int Container::gatherResourcesFromAnnotation(IResource** &pp, const char *annotationName, const char* str, int *ival, float *fval)
{
    memset(resArray, 0, 100*sizeof(IResource*));
    pp = (IResource**)resArray;
    int n = 0;
    if(!annotationName)
        return 0;
    ResourceVec::iterator i2 = m_resources.begin();
    while(i2 != m_resources.end())
    {
        IAnnotation* pAnnot = (*i2)->annotations();
        if((str && findMatchingAnnotation(pAnnot, annotationName, str))
         ||(ival && (pAnnot->getAnnotationInt(annotationName)==ival[0]))
         ||(fval && (pAnnot->getAnnotationFloat(annotationName)==fval[0])))
        {
            if(n >= 100)
            {
                nvFX::printf(">>Error : gathering of resources exceeded 100 !");
                return n;
            }
            resArray[n++] = (*i2);
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
static IFrameBufferObject* fboArray[100];
int Container::gatherFbosFromAnnotation(IFrameBufferObject** &pp, const char *annotationName, const char* str, int *ival, float *fval)
{
    memset(fboArray, 0, 100*sizeof(IShader*));
    pp = (IFrameBufferObject**)fboArray;
    int n = 0;
    if(!annotationName)
        return 0;
    FboVec::iterator i2 = m_fbos.begin();
    while(i2 != m_fbos.end())
    {
        IAnnotation* pAnnot = (*i2)->annotations();
        if((str && findMatchingAnnotation(pAnnot, annotationName, str))
         ||(ival && (pAnnot->getAnnotationInt(annotationName)==ival[0]))
         ||(fval && (pAnnot->getAnnotationFloat(annotationName)==fval[0])))
        {
            if(n >= 100)
            {
                nvFX::printf(">>Error : gathering of FBOs exceeded 100 !");
                return n;
            }
            fboArray[n++] = (*i2);
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
static IUniform* uArray[100];
int Container::gatherUniformsFromAnnotation(IUniform**&pp, const char *annotationName, const char* str, int *ival, float *fval)
{
    memset(uArray, 0, 100*sizeof(IUniform*));
    pp = (IUniform**)uArray;
    int n = 0;
    if(!annotationName)
        return 0;
    UniformMap::iterator i2 = m_uniforms.begin();
    while(i2 != m_uniforms.end())
    {
        IAnnotation* pAnnot = i2->second->annotations();
        if((str && findMatchingAnnotation(pAnnot, annotationName, str))
         ||(ival && (pAnnot->getAnnotationInt(annotationName)==ival[0]))
         ||(fval && (pAnnot->getAnnotationFloat(annotationName)==fval[0])))
        {
            if(n >= 100)
            {
                nvFX::printf(">>Error : gathering of uniforms exceeded 100 !");
                return n;
            }
            uArray[n++] = i2->second;
        }
        ++i2;
    }
    // warning : means that we cannot put in the same list some global shaders from here with
    // local shader previously found in this method...
    if(n == 0)
    {
        // Try in the global repository
        return static_cast<UniformRepository*>(getUniformRepositorySingleton())->gatherFromAnnotation(pp, annotationName, str, ival, fval);
    }
    return n;
}
static ICstBuffer* cbArray[100];
int Container::gatherCstBuffersFromAnnotation(ICstBuffer**&pp, const char *annotationName, const char* str, int *ival, float *fval)
{
    memset(cbArray, 0, 100*sizeof(ICstBuffer*));
    pp = (ICstBuffer**)cbArray;
    int n = 0;
    if(!annotationName)
        return 0;
    CstBufferVec::iterator i2 = m_cstBuffers.begin();
    while(i2 != m_cstBuffers.end())
    {
        IAnnotation* pAnnot = (*i2)->annotations();
        if((str && findMatchingAnnotation(pAnnot, annotationName, str))
         ||(ival && (pAnnot->getAnnotationInt(annotationName)==ival[0]))
         ||(fval && (pAnnot->getAnnotationFloat(annotationName)==fval[0])))
        {
            if(n >= 100)
            {
                nvFX::printf(">>Error : gathering of constant buffers exceeded 100 !");
                return n;
            }
            cbArray[n++] = (*i2);
        }
        ++i2;
    }
    // warning : means that we cannot put in the same list some global shaders from here with
    // local shader previously found in this method...
    if(n == 0)
    {
        // Try in the global repository
        return static_cast<CstBufferRepository*>(getCstBufferRepositorySingleton())->gatherFromAnnotation(pp, annotationName, str, ival, fval);
    }
    return n;
}

int Container::gatherShadersFromAnnotation(IShader** &ppShaders, const char *annotationName, const char* value)
{
    return gatherShadersFromAnnotation(ppShaders, annotationName, value, NULL, NULL);
}
int Container::gatherShadersFromAnnotation(IShader** &ppShaders, const char *annotationName, int value)
{
    return gatherShadersFromAnnotation(ppShaders, annotationName, NULL, &value, NULL);
}
int Container::gatherShadersFromAnnotation(IShader** &ppShaders, const char *annotationName, float value)
{
    return gatherShadersFromAnnotation(ppShaders, annotationName, NULL, NULL, &value);
}
int Container::gatherShadersFromNamespace(IShader** &ppShaders, const char *ns)
{
    memset(shdArray, 0, 100*sizeof(IShader*));
    ppShaders = (IShader**)shdArray;
    int n = 0;
    if(!ns)
        return 0;
    std::string strNS(ns);
    ShaderVec::iterator i2 = m_shaders.begin();
    while(i2 != m_shaders.end())
    {
        if((*i2)->m_name.ns() == strNS)
        {
            if(n >= 100)
            {
                nvFX::printf(">>Error : gathering of shader exceeded 100 !");
                return n;
            }
            shdArray[n++] = (*i2);
        }
        ++i2;
    }
    // warning : means that we cannot put in the same list some global shaders from here with
    // local shader previously found in this method...
    if(n == 0)
    {
        // Try in the global repository
        return getShaderModuleRepositorySingleton()->gatherFromNamespace(ppShaders, ns);
    }
    return n;
}
/*************************************************************************/ /**
 **
 ** 
 **
 **/ /*************************************************************************/ 
int Container::gatherUniformsFromAnnotation(IUniform**&pp, const char *annotationName, const char* value)
{
    return gatherUniformsFromAnnotation(pp, annotationName, value, NULL, NULL);
}
int Container::gatherUniformsFromAnnotation(IUniform**&pp, const char *annotationName, int value)
{
    return gatherUniformsFromAnnotation(pp, annotationName, NULL, &value, NULL);
}
int Container::gatherUniformsFromAnnotation(IUniform**&pp, const char *annotationName, float value)
{
    return gatherUniformsFromAnnotation(pp, annotationName, NULL, NULL, &value);
}
int Container::gatherUniformsFromNamespace(IUniform**&pp, const char *ns)
{
    static IUniform* uArray[100];
    memset(uArray, 0, 100*sizeof(IShader*));
    pp = (IUniform**)uArray;
    int n = 0;
    if(!ns)
        return 0;
    std::string strNS(ns);
    UniformMap::iterator i2 = m_uniforms.begin();
    while(i2 != m_uniforms.end())
    {
        if(i2->second->m_name.ns() == strNS)
        {
            if(n >= 100)
            {
                nvFX::printf(">>Error : gathering of uniforms exceeded 100 !");
                return n;
            }
            uArray[n++] = i2->second;
        }
        ++i2;
    }
    // warning : means that we cannot put in the same list some global shaders from here with
    // local shader previously found in this method...
    if(n == 0)
    {
        // Try in the global repository
        return getUniformRepositorySingleton()->gatherFromNamespace(pp, ns);
    }
    return n;
}
int Container::gatherCstBuffersFromAnnotation(ICstBuffer**&pp, const char *annotationName, const char* value)
{
    return gatherCstBuffersFromAnnotation(pp, annotationName, value, NULL, NULL);
}
int Container::gatherCstBuffersFromAnnotation(ICstBuffer**&pp, const char *annotationName, int value)
{
    return gatherCstBuffersFromAnnotation(pp, annotationName, NULL, &value, NULL);
}
int Container::gatherCstBuffersFromAnnotation(ICstBuffer**&pp, const char *annotationName, float value)
{
    return gatherCstBuffersFromAnnotation(pp, annotationName, NULL, NULL, &value);
}
int Container::gatherCstBuffersFromNamespace(ICstBuffer**&pp, const char *ns)
{
    memset(cbArray, 0, 100*sizeof(ICstBuffer*));
    pp = (ICstBuffer**)cbArray;
    int n = 0;
    if(!ns)
        return 0;
    std::string strNS(ns);
    CstBufferVec::iterator i2 = m_cstBuffers.begin();
    while(i2 != m_cstBuffers.end())
    {
        if((*i2)->m_name.ns() == strNS)
        {
            if(n >= 100)
            {
                nvFX::printf(">>Error : gathering of cst buffers exceeded 100 !");
                return n;
            }
            cbArray[n++] = (*i2);
        }
        ++i2;
    }
    // warning : means that we cannot put in the same list some global shaders from here with
    // local shader previously found in this method...
    if(n == 0)
    {
        // Try in the global repository
        return getCstBufferRepositorySingleton()->gatherFromNamespace(pp, ns);
    }
    return n;
}


/*************************************************************************/ /**
 **
 ** 
 **
 **/ /*************************************************************************/ 
/*void Container::consolidateShaders()
{
    //
    // GLSL
    //
    std::string strHeader;
    ShaderVec::iterator it = m_shaders.begin();
    while(it != m_shaders.end())
    {
        if((*it)->m_name.length() == 0)
            strHeader += (*it)->m_shaderCode;
        ++it;
    }
    it = m_shaders.begin();
    while(it != m_shaders.end())
    {
        if((*it)->m_name.length() > 0)
            (*it)->addHeaderCode(strHeader.c_str());
        ++it;
    }
}*/

/*************************************************************************/ /**
 ** 
 ** 
 **/ /*************************************************************************/ 
ISamplerState*   Container::createSamplerState(const char *name)
{
    SamplerState *p = new_SamplerState(this, name);
    m_samplerStates.push_back(p);
    return p;
}
/*************************************************************************/ /**
 ** 
 ** 
 **/ /*************************************************************************/ 
ITechnique*          Container::findTechnique(const char * name)
{
    if(!name)
        return NULL;
    TechVec::iterator i2 = m_techniques.begin();
    while(i2 != m_techniques.end())
    {
        if(!strcmp((*i2)->getName(), name))
            return *i2;
        ++i2;
    }
    return NULL;
}
/*************************************************************************/ /**
 ** 
 ** 
 **/ /*************************************************************************/ 
ITechnique*          Container::findTechnique(int i)
{
    if(i < (int)m_techniques.size())
        return m_techniques[i];
    return NULL;
}
/*************************************************************************/ /**
 ** 
 ** 
 **/ /*************************************************************************/ 
ISamplerState*       Container::findSamplerState(const char * name)
{
    if(!name)
        return NULL;
    SStateVec::iterator i2 = m_samplerStates.begin();
    while(i2 != m_samplerStates.end())
    {
        if(!strcmp((*i2)->getName(), name))
            return *i2;
        ++i2;
    }
    return NULL;
}
/*************************************************************************/ /**
 ** 
 ** 
 **/ /*************************************************************************/ 
ISamplerState*       Container::findSamplerState(int i)
{
    if(i < (int)m_samplerStates.size())
        return m_samplerStates[i];
    return NULL;
}
/*************************************************************************/ /**
 ** 
 ** 
 **/ /*************************************************************************/ 
IStateGroupRaster*   Container::findStateGroupRaster(const char * name)
{
    if(!name)
        return NULL;
    StateGroupRasterVec::iterator i2 = m_stateGroupRasters.begin();
    while(i2 != m_stateGroupRasters.end())
    {
        if(!strcmp((*i2)->getName(), name))
            return *i2;
        ++i2;
    }
    return NULL;
}
/*************************************************************************/ /**
 ** 
 ** 
 **/ /*************************************************************************/ 
IStateGroupRaster*   Container::findStateGroupRaster(int i)
{
    if(i < (int)m_stateGroupRasters.size())
        return m_stateGroupRasters[i];
    return NULL;
}
/*************************************************************************/ /**
 ** 
 ** 
 **/ /*************************************************************************/ 
IStateGroupCS*       Container::findStateGroupCS(const char * name)
{
    if(!name)
        return NULL;
    StateGroupCSVec::iterator i2 = m_stateGroupCSs.begin();
    while(i2 != m_stateGroupCSs.end())
    {
        if(!strcmp((*i2)->getName(), name))
            return *i2;
        ++i2;
    }
    return NULL;
}
/*************************************************************************/ /**
 ** 
 ** 
 **/ /*************************************************************************/ 
IStateGroupCS*       Container::findStateGroupCS(int i)
{
    if(i < (int)m_stateGroupCSs.size())
        return m_stateGroupCSs[i];
    return NULL;
}
/*************************************************************************/ /**
 ** 
 ** 
 **/ /*************************************************************************/ 
IStateGroupDST*      Container::findStateGroupDST(const char * name)
{
    if(!name)
        return NULL;
    StateGroupDSTVec::iterator i2 = m_stateGroupDSTs.begin();
    while(i2 != m_stateGroupDSTs.end())
    {
        if(!strcmp((*i2)->getName(), name))
            return *i2;
        ++i2;
    }
    return NULL;
}
#ifndef OGLES2
/*************************************************************************/ /**
 ** 
 ** 
 **/ /*************************************************************************/ 
IStateGroupPath*      Container::findStateGroupPR(const char * name)
{
    if(!name)
        return NULL;
    StateGroupPRVec::iterator i2 = m_stateGroupPRs.begin();
    while(i2 != m_stateGroupPRs.end())
    {
        if(!strcmp((*i2)->getName(), name))
            return *i2;
        ++i2;
    }
    return NULL;
}
#endif
/*************************************************************************/ /**
 ** 
 ** 
 **/ /*************************************************************************/ 
IStateGroupDST*      Container::findStateGroupDST(int i)
{
    if(i < (int)m_stateGroupDSTs.size())
        return m_stateGroupDSTs[i];
    return NULL;
}

#ifndef OGLES2
/*************************************************************************/ /**
 ** 
 ** 
 **/ /*************************************************************************/ 
IStateGroupPath*      Container::findStateGroupPR(int i)
{
    if(i < (int)m_stateGroupPRs.size())
        return m_stateGroupPRs[i];
    return NULL;
}
#endif
/*************************************************************************/ /**
 ** TODO: we should take into account the name-space when searching!!
 ** true everywhere we search/find a uniform from m_uniforms
 ** for now, there will be a bug if a same uniform name is used 2 times, once
 ** in a namespace, and another time outside...
 **/ /*************************************************************************/ 
IUniform*            Container::findUniform(const char * name)
{
    UniformMap::iterator it = m_uniforms.find(name);
    if(it == m_uniforms.end())
    {
        //
        // Now let's look for this guy in the constant buffers.
        //
        CstBufferVec::iterator iC = m_cstBuffers.begin();
        while(iC != m_cstBuffers.end())
        {
            IUniform *pU = (*iC)->findUniform(name);
            if(pU)
                return pU;
            ++iC;
        }
        return NULL;
    }
    else return it->second;
}
/*************************************************************************/ /**
 ** 
 ** 
 **/ /*************************************************************************/ 
IUniform*            Container::findUniform(int i)
{
    if(m_uniforms.empty())
        return NULL;
    UniformMap::iterator it = m_uniforms.begin();
    while(i--)
    {
        ++it;
        if(it == m_uniforms.end())
            return NULL;
    }
    return it->second;
}

/*************************************************************************/ /**
 ** 
 ** 
 **/ /*************************************************************************/ 
ICstBuffer*            Container::findCstBuffer(const char * name)
{
    CstBufferVec::iterator it = m_cstBuffers.begin();
    while(it != m_cstBuffers.end())
    {
        if(!strcmp((*it)->getName(), name))
            break;
        ++it;
    }
    if(it == m_cstBuffers.end())
        return NULL;
    else return *it;
}
/*************************************************************************/ /**
 ** 
 ** 
 **/ /*************************************************************************/ 
ICstBuffer*            Container::findCstBuffer(int i)
{
    if(i >= (int)m_cstBuffers.size())
        return NULL;
    return m_cstBuffers[i];
}

/*************************************************************************/ /**
 ** 
 ** 
 **/ /*************************************************************************/ 
IStateGroupRaster*   Container::createStateGroup_Raster(const char *name)
{
    StateGroupRaster *p = new_StateGroupRaster(this, name);
    m_stateGroupRasters.push_back(p);
    return p;
}
/*************************************************************************/ /**
 ** 
 ** 
 **/ /*************************************************************************/ 
IStateGroupCS*       Container::createStateGroup_CS(const char *name)
{
    StateGroupCS *p = new_StateGroupCS(this, name);
    m_stateGroupCSs.push_back(p);
    return p;
}
/*************************************************************************/ /**
 ** 
 ** 
 **/ /*************************************************************************/ 
IStateGroupDST*      Container::createStateGroup_DST(const char *name)
{
    StateGroupDST *p = new_StateGroupDST(this, name);
    m_stateGroupDSTs.push_back(p);
    return p;
}
#ifndef OGLES2
/*************************************************************************/ /**
 ** 
 ** 
 **/ /*************************************************************************/ 
IStateGroupPath*      Container::createStateGroup_PR(const char *name)
{
    StateGroupPath *p = new_StateGroupPR(this, name);
    m_stateGroupPRs.push_back(p);
    return p;
}
#endif
/*************************************************************************/ /**
 ** \brief creates a constant buffer
 ** 
 **/ /*************************************************************************/ 
ICstBuffer*          Container::createCstBuffer(const char *name, bool bGlobal)
{
    for(int i=0; i<(int)m_cstBuffers.size(); i++)
    {
        if(m_cstBuffers[i] && (!strcmp(m_cstBuffers[i]->getName(), name)))
            return NULL;//m_cstBuffers[i];
    }
    CstBuffer *p;
    if(bGlobal)
    {
        ICstBufferRepositoryEx* pRep = getCstBufferRepositorySingleton()->getExInterface();
        p = static_cast<CstBuffer*>(pRep->createCstBuffer(name));
        if(p == NULL)
            return NULL;
    } else {
        p = new_CstBuffer(name);
    }
    addCstBuffer(p);
    return p;
}
/*************************************************************************/ /**
 ** 
 ** 
 **/ /*************************************************************************/ 
bool Container::addCstBuffer(ICstBuffer* p)
{
    CstBuffer* pp = static_cast<CstBuffer*>(p);
    m_cstBuffers.push_back(pp);
    // Let's walk through the techniques and passes to make sure the existing ones
    // know about this new guy
    TechVec::iterator itTech = m_techniques.begin();
    while(itTech != m_techniques.end())
    {
        Technique::PassVec::iterator itPass = (*itTech)->m_passes.begin();
        while(itPass != (*itTech)->m_passes.end())
        {
            pp->update(itPass->pass, -1, true, true); // targeted update of the uniform: will setup its target list
            ++itPass;
        }
        ++itTech;
    }
    return true;
}

/*************************************************************************/ /**
 ** \brief create a uniform object that can be used to update uniforms in GLSL
 ** (or others like Cg or CUDA...).
 **/ /*************************************************************************/ 
IUniform*      Container::createUniform(const char *name, const char *groupname, const char *semantic, bool bGlobal)
{
    UniformMap::iterator it = m_uniforms.find(name);
    if(it != m_uniforms.end())
        return NULL;//it->second;
    //
    // Now let's look for this guy in the constant buffers.
    // NOTE: I should remove this
    //
    //CstBufferVec::iterator iC = m_cstBuffers.begin();
    //while(iC != m_cstBuffers.end())
    //{
    //    IUniform *pU = (*iC)->findUniform(name);
    //    if(pU) {
    //        CHECK_TRUE(false,"TODO: check if this is a good idea");
    //        return pU;
    //    }
    //    ++iC;
    //}
    //
    // Nothing found... ok. Create
    //
    Uniform *p;
    if(bGlobal)
    {
        IUniformRepositoryEx* pRep = getUniformRepositorySingleton()->getExInterface();
        p = static_cast<Uniform*>(pRep->createUniform(name, groupname, semantic));
        // we may fail : if for example this uniform already exists in the global space
        if(p == NULL)
            return NULL;
    } else {
        p = new_Uniform(name, groupname, semantic);
    }
    addUniform(p);
    return p;
}
/*************************************************************************/ /**
 ** Add a uniform to the container
 ** - return false if already here
 ** - return true otherwise
 **/ /*************************************************************************/ 
bool Container::addUniform(IUniform *p)
{
    Uniform* pp = static_cast<Uniform*>(p);
    UniformMap::iterator iU = m_uniforms.find(pp->getName());
    // check if this uniform name is already there
    if(iU != m_uniforms.end())
        return false;
    m_uniforms[pp->m_name] = pp;
    // Let's walk through the techniques and passes to make sure the existing ones
    // know about this new guy
    TechVec::iterator itTech = m_techniques.begin();
    while(itTech != m_techniques.end())
    {
        Technique::PassVec::iterator itPass = (*itTech)->m_passes.begin();
        while(itPass != (*itTech)->m_passes.end())
        {
            int layerID;
            for(int i=0; (layerID=itPass->pass->getLayerId(i)) >= 0; i++)
                pp->update(pp->m_data, itPass->pass, layerID, true); // targeted update of the uniform: will setup its target list
            ++itPass;
        }
        ++itTech;
    }
    return true;
}

/*************************************************************************/ /**
 ** 
 ** 
 **/ /*************************************************************************/ 
int Container::createUniformsFromPass(IPass *pass)
{
    /*static char name[61];
    GLsizei 	length;
    GLint 	size;
    GLenum 	type;
    for(int i=0; i<2; i++)
    {
        glGetActiveUniform(glslProgram->getProgram(),
        i,
        60,
        &length,
        &size,
        &type,
        name);
    }*/
    return 0;
}
/*************************************************************************/ /**
 ** 
 ** 
 **/ /*************************************************************************/ 
bool            Container::destroy()
{
#ifdef WIN32
    //CHECK_TRUE(false,"Container::destroy() TODO : release refCnt of resources (FBO/TEX/RB and global Shaders");
    OutputDebugStringA("Container::destroy() TODO");
#endif
    //CHECK_TRUE(false,"TODO");
    return false;
}
/*************************************************************************/ /**
 ** 
 ** 
 **/ /*************************************************************************/ 
bool            Container::destroy(ITechnique* p)
{
    TechVec::iterator it = m_techniques.begin();
    while(it != m_techniques.end())
    {
        if(*it == p)
        {
            m_techniques.erase(it);
            delete p;
            p = NULL;
            return true;
        }
        ++it;
    }
    return false;
}
/*************************************************************************/ /**
 ** 
 ** 
 **/ /*************************************************************************/ 
bool            Container::destroy(IShader* p)
{
    // Note: the global repository gets updated when the container adds/removes a shader.
    // and not when a program adds/removes a shader. Reason: a program always belongs to a container
    ShaderModuleRepository* pShdRep = 
        static_cast<ShaderModuleRepository*>(getShaderModuleRepositorySingleton());
    int c = pShdRep->releaseShader(p);
    ShaderVec::iterator it = m_shaders.begin();
    while(it != m_shaders.end())
    {
        if(*it == p)
        {
            (*it)->removeUser(this);
            m_shaders.erase(it);
            return true;
        }
        ++it;
    }
    return false;
}
/*************************************************************************/ /**
 ** 
 ** 
 **/ /*************************************************************************/ 
bool            Container::destroy(ISamplerState* p)
{
    SStateVec::iterator it = m_samplerStates.begin();
    while(it != m_samplerStates.end())
    {
        if(*it == p)
        {
            m_samplerStates.erase(it);
            delete p;
            p = NULL;
            return true;
        }
        ++it;
    }
    return false;
}
/*************************************************************************/ /**
 ** 
 ** 
 **/ /*************************************************************************/ 
bool Container::destroy(IStateGroupRaster* p)
{
    StateGroupRasterVec::iterator it = m_stateGroupRasters.begin();
    while(it != m_stateGroupRasters.end())
    {
        if(*it == p)
        {
            m_stateGroupRasters.erase(it);
            delete p;
            p = NULL;
            return true;
        }
        ++it;
    }
    return false;
}
/*************************************************************************/ /**
 ** 
 ** 
 **/ /*************************************************************************/ 
bool Container::destroy(IStateGroupCS* p)
{
    StateGroupCSVec::iterator it = m_stateGroupCSs.begin();
    while(it != m_stateGroupCSs.end())
    {
        if(*it == p)
        {
            m_stateGroupCSs.erase(it);
            delete p;
            p = NULL;
            return true;
        }
        ++it;
    }
    return false;
}
/*************************************************************************/ /**
 ** 
 ** 
 **/ /*************************************************************************/ 
bool Container::destroy(IStateGroupDST* p)
{
    StateGroupDSTVec::iterator it = m_stateGroupDSTs.begin();
    while(it != m_stateGroupDSTs.end())
    {
        if(*it == p)
        {
            m_stateGroupDSTs.erase(it);
            delete p;
            p = NULL;
            return true;
        }
        ++it;
    }
    return false;
}
#ifndef OGLES2
/*************************************************************************/ /**
 ** 
 ** 
 **/ /*************************************************************************/ 
bool Container::destroy(IStateGroupPath* p)
{
    StateGroupPRVec::iterator it = m_stateGroupPRs.begin();
    while(it != m_stateGroupPRs.end())
    {
        if(*it == p)
        {
            m_stateGroupPRs.erase(it);
            delete p;
            p = NULL;
            return true;
        }
        ++it;
    }
    return false;
}
#endif
/*************************************************************************/ /**
 ** 
 ** 
 **/ /*************************************************************************/ 
bool Container::destroy(IUniform *p)
{
    UniformMap::iterator it = m_uniforms.begin();
    while(it != m_uniforms.end())
    {
        if(it->second == p)
        {
            delete_Uniform(it->second);
            m_uniforms.erase(it);
            return true;
        }
        ++it;
    }
    return false;
}

/*************************************************************************/ /**
 ** \brief used during Pass::validate().
 **
 ** Therefore we also validate things at the same time here
 **/ /*************************************************************************/ 
bool Container::updateUniforms(Pass *pass, int layerID, bool bCreateIfNeeded, bool bOnlyShadowed)
{
    NXPROFILEFUNCCOL2("update Unifs", COLOR_BLUE2, 2);
    bool bRes = true;
    int n = 0;
    UniformMap::iterator it = m_uniforms.begin();
    while(it != m_uniforms.end())
    {
        if((!bOnlyShadowed) || it->second->m_data)
        {
            if(it->second->update(it->second->m_data, pass, layerID, bCreateIfNeeded)) // use the return value to see if we did something
                ++n;
        }
        ++it;
    }
#ifdef USECUDA
    // CUDA required that we map the resources for access in the kernel
    // let's release these resources (unmap)
    it = m_uniforms.begin();
    while(it != m_uniforms.end())
    {
        Uniform::ShadowedData* data;
        switch(it->second->getType())
        {
        case IUniform::TTexture1D:
        case IUniform::TTexture2D:
        case IUniform::TTexture2DRect:
        case IUniform::TTexture3D:
        case IUniform::TTextureCube:
            data = it->second->getShadowedData();
        if(data && data->tex.pRes)
            data->tex.pRes->unmapFromCUDA();
        }
        ++it;
    }
#endif
    return bRes; // return the # of updated uniforms
}

/*************************************************************************/ /**
 ** \brief remove references to the pass in all existing uniforms
 ** 
 **/ /*************************************************************************/ 
void Container::deleteUniformsTarget(Pass *pass, int layerID)
{
    UniformMap::iterator it = m_uniforms.begin();
    while(it != m_uniforms.end())
    {
        it->second->deleteTarget(pass, layerID);
        ++it;
    }
}
/*************************************************************************/ /**
 ** \brief invalidate references to the pass in all existing uniforms
 ** to force re-evaluation of the parameter handle or offsets
 ** 
 **/ /*************************************************************************/ 
void Container::invalidateUniformsTarget(Pass *pass, int layerID)
{
    UniformMap::iterator it = m_uniforms.begin();
    while(it != m_uniforms.end())
    {
        it->second->invalidateTarget(pass, layerID);
        ++it;
    }
}

/*************************************************************************/ /**
 ** \brief used during Pass::validate().
 **
 ** Therefore we also validate things at the same time here
 **/ /*************************************************************************/ 
bool Container::updateCstBuffers(Pass *pass, int layerID, bool bCreateIfNeeded, bool bCreateBufferIfNeeded)
{
    NXPROFILEFUNCCOL2("update CBuffs", COLOR_BLUE3, 2);
    bool bRes = true;
    int n = 0;
    CstBufferVec::iterator it = m_cstBuffers.begin();
    while(it != m_cstBuffers.end())
    {
        CstBuffer* pCBuff = *it;
        if(pCBuff->update(pass, layerID, bCreateIfNeeded, bCreateBufferIfNeeded)) // use the return value to see if we did something
            ++n;
        ++it;
    }
    return bRes; // return the # of updated cstBuffers
}

/*************************************************************************/ /**
 ** \brief remove references to the pass in all existing cstBuffers
 ** 
 **/ /*************************************************************************/ 
void Container::deleteCstBuffersTarget(Pass *pass, int layerID)
{
    CstBufferVec::iterator it = m_cstBuffers.begin();
    while(it != m_cstBuffers.end())
    {
//        assert(0);
      CHECK_TRUE(false);
        //TODO
        //(*it)->deleteTarget(pass, layerID);
        ++it;
    }
}
/*************************************************************************/ /**
 ** \brief invalidate references to the pass in all existing cstBuffers
 ** to force re-evaluation of the parameter handle or offsets
 ** 
 **/ /*************************************************************************/ 
void Container::invalidateCstBuffersTarget(Pass *pass, int layerID)
{
    CstBufferVec::iterator it = m_cstBuffers.begin();
    while(it != m_cstBuffers.end())
    {
        (*it)->invalidateTarget(pass, layerID);
        ++it;
    }
}

/*************************************************************************/ /**
 ** \brief used during Pass::validate().
 **
 ** Therefore we also validate things at the same time here
 **/ /*************************************************************************/ 
bool Container::updateSamplerStates(Pass *pass, int layerID, bool bCreateIfNeeded, bool bOnlyShadowed)
{
    NXPROFILEFUNCCOL2("update SSs", COLOR_BLUE, 2);
    bool bRes = true;
    int n = 0;
    SStateVec::iterator it = m_samplerStates.begin();
    while(it != m_samplerStates.end())
    {
        if((!bOnlyShadowed) || (*it)->m_data)
        {
            if((*it)->update((*it)->m_data, pass, layerID, bCreateIfNeeded)) // use the return value to see if we did something
                ++n;
        }
        ++it;
    }
    return bRes; // return the # of updated uniforms
}

/*************************************************************************/ /**
 ** \brief used during Pass::validate().
 **
 ** Therefore we also validate things at the same time here
 **/ /*************************************************************************/ 
bool Container::validateSamplerStates()
{
    bool bRes = true;
    bool ret = true;
    int n = 0;
    SStateVec::iterator it = m_samplerStates.begin();
    while(it != m_samplerStates.end())
    {
        bRes = (*it)->validate();
        if(bRes) ++n;
        else ret = false;
        ++it;
    }
    return ret;
}

/*************************************************************************/ /**
 ** 
 **/ /*************************************************************************/ 
void Container::deleteSamplerStatesTarget(Pass *pass, int layerID)
{
    SStateVec::iterator it = m_samplerStates.begin();
    while(it != m_samplerStates.end())
    {
        (*it)->deleteTarget(pass, layerID);
        ++it;
    }
}
/*************************************************************************/ /**
 ** 
 **/ /*************************************************************************/ 
void Container::invalidateSamplerStatesTarget(Pass *pass, int layerID)
{
    SStateVec::iterator it = m_samplerStates.begin();
    while(it != m_samplerStates.end())
    {
        (*it)->invalidateTarget(pass, layerID);
        ++it;
    }
}

/*************************************************************************/ /**
 ** 
 **/ /*************************************************************************/ 
/*void Container::deleteTexturesTarget(Pass *pass, int layerID)
{
    STextureVec::iterator it = m_textures.begin();
    while(it != m_textures.end())
    {
        (*it)->deleteTarget(pass, layerID);
        ++it;
    }
}*/
/*************************************************************************/ /**
 ** 
 **/ /*************************************************************************/ 
/*void Container::invalidateTexturesTarget(Pass *pass, int layerID)
{
    STextureVec::iterator it = m_textures.begin();
    while(it != m_textures.end())
    {
        (*it)->invalidateTarget(pass, layerID);
        ++it;
    }
}*/
/*************************************************************************/ /**
 ** 
 ** 
 ** 
 **/ /*************************************************************************/ 
void Container::invalidateTargets(Pass *pass, int layerID)
{
    invalidateCstBuffersTarget(pass, layerID);
    invalidateUniformsTarget(pass, layerID);
    invalidateSamplerStatesTarget(pass, layerID);
}

/*************************************************************************/ /**
 ** 
 ** 
 ** 
 **/ /*************************************************************************/ 
void Container::moveGlobalUniformsToBuffer(CstBuffer* pBuff)
{
    CHECK_POINTER_RET(pBuff);            

    UniformMap::iterator iU = m_uniforms.begin();
    UniformMap::iterator iU2;
    while(iU != m_uniforms.end())
    {
        iU2 = iU;
        ++iU2;
        switch(iU->second->getType())
        {
        // texture part : they can stay outside
        case IUniform::TTexture:
        case IUniform::TTexture1D:
        case IUniform::TTexture2D:
        case IUniform::TTexture2DRect:
        case IUniform::TTexture3D:
        case IUniform::TTextureCube:
            iU = iU2;
            continue;
        }
        Uniform* p = iU->second;
        if (NULL!=pBuff)
        pBuff->m_uniforms.push_back(p);
        p->m_targets.clear();
        //
        // let's create a target for no specific pass : the CstBuffer will have
        // the knowledge of which pass needs it.
        // uniforms attached to a CstBuffer are further away and not directly related
        // to the shaders
        //
        Uniform::STarget t;
        t.ttype = TCSTBUFFER;
        t.valid = true;
        t.dirty = false;
        // there is no way to identify the target, at this stage. So let's create a target for no Pass. 
        // Later one target for a pass will be created
        t.pass = NULL; 
        // put the ptr to allow the uniform to interact with the owner, in this case
        // NOTE: only one owner for a uniform is allowed : if we have many targets, the same CstBuffer ptr will be used (or it's a bug)
        t.pBuffer = pBuff;
        //t.offsetBytes = ...
        p->m_targets.push_back(t);

        m_uniforms.erase(iU);
        iU = iU2;
    }
}

/*************************************************************************/ /**
** This will
** - look for an existing combination of IShader's for the required type
** - look for an existing program in the global repository
** - create one if none available
** - add it to the global repository is it can
**
** a combination of IShader leads to a functional Shader program
**
** later, this Shader program can be used in D3D or GLSL (ARB_separate_shader_object)
**
** We maintain these combinations so that we can avoid redundant shaders :
** if many passes did setup shaders in the same fashion, only one will be shared
**/ /*************************************************************************/ 
/*IShaderProgram*/IProgram* Container::createShaderProgram(ShaderType type, int numShaders, IShader ** pShaders)
{
    //
    // first look for a similar setup
    //
    for(int i=0; i<(int)m_shaderprograms.size(); i++)
    {
        /*IShaderProgram*/Program *pSP = static_cast<Program*>(m_shaderprograms[i]);
        if(pSP->getNumShaders() != numShaders)
            continue;
        int j;
        for(j=0; j<numShaders; j++)
        {
            IShader *pS = pSP->getShader(pShaders[j]);
            if(pS != pShaders[j])
                break;
        }
        if(j == numShaders)
        {
#ifdef NVFXVERBOSE
            LOGD("Found local Program %d (", pSP->getProgram() );
            IShader* pshd;
            for(int ii=0; pshd = pSP->getShader(ii); ii++)
                LOGD("%s ", pshd->getName());
            LOGD(")... using it\n");
#endif
            return pSP;
        }
    }
    //
    // Check if the shaders are all 'global'. If so, check if the program is already there
    //
    ShaderModuleRepository* pRep = 
        static_cast<ShaderModuleRepository*>(getShaderModuleRepositorySingleton());
    int j;
    for(j=0; j<numShaders; j++)
    {
        IShader *pS = pRep->findShader(pShaders[j]->getName());
        if(pS == NULL)
            break;
    }
    /*IShaderProgram*/Program *pSP = NULL;
    if(j == numShaders) // means all shaders are global
    {
        // try to find the program from the shader combination
        pSP = static_cast<Program*>(pRep->findProgram(type, numShaders, pShaders));
    }
    //
    // Not there : create it
    //
    if(pSP == NULL)
    {
        switch(type)
        {
        case FX_CUDAPROG: // for CUDA, create a special version
            pSP = new_ProgramCUDA(this);
            break;
        case FX_PATHPROG:
            pSP = new_ProgramPath(this);
            break;
        case FX_COMPUTEPROG:
            pSP = new_Program(this);
            break;
        case FX_UNKNOWNSHADER:
            CHECK_TRUE(false);
            break;
        default:
            pSP = new_Program(this);
            break;
        }
        for(int i=0; i<numShaders; i++)
        {
            pSP->addShader(type, pShaders[i], this);
        }
#ifdef NVFXVERBOSE
        LOGD("Creating Program %d (", pSP->getProgram() );
        IShader* pshd;
        for(int ii=0; pshd = pSP->getShader(ii); ii++)
            LOGD("%s ", pshd->getName());
        LOGD(")\n");
#endif
        if(j == numShaders) // we may want to share this guy
        {
            LOGD("Program %d made Global\n", pSP->getProgram() );
            pRep->addProgram(pSP);
        }
    } else {
#ifdef NVFXVERBOSE
        LOGD("Program %d (", pSP->getProgram() );
        IShader* pshd;
        for(int ii=0; pshd = pSP->getShader(ii); ii++)
            LOGD("%s ", pshd->getName());
        LOGD(") already there as Global. Using it\n");
#endif
        pRep->addProgram(pSP); // add a Ref
    }
    if(!pSP->link(this))
    {
        return NULL;
    }
    m_shaderprograms.push_back(pSP);
    return pSP;
}
/*************************************************************************/ /**
**
**/ /*************************************************************************/ 
bool    Container::destroy(IProgram* p)
{
    ShaderModuleRepository* pShdRep = 
        static_cast<ShaderModuleRepository*>(getShaderModuleRepositorySingleton());
    ShaderProgramVec::iterator ip = m_shaderprograms.begin();
    while(ip != m_shaderprograms.end())
    {
        if(*ip && ((*ip) == p))
        {
            int refcnt = pShdRep->releaseProgram(*ip); // remove it from the global pool if existed there
            if(refcnt <= 0) // meaning: the program is not (or not anymore) in the global repository.
            {
#ifdef _DEBUG
                LOGD("Program %d (", (*ip)->getProgram());
                IShader* pshd;
                for(int ii=0; pshd = (*ip)->getShader(ii); ii++)
                    LOGD("%s ", pshd->getName());
                LOGD(") no more used... DELETING it\n");
#endif
                // good enough : query the first shader in the program
                switch((*ip)->getShader(0)->getType())
                {
                case TGLSL:
                case THLSL:
                    delete_Program(*ip);
                    break;
                case TCUDA:
                    delete_ProgramCUDA(*ip);
                    break;
                }
            }
            m_shaderprograms.erase(ip);
            return true;
        }
        ++ip;
    }
    return false;
}

/*************************************************************************/ /**
**
**/ /*************************************************************************/ 
bool    Container::addResource(IResource *pRes)
{
    Resource *p2 = static_cast<Resource*>(pRes);
    ResourceVec::iterator it = m_resources.begin();
    while(it != m_resources.end())
    {
        if((*it) == pRes)
            return false;
        ++it;
    }
    m_resources.push_back(p2);
    return true;
}
/*************************************************************************/ /**
**
**/ /*************************************************************************/ 
bool    Container::addFbo(IFrameBufferObject *pFbo)
{
    FrameBufferObject *p2 = static_cast<FrameBufferObject*>(pFbo);
    FboVec::iterator it = m_fbos.begin();
    while(it != m_fbos.end())
    {
        if((*it) == pFbo)
            return false;
        ++it;
    }
    m_fbos.push_back(p2);
    return true;
}
/*************************************************************************/ /**
**
**/ /*************************************************************************/ 
IResource*          Container::createResource(const char * name, ResourceType type, bool bGlobal)
{
    IResource* pRes;
    //if(bGlobal)
    if(!(pRes = findResource(name)))
    {
        IResourceRepositoryEx* pRep = getResourceRepositorySingleton()->getExInterface();
        pRes = pRep->createResource(name, type);
        addResource(pRes);
    }
#pragma MESSAGE("WARNING: for now, forcing the resources to be always global")
// Warning : even if not global, the repository is good for the resizing of resources...
// so we might need to put the resource in the repository even if not global
    //else {
    //    pRes = new_Resource(this, name);
    //    pRes->setType(type);
    //}
    return pRes;
}
/*************************************************************************/ /**
**
**/ /*************************************************************************/ 
IFrameBufferObject* Container::createFBO(const char * name, bool bGlobal)
{
    IFrameBufferObject* pFBO;
    //if(bGlobal)
    {
        IFrameBufferObjectsRepositoryEx* pRep = getFrameBufferObjectsRepositorySingleton()->getExInterface();
        pFBO = pRep->createFBO(name);
        addFbo(pFBO);
    }
// Warning : even if not global, the repository is good for the resizing of FBO resources...
// so we might need to put the resource in the repository even if not global
    //else {
    //    pRes = FrameBufferObject(this, name);
    //}
    return pFBO;
}

/*************************************************************************/ /**
**
**/ /*************************************************************************/ 
IResource* Container::findResource(const char * name)
{
    ResourceVec::iterator it = m_resources.begin();
    while(it != m_resources.end())
    {
        if(!strcmp((*it)->getName(), name))
            return (*it);
        ++it;
    }
    return NULL;
}
IResource* Container::findResource(int i)
{
    if((i<0)||(i>=(int)m_resources.size()))
        return NULL;
    return m_resources[i];
}

int Container::gatherResourcesFromAnnotation(IResource** &ppResources, const char *annotationName, const char* value)
{
    CHECK_TRUE(false);
    return 0;
}
int Container::gatherResourcesFromAnnotation(IResource** &ppResources, const char *annotationName, int value)
{
    CHECK_TRUE(false);
    return 0;
}
int Container::gatherResourcesFromAnnotation(IResource** &ppResources, const char *annotationName, float value)
{
    CHECK_TRUE(false);
    return 0;
}
int Container::gatherResourcesFromNamespace(IResource** &pp, const char *ns)
{
    memset(resArray, 0, 100*sizeof(IResource*));
    pp = (IResource**)resArray;
    int n = 0;
    if(!ns)
        return 0;
    std::string strNS(ns);
    ResourceVec::iterator i2 = m_resources.begin();
    while(i2 != m_resources.end())
    {
        if((*i2)->m_name.ns() == strNS)
        {
            if(n >= 100)
            {
                nvFX::printf(">>Error : gathering of resources exceeded 100 !");
                return n;
            }
            resArray[n++] = (*i2);
        }
        ++i2;
    }
    // warning : means that we cannot put in the same list some global shaders from here with
    // local shader previously found in this method...
    if(n == 0)
    {
        // Try in the global repository
        return getResourceRepositorySingleton()->gatherFromNamespace(pp, ns);
    }
    return n;
}

IFrameBufferObject* Container::findFbo(const char * name)
{
    FboVec::iterator it = m_fbos.begin();
    while(it != m_fbos.end())
    {
        if(!strcmp((*it)->getName(), name))
            return (*it);
        ++it;
    }
    return NULL;
}

int Container::gatherFbosFromAnnotation(IFrameBufferObject** &ppFbos, const char *annotationName, const char* value)
{
    CHECK_TRUE(false);
    return 0;
}
int Container::gatherFbosFromAnnotation(IFrameBufferObject** &ppFbos, const char *annotationName, int value)
{
    CHECK_TRUE(false);
    return 0;
}
int Container::gatherFbosFromAnnotation(IFrameBufferObject** &ppFbos, const char *annotationName, float value)
{
    CHECK_TRUE(false);
    return 0;
}
int Container::gatherFbosFromNamespace(IFrameBufferObject** &pp, const char *ns)
{
    memset(cbArray, 0, 100*sizeof(IFrameBufferObject*));
    pp = (IFrameBufferObject**)fboArray;
    int n = 0;
    if(!ns)
        return 0;
    std::string strNS(ns);
    FboVec::iterator i2 = m_fbos.begin();
    while(i2 != m_fbos.end())
    {
        if((*i2)->m_name.ns() == strNS)
        {
            if(n >= 100)
            {
                nvFX::printf(">>Error : gathering of fbos exceeded 100 !");
                return n;
            }
            fboArray[n++] = (*i2);
        }
        ++i2;
    }
    // warning : means that we cannot put in the same list some global shaders from here with
    // local shader previously found in this method...
    if(n == 0)
    {
        // Try in the global repository
        return getFrameBufferObjectsRepositorySingleton()->gatherFromNamespace(pp, ns);
    }
    return n;
}
