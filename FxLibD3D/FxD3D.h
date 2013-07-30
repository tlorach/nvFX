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
#ifndef __FXD3D_H__
#define __FXD3D_H__
#include <vector>
#include <map>
#include <string>

#ifndef LOGI
#   define  LOGI(...)  { m_container->eprintf(__VA_ARGS__); m_container->eprintf("\n"); }
#   define  LOGE(...)  { m_container->eprintf(">>Error : "); m_container->eprintf(__VA_ARGS__); m_container->eprintf("\n"); }
#endif

#define SHADERCONCAT_USE // DX can't link any sort of Shader objects...

#include "FxLib.h"
#ifdef USE_D3D11
#include <d3d11.h>
#include <d3d11Shader.h>
#include <d3dx11.h>
#include <d3dx10.h>
typedef ID3D11Texture1D         ID3D1XTexture1D;
typedef ID3D11Texture2D         ID3D1XTexture2D;
typedef ID3D11Texture3D         ID3D1XTexture3D;
typedef ID3D11Device            ID3D1XDevice;
typedef ID3D11RasterizerState   ID3D1XRasterizerState;
typedef ID3D11BlendState        ID3D1XBlendState;
typedef ID3D11DepthStencilState ID3D1XDepthStencilState;
typedef ID3D10Blob              ID3D1XBlob;
typedef ID3D11DeviceChild       ID3D1XDeviceChild;
typedef ID3D11VertexShader      ID3D1XVertexShader;
typedef ID3D11GeometryShader    ID3D1XGeometryShader;
typedef ID3D11PixelShader       ID3D1XPixelShader;
typedef ID3D11ShaderReflection  ID3D1XShaderReflection;
typedef ID3D11Buffer            ID3D1XBuffer;
typedef ID3D11SamplerState      ID3D1XSamplerState;
typedef ID3D11ShaderReflectionConstantBuffer    ID3D1XShaderReflectionConstantBuffer;
typedef ID3D11ShaderReflectionVariable          ID3D1XShaderReflectionVariable;
typedef ID3D11ShaderResourceView                ID3D1XShaderResourceView;
typedef D3D11_BUFFER_DESC       D3D1X_BUFFER_DESC;
typedef D3D11_SHADER_DESC       D3D1X_SHADER_DESC;
typedef D3D11_SUBRESOURCE_DATA  D3D1X_SUBRESOURCE_DATA;
typedef D3D11_FILL_MODE         D3D1X_FILL_MODE;
typedef D3D11_CULL_MODE         D3D1X_CULL_MODE;
typedef D3D11_COMPARISON_FUNC   D3D1X_COMPARISON_FUNC;
typedef D3D11_STENCIL_OP        D3D1X_STENCIL_OP;
typedef D3D11_BLEND             D3D1X_BLEND;
typedef D3D11_BLEND_OP          D3D1X_BLEND_OP;
typedef D3D11_RASTERIZER_DESC   D3D1X_RASTERIZER_DESC;
typedef D3D11_BLEND_DESC        D3D1X_BLEND_DESC;
typedef D3D11_FILTER            D3D1X_FILTER;
typedef D3D11_SAMPLER_DESC      D3D1X_SAMPLER_DESC;
typedef D3D11_SHADER_BUFFER_DESC                D3D1X_SHADER_BUFFER_DESC;
typedef D3D11_DEPTH_STENCIL_DESC                D3D1X_DEPTH_STENCIL_DESC;
typedef D3D11_SHADER_VARIABLE_DESC              D3D1X_SHADER_VARIABLE_DESC;
typedef D3D11_SHADER_VARIABLE_DESC              D3D1X_SHADER_VARIABLE_DESC;
typedef D3D11_SIGNATURE_PARAMETER_DESC          D3D1X_SIGNATURE_PARAMETER_DESC;
typedef D3D11_SHADER_INPUT_BIND_DESC            D3D1X_SHADER_INPUT_BIND_DESC;
typedef D3D11_TEXTURE_ADDRESS_MODE              D3D1X_TEXTURE_ADDRESS_MODE;

