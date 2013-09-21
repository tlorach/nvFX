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
#ifndef __FXLIBEX_H__
#define __FXLIBEX_H__

#include "FxLib.h"

namespace nvFX
{
    /// \brief structure only used to pass the array of arguments for CUDA kernel
    struct ArgVal
    {
        ArgVal(IUniform::Type type_=IUniform::TUndefined) : type(type_) {}
        IUniform::Type      type;
        union Val {
            float           f[4];
            int             i[4];
            unsigned int    ui[4];
            IUniform*       pUniform;
            IResource*      pResource;
            void*           p;
            // TODO : add missing items
        };
        Val val;
    };
    /*************************************************************************/ /**
     ** \brief the extended interface 
     ** 
     **/ /*************************************************************************/ 
    class ICstBufferEx : public ICstBuffer
    {
    public:
        /// \brief implementation of the virtual method getExInterface()
        virtual ICstBufferEx*       getExInterface() { return this; }
        ICstBufferEx() : ICstBuffer() {}
        virtual void        setName(const char *name) = 0;
        /// \brief (\b Invokes \b GFX \b API) use directly - without storing it -  a buffer ID to the uniform. Typically used for UBO in OpenGL
        /// \todo write this method
        //@@virtual ICstBuffer* updateGLBuffer(int buffer, IPass *pass, bool bBindProgram=false) { return this; }// = 0;
        /// \brief (\b Invokes \b GFX \b API) use directly - without storing it -  a buffer ptr to the uniform. Typically used for Constant Buffer in OpenGL
        /// \todo write this method
        //@@virtual ICstBuffer* updateD3DBuffer(void *buffer, IPass *pass, bool bBindProgram=false) { return this; }// = 0;
        /// \name methods when the Buffer is hosting uniforms
        /// @{
        /// \brief Creates a uniform
        virtual IUniform*   createUniform(const char *name, const char *semantic=NULL) = 0;
        /// \brief (\b Invokes \b GFX \b API) update the buffer from the uniform objects is contains. returns true if some update happened.
        /// obviously this method can work only if a buffer is properly associated to ICstBuffer
        /// \warning previous values will be \b discarded !
        virtual bool        updateFromUniforms(bool bForceUpdating=false) = 0;
        /// @}
    };
    /*************************************************************************/ /**
     ** \brief the extended interface 
     ** 
     **/ /*************************************************************************/ 
    class IUniformEx : public IUniform
    {
    public:
        /// \brief implementation of the virtual method getExInterface()
        virtual IUniformEx*       getExInterface() { return this; }
        IUniformEx() : IUniform() {}
        virtual void        setName(const char *name) = 0;
        virtual void        setType(IUniform::Type t) = 0; ///< \Remark : this overrides a previous type assigned to this uniform
        virtual void        setPrecision(IUniform::PrecisionType p) = 0;
        virtual void        setArraySz(int sz) = 0;
        /// \name target update and mgt
        // @{
        /// \brief delete a target pass
        virtual IUniform*    deleteTarget(IPass *pass, int layerID) = 0;
        /// \brief invalidates a target pass : in GLSL, the uniform location will be invalidated, for example
        virtual IUniform*    invalidateTarget(IPass *pass, int layerID) = 0;
        /// @}
    };
    /*************************************************************************/ /**
     ** \brief the extended interface 
     ** 
     **/ /*************************************************************************/ 
    class IStateGroupRasterEx : public IStateGroupRaster
    {
    public:
        /// \brief implementation of the virtual method getExInterface()
        virtual IStateGroupRasterEx*       getExInterface() { return this; }
        IStateGroupRasterEx() : IStateGroupRaster() {}
        virtual void            setName(const char *name) = 0;
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
        virtual State*          createState(GLenum sstate, GLenum value) = 0;
        /**
        create states for the following:
        \verbatim
                                             Dynamic      Max
           Property                          Allowed     Index      Default Value
        -----------------------------       ---------    -----      -------------
        GL_POLYGON_MODE                        no          2           FILL
        \endverbatim
        **/
        virtual State*          createState(GLenum sstate, GLenum value1, GLenum value2) = 0;
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
        virtual State*          createState(GLenum sstate, float value) = 0;
        virtual bool            validate() = 0; ///< \brief (\b Invokes \b GFX \b API) before applying, validation tells if any issue and also create the object (D3D or OGL extension for state groups)
    };
    /*************************************************************************/ /**
     ** \brief the extended interface 
     ** 
     **/ /*************************************************************************/ 
    class IStateGroupCSEx : public IStateGroupCS
    {
    public:
        /// \brief implementation of the virtual method getExInterface()
        virtual IStateGroupCSEx*       getExInterface() { return this; }
        IStateGroupCSEx() : IStateGroupCS() {}
        virtual void            setName(const char *name) = 0;
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
        virtual State*          createState(GLenum sstate, GLenum value) = 0;
        /**
        create states for the following:
        \verbatim
                                             Dynamic      Max
           Property                          Allowed     Index      Default Value
        -----------------------------       ---------    -----      -------------
        GL_COLOR_WRITEMASK                     no         [1]          0xF
        \endverbatim
        **/
        virtual State*          createState(GLenum state, GLboolean x, GLboolean y, GLboolean z, GLboolean w) = 0;
        /**
        create states for the following:
        \verbatim
                                             Dynamic      Max
           Property                          Allowed     Index      Default Value
        -----------------------------       ---------    -----      -------------
        GL_BLEND_COLOR                         yes         4           (0,0,0,0) 
        \endverbatim
        **/
        virtual State*          createState(GLenum state, GLclampf x, GLclampf y, GLclampf z, GLclampf w) = 0;
        /**
        create states for the following:
        \verbatim
                                             Dynamic      Max
           Property                          Allowed     Index      Default Value
        -----------------------------       ---------    -----      -------------
        GL_ALPHA_TEST_REF                      yes         1           0
        \endverbatim
        **/
        virtual State*          createState(GLenum state, GLclampf value) = 0;
        virtual bool            validate() = 0; ///< \brief before applying, validation tells if any issue and also create the object (D3D or OGL extension for state groups)
    };
    /*************************************************************************/ /**
     ** \brief the extended interface 
     ** 
     **/ /*************************************************************************/ 
    class IStateGroupDSTEx : public IStateGroupDST
    {
    public:
        /// \brief implementation of the virtual method getExInterface()
        virtual IStateGroupDSTEx*       getExInterface() { return this; }
        IStateGroupDSTEx() : IStateGroupDST() {}
        virtual void            setName(const char *name) = 0;
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
        virtual State*          createState(GLenum state, GLenum value) = 0;
        /**
        create states for the following:
        \verbatim
                                          Dynamic      Max
        Property                          Allowed     Index      Default Value
        --------------------------       ---------    -----      -------------
        DEPTH_BOUNDS_EXT                    yes         2           (0.0, 1.0)
        \endverbatim
        **/
        virtual State*          createState(GLenum state, float val1, float val2) = 0;
        virtual bool            validate() = 0; ///< \brief before applying, validation tells if any issue and also create the object (D3D or OGL extension for state groups)
    };
#ifndef OGLES2
    /*************************************************************************/ /**
     ** \brief the extended interface 
     ** This state group is really OpenGL-centric : deals with NV_path_rendering
     ** this interface is essentially used by the nvFX parser. But you could use it
     ** if you send proper enums to it (using OpenGL enum numbers)
     **/ /*************************************************************************/ 
    class IStateGroupPathEx : public IStateGroupPath
    {
    public:
        /// \brief implementation of the virtual method getExInterface()
        virtual IStateGroupPathEx*       getExInterface() { return this; }
        IStateGroupPathEx() : IStateGroupPath() {}
        virtual void            setName(const char *name) = 0;
        /**
        create states for the following (see GL_NV_path_rendering for details):
        \verbatim
           Property                 Values
        --------------------------------------------------------
        GL_PATH_STENCIL_FUNC_NV        GL_ALWAYS etc.
        GL_PATH_FOG_GEN_MODE_NV        FOG_COORDINATE|FRAGMENT_DEPTH
        GL_PATH_COVER_DEPTH_FUNC_NV    depth func enums

        GL_PATH_INITIAL_END_CAP_NV     GL_FLAT_NV|GL_SQUARE_NV|GL_ROUND_NV|GL_TRIANGULAR_NV
        GL_PATH_TERMINAL_END_CAP_NV    GL_FLAT_NV|GL_SQUARE_NV|GL_ROUND_NV|GL_TRIANGULAR_NV
        GL_PATH_INITIAL_DASH_CAP_NV    GL_FLAT_NV|GL_SQUARE_NV|GL_ROUND_NV|GL_TRIANGULAR_NV
        GL_PATH_TERMINAL_DASH_CAP_NV   GL_FLAT_NV|GL_SQUARE_NV|GL_ROUND_NV|GL_TRIANGULAR_NV
        GL_PATH_JOIN_STYLE_NV          GL_MITER_REVERT_NV|GL_MITER_TRUNCATE_NV|GL_BEVEL_NV|GL_ROUND_NV|GL_NONE
        GL_PATH_DASH_OFFSET_RESET_NV   GL_MOVE_TO_RESET_NV|GL_MOVE_TO_CONTINUES_NV

        GL_PATH_FILL_MODE_NV           GL_COUNT_UP_NV|GL_INVERT| GL_COUNT_DOWN_NV
        GL_PATH_FILL_MASK_NV           int
        GL_PATH_FILL_COVER_MODE_NV     GL_BOUNDING_BOX_NV|GL_CONVEX_HULL_NV
        GL_PATH_STROKE_COVER_MODE_NV   GL_BOUNDING_BOX_NV|GL_CONVEX_HULL_NV
        \endverbatim
        **/
        virtual State*          createState(GLenum state, GLenum value) = 0;
        /**
        create states for the following (see GL_NV_path_rendering for details):
        \verbatim
           Property                 Values
        --------------------------------------------------------
        GL_PATH_STENCIL_DEPTH_OFFSET_FACTOR_NV  float
        GL_PATH_STENCIL_DEPTH_OFFSET_UNITS_NV   float
        GL_PATH_STROKE_WIDTH_NV        Float
        GL_PATH_MITER_LIMIT_NV         Float
        GL_PATH_DASH_OFFSET_NV         Float
        GL_PATH_CLIENT_LENGTH_NV       Float
        \endverbatim
        **/
        virtual State*          createState(GLenum state, float val) = 0;
        /**
        create states for the following (see GL_NV_path_rendering for details):
        \verbatim
           Property                 Values
        --------------------------------------------------------
        GL_PATH_FILL_MASK_NV           int
        \endverbatim
        **/
        virtual State*          createState(GLenum state, int val) = 0;
        /** TODO or TO CHANGE
        The following is not handled for now. Note that these are not states, but
        functions such as PathTexGenNV PathColorGenNV transformValues/transformType args...
        \verbatim
           Property                 Values
        --------------------------------------------------------
        PATH_TRANSFORM_TYPE         TRANSLATE_2D_NV|GL_NONE, GL_TRANSLATE_X_NV|GL_TRANSLATE_Y_NV|GL_TRANSLATE_3D_NV|GL_AFFINE_2D_NV|GL_AFFINE_3D_NV|GL_TRANSPOSE_AFFINE_2D_NV|GL_TRANSPOSE_AFFINE_3D_NV
        PATH_TRANSFORM              List floats (up to 12)
        PATH_TEX_GEN[0]             { OBJECT_LINEAR|OBJECT_LINEAR|PATH_OBJECT_BOUNDING_BOX|EYE_LINEAR, 0, 0.0}
        PATH_COLOR_GEN[0|1]         {OBJECT_LINEAR|OBJECT_LINEAR|PATH_OBJECT_BOUNDING_BOX|EYE_LINEAR, LUMINANCE|ALPHA|INTENSITY|LUMINANCE_ALPHA|RGB|RGBA, 0.0}
        \endverbatim
        **/
        virtual bool            validate() = 0; ///< \brief before applying, validation tells if any issue and also create the object (D3D or OGL extension for state groups)
    };
#endif
    /*************************************************************************/ /**
     ** \brief the extended interface 
     ** 
     **/ /*************************************************************************/ 
    class ISamplerStateEx : public ISamplerState
    {
    public:
        /// \brief implementation of the virtual method getExInterface()
        virtual ISamplerStateEx*       getExInterface() { return this; }
        ISamplerStateEx() : ISamplerState() {}
        virtual void            setName(const char *name) = 0;
        // Maybe we should create local enums to evoid depending on OpenGL and D3D
        //enum Type
        //{
        //    TEXTUREMINFILTER,
        //    TEXTUREMAGFILTER,
        //    TEXTUREWRAPS,
        //    TEXTUREWRAPT,
        //    TEXTUREWRAPR,
        //};
        virtual State* createState(GLenum sstate, GLenum value) = 0; //< easy use for OpenGL GLenums. Working for D3D, too.
        /// \validates the texture object. Essentially needed to setup something on Gfx API side (D3D, for example... no use for OpenGL)
        virtual bool    validate() = 0; ///< \brief (\b Invokes \b GFX \b API) 
    };
    /*************************************************************************/ /**
     ** \brief the extended interface container
     ** 
     **/ /*************************************************************************/ 
    class IContainerEx : public IContainer
    {
    public:
        /// \brief implementation of the virtual method getExInterface()
        virtual IContainerEx*       getExInterface() { return this; }
        /// \name creation methods
        /// @{
        virtual ITechnique*         createTechnique(const char *name = NULL) = 0;
        /// \brief Creates a GLSL shader module. this piece of shader can then be used to create a Shader-Program or be in a Program
        virtual IShader*            createGLSLShader(const char *name = NULL, bool bGlobal=false) = 0;
        virtual bool                addGLSLShader(IShader* p) = 0;
        /// \brief Creates a HLSL10 shader module. this piece of shader can then be used to create a Shader-Program or be in a Program
        virtual IShader*            createHLSL10Shader(const char *name = NULL, bool bGlobal=false) = 0;
        virtual bool                addHLSL10Shader(IShader* p) = 0;
        /// \brief Creates a HLSL11 shader module. this piece of shader can then be used to create a Shader-Program or be in a Program
        virtual IShader*            createHLSL11Shader(const char *name = NULL, bool bGlobal=false) = 0;
        virtual bool                addHLSL11Shader(IShader* p) = 0;
        /// \brief Creates a CUDA code. IShader is the interface although this is not really a 'shader'
        virtual IShader*            createCUDAShader(const char *name = NULL, bool bGlobal=false) = 0;
        virtual bool                addCUDAShader(IShader* p) = 0;
        /// \brief Creates a SVG/PS Path code. IShader is the interface although this is not really a 'shader'
        virtual IShader*            createPathShader(const char *name = NULL, bool bGlobal=false, bool bPostscript=false) = 0;
        virtual bool                addPathShader(IShader* p) = 0;
        /// \brief Creates a GLSL \e shader-program. This really defines a full shader : Vertex, fragment, geometry, Hull/CTS, Eval-shader...
        virtual /*IShaderProgram*/IProgram*     createShaderProgram(ShaderType type, int numShaders, IShader ** pShaders) = 0;
        //virtual DXComputeModule*  createDXComputeModule(const char *name = NULL) = 0;
        //virtual OptixModule*      createOptixModule(const char *name = NULL) = 0;
        //virtual CGShader*         createCGShader(const char *name = NULL) = 0;
        /// \brief appends the common shader parts (unnamed shaders) to the named shaders
        //virtual void              consolidateShaders() = 0;
        virtual ISamplerState*      createSamplerState(const char *name = NULL) = 0;
        virtual IStateGroupRaster*  createStateGroup_Raster(const char *name = NULL) = 0;
        virtual IStateGroupCS*      createStateGroup_CS(const char *name = NULL) = 0;
        virtual IStateGroupDST*     createStateGroup_DST(const char *name = NULL) = 0;
#ifndef OGLES2
        virtual IStateGroupPath*    createStateGroup_PR(const char *name = NULL) = 0;
#endif
        /// \brief Creates a constant buffer in which we will put some uniforms. name=NULL returns a default cst buffer ($Global in D3D)
        virtual ICstBuffer*         createCstBuffer(const char *name, bool bGlobal=true) = 0;
        virtual bool                addCstBuffer(ICstBuffer* p) = 0;
        virtual int                 createUniformsFromPass(IPass *pass) = 0; ///< create the IUniform classes related to a IPass
        /// \brief Creates a uniform. for uniform parameters : they will be created in a default cst buffer
        virtual IUniform*           createUniform(const char *name, const char *groupname=NULL, const char *semantic=NULL, bool bGlobal=true) = 0;
        virtual bool                addUniform(IUniform *p) = 0;
        /// @}
        /// \name destruction methods
        /// @{
        virtual bool    destroy() = 0; ///< destroy the container itself
        virtual bool    destroy(ITechnique* p) = 0;
        virtual bool    destroy(IShader* p) = 0;
        virtual bool    destroy(ISamplerState* p) = 0;
        virtual bool    destroy(IStateGroupRaster* p) = 0;
        virtual bool    destroy(IStateGroupCS* p) = 0;
        virtual bool    destroy(IStateGroupDST* p) = 0;
#ifndef OGLES2
        virtual bool    destroy(IStateGroupPath* p) = 0;
#endif
        virtual bool    destroy(IUniform *p) = 0;
        /// @}
        /// \name Resource Specific
        /// @{
        virtual IResource* createResource(const char * name, ResourceType type, bool bGlobal) = 0;
        virtual bool     addResource(IResource *pRes) = 0;
        virtual IFrameBufferObject* createFBO(const char * name, bool bGlobal) = 0;
        virtual bool     addFbo(IFrameBufferObject *pFbo) = 0;
        /// @}
        /// \name Code generation
        /// @{
        /// \brief serializes the whole data to C++ code
        virtual bool     serializeToCpp(const char* fname) = 0;
        /// \brief serialized the whole data to nvFX file format.
        virtual bool     serializeToNvFX(const char* fname) = 0;
        /// @}
        /// \name general settings for misc behavior
        /// @{
        /// separate shaders is optional in GLSL... and could even fail on old HW/drivers
        virtual void     separateShadersEnable(bool bYes) = 0;
        virtual bool     separateShadersEnabled() = 0;
        // TODO : DSA On/Off ?
        /// @}
    };
    /*************************************************************************/ /**
     ** \brief the extended interface 
     ** 
     **/ /*************************************************************************/ 
    class ITechniqueEx : public ITechnique
    {
    public:
        /// \brief implementation of the virtual method getExInterface()
        virtual ITechniqueEx*       getExInterface() { return this; }
        ITechniqueEx() : ITechnique() {}
        virtual void            setName(const char *name) = 0;
        /// \name creation/destruction of passes
        /// @{
        /// \brief the way to create a pass. the technique owns it
        virtual IPass*           addPass(const char *name = NULL) = 0;
        /// \brief used if IPass was allocated from outside. Warning: a pass must be only assigned to ONE technique
        //void            addPass(IPass *);
        /// \brief remove the specified pass from this technique
        virtual bool            destroy(IPass *p) = 0;
        /// @}
        /// \name working with sub-technique
        /// This sections is useful if one wants to group passes in a single technique to then later
        /// easily activate/unactivate the whole as a consistent section
        /// @{
        /// \brief appends a reference to an existing technique
        /// Adding a sub-technique will just append its passes. No reference to pTech are kept for possible update
        virtual bool             addSubTechnique(ITechnique *pTech, bool bActive=true) = 0;
        virtual void             setSubTechniqueActive(ITechnique *pTech, bool bActive=true) = 0;
        virtual bool             getSubTechniqueActive(ITechnique *pTech) = 0;
        /// @}
    };
    /*************************************************************************/ /**
     ** \brief the extended interface 
     ** 
     **/ /*************************************************************************/ 
    class IPassStateEx : public IPassState
    {
    public:
        /// \brief implementation of the virtual method getExInterface()
        virtual IPassStateEx*       getExInterface() { return this; }
        IPassStateEx() : IPassState() {}
        virtual bool        setShaders(Type type, IShader **progs, int numProgs) = 0;
        // todo : add for others (Geom etc.)
        virtual void        setStateGroup(IStateGroupRaster *pSGR) = 0;
        virtual void        setStateGroup(IStateGroupCS *pSGCS) = 0;
        virtual void        setStateGroup(IStateGroupDST *pSGDST) = 0;
#ifndef OGLES2
        virtual void        setStateGroup(IStateGroupPath *pSGPR) = 0;
#endif
        /// \brief set a state that will assign a resource to a texture (D3D)
        virtual bool        setTextureResource(const char *varName, IResource *pRes) = 0;
        /// \brief set a state that will assign a resource to a sampler (OGL)
        virtual bool        setSamplerResource(const char *varName, IResource *pRes, int texUnit=-1, ISamplerState *pSState=NULL) = 0;
        /// \brief set a sampler-state to a sampler
        virtual bool        setSamplerState(const char *varName, ISamplerState* pSState) = 0;
        //virtual bool        setTextureResource(const char *varName, IResource *pRes) = 0;
        /// \brief set a resource to an image for Load/Store
        virtual bool        setImageResource(const char *varName, IResource *pRes, int texUnit=-1) = 0;
#ifndef OGLES2
        /// \brief set a string to a sub-routine uniform variable
        /// \arg idx used if the sub-routine uniform is an array
        /// \warning idx not working, yet
        virtual bool        setUniform(const char *srcVarName, int idx, const char *dstVarName) = 0;
        /// \brief set strings to a sub-routine uniform array
        /// \warning idx not working, yet
        virtual bool        setUniform(const char *srcVarName, const char **dstVarNames, int numNames) = 0;
#endif
        /// \brief set a float or vector float to a uniform variable
        /// \warning idx not working, yet
        virtual bool        setUniform(const char *varName, int idx, float *pF, int nComps, int nVecs=1) = 0;
        /// \brief set integer or vector integer to a uniform variable
        /// \warning idx not working, yet
        virtual bool        setUniform(const char *varName, int idx, int *pF, int nComps, int nVecs=1) = 0;
        /// \name scene-level states
        /// @{
        virtual bool        setRenderingMode(RenderingMode rmode) = 0;
        virtual bool        setRenderGroup(int g) = 0;
        virtual bool        setClearMode(ClearMode cmode) = 0;
        virtual bool        setClearColor(int i, float r, float g, float b, float a) = 0;
        virtual bool        setRenderTarget(IFrameBufferObject *p) = 0;
        virtual bool        setBlitFBOToActiveTarget(IFrameBufferObject *p) = 0;
        virtual bool        setViewport(int x, int y, int w, int h, float minZ=0.0f, float maxZ=1.0f) = 0;
        virtual bool        setSwapResources(IFrameBufferObject *p1, IFrameBufferObject *p2) = 0;
        /// @}
        /// \name Compute specific (GLSL/DX)
        /// @{
        virtual bool        setStateComputeGroup(int x, int y, int z=1) = 0;
        /// @}
        /// \name CUDA specific
        /// @{
        virtual bool        setStateKernelEntry(const char * kernelName, int numArgs, ArgVal *argvalues) = 0;
        virtual bool        setStateBlockSz(int x, int y, int z=1) = 0;
        virtual bool        setStateGridSz(int x, int y, int z=1) = 0;
        virtual bool        setStateSharedMemory(int Sz) = 0;
        /// @}
        /// \brief (\b Invokes \b GFX \b API) 
        virtual void        apply(bool bUniforms=false, PassInfo * pr=NULL) = 0;
    };
    /*************************************************************************/ /**
     ** \brief the extended interface 
     ** 
     **/ /*************************************************************************/ 
    class IPassEx : public IPass
    {
    public:
        /// \brief implementation of the virtual method getExInterface()
        virtual IPassEx*       getExInterface() { return this; }
        IPassEx() : IPass() {}
        /// \brief sets the pass name
        virtual void            setName(const char *name) = 0;
        /// \name override methods : states to be passed to children effects in the scene.
        /// @{
        /// \brief Create a IPass state-override shader "type". only one shader can be passed
        virtual IPassState*      createStateOverride(IPassState::Type type, IShader *shd, const char *groupName = NULL) = 0;
        /// \brief Create a IPass state shader-override "type" from a list of shaders
        /// if the pass-state already exists, the existing one will be returned.
        virtual IPassState*      createStateOverride(IPassState::Type type, IShader **shd, int numShaders, const char *groupName = NULL) = 0;
        /// \brief create a state that assigns a resource to a sampler (OGL)
        virtual IPassState*      createStateSamplerResourceOverride(const char *varName, IResource *pRes, int texUnit=-1) = 0;
        /// \brief create a state that assigns a resource to a texture (D3D)
        virtual IPassState*      createStateTextureResourceOverride(const char *varName, IResource *pRes) = 0;
        /// \brief create a state that assigns a resource to an image Load/Store
        virtual IPassState*      createStateImageResourceOverride(const char *varName, IResource *pRes, int unit=-1) = 0;
        /// \brief create a state to assign a sampler-state to a sampler
        virtual IPassState*      createStateSamplerStateOverride(const char *varName, ISamplerState * pSState) = 0;
        // TODO ? : removal of state Override ? no need for now...
        /// @}
        /// \name creation/destruction/find methods for pass-states
        /// @{
        /// \brief create a state to assign a sampler-state to a sampler
        virtual IPassState*      createStateSamplerState(const char *varName, ISamplerState * pSState) = 0;
        /// \brief Create a IPass state shader "type". only one shader can be passed
        virtual IPassState*      createState(IPassState::Type type, IShader *shd, const char *groupName = NULL, bool bUseActiveLayer = false) = 0;
        /// \brief Create a IPass state shader "type" from a list of shaders
        /// if the pass-state already exists, the existing one will be returned.
        virtual IPassState*      createState(IPassState::Type type, IShader **shd, int numShaders, const char *groupName = NULL, bool bUseActiveLayer = false) = 0;
#ifndef OGLES2
        /// \brief Typically used to associate subroutine implementations
        virtual IPassState*      createState(const char *srcVarName, int idx, const char *dstVarName) = 0;
        /// \brief Typically used to associate subroutine implementations to an array of uniform routines
        virtual IPassState*      createState(const char *srcVarName, const char **dstVarNames, int numNames) = 0;
#endif
        /// \brief used to assign a float or a vector to a uniform variable
        virtual IPassState*      createState(const char *varName, int idx, float *pF, int nComps) = 0;
        /// \brief used to assign an int or a int-vector to a uniform variable
        virtual IPassState*      createState(const char *varName, int idx, int *pI, int nComps) = 0;
        /// \brief used to assign a float or a vector array to a uniform variable
        virtual IPassState*      createState(const char *varName, int idx, float *pF, int nComps, int nVecs) = 0;
        /// \brief used to assign an int or a int-vector array to a uniform variable
        virtual IPassState*      createState(const char *varName, int idx, int *pI, int nComps, int nVecs) = 0;
        /// \brief assigns to the pass a state group for rasterization
        virtual IPassState*      createState(IStateGroupRaster *pSGRaster) = 0;
        /// \brief assigns to the pass a state group for Color samples
        virtual IPassState*      createState(IStateGroupCS *pSGCS) = 0;
        /// \brief assigns to the pass a state group for Depth-Stencil
        virtual IPassState*      createState(IStateGroupDST *pSGDST) = 0;
#ifndef OGLES2
        /// \brief assigns to the pass a state group for Path Rendering
        virtual IPassState*      createState(IStateGroupPath *pSGPR) = 0;
#endif
        /// \brief create a state that assigns a resource to a texture (D3D)
        virtual IPassState*      createStateTextureResource(const char *varName, IResource *pRes) = 0;
        /// \brief create a state that assigns a resource to a sampler
        virtual IPassState*      createStateSamplerResource(const char *varName, IResource *pRes, int texUnit=-1) = 0;
        /// \brief create a state that assigns a resource to an image Load/Store
        virtual IPassState*      createStateImageResource(const char *varName, IResource *pRes, int unit=-1) = 0;
        /// \name additional states for scene-level effects
        /// @{
        virtual IPassState*      createStateRenderMode(RenderingMode rmode) = 0;
        virtual IPassState*      createStateRenderGroup(int renderGroup) = 0;
        virtual IPassState*      createStateClearMode(ClearMode cmode) = 0;
        virtual IPassState*      createStateClearColor(int i, float r, float g, float b, float a) = 0;
        virtual IPassState*      createStateRenderTarget(IFrameBufferObject *p) = 0;
        virtual IPassState*      createStateBlitFBOToActiveTarget(IFrameBufferObject *p) = 0;
        virtual IPassState*      createStateViewport(int x, int y, int w, int h, float minZ=0.0f, float maxZ=1.0f) = 0;
        /// \brief state that will perform a resource swap : FBO and texture API references completely swapped. Good for ping-pong computation
        virtual IPassState*      createStateSwapResources(IFrameBufferObject *p1, IFrameBufferObject *p2) = 0;
        /// @}
        /// \brief destroys a pass-state
        virtual bool            destroy(IPassState* p) = 0;
        /// @}
        /// \name program-related methods
        /// @{
        /// \brief  returns the program associated with this pass, if it exists
        /// \arg \e layerID = -1 means it will take the active program. Otherwise it will get the program associated with layerID
        /// When CUDA is specified in the pass, this method will return the CUDA program
        virtual IProgram*    getProgram(int layerID) = 0;
        /// \brief  returns the program associated with this pass, if it exists
        /// \arg \e layerID = -1 means it will take the active program. Otherwise it will get the program associated with layerID
        /// When CUDA is used in the pass, this program pipeline will be empty
        virtual IProgramPipeline* getProgramPipeline(int layerID) = 0;
        /// \brief sets the active Layer ID, and create it if requested
        virtual bool            setActiveProgramLayer(int id, bool createIfNA=false) = 0;
        virtual int             getActiveProgramLayer() = 0;
        virtual int             getLayerId(int i) = 0;
        virtual void            removeProgramLayer(int id) = 0;
        /// @}
        /// \name for Compute setup (GLSL/DX)
        /// @{
        virtual IPassState* createStateComputeGroup(int x, int y, int z=1) = 0;
        /// @}
        /// \name CUDA-specific
        /// @{
        /// \brief Create a kernel entry state
        virtual IPassState*      createStateKernelEntry(const char * kernelName, int numArgs, ArgVal *argvalues) = 0;
        virtual IPassState*      createStateBlockSz(int x, int y, int z=1) = 0;
        virtual IPassState*      createStateGridSz(int x, int y, int z=1) = 0;
        virtual IPassState*      createStateSharedMemory(int Sz) = 0;
        /// @}
    };
    /*************************************************************************/ /**
     ** \brief the extended interface 
     ** 
     **/ /*************************************************************************/ 
    class IShaderEx : public IShader
    {
    public:
        /// \brief implementation of the virtual method getExInterface()
        virtual IShaderEx*       getExInterface() { return this; }
        IShaderEx() : IShader() {}
        virtual void    setName(const char *name) = 0;
        virtual void    cleanupShader() = 0;///< \brief (\b Invokes \b GFX \b API) 
        virtual void    cleanupSourceCode() = 0;
        virtual bool    loadShaderCodeFromFile(const char *fname) = 0;
        /// \arg \b code is the shader code
        /// \arg \b startLineNumber is the line number within the effect to allow proper line reference later
        /// \arg \b fname is an optional filename : for example to know in which #include an issue happened on a line of code
        virtual bool    loadShaderCode(const char *code, int startLineNumber=0, const char *fname=NULL) = 0;
        /// \arg \b type can be 0 to 4 (FX_VTXPROG FX_GEOMPROG FX_FRAGPROG FX_TCSPROG FX_TESPROG)
        virtual bool    isCompiled(int type) = 0;
        virtual bool    addHeaderCode(const char *code, int startLineNumber=0, const char *fname=NULL) = 0;
        virtual bool    appendCode(const char *code, int startLineNumber=0, const char *fname=NULL) = 0;
        /// \brief (\b Invokes \b GFX \b API) 
        virtual bool    compileShader(ShaderType type, IContainer* pContainer) { return false; }
        virtual GLhandleARB getGLSLShaderObj(GLenum type) = 0;
        /// \name CUDA-specific
        /// @{
        /// \brief structure only used to pass the array of arguments
        struct Argument {
            Argument(ArgumentType t_=TUndefined, const char *argName_=NULL) : t(t_), argName(argName_) {}
            ArgumentType t;
            const char *argName;
        };
        /// \brief this method will turn the IShader as a function with arguments, rather than being
        /// a chunk of code. This means that the whole code here will be put as part of the code of a
        /// function. This feature is a way to allow CUDA kernels to be declared easily
        /// \arg \e funcName to NULL means we take the IShader name as the function name
        virtual bool            setAsKernel(int nArgs, Argument* args, const char * funcName=NULL) = 0;
        /// \brief returns the i'th argument
        virtual Argument        getArgument(int i) = 0;
        /// \brief returns true if this IShader is being used as a kernel function.
        /// \return returns the name of the kernel, NULL if not used as a kernel
        virtual const char *    isUsedAsKernel() = 0;
        /// @}
    };
    /*************************************************************************/ /**
     ** \brief the extended interface 
     ** 
     **/ /*************************************************************************/ 
    class IProgramEx : public IProgram
    {
    public:
        /// \brief implementation of the virtual method getExInterface()
        virtual IProgramEx*       getExInterface() { return this; }
        IProgramEx() : IProgram() {}
        virtual bool        addShader(ShaderType type, IShader* pShader, IContainer* pContainer) = 0; ///< \brief (\b Invokes \b GFX \b API) 
        virtual bool        addFragmentShader(IShader* pShader, IContainer* pContainer) = 0; ///< \brief (\b Invokes \b GFX \b API) 
        virtual bool        addVertexShader(IShader* pShader, IContainer* pContainer) = 0; ///< \brief (\b Invokes \b GFX \b API) 
        virtual void        cleanup() = 0;///< \brief (\b Invokes \b GFX \b API) 
        virtual bool        bind(IContainer* pContainer) = 0; ///< \brief (\b Invokes \b GFX \b API) 
        virtual void        unbind(IContainer* pContainer) = 0; ///< \brief (\b Invokes \b GFX \b API) 
        virtual bool        link(IContainer* pContainer) = 0; ///< \brief (\b Invokes \b GFX \b API) 
        virtual bool        linkNeeded(int bYes=-1) = 0;
        virtual int         getUniformLocation(const char* name) = 0;///< \brief (\b Invokes \b GFX \b API) 
        virtual int         getASMCode(char* buffer, int bufLen) = 0;///< \brief gives the compiled GLSL code. returns the length of the ASM string code
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
        virtual void        setSampler(const char * texname, int texUnit=-1) = 0;///< \brief (\b Invokes \b GFX \b API) 
        virtual void        bindTexture(ResourceType target, const char * texname, GLuint texid, int texUnit=-1) = 0;///< \brief (\b Invokes \b GFX \b API) 

#ifndef OGLES2
        virtual bool        bindSubRoutineToUniform(int uniformID, GLenum shadertype, char **subroutineNames, int numNames) = 0;///< \brief (\b Invokes \b GFX \b API) 
        virtual int         getSubRoutineID(const char *name, GLenum shadertype) = 0;///< \brief (\b Invokes \b GFX \b API) 
#endif
        /// \name CUDA-specific
        /// @{
        /// \brief create a kernel entry with arguments. Of course it is useful only when IProgramEx refers to a Compute program (CUDA module)
        virtual int         createKernelEntry(const char*kernelName, int *gridSz/*[3]*/, int *blockSz/*[3]*/, int ShMemSz,int numArgs, ArgVal *args) = 0;
        /// \brief runs a kernel entry from an ID referencing a combination of kernel name + args + shared mem + grid/blocks size
        /// basically, one pass may have created one combination for its setup
        virtual bool        executeKernelEntry(int entryID) = 0;
        /// @}
        /// \name Compute and NVPath specific method(s)
        /// @{
        /// \brief Executes GLSL-Compute : glDispatchCompute()
        virtual bool execute(int szx, int szy, int szz=1) = 0;
        /// \brief Executes Path rendering (Stencil/Cover Fill/Stroke)
        /// mode refers to RENDER_STENCILFILLPATH,RENDER_STENCILSTROKEPATH,RENDER_COVERFILLPATH,RENDER_COVERSTROKEPATH
        /// for arguments, refere to the GL_NV_path_rendering spec
        /// however, this method will be invoked by the pass::execute() for you, unless to tell the method to not do it
        virtual bool execute(RenderingMode mode, const PassInfo::PathInfo *p) = 0;
        /// @}
    };
    /*************************************************************************/ /**
     ** \brief the extended interface 
     ** 
     **/ /*************************************************************************/ 
    class IAnnotationEx : public IAnnotation
    {
    public:
        /// \brief implementation of the virtual method getExInterface()
        virtual IAnnotationEx*       getExInterface() { return this; }
        IAnnotationEx() : IAnnotation() {}
        virtual bool         addAnnotation(const char *annotName, const char * str) = 0;
        virtual bool         addAnnotation(const char *annotName, float f) = 0;
        virtual bool         addAnnotation(const char *annotName, int i) = 0;
        virtual bool         delAnnotation(const char *annotName) = 0;
        virtual bool         copy(IAnnotation* pIAnnotations) = 0;
        virtual bool         clear() = 0;
        /// a singleton that can be used to store annotations. Essentially needed by the parser
        static IAnnotationEx* getAnnotationSingleton(int n=0);
    };
    /*************************************************************************/ /**
     ** \brief the extended interface 
     ** 
     **/ /*************************************************************************/ 
    class IResourceEx : public IResource
    {
    public:
        /// \brief implementation of the virtual method getExInterface()
        virtual IResourceEx*       getExInterface() { return this; }
        IResourceEx() : IResource() {}
        /// \brief sets the pass name
        virtual void        setName(const char *name) = 0;
        /// \name resource assignment
        /// @{
        /// \brief resource type
        /// \todo change ResourceType with ResourceType ?
        virtual void        setType(ResourceType type) = 0;
        virtual IContainer* getContainerCreator() = 0; ///< return the container that created this resource. NULL if created from outside of any resource
        /// @}
        /// \name resource creation from within nvFX: when effect creates some render-targets, as a typical example
        /// \todo methods to set/get the associated texture from texture repository
        /// @{
        virtual void        setDimensions(int w, int h, int d=1) = 0;///< \brief resource Size
        virtual void        setDimensionsApplicationDependent(bool bYes=true) = 0;///< \brief resource Size will depends on the app
        virtual void        setMSAA(int d, int s=0) = 0;///< \brief MSAA (+CSAA)
        virtual void        setMSAAApplicationDependent(bool bYes=true) = 0;///< \brief MSAA will depends on the app
        virtual void        setDimensionFactor(float w,float h) = 0;///< \brief if we need to
        /// \brief this points to a Texture that is supposed to be the target when resolve happens. This is way to \e shortcut the 'resolve'
        /// process when possible : when no MSAA involved, the RenderBuffer could be replaced by this texture directly...
        virtual void        setResolveTarget(IResource* p) = 0;
        virtual bool        setFormat(ResourceFormat f) = 0; ///< \brief tells which format we want for the creation of this resource
        virtual ResourceFormat getFormat() = 0; ///< \brief returns the format
        /// @}
        /// \brief (\b Invokes \b GFX \b API)
        virtual bool        validationNeeded() = 0;
        virtual bool        validate() = 0;
        virtual bool        invalidate() = 0;
        //virtual bool        unmapFromCUDA() =0; ///< for CUDA only
        /// \name reference counter of users.
        /// Allows to figure-out if it is worth updating and allocating memory for the resource.
        /// if the user count is 0, we could release (invalidate) the memory related to this resource
        /// @{
        virtual int         incUserCnt() = 0; ///< \brief incremement the reference counter of users
        virtual int         decUserCnt() = 0; ///< \brief decremement the reference counter of users
        virtual int         getUserCnt() = 0; ///< \brief decremement the reference counter of users
        /// @}
    };
    /*************************************************************************/ /**
     ** \brief the extended interface 
     ** 
     **/ /*************************************************************************/ 
    class IFrameBufferObjectEx : public IFrameBufferObject
    {
    public:
        /// \brief implementation of the virtual method getExInterface()
        virtual IFrameBufferObjectEx*       getExInterface() { return this; }
        IFrameBufferObjectEx() : IFrameBufferObject() {}
        /// \brief sets the pass name
        virtual void            setName(const char *name) = 0;
        /// \name resource assignment
        /// @{
        virtual void            setDSTResource(IResource * pDST) = 0;
        virtual void            setColorResource(IResource ** pp, int n) = 0;
        /// @}
        virtual bool            setCurrent(bool bAdjustViewport) = 0;
        virtual bool            blitTo(IFrameBufferObject* pDst) = 0;
        /// \brief (\b Invokes \b GFX \b API)
        virtual bool            validate() = 0;
        virtual bool            invalidate() = 0;
    };
    /*************************************************************************/ /**
     ** \brief the extended interface 
     ** 
     **/ /*************************************************************************/ 
    class IFrameBufferObjectsRepositoryEx : public IFrameBufferObjectsRepository
    {
    public:
        /// \brief implementation of the virtual method getExInterface()
        virtual IFrameBufferObjectsRepositoryEx*       getExInterface() { return this; }
        IFrameBufferObjectsRepositoryEx() : IFrameBufferObjectsRepository() {}

