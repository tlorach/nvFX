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

    1.00 : build CL.13240315 6/12/2012 5:58pm
    1.01 : tlorach@nvidia.com : some optional CUDA support
    1.03 : tlorach@nvidia.com : GLSLCompute support
    1.04 : tlorach@nvidia.com : NV Path rendering support
    1.05 : tlorach@nvidia.com : added global/local qual. for Uniforms and CstBuffer. We can share them across Effects
    1.06 : tlorach@nvidia.com : Added separable shader objects as an option in the effect
    1.07 : tlorach@nvidia.com : case sensitive token, better parser and changes in keywords
    1.08 : tlorach@nvidia.com : added automatic texture unit assignment to sampler-uniforms and added samplers overriding
    1.09 : tlorach@nvidia.com : added sampler overriding : to allow samplers of additional shader code to be updated
    1.16 : tlorach@nvidia.com : Texture/resource changes : removed ITexture[Ex] and using IResource[Ex] only
    1.17 : tlorach@nvidia.com : Clear definition of texture samplers/D3D textures as uniforms. But CUDA for now is down because of this change.
    1.18 : tlorach@nvidia.com : added the concept of "namespace" inside a nvFX effect file. namespace can have annotations. So can Shaders, now
    1.19 : tlorach@nvidia.com : fixed some issues with resource annotations. Cannot anymore put resources inside texture-resource params
    1.20 : tlorach@nvidia.com : fixed some issues with shader annotations
*/
#ifndef __FXLIB_H__
#define __FXLIB_H__

#ifndef assert
#define assert(a)
#endif
// Possible missing types (ES2)
#ifndef GLhandleARB
typedef unsigned int GLhandleARB;
#endif
#ifndef GLenum
typedef unsigned int GLenum;
typedef unsigned char GLboolean;
typedef float GLclampf;
typedef int GLint;
typedef unsigned int GLuint;
#endif
#ifdef WIN32
typedef void* BufferHandle;
#else
typedef unsigned int BufferHandle;
#endif