#define D3DX1XCompileFromMemory D3DX11CompileFromMemory
#define D3D1X(a) D3D11_##a
#define D3D1X_USAGE D3D11_USAGE

#else

#include <d3d10.h>
#include <d3dx10.h>
typedef ID3D10Texture1D         ID3D1XTexture1D;
typedef ID3D10Texture2D         ID3D1XTexture2D;
typedef ID3D10Texture3D         ID3D1XTexture3D;
typedef ID3D10Device            ID3D1XDevice;
typedef ID3D10RasterizerState   ID3D1XRasterizerState;
typedef ID3D10BlendState        ID3D1XBlendState;
typedef ID3D10DepthStencilState ID3D1XDepthStencilState;
typedef ID3D10Blob              ID3D1XBlob;
typedef ID3D10DeviceChild       ID3D1XDeviceChild;
typedef ID3D10VertexShader      ID3D1XVertexShader;
typedef ID3D10GeometryShader    ID3D1XGeometryShader;
typedef ID3D10PixelShader       ID3D1XPixelShader;
typedef ID3D10ShaderReflection  ID3D1XShaderReflection;
typedef ID3D10Buffer            ID3D1XBuffer;
typedef ID3D10SamplerState      ID3D1XSamplerState;
typedef ID3D10ShaderReflectionConstantBuffer    ID3D1XShaderReflectionConstantBuffer;
typedef ID3D10ShaderReflectionVariable          ID3D1XShaderReflectionVariable;
typedef ID3D10ShaderResourceView                ID3D1XShaderResourceView;
typedef D3D10_BUFFER_DESC       D3D1X_BUFFER_DESC;
typedef D3D10_SHADER_DESC       D3D1X_SHADER_DESC;
typedef D3D10_SUBRESOURCE_DATA  D3D1X_SUBRESOURCE_DATA;
typedef D3D10_FILL_MODE         D3D1X_FILL_MODE;
typedef D3D10_CULL_MODE         D3D1X_CULL_MODE;
typedef D3D10_COMPARISON_FUNC   D3D1X_COMPARISON_FUNC;
typedef D3D10_STENCIL_OP        D3D1X_STENCIL_OP;
typedef D3D10_BLEND             D3D1X_BLEND;
typedef D3D10_BLEND_OP          D3D1X_BLEND_OP;
typedef D3D10_RASTERIZER_DESC   D3D1X_RASTERIZER_DESC;
typedef D3D10_BLEND_DESC        D3D1X_BLEND_DESC;
typedef D3D10_FILTER            D3D1X_FILTER;
typedef D3D10_SAMPLER_DESC      D3D1X_SAMPLER_DESC;
typedef D3D10_SHADER_BUFFER_DESC                D3D1X_SHADER_BUFFER_DESC;
typedef D3D10_DEPTH_STENCIL_DESC                D3D1X_DEPTH_STENCIL_DESC;
typedef D3D10_SHADER_VARIABLE_DESC              D3D1X_SHADER_VARIABLE_DESC;
typedef D3D10_SHADER_VARIABLE_DESC              D3D1X_SHADER_VARIABLE_DESC;
typedef D3D10_SIGNATURE_PARAMETER_DESC          D3D1X_SIGNATURE_PARAMETER_DESC;
typedef D3D10_SHADER_INPUT_BIND_DESC            D3D1X_SHADER_INPUT_BIND_DESC;
typedef D3D10_TEXTURE_ADDRESS_MODE              D3D1X_TEXTURE_ADDRESS_MODE;

#define D3DX1XCompileFromMemory D3DX10CompileFromMemory
#define D3D1X(a) D3D10_##a
#define D3D1X_USAGE D3D10_USAGE
#endif
#include <D3Dcompiler.h>

