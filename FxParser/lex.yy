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

    $Id: //sw/devrel/Playpen/tristan/GitHub/nvFX/FxParser/lex.yy#9 $
    $Date: 2013/06/20 $
    $Revision: #9 $
    $Author: tlorach $
    $Change: 16301413 $
*/


%option prefix="FxParser"
%option noc++
%option batch
%option case-insensitive

%{
	#define yylval FxParserlval //hack, if not it does not compile
    #pragma warning(disable:4996)
    #include  <math.h>
    #include  <string>
    #include  <stack>

    #include  <string.h>
    #include  <stdio.h>
    #include  <assert.h>
    #include  "Grammar.hpp"
    #include  "FxLibEx.h"
    #include  "FxParser.h"
    #define MAX_STR_CONST 16382 // TODO: Change this and make it dynamic... shaders could be big

    namespace nvFX
    {
        extern IncludeCb g_includeCb;
    }
    int  line_num  =  1;
    std::string file_name;
    int  bracket_level = 0;
    char  *string_buf_ptr;
    //#define  MAX_INCLUDE_DEPTH  10
    //YY_BUFFER_STATE  include_stack[MAX_INCLUDE_DEPTH];
    //int  include_stack_ptr  =  0;
    struct IncludeData {
        IncludeData(YY_BUFFER_STATE s, int ln, const char *fname, FILE *fd=NULL) :
            state(s), line_num(ln), file_name(fname), fileToClose(fd) {}
        YY_BUFFER_STATE state;
        int             line_num;
        std::string     file_name; // the name of the current file (when in #include, for example)
        FILE*           fileToClose; // keep track of the file descriptor to make sure we will close it
    };
    std::stack<IncludeData> include_stack;

    // we need to do this because yy_pop_state is generated as static
    static void yy_pop_state();
    void lex_pop_state()
    {
        yy_pop_state();
    }
%}

%option stack
%x fbo fbo1 clearmode rendermode incl comment comment2 str shader shaderbody cstbuffer technique pass sampler_state dst_state pr_state color_sample_state rasterization_state resource resource1
DIGIT       [0-9]
HEXNUM       0x[0-9a-fA-F]+
VARIABLE    [a-zA-Z_]+[\.a-zA-Z\-_0-9]*(::)?[\.a-zA-Z\-_0-9]*
%%
    int  comment_caller;
    char  string_buf[MAX_STR_CONST];

<INITIAL,shader,fbo1,resource1,cstbuffer>{
    global return GLOBAL;
    local return LOCAL;
}

namespace {
    bracket_level = 0;
    return NAMESPACE;
}

    /*==================================================================
      FBO
    */
FBO {
    bracket_level = 0;
    yy_push_state(fbo1);
    return FBO;
}
    /*==================================================================
      Texture for scene-level effect : RenderTexture and RenderBuffer
      They share the same as Texture : they are resources
    */
RenderTexture {
    bracket_level = 0;
    yy_push_state(resource1);
    return RENDERTEXTURE;
}
RenderTextureRect(angle)? {
    bracket_level = 0;
    yy_push_state(resource1);
    return RENDERTEXTURERECT;
}
RenderBuffer {
    bracket_level = 0;
    yy_push_state(resource1);
    return RENDERBUFFER;
}
    /*==================================================================
      
    */
TextureResource {
    bracket_level = 0;
    yy_push_state(resource1);
    return TEXTURERESOURCE;
}
TextureResourceCube {
    bracket_level = 0;
    yy_push_state(resource1);
    return TEXTURERESOURCECUBE;
}
TextureResource1D {
    bracket_level = 0;
    yy_push_state(resource1);
    return TEXTURERESOURCE1D;
}
TextureResource2D {
    bracket_level = 0;
    yy_push_state(resource1);
    return TEXTURERESOURCE2D;
}
TextureResource3D {
    bracket_level = 0;
    yy_push_state(resource1);
    return TEXTURERESOURCE3D;
}
TextureResourceRect(angle)? {
    bracket_level = 0;
    yy_push_state(resource1);
    return TEXTURERESOURCERECT;
}
GLSLShader {
    comment_caller  =  INITIAL;
    bracket_level = 0;
    yy_push_state(shader);
    return GLSLSHADER;
}
SVGPath {
    comment_caller  =  INITIAL;
    bracket_level = 0;
    yy_push_state(shader);
    return SVGPATH;
}
PostscriptPath {
    comment_caller  =  INITIAL;
    bracket_level = 0;
    yy_push_state(shader);
    return PSPATH;
}
HLSL10Shader {
    comment_caller  =  INITIAL;
    bracket_level = 0;
    yy_push_state(shader);
    return HLSL10SHADER;
}
HLSL11Shader {
    comment_caller  =  INITIAL;
    bracket_level = 0;
    yy_push_state(shader);
    return HLSL11SHADER;
}
CUDACode {
    comment_caller  =  INITIAL;
    bracket_level = 0;
    yy_push_state(shader);
    return CUDACODE;
}
CUDAKernel {
    comment_caller  =  INITIAL;
    bracket_level = 0;
    yy_push_state(shader);
    return CUDAKERNEL;
}
SamplerState {
    yy_push_state(sampler_state);
    return SAMPLER_STATE;
}
DepthStencilState {
    yy_push_state(dst_state);
    return DST_STATE;
}
PathRenderingState {
    yy_push_state(pr_state);
    return PR_STATE;
}
BlendState {
    yy_push_state(color_sample_state);
    return COLOR_SAMPLE_STATE;
}
ColorSampleState {
    yy_push_state(color_sample_state);
    return COLOR_SAMPLE_STATE;
}
RasterizationState {
    yy_push_state(rasterization_state);
    return RASTERIZATION_STATE;
}
Technique {
    yy_push_state(technique);
    return TECHNIQUE;
}
    /*
    Constant Buffer
    */
ConstantBuffer {
    yy_push_state(cstbuffer);
    return CSTBUFFER;
}

#extension {
    return EXTENSION;
}

(GL_)?ARB_separate_shader_objects {
    return SEPARATE_SHADER;
}

    /*
    OPTIX.
    Not using any restricted scope since I should change the whole lexer to avoid this (TODO)
    */
OptixContext        return OPTIX_CONTEXT;
OptixContextEntry   return OPTIX_CONTEXT_ENTRY;
RayGenerationProgram return RAY_GENERATION_PROGRAM;
ExceptionProgram    return EXCEPTION_PROGRAM;
IntersectionProgram return INTERSECTION_PROGRAM;
BoundingBoxProgram  return BOUNDING_BOX_PROGRAM;
MissProgram         return MISS_PROGRAM;
DefaultMaterial     return DEFAULT_MATERIAL;
StackSize           return STACK_SIZE;
OutputBuffer        return OUTPUT_BUFFER;

OptixMaterial       return OPTIX_MATERIAL;
ClosestHitProgram   return CLOSEST_HIT_PROGRAM;
AnyHitProgram       return ANY_HIT_PROGRAM;

OptixBuffer         return OPTIX_BUFFER;
Size                return RESOURCESIZE;    /*could be later used by other parts of the grammar*/
Format              return RESOURCEFORMAT;

PTXProgramFromFile  return PTX_PROGRAM_FROM_FILE; /* not only for Optix*/
FileName            return FILE_NAME;
EntryPoint          return ENTRY_POINT;


