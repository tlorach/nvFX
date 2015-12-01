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
#ifndef __FXCUDA_H__
#define __FXCUDA_H__
#include <vector>
#include <map>
#include <string>

#define USEMAPRANGE
//#define USEBUFFERSUBDATA

//#undef NVFXCHECKCUDAERRORS
#ifdef NVFXCHECKCUDAERRORS
#define CHECKCUDAERRORS(s) checkCUDAError(s)
#define PRINT_CUDA_ERROR(s)  checkCUDAError(s)
#else
#define CHECKCUDAERRORS(s) 0
#define PRINT_CUDA_ERROR
#endif

#include "Fx.h"
#include <cuda.h>// Would be good to remove this include !
#include <cudaGL.h>
#include <builtin_types.h>

    class CUmoduleManager;
namespace nvFX
{
    class Container;
    class Technique;
    class Pass;
    class CUDAShader;
    class CUDAProgram;
    class SamplerState;
    class Uniform;
    class PassState;
    class CstBuffer;
    class Shader;

    /*************************************************************************/ /**
     ** \name internal structures
     **
     **/ /*************************************************************************/ 
    /// @{
	//
	/// Linear memory buffers
	//
	struct CUDABuffer
	{
        CUDABuffer() : size(0), dptr(NULL) {}
		int						size;
		CUdeviceptr				dptr;
	};
	extern std::map<int, CUDABuffer>	buffers;
    enum CUDAArgType
    {
        CUDAARG_UNKNOWN,
        CUDAARG_INT,
        CUDAARG_FLOAT,
        CUDAARG_PTR_BUFFER,
        CUDAARG_PTR_BUFFER_TEX, // can be possible with interop. But better to go through tex sampler
        //CUDAARG_PTR_BUFFER_RB,
    };

    union CUDAArgVal {
        float f;
        int    i;
        CUdeviceptr dptr;
    };
    struct CUDAArg
    {
        CUDAArg() { vals = NULL; }
        std::string    name;
        //CGparameter    param;
        int textureRef;
        int offset;
        int sizeOf;
        int ncomps;
        CUDAArgType type;
        CUDAArgVal* vals;
    };
    struct CUDAKernel
    {
        CUDAKernel()
            {argsSz = 0; fpFunc = NULL; sharedMemory = 0; block = dim3(); grid = dim3(); streamID = NULL; }
        std::string    kernelName;
        std::string    kernelFuncName;
        //CGparameter    param;
        CUfunction    fpFunc;
        CUstream    streamID;
        int            sharedMemory;
        dim3        block;
        dim3        grid;
        std::vector<CUDAArg> args;
        int            argsSz;
    };
    /// @}
    /*************************************************************************/ /**
     ** 
     ** 
     **/ /*************************************************************************/ 
    class CstBufferCUDA : public CstBuffer
    {
    private:
        int                        m_size;
        int                        m_handle;
        CUdeviceptr                m_dptr;
    public:
        CstBufferCUDA(const char* name) : CstBuffer(name) {}
        ~CstBufferCUDA() {}
        virtual CstBuffer*  update(Pass *pass, int layerID, bool bCreateIfNeeded, bool bCreateBufferIfNeeded)
        { /*TODO if needed*/return this; }
    };
    /*************************************************************************/ /**
    ** \brief Uniform Parameter class
    ** \note CUDA uniform extends GLSL uniforms. Nothing done for D3D case (yet)
    **/ /*************************************************************************/ 
    class UniformCUDA : public Uniform
    {
    public:
        virtual ~UniformCUDA();
        UniformCUDA(const char* name = NULL, const char* groupname = NULL, const char* semantic = NULL);

        Uniform*            updateTextures(ShadowedData* data, Pass *pass, int layerID, bool bCreateIfNeeded);
        virtual Uniform*    update(ShadowedData* pData, Pass *pass, int ppID, bool bCreateIfNeeded);
        virtual Uniform*    updateForTarget(ShadowedData* pData, int target);
    };

    /*************************************************************************/ /**
     ** 
     **/ /*************************************************************************/ 
    class CUDAShader : public Shader
    {
    public:
        CUDAShader(const char *name = NULL);
        ~CUDAShader();

        virtual void    cleanupShader()                     {}
        virtual bool    isCompiled(int type)                { return false; }
        virtual GLhandleARB getGLSLShaderObj(GLenum type)   { return NULL; }

        friend class CUDAProgram;
        friend class CUDAShaderProgram;
        friend class Container;
        friend class Pass;
    };

