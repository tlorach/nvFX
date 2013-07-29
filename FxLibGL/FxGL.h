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

    $Id: //sw/devrel/Playpen/tristan/GitHub/nvFX/FxLibGL/FxGL.h#7 $
    $Date: 2013/06/20 $
    $Revision: #7 $
    $Author: tlorach $
    $Change: 16301413 $
*/
#ifndef __FXGLSL_H__
#define __FXGLSL_H__
#include <vector>
#include <map>
#include <string>

//#define USEMAPRANGE
#define USEBUFFERSUBDATA

//#undef NVFXCHECKGLERRORS
#ifdef NVFXCHECKGLERRORS
#define CHECKGLERRORS(s) checkGLError(s)
#define PRINT_GL_ERROR(s)  checkGLError(s)
#else
#define CHECKGLERRORS(s) 0
#define PRINT_GL_ERROR(s) 0
#endif

#ifdef USECUDA
#include "FxCUDA.h"
#else
#include "FxLibEx.h"
#endif
#include "OpenGL.h"
#include "Fx.h"

namespace nvFX
{
    class Container;
    class Technique;
    class Pass;
    class GLSLShader;
    class GLSLProgram;
    class SamplerState;
    class Uniform;
    class PassState;
    class CstBuffer;
    class FrameBufferObjectGL;
    class Shader;
    class Texture;

    extern GLenum drawBuffers[];

    extern int ResourceFormatByteSize(ResourceFormat f);
    extern int ResourceFormatByteSize(int f);
    extern GLenum ResourceFormatAsGL(ResourceFormat f);
    extern const char * ResourceFormatAsString(ResourceFormat f);
    extern GLenum getGLAccess(AccessMode a);

    /*************************************************************************/ /**
     ** 
     ** 
     **/ /*************************************************************************/ 
    class CstBufferGL : 
#ifdef USECUDA
        public CstBufferCUDA
#else
        public CstBuffer
#endif
    {
    public:
        CstBufferGL(const char* name);
        ~CstBufferGL();

        // TODO: change this approach : update() should be done in CstBuffer
        // target creation should be done outside of update
        virtual CstBuffer*  update(Pass *pass, int layerID, bool bBindProgram, bool bCreateIfNeeded, bool bCreateBufferIfNeeded);
        virtual CstBuffer*  update2(Pass *pass, bool bBindProgram, bool bCreateIfNeeded, bool bCreateBufferIfNeeded);
        virtual CstBuffer*  updateForTarget(STarget &t, bool bBindProgram);
        virtual int         buildGLBuffer(BufferUsageGL usage, int sizeMultiplier);

		virtual int         bufferSizeAndData(char *pData, int *sz=NULL);
        int                 bufferSizeAndData(char *pData, Uniform* pU, int &szBytes, int &pk);

        void                updateGLSL(STarget &t, bool bBindProgram);


        virtual ICstBuffer* setGLBuffer(int buffer);
        virtual bool        updateFromUniforms(bool bForceUpdating=false);
        virtual bool        mapBuffer(void** ppData, bool bForceUpdating);
        virtual void        unmapBuffer();
        virtual bool        update();
    };
    /*************************************************************************/ /**
    ** \brief Uniform Parameter class
    ** This implementation is suffix'ed GLSL but it doesn't mean this is only for GLSL :
    ** it means that we are in the OpenGL-nvFX implementation. remember that D3D and OpenGL
    ** are mutually exclusive. However, the Compute part is not : D3D and OpenGL implementations
    ** must implement compute parts : OpenGL and D3D must support CUDA; D3D must support DXCompute,
    ** OpenGL will support GLSL-Compute. Those compute part will partly reside into the OpenGL/D3D libraries.
    **/ /*************************************************************************/ 
    class UniformGLSL : 
#ifdef USECUDA
        // in the case of CUDA, we inherit from a class that add additional stuff for CUDA work
        public UniformCUDA
#else
        public Uniform
#endif
    {
    public:
        virtual ~UniformGLSL();
        UniformGLSL(const char* name = NULL, const char* groupname = NULL, const char* semantic = NULL);
        virtual Uniform*    update(ShadowedData* pData, Pass *pass, int layerID, bool bBindProgram, bool bCreateIfNeeded);
        virtual Uniform*    update2(ShadowedData* pData, Pass *pass, bool bBindProgram, bool bCreateIfNeeded);
        void                updateGLSL(ShadowedData* pData, STarget &t, bool bBindProgram);
        /// \brief update for a specific target only
        virtual Uniform*    updateForTarget(ShadowedData* pData, STarget &t, bool bBindProgram);
    };