<INITIAL,cstbuffer>{
    \{  {
        return yytext[0];
    }
    \} {
        //yy_pop_state();
        return yytext[0];
    }
}
<INITIAL,cstbuffer,shader>{
    void   return VOID_TYPE;
    unsigned return UNSIGNED;
    float  return FLOAT_TYPE;
    float2 return FLOAT2_TYPE;
    float3 return FLOAT3_TYPE;
    float4 return FLOAT4_TYPE;
    vec2   return FLOAT2_TYPE;
    vec3   return FLOAT3_TYPE;
    vec4   return FLOAT4_TYPE;
    mat2   return MAT2_TYPE;
    mat3   return MAT3_TYPE;
    mat4   return MAT4_TYPE;
    float2x2   return MAT2_TYPE;
    float3x3   return MAT3_TYPE;
    float4x4   return MAT4_TYPE;
    bool   return BOOL_TYPE;
    bvec2  return BOOL2_TYPE;
    bvec3  return BOOL3_TYPE;
    bvec4  return BOOL4_TYPE;
    int    return INT_TYPE;
    int2   return INT2_TYPE;
    int3   return INT3_TYPE;
    int4   return INT4_TYPE;
    ivec2  return INT2_TYPE;
    ivec3  return INT3_TYPE;
    ivec4  return INT4_TYPE;
    uniform return UNIFORM;
    mediump return MEDIUMP;
    highp   return HIGHP;
    lowp    return LOWP;
    Texture1D       return TEXTURE1D_TYPE;
    Texture2D       return TEXTURE2D_TYPE;
    Texture2DRect   return TEXTURE2DRECT_TYPE;
    Texture3D       return TEXTURE3D_TYPE;
    TextureCube     return TEXTURECUBE_TYPE;
    sampler1D       return SAMPLER1D_TYPE;
    sampler2D       return SAMPLER2D_TYPE;
    sampler2DRect   return SAMPLER2DRECT_TYPE;
    sampler3D       return SAMPLER3D_TYPE;
    samplerCube     return SAMPLERCUBE_TYPE;
}
    /*==================================================================
      Float/double values
    */
<INITIAL,cstbuffer,technique,sampler_state,dst_state,pr_state,color_sample_state,rasterization_state,pass,resource,resource1,shader>{
    {DIGIT}+ {
        yylval.ival = atoi(yytext);
        return FX_INT;
    }
    {HEXNUM} {
        sscanf(yytext, "0x%x", &yylval.ival);
        return FX_INT;
    }
{DIGIT}+"."{DIGIT}*f? {
        yylval.fval = (float)atof(yytext);
        return FX_FLOAT;
    }
}
    /*==================================================================
      Boolean
    */
<INITIAL,cstbuffer,technique,sampler_state,dst_state,pr_state,color_sample_state,rasterization_state,pass,resource,resource1>{
    [e|E]nable          return FX_TRUE;
    [d|D]isable         return FX_FALSE;
    [y|Y]es             return FX_TRUE;
    [n|N]o              return FX_FALSE;
    [t|T]rue            return FX_TRUE;
    [f|F]alse           return FX_FALSE;
    TRUE   return FX_TRUE;
    FALSE  return FX_FALSE;
}
    /*==================================================================
      OpenGL Enums
    */