namespace nvFX
{
class Container;
class Technique;
class Pass;
class D3DShader;
class D3DShaderProgram;
class SamplerState;
class Uniform;
class PassState;
class Shader;
class Texture;

//bool checkD3DError(const char *str);

/*************************************************************************/ /**
 ** 
 ** 
 **/ /*************************************************************************/ 
class CstBufferD3D : public CstBuffer
{
private:
    void                updateD3D(STarget &t);
    int                 bufferSizeAndData(char *pData, Uniform* pU, int &szBytes, int &pk);
public:
    CstBufferD3D(const char* name);
    ~CstBufferD3D();

    // TODO: change this approach : update() should be done in CstBuffer
    // target creation should be done outside of update
    virtual CstBuffer*  update(Pass *pass, int layerID, bool bBindProgram, bool bCreateIfNeeded, bool bCreateBufferIfNeeded);
    virtual CstBuffer*  update2(Pass *pass, bool bBindProgram, bool bCreateIfNeeded, bool bCreateBufferIfNeeded);
    virtual CstBuffer*  updateForTarget(STarget &t, bool bBindProgram = false);
    virtual int         bufferSizeAndData(char *pData, int *sz=NULL);
    virtual void*       buildD3DBuffer(ICstBuffer::BufferUsageD3D usage);
    virtual ICstBuffer* setD3DBuffer(void* buffer);
    virtual bool        updateFromUniforms(bool bForceUpdating=false);
    virtual bool        mapBuffer(void** ppData, bool bForceUpdating=false);
    virtual void        unmapBuffer();
};
/*************************************************************************/ /**
** \brief Uniform Parameter class
**/ /*************************************************************************/ 
class UniformD3D : public Uniform
{
public:
    ~UniformD3D();
    UniformD3D(const char* name = NULL, const char* groupname = NULL, const char* semantic = NULL);
    // TODO: change this approach : update() should be done in CstBuffer
    // target creation should be done outside of update
    virtual Uniform*    update(ShadowedData* pData, Pass *pass, int ppID, bool bBindProgram, bool bCreateIfNeeded);
    virtual Uniform*    update2(ShadowedData* pData, Pass *pass, bool bBindProgram, bool bCreateIfNeeded);
    void                updateD3D(ShadowedData* pData, STarget &t, bool bBindProgram);
    virtual Uniform*    updateForTarget(ShadowedData* pData, STarget &t, bool bBindProgram = false);
};

/*************************************************************************/ /**
 ** \brief class for Rasterization states
 **
 ** This class is based on 
 **
 ** \todo use this extension whenever possible...
 **/ /*************************************************************************/ 
 class StateGroupRasterD3D : public StateGroupRaster
{
private:
    ID3D1XRasterizerState*  m_d3dStateGroup;
public:
    ~StateGroupRasterD3D();
    StateGroupRasterD3D(Container* pCont, const char* name = NULL);
    void            apply();
    bool            validate();
};

/*************************************************************************/ /**
 ** \brief class for Color samples states
 ** 
 **/ /*************************************************************************/ 
 class StateGroupCSD3D : public StateGroupCS
{
private:
    ID3D1XBlendState*       m_d3dStateGroup;
public:
    ~StateGroupCSD3D();
    StateGroupCSD3D(Container* pCont, const char* name = NULL);
    void            apply();
    bool            validate();
};

/*************************************************************************/ /**
 ** \brief class for Depth Stencil States
 ** 
 **/ /*************************************************************************/ 
 class StateGroupDSTD3D : public StateGroupDST
{
private:
    ID3D1XDepthStencilState*    m_d3dStateGroup;
public:
    ~StateGroupDSTD3D();
    StateGroupDSTD3D(Container* pCont, const char* name = NULL);
    void            apply();
    bool            validate();
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
class D3DShader : public Shader
{
public:
    D3DShader(const char *name = NULL, IContainer* pCont = NULL);
    ~D3DShader();