    /*************************************************************************/ /**
     ** \brief common shader abstract class
     **
     ** Cg, GLSL and others would derive from it
     **/ /*************************************************************************/ 
    /*class Shader : public IShader
    {
    protected:
        std::string m_name;
        std::string m_shaderCode; ///< this string will hold shader code until we finally compile it
        Container*  m_container; ///< just referenced here to be able to access cgContext :-(
    public:
        Shader(const char *name, Container* pCont) : IShader(name, pCont) { if(name) m_name = std::string(name); m_container = pCont; }
        const char* getName()               { return m_name.c_str(); }
        void    setName(const char *name)   { m_name = std::string(name); }
        const char* getShaderCode()         { return m_shaderCode.c_str(); }

        virtual void    cleanupShader() = 0;
        virtual void    cleanupSourceCode() = 0;
        virtual bool    loadShaderCodeFromFile(const char *fname) = 0;
        virtual bool    loadShaderCode(const char *code) = 0;
        /// \arg \b type can be 0 to 4 (FX_VTXPROG FX_GEOMPROG FX_FRAGPROG FX_TCSPROG FX_TESPROG)
        virtual bool    isCompiled(int type) = 0;
        virtual IShader* asGLSLShader() { return NULL; }
        virtual bool    compileShader(ShaderType type, IContainer *pContainer) = 0;
        virtual bool    addHeaderCode(const char *code, int startLineNumber) = 0;

        friend Container;
        friend Pass;
    };*/
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
    class GLSLShader : public Shader
    {
    public:
        GLSLShader(const char *name = NULL, IContainer* pCont = NULL);
        ~GLSLShader();

        void    cleanupShader();
        /// \arg \b type can be 0 to 4 (FX_VTXPROG FX_GEOMPROG FX_FRAGPROG FX_TCSPROG FX_TESPROG)
        bool    isCompiled(int type);
        bool    compileShader(ShaderType type, IContainer *pContainer);

        GLhandleARB getGLSLShaderObj(GLenum type);

        //IShader* asGLSLShader() { return this; }

        friend class GLSLProgram;
        friend class GLSLShaderProgram;
        friend class Container;
        friend class Pass;
    };
    /*************************************************************************/ /**
     ** 
     ** 
     **/ /*************************************************************************/ 
    class GLSLProgram : public Program
    {
    private:
        //typedef std::vector<GLSLShader*> ShaderVec;
        typedef std::map<std::string, GLSLShader*> ShaderMap;
        bool        m_usable;
        //bool        m_bound; // turns out to be useless and risky
        bool        m_linkNeeded;
        bool        m_separable;
        int         m_shaderFlags; /// place where we store GL_VERTEX_SHADER_BIT etc.
        GLhandleARB m_program;
        //ShaderVec   m_commonShaders; ///< list of common source code for all shaders (== include)
        ShaderMap   m_computeShaders; ///< list of attached compute shaders, sorted by the name
        ShaderMap   m_vtxShaders; ///< list of attached vertex shaders, sorted by the name
        ShaderMap   m_geomShaders;
        ShaderMap   m_tcsShaders;
        ShaderMap   m_tesShaders;
        ShaderMap   m_fragShaders; ///< list of attached fragment shaders, sorted by the name
        //TODO: Geom, tess...
        // List of sub-routine indices that we must activate at program activation time
        std::vector<GLuint> m_srIndicesVS;
        std::vector<GLuint> m_srIndicesFS;
        std::vector<GLuint> m_srIndicesGS;
        std::vector<GLuint> m_srIndicesTCS;
        std::vector<GLuint> m_srIndicesTES;
        std::vector<GLuint> m_srIndicesCS;
#ifdef SHADERCONCAT_USE
//#pragma MESSAGE("Lame use for GLSL: concatenation of shader pieces of code... iOS issue")
        // iOS seems to not work as ES spec definition. Need a hack
        std::string         m_VPCode;
        std::string         m_FPCode;
        GLhandleARB         m_vtxShader;
        GLhandleARB         m_fragShader;
#endif
    public:
        GLSLProgram(Container *pCont);
        ~GLSLProgram();

        virtual bool        addShader(ShaderType type, IShader* pShader, IContainer* pContainer);
        virtual bool        addFragmentShader(IShader* pShader, IContainer* pContainer);
        virtual bool        addVertexShader(IShader* pShader, IContainer* pContainer);
        //TODO: geom, tess...
        virtual IShader*    getShader(int n, ShaderType *t = NULL);
        virtual IShader*    getShader(IShader *pShader, ShaderType *t = NULL);
        virtual IShader*    getShader(const char *name, ShaderType *t = NULL);
        virtual int         getNumShaders();

