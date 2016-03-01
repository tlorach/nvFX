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
#ifndef __FX_H__
#define __FX_H__
#ifdef MEMORY_LEAKS_CHECK
	#pragma message("build will Check for Memory Leaks!")
	#define _CRTDBG_MAP_ALLOC
	#include <stdlib.h>
	#include <crtdbg.h>
    inline void* operator new(size_t size, const char *file, int line)
    {
       return ::operator new(size, 1, file, line);
    }

    inline void __cdecl operator delete(void *ptr, const char *file, int line) 
    {
       ::operator delete(ptr, _NORMAL_BLOCK, file, line);
    }

    #define DEBUG_NEW new( __FILE__, __LINE__)
    #define MALLOC_DBG(x) _malloc_dbg(x, 1, __FILE__, __LINE__);
    #define malloc(x) MALLOC_DBG(x)
    #define new DEBUG_NEW
#endif
#include <vector>
#include <map>
#include <set>
#include <string>
#include <string.h>

#ifdef _DEBUG
#include <assert.h>
#define CHECK_TRUE(a) assert(a)
#define CHECK_TRUE_MSG(cond,msg) { if(!(cond)) {assert(!msg);}}
#define CHECK_POINTER(a) {if (a == NULL) assert(0);}
#define CHECK_TRUE_RET(cond) {if (!(cond)) return;}
#define CHECK_POINTER_RET(ptr) {if (ptr == NULL) return;}
#else
#define CHECK_TRUE(a)
#define CHECK_TRUE_MSG(cond,msg)
#define CHECK_POINTER(a)
#define CHECK_TRUE_RET(cond)
#define CHECK_POINTER_RET(ptr)
#endif

#ifndef OGLES2
#   define USEGLSLCOMPUTE
#endif

// This allows to ignore/display pragma message that are in the code, while compiling
#ifdef DISPLAY_TODO_MSGS
#define MESSAGE(m) message(m)
#else
#define MESSAGE(m)
#endif

// USE_OLDPROGRAM switches back to the regular single GLSL program for all the stages of the rendering pipeline
// It is the only one available under ES2
#ifdef OGLES2
#   define USE_OLDPROGRAM
#endif
// SHADERCONCAT_USE is a needed simplification to make ES2 work. This flag will be set when targetting ES2. But can work on regular OGL, too
// Note that D3D needs this because there is no Shader object
//#define SHADERCONCAT_USE
//#define USE_OLDPROGRAM

#ifndef LOGD
#if defined(NVFXVERBOSE)
#   define  LOGD(...)  { nvFX::printf(__VA_ARGS__); }
#else
#   define  LOGD(...)
#endif
#endif
#ifndef LOGI
#   define  LOGI(...)  { nvFX::printf(__VA_ARGS__); }
#endif
#ifndef LOGE
#if defined(WIN32) && defined(_DEBUG)
#   define  LOGE(...)  { /*DebugBreak();*/ nvFX::eprintf(__VA_ARGS__); }
#else
#   define  LOGE(...)  { nvFX::eprintf(__VA_ARGS__); nvFX::eprintf("\n"); }
#endif
#endif

#ifdef USE_NSIGHT
#include "NSightEvents.h"
#else
#	define NX_RANGE int
#	define NX_MARK(name)
#	define NX_RANGESTART(name) 0
#	define NX_RANGEEND(id)
#	define NX_RANGEPUSH(name)
#	define NX_RANGEPUSHCOL(name, c)
#	define NX_RANGEPOP()
#	define NXPROFILEFUNC(name)
#	define NXPROFILEFUNCCOL(name, c)
#	define NXPROFILEFUNCCOL2(name, c, a)
#endif
#define COLOR_RED     0xFFFF0000
#define COLOR_RED2    0xFFAA0000
#define COLOR_ORANGE  0xFFFFA040
#define COLOR_GREEN   0xFF00FF00
#define COLOR_GREEN2  0xFF00AA00
#define COLOR_GREEN3  0xFFB6FF00
#define COLOR_BLUE    0xFF0000FF
#define COLOR_BLUE2   0xFF0000AA
#define COLOR_BLUE3   0xFF0000C8
#define COLOR_YELLOW  0xFFFFFF00
#define COLOR_YELLOW2 0xFFAAAA00
#define COLOR_MAGENTA 0xFFFF00FF
#define COLOR_MAGENTA2 0xFFAA00AA
#define COLOR_CYAN    0xFF00FFFF
#define COLOR_CYAN2   0xFF00AAAA

#include "FxLibEx.h"

namespace nvFX
{
#ifdef USE_NSIGHT
extern void initNSight();
extern nvtxRangePushEx_Pfn nvtxRangePushEx_dyn;
extern nvtxRangePush_Pfn   nvtxRangePush_dyn;
extern nvtxRangePop_Pfn    nvtxRangePop_dyn;
extern nvtxEventAttributes_t eventAttr;
#endif
class Container;
class Shader;
class Technique;
class Pass;
class Uniform;
class CstBuffer;
class PassState;
class SamplerState;
class StateGroupRaster;
class StateGroupCS;
class StateGroupDST;
class StateGroupPath;
//class ShaderProgram;
class Program;
class ProgramPipeline;
class Resource;
class FrameBufferObject;
class FrameBufferObjectsRepository;
class UniformObjectsRepository;
class CstBufferObjectsRepository;
class ResourceRepository;

extern messageCallbackFunc g_errFunc;
extern messageCallbackFunc g_msgFunc;

///< return an id to be used for scene-level layer Id, for example...
/// meant to help getting a unique Id in any case
extern void getID(int *ids, int numids);

extern Resource*           new_Resource(ResourceRepository *pCont, const char* name);
extern void                delete_Resource(Resource *p);

extern ProgramPipeline*    new_ProgramPipeline(Container *pCont);
extern void                delete_ProgramPipeline(ProgramPipeline *p);
//extern ShaderProgram*      new_ShaderProgram(Container *pCont, ShaderType type, int numShaders, IShader ** pShaders);
//extern void                delete_ShaderProgram(ShaderProgram *p);

extern SamplerState*       new_SamplerState(Container *pCont, const char* name);
extern void                delete_SamplerState(SamplerState *p);
extern StateGroupRaster*   new_StateGroupRaster(Container *pCont, const char* name);
extern void                delete_StateGroupRaster(StateGroupRaster *p);
extern StateGroupCS*       new_StateGroupCS(Container *pCont, const char* name);
extern void                delete_StateGroupCS(StateGroupCS *p);
extern StateGroupDST*      new_StateGroupDST(Container *pCont, const char* name);
extern void                delete_StateGroupDST(StateGroupDST *p);
#ifndef OGLES2
extern StateGroupPath*     new_StateGroupPR(Container *pCont, const char* name);
extern void                delete_StateGroupPR(StateGroupPath *p);
#endif

extern Program*            new_Program(Container *pCont);
extern void                delete_Program(IProgram *pProg);

extern Shader*             new_GLSLShader(const char* name);
extern Shader*             new_HLSL10Shader(const char* name);
extern Shader*             new_HLSL11Shader(const char* name);
extern void                delete_Shader(IShader *pShd);

extern Program*            new_ProgramPath(Container *pCont);
extern Shader*             new_ShaderPath(const char* name, bool bPostscript);

#ifdef USECUDA
extern Shader*             new_ShaderCUDA(const char* name);
extern Program*            new_ProgramCUDA(Container *pCont);
extern void                delete_ShaderCUDA(IShader *pShd);
extern void                delete_ProgramCUDA(IProgram *pProg);
#else
inline Shader*             new_ShaderCUDA(const char* name) { return NULL; }
inline Program*            new_ProgramCUDA(Container *pCont) { return NULL; }
inline void                delete_ShaderCUDA(IShader *pShd) {}
inline void                delete_ProgramCUDA(IProgram *pProg) {}
#endif
extern CstBuffer*          new_CstBuffer(const char* name = NULL);
extern void                delete_CstBuffer(CstBuffer *p);
extern Uniform*            new_Uniform(const char* name = NULL, const char* groupname = NULL, const char* semantic = NULL);
extern void                delete_Uniform(Uniform *pProg);
extern bool                checkGLError(const char *str);
extern bool                buildCodeFromUniforms(IContainer* pCont);
#ifdef USECUDA
extern bool                buildCodeFromUniformsCUDA(IContainer* pCont);
#endif

typedef std::map<int, std::string> AttribMap;

/*************************************************************************/ /**
 ** \brief extends the string to manage "namespace".
 **
 ** \note: '::' \e cannot be in shader codes. So we substitute '::' from nvFX to '_NS_'
 ** so that it always compiles
 ** \note: OpenGL driver reserved '__' and '___' for parameters. NO WAY to use them !!!
 **/ /*************************************************************************/ 
class StringName
{
private:
    std::string m_name;
public:
    StringName(const char *s=NULL)
    {
        if(s){
            m_name = s;
            int n = (int)m_name.find("::");
            if(n >= 0)
                m_name.replace(n, 2, "_NS_");
        }
    }
    const char* c_str() { return m_name.c_str(); }
    std::string name() { return m_name; }
    std::string ns() {
        int n = (int)m_name.find("_NS_");
        if(n >= 0)
            return m_name.substr(0, n-1);
        return std::string("");
    }
    void set(const char* name, const char* ns)
    { 
        if(ns && (ns[0]!='\0'))
            m_name = std::string(ns) + std::string("_NS_") + std::string(name);
        else
            m_name = std::string(name);
    }
    void set(std::string name, std::string ns)
    {
        if(ns.empty())
            m_name = name;
        else
            m_name = ns + std::string("_NS_") + name;
    }
    operator std::string&()
    {
        return m_name;
    }
    //StringName& operator=(const std::string &s)
    //{
    //    m_name = s;
    //    int n = (int)m_name.find("::");
    //    if(n >= 0)
    //        m_name.replace(n, 2, "_NS_");
    //    return *this;
    //}
    bool operator==(const StringName& s) const
    {
        return(s.m_name ==  m_name);
    }
    bool operator==(const std::string& s) const
    {
        return (s ==  m_name);
    }
};

/*************************************************************************/ /**
 ** 
 ** 
 **/ /*************************************************************************/ 
class Annotation : public IAnnotationEx
{
private:
    typedef std::map<std::string, std::string> StringMap;
    typedef std::map<std::string, float> FloatMap;
    typedef std::map<std::string, int> IntMap;

    StringMap   m_strings;
    FloatMap    m_floats;
    IntMap      m_ints;
public:
    virtual bool         addAnnotation(const char *annotName, const char * str);
    virtual bool         addAnnotation(const char *annotName, float f);
    virtual bool         addAnnotation(const char *annotName, int i);
    virtual bool         delAnnotation(const char *annotName);
    virtual const char * getAnnotationString(const char *annotName);
    virtual float        getAnnotationFloat(const char *annotName);
    virtual int          getAnnotationInt(const char *annotName);
    virtual bool         copy(IAnnotation* pIAnnotations);
    virtual bool         clear();

    bool                 serialize(FILE* fd, bool asCPP);

    friend class Container;
    friend class Uniform;
};
/*************************************************************************/ /**
 ** NOTE: a shader must be in a domain (vertex/frag...)
 ** - If we want to share it between domains : must be created many times in each
 ** - data must be consistent
 ** - we can concatenate many shaders together
 ** - we can prototype some functions : linkage will choose the right implementation
 ** - we need to differenciate between code chunk to copy and code chunk to link
 ** - add a name at creation time
 ** 
 **/ /*************************************************************************/ 
class Shader : public IShaderEx
{
protected:
    StringName          m_name;
    Annotation          m_annotations;
    TargetType          m_targetType;
    /// Keep track of who is using this shader module.
    /// two possible users: 1/ the effect, as the owner; 2/ a program used in a pass
    enum SUserType { User_EffectContainer, User_Program };
    struct SUser {
        SUserType type;
        union {
            Program*    program;
            Container*  container;
        };
    };
	struct less	{
	    bool operator()(const SUser& _Left, const SUser& _Right) const { return (_Left.program < _Right.program); }
	};
    typedef std::set<SUser, less> UserSet;
    UserSet             m_users;
    ///.keeps track of areas in the shader code to map line # with effect line #
    struct codeInfo {
        codeInfo(int a, int b, int c, const char *s) { lineInFX=a; lineinShader=b; totalLinesInFX=c; if(s) fname = s; }
        int lineInFX;
        int lineinShader;
        int totalLinesInFX;
        std::string fname;
    };
    std::vector<codeInfo>   m_startLineNumbers;
    int                     m_totalLines;
    std::string             m_shaderCode; ///< this string will hold shader code until we finally compile it
    /// a map of GLSL objects sorted with the domain (type) : vertex, fragment...
    typedef std::map<GLenum, GLhandleARB> ShaderObjects;
    ShaderObjects           m_shaderObjs;
    /// \name Kernel specific (CUDA and possible others)
    /// @{
    /// if ever we use this shader code as a kernel code
    std::string                         m_kernelName;
    /// array of arguments, if needed
    std::vector<IShaderEx::Argument>    m_kernelArgs;
    /// @}
    void    releaseMe();
public:
    Shader(const char *name = NULL);
    virtual ~Shader();
    virtual const char* getName()               { return m_name.c_str(); }
    virtual void    setName(const char *name)   { m_name = name; }
    virtual IAnnotation *annotations() { return &m_annotations; }
    virtual TargetType  getType()               { return m_targetType; }
    virtual const char* getShaderCode()         { return m_shaderCode.c_str(); }
    virtual int         getShaderCodeSz()       { return (int)m_shaderCode.size(); }
    /// \arg \b type can be 0 to 4 (FX_VTXPROG FX_GEOMPROG FX_FRAGPROG FX_TCSPROG FX_TESPROG)
    virtual bool    loadShaderCodeFromFile(const char *fname);
    virtual bool    loadShaderCode(const char *code, int startLineNumber=0, const char *fname=NULL);
    /// \brief adds some code as if we did #include...
    virtual bool    addHeaderCode(const char *code, int startLineNumber=0, const char *fname=NULL);
    /// \brief appends some code at the end
    virtual bool    appendCode(const char *code, int startLineNumber=0, const char *fname=NULL);
    virtual void    cleanupSourceCode();
    /// \name CUDA-specific
    /// @{
    virtual bool            setAsKernel(int nArgs, Argument* args, const char * funcName=NULL);
    virtual Argument        getArgument(int i);
    virtual const char *    isUsedAsKernel();
    /// @}
    void    addUser(Program *program);
    void    removeUser(Program *program);
    void    addUser(Container* pCont);
    void    removeUser(Container* pCont);
    friend class Container;
    friend class ShaderModuleRepository;
};
/*************************************************************************/ /**
 ** a Program is intended to be used through a pass. The pass can either
 ** have one program (non-separable case) or many programs combined together
 ** thanks to the "Shader Pipeline"
 ** if a Program doesn't have anymore any target (m_targets), it means we should
 ** delete this program since no use anywhere
 ** 
 **/ /*************************************************************************/ 
class Program : public IProgramEx
{
protected:
    Container*          m_container;
    /// Keep track of who is using this program.
    /// the only target for a program is the pass through its layerID
    struct STarget {
        Pass*      pass;
        int         passLayerId; ///< ID of which program to take in the pass
    };
    std::vector<STarget>   m_targets;
    void                addTarget(Pass* p, int layerID);
    int                 releaseTarget(Pass* p, int layerID);
public:
    Program(Container *pCont) : m_container(pCont) {}
    ~Program() {}
    virtual int         createKernelEntry(const char*kernelName, int *gridSz/*[3]*/, int *blockSz/*[3]*/, int ShMemSz,int numArgs, ArgVal *args) { return -1; }
    virtual bool        executeKernelEntry(int entryID) { return false; }
    virtual int         getASMCode(char* buffer, int bufLen) { return 0; }
    virtual bool execute(int szx, int szy, int szz=1) { return false; }
    virtual bool execute(RenderingMode mode, const PassInfo::PathInfo *p) { return false; }