namespace nvFX
{
#define NVFX_MAJOR_VERSION 1
#define NVFX_MINOR_VERSION 20

typedef void (* messageCallbackFunc)(const char *errMsg);

class ICstBuffer;
class ICstBufferEx;
class IContainer;
class IContainerEx;
class ITechnique;
class ITechniqueEx;
class IPass;
class IPassEx;
class IProgram;
class IProgramEx;
class ISamplerState;
class ISamplerStateEx;
class IStateGroupRaster;
class IStateGroupRasterEx;
class IStateGroupCS;
class IStateGroupCSEx;
class IStateGroupDST;
class IStateGroupDSTEx;
class IStateGroupPathEx;
class IUniform;
class IUniformEx;
class IPassState;
class IPassStateEx;
class IShader;
class IShaderEx;
class IResource;
class IResourceEx;
class IFrameBufferObject;
class IFrameBufferObjectEx;
class IAnnotation;
class IAnnotationEx;
//class IShaderProgram;
class IProgramPipeline;
class IProgramPipelineEx;
class IResourceRepository;
class IResourceRepositoryEx;
class IFrameBufferObjectsRepository;
class IFrameBufferObjectsRepositoryEx;
class IFrameBufferObjectsRepository;
class IFrameBufferObjectsRepositoryEx;
class IShaderModuleRepository;
class IShaderModuleRepositoryEx;
class ICstBufferRepositoryEx;
class IUniformRepositoryEx;

enum AccessMode { // for image Load/Store access (and others later)
    ACCESS_UNDEFINED = 0,
    ACCESS_READ_WRITE,
    ACCESS_READ_ONLY,
    ACCESS_WRITE_ONLY,
};
/*************************************************************************/ /**
 ** \brief primitive mode that eventually gets requested by a pass
 ** 
 **/ /*************************************************************************/ 
enum PrimitiveMode
{
    PRIMITIVEMODE_DEFAULT = 0,
    PRIMITIVEMODE_PATCH = 1,
    PRIMITIVEMODE_UNDEFINED
};
/*************************************************************************/ /**
 ** \brief rendering mode that eventually gets requested by a pass
 ** the application must then take the proper acion from what was requested
 **/ /*************************************************************************/ 
enum ClearMode
{
    Clear_none = 0,
    Clear_all = 7,
    Clear_color = 1,
    Clear_depth = 2,
    Clear_stencil = 4,
    Clear_color_depth = 3,
    Clear_depth_stencil = 6,
};
/*************************************************************************/ /**
 ** \brief formats available for the render-buffers and render-textures
 **/ /*************************************************************************/ 
// Note: these defines are useful to create a different list from the one down here : undef and define them again...
#define RESOURCEFORMATITEM(a) a,
#define RESOURCEFORMATITEM_VAL(a,b) a = b,
#define RESOURCEFORMATITEM_NOTUSED(a)
#ifdef OGLES2
#define RESOURCEFORMATITEM_ES(a) a,
#else
#define RESOURCEFORMATITEM_ES(a)
#endif
#define RESOURCEFORMATLIST \
RESOURCEFORMATITEM_VAL(FMT_UNKNOWN,0)\
RESOURCEFORMATITEM_ES(FMT_ALPHA)\
RESOURCEFORMATITEM_ES(FMT_LUMINANCE)\
RESOURCEFORMATITEM_ES(FMT_LUMINANCE_ALPHA)\
RESOURCEFORMATITEM_ES(FMT_RGB)\
RESOURCEFORMATITEM_ES(FMT_RGBA)/*ES2 case : we keep the rest down here just for the lexer to not fail. But they won't work for ES2*/\
RESOURCEFORMATITEM_NOTUSED(FMT_INTENSITY)\
RESOURCEFORMATITEM(FMT_INTENSITY8)\
RESOURCEFORMATITEM(FMT_INTENSITY16F)\
RESOURCEFORMATITEM(FMT_INTENSITY32F)\
RESOURCEFORMATITEM_NOTUSED(FMT_LUMINANCE_ALPHA)\
RESOURCEFORMATITEM(FMT_LUMINANCE_ALPHA8)\
RESOURCEFORMATITEM_NOTUSED(FMT_LA)\
RESOURCEFORMATITEM_NOTUSED(FMT_LA8)\
RESOURCEFORMATITEM(FMT_RG16F_NV)\
RESOURCEFORMATITEM_NOTUSED(FMT_LA16F)\
RESOURCEFORMATITEM(FMT_LUMINANCE_ALPHA16F)\
RESOURCEFORMATITEM_NOTUSED(FMT_LA32F)\
RESOURCEFORMATITEM(FMT_LUMINANCE_ALPHA32F)\
RESOURCEFORMATITEM(FMT_RG32F_NV)\
RESOURCEFORMATITEM_NOTUSED(FMT_RGB)\
RESOURCEFORMATITEM(FMT_RGB8)\
RESOURCEFORMATITEM(FMT_RGB16F)\
RESOURCEFORMATITEM(FMT_RGB32F)\
RESOURCEFORMATITEM_NOTUSED(FMT_RGBA)\
RESOURCEFORMATITEM(FMT_RGBA8)\
RESOURCEFORMATITEM(FMT_RGBA16F)\
RESOURCEFORMATITEM(FMT_RGBA32F)\
RESOURCEFORMATITEM(FMT_DEPTH24STENCIL8)\
RESOURCEFORMATITEM(FMT_DEPTH_COMPONENT32F)\
RESOURCEFORMATITEM(FMT_DEPTH32F_STENCIL8)\
RESOURCEFORMATITEM(FMT_RGBA8UI)\
RESOURCEFORMATITEM_NOTUSED(FLOAT_R_NV)\
RESOURCEFORMATITEM_NOTUSED(FLOAT_RG_NV)\
RESOURCEFORMATITEM_NOTUSED(FLOAT_RGB_NV)\
RESOURCEFORMATITEM_NOTUSED(FLOAT_RGBA_NV)\
RESOURCEFORMATITEM_NOTUSED(FLOAT_R16_NV)\
RESOURCEFORMATITEM_NOTUSED(FLOAT_R32_NV)\
RESOURCEFORMATITEM_NOTUSED(FLOAT_RG16_NV)\
RESOURCEFORMATITEM_NOTUSED(FLOAT_RG32_NV)\
RESOURCEFORMATITEM_NOTUSED(FLOAT_RGB16_NV)\
RESOURCEFORMATITEM_NOTUSED(FLOAT_RGB32_NV)\
RESOURCEFORMATITEM_NOTUSED(FLOAT_RGBA16_NV)\
RESOURCEFORMATITEM_NOTUSED(FLOAT_RGBA32_NV)\
RESOURCEFORMATITEM(FMT_FLOATRGBA16NV)\
RESOURCEFORMATITEM(FMT_RG)\
RESOURCEFORMATITEM(FMT_RG_INTEGER)\
RESOURCEFORMATITEM(FMT_R8)\
RESOURCEFORMATITEM(FMT_R16)\
RESOURCEFORMATITEM(FMT_RG8)\
RESOURCEFORMATITEM(FMT_RG16)\
RESOURCEFORMATITEM(FMT_R16F)\
RESOURCEFORMATITEM(FMT_R32F)\
RESOURCEFORMATITEM(FMT_RG16F)\
RESOURCEFORMATITEM(FMT_RG32F)\
RESOURCEFORMATITEM(FMT_R8I)\
RESOURCEFORMATITEM(FMT_R8UI)\
RESOURCEFORMATITEM(FMT_R16I)\
RESOURCEFORMATITEM(FMT_R16UI)\
RESOURCEFORMATITEM(FMT_R32I)\
RESOURCEFORMATITEM(FMT_R32UI)\
RESOURCEFORMATITEM(FMT_RG8I)\
RESOURCEFORMATITEM(FMT_RG8UI)\
RESOURCEFORMATITEM(FMT_RG16I)\
RESOURCEFORMATITEM(FMT_RG16UI)\
RESOURCEFORMATITEM(FMT_RG32I)\
RESOURCEFORMATITEM(FMT_RG32UI)

enum ResourceFormat
{
    RESOURCEFORMATLIST
};
/*************************************************************************/ /**
 ** \brief rendering mode that eventually gets requested by a pass
 ** the application must then take the proper acion from what was requested
 **/ /*************************************************************************/ 
enum RenderingMode
{
    PASS_DONE = 0,
    RENDER_SCENEGRAPH_SHADED = 1,
    RENDER_SCENEGRAPH_NOSHADING = 2,
    RENDER_FULLSCREEN_QUAD = 3,
    RENDER_SCENEGRAPH_SHADED_OPAQUE_ONLY = 4,
    RENDER_SCENEGRAPH_SHADED_TRANSPARENT_ONLY = 5,
    RENDER_SCENEGRAPH_NOSHADING_OPAQUE_ONLY = 6,
    RENDER_SCENEGRAPH_NOSHADING_TRANSPARENT_ONLY = 7,
    RENDER_SCENEGRAPH_OUTLINES_ONLY = 8,
    RENDER_DEBUG_QUAD0 = 9,
    RENDER_DEBUG_QUAD1 = 10,
    RENDER_DEBUG_QUAD2 = 11,
    RENDER_DEBUG_QUAD3 = 12,
    RENDER_DEBUG_SCENE = 13,
    RENDER_DEBUG_SCENE_LINES = 14,
    DO_NOTHING = 15,
//#ifdef USEOPTIX
    RENDER_OPTIX_REFLECTION = 16,
    RENDER_OPTIX_SHADOW = 17,
    RENDER_OPTIX_REFLECTION_AND_SHADOW = 18,
    RENDER_OPTIX = 19,
//#endif
    RUN_CUDA = 20,
    RUN_GLSLCOMPUTE = 21,
    RENDER_CUSTOM0, ///< custom rendering mode :for any purpose...
    RENDER_CUSTOM1,
    RENDER_CUSTOM2,
    RENDER_CUSTOM3,
    RENDER_CUSTOM4,
    RENDER_CUSTOM5,
    RENDER_CUSTOM6,
    RENDER_CUSTOM7,
    RENDER_CUSTOM8,
    RENDER_CUSTOM9,
    // render modes for NVIDIA Path rendering extension
    RENDER_STENCILFILLPATH,
    RENDER_STENCILSTROKEPATH,
    RENDER_COVERFILLPATH,
    RENDER_COVERSTROKEPATH,
    RENDERING_UNDEFINED,
    PASS_ERROR
};
/*************************************************************************/ /**
 ** \brief structure returned by the IPass::execute
 ** 
 **/ /*************************************************************************/ 
struct PassInfo  
{
#       define PR_RENDERINGMODE     1
#       define PR_RENDERINGGROUP    (1<<1) // the pass did set a rendering group
#       define PR_PRIMITIVEMODE     (1<<2)
#       define PR_CURRENTFBO        (1<<3) // the pass change the FBO target
#       define PR_BLITSOURCEFBO     (1<<4) // the pass made a blit
#       define PR_CLEARMODE         (1<<5)
#       define PR_CLEARCOLOR        (1<<6) // the pass changed the clear color
#       define PR_NEXTPASS          (1<<7) // the pass specified a specific next pass
#       define PR_VIEWPORT          (1<<8)  // the pass changed the viewport
#       define PR_OPTIX             (1<<9)  // the pass used Optix
#       define PR_CUDA              (1<<10) // the pass used CUDA
#       define PR_SWAPRESOURCES     (1<<11) // the pass swapped resources
#       define PR_COMPUTE           (1<<12) // the pass used compute
#       define PR_PATHRENDERING     (1<<13) // the pass used some path rendering
#       define PR_SHADERS           (1<<14) // the pass used some shaders
#       define PR_TESSSHADERS       (1<<15) // the pass used some tessellation shaders (good to know for issuing proper Draw commands - patches)
    unsigned int    flags; ///< flags that tell about modified fields
    IFrameBufferObject *prevFBO;        // to keep track of the previous FBO that was activated
    IFrameBufferObject *currentFBO;
    IFrameBufferObject *blitSourceFBO; // > 0 if needed to be done
    IFrameBufferObject *swapResourceFBOs[2];
    ClearMode       clearMode;
    bool            clearColorValid[4/*MAX_RENDERTARGETS*/];
    float           clearColor[4/*MAX_RENDERTARGETS*/][4];
    int             viewport[4]; // one for each MAX_RENDERTARGETS ? ///< contains (x,y,w,h, minZ, maxZ)
    float           viewportZ[2]; // one for each MAX_RENDERTARGETS ? ///< contains (x,y,w,h, minZ, maxZ)
    int             nextPass; // used to point to the next pass for the next round. good for skipping early passes used for init
    int             renderingGroup; // arbitrary number to request for some specific groups to render
    RenderingMode   renderingMode;
    PrimitiveMode   primitiveMode;
    int             overrideID; // ID for shader "override" action
    int             bActive; // if active or not. Filled by nvFX::ITechnique::getPass
    int             loopCnt; // # of loops required for this pass Filled by nvFX::ITechnique::getPass
    int             kernelEntry; // index of the kernel entry : points to a slot combining the entry+args
    /// \brief this structure gathers pass-states updated for Path rendering only
    struct PathInfo {
        unsigned int fillMode; ///< typically INVERT, COUNT_UP_NV, COUNT_DOWN_NV or PATH_FILL_MODE_NV
        unsigned int strokeReference;
        unsigned int fillMask;
        unsigned int strokeMask;
        unsigned int transformType;///< typically TRANSLATE_X_NV etc.
        unsigned int fillCoverMode; ///< typically CONVEX_HULL_NV or BOUNDING_BOX_NV
        unsigned int strokeCoverMode; ///< typically CONVEX_HULL_NV or BOUNDING_BOX_NV
        const float* transfValues;
    };
    PathInfo        pathRenderingSettings; ///< \brief settings for Path rendering (see nvFX::IProgramEx::execute )
};
/*************************************************************************/ /**
** \brief Types of shaders
**/ /*************************************************************************/ 
enum ShaderType
{
    FX_VTXPROG = 0,
    FX_GEOMPROG = 1,
    FX_FRAGPROG = 2,
    FX_TCSPROG = 3,
    FX_TESPROG = 4,
    FX_CUDAPROG = 5,
    FX_COMPUTEPROG = 6,
    FX_PATHPROG = 7,
    FX_UNKNOWNSHADER = 8,
};
/*************************************************************************/ /**
** \brief Type for textures
**/ /*************************************************************************/ 
enum TextureType
{
    TEXTURE_UNKNOWN = 0,
    TEXTURE_1D = 0x0DE0,
    TEXTURE_2D = 0x0DE1,
    TEXTURE_2DRECT = 0x84F5,
    TEXTURE_3D = 0x806F,
    TEXTURE_CUBE_MAP = 0x8513,
    TEXTURE_RECTANGLE = 0x84F5,
    // TODO: Add more ( http://www.opengl.org/wiki/Sampler_(GLSL) )
};
/*************************************************************************/ /**
** \brief Type for resource
** \todo See how to deal with Cube, 3D textures...
**/ /*************************************************************************/ 
enum ResourceType
{
    RESOURCE_UNKNOWN = 0,
    RESTEX_1D = 0x0DE0,
    RESTEX_2D = 0x0DE1,
    RESTEX_3D = 0x806F,
    RESTEX_2DRECT = 0x84F5,
    RESTEX_CUBE_MAP = 0x8513,
    RESRBUF_2D = 1,
    // TODO: Add more ( http://www.opengl.org/wiki/Sampler_(GLSL) )
};

/*************************************************************************/ /**
** \brief Target types : when we create uniforms (and Sampler-states or Textures
** they may have multiple targets : a parameter could either be used 
** in CUDA and OGL, for example
**/ /*************************************************************************/ 
enum TargetType {
    TGLSL,
    TCUDA,
    TGLSLCOMPUTE,
    THLSL, // generic HLSL Target type : for Shaders that are shared among Vtx, pix...
    THLSL_VTX,
    THLSL_PIX,
    THLSL_GS,
    THLSL_HS,
    THLSL_DS,
    TDXCOMPUTE,
    TCSTBUFFER, ///< the uniform is inside a UBO, D3D buffer or any other.
    TPATH, ///< target is OpenGL path rendering
    TANY
};

/************************************************************************************/ /**
** \brief Argument types : arguments for CUDA kernels (and possibly D3D shaders, later)
**/ /*************************************************************************************/ 
enum ArgumentType {
    TUndefined = 0,
    TInt, TInt2, TInt3, TInt4,
    TUInt, TUInt2, TUInt3, TUInt4,
    TFloat, TVec2, TVec3, TVec4,
    //TBool, TBool2, TBool3, TBool4,
    //TMat2, TMat3, TMat4, 
    TIntPtr, TInt2Ptr, TInt3Ptr, TInt4Ptr,
    TUIntPtr, TUInt2Ptr, TUInt3Ptr, TUInt4Ptr,
    TFloatPtr, TVec2Ptr, TVec3Ptr, TVec4Ptr,
};

/*************************************************************************/ /**
** \brief base interface used by all of our API interfaces
**/ /*************************************************************************/ 
template <class T>
class IBaseInterface
{
public:
    /// \brief returns the interface for container management (what the nvFX parser would use)
    /// \note for now this one has an implementation for debug purpose. TO REMOVE ASAP
    virtual T*       getExInterface()// = 0;
    {
        //return static_cast<T*>(this);
        //CHECK_TRUE_MSG(false,"**** getExInterface() : this method MUST be implemented ****");
        return NULL;
    }
};
/*************************************************************************/ /**
** \brief ICstBuffer interface : to work with constant buffers (UBO/D3D cst buffer)
**
** In OpenGL : maps the "layout(std140) uniform <name> { ..."
** 
** In D3D : maps the "cbuffer <name> { ..."
** \note for now there is no annotation and no semantics for this... see later if needed
**
** There are few different ways of using a Constant Buffer :
** - totally managed by the application : this interface got created from the knowledge
** of the application. Nothing else came from the nvFX effect
** - totally managed by the application and contains sub-uniforms (createUniform). In this
** case, the application can use IUniform to set separate values of this buffer. Then
** nvFX can help to update these values into the buffer.
** - Cst buffer resulting from nvFX effect parsing : introspection knows about this buffer
** and create sub-uniforms. Default values, may be available for initialization of the buffer.
** It doesn't prevent the application to ignore these uniform objects. But it can also use them
**
** \note as we allow either a \b direct access to the buffer as well as an access
** with the help of IUniform for sub-uniforms, there could be some conflicts :
** if the application mapped the buffer for updating data, the IPass::execute()
** may also trigger access it, to update \b dirty sub-uniforms, if needed (app changed values).
** The best way to use cst buffers : to choose which way to update data.
** Avoid mixing both approaches...
**/ /*************************************************************************/ 
class ICstBuffer : public IBaseInterface<ICstBufferEx>
{
protected:
    ICstBuffer() {}
public:
	virtual ~ICstBuffer() {}
    virtual IAnnotation *annotations() = 0;
    virtual const char* getName() = 0;
    /// \name methods when we assign an existing buffer created from outside
    /// @{
    /// \brief assign a buffer ID to the uniform. Typically used for UBO in OpenGL
    virtual ICstBuffer* setGLBuffer(int buffer) = 0;
    /// \brief assign a buffer ptr to the uniform. Typically used for Constant Buffer
    virtual ICstBuffer* setD3DBuffer(void *buffer) = 0;
    /// \brief returns the ID of the assigned GL buffer
    virtual int         getGLBuffer() = 0;
    /// \brief returns the ptr of the assigned interface buffer
    virtual void*       getD3DBuffer() = 0;
    /// @}
    /// \name methods when the Buffer is hosting uniforms
    /// @{
    /// \brief returns a named uniform if exists
    virtual IUniform*   findUniform(const char * name) = 0;
    /// \brief returns a uniform if #i exists
    virtual IUniform*   findUniform(int i) = 0;