<sampler_state,dst_state,pr_state,color_sample_state,rasterization_state,pass,resource>{
    ACCUM yylval.glenum=GLACCUM; return GLACCUM;
    LOAD yylval.glenum=GLLOAD; return GLLOAD;
    RETURN yylval.glenum=GLRETURN; return GLRETURN;
    MULT yylval.glenum=GLMULT; return GLMULT;
    ADD yylval.glenum=GLADD; return GLADD;
    NEVER yylval.glenum=GLNEVER; return GLNEVER;
    LESS yylval.glenum=GLLESS; return GLLESS;
    EQUAL yylval.glenum=GLEQUAL; return GLEQUAL;
    GREATER yylval.glenum=GLGREATER; return GLGREATER;
    NOTEQUAL yylval.glenum=GLNOTEQUAL; return GLNOTEQUAL;
    ALWAYS yylval.glenum=GLALWAYS; return GLALWAYS;
    LEQUAL yylval.glenum=GLLEQUAL; return GLLEQUAL;
    GEQUAL yylval.glenum=GLGEQUAL; return GLGEQUAL;
    KEEP yylval.glenum=GLKEEP; return GLKEEP;
    REPLACE yylval.glenum=GLREPLACE; return GLREPLACE;
    INCR yylval.glenum=GLINCR; return GLINCR;
    DECR yylval.glenum=GLDECR; return GLDECR;
    UPPER_LEFT yylval.glenum=GLUPPERLEFT; return GLUPPERLEFT;
    LOWER_LEFT yylval.glenum=GLLOWERLEFT; return GLLOWERLEFT;
    FRONT_LEFT yylval.glenum=GLFRONTLEFT; return GLFRONTLEFT;
    FRONT_RIGHT yylval.glenum=GLFRONTRIGHT; return GLFRONTRIGHT;
    BACK_LEFT yylval.glenum=GLBACKLEFT; return GLBACKLEFT;
    BACK_RIGHT yylval.glenum=GLBACKRIGHT; return GLBACKRIGHT;
    FRONT yylval.glenum=GLFRONT; return GLFRONT;
    BACK yylval.glenum=GLBACK; return GLBACK;
    LEFT yylval.glenum=GLLEFT; return GLLEFT;
    RIGHT yylval.glenum=GLRIGHT; return GLRIGHT;
    FRONT_AND_BACK yylval.glenum=GLFRONTANDBACK; return GLFRONTANDBACK;
    CCW yylval.glenum=GLCCW; return GLCCW;
    CW yylval.glenum=GLCCW; return GLCCW;
    POINT yylval.glenum=GLPOINT; return GLPOINT;
    LINE yylval.glenum=GLLINE; return GLLINE;
    FILL yylval.glenum=GLFILL; return GLFILL;
    ZERO yylval.glenum=GLZERO; return GLZERO;
    ONE yylval.glenum=GLONE; return GLONE;
    SRC_COLOR yylval.glenum=GLSRCCOLOR; return GLSRCCOLOR;
    ONE_MINUS_SRC_COLOR yylval.glenum=GLONEMINUSSRCCOLOR; return GLONEMINUSSRCCOLOR;
    SRC_ALPHA yylval.glenum=GLSRCALPHA; return GLSRCALPHA;
    ONE_MINUS_SRC_ALPHA yylval.glenum=GLONEMINUSSRCALPHA; return GLONEMINUSSRCALPHA;
    DST_ALPHA yylval.glenum=GLDSTALPHA; return GLDSTALPHA;
    ONE_MINUS_DST_ALPHA yylval.glenum=GLONEMINUSDSTALPHA; return GLONEMINUSDSTALPHA;
    DST_COLOR yylval.glenum=GLDSTCOLOR; return GLDSTCOLOR;
    ONE_MINUS_DST_COLOR yylval.glenum=GLONEMINUSDSTCOLOR; return GLONEMINUSDSTCOLOR;
    SRC_ALPHA_SATURATE yylval.glenum=GLSRCALPHASATURATE; return GLSRCALPHASATURATE;
    CONSTANT_COLOR yylval.glenum=GLCONSTANTCOLOR; return GLCONSTANTCOLOR;
    ONE_MINUS_CONSTANT_COLOR yylval.glenum=GLONEMINUSCONSTANTCOLOR; return GLONEMINUSCONSTANTCOLOR;
    CONSTANT_ALPHA yylval.glenum=GLCONSTANTALPHA; return GLCONSTANTALPHA;
    ONE_MINUS_CONSTANT_ALPHA yylval.glenum=GLONEMINUSCONSTANTALPHA; return GLONEMINUSCONSTANTALPHA;
    BLEND_COLOR yylval.glenum=GLBLENDCOLOR; return GLBLENDCOLOR;
    FUNC_ADD yylval.glenum=GLFUNCADD; return GLFUNCADD;
    MIN yylval.glenum=GLMIN; return GLMIN;
    MAX yylval.glenum=GLMAX; return GLMAX;
    FUNC_SUBTRACT yylval.glenum=GLFUNCSUBTRACT; return GLFUNCSUBTRACT;
    FUNC_REVERSE_SUBTRACT yylval.glenum=GLFUNCREVERSESUBTRACT; return GLFUNCREVERSESUBTRACT;
    CLEAR yylval.glenum=GLCLEAR; return GLCLEAR;
    SET yylval.glenum=GLSET; return GLSET;
    COPY yylval.glenum=GLCOPY; return GLCOPY;
    COPY_INVERTED yylval.glenum=GLCOPYINVERTED; return GLCOPYINVERTED;
    NOOP yylval.glenum=GLNOOP; return GLNOOP;
    INVERT yylval.glenum=GLINVERT; return GLINVERT;
    AND yylval.glenum=GLAND; return GLAND;
    NAND yylval.glenum=GLNAND; return GLNAND;
    OR yylval.glenum=GLOR; return GLOR;
    NOR yylval.glenum=GLNOR; return GLNOR;
    XOR yylval.glenum=GLXOR; return GLXOR;
    EQUIV yylval.glenum=GLEQUIV; return GLEQUIV;
    AND_REVERSE yylval.glenum=GLANDREVERSE; return GLANDREVERSE;
    AND_INVERTED yylval.glenum=GLANDINVERTED; return GLANDINVERTED;
    OR_REVERSE yylval.glenum=GLORREVERSE; return GLORREVERSE;
    OR_INVERTED yylval.glenum=GLORINVERTED; return GLORINVERTED;
    LUMINANCE yylval.glenum=GLLUMINANCE; return GLLUMINANCE;
    ALPHA yylval.glenum=GLALPHA; return GLALPHA;
    INTENSITY yylval.glenum=GLINTENSITY; return GLINTENSITY;
    LUMINANCEALPHA yylval.glenum=GLLUMINANCEALPHA; return GLLUMINANCEALPHA;
    RGB yylval.glenum=GLRGB; return GLRGB;
    RGBA yylval.glenum=GLRGBA; return GLRGBA;
}
<pass,rasterization_state>{
    \{  {
        return yytext[0];
    }
    \} {
        //yy_pop_state();
        return yytext[0];
    }
    POINT_SIZE yylval.glenum=GLPOINTSIZE; return GLPOINTSIZE;
    POINT_SMOOTH yylval.glenum=GLPOINTSMOOTH; return GLPOINTSMOOTH;
    POINT_SPRITE yylval.glenum=GLPOINTSPRITE; return GLPOINTSPRITE;
    POINT_SPRITE_COORD_ORIGIN yylval.glenum=GLPOINTSPRITECOORDORIGIN; return GLPOINTSPRITECOORDORIGIN;
    LINE_WIDTH yylval.glenum=GLLINEWIDTH; return GLLINEWIDTH;
    LINE_SMOOTH yylval.glenum=GLLINESMOOTH; return GLLINESMOOTH;
    /*COMBINED_LINE_STIPPLE_NV yylval.glenum=GLCOMBINEDLINESTIPPLENV; return GLCOMBINEDLINESTIPPLENV;*/
    LINE_STIPPLE_PATTERN yylval.glenum=GLLINESTIPPLEPATTERN; return GLLINESTIPPLEPATTERN;
    LINE_STIPPLE_REPEAT yylval.glenum=GLLINESTIPPLEREPEAT; return GLLINESTIPPLEREPEAT;
    LINE_STIPPLE yylval.glenum=GLLINESTIPPLE; return GLLINESTIPPLE;
    CULL_FACE yylval.glenum=GLCULLFACE; return GLCULLFACE;
    CULL_FACE_MODE yylval.glenum=GLCULLFACEMODE; return GLCULLFACEMODE;
    FRONT_FACE yylval.glenum=GLFRONTFACE; return GLFRONTFACE;
    POLYGON_SMOOTH yylval.glenum=GLPOLYGONSMOOTH; return GLPOLYGONSMOOTH;
    POLYGON_MODE yylval.glenum=GLPOLYGONMODE; return GLPOLYGONMODE;
    POLYGON_OFFSET_FACTOR yylval.glenum=GLPOLYGONOFFSETFACTOR; return GLPOLYGONOFFSETFACTOR;
    POLYGON_OFFSET_UNITS yylval.glenum=GLPOLYGONOFFSETUNITS; return GLPOLYGONOFFSETUNITS;
    POLYGON_OFFSET_POINT yylval.glenum=GLPOLYGONOFFSETPOINT; return GLPOLYGONOFFSETPOINT;
    POLYGON_OFFSET_LINE yylval.glenum=GLPOLYGONOFFSETLINE; return GLPOLYGONOFFSETLINE;
    POLYGON_OFFSET_FILL yylval.glenum=GLPOLYGONOFFSETFILL; return GLPOLYGONOFFSETFILL;
    PolygonOffset return GLPOLYGONOFFSET;
    POLYGON_STIPPLE_BIT yylval.glenum=GLPOLYGONSTIPPLEBIT; return GLPOLYGONSTIPPLEBIT;
    RASTERIZER_DISCARD yylval.glenum=GLRASTERIZERDISCARD; return GLRASTERIZERDISCARD;
}
<pass,color_sample_state>{
    \{  {
        return yytext[0];
    }
    \} {
        //yy_pop_state();
        return yytext[0];
    }
    ALPHA_TEST yylval.glenum=GLALPHATEST; return GLALPHATEST;
    ALPHA_TEST_FUNC yylval.glenum=GLALPHATESTFUNC; return GLALPHATESTFUNC;
    ALPHA_TEST_REF yylval.glenum=GLALPHATESTREF; return GLALPHATESTREF;
    ALPHAFUNC return GLALPHAFUNC;
    BLEND yylval.glenum=GLBLEND; return GLBLEND;
    BlendFuncSeparate return GLBLENDFUNCSEPARATE;
    BlendFunc return GLBLENDFUNC;
    BLEND_SRC yylval.glenum=GLBLENDSRC; return GLBLENDSRC;
    BLEND_SRC_RGB yylval.glenum=GLBLENDSRCRGB; return GLBLENDSRCRGB;
    BLEND_SRC_ALPHA yylval.glenum=GLBLENDSRCALPHA; return GLBLENDSRCALPHA;
    BLEND_DST yylval.glenum=GLBLENDDST; return GLBLENDDST;
    BLEND_DST_RGB yylval.glenum=GLBLENDDSTRGB; return GLBLENDDSTRGB;
    BLEND_DST_ALPHA yylval.glenum=GLBLENDDSTALPHA; return GLBLENDDSTALPHA;
    /*BLEND_EQUATION_RGBA_NV yylval.glenum=GLBLENDEQUATIONRGBANV; return GLBLENDEQUATIONRGBANV;*/
    BLEND_EQUATION_RGB yylval.glenum=GLBLENDEQUATIONRGB; return GLBLENDEQUATIONRGB;
    BLEND_EQUATION_ALPHA yylval.glenum=GLBLENDEQUATIONALPHA; return GLBLENDEQUATIONALPHA;
    BlendEquation return GLBLENDEQUATION;
    /*BlendEquationSeparate return GLBLENDEQUATIONSEPARATE;*/
    BLEND_EQUATION return GLBLENDEQUATION;
    /*PER_DRAW_BUFFER_BLEND_FUNCTION_NV yylval.glenum=GLPERDRAWBUFFERBLENDFUNCTIONNV; return GLPERDRAWBUFFERBLENDFUNCTIONNV;
    PER_DRAW_BUFFER_BLEND_EQUATION_NV yylval.glenum=GLPERDRAWBUFFERBLENDEQUATIONNV; return GLPERDRAWBUFFERBLENDEQUATIONNV;
    PER_DRAW_BUFFER_BLEND_ENABLE_NV yylval.glenum=GLPERDRAWBUFFERBLENDENABLENV; return GLPERDRAWBUFFERBLENDENABLENV;
    PER_DRAW_BUFFER_COLOR_WRITEMASK_NV yylval.glenum=GLPERDRAWBUFFERCOLORWRITEMASKNV; return GLPERDRAWBUFFERCOLORWRITEMASKNV;*/
    /*BLEND_COLOR yylval.glenum=GLBLENDCOLOR; return GLBLENDCOLOR;*/
    FRAMEBUFFER_SRGB yylval.glenum=GLFRAMEBUFFERSRGB; return GLFRAMEBUFFERSRGB;
    DITHER yylval.glenum=GLDITHER; return GLDITHER;
    COLOR_LOGIC_OP yylval.glenum=GLCOLORLOGICOP; return GLCOLORLOGICOP;
    LOGIC_OP_MODE yylval.glenum=GLLOGICOPMODE; return GLLOGICOPMODE;
    LogicOp yylval.glenum=GLLOGICOPMODE; return GLLOGICOPMODE;
    COLOR_(WRITE)?MASK yylval.glenum=GLCOLORWRITEMASK; return GLCOLORWRITEMASK;
    MULTISAMPLE yylval.glenum=GLMULTISAMPLE; return GLMULTISAMPLE;
    SAMPLE_MASK yylval.glenum=GLSAMPLEMASK; return GLSAMPLEMASK;
    SAMPLE_MASK_VALUE yylval.glenum=GLSAMPLEMASKVALUE; return GLSAMPLEMASKVALUE;
    SAMPLE_ALPHA_TO_ONE yylval.glenum=GLSAMPLEALPHATOONE; return GLSAMPLEALPHATOONE;
    SAMPLE_ALPHA_TO_COVERAGE yylval.glenum=GLSAMPLEALPHATOCOVERAGE; return GLSAMPLEALPHATOCOVERAGE;
}
<pass,dst_state>{
    \{  {
        return yytext[0];
    }
    \} {
        //yy_pop_state();
        return yytext[0];
    }
    DEPTH_TEST yylval.glenum=GLDEPTHTEST; return GLDEPTHTEST;
    DEPTH_FUNC yylval.glenum=GLDEPTHFUNC; return GLDEPTHFUNC;
    DEPTH_(WRITE)?MASK yylval.glenum=GLDEPTHWRITEMASK; return GLDEPTHWRITEMASK;
    DEPTH_CLAMP yylval.glenum=GLDEPTHCLAMP; return GLDEPTHCLAMP;
    DEPTH_BOUNDS_TEST_EXT yylval.glenum=GLDEPTHBOUNDSTESTEXT; return GLDEPTHBOUNDSTESTEXT;
    DEPTH_BOUNDS_EXT yylval.glenum=GLDEPTHBOUNDSEXT; return GLDEPTHBOUNDSEXT;
    STENCIL_TEST yylval.glenum=GLSTENCILTEST; return GLSTENCILTEST;
    STENCIL_(FRONT_)?FUNC yylval.glenum=GLSTENCILFUNC; return GLSTENCILFUNC;
    STENCIL_(FRONT_)?VALUE_MASK yylval.glenum=GLSTENCILVALUEMASK; return GLSTENCILVALUEMASK;
    STENCIL_(FRONT_)?REF yylval.glenum=GLSTENCILREF; return GLSTENCILREF;
    STENCIL_(FRONT_)?FAIL yylval.glenum=GLSTENCILFAIL; return GLSTENCILFAIL;
    STENCIL_(FRONT_)?PASS_DEPTH_FAIL yylval.glenum=GLSTENCILPASSDEPTHFAIL; return GLSTENCILPASSDEPTHFAIL;
    STENCIL_(FRONT_)?PASS_DEPTH_PASS yylval.glenum=GLSTENCILPASSDEPTHPASS; return GLSTENCILPASSDEPTHPASS;
    STENCIL_(FRONT_)?WRITEMASK yylval.glenum=GLSTENCILWRITEMASK; return GLSTENCILWRITEMASK;
    /*STENCIL_FRONT_FUNC yylval.glenum=GLSTENCILFRONTFUNC; return GLSTENCILFRONTFUNC;*/
    /*STENCIL_FRONT_VALUE_MASK yylval.glenum=GLSTENCILFRONTVALUEMASK; return GLSTENCILFRONTVALUEMASK;*/
    /*STENCIL_FRONT_REF yylval.glenum=GLSTENCILFRONTREF; return GLSTENCILFRONTREF;*/
    /*STENCIL_FRONT_FAIL yylval.glenum=GLSTENCILFRONTFAIL; return GLSTENCILFRONTFAIL;*/
    /*STENCIL_FRONT_PASS_DEPTH_FAIL yylval.glenum=GLSTENCILFRONTPASSDEPTHFAIL; return GLSTENCILFRONTPASSDEPTHFAIL;*/
    /*STENCIL_FRONT_PASS_DEPTH_PASS yylval.glenum=GLSTENCILFRONTPASSDEPTHPASS; return GLSTENCILFRONTPASSDEPTHPASS;*/
    /*STENCIL_FRONT_WRITEMASK yylval.glenum=GLSTENCILFRONTWRITEMASK; return GLSTENCILFRONTWRITEMASK;*/
    STENCIL_BACK_FUNC yylval.glenum=GLSTENCILBACKFUNC; return GLSTENCILBACKFUNC;
    STENCIL_BACK_VALUE_MASK yylval.glenum=GLSTENCILBACKVALUEMASK; return GLSTENCILBACKVALUEMASK;
    STENCIL_BACK_REF yylval.glenum=GLSTENCILBACKREF; return GLSTENCILBACKREF;
    STENCIL_BACK_FAIL yylval.glenum=GLSTENCILBACKFAIL; return GLSTENCILBACKFAIL;
    STENCIL_BACK_PASS_DEPTH_FAIL yylval.glenum=GLSTENCILBACKPASSDEPTHFAIL; return GLSTENCILBACKPASSDEPTHFAIL;
    STENCIL_BACK_PASS_DEPTH_PASS yylval.glenum=GLSTENCILBACKPASSDEPTHPASS; return GLSTENCILBACKPASSDEPTHPASS;
    STENCIL_BACK_WRITEMASK yylval.glenum=GLSTENCILBACKWRITEMASK; return GLSTENCILBACKWRITEMASK;
    StencilFuncSeparate return GLSTENCILFUNCSEPARATE;
    StencilOp return GLSTENCILOP;
    StencilOpSeparate return GLSTENCILOPSEPARATE;
    STENCILBACKOP return GLSTENCILBACKOP;
}
    /*==================================================================
      new GL enums for the Path rendering part
    */