    void    cleanupShader();
    /// \arg \b type can be 0 to 4 (FX_VTXPROG FX_GEOMPROG FX_FRAGPROG FX_TCSPROG FX_TESPROG)
    bool    isCompiled(int type);
    bool    compileShader(GLenum type=0);

    GLhandleARB getGLSLShaderObj(GLenum type);

    friend class D3DShaderProgram;
    friend class Container;
    friend class Pass;
};
/*************************************************************************/ /**
 ** \brief a program in the OpenGL meaning : contains 1 or more shader stages
 **
 ** Important to understand : this design is originally meant to be orthogonal with
 ** OpenGL. For separate Shader definition.
 ** Now, this nvFX::D3DShaderProgram is capable of handling only one stage of shader.
 ** We still name it 'Program' (D3DShaderProgram), to stay consistent with OpenGL :
 ** the interface is still IProgram.
 **/ /*************************************************************************/ 
class D3DShaderProgram : public Program
{
private:
    typedef std::map<std::string, D3DShader*> ShaderMap;
    /// \brief structure that contains all about a specific shader stage
    struct ShaderData {
        ShaderMap           shaders; ///< \brief list of attached vertex shaders, sorted by the name
        std::string         code; ///< \brief source code for the shader to compile
        ID3D1XBlob *        compiledShader; ///< \brief compiled shader. Need to keep it for IA signature, for example
        ID3D1XShaderReflection* reflector; ///< the interface to query what is inside the shader
        union {
            ID3D1XDeviceChild*      shader;
            ID3D1XVertexShader*     vtxShader;
            ID3D1XGeometryShader*   gsShader;
            ID3D1XPixelShader*      pixShader;
            // TODO: add the others
        };
    };
    bool        m_linkNeeded;
    int         m_shaderFlags; /// place where we store FX_VERTEX_SHADER_BIT etc. NOTE: only one flag is allowed in D3D (one shader per Program... separable mode)
    ShaderData  m_data; ///< shader data. Now only one : a program can host only one shader at a time
    Container*          m_container; ///< used to call eprintf, for example
    void replaceLineNumbers(char *strDest, int len, LPCSTR log, std::vector<D3DShader::codeInfo> &codeBlocks);
public:
    D3DShaderProgram(Container *pCont);
    ~D3DShaderProgram();

    bool        addShader(ShaderType type, IShader* pShader, IContainer* pContainer);
    // we should remove those 2 methods...
    bool        addFragmentShader(IShader* pShader, IContainer* pContainer);
    bool        addVertexShader(IShader* pShader, IContainer* pContainer);
    IShader*    getShader(int n, ShaderType *t = NULL);
    IShader*    getShader(IShader *pShader, ShaderType *t = NULL);
    IShader*    getShader(const char *name, ShaderType *t = NULL);
    int         getNumShaders();

    void        cleanup();
    bool        bind(IContainer* pContainer);
    void        unbind(IContainer* pContainer);
    bool        link(IContainer* pContainer);
    bool        linkNeeded(int bYes) { if(bYes >= 0) m_linkNeeded = bYes?true:false; return m_linkNeeded; }
    int         getProgram() {return 0;}
    int         getProgramShaderFlags() { return m_shaderFlags; }
    int         getUniformLocation(const char* name);

    int         getAttribLocation(const char* attrName);
    void        bindAttribLocation(int i, const char* attrName);

    void        setUniform(const char* name, float val);
    void        setUniform(const char* name, int val);
    void        setUniformVector(const char * name, const float* val, int count, int numUniforms=1);
    void        setUniformVector(const char * name, const int* val, int count, int numUniforms=1);
    void        setSampler(const char * texname, int texunit);
    void        bindTexture(ResourceType target, const char * texname, GLuint texid, int texunit);

#ifndef OGLES2
    bool        bindSubRoutineToUniform(int uniformID, GLenum shadertype, char **subroutineNames, int numNames);
    int         getSubRoutineID(const char *name, GLenum shadertype);
#endif
    /// \name D3D-specific methods
    /// @{
    virtual void* getD3DIASignature(); ///< returns the Input attribute signature (in fact : this is the vertex shader compiled Blob)
    virtual int getD3DIASignatureSize(); ///< returns the Input attribute signature size (in fact : this is the vertex shader compiled Blob size)
    /// @}