    /*************************************************************************/ /**
     ** 
     ** 
     **/ /*************************************************************************/ 
    struct KernelEntry
    {
        std::string funcName;
        int         gridSz[3];
        int         blockSz[3];
        int         ShMemSz;
        int         numArgs;
        ArgVal      *args;
        CUfunction  cudaFunc;
    };
    /*************************************************************************/ /**
     ** 
     ** 
     **/ /*************************************************************************/ 
    class CUDAProgram : public Program
    {
    private:
        int                 m_moduleID;
        CUmodule            m_cuModule;
        std::vector<KernelEntry> m_kernelEntries;

        typedef std::map<std::string, CUDAShader*> ShaderMap;
        bool        m_usable;
        bool        m_linkNeeded;
        ShaderMap   m_shaders; ///< list of attached vertex shaders, sorted by the name
        std::string m_fullCode;
        std::map<std::string, CUDABuffer> m_buffers; // CUDA buffers

        CUDABuffer  *CreateCudaBuffer(const char *name, int Sz);
    public:
        CUDAProgram(Container *pCont);
        ~CUDAProgram();
        virtual bool        addShader(ShaderType type, IShader* pShader, IContainer* pContainer);
        virtual bool        addFragmentShader(IShader* pShader, IContainer* pContainer)             { return false; }
        virtual bool        addVertexShader(IShader* pShader, IContainer* pContainer)               { return false; }

        virtual IShader*    getShader(int n, ShaderType *t = NULL);
        virtual IShader*    getShader(IShader *pShader, ShaderType *t = NULL);
        virtual IShader*    getShader(const char *name, ShaderType *t = NULL);
        virtual int         getNumShaders();

        virtual void        cleanup();
        virtual bool        bind(IContainer* pContainer);
        virtual void        unbind(IContainer* pContainer);
        virtual bool        link(IContainer* pContainer);
        virtual bool        linkNeeded(int bYes)                                { return false; }
        virtual int         getProgram()                                        { return 0;}
        virtual int         getProgramShaderFlags()                             { return 0; }
        virtual int         getUniformLocation(const char* name)                { return 0; }

        virtual int         getAttribLocation(const char* attrName)             { return 0; }
        virtual void        bindAttribLocation(int i, const char* attrName)     {}

        virtual void        setUniform(const char* name, float val)                                                 {}
        virtual void        setUniform(const char* name, int val)                                                   {}
        virtual void        setUniformVector(const char * name, const float* val, int count, int numUniforms=1)     {}
        virtual void        setUniformVector(const char * name, const int* val, int count, int numUniforms=1)       {}
        virtual void        setSampler(const char * texname, int texunit)                                           {}
        virtual void        bindTexture(ResourceType target, const char * texname, GLuint texid, int texunit)        {}
#ifndef OGLES2
        virtual bool        bindSubRoutineToUniform(int uniformID, GLenum shadertype, char **subroutineNames, int numNames) { return false; }
        virtual int         getSubRoutineID(const char *name, GLenum shadertype)    { return 0; }
#endif

        virtual void*       getD3DIASignature()                                 { return NULL; }
        virtual int         getD3DIASignatureSize()                             { return 0; }

		virtual bool		execute(int szx, int szy, int szz=1)                { return false; } // fails because only for Graphic-compute
        virtual bool        execute(RenderingMode mode, const PassInfo::PathInfo *p) { return false; } // fails because only for path programs

        virtual int         createKernelEntry(const char*kernelName, int *gridSz/*[3]*/, int *blockSz/*[3]*/, int ShMemSz,int numArgs, ArgVal *args);
        virtual bool        executeKernelEntry(int entryID);

        friend class UniformCUDA;
    };

    /*************************************************************************/ /**
     ** 
     ** 
     **/ /*************************************************************************/ 
    class ResourceCUDA : public Resource
    {
    private:
        int                 m_size, m_xByteSz;
        size_t              m_pitch;
        CUdeviceptr         m_dptr;
        CUgraphicsResource  m_cudaResource;
        //CUtexref            m_texRef; wrong place : should be in a target
    protected:
        bool            setupAsCUDATarget();
        bool            setupAsCUDATexture();
        bool            updateResourceFromCUDA(CUstream streamID=NULL);
        CUarray         mapResource(CUstream streamID=NULL);
        bool            unmapResource(CUstream streamID=NULL);
        bool            unmapFromCUDA() { return unmapResource(); }
    public:
        ResourceCUDA(ResourceRepository* pCont, const char *name=NULL);
        ~ResourceCUDA();

        friend class CUDAProgram;
        friend class UniformCUDA;
    };


}//namespace nvFX

#endif