	/// \brief almost redundant enums for OpenGL. But here to be self-sufficient
	enum BufferUsageGL
	{
		STREAM_DRAW = 0x88E0,
		STATIC_DRAW = 0x88E4,
		DYNAMIC_DRAW = 0x88E8,
	};
    // see later if we need other deleteUniform() etc.
    /// \brief (\b Invokes \b GFX \b API) create the OpenGL resource : the UBO that will be connected to the programs.
    /// This is an optional feature : when one wants to rely on nvFX to create the resource.
    /// This method can only work if sub-uniforms are in the object : to allow to guess the buffer size
    virtual int         buildGLBuffer(BufferUsageGL usage = STATIC_DRAW, int sizeMultiplier=1) { return 0; }

	/// \brief same as in D3D. But added it here for the argument of buildD3DBuffer
	enum BufferUsageD3D { 
	  D3D_USAGE_DEFAULT    = 0,
	  D3D_USAGE_IMMUTABLE  = 1,
	  D3D_USAGE_DYNAMIC    = 2,
	};
	/// \brief (\b Invokes \b GFX \b API) create the D3D resource : the Constant Buffer that will be connected to the programs.
    /// This is an optional feature : when one wants to rely on nvFX to create the resource.
    /// This method can only work if sub-uniforms are in the object : to allow to guess the buffer size
	virtual void*       buildD3DBuffer(BufferUsageD3D usage = D3D_USAGE_IMMUTABLE, int sizeMultiplier=1) { return NULL; }
	/// \brief way to change the offset of the attached buffer to the cbuffer/uniform block. Allows fast change (materials...)
	/// This feature is only valid if the buffer creation (buildGLBuffer/buildD3DBuffer) has sizeMultiplier > 1
	virtual bool		offsetBufferBlock(int n) = 0;
	virtual int			getOffsetBufferBlock() = 0;
    /// @}
    /// \brief (\b Invokes \b GFX \b API) returns a temporary pointer of the mapped aread of the buffer \b and \first : it updates the areas from uniforms if needed
    virtual bool        mapBuffer(void** ppData, bool bForceUpdating=false) = 0;
    /// \brief (\b Invokes \b GFX \b API) unmaps the buffer
    virtual void        unmapBuffer() = 0;
    /// \brief updates the Constant buffer : dirty sub-uniforms will be copied to the buffer (OGL/D3D...) associated with this buffer object. return true if succeeded.
    /// This is typically needed when you changed some uniform values of a buffer after you executed the pass : you must propagate these changes
    virtual bool        update() = 0;
};
/*************************************************************************/ /**
** \brief IUniform Parameter interface
**/ /*************************************************************************/ 
class IUniform : public IBaseInterface<IUniformEx>
{
protected:
    IUniform() {}
public:
	virtual ~IUniform() {}
    enum Type {
        TUndefined = 0,
        TInt, TInt2, TInt3, TInt4,
        //UInt,
        TBool, TBool2, TBool3, TBool4,
        TFloat, TVec2, TVec3, TVec4,
        TMat2, TMat3, TMat4, 
        TUBO, // Uniform Buffer Object
        TCB,  // Constant Buffer (D3D)
        TUniform,
        TTexture, // a generic texture : for example when the app created this uniform to bind a texture to a sampler
        // Note: texture==sampler in this type list. D3D uses texture objects, OpenGL will use samplers...
        // Those typed texture/GL-samplers are when the parser encoutered an external declaration of uniform : it needs to know the type
        TTexture1D,
        TTexture2D,
        TTexture2DRect,
        TTexture3D,
        TTextureCube,
        TTexUnit, //TODO: check about this... used for the Cg case (can't assign texunit)
        // TODO : add missing cases
#ifndef OGLES2
        TSubroutineUniform,
        TSubroutineUniformByID,
#endif
    };
    enum PrecisionType /// precision is only used for OpenGL ES
    {
        PHighP = 0,
        PMediumP = 1,
        PLowP = 2,
    };

    virtual IAnnotation *annotations() = 0;
    virtual const char* getGroupName() = 0; ///< optional information when the uniform got created by "Introspec" group
    virtual const char* getSemantic() = 0; ///< optional information when the uniform got created by "Introspec" group
    virtual const char* getName() = 0;
    virtual IUniform::Type getType() = 0;
    virtual PrecisionType getPrecision() = 0;
    virtual int         getArraySz() = 0;
    /// \name methods to assign values to shadowed area of the IUniform
    /// \remark When setting values, we automatically assign a type to the uniform...
    /// all return IUniform* so we can "cascade" calls
    ///@{
    virtual IUniform*    setValue1f(float f) = 0;
    virtual IUniform*    setValue2f(float f1, float f2) = 0;
    virtual IUniform*    setValue3f(float f1, float f2, float f3) = 0;
    virtual IUniform*    setValue4f(float f1, float f2, float f3, float f4) = 0;
    virtual IUniform*    setValue1fv(float *pf) = 0;
    virtual IUniform*    setValue2fv(float *pf) = 0;
    virtual IUniform*    setValue3fv(float *pf) = 0;
    virtual IUniform*    setValue4fv(float *pf) = 0;
    /// Set an array of values/vectors. Note that the final size will be from the *last* update call : nVecs + offsetVec
    virtual IUniform*    setValuefv(float *pf, int dim, int nVecs=1, int offsetVec=0) = 0;

    virtual IUniform*    setValue1i(int f) = 0;
    virtual IUniform*    setValue2i(int f1, int f2) = 0;
    virtual IUniform*    setValue3i(int f1, int f2, int f3) = 0;
    virtual IUniform*    setValue4i(int f1, int f2, int f3, int f4) = 0;
    virtual IUniform*    setValue1iv(int *pf) = 0;
    virtual IUniform*    setValue2iv(int *pf) = 0;
    virtual IUniform*    setValue3iv(int *pf) = 0;
    virtual IUniform*    setValue4iv(int *pf) = 0;
    /// Set an array of values/vectors. Note that the final size will be from the *last* update call : nVecs + offsetVec
    virtual IUniform*    setValueiv(int *pf, int dim, int nVecs=1, int offsetVec=0) = 0;

    virtual IUniform*    setValue1b(bool f) = 0;
    virtual IUniform*    setValue2b(bool f1, bool f2) = 0;
    virtual IUniform*    setValue3b(bool f1, bool f2, bool f3) = 0;
    virtual IUniform*    setValue4b(bool f1, bool f2, bool f3, bool f4) = 0;
    virtual IUniform*    setValue1bv(bool *pf) = 0;
    virtual IUniform*    setValue2bv(bool *pf) = 0;
    virtual IUniform*    setValue3bv(bool *pf) = 0;
    virtual IUniform*    setValue4bv(bool *pf) = 0;
    /// Set an array of values/vectors. Note that the final size will be from the *last* update call : nVecs + offsetVec
    virtual IUniform*    setValuebv(bool *pf, int dim, int nVecs=1, int offsetVec=0) = 0;