<pass>{
    FOG_COORDINATE      yylval.glenum=GLFOGCOORDINATE; return GLFOGCOORDINATE;
    FRAGMENT_DEPTH      yylval.glenum=GLFRAGMENTDEPTH; return GLFRAGMENTDEPTH;
    OBJECT_LINEAR       yylval.glenum=GLOBJECTLINEAR; return GLOBJECTLINEAR;
    PATH_OBJECT_BOUNDING_BOX yylval.glenum=GLPATHOBJECTBOUNDINGBOX; return GLPATHOBJECTBOUNDINGBOX;
    EYE_LINEAR          yylval.glenum=GLEYELINEAR; return GLEYELINEAR;
    FLAT                   yylval.glenum=GLFLAT; return GLFLAT;
    SQUARE                 yylval.glenum=GLSQUARE; return GLSQUARE;
    ROUND                  yylval.glenum=GLROUND; return GLROUND;
    TRIANGULAR             yylval.glenum=GLTRIANGULAR; return GLTRIANGULAR;
    MITER_REVERT        yylval.glenum=GLMITERREVERT; return GLMITERREVERT;
    MITER_TRUNCATE      yylval.glenum=GLMITERTRUNCATE; return GLMITERTRUNCATE;
    BEVEL                  yylval.glenum=GLBEVEL; return GLBEVEL;
    MOVE_TO_RESET    yylval.glenum=GLMOVETORESET; return GLMOVETORESET;
    MOVE_TO_CONTINUES yylval.glenum=GLMOVETOCONTINUES; return GLMOVETOCONTINUES;
    COUNT_UP            yylval.glenum=GLCOUNTUP; return GLCOUNTUP;
    /*INVERT                 yylval.glenum=GLINVERT; return GLINVERT;*/
    COUNT_DOWN          yylval.glenum=GLCOUNTDOWN; return GLCOUNTDOWN;
    BOUNDING_BOX        yylval.glenum=GLBOUNDINGBOX; return GLBOUNDINGBOX;
    CONVEX_HULL         yylval.glenum=GLCONVEXHULL; return GLCONVEXHULL;
    TRANSLATE_2D        yylval.glenum=GLTRANSLATE2D; return GLTRANSLATE2D;
    TRANSLATE_X         yylval.glenum=GLTRANSLATEX; return GLTRANSLATEX;
    TRANSLATE_Y         yylval.glenum=GLTRANSLATEY; return GLTRANSLATEY;
    TRANSLATE_3D        yylval.glenum=GLTRANSLATE3D; return GLTRANSLATE3D;
    AFFINE_2D           yylval.glenum=GLAFFINE2D; return GLAFFINE2D;
    AFFINE_3D           yylval.glenum=GLAFFINE3D; return GLAFFINE3D;
    TRANSPOSE_AFFINE_2D  yylval.glenum=GLTRANSPOSEAFFINE2D; return GLTRANSPOSEAFFINE2D;
    TRANSPOSE_AFFINE_3D  yylval.glenum=GLTRANSPOSEAFFINE3D; return GLTRANSPOSEAFFINE3D;
}
    /*==================================================================
      States for NV Path rendering. TODO: make it available as a state group, too
      For now, we only expose them in the pass
      TODO: grammar of render state of state groups could be simplified by returning special token depending on type of assignment
            ==> We could indeed return always the same TOKEN for many similar states while yylval.glenum would be different
    */