        virtual void        cleanup();
        virtual bool        bind(IContainer* pContainer);
        virtual void        unbind(IContainer* pContainer);
        virtual bool        link(IContainer* pContainer);
        virtual bool        linkNeeded(int bYes) { if(bYes >= 0) m_linkNeeded = bYes?true:false; return m_linkNeeded; }
        virtual int         getProgram() {return m_program;}
        virtual int         getProgramShaderFlags() { return m_shaderFlags; }
        virtual int         getUniformLocation(const char* name);

        virtual int         getAttribLocation(const char* attrName);
        virtual void        bindAttribLocation(int i, const char* attrName);

        virtual void        setUniform(const char* name, float val);
        virtual void        setUniform(const char* name, int val);
        virtual void        setUniformVector(const char * name, const float* val, int count, int numUniforms=1);
        virtual void        setUniformVector(const char * name, const int* val, int count, int numUniforms=1);
        virtual void        setSampler(const char * texname, int texunit);
        virtual void        bindTexture(ResourceType target, const char * texname, GLuint texid, int texunit);

#ifndef OGLES2
        virtual bool        bindSubRoutineToUniform(int uniformID, GLenum shadertype, char **subroutineNames, int numNames);
        virtual int         getSubRoutineID(const char *name, GLenum shadertype);
#endif
        /// \name D3D-specific methods
        /// @{
        virtual void*       getD3DIASignature() { return NULL; }
        virtual int         getD3DIASignatureSize() { return 0; }
        /// @}
        /// \name Compute-specific method(s)
        /// @{
        virtual bool execute(int szx, int szy, int szz);
        virtual bool execute(RenderingMode mode, const PassInfo::PathInfo *p) { return false; }
        /// @}

        //friend      class Container;
        //friend      class Pass;
    };

    /*************************************************************************/ /**
     ** \brief 
     ** 
     ** 
     **/ /*************************************************************************/ 
    class GLSLProgramPipeline : public ProgramPipeline
    {
    protected:
        GLuint  m_pipelineID;
        virtual bool    validate();
        virtual bool    invalidate(int stages);
        virtual bool    bind();
        virtual void    unbind();
    public:
        GLSLProgramPipeline(Container *pCont);
        ~GLSLProgramPipeline();
    };
    /*************************************************************************/ /**
     ** \brief class to control the sampler-states created from nvFX::Container
     ** 
     **/ /*************************************************************************/ 
    class SamplerStateGL : public SamplerState
    {
    public:
        ~SamplerStateGL();
        SamplerStateGL(Container *pCont, const char* name = NULL);
        void            updateGLTexture(GLenum target, GLint tex, bool bindTexture);
        SamplerState*   update(void *data, Pass *pass, int layerID, bool bBindProgram, bool bCreateIfNeeded);
        SamplerState*   updateForTarget(void *data, STarget &t, bool bBindProgram = false);
    };

    /*************************************************************************/ /**
     ** 
     ** 
     **/ /*************************************************************************/ 
    class StateGroupRasterGL : public StateGroupRaster
    {
    public:
        StateGroupRasterGL(Container *pCont, const char* name) : StateGroupRaster(pCont, name) {}
        virtual void apply();
        virtual bool validate();
    };

    class StateGroupCSGL : public StateGroupCS
    {
    public:
        StateGroupCSGL(Container *pCont, const char* name) : StateGroupCS(pCont, name) {}
        virtual void apply();
        virtual bool validate();
    };

    class StateGroupDSTGL : public StateGroupDST
    {
    public:
        StateGroupDSTGL(Container *pCont, const char* name) : StateGroupDST(pCont, name) {}
        virtual void apply();
        virtual bool validate();
    };

#ifndef OGLES2
    class StateGroupPathGL : public StateGroupPath
    {
    public:
        StateGroupPathGL(Container *pCont, const char* name) : StateGroupPath(pCont, name) {}
        void apply(IPass *pass, PassInfo * pr, int n);
        bool validate();
    };
#endif
    /*************************************************************************/ /**
     ** 
     ** 
     **/ /*************************************************************************/ 
    class ResourceGL : 
#ifdef USECUDA
        public ResourceCUDA
#else
        public Resource
#endif
    {
    protected:
        virtual bool    createRenderResource();
    public:
        ResourceGL(ResourceRepository* pCont, const char *name=NULL);
        ~ResourceGL();
        virtual bool    validationNeeded();
        virtual bool    validate();
        virtual bool    invalidate();
        virtual bool        setD3DResourceView(void *p) { return false;}
        virtual void*       getD3DResourceView() { return NULL; }
    };