    virtual IUniform*    setMatrix4f(float *pm) = 0;
    // TODO: add missing cases
    // TODO: add getXXX() ?
    /// \brief assign a buffer ID to the uniform. Typically used for UBO in OpenGL
    virtual IUniform*    setBuffer(int buffer) = 0;
    /// \brief assign a buffer ptr to the uniform. Typically used for Constant Buffer in OpenGL
    virtual IUniform*    setBuffer(void *buffer) = 0;
    /// \name Methods to deal with textures samplers (GLSL)
    /// @{
    /// \brief assign a sampler-state object to a sampler uniform
    virtual IUniform*    setSamplerState(ISamplerState *pSState) = 0;
    /// \brief set the sampler texture Unit. Cg cannot do it... automatically done
    virtual IUniform*    setSamplerUnit(int i) = 0;
    /// \brief directly set a GL texture ID to the sampler. \arg texUnit>=0 is like callking setSamplerUnit()
    virtual IUniform*    setSamplerResource(ResourceType resType, int pOglTexID, int texUnit=-1, ISamplerState *pSState=NULL) = 0;
    /// \brief Associate a nvFX::IResource object to the sampler (OGL)
    virtual IUniform*    setSamplerResource(IResource* pRes, int texUnit=-1, ISamplerState *pSState=NULL) = 0;
    /// \brief Associate a nvFX::IResource object to the texture (D3D)
    virtual IUniform*    setTextureResource(IResource* pRes) = 0;
    /// @}
    /// \name Methods to deal with image Load/Store features (ARB_image_load_store)... should work with DX equivalent (TODO)
    /// @{
    /// \brief set the image texture unit for image Load/Store in shaders
    virtual IUniform*    setImageUnit(int i) = 0;
    /// \brief directly set a GL texture ID to the sampler. \arg texUnit>=0 is like callking setSamplerUnit()
    /// \todo change optional arguments to pointer. When NULL, clearly means that they are not used
    virtual IUniform*    setImageResource(ResourceType resType, int oglTexID, int texUnit=-1) = 0;
    /// \brief Associate a nvFX::IResource object to the sampler
    virtual IUniform*    setImageResource(IResource* pRes, int texUnit=-1) = 0;
    /// @}
    /// \brief \e Associates a uniform (texture) to the uniform (sampler)
    virtual IUniform*    setUniform(IUniform* pU) = 0;
    /// \brief sets a uniform from the values of another one
    virtual IUniform*    setFromUniform(IUniform* pU) = 0;
    /// \brief assign a subroutine to a uniform variable
#ifndef OGLES2
    virtual IUniform*    setSubroutine(const char *funcName, int idx) = 0;
    virtual IUniform*    setSubroutines(const char **funcNames, int numFuncNames) = 0;
#endif
    ///@}
    /// \name methods to get values from shadowed area of the IUniform
    ///@{
    virtual bool         getValuefv(float *pf, int dim, int nVecs=1) = 0;
    virtual bool         getValueiv(int *pf, int dim, int nVecs=1) = 0;
    virtual bool         getValuebv(bool *pf, int dim, int nVecs=1) = 0;
    virtual bool         getValueRaw(void *, int szMax=0, int bytesPadding=0) = 0; // Raw copy of what is shadowed in the uniform
    ///@}
    /// \name methods to directly assign values to targets without the use of shadowed area
    /// When using these methods, you must be aware that the values won't be persistent
    /// in the unfiform class, as it would when using setXXX().
    ///
    /// \b All \b Invoke \b GFX \b API
    ///
    /// all return IUniform* so we can "cascade" calls
    ///@{
    virtual IUniform*    updateValue1f(float f, IPass *pass, bool bBindProgram=false) = 0;
    virtual IUniform*    updateValue2f(float f1, float f2, IPass *pass, bool bBindProgram=false) = 0;
    virtual IUniform*    updateValue3f(float f1, float f2, float f3, IPass *pass, bool bBindProgram=false) = 0;
    virtual IUniform*    updateValue4f(float f1, float f2, float f3, float f4, IPass *pass, bool bBindProgram=false) = 0;
    virtual IUniform*    updateValue1fv(float *pf, IPass *pass, bool bBindProgram=false) = 0;
    virtual IUniform*    updateValue2fv(float *pf, IPass *pass, bool bBindProgram=false) = 0;
    virtual IUniform*    updateValue3fv(float *pf, IPass *pass, bool bBindProgram=false) = 0;
    virtual IUniform*    updateValue4fv(float *pf, IPass *pass, bool bBindProgram=false) = 0;
    virtual IUniform*    updateValuefv(float *pf, int dim, IPass *pass, bool bBindProgram=false) = 0;

    virtual IUniform*    updateValue1i(int f, IPass *pass, bool bBindProgram=false) = 0;
    virtual IUniform*    updateValue2i(int f1, int f2, IPass *pass, bool bBindProgram=false) = 0;
    virtual IUniform*    updateValue3i(int f1, int f2, int f3, IPass *pass, bool bBindProgram=false) = 0;
    virtual IUniform*    updateValue4i(int f1, int f2, int f3, int f4, IPass *pass, bool bBindProgram=false) = 0;
    virtual IUniform*    updateValue1iv(int *pf, IPass *pass, bool bBindProgram=false) = 0;
    virtual IUniform*    updateValue2iv(int *pf, IPass *pass, bool bBindProgram=false) = 0;
    virtual IUniform*    updateValue3iv(int *pf, IPass *pass, bool bBindProgram=false) = 0;
    virtual IUniform*    updateValue4iv(int *pf, IPass *pass, bool bBindProgram=false) = 0;
    virtual IUniform*    updateValueiv(int *pf, int dim, IPass *pass, bool bBindProgram=false) = 0;

    virtual IUniform*    updateValue1b(bool f, IPass *pass, bool bBindProgram=false) = 0;
    virtual IUniform*    updateValue2b(bool f1, bool f2, IPass *pass, bool bBindProgram=false) = 0;
    virtual IUniform*    updateValue3b(bool f1, bool f2, bool f3, IPass *pass, bool bBindProgram=false) = 0;
    virtual IUniform*    updateValue4b(bool f1, bool f2, bool f3, bool f4, IPass *pass, bool bBindProgram=false) = 0;
    virtual IUniform*    updateValue1bv(bool *pf, IPass *pass, bool bBindProgram=false) = 0;
    virtual IUniform*    updateValue2bv(bool *pf, IPass *pass, bool bBindProgram=false) = 0;
    virtual IUniform*    updateValue3bv(bool *pf, IPass *pass, bool bBindProgram=false) = 0;
    virtual IUniform*    updateValue4bv(bool *pf, IPass *pass, bool bBindProgram=false) = 0;
    virtual IUniform*    updateValuebv(bool *pf, int dim, IPass *pass, bool bBindProgram=false) = 0;

    virtual IUniform*    updateMatrix4f(float *pm, IPass *pass, bool bBindProgram=false) = 0;
    virtual IUniform*    updateSamplerState(ISamplerState *pSState, IPass *pass, bool bBindProgram=false) = 0;
    /// \name Used for image Load Store
    /// @{
    virtual IUniform*    updateImageUnit(int i, IPass *pass = NULL, bool bBindProgram=false) = 0;
    virtual IUniform*    updateImageResource(ResourceType resType, int oglTexID, int texUnit=-1, IPass *pass = NULL, bool bBindProgram=false) = 0;
    virtual IUniform*    updateImageResource(IResource* pRes, int texUnit=-1, IPass *pass = NULL, bool bBindProgram=false) = 0;
    /// @}
    /// \name Used for samplers (OpenGL)
    /// @{
    virtual IUniform*    updateSamplerUnit(int i, IPass *pass, bool bBindProgram=false) = 0;
    virtual IUniform*    updateSamplerResource(ResourceType resType, int oglTexID, int texUnit=-1, ISamplerState *pSState=NULL, IPass *pass = NULL, bool bBindProgram=false) = 0;
    virtual IUniform*    updateSamplerResource(IResource* pRes, int texUnit=-1, ISamplerState *pSState=NULL, IPass *pass = NULL, bool bBindProgram=false) = 0;
    /// @}
    virtual IUniform*    updateBuffer(int buffer, IPass *pass, bool bBindProgram=false) = 0;
    virtual IUniform*    updateBuffer(void* buffer, IPass *pass, bool bBindProgram=false) = 0;
    virtual IUniform*    updateUniform(IUniform* pU, IPass *pass, bool bBindProgram=false) = 0;
#ifndef OGLES2
    /// \brief (\b Invokes \b GFX \b API) update a subroutine to a uniform variable by using sub-routine name
    virtual IUniform*    updateSubroutine(const char *funcName, int idx, IPass *pass, bool bBindProgram=false) = 0;
    /// \brief (\b Invokes \b GFX \b API) update many subroutine to a uniform variable by using sub-routine names
    virtual IUniform*    updateSubroutines(const char **funcNames, IPass *pass, bool bBindProgram=false) = 0;
#endif
    // TODO: add missing cases
    ///@}
    /// \name target update and mgt
    /// @{
    /// \brief (\b Invokes \b GFX \b API) update targets with the shadowed values
    virtual IUniform*    update(IPass *pass = NULL, bool bBindProgram=false) = 0;
    /// \brief return # of targets available for this uniform
    virtual int         getNumTargets(TargetType t=TANY) = 0;
    /// @}
};
/*************************************************************************/ /**
 ** \brief interface for Rasterization states
 **
 ** \todo Add the states of D3D10&11
 **/ /*************************************************************************/ 
class IStateGroupRaster : public IBaseInterface<IStateGroupRasterEx>
{
public:
	virtual ~IStateGroupRaster() {}
    struct State
    {
        GLenum state;
        union {
            GLenum values[2];
            float fvals[2];
        };
        State(GLenum s) {state=s; }
        State(GLenum s, GLenum v) {state=s; values[0]=v; }
        State(GLenum s, GLenum v1, GLenum v2) {state=s; values[0]=v1; values[1]=v2; }
        State(GLenum s, float v) {state=s; fvals[0]=v; }
    };
    virtual IAnnotation *annotations() = 0;
    virtual const char*     getName() = 0;
    virtual State*          findState(GLenum state) = 0;
    virtual State*          getState(int i) = 0;
    virtual void            apply() = 0; ///< \brief (\b Invokes \b GFX \b API) apply the states : send them to the Graphic API
};

/*************************************************************************/ /**
 ** \brief interface for Color samples states
 ** 
 ** \todo Add the states of D3D10&11
 **/ /*************************************************************************/ 
class IStateGroupCS : public IBaseInterface<IStateGroupCSEx>
{
public:
	virtual ~IStateGroupCS() {}
    struct State
    {
        GLenum state;
        union {
            struct { GLenum values[4]; float fvalue; };
            GLboolean       bvals[4];
            float           fvals[4];
        };
        State(GLenum s) {state=s; }
        State(GLenum s, GLenum v) {state=s; values[0]=v; }
        State(GLenum s, GLenum x, GLenum y, GLenum z, GLenum w)
            {state=s; values[0]=x; values[1]=y; values[2]=z; values[3]=w; }
        State(GLenum s, float x, float y, float z, float w)
            {state=s; fvals[0]=x; fvals[1]=y; fvals[2]=z; fvals[3]=w; }
        State(GLenum s, GLboolean x, GLboolean y, GLboolean z, GLboolean w)
            {state=s; bvals[0]=x; bvals[1]=y; bvals[2]=z; bvals[3]=w; }
    };