    friend class Pass;
};


/*************************************************************************/ /**
 ** \brief Frame buffer Object repository
 ** This repository is \e outside of any effect container. The reason is that
 ** resources should be shared accross effects and even available for the app
 **
 **/ /*************************************************************************/ 
class FrameBufferObjectsRepository : public IFrameBufferObjectsRepositoryEx
{
protected:
    int         m_vp[4]; ///< keep track of the main client window viewport for "application-dependent" resources
    int         m_msaa[2]; ///< keep track of the application msaa mode
    BufferHandle m_backbuffer; ///< keep track of the backbuffer reference (FBO or D3D interface...)
    BufferHandle m_backbufferDST; ///< keep track of the backbuffer reference (D3D interface DST...)
#ifdef WIN32
    const void*       m_pDevice; // typically needed for D3D case
#endif
    //BaseOwner*  m_pOwner;
    struct Fbo {
        Fbo(FrameBufferObject* p_=NULL, int refCnt_=0) : p(p_), refCnt(refCnt_) {}
        FrameBufferObject* p;
        int refCnt;
    };
    typedef std::map<std::string, Fbo> FboMap;
    FboMap      m_FBOs;
    void        destroy(int i);
    void        destroy(IFrameBufferObject* p);
    bool        checkFramebufferStatus();
    bool        removeFBO(FrameBufferObject *p);
public:
    FrameBufferObjectsRepository();
    virtual ~FrameBufferObjectsRepository();
    IFrameBufferObject* createFBO(const char * name);
    int         addFBO(IFrameBufferObject* p);
    int         releaseFbo(IFrameBufferObject* p);
    bool        setParams(int x, int y, int w, int h, int depthSamples, int coverageSamples, BufferHandle backbuffer, BufferHandle backbufferDST, void *pDev);
    bool        validateAll();
    bool        updateValidated();
    bool        finish();

    IFrameBufferObject*      find(const char * fboName);
    IFrameBufferObject*      find(int i);

    int gatherFromAnnotation(IFrameBufferObject** &pp, const char *annotationName, const char* value, int *ival, float *fval);
    int gatherFromAnnotation(IFrameBufferObject** &pp, const char *annotationName, const char* value=NULL);
    int gatherFromAnnotation(IFrameBufferObject** &pp, const char *annotationName, int value);
    int gatherFromAnnotation(IFrameBufferObject** &pp, const char *annotationName, float value);
    int gatherFromNamespace(IFrameBufferObject** &pp, const char *ns);

    bool        blit(IFrameBufferObject* pDst, IFrameBufferObject* pSrc);
    bool        setCurrent(IFrameBufferObject* pFBO, bool bAdjustViewport);

    //bool        activate(IFrameBufferObject* pFBO);
    //bool        setDrawBuffers(IFrameBufferObject* pFBO);

#ifdef WIN32
    const void*          getDevice() { return m_pDevice; }
    void                 setDevice(const void* pDev) { m_pDevice = pDev; }
#endif
    BufferHandle getBackBuffer() { return m_backbuffer; }
    BufferHandle getBackBufferDST() { return m_backbufferDST; }

    friend class IFrameBufferObjectsRepository;
    friend class FrameBufferObject;
    friend class Container;
};

/*************************************************************************/ /**
 ** \brief Uniform repository
 ** This repository is \e outside of any effect container. The reason is that
 ** resources should be shared accross effects and even available for the app
 **
 **/ /*************************************************************************/ 
class UniformRepository : public IUniformRepositoryEx
{
protected:
    struct Item {
        Item(Uniform* p_=NULL, int refCnt_=0) : p(p_), refCnt(refCnt_) {}
        Uniform* p;
        int refCnt;
    };
    typedef std::map<std::string, Item> ItemMap;
    ItemMap      m_Items;
    bool        remove(Uniform *p);
public:
    UniformRepository();
    virtual ~UniformRepository();
    IUniform*   createUniform(const char *name, const char *groupname, const char *semantic);
    int         addUniform(IUniform* p);
    int         releaseUniform(IUniform* p);

    IUniform*  find(const char * Name);
    IUniform*  find(IUniform* p);
    IUniform*  findFromSemantic(const char * sem);
    IUniform*  get(int i);

    int gatherFromAnnotation(IUniform** &pp, const char *annotationName, const char* value, int *ival, float *fval);
    int gatherFromAnnotation(IUniform** &pp, const char *annotationName, const char* value=NULL);
    int gatherFromAnnotation(IUniform** &pp, const char *annotationName, int value);
    int gatherFromAnnotation(IUniform** &pp, const char *annotationName, float value);
    int gatherFromNamespace(IUniform** &pp, const char *ns);
};

/*************************************************************************/ /**
 ** \brief Uniform repository
 ** This repository is \e outside of any effect container. The reason is that
 ** resources should be shared accross effects and even available for the app
 **
 **/ /*************************************************************************/ 
class CstBufferRepository : public ICstBufferRepositoryEx
{
protected:
    struct Item {
        Item(CstBuffer* p_=NULL, int refCnt_=0) : p(p_), refCnt(refCnt_) {}
        CstBuffer* p;
        int refCnt;
    };
    typedef std::map<std::string, Item> ItemMap;
    ItemMap      m_Items;
    bool        remove(CstBuffer *p);
public:
    CstBufferRepository();
    virtual ~CstBufferRepository();
    ICstBuffer*   createCstBuffer(const char *name);
    int         addCstBuffer(ICstBuffer* p);
    int         releaseCstBuffer(ICstBuffer* p);

    ICstBuffer*  find(const char * Name);
    ICstBuffer*  find(ICstBuffer* p);
    ICstBuffer*  findFromSemantic(const char * sem);
    ICstBuffer*  get(int i);

    int gatherFromAnnotation(ICstBuffer** &pp, const char *annotationName, const char* str, int *ival, float *fval);
    int gatherFromAnnotation(ICstBuffer** &pp, const char *annotationName, const char* value=NULL);
    int gatherFromAnnotation(ICstBuffer** &pp, const char *annotationName, int value);
    int gatherFromAnnotation(ICstBuffer** &pp, const char *annotationName, float value);
    int gatherFromNamespace(ICstBuffer** &pp, const char *ns);
};

/*************************************************************************/ /**
 ** \brief
 **
 **/ /*************************************************************************/ 
class ResourceRepository : public IResourceRepositoryEx
{
protected:
    int         m_vp[4]; ///< keep track of the main client window viewport for "application-dependent" resources
    int         m_msaa[2]; ///< keep track of the application msaa mode
    BufferHandle m_backbuffer; ///< keep track of the backbuffer reference (FBO or D3D interface...)
    BufferHandle m_backbufferDST; ///< keep track of the backbuffer reference (FBO or D3D interface...)
    const void*  m_pDevice; // typically needed for D3D case
    //BaseOwner*				m_pOwner;
    struct Res {
        Res(Resource* p_=NULL, int refCnt_=0) : p(p_), refCnt(refCnt_) {}
        Resource* p;
        int refCnt;
    };
    typedef std::map<std::string, Res> ResourceMap;
    ResourceMap m_resources;
    //bool		createRenderBuffer(int w, int h, GLenum format, int depthSamples, int coverageSamples);
    //Resource* 	loadTextureFromFile(LPCSTR fname/*, CGtype type*/);
public:
    ResourceRepository();
    virtual ~ResourceRepository();

    bool        finish();
    IResource*  createResource(const char * name, ResourceType type);
    int         addResource(IResource* pRes);
    int         releaseResource(IResource* pRes);

    bool        setParams(int x, int y, int w, int h, int depthSamples, int coverageSamples, BufferHandle backbuffer, BufferHandle backbufferDST, void *pDev);
    bool        validateAll();
    bool        updateValidated();
    bool        invalidateUnusedResources();
    //IResource*  createTexture2DRandomRays(const char * name, int nbDirs); // for HBAO

    IResource*  loadTexture(const char * texName/*, CGtype samplerType*/);
    IResource*  loadTexture3DSlices(const char * texName, const char * *fileNames, int nFiles);
    void        releaseTextureID(GLuint texID);

    IResource*  find(const char * texName);
    IResource*  find(GLuint texID); ///< \brief for OpenGL : GL ID of the texture
    IResource*  find(void *p); ///< \brief for D3D : pointer to interface
    IResource*  get(int i); ///< \brief return item i. NULL if NA

    int gatherFromAnnotation(IResource** &pp, const char *annotationName, const char* value, int *ival, float *fval);
    int gatherFromAnnotation(IResource** &pp, const char *annotationName, const char* value=NULL);
    int gatherFromAnnotation(IResource** &pp, const char *annotationName, int value);
    int gatherFromAnnotation(IResource** &pp, const char *annotationName, float value);
    int gatherFromNamespace(IResource** &pp, const char *ns);

    bool                removeResource(Resource *p);
    const int *          getViewport() { return m_vp; }
    const int *          getMsaa() { return m_msaa; }
    const void*          getDevice() { return m_pDevice; }
    void                 setDevice(const void* pDev) { m_pDevice = pDev; }

    //friend class Resource;
};
/*************************************************************************/ /**
 **
 **
 **/ /*************************************************************************/ 
class ShaderModuleRepository : public IShaderModuleRepositoryEx
{
protected:
    struct Shd {
        Shd(IShader* p_=NULL, int c_=0) : p(p_), refCnt(c_) {}
        IShader*    p;
        int         refCnt;
    };
    struct Prog {
        Prog(IProgram* p_, int c_) : p(p_), refCnt(c_) {}
        IProgram*    p;
        int         refCnt;
    };
    typedef std::map<std::string, Shd>  ShdMap;
    typedef std::vector<Prog>           ProgVec;
    ShdMap          m_shaders;
    ProgVec         m_programs;

public:
    virtual ~ShaderModuleRepository();
    virtual IShader* findShader(const char * name, TargetType t=TANY);
    int         gatherFromAnnotation(IShader** &ppShaders, const char *annotationName, const char* str, int *ival, float *fval);
    virtual int gatherFromAnnotation(IShader** &ppShaders, const char *annotationName, const char* value=NULL);
    virtual int gatherFromAnnotation(IShader** &ppShaders, const char *annotationName, int value);
    virtual int gatherFromAnnotation(IShader** &ppShaders, const char *annotationName, float value);
    virtual int gatherFromNamespace(IShader** &ppShaders, const char *ns);
    virtual IProgram*   findProgram(ShaderType type, int numShaders, IShader ** pShaders);
    virtual int addShader(IShader *p);
    virtual int addProgram(IProgram* p);
    virtual int releaseShader(IShader *p);
    virtual int releaseProgram(IProgram* p);
    virtual IShader*    getShader(int i);
    virtual IProgram*   getProgram(int i);
};
/*************************************************************************/ /**
 ** 
 ** 
 **/ /*************************************************************************/ 
class CstBuffer : public ICstBufferEx
{
protected:
    struct STarget {
        TargetType  ttype;
        bool        valid;
        bool        dirty;
        Pass*       pass;
        int         passLayerId; ///< this is the ID in the pass for which program to take
        int         shaderProgramNumber; ///< this is the number of shader program target. In OpenGL separable case, this value can be more than 0 if the target aimed the second shader for a fragment prog, for example
        union {
            int     uniformLocation;
            int     bufferIndex; // D3D Constant buffer offset for VS
            void *      cudaHandle; //To see later...
        };
    };
    typedef std::vector<Uniform*> UniformVec;
    StringName              m_name;
    // TODO: write the code when some targets need to be removed
    std::vector<STarget>    m_targets;
	int						m_activeTarget; // keep track of the last target used (inside a Pass::execute)

    bool                    m_ownBufferId;
    unsigned int            m_bufferId; ///< typically GLSL buffer Id
	int						m_sizeMultiplier; ///< a multiplier, to allocate a bigger buffer for many occurences
	int						m_sizeOfCstBufferAligned; ///< size of the constant buffer with alignment taken into account
	int						m_sizeOfCstBuffer; ///< Size of the constant buffer
    void *                  m_stagingBuffer; ///< temporary storage were data are put together before being sent to the driver (especially for OpenGL)
	int						m_bufferOffset; ///< offset within the constant buffer attached here
    void *                  m_pBufferInterface; ///< any pointer (for example : D3D buffer interface)
    UniformVec              m_uniforms;
    Annotation              m_annotations;