    /*************************************************************************/ /**
     ** 
     ** 
     **/ /*************************************************************************/ 
    inline GLenum getGLTarget(ResourceType t)
    {
        switch(t)
        {
#ifndef OGLES2
        case RESTEX_1D: return GL_TEXTURE_1D;
        case RESTEX_2DRECT: return GL_TEXTURE_RECTANGLE;
        case RESTEX_3D: return GL_TEXTURE_3D;
        case RESTEX_CUBE_MAP: return GL_TEXTURE_CUBE_MAP;
#endif
        case RESTEX_2D: return GL_TEXTURE_2D;
        case RESRBUF_2D:
        default :
            break;
        }
        CHECK_TRUE_MSG(false,"getGLTarget() Missing a case for the ResourceType you want");
        return 0;
    }
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
    class PathShader : public Shader
    {
    private:
        bool    m_bPostscript;
    public:
        PathShader(const char *name = NULL, IContainer* pCont = NULL, bool bPostscript=false);
        ~PathShader();

        void    cleanupShader();
        /// \arg \b type can be 0 to 4 (FX_VTXPROG FX_GEOMPROG FX_FRAGPROG FX_TCSPROG FX_TESPROG)
        bool    isCompiled(int type);
        bool    compileShader(ShaderType type, IContainer *pContainer);

        GLhandleARB getGLSLShaderObj(GLenum type); // TODO: rename this method without GLSL (or as GL)

        //IShader* asPathShader() { return this; }

        friend class PathProgram;
        friend class Container;
        friend class Pass;
    };
    /*************************************************************************/ /**
     ** 
     ** 
     **/ /*************************************************************************/ 
    class PathProgram : public Program
    {
    private:
        //typedef std::vector<PathShader*> ShaderVec;
        typedef std::vector<PathShader*> ShaderVec;
        bool        m_usable;
        //bool        m_bound; // turns out to be useless and risky
        bool        m_linkNeeded;
        bool        m_separable;
        int         m_shaderFlags; /// place where we store GL_VERTEX_SHADER_BIT etc.
        GLhandleARB m_program;
        ShaderVec   m_shaders; ///< list of attached compute shaders, sorted by the name
    public:
        PathProgram(Container *pCont);
        ~PathProgram();

        virtual bool        addShader(ShaderType type, IShader* pShader, IContainer* pContainer);
        virtual bool        addFragmentShader(IShader* pShader, IContainer* pContainer);
        virtual bool        addVertexShader(IShader* pShader, IContainer* pContainer);
        //TODO: geom, tess...
        virtual IShader*    getShader(int n, ShaderType *t = NULL);
        virtual IShader*    getShader(IShader *pShader, ShaderType *t = NULL);
        virtual IShader*    getShader(const char *name, ShaderType *t = NULL);
        virtual int         getNumShaders();

        virtual void        cleanup();
        virtual bool        bind(IContainer* pContainer);
        virtual void        unbind(IContainer* pContainer);
        virtual bool        link(IContainer* pContainer);
        virtual bool        linkNeeded(int bYes) { if(bYes >= 0) m_linkNeeded = bYes?true:false; return m_linkNeeded; }
        virtual int         getProgram() {return m_program;}
        virtual int         getProgramShaderFlags() { return m_shaderFlags; }
        virtual int         getUniformLocation(const char* name);

        virtual int         getAttribLocation(const char* attrName);
        virtual void        bindAttribLocation(int i, const char* attrName);

        virtual void        setUniform(const char* name, float val);
        virtual void        setUniform(const char* name, int val);
        virtual void        setUniformVector(const char * name, const float* val, int count, int numUniforms=1);
        virtual void        setUniformVector(const char * name, const int* val, int count, int numUniforms=1);
        virtual void        setSampler(const char * texname, int texunit);
        virtual void        bindTexture(ResourceType target, const char * texname, GLuint texid, int texunit);

#ifndef OGLES2
        virtual bool        bindSubRoutineToUniform(int uniformID, GLenum shadertype, char **subroutineNames, int numNames);
        virtual int         getSubRoutineID(const char *name, GLenum shadertype);
#endif
        /// \name D3D-specific methods
        /// @{
        virtual void*       getD3DIASignature() { return NULL; }
        virtual int         getD3DIASignatureSize() { return 0; }
        /// @}
        /// \name Compute-specific method(s)
        /// @{
        virtual bool execute(int szx, int szy, int szz);
        virtual bool execute(RenderingMode mode, const PassInfo::PathInfo *p);
        /// @}

        //friend      class Container;
        //friend      class Pass;
    };



}//namespace nvFX

#endif