        /// \brief swaps 2 similar FBOs.
        static bool swapResources(IFrameBufferObject *pF1, IFrameBufferObject *pF2);

        /// \brief Creation of a FBO.
        virtual IFrameBufferObject* createFBO(const char * name) = 0;
        /// \brief add an existing FBO
        virtual int                 addFBO(IFrameBufferObject* p) = 0;
        /// \brief release a resource. could lead to deleting it
        virtual int                 releaseFbo(IFrameBufferObject* p) = 0;

        virtual bool      blit(IFrameBufferObject* pDst, IFrameBufferObject* pSrc) = 0;
        virtual bool      setCurrent(IFrameBufferObject* pFBO, bool bAdjustViewport) = 0;

    };
    /*************************************************************************/ /**
     ** \brief the extended interface 
     ** 
     **/ /*************************************************************************/ 
    class IResourceRepositoryEx : public IResourceRepository
    {
    public:
        /// \brief implementation of the virtual method getExInterface()
        virtual IResourceRepositoryEx*       getExInterface() { return this; }
        IResourceRepositoryEx() : IResourceRepository() {}
        /// \brief Create a resource "name".
        virtual IResource*  createResource(const char * name, ResourceType type) = 0;
        /// \brief add an existing resource
        virtual int         addResource(IResource* pRes) = 0;
        /// \brief release a resource. could lead to deleting it
        virtual int         releaseResource(IResource* pRes) = 0;
        //virtual IResource*  createTexture2DRandomRays(const char * name, int nbDirs) = 0; // for HBAO