    virtual IAnnotation *   annotations() = 0;
    virtual const char*     getName() = 0;
    virtual State*          findState(GLenum state) = 0;
    virtual State*          getState(int i) = 0;
    virtual void            apply() = 0; ///< \brief apply the states : send them to the Graphic API
};

/*************************************************************************/ /**
 ** \brief interface for Depth Stencil States
 ** 
 ** \todo Add the states of D3D10&11
 **/ /*************************************************************************/ 
class IStateGroupDST : public IBaseInterface<IStateGroupDSTEx>
{
public:
	virtual ~IStateGroupDST() {}
    struct State
    {
        GLenum state;
        union {
            struct { GLenum value, values[2]; };
            float           fvals[2];
        };
        State(GLenum s) {state=s; }
        State(GLenum s, GLenum v) {state=s; value=v; }
        State(GLenum s, float v1, float v2) {state=s; fvals[0]=v1; fvals[1]=v2; }
    };

    virtual IAnnotation *   annotations() = 0;
    virtual const char*     getName() = 0;
    virtual State*          findState(GLenum state) = 0;
    virtual State*          getState(int i) = 0;
    virtual void            apply() = 0; ///< \brief apply the states : send them to the Graphic API
};

#ifndef OGLES2
/*************************************************************************/ /**
 ** \brief interface for NV Path-rendering states. OpenGL exclusive...
 ** although state groups don't exist in the driver, it is cleaner to do so
 **/ /*************************************************************************/ 
class IStateGroupPath : public IBaseInterface<IStateGroupPathEx>
{
public:
	virtual ~IStateGroupPath() {}
    struct State
    {
        GLenum state;
        union {
            GLenum  value;
            float   fval[2];
            int     ival;
        };
        State(GLenum s, GLenum v)   { state=s; value=v; }
        State(GLenum s, float v)    { state=s; fval[0]=v; }
        State(GLenum s, int v)      { state=s; ival=v; }
        State(GLenum s, float f, float u)      { state=s; fval[0]=f; fval[1]=u; }
    };

    virtual IAnnotation *   annotations() = 0;
    virtual const char*     getName() = 0;
    virtual State*          findState(GLenum state) = 0;
    virtual void            apply(IPass *pass, PassInfo * pr, int n=0) = 0; ///< \brief apply the states : send them to the Graphic API
};
#endif
/*************************************************************************/ /**
 ** \brief interface to control the sampler-states created from nvFX::IContainer
 ** 
 ** In OpenGL, this object is purely nvFX. Nothing specific on OpenGL corresponds
 ** this it. However in D3D, this object directly maps (and creates) to SamplerState.
 ** In this case, ISamplerState takes care of the D3D object creation. The reason:
 ** Sampler state details essentially come from nvFX file, more than from the app.
 ** This is different from the constant buffers and textures : more on app side.
 **/ /*************************************************************************/ 
class ISamplerState : public IBaseInterface<ISamplerStateEx>
{
public:
	virtual ~ISamplerState() {}
    struct State
    {
        GLenum sstate;
        GLenum value;
        State(GLenum s, GLenum v) {sstate=s; value=v; }
    };

    virtual IAnnotation *   annotations() = 0;
    virtual const char*     getName() = 0;
    /// \name D3D-specific
    /// @{
    virtual bool    setD3DSamplerState(void *p) = 0; ///< \brief instead of creating states inside this object, we can directly assign an existing sampler-state from the API
    virtual void*   getD3DSamplerState() = 0;
    //virtual bool    updateD3DSamplerState(void *p) = 0; ///< \brief binds a samplerstate interface directly to the device, without keeping it in this object
    /// @}
    /// \name OpenGL-specific
    /// @{
    virtual void    updateGLTexture(GLenum target, GLint tex, bool bindTexture=false) = 0;
    /// @}
};

/*************************************************************************/ /**
 ** \brief the main interface container. Can be seen as the equivalent of a context
 ** 
 **/ /*************************************************************************/ 
class IContainer : public IBaseInterface<IContainerEx>
{
public:
	virtual ~IContainer() {}
    virtual IAnnotation *   annotations() = 0;
    /// \brief set/get container name
    virtual const char*     getName() = 0;
    /// \brief set the container name : trims suffix if any
    virtual void            setName(const char*name) = 0;
    /// @}
    /// \name Resource Specific
    /// @{
    /// \brief looks for a resource from its name
    virtual IResource*          findResource(const char * name) = 0;
    /// \brief gets a resource from its indice in the table of resources
    virtual IResource*          findResource(int i) = 0;
    virtual int              gatherResourcesFromAnnotation(IResource** &ppResources, const char *annotationName, const char* value=NULL) = 0;
    virtual int              gatherResourcesFromAnnotation(IResource** &ppResources, const char *annotationName, int value) = 0;
    virtual int              gatherResourcesFromAnnotation(IResource** &ppResources, const char *annotationName, float value) = 0;
    virtual int              gatherResourcesFromNamespace(IResource** &ppResources, const char *ns) = 0;
    virtual IFrameBufferObject* findFbo(const char * name) = 0;
    virtual int              gatherFbosFromAnnotation(IFrameBufferObject** &ppFbos, const char *annotationName, const char* value=NULL) = 0;
    virtual int              gatherFbosFromAnnotation(IFrameBufferObject** &ppFbos, const char *annotationName, int value) = 0;
    virtual int              gatherFbosFromAnnotation(IFrameBufferObject** &ppFbos, const char *annotationName, float value) = 0;
    virtual int              gatherFbosFromNamespace(IFrameBufferObject** &ppFbos, const char *ns) = 0;
    /// @}
    /// \name Shader specific
    /// @{
    virtual IShader*         findShader(const char * name, TargetType t=TANY) = 0;
    virtual IShader*         findShader(int i) = 0;
    virtual int              gatherShadersFromAnnotation(IShader** &ppShaders, const char *annotationName, const char* value=NULL) = 0;
    virtual int              gatherShadersFromAnnotation(IShader** &ppShaders, const char *annotationName, int value) = 0;
    virtual int              gatherShadersFromAnnotation(IShader** &ppShaders, const char *annotationName, float value) = 0;
    virtual int              gatherShadersFromNamespace(IShader** &ppShaders, const char *ns) = 0;
    /// @}
    /// \name find methods
    /// @{
    /// \brief finds any sort of things
    virtual int                  getNumTechniques() = 0;
    virtual ITechnique*          findTechnique(const char * name) = 0;
    virtual ITechnique*          findTechnique(int i) = 0;
    virtual ISamplerState*       findSamplerState(const char * name) = 0;
    virtual ISamplerState*       findSamplerState(int i) = 0;
    virtual IStateGroupRaster*   findStateGroupRaster(const char * name) = 0;
    virtual IStateGroupRaster*   findStateGroupRaster(int i) = 0;
    virtual IStateGroupCS*       findStateGroupCS(const char * name) = 0;
    virtual IStateGroupCS*       findStateGroupCS(int i) = 0;
    virtual IStateGroupDST*      findStateGroupDST(const char * name) = 0;
    virtual IStateGroupDST*      findStateGroupDST(int i) = 0;
#ifndef OGLES2
    virtual IStateGroupPath*     findStateGroupPR(const char * name) = 0;
    virtual IStateGroupPath*     findStateGroupPR(int i) = 0;
#endif
    virtual IUniform*            findUniform(const char * name) = 0;
    virtual IUniform*            findUniform(int i) = 0;
    virtual int                  gatherUniformsFromAnnotation(IUniform**&pp, const char *annotationName, const char* value=NULL) = 0;
    virtual int                  gatherUniformsFromAnnotation(IUniform**&pp, const char *annotationName, int value) = 0;
    virtual int                  gatherUniformsFromAnnotation(IUniform**&pp, const char *annotationName, float value) = 0;
    virtual int                  gatherUniformsFromNamespace(IUniform**&pp, const char *ns) = 0;