    CstBuffer(const char* name);
    virtual ~CstBuffer();
    /// \brief update for a specific target only
    virtual CstBuffer*    updateForTarget(int target) = 0;
    virtual int           bufferSizeAndData(char *pData, int *sz=NULL) = 0;
public:
    bool        getDirty(Pass *pass=NULL); ///< get Dirty status for either all targets or one specific, using pass ptr
    void        setDirty(bool bYes, Pass *pass=NULL); ///< set Dirty flag. Either for all targets or one specific
    /// \brief invalidates a target pass : in GLSL, the uniform location will be invalidated, for example
    virtual CstBuffer*  invalidateTarget(IPass *pass, int layerID);

    virtual IAnnotation *annotations() { return &m_annotations; }
    virtual CstBuffer*  update(Pass *pass, int layerID, bool bCreateIfNeeded, bool bCreateBufferIfNeeded) = 0;
    virtual const char* getName() { return m_name.c_str(); }
    virtual void        setName(const char *name) { m_name = name; }
    virtual ICstBuffer* setGLBuffer(int buffer);
    virtual int         getGLBuffer() { return m_bufferId; }
    virtual ICstBuffer* setD3DBuffer(void *buffer);
    virtual void*       getD3DBuffer() { return m_pBufferInterface; }
    virtual IUniform*   createUniform(const char *name, const char *semantic=NULL);
    virtual int         buildGLBuffer(BufferUsageGL usage, int sizeMultiplier) { return 0; }
    virtual void*       buildD3DBuffer(BufferUsageD3D usage, int sizeMultiplier) { return NULL; }
    virtual IUniform*   findUniform(const char * name);
    virtual IUniform*   findUniform(int i);
    virtual bool        updateFromUniforms(bool bForceUpdating) { return false; }
    virtual bool        mapBuffer(void** pBuf, bool bForceUpdating=false) {return false; }
    virtual void        unmapBuffer() { }
    virtual bool        update() { return false; }
	virtual bool		bindBufferRange(int n, IPass* pPass);
	virtual int			getBufferRange() { return m_bufferOffset; }

    friend class Container;
    friend class Pass;
    friend class CstBufferRepository;
};

/*************************************************************************/ /**
** \brief Uniform Parameter class
**/ /*************************************************************************/ 
class Uniform : public IUniformEx
{
protected:
    struct STarget {
        TargetType  ttype;
        bool        valid;
        bool        dirty;
        Pass*      pass;
        int         passLayerId; ///< ID of which program to take in the pass
        int         shaderProgramNumber; ///< this is the number of shader program target. In OpenGL separable case, this value can be more than 0 if the target aimed the second shader for a fragment prog, for example
        union {
            struct { // ttype is TCSTBUFFER
                CstBuffer*  pBuffer; ///< buffer to which this uniform applies
                int         offsetBytes; ///< offset in the buffer
                int         size;
            };
            struct { // ttype is TGLSL
                int     uniformLocation;
                GLenum  shadertype; // for when the location is for a subroutine
            };
            int         bufferIndex; // D3D Constant buffer offset
            void *      cudaTexRef; //To see later...
        };
    };
    Annotation              m_annotations;
    StringName              m_name;
    std::string             m_groupname; // essentially used when created from "Introspec"
    std::string             m_semantic; // essentially used when created from "Introspec"
    // TODO: write the code when some targets need to be removed
    std::vector<STarget>    m_targets;
	int						m_activeTarget; // keep track of the last target used (inside a Pass::execute)

    Type                    m_type;
    PrecisionType           m_precision;
    int                     m_idx;
    int                     m_arraySz;
    // TODO: take care of arrays, too...
    // TODO: set the arrays ivals/fvals/subroutineNames to [1] and change sizes at allocation time...
public:
    struct ShadowedData {
        struct Tex {
            int             textureUnit;// GLTexture
            ISamplerState *  pSState;
            TextureType      textureType;
            Resource*        pRes;
            union {
                int          textureID;
                void*        m_d3dResourceView;
            };
            // Image Load/Store (GL_ARB_shader_image_load_store)
            bool             bLayered;
            //int              level; // To add
            int              layer;  ///< layer number
            AccessMode       access; ///< READ_ONLY/WRITE_ONLY/READ_WRITE. 0 if normal texture work
        };
        bool            tooSmall(IUniform::Type type, size_t nitems, size_t* pSz=NULL);
        ShadowedData*   allocate(IUniform::Type type, size_t nitems=1);
        ShadowedData*   copy(const ShadowedData* pSrc);
        void            free();
        size_t          size;
        union {
            // NOTE: added more room here for the case where we pass arrays of vectors : 8 vec4 max
            // TODO: dynamically allocate them...
//#pragma MESSAGE(__FILE__ "(257) : TODO TODO TODO TODO : dynamically allocate the right size depending on what is needed. Declate arrays like bvals[1]...")
            bool                bvals[1];
            int                 ivals[1];
            float               fvals[1];
            Uniform *           pUniform; // for TUniform (reference to a uniform)
            void *              pVoid; // any pointer (for example : D3D buffer
#ifndef OGLES2
            char*               subroutineNames[1]; //max == arbitrarily max of 16 subroutines...
#endif
            Tex                 tex;
        };
    }; // ShadowedData
    // casting and offset of the pointer to directly reach the area of values, bypassing the size
    // this is used when casting a float, matrix etc. to a pseudo ShadowedData
    // NOTE: obviously, the field 'size' of ShadowedData is then invalid... but in this case we don't use it !
    #define WILDCAST_AS_SHADOWEDDATA(a) (ShadowedData*)((char*)a-sizeof(size_t))
protected:
    ShadowedData*           m_data;

    void                setTargetDirtyFlag(STarget &t, bool bDirty);
public:
    virtual ~Uniform();
    Uniform(const char* name = NULL, const char* groupname = NULL, const char* semantic = NULL);
    virtual Uniform*    update(ShadowedData* pData, Pass *pass, int layerID, bool bCreateIfNeeded) = 0;
    ShadowedData* getShadowedData() { return m_data; }
    /// \brief returns the interface for annotations
    virtual IAnnotation *annotations() { return &m_annotations; }
    const char* getGroupName() { return m_groupname.c_str(); }
    const char* getSemantic()  { return m_semantic.c_str(); }
    const char* getName() { return m_name.c_str(); }
    void        setName(const char *name) { m_name = name; }
    void        setType(IUniform::Type t) { m_type = t; }
    IUniform::Type getType() { return m_type; }
    void        setPrecision(IUniform::PrecisionType p) { m_precision = p; }
    PrecisionType getPrecision() { return m_precision; }
    int         getArraySz() { return m_arraySz; }
    void        setArraySz(int sz);
    int         getSizeOfType();
    bool        getDirty(Pass *pass=NULL); ///< get Dirty status for either all targets or one specific, using pass ptr
    void        setDirty(bool bYes, Pass *pass=NULL); ///< set Dirty flag. Either for all targets or one specific
    /// \name methods to assign values to shadowed area of the Uniform
    /// \remark When setting values, we automatically assign a type to the uniform...
    /// all return Uniform* so we can "cascade" calls
    ///@{
    IUniform*    setValue1f(float f);
    IUniform*    setValue2f(float f1, float f2);
    IUniform*    setValue3f(float f1, float f2, float f3);
    IUniform*    setValue4f(float f1, float f2, float f3, float f4);
    IUniform*    setValue1fv(float *pf);
    IUniform*    setValue2fv(float *pf);
    IUniform*    setValue3fv(float *pf);
    IUniform*    setValue4fv(float *pf);
    IUniform*    setValuefv(float *pf, int dim, int nVecs=1, int offsetVec=0);

    IUniform*    setValue1i(int f);
    IUniform*    setValue2i(int f1, int f2);
    IUniform*    setValue3i(int f1, int f2, int f3);
    IUniform*    setValue4i(int f1, int f2, int f3, int f4);
    IUniform*    setValue1iv(int *pf);
    IUniform*    setValue2iv(int *pf);
    IUniform*    setValue3iv(int *pf);
    IUniform*    setValue4iv(int *pf);
    IUniform*    setValueiv(int *pf, int dim, int nVecs=1, int offsetVec=0);

    IUniform*    setValue1b(bool f);
    IUniform*    setValue2b(bool f1, bool f2);
    IUniform*    setValue3b(bool f1, bool f2, bool f3);
    IUniform*    setValue4b(bool f1, bool f2, bool f3, bool f4);
    IUniform*    setValue1bv(bool *pf);
    IUniform*    setValue2bv(bool *pf);
    IUniform*    setValue3bv(bool *pf);
    IUniform*    setValue4bv(bool *pf);
    IUniform*    setValuebv(bool *pf, int dim, int nVecs=1, int offsetVec=0);

    IUniform*    setMatrix4f(float *pm);
    // TODO: add missing cases
    // TODO: add getXXX() ?
    IUniform*    setBuffer(int buffer);
    IUniform*    setBuffer(void* buffer);
    /// \brief assign a sampler-state object to a sampler uniform
    IUniform*    setSamplerState(ISamplerState *pSState);
    /// \brief set the sampler texture Unit. Cg cannot do it... automatically done
    IUniform*    setSamplerUnit(int i);
    /// \brief directly set a GL texture ID to the sampler. \arg texUnit>=0 is like callking setSamplerUnit()
    IUniform*    setSamplerResource(ResourceType resType, int oglTexID, int texUnit=-1, ISamplerState *pSState=NULL);
    /// \brief Associate a nvFX::IResource object to the sampler (OGL)
    IUniform*    setSamplerResource(IResource* pRes, int texUnit=-1, ISamplerState *pSState=NULL);
    /// \brief Associate a nvFX::IResource object to the texture (D3D)
    IUniform*    setTextureResource(IResource* pRes);
    /// \brief set the image texture unit for image Load/Store in shaders
    IUniform*    setImageUnit(int i);
    /// \brief directly set a GL texture ID to the sampler. \arg texUnit>=0 is like callking setSamplerUnit()
    IUniform*    setImageResource(ResourceType resType, int oglTexID, int texUnit);
    /// \brief Associate a nvFX::IResource object to the sampler
    IUniform*    setImageResource(IResource* pRes, int texUnit);
    /// \brief Associate a uniform (texture) to the uniform (sampler)
    IUniform*    setUniform(IUniform* pU);
    /// \brief sets a uniform from the values of another one
    IUniform*    setFromUniform(IUniform* pU);
#ifndef OGLES2
    /// \brief assign a subroutine to a uniform variable
    IUniform*    setSubroutine(const char *funcName, int idx);
    /// \brief assign many subroutine to a uniform variable array
    IUniform*    setSubroutines(const char **funcNames, int numFuncNames);
#endif
    ///@}
    /// \name methods to get values from shadowed area of the IUniform
    ///@{
    bool         getValuefv(float *pf, int dim, int nVecs=1);
    bool         getValueiv(int *pf, int dim, int nVecs=1);
    bool         getValuebv(bool *pf, int dim, int nVecs=1);
    bool         getValueRaw(void *, int szMax=0, int bytesPadding=0);
    ///@}
    /// \brief update targets with the shadowed values
    IUniform*    update(IPass *pass = NULL);
    /// \brief update for a specific target only
    virtual Uniform*    updateForTarget(ShadowedData* pData, int target) = 0;
    /// \name methods to directly assign values to targets without the use of shadowed area
    /// When using these methods, you must be aware that the values won't be persistent
    /// in the unfiform class, as it would when using setXXX().
    ///
    /// all return IUniform* so we can "cascade" calls
    ///@{
    IUniform*    updateValue1f(float f, IPass *pass = NULL);
    IUniform*    updateValue2f(float f1, float f2, IPass *pass = NULL);
    IUniform*    updateValue3f(float f1, float f2, float f3, IPass *pass = NULL);
    IUniform*    updateValue4f(float f1, float f2, float f3, float f4, IPass *pass = NULL);
    IUniform*    updateValue1fv(float *pf, IPass *pass = NULL);
    IUniform*    updateValue2fv(float *pf, IPass *pass = NULL);
    IUniform*    updateValue3fv(float *pf, IPass *pass = NULL);
    IUniform*    updateValue4fv(float *pf, IPass *pass = NULL);
    IUniform*    updateValuefv(float *pf, int dim, IPass *pass = NULL);

    IUniform*    updateValue1i(int f, IPass *pass = NULL);
    IUniform*    updateValue2i(int f1, int f2, IPass *pass = NULL);
    IUniform*    updateValue3i(int f1, int f2, int f3, IPass *pass = NULL);
    IUniform*    updateValue4i(int f1, int f2, int f3, int f4, IPass *pass = NULL);
    IUniform*    updateValue1iv(int *pf, IPass *pass = NULL);
    IUniform*    updateValue2iv(int *pf, IPass *pass = NULL);
    IUniform*    updateValue3iv(int *pf, IPass *pass = NULL);
    IUniform*    updateValue4iv(int *pf, IPass *pass = NULL);
    IUniform*    updateValueiv(int *pf, int dim, IPass *pass = NULL);

    IUniform*    updateValue1b(bool f, IPass *pass);
    IUniform*    updateValue2b(bool f1, bool f2, IPass *pass);
    IUniform*    updateValue3b(bool f1, bool f2, bool f3, IPass *pass);
    IUniform*    updateValue4b(bool f1, bool f2, bool f3, bool f4, IPass *pass);
    IUniform*    updateValue1bv(bool *pf, IPass *pass);
    IUniform*    updateValue2bv(bool *pf, IPass *pass);
    IUniform*    updateValue3bv(bool *pf, IPass *pass);
    IUniform*    updateValue4bv(bool *pf, IPass *pass);
    IUniform*    updateValuebv(bool *pf, int dim, IPass *pass);