    friend      class Container;
    friend      class Pass;
    friend      class UniformD3D;
    friend      class SamplerStateD3D;
    friend      class CstBufferD3D;
};
/*************************************************************************/ /**
 ** 
 ** 
 **/ /*************************************************************************/ 
class SamplerStateD3D : public SamplerState
{
public:
    SamplerStateD3D(Container* pCont, const char* name = NULL);
    ~SamplerStateD3D();
    virtual bool        setD3DSamplerState(void *p);
    /// \brief updates sampler-state. Same idea than uniform set/update mechanism. But this one is OpenGL focused
    virtual void    update(GLenum target, GLint tex, bool bindTexture=false);
    /// \brief update targets with the shadowed values. Same mechanism as for uniform (set() vs. update())
    virtual SamplerState*  update(void *data, Pass *pass, int layerID, bool bBindProgram, bool bCreateIfNeeded);
    /// \brief update2 is called when layerID == -1
    virtual SamplerState*  update2(void *data, Pass *pass, bool bBindProgram, bool bCreateIfNeeded);
    /// \brief update only for a specific target
    virtual SamplerState*  updateForTarget(void *data, STarget &t, bool bBindProgram = false);
    /// \validates the texture object. Essentially needed to setup something on Gfx API side (D3D, for example... not OpenGL)
    bool            validate();
    // TODO ? Add setXXX() ?
};

/*************************************************************************/ /**
 ** 
 ** 
 **/ /*************************************************************************/ 
class ResourceD3D : public Resource
{
protected:
    DXGI_FORMAT                         m_d3dFmt;
    union {
        ID3D10Texture1D*                m_pTexture1D;
        ID3D10Texture2D*                m_pTexture2D;
        ID3D10Texture3D*                m_pTexture3D;
        void*                           m_ptr;
    };
    ID3D10ShaderResourceView*           m_pTextureView;
    union {
        ID3D10RenderTargetView*         m_pTextureRTView; ///< used if the resource is part of a 'FBO'
        ID3D10DepthStencilView*         m_pTextureDSTView; ///< used if the resource is part of a 'FBO'
    };
    bool    createRenderResource();
public:
    ResourceD3D(ResourceRepository* pCont, const char *name=NULL);
    ~ResourceD3D();
    virtual bool    validationNeeded();
    virtual bool    validate();
    virtual bool    invalidate();

    virtual bool        setD3DResourceView(void *p);
    virtual void*       getD3DResourceView();

    friend class FrameBufferObject;
    friend class FrameBufferObjectsRepository;
    friend class Pass;
    friend class UniformD3D;
};


inline TargetType ShaderFlags2Type(int flag)
{
    switch(flag)
    {
    case FX_VERTEX_SHADER_BIT:          return THLSL_VTX;
    case FX_FRAGMENT_SHADER_BIT:        return THLSL_PIX;
    case FX_GEOMETRY_SHADER_BIT:        return THLSL_GS;
    case FX_TESS_CONTROL_SHADER_BIT:    return THLSL_HS;
    case FX_TESS_EVALUATION_SHADER_BIT: return THLSL_DS;
    }
    return TANY;
}

/*************************************************************************/ /**
 ** \brief 
 ** 
 ** 
 **/ /*************************************************************************/ 
class D3DProgramPipeline : public ProgramPipeline
{
protected:
    virtual bool    bind();
public:
    D3DProgramPipeline(Container *pCont);
};

}//namespace nvFX

#endif