    virtual ICstBuffer*          findCstBuffer(const char * name) = 0;
    virtual ICstBuffer*          findCstBuffer(int i) = 0;
    virtual int                  gatherCstBuffersFromAnnotation(ICstBuffer**&pp, const char *annotationName, const char* value=NULL) = 0;
    virtual int                  gatherCstBuffersFromAnnotation(ICstBuffer**&pp, const char *annotationName, int value) = 0;
    virtual int                  gatherCstBuffersFromAnnotation(ICstBuffer**&pp, const char *annotationName, float value) = 0;
    virtual int                  gatherCstBuffersFromNamespace(ICstBuffer**&pp, const char *ns) = 0;
    /// @}
    /// \name creation methods
    /// @{
    /// \brief pD is a possible device pointer (in D3D). see nvFX::IContainer::setDevice, too
    static __declspec( dllexport ) IContainer*          create(const char *name = NULL);
    /// @}
    /// \name destruction methods
    /// @{
    static bool     destroy(IContainer* p);
    virtual bool    destroy() = 0; ///< destroy the container itself
    /// @}
    // \brief optimize the programs/shaders being used : remove useless shaders from the effect, after releaseOverrides(), for example
    virtual void                consolidateShaders() = 0;
};
/*************************************************************************/ /**
 ** \brief interface used to drive a technique created by nvFX::IContainer
 ** 
 **/ /*************************************************************************/ 
class ITechnique : public IBaseInterface<ITechniqueEx>
{
public:
	virtual ~ITechnique() {}
    virtual IAnnotation *   annotations() = 0;
    virtual const char*     getName() = 0;
    /// \name pass access
    /// @{
    virtual int             getNumPasses() = 0;
    /// \brief tells if the pass is active or not
    virtual bool            isPassActive(int n) = 0;
    /// \brief return the pass # i
    /// \Note when sub-techniques are being used, the walkthrough includes the passes of the sub-techniques
    /// \arg \e pActive optionally to receive the status of the pass, if active or not
    /// \arg \e pLoopCnt optionally to receive the amount of times to repeat this pass
    /// \Note maybe we could instead use nvFX::PassInfo : getPass could fill-in the 2 fields
    //  in this struct, and even clear the rest. This could allows more flexible use of nvFX::PassInfo through
    //  nvFX::IPass::execute
    virtual IPass*           getPass(int i, PassInfo *pPassInfo=NULL) = 0;
    /// @}
    /// \brief (\b Invokes \b GFX \b API) validates the technique and sub-sequent passes
    virtual bool            validate() = 0;
    /// \brief invalidates the technique and sub-sequent passes. Can be needed if we messed-up with a program or anything shared with this
    virtual bool            invalidate() = 0;
    /// \brief (\b Invokes \b GFX \b API) allows to map the attributes of a vertex shader to *all* the sub-sequence passes
    virtual bool            bindAttribute(const char * name, int unit) = 0;
    /// \brief (\b Invokes \b GFX \b API) binds mapped attributes to the program (triggers a linkage) to *all* the sub-sequence passes
    virtual bool            bindAttributes() = 0;
    /// \brief erase all mapped attributes of *all* the sub-sequence passes
    virtual void            clearAttributeBinding() = 0;
    /// \brief returns the string name of a specific attribute #. NULL if nothing assigned
    virtual const char *    getAttributeNameForLocation(int loc) = 0;
    /// \brief returns # of associated attributes
    virtual int             getNumBoundAttributes() = 0;
    /// \brief set the active program layer
    virtual void            setActiveProgramLayer(int id, bool createIfNA=false) = 0;
    virtual int             getActiveProgramLayer() = 0;
    virtual void            removeProgramLayer(int id) = 0;
};

/*************************************************************************/ /**
 ** \brief interface for pass-states within a IPass
 ** \remark the pass states can accept the states defined for
 ** IStateGroupRaster, IStateGroupCS and IStateGroupDST
 **/ /*************************************************************************/ 
class IPassState : public IBaseInterface<IPassStateEx>
{
public:
	virtual ~IPassState() {}
    /// \brief type of pass-state
    /// \todo add more types for more shader units etc.
    enum Type
    {
        TUniform, ///< a uniform parameter assignment from the pass
        TStateGroup_Raster, ///< a state group assignment
        TStateGroup_CS, ///< a state-group assignment
        TStateGroup_DST, ///< a state-group assignment
        TStateGroup_PR, ///< a state-group assignment for path rendering
        TVertexShader, ///< a vertex shader assignment
        TFragmentShader, ///< a fragment shader assignment
        TGeometryShader, ///< a geom shader assignment
        TTCSShader, ///< a Tess ctrl point shader (== D3D Hull)
        TTESShader, ///< a Tess evaluation shader
        TComputeShader, ///< a compute shader assignment (GLSL or DX compute)
        TComputeGroup, ///< a compute group dimension (GLSL or DX compute)
#ifndef OGLES2
        TSubRoutineUniform, ///< a uniform subroutine parameter assignment from the pass
#endif
        // Additional type of pass-states
        TRenderMode,
        TRenderGroup,
        TClearMode,
        TClearColor,
        TFBOTarget,
        TBlitFBOSrc,
        TViewport,
        TSwapResource, ///< for FBO+texture resource swapping operation
        TCUDAModule, ///< a CUDA module : some CUDACode/CUDAKernel put together
        TCUDAKernelEntry, ///< defines the CUDA Entry point use for invokation (func name + args vals)
        TCUDAGridSz, ///< size of the Grid for CUDA
        TCUDABlockSz, ///< size of the Block for CUDA
        TCUDAShMemSz, ///< size of the shared memory
        // Path rendering 'states'
        TPathShader, ///< the pass state specifies some vector commands (SVG/PS). No a shader but set of instructions !
        TPathFillMode, ///< typically INVERT, COUNT_UP_NV, COUNT_DOWN_NV or PATH_FILL_MODE_NV
        TPathReference,
        TPathMask,
        TPathTransformType,///< typically TRANSLATE_X_NV etc.
        TPathCoverMode, ///< typically CONVEX_HULL_NV or BOUNDING_BOX_NV
        TPathTransfValues,
    };
    /// \brief for Type::TUniform : various kind of variables can be there
    enum VarType
    {
        Float, ///< float or vector of floats
        Int, ///< int or vector of int's
#ifndef OGLES2
        SubroutineName, ///< a name of a Subroutine implementation
#endif
        Unknown
    };

    virtual const char *getName() = 0;
    virtual Type        getType() = 0;
    virtual IUniform*   getUniform() = 0;
    virtual IShader*    getShader(int idx) = 0;
    // TODO: add some getXXX() for various types of pass-states
};

/*************************************************************************/ /**
 ** \brief interface for the pass of an effect container created by nvFX::ITechnique
 ** 
 **/ /*************************************************************************/ 
class IPass : public IBaseInterface<IPassEx>
{
public:
	virtual ~IPass() {}
    virtual IAnnotation *   annotations() = 0;
    /// \brief returns the pass name
    virtual const char*     getName() = 0;
    /// \name validation/execution
    /// @{
    /// \brief (\b Invokes \b GFX \b API) validates the pass. It will create the GLSL/Cg (and others) programs if needed
    virtual bool            validate() = 0;
    /// \brief creates in \e dest techniques passes additional pass states from \e override states
    virtual bool            setupOverrides(ITechnique **dest, int numPasses) = 0;
    /// \brief creates in \e dest passes additional pass states from \e override states
    virtual bool            setupOverrides(IPass **dest, int numPasses) = 0;
    /// \brief removes a specific override
    virtual bool            releaseOverrides(ITechnique **dest, int numTechs) = 0;
    /// \brief removes a specific override
    virtual bool            releaseOverrides(IPass **dest, int numPasses) = 0;
    /// \brief validates the pass. It will create the GLSL/Cg (and others) programs if needed
    virtual bool            invalidate() = 0;
    /// \brief (\b Invokes \b GFX \b API) executes the states of this pass : [GLSL] program and other states (render states)...
    /// \todo \e PassInfo would be a ptr to a struct in which the requests from the pass would be put (scene-level effect)
    /// \arg \e cancelInternalAction allows you to cancel some implicit actions that execute() would perform. See nvFX::PassInfo::flags
    virtual bool            execute(PassInfo * pr=NULL, unsigned int cancelInternalAction=0) = 0;
    /// \brief disable the program that is used in this pass
    /// @}
    /// \brief finds a state by its group name. Typically used for groups of shaders
    virtual IPassState*      findState(const char *groupName, int layerID=0) = 0;
    /// \brief finds a state-override by its group name. Typically used for groups of shaders
    virtual IPassState*      findStateOverride(const char *groupName) = 0;
    /// \brief disable the potential program that would be here for the pass
    virtual void            unbindProgram() = 0;
    /// \brief (\b Invokes \b GFX \b API) allows to map the attributes of a vertex shader
    virtual bool            bindAttribute(const char * name, int unit) = 0;
    /// \brief (\b Invokes \b GFX \b API) binds mapped attributes to the program (triggers a linkage)
    virtual bool            bindAttributes() = 0;
    /// \brief erase all mapped attributes
    virtual void            clearAttributeBinding() = 0;
    /// \brief returns the string name of a specific attribute #. NULL if nothing assigned
    virtual const char *    getAttributeNameForLocation(int loc) = 0;
    /// \brief returns # of associated attributes
    virtual int             getNumBoundAttributes() = 0;
    /// \brief tells if the pass got validated or not
    virtual bool            isValidated() = 0;
    /// \brief tells how many times this pass should be repeated (Loop)
    virtual void            setLoopCount(int cnt) = 0;
    virtual int             getLoopCount() = 0;
    /// \brief tells if this pass is active or not
    virtual void            setActive(bool bYes=true) = 0;
    virtual bool            getActive() = 0;
    /// \name D3D-specific methods
    /// @{
    virtual void* getD3DIASignature() = 0; ///< \brief (\b Invokes \b GFX \b API) returns the Input attribute signature (in fact : this is the vertex shader compiled Blob). Same as the one in IProgram
    virtual int getD3DIASignatureSize() = 0; ///< \brief (\b Invokes \b GFX \b API) returns the Input attribute signature size (in fact : this is the vertex shader compiled Blob size). Same as the one in IProgram
    /// @}
};

/*************************************************************************/ /**
 ** \brief the Shader interface
 ** NOTE: a shader must be in a domain (vertex/frag...)
 ** - If we want to share it between domains : must be created many times in each
 ** - data must be consistent
 ** - we can concatenate many shaders together
 ** - we can prototype some functions : linkage will choose the right implementation
 ** - we need to differenciate between code chunk to copy and code chunk to link
 ** - add a name at creation time
 ** 
 **/ /*************************************************************************/ 
class IShader : public IBaseInterface<IShaderEx>
{
public:
	virtual ~IShader() {}
    virtual const char* getName() = 0;
    virtual TargetType  getType() = 0;
    virtual const char* getShaderCode() = 0;
    virtual int         getShaderCodeSz() = 0;
    virtual IAnnotation *annotations() = 0;
};
/*************************************************************************/ /**
 ** \brief 
 ** 
 ** 
 **/ /*************************************************************************/ 
class IProgramPipeline : public IBaseInterface<IProgramPipelineEx>
{
public:
	virtual ~IProgramPipeline() {}
    virtual /*IShaderProgram*/IProgram* getShaderProgram(int i) = 0;
    virtual int                         getProgramShaderFlags() = 0;
};
//
// corresponds to OpenGL's GL_... equivalent. For getProgramShaderFlags()
//
#define FX_VERTEX_SHADER_BIT            0x00000001
#define FX_FRAGMENT_SHADER_BIT          0x00000002
#define FX_GEOMETRY_SHADER_BIT          0x00000004
#define FX_TESS_CONTROL_SHADER_BIT      0x00000008
#define FX_TESS_EVALUATION_SHADER_BIT   0x00000010
#define FX_COMPUTE_SHADER_BIT           0x00000020
#define FX_PATH_CODE_BIT                0x00000040
#define FX_ALL_SHADER_BITS              0xFFFFFFFF
/*************************************************************************/ /**
 ** \brief the program interface
 ** 
 ** Program is a GLSL concept : after linkage of all the involved shaders
 ** (all GPU stages together), the program can be activated for rendering
 **
 ** \note in D3D, this has no meaning. We will thus use it as a container for 
 ** all the possible shaders involved in the execution of the pass which owns it
 **/ /*************************************************************************/ 
class IProgram : public IBaseInterface<IProgramEx>
{
public:
	virtual ~IProgram() {}
    //TODO: geom, tess...
    virtual IShader*    getShader(int n, ShaderType *t = NULL) = 0; ///< \brief 
    virtual IShader*    getShader(IShader *pShader, ShaderType *t = NULL) = 0; ///< \brief 
    virtual IShader*    getShader(const char *name, ShaderType *t = NULL) = 0; ///< \brief 
    virtual int         getNumShaders() = 0; ///< \brief 