    IUniform*    updateMatrix4f(float *pm, IPass *pass = NULL);
    IUniform*    updateSamplerState(ISamplerState *pSState, IPass *pass = NULL);
    IUniform*    updateSamplerUnit(int i, IPass *pass = NULL);
    IUniform*    updateSamplerResource(ResourceType resType, int oglTexID, int texUnit=-1, ISamplerState *pSState=NULL, IPass *pass = NULL);
    IUniform*    updateSamplerResource(IResource* pRes, int texUnit=-1, ISamplerState *pSState=NULL, IPass *pass = NULL);
    /// \name Used for image Load Store
    /// @{
    IUniform*    updateImageUnit(int i, IPass *pass = NULL);
    IUniform*    updateImageResource(ResourceType resType, int oglTexID, int texUnit=-1, IPass *pass = NULL);
    IUniform*    updateImageResource(IResource* pRes, int texUnit=-1, IPass *pass = NULL);
    /// @}
    IUniform*    updateBuffer(int buffer, IPass *pass = NULL);
    IUniform*    updateBuffer(void* buffer, IPass *pass = NULL);
    IUniform*    updateUniform(IUniform* pU, IPass *pass = NULL);
    IUniform*    updateFromUniform(IUniform* pU, IPass *pass = NULL);
#ifndef OGLES2
    IUniform*    updateSubroutine(const char *funcName, int idx, IPass *pass = NULL);
    IUniform*    updateSubroutines(const char **funcNames, IPass *pass = NULL);
    IUniform*    updateSubroutines(int *funcIDs, int numIDs, IPass *pass);
#endif
    // TODO: add missing cases
    ///@}
    /// \brief delete a target pass
    IUniform*    deleteTarget(IPass *pass, int layerID);
    /// \brief invalidates a target pass : in GLSL, the uniform location will be invalidated, for example
    IUniform*    invalidateTarget(IPass *pass, int layerID);
    /// \brief return # of targets available for this uniform
    int         getNumTargets(TargetType t=TANY);
    
    bool        serialize(FILE* fd, bool asCPP);

    void        copyFrom(Uniform* pU);

    friend class Container;
    friend class PassState;
    friend class Pass;
    friend class CstBuffer;
    friend class CstBufferGL;
    friend class UniformRepository;
};
/*************************************************************************/ /**
 ** \brief 
 ** a program pipeline is what allows OpenGL to use separate shader stages
 ** and put them together. It would be fake in D3D.
 ** Note this class is only meant to be used by the Pass Class. So the 'target'
 ** setup in the Program::addTarget is done in Pass (no need to complicate more)
 ** 
 **/ /*************************************************************************/ 
class ProgramPipeline : public IProgramPipeline//Ex
{
protected:
    std::vector</*ShaderProgram*/Program*> m_progShaders;
    int             m_shaderFlags;
    Container*      m_container;
    bool            m_validated;
    virtual bool    validate();
    virtual bool    invalidate(int stages);
    virtual bool    bind();
    virtual void    unbind();
public:
    virtual ~ProgramPipeline();
    ProgramPipeline(Container *pCont);
    virtual /*IShaderProgram*/IProgram* getShaderProgram(int i);
    virtual int         getProgramShaderFlags() { return m_shaderFlags; }

    /*virtual*/ bool        addProgramShader(/*IShaderProgram*/IProgram* pProgShader);
    /*virtual*/ bool        removeProgramShader(IProgram* pProgShader);
    /*virtual*/ IProgram*   removeProgramShader(int stageFlags);

    friend class Pass;
};
#if 0
/*************************************************************************/ /**
 ** \brief the Shader-Program interface
 ** 
 ** Shader-Program is an OpenGL concept : a way to allow separate shader stages
 ** to be mixed. 
 **
 ** See http://www.opengl.org/registry/specs/ARB/separate_shader_objects.txt
 **
 ** This interface is an alternate solution again the more traditional approach
 ** of GLSL where everything is inside a single program. See nvFX::IProgram
 ** \note in D3D, this directy relates to a ID3D10VertexShader or ID3D11VertexShader
 ** 
 **/ /*************************************************************************/ 
class ShaderProgram : public IShaderProgramEx
{
protected:
    typedef std::vector<Shader*> ShaderVec;
    ShaderType  m_type;
    int         m_refCnt;
    bool        m_usable;
    ShaderVec   m_shaders;
    // List of sub-routine indices that we must activate at program activation time
    std::vector<GLuint> m_srIndices;
    Container*          m_container; ///< used to call eprintf, for example
#ifdef SHADERCONCAT_USE
//#pragma MESSAGE("Lame use for GLSL: concatenation of shader pieces of code... iOS issue")
    // iOS seems to not work as ES spec definition. Need a hack
    std::string         m_code;
#endif
    bool        addShader(IShader* pShader);

public:
    ShaderProgram(Container *pCont, ShaderType type, int numShaders, IShader ** pShaders);
    virtual ~ShaderProgram();

    virtual ShaderType  getType() { return m_type; }
    virtual IShader*    getShader(int n) { if(n < (int)m_shaders.size()) return m_shaders[n]; return NULL; }
    virtual int         getNumShaders() { return m_shaders.size(); }

    virtual void        cleanup() = 0;///< \brief (\b Invokes \b GFX \b API) 
    virtual bool        bind() = 0; ///< \brief (\b Invokes \b GFX \b API) 
    virtual void        unbind() = 0; ///< \brief (\b Invokes \b GFX \b API) 
    virtual bool        link() = 0; ///< \brief (\b Invokes \b GFX \b API) 
    virtual bool        linkNeeded(int bYes=-1) = 0;
    virtual int         getProgram() = 0;
    virtual int         getUniformLocation(const char* name) = 0;///< \brief (\b Invokes \b GFX \b API) 
    /// \name Input Attribute specific functions
    /// @{
    /// \brief returns attribute information
    // TODO : using _D3D10_SIGNATURE_PARAMETER_DESC and/or GLSL funcs
    // Create struct AttrInfo...
    //virtual int         getAttribLocation(int i, AttrInfo &info) = 0;
    /// \brief (\b Invokes \b GFX \b API)  returns the locations of a named input attribute, if exists in the shader (vtx shader).
    virtual int         getAttribLocation(const char* attrName) = 0;
    /// \brief (\b Invokes \b GFX \b API) OpenGL specific. No way to change the location of attributes in D3D
    virtual void        bindAttribLocation(int i, const char* attrName) = 0;
    /// @}
    virtual void        setUniform(const char* name, float val) = 0;///< \brief (\b Invokes \b GFX \b API) 
    virtual void        setUniform(const char* name, int val) = 0;///< \brief (\b Invokes \b GFX \b API) 
    virtual void        setUniformVector(const char * name, const float* val, int count, int numUniforms=1) = 0;///< \brief (\b Invokes \b GFX \b API) 
    virtual void        setUniformVector(const char * name, const int* val, int count, int numUniforms=1) = 0;///< \brief (\b Invokes \b GFX \b API) 
    virtual void        setSampler(const char * texname, int texunit) = 0;///< \brief (\b Invokes \b GFX \b API) 
    virtual void        bindTexture(ResourceType target, const char * texname, GLuint texid, int texunit) = 0;///< \brief (\b Invokes \b GFX \b API) 

#ifndef OGLES2
    virtual bool        bindSubRoutineToUniform(int uniformID, char **subroutineNames, int numNames) = 0;///< \brief (\b Invokes \b GFX \b API) 
    virtual int         getSubRoutineID(const char *name) = 0;///< \brief (\b Invokes \b GFX \b API) 
#endif
    /// \name D3D-specific methods
    /// @{
    virtual void* getD3DIASignature() = 0; ///< \brief (\b Invokes \b GFX \b API)  returns the Input attribute signature (in fact : this is the vertex shader compiled Blob)
    virtual int getD3DIASignatureSize() = 0; ///< \brief (\b Invokes \b GFX \b API)  returns the Input attribute signature size (in fact : this is the vertex shader compiled Blob size)
    /// @}
};
#endif

/*************************************************************************/ /**
 ** \brief the class that contains all. Can be seen as the equivalent of a context
 ** 
 **/ /*************************************************************************/ 
class Container : public IContainerEx
{
private:
    Container();
    virtual ~Container();
    typedef std::vector<Technique*>     TechVec;
    typedef std::vector<Shader*>        ShaderVec;
    typedef std::vector<SamplerState*>  SStateVec;
    typedef std::vector<StateGroupRaster*> StateGroupRasterVec;
    typedef std::vector<StateGroupCS*>     StateGroupCSVec;
    typedef std::vector<StateGroupDST*>    StateGroupDSTVec;
    typedef std::vector<StateGroupPath*>    StateGroupPRVec;
    typedef std::map<std::string, Uniform*> UniformMap;
    typedef std::vector<CstBuffer*>         CstBufferVec;
    typedef std::vector</*IShaderProgram*/IProgram*>     ShaderProgramVec; // NOTE: we could use std::map with a special key and compare function... yeah
    typedef std::vector<Resource*>         ResourceVec;
    typedef std::vector<FrameBufferObject*> FboVec;
    //----------------------- DATA ----------------
    // TODO: resources names should contain name-space when sorted/stored or checked!!
    TechVec             m_techniques;
    ShaderVec           m_shaders;
    ShaderProgramVec    m_shaderprograms;
    SStateVec           m_samplerStates;
    CstBufferVec        m_cstBuffers; ///< array of constant buffers
    UniformMap          m_uniforms; ///< map of uniforms out of any cst buffer
    StateGroupRasterVec m_stateGroupRasters;
    StateGroupCSVec     m_stateGroupCSs;
    StateGroupDSTVec    m_stateGroupDSTs;
    StateGroupPRVec     m_stateGroupPRs;
    ResourceVec         m_resources;
    FboVec              m_fbos;
    std::string         m_name;
    Annotation          m_annotations;
    bool                m_separateShaders;
    //----------------------------------------------
    struct shaderset {
        int vtx;
        int tcs;
        int eshd;
        int geom;
        int frag;
    };
    // Used to store common programs according to the combination of shaders for each stage
    /*struct shadercompareLess
    {
        bool operator()(const shaderset s1, const shaderset s2) const
        {
            if(s1.vtx < s2.vtx) return true;
            else if(s1.vtx > s2.vtx) return false;
            if(s1.vtx < s2.vtx) return true;
            else if(s1.vtx > s2.vtx) return false;
            if(s1.tcs< s2.tcs) return true;
            else if(s1.tcs > s2.tcs) return false;
            if(s1.eshd < s2.eshd) return true;
            else if(s1.eshd > s2.eshd) return false;
            if(s1.geom< s2.geom) return true;
            else if(s1.geom > s2.geom) return false;
            if(s1.frag< s2.frag) return true;
            else if(s1.frag > s2.frag) return false;
        };
    };
    std::map<shaderset, Program*, shadercompareLess> m_programs;*/