<pass,pr_state>{
    \{  {
        return yytext[0];
    }
    \} {
        return yytext[0];
    }
    PATH_STENCIL_DEPTH_OFFSET yylval.glenum= GLPATHSTENCILDEPTHOFFSET; return GLPATHSTENCILDEPTHOFFSET;
    PATH_STENCIL_FUNC yylval.glenum= GLPATHSTENCILFUNC; return GLPATHSTENCILFUNC;
    PATH_FOG_GEN yylval.glenum= GLPATHFOGGEN; return GLPATHFOGGEN;
    PATH_TEX_GEN yylval.glenum= GLPATHTEXGEN; return GLPATHTEXGEN;
    PATH_COLOR_GEN yylval.glenum= GLPATHCOLORGEN; return GLPATHCOLORGEN;
    PATH_COVER_DEPTH_FUNC yylval.glenum= GLPATHCOVERDEPTHFUNC; return GLPATHCOVERDEPTHFUNC;
    PATH_STROKE_WIDTH yylval.glenum= GLPATHSTROKEWIDTH; return GLPATHSTROKEWIDTH;
    PATH_INITIAL_END_CAP yylval.glenum= GLPATHINITIALENDCAP; return GLPATHINITIALENDCAP;
    PATH_TERMINAL_END_CAP yylval.glenum= GLPATHTERMINALENDCAP; return GLPATHTERMINALENDCAP;
    PATH_INITIAL_DASH_CAP yylval.glenum= GLPATHINITIALDASHCAP; return GLPATHINITIALDASHCAP;
    PATH_TERMINAL_DASH_CAP yylval.glenum= GLPATHTERMINALDASHCAP; return GLPATHTERMINALDASHCAP;
    PATH_JOIN_STYLE yylval.glenum= GLPATHJOINSTYLE; return GLPATHJOINSTYLE;
    PATH_MITER_LIMIT yylval.glenum= GLPATHMITERLIMIT; return GLPATHMITERLIMIT;
    PATH_DASH_OFFSET yylval.glenum= GLPATHDASHOFFSET; return GLPATHDASHOFFSET;
    PATH_DASH_OFFSET_RESET yylval.glenum= GLPATHDASHOFFSETRESET; return GLPATHDASHOFFSETRESET;
    PATH_CLIENT_LENGTH yylval.glenum= GLPATHCLIENTLENGTH; return GLPATHCLIENTLENGTH;
    PATH_FILL_MODE yylval.glenum= GLPATHFILLMODE; return GLPATHFILLMODE;
    PATH_FILL_MASK yylval.glenum= GLPATHFILLMASK; return GLPATHFILLMASK;
    PATH_FILL_COVER_MODE yylval.glenum= GLPATHFILLCOVERMODE; return GLPATHFILLCOVERMODE;
    PATH_STROKE_COVER_MODE yylval.glenum= GLPATHSTROKECOVERMODE; return GLPATHSTROKECOVERMODE;
    PATH_STROKE_MASK yylval.glenum= GLPATHSTROKEMASK; return GLPATHSTROKEMASK;
    PATH_STROKE_REFERENCE yylval.glenum= GLPATHSTROKEREFERENCE; return GLPATHSTROKEREFERENCE;
    PATH_TRANSFORM_TYPE yylval.glenum= GLPATHTRANSFORMTYPE; return GLPATHTRANSFORMTYPE;
    PATH_TRANSFORM yylval.glenum= GLPATHTRANSFORM; return GLPATHTRANSFORM;
}
    /*==================================================================
      Start of string
    */
<INITIAL,resource,resource1,pass,technique,cstbuffer,shader>
{
    \"    string_buf_ptr  =  string_buf;  yy_push_state(str);
}
    /*==================================================================
      fallback for any other character : sent as a token ID
    */
    /*. return yytext[0];*/

    /*==================================================================
      Comment starting points
    */
<INITIAL,str,shader,cstbuffer,technique,pass,sampler_state,dst_state,pr_state,color_sample_state,rasterization_state,resource,resource1,fbo,fbo1>"/*" {
    comment_caller  =  INITIAL;
    yy_push_state(comment);
}

<INITIAL,str,shader,cstbuffer,technique,pass,sampler_state,dst_state,pr_state,color_sample_state,rasterization_state,resource,resource1,fbo,fbo1>"//" {
    comment_caller  =  INITIAL;
    yy_push_state(comment2);
}

<comment>{
    [^*\n]* /*  eat  anything  that's  not  a  '*'  */
    "*"+[^*/\n]*     /*  eat  up  '*'s  not  followed  by  '/'s  */
    \n ++line_num;
    "*"+"/" yy_pop_state();//BEGIN(comment_caller);
}

<comment2>{
    .*
    \n ++line_num;  yy_pop_state();//BEGIN(comment_caller);
}

    /*==================================================================
      rules for inside a string
    */
<str>{
    \"    {  /*  saw  closing  quote  -  all  done  */
        yy_pop_state();//BEGIN(INITIAL);
        *string_buf_ptr  =  '\0';
        yylval.s = new std::string(string_buf);
        return FX_STR;
    }
    \n    {
        fprintf(stderr,"\nunterminated  string  constant at line %d\n", line_num);
        yyterminate();
    }
    \\[0-7]{1,3}    {
        /*  octal  escape  sequence  */
        int  result;
        (void)  sscanf(  yytext  +  1,  "%o",  &result  );
        if  (  result  >  0xff  )
            /*  error,  constant  is  out-of-bounds  */
        *string_buf_ptr++  =  result;
    }
    \\[0-9]+    {
        /*  generate  error  -  bad  escape  sequence;  something
        *  like  '\48'  or  '\0777777'
        */
    }
    \\n    *string_buf_ptr++  =  '\n';
    \\t    *string_buf_ptr++  =  '\t';
    \\r    *string_buf_ptr++  =  '\r';
    \\b    *string_buf_ptr++  =  '\b';
    \\f    *string_buf_ptr++  =  '\f';
    \\(.|\n)    *string_buf_ptr++  =  yytext[1];
    [^\\\n\"]+ {
        char  *yptr  =  yytext;
        while  (  *yptr  )
        *string_buf_ptr++  =  *yptr++;
    }
}

    /*==================================================================
      rules for inside a GLSL section
    */