    virtual int         getProgram() = 0;
    virtual int         getProgramShaderFlags() = 0; ///< flags for Separate shader in OpenGL. See http://www.opengl.org/registry/specs/ARB/separate_shader_objects.txt
    /// \name D3D-specific methods
    /// @{
    virtual void* getD3DIASignature() = 0; ///< \brief (\b Invokes \b GFX \b API)  returns the Input attribute signature (in fact : this is the vertex shader compiled Blob)
    virtual int getD3DIASignatureSize() = 0; ///< \brief (\b Invokes \b GFX \b API)  returns the Input attribute signature size (in fact : this is the vertex shader compiled Blob size)
    /// @}
};

/*************************************************************************/ /**
 ** \brief Annotation interface that can be used by any class
 **
 ** the other class could have a "delegate" of annotations or derive from it.
 ** Then IAnnotation *annotations() should be a method available in the interfaces
 ** of the class that can deliver some annotations. See nvFX::Texture as an example
 ***
 ** \todo Add more methods...
 **/ /*************************************************************************/ 
class IAnnotation : public IBaseInterface<IAnnotationEx>
{
public:
	virtual ~IAnnotation() {}
    virtual const char * getAnnotationString(const char *annotName) = 0;
    virtual float        getAnnotationFloat(const char *annotName) = 0;
    virtual int          getAnnotationInt(const char *annotName) = 0;
};
/*************************************************************************/ /**
 ** \brief texture interface.
 **
 ** - In OpenGL : this doesn't have any real use... No texture object in GLSL. IResource should be good enough
 ** - D3D : it is referencing the Textures of HLSL code. This is a binding point of the GLSL texture, when sampling
 ** 
 ** \remark annotations can be added. For now the least needed it done...
 ** i.e. just strings. Tipical use : default filename for a texture
 **
 **/ /*************************************************************************/ 
/*
PUT IN UNIFORMS THE MISSING FIELDS FOR D3D TO USE UNIFORMS INSTEAD OF TEXTURE
THEN REMOVE THIS CODE
class ITexture : public IBaseInterface<ITextureEx>
{
public:
    virtual const char* getName() = 0;
    /// \brief returns the interface for annotations
    virtual IAnnotation *annotations() = 0;
    /// \name resource assignment
    /// @{
    virtual void        setType(ResourceType type) = 0;///< \brief Texture type, for any API
    virtual ResourceType getType() = 0;///< \brief Texture type, for any API
    /// @}
    /// \name D3D-specific
    /// @{
    virtual void        setD3DResourceView(void *p) = 0; ///< \brief (\b Invokes \b GFX \b API) for example in D3D case : we assign a resource-view interface pointer
    virtual void*       getD3DResourceView() = 0; ///< \brief for example in D3D case : returns the resource-view interface pointer
    /// @}
    /// \name OpenGL-specific
    /// Note that you should use IResource, instead... kept here for compatibility
    /// @{
    virtual void        setGLTexture(int id) = 0; ///< \brief has a meaning for OpenGL only : the texture name (or ID)
    virtual void        setGLTexture(ResourceType type, int id) = 0;///< \brief has a meaning for OpenGL only, GL_TEXTURE_XXX and Texture name (id)
    virtual int         getGLTextureID() = 0;///< \brief returns the OpenGL texture name (id)
    virtual void        setSamplerState(ISamplerState * pSState) = 0;/// \brief attach a sampler state to this texture. Typical OpenGL way to do
    virtual ISamplerState*   getSamplerState() = 0; ///< return the currrent attached sampler-state
    /// @}
    /// \name way to use nvFX resources with a texture object
    /// @{
    virtual void        setResource(IResource *pResource) = 0; ///< \brief Assign a resource interface to the texture. For OpenGL, this doesn't really mean anything (no Texture object in GLSL)
    virtual IResource*  getResource() = 0;
    /// @}
};
*/
/*************************************************************************/ /**
 ** \brief resource interface. Typically used for RenderBuffer and Texture resources
 **
 ** - as a texture resource reference in OpenGL : Texture ID. Use the "OpenGL-specific" methods.
 ** - for D3D : would contain the entry point for resource-view
 ** .
 ** Some methods allow you to create a texture from this class, rather than binding something to it.
 **   this is used essentially by the parser to create a render target resource that got defined in the nvFX file
 ** 
 ** \remark annotations can be added. For now the least needed it done...
 ** i.e. just strings. Tipical use : default filename for a texture
 **
 **/ /*************************************************************************/ 
class IResource : public IBaseInterface<IResourceEx>
{
public:
	virtual ~IResource() {}
    /// \brief returns the interface for annotations
    virtual IAnnotation *annotations() = 0;
    virtual const char* getName() = 0;
    /// \name resource assignment
    /// @{
    /// \brief resource type
    /// \todo change ResourceType with ResourceType ?
    virtual ResourceType getType() = 0;
    /// @}
    /// \name D3D-specific... TODO !
    /// This is a way to associate data to this object for D3D or to simply query the interface pointers
    /// \todo maybe we should allow to return the resource itself (ID3D10Texture2D etc.) and the possible ID3D10RenderTargetView/ID3D10DepthStencilView...
    /// 
    /// @{
    virtual bool        setD3DResourceView(void *p) = 0; ///< \brief (\b Invokes \b GFX \b API) for example in D3D case : we assign a resource-view interface pointer
    virtual void*       getD3DResourceView() = 0; ///< \brief for example in D3D case : returns the resource-view interface pointer
    /// @}
    /// \name OpenGL-specific
    /// This is a way to associate data to this object for OpenGL
    /// @{
    virtual void        setGLTexture(int id) = 0; ///< \brief has a meaning for OpenGL only : the texture name (or ID)
    virtual void        setGLTexture(ResourceType type, int id) = 0;///< \brief has a meaning for OpenGL only, GL_TEXTURE_XXX and Texture name (id)
    virtual int         getGLTextureID() = 0;///< \brief returns the OpenGL texture name (id)
    virtual ISamplerState*   getSamplerState() = 0; ///< return the currrent attached sampler-state
    virtual void        setSamplerState(ISamplerState * pSState) = 0;/// \brief attach a sampler state to this texture. Typical OpenGL way to do
    /// @}
    virtual int         getWidth() = 0;
    virtual int         getHeight() = 0;
    virtual bool        getDimensionsApplicationDependent() = 0;
    virtual void        getDimensionFactor(float &w,float &h) = 0;
    virtual void        getMSAA(int &d, int &s) = 0;
    virtual bool        getMSAAApplicationDependent() = 0;
    virtual IResource*  getResolveTarget() = 0;
};

/*************************************************************************/ /**
 ** \brief Frame buffer Object binding point (FBO in OpenGL), gathering many render targets : textures or buffers
 **
 **
 **/ /*************************************************************************/ 
class IFrameBufferObject : public IBaseInterface<IFrameBufferObjectEx>
{
public:
	virtual ~IFrameBufferObject() {}
    /// \brief returns the interface for annotations
    virtual IAnnotation*    annotations() = 0;
    virtual const char*     getName() = 0;
    /// \name resource assignment
    /// @{
    virtual IResource*      getDSTResource() = 0;
    virtual IResource*      getColorResource(int n) = 0;
    /// @}
    virtual int             getWidth() = 0;
    virtual int             getHeight() = 0;
    virtual int             getNumRenderTargets() = 0;
};
/*************************************************************************/ /**
 ** \brief Frame buffer Object binding point (FBO in OpenGL), gathering many render targets : textures or buffers
 **
 **
 **/ /*************************************************************************/ 
class IFrameBufferObjectsRepository : public IBaseInterface<IFrameBufferObjectsRepositoryEx>
{
public:
	virtual ~IFrameBufferObjectsRepository() {}
    virtual IFrameBufferObject*      find(const char * fboName) = 0;
    virtual IFrameBufferObject*      find(int i) = 0;
    virtual int gatherFromAnnotation(IFrameBufferObject** &pp, const char *annotationName, const char* value=NULL) = 0;
    virtual int gatherFromAnnotation(IFrameBufferObject** &pp, const char *annotationName, int value) = 0;
    virtual int gatherFromAnnotation(IFrameBufferObject** &pp, const char *annotationName, float value) = 0;
    virtual int gatherFromNamespace(IFrameBufferObject** &pp, const char *ns) = 0;