    //int                 updateUniforms(Technique *tech, bool bCreateIfNeeded, bool bOnlyShadowed); ///< walk through uniforms and update them for a technique only
    /// \name Cst buffer updating (OGL / D3D)
    /// @{
    bool                updateCstBuffers(Pass *pass, int layerID, bool bCreateIfNeeded, bool bCreateBufferIfNeeded);
    void                deleteCstBuffersTarget(Pass *pass, int layerID); ///< removes any reference to pass
    void                invalidateCstBuffersTarget(Pass *pass, int layerID); ///< removes any reference to pass
    /// @}
    /// \name Uniforms updating (essentially for OGL)
    /// @{
    bool                updateUniforms(Pass *pass, int layerID, bool bCreateIfNeeded, bool bOnlyShadowed);
    void                deleteUniformsTarget(Pass *pass, int layerID); ///< removes any reference to pass
    void                invalidateUniformsTarget(Pass *pass, int layerID); ///< removes any reference to pass
    /// @}
    /// \name SamplerState updating (essentially for D3D)
    /// @{
    bool                updateSamplerStates(Pass *pass, int layerID, bool bCreateIfNeeded, bool bOnlyShadowed);
    void                deleteSamplerStatesTarget(Pass *pass, int layerID); ///< removes any reference to pass
    void                invalidateSamplerStatesTarget(Pass *pass, int layerID); ///< removes any reference to pass
    bool                validateSamplerStates(); ///< \brief validate SamplerStates (create the D3D objects)
    /// @}
    /// \name Texture updating (essentially for D3D, where texture are really part of shaders)
    /// @{
    //bool                updateTextures(Pass *pass, int layerID, bool bCreateIfNeeded, bool bOnlyShadowed);
    //void                deleteTexturesTarget(Pass *pass, int layerID); ///< removes any reference to pass
    //void                invalidateTexturesTarget(Pass *pass, int layerID); ///< removes any reference to pass
    /// @}
    void                invalidateTargets(Pass *pass, int layerID); ///< calls all the sub-sequence invalidate calls...
    int                 gatherShadersFromAnnotation(IShader** &pp, const char *annotationName, const char* value, int *ival, float *fval);
    int                 gatherResourcesFromAnnotation(IResource** &pp, const char *annotationName, const char* value, int *ival, float *fval);
    int                 gatherFbosFromAnnotation(IFrameBufferObject** &pp, const char *annotationName, const char* value, int *ival, float *fval);
    int                 gatherUniformsFromAnnotation(IUniform**&pp, const char *annotationName, const char* value, int *ival, float *fval);
    int                 gatherCstBuffersFromAnnotation(ICstBuffer**&pp, const char *annotationName, const char* value, int *ival, float *fval);
public:
    virtual IAnnotation *annotations() { return &m_annotations; }
    /// \brief get container name
    virtual const char*     getName() { return m_name.c_str(); }
    /// \brief set the container name : trims suffix if any
    virtual void            setName(const char*name) { m_name = name; int pos = (int)m_name.rfind("."); if(pos >= 0) m_name = m_name.substr(0, pos); }
    /// \name creation methods
    /// @{
    static IContainer*  create(const char *name = NULL);
    ITechnique*         createTechnique(const char *name = NULL);
    IShader*            createGLSLShader(const char *name = NULL, bool bGlobal=false);
    bool                addGLSLShader(IShader* p);
    IShader*            createHLSL10Shader(const char *name = NULL, bool bGlobal=false);
    bool                addHLSL10Shader(IShader* p);
    IShader*            createHLSL11Shader(const char *name = NULL, bool bGlobal=false);
    bool                addHLSL11Shader(IShader* p);
    IShader*            createCUDAShader(const char *name=NULL, bool bGlobal=false);
    bool                addCUDAShader(IShader* p);
    IShader*            createPathShader(const char *name = NULL, bool bGlobal=false, bool bPostscript=false);
    bool                addPathShader(IShader* p);
    IProgram/*IShaderProgram*/*     createShaderProgram(ShaderType type, int numShaders, IShader ** pShaders);
    void                        consolidateShaders();
    //virtual DXComputeModule*  createDXComputeModule(const char *name = NULL) = 0;
    //virtual CUDAModule*       createCUDAModule(const char *name = NULL) = 0;
    //virtual OptixModule*      createOptixModule(const char *name = NULL) = 0;
    //virtual CGShader*         createCGShader(const char *name = NULL) = 0;
    /// \brief appends the common shader parts (unnamed shaders) to the named shaders
    //void                consolidateShaders();
    ISamplerState*       createSamplerState(const char *name = NULL);
    IStateGroupRaster*   createStateGroup_Raster(const char *name = NULL);
    IStateGroupCS*       createStateGroup_CS(const char *name = NULL);
    IStateGroupDST*      createStateGroup_DST(const char *name = NULL);
#ifndef OGLES2
    IStateGroupPath*     createStateGroup_PR(const char *name = NULL);
#endif
    IUniform*            createUniform(const char *name, const char *groupname=NULL, const char *semantic=NULL, bool bGlobal=true);
    bool                 addUniform(IUniform *p);
    ICstBuffer*          createCstBuffer(const char *name, bool bGlobal=true);
    bool                 addCstBuffer(ICstBuffer* p);
    int                  createUniformsFromPass(IPass *pass); ///< create the Uniform classes related to a Pass
    /// @}
    /// \name destruction methods
    /// @{
    bool    destroy();
    bool    destroy(ITechnique* p);
    bool    destroy(IShader* p);
    //bool    destroy(ICUDAModule* p);
    //bool    destroy(IOptixModule* p);
    bool    destroy(ISamplerState* p);
    bool    destroy(IStateGroupRaster* p);
    bool    destroy(IStateGroupCS* p);
    bool    destroy(IStateGroupDST* p);
#ifndef OGLES2
    bool    destroy(IStateGroupPath* p);
#endif
    bool    destroy(IUniform *p);
    bool    destroy(IProgram* p); ///< \brief called by the nvFX::IProgram::release()
    /// @}
    /// \name Resource Specific
    /// @{
    bool    addResource(IResource *pRes);
    bool    addFbo(IFrameBufferObject *pFbo);
    IResource*          createResource(const char * name, ResourceType type, bool bGlobal);
    IFrameBufferObject* createFBO(const char * name, bool bGlobal);
    IResource*          findResource(const char * name);
    IResource*          findResource(int i);
    int              gatherResourcesFromAnnotation(IResource** &ppResources, const char *annotationName, const char* value=NULL);
    int              gatherResourcesFromAnnotation(IResource** &ppResources, const char *annotationName, int value);
    int              gatherResourcesFromAnnotation(IResource** &ppResources, const char *annotationName, float value);
    int              gatherResourcesFromNamespace(IResource** &ppResources, const char *ns);
    IFrameBufferObject* findFbo(const char * name);
    int              gatherFbosFromAnnotation(IFrameBufferObject** &ppFbos, const char *annotationName, const char* value=NULL);
    int              gatherFbosFromAnnotation(IFrameBufferObject** &ppFbos, const char *annotationName, int value);
    int              gatherFbosFromAnnotation(IFrameBufferObject** &ppFbos, const char *annotationName, float value);
    int              gatherFbosFromNamespace(IFrameBufferObject** &ppFbos, const char *ns);
    /// @}
    /// \name Shader specific
    /// @{
    IShader*         findShader(const char * name, TargetType t=TANY);
    IShader*         findShader(int i);
    int              gatherShadersFromAnnotation(IShader** &ppShaders, const char *annotationName, const char* value=NULL);
    int              gatherShadersFromAnnotation(IShader** &ppShaders, const char *annotationName, int value);
    int              gatherShadersFromAnnotation(IShader** &ppShaders, const char *annotationName, float value);
    int              gatherShadersFromNamespace(IShader** &ppShaders, const char *ns);
    /// @}
    /// \name find methods
    /// @{
    /// \brief finds any sort of things
    int                  getNumTechniques() { return (int)m_techniques.size(); }
    ITechnique*          findTechnique(const char * name);
    ITechnique*          findTechnique(int i);
    ISamplerState*       findSamplerState(const char * name);
    ISamplerState*       findSamplerState(int i);
    IStateGroupRaster*   findStateGroupRaster(const char * name);
    IStateGroupRaster*   findStateGroupRaster(int i);
    IStateGroupCS*       findStateGroupCS(const char * name);
    IStateGroupCS*       findStateGroupCS(int i);
    IStateGroupDST*      findStateGroupDST(const char * name);
    IStateGroupDST*      findStateGroupDST(int i);
#ifndef OGLES2
    IStateGroupPath*     findStateGroupPR(const char * name);
    IStateGroupPath*     findStateGroupPR(int i);
#endif
    IUniform*            findUniform(const char * name);
    IUniform*            findUniform(int i);
    int                  gatherUniformsFromAnnotation(IUniform**&pp, const char *annotationName, const char* value=NULL);
    int                  gatherUniformsFromAnnotation(IUniform**&pp, const char *annotationName, int value);
    int                  gatherUniformsFromAnnotation(IUniform**&pp, const char *annotationName, float value);
    int                  gatherUniformsFromNamespace(IUniform**&pp, const char *ns);
    ICstBuffer*          findCstBuffer(const char * name);
    ICstBuffer*          findCstBuffer(int i);
    int                  gatherCstBuffersFromAnnotation(ICstBuffer**&pp, const char *annotationName, const char* value=NULL);
    int                  gatherCstBuffersFromAnnotation(ICstBuffer**&pp, const char *annotationName, int value);
    int                  gatherCstBuffersFromAnnotation(ICstBuffer**&pp, const char *annotationName, float value);
    int                  gatherCstBuffersFromNamespace(ICstBuffer**&pp, const char *ns);
    /// @}
    /// \brief special method to move all Container's uniforms to the Cst buffer. For D3D case
    void                 moveGlobalUniformsToBuffer(CstBuffer* pBuff);
    /// \name Code generation
    /// @{
    virtual bool     serializeToCpp(const char* fname);
    virtual bool     serializeToNvFX(const char* fname);
    bool     serialize(FILE* fd, bool asCPP);
    /// @}
    /// \name general settings for misc behavior
    /// @{
    /// separate shaders is optional in GLSL... and could even fail on old HW/drivers
    virtual void     separateShadersEnable(bool bYes);
    virtual bool     separateShadersEnabled() { return m_separateShaders; }
    /// @}

    friend class Pass;
    friend class Technique;
    friend class Shader;
    friend class Uniform;
    friend class PassState;
    friend class Program;
    friend class IContainer;
    friend class IContainerEx;
};

/*************************************************************************/ /**
 ** \brief class used to drive a technique created by nvFX::Container
 ** 
 **/ /*************************************************************************/ 
 class Technique : public ITechniqueEx
{
private:
    virtual ~Technique();
    Technique(Container *pCont, const char *name=NULL);
    struct PassTable
    {
        PassTable(Pass* pass_, Technique* pOwner_, bool bActive_) : pass(pass_), pOwner(pOwner_), bActive(bActive_) {}
        Pass*       pass;    // the pass itself
        Technique*  pOwner;  // the owner : sub-technique would lead to a specific ptr different from 'this'
        bool        bActive; // the state of this state
    };
    typedef std::vector<PassTable> PassVec;
    AttribMap           m_attributes; // map where we store the attributes for possible vertex shader in this pass
    PassVec             m_passes;
    std::string         m_name;
    Container*          m_container; ///< referenced to be able to access to nvFX::Uniform
    Annotation          m_annotations;
public:
    virtual IAnnotation *annotations() { return &m_annotations; }
    int             getNumPasses() { return (int)m_passes.size(); }
    bool            isPassActive(int i) { if(i>=(int)m_passes.size())return false; return m_passes[i].bActive; };
    bool            isPassActive(IPass* p);
    bool            setPassActive(IPass* p, bool bYes);
    IPass*          getPass(int i, PassInfo *pPassInfo=NULL);
    const char*     getName() { return m_name.c_str(); }
    void            setName(const char *name) { m_name = name; }
    /// \brief the preferred way to create a pass : the tehnique owns it
    IPass*          addPass(const char *name = NULL);
    /// \brief used if Pass was allocated from outside. Warning: a pass must be only assigned to ONE technique
    //void            addPass(Pass *);
    bool            destroy(IPass *p);

    bool            addSubTechnique(ITechnique *pTech, bool bActive=true);
    void            setSubTechniqueActive(ITechnique *pTech, bool bActive=true);
    bool            getSubTechniqueActive(ITechnique *pTech);

    bool            validate();
    bool            invalidate();
    bool            validateResources();
    bool            invalidateResources();

    /// \brief allows to map the attributes of a vertex shader to *all* the sub-sequence passes
    bool            bindAttribute(const char * name, int unit);
    /// \brief binds mapped attributes to the program (triggers a linkage) to *all* the sub-sequence passes
    bool            bindAttributes();
    /// \brief erase all mapped attributes of *all* the sub-sequence passes
    void            clearAttributeBinding();
    /// \brief returns the string name of a specific attribute #. NULL if nothing assigned
    const char *    getAttributeNameForLocation(int loc);
    /// \brief returns # of associated attributes
    int             getNumBoundAttributes();
    
    void            setActiveProgramLayer(int id, bool createIfNA=false);
    int             getActiveProgramLayer();
    void            removeProgramLayer(int id);

    friend          class Container;
};

/*************************************************************************/ /**
 ** 
 ** Note: the pass states can accept the states defined for
 ** StateGroupRaster, StateGroupCS and StateGroupDST
 **/ /*************************************************************************/ 
class PassState : public IPassStateEx
{
private:
    virtual ~PassState();
    PassState(Container * pCont, Pass* pParent, const char *name=NULL); // Container essentially needed for messaging/debug purpose
    PassState(Container * pCont, Pass* pParent, PassState* pState);
    PassState::Type m_type;
    union {
        // Section for StateGroups
        StateGroupRaster*  m_pSGR;
        StateGroupCS*      m_pSGCS;
        StateGroupDST*     m_pSGDST;
        StateGroupPath*    m_pSGPR;
        struct { // the idea: a local uniform will be used to copy data to the global one
            Uniform *           m_uniformSrc;///< a pointer to a dedicated uniform instance (with its dedicated values) for this pass-state
            Uniform *           m_uniformDst;///< a pointer to the global uniform
        };
        RenderingMode       m_rendermode; ///< rendermode specified by a pass. The app must respond to this
        int                 m_rendergroup; ///< an arbitrary value that would be set from a pass
        int                 m_viewport[4];//one for each MAX_RENDERTARGETS ?? ///< viewport dimensions + depth min/max
        float               m_viewportZ[2];//one for each MAX_RENDERTARGETS ?? ///< viewport dimensions + depth min/max
        ClearMode           m_clearMode;
        FrameBufferObject* m_FBOTarget; ///< NULL is the backbuffer. >0 vals is ref. to a FBO
        FrameBufferObject* m_BlitFBOSrc; ///< if !=NULL there will be a blit
        FrameBufferObject* m_FBO[2]; ///< 2 FBOs for the Resource swap operation
        struct {
            float           m_clearColor[4/*MAX_RENDERTARGETS*/];
            int             m_colorTarget;
        };
        int                 m_gridSz[3]; // used for CUDA and GLSL/DX Compute
#ifdef USECUDA
        int                 m_blockSz[3];
        int                 m_sharedMemSz;
        struct {
            int             m_kernelEntryId;
            int             m_numArgs;
            ArgVal          *m_argvalues;
        };
#endif
    };
    typedef std::vector<Shader*> ShaderVec;
    ShaderVec               m_shaders;
    std::string             m_name; ///< a Pass-state can be names, optionally
    Container*              m_container;
    Pass*                   m_parent;///< parent owner
public:
    const char *getName() { return m_name.c_str(); }
    Type        getType()               { return m_type; };
#ifndef OGLES2
    IUniform*    getUniform() {return (m_type == TUniform)||(m_type == TSubRoutineUniform) ? m_uniformSrc : NULL; };
#else
    IUniform*    getUniform() {return (m_type == TUniform) ? m_uniformSrc : NULL; };
#endif
    IStateGroupRaster*  getStateGroupRaster();
    IStateGroupCS*      getStateGroupCS();
    IStateGroupDST*     getStateGroupDST();
#ifndef OGLES2
    IStateGroupPath*    getStateGroupPathRendering();
#endif
    IFrameBufferObject* getFBOTarget();
    IFrameBufferObject* getBlitFBOToActiveTarget();

    IShader*     getShader(int idx)     { if(idx<(int)m_shaders.size()) return static_cast<IShader*>(m_shaders[idx]); return NULL; };
    bool        clearShaders();
    bool        setShaders(Type type, IShader **progs, int numProgs);
    // todo : add for others (Geom etc.)
    void        setStateGroup(IStateGroupRaster *pSGR);
    void        setStateGroup(IStateGroupCS *pSGCS);
    void        setStateGroup(IStateGroupDST *pSGDST);
#ifndef OGLES2
    void        setStateGroup(IStateGroupPath *pSGPR);
#endif
    /// \brief set a state that will assign a resource to a texture (D3D)
    bool        setTextureResource(const char *varName, IResource *pRes);
    /// \brief set a state that will assign a resource to a sampler (OGL)
    bool        setSamplerResource(const char *varName, IResource *pRes, int texUnit=-1, ISamplerState *pSState=NULL);
    /// \brief set a sampler-state to a sampler
    bool        setSamplerState(const char *varName, ISamplerState* pSState);
    //bool        setTextureResource(const char *varName, IResource *pRes);
    /// \brief set a resource to an image for Load/Store
    bool        setImageResource(const char *varName, IResource *pRes, int texUnit);
#ifndef OGLES2
    /// \brief set a string to a sub-routine uniform variable
    /// \arg idx used if the sub-routine uniform is an array
    /// \warning idx not working, yet
    bool        setUniform(const char *srcVarName, int idx, const char *dstVarName);
    /// \brief set strings to a sub-routine uniform array
    /// \warning idx not working, yet
    bool        setUniform(const char *srcVarName, const char **dstVarNames, int numNames);
#endif
    /// \brief set a float or vector float to a uniform variable
    /// \warning idx not working, yet
    bool        setUniform(const char *varName, int idx, float *pF, int nComps, int nVecs=1);
    /// \brief set integer or vector integer to a uniform variable
    /// \warning idx not working, yet
    bool        setUniform(const char *varName, int idx, int *pF, int nComps, int nVecs=1);
    // TODO: add more
    /// \name scene-level states
    /// @{
    bool        setRenderingMode(RenderingMode rmode);
    bool        setRenderGroup(int g);
    bool        setClearMode(ClearMode cmode);
    bool        setClearColor(int i, float r, float g, float b, float a);
    bool        setRenderTarget(IFrameBufferObject *p);
    bool        setBlitFBOToActiveTarget(IFrameBufferObject *p);
    bool        setViewport(int x, int y, int w, int h, float minZ, float maxZ);
    bool        setSwapResources(IFrameBufferObject *p1, IFrameBufferObject *p2);
    /// @}
    /// \name Compute specific (GLSL/DX)
    /// @{
    bool        setStateComputeGroup(int x, int y, int z=1);
    /// @}
    /// \name CUDA specific
    /// @{
    bool        setStateKernelEntry(const char * kernelName, int numArgs, ArgVal *argvalues);
    bool        setStateBlockSz(int x, int y, int z);
    bool        setStateGridSz(int x, int y, int z);
    bool        setStateSharedMemory(int Sz);
    /// @}
    void        apply(bool bUniforms, PassInfo   * pr);