<shader>{
    \{  {
        bracket_level = 1; // must be one...
        string_buf_ptr  =  string_buf;
        *string_buf_ptr = '\0';
        yy_push_state(shaderbody);
        return yytext[0];
    }
    {VARIABLE} {
        // TODO create and REGISTER the variable in a table
        yylval.s = new std::string(yytext);
        // NOTE: "::" is really not friendly with GLSL (and other shading languages... ?)
        int n = (int)yylval.s->find("::");
        if(n==0)
            yylval.s->replace(0, 2, "");
        if(n>0)
            yylval.s->replace(n, 2, "_NS_");
        return VARNAME;
    }
}
<shaderbody>{
    \{  {
        bracket_level++;
        *string_buf_ptr++  =  yytext[0];
    }
    \} {
        bracket_level--;
        if((bracket_level) == 0)
        {
            *string_buf_ptr  =  '\0';
            yylval.s = new std::string(string_buf);
            // get rid of possible namespaces (uniforms could have some, because of nvFX namespace)
            // This is for now the only case 
            while(1)
            {
                int n = (int)yylval.s->find("::");
                if(n<0)
                    break;
                yylval.s->replace(n, 2, "_NS_");
            }
            // shall I just do BEGIN(INITIAL) ?
            yy_pop_state(); // back to shader
            yy_pop_state();// back to INITIAL
            return CODEBODY;
        } else {
            *string_buf_ptr++  =  yytext[0];
        }
    }
    \n {  /*copy the GLSL data*/
        char  *yptr  =  yytext;
        while  (  *yptr  )
        *string_buf_ptr++  =  *yptr++;
        ++line_num; 
    }
    [^\n^\{^\}]+ {  /*copy the GLSL data*/
        char  *yptr  =  yytext;
        while  (  *yptr  )
        *string_buf_ptr++  =  *yptr++;
    }
}
    /*==================================================================
      rules for inside a  section
    */
<technique>{
    Pass {
        comment_caller  =  INITIAL;
        yy_push_state(pass);
        return PASS;
    }
    \{  {
        return yytext[0];
    }
    \} {
        //yy_pop_state();//BEGIN(INITIAL);
        return yytext[0];
    }
    Technique return TECHNIQUE;
    OFF return OFF;
}
    /*==================================================================
      rules for inside a  section
    */
<INITIAL,pass>{
    /*\{  {
        return yytext[0];
    }
    \} {
        //yy_pop_state();
        return yytext[0];
    }*/
    OptixContext        return OPTIX_CONTEXT;
    OptixContextEntry   return OPTIX_CONTEXT_ENTRY;
    VertexProgram       return VERTEXPROGRAM;
    /*VertexShader        return VERTEXPROGRAM;*/
    FragmentProgram     return FRAGMENTPROGRAM;
    /*FragmentShader      return FRAGMENTPROGRAM;*/
    /*PixelShader         return FRAGMENTPROGRAM;*/
    GeometryProgram     return GEOMETRYPROGRAM;
    /*GeometryShader      return GEOMETRYPROGRAM;*/
    /*HullShader          return HULLPROGRAM;*/
    /*TCSShader           return HULLPROGRAM;*/
    TCSProgram          return HULLPROGRAM;
    /*EvalShader          return EVALPROGRAM;*/
    /*EvalProgram         return EVALPROGRAM;*/
    /*TESShader           return EVALPROGRAM;*/
    TESProgram          return EVALPROGRAM;
    ComputeProgram      return COMPUTEPROGRAM;
    /*ComputeShader       return COMPUTEPROGRAM;*/
    PathSource          return PATHSOURCE;

    VertexProgramOverride   return VERTEXPROGRAMOVERRIDE;
    /*VertexShaderOverride    return VERTEXPROGRAMOVERRIDE;*/
    FragmentProgramOverride return FRAGMENTPROGRAMOVERRIDE;
    /*FragmentShaderOverride  return FRAGMENTPROGRAMOVERRIDE;*/
    /*PixelShaderOverride     return FRAGMENTPROGRAMOVERRIDE;*/
    GeometryProgramOverride return GEOMETRYPROGRAMOVERRIDE;
    /*GeometryShaderOverride  return GEOMETRYPROGRAMOVERRIDE;*/
    /*HullShaderOverride      return HULLPROGRAMOVERRIDE;*/
    /*EvalShaderOverride      return EVALPROGRAMOVERRIDE;*/
    TCSProgramOverride      return HULLPROGRAMOVERRIDE;
    TESProgramOverride      return EVALPROGRAMOVERRIDE;
    ComputeProgramOverride  return COMPUTEPROGRAMOVERRIDE;
    ComputeShaderOverride   return COMPUTEPROGRAMOVERRIDE;

    Profile                     return SHDPROFILE;
    ImageAccess           return IMAGEACCESS;
    ImageLayer            return IMAGELAYER;
    ImageLayered          return IMAGELAYERED;
    WRITE_ONLY            return WRITE_ONLY;
    READ_ONLY             return READ_ONLY;
    READ_WRITE            return READ_WRITE;
    ImageResource         return IMAGERESOURCE;
    Uniform               return UNIFORM;
    ImageUnit             return IMAGEUNIT;
    TextureResource       return TEXTURERESOURCE;
    SamplerResource       return SAMPLERRESOURCE;
    SamplerTexUnit        return SAMPLERTEXUNIT;
    SamplerState          return SETSAMPLERSTATE;
    DepthStencilState     return SETDSTSTATE;
    RasterizationState    return SETRASTERIZATIONSTATE;
    ColorSampleState      return SETCOLORSAMPLESTATE;

    ImageAccessOverride     return IMAGEACCESSOVERRIDE;
    ImageLayerOverride      return IMAGELAYEROVERRIDE;
    ImageLayeredOverride    return IMAGELAYEREDOVERRIDE;
    ImageResourceOverride   return IMAGERESOURCEOVERRIDE;
    UniformOverride         return UNIFORMOVERRIDE;
    ImageUnitOverride       return IMAGEUNITOVERRIDE;
    TextureResourceOverride return TEXTURERESOURCEOVERRIDE;
    SamplerResourceOverride return SAMPLERRESOURCEOVERRIDE;
    SamplerTexUnitOverride  return SAMPLERTEXUNITOVERRIDE;
    SamplerStateOverride    return SETSAMPLERSTATEOVERRIDE;

    RenderMode {
        yy_push_state(rendermode);
        return RENDERMODE;
    }
    RenderGroup             return RENDERGROUP;
    BlitFBOToActiveTarget   return BLITFBOTOACTIVETARGET;
    ClearColor              return CLEARCOLOR;
    CurrentTarget           return CURRENTTARGET;
    Viewport                return VIEWPORT;
    SwapResources           return SWAPRESOURCES;

    Loop                    return LOOP;
    Active                  return ACTIVE;

    CudaModule              return CUDAMODULE;
    CudaKernel              return CUDAKERNEL;
    CudaSharedMemory        return CUDASHAREDMEMORY;
    CudaGrid                return CUDAGRID;
    CudaBlock               return CUDABLOCK;

    ComputeGroups           return COMPUTEGROUPS;
    ClearMode {
        yy_push_state(clearmode);
        return CLEARMODE;
    }
}
<clearmode>{
    \= return '=';
    ; { yy_pop_state(); return ';'; }
    \| return '|';
    none { yylval.clear_mode = nvFX::Clear_none; return CLEAR_ENUM; }
    all { yylval.clear_mode = nvFX::Clear_all; return CLEAR_ENUM; }
    COLOR_BUFFER(_BIT)? { yylval.clear_mode = nvFX::Clear_color; return CLEAR_ENUM; }
    DEPTH_BUFFER(_BIT)? { yylval.clear_mode = nvFX::Clear_depth; return CLEAR_ENUM; }
    STENCIL_BUFFER(_BIT)? { yylval.clear_mode = nvFX::Clear_stencil; return CLEAR_ENUM; }
    color { yylval.clear_mode = nvFX::Clear_color; return CLEAR_ENUM; }
    depth { yylval.clear_mode = nvFX::Clear_depth; return CLEAR_ENUM; }
    stencil { yylval.clear_mode = nvFX::Clear_stencil; return CLEAR_ENUM; }
    color_depth { yylval.clear_mode = nvFX::Clear_color_depth; return CLEAR_ENUM; }
    depth_stencil { yylval.clear_mode = nvFX::Clear_depth_stencil; return CLEAR_ENUM; }
}
<rendermode>{
    \= return '=';
    Optix { yy_pop_state(); yylval.render_mode = nvFX::RENDER_OPTIX; return RENDERMODE_ENUM; }
    SceneShading { yy_pop_state(); yylval.render_mode = nvFX::RENDER_SCENEGRAPH_SHADED; return RENDERMODE_ENUM; }
    SceneNoShading { yy_pop_state(); yylval.render_mode = nvFX::RENDER_SCENEGRAPH_NOSHADING; return RENDERMODE_ENUM; }
    fullscreen_quad { yy_pop_state(); yylval.render_mode = nvFX::RENDER_FULLSCREEN_QUAD;return RENDERMODE_ENUM; }
    SceneShadingOpaqueOnly { yy_pop_state(); yylval.render_mode = nvFX::RENDER_SCENEGRAPH_SHADED_OPAQUE_ONLY;return RENDERMODE_ENUM; }
    SceneShadingTransparentOnly { yy_pop_state(); yylval.render_mode = nvFX::RENDER_SCENEGRAPH_SHADED_TRANSPARENT_ONLY;return RENDERMODE_ENUM; }
    SceneNoShadingOpaqueOnly { yy_pop_state(); yylval.render_mode = nvFX::RENDER_SCENEGRAPH_NOSHADING_OPAQUE_ONLY;return RENDERMODE_ENUM; }
    SceneNoShadingTransparentOnly { yy_pop_state(); yylval.render_mode = nvFX::RENDER_SCENEGRAPH_NOSHADING_TRANSPARENT_ONLY;return RENDERMODE_ENUM; }
    SceneOutlinesOnly { yy_pop_state(); yylval.render_mode = nvFX::RENDER_SCENEGRAPH_OUTLINES_ONLY;return RENDERMODE_ENUM; }
    DebugQuad0 { yy_pop_state(); yylval.render_mode = nvFX::RENDER_DEBUG_QUAD0;return RENDERMODE_ENUM; }
    DebugQuad1 { yy_pop_state(); yylval.render_mode = nvFX::RENDER_DEBUG_QUAD1;return RENDERMODE_ENUM; }
    DebugQuad2 { yy_pop_state(); yylval.render_mode = nvFX::RENDER_DEBUG_QUAD2;return RENDERMODE_ENUM; }
    DebugQuad3 { yy_pop_state(); yylval.render_mode = nvFX::RENDER_DEBUG_QUAD3;return RENDERMODE_ENUM; }
    DebugScene { yy_pop_state(); yylval.render_mode = nvFX::RENDER_DEBUG_SCENE;return RENDERMODE_ENUM; }
    DebugSceneLines { yy_pop_state(); yylval.render_mode = nvFX::RENDER_DEBUG_SCENE_LINES;return RENDERMODE_ENUM; }
    DoNothing { yy_pop_state(); yylval.render_mode = nvFX::DO_NOTHING;return RENDERMODE_ENUM; }
    OptixReflection { yy_pop_state(); yylval.render_mode = nvFX::RENDER_OPTIX_REFLECTION;return RENDERMODE_ENUM; }
    OptixShadow { yy_pop_state(); yylval.render_mode = nvFX::RENDER_OPTIX_SHADOW;return RENDERMODE_ENUM; }
    OptixReflectionAndShadow { yy_pop_state(); yylval.render_mode = nvFX::RENDER_OPTIX_REFLECTION_AND_SHADOW;return RENDERMODE_ENUM; }
    Optix { yy_pop_state(); yylval.render_mode = nvFX::RENDER_OPTIX;return RENDERMODE_ENUM; }
    CUDA { yy_pop_state(); yylval.render_mode = nvFX::RUN_CUDA;return RENDERMODE_ENUM; }
    GLSLCompute { yy_pop_state(); yylval.render_mode = nvFX::RUN_GLSLCOMPUTE;return RENDERMODE_ENUM; }
    Custom0 { yy_pop_state(); yylval.render_mode = nvFX::RENDER_CUSTOM0;return RENDERMODE_ENUM; }
    Custom1 { yy_pop_state(); yylval.render_mode = nvFX::RENDER_CUSTOM1;return RENDERMODE_ENUM; }
    Custom2 { yy_pop_state(); yylval.render_mode = nvFX::RENDER_CUSTOM2;return RENDERMODE_ENUM; }
    Custom3 { yy_pop_state(); yylval.render_mode = nvFX::RENDER_CUSTOM3;return RENDERMODE_ENUM; }
    Custom4 { yy_pop_state(); yylval.render_mode = nvFX::RENDER_CUSTOM4;return RENDERMODE_ENUM; }
    Custom5 { yy_pop_state(); yylval.render_mode = nvFX::RENDER_CUSTOM5;return RENDERMODE_ENUM; }
    Custom6 { yy_pop_state(); yylval.render_mode = nvFX::RENDER_CUSTOM6;return RENDERMODE_ENUM; }
    Custom7 { yy_pop_state(); yylval.render_mode = nvFX::RENDER_CUSTOM7;return RENDERMODE_ENUM; }
    Custom8 { yy_pop_state(); yylval.render_mode = nvFX::RENDER_CUSTOM8;return RENDERMODE_ENUM; }
    Custom9 { yy_pop_state(); yylval.render_mode = nvFX::RENDER_CUSTOM9;return RENDERMODE_ENUM; }
    StencilFillPath           { yy_pop_state(); yylval.render_mode = nvFX::RENDER_STENCILFILLPATH;return RENDERMODE_ENUM; }
    StencilStrokePath  { yy_pop_state(); yylval.render_mode = nvFX::RENDER_STENCILSTROKEPATH;return RENDERMODE_ENUM; }
    CoverFillPath      { yy_pop_state(); yylval.render_mode = nvFX::RENDER_COVERFILLPATH;return RENDERMODE_ENUM; }
    CoverStrokePath    { yy_pop_state(); yylval.render_mode = nvFX::RENDER_COVERSTROKEPATH;return RENDERMODE_ENUM; }
}
<pass,technique>{
    Attribute           return ATTRIBUTE;
}
    /*==================================================================
      rules for inside a  FBO section 
    */
<fbo1>{
    \{  {
        yy_push_state(fbo); //leads to 'fbo' (fbo1 is a fast transition part)
        return yytext[0];
    }
    ; {
        yy_pop_state();
        return yytext[0];
    }
}
    /*==================================================================
      rules for inside a  FBO section 
    */
<fbo>{
    \{  {
        return yytext[0];
    }
    \} {
        //yy_pop_state(); // back to resource1
        //yy_pop_state(); // back to the one before resource1
        return yytext[0];
    }
    Color        return COLORTARGETS;
    DepthStencil return DSTTARGET;
}
    /*==================================================================
      rules for inside a  texture section 
    */
<resource1>{
    \{  {
        yy_push_state(resource); //leads to resource' (resource1 is a fast transition part)
        return yytext[0];
    }
}
    /*==================================================================
      rules for inside a  texture section 
    */
<INITIAL,resource>{
    \{  {
        return yytext[0];
    }
    \} {
        //yy_pop_state(); // back to resource1
        //yy_pop_state(); // back to the one before resource1
        return yytext[0];
    }
    SamplerState return SETSAMPLERSTATE;
    ApplicationDefined  return APPDEFINED;
    Default             return APPDEFINED;
    MSAA                return RESOURCEMSAA;
    Format              return RESOURCEFORMAT;
    Size                return RESOURCESIZE;
    SizeFactor          return RESOURCESIZEFACTOR;
    ResolveTarget       return RESOURCERESOLVETARGET;
    RGBA8               yylval.resource_format = FMT_RGBA8; return RESOURCEFORMAT_ENUM;
    RGBA16F             yylval.resource_format = FMT_RGBA16F; return RESOURCEFORMAT_ENUM;
    RGBA32F             yylval.resource_format = FMT_RGBA32F; return RESOURCEFORMAT_ENUM;
    INTENSITY8          yylval.resource_format = FMT_INTENSITY8; return RESOURCEFORMAT_ENUM;
    INTENSITY16F        yylval.resource_format = FMT_INTENSITY16F; return RESOURCEFORMAT_ENUM;
    INTENSITY32F        yylval.resource_format = FMT_INTENSITY32F; return RESOURCEFORMAT_ENUM;
    LUMINANCE_ALPHA8    yylval.resource_format = FMT_LUMINANCE_ALPHA8; return RESOURCEFORMAT_ENUM;
    RG16F_NV            yylval.resource_format = FMT_RG16F_NV; return RESOURCEFORMAT_ENUM;
    LUMINANCE_ALPHA16F  yylval.resource_format = FMT_LUMINANCE_ALPHA16F; return RESOURCEFORMAT_ENUM;
    LUMINANCE_ALPHA32F  yylval.resource_format = FMT_LUMINANCE_ALPHA32F; return RESOURCEFORMAT_ENUM;
    RG32F_NV            yylval.resource_format = FMT_RG32F_NV; return RESOURCEFORMAT_ENUM;
    RGB8                yylval.resource_format = FMT_RGB8; return RESOURCEFORMAT_ENUM;
    RGB16F              yylval.resource_format = FMT_RGB16F; return RESOURCEFORMAT_ENUM;
    RGB32F              yylval.resource_format = FMT_RGB32F; return RESOURCEFORMAT_ENUM;
    DEPTH24STENCIL8     yylval.resource_format = FMT_DEPTH24STENCIL8; return RESOURCEFORMAT_ENUM;
    DEPTH_COMPONENT32F  yylval.resource_format = FMT_DEPTH_COMPONENT32F; return RESOURCEFORMAT_ENUM;
    DEPTH32F_STENCIL8   yylval.resource_format = FMT_DEPTH32F_STENCIL8; return RESOURCEFORMAT_ENUM;
    RGBA8UI             yylval.resource_format = FMT_RGBA8UI; return RESOURCEFORMAT_ENUM;
    FLOAT_RGBA16_NV     yylval.resource_format = FMT_FLOATRGBA16NV; return RESOURCEFORMAT_ENUM;
    RG                  yylval.resource_format = FMT_RG                  ; return RESOURCEFORMAT_ENUM;
    RG_INTEGER          yylval.resource_format = FMT_RG_INTEGER          ; return RESOURCEFORMAT_ENUM;
    R8                  yylval.resource_format = FMT_R8                  ; return RESOURCEFORMAT_ENUM;
    R16                 yylval.resource_format = FMT_R16                 ; return RESOURCEFORMAT_ENUM;
    RG8                 yylval.resource_format = FMT_RG8                 ; return RESOURCEFORMAT_ENUM;
    RG16                yylval.resource_format = FMT_RG16                ; return RESOURCEFORMAT_ENUM;
    R16F                yylval.resource_format = FMT_R16F                ; return RESOURCEFORMAT_ENUM;
    R32F                yylval.resource_format = FMT_R32F                ; return RESOURCEFORMAT_ENUM;
    RG16F               yylval.resource_format = FMT_RG16F               ; return RESOURCEFORMAT_ENUM;
    RG32F               yylval.resource_format = FMT_RG32F               ; return RESOURCEFORMAT_ENUM;
    R8I                 yylval.resource_format = FMT_R8I                 ; return RESOURCEFORMAT_ENUM;
    R8UI                yylval.resource_format = FMT_R8UI                ; return RESOURCEFORMAT_ENUM;
    R16I                yylval.resource_format = FMT_R16I                ; return RESOURCEFORMAT_ENUM;
    R16UI               yylval.resource_format = FMT_R16UI               ; return RESOURCEFORMAT_ENUM;
    R32I                yylval.resource_format = FMT_R32I                ; return RESOURCEFORMAT_ENUM;
    R32UI               yylval.resource_format = FMT_R32UI               ; return RESOURCEFORMAT_ENUM;
    RG8I                yylval.resource_format = FMT_RG8I                ; return RESOURCEFORMAT_ENUM;
    RG8UI               yylval.resource_format = FMT_RG8UI               ; return RESOURCEFORMAT_ENUM;
    RG16I               yylval.resource_format = FMT_RG16I               ; return RESOURCEFORMAT_ENUM;
    RG16UI              yylval.resource_format = FMT_RG16UI              ; return RESOURCEFORMAT_ENUM;
    RG32I               yylval.resource_format = FMT_RG32I               ; return RESOURCEFORMAT_ENUM;
    RG32UI              yylval.resource_format = FMT_RG32UI              ; return RESOURCEFORMAT_ENUM;
}
    /*==================================================================
      rules for inside a  section
    */
<sampler_state>{
    \{  {
        return yytext[0];
    }
    \} {
        //yy_pop_state();
        return yytext[0];
    }
    TEXTURE_MIN_FILTER    yylval.glenum=GLTEXTUREMINFILTER; return GLTEXTUREMINFILTER;
    TEXTURE_MAG_FILTER    yylval.glenum=GLTEXTUREMAGFILTER; return GLTEXTUREMAGFILTER;
    TEXTURE_WRAP_S        yylval.glenum=GLTEXTUREWRAPS; return GLTEXTUREWRAPS;
    TEXTURE_WRAP_T        yylval.glenum=GLTEXTUREWRAPT; return GLTEXTUREWRAPT;
    TEXTURE_WRAP_R        yylval.glenum=GLTEXTUREWRAPR; return GLTEXTUREWRAPR;
    /* D3D keywords
    AddressS                          yylval.glenum=GLTEXTUREWRAPS; return GLTEXTUREWRAPS;
    AddressT                          yylval.glenum=GLTEXTUREWRAPT; return GLTEXTUREWRAPT;
    AddressR                          yylval.glenum=GLTEXTUREWRAPR; return GLTEXTUREWRAPR;
    AddressU                          yylval.glenum=GLTEXTUREWRAPS; return GLTEXTUREWRAPS;
    AddressV                          yylval.glenum=GLTEXTUREWRAPT; return GLTEXTUREWRAPT;
    AddressW                          yylval.glenum=GLTEXTUREWRAPR; return GLTEXTUREWRAPR;*/
    LINEAR_MIPMAP_LINEAR  yylval.glenum=GLLINEARMIPMAPLINEAR; return GLLINEARMIPMAPLINEAR;
    LINEAR              yylval.glenum=GLLINEAR; return GLLINEAR;
    CLAMP               yylval.glenum=GLCLAMP; return GLCLAMP;
    CLAMP_TO_EDGE       yylval.glenum=GLCLAMPTOEDGE; return GLCLAMPTOEDGE;
    REPEAT              yylval.glenum=GLREPEAT; return GLREPEAT;
    NEAREST             yylval.glenum=GLNEAREST; return GLNEAREST;
    MIRRORED_REPEAT     yylval.glenum=GLMIRROREDREPEAT; return GLMIRROREDREPEAT;
    MIRROR              yylval.glenum=GLMIRROREDREPEAT; return GLMIRROREDREPEAT;
}

    /*==================================================================
      Variable
    */
<INITIAL,cstbuffer,technique,sampler_state,dst_state,pr_state,color_sample_state,rasterization_state,pass,resource,resource1,fbo,fbo1>{VARIABLE} {
    // TODO create and REGISTER the variable in a table
    yylval.s = new std::string(yytext);
        // NOTE: "::" is really not friendly with GLSL (and other shading languages... ?)
    int n = (int)yylval.s->find("::");
    if(n==0)
        yylval.s->replace(0, 2, "");
    if(n>0)
        yylval.s->replace(n, 2, "_NS_");
    return VARNAME;
}
    /*==================================================================
      strip space and tabs
    */
<INITIAL,cstbuffer,sampler_state,dst_state,pr_state,color_sample_state,rasterization_state,technique,pass,shader,resource,resource1,fbo,fbo1>[ \t]+ {
}
    /*==================================================================
      New Line : count and strip it out
    */
<INITIAL,cstbuffer,sampler_state,dst_state,pr_state,color_sample_state,rasterization_state,technique,pass,shader,resource,resource1,fbo,fbo1>\n {
    ++line_num;
    /*return yytext[0];*/
}
    /*==================================================================
      other chars
    */
<INITIAL,cstbuffer,sampler_state,dst_state,pr_state,color_sample_state,rasterization_state,technique,pass,shader,resource,resource1,fbo,fbo1>. {
    if((yytext[0] >= 33) && (yytext[0] <= 126))
        return yytext[0];
    else {
        nvFX::printf("Warning: line %d : odd character found (%u)...\n", line_num, (unsigned char)yytext[0]);
    }
}
    /*==================================================================
      ==================================================================
      !! SIMPLIFIED PREPROCESSOR !!
      More complex preprocessor would require a complete parser that would
      pre-process the code. Here we avoid doing so : directly loading nested 
      includes : Not even using the grammar analyzer !
      ==================================================================
      ==================================================================
    */
#include BEGIN(incl);
<incl>[<" \t]* /*  eat  the  whitespace and " or < as often in #include */
<incl>[^ \t\n]+     {  /*  got  the  include  file  name  */
    char * s = strchr(yytext, '\"'); // get rid of the quote or >
    if(!s) s = strchr(yytext, '>');
    if(s)
        *s = '\0';
    include_stack.push(IncludeData(YY_CURRENT_BUFFER, line_num, file_name.c_str()));
    file_name = std::string(yytext);
    line_num  = 1;
    FILE *fd = NULL;
    const char *buf = NULL;
    if(nvFX::g_includeCb)
        nvFX::g_includeCb(yytext, fd, buf);
    if(buf)
        yy_switch_to_buffer( yy_scan_string(buf) );
    else if(fd)
        yy_switch_to_buffer( yy_create_buffer(  fd,  YY_BUF_SIZE  )  );
    else {
        fd  =  fopen(  yytext,  "r"  );
        if(!fd)
        {
            fprintf(  stderr,  "err loading file %s", yytext);
        } else {
            yy_switch_to_buffer( yy_create_buffer(  fd,  YY_BUF_SIZE  )  );
        }
    }
    if(fd)
    {
        // let's keep track of this newly opened file : when poping, we will have to close it
        IncludeData &incData = include_stack.top();
        incData.fileToClose = fd;
        BEGIN(INITIAL);
    } else {
        YY_FATAL_ERROR( "failure in including a file" );
    }
}
<<EOF>> {
    if(include_stack.empty())//(  --include_stack_ptr  <  0  )
    {
        yyterminate();
    }
    else
    {
        yy_delete_buffer(  YY_CURRENT_BUFFER  );
        IncludeData &incData = include_stack.top();
        yy_switch_to_buffer(incData.state);//[include_stack_ptr]);
        line_num = incData.line_num;
        file_name = incData.file_name;
        if(incData.fileToClose)
            fclose(incData.fileToClose);
        include_stack.pop();
    }
}