    virtual BufferHandle getBackBuffer() = 0;///< D3D interface for color buffer \e OR OpenGL FBO Id (0 for backbuffer)
    virtual BufferHandle getBackBufferDST() = 0;///< D3D interface for DST buffer
    /// \brief (\b Invokes \b GFX \b API)
    /// \arg \e backbuffer is either a FBO Id; 0 for the backbuffer; or a D3D render target
    /// \arg \e x,y,w,h is the viewport of this backbuffer. It will allow some resources to be allocated accordingly to this
    /// \arg depthSamples and coverageSamples are MSAA+CSAA
    virtual bool    validate(int x, int y, int w, int h, int depthSamples, int coverageSamples, BufferHandle backbuffer, BufferHandle backbufferDST=NULL, void *pDev=NULL) = 0;
    virtual bool    finish() = 0;
};
/*************************************************************************/ /**
 ** \brief interface to access the storage of resources created by nvFX
 ** get the singleton with nvFX::getResourceRepositorySingleton() method
 **/ /*************************************************************************/ 
class IResourceRepository : public IBaseInterface<IResourceRepositoryEx>
{
public:
	virtual ~IResourceRepository() {}
    /// \brief (\b Invokes \b GFX \b API)
    /// \arg \e backbuffer is either a FBO Id; 0 for the backbuffer; or a D3D render target
    /// \arg \e x,y,w,h is the viewport of this backbuffer. It will allow some resources to be allocated accordingly to this
    /// \arg depthSamples and coverageSamples are MSAA+CSAA
    /// \arg \e pDev is essentially used in the case of D3D : D3D device.
    virtual bool        validate(int x, int y, int w, int h, int depthSamples, int coverageSamples, BufferHandle backbuffer, BufferHandle backbufferDST=NULL, void *pDev=NULL) = 0;
    virtual bool        finish() = 0;

    virtual IResource*  find(const char * texName) = 0;
    virtual IResource*  find(GLuint texID) = 0; ///< \brief for OpenGL : GL ID of the texture
    virtual IResource*  find(void *p) = 0; ///< \brief for D3D : pointer to interface
    virtual IResource*  get(int i) = 0; ///< \brief return item i. NULL if NA
    virtual int gatherFromAnnotation(IResource** &pp, const char *annotationName, const char* value=NULL) = 0;
    virtual int gatherFromAnnotation(IResource** &pp, const char *annotationName, int value) = 0;
    virtual int gatherFromAnnotation(IResource** &pp, const char *annotationName, float value) = 0;
    virtual int gatherFromNamespace(IResource** &pp, const char *ns) = 0;
};
/*************************************************************************/ /**
 ** \brief interface to access public CstBuffers created by nvFX
 ** get the singleton with nvFX::getCstBufferRepositorySingleton() method
 **/ /*************************************************************************/ 
class ICstBufferRepository : public IBaseInterface<ICstBufferRepositoryEx>
{
public:
	virtual ~ICstBufferRepository() {}
    virtual ICstBuffer*  find(ICstBuffer* p) = 0;
    virtual ICstBuffer*  find(const char * Name) = 0;
    virtual int gatherFromAnnotation(ICstBuffer** &pp, const char *annotationName, const char* value=NULL) = 0;
    virtual int gatherFromAnnotation(ICstBuffer** &pp, const char *annotationName, int value) = 0;
    virtual int gatherFromAnnotation(ICstBuffer** &pp, const char *annotationName, float value) = 0;
    virtual int gatherFromNamespace(ICstBuffer** &pp, const char *ns) = 0;
    virtual ICstBuffer*  findFromSemantic(const char * sem) = 0;
    virtual ICstBuffer*  get(int i) = 0; ///< \brief return item i. NULL if NA
};
/*************************************************************************/ /**
 ** \brief interface to access public Uniforms created by nvFX
 ** get the singleton with nvFX::getUniformRepositorySingleton() method
 **/ /*************************************************************************/ 
class IUniformRepository : public IBaseInterface<IUniformRepositoryEx>
{
public:
	virtual ~IUniformRepository() {}
    virtual IUniform*  find(IUniform* p) = 0;
    virtual IUniform*  find(const char * Name) = 0;
    virtual int gatherFromAnnotation(IUniform** &pp, const char *annotationName, const char* value=NULL) = 0;
    virtual int gatherFromAnnotation(IUniform** &pp, const char *annotationName, int value) = 0;
    virtual int gatherFromAnnotation(IUniform** &pp, const char *annotationName, float value) = 0;
    virtual int gatherFromNamespace(IUniform** &pp, const char *ns) = 0;
    virtual IUniform*  findFromSemantic(const char * sem) = 0;
    virtual IUniform*  get(int i) = 0; ///< \brief return item i. NULL if NA
};
/*************************************************************************/ /**
 ** \brief interface to access the storage of 'global' shaders
 ** get the singleton with nvFX::getShaderModuleRepositorySingleton() method
 **
 ** The idea here is to allow some dedicated sharing of shader modules and eventually
 ** some programs (result of linkage of shader modules). This way, we can factorize
 ** some shader objects and shaders generated inside the graphic API : a new effect
 ** will be able to fetch Shaders and linked programs from here when possible
 **/ /*************************************************************************/ 
class IShaderModuleRepository : public IBaseInterface<IShaderModuleRepositoryEx>
{
public:
	virtual ~IShaderModuleRepository() {}
    /// \name methods to search for items
    /// @{
    /// \brief finds back a shader from its name
    virtual IShader* findShader(const char * name, TargetType t=TANY) = 0;
    virtual int gatherFromAnnotation(IShader** &ppShaders, const char *annotationName, const char* value=NULL) = 0;
    virtual int gatherFromAnnotation(IShader** &ppShaders, const char *annotationName, int value) = 0;
    virtual int gatherFromAnnotation(IShader** &ppShaders, const char *annotationName, float value) = 0;
    virtual int gatherFromNamespace(IShader** &ppShaders, const char *ns) = 0;
    /// \brief finds a program from the combination of shaders
    /// internally, programs are handled by passes. But passes can use separable shader programs
    /// (so does D3D by default) the result of program linkage for rendering pipeline stage
    /// could be global, if all the nvFX::IShader sources are global, too. So it is good to have a
    /// way to find them back
    virtual IProgram*   findProgram(ShaderType type, int numShaders, IShader ** pShaders) = 0;
    /// @}
};

/*************************************************************************/ /**
 ** \brief returns the version
 ** 
 **/ /*************************************************************************/ 
extern unsigned long getVersion();
/// \brief used internally to post messages to messageCallbackFunc. You could still use it, too...
void printf(const char * fmt, ...);
void eprintf(const char * fmt, ...);
/// \brief give an optional error callback function. Either for Dx/GLSL/Cg messages, but for anything else (effect parser...)
void setErrorCallback(messageCallbackFunc func);
/// \brief give an optional message callback function: logs or warnings
void setMessageCallback(messageCallbackFunc func);
/// \brief There is only one repository available for all effects
IResourceRepository* getResourceRepositorySingleton();
/// \brief There is only one repository available for all effects
IFrameBufferObjectsRepository* getFrameBufferObjectsRepositorySingleton();
/// \brief There is only one repository available for all effects
IShaderModuleRepository* getShaderModuleRepositorySingleton();
/// \brief There is only one repository available for all uniforms
IUniformRepository* getUniformRepositorySingleton();
/// \brief There is only one repository available for all CstBuffers
ICstBufferRepository* getCstBufferRepositorySingleton();
// NOTE: this was in the container. needed for D3D case.
// TODO: See what is the best solution to access the device interface... how shall we store this device of Uniform/cstbuffers ?
/// \brief associate a Device pointer. Essentially used for DirectX case (OpenGL does use current context)
void                setDevice(const void *p);
/// \brief returns the associated Device pointer. Essentially used for DirectX case (OpenGL does use current context)
const void*         getDevice();

}//namespace nvFX
#endif //__FXLIB_H__