    friend class Pass;
};

/*************************************************************************/ /**
 ** \brief class for the pass of an effect container created by nvFX::Technique
 ** 
 **/ /*************************************************************************/ 
class Pass : public IPassEx
{
protected:
    typedef std::vector<PassState*> StateVec;
    struct UniformAndTarget { UniformAndTarget(Uniform* pU_, int target_) : pU(pU_), target(target_) {} Uniform* pU; int target; };
    typedef std::vector< UniformAndTarget >   UniformVec;
    struct CstBufferAndTarget { CstBufferAndTarget(CstBuffer* pC_, int target_) : pC(pC_), target(target_) {} CstBuffer* pC; int target; };
    typedef std::vector< CstBufferAndTarget >   CstBufferVec;
    struct SamplerStateAndTarget { SamplerStateAndTarget(SamplerState* pS_, int target_) : pS(pS_), target(target_) {} SamplerState* pS; int target; };
    typedef std::vector< SamplerStateAndTarget >   SamplerStateVec;
    struct StatesLayer
    {
        StatesLayer() { programPipeline = NULL; program = NULL; 
        memset(textureUnits, 0, sizeof(Uniform*)*32);
#ifdef USECUDA
        programCUDA=NULL; 
        kernelEntry=-1;
#endif
        }
        StateVec            statesForValidation; ///< states that need only be read during validation
        StateVec            statesForExecution; ///< states that need to be executed during nvFX::Pass::execute()
        ProgramPipeline*    programPipeline; ///< using http://www.opengl.org/registry/specs/ARB/separate_shader_objects.txt (D3D has it natively)
        Program*            program; ///< typical use of the OpenGL Program way (D3D doesn, so we would fake it... not so good)
#ifdef USECUDA
        Program*            programCUDA; ///< a CUDA program == CUDA Module. It is separated from the rest because parallel to GL/D3D
        int                 kernelEntry; ///< a CUDA kernel entry : references a structure where we have the info for the right kernel to use
#endif
        UniformVec          usedUniforms;   ///< \brief the uniforms that are being used by the programs. Needed to update them when executing a pass
        CstBufferVec        usedCstBuffers; ///< \brief the cst buffers that are being used by the programs. Needed to update them when executing a pass
        SamplerStateVec     usedSamplerStates; ///< \brief the sampler states that are being used by the programs. Needed to update them when executing a pass
        Uniform*            textureUnits[32]; ///< \brief which texture unit is allocated to which Uniform
    };
    /// shader states are sorted according to some "layerID" : various implementations of shader programs from different combinations of pieces of shader code
    /// Note that the ID #0 is the default one. It essentially contains the default states : render-states and base shaders.
    /// All the other ID > 0 are the additional ones, which must contain only additional shaders or changes on shaders
    typedef std::map<int, StatesLayer> StatesLayerMap;

    // pass-states :
    StatesLayerMap          m_statesLayers;
    StateVec                m_statesOverride; ///< \brief states for overriding sub-effects in a scene

    StatesLayer*            m_pActiveStatesLayer;
    StatesLayer*            m_pBaseStatesLayer;
    int                     m_activeStatesLayer;
    int                     m_overrideID; ///< \brief id that is created when the pass needs sub-effects to behave specifically when under this ID
// Note: in theory, the Attribs mapping could change if we changed the input of the vertex shader for a specific programPipelineID
// TODO...
    AttribMap               m_attributes; // map where we store the attributes for possible vertex shader in this pass
    std::string             m_name;
    #define VALIDATED_PASS      1
    #define VALIDATED_RESOURCES 2
    unsigned int            m_validated;
    int                     m_loopCnt;
    Container*              m_container; ///< just referenced here to be able to access to nvFX::Uniform
    Technique*              m_parent; ///<parent owner
    // GLSL Compute Group size
    int                     m_GLSLComputeGroupSize[3]; ///< Grid/Group size if ever GLSL Compute used in this pass
    /// \name programs can either be using regular OpenGL GLSL program or ARB separate shader object.
    /// however we cannot mix both... could be tricky
    /// @{
    /// a pass implicitly has a program setup if glsl is involved
    /// more than one program, gathered into a program pipeline. See http://www.opengl.org/registry/specs/ARB/separate_shader_objects.txt
    /// @}
    Annotation              m_annotations;

    bool            updateUniform(Uniform *uniform);
    void            clearRenderTargets(PassInfo   *pr);
    void            setViewPort(int *viewport, float *viewportZ=NULL); ///<ptr to a 4 components int array
    /// Helper for nvFX::execute
    void            drawFullscreenQuad(float size = 2.0f, float offsetx = -1.0f, float offsety = -1.0f);
    void            addUniform(Uniform *pU, int target); ///< allows to add a uniform and the specific target to the pass that will need it to be updated
    void            addCstBuffer(CstBuffer *pC, int target); ///< allows to add a cst buffer and the specific target to the pass that will need it to be updated
    void            addSamplerState(SamplerState *pS, int target); ///< allows to add a sampler state and the specific target to the pass that will need it to be updated
    void            addResource(Resource *pRes, int target); ///< allows to add a resource and the specific target to the pass that will need it to be updated
    IProgram*       createShaderProgram(std::map<std::string, PassState*> &passStateShader, ShaderType type, ProgramPipeline* &programPipeline, int layerID);
    /// check the texture units of the used-uniforms dedicated to texture sampler
    bool            validateUniformTextureUnits(bool allowUnitAssignment);
    /// check the texture units of the used-uniforms dedicated to texture sampler
    bool            validateUniformTextureUnit(Uniform* pU, bool allowUnitAssignment, int layerID=-1);
    void            delete_ProgramPipeline(ProgramPipeline* pp, int layerID);
public:
    virtual ~Pass();
    /// \brief public constructor. Needs the parent container (to find back some uniforms)
    Pass(Container *pCont, Technique *pParent, const char *name = NULL);
    virtual IAnnotation *annotations() { return &m_annotations; }
    /// \brief returns the pass name
    const char*     getName() { return m_name.c_str(); }
    /// \brief sets the pass name
    void            setName(const char *name) { m_name = name; }
    /// \name pass-state methods
    /// @{
    /// \brief create a state to assign a sampler-state to a sampler
    virtual IPassState*      createStateSamplerState(const char *varName, ISamplerState * pSState);
    /// \brief Create a IPass state-override shader "type". only one shader can be passed
    virtual IPassState*      createStateOverride(IPassState::Type type, IShader *shd, const char *groupName);
    /// \brief Create a IPass state shader-override "type" from a list of shaders
    /// if the pass-state already exists, the existing one will be returned.
    virtual IPassState*      createStateOverride(IPassState::Type type, IShader **shd, int numShaders, const char *groupName);
    virtual IPassState*      createStateSamplerResourceOverride(const char *varName, IResource *pRes, int texUnit);
    virtual IPassState*      createStateTextureResourceOverride(const char *varName, IResource *pRes);
    virtual IPassState*      createStateImageResourceOverride(const char *varName, IResource *pRes, int unit);
    virtual IPassState*      createStateSamplerStateOverride(const char *varName, ISamplerState * pSState);
    /// \brief Creates ON THE ACTIVE STATE LAYER a copy of the original state
    IPassState*      createStateFromState(IPassState* pState);
    /// \brief Create a Pass state shader "type". only one shader can be passed
    IPassState*      createState(IPassState::Type type, IShader *shd, const char *groupName, bool bUseActiveLayer);
    /// \brief Create a Pass state shader "type" from a list of shaders
    IPassState*      createState(IPassState::Type type, IShader **shd, int numShaders, const char *groupName, bool bUseActiveLayer);
#ifndef OGLES2
    /// \brief Typically used to associate subroutine implementations
    IPassState*      createState(const char *srcVarName, int idx, const char *dstVarName);
    /// \brief Typically used to associate subroutine implementations to an array of uniform routines
    IPassState*      createState(const char *srcVarName, const char **dstVarNames, int numNames);
#endif
    /// \brief used to assign a float or a vector to a uniform variable
    IPassState*      createState(const char *varName, int idx, float *pF, int nComps);
    /// \brief used to assign an int or a int-vector to a uniform variable
    IPassState*      createState(const char *varName, int idx, int *pI, int nComps);
    /// \brief used to assign a float or a vector array to a uniform variable
    IPassState*      createState(const char *varName, int idx, float *pF, int nComps, int nVecs);
    /// \brief used to assign an int or a int-vector array to a uniform variable
    IPassState*      createState(const char *varName, int idx, int *pI, int nComps, int nVecs);
    /// \brief assigns to the pass a state group for rasterization
    IPassState*      createState(IStateGroupRaster *pSGRaster);
    /// \brief assigns to the pass a state group for Color samples
    IPassState*      createState(IStateGroupCS *pSGCS);
    /// \brief assigns to the pass a state group for Depth-Stencil
    IPassState*      createState(IStateGroupDST *pSGDST);
#ifndef OGLES2
    /// \brief assigns to the pass a state group for Path rendering
    IPassState*      createState(IStateGroupPath *pSGPR);
#endif
    /// \brief create a state that assigns a resource to a sampler (OGL)
    IPassState*      createStateSamplerResource(const char *varName, IResource *pRes, int texUnit=-1);
    /// \brief create a state that assigns a resource to a texture (D3D)
    IPassState*      createStateTextureResource(const char *varName, IResource *pRes);
    /// \brief create a state that assigns a resource to an image Load/Store
    IPassState*      createStateImageResource(const char *varName, IResource *pRes, int unit=-1);
    /// \brief finds a state by its group name. Typically used for groups of shaders
    IPassState*      findState(const char *groupName, int layerID);
    IPassState*      findStateOverride(const char *groupName);
    /// \brief destroys a pass-state
    bool            destroy(IPassState* p);
    /// @}
    /// \name additional states for scene-level effects
    /// @{
    IPassState*      createStateRenderMode(RenderingMode rmode);
    IPassState*      createStateRenderGroup(int renderGroup);
    IPassState*      createStateClearMode(ClearMode cmode);
    IPassState*      createStateClearColor(int i, float r, float g, float b, float a);
    IPassState*      createStateRenderTarget(IFrameBufferObject *p);
    IPassState*      createStateBlitFBOToActiveTarget(IFrameBufferObject *p);
    IPassState*      createStateViewport(int x, int y, int w, int h, float minZ, float maxZ);
    IPassState*      createStateSwapResources(IFrameBufferObject *p1, IFrameBufferObject *p2);
    /// @}
    /// \name for Compute setup (GLSL/DX)
    /// @{
    virtual IPassState* createStateComputeGroup(int x, int y, int z=1);
    /// @}
    /// \name for CUDA setup
    /// @{
    virtual IPassState* createStateKernelEntry(const char * kernelName, int numArgs, ArgVal *argvalues);
    virtual IPassState* createStateBlockSz(int x, int y, int z=1);
    virtual IPassState* createStateGridSz(int x, int y, int z=1);
    virtual IPassState* createStateSharedMemory(int Sz);
    /// @}
    /// \name Programs/shaders
    /// @{
    /// \brief  returns the program associated with this pass, if it exists
    IProgram*    getProgram(int layerID);
    /// \brief  returns the program-pipeline associated with this pass, if it exists. 
    /// Note that nvFX::IProgramPipeline is the alternate solution against nvFX::IProgram, 
    /// so getProgram() may return NULL while getProgramPipeline() may return a pointer. And vice versa.
    IProgramPipeline* getProgramPipeline(int layerID);
    /// \brief disable the program that is used in this pass
    void            unbindProgram();
    /// @}
    /// \brief validates the pass. It will create the GLSL/Cg (and others) programs if needed
    bool            validate(); ///< called by Technique::validate()
    /// \brief validates the related resources needed by this pass.
    bool            validateResources();
    /// \brief creates in \e dest techniques passes additional pass states from \e override states
    bool            setupOverrides(ITechnique **dest, int numPasses);
    /// \brief creates in \e dest passes additional pass states from \e override states
    bool            setupOverrides(IPass **dest, int numPasses);
    /// \brief removes a specific override
    bool            releaseOverrides(ITechnique **dest, int numTechs);
    /// \brief removes a specific override
    bool            releaseOverrides(IPass **dest, int numPasses);
    /// \brief validates the pass. It will create the GLSL/Cg (and others) programs if needed
    bool            invalidate();
    /// \brief invalidates the related resources needed by this pass.
    bool            invalidateResources();
    /// \brief executes the states of this pass : [GLSL] program and other states (render states)...
    /// \arg \e cancelInternalAction allows you to cancel some implicit actions that execute() would perform. See nvFX::PassInfo  ::flags
    bool            execute(PassInfo   * pr=NULL, unsigned int cancelInternalAction=0);