        virtual IResource*  loadTexture(const char * texName/*, CGtype samplerType*/) = 0;
        virtual IResource*  loadTexture3DSlices(const char * texName, const char * *fileNames, int nFiles) = 0;
        virtual void        releaseTextureID(GLuint texID) = 0;
    };
    /*************************************************************************/ /**
     ** \brief the extended interface 
     ** 
     **/ /*************************************************************************/ 
    class IUniformRepositoryEx : public IUniformRepository
    {
    public:
        /// \brief implementation of the virtual method getExInterface()
        virtual IUniformRepositoryEx*       getExInterface() { return this; }
        IUniformRepositoryEx() : IUniformRepository() {}
        /// \brief Create a Uniform "name". Will fail if an already existing Uniform of the same name is already here
        virtual IUniform*   createUniform(const char *name, const char *groupname=NULL, const char *semantic=NULL) = 0;
        /// \brief add an existing Uniform. Will increment the refCnt if already in the repository
        virtual int         addUniform(IUniform* p) = 0;
        /// \brief release a Uniform. could lead to deleting it
        virtual int         releaseUniform(IUniform* p) = 0;
    };
    /*************************************************************************/ /**
     ** \brief the extended interface 
     ** 
     **/ /*************************************************************************/ 
    class ICstBufferRepositoryEx : public ICstBufferRepository
    {
    public:
        /// \brief implementation of the virtual method getExInterface()
        virtual ICstBufferRepositoryEx*       getExInterface() { return this; }
        ICstBufferRepositoryEx() : ICstBufferRepository() {}
        /// \brief Create a CstBuffer "name". Will fail if the a same name exists as a CstBuffer
        virtual ICstBuffer*   createCstBuffer(const char *name) = 0;
        /// \brief add an existing CstBuffer. If already in the repository, we will increase the ref counter
        virtual int         addCstBuffer(ICstBuffer* p) = 0;
        /// \brief release a CstBuffer. could lead to deleting it
        virtual int         releaseCstBuffer(ICstBuffer* p) = 0;
    };
    /*************************************************************************/ /**
     ** \brief the extended interface 
     ** 
     **/ /*************************************************************************/ 
    class IShaderModuleRepositoryEx : public IShaderModuleRepository
    {
    public:
        /// \brief implementation of the virtual method getExInterface()
        virtual IShaderModuleRepositoryEx*       getExInterface() { return this; }
        IShaderModuleRepositoryEx() : IShaderModuleRepository() {}
        /// \name methods to add some items
        /// @{
        /// \brief add a shader
        virtual int addShader(IShader *p) = 0;
        /// \brief add a program
        virtual int addProgram(IProgram* p) = 0;
        /// @}
        /// \name methods to release some items. Note : a reference count is used before removing it totally
        /// @{
        /// \brief release a shader
        virtual int releaseShader(IShader *p) = 0;
        /// \brief release a program. returns -1 if p wasn't in the pool; or the refcnt after releasing
        virtual int releaseProgram(IProgram* p) = 0;
        /// @}
    };
    /*************************************************************************/ /**
     ** \brief the extended interface 
     ** to remove: give some control that would break consistency in nvFX
     **/ /*************************************************************************/ 
    //class IProgramPipelineEx : public IProgramPipeline
    //{
    //public:
    //    /// \brief implementation of the virtual method getExInterface()
    //    virtual IProgramPipelineEx*       getExInterface() { return this; }
    //    IProgramPipelineEx() : IProgramPipeline() {}
    //    /// \brief adds any kind of program to the pipeline.
    //    virtual bool            addProgramShader(IProgram* pProgShader) = 0;
    //    /// \brief remove a specific program from this pipeline
    //    virtual bool            removeProgramShader(IProgram* pProgShader) = 0;
    //    /// \brief remove any program that contains the specific shader stage type
    //    virtual IProgram*       removeProgramShader(int stageFlags) = 0;
    //    virtual bool            validate() = 0;
    //};
}//namespace nvFX

#endif //__FXLIBEX_H__