    bool            setActiveProgramLayer(int id, bool createIfNA=false);
    int             getActiveProgramLayer();
    int             getLayerId(int i);
    void            removeProgramLayer(int id);
    /// \brief allows to map the attributes of a vertex shader
    bool            bindAttribute(const char * name, int unit);
    /// \brief internal version : Technique may call it
    bool            bindAttributes(AttribMap &attributes);
    /// \brief binds mapped attributes to the program (triggers a linkage)
    bool            bindAttributes();
    /// \brief erase all mapped attributes
    void            clearAttributeBinding();
    /// \brief tells if the pass got validated or not
    bool            isValidated() { return m_validated&VALIDATED_PASS; };
    /// \brief returns the string name of a specific attribute #. NULL if nothing assigned
    const char *    getAttributeNameForLocation(int loc);
    /// \brief returns # of associated attributes
    int             getNumBoundAttributes();
    /// \brief tells how many times this pass should be repeated (Loop)
    void            setLoopCount(int cnt);
    int             getLoopCount();
    /// \brief tells if this pass is active or not
    void            setActive(bool bYes=true);
    bool            getActive();
    /// \name D3D-specific methods
    /// @{
    void* getD3DIASignature(); ///< returns the Input attribute signature (in fact : this is the vertex shader compiled Blob)
    int getD3DIASignatureSize(); ///< returns the Input attribute signature size (in fact : this is the vertex shader compiled Blob size)
    /// @}

    //friend class PassState;
    friend class Technique;
    friend class UniformGLSL;
    friend class UniformD3D;
    friend class CstBufferGL;
    friend class CstBufferD3D;
    friend class SamplerStateGL;
    friend class SamplerStateD3D;
    friend class Container;
    friend class PassState;
    friend class StateGroupPathGL;
    friend class UniformCUDA;
};

/*************************************************************************/ /**
 ** \brief class to control the sampler-states created from nvFX::Container
 ** 
 **/ /*************************************************************************/ 
class SamplerState : public ISamplerStateEx
{
protected:
    /// \brief contains the target information : how to plug things into the target
    struct STarget {
        TargetType  ttype;
        bool        valid;
        bool        dirty;
        IPass*       pass;
        int         passLayerId;
        int         shaderProgramNumber;
        //union {
            int         index; // D3D offset
        //};
    };
    // TODO: write the code when some targets need to be removed
    std::vector<STarget>    m_targets;
	int						m_activeTarget; // keep track of the last target used (inside a Pass::execute)

    void*   m_data; ///< data that will be passed to the target. It could be a ID3D1XSamplerState ptr...
    Container*      m_container; ///< we might need the container : to get back device interface...

    StringName      m_name;

    typedef std::vector<State*> StateVec;
    StateVec        m_states;

    Annotation              m_annotations;

    /// \brief update targets with the shadowed values
    virtual SamplerState*  update(void *data, Pass *pass, int layerID, bool bCreateIfNeeded) = 0;
    virtual SamplerState*  updateForTarget(void *data, int target = false) = 0;
public:
    virtual ~SamplerState();
    SamplerState(Container* pCont, const char* name = NULL);
    virtual IAnnotation *annotations() { return &m_annotations; }
    const char*     getName() { return m_name.c_str(); }
    void            setName(const char *name) { m_name = name; }
    //enum Type
    //{
    //    GLTEXTUREMINFILTER,
    //    GLTEXTUREMAGFILTER,
    //    GLTEXTUREWRAPS,
    //    GLTEXTUREWRAPT,
    //    GLTEXTUREWRAPR,
    //};
    State* createState(GLenum sstate, GLenum value); ///< \brief easy use for OpenGL GLenums...
    /// \name D3D-specific
    ///@{
    virtual bool        setD3DSamplerState(void *p) { return false; } ///< \brief instead of creating states inside this object, we can directly assign an existing sampler-state from the API
    virtual void*       getD3DSamplerState() { return m_data; }
    /// \validates the sampler-state object. Essentially needed to setup something on Gfx API side (D3D, for example... not OpenGL)
    virtual bool       validate()  { return true; }
    ///@}
    /// \name OpenGL-specific
    ///@{
    virtual void        updateGLTexture(GLenum target, GLint tex, bool bindTexture=false); ///< invoke OpenGL calls that will setup sampler states in the OpenGL texture
    ///@}
    /// \name target management
    ///@{
    /// \brief delete a target pass
    virtual ISamplerState*    deleteTarget(IPass *pass, int layerID);
    /// \brief invalidates a target pass : in GLSL, the uniform location will be invalidated, for example
    virtual ISamplerState*    invalidateTarget(IPass *pass, int layerID);
    /// \brief return # of targets available for this uniform
    virtual int         getNumTargets(TargetType t=TANY);
    /// @}
    friend          class Container;
    friend          class Pass;
};
/*************************************************************************/ /**
 ** \brief
 **
 **/ /*************************************************************************/ 
class Resource : public IResourceEx
{
protected:
    /// \brief the data required to create a Texture/renderbuffer from this object. Typically used when nvFX effect creates a render-target/buffer
    struct ResourceCreationData {
        ResourceCreationData() : 
            fmt(FMT_UNKNOWN), 
            resolveTarget(NULL), appDepMSAA(false), appDepSz(false) { sz[0]=sz[1]=sz[2]=1; msaa[0]=msaa[1]=0; szFact[0]=szFact[1]=1.0f; bExternal=false;}
        int             sz[3];
        ResourceFormat  fmt;
        int             msaa[2];
        float           szFact[2];
        Resource*       resolveTarget;
        bool            appDepMSAA;
        bool            appDepSz;
        bool            bExternal; ///< true if the resource is externally managed (typical texture from an application, for example)
    };
    ResourceCreationData m_creationData; ///< contains the data for resource creation
    StringName      m_name;
    unsigned int    m_OGLId; ///< storage for OpenGL case... TODO: move this one to the sub-class for OGL
    //void*           m_d3dresource; // Essentially for DX (we may merge this with m_OGLTex as a generic void* for both...)
    ResourceType    m_type;
    SamplerState*   m_samplerState; ///< we can attach a sampler state to a texture (a la OpenGL)
    Annotation      m_annotations;
    ResourceRepository* m_pOwner;
    Container*      m_pContainer;
    int             m_userCnt; ///< stores the amoung of users for this resource. \Remark users are the one really using as an active resource
public:
    Resource(ResourceRepository* pCont, const char *name=NULL);
    virtual ~Resource();
    virtual bool        isValidated() { return m_OGLId/*||m_d3dresource*/ ? true:false; }
    /// \brief returns the interface for annotations
    virtual IAnnotation *annotations() { return &m_annotations; }
    virtual const char* getName() { return m_name.c_str(); }
    /// \brief sets the pass name
    virtual void        setName(const char *name) { m_name = name; }

    virtual ResourceType getType() { return m_type; }
    virtual void        setType(ResourceType t) { m_type = t; }
    virtual IContainer* getContainerCreator() { return m_pContainer; }
    // TODO: when overriding m_OGLId, we should make sure to release possible allocated resources from before.
    virtual void        setGLTexture(int id) { m_OGLId = id; m_creationData.bExternal = true; };
    virtual void        setGLTexture(ResourceType type, int id) { m_type = type; m_OGLId = id; m_creationData.bExternal = true; };
    virtual int         getGLTextureID() { return m_OGLId; }
    virtual void        setSamplerState(ISamplerState * pSState) { m_samplerState = static_cast<SamplerState*>(pSState); }
    virtual ISamplerState*   getSamplerState()                       { return m_samplerState; }

    /// \name resource creation method : when the effect creates some render-targets, as a typical example
    /// \todo methods to set/get the associated texture from texture repository
    /// @{
    virtual void        setDimensions(int w, int h, int d=1);///< \brief resource Size
    virtual void        setDimensionsApplicationDependent(bool bYes=true);///< \brief resource Size will depends on the app
    virtual bool        getDimensionsApplicationDependent() { return m_creationData.appDepSz; }
    virtual void        setMSAA(int d, int s=0);///< \brief MSAA (+CSAA)
    virtual void        getMSAA(int &d, int &s) { d = m_creationData.msaa[0]; s = m_creationData.msaa[1]; }
    virtual void        setMSAAApplicationDependent(bool bYes=true);///< \brief MSAA will depends on the app
    virtual bool        getMSAAApplicationDependent() { return m_creationData.appDepMSAA; }
    virtual void        setDimensionFactor(float w,float h);///< \brief if we need to
    virtual void        getDimensionFactor(float &w,float &h) { w = m_creationData.szFact[0]; h = m_creationData.szFact[1]; }
    /// \brief this points to a Texture that is supposed to be the target when resolve happens. This is way to \e shortcut the 'resolve'
    /// process when possible : when no MSAA involved, the RenderBuffer could be replaced by this texture directly...
    virtual void        setResolveTarget(IResource* p);
    virtual IResource*  getResolveTarget() { return m_creationData.resolveTarget; }
    virtual bool        setFormat(ResourceFormat f); ///< \brief tells which format we want for the creation of this resource
    virtual ResourceFormat getFormat() { return m_creationData.fmt; }
    /// @}

    virtual int         getWidth() { return (int)((float)m_creationData.sz[0] * m_creationData.szFact[0]);}
    virtual int         getHeight() {return (int)((float)m_creationData.sz[1] * m_creationData.szFact[1]);}

    virtual bool            unmapFromCUDA() { return false; } ///< for CUDA only

    /// \name reference counter of users.
    /// @{
    virtual int         incUserCnt() { 
        m_userCnt++;
        LOGD("Incremented Resource %s use-count to %d\n", m_name.c_str(), m_userCnt);
        return m_userCnt;
    }
    virtual int         decUserCnt() { 
        if(m_userCnt > 0) m_userCnt--; 
        LOGD("Deccremented Resource %s use-count to %d\n", m_name.c_str(), m_userCnt);
        return m_userCnt; }
    virtual int         getUserCnt() { return m_userCnt; }
    /// @}
#if 0
    Resource(ResourceRepository *owner)
    {
        sizeFactor[0] = 1.0;
        sizeFactor[1] = 1.0;
        pOwner = owner;
        refCnt = 0;
        texID = 0;
        rbID = 0;
        format = 0;
        w = h = d = 0;
        byteSize = 0;
        effectRTID = -1;
        depthSamples = 1;
        coverageSamples = 0;
        target = 0;
        bIsSizeAppDependent = false;
        bIsAAAppDependent = false;
        bDirty = false;
        resolveTextureTarget = NULL;
    }
    int				refCnt;
    int				effectRTID; // this ID is the one reference from the outside - no link with OpenGL
    GLuint			texID;	// if this is a texture
    GLuint			rbID;	// if this is a renderbuffer
    GLenum			format;
    GLenum			StupidOGLExternalFmt; // shame on OpenGL... needed when creating a render target texture!!!!
    GLenum			target; // GL_TEXTURE_2D etc.
    int				w,h, d;
    float			sizeFactor[2]; // size factor (w,h) = sizeFactor * windowSize (for example)
    size_t			byteSize;

    // hint of what will happen : the renderbuffer will be used to be then resolved to this texture.
    // if non-NULL and non-MSAA mode, we will try to use resolveTextureTarget and not allocate for the renderbuffer.
    Resource*		resolveTextureTarget;

    int				depthSamples;		//Only valid for renderbuffers
    int				coverageSamples;	//Only valid for renderbuffers

    // true if the size/AA of this texture depends on the window(s) of the app
    bool			bIsSizeAppDependent;
    bool			bIsAAAppDependent; 
    bool			bDirty; // used when we changed the texture/renderbuffer id or when we changed the reference to resolveTextureTarget. So the FBO can be updated

#ifdef USECUDA
    //CUgraphicsResource cudaResource; /// we put here the info on resource so that we can update it when the resource gets resized
#endif

    std::string		name;

    ResourceRepository*	pOwner;
#endif
    friend class Container;
    friend class ResourceRepository;
    friend class FrameBufferObject;
    friend class FrameBufferObjectsRepository;
    friend class IFrameBufferObjectsRepository;
    friend class IFrameBufferObjectsRepositoryEx;

};
/*************************************************************************/ /**
 ** \brief class for Rasterization states
 **
 ** This class is based on 
 **
 ** \todo use this extension whenever possible...
 **/ /*************************************************************************/ 
class StateGroupRaster : public IStateGroupRasterEx
{
protected:
    StringName      m_name;
    typedef std::vector<State*> StateVec;
    StateVec        m_states;
    Container*      m_container;
    Annotation      m_annotations;
public:
    virtual ~StateGroupRaster();
    StateGroupRaster(Container* pCont, const char* name = NULL);
    virtual IAnnotation *annotations() { return &m_annotations; }
    const char*     getName() { return m_name.c_str(); }
    void            setName(const char *name) { m_name = name; }
    /**
    create states for the following:
    \verbatim
                                         Dynamic      Max
       Property                          Allowed     Index      Default Value
    -----------------------------       ---------    -----      -------------
    GL_POINT_SMOOTH                        no          1           FALSE
    GL_POINT_SPRITE                        no          1           FALSE
    GL_POINT_SPRITE_COORD_ORIGIN           no          1           UPPER_LEFT
    GL_LINE_SMOOTH                         no          1           FALSE
    GL_LINE_STIPPLE                        no          1           FALSE
    GL_CULL_FACE                           no          1           FALSE
    GL_CULL_FACE_MODE                      no          1           BACK
    GL_FRONT_FACE                          no          1           CCW
    GL_POLYGON_SMOOTH                      no          1           FALSE
    GL_POLYGON_OFFSET_POINT                no          1           FALSE
    GL_POLYGON_OFFSET_LINE                 no          1           FALSE
    GL_POLYGON_OFFSET_FILL                 no          1           FALSE
    GL_POLYGON_STIPPLE                     no          1           FALSE
    GL_RASTERIZER_DISCARD                  no          1           FALSE
    GL_COMBINED_LINE_STIPPLE_NV            yes         1           0x00FFFF00
    GL_LINE_STIPPLE_PATTERN                no          1           0xFFFF
    GL_LINE_STIPPLE_REPEAT                 no          1           1
    \endverbatim
    **/
    State*          createState(GLenum sstate, GLenum value);
    /**
    create states for the following:
    \verbatim
                                         Dynamic      Max
       Property                          Allowed     Index      Default Value
    -----------------------------       ---------    -----      -------------
    GL_POLYGON_MODE                        no          2           FILL
    \endverbatim
    **/
    State*          createState(GLenum sstate, GLenum value1, GLenum value2);
    /**
    create states for the following:
    \verbatim
                                         Dynamic      Max
       Property                          Allowed     Index      Default Value
    -----------------------------       ---------    -----      -------------
    GL_POINT_SIZE                          yes         1           1.0
    GL_LINE_WIDTH                          yes         1           1.0
    GL_POLYGON_OFFSET_FACTOR               yes         1           0
    GL_POLYGON_OFFSET_UNITS                yes         1           0
    \endverbatim
    **/
    State*          createState(GLenum sstate, float value);
    State*          findState(GLenum state);
    State*          getState(int i);
    //void            apply(); will be implemented in derived ones
    bool            validate();
};

/*************************************************************************/ /**
 ** \brief class for Color samples states
 ** 
 **/ /*************************************************************************/ 
 class StateGroupCS : public IStateGroupCSEx
{
protected:
    StringName      m_name;

    typedef std::vector<State*> StateVec;
    StateVec        m_states;
    Container*      m_container;
    Annotation      m_annotations;
public:
    virtual ~StateGroupCS();
    StateGroupCS(Container* pCont, const char* name = NULL);
    virtual IAnnotation *annotations() { return &m_annotations; }
    const char*     getName() { return m_name.c_str(); }
    void            setName(const char *name) { m_name = name; }
    /**
    create states for the following:
    \verbatim
                                         Dynamic      Max
       Property                          Allowed     Index      Default Value
    -----------------------------       ---------    -----      -------------
    GL_ALPHA_TEST                          no          1           FALSE   
    GL_ALPHA_TEST_FUNC                     no          1           ALWAYS
    GL_BLEND_SRC                           no         [1]          ONE
    GL_BLEND_SRC_RGB                       no         [1]          ONE
    GL_BLEND_SRC_ALPHA                     no         [1]          ONE
    GL_BLEND_DST                           no         [1]          ZERO
    GL_BLEND_DST_RGB                       no         [1]          ZERO
    GL_BLEND_DST_ALPHA                     no         [1]          ZERO
    GL_BLEND_EQUATION_RGBA_NV              no         [1]          FUNC_ADD
    GL_BLEND_EQUATION_RGB                  no         [1]          FUNC_ADD
    GL_BLEND_EQUATION_ALPHA                no         [1]          FUNC_ADD  
    GL_PER_DRAW_BUFFER_BLEND_FUNCTION_NV   no          1           FALSE
    GL_PER_DRAW_BUFFER_BLEND_EQUATION_NV   no          1           FALSE
    GL_PER_DRAW_BUFFER_BLEND_ENABLE_NV     no          1           FALSE
    GL_PER_DRAW_BUFFER_COLOR_WRITEMASK_NV  no          1           FALSE
    GL_FRAMEBUFFER_SRGB                    no          1           FALSE
    GL_DITHER                              no          1           TRUE
    GL_COLOR_LOGIC_OP                      no          1           FALSE
    GL_LOGIC_OP_MODE                       no          1           COPY
    GL_MULTISAMPLE                         no          1           TRUE
    GL_SAMPLE_MASK                         no          1           FALSE
    GL_SAMPLE_ALPHA_TO_ONE                 no          1           FALSE
    GL_SAMPLE_ALPHA_TO_COVERAGE            no          1           FALSE
    GL_BLEND                               no         [1]          0
    GL_SAMPLE_MASK_VALUE                   yes        [2]          0xFFFFFFFF
    \endverbatim
    **/
    State*          createState(GLenum sstate, GLenum value);
    /**
    create states for the following:
    \verbatim
                                         Dynamic      Max
       Property                          Allowed     Index      Default Value
    -----------------------------       ---------    -----      -------------
    GL_COLOR_WRITEMASK                     no         [1]          0xF
    \endverbatim
    **/
    State*          createState(GLenum state, GLboolean x, GLboolean y, GLboolean z, GLboolean w);
    /**
    create states for the following:
    \verbatim
                                         Dynamic      Max
       Property                          Allowed     Index      Default Value
    -----------------------------       ---------    -----      -------------
    GL_BLEND_COLOR                         yes         4           (0,0,0,0) 
    \endverbatim
    **/
    State*          createState(GLenum state, GLclampf x, GLclampf y, GLclampf z, GLclampf w);
    /**
    create states for the following:
    \verbatim
                                         Dynamic      Max
       Property                          Allowed     Index      Default Value
    -----------------------------       ---------    -----      -------------
    GL_ALPHA_TEST_REF                      yes         1           0
    \endverbatim
    **/
    State*          createState(GLenum state, GLclampf value);
    State*          findState(GLenum state);
    State*          getState(int i);
    //void            apply(); will be implemented in derived ones
    bool            validate();
};

/*************************************************************************/ /**
 ** \brief class for Depth Stencil States
 ** 
 **/ /*************************************************************************/ 
 class StateGroupDST : public IStateGroupDSTEx
{
protected:
    StringName      m_name;
    typedef std::vector<State*> StateVec; // Shall we make it std::map<GLenum> instead ?
    StateVec        m_states;
    Container*      m_container;
    Annotation      m_annotations;
public:
    virtual ~StateGroupDST();
    StateGroupDST(Container* pCont, const char* name = NULL);
    virtual IAnnotation *annotations() { return &m_annotations; }
    const char*     getName() { return m_name.c_str(); }
    void            setName(const char *name) { m_name = name; }
    /**
    create states for the following:
    \verbatim
                                         Dynamic      Max
       Property                          Allowed     Index      Default Value
    -----------------------------       ---------    -----      -------------
    GL_DEPTH_TEST                          no          1           FALSE
    GL_DEPTH_FUNC                          no          1           LESS
    GL_DEPTH_WRITEMASK                     no          1           TRUE
    GL_DEPTH_CLAMP                         no          1           FALSE
    GL_DEPTH_BOUNDS_TEST_EXT               no          1           FALSE
    // DEPTH_BOUNDS_EXT[0] = MIN, DEPTH_BOUNDS_EXT[1] = MAX
    GL_DEPTH_BOUNDS_EXT                    yes         2           (0.0, 1.0)
    GL_STENCIL_TEST                        no          1           FALSE
    GL_STENCIL_FUNC                        no          1           ALWAYS
    GL_STENCIL_FAIL                        no          1           KEEP
    GL_STENCIL_PASS_DEPTH_FAIL             no          1           KEEP
    GL_STENCIL_PASS_DEPTH_PASS             no          1           KEEP
    GL_STENCIL_FRONT_FUNC                  no          1           ALWAYS
    GL_STENCIL_FRONT_FAIL                  no          1           KEEP
    GL_STENCIL_FRONT_PASS_DEPTH_FAIL       no          1           KEEP
    GL_STENCIL_FRONT_PASS_DEPTH_PASS       no          1           KEEP
    GL_STENCIL_BACK_FUNC                   no          1           ALWAYS
    GL_STENCIL_BACK_FAIL                   no          1           KEEP
    GL_STENCIL_BACK_PASS_DEPTH_FAIL        no          1           KEEP
    GL_STENCIL_BACK_PASS_DEPTH_PASS        no          1           KEEP
    GL_STENCIL_REF                         yes         1           0
    GL_STENCIL_WRITEMASK                   yes         1           0xFFFFFFFF
    GL_STENCIL_VALUE_MASK                  yes         1           0xFFFFFFFF
    GL_STENCIL_FRONT_VALUE_MASK            yes         1           0xFFFFFFFF
    GL_STENCIL_FRONT_WRITEMASK             yes         1           0xFFFFFFFF
    GL_STENCIL_BACK_VALUE_MASK             yes         1           0xFFFFFFFF
    GL_STENCIL_FRONT_REF                   yes         1           0
    GL_STENCIL_BACK_WRITEMASK              yes         1           0xFFFFFFFF
    GL_STENCIL_BACK_REF                    yes         1           0
    \endverbatim
    **/
    State*          createState(GLenum state, GLenum value);
    /**
    create states for the following:
    \verbatim
                                      Dynamic      Max
    Property                          Allowed     Index      Default Value
    --------------------------       ---------    -----      -------------
    DEPTH_BOUNDS_EXT                    yes         2           (0.0, 1.0)
    \endverbatim
    **/
    State*          createState(GLenum state, float val1, float val2);
    State*          findState(GLenum state);
    State*          getState(int i);
    //void            apply(); will be implemented in derived ones
    bool            validate();
};

#ifndef OGLES2
/*************************************************************************/ /**
 ** \brief interface for NV Path-rendering states. OpenGL exclusive...
 ** although state groups don't exist in the driver, it is cleaner to do so
 **/ /*************************************************************************/ 
class StateGroupPath : public IStateGroupPathEx
{
protected:
    StringName      m_name;
    typedef std::vector<State*> StateVec; // Shall we make it std::map<GLenum> instead ?
    StateVec        m_states;
    Container*      m_container;
    Annotation      m_annotations;
public:
    StateGroupPath(Container* pCont, const char* name = NULL);
    virtual         ~StateGroupPath();
    virtual IAnnotation *annotations() { return &m_annotations; }
    const char*     getName()                   { return m_name.c_str(); }
    void            setName(const char *name)   { m_name = name; }
    State*          findState(GLenum state);
    State*          createState(GLenum state, GLenum value);
    State*          createState(GLenum state, float value);
    State*          createState(GLenum state, int val);
    bool            validate(); ///< \brief before applying, validation tells if any issue and also create the object (D3D or OGL extension for state groups)
};
#endif
/*************************************************************************/ /**
 ** \brief Frame buffer Object binding point (FBO in OpenGL), gathering many render targets : textures or buffers
 **
 **
 **/ /*************************************************************************/ 
class FrameBufferObject : public IFrameBufferObjectEx
{
protected:
    typedef std::vector<Resource*> ResourceVec;
    int             m_w,m_h;		///< \brief size needed for blit operation, for example
    StringName      m_name;
    Annotation      m_annotations;
    Resource*       m_dst;
    ResourceVec     m_colors;
    GLuint          m_fboID; ///< \brief essentially for OpenGL. But we keep it here
    FrameBufferObjectsRepository*  m_pOwner;///< \brief the pool where it originally belongs
    /// \brief only friends can create it
    FrameBufferObject(FrameBufferObjectsRepository* pOwner, const char *name=NULL) :
        m_w(0), m_h(0), m_dst(NULL), m_fboID(0), m_pOwner(pOwner) { if(name) m_name=name; }
    virtual ~FrameBufferObject();
public:
    /// \brief returns the interface for annotations
    IAnnotation*    annotations() { return &m_annotations; }
    const char*     getName() { return m_name.c_str(); }
    GLuint          getID() { return m_fboID; }
    /// \brief sets the pass name
    void            setName(const char *name) { m_name = name; }
    /// \name resource assignment
    /// @{
    void            setDSTResource(IResource * pDST);
    void            setColorResource(IResource ** pp, int n);
    IResource*      getDSTResource() { return m_dst; }
    IResource*      getColorResource(int n) { return n >= (int)m_colors.size() ? NULL : m_colors[n]; }
    /// @}
    /// \name methods to be implemented in the specialized API
    /// @{
    bool            setCurrent(bool bAdjustViewport);
    bool            blitTo(IFrameBufferObject* pDst);
    bool            validate();
    bool            invalidate();
    /// @}
    int             getWidth() { return m_w; }
    int             getHeight(){ return m_h; }
    int             getNumRenderTargets() { return (int)m_colors.size(); }

    friend class Container;
    friend class FrameBufferObjectsRepository;
    friend class IFrameBufferObjectsRepository;
    friend class IFrameBufferObjectsRepositoryEx;
};

//
/// Helper to convert shader program types
//
inline ShaderType asShaderType(IPassState::Type t)
{
    switch(t)
    {
    case IPassState::TVertexShader:
        return FX_VTXPROG;
    case IPassState::TFragmentShader:
        return FX_FRAGPROG;
    case IPassState::TGeometryShader:
        return FX_GEOMPROG;
    case IPassState::TTCSShader:
        return FX_TCSPROG;
    case IPassState::TTESShader:
        return FX_TESPROG;
    case IPassState::TComputeShader:
        return FX_COMPUTEPROG;
    case IPassState::TPathShader:
        return FX_PATHPROG;
    case IPassState::TCUDAModule:
        return FX_CUDAPROG;
    default:
        break;
    }
    return FX_UNKNOWNSHADER;
}

inline int asShaderFlag(IPassState::Type type)
{
    switch(type)
    {
    case IPassState::TVertexShader:
        return FX_VERTEX_SHADER_BIT;
    case IPassState::TFragmentShader:
        return FX_FRAGMENT_SHADER_BIT;
    case IPassState::TGeometryShader:
        return FX_GEOMETRY_SHADER_BIT;
    case IPassState::TTCSShader:
        return FX_TESS_CONTROL_SHADER_BIT;
    case IPassState::TTESShader:
        return FX_TESS_EVALUATION_SHADER_BIT;
    case IPassState::TComputeShader:
        return FX_COMPUTE_SHADER_BIT;
    case IPassState::TPathShader:
        return FX_PATH_CODE_BIT;
    case IPassState::TCUDAModule:
        break;
    }
    return 0;
}
inline int asShaderFlag(ShaderType type)
{
    switch(type)
    {
    case FX_VTXPROG:
        return FX_VERTEX_SHADER_BIT;
    case FX_FRAGPROG:
        return FX_FRAGMENT_SHADER_BIT;
    case FX_GEOMPROG:
        return FX_GEOMETRY_SHADER_BIT;
    case FX_TCSPROG:
        return FX_TESS_CONTROL_SHADER_BIT;
    case FX_TESPROG:
        return FX_TESS_EVALUATION_SHADER_BIT;
    case FX_COMPUTEPROG:
        return FX_COMPUTE_SHADER_BIT;
    case FX_PATHPROG:
        return FX_PATH_CODE_BIT;
    case FX_CUDAPROG:
        break;
    }
    return 0;
}

}//namespace nvFX

#endif
