
/* A Bison parser, made by GNU Bison 2.4.1.  */

/* Skeleton implementation for Bison's Yacc-like parsers in C
   
      Copyright (C) 1984, 1989, 1990, 2000, 2001, 2002, 2003, 2004, 2005, 2006
   Free Software Foundation, Inc.
   
   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.
   
   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.
   
   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.  */

/* As a special exception, you may create a larger work that contains
   part or all of the Bison parser skeleton and distribute that work
   under terms of your choice, so long as that work isn't itself a
   parser generator using the skeleton or a modified version thereof
   as a parser skeleton.  Alternatively, if you modify or redistribute
   the parser skeleton itself, you may (at your option) remove this
   special exception, which will cause the skeleton and the resulting
   Bison output files to be licensed under the GNU General Public
   License without this special exception.
   
   This special exception was added by the Free Software Foundation in
   version 2.2 of Bison.  */

/* C LALR(1) parser skeleton written by Richard Stallman, by
   simplifying the original so-called "semantic" parser.  */

/* All symbols defined below should begin with yy or YY, to avoid
   infringing on user name space.  This should be done even for local
   variables, as they might otherwise be expanded by user macros.
   There are some unavoidable exceptions within include files to
   define necessary library symbols; they are noted "INFRINGES ON
   USER NAME SPACE" below.  */

/* Identify Bison output.  */
#define YYBISON 1

/* Bison version.  */
#define YYBISON_VERSION "2.4.1"

/* Skeleton name.  */
#define YYSKELETON_NAME "yacc.c"

/* Pure parsers.  */
#define YYPURE 0

/* Push parsers.  */
#define YYPUSH 0

/* Pull parsers.  */
#define YYPULL 1

/* Using locations.  */
#define YYLSP_NEEDED 0

/* Substitute the variable and function names.  */
#define yyparse         FxParserparse
#define yylex           FxParserlex
#define yyerror         FxParsererror
#define yylval          FxParserlval
#define yychar          FxParserchar
#define yydebug         FxParserdebug
#define yynerrs         FxParsernerrs


/* Copy the first part of user declarations.  */

/* Line 189 of yacc.c  */
#line 36 "Grammar.ypp"

    /* Documentation on the keywords and grammar created in : ..\doxygen\Keywords.dxt */
    #include  <math.h>
    #include  <assert.h>
    #include  <stdio.h>
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
	#else
	#include <stdlib.h>
	#endif
    #include  <string.h>
    #include  <vector>
    #include  <map>
    #include  <string>

    #include "FxLibEx.h"
    #include "FxParser.h"

    using namespace nvFX;
    #ifndef LOGD
    #define LOGD(...)  { /*nvFX::printf(__VA_ARGS__);*/ }
    #endif
    #ifndef LOGI
    #define LOGI(...)  { nvFX::printf(__VA_ARGS__); }
    #endif

    #ifndef OGLES2
    #   define USEGLSLCOMPUTE
    #endif
    /*
     * values taken from OpenGL
     *
     */
    #define GL_TRUE 1
    #define GL_FALSE 0


    /*
     * Here we store temporary pointers of current objects being processed by the grammar
     * the reason : the grammar executes from the deepest part to the highest part of the tree
     * and the FX library doesn the opposite : objects are created by parents
     */
    IContainerEx*       curContainer = NULL;
    ITechniqueEx*       curTechnique;
    IPassEx*            curPass;
    IShaderEx*          curShader;
    IProgramEx*         curProgram;
    ISamplerStateEx*    curSamplerState;
    IStateGroupRasterEx* curRState;
    IStateGroupCSEx*    curCSState;
    IStateGroupDSTEx*   curDSTState;
    IStateGroupPathEx*  curPRState;
    IResourceEx*        curResource;
    IFrameBufferObjectEx* curFBO;
    IUniformEx*         curUniform;
    IAnnotationEx*      curAnnotations;
    IAnnotationEx*      namespaceAnnotations;
    ICstBufferEx*       curCstBuffer;
    bool                bCurCstBufferAlreadyPopulated; // true to prevent creation of sub-uniforms
    std::string         namespaceName;
    IShaderModuleRepositoryEx* pShdRep;
    IFrameBufferObjectsRepositoryEx* pFboRep;
    IResourceRepositoryEx* pResRep;
    IUniformRepositoryEx* pURep;
    ICstBufferRepositoryEx* pCRep;
    //
    // Those functions are used to create the states if NULL
    // this case happens if states are defined in a Pass directly :
    // we will then create implicit stategroups for this pass
    //
    void check_curDSTState()
    {
        if(curDSTState) return;
        LOGD("no current DST State Group. We are in a Pass, Create one for this Pass\n");
        assert(curPass);
        curDSTState = curContainer->createStateGroup_DST(curPass->getName())->getExInterface();
        curPass->createState(curDSTState);
    }
    void check_curRState()
    {
        if(curRState) return;
        LOGD("no current Rasterization State Group. We are in a Pass, Create one for this Pass\n");
        assert(curPass);
        curRState = curContainer->createStateGroup_Raster(curPass->getName())->getExInterface();
        curPass->createState(curRState);
    }
    void check_curCSState()
    {
        if(curCSState) return;
        LOGD("no current Color sample State Group. We are in a Pass, Create one for this Pass\n");
        assert(curPass);
        curCSState = curContainer->createStateGroup_CS(curPass->getName())->getExInterface();
        curPass->createState(curCSState);
    }
#ifndef OGLES2
    void check_curPRState()
    {
        if(curPRState) return;
        LOGI("no current rendering-path State Group. We are in a Pass, Create one for this Pass\n");
        assert(curPass);
        curPRState = curContainer->createStateGroup_PR(curPass->getName())->getExInterface();
        curPass->createState(curPRState);
    }
#endif
    extern GLenum getGLEnumValue(GLenum BisonEnum);

    void createUniform(nvFX::IUniform::Type T, const char* name, const char* sem, bool global=true, IUniform::PrecisionType precision=IUniform::PHighP)
    {
        extern void yyerror(char  const  *);
        IUniform *p = NULL;
        if(curCstBuffer) // if a constant buffer is available, populate it
        {
            // only if allowed (if it came from an existing Global one, we don't want to add anything
            if(bCurCstBufferAlreadyPopulated) {
                LOGD("Skipping the sub-uniform of the constant\n");
                p = curCstBuffer->findUniform(name);
                if(p == NULL)
                    yyerror("Declaration of the CstBuffer differs from the one already in global area\n");
            } else {
                p = curCstBuffer->getExInterface()->createUniform(name, sem);
                p->getExInterface()->setType(T);
            }
        } else { // otherwise just make a regular uniform creation at the root of the effect
            // TODO: we should take into account the name-space !!
            p = curContainer->findUniform(name);
            if(!p)
            {
                p = curContainer->createUniform(name, namespaceName.c_str(), sem, global);
                if(p) {
                    p->getExInterface()->setType(T);
                    p->getExInterface()->setPrecision(precision);
                }
            }
        }
        curUniform = p ? p->getExInterface() : NULL;
    }
    inline int getTypeDim(IUniform::Type t)
    {
        switch(t)
        {
        case IUniform::TFloat:
        case IUniform::TInt:
        case IUniform::TBool:
            return 1;
        case IUniform::TVec2:
        case IUniform::TInt2:
        case IUniform::TBool2:
            return 2;
        case IUniform::TVec3:
        case IUniform::TInt3:
        case IUniform::TBool3:
            return 3;
        case IUniform::TVec4:
        case IUniform::TInt4:
        case IUniform::TBool4:
            return 4;
        }
        return 0;
    }


/* Line 189 of yacc.c  */
#line 260 "D:/FromTiers/git/nvFX/FxParser/Grammar.cpp"

/* Enabling traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif

/* Enabling verbose error messages.  */
#ifdef YYERROR_VERBOSE
# undef YYERROR_VERBOSE
# define YYERROR_VERBOSE 1
#else
# define YYERROR_VERBOSE 0
#endif

/* Enabling the token table.  */
#ifndef YYTOKEN_TABLE
# define YYTOKEN_TABLE 0
#endif

/* "%code requires" blocks.  */

/* Line 209 of yacc.c  */
#line 214 "Grammar.ypp"

    #include  <vector>
    #include  <string>
    #include  "FxLibEx.h"
    using namespace nvFX;

    /*#define yyparse         FxParserparse
    #define yylex           FxParserlex
    #define yyerror         FxParsererror
    #define yylval          FxParserlval
    #define yychar          FxParserchar
    #define yydebug         FxParserdebug
    #define yynerrs         FxParsernerrs

    #define yy_create_buffer FxParser_create_buffer
    #define yy_delete_buffer FxParser_delete_buffer
    #define yy_scan_buffer FxParser_scan_buffer
    #define yy_scan_string FxParser_scan_string
    #define yy_scan_bytes FxParser_scan_bytes
    #define yy_flex_debug FxParser_flex_debug
    #define yy_init_buffer FxParser_init_buffer
    #define yy_flush_buffer FxParser_flush_buffer
    #define yy_load_buffer_state FxParser_load_buffer_state
    #define yy_switch_to_buffer FxParser_switch_to_buffer
    #define yyin FxParserin
    #define yyleng FxParserleng
    #define yylex FxParserlex
    #define yyout FxParserout
    #define yyrestart FxParserrestart
    #define yytext FxParsertext
    #define yylineno FxParserlineno
    #define yywrap FxParserwrap
    */
    #pragma warning(disable:4996)

    extern int  yydebug;
    extern int  yyparse ();
    extern void yyerror(char  const  *);
    extern int  yylex();
    extern void lex_pop_state();
    extern void lex_push_state();
    extern FILE *yyin;
    extern FILE *yyout;
    extern std::string file_name; // the name of the current file (when in #include, for example)
    extern int  line_num;
    extern int  bracket_level;

    extern IContainerEx*   curContainer;

    //typedef std::vector<CgShader*>      CgShaderList;
    typedef std::vector<IShader*>        ShaderList;
    typedef std::vector<float>          FloatList;
    typedef std::vector<FloatList* >    FloatVecList; ///<vector list
    typedef std::vector<int>            IntList;
    typedef std::vector<IntList* >      IntVecList; ///<vector list
    typedef std::vector</*bool*/int>    BoolList; // bool ran me into compile err in XCode
    typedef std::vector<BoolList* >     BoolVecList; ///<vector list
    typedef std::vector<std::string>    VarNameList;
    struct Argument {
        Argument(ArgumentType type_, std::string *name_) : type(type_), name(*name_) {};
        ArgumentType    type;
        std::string     name;
    };
    typedef std::vector<Argument*>          ArgList;
    typedef std::vector<ArgVal*>            ArgValList;
    struct SUniformQualifiers
    {
        bool hasUniformKeyword; // not really necessary... but hey...
        bool global;
        IUniform::PrecisionType precisionType;
    };
    struct ValueList
    {
        FloatVecList*   floatVecList;
        IntVecList*     intVecList;
        BoolVecList*    boolVecList;
    };
    struct SASzSAUva
    {
        int             arraySz;
        std::string*    semantic;
        ValueList*      valueList;
    };
    struct Sglobal_varname
    {
        bool            global;
        std::string*    varName;
    };

    /*struct Variable
    {
        std::string name;
        //...
    };*/

    #ifndef LOGD
    #define  LOGD(...)  { /*nvFX::printf(__VA_ARGS__);*/ }
    #endif
    #ifndef LOGI
    #define  LOGI(...)  { ::printf(__VA_ARGS__); }
    #endif
    extern float *flattenFVecList(int t, int vecDim, FloatVecList* pfloatVecList);
    extern int   *flattenIVecList(int t, int vecDim, IntVecList* pintVecList);
    extern bool  *flattenBVecList(int t, int vecDim, BoolVecList* pboolVecList);

    //
    // Check 
    //
    inline bool checkArgCompatibility(ArgVal *argval, IShaderEx::Argument &arg)
    {
        if(argval->type == IUniform::TUniform)
        {
            ArgVal argval2;
            argval2.type = argval->val.pUniform->getType();
            // recursive walk through to finally check the real time
            return checkArgCompatibility(&argval2, arg);
        }
        switch(arg.t)
        {
        case TInt:
        case TUInt:
            if(argval->type == IUniform::TInt)
                return true;
            break;
        case TInt2:
            if(argval->type == IUniform::TInt2)
                return true;
            break;
        case TInt3:
            if(argval->type == IUniform::TInt3)
                return true;
            break;
        case TInt4:
            if(argval->type == IUniform::TInt4)
                return true;
            break;
        //case TUInt2:        case TUInt3:        case TUInt4:
        case TFloat:
            if(argval->type == IUniform::TFloat)
                return true;
            if(argval->type == IUniform::TInt)
                return true;
            break;
        //case TVec2:        case TVec3:        case TVec4:
        case TIntPtr:
        case TUIntPtr:
        case TFloatPtr:
            switch(argval->type)
            {
            case IUniform::TCB:
            case IUniform::TTexture:
            case IUniform::TTexture1D:
            case IUniform::TTexture2D:
            case IUniform::TTexture2DShadow:
            case IUniform::TTexture2DRect:
            case IUniform::TTexture3D:
            case IUniform::TTextureCube:
                return true;
            default:
                break;
            }
            return false;
        //case TInt2Ptr:        case TInt3Ptr:        case TInt4Ptr:
        //case TUInt2Ptr:        case TUInt3Ptr:        case TUInt4Ptr:
        //case TVec2Ptr:        case TVec3Ptr:        case TVec4Ptr:
        //TBool, TBool2, TBool3, TBool4,
        //TMat2, TMat3, TMat4, 
            break;
        }
        return false;
    }



/* Line 209 of yacc.c  */
#line 459 "D:/FromTiers/git/nvFX/FxParser/Grammar.cpp"

/* Tokens.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
   /* Put the tokens into the symbol table, so that GDB and other debuggers
      know about them.  */
   enum yytokentype {
     NAMESPACE = 258,
     FX_TRUE = 259,
     FX_FALSE = 260,
     FX_FLOAT = 261,
     FX_INT = 262,
     FX_BOOL = 263,
     FX_STR = 264,
     GLSLSHADER = 265,
     HLSL10SHADER = 266,
     HLSL11SHADER = 267,
     CGSHADER = 268,
     SAMPLER_STATE = 269,
     DST_STATE = 270,
     PR_STATE = 271,
     COLOR_SAMPLE_STATE = 272,
     RASTERIZATION_STATE = 273,
     TECHNIQUE = 274,
     PASS = 275,
     CODEBODY = 276,
     VARNAME = 277,
     TEXTURERESOURCE = 278,
     TEXTURERESOURCE1D = 279,
     TEXTURERESOURCE2D = 280,
     TEXTURERESOURCE3D = 281,
     TEXTURERESOURCERECT = 282,
     TEXTURERESOURCECUBE = 283,
     VOID_TYPE = 284,
     UNSIGNED = 285,
     HIGHP = 286,
     MEDIUMP = 287,
     LOWP = 288,
     UNIFORM = 289,
     CSTBUFFER = 290,
     FLOAT_TYPE = 291,
     FLOAT2_TYPE = 292,
     FLOAT3_TYPE = 293,
     FLOAT4_TYPE = 294,
     MAT2_TYPE = 295,
     MAT3_TYPE = 296,
     MAT4_TYPE = 297,
     BOOL_TYPE = 298,
     BOOL2_TYPE = 299,
     BOOL3_TYPE = 300,
     BOOL4_TYPE = 301,
     INT_TYPE = 302,
     INT2_TYPE = 303,
     INT3_TYPE = 304,
     INT4_TYPE = 305,
     TEXTURE1D_TYPE = 306,
     TEXTURE2D_TYPE = 307,
     TEXTURE2DSHADOW_TYPE = 308,
     TEXTURE2DRECT_TYPE = 309,
     TEXTURE3D_TYPE = 310,
     TEXTURECUBE_TYPE = 311,
     SAMPLER1D_TYPE = 312,
     SAMPLER2D_TYPE = 313,
     SAMPLER2DSHADOW_TYPE = 314,
     SAMPLER2DRECT_TYPE = 315,
     SAMPLER3D_TYPE = 316,
     SAMPLERCUBE_TYPE = 317,
     EXTENSION = 318,
     SEPARATE_SHADER = 319,
     DEPTHMASK = 100,
     DEPTHFUNC = 101,
     VERTEXPROGRAM = 320,
     FRAGMENTPROGRAM = 321,
     GEOMETRYPROGRAM = 322,
     HULLPROGRAM = 323,
     EVALPROGRAM = 324,
     SHDPROFILE = 325,
     SAMPLERRESOURCE = 326,
     SAMPLERTEXUNIT = 327,
     SETSAMPLERSTATE = 328,
     SETDSTSTATE = 329,
     SETRASTERIZATIONSTATE = 330,
     SETCOLORSAMPLESTATE = 331,
     IMAGERESOURCE = 332,
     IMAGEUNIT = 333,
     IMAGEACCESS = 334,
     IMAGELAYER = 335,
     IMAGELAYERED = 336,
     WRITE_ONLY = 337,
     READ_ONLY = 338,
     READ_WRITE = 339,
     VERTEXPROGRAMOVERRIDE = 340,
     FRAGMENTPROGRAMOVERRIDE = 341,
     GEOMETRYPROGRAMOVERRIDE = 342,
     HULLPROGRAMOVERRIDE = 343,
     EVALPROGRAMOVERRIDE = 344,
     IMAGEACCESSOVERRIDE = 345,
     IMAGELAYEROVERRIDE = 346,
     IMAGELAYEREDOVERRIDE = 347,
     IMAGERESOURCEOVERRIDE = 348,
     UNIFORMOVERRIDE = 349,
     IMAGEUNITOVERRIDE = 350,
     TEXTURERESOURCEOVERRIDE = 351,
     SAMPLERRESOURCEOVERRIDE = 352,
     SAMPLERTEXUNITOVERRIDE = 353,
     SETSAMPLERSTATEOVERRIDE = 354,
     GLNONE = 355,
     GLACCUM = 356,
     GLLOAD = 357,
     GLRETURN = 358,
     GLMULT = 359,
     GLADD = 360,
     GLNEVER = 361,
     GLLESS = 362,
     GLEQUAL = 363,
     GLGREATER = 364,
     GLNOTEQUAL = 365,
     GLALWAYS = 366,
     GLLEQUAL = 367,
     GLGEQUAL = 368,
     GLKEEP = 369,
     GLREPLACE = 370,
     GLINCR = 371,
     GLDECR = 372,
     GLUPPERLEFT = 373,
     GLLOWERLEFT = 374,
     GLFRONTLEFT = 375,
     GLFRONTRIGHT = 376,
     GLBACKLEFT = 377,
     GLBACKRIGHT = 378,
     GLFRONT = 379,
     GLBACK = 380,
     GLLEFT = 381,
     GLRIGHT = 382,
     GLFRONTANDBACK = 383,
     GLCCW = 384,
     GLCW = 385,
     GLPOINT = 386,
     GLLINE = 387,
     GLFILL = 388,
     GLZERO = 389,
     GLONE = 390,
     GLSRCCOLOR = 391,
     GLONEMINUSSRCCOLOR = 392,
     GLSRCALPHA = 393,
     GLONEMINUSSRCALPHA = 394,
     GLDSTALPHA = 395,
     GLONEMINUSDSTALPHA = 396,
     GLDSTCOLOR = 397,
     GLONEMINUSDSTCOLOR = 398,
     GLSRCALPHASATURATE = 399,
     GLCONSTANTCOLOR = 400,
     GLONEMINUSCONSTANTCOLOR = 401,
     GLCONSTANTALPHA = 402,
     GLONEMINUSCONSTANTALPHA = 403,
     GLFUNCADD = 404,
     GLMIN = 405,
     GLMAX = 406,
     GLFUNCSUBTRACT = 407,
     GLFUNCREVERSESUBTRACT = 408,
     GLCLEAR = 409,
     GLSET = 410,
     GLCOPY = 411,
     GLCOPYINVERTED = 412,
     GLNOOP = 413,
     GLINVERT = 414,
     GLAND = 415,
     GLNAND = 416,
     GLOR = 417,
     GLNOR = 418,
     GLXOR = 419,
     GLEQUIV = 420,
     GLANDREVERSE = 421,
     GLANDINVERTED = 422,
     GLORREVERSE = 423,
     GLORINVERTED = 424,
     GLTEXTUREMAGFILTER = 425,
     GLTEXTUREMINFILTER = 426,
     GLTEXTUREWRAPS = 427,
     GLTEXTUREWRAPT = 428,
     GLTEXTUREWRAPR = 429,
     GLNEAREST = 430,
     GLLINEAR = 431,
     GLNEARESTMIPMAPNEAREST = 432,
     GLLINEARMIPMAPNEAREST = 433,
     GLNEARESTMIPMAPLINEAR = 434,
     GLLINEARMIPMAPLINEAR = 435,
     GLCLAMPTOEDGE = 436,
     GLCLAMP = 437,
     GLREPEAT = 438,
     GLMIRROREDREPEAT = 439,
     GLTEXTURECOMPAREMODE = 440,
     GLCOMPARERTOTEXTURE = 441,
     GLTEXTURECOMPAREFUNC = 442,
     GLDEPTHTEXTUREMODE = 443,
     GLPOINTSIZE = 444,
     GLPOINTSMOOTH = 445,
     GLPOINTSPRITE = 446,
     GLPROGRAMPOINTSIZE = 447,
     GLPOINTSPRITECOORDORIGIN = 448,
     GLLINEWIDTH = 449,
     GLLINESMOOTH = 450,
     GLCOMBINEDLINESTIPPLE = 451,
     GLLINESTIPPLEPATTERN = 452,
     GLLINESTIPPLEREPEAT = 453,
     GLLINESTIPPLE = 454,
     GLCULLFACE = 455,
     GLCULLFACEMODE = 456,
     GLFRONTFACE = 457,
     GLPOLYGONSMOOTH = 458,
     GLPOLYGONMODE = 459,
     GLPOLYGONOFFSET = 460,
     GLPOLYGONOFFSETFACTOR = 461,
     GLPOLYGONOFFSETUNITS = 462,
     GLPOLYGONOFFSETPOINT = 463,
     GLPOLYGONOFFSETLINE = 464,
     GLPOLYGONOFFSETFILL = 465,
     GLPOLYGONSTIPPLEBIT = 466,
     GLRASTERIZERDISCARD = 467,
     GLALPHATEST = 468,
     GLALPHAFUNC = 469,
     GLALPHATESTFUNC = 470,
     GLALPHATESTREF = 471,
     GLBLENDFUNC = 472,
     GLBLENDFUNCSEPARATE = 473,
     GLBLEND = 474,
     GLBLENDSRC = 475,
     GLBLENDSRCRGB = 476,
     GLBLENDSRCALPHA = 477,
     GLBLENDDST = 478,
     GLBLENDDSTRGB = 479,
     GLBLENDDSTALPHA = 480,
     GLBLENDEQUATIONSEPARATE = 481,
     GLBLENDEQUATION = 482,
     GLBLENDEQUATIONRGB = 483,
     GLBLENDEQUATIONALPHA = 484,
     GLBLENDCOLOR = 485,
     GLFRAMEBUFFERSRGB = 486,
     GLDITHER = 487,
     GLCOLORLOGICOP = 488,
     GLLOGICOPMODE = 489,
     GLCOLORWRITEMASK = 490,
     GLMULTISAMPLE = 491,
     GLSAMPLEMASK = 492,
     GLSAMPLEMASKVALUE = 493,
     GLSAMPLEALPHATOONE = 494,
     GLSAMPLEALPHATOCOVERAGE = 495,
     GLDEPTHTEST = 496,
     GLDEPTHFUNC = 497,
     GLDEPTHWRITEMASK = 498,
     GLDEPTHCLAMP = 499,
     GLDEPTHBOUNDSTESTEXT = 500,
     GLDEPTHBOUNDSEXT = 501,
     GLSTENCILTEST = 502,
     GLSTENCILFUNC = 503,
     GLSTENCILFUNCSEPARATE = 504,
     GLSTENCILVALUEMASK = 505,
     GLSTENCILREF = 506,
     GLSTENCILFAIL = 507,
     GLSTENCILPASSDEPTHFAIL = 508,
     GLSTENCILPASSDEPTHPASS = 509,
     GLSTENCILOP = 510,
     GLSTENCILOPSEPARATE = 511,
     GLSTENCILBACKOP = 512,
     GLSTENCILWRITEMASK = 513,
     GLSTENCILBACKFUNC = 514,
     GLSTENCILBACKVALUEMASK = 515,
     GLSTENCILBACKREF = 516,
     GLSTENCILBACKFAIL = 517,
     GLSTENCILBACKPASSDEPTHFAIL = 518,
     GLSTENCILBACKPASSDEPTHPASS = 519,
     GLSTENCILBACKWRITEMASK = 520,
     ATTRIBUTE = 521,
     RENDERGROUP = 522,
     RENDERMODE = 523,
     RENDERMODE_ENUM = 524,
     BLITFBOTOACTIVETARGET = 525,
     CLEARMODE = 526,
     CLEAR_ENUM = 527,
     CURRENTTARGET = 528,
     CLEARCOLOR = 529,
     VIEWPORT = 530,
     FBO = 531,
     RENDERBUFFER = 532,
     RENDERTEXTURE = 533,
     RENDERTEXTURERECT = 534,
     DSTTARGET = 535,
     COLORTARGETS = 536,
     RESOURCEMSAA = 537,
     RESOURCESIZE = 538,
     RESOURCESIZEFACTOR = 539,
     RESOURCERESOLVETARGET = 540,
     RESOURCEFORMAT = 541,
     RESOURCEFORMAT_ENUM = 542,
     APPDEFINED = 543,
     SWAPRESOURCES = 544,
     LOOP = 545,
     ACTIVE = 546,
     OFF = 547,
     GLOBAL = 548,
     LOCAL = 549,
     CUDAKERNEL = 550,
     CUDACODE = 551,
     CUDAMODULE = 552,
     CUDASHAREDMEMORY = 553,
     CUDAGRID = 554,
     CUDABLOCK = 555,
     OPTIX_CONTEXT = 556,
     OPTIX_CONTEXT_ENTRY = 557,
     RAY_GENERATION_PROGRAM = 558,
     EXCEPTION_PROGRAM = 559,
     INTERSECTION_PROGRAM = 560,
     BOUNDING_BOX_PROGRAM = 561,
     MISS_PROGRAM = 562,
     DEFAULT_MATERIAL = 563,
     STACK_SIZE = 564,
     OUTPUT_BUFFER = 565,
     OPTIX_MATERIAL = 566,
     CLOSEST_HIT_PROGRAM = 567,
     ANY_HIT_PROGRAM = 568,
     OPTIX_BUFFER = 569,
     PTX_PROGRAM_FROM_FILE = 570,
     FILE_NAME = 571,
     ENTRY_POINT = 572,
     COMPUTEGROUPS = 573,
     COMPUTEPROGRAM = 574,
     COMPUTEPROGRAMOVERRIDE = 575,
     SVGPATH = 576,
     PSPATH = 577,
     PATHSOURCEOVERRIDE = 578,
     PATHSOURCE = 579,
     GLPATHSTENCILDEPTHOFFSET = 580,
     GLPATHSTENCILFUNC = 581,
     GLPATHFOGGEN = 582,
     GLPATHTEXGEN = 583,
     GLPATHCOLORGEN = 584,
     GLPATHCOVERDEPTHFUNC = 585,
     GLPATHSTROKEWIDTH = 586,
     GLPATHINITIALENDCAP = 587,
     GLPATHTERMINALENDCAP = 588,
     GLPATHINITIALDASHCAP = 589,
     GLPATHTERMINALDASHCAP = 590,
     GLPATHJOINSTYLE = 591,
     GLPATHMITERLIMIT = 592,
     GLPATHDASHOFFSET = 593,
     GLPATHDASHOFFSETRESET = 594,
     GLPATHCLIENTLENGTH = 595,
     GLPATHFILLMODE = 596,
     GLPATHFILLMASK = 597,
     GLPATHFILLCOVERMODE = 598,
     GLPATHSTROKEMASK = 599,
     GLPATHSTROKEREFERENCE = 600,
     GLPATHSTROKECOVERMODE = 601,
     GLPATHTRANSFORMTYPE = 602,
     GLPATHTRANSFORM = 603,
     GLFOGCOORDINATE = 604,
     GLFRAGMENTDEPTH = 605,
     GLOBJECTLINEAR = 606,
     GLPATHOBJECTBOUNDINGBOX = 607,
     GLEYELINEAR = 608,
     GLFLAT = 609,
     GLSQUARE = 610,
     GLROUND = 611,
     GLTRIANGULAR = 612,
     GLMITERREVERT = 613,
     GLMITERTRUNCATE = 614,
     GLBEVEL = 615,
     GLMOVETORESET = 616,
     GLMOVETOCONTINUES = 617,
     GLCOUNTUP = 618,
     GLCOUNTDOWN = 619,
     GLBOUNDINGBOX = 620,
     GLCONVEXHULL = 621,
     GLTRANSLATE2D = 622,
     GLTRANSLATEX = 623,
     GLTRANSLATEY = 624,
     GLTRANSLATE3D = 625,
     GLAFFINE2D = 626,
     GLAFFINE3D = 627,
     GLTRANSPOSEAFFINE2D = 628,
     GLTRANSPOSEAFFINE3D = 629,
     GLLUMINANCE = 630,
     GLALPHA = 631,
     GLINTENSITY = 632,
     GLLUMINANCEALPHA = 633,
     GLRGB = 634,
     GLRGBA = 635,
     NEG = 636
   };
#endif



#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
typedef union YYSTYPE
{

/* Line 214 of yacc.c  */
#line 417 "Grammar.ypp"

    bool            bval;
    bool*           pbval;
    int             ival;
    int*            pival;
    float           fval;
    float*          pfval;
    std::string*    s; // a std::string sub-class to help to deal with "namespace"
    VarNameList*    varNameList;
    //CgShaderList*   cgShaderList;
    ShaderList*     shaderList;
    FloatList*      floatList;
    FloatVecList*   floatVecList;
    IntList*        intList;
    IntVecList*     intVecList;
    BoolList*       boolList;
    BoolVecList*    boolVecList;
    GLenum          glenum;
    IPassState::Type passstateType;
    IUniform::Type  uniformType;
    ResourceType    resourceType;
    /*Variable*       var;*/
    RenderingMode   render_mode;
    ClearMode       clear_mode;
    ResourceFormat  resource_format;
    ArgList*        argList;
    Argument*       argument;
    ArgVal*         argVal;
    ArgValList*     argValList;
    SASzSAUva*      saszsauva;
    IUniform::PrecisionType precisionType;
    SUniformQualifiers uniformQualifiers;
    ValueList*      valueList;
    Sglobal_varname global_varname_annots;



/* Line 214 of yacc.c  */
#line 897 "D:/FromTiers/git/nvFX/FxParser/Grammar.cpp"
} YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
#endif


/* Copy the second part of user declarations.  */


/* Line 264 of yacc.c  */
#line 909 "D:/FromTiers/git/nvFX/FxParser/Grammar.cpp"

#ifdef short
# undef short
#endif

#ifdef YYTYPE_UINT8
typedef YYTYPE_UINT8 yytype_uint8;
#else
typedef unsigned char yytype_uint8;
#endif

#ifdef YYTYPE_INT8
typedef YYTYPE_INT8 yytype_int8;
#elif (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
typedef signed char yytype_int8;
#else
typedef short int yytype_int8;
#endif

#ifdef YYTYPE_UINT16
typedef YYTYPE_UINT16 yytype_uint16;
#else
typedef unsigned short int yytype_uint16;
#endif

#ifdef YYTYPE_INT16
typedef YYTYPE_INT16 yytype_int16;
#else
typedef short int yytype_int16;
#endif

#ifndef YYSIZE_T
# ifdef __SIZE_TYPE__
#  define YYSIZE_T __SIZE_TYPE__
# elif defined size_t
#  define YYSIZE_T size_t
# elif ! defined YYSIZE_T && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
#  include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  define YYSIZE_T size_t
# else
#  define YYSIZE_T unsigned int
# endif
#endif

#define YYSIZE_MAXIMUM ((YYSIZE_T) -1)

#ifndef YY_
# if YYENABLE_NLS
#  if ENABLE_NLS
#   include <libintl.h> /* INFRINGES ON USER NAME SPACE */
#   define YY_(msgid) dgettext ("bison-runtime", msgid)
#  endif
# endif
# ifndef YY_
#  define YY_(msgid) msgid
# endif
#endif

/* Suppress unused-variable warnings by "using" E.  */
#if ! defined lint || defined __GNUC__
# define YYUSE(e) ((void) (e))
#else
# define YYUSE(e) /* empty */
#endif

/* Identity function, used to suppress warnings about constant conditions.  */
#ifndef lint
# define YYID(n) (n)
#else
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static int
YYID (int yyi)
#else
static int
YYID (yyi)
    int yyi;
#endif
{
  return yyi;
}
#endif

#if ! defined yyoverflow || YYERROR_VERBOSE

/* The parser invokes alloca or malloc; define the necessary symbols.  */

# ifdef YYSTACK_USE_ALLOCA
#  if YYSTACK_USE_ALLOCA
#   ifdef __GNUC__
#    define YYSTACK_ALLOC __builtin_alloca
#   elif defined __BUILTIN_VA_ARG_INCR
#    include <alloca.h> /* INFRINGES ON USER NAME SPACE */
#   elif defined _AIX
#    define YYSTACK_ALLOC __alloca
#   elif defined _MSC_VER
#    include <malloc.h> /* INFRINGES ON USER NAME SPACE */
#    define alloca _alloca
#   else
#    define YYSTACK_ALLOC alloca
#    if ! defined _ALLOCA_H && ! defined _STDLIB_H && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
#     include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#     ifndef _STDLIB_H
#      define _STDLIB_H 1
#     endif
#    endif
#   endif
#  endif
# endif

# ifdef YYSTACK_ALLOC
   /* Pacify GCC's `empty if-body' warning.  */
#  define YYSTACK_FREE(Ptr) do { /* empty */; } while (YYID (0))
#  ifndef YYSTACK_ALLOC_MAXIMUM
    /* The OS might guarantee only one guard page at the bottom of the stack,
       and a page size can be as small as 4096 bytes.  So we cannot safely
       invoke alloca (N) if N exceeds 4096.  Use a slightly smaller number
       to allow for a few compiler-allocated temporary stack slots.  */
#   define YYSTACK_ALLOC_MAXIMUM 4032 /* reasonable circa 2006 */
#  endif
# else
#  define YYSTACK_ALLOC YYMALLOC
#  define YYSTACK_FREE YYFREE
#  ifndef YYSTACK_ALLOC_MAXIMUM
#   define YYSTACK_ALLOC_MAXIMUM YYSIZE_MAXIMUM
#  endif
#  if (defined __cplusplus && ! defined _STDLIB_H \
       && ! ((defined YYMALLOC || defined malloc) \
	     && (defined YYFREE || defined free)))
#   include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#   ifndef _STDLIB_H
#    define _STDLIB_H 1
#   endif
#  endif
#  ifndef YYMALLOC
#   define YYMALLOC malloc
#   if ! defined malloc && ! defined _STDLIB_H && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
void *malloc (YYSIZE_T); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
#  ifndef YYFREE
#   define YYFREE free
#   if ! defined free && ! defined _STDLIB_H && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
void free (void *); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
# endif
#endif /* ! defined yyoverflow || YYERROR_VERBOSE */


#if (! defined yyoverflow \
     && (! defined __cplusplus \
	 || (defined YYSTYPE_IS_TRIVIAL && YYSTYPE_IS_TRIVIAL)))

/* A type that is properly aligned for any stack member.  */
union yyalloc
{
  yytype_int16 yyss_alloc;
  YYSTYPE yyvs_alloc;
};

/* The size of the maximum gap between one aligned stack and the next.  */
# define YYSTACK_GAP_MAXIMUM (sizeof (union yyalloc) - 1)

/* The size of an array large to enough to hold all stacks, each with
   N elements.  */
# define YYSTACK_BYTES(N) \
     ((N) * (sizeof (yytype_int16) + sizeof (YYSTYPE)) \
      + YYSTACK_GAP_MAXIMUM)

/* Copy COUNT objects from FROM to TO.  The source and destination do
   not overlap.  */
# ifndef YYCOPY
#  if defined __GNUC__ && 1 < __GNUC__
#   define YYCOPY(To, From, Count) \
      __builtin_memcpy (To, From, (Count) * sizeof (*(From)))
#  else
#   define YYCOPY(To, From, Count)		\
      do					\
	{					\
	  YYSIZE_T yyi;				\
	  for (yyi = 0; yyi < (Count); yyi++)	\
	    (To)[yyi] = (From)[yyi];		\
	}					\
      while (YYID (0))
#  endif
# endif

/* Relocate STACK from its old location to the new one.  The
   local variables YYSIZE and YYSTACKSIZE give the old and new number of
   elements in the stack, and YYPTR gives the new location of the
   stack.  Advance YYPTR to a properly aligned location for the next
   stack.  */
# define YYSTACK_RELOCATE(Stack_alloc, Stack)				\
    do									\
      {									\
	YYSIZE_T yynewbytes;						\
	YYCOPY (&yyptr->Stack_alloc, Stack, yysize);			\
	Stack = &yyptr->Stack_alloc;					\
	yynewbytes = yystacksize * sizeof (*Stack) + YYSTACK_GAP_MAXIMUM; \
	yyptr += yynewbytes / sizeof (*yyptr);				\
      }									\
    while (YYID (0))

#endif

/* YYFINAL -- State number of the termination state.  */
#define YYFINAL  2
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   5118

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  403
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  128
/* YYNRULES -- Number of rules.  */
#define YYNRULES  769
/* YYNRULES -- Number of states.  */
#define YYNSTATES  2153

/* YYTRANSLATE(YYLEX) -- Bison symbol number corresponding to YYLEX.  */
#define YYUNDEFTOK  2
#define YYMAXUTOK   636

#define YYTRANSLATE(YYX)						\
  ((unsigned int) (YYX) <= YYMAXUTOK ? yytranslate[YYX] : YYUNDEFTOK)

/* YYTRANSLATE[YYLEX] -- Bison symbol number corresponding to YYLEX.  */
static const yytype_uint16 yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     392,   393,   386,   385,   394,   384,   402,   387,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,   391,   390,
     398,   383,   399,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,   400,     2,   401,   389,     2,     2,     2,     2,     2,
      65,    66,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,   395,   397,   396,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     1,     2,     3,     4,
       5,     6,     7,     8,     9,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    21,    22,    23,    24,
      25,    26,    27,    28,    29,    30,    31,    32,    33,    34,
      35,    36,    37,    38,    39,    40,    41,    42,    43,    44,
      45,    46,    47,    48,    49,    50,    51,    52,    53,    54,
      55,    56,    57,    58,    59,    60,    61,    62,    63,    64,
      67,    68,    69,    70,    71,    72,    73,    74,    75,    76,
      77,    78,    79,    80,    81,    82,    83,    84,    85,    86,
      87,    88,    89,    90,    91,    92,    93,    94,    95,    96,
      97,    98,    99,   100,   101,   102,   103,   104,   105,   106,
     107,   108,   109,   110,   111,   112,   113,   114,   115,   116,
     117,   118,   119,   120,   121,   122,   123,   124,   125,   126,
     127,   128,   129,   130,   131,   132,   133,   134,   135,   136,
     137,   138,   139,   140,   141,   142,   143,   144,   145,   146,
     147,   148,   149,   150,   151,   152,   153,   154,   155,   156,
     157,   158,   159,   160,   161,   162,   163,   164,   165,   166,
     167,   168,   169,   170,   171,   172,   173,   174,   175,   176,
     177,   178,   179,   180,   181,   182,   183,   184,   185,   186,
     187,   188,   189,   190,   191,   192,   193,   194,   195,   196,
     197,   198,   199,   200,   201,   202,   203,   204,   205,   206,
     207,   208,   209,   210,   211,   212,   213,   214,   215,   216,
     217,   218,   219,   220,   221,   222,   223,   224,   225,   226,
     227,   228,   229,   230,   231,   232,   233,   234,   235,   236,
     237,   238,   239,   240,   241,   242,   243,   244,   245,   246,
     247,   248,   249,   250,   251,   252,   253,   254,   255,   256,
     257,   258,   259,   260,   261,   262,   263,   264,   265,   266,
     267,   268,   269,   270,   271,   272,   273,   274,   275,   276,
     277,   278,   279,   280,   281,   282,   283,   284,   285,   286,
     287,   288,   289,   290,   291,   292,   293,   294,   295,   296,
     297,   298,   299,   300,   301,   302,   303,   304,   305,   306,
     307,   308,   309,   310,   311,   312,   313,   314,   315,   316,
     317,   318,   319,   320,   321,   322,   323,   324,   325,   326,
     327,   328,   329,   330,   331,   332,   333,   334,   335,   336,
     337,   338,   339,   340,   341,   342,   343,   344,   345,   346,
     347,   348,   349,   350,   351,   352,   353,   354,   355,   356,
     357,   358,   359,   360,   361,   362,   363,   364,   365,   366,
     367,   368,   369,   370,   371,   372,   373,   374,   375,   376,
     377,   378,   379,   380,   381,   382,   388
};

#if YYDEBUG
/* YYPRHS[YYN] -- Index of the first RHS symbol of rule number YYN in
   YYRHS.  */
static const yytype_uint16 yyprhs[] =
{
       0,     0,     3,     4,     7,    10,    13,    16,    19,    22,
      25,    28,    31,    34,    37,    40,    43,    46,    49,    52,
      55,    58,    61,    64,    67,    70,    73,    76,    79,    82,
      87,    89,    91,    93,    97,   101,   105,   109,   113,   117,
     121,   125,   129,   133,   137,   141,   145,   148,   152,   156,
     160,   164,   168,   170,   174,   178,   182,   185,   189,   191,
     193,   195,   197,   199,   201,   203,   205,   207,   209,   211,
     212,   214,   216,   218,   222,   223,   225,   229,   230,   232,
     236,   238,   240,   242,   244,   248,   252,   255,   259,   265,
     268,   270,   274,   278,   284,   287,   289,   293,   295,   297,
     299,   301,   302,   304,   306,   310,   313,   315,   317,   319,
     321,   323,   325,   327,   329,   331,   333,   335,   337,   339,
     341,   343,   345,   347,   349,   351,   353,   355,   357,   359,
     361,   363,   365,   367,   369,   371,   373,   375,   377,   379,
     381,   383,   385,   387,   389,   391,   393,   395,   397,   399,
     401,   403,   405,   407,   409,   411,   413,   415,   417,   419,
     421,   423,   425,   427,   429,   431,   433,   435,   437,   439,
     441,   443,   445,   447,   449,   451,   453,   455,   457,   459,
     461,   463,   465,   467,   469,   471,   473,   475,   477,   479,
     481,   483,   485,   487,   489,   491,   493,   495,   497,   499,
     501,   503,   505,   507,   509,   511,   513,   515,   517,   521,
     522,   525,   528,   534,   540,   546,   555,   561,   570,   576,
     585,   591,   600,   606,   615,   621,   630,   636,   645,   651,
     660,   666,   675,   681,   690,   696,   705,   711,   720,   726,
     735,   741,   750,   761,   770,   778,   790,   804,   816,   830,
     836,   845,   854,   863,   872,   883,   896,   909,   918,   927,
     938,   951,   960,   969,   978,   987,   998,  1009,  1018,  1027,
    1038,  1044,  1051,  1057,  1064,  1070,  1077,  1083,  1089,  1095,
    1101,  1107,  1116,  1124,  1132,  1141,  1147,  1153,  1161,  1167,
    1177,  1183,  1189,  1195,  1204,  1208,  1209,  1216,  1217,  1225,
    1233,  1241,  1245,  1250,  1252,  1255,  1258,  1259,  1266,  1267,
    1275,  1277,  1279,  1280,  1282,  1284,  1286,  1287,  1290,  1294,
    1299,  1304,  1309,  1314,  1319,  1320,  1322,  1323,  1325,  1329,
    1334,  1335,  1337,  1339,  1343,  1351,  1356,  1357,  1363,  1366,
    1368,  1370,  1372,  1374,  1376,  1378,  1382,  1383,  1391,  1392,
    1398,  1404,  1410,  1418,  1424,  1430,  1438,  1444,  1450,  1456,
    1457,  1458,  1468,  1469,  1470,  1480,  1481,  1482,  1492,  1493,
    1499,  1505,  1513,  1519,  1520,  1521,  1531,  1533,  1535,  1537,
    1539,  1541,  1543,  1545,  1547,  1549,  1551,  1553,  1555,  1557,
    1559,  1561,  1563,  1565,  1567,  1569,  1571,  1573,  1575,  1577,
    1579,  1581,  1583,  1585,  1587,  1588,  1594,  1600,  1606,  1612,
    1618,  1624,  1630,  1636,  1640,  1641,  1649,  1652,  1657,  1662,
    1667,  1672,  1677,  1682,  1687,  1692,  1697,  1702,  1707,  1712,
    1717,  1722,  1727,  1736,  1741,  1746,  1755,  1760,  1765,  1770,
    1775,  1780,  1785,  1790,  1799,  1804,  1813,  1826,  1831,  1836,
    1841,  1846,  1851,  1856,  1861,  1866,  1871,  1880,  1893,  1898,
    1903,  1908,  1913,  1926,  1931,  1936,  1941,  1946,  1951,  1956,
    1961,  1966,  1971,  1980,  1985,  1990,  2001,  2014,  2019,  2024,
    2029,  2034,  2039,  2044,  2055,  2068,  2073,  2084,  2089,  2094,
    2099,  2104,  2109,  2114,  2115,  2121,  2127,  2133,  2139,  2145,
    2151,  2157,  2163,  2169,  2175,  2181,  2187,  2193,  2199,  2205,
    2215,  2221,  2227,  2233,  2239,  2245,  2251,  2255,  2256,  2264,
    2267,  2268,  2274,  2280,  2286,  2292,  2298,  2304,  2310,  2316,
    2322,  2328,  2334,  2340,  2345,  2354,  2368,  2374,  2380,  2386,
    2392,  2406,  2412,  2418,  2424,  2430,  2434,  2435,  2443,  2446,
    2447,  2453,  2459,  2465,  2471,  2477,  2487,  2493,  2499,  2505,
    2511,  2517,  2523,  2529,  2535,  2541,  2547,  2553,  2559,  2565,
    2571,  2577,  2581,  2582,  2590,  2594,  2603,  2608,  2613,  2627,
    2641,  2646,  2651,  2656,  2661,  2666,  2671,  2676,  2681,  2686,
    2691,  2696,  2701,  2706,  2711,  2716,  2721,  2726,  2727,  2737,
    2743,  2749,  2764,  2779,  2785,  2791,  2797,  2803,  2809,  2815,
    2821,  2827,  2833,  2839,  2845,  2851,  2857,  2863,  2869,  2875,
    2881,  2885,  2886,  2894,  2898,  2899,  2905,  2911,  2917,  2923,
    2926,  2930,  2931,  2933,  2936,  2941,  2946,  2949,  2952,  2955,
    2960,  2962,  2964,  2966,  2968,  2971,  2974,  2978,  2980,  2982,
    2985,  2987,  2989,  2991,  2993,  2995,  2997,  2999,  3002,  3005,
    3009,  3010,  3012,  3014,  3017,  3019,  3022,  3025,  3029,  3031,
    3034,  3037,  3041,  3044,  3048,  3052,  3057,  3059,  3061,  3063,
    3065,  3067,  3069,  3071,  3073,  3075,  3077,  3079,  3081,  3083,
    3085,  3087,  3089,  3091,  3093,  3095,  3097,  3099,  3101,  3103,
    3105,  3107,  3109,  3111,  3113,  3114,  3121,  3123,  3126,  3128,
    3130,  3132,  3134,  3136,  3138,  3140,  3142,  3144,  3146,  3148,
    3150,  3152,  3154,  3156,  3158,  3160,  3162,  3165,  3166,  3174,
    3175,  3178,  3182,  3183,  3188,  3195,  3203,  3209,  3215,  3221,
    3227,  3233,  3239,  3250,  3259,  3267,  3268,  3269,  3278,  3279,
    3287,  3295,  3306,  3315,  3323,  3324,  3325,  3334,  3335,  3341,
    3347,  3355,  3356,  3357,  3366,  3367,  3373,  3379,  3380,  3381
};

/* YYRHS -- A `-1'-separated list of the rules' RHS.  */
static const yytype_int16 yyrhs[] =
{
     404,     0,    -1,    -1,   404,   390,    -1,   404,   405,    -1,
     404,   433,    -1,   404,   440,    -1,   404,   444,    -1,   404,
     443,    -1,   404,   450,    -1,   404,   449,    -1,   404,   453,
      -1,   404,   456,    -1,   404,   464,    -1,   404,   473,    -1,
     404,   483,    -1,   404,   487,    -1,   404,   480,    -1,   404,
     477,    -1,   404,   510,    -1,   404,   513,    -1,   404,   507,
      -1,   404,   441,    -1,   404,   442,    -1,   404,   516,    -1,
     404,   520,    -1,   404,   524,    -1,   404,   528,    -1,   404,
       1,    -1,    63,    64,   391,   409,    -1,     9,    -1,    22,
      -1,     6,    -1,   407,   385,   407,    -1,   408,   385,   407,
      -1,   407,   385,   408,    -1,   407,   384,   407,    -1,   408,
     384,   407,    -1,   407,   384,   408,    -1,   407,   386,   407,
      -1,   408,   386,   407,    -1,   407,   386,   408,    -1,   407,
     387,   407,    -1,   408,   387,   407,    -1,   407,   387,   408,
      -1,   408,   387,   408,    -1,   384,   407,    -1,   407,   389,
     407,    -1,   408,   389,   407,    -1,   407,   389,   408,    -1,
     408,   389,   408,    -1,   392,   407,   393,    -1,     7,    -1,
     408,   385,   408,    -1,   408,   384,   408,    -1,   408,   386,
     408,    -1,   384,   408,    -1,   392,   408,   393,    -1,     4,
      -1,     5,    -1,   409,    -1,   408,    -1,   407,    -1,   408,
      -1,   407,    -1,   409,    -1,   407,    -1,   408,    -1,   409,
      -1,    -1,   394,    -1,     5,    -1,    22,    -1,   414,   413,
      22,    -1,    -1,   414,    -1,   417,   414,   418,    -1,    -1,
      22,    -1,   416,   413,    22,    -1,   395,    -1,   396,    -1,
     407,    -1,   408,    -1,   419,   394,   407,    -1,   419,   394,
     408,    -1,   419,     1,    -1,   417,   419,   418,    -1,   420,
     394,   417,   419,   418,    -1,   420,     1,    -1,   408,    -1,
     421,   394,   408,    -1,   417,   421,   418,    -1,   422,   394,
     417,   421,   418,    -1,   422,     1,    -1,   409,    -1,   423,
     394,   409,    -1,   408,    -1,   407,    -1,    22,    -1,     1,
      -1,    -1,    29,    -1,   424,    -1,   425,   394,   424,    -1,
     425,     1,    -1,   103,    -1,   104,    -1,   105,    -1,   106,
      -1,   107,    -1,   108,    -1,   109,    -1,   110,    -1,   111,
      -1,   112,    -1,   113,    -1,   114,    -1,   115,    -1,   116,
      -1,   117,    -1,   118,    -1,   119,    -1,   120,    -1,   121,
      -1,   122,    -1,   123,    -1,   124,    -1,   125,    -1,   126,
      -1,   127,    -1,   128,    -1,   129,    -1,   130,    -1,   131,
      -1,   132,    -1,   133,    -1,   134,    -1,   135,    -1,   136,
      -1,   137,    -1,   138,    -1,   139,    -1,   140,    -1,   141,
      -1,   142,    -1,   143,    -1,   144,    -1,   145,    -1,   146,
      -1,   147,    -1,   148,    -1,   149,    -1,   150,    -1,   151,
      -1,   152,    -1,   153,    -1,   154,    -1,   155,    -1,   156,
      -1,   157,    -1,   158,    -1,   159,    -1,   160,    -1,   161,
      -1,   162,    -1,   163,    -1,   164,    -1,   165,    -1,   166,
      -1,   167,    -1,   168,    -1,   169,    -1,   170,    -1,   171,
      -1,   377,    -1,   378,    -1,   379,    -1,   380,    -1,   381,
      -1,   382,    -1,   351,    -1,   352,    -1,   353,    -1,   354,
      -1,   355,    -1,   356,    -1,   357,    -1,   358,    -1,   359,
      -1,   360,    -1,   361,    -1,   362,    -1,   363,    -1,   364,
      -1,   365,    -1,   366,    -1,   367,    -1,   368,    -1,   369,
      -1,   370,    -1,   371,    -1,   372,    -1,   373,    -1,   374,
      -1,   375,    -1,   376,    -1,   274,    -1,   427,   397,   274,
      -1,    -1,   428,   475,    -1,   428,   485,    -1,   428,   303,
     383,    22,   390,    -1,   428,   304,   383,   408,   390,    -1,
     428,    67,   383,   415,   390,    -1,   428,    67,   398,   406,
     399,   383,   415,   390,    -1,   428,    68,   383,   415,   390,
      -1,   428,    68,   398,   406,   399,   383,   415,   390,    -1,
     428,   321,   383,   415,   390,    -1,   428,   321,   398,   406,
     399,   383,   415,   390,    -1,   428,   326,   383,   415,   390,
      -1,   428,   326,   398,   406,   399,   383,   415,   390,    -1,
     428,    69,   383,   415,   390,    -1,   428,    69,   398,   406,
     399,   383,   415,   390,    -1,   428,    70,   383,   415,   390,
      -1,   428,    70,   398,   406,   399,   383,   415,   390,    -1,
     428,    71,   383,   415,   390,    -1,   428,    71,   398,   406,
     399,   383,   415,   390,    -1,   428,    87,   383,   415,   390,
      -1,   428,    87,   398,   406,   399,   383,   415,   390,    -1,
     428,    88,   383,   415,   390,    -1,   428,    88,   398,   406,
     399,   383,   415,   390,    -1,   428,   322,   383,   415,   390,
      -1,   428,   322,   398,   406,   399,   383,   415,   390,    -1,
     428,   325,   383,   415,   390,    -1,   428,   325,   398,   406,
     399,   383,   415,   390,    -1,   428,    89,   383,   415,   390,
      -1,   428,    89,   398,   406,   399,   383,   415,   390,    -1,
     428,    90,   383,   415,   390,    -1,   428,    90,   398,   406,
     399,   383,   415,   390,    -1,   428,    91,   383,   415,   390,
      -1,   428,    91,   398,   406,   399,   383,   415,   390,    -1,
     428,    34,   392,    22,   393,   383,   417,   416,   418,   390,
      -1,   428,    34,   392,    22,   393,   383,    22,   390,    -1,
     428,    34,   392,    22,   393,   500,   390,    -1,   428,    34,
     392,    22,   400,   408,   401,   393,   383,    22,   390,    -1,
     428,    34,   392,    22,   400,   408,   401,   393,   383,   417,
     420,   418,   390,    -1,   428,    34,   392,    22,   400,   408,
     401,   393,   383,   422,   390,    -1,   428,    34,   392,    22,
     400,   408,   401,   393,   383,   417,   423,   418,   390,    -1,
     428,    72,   383,     9,   390,    -1,   428,    23,   392,    22,
     393,   383,    22,   390,    -1,   428,    98,   392,    22,   393,
     383,    22,   390,    -1,   428,    73,   392,    22,   393,   383,
      22,   390,    -1,   428,    99,   392,    22,   393,   383,    22,
     390,    -1,   428,    34,   392,    22,   402,    73,   393,   383,
      22,   390,    -1,   428,    73,   392,    22,   393,   383,   395,
      22,   394,   408,   396,   390,    -1,   428,    99,   392,    22,
     393,   383,   395,    22,   394,   408,   396,   390,    -1,   428,
      79,   392,    22,   393,   383,    22,   390,    -1,   428,    95,
     392,    22,   393,   383,    22,   390,    -1,   428,    34,   392,
      22,   402,    79,   393,   383,    22,   390,    -1,   428,    79,
     392,    22,   393,   383,   395,    22,   394,   408,   396,   390,
      -1,   428,    74,   392,    22,   393,   383,   408,   390,    -1,
     428,   100,   392,    22,   393,   383,   408,   390,    -1,   428,
      80,   392,    22,   393,   383,   408,   390,    -1,   428,    97,
     392,    22,   393,   383,   408,   390,    -1,   428,    34,   392,
      22,   402,    74,   393,   383,   408,   390,    -1,   428,    34,
     392,    22,   402,    80,   393,   383,   408,   390,    -1,   428,
      75,   392,    22,   393,   383,    22,   390,    -1,   428,   101,
     392,    22,   393,   383,    22,   390,    -1,   428,    34,   392,
      22,   402,    75,   393,   383,    22,   390,    -1,   428,    76,
     383,    22,   390,    -1,   428,    76,   392,    22,   393,   390,
      -1,   428,    77,   383,    22,   390,    -1,   428,    77,   392,
      22,   393,   390,    -1,   428,    78,   383,    22,   390,    -1,
     428,    78,   392,    22,   393,   390,    -1,   428,   270,   383,
     271,   390,    -1,   428,   269,   383,   411,   390,    -1,   428,
     272,   383,    22,   390,    -1,   428,   273,   383,   427,   390,
      -1,   428,   275,   383,    22,   390,    -1,   428,   276,   400,
     411,   401,   383,   420,   390,    -1,   428,   276,   383,   417,
     419,   418,   390,    -1,   428,   277,   383,   417,   421,   418,
     390,    -1,   428,   291,   392,    22,   394,    22,   393,   390,
      -1,   428,   292,   383,   411,   390,    -1,   428,   293,   383,
     410,   390,    -1,   428,   320,   383,   417,   421,   418,   390,
      -1,   428,   299,   383,   415,   390,    -1,   428,   297,   383,
     395,    22,   394,   425,   396,   390,    -1,   428,   300,   383,
     408,   390,    -1,   428,   301,   383,   422,   390,    -1,   428,
     302,   383,   422,   390,    -1,   428,   268,   392,   406,   393,
     383,   411,   390,    -1,   428,     1,   390,    -1,    -1,    20,
     430,   491,   395,   428,   396,    -1,    -1,    20,    22,   431,
     491,   395,   428,   396,    -1,   268,   392,    22,   393,   383,
     411,   390,    -1,   268,   392,     9,   393,   383,   411,   390,
      -1,    19,    22,   390,    -1,    19,    22,   294,   390,    -1,
     429,    -1,   432,   429,    -1,   432,     1,    -1,    -1,    19,
     434,   491,   395,   432,   396,    -1,    -1,    19,    22,   435,
     491,   395,   432,   396,    -1,   296,    -1,   295,    -1,    -1,
     296,    -1,   295,    -1,    22,    -1,    -1,   438,   491,    -1,
     436,   438,   491,    -1,    10,   439,   395,    21,    -1,   323,
     439,   395,    21,    -1,   324,   439,   395,    21,    -1,    12,
     439,   395,    21,    -1,    11,   439,   395,    21,    -1,    -1,
      30,    -1,    -1,   386,    -1,    36,   446,    22,    -1,   445,
      47,   446,    22,    -1,    -1,    29,    -1,   447,    -1,   448,
     394,   447,    -1,   297,   439,   392,   448,   393,   395,    21,
      -1,   298,   439,   395,    21,    -1,    -1,   451,    75,   383,
      22,   390,    -1,   451,     1,    -1,    23,    -1,    24,    -1,
      25,    -1,    26,    -1,    27,    -1,    28,    -1,   452,    22,
     390,    -1,    -1,   452,   438,   454,   491,   395,   451,   396,
      -1,    -1,   455,    75,   383,    22,   390,    -1,   455,    22,
     383,     9,   390,    -1,   455,   284,   383,   290,   390,    -1,
     455,   284,   383,   417,   421,   418,   390,    -1,   455,   284,
     383,   411,   390,    -1,   455,   285,   383,   290,   390,    -1,
     455,   285,   383,   417,   421,   418,   390,    -1,   455,   286,
     383,   412,   390,    -1,   455,   287,   383,    22,   390,    -1,
     455,   288,   383,   289,   390,    -1,    -1,    -1,   280,   438,
     437,   457,   491,   458,   395,   455,   396,    -1,    -1,    -1,
     281,   438,   437,   459,   491,   460,   395,   455,   396,    -1,
      -1,    -1,   279,   438,   437,   461,   491,   462,   395,   455,
     396,    -1,    -1,   463,    22,   383,     9,   390,    -1,   463,
     282,   383,    22,   390,    -1,   463,   283,   383,   417,   416,
     418,   390,    -1,   463,   283,   383,    22,   390,    -1,    -1,
      -1,   278,   438,   437,   465,   491,   466,   395,   463,   396,
      -1,   177,    -1,   178,    -1,   179,    -1,   180,    -1,   181,
      -1,   182,    -1,     1,    -1,   185,    -1,   184,    -1,   186,
      -1,   183,    -1,     1,    -1,   377,    -1,   379,    -1,   378,
      -1,     1,    -1,   108,    -1,   114,    -1,   115,    -1,   109,
      -1,   111,    -1,   110,    -1,   112,    -1,   113,    -1,     1,
      -1,   102,    -1,   188,    -1,     1,    -1,    -1,   472,   173,
     383,   467,   390,    -1,   472,   172,   383,   467,   390,    -1,
     472,   174,   383,   468,   390,    -1,   472,   175,   383,   468,
     390,    -1,   472,   176,   383,   468,   390,    -1,   472,   187,
     383,   471,   390,    -1,   472,   189,   383,   470,   390,    -1,
     472,   190,   383,   469,   390,    -1,   472,     1,   390,    -1,
      -1,    14,   438,   474,   491,   395,   472,   396,    -1,   473,
       1,    -1,   191,   383,   412,   390,    -1,   192,   383,   410,
     390,    -1,   193,   383,   410,   390,    -1,   194,   383,   410,
     390,    -1,   195,   383,   426,   390,    -1,   196,   383,   412,
     390,    -1,   197,   383,   410,   390,    -1,   199,   383,   411,
     390,    -1,   200,   383,   411,   390,    -1,   201,   383,   410,
     390,    -1,   202,   383,   410,   390,    -1,   202,   383,   426,
     390,    -1,   203,   383,   426,   390,    -1,   204,   383,   426,
     390,    -1,   205,   383,   410,   390,    -1,   206,   383,   417,
     426,   413,   426,   418,   390,    -1,   208,   383,   412,   390,
      -1,   209,   383,   412,   390,    -1,   207,   383,   417,   412,
     394,   412,   418,   390,    -1,   210,   383,   410,   390,    -1,
     211,   383,   410,   390,    -1,   212,   383,   410,   390,    -1,
     214,   383,   410,   390,    -1,   215,   383,   410,   390,    -1,
     217,   383,   426,   390,    -1,   218,   383,   412,   390,    -1,
     216,   383,   417,   426,   394,   412,   418,   390,    -1,   221,
     383,   410,   390,    -1,   219,   383,   395,   426,   394,   426,
     396,   390,    -1,   220,   383,   395,   426,   394,   426,   394,
     426,   394,   426,   396,   390,    -1,   222,   383,   426,   390,
      -1,   223,   383,   426,   390,    -1,   224,   383,   426,   390,
      -1,   225,   383,   426,   390,    -1,   226,   383,   426,   390,
      -1,   227,   383,   426,   390,    -1,   230,   383,   426,   390,
      -1,   231,   383,   426,   390,    -1,   229,   383,   426,   390,
      -1,   228,   383,   417,   426,   394,   426,   418,   390,    -1,
     232,   383,   417,   412,   413,   412,   413,   412,   413,   412,
     418,   390,    -1,   233,   383,   410,   390,    -1,   234,   383,
     410,   390,    -1,   235,   383,   410,   390,    -1,   236,   383,
     426,   390,    -1,   237,   383,   417,   410,   413,   410,   413,
     410,   413,   410,   418,   390,    -1,   238,   383,   410,   390,
      -1,   239,   383,   410,   390,    -1,   241,   383,   410,   390,
      -1,   242,   383,   410,   390,    -1,   243,   383,   410,   390,
      -1,   244,   383,   426,   390,    -1,   245,   383,   410,   390,
      -1,   246,   383,   410,   390,    -1,   247,   383,   410,   390,
      -1,   248,   383,   417,   412,   413,   412,   418,   390,    -1,
     249,   383,   410,   390,    -1,   250,   383,   426,   390,    -1,
     250,   383,   395,   426,   394,   411,   394,   411,   396,   390,
      -1,   251,   383,   395,   426,   394,   426,   394,   411,   394,
     411,   396,   390,    -1,   252,   383,   411,   390,    -1,   253,
     383,   411,   390,    -1,   260,   383,   411,   390,    -1,   254,
     383,   426,   390,    -1,   255,   383,   426,   390,    -1,   256,
     383,   426,   390,    -1,   257,   383,   395,   426,   394,   426,
     394,   426,   396,   390,    -1,   258,   383,   395,   426,   394,
     426,   394,   426,   394,   426,   396,   390,    -1,   261,   383,
     426,   390,    -1,   261,   383,   395,   426,   394,   411,   394,
     411,   396,   390,    -1,   262,   383,   411,   390,    -1,   263,
     383,   411,   390,    -1,   264,   383,   426,   390,    -1,   265,
     383,   426,   390,    -1,   266,   383,   426,   390,    -1,   267,
     383,   411,   390,    -1,    -1,   476,   191,   383,   412,   390,
      -1,   476,   192,   383,   410,   390,    -1,   476,   193,   383,
     410,   390,    -1,   476,   194,   383,   410,   390,    -1,   476,
     195,   383,   426,   390,    -1,   476,   196,   383,   412,   390,
      -1,   476,   197,   383,   410,   390,    -1,   476,   199,   383,
     411,   390,    -1,   476,   200,   383,   411,   390,    -1,   476,
     201,   383,   410,   390,    -1,   476,   202,   383,   410,   390,
      -1,   476,   203,   383,   426,   390,    -1,   476,   204,   383,
     426,   390,    -1,   476,   205,   383,   410,   390,    -1,   476,
     206,   383,   426,   390,    -1,   476,   206,   383,   417,   426,
     413,   426,   418,   390,    -1,   476,   208,   383,   412,   390,
      -1,   476,   209,   383,   412,   390,    -1,   476,   210,   383,
     410,   390,    -1,   476,   211,   383,   410,   390,    -1,   476,
     212,   383,   410,   390,    -1,   476,   214,   383,   410,   390,
      -1,   476,     1,   390,    -1,    -1,    18,   438,   478,   491,
     395,   476,   396,    -1,   477,     1,    -1,    -1,   479,   215,
     383,   410,   390,    -1,   479,   217,   383,   426,   390,    -1,
     479,   218,   383,   412,   390,    -1,   479,   221,   383,   410,
     390,    -1,   479,   222,   383,   426,   390,    -1,   479,   223,
     383,   426,   390,    -1,   479,   224,   383,   426,   390,    -1,
     479,   225,   383,   426,   390,    -1,   479,   226,   383,   426,
     390,    -1,   479,   227,   383,   426,   390,    -1,   479,   230,
     383,   426,   390,    -1,   479,   231,   383,   426,   390,    -1,
     229,   383,   426,   390,    -1,   228,   383,   417,   426,   394,
     426,   418,   390,    -1,   479,   232,   383,   417,   412,   413,
     412,   413,   412,   413,   412,   418,   390,    -1,   479,   233,
     383,   410,   390,    -1,   479,   234,   383,   410,   390,    -1,
     479,   235,   383,   410,   390,    -1,   479,   236,   383,   426,
     390,    -1,   479,   237,   383,   417,   410,   413,   410,   413,
     410,   413,   410,   418,   390,    -1,   479,   238,   383,   410,
     390,    -1,   479,   239,   383,   410,   390,    -1,   479,   241,
     383,   410,   390,    -1,   479,   242,   383,   410,   390,    -1,
     479,     1,   390,    -1,    -1,    17,   438,   481,   491,   395,
     479,   396,    -1,   480,     1,    -1,    -1,   482,   243,   383,
     410,   390,    -1,   482,   244,   383,   426,   390,    -1,   482,
     245,   383,   410,   390,    -1,   482,   246,   383,   410,   390,
      -1,   482,   247,   383,   410,   390,    -1,   482,   248,   383,
     417,   412,   413,   412,   418,   390,    -1,   482,   249,   383,
     410,   390,    -1,   482,   250,   383,   426,   390,    -1,   482,
     252,   383,   411,   390,    -1,   482,   253,   383,   411,   390,
      -1,   482,   254,   383,   426,   390,    -1,   482,   255,   383,
     426,   390,    -1,   482,   256,   383,   426,   390,    -1,   482,
     260,   383,   411,   390,    -1,   482,   261,   383,   426,   390,
      -1,   482,   262,   383,   411,   390,    -1,   482,   263,   383,
     411,   390,    -1,   482,   264,   383,   426,   390,    -1,   482,
     265,   383,   426,   390,    -1,   482,   266,   383,   426,   390,
      -1,   482,   267,   383,   411,   390,    -1,   482,     1,   390,
      -1,    -1,    15,   438,   484,   491,   395,   482,   396,    -1,
     483,     1,   390,    -1,   327,   383,   395,   412,   394,   412,
     396,   390,    -1,   328,   383,   426,   390,    -1,   329,   383,
     426,   390,    -1,   330,   400,   411,   401,   383,   395,   426,
     394,   411,   394,   412,   396,   390,    -1,   331,   400,   411,
     401,   383,   395,   426,   394,   426,   394,   412,   396,   390,
      -1,   332,   383,   426,   390,    -1,   333,   383,   412,   390,
      -1,   334,   383,   426,   390,    -1,   335,   383,   426,   390,
      -1,   336,   383,   426,   390,    -1,   337,   383,   426,   390,
      -1,   338,   383,   426,   390,    -1,   339,   383,   412,   390,
      -1,   340,   383,   412,   390,    -1,   341,   383,   426,   390,
      -1,   342,   383,   412,   390,    -1,   343,   383,   426,   390,
      -1,   344,   383,   411,   390,    -1,   345,   383,   426,   390,
      -1,   348,   383,   426,   390,    -1,   346,   383,   426,   390,
      -1,   347,   383,   426,   390,    -1,    -1,   486,   327,   383,
     395,   412,   394,   412,   396,   390,    -1,   486,   328,   383,
     426,   390,    -1,   486,   329,   383,   426,   390,    -1,   486,
     330,   400,   411,   401,   383,   395,   426,   394,   411,   394,
     412,   396,   390,    -1,   486,   331,   400,   411,   401,   383,
     395,   426,   394,   426,   394,   412,   396,   390,    -1,   486,
     332,   383,   426,   390,    -1,   486,   333,   383,   412,   390,
      -1,   486,   334,   383,   426,   390,    -1,   486,   335,   383,
     426,   390,    -1,   486,   336,   383,   426,   390,    -1,   486,
     337,   383,   426,   390,    -1,   486,   338,   383,   426,   390,
      -1,   486,   339,   383,   412,   390,    -1,   486,   340,   383,
     412,   390,    -1,   486,   341,   383,   426,   390,    -1,   486,
     342,   383,   412,   390,    -1,   486,   343,   383,   426,   390,
      -1,   486,   344,   383,   411,   390,    -1,   486,   345,   383,
     426,   390,    -1,   486,   348,   383,   426,   390,    -1,   486,
     346,   383,   426,   390,    -1,   486,   347,   383,   426,   390,
      -1,   486,     1,   390,    -1,    -1,    16,   438,   488,   491,
     395,   486,   396,    -1,   487,     1,   390,    -1,    -1,   489,
      22,   383,     9,   390,    -1,   489,    22,   383,   412,   390,
      -1,   489,    22,   383,   408,   390,    -1,   489,    22,   383,
     409,   390,    -1,   489,     1,    -1,   398,   489,   399,    -1,
      -1,   490,    -1,   383,   407,    -1,   383,   417,   419,   418,
      -1,   383,   417,   420,   418,    -1,   383,   408,    -1,   383,
       1,    -1,   383,   409,    -1,   383,   417,   423,   418,    -1,
      31,    -1,    32,    -1,    33,    -1,    34,    -1,    34,   436,
      -1,    34,   498,    -1,    34,   436,   498,    -1,   436,    -1,
     498,    -1,   436,   498,    -1,   492,    -1,   493,    -1,   494,
      -1,   495,    -1,   496,    -1,   497,    -1,     1,    -1,   391,
      22,    -1,   400,   401,    -1,   400,   408,   401,    -1,    -1,
     490,    -1,   500,    -1,   490,   500,    -1,   501,    -1,   501,
     490,    -1,   501,   500,    -1,   501,   490,   500,    -1,   502,
      -1,   502,   490,    -1,   502,   500,    -1,   502,   490,   500,
      -1,   502,   501,    -1,   502,   501,   490,    -1,   502,   501,
     500,    -1,   502,   501,   490,   500,    -1,    36,    -1,    37,
      -1,    38,    -1,    39,    -1,    47,    -1,    48,    -1,    49,
      -1,    50,    -1,    43,    -1,    44,    -1,    45,    -1,    46,
      -1,    40,    -1,    41,    -1,    42,    -1,    51,    -1,    52,
      -1,    53,    -1,    54,    -1,    55,    -1,    56,    -1,    57,
      -1,    58,    -1,    59,    -1,    60,    -1,    61,    -1,    62,
      -1,     1,    -1,    -1,   499,   504,    22,   506,   503,   390,
      -1,   505,    -1,   507,   505,    -1,   440,    -1,   507,    -1,
     444,    -1,   443,    -1,   450,    -1,   449,    -1,   453,    -1,
     456,    -1,   464,    -1,   473,    -1,   483,    -1,   487,    -1,
     480,    -1,   477,    -1,   513,    -1,   441,    -1,   442,    -1,
     508,    -1,   509,   508,    -1,    -1,     3,    22,   511,   491,
     395,   509,   396,    -1,    -1,   395,   396,    -1,   395,   507,
     396,    -1,    -1,    35,   439,   514,   512,    -1,   305,   383,
     417,   416,   418,   390,    -1,   515,   306,   383,   417,   416,
     418,   390,    -1,   515,   307,   383,    22,   390,    -1,   515,
     308,   383,    22,   390,    -1,   515,   309,   383,    22,   390,
      -1,   515,   310,   383,    22,   390,    -1,   515,   311,   383,
     408,   390,    -1,   515,   312,   383,    22,   390,    -1,   515,
      34,   392,    22,   393,   383,   417,   416,   418,   390,    -1,
     515,    34,   392,    22,   393,   383,    22,   390,    -1,   515,
      34,   392,    22,   393,   500,   390,    -1,    -1,    -1,   303,
     439,   517,   491,   518,   395,   515,   396,    -1,    -1,   519,
     315,   383,   417,   416,   418,   390,    -1,   519,   314,   383,
     417,   416,   418,   390,    -1,   519,    34,   392,    22,   393,
     383,   417,   416,   418,   390,    -1,   519,    34,   392,    22,
     393,   383,    22,   390,    -1,   519,    34,   392,    22,   393,
     500,   390,    -1,    -1,    -1,   313,   439,   521,   491,   522,
     395,   519,   396,    -1,    -1,   523,   288,   383,   289,   390,
      -1,   523,   285,   383,   290,   390,    -1,   523,   285,   383,
     417,   421,   418,   390,    -1,    -1,    -1,   316,    22,   525,
     491,   526,   395,   523,   396,    -1,    -1,   527,   318,   383,
       9,   390,    -1,   527,   319,   383,     9,   390,    -1,    -1,
      -1,   317,   439,   529,   491,   530,   395,   527,   396,    -1
};

/* YYRLINE[YYN] -- source line where rule number YYN was defined.  */
static const yytype_uint16 yyrline[] =
{
       0,  1017,  1017,  1018,  1019,  1020,  1022,  1024,  1026,  1028,
    1029,  1030,  1031,  1032,  1033,  1034,  1035,  1036,  1037,  1038,
    1039,  1040,  1041,  1043,  1045,  1046,  1047,  1048,  1049,  1052,
    1061,  1062,  1068,  1069,  1070,  1071,  1072,  1073,  1074,  1075,
    1076,  1077,  1078,  1079,  1080,  1081,  1082,  1083,  1084,  1085,
    1086,  1087,  1095,  1096,  1097,  1098,  1099,  1100,  1105,  1106,
    1112,  1113,  1116,  1120,  1121,  1125,  1128,  1129,  1130,  1148,
    1150,  1155,  1156,  1171,  1188,  1204,  1205,  1212,  1213,  1218,
    1230,  1235,  1243,  1247,  1251,  1255,  1259,  1265,  1269,  1273,
    1276,  1285,  1296,  1300,  1304,  1307,  1311,  1331,  1335,  1339,
    1368,  1373,  1374,  1375,  1379,  1383,  1400,  1401,  1402,  1403,
    1404,  1405,  1406,  1407,  1408,  1409,  1410,  1411,  1412,  1413,
    1414,  1415,  1416,  1417,  1418,  1419,  1420,  1421,  1422,  1423,
    1424,  1425,  1426,  1427,  1428,  1429,  1430,  1431,  1432,  1433,
    1434,  1435,  1436,  1437,  1438,  1439,  1440,  1441,  1442,  1443,
    1444,  1445,  1446,  1447,  1448,  1449,  1450,  1451,  1452,  1453,
    1454,  1455,  1456,  1457,  1458,  1459,  1460,  1461,  1462,  1463,
    1464,  1465,  1466,  1467,  1468,  1470,  1471,  1472,  1473,  1474,
    1475,  1476,  1477,  1478,  1479,  1480,  1481,  1482,  1483,  1484,
    1485,  1486,  1487,  1488,  1489,  1490,  1491,  1492,  1493,  1494,
    1495,  1496,  1497,  1498,  1499,  1500,  1501,  1505,  1506,  1513,
    1514,  1515,  1516,  1521,  1525,  1530,  1536,  1541,  1547,  1552,
    1558,  1563,  1569,  1574,  1580,  1585,  1591,  1596,  1602,  1607,
    1613,  1618,  1624,  1629,  1635,  1640,  1646,  1651,  1657,  1662,
    1668,  1673,  1679,  1694,  1703,  1760,  1779,  1807,  1816,  1821,
    1825,  1842,  1859,  1876,  1893,  1910,  1927,  1944,  1961,  1978,
    1995,  2012,  2025,  2038,  2051,  2064,  2077,  2090,  2111,  2132,
    2153,  2163,  2173,  2183,  2193,  2203,  2216,  2219,  2222,  2236,
    2239,  2253,  2263,  2267,  2274,  2295,  2298,  2304,  2319,  2329,
    2375,  2383,  2400,  2420,  2424,  2433,  2433,  2447,  2447,  2465,
    2469,  2473,  2484,  2502,  2503,  2504,  2510,  2510,  2518,  2518,
    2533,  2534,  2539,  2540,  2541,  2549,  2558,  2559,  2560,  2573,
    2597,  2618,  2639,  2677,  2715,  2716,  2718,  2719,  2724,  2729,
    2738,  2739,  2740,  2744,  2752,  2812,  2853,  2854,  2882,  2889,
    2890,  2891,  2892,  2893,  2894,  2896,  2902,  2902,  2918,  2919,
    2929,  2935,  2939,  2944,  2948,  2952,  2958,  2962,  2975,  2987,
    2994,  2987,  3002,  3009,  3002,  3017,  3024,  3017,  3036,  3037,
    3046,  3060,  3081,  3098,  3105,  3098,  3118,  3119,  3120,  3121,
    3122,  3123,  3124,  3127,  3128,  3129,  3130,  3131,  3134,  3135,
    3136,  3137,  3140,  3141,  3142,  3143,  3144,  3145,  3146,  3147,
    3148,  3151,  3152,  3153,  3159,  3160,  3163,  3166,  3169,  3172,
    3175,  3178,  3181,  3184,  3189,  3189,  3199,  3214,  3215,  3216,
    3217,  3218,  3219,  3220,  3222,  3223,  3224,  3225,  3226,  3227,
    3228,  3229,  3230,  3231,  3232,  3233,  3238,  3239,  3240,  3242,
    3243,  3244,  3245,  3246,  3251,  3252,  3257,  3264,  3265,  3266,
    3267,  3268,  3269,  3271,  3272,  3273,  3278,  3287,  3289,  3290,
    3291,  3292,  3293,  3300,  3301,  3303,  3304,  3305,  3306,  3307,
    3308,  3309,  3310,  3312,  3313,  3314,  3320,  3326,  3327,  3328,
    3329,  3330,  3331,  3332,  3339,  3352,  3353,  3359,  3360,  3361,
    3362,  3363,  3364,  3370,  3371,  3372,  3373,  3374,  3375,  3376,
    3377,  3379,  3380,  3381,  3382,  3383,  3384,  3385,  3386,  3387,
    3388,  3389,  3390,  3391,  3392,  3394,  3395,  3400,  3400,  3410,
    3415,  3416,  3417,  3418,  3419,  3420,  3421,  3422,  3423,  3424,
    3425,  3427,  3428,  3429,  3434,  3443,  3445,  3446,  3447,  3448,
    3449,  3456,  3457,  3459,  3460,  3461,  3466,  3466,  3476,  3481,
    3482,  3483,  3484,  3485,  3486,  3487,  3489,  3490,  3491,  3492,
    3493,  3494,  3495,  3496,  3504,  3505,  3506,  3507,  3508,  3509,
    3510,  3511,  3516,  3516,  3526,  3529,  3530,  3531,  3532,  3533,
    3534,  3535,  3536,  3537,  3538,  3539,  3540,  3541,  3542,  3543,
    3544,  3545,  3546,  3547,  3548,  3549,  3550,  3558,  3559,  3560,
    3565,  3570,  3575,  3580,  3585,  3590,  3595,  3600,  3605,  3610,
    3615,  3620,  3625,  3630,  3635,  3640,  3645,  3650,  3655,  3660,
    3671,  3677,  3677,  3693,  3703,  3704,  3711,  3717,  3723,  3735,
    3738,  3740,  3741,  3746,  3756,  3763,  3769,  3776,  3788,  3798,
    3804,  3805,  3806,  3813,  3818,  3823,  3828,  3833,  3842,  3847,
    3858,  3861,  3864,  3867,  3873,  3876,  3879,  3885,  3892,  3895,
    3906,  3910,  3914,  3919,  3924,  3929,  3934,  3940,  3946,  3951,
    3956,  3962,  3968,  3974,  3980,  3987,  3999,  4000,  4001,  4002,
    4003,  4004,  4005,  4006,  4007,  4008,  4009,  4010,  4011,  4012,
    4013,  4014,  4015,  4016,  4017,  4018,  4019,  4020,  4021,  4022,
    4023,  4024,  4025,  4026,  4032,  4032,  4116,  4117,  4121,  4122,
    4123,  4124,  4125,  4126,  4127,  4128,  4129,  4130,  4131,  4132,
    4133,  4134,  4135,  4136,  4137,  4141,  4142,  4155,  4155,  4174,
    4175,  4176,  4181,  4181,  4223,  4226,  4229,  4232,  4235,  4238,
    4241,  4243,  4246,  4253,  4258,  4274,  4275,  4274,  4284,  4285,
    4288,  4291,  4298,  4303,  4319,  4320,  4319,  4329,  4330,  4338,
    4342,  4350,  4352,  4350,  4361,  4362,  4365,  4370,  4371,  4370
};
#endif

#if YYDEBUG || YYERROR_VERBOSE || YYTOKEN_TABLE
/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "$end", "error", "$undefined", "NAMESPACE", "FX_TRUE", "FX_FALSE",
  "FX_FLOAT", "FX_INT", "FX_BOOL", "FX_STR", "GLSLSHADER", "HLSL10SHADER",
  "HLSL11SHADER", "CGSHADER", "SAMPLER_STATE", "DST_STATE", "PR_STATE",
  "COLOR_SAMPLE_STATE", "RASTERIZATION_STATE", "TECHNIQUE", "PASS",
  "CODEBODY", "VARNAME", "TEXTURERESOURCE", "TEXTURERESOURCE1D",
  "TEXTURERESOURCE2D", "TEXTURERESOURCE3D", "TEXTURERESOURCERECT",
  "TEXTURERESOURCECUBE", "VOID_TYPE", "UNSIGNED", "HIGHP", "MEDIUMP",
  "LOWP", "UNIFORM", "CSTBUFFER", "FLOAT_TYPE", "FLOAT2_TYPE",
  "FLOAT3_TYPE", "FLOAT4_TYPE", "MAT2_TYPE", "MAT3_TYPE", "MAT4_TYPE",
  "BOOL_TYPE", "BOOL2_TYPE", "BOOL3_TYPE", "BOOL4_TYPE", "INT_TYPE",
  "INT2_TYPE", "INT3_TYPE", "INT4_TYPE", "TEXTURE1D_TYPE",
  "TEXTURE2D_TYPE", "TEXTURE2DSHADOW_TYPE", "TEXTURE2DRECT_TYPE",
  "TEXTURE3D_TYPE", "TEXTURECUBE_TYPE", "SAMPLER1D_TYPE", "SAMPLER2D_TYPE",
  "SAMPLER2DSHADOW_TYPE", "SAMPLER2DRECT_TYPE", "SAMPLER3D_TYPE",
  "SAMPLERCUBE_TYPE", "EXTENSION", "SEPARATE_SHADER", "DEPTHMASK",
  "DEPTHFUNC", "VERTEXPROGRAM", "FRAGMENTPROGRAM", "GEOMETRYPROGRAM",
  "HULLPROGRAM", "EVALPROGRAM", "SHDPROFILE", "SAMPLERRESOURCE",
  "SAMPLERTEXUNIT", "SETSAMPLERSTATE", "SETDSTSTATE",
  "SETRASTERIZATIONSTATE", "SETCOLORSAMPLESTATE", "IMAGERESOURCE",
  "IMAGEUNIT", "IMAGEACCESS", "IMAGELAYER", "IMAGELAYERED", "WRITE_ONLY",
  "READ_ONLY", "READ_WRITE", "VERTEXPROGRAMOVERRIDE",
  "FRAGMENTPROGRAMOVERRIDE", "GEOMETRYPROGRAMOVERRIDE",
  "HULLPROGRAMOVERRIDE", "EVALPROGRAMOVERRIDE", "IMAGEACCESSOVERRIDE",
  "IMAGELAYEROVERRIDE", "IMAGELAYEREDOVERRIDE", "IMAGERESOURCEOVERRIDE",
  "UNIFORMOVERRIDE", "IMAGEUNITOVERRIDE", "TEXTURERESOURCEOVERRIDE",
  "SAMPLERRESOURCEOVERRIDE", "SAMPLERTEXUNITOVERRIDE",
  "SETSAMPLERSTATEOVERRIDE", "GLNONE", "GLACCUM", "GLLOAD", "GLRETURN",
  "GLMULT", "GLADD", "GLNEVER", "GLLESS", "GLEQUAL", "GLGREATER",
  "GLNOTEQUAL", "GLALWAYS", "GLLEQUAL", "GLGEQUAL", "GLKEEP", "GLREPLACE",
  "GLINCR", "GLDECR", "GLUPPERLEFT", "GLLOWERLEFT", "GLFRONTLEFT",
  "GLFRONTRIGHT", "GLBACKLEFT", "GLBACKRIGHT", "GLFRONT", "GLBACK",
  "GLLEFT", "GLRIGHT", "GLFRONTANDBACK", "GLCCW", "GLCW", "GLPOINT",
  "GLLINE", "GLFILL", "GLZERO", "GLONE", "GLSRCCOLOR",
  "GLONEMINUSSRCCOLOR", "GLSRCALPHA", "GLONEMINUSSRCALPHA", "GLDSTALPHA",
  "GLONEMINUSDSTALPHA", "GLDSTCOLOR", "GLONEMINUSDSTCOLOR",
  "GLSRCALPHASATURATE", "GLCONSTANTCOLOR", "GLONEMINUSCONSTANTCOLOR",
  "GLCONSTANTALPHA", "GLONEMINUSCONSTANTALPHA", "GLFUNCADD", "GLMIN",
  "GLMAX", "GLFUNCSUBTRACT", "GLFUNCREVERSESUBTRACT", "GLCLEAR", "GLSET",
  "GLCOPY", "GLCOPYINVERTED", "GLNOOP", "GLINVERT", "GLAND", "GLNAND",
  "GLOR", "GLNOR", "GLXOR", "GLEQUIV", "GLANDREVERSE", "GLANDINVERTED",
  "GLORREVERSE", "GLORINVERTED", "GLTEXTUREMAGFILTER",
  "GLTEXTUREMINFILTER", "GLTEXTUREWRAPS", "GLTEXTUREWRAPT",
  "GLTEXTUREWRAPR", "GLNEAREST", "GLLINEAR", "GLNEARESTMIPMAPNEAREST",
  "GLLINEARMIPMAPNEAREST", "GLNEARESTMIPMAPLINEAR", "GLLINEARMIPMAPLINEAR",
  "GLCLAMPTOEDGE", "GLCLAMP", "GLREPEAT", "GLMIRROREDREPEAT",
  "GLTEXTURECOMPAREMODE", "GLCOMPARERTOTEXTURE", "GLTEXTURECOMPAREFUNC",
  "GLDEPTHTEXTUREMODE", "GLPOINTSIZE", "GLPOINTSMOOTH", "GLPOINTSPRITE",
  "GLPROGRAMPOINTSIZE", "GLPOINTSPRITECOORDORIGIN", "GLLINEWIDTH",
  "GLLINESMOOTH", "GLCOMBINEDLINESTIPPLE", "GLLINESTIPPLEPATTERN",
  "GLLINESTIPPLEREPEAT", "GLLINESTIPPLE", "GLCULLFACE", "GLCULLFACEMODE",
  "GLFRONTFACE", "GLPOLYGONSMOOTH", "GLPOLYGONMODE", "GLPOLYGONOFFSET",
  "GLPOLYGONOFFSETFACTOR", "GLPOLYGONOFFSETUNITS", "GLPOLYGONOFFSETPOINT",
  "GLPOLYGONOFFSETLINE", "GLPOLYGONOFFSETFILL", "GLPOLYGONSTIPPLEBIT",
  "GLRASTERIZERDISCARD", "GLALPHATEST", "GLALPHAFUNC", "GLALPHATESTFUNC",
  "GLALPHATESTREF", "GLBLENDFUNC", "GLBLENDFUNCSEPARATE", "GLBLEND",
  "GLBLENDSRC", "GLBLENDSRCRGB", "GLBLENDSRCALPHA", "GLBLENDDST",
  "GLBLENDDSTRGB", "GLBLENDDSTALPHA", "GLBLENDEQUATIONSEPARATE",
  "GLBLENDEQUATION", "GLBLENDEQUATIONRGB", "GLBLENDEQUATIONALPHA",
  "GLBLENDCOLOR", "GLFRAMEBUFFERSRGB", "GLDITHER", "GLCOLORLOGICOP",
  "GLLOGICOPMODE", "GLCOLORWRITEMASK", "GLMULTISAMPLE", "GLSAMPLEMASK",
  "GLSAMPLEMASKVALUE", "GLSAMPLEALPHATOONE", "GLSAMPLEALPHATOCOVERAGE",
  "GLDEPTHTEST", "GLDEPTHFUNC", "GLDEPTHWRITEMASK", "GLDEPTHCLAMP",
  "GLDEPTHBOUNDSTESTEXT", "GLDEPTHBOUNDSEXT", "GLSTENCILTEST",
  "GLSTENCILFUNC", "GLSTENCILFUNCSEPARATE", "GLSTENCILVALUEMASK",
  "GLSTENCILREF", "GLSTENCILFAIL", "GLSTENCILPASSDEPTHFAIL",
  "GLSTENCILPASSDEPTHPASS", "GLSTENCILOP", "GLSTENCILOPSEPARATE",
  "GLSTENCILBACKOP", "GLSTENCILWRITEMASK", "GLSTENCILBACKFUNC",
  "GLSTENCILBACKVALUEMASK", "GLSTENCILBACKREF", "GLSTENCILBACKFAIL",
  "GLSTENCILBACKPASSDEPTHFAIL", "GLSTENCILBACKPASSDEPTHPASS",
  "GLSTENCILBACKWRITEMASK", "ATTRIBUTE", "RENDERGROUP", "RENDERMODE",
  "RENDERMODE_ENUM", "BLITFBOTOACTIVETARGET", "CLEARMODE", "CLEAR_ENUM",
  "CURRENTTARGET", "CLEARCOLOR", "VIEWPORT", "FBO", "RENDERBUFFER",
  "RENDERTEXTURE", "RENDERTEXTURERECT", "DSTTARGET", "COLORTARGETS",
  "RESOURCEMSAA", "RESOURCESIZE", "RESOURCESIZEFACTOR",
  "RESOURCERESOLVETARGET", "RESOURCEFORMAT", "RESOURCEFORMAT_ENUM",
  "APPDEFINED", "SWAPRESOURCES", "LOOP", "ACTIVE", "OFF", "GLOBAL",
  "LOCAL", "CUDAKERNEL", "CUDACODE", "CUDAMODULE", "CUDASHAREDMEMORY",
  "CUDAGRID", "CUDABLOCK", "OPTIX_CONTEXT", "OPTIX_CONTEXT_ENTRY",
  "RAY_GENERATION_PROGRAM", "EXCEPTION_PROGRAM", "INTERSECTION_PROGRAM",
  "BOUNDING_BOX_PROGRAM", "MISS_PROGRAM", "DEFAULT_MATERIAL", "STACK_SIZE",
  "OUTPUT_BUFFER", "OPTIX_MATERIAL", "CLOSEST_HIT_PROGRAM",
  "ANY_HIT_PROGRAM", "OPTIX_BUFFER", "PTX_PROGRAM_FROM_FILE", "FILE_NAME",
  "ENTRY_POINT", "COMPUTEGROUPS", "COMPUTEPROGRAM",
  "COMPUTEPROGRAMOVERRIDE", "SVGPATH", "PSPATH", "PATHSOURCEOVERRIDE",
  "PATHSOURCE", "GLPATHSTENCILDEPTHOFFSET", "GLPATHSTENCILFUNC",
  "GLPATHFOGGEN", "GLPATHTEXGEN", "GLPATHCOLORGEN", "GLPATHCOVERDEPTHFUNC",
  "GLPATHSTROKEWIDTH", "GLPATHINITIALENDCAP", "GLPATHTERMINALENDCAP",
  "GLPATHINITIALDASHCAP", "GLPATHTERMINALDASHCAP", "GLPATHJOINSTYLE",
  "GLPATHMITERLIMIT", "GLPATHDASHOFFSET", "GLPATHDASHOFFSETRESET",
  "GLPATHCLIENTLENGTH", "GLPATHFILLMODE", "GLPATHFILLMASK",
  "GLPATHFILLCOVERMODE", "GLPATHSTROKEMASK", "GLPATHSTROKEREFERENCE",
  "GLPATHSTROKECOVERMODE", "GLPATHTRANSFORMTYPE", "GLPATHTRANSFORM",
  "GLFOGCOORDINATE", "GLFRAGMENTDEPTH", "GLOBJECTLINEAR",
  "GLPATHOBJECTBOUNDINGBOX", "GLEYELINEAR", "GLFLAT", "GLSQUARE",
  "GLROUND", "GLTRIANGULAR", "GLMITERREVERT", "GLMITERTRUNCATE", "GLBEVEL",
  "GLMOVETORESET", "GLMOVETOCONTINUES", "GLCOUNTUP", "GLCOUNTDOWN",
  "GLBOUNDINGBOX", "GLCONVEXHULL", "GLTRANSLATE2D", "GLTRANSLATEX",
  "GLTRANSLATEY", "GLTRANSLATE3D", "GLAFFINE2D", "GLAFFINE3D",
  "GLTRANSPOSEAFFINE2D", "GLTRANSPOSEAFFINE3D", "GLLUMINANCE", "GLALPHA",
  "GLINTENSITY", "GLLUMINANCEALPHA", "GLRGB", "GLRGBA", "'='", "'-'",
  "'+'", "'*'", "'/'", "NEG", "'^'", "';'", "':'", "'('", "')'", "','",
  "'{'", "'}'", "'|'", "'<'", "'>'", "'['", "']'", "'.'", "$accept",
  "input", "extension", "name", "fexp", "iexp", "bexp", "asbexp", "asiexp",
  "asfexp", "separator", "shaderlist", "one_or_more_shaders",
  "varnamelist", "liststart", "listend", "fexplist", "fexpveclist",
  "iexplist", "iexpveclist", "bexplist", "argval", "argvallist", "glenum",
  "clear_flags", "passstates", "pass", "$@1", "$@2", "passes", "tech",
  "$@3", "$@4", "global", "global_true", "nsvarname",
  "global_varname_annots", "glsl", "svgpath", "pspath", "hlsl11", "hlsl10",
  "unsigned", "pointer", "argument", "arglist", "cudak", "cudac",
  "texparams", "resourcetype", "texture", "$@5", "resourceparams",
  "resource", "$@6", "$@7", "$@8", "$@9", "$@10", "$@11", "fboparams",
  "fbo", "$@12", "$@13", "sfilter", "suvmapping", "dtexmode",
  "texcomparefn", "texcomparemode", "sstates", "sstate", "$@14",
  "passglsstates", "rstates", "rstate", "$@15", "cstates", "csstate",
  "$@16", "dststates", "dststate", "$@17", "passprstates", "prstates",
  "prstate", "$@18", "annotation", "annotations2", "annotations",
  "valueassignment_f", "valueassignment_fv", "valueassignment_fvl",
  "valueassignment_i", "valueassignment_b", "valueassignment_bv",
  "precision", "uniform_qualifiers", "uniformvalueassigment", "semantic",
  "arraySz", "arraysz_semantic_annotations_uniformvalueassigment",
  "uniform_type", "uniform", "$@19", "uniforms", "namespaceitem",
  "namespaceitems", "namespace", "$@20", "cstbufferuniforms", "cstbuffer",
  "$@21", "optix_context_data", "optix_context", "$@22", "$@23",
  "optix_material_data", "optix_material", "$@24", "$@25",
  "optix_buffer_data", "optix_buffer", "$@26", "$@27",
  "ptx_program_from_file_data", "ptx_program_from_file", "$@28", "$@29", 0
};
#endif

# ifdef YYPRINT
/* YYTOKNUM[YYLEX-NUM] -- Internal token number corresponding to
   token YYLEX-NUM.  */
static const yytype_uint16 yytoknum[] =
{
       0,   256,   257,   258,   259,   260,   261,   262,   263,   264,
     265,   266,   267,   268,   269,   270,   271,   272,   273,   274,
     275,   276,   277,   278,   279,   280,   281,   282,   283,   284,
     285,   286,   287,   288,   289,   290,   291,   292,   293,   294,
     295,   296,   297,   298,   299,   300,   301,   302,   303,   304,
     305,   306,   307,   308,   309,   310,   311,   312,   313,   314,
     315,   316,   317,   318,   319,   100,   101,   320,   321,   322,
     323,   324,   325,   326,   327,   328,   329,   330,   331,   332,
     333,   334,   335,   336,   337,   338,   339,   340,   341,   342,
     343,   344,   345,   346,   347,   348,   349,   350,   351,   352,
     353,   354,   355,   356,   357,   358,   359,   360,   361,   362,
     363,   364,   365,   366,   367,   368,   369,   370,   371,   372,
     373,   374,   375,   376,   377,   378,   379,   380,   381,   382,
     383,   384,   385,   386,   387,   388,   389,   390,   391,   392,
     393,   394,   395,   396,   397,   398,   399,   400,   401,   402,
     403,   404,   405,   406,   407,   408,   409,   410,   411,   412,
     413,   414,   415,   416,   417,   418,   419,   420,   421,   422,
     423,   424,   425,   426,   427,   428,   429,   430,   431,   432,
     433,   434,   435,   436,   437,   438,   439,   440,   441,   442,
     443,   444,   445,   446,   447,   448,   449,   450,   451,   452,
     453,   454,   455,   456,   457,   458,   459,   460,   461,   462,
     463,   464,   465,   466,   467,   468,   469,   470,   471,   472,
     473,   474,   475,   476,   477,   478,   479,   480,   481,   482,
     483,   484,   485,   486,   487,   488,   489,   490,   491,   492,
     493,   494,   495,   496,   497,   498,   499,   500,   501,   502,
     503,   504,   505,   506,   507,   508,   509,   510,   511,   512,
     513,   514,   515,   516,   517,   518,   519,   520,   521,   522,
     523,   524,   525,   526,   527,   528,   529,   530,   531,   532,
     533,   534,   535,   536,   537,   538,   539,   540,   541,   542,
     543,   544,   545,   546,   547,   548,   549,   550,   551,   552,
     553,   554,   555,   556,   557,   558,   559,   560,   561,   562,
     563,   564,   565,   566,   567,   568,   569,   570,   571,   572,
     573,   574,   575,   576,   577,   578,   579,   580,   581,   582,
     583,   584,   585,   586,   587,   588,   589,   590,   591,   592,
     593,   594,   595,   596,   597,   598,   599,   600,   601,   602,
     603,   604,   605,   606,   607,   608,   609,   610,   611,   612,
     613,   614,   615,   616,   617,   618,   619,   620,   621,   622,
     623,   624,   625,   626,   627,   628,   629,   630,   631,   632,
     633,   634,   635,    61,    45,    43,    42,    47,   636,    94,
      59,    58,    40,    41,    44,   123,   125,   124,    60,    62,
      91,    93,    46
};
# endif

/* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_uint16 yyr1[] =
{
       0,   403,   404,   404,   404,   404,   404,   404,   404,   404,
     404,   404,   404,   404,   404,   404,   404,   404,   404,   404,
     404,   404,   404,   404,   404,   404,   404,   404,   404,   405,
     406,   406,   407,   407,   407,   407,   407,   407,   407,   407,
     407,   407,   407,   407,   407,   407,   407,   407,   407,   407,
     407,   407,   408,   408,   408,   408,   408,   408,   409,   409,
     410,   410,   410,   411,   411,   411,   412,   412,   412,   413,
     413,   414,   414,   414,   415,   415,   415,   416,   416,   416,
     417,   418,   419,   419,   419,   419,   419,   420,   420,   420,
     421,   421,   422,   422,   422,   423,   423,   424,   424,   424,
     424,   425,   425,   425,   425,   425,   426,   426,   426,   426,
     426,   426,   426,   426,   426,   426,   426,   426,   426,   426,
     426,   426,   426,   426,   426,   426,   426,   426,   426,   426,
     426,   426,   426,   426,   426,   426,   426,   426,   426,   426,
     426,   426,   426,   426,   426,   426,   426,   426,   426,   426,
     426,   426,   426,   426,   426,   426,   426,   426,   426,   426,
     426,   426,   426,   426,   426,   426,   426,   426,   426,   426,
     426,   426,   426,   426,   426,   426,   426,   426,   426,   426,
     426,   426,   426,   426,   426,   426,   426,   426,   426,   426,
     426,   426,   426,   426,   426,   426,   426,   426,   426,   426,
     426,   426,   426,   426,   426,   426,   426,   427,   427,   428,
     428,   428,   428,   428,   428,   428,   428,   428,   428,   428,
     428,   428,   428,   428,   428,   428,   428,   428,   428,   428,
     428,   428,   428,   428,   428,   428,   428,   428,   428,   428,
     428,   428,   428,   428,   428,   428,   428,   428,   428,   428,
     428,   428,   428,   428,   428,   428,   428,   428,   428,   428,
     428,   428,   428,   428,   428,   428,   428,   428,   428,   428,
     428,   428,   428,   428,   428,   428,   428,   428,   428,   428,
     428,   428,   428,   428,   428,   428,   428,   428,   428,   428,
     428,   428,   428,   428,   428,   430,   429,   431,   429,   429,
     429,   429,   429,   432,   432,   432,   434,   433,   435,   433,
     436,   436,   437,   437,   437,   438,   439,   439,   439,   440,
     441,   442,   443,   444,   445,   445,   446,   446,   447,   447,
     448,   448,   448,   448,   449,   450,   451,   451,   451,   452,
     452,   452,   452,   452,   452,   453,   454,   453,   455,   455,
     455,   455,   455,   455,   455,   455,   455,   455,   455,   457,
     458,   456,   459,   460,   456,   461,   462,   456,   463,   463,
     463,   463,   463,   465,   466,   464,   467,   467,   467,   467,
     467,   467,   467,   468,   468,   468,   468,   468,   469,   469,
     469,   469,   470,   470,   470,   470,   470,   470,   470,   470,
     470,   471,   471,   471,   472,   472,   472,   472,   472,   472,
     472,   472,   472,   472,   474,   473,   473,   475,   475,   475,
     475,   475,   475,   475,   475,   475,   475,   475,   475,   475,
     475,   475,   475,   475,   475,   475,   475,   475,   475,   475,
     475,   475,   475,   475,   475,   475,   475,   475,   475,   475,
     475,   475,   475,   475,   475,   475,   475,   475,   475,   475,
     475,   475,   475,   475,   475,   475,   475,   475,   475,   475,
     475,   475,   475,   475,   475,   475,   475,   475,   475,   475,
     475,   475,   475,   475,   475,   475,   475,   475,   475,   475,
     475,   475,   475,   476,   476,   476,   476,   476,   476,   476,
     476,   476,   476,   476,   476,   476,   476,   476,   476,   476,
     476,   476,   476,   476,   476,   476,   476,   478,   477,   477,
     479,   479,   479,   479,   479,   479,   479,   479,   479,   479,
     479,   479,   479,   479,   479,   479,   479,   479,   479,   479,
     479,   479,   479,   479,   479,   479,   481,   480,   480,   482,
     482,   482,   482,   482,   482,   482,   482,   482,   482,   482,
     482,   482,   482,   482,   482,   482,   482,   482,   482,   482,
     482,   482,   484,   483,   483,   485,   485,   485,   485,   485,
     485,   485,   485,   485,   485,   485,   485,   485,   485,   485,
     485,   485,   485,   485,   485,   485,   485,   486,   486,   486,
     486,   486,   486,   486,   486,   486,   486,   486,   486,   486,
     486,   486,   486,   486,   486,   486,   486,   486,   486,   486,
     486,   488,   487,   487,   489,   489,   489,   489,   489,   489,
     490,   491,   491,   492,   493,   494,   495,   495,   496,   497,
     498,   498,   498,   499,   499,   499,   499,   499,   499,   499,
     500,   500,   500,   500,   500,   500,   500,   501,   502,   502,
     503,   503,   503,   503,   503,   503,   503,   503,   503,   503,
     503,   503,   503,   503,   503,   503,   504,   504,   504,   504,
     504,   504,   504,   504,   504,   504,   504,   504,   504,   504,
     504,   504,   504,   504,   504,   504,   504,   504,   504,   504,
     504,   504,   504,   504,   506,   505,   507,   507,   508,   508,
     508,   508,   508,   508,   508,   508,   508,   508,   508,   508,
     508,   508,   508,   508,   508,   509,   509,   511,   510,   512,
     512,   512,   514,   513,   515,   515,   515,   515,   515,   515,
     515,   515,   515,   515,   515,   517,   518,   516,   519,   519,
     519,   519,   519,   519,   521,   522,   520,   523,   523,   523,
     523,   525,   526,   524,   527,   527,   527,   529,   530,   528
};

/* YYR2[YYN] -- Number of symbols composing right hand side of rule YYN.  */
static const yytype_uint8 yyr2[] =
{
       0,     2,     0,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     4,
       1,     1,     1,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     2,     3,     3,     3,
       3,     3,     1,     3,     3,     3,     2,     3,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     0,
       1,     1,     1,     3,     0,     1,     3,     0,     1,     3,
       1,     1,     1,     1,     3,     3,     2,     3,     5,     2,
       1,     3,     3,     5,     2,     1,     3,     1,     1,     1,
       1,     0,     1,     1,     3,     2,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     3,     0,
       2,     2,     5,     5,     5,     8,     5,     8,     5,     8,
       5,     8,     5,     8,     5,     8,     5,     8,     5,     8,
       5,     8,     5,     8,     5,     8,     5,     8,     5,     8,
       5,     8,    10,     8,     7,    11,    13,    11,    13,     5,
       8,     8,     8,     8,    10,    12,    12,     8,     8,    10,
      12,     8,     8,     8,     8,    10,    10,     8,     8,    10,
       5,     6,     5,     6,     5,     6,     5,     5,     5,     5,
       5,     8,     7,     7,     8,     5,     5,     7,     5,     9,
       5,     5,     5,     8,     3,     0,     6,     0,     7,     7,
       7,     3,     4,     1,     2,     2,     0,     6,     0,     7,
       1,     1,     0,     1,     1,     1,     0,     2,     3,     4,
       4,     4,     4,     4,     0,     1,     0,     1,     3,     4,
       0,     1,     1,     3,     7,     4,     0,     5,     2,     1,
       1,     1,     1,     1,     1,     3,     0,     7,     0,     5,
       5,     5,     7,     5,     5,     7,     5,     5,     5,     0,
       0,     9,     0,     0,     9,     0,     0,     9,     0,     5,
       5,     7,     5,     0,     0,     9,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     0,     5,     5,     5,     5,     5,
       5,     5,     5,     3,     0,     7,     2,     4,     4,     4,
       4,     4,     4,     4,     4,     4,     4,     4,     4,     4,
       4,     4,     8,     4,     4,     8,     4,     4,     4,     4,
       4,     4,     4,     8,     4,     8,    12,     4,     4,     4,
       4,     4,     4,     4,     4,     4,     8,    12,     4,     4,
       4,     4,    12,     4,     4,     4,     4,     4,     4,     4,
       4,     4,     8,     4,     4,    10,    12,     4,     4,     4,
       4,     4,     4,    10,    12,     4,    10,     4,     4,     4,
       4,     4,     4,     0,     5,     5,     5,     5,     5,     5,
       5,     5,     5,     5,     5,     5,     5,     5,     5,     9,
       5,     5,     5,     5,     5,     5,     3,     0,     7,     2,
       0,     5,     5,     5,     5,     5,     5,     5,     5,     5,
       5,     5,     5,     4,     8,    13,     5,     5,     5,     5,
      13,     5,     5,     5,     5,     3,     0,     7,     2,     0,
       5,     5,     5,     5,     5,     9,     5,     5,     5,     5,
       5,     5,     5,     5,     5,     5,     5,     5,     5,     5,
       5,     3,     0,     7,     3,     8,     4,     4,    13,    13,
       4,     4,     4,     4,     4,     4,     4,     4,     4,     4,
       4,     4,     4,     4,     4,     4,     4,     0,     9,     5,
       5,    14,    14,     5,     5,     5,     5,     5,     5,     5,
       5,     5,     5,     5,     5,     5,     5,     5,     5,     5,
       3,     0,     7,     3,     0,     5,     5,     5,     5,     2,
       3,     0,     1,     2,     4,     4,     2,     2,     2,     4,
       1,     1,     1,     1,     2,     2,     3,     1,     1,     2,
       1,     1,     1,     1,     1,     1,     1,     2,     2,     3,
       0,     1,     1,     2,     1,     2,     2,     3,     1,     2,
       2,     3,     2,     3,     3,     4,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     0,     6,     1,     2,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     2,     0,     7,     0,
       2,     3,     0,     4,     6,     7,     5,     5,     5,     5,
       5,     5,    10,     8,     7,     0,     0,     8,     0,     7,
       7,    10,     8,     7,     0,     0,     8,     0,     5,     5,
       7,     0,     0,     8,     0,     5,     5,     0,     0,     8
};

/* YYDEFACT[STATE-NAME] -- Default rule to reduce with in state
   STATE-NUM when YYTABLE doesn't specify something else to do.  Zero
   means the default is an error.  */
static const yytype_uint16 yydefact[] =
{
       2,     0,     1,    28,     0,   316,   316,   316,     0,     0,
       0,     0,     0,   306,   339,   340,   341,   342,   343,   344,
     640,   641,   642,   643,   316,     0,     0,     0,     0,     0,
     311,   310,   316,   316,   316,   316,     0,   316,   316,   316,
       3,     4,     5,   647,     6,    22,    23,     8,     7,    10,
       9,     0,    11,    12,    13,     0,     0,     0,     0,     0,
     648,     0,   706,    21,    19,    20,    24,    25,    26,    27,
     727,   315,     0,   631,     0,     0,     0,   414,   572,   621,
     546,   517,   308,   631,   644,   645,   732,     0,   312,   312,
     312,   312,     0,     0,   745,   754,   761,   767,     0,     0,
     649,   315,   346,   416,   519,   548,     0,     0,   703,   676,
     677,   678,   679,   688,   689,   690,   684,   685,   686,   687,
     680,   681,   682,   683,   691,   692,   693,   694,   695,   696,
     697,   698,   699,   700,   701,   702,     0,   707,   631,   631,
     624,   632,   317,     0,     0,     0,   631,   631,   631,   631,
     631,   631,     0,   646,   729,     0,   314,   313,   373,   365,
     359,   362,   330,     0,   631,   631,   631,   631,     0,     0,
     345,   631,   574,   623,   704,     0,   318,     0,   319,   323,
     322,     0,     0,     0,     0,     0,     0,     0,     0,   733,
      58,    59,    29,   631,   631,   631,   631,   331,   325,   326,
       0,   332,     0,   335,   746,   755,   762,   768,   320,   321,
       0,     0,     0,   629,     0,   630,   404,   549,   597,   520,
     493,     0,     0,   295,     0,   303,     0,   730,     0,   374,
     366,   360,   363,   327,     0,   326,     0,   324,     0,     0,
       0,     0,   336,   656,     0,     0,     0,     0,   650,   651,
     652,   653,   654,   655,   662,     0,     0,     0,   708,   723,
     724,   711,   710,   713,   712,   714,   715,   716,     0,     0,
       0,     0,     0,   709,   725,     0,   722,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   297,   631,     0,
     305,   307,   304,   731,     0,     0,     0,     0,   328,     0,
       0,   333,     0,   748,   757,   764,     0,   637,    32,    52,
       0,     0,    80,   633,   636,   638,     0,   657,     0,     0,
     658,     0,   663,     0,   666,     0,   670,     0,   705,   728,
     726,     0,    66,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   415,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   573,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   622,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   547,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   518,   309,     0,   301,   631,     0,     0,     0,   368,
     348,   348,   348,   329,   334,     0,     0,     0,     0,     0,
     338,     0,   347,    46,    56,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    82,    83,    95,
       0,     0,     0,     0,    56,     0,     0,     0,     0,   659,
     667,   671,     0,   674,   625,   627,   628,   626,   413,     0,
       0,     0,     0,     0,     0,     0,     0,   571,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   620,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   106,   107,   108,   109,   110,   111,   112,
     113,   114,   115,   116,   117,   118,   119,   120,   121,   122,
     123,   124,   125,   126,   127,   128,   129,   130,   131,   132,
     133,   134,   135,   136,   137,   138,   139,   140,   141,   142,
     143,   144,   145,   146,   147,   148,   149,   150,   151,   152,
     153,   154,   155,   156,   157,   158,   159,   160,   161,   162,
     163,   164,   165,   166,   167,   168,   169,   170,   171,   172,
     173,   174,   181,   182,   183,   184,   185,   186,   187,   188,
     189,   190,   191,   192,   193,   194,   195,   196,   197,   198,
     199,   200,   201,   202,   203,   204,   205,   206,   175,   176,
     177,   178,   179,   180,     0,   545,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   516,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     302,     0,   209,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   747,     0,
       0,     0,   756,     0,     0,   763,     0,     0,   769,     0,
      51,    57,    36,    38,    33,    35,    39,    41,    42,    44,
      47,    49,    37,    54,    34,    53,    40,    55,    43,    45,
      48,    50,     0,    86,     0,    81,   634,    89,     0,   635,
       0,   639,    54,    53,    55,   675,   382,   376,   377,   378,
     379,   380,   381,     0,     0,   387,   386,   384,   383,   385,
       0,     0,     0,   403,   401,   402,     0,   400,   392,   395,
     397,   396,   398,   399,   393,   394,     0,   391,   388,   390,
     389,     0,    62,    61,    60,     0,     0,     0,     0,     0,
       0,     0,     0,    64,    63,    65,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    67,    68,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   533,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   209,
       0,     0,     0,     0,     0,     0,   375,     0,     0,     0,
       0,     0,     0,     0,   367,   361,   364,    77,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    87,    84,    85,     0,    96,   406,
     405,   407,   408,   409,   410,   411,   412,   550,   551,   552,
     553,   554,    69,   556,   557,   558,   559,   560,   561,   562,
     563,   564,   565,   566,   567,   568,   569,   570,     0,   599,
     600,     0,     0,   603,   604,   605,   606,   607,   608,   609,
     610,   611,   612,   613,   614,   615,   616,   618,   619,   617,
       0,   521,   522,   523,   524,   525,   526,   527,   528,   529,
     530,   531,   532,    69,   536,   537,   538,   539,    69,   541,
     542,   543,   544,   494,   495,   496,   497,   498,   499,   500,
     501,   502,   503,   504,   505,   506,   507,    69,   508,   510,
     511,   512,   513,   514,   515,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   296,   210,   211,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    78,    69,     0,
      77,     0,     0,     0,     0,     0,     0,     0,    77,    77,
       0,     0,     0,     0,     0,   337,     0,    70,     0,     0,
       0,     0,     0,     0,     0,     0,   298,   294,     0,     0,
      74,     0,    74,     0,    74,     0,    74,     0,    74,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    74,     0,    74,     0,    74,     0,    74,     0,
      74,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    74,     0,     0,     0,     0,     0,
       0,    74,     0,    74,     0,    74,     0,    74,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   300,   299,     0,     0,     0,    77,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      69,   736,   737,   738,   739,   740,   741,     0,    69,    69,
     759,    90,     0,   758,   765,   766,    88,     0,     0,     0,
       0,     0,    69,    69,     0,     0,     0,    71,    72,    69,
       0,     0,    30,    31,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   207,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   369,
     370,   372,    69,   350,   349,   351,   353,     0,   354,     0,
     356,   357,   358,    79,   734,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   534,     0,
       0,     0,     0,     0,     0,     0,     0,   214,    69,     0,
     216,     0,   222,     0,   224,     0,   226,     0,   249,     0,
       0,     0,   270,     0,   272,     0,   274,     0,     0,     0,
     228,     0,   230,     0,   236,     0,   238,     0,   240,     0,
       0,     0,     0,     0,     0,     0,   417,   418,   419,   420,
     421,   422,   423,   424,   425,   426,   427,   428,   429,   430,
     431,    69,     0,   433,   434,   436,   437,   438,   439,   440,
       0,   441,   442,     0,     0,   444,   447,   448,   449,   450,
     451,   452,     0,   455,   453,   454,    69,   458,   459,   460,
     461,    69,   463,   464,   465,   466,   467,   468,   469,   470,
     471,    69,   473,     0,   474,     0,   477,   478,   480,   481,
     482,     0,     0,   479,     0,   485,   487,   488,   489,   490,
     491,   492,     0,   277,   276,   278,   279,     0,   280,     0,
       0,     0,     0,   285,   286,     0,   288,   290,     0,    94,
     291,     0,   292,   212,   213,     0,   218,     0,   232,     0,
     234,     0,   220,     0,     0,   576,   577,     0,     0,   580,
     581,   582,   583,   584,   585,   586,   587,   588,   589,   590,
     591,   592,   593,   595,   596,   594,     0,     0,     0,     0,
      77,   744,   735,     0,    77,   753,   750,   749,    91,   760,
     555,   598,     0,     0,    69,    69,   509,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    73,    76,    74,    74,
      74,    74,    74,     0,     0,     0,   271,   273,   275,     0,
       0,    74,    74,    74,    74,    74,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   208,     0,     0,
       0,     0,     0,    92,     0,     0,    74,    74,    74,    74,
       0,     0,     0,   371,   352,   355,   743,    69,   752,    69,
       0,     0,     0,     0,     0,     0,    77,   244,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    69,    69,     0,     0,     0,     0,
       0,     0,     0,   282,     0,   283,     0,   100,    99,   102,
      98,    97,   103,     0,     0,   287,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   250,
     243,    69,     0,     0,     0,     0,     0,     0,   215,   217,
     223,   225,   227,   252,     0,   261,   267,   257,     0,   263,
     229,   231,   237,   239,   241,   258,   264,   251,   253,     0,
     262,   268,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   293,   281,   284,   105,
       0,     0,    93,   219,   233,   235,   221,     0,     0,     0,
     742,   751,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   432,   435,   443,   445,
       0,   456,    69,    69,   472,     0,     0,     0,     0,     0,
     104,   289,   575,     0,     0,     0,     0,   535,   540,   242,
       0,     0,     0,   254,   265,   269,   259,   266,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   601,   602,   245,     0,     0,   247,     0,     0,     0,
       0,     0,     0,   475,     0,   483,     0,   486,     0,     0,
       0,     0,   255,   260,   256,     0,     0,     0,     0,     0,
       0,     0,   246,   248,   446,   457,   462,   476,   484,     0,
       0,   578,   579
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
      -1,     1,    41,  1444,   802,   803,   804,   805,   816,   335,
    1407,  1439,  1440,  1198,  1441,   756,   481,   482,  1778,  1574,
     483,  1972,  1973,   654,  1562,   900,   225,   288,   445,   226,
      42,    83,   151,    72,   158,    73,    74,   258,   259,   260,
     261,   262,   200,   234,   201,   202,   263,   264,   306,    51,
     265,   171,   706,   266,   195,   296,   196,   297,   194,   295,
     705,   267,   193,   294,   773,   780,   801,   796,   786,   278,
     268,   146,  1183,   284,   269,   150,   283,   270,   149,   279,
     271,   147,  1184,   280,   272,   148,   177,   141,   142,   248,
     249,   250,   251,   252,   253,    60,    61,   254,   255,   256,
     257,   136,    62,   211,   273,   274,   275,    64,   138,   189,
     276,   154,   456,    66,   164,   238,   457,    67,   165,   239,
     458,    68,   166,   240,   459,    69,   167,   241
};

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
#define YYPACT_NINF -1853
static const yytype_int16 yypact[] =
{
   -1853,  3414, -1853, -1853,   118,    17,    17,    17,   129,   129,
     129,   129,   129,   158, -1853, -1853, -1853, -1853, -1853, -1853,
   -1853, -1853, -1853,   557,    17,   -31,   129,   129,   129,   129,
   -1853, -1853,    17,    17,    17,    17,   188,    17,    17,    17,
   -1853, -1853, -1853,   649, -1853, -1853, -1853, -1853, -1853, -1853,
   -1853,   230, -1853, -1853, -1853,  3472,  3528,  3618,  3854,  3936,
   -1853,  2391, -1853,   658, -1853, -1853, -1853, -1853, -1853, -1853,
   -1853, -1853,   129,  -329,  -296,  -277,  -121, -1853, -1853, -1853,
   -1853, -1853, -1853,  -329,   649, -1853, -1853,  -113,    79,    79,
      79,    79,   -68,   -76, -1853, -1853, -1853, -1853,   -53,    44,
   -1853,    10, -1853, -1853, -1853, -1853,    51,    58, -1853, -1853,
   -1853, -1853, -1853, -1853, -1853, -1853, -1853, -1853, -1853, -1853,
   -1853, -1853, -1853, -1853, -1853, -1853, -1853, -1853, -1853, -1853,
   -1853, -1853, -1853, -1853, -1853, -1853,   429, -1853,  -329,  -329,
   -1853, -1853, -1853,   444,   459,   463,  -329,  -329,  -329,  -329,
    -329,  -329,   122, -1853,   125,   430, -1853, -1853, -1853, -1853,
   -1853, -1853,    83,   502,  -329,  -329,  -329,  -329,   505,   509,
   -1853,  -329, -1853, -1853, -1853,   146, -1853,     3, -1853, -1853,
   -1853,   148,   150,   182,   198,   206,   209,    12,   321, -1853,
   -1853, -1853, -1853,  -329,  -329,  -329,  -329, -1853, -1853,   175,
     486, -1853,   163, -1853, -1853, -1853, -1853, -1853, -1853, -1853,
     215,     6,  3050, -1853,   229, -1853, -1853, -1853, -1853,   226,
   -1853,    12,   592,   596,   228, -1853,    23, -1853,   645, -1853,
   -1853, -1853, -1853, -1853,   620,   175,   249,   217,   252,   254,
     255,   257, -1853, -1853,   202,   631,    61,     8, -1853, -1853,
   -1853, -1853, -1853, -1853, -1853,    39,    34,   266, -1853, -1853,
   -1853, -1853, -1853, -1853, -1853, -1853, -1853, -1853,    48,   272,
     548,  1887,  2518,   658, -1853,  4080, -1853,   239,    47,  1109,
     657,   279,   281,   274,  1945,    25,  -267, -1853,  -329,   120,
   -1853, -1853, -1853, -1853,   271,   280,   289,   290, -1853,   647,
     666, -1853,   388, -1853, -1853, -1853,     9, -1853, -1853, -1853,
     219,   219, -1853,   573,   580, -1853,   262, -1853,   208,   208,
   -1853,  -276, -1853,   138, -1853,   165, -1853,    40, -1853, -1853,
   -1853,   305,   573,   425,   307,   311,   313,   322,   333,   343,
     351,   352,   353,   365,   369, -1853,   363,   371,   372,   373,
     390,   392,   393,   394,   406,   407,   410,   418,   419,   438,
     439,   440,   441,   451,   453,   455,   457,   464, -1853,   458,
     467,   468,   471,   408,   466,   472,   480,   487,   495,   496,
     501,   503,   506,   510,   515,   517,   523,   524,   528,   530,
     531,   538, -1853,   493,  4736,   526,   539,   546,   549,   551,
     560,   562,   566,   567,   569,   572,   578,   587,   590,   591,
     593,   594,   595,   597,   598,   623,   630,   632, -1853,   534,
     634,   635,   636,   638,   639,   642,   652,   653,   655,   661,
     662,   665,   667,   669,   671,   673,   674,   676,   677,   682,
     683, -1853, -1853,   543, -1853,  -329,   507,   542,   582, -1853,
   -1853, -1853, -1853, -1853, -1853,   684,   550,   -13,  -125,  -225,
   -1853,   686, -1853,   625,   627,   395,   446,   219,   219,   219,
     219,   219,   219,   219,   219,   219,   219,   573,   580, -1853,
     219,     5,    37,   -56, -1853,   420,   208,   208,   208, -1853,
   -1853, -1853,   195, -1853, -1853, -1853, -1853, -1853, -1853,   153,
     153,   132,   132,   132,    89,   629,     7, -1853,    11,  4736,
      11,    11,    11,   493,    11,  4736,    11,    11,  4736,  4736,
    4736,    11,  4736,    11,    11,  4736,  4736,  4736,    11, -1853,
     584,  4736,  4736,    11,    11,  4736,    11,  4736,  4736,  4736,
    4736,  4736,    11,    11,  4736,    11,  4736,    11,  4736,  4736,
    4736,  4736,  4736, -1853, -1853, -1853, -1853, -1853, -1853, -1853,
   -1853, -1853, -1853, -1853, -1853, -1853, -1853, -1853, -1853, -1853,
   -1853, -1853, -1853, -1853, -1853, -1853, -1853, -1853, -1853, -1853,
   -1853, -1853, -1853, -1853, -1853, -1853, -1853, -1853, -1853, -1853,
   -1853, -1853, -1853, -1853, -1853, -1853, -1853, -1853, -1853, -1853,
   -1853, -1853, -1853, -1853, -1853, -1853, -1853, -1853, -1853, -1853,
   -1853, -1853, -1853, -1853, -1853, -1853, -1853, -1853, -1853, -1853,
   -1853, -1853, -1853, -1853, -1853, -1853, -1853, -1853, -1853, -1853,
   -1853, -1853, -1853, -1853, -1853, -1853, -1853, -1853, -1853, -1853,
   -1853, -1853, -1853, -1853, -1853, -1853, -1853, -1853, -1853, -1853,
   -1853, -1853, -1853, -1853,   640, -1853,    11,  4736,    11,    11,
    4736,  4736,  4736,  4736,  4736,  4736,  4736,  4736,   493,    11,
      11,    11,  4736,   493,    11,    11,    11,    11, -1853,    11,
      11,    11,    11,  4736,    11,    11,    11,    11,    11,    11,
    4736,  4736,    11,  4302,    11,    11,    11,    11,    11,    11,
   -1853,   678, -1853,   688,   691,    82,    63,   512,   532,   493,
     628,   692,   693,   694,   695,   698,   700,   701, -1853,   696,
     704,   706, -1853,   707,   709, -1853,   711,   713, -1853,  1010,
   -1853, -1853,  -211,  -187,  -211,  -187,   625,   627,   625,   627,
     625,   627,  -211,  -187,  -211,  -187,   625,   627,   625,   627,
     625,   627,     5, -1853,   219, -1853, -1853, -1853,   493, -1853,
     430, -1853,   714,   714, -1853, -1853, -1853, -1853, -1853, -1853,
   -1853, -1853, -1853,   705,   708, -1853, -1853, -1853, -1853, -1853,
     715,   716,   717, -1853, -1853, -1853,   718, -1853, -1853, -1853,
   -1853, -1853, -1853, -1853, -1853, -1853,   719, -1853, -1853, -1853,
   -1853,   721,   573,   580, -1853,   722,   724,   726,   727,   729,
      11,   732,   733,   573,   580, -1853,   734,   735,   737,   741,
     743,   747,   750,   752,   755,   757,   760,   762,   763,    11,
     764,   765,   702,   703,   766,   580, -1853,   767,   770,   771,
     772,   774,   775,   776,   777,   778,   779,   780,   783,   784,
     785,   786,   789,   790, -1853,   793,   796,   798,   799,   801,
     802,   804,   805,   807,   808,   809,   810,    11,   811,   812,
     813,   814,    11,   815,   816,   817,   819,   820,   821,   822,
     824,   829,   831,   832,   833,   838,   840,   842,   844,   845,
     846,  4736,   847,   848,   849,   850,   851,   852,   853, -1853,
    2392,    11,    11,   861,   862,   863, -1853,   864,   865,   866,
     867,   868,   869,   870, -1853, -1853, -1853,  1077,  1080,   493,
    1093,  1232,  1238,  1241,   208,  1242,  1243,   493,   493,  -271,
     985,  1268,  1269,   889, -1853,   573,   580,   219, -1853, -1853,
   -1853, -1853, -1853, -1853, -1853, -1853, -1853, -1853, -1853, -1853,
   -1853, -1853,   887, -1853, -1853, -1853, -1853, -1853, -1853, -1853,
   -1853, -1853, -1853, -1853, -1853, -1853, -1853, -1853,   890, -1853,
   -1853,   900,   902, -1853, -1853, -1853, -1853, -1853, -1853, -1853,
   -1853, -1853, -1853, -1853, -1853, -1853, -1853, -1853, -1853, -1853,
    4736, -1853, -1853, -1853, -1853, -1853, -1853, -1853, -1853, -1853,
   -1853, -1853, -1853,   887, -1853, -1853, -1853, -1853,   887, -1853,
   -1853, -1853, -1853, -1853, -1853, -1853, -1853, -1853, -1853, -1853,
   -1853, -1853, -1853, -1853, -1853, -1853, -1853,   887, -1853, -1853,
   -1853, -1853, -1853, -1853, -1853,  2955,   896,   895,   897,  -295,
    -216,  -213,  -200,  -165,   907,   901,   904,   905,  -204,  -107,
     -69,   909,   910,  -156,  -134,  -128,  -126,  -104,   916,   917,
     918,   919,   924,   925,   911,   935,   936,   937,   940,   942,
     944,   945,   946,   947,   948,   949,   950,   951,   952,   953,
     954,   955,   956,   957,   961,   962,   963,   964,   965,   966,
     967,   968,   977,   984,   994,   995,   996,   997,   999,  1002,
    1003,  1004,  1006,  1008,  1009,  1012,  1013,  1014,  1015,  1017,
    1019,  1023,  1025,  1026,  1028,  1029,  1030,  1031,  1032,  1034,
    1035,  1037,  1041,  1043,  1045,  1046,  1047,  1048,  1050,  1051,
    1052,  1056,  1057,  1058,  1059,  1060,  1061,   976,  1062,  1063,
    1064,  1065,  1066,  -219,  1067,  1001,  1068,  1069,  1070,  1071,
    1072,  1073,  1074,  1075,  1076,  1079,   -73,   -62,   -61,   -42,
    1081,  1082,  1083,   892,  1086,  1084,  1085,  1088,  1089,  1091,
    1092,  1097,  1098,  1100,  1104,  1105,  1106,  1107,  1108,  1110,
    1111,  1114, -1853, -1853, -1853,  1102,  1112,  1385,  1438,    80,
    1454,  1447,   256,  -240,    11,  1476,  1210, -1853,   -37,  1115,
    1077,  1119,  1120,  1121,  1122,   381,  1124,  1126,  1077,  1077,
    1125,   208,  1134,  1136,  1139, -1853,     5, -1853,    11,    11,
    1138,  1140,  1135,    11,    11,  4736, -1853, -1853,  1478,  1479,
      65,   354,    65,   354,    65,   354,    65,   354,    65,   354,
    1527,  1515,  1518,  1519,  1520,  1521,  1522,  1523,  1524,  1525,
    1526,  1528,    65,   354,    65,   354,    65,   354,    65,   354,
      65,   354,  1529,  1530,  1531,  1532,  1533,  1534,    11,    11,
      11,    11,  4736,    11,    11,    11,    11,    11,  3918,  4736,
    4736,    11,   493,   493,    11,    11,    11,    11,    11,    11,
      11,   493,  4736,    11,  1154,  1162,    11,  4736,  4736,  4736,
    4736,  4736,  4736,   493,  4736,  4736,  4736,   493,    11,    11,
      11,  4736,   493,    11,    11,    11,    11,    11,  4736,    11,
      11,    11,   493,    11,  4374,  1163,    11,    11,  4736,  4736,
    4736,  1164,  1166,    11,  4443,    11,    11,  4736,  4736,  4736,
      11,   354,    11,  1291,  1541,  1290,  1544,   493,    11,   493,
    1545,    11,    11,  1174,    65,   208,   493,   493,  1548,   208,
     493,    65,   354,    65,   354,    65,   354,    65,   354,  1176,
    4736,  4736,    11,    11,  4736,    11,  4736,  4736,  4736,  4736,
    4736,    11,    11,  4736,    11,  4736,    11,  4736,  4736,  4736,
    4736, -1853, -1853,  1183,  1184,  1185,  1077,  1186,  1187,  1188,
    1190,   208,  1191,   208,  1192,  1193,  1195,  1564,  1197,    35,
     -37, -1853, -1853, -1853, -1853, -1853, -1853,    53,   -37,   -37,
   -1853,   325,   -14, -1853, -1853, -1853, -1853,  1135,  1194,  4736,
    4736,  1199,   887,   887,  1135,  1198,   213, -1853, -1853,  -360,
    1204,   168, -1853, -1853,  1196,  1206,  1201,  1207,  1202,  1208,
    1203,  1214,  1209,  1215,  1213,  1216,  1217,  1221,  1220,  1224,
    1222,  1226,  1225,  1227,  1228,  1229,  1223,  1233,  1230,  1234,
    1231,  1235,  1236,  1237,  1246,  1239,  1240,  1244,  1247,  1248,
    1249,  1256,  1257,  1258,  1259,  1262,  1263,  1264,  1265,  1266,
    1267,  1270,  1271,  1272,  1273,  1274,  4736,    11,  1276,  1281,
    1282,  1283,  1284,  1285,  1286,  4736,  1288,  1289,  4736,  4736,
    1294,  1299,  1300,  1304,  1307,  1308,  1311,  4736,  1312,  1314,
    1315,    11,  1316,  1321,  1322,  1323,    11,  1324,  1325,  1326,
    1327,  1330,  1331,  1332,  1334,  1337,    11,  1338,  4736,  1342,
    4736,  1343,  1344,  1345,  1346,  1347,  4736,  4736,  1350,  4736,
    1351,  1353,  1354,  1356,  1357,  1358,  1359,  1360,  1361,  1362,
    1364, -1853,   -30,  1365,   219,  1349,   208,  1369,  1366,  1368,
    1585,  1375,   483,   208,    56,    76,  1376,   490,   208,  1377,
    1260,  1378,  1370,  1380,  1372,  1383,  1381,    11,  1384,  1386,
    1374,  1382,  1387,  1389,  1391,  1392,  1394,  1395,  1396,  1397,
    1398,  1399,  1401,  1402,  1403,  1407,  1408,  1409,  1412, -1853,
   -1853, -1853,   -37, -1853, -1853, -1853, -1853,   -14, -1853,   -14,
   -1853, -1853, -1853, -1853, -1853,   130,  1413,  1414,   143,  1415,
    1416,  1418,   208,  1419,  1421,  1425,  1422,  1423, -1853,    11,
      11,  1429,  1245,    66,   208,   712,  1604, -1853,   -37,  1251,
   -1853,  1253, -1853,  1275, -1853,  1439, -1853,  1440, -1853,  1441,
    1444,  1445, -1853,  1443, -1853,  1448, -1853,  1449,  1446,  1452,
   -1853,  1458, -1853,  1459, -1853,  1460, -1853,  1461, -1853,  1462,
    1463,  1464,  1465,  1466,  1467,  1468, -1853, -1853, -1853, -1853,
   -1853, -1853, -1853, -1853, -1853, -1853, -1853, -1853, -1853, -1853,
   -1853,   887,  1469, -1853, -1853, -1853, -1853, -1853, -1853, -1853,
    1470, -1853, -1853,  1471,  1472, -1853, -1853, -1853, -1853, -1853,
   -1853, -1853,  1473, -1853, -1853, -1853,   887, -1853, -1853, -1853,
   -1853,   887, -1853, -1853, -1853, -1853, -1853, -1853, -1853, -1853,
   -1853,   887, -1853,  1474, -1853,  1475, -1853, -1853, -1853, -1853,
   -1853,  1477,  1480, -1853,  1481, -1853, -1853, -1853, -1853, -1853,
   -1853, -1853,  1489, -1853, -1853, -1853, -1853,  1563, -1853,     5,
    1495,   -14,  1609, -1853, -1853,  1487, -1853, -1853,   -14, -1853,
   -1853,   493, -1853, -1853, -1853,   -14, -1853,  1496, -1853,  1499,
   -1853,  1501, -1853,  1502,  1492, -1853, -1853,  1504,  1506, -1853,
   -1853, -1853, -1853, -1853, -1853, -1853, -1853, -1853, -1853, -1853,
   -1853, -1853, -1853, -1853, -1853, -1853,  1500,  1503,  1505,  1510,
     207, -1853, -1853,  1516,   207, -1853, -1853, -1853,   325, -1853,
   -1853, -1853,    11,  4736,   887,   887, -1853,  1737,   152,  1517,
    -258,  1498,  1536,  1537,  1538,  1539, -1853, -1853,    65,    65,
      65,    65,    65,    98,   208,  1831, -1853, -1853, -1853,   123,
     208,    65,    65,    65,    65,    65,  1832,   208,  1833,   124,
     208,  1835,  4736,    11,    11,  4736,  4736,  4736,    11,    11,
      11,    11,  4736,  4736,  4736,    11,    11, -1853,  1543,   493,
    1549,  1542,    85, -1853,   208,  1550,    65,    65,    65,    65,
      11,  1497,  1513, -1853, -1853, -1853, -1853,   -37, -1853,   -37,
    1547,  1551,    11,    11,  1552,  1553,   207, -1853,  1555,  1511,
    1540,  1561,  1566,  1568,  1572,  1573,  1574,  1576,  1577,  1579,
    1836,   497,  1580,  1586,  1837,   511,  1587,  1588,  1589,  1591,
    1592,  1594,   519,  1595,  1596,  1838,   533,  1597,  1135,  1135,
    1135,  1593,  1598,  1135,   887,   887,  1135,  1599,  1600,  1601,
    1602,  1603,  1608, -1853,    77, -1853,  1610, -1853, -1853, -1853,
     573,   580, -1853,    46,   -14, -1853,  1617,  1620,  1621,  1622,
    1618,  4736,  4736,  1625,  1626,    11,    11,  1135,  1135, -1853,
   -1853,   -37,  1605,  1839,   208,  1840,  1874,   208, -1853, -1853,
   -1853, -1853, -1853, -1853,  1623, -1853, -1853, -1853,  1628, -1853,
   -1853, -1853, -1853, -1853, -1853, -1853, -1853, -1853, -1853,  1630,
   -1853, -1853,  1635,  1636,  1639,  1643,  4736,  1644,    11,    11,
    1645,    11,    11,  4736,  4736,    11, -1853, -1853, -1853, -1853,
     110,  1646, -1853, -1853, -1853, -1853, -1853,  1647,  1649,  1650,
   -1853, -1853,  1624,  1656,  1651,  1663,  1664,   147,  1665,   541,
    1666,  1667,   552,   208,   208,   208, -1853, -1853, -1853, -1853,
    1668, -1853,   887,   887, -1853,  1662,  1669,  1674,  1678,  1677,
   -1853, -1853, -1853,    11,  4736,  1670,  1671, -1853, -1853, -1853,
    1684,   304,    96, -1853, -1853, -1853, -1853, -1853,   287,   329,
     334,  4736,    11,    11,  1685,    11,  1686,  4736,  1687,  1688,
    1689, -1853, -1853, -1853,    37,   -56, -1853,  1690,  1691,  1694,
    1682,  1135,  1135, -1853,  1683, -1853,  1692, -1853,    11,    11,
    1695,  1696, -1853, -1853, -1853,  1697,  1699,  1700,  1701,  1707,
    1703,  1704, -1853, -1853, -1853, -1853, -1853, -1853, -1853,  1714,
    1716, -1853, -1853
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
   -1853, -1853, -1853,   166,  1485,   -60,  -155,  1011,  -293,  -490,
    -811,   475,  -593, -1195,  -221,  -471,  -479, -1852, -1208, -1355,
    -182,  -123, -1853,  1583, -1853,  1039,  -170, -1853, -1853,  1706,
   -1853, -1853, -1853,   637,   633,  1000,   725,  1923,  1989,  1990,
    1998,  2039, -1853,  1824,  1870, -1853,  2110,  2111, -1853, -1853,
    2112, -1853,   181,  2115, -1853, -1853, -1853, -1853, -1853, -1853,
   -1853,  2116, -1853, -1853,  1619,   134, -1853, -1853, -1853, -1853,
    2124, -1853, -1853, -1853,  2125, -1853, -1853,  2127, -1853, -1853,
    2129, -1853, -1853, -1853,  2151, -1853, -1853,  -150,  1900, -1853,
   -1853, -1853, -1853, -1853, -1853,    78, -1853,  -227,  1902, -1853,
   -1853, -1853,   -41, -1853,    50,  1885, -1853, -1853, -1853, -1853,
    2160, -1853, -1853, -1853, -1853, -1853, -1853, -1853, -1853, -1853,
   -1853, -1853, -1853, -1853, -1853, -1853, -1853, -1853
};

/* YYTABLE[YYPACT[STATE-NUM]].  What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule which
   number is the opposite.  If zero, do what YYDEFACT says.
   If YYTABLE_NINF, syntax error.  */
#define YYTABLE_NINF -722
static const yytype_int16 yytable[] =
{
     192,   752,  1575,  1422,   213,  1410,   753,   243,   797,   243,
     460,   759,   761,  1418,  1419,   190,   191,   308,   309,  1210,
     322,   719,   137,   316,   290,   214,   290,   443,   324,   326,
     -75,   222,   223,    87,  1217,   243,   243,  1964,   757,    71,
     243,   243,   222,   223,   222,   223,   837,  2039,   336,   103,
    1402,    63,   843,   844,   243,   846,   292,  1779,  -717,  -717,
    -717,   247,  -717,  -717,  -717,  -717,  -717,   243,   309,   140,
    1437,  -717,  -717,  -717,  -717,  -717,  -717,  1779,   757,  -717,
    -717,  -717,  -717,  -717,   461,   907,  1967,  1438,  1230,   315,
     783,   308,   309,   726,   727,   480,   490,  1779,   491,   143,
     493,    85,  1395,  1231,   903,   323,   325,  1968,   486,   487,
     488,  1967,   197,   198,  1969,   292,   308,   309,   144,   199,
    1929,   100,   334,   444,   312,   489,   486,   487,   488,   447,
    -324,   307,  1968,   775,   190,   191,   308,   309,   908,   243,
      70,  1218,   448,  1918,   307,  1933,  1944,   190,   191,   308,
     309,    71,  1819,   307,   766,   312,   190,   191,   308,   309,
     723,   479,   153,   724,  1347,  1823,   243,  1232,   857,  2090,
    1234,   728,   552,  1437,  1915,   469,   470,   492,   471,  1244,
      82,  1348,  1233,  1236,   314,  1235,   321,   137,  1245,   877,
    1438,   784,  1223,  1617,   882,  1619,   243,  1224,  1237,   474,
     475,  1612,   476,   307,   893,   894,   190,   191,   308,   309,
      96,   190,   191,   308,   309,   309,  1225,   333,  1238,   337,
     338,   339,   340,   341,   817,   308,   309,  1252,   821,  1197,
     823,   824,   137,  1239,   342,   828,   343,   344,   228,  2114,
     832,   833,  1253,   190,   191,   308,   309,   198,   331,  1254,
     464,   466,   101,   199,   848,  1256,   478,  1258,   484,   485,
     190,   191,   308,   309,  1255,   765,   190,   191,   308,   309,
    1257,   725,  1259,   104,   145,   395,  1246,   785,   155,  1260,
     224,   934,  -721,  -721,  -721,  1247,  -721,  -721,  -721,  -721,
    -721,   224,   810,   224,  1261,  -721,  -721,  -721,  -721,  -721,
    -721,   720,   721,  -721,  -721,  -721,  -721,  -721,   190,   191,
    1361,   309,    30,    31,  1248,   776,   777,   778,   779,   163,
     952,  1363,  1365,  1249,   162,  1362,  -717,  -717,  -717,  -717,
     767,   768,   769,   770,   771,   772,  1364,  1366,   760,   968,
     755,  1367,   168,  -717,  -717,  -717,  -717,   909,   910,   911,
     912,   913,    20,    21,    22,    23,  1368,  1217,  1771,   755,
    1766,   815,   815,  1442,   904,   905,   815,  1767,   815,   815,
    1785,  -717,  -717,   815,   156,   157,  1443,  1003,   815,   815,
    1632,   836,   755,   722,   798,   799,   800,   836,   836,   244,
     836,   244,   815,   884,   885,   310,  -660,   245,  -661,   754,
     170,   755,   215,   311,   140,   462,   246,   733,   735,   737,
     739,   741,   743,   745,   747,   749,   751,   244,  1625,   291,
     478,   442,   244,   244,  -668,   245,   762,   763,   764,  -664,
    -672,   758,   140,   755,   190,   191,  1628,   140,   140,   169,
    2040,   172,  2041,   345,  -717,   318,  1780,   867,   173,  1838,
    1781,   174,   872,   319,   281,   282,   814,   814,  1216,   914,
     312,   814,   320,   814,   814,   178,  1782,  2037,   814,   310,
    1781,   758,   891,   814,   814,   312,   835,   311,   906,  -101,
     179,  -101,   835,   835,   180,   835,  2116,   814,   917,   396,
    1781,   397,   398,  1930,   310,   399,   400,   401,   402,   403,
     404,   405,   311,   836,   406,   407,   408,   409,   410,   411,
     412,   413,   414,   415,   310,   416,   417,   187,  1934,  1945,
     188,   244,   311,   203,   836,   312,   208,   310,  -665,   836,
     209,   815,   815,   235,   907,   311,   310,   937,   312,   836,
     836,   212,   312,   216,   311,   217,  1399,   312,   244,   105,
    -721,  -721,  -721,  -721,   907,  -669,   236,   237,  -720,  -720,
    -720,   233,  -720,  -720,  -720,  -720,  -720,  -721,  -721,  -721,
    -721,  -720,  -720,  -720,  -720,  -720,  -720,   218,   244,  -720,
    -720,  -720,  -720,  -720,   710,  -673,   310,   908,    20,    21,
      22,   310,   318,   219,   311,  -721,  -721,   312,   835,   311,
     319,   220,   312,   310,   221,   938,  1643,   908,  1185,  1186,
     242,   311,   277,  1644,   286,  1645,    30,    31,   287,   835,
     289,  1639,  1640,   310,   835,  1907,   814,   814,  1646,  1909,
     787,   311,   707,   708,   835,   835,   781,   782,    43,  1445,
     310,  1447,   298,  1449,   300,  1451,   310,   302,   311,   303,
     304,   312,   305,   317,   311,   836,   328,   312,   369,  1465,
      84,  1467,   393,  1469,   394,  1471,   449,  1473,  -721,   453,
     418,   486,   487,   488,   836,   450,    20,    21,    22,    23,
      20,    21,    22,  2117,   451,   452,  1974,   454,   318,    20,
      21,    22,    23,   455,   936,   494,   319,   496,  1200,   312,
      43,   497,  2092,   498,  1404,   499,  1208,  1209,  1211,   486,
     487,   488,   836,   486,   487,   488,   500,   227,   486,   487,
     488,  1991,   159,   160,   161,  2118,   501,  1408,  1427,  1428,
    2119,    75,    76,  1432,   502,   503,   504,   788,   789,   790,
     791,   792,   793,   794,   795,  1426,   815,   815,   505,    86,
     835,  1431,   506,   507,   508,   509,   510,    92,    93,    94,
      95,  1571,    97,    98,    99,   486,   487,   488,  1579,   835,
    1581,  1415,  1583,   511,  1585,   512,   513,   514,  1481,   467,
     468,   469,   470,  1486,   471,  1841,  1842,  1843,   730,   515,
     516,  1844,  1845,   517,  1498,  1499,   909,   910,   911,   912,
     913,   518,   519,  1507,   486,   487,   488,   835,   533,   472,
     473,   474,   475,   731,   476,   495,   909,   910,   911,   912,
     913,   520,   521,   522,   523,    43,  -720,  -720,  -720,  -720,
     472,   473,   474,   475,   524,   476,   525,  1646,   526,   731,
     527,   814,   814,  -720,  -720,  -720,  -720,   528,   529,    43,
     530,   531,    30,    31,   532,   535,   711,   712,   713,   714,
     715,   716,   717,   536,  1205,    43,   534,   486,   487,   488,
     537,  -720,  -720,  1777,   486,   487,   488,   478,   538,   539,
    1784,   486,   487,   488,   540,  1593,   541,  2005,   312,   542,
    1872,  1599,  1600,   543,  1602,   486,   487,   488,   544,  1400,
     545,  2009,   702,   486,   487,   488,   546,   547,   915,  2016,
      43,   548,    43,   549,   550,  1878,   655,   486,   487,   488,
    1879,   551,   656,  2020,   678,   486,   487,   488,   916,   657,
    1880,  2094,   658,   700,   659,   703,   486,   487,   488,  1627,
      30,    31,  2097,   660,  -720,   661,   718,  1630,  1631,   662,
     663,  1633,   664,    30,    31,   665,  1634,   467,   468,   469,
     470,   666,   471,  1641,   472,   473,   474,   475,  1396,   476,
     667,  1401,  1403,   668,   669,   704,   670,   671,   672,   829,
     673,   674,  1488,  1489,   370,   371,   372,   373,   374,   375,
     376,   377,   378,   379,   380,   381,   382,   383,   384,   385,
     386,   387,   388,   389,   390,   391,   675,  1702,    77,    78,
      79,    80,    81,   676,   471,   677,   476,   679,   680,   681,
     918,   682,   683,  1912,  1913,   684,    88,    89,    90,    91,
     854,  1726,   933,  1541,  1542,   685,   686,   815,   687,   836,
    1548,   293,  1551,  1552,   688,   689,  1741,  1556,   690,  1558,
     691,   102,   692,   392,   693,  1565,   694,   695,  1568,   696,
     697,  1496,  1497,   836,   836,   698,   699,   709,   836,   729,
    1505,   901,   139,   899,   902,   919,   920,   921,   922,  1590,
    1591,   923,  1517,   924,   925,  1769,  1521,   927,   926,   928,
     929,  1526,   930,  1604,   931,   939,   932,  1794,   940,  1197,
     488,  1536,  1199,   971,   972,   941,   942,   943,   944,   945,
     346,   946,   947,   836,   948,  1201,   949,   950,   836,   951,
     815,   815,   953,   954,   955,   956,  1564,   957,  1566,   836,
     836,   958,   814,   959,   835,  1573,  1573,   960,   836,  1578,
     961,  1816,   962,  2028,  2029,   963,  1817,   964,  1818,  1834,
     965,  1421,   966,   967,   969,   970,   973,   974,   835,   835,
     975,   976,   977,   835,   978,   979,   980,   981,   982,   983,
     984,   815,   815,   985,   986,   987,   988,  1847,   815,   989,
     815,   815,  1626,   991,   990,   815,   992,   815,   993,   994,
    1629,   995,   996,   815,   997,   998,   815,   999,  1000,  1001,
    1002,  1004,  1005,  1006,  1007,  1009,  1010,  1011,   835,  1012,
    1013,  1014,  1015,   835,  1016,   814,   814,   815,   815,  1017,
     836,  1018,  1019,  1020,   835,   835,   836,   836,  1021,   836,
    1022,   815,  1023,   835,  1024,  1025,  1026,  1028,  1029,  1030,
    1031,  1032,  1033,  1034,  1187,  1188,  1189,  1190,  1191,  1192,
    1193,  1194,  1195,  1196,  1202,  1924,  1925,  1926,  1927,  1928,
    1203,  2102,  2103,  1204,  1206,  1207,   814,   814,  1936,  1937,
    1938,  1939,  1940,   814,  1212,   814,   814,  1213,  1214,  1215,
     814,  1217,   814,  1220,  1219,  1221,  1227,  1228,   814,  1229,
    1240,   814,  1372,  1241,  1268,  1572,  1242,  1243,  1888,  1577,
    1890,  1250,  1251,  1976,  1977,  1978,  1979,  1893,  1262,  1263,
    1264,  1265,   814,   814,  1895,   835,  1266,  1267,  1269,  1270,
    1271,   835,   835,  1272,   835,  1273,   814,  1274,  1275,  1276,
    1277,  1278,  1279,  1280,  1281,  1282,  1283,  1284,  1285,  1286,
    1287,  1421,   836,  1421,  1288,  1289,  1290,  1291,  1292,  1293,
    1294,  1295,   347,   348,   349,   350,   351,   352,   353,   354,
    1296,   355,   356,   357,   358,   359,   836,  1297,  1341,   360,
     361,   362,   363,   364,   365,   366,   367,  1298,  1299,  1300,
    1301,   836,  1302,  1949,  1950,  1303,  1304,  1305,  1954,  1306,
    1956,  1307,  1308,  1350,  1393,  1309,  1310,  1311,  1312,  1446,
    1313,  1448,  1314,  1450,  1820,  1452,  1315,  1824,  1316,  1317,
    1980,  1318,  1319,  1320,  1321,  1322,  1839,  1323,  1324,  1466,
    1325,  1468,  1987,  1470,  1326,  1472,  1327,  1474,  1328,  1329,
    1330,  1331,   836,  1332,  1333,  1334,  1983,   835,  1984,  1335,
    1336,  1337,  1338,  1339,  1340,  1342,  1343,  1344,  1345,  1346,
    1349,  1351,  1352,  1353,  1354,  1355,  1356,  1357,  1358,  1359,
    1394,   835,  1360,  1397,  1369,  1370,  1371,  1374,  1375,  1398,
     315,  1376,  1377,   315,  1378,  1379,   835,  2022,  2023,  2024,
    1380,  1381,  2027,  1382,   836,  2030,  1373,  1383,  1384,  1385,
    1386,  1387,  1391,  1388,  1389,  2052,  2053,  1390,  1405,  1406,
    1435,  1436,  1392,  2042,   478,   368,  1421,  1557,  1409,  1411,
    1412,  1413,  1414,  1421,  1416,  1420,  2054,  2055,  1421,  1417,
    2056,   807,   808,   809,  1423,   811,  1424,   835,  1580,  1425,
    1582,   755,  1584,  1429,  1586,  1430,  1453,  1454,  2072,  1910,
    1455,  1456,  1457,  1458,  1459,  1460,  1461,  1462,  1463,  1508,
    1464,  1475,  1476,  1477,  1478,  1479,  1480,  1509,  1540,  1546,
    1894,  1547,  1559,  1560,  1561,   314,  1563,  1567,   314,  1570,
    1576,  1587,  1828,  1609,  1610,  1611,  1613,  1614,  1615,   835,
    1616,  1618,  1620,  1621,  1840,  1622,  1623,  1624,  1957,  1638,
    1635,  1642,  1961,  1962,  1647,  1649,  1650,  1652,  1654,   480,
    1651,  1653,  1655,   480,  1656,  1658,  1659,  1775,  1657,  1660,
    1661,  1662,  2121,  1663,  1664,  1665,  1666,  1916,  1667,  1670,
    1668,  1669,  1671,  1672,  1674,  1676,  1846,  1678,  1837,  1673,
    1675,  1891,  1680,  1681,  1848,  1677,  1849,  1682,  2140,  2141,
    1683,  1684,  1685,  2130,  2131,  1679,  1686,  1687,  1688,  1689,
    2136,  2137,  1690,  1691,  1692,  1693,  1694,  1695,  1850,  1787,
    1696,  1697,  1698,  1699,  1700,   479,  1703,   855,   480,   479,
     858,  1704,  1705,  1706,  1707,  1708,  1709,   815,  1711,  1712,
     868,   869,   870,   315,  1715,   873,   874,   875,   876,  1716,
    1717,   878,   879,   880,  1718,   480,   883,  1719,  1720,   886,
     887,  1721,  1723,   890,  1724,  1725,  1727,   895,   896,   897,
     898,  1728,  1729,  1730,  1732,  1733,  1734,  1735,   836,   836,
    1736,  1737,  1738,   836,  1739,   836,   815,  1740,  1742,   313,
     815,   815,  1744,  1746,  1747,  1748,  1749,  1750,  2075,  2076,
    1753,  1755,  2079,  1756,  1757,   836,  1758,  1759,  1760,  1761,
    1770,  1763,  1764,  1762,  1765,  1768,  1773,   836,  1774,  1914,
     478,   479,   332,  1772,   478,  1776,  1783,  1786,  1788,  1789,
    1790,  1791,   814,  1792,  1795,  1797,  1796,  1799,   314,  1800,
    1793,  1801,  1802,  1798,  1803,  1804,  1805,  1806,  1807,  1808,
    2109,  1809,  1810,  1811,  1931,   463,   465,  1812,  1813,  1814,
    1935,   477,  1815,  1821,  1822,  1825,  1826,  1942,  1827,  1829,
    1946,  1830,  2124,   835,   835,  1831,  1832,  1833,   835,  1836,
     835,   814,  1851,  1852,  1853,   814,   814,  1854,  1855,  1859,
     836,   836,  1971,  1856,  1421,  1860,  2091,  1887,  1857,  1858,
     835,  1861,  1862,  1863,  1864,  1865,  1866,  1867,  1868,  1869,
    1870,  1871,   835,  1932,  1941,  1943,   478,  1947,  2004,  2008,
    2019,  2058,  2060,  1873,  1874,  1875,  1876,  1877,  1881,  1882,
     480,  1883,  1886,   836,  1884,  1885,   815,   815,  1889,  1896,
     815,  1892,  1897,  1008,  1898,  1899,  1900,  1901,   106,  1902,
    1903,  1919,  1981,  1904,  1993,  1905,  2061,  -718,  -718,  -718,
    1906,  -718,  -718,  -718,  -718,  -718,  1908,  1917,  1982,  2115,
    -718,  -718,  -718,  -718,  -718,  -718,  1648,  2080,  -718,  -718,
    -718,  -718,  -718,  1994,    44,   835,   835,   285,   815,  1920,
    1921,  1922,  1923,  1963,  2059,  1966,   479,  2062,  1035,  1965,
    1975,  1985,  1989,  1990,  1995,  1986,   419,   836,  1992,  1996,
     815,  1997,   732,   734,   736,   738,   740,   742,   744,   746,
     748,   750,  1998,  1999,  2000,   477,  2001,  2002,   835,  2003,
    2006,   814,   814,   836,   836,   814,  2007,  2010,  2011,  2012,
    1971,  2013,  2014,   152,  2015,  2017,  2018,  2021,  2057,  2025,
      45,    46,  2026,  2031,  2032,  2033,  2034,  2035,  2036,    47,
    2038,   813,   813,  2098,  2099,  2100,   813,  2043,   813,   813,
    2044,  2045,  2046,   813,  2047,  2050,  2051,  2063,   813,   813,
    2085,   332,  2064,   814,  2065,  2066,  2067,   332,   332,  2068,
     332,  1421,   813,  2069,  2071,  2074,  2081,  2082,   175,   176,
      48,  2087,   835,  2083,  2084,   814,   181,   182,   183,   184,
     185,   186,  2086,  2088,  2089,  2093,  2095,  2096,  2104,   299,
    2111,  2112,  2101,  2105,   204,   205,   206,   207,   835,   835,
    2106,   210,  2107,  2108,  2113,  2123,  2125,  2127,  2135,  2138,
    2132,  2133,  2128,  2129,  2134,  2142,  2143,  2144,  2139,  2145,
    2146,  2147,   806,   229,   230,   231,   232,  2148,   812,  2149,
    2150,   818,   819,   820,  2151,   822,  2152,   301,   825,   826,
     827,    49,    50,    52,   830,   831,    53,    54,   834,   774,
     838,   839,   840,   841,   842,    55,    56,   845,    57,   847,
      58,   849,   850,   851,   852,   853,   420,   421,   422,   423,
     424,   425,   426,   332,   427,   428,   429,   430,   431,   432,
     433,   434,    59,   435,   436,   437,   438,   439,   327,   440,
     330,    65,     0,     0,   332,  -718,  -718,  -718,  -718,   332,
       0,   813,   813,     0,     0,     0,     0,     0,     0,   332,
     332,     0,  -718,  -718,  -718,  -718,     0,     0,   446,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
    -718,  -718,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,  1433,     0,     0,     0,   935,
     856,     0,     0,   859,   860,   861,   862,   863,   864,   865,
     866,     0,     0,     0,     0,   871,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   881,     0,     0,     0,
       0,     0,     0,   888,   889,     0,   892,     0,     0,     0,
    1482,  1483,  1484,  -718,     0,  1487,     0,     0,  1490,  1491,
       0,     0,  1495,     0,     0,   332,     0,  1500,  1501,  1502,
    1503,  1504,     0,     0,     0,     0,     0,  1510,     0,     0,
       0,     0,     0,     0,   332,     0,     0,     0,     0,  1522,
    1523,  1524,     0,     0,  1527,  1528,  1529,  1530,  1531,     0,
    1533,  1534,  1535,     0,  1537,     0,     0,     0,     0,     0,
       0,   441,     0,     0,     0,   701,     0,     0,     0,     0,
       0,     0,   332,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,  1569,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   813,   813,     0,     0,
       0,     0,   108,  1036,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,  1037,     0,     0,     0,     0,
       0,     0,   477,     0,     0,     0,  1038,   109,   110,   111,
     112,   113,   114,   115,   116,   117,   118,   119,   120,   121,
     122,   123,   124,   125,   126,   127,   128,   129,   130,   131,
     132,   133,   134,   135,     0,     0,     0,     0,     0,  1039,
    1040,  1041,  1042,  1043,  1044,  1045,  1046,  1047,  1048,  1049,
    1050,  1051,  1052,     0,  1027,     0,     0,     0,     0,  1053,
    1054,  1055,  1056,  1057,     0,     0,     0,  1058,     0,  1059,
    1060,  1061,  1062,  1063,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   107,
       0,     0,     0,     0,     0,     0,     0,     0,  -719,  -719,
    -719,     0,  -719,  -719,  -719,  -719,  -719,  1731,     0,     0,
       0,  -719,  -719,  -719,  -719,  -719,  -719,     0,     0,  -719,
    -719,  -719,  -719,  -719,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,  1222,     0,     0,     0,     0,     0,     0,
       0,     0,     0,  1064,  1065,  1066,  1067,  1068,  1069,  1070,
       0,  1071,  1072,  1073,  1074,  1075,  1076,  1077,  1078,  1079,
    1080,  1081,  1082,  1083,  1084,     0,  1085,  1086,  1087,  1088,
    1089,  1090,  1091,  1092,  1093,  1094,  1095,  1096,  1097,  1098,
    1099,  1100,  1101,  1102,  1103,  1104,  1105,  1106,  1107,  1108,
    1109,  1110,     0,  1111,  1112,  1113,  1114,  1115,  1116,  1117,
    1118,  1119,  1120,  1121,  1122,  1123,  1124,  1125,  1126,  1127,
    1128,  1835,  1129,  1130,  1131,  1132,  1133,  1134,  1135,  1136,
    1137,  1138,  1139,     0,  1140,  1141,     0,  1142,  1143,  1144,
       0,     0,     0,     0,     0,     0,     0,   813,     0,   332,
       0,     0,     0,  1145,  1146,  1147,     0,     0,     0,  1148,
       0,  1149,  1150,  1151,  1152,  1153,  1154,     0,     0,     0,
       0,     0,     0,   332,   332,     0,     0,     0,   332,     0,
       0,     0,  1155,  1156,  1157,     0,     0,  1158,  1159,  1160,
    1161,  1162,  1163,  1164,  1165,  1166,  1167,  1168,  1169,  1170,
    1171,  1172,  1173,  1174,  1175,  1176,  1177,  1178,  1179,  1180,
    1181,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   332,     0,     0,     0,     0,   332,     0,
     813,   813,     0,     0,     0,     0,     0,     0,     0,   332,
     332,     0,     0,     0,     0,     0,     0,     0,   332,     0,
       0,     0,     0,     0,     0,     0,     0,     0,  1182,     0,
       0,     0,     0,     0,     0,     0,  -719,  -719,  -719,  -719,
       0,     0,     0,     0,     0,     0,     0,     0,  1434,     0,
       0,   813,   813,  -719,  -719,  -719,  -719,     0,   813,     0,
     813,   813,     0,     0,     0,   813,     0,   813,     0,     0,
       0,     0,     0,   813,     0,     0,   813,     0,     0,     0,
       0,  -719,  -719,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,  1485,     0,   813,   813,     0,
     332,  1492,  1493,  1494,     0,     0,   332,   332,     0,   332,
       0,   813,     0,     0,     0,  1506,     0,     0,     0,     0,
    1511,  1512,  1513,  1514,  1515,  1516,     0,  1518,  1519,  1520,
    1955,     0,     0,     0,  1525,     0,     0,     0,     0,     0,
       0,  1532,     0,     0,     0,     0,     0,  1539,     0,     0,
       0,  1543,  1544,  1545,  -719,     0,     0,  1550,     0,     0,
    1553,  1554,  1555,     0,  1988,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,  1588,  1589,     0,  1036,  1592,     0,  1594,
    1595,  1596,  1597,  1598,     0,     0,  1601,     0,  1603,     0,
    1605,  1606,  1607,  1608,     0,     0,     0,     0,  1037,     0,
       0,     0,   332,     0,     0,     0,     0,     0,     0,  1038,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   332,     0,     0,     0,
       0,     0,  1636,  1637,     0,     0,     0,     0,     0,     0,
       0,   332,  1039,  1040,  1041,  1042,  1043,  1044,  1045,  1046,
    1047,  1048,  1049,  1050,  1051,  1052,     0,     0,     0,     0,
    2073,     0,  1053,  1054,  1055,  1056,  1057,     0,     0,   477,
    1058,     0,  1059,  1060,  1061,  1062,  1063,     0,     0,     0,
       5,     6,     7,     0,     8,     9,    10,    11,    12,     0,
       0,     0,   332,    14,    15,    16,    17,    18,    19,  1701,
       0,    20,    21,    22,    23,    24,     0,     0,  1710,     0,
       0,  1713,  1714,     0,     0,     0,     0,     0,     0,     0,
    1722,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     313,     0,     0,   313,  2122,     0,     0,     0,     0,     0,
       0,  1743,     0,  1745,   332,     0,     0,     0,     0,  1751,
    1752,     0,  1754,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,  1064,  1065,  1066,  1067,
    1068,  1069,  1070,     0,  1071,  1072,  1073,  1074,  1075,  1076,
    1077,  1078,  1079,  1080,  1081,  1082,  1083,  1084,     0,  1085,
    1086,  1087,  1088,  1089,  1090,  1091,  1092,  1093,  1094,  1095,
    1096,  1097,  1098,  1099,  1100,  1101,  1102,  1103,  1104,  1105,
    1106,  1107,  1108,  1109,  1110,     0,  1111,  1112,  1113,  1114,
    1115,  1116,  1117,  1118,  1119,  1120,  1121,  1122,  1123,  1124,
    1125,  1126,  1127,  1128,     0,  1129,  1130,  1131,  1132,  1133,
    1134,  1135,  1136,  1137,  1138,  1139,     0,  1140,  1141,     0,
    1142,  1143,  1144,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,  1145,  1146,  1147,     0,
       0,     0,  1148,     0,  1149,  1150,  1151,  1152,  1153,  1154,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,  1155,  1156,  1157,     0,     0,
    1158,  1159,  1160,  1161,  1162,  1163,  1164,  1165,  1166,  1167,
    1168,  1169,  1170,  1171,  1172,  1173,  1174,  1175,  1176,  1177,
    1178,  1179,  1180,  1181,     0,   477,     0,     0,     0,   477,
       0,     0,     0,     0,     0,     0,     0,   813,     0,     0,
       0,     0,     0,   313,     0,     0,     0,     0,    26,    27,
      28,    29,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    30,    31,    32,    33,     0,
       0,  1226,     0,     0,     0,     0,     0,     0,   332,   332,
       0,     0,     0,   332,     0,   332,   813,     0,     0,     0,
     813,   813,     0,    38,    39,     0,     0,  1970,     0,     0,
       0,     0,     0,     0,     0,   332,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   332,     0,     0,
       0,   477,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     2,     3,  1911,     4,     0,     0,
       0,     0,     0,     0,     5,     6,     7,     0,     8,     9,
      10,    11,    12,    13,     0,     0,     0,    14,    15,    16,
      17,    18,    19,     0,     0,    20,    21,    22,    23,    24,
       0,     0,     0,     0,     0,  1948,     0,     0,  1951,  1952,
    1953,     0,     0,     0,     0,  1958,  1959,  1960,     0,     0,
     332,   332,   -14,   103,     0,   -14,     0,    25,     0,     0,
       0,     0,   -14,   -14,   -14,     0,   -14,   -14,   -14,   -14,
     -14,   -14,     0,     0,     0,   -14,   -14,   -14,   -14,   -14,
     -14,     0,     0,   -14,   -14,   -14,   -14,   -14,     0,     0,
       0,     0,     0,   332,     0,     0,   813,   813,     0,     0,
     813,     0,     0,     0,     0,  1970,     0,     0,   -18,   104,
       0,   -18,     0,     0,     0,   -14,     0,     0,   -18,   -18,
     -18,     0,   -18,   -18,   -18,   -18,   -18,   -18,     0,     0,
       0,   -18,   -18,   -18,   -18,   -18,   -18,     0,     0,   -18,
     -18,   -18,   -18,   -18,  2048,  2049,     0,     0,   813,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   332,     0,     0,
     813,   -18,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,  2070,
       0,     0,     0,   332,   332,     0,  2077,  2078,   -17,   105,
       0,   -17,     0,     0,     0,     0,     0,     0,   -17,   -17,
     -17,     0,   -17,   -17,   -17,   -17,   -17,   -17,     0,     0,
       0,   -17,   -17,   -17,   -17,   -17,   -17,     0,     0,   -17,
     -17,   -17,   -17,   -17,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,  2110,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   -17,     0,     0,  2120,     0,     0,     0,     0,     0,
    2126,     0,    26,    27,    28,    29,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    30,
      31,    32,    33,     0,     0,     0,     0,    34,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    35,     0,     0,
      36,    37,     0,     0,     0,     0,     0,    38,    39,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     -14,   -14,   -14,   -14,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   -14,   -14,   -14,
     -14,     0,     0,     0,     0,   -14,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   -14,     0,     0,   -14,   -14,
       0,     0,     0,     0,     0,   -14,   -14,     0,     0,     0,
       0,     0,     0,     0,    40,     0,   -18,   -18,   -18,   -18,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   -18,   -18,   -18,   -18,     0,     0,     0,
       0,   -18,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   -18,     0,     0,   -18,   -18,     0,     0,     0,     0,
       0,   -18,   -18,     0,   -15,   106,     0,   -15,     0,     0,
       0,     0,   -14,     0,   -15,   -15,   -15,     0,   -15,   -15,
     -15,   -15,   -15,   -15,     0,     0,     0,   -15,   -15,   -15,
     -15,   -15,   -15,     0,     0,   -15,   -15,   -15,   -15,   -15,
       0,     0,     0,     0,     0,     0,   -17,   -17,   -17,   -17,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   -17,   -17,   -17,   -17,   -15,   -18,     0,
       0,   -17,   190,   191,   308,   309,     0,     0,     0,     0,
       0,   -17,     0,     0,   -17,   -17,   -16,   107,     0,   -16,
       0,   -17,   -17,     0,     0,     0,   -16,   -16,   -16,     0,
     -16,   -16,   -16,   -16,   -16,   -16,     0,     0,     0,   -16,
     -16,   -16,   -16,   -16,   -16,     0,     0,   -16,   -16,   -16,
     -16,   -16,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   -16,
       0,     0,     0,     0,     0,     0,     0,     0,   -17,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   553,   554,   555,   556,   557,   558,   559,   560,   561,
     562,   563,   564,   565,   566,   567,   568,   569,   570,   571,
     572,   573,   574,   575,   576,   577,   578,   579,   580,   581,
     582,   583,   584,   585,   586,   587,   588,   589,   590,   591,
     592,   593,   594,   595,   596,   597,   598,   599,   600,   601,
     602,   603,   604,   605,   606,   607,   608,   609,   610,   611,
     612,   613,   614,   615,   616,   617,   618,   619,   620,   621,
       5,     6,     7,     0,     8,     9,    10,    11,    12,     0,
       0,     0,     0,    14,    15,    16,    17,    18,    19,     0,
       0,    20,    21,    22,    23,    24,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   -15,   -15,   -15,   -15,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   -15,
     -15,   -15,   -15,     0,     0,     0,     0,   -15,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   -15,     0,     0,
     -15,   -15,     0,     0,     0,     0,     0,   -15,   -15,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   -16,   -16,   -16,   -16,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   -16,   -16,   -16,   -16,     0,     0,     0,     0,   -16,
       0,     0,     0,     0,   -15,     0,     0,     0,     0,   -16,
       0,     0,   -16,   -16,     0,     0,     0,     0,     0,   -16,
     -16,     0,     0,     0,     0,     0,     0,     0,     0,   622,
     623,   624,   625,   626,   627,   628,   629,   630,   631,   632,
     633,   634,   635,   636,   637,   638,   639,   640,   641,   642,
     643,   644,   645,   646,   647,   648,   649,   650,   651,   652,
     653,     0,   310,     0,     0,     0,     0,     0,     0,     0,
     311,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   -16,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    26,    27,
      28,    29,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    30,    31,    32,    33,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    38,    39,   553,   554,   555,   556,   557,
     558,   559,   560,   561,   562,   563,   564,   565,   566,   567,
     568,   569,   570,   571,   572,   573,   574,   575,   576,   577,
     578,   579,   580,   581,   582,   583,   584,   585,   586,   587,
     588,   589,   590,   591,   592,   593,   594,   595,   596,   597,
     598,   599,   600,   601,   602,   603,   604,   605,   606,   607,
     608,   609,   610,   611,   612,   613,   614,   615,   616,   617,
     618,   619,   620,   621,     0,     0,   329,   553,   554,   555,
     556,   557,   558,   559,   560,   561,   562,   563,   564,   565,
     566,   567,   568,   569,   570,   571,   572,   573,   574,   575,
     576,   577,   578,   579,   580,   581,   582,   583,   584,   585,
     586,   587,   588,   589,   590,   591,   592,   593,   594,   595,
     596,   597,   598,   599,   600,   601,   602,   603,   604,   605,
     606,   607,   608,   609,   610,   611,   612,   613,   614,   615,
     616,   617,   618,   619,   620,   621,   553,   554,   555,   556,
     557,   558,   559,   560,   561,   562,   563,   564,   565,   566,
     567,   568,   569,   570,   571,   572,   573,   574,   575,   576,
     577,   578,   579,   580,   581,   582,   583,   584,   585,   586,
     587,   588,   589,   590,   591,   592,   593,   594,   595,   596,
     597,   598,   599,   600,   601,   602,   603,   604,   605,   606,
     607,   608,   609,   610,   611,   612,   613,   614,   615,   616,
     617,   618,   619,   620,   621,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   622,   623,   624,   625,   626,   627,   628,
     629,   630,   631,   632,   633,   634,   635,   636,   637,   638,
     639,   640,   641,   642,   643,   644,   645,   646,   647,   648,
     649,   650,   651,   652,   653,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   312,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   622,   623,   624,   625,   626,
     627,   628,   629,   630,   631,   632,   633,   634,   635,   636,
     637,   638,   639,   640,   641,   642,   643,   644,   645,   646,
     647,   648,   649,   650,   651,   652,   653,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,  1538,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   622,   623,   624,   625,   626,   627,
     628,   629,   630,   631,   632,   633,   634,   635,   636,   637,
     638,   639,   640,   641,   642,   643,   644,   645,   646,   647,
     648,   649,   650,   651,   652,   653,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,  1549,   553,
     554,   555,   556,   557,   558,   559,   560,   561,   562,   563,
     564,   565,   566,   567,   568,   569,   570,   571,   572,   573,
     574,   575,   576,   577,   578,   579,   580,   581,   582,   583,
     584,   585,   586,   587,   588,   589,   590,   591,   592,   593,
     594,   595,   596,   597,   598,   599,   600,   601,   602,   603,
     604,   605,   606,   607,   608,   609,   610,   611,   612,   613,
     614,   615,   616,   617,   618,   619,   620,   621,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   622,   623,   624,
     625,   626,   627,   628,   629,   630,   631,   632,   633,   634,
     635,   636,   637,   638,   639,   640,   641,   642,   643,   644,
     645,   646,   647,   648,   649,   650,   651,   652,   653
};

static const yytype_int16 yycheck[] =
{
     155,   480,  1357,  1211,     1,  1200,     1,     1,     1,     1,
       1,   482,   483,  1208,  1209,     4,     5,     6,     7,   290,
     247,    34,    63,   244,     1,    22,     1,   294,   255,   256,
     390,    19,    20,    64,   394,     1,     1,  1889,     1,    22,
       1,     1,    19,    20,    19,    20,   536,     1,     1,     1,
     290,     1,   542,   543,     1,   545,   226,     1,    10,    11,
      12,   211,    14,    15,    16,    17,    18,     1,     7,   398,
       5,    23,    24,    25,    26,    27,    28,     1,     1,    31,
      32,    33,    34,    35,    75,    22,     1,    22,   383,   244,
       1,     6,     7,   318,   319,   316,   323,     1,   325,   395,
     327,    23,    22,   398,    22,   255,   256,    22,   384,   385,
     386,     1,    29,    30,    29,   285,     6,     7,   395,    36,
      22,    43,   277,   390,   395,   401,   384,   385,   386,     9,
      47,     1,    22,     1,     4,     5,     6,     7,    75,     1,
      22,   952,    22,   401,     1,    22,    22,     4,     5,     6,
       7,    22,    22,     1,     1,   395,     4,     5,     6,     7,
     285,   316,    84,   288,   383,    22,     1,   383,   658,    22,
     383,   396,   393,     5,    22,   386,   387,   327,   389,   383,
      22,   400,   398,   383,   244,   398,   246,   228,   392,   679,
      22,   102,  1003,  1401,   684,  1403,     1,  1008,   398,   386,
     387,  1396,   389,     1,   694,   695,     4,     5,     6,     7,
      22,     4,     5,     6,     7,     7,  1027,   277,   383,   172,
     173,   174,   175,   176,   517,     6,     7,   383,   521,    22,
     523,   524,   273,   398,   187,   528,   189,   190,   188,  2091,
     533,   534,   398,     4,     5,     6,     7,    30,     9,   383,
     310,   311,    22,    36,   547,   383,   316,   383,   318,   319,
       4,     5,     6,     7,   398,   492,     4,     5,     6,     7,
     398,   396,   398,     1,   395,     1,   383,   188,   391,   383,
     268,   752,    10,    11,    12,   392,    14,    15,    16,    17,
      18,   268,   513,   268,   398,    23,    24,    25,    26,    27,
      28,   314,   315,    31,    32,    33,    34,    35,     4,     5,
     383,     7,   295,   296,   383,   183,   184,   185,   186,   395,
     810,   383,   383,   392,   392,   398,   278,   279,   280,   281,
     177,   178,   179,   180,   181,   182,   398,   398,   394,   829,
     396,   383,   395,   295,   296,   297,   298,   284,   285,   286,
     287,   288,    31,    32,    33,    34,   398,   394,  1566,   396,
     390,   516,   517,     9,   282,   283,   521,   397,   523,   524,
    1578,   323,   324,   528,   295,   296,    22,   867,   533,   534,
     394,   536,   396,   396,   377,   378,   379,   542,   543,   383,
     545,   383,   547,   686,   687,   384,   390,   391,   390,   394,
     390,   396,   399,   392,   398,   396,   400,   467,   468,   469,
     470,   471,   472,   473,   474,   475,   476,   383,   383,   396,
     480,   396,   383,   383,   390,   391,   486,   487,   488,   390,
     390,   394,   398,   396,     4,     5,   383,   398,   398,   395,
     394,   390,   396,   396,   396,   384,   390,   668,   390,   383,
     394,    22,   673,   392,   228,   229,   516,   517,   937,   396,
     395,   521,   401,   523,   524,    21,   390,   390,   528,   384,
     394,   394,   693,   533,   534,   395,   536,   392,   396,   394,
      21,   396,   542,   543,    21,   545,   390,   547,   709,   215,
     394,   217,   218,   395,   384,   221,   222,   223,   224,   225,
     226,   227,   392,   658,   230,   231,   232,   233,   234,   235,
     236,   237,   238,   239,   384,   241,   242,   395,   395,   395,
     395,   383,   392,    21,   679,   395,    21,   384,   390,   684,
      21,   686,   687,    47,    22,   392,   384,   758,   395,   694,
     695,   395,   395,   395,   392,   395,   290,   395,   383,     1,
     278,   279,   280,   281,    22,   390,   393,   394,    10,    11,
      12,   386,    14,    15,    16,    17,    18,   295,   296,   297,
     298,    23,    24,    25,    26,    27,    28,   395,   383,    31,
      32,    33,    34,    35,    34,   390,   384,    75,    31,    32,
      33,   384,   384,   395,   392,   323,   324,   395,   658,   392,
     392,   395,   395,   384,   395,   760,   393,    75,   901,   902,
     395,   392,   383,   400,    22,   402,   295,   296,    22,   679,
     392,  1432,  1433,   384,   684,  1820,   686,   687,  1439,  1824,
       1,   392,   451,   452,   694,   695,   502,   503,     1,  1232,
     384,  1234,    22,  1236,   395,  1238,   384,   395,   392,   395,
     395,   395,   395,    22,   392,   810,   390,   395,     1,  1252,
      23,  1254,   383,  1256,   383,  1258,   395,  1260,   396,    22,
     396,   384,   385,   386,   829,   395,    31,    32,    33,    34,
      31,    32,    33,   396,   395,   395,  1894,    21,   384,    31,
      32,    33,    34,   305,   754,   390,   392,   390,   919,   395,
      63,   390,  2057,   390,  1194,   383,   927,   928,   929,   384,
     385,   386,   867,   384,   385,   386,   383,   396,   384,   385,
     386,  1916,    89,    90,    91,   396,   383,  1198,  1218,  1219,
     396,     6,     7,  1223,   383,   383,   383,   108,   109,   110,
     111,   112,   113,   114,   115,  1216,   901,   902,   383,    24,
     810,  1222,   383,   390,   383,   383,   383,    32,    33,    34,
      35,  1354,    37,    38,    39,   384,   385,   386,  1361,   829,
    1363,   390,  1365,   383,  1367,   383,   383,   383,  1268,   384,
     385,   386,   387,  1273,   389,    73,    74,    75,   393,   383,
     383,    79,    80,   383,  1284,  1285,   284,   285,   286,   287,
     288,   383,   383,  1293,   384,   385,   386,   867,   400,   384,
     385,   386,   387,   393,   389,   390,   284,   285,   286,   287,
     288,   383,   383,   383,   383,   188,   278,   279,   280,   281,
     384,   385,   386,   387,   383,   389,   383,  1648,   383,   393,
     383,   901,   902,   295,   296,   297,   298,   383,   390,   212,
     383,   383,   295,   296,   383,   383,   306,   307,   308,   309,
     310,   311,   312,   383,   924,   228,   400,   384,   385,   386,
     383,   323,   324,   390,   384,   385,   386,   937,   383,   383,
     390,   384,   385,   386,   383,  1375,   383,   390,   395,   383,
    1701,  1381,  1382,   383,  1384,   384,   385,   386,   383,  1192,
     383,   390,   395,   384,   385,   386,   383,   383,   396,   390,
     273,   383,   275,   383,   383,  1726,   390,   384,   385,   386,
    1731,   383,   383,   390,   390,   384,   385,   386,   396,   383,
    1741,   390,   383,   390,   383,   393,   384,   385,   386,  1410,
     295,   296,   390,   383,   396,   383,   396,  1418,  1419,   383,
     383,  1422,   383,   295,   296,   383,  1427,   384,   385,   386,
     387,   383,   389,  1434,   384,   385,   386,   387,  1189,   389,
     383,  1192,  1193,   383,   383,   393,   383,   383,   383,   395,
     383,   383,  1275,  1276,   327,   328,   329,   330,   331,   332,
     333,   334,   335,   336,   337,   338,   339,   340,   341,   342,
     343,   344,   345,   346,   347,   348,   383,  1497,     8,     9,
      10,    11,    12,   383,   389,   383,   389,   383,   383,   383,
     392,   383,   383,  1834,  1835,   383,    26,    27,    28,    29,
     390,  1521,    22,  1326,  1327,   383,   383,  1192,   383,  1194,
    1333,   396,  1335,  1336,   383,   383,  1536,  1340,   383,  1342,
     383,    51,   383,   396,   383,  1348,   383,   383,  1351,   383,
     383,  1282,  1283,  1218,  1219,   383,   383,   383,  1223,   383,
    1291,   383,    72,   395,   383,   383,   383,   383,   383,  1372,
    1373,   383,  1303,   383,   383,  1564,  1307,   383,   392,   383,
     383,  1312,   383,  1386,   383,   390,   383,  1587,   390,    22,
     386,  1322,    22,   401,   401,   390,   390,   390,   390,   390,
       1,   390,   390,  1268,   390,    22,   390,   390,  1273,   390,
    1275,  1276,   390,   390,   390,   390,  1347,   390,  1349,  1284,
    1285,   390,  1192,   390,  1194,  1356,  1357,   390,  1293,  1360,
     390,  1612,   390,  1954,  1955,   390,  1617,   390,  1619,  1639,
     390,  1211,   390,   390,   390,   390,   390,   390,  1218,  1219,
     390,   390,   390,  1223,   390,   390,   390,   390,   390,   390,
     390,  1326,  1327,   390,   390,   390,   390,  1648,  1333,   390,
    1335,  1336,  1409,   390,   394,  1340,   390,  1342,   390,   390,
    1417,   390,   390,  1348,   390,   390,  1351,   390,   390,   390,
     390,   390,   390,   390,   390,   390,   390,   390,  1268,   390,
     390,   390,   390,  1273,   390,  1275,  1276,  1372,  1373,   390,
    1375,   390,   390,   390,  1284,  1285,  1381,  1382,   390,  1384,
     390,  1386,   390,  1293,   390,   390,   390,   390,   390,   390,
     390,   390,   390,   390,   383,   383,   383,   383,   383,   383,
     383,   383,   383,   383,    22,  1848,  1849,  1850,  1851,  1852,
      22,  2072,  2073,    22,    22,    22,  1326,  1327,  1861,  1862,
    1863,  1864,  1865,  1333,   289,  1335,  1336,     9,     9,   390,
    1340,   394,  1342,   383,   394,   383,   390,   392,  1348,   392,
     383,  1351,   400,   392,   383,  1355,   392,   392,  1769,  1359,
    1771,   392,   392,  1896,  1897,  1898,  1899,  1778,   392,   392,
     392,   392,  1372,  1373,  1785,  1375,   392,   392,   383,   383,
     383,  1381,  1382,   383,  1384,   383,  1386,   383,   383,   383,
     383,   383,   383,   383,   383,   383,   383,   383,   383,   383,
     383,  1401,  1497,  1403,   383,   383,   383,   383,   383,   383,
     383,   383,   243,   244,   245,   246,   247,   248,   249,   250,
     383,   252,   253,   254,   255,   256,  1521,   383,   392,   260,
     261,   262,   263,   264,   265,   266,   267,   383,   383,   383,
     383,  1536,   383,  1873,  1874,   383,   383,   383,  1878,   383,
    1880,   383,   383,   392,     9,   383,   383,   383,   383,  1233,
     383,  1235,   383,  1237,  1625,  1239,   383,  1628,   383,   383,
    1900,   383,   383,   383,   383,   383,  1643,   383,   383,  1253,
     383,  1255,  1912,  1257,   383,  1259,   383,  1261,   383,   383,
     383,   383,  1587,   383,   383,   383,  1907,  1497,  1909,   383,
     383,   383,   383,   383,   383,   383,   383,   383,   383,   383,
     383,   383,   383,   383,   383,   383,   383,   383,   383,   383,
      22,  1521,   383,     9,   383,   383,   383,   383,   383,    22,
    1625,   383,   383,  1628,   383,   383,  1536,  1948,  1949,  1950,
     383,   383,  1953,   383,  1639,  1956,   400,   383,   383,   383,
     383,   383,   390,   383,   383,  1985,  1986,   383,    22,   289,
      22,    22,   390,  1974,  1564,   396,  1566,  1341,   393,   390,
     390,   390,   390,  1573,   390,   390,  1987,  1988,  1578,   393,
    1991,   510,   511,   512,   390,   514,   390,  1587,  1362,   390,
    1364,   396,  1366,   395,  1368,   395,     9,    22,  2028,  1832,
      22,    22,    22,    22,    22,    22,    22,    22,    22,   395,
      22,    22,    22,    22,    22,    22,    22,   395,   395,   395,
    1781,   395,   271,    22,   274,  1625,    22,    22,  1628,   395,
      22,   395,  1632,   390,   390,   390,   390,   390,   390,  1639,
     390,   390,   390,   390,  1644,   390,    22,   390,  1881,   390,
     396,   393,  1885,  1886,   390,   399,   390,   390,   390,  1820,
     399,   399,   399,  1824,   390,   390,   393,    22,   399,   393,
     393,   390,  2102,   393,   390,   393,   390,  1838,   393,   390,
     393,   393,   399,   390,   390,   390,    22,   390,   383,   399,
     399,    22,   393,   393,   383,   399,   383,   393,  2128,  2129,
     393,   393,   393,  2114,  2115,   399,   390,   390,   390,   390,
    2121,  2122,   390,   390,   390,   390,   390,   390,   383,   399,
     390,   390,   390,   390,   390,  1820,   390,   656,  1889,  1824,
     659,   390,   390,   390,   390,   390,   390,  1832,   390,   390,
     669,   670,   671,  1838,   390,   674,   675,   676,   677,   390,
     390,   680,   681,   682,   390,  1916,   685,   390,   390,   688,
     689,   390,   390,   692,   390,   390,   390,   696,   697,   698,
     699,   390,   390,   390,   390,   390,   390,   390,  1873,  1874,
     390,   390,   390,  1878,   390,  1880,  1881,   390,   390,   244,
    1885,  1886,   390,   390,   390,   390,   390,   390,  2031,  2032,
     390,   390,  2035,   390,   390,  1900,   390,   390,   390,   390,
     401,   390,   390,   393,   390,   390,   390,  1912,   390,    22,
    1820,  1916,   277,   394,  1824,   390,   390,   390,   390,   399,
     390,   399,  1832,   390,   390,   401,   390,   390,  1838,   390,
     399,   390,   390,   401,   390,   390,   390,   390,   390,   390,
    2083,   390,   390,   390,  1854,   310,   311,   390,   390,   390,
    1860,   316,   390,   390,   390,   390,   390,  1867,   390,   390,
    1870,   390,  2105,  1873,  1874,   390,   394,   394,  1878,   390,
    1880,  1881,   383,   383,   383,  1885,  1886,   383,   383,   383,
    1985,  1986,  1892,   390,  1894,   383,  2057,   274,   390,   390,
    1900,   383,   383,   383,   383,   383,   383,   383,   383,   383,
     383,   383,  1912,    22,    22,    22,  1916,    22,    22,    22,
      22,    22,    22,   394,   394,   394,   394,   394,   394,   394,
    2091,   394,   383,  2028,   394,   394,  2031,  2032,   383,   383,
    2035,   394,   383,   872,   383,   383,   394,   383,     1,   383,
     390,   393,   395,   390,   383,   390,    22,    10,    11,    12,
     390,    14,    15,    16,    17,    18,   390,   390,   395,  2091,
      23,    24,    25,    26,    27,    28,  1441,  2040,    31,    32,
      33,    34,    35,   383,     1,  1985,  1986,   221,  2083,   393,
     393,   393,   393,   390,  1994,   393,  2091,  1997,   899,   390,
     390,   394,   390,   390,   383,   394,     1,  2102,   393,   383,
    2105,   383,   467,   468,   469,   470,   471,   472,   473,   474,
     475,   476,   390,   390,   390,   480,   390,   390,  2028,   390,
     390,  2031,  2032,  2128,  2129,  2035,   390,   390,   390,   390,
    2040,   390,   390,    83,   390,   390,   390,   390,   383,   396,
       1,     1,   394,   394,   394,   394,   394,   394,   390,     1,
     390,   516,   517,  2063,  2064,  2065,   521,   390,   523,   524,
     390,   390,   390,   528,   396,   390,   390,   394,   533,   534,
     396,   536,   394,  2083,   394,   390,   390,   542,   543,   390,
     545,  2091,   547,   390,   390,   390,   390,   390,   138,   139,
       1,   390,  2102,   394,   394,  2105,   146,   147,   148,   149,
     150,   151,   396,   390,   390,   390,   390,   390,   396,   235,
     390,   390,   394,   394,   164,   165,   166,   167,  2128,  2129,
     396,   171,   394,   396,   390,   390,   390,   390,   396,   396,
     390,   390,   394,   394,   390,   390,   390,   390,   396,   390,
     390,   390,   509,   193,   194,   195,   196,   390,   515,   396,
     396,   518,   519,   520,   390,   522,   390,   237,   525,   526,
     527,     1,     1,     1,   531,   532,     1,     1,   535,   500,
     537,   538,   539,   540,   541,     1,     1,   544,     1,   546,
       1,   548,   549,   550,   551,   552,   191,   192,   193,   194,
     195,   196,   197,   658,   199,   200,   201,   202,   203,   204,
     205,   206,     1,   208,   209,   210,   211,   212,   256,   214,
     275,     1,    -1,    -1,   679,   278,   279,   280,   281,   684,
      -1,   686,   687,    -1,    -1,    -1,    -1,    -1,    -1,   694,
     695,    -1,   295,   296,   297,   298,    -1,    -1,   288,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     323,   324,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,  1224,    -1,    -1,    -1,   754,
     657,    -1,    -1,   660,   661,   662,   663,   664,   665,   666,
     667,    -1,    -1,    -1,    -1,   672,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   683,    -1,    -1,    -1,
      -1,    -1,    -1,   690,   691,    -1,   693,    -1,    -1,    -1,
    1269,  1270,  1271,   396,    -1,  1274,    -1,    -1,  1277,  1278,
      -1,    -1,  1281,    -1,    -1,   810,    -1,  1286,  1287,  1288,
    1289,  1290,    -1,    -1,    -1,    -1,    -1,  1296,    -1,    -1,
      -1,    -1,    -1,    -1,   829,    -1,    -1,    -1,    -1,  1308,
    1309,  1310,    -1,    -1,  1313,  1314,  1315,  1316,  1317,    -1,
    1319,  1320,  1321,    -1,  1323,    -1,    -1,    -1,    -1,    -1,
      -1,   396,    -1,    -1,    -1,   445,    -1,    -1,    -1,    -1,
      -1,    -1,   867,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,  1352,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   901,   902,    -1,    -1,
      -1,    -1,     1,     1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    23,    -1,    -1,    -1,    -1,
      -1,    -1,   937,    -1,    -1,    -1,    34,    36,    37,    38,
      39,    40,    41,    42,    43,    44,    45,    46,    47,    48,
      49,    50,    51,    52,    53,    54,    55,    56,    57,    58,
      59,    60,    61,    62,    -1,    -1,    -1,    -1,    -1,    67,
      68,    69,    70,    71,    72,    73,    74,    75,    76,    77,
      78,    79,    80,    -1,   891,    -1,    -1,    -1,    -1,    87,
      88,    89,    90,    91,    -1,    -1,    -1,    95,    -1,    97,
      98,    99,   100,   101,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,     1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    10,    11,
      12,    -1,    14,    15,    16,    17,    18,  1526,    -1,    -1,
      -1,    23,    24,    25,    26,    27,    28,    -1,    -1,    31,
      32,    33,    34,    35,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   990,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   191,   192,   193,   194,   195,   196,   197,
      -1,   199,   200,   201,   202,   203,   204,   205,   206,   207,
     208,   209,   210,   211,   212,    -1,   214,   215,   216,   217,
     218,   219,   220,   221,   222,   223,   224,   225,   226,   227,
     228,   229,   230,   231,   232,   233,   234,   235,   236,   237,
     238,   239,    -1,   241,   242,   243,   244,   245,   246,   247,
     248,   249,   250,   251,   252,   253,   254,   255,   256,   257,
     258,  1640,   260,   261,   262,   263,   264,   265,   266,   267,
     268,   269,   270,    -1,   272,   273,    -1,   275,   276,   277,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,  1192,    -1,  1194,
      -1,    -1,    -1,   291,   292,   293,    -1,    -1,    -1,   297,
      -1,   299,   300,   301,   302,   303,   304,    -1,    -1,    -1,
      -1,    -1,    -1,  1218,  1219,    -1,    -1,    -1,  1223,    -1,
      -1,    -1,   320,   321,   322,    -1,    -1,   325,   326,   327,
     328,   329,   330,   331,   332,   333,   334,   335,   336,   337,
     338,   339,   340,   341,   342,   343,   344,   345,   346,   347,
     348,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,  1268,    -1,    -1,    -1,    -1,  1273,    -1,
    1275,  1276,    -1,    -1,    -1,    -1,    -1,    -1,    -1,  1284,
    1285,    -1,    -1,    -1,    -1,    -1,    -1,    -1,  1293,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   396,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   278,   279,   280,   281,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,  1225,    -1,
      -1,  1326,  1327,   295,   296,   297,   298,    -1,  1333,    -1,
    1335,  1336,    -1,    -1,    -1,  1340,    -1,  1342,    -1,    -1,
      -1,    -1,    -1,  1348,    -1,    -1,  1351,    -1,    -1,    -1,
      -1,   323,   324,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,  1272,    -1,  1372,  1373,    -1,
    1375,  1278,  1279,  1280,    -1,    -1,  1381,  1382,    -1,  1384,
      -1,  1386,    -1,    -1,    -1,  1292,    -1,    -1,    -1,    -1,
    1297,  1298,  1299,  1300,  1301,  1302,    -1,  1304,  1305,  1306,
    1879,    -1,    -1,    -1,  1311,    -1,    -1,    -1,    -1,    -1,
      -1,  1318,    -1,    -1,    -1,    -1,    -1,  1324,    -1,    -1,
      -1,  1328,  1329,  1330,   396,    -1,    -1,  1334,    -1,    -1,
    1337,  1338,  1339,    -1,  1913,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,  1370,  1371,    -1,     1,  1374,    -1,  1376,
    1377,  1378,  1379,  1380,    -1,    -1,  1383,    -1,  1385,    -1,
    1387,  1388,  1389,  1390,    -1,    -1,    -1,    -1,    23,    -1,
      -1,    -1,  1497,    -1,    -1,    -1,    -1,    -1,    -1,    34,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,  1521,    -1,    -1,    -1,
      -1,    -1,  1429,  1430,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,  1536,    67,    68,    69,    70,    71,    72,    73,    74,
      75,    76,    77,    78,    79,    80,    -1,    -1,    -1,    -1,
    2029,    -1,    87,    88,    89,    90,    91,    -1,    -1,  1564,
      95,    -1,    97,    98,    99,   100,   101,    -1,    -1,    -1,
      10,    11,    12,    -1,    14,    15,    16,    17,    18,    -1,
      -1,    -1,  1587,    23,    24,    25,    26,    27,    28,  1496,
      -1,    31,    32,    33,    34,    35,    -1,    -1,  1505,    -1,
      -1,  1508,  1509,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
    1517,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
    1625,    -1,    -1,  1628,  2103,    -1,    -1,    -1,    -1,    -1,
      -1,  1538,    -1,  1540,  1639,    -1,    -1,    -1,    -1,  1546,
    1547,    -1,  1549,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   191,   192,   193,   194,
     195,   196,   197,    -1,   199,   200,   201,   202,   203,   204,
     205,   206,   207,   208,   209,   210,   211,   212,    -1,   214,
     215,   216,   217,   218,   219,   220,   221,   222,   223,   224,
     225,   226,   227,   228,   229,   230,   231,   232,   233,   234,
     235,   236,   237,   238,   239,    -1,   241,   242,   243,   244,
     245,   246,   247,   248,   249,   250,   251,   252,   253,   254,
     255,   256,   257,   258,    -1,   260,   261,   262,   263,   264,
     265,   266,   267,   268,   269,   270,    -1,   272,   273,    -1,
     275,   276,   277,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   291,   292,   293,    -1,
      -1,    -1,   297,    -1,   299,   300,   301,   302,   303,   304,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   320,   321,   322,    -1,    -1,
     325,   326,   327,   328,   329,   330,   331,   332,   333,   334,
     335,   336,   337,   338,   339,   340,   341,   342,   343,   344,
     345,   346,   347,   348,    -1,  1820,    -1,    -1,    -1,  1824,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,  1832,    -1,    -1,
      -1,    -1,    -1,  1838,    -1,    -1,    -1,    -1,   278,   279,
     280,   281,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   295,   296,   297,   298,    -1,
      -1,   396,    -1,    -1,    -1,    -1,    -1,    -1,  1873,  1874,
      -1,    -1,    -1,  1878,    -1,  1880,  1881,    -1,    -1,    -1,
    1885,  1886,    -1,   323,   324,    -1,    -1,  1892,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,  1900,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,  1912,    -1,    -1,
      -1,  1916,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,     0,     1,  1833,     3,    -1,    -1,
      -1,    -1,    -1,    -1,    10,    11,    12,    -1,    14,    15,
      16,    17,    18,    19,    -1,    -1,    -1,    23,    24,    25,
      26,    27,    28,    -1,    -1,    31,    32,    33,    34,    35,
      -1,    -1,    -1,    -1,    -1,  1872,    -1,    -1,  1875,  1876,
    1877,    -1,    -1,    -1,    -1,  1882,  1883,  1884,    -1,    -1,
    1985,  1986,     0,     1,    -1,     3,    -1,    63,    -1,    -1,
      -1,    -1,    10,    11,    12,    -1,    14,    15,    16,    17,
      18,    19,    -1,    -1,    -1,    23,    24,    25,    26,    27,
      28,    -1,    -1,    31,    32,    33,    34,    35,    -1,    -1,
      -1,    -1,    -1,  2028,    -1,    -1,  2031,  2032,    -1,    -1,
    2035,    -1,    -1,    -1,    -1,  2040,    -1,    -1,     0,     1,
      -1,     3,    -1,    -1,    -1,    63,    -1,    -1,    10,    11,
      12,    -1,    14,    15,    16,    17,    18,    19,    -1,    -1,
      -1,    23,    24,    25,    26,    27,    28,    -1,    -1,    31,
      32,    33,    34,    35,  1981,  1982,    -1,    -1,  2083,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,  2102,    -1,    -1,
    2105,    63,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,  2026,
      -1,    -1,    -1,  2128,  2129,    -1,  2033,  2034,     0,     1,
      -1,     3,    -1,    -1,    -1,    -1,    -1,    -1,    10,    11,
      12,    -1,    14,    15,    16,    17,    18,    19,    -1,    -1,
      -1,    23,    24,    25,    26,    27,    28,    -1,    -1,    31,
      32,    33,    34,    35,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,  2084,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    63,    -1,    -1,  2101,    -1,    -1,    -1,    -1,    -1,
    2107,    -1,   278,   279,   280,   281,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   295,
     296,   297,   298,    -1,    -1,    -1,    -1,   303,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   313,    -1,    -1,
     316,   317,    -1,    -1,    -1,    -1,    -1,   323,   324,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     278,   279,   280,   281,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   295,   296,   297,
     298,    -1,    -1,    -1,    -1,   303,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   313,    -1,    -1,   316,   317,
      -1,    -1,    -1,    -1,    -1,   323,   324,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   390,    -1,   278,   279,   280,   281,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   295,   296,   297,   298,    -1,    -1,    -1,
      -1,   303,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   313,    -1,    -1,   316,   317,    -1,    -1,    -1,    -1,
      -1,   323,   324,    -1,     0,     1,    -1,     3,    -1,    -1,
      -1,    -1,   390,    -1,    10,    11,    12,    -1,    14,    15,
      16,    17,    18,    19,    -1,    -1,    -1,    23,    24,    25,
      26,    27,    28,    -1,    -1,    31,    32,    33,    34,    35,
      -1,    -1,    -1,    -1,    -1,    -1,   278,   279,   280,   281,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   295,   296,   297,   298,    63,   390,    -1,
      -1,   303,     4,     5,     6,     7,    -1,    -1,    -1,    -1,
      -1,   313,    -1,    -1,   316,   317,     0,     1,    -1,     3,
      -1,   323,   324,    -1,    -1,    -1,    10,    11,    12,    -1,
      14,    15,    16,    17,    18,    19,    -1,    -1,    -1,    23,
      24,    25,    26,    27,    28,    -1,    -1,    31,    32,    33,
      34,    35,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    63,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   390,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   103,   104,   105,   106,   107,   108,   109,   110,   111,
     112,   113,   114,   115,   116,   117,   118,   119,   120,   121,
     122,   123,   124,   125,   126,   127,   128,   129,   130,   131,
     132,   133,   134,   135,   136,   137,   138,   139,   140,   141,
     142,   143,   144,   145,   146,   147,   148,   149,   150,   151,
     152,   153,   154,   155,   156,   157,   158,   159,   160,   161,
     162,   163,   164,   165,   166,   167,   168,   169,   170,   171,
      10,    11,    12,    -1,    14,    15,    16,    17,    18,    -1,
      -1,    -1,    -1,    23,    24,    25,    26,    27,    28,    -1,
      -1,    31,    32,    33,    34,    35,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   278,   279,   280,   281,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   295,
     296,   297,   298,    -1,    -1,    -1,    -1,   303,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   313,    -1,    -1,
     316,   317,    -1,    -1,    -1,    -1,    -1,   323,   324,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   278,   279,   280,   281,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   295,   296,   297,   298,    -1,    -1,    -1,    -1,   303,
      -1,    -1,    -1,    -1,   390,    -1,    -1,    -1,    -1,   313,
      -1,    -1,   316,   317,    -1,    -1,    -1,    -1,    -1,   323,
     324,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   351,
     352,   353,   354,   355,   356,   357,   358,   359,   360,   361,
     362,   363,   364,   365,   366,   367,   368,   369,   370,   371,
     372,   373,   374,   375,   376,   377,   378,   379,   380,   381,
     382,    -1,   384,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     392,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   390,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   278,   279,
     280,   281,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   295,   296,   297,   298,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   323,   324,   103,   104,   105,   106,   107,
     108,   109,   110,   111,   112,   113,   114,   115,   116,   117,
     118,   119,   120,   121,   122,   123,   124,   125,   126,   127,
     128,   129,   130,   131,   132,   133,   134,   135,   136,   137,
     138,   139,   140,   141,   142,   143,   144,   145,   146,   147,
     148,   149,   150,   151,   152,   153,   154,   155,   156,   157,
     158,   159,   160,   161,   162,   163,   164,   165,   166,   167,
     168,   169,   170,   171,    -1,    -1,   396,   103,   104,   105,
     106,   107,   108,   109,   110,   111,   112,   113,   114,   115,
     116,   117,   118,   119,   120,   121,   122,   123,   124,   125,
     126,   127,   128,   129,   130,   131,   132,   133,   134,   135,
     136,   137,   138,   139,   140,   141,   142,   143,   144,   145,
     146,   147,   148,   149,   150,   151,   152,   153,   154,   155,
     156,   157,   158,   159,   160,   161,   162,   163,   164,   165,
     166,   167,   168,   169,   170,   171,   103,   104,   105,   106,
     107,   108,   109,   110,   111,   112,   113,   114,   115,   116,
     117,   118,   119,   120,   121,   122,   123,   124,   125,   126,
     127,   128,   129,   130,   131,   132,   133,   134,   135,   136,
     137,   138,   139,   140,   141,   142,   143,   144,   145,   146,
     147,   148,   149,   150,   151,   152,   153,   154,   155,   156,
     157,   158,   159,   160,   161,   162,   163,   164,   165,   166,
     167,   168,   169,   170,   171,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   351,   352,   353,   354,   355,   356,   357,
     358,   359,   360,   361,   362,   363,   364,   365,   366,   367,
     368,   369,   370,   371,   372,   373,   374,   375,   376,   377,
     378,   379,   380,   381,   382,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   395,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   351,   352,   353,   354,   355,
     356,   357,   358,   359,   360,   361,   362,   363,   364,   365,
     366,   367,   368,   369,   370,   371,   372,   373,   374,   375,
     376,   377,   378,   379,   380,   381,   382,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   395,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   351,   352,   353,   354,   355,   356,
     357,   358,   359,   360,   361,   362,   363,   364,   365,   366,
     367,   368,   369,   370,   371,   372,   373,   374,   375,   376,
     377,   378,   379,   380,   381,   382,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   395,   103,
     104,   105,   106,   107,   108,   109,   110,   111,   112,   113,
     114,   115,   116,   117,   118,   119,   120,   121,   122,   123,
     124,   125,   126,   127,   128,   129,   130,   131,   132,   133,
     134,   135,   136,   137,   138,   139,   140,   141,   142,   143,
     144,   145,   146,   147,   148,   149,   150,   151,   152,   153,
     154,   155,   156,   157,   158,   159,   160,   161,   162,   163,
     164,   165,   166,   167,   168,   169,   170,   171,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   351,   352,   353,
     354,   355,   356,   357,   358,   359,   360,   361,   362,   363,
     364,   365,   366,   367,   368,   369,   370,   371,   372,   373,
     374,   375,   376,   377,   378,   379,   380,   381,   382
};

/* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
   symbol of state STATE-NUM.  */
static const yytype_uint16 yystos[] =
{
       0,   404,     0,     1,     3,    10,    11,    12,    14,    15,
      16,    17,    18,    19,    23,    24,    25,    26,    27,    28,
      31,    32,    33,    34,    35,    63,   278,   279,   280,   281,
     295,   296,   297,   298,   303,   313,   316,   317,   323,   324,
     390,   405,   433,   436,   440,   441,   442,   443,   444,   449,
     450,   452,   453,   456,   464,   473,   477,   480,   483,   487,
     498,   499,   505,   507,   510,   513,   516,   520,   524,   528,
      22,    22,   436,   438,   439,   439,   439,   438,   438,   438,
     438,   438,    22,   434,   436,   498,   439,    64,   438,   438,
     438,   438,   439,   439,   439,   439,    22,   439,   439,   439,
     498,    22,   438,     1,     1,     1,     1,     1,     1,    36,
      37,    38,    39,    40,    41,    42,    43,    44,    45,    46,
      47,    48,    49,    50,    51,    52,    53,    54,    55,    56,
      57,    58,    59,    60,    61,    62,   504,   505,   511,   438,
     398,   490,   491,   395,   395,   395,   474,   484,   488,   481,
     478,   435,   491,   498,   514,   391,   295,   296,   437,   437,
     437,   437,   392,   395,   517,   521,   525,   529,   395,   395,
     390,   454,   390,   390,    22,   491,   491,   489,    21,    21,
      21,   491,   491,   491,   491,   491,   491,   395,   395,   512,
       4,     5,   409,   465,   461,   457,   459,    29,    30,    36,
     445,   447,   448,    21,   491,   491,   491,   491,    21,    21,
     491,   506,   395,     1,    22,   399,   395,   395,   395,   395,
     395,   395,    19,    20,   268,   429,   432,   396,   507,   491,
     491,   491,   491,   386,   446,    47,   393,   394,   518,   522,
     526,   530,   395,     1,   383,   391,   400,   490,   492,   493,
     494,   495,   496,   497,   500,   501,   502,   503,   440,   441,
     442,   443,   444,   449,   450,   453,   456,   464,   473,   477,
     480,   483,   487,   507,   508,   509,   513,   383,   472,   482,
     486,   228,   229,   479,   476,   432,    22,    22,   430,   392,
       1,   396,   429,   396,   466,   462,   458,   460,    22,   446,
     395,   447,   395,   395,   395,   395,   451,     1,     6,     7,
     384,   392,   395,   407,   408,   409,   417,    22,   384,   392,
     401,   408,   500,   490,   500,   490,   500,   501,   390,   396,
     508,     9,   407,   408,   409,   412,     1,   172,   173,   174,
     175,   176,   187,   189,   190,   396,     1,   243,   244,   245,
     246,   247,   248,   249,   250,   252,   253,   254,   255,   256,
     260,   261,   262,   263,   264,   265,   266,   267,   396,     1,
     327,   328,   329,   330,   331,   332,   333,   334,   335,   336,
     337,   338,   339,   340,   341,   342,   343,   344,   345,   346,
     347,   348,   396,   383,   383,     1,   215,   217,   218,   221,
     222,   223,   224,   225,   226,   227,   230,   231,   232,   233,
     234,   235,   236,   237,   238,   239,   241,   242,   396,     1,
     191,   192,   193,   194,   195,   196,   197,   199,   200,   201,
     202,   203,   204,   205,   206,   208,   209,   210,   211,   212,
     214,   396,   396,   294,   390,   431,   491,     9,    22,   395,
     395,   395,   395,    22,    21,   305,   515,   519,   523,   527,
       1,    75,   396,   407,   408,   407,   408,   384,   385,   386,
     387,   389,   384,   385,   386,   387,   389,   407,   408,   409,
     417,   419,   420,   423,   408,   408,   384,   385,   386,   401,
     500,   500,   490,   500,   390,   390,   390,   390,   390,   383,
     383,   383,   383,   383,   383,   383,   383,   390,   383,   383,
     383,   383,   383,   383,   383,   383,   383,   383,   383,   383,
     383,   383,   383,   383,   383,   383,   383,   383,   383,   390,
     383,   383,   383,   400,   400,   383,   383,   383,   383,   383,
     383,   383,   383,   383,   383,   383,   383,   383,   383,   383,
     383,   383,   417,   103,   104,   105,   106,   107,   108,   109,
     110,   111,   112,   113,   114,   115,   116,   117,   118,   119,
     120,   121,   122,   123,   124,   125,   126,   127,   128,   129,
     130,   131,   132,   133,   134,   135,   136,   137,   138,   139,
     140,   141,   142,   143,   144,   145,   146,   147,   148,   149,
     150,   151,   152,   153,   154,   155,   156,   157,   158,   159,
     160,   161,   162,   163,   164,   165,   166,   167,   168,   169,
     170,   171,   351,   352,   353,   354,   355,   356,   357,   358,
     359,   360,   361,   362,   363,   364,   365,   366,   367,   368,
     369,   370,   371,   372,   373,   374,   375,   376,   377,   378,
     379,   380,   381,   382,   426,   390,   383,   383,   383,   383,
     383,   383,   383,   383,   383,   383,   383,   383,   383,   383,
     383,   383,   383,   383,   383,   383,   383,   383,   390,   383,
     383,   383,   383,   383,   383,   383,   383,   383,   383,   383,
     383,   383,   383,   383,   383,   383,   383,   383,   383,   383,
     390,   491,   395,   393,   393,   463,   455,   455,   455,   383,
      34,   306,   307,   308,   309,   310,   311,   312,   396,    34,
     314,   315,   396,   285,   288,   396,   318,   319,   396,   383,
     393,   393,   407,   408,   407,   408,   407,   408,   407,   408,
     407,   408,   407,   408,   407,   408,   407,   408,   407,   408,
     407,   408,   419,     1,   394,   396,   418,     1,   394,   418,
     394,   418,   408,   408,   408,   500,     1,   177,   178,   179,
     180,   181,   182,   467,   467,     1,   183,   184,   185,   186,
     468,   468,   468,     1,   102,   188,   471,     1,   108,   109,
     110,   111,   112,   113,   114,   115,   470,     1,   377,   378,
     379,   469,   407,   408,   409,   410,   426,   410,   410,   410,
     417,   410,   426,   407,   408,   409,   411,   411,   426,   426,
     426,   411,   426,   411,   411,   426,   426,   426,   411,   395,
     426,   426,   411,   411,   426,   408,   409,   412,   426,   426,
     426,   426,   426,   412,   412,   426,   412,   426,   411,   426,
     426,   426,   426,   426,   390,   410,   426,   412,   410,   426,
     426,   426,   426,   426,   426,   426,   426,   417,   410,   410,
     410,   426,   417,   410,   410,   410,   410,   412,   410,   410,
     410,   426,   412,   410,   411,   411,   410,   410,   426,   426,
     410,   417,   426,   412,   412,   410,   410,   410,   410,   395,
     428,   383,   383,    22,   282,   283,   396,    22,    75,   284,
     285,   286,   287,   288,   396,   396,   396,   417,   392,   383,
     383,   383,   383,   383,   383,   383,   392,   383,   383,   383,
     383,   383,   383,    22,   418,   407,   408,   417,   409,   390,
     390,   390,   390,   390,   390,   390,   390,   390,   390,   390,
     390,   390,   412,   390,   390,   390,   390,   390,   390,   390,
     390,   390,   390,   390,   390,   390,   390,   390,   412,   390,
     390,   401,   401,   390,   390,   390,   390,   390,   390,   390,
     390,   390,   390,   390,   390,   390,   390,   390,   390,   390,
     394,   390,   390,   390,   390,   390,   390,   390,   390,   390,
     390,   390,   390,   412,   390,   390,   390,   390,   410,   390,
     390,   390,   390,   390,   390,   390,   390,   390,   390,   390,
     390,   390,   390,   390,   390,   390,   390,   426,   390,   390,
     390,   390,   390,   390,   390,   428,     1,    23,    34,    67,
      68,    69,    70,    71,    72,    73,    74,    75,    76,    77,
      78,    79,    80,    87,    88,    89,    90,    91,    95,    97,
      98,    99,   100,   101,   191,   192,   193,   194,   195,   196,
     197,   199,   200,   201,   202,   203,   204,   205,   206,   207,
     208,   209,   210,   211,   212,   214,   215,   216,   217,   218,
     219,   220,   221,   222,   223,   224,   225,   226,   227,   228,
     229,   230,   231,   232,   233,   234,   235,   236,   237,   238,
     239,   241,   242,   243,   244,   245,   246,   247,   248,   249,
     250,   251,   252,   253,   254,   255,   256,   257,   258,   260,
     261,   262,   263,   264,   265,   266,   267,   268,   269,   270,
     272,   273,   275,   276,   277,   291,   292,   293,   297,   299,
     300,   301,   302,   303,   304,   320,   321,   322,   325,   326,
     327,   328,   329,   330,   331,   332,   333,   334,   335,   336,
     337,   338,   339,   340,   341,   342,   343,   344,   345,   346,
     347,   348,   396,   475,   485,   411,   411,   383,   383,   383,
     383,   383,   383,   383,   383,   383,   383,    22,   416,    22,
     417,    22,    22,    22,    22,   408,    22,    22,   417,   417,
     290,   417,   289,     9,     9,   390,   419,   394,   413,   394,
     383,   383,   426,   413,   413,   413,   396,   390,   392,   392,
     383,   398,   383,   398,   383,   398,   383,   398,   383,   398,
     383,   392,   392,   392,   383,   392,   383,   392,   383,   392,
     392,   392,   383,   398,   383,   398,   383,   398,   383,   398,
     383,   398,   392,   392,   392,   392,   392,   392,   383,   383,
     383,   383,   383,   383,   383,   383,   383,   383,   383,   383,
     383,   383,   383,   383,   383,   383,   383,   383,   383,   383,
     383,   383,   383,   383,   383,   383,   383,   383,   383,   383,
     383,   383,   383,   383,   383,   383,   383,   383,   383,   383,
     383,   383,   383,   383,   383,   383,   383,   383,   383,   383,
     383,   383,   383,   383,   383,   383,   383,   383,   383,   383,
     383,   383,   383,   383,   383,   383,   383,   383,   383,   383,
     383,   392,   383,   383,   383,   383,   383,   383,   400,   383,
     392,   383,   383,   383,   383,   383,   383,   383,   383,   383,
     383,   383,   398,   383,   398,   383,   398,   383,   398,   383,
     383,   383,   400,   400,   383,   383,   383,   383,   383,   383,
     383,   383,   383,   383,   383,   383,   383,   383,   383,   383,
     383,   390,   390,     9,    22,    22,   417,     9,    22,   290,
     411,   417,   290,   417,   412,    22,   289,   413,   418,   393,
     416,   390,   390,   390,   390,   390,   390,   393,   416,   416,
     390,   408,   421,   390,   390,   390,   418,   412,   412,   395,
     395,   418,   412,   410,   426,    22,    22,     5,    22,   414,
     415,   417,     9,    22,   406,   415,   406,   415,   406,   415,
     406,   415,   406,     9,    22,    22,    22,    22,    22,    22,
      22,    22,    22,    22,    22,   415,   406,   415,   406,   415,
     406,   415,   406,   415,   406,    22,    22,    22,    22,    22,
      22,   412,   410,   410,   410,   426,   412,   410,   411,   411,
     410,   410,   426,   426,   426,   410,   417,   417,   412,   412,
     410,   410,   410,   410,   410,   417,   426,   412,   395,   395,
     410,   426,   426,   426,   426,   426,   426,   417,   426,   426,
     426,   417,   410,   410,   410,   426,   417,   410,   410,   410,
     410,   410,   426,   410,   410,   410,   417,   410,   395,   426,
     395,   411,   411,   426,   426,   426,   395,   395,   411,   395,
     426,   411,   411,   426,   426,   426,   411,   406,   411,   271,
      22,   274,   427,    22,   417,   411,   417,    22,   411,   410,
     395,   415,   408,   417,   422,   422,    22,   408,   417,   415,
     406,   415,   406,   415,   406,   415,   406,   395,   426,   426,
     411,   411,   426,   412,   426,   426,   426,   426,   426,   412,
     412,   426,   412,   426,   411,   426,   426,   426,   426,   390,
     390,   390,   416,   390,   390,   390,   390,   421,   390,   421,
     390,   390,   390,    22,   390,   383,   500,   418,   383,   500,
     418,   418,   394,   418,   418,   396,   426,   426,   390,   413,
     413,   418,   393,   393,   400,   402,   413,   390,   414,   399,
     390,   399,   390,   399,   390,   399,   390,   399,   390,   393,
     393,   393,   390,   393,   390,   393,   390,   393,   393,   393,
     390,   399,   390,   399,   390,   399,   390,   399,   390,   399,
     393,   393,   393,   393,   393,   393,   390,   390,   390,   390,
     390,   390,   390,   390,   390,   390,   390,   390,   390,   390,
     390,   426,   412,   390,   390,   390,   390,   390,   390,   390,
     426,   390,   390,   426,   426,   390,   390,   390,   390,   390,
     390,   390,   426,   390,   390,   390,   412,   390,   390,   390,
     390,   410,   390,   390,   390,   390,   390,   390,   390,   390,
     390,   412,   390,   426,   390,   426,   390,   390,   390,   390,
     390,   426,   426,   390,   426,   390,   390,   390,   390,   390,
     390,   390,   393,   390,   390,   390,   390,   397,   390,   419,
     401,   421,   394,   390,   390,    22,   390,   390,   421,     1,
     390,   394,   390,   390,   390,   421,   390,   399,   390,   399,
     390,   399,   390,   399,   412,   390,   390,   401,   401,   390,
     390,   390,   390,   390,   390,   390,   390,   390,   390,   390,
     390,   390,   390,   390,   390,   390,   418,   418,   418,    22,
     417,   390,   390,    22,   417,   390,   390,   390,   408,   390,
     390,   390,   394,   394,   412,   410,   390,   383,   383,   500,
     408,    73,    74,    75,    79,    80,    22,   418,   383,   383,
     383,   383,   383,   383,   383,   383,   390,   390,   390,   383,
     383,   383,   383,   383,   383,   383,   383,   383,   383,   383,
     383,   383,   413,   394,   394,   394,   394,   394,   413,   413,
     413,   394,   394,   394,   394,   394,   383,   274,   418,   383,
     418,    22,   394,   418,   417,   418,   383,   383,   383,   383,
     394,   383,   383,   390,   390,   390,   390,   416,   390,   416,
     411,   426,   413,   413,    22,    22,   417,   390,   401,   393,
     393,   393,   393,   393,   415,   415,   415,   415,   415,    22,
     395,   408,    22,    22,   395,   408,   415,   415,   415,   415,
     415,    22,   408,    22,    22,   395,   408,    22,   426,   412,
     412,   426,   426,   426,   412,   410,   412,   411,   426,   426,
     426,   411,   411,   390,   420,   390,   393,     1,    22,    29,
     407,   408,   424,   425,   421,   390,   415,   415,   415,   415,
     412,   395,   395,   418,   418,   394,   394,   412,   410,   390,
     390,   416,   393,   383,   383,   383,   383,   383,   390,   390,
     390,   390,   390,   390,    22,   390,   390,   390,    22,   390,
     390,   390,   390,   390,   390,   390,   390,   390,   390,    22,
     390,   390,   418,   418,   418,   396,   394,   418,   413,   413,
     418,   394,   394,   394,   394,   394,   390,   390,   390,     1,
     394,   396,   418,   390,   390,   390,   390,   396,   426,   426,
     390,   390,   412,   412,   418,   418,   418,   383,    22,   408,
      22,    22,   408,   394,   394,   394,   390,   390,   390,   390,
     426,   390,   412,   410,   390,   411,   411,   426,   426,   411,
     424,   390,   390,   394,   394,   396,   396,   390,   390,   390,
      22,   417,   422,   390,   390,   390,   390,   390,   408,   408,
     408,   394,   413,   413,   396,   394,   396,   394,   396,   411,
     426,   390,   390,   390,   420,   423,   390,   396,   396,   396,
     426,   412,   410,   390,   411,   390,   426,   390,   394,   394,
     418,   418,   390,   390,   390,   396,   418,   418,   396,   396,
     412,   412,   390,   390,   390,   390,   390,   390,   390,   396,
     396,   390,   390
};

#define yyerrok		(yyerrstatus = 0)
#define yyclearin	(yychar = YYEMPTY)
#define YYEMPTY		(-2)
#define YYEOF		0

#define YYACCEPT	goto yyacceptlab
#define YYABORT		goto yyabortlab
#define YYERROR		goto yyerrorlab


/* Like YYERROR except do call yyerror.  This remains here temporarily
   to ease the transition to the new meaning of YYERROR, for GCC.
   Once GCC version 2 has supplanted version 1, this can go.  */

#define YYFAIL		goto yyerrlab

#define YYRECOVERING()  (!!yyerrstatus)

#define YYBACKUP(Token, Value)					\
do								\
  if (yychar == YYEMPTY && yylen == 1)				\
    {								\
      yychar = (Token);						\
      yylval = (Value);						\
      yytoken = YYTRANSLATE (yychar);				\
      YYPOPSTACK (1);						\
      goto yybackup;						\
    }								\
  else								\
    {								\
      yyerror (YY_("syntax error: cannot back up")); \
      YYERROR;							\
    }								\
while (YYID (0))


#define YYTERROR	1
#define YYERRCODE	256


/* YYLLOC_DEFAULT -- Set CURRENT to span from RHS[1] to RHS[N].
   If N is 0, then set CURRENT to the empty location which ends
   the previous symbol: RHS[0] (always defined).  */

#define YYRHSLOC(Rhs, K) ((Rhs)[K])
#ifndef YYLLOC_DEFAULT
# define YYLLOC_DEFAULT(Current, Rhs, N)				\
    do									\
      if (YYID (N))                                                    \
	{								\
	  (Current).first_line   = YYRHSLOC (Rhs, 1).first_line;	\
	  (Current).first_column = YYRHSLOC (Rhs, 1).first_column;	\
	  (Current).last_line    = YYRHSLOC (Rhs, N).last_line;		\
	  (Current).last_column  = YYRHSLOC (Rhs, N).last_column;	\
	}								\
      else								\
	{								\
	  (Current).first_line   = (Current).last_line   =		\
	    YYRHSLOC (Rhs, 0).last_line;				\
	  (Current).first_column = (Current).last_column =		\
	    YYRHSLOC (Rhs, 0).last_column;				\
	}								\
    while (YYID (0))
#endif


/* YY_LOCATION_PRINT -- Print the location on the stream.
   This macro was not mandated originally: define only if we know
   we won't break user code: when these are the locations we know.  */

#ifndef YY_LOCATION_PRINT
# if YYLTYPE_IS_TRIVIAL
#  define YY_LOCATION_PRINT(File, Loc)			\
     fprintf (File, "%d.%d-%d.%d",			\
	      (Loc).first_line, (Loc).first_column,	\
	      (Loc).last_line,  (Loc).last_column)
# else
#  define YY_LOCATION_PRINT(File, Loc) ((void) 0)
# endif
#endif


/* YYLEX -- calling `yylex' with the right arguments.  */

#ifdef YYLEX_PARAM
# define YYLEX yylex (YYLEX_PARAM)
#else
# define YYLEX yylex ()
#endif

/* Enable debugging if requested.  */
#if YYDEBUG

# ifndef YYFPRINTF
#  include <stdio.h> /* INFRINGES ON USER NAME SPACE */
#  define YYFPRINTF fprintf
# endif

# define YYDPRINTF(Args)			\
do {						\
  if (yydebug)					\
    YYFPRINTF Args;				\
} while (YYID (0))

# define YY_SYMBOL_PRINT(Title, Type, Value, Location)			  \
do {									  \
  if (yydebug)								  \
    {									  \
      YYFPRINTF (stderr, "%s ", Title);					  \
      yy_symbol_print (stderr,						  \
		  Type, Value); \
      YYFPRINTF (stderr, "\n");						  \
    }									  \
} while (YYID (0))


/*--------------------------------.
| Print this symbol on YYOUTPUT.  |
`--------------------------------*/

/*ARGSUSED*/
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_symbol_value_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep)
#else
static void
yy_symbol_value_print (yyoutput, yytype, yyvaluep)
    FILE *yyoutput;
    int yytype;
    YYSTYPE const * const yyvaluep;
#endif
{
  if (!yyvaluep)
    return;
# ifdef YYPRINT
  if (yytype < YYNTOKENS)
    YYPRINT (yyoutput, yytoknum[yytype], *yyvaluep);
# else
  YYUSE (yyoutput);
# endif
  switch (yytype)
    {
      default:
	break;
    }
}


/*--------------------------------.
| Print this symbol on YYOUTPUT.  |
`--------------------------------*/

#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_symbol_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep)
#else
static void
yy_symbol_print (yyoutput, yytype, yyvaluep)
    FILE *yyoutput;
    int yytype;
    YYSTYPE const * const yyvaluep;
#endif
{
  if (yytype < YYNTOKENS)
    YYFPRINTF (yyoutput, "token %s (", yytname[yytype]);
  else
    YYFPRINTF (yyoutput, "nterm %s (", yytname[yytype]);

  yy_symbol_value_print (yyoutput, yytype, yyvaluep);
  YYFPRINTF (yyoutput, ")");
}

/*------------------------------------------------------------------.
| yy_stack_print -- Print the state stack from its BOTTOM up to its |
| TOP (included).                                                   |
`------------------------------------------------------------------*/

#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_stack_print (yytype_int16 *yybottom, yytype_int16 *yytop)
#else
static void
yy_stack_print (yybottom, yytop)
    yytype_int16 *yybottom;
    yytype_int16 *yytop;
#endif
{
  YYFPRINTF (stderr, "Stack now");
  for (; yybottom <= yytop; yybottom++)
    {
      int yybot = *yybottom;
      YYFPRINTF (stderr, " %d", yybot);
    }
  YYFPRINTF (stderr, "\n");
}

# define YY_STACK_PRINT(Bottom, Top)				\
do {								\
  if (yydebug)							\
    yy_stack_print ((Bottom), (Top));				\
} while (YYID (0))


/*------------------------------------------------.
| Report that the YYRULE is going to be reduced.  |
`------------------------------------------------*/

#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_reduce_print (YYSTYPE *yyvsp, int yyrule)
#else
static void
yy_reduce_print (yyvsp, yyrule)
    YYSTYPE *yyvsp;
    int yyrule;
#endif
{
  int yynrhs = yyr2[yyrule];
  int yyi;
  unsigned long int yylno = yyrline[yyrule];
  YYFPRINTF (stderr, "Reducing stack by rule %d (line %lu):\n",
	     yyrule - 1, yylno);
  /* The symbols being reduced.  */
  for (yyi = 0; yyi < yynrhs; yyi++)
    {
      YYFPRINTF (stderr, "   $%d = ", yyi + 1);
      yy_symbol_print (stderr, yyrhs[yyprhs[yyrule] + yyi],
		       &(yyvsp[(yyi + 1) - (yynrhs)])
		       		       );
      YYFPRINTF (stderr, "\n");
    }
}

# define YY_REDUCE_PRINT(Rule)		\
do {					\
  if (yydebug)				\
    yy_reduce_print (yyvsp, Rule); \
} while (YYID (0))

/* Nonzero means print parse trace.  It is left uninitialized so that
   multiple parsers can coexist.  */
int yydebug;
#else /* !YYDEBUG */
# define YYDPRINTF(Args)
# define YY_SYMBOL_PRINT(Title, Type, Value, Location)
# define YY_STACK_PRINT(Bottom, Top)
# define YY_REDUCE_PRINT(Rule)
#endif /* !YYDEBUG */


/* YYINITDEPTH -- initial size of the parser's stacks.  */
#ifndef	YYINITDEPTH
# define YYINITDEPTH 200
#endif

/* YYMAXDEPTH -- maximum size the stacks can grow to (effective only
   if the built-in stack extension method is used).

   Do not make this value too large; the results are undefined if
   YYSTACK_ALLOC_MAXIMUM < YYSTACK_BYTES (YYMAXDEPTH)
   evaluated with infinite-precision integer arithmetic.  */

#ifndef YYMAXDEPTH
# define YYMAXDEPTH 10000
#endif



#if YYERROR_VERBOSE

# ifndef yystrlen
#  if defined __GLIBC__ && defined _STRING_H
#   define yystrlen strlen
#  else
/* Return the length of YYSTR.  */
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static YYSIZE_T
yystrlen (const char *yystr)
#else
static YYSIZE_T
yystrlen (yystr)
    const char *yystr;
#endif
{
  YYSIZE_T yylen;
  for (yylen = 0; yystr[yylen]; yylen++)
    continue;
  return yylen;
}
#  endif
# endif

# ifndef yystpcpy
#  if defined __GLIBC__ && defined _STRING_H && defined _GNU_SOURCE
#   define yystpcpy stpcpy
#  else
/* Copy YYSRC to YYDEST, returning the address of the terminating '\0' in
   YYDEST.  */
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static char *
yystpcpy (char *yydest, const char *yysrc)
#else
static char *
yystpcpy (yydest, yysrc)
    char *yydest;
    const char *yysrc;
#endif
{
  char *yyd = yydest;
  const char *yys = yysrc;

  while ((*yyd++ = *yys++) != '\0')
    continue;

  return yyd - 1;
}
#  endif
# endif

# ifndef yytnamerr
/* Copy to YYRES the contents of YYSTR after stripping away unnecessary
   quotes and backslashes, so that it's suitable for yyerror.  The
   heuristic is that double-quoting is unnecessary unless the string
   contains an apostrophe, a comma, or backslash (other than
   backslash-backslash).  YYSTR is taken from yytname.  If YYRES is
   null, do not copy; instead, return the length of what the result
   would have been.  */
static YYSIZE_T
yytnamerr (char *yyres, const char *yystr)
{
  if (*yystr == '"')
    {
      YYSIZE_T yyn = 0;
      char const *yyp = yystr;

      for (;;)
	switch (*++yyp)
	  {
	  case '\'':
	  case ',':
	    goto do_not_strip_quotes;

	  case '\\':
	    if (*++yyp != '\\')
	      goto do_not_strip_quotes;
	    /* Fall through.  */
	  default:
	    if (yyres)
	      yyres[yyn] = *yyp;
	    yyn++;
	    break;

	  case '"':
	    if (yyres)
	      yyres[yyn] = '\0';
	    return yyn;
	  }
    do_not_strip_quotes: ;
    }

  if (! yyres)
    return yystrlen (yystr);

  return yystpcpy (yyres, yystr) - yyres;
}
# endif

/* Copy into YYRESULT an error message about the unexpected token
   YYCHAR while in state YYSTATE.  Return the number of bytes copied,
   including the terminating null byte.  If YYRESULT is null, do not
   copy anything; just return the number of bytes that would be
   copied.  As a special case, return 0 if an ordinary "syntax error"
   message will do.  Return YYSIZE_MAXIMUM if overflow occurs during
   size calculation.  */
static YYSIZE_T
yysyntax_error (char *yyresult, int yystate, int yychar)
{
  int yyn = yypact[yystate];

  if (! (YYPACT_NINF < yyn && yyn <= YYLAST))
    return 0;
  else
    {
      int yytype = YYTRANSLATE (yychar);
      YYSIZE_T yysize0 = yytnamerr (0, yytname[yytype]);
      YYSIZE_T yysize = yysize0;
      YYSIZE_T yysize1;
      int yysize_overflow = 0;
      enum { YYERROR_VERBOSE_ARGS_MAXIMUM = 5 };
      char const *yyarg[YYERROR_VERBOSE_ARGS_MAXIMUM];
      int yyx;

# if 0
      /* This is so xgettext sees the translatable formats that are
	 constructed on the fly.  */
      YY_("syntax error, unexpected %s");
      YY_("syntax error, unexpected %s, expecting %s");
      YY_("syntax error, unexpected %s, expecting %s or %s");
      YY_("syntax error, unexpected %s, expecting %s or %s or %s");
      YY_("syntax error, unexpected %s, expecting %s or %s or %s or %s");
# endif
      char *yyfmt;
      char const *yyf;
      static char const yyunexpected[] = "syntax error, unexpected %s";
      static char const yyexpecting[] = ", expecting %s";
      static char const yyor[] = " or %s";
      char yyformat[sizeof yyunexpected
		    + sizeof yyexpecting - 1
		    + ((YYERROR_VERBOSE_ARGS_MAXIMUM - 2)
		       * (sizeof yyor - 1))];
      char const *yyprefix = yyexpecting;

      /* Start YYX at -YYN if negative to avoid negative indexes in
	 YYCHECK.  */
      int yyxbegin = yyn < 0 ? -yyn : 0;

      /* Stay within bounds of both yycheck and yytname.  */
      int yychecklim = YYLAST - yyn + 1;
      int yyxend = yychecklim < YYNTOKENS ? yychecklim : YYNTOKENS;
      int yycount = 1;

      yyarg[0] = yytname[yytype];
      yyfmt = yystpcpy (yyformat, yyunexpected);

      for (yyx = yyxbegin; yyx < yyxend; ++yyx)
	if (yycheck[yyx + yyn] == yyx && yyx != YYTERROR)
	  {
	    if (yycount == YYERROR_VERBOSE_ARGS_MAXIMUM)
	      {
		yycount = 1;
		yysize = yysize0;
		yyformat[sizeof yyunexpected - 1] = '\0';
		break;
	      }
	    yyarg[yycount++] = yytname[yyx];
	    yysize1 = yysize + yytnamerr (0, yytname[yyx]);
	    yysize_overflow |= (yysize1 < yysize);
	    yysize = yysize1;
	    yyfmt = yystpcpy (yyfmt, yyprefix);
	    yyprefix = yyor;
	  }

      yyf = YY_(yyformat);
      yysize1 = yysize + yystrlen (yyf);
      yysize_overflow |= (yysize1 < yysize);
      yysize = yysize1;

      if (yysize_overflow)
	return YYSIZE_MAXIMUM;

      if (yyresult)
	{
	  /* Avoid sprintf, as that infringes on the user's name space.
	     Don't have undefined behavior even if the translation
	     produced a string with the wrong number of "%s"s.  */
	  char *yyp = yyresult;
	  int yyi = 0;
	  while ((*yyp = *yyf) != '\0')
	    {
	      if (*yyp == '%' && yyf[1] == 's' && yyi < yycount)
		{
		  yyp += yytnamerr (yyp, yyarg[yyi++]);
		  yyf += 2;
		}
	      else
		{
		  yyp++;
		  yyf++;
		}
	    }
	}
      return yysize;
    }
}
#endif /* YYERROR_VERBOSE */


/*-----------------------------------------------.
| Release the memory associated to this symbol.  |
`-----------------------------------------------*/

/*ARGSUSED*/
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yydestruct (const char *yymsg, int yytype, YYSTYPE *yyvaluep)
#else
static void
yydestruct (yymsg, yytype, yyvaluep)
    const char *yymsg;
    int yytype;
    YYSTYPE *yyvaluep;
#endif
{
  YYUSE (yyvaluep);

  if (!yymsg)
    yymsg = "Deleting";
  YY_SYMBOL_PRINT (yymsg, yytype, yyvaluep, yylocationp);

  switch (yytype)
    {
      case 414: /* "shaderlist" */

/* Line 1000 of yacc.c  */
#line 457 "Grammar.ypp"
	{ if((yyvaluep->shaderList)) delete (yyvaluep->shaderList); };

/* Line 1000 of yacc.c  */
#line 4326 "D:/FromTiers/git/nvFX/FxParser/Grammar.cpp"
	break;
      case 415: /* "one_or_more_shaders" */

/* Line 1000 of yacc.c  */
#line 457 "Grammar.ypp"
	{ if((yyvaluep->shaderList)) delete (yyvaluep->shaderList); };

/* Line 1000 of yacc.c  */
#line 4335 "D:/FromTiers/git/nvFX/FxParser/Grammar.cpp"
	break;
      case 416: /* "varnamelist" */

/* Line 1000 of yacc.c  */
#line 456 "Grammar.ypp"
	{ if((yyvaluep->varNameList)) delete (yyvaluep->varNameList); };

/* Line 1000 of yacc.c  */
#line 4344 "D:/FromTiers/git/nvFX/FxParser/Grammar.cpp"
	break;
      case 419: /* "fexplist" */

/* Line 1000 of yacc.c  */
#line 458 "Grammar.ypp"
	{ if((yyvaluep->floatList)) delete (yyvaluep->floatList); };

/* Line 1000 of yacc.c  */
#line 4353 "D:/FromTiers/git/nvFX/FxParser/Grammar.cpp"
	break;
      case 420: /* "fexpveclist" */

/* Line 1000 of yacc.c  */
#line 459 "Grammar.ypp"
	{ if((yyvaluep->floatVecList)) delete (yyvaluep->floatVecList); };

/* Line 1000 of yacc.c  */
#line 4362 "D:/FromTiers/git/nvFX/FxParser/Grammar.cpp"
	break;
      case 421: /* "iexplist" */

/* Line 1000 of yacc.c  */
#line 460 "Grammar.ypp"
	{ if((yyvaluep->intList)) delete (yyvaluep->intList); };

/* Line 1000 of yacc.c  */
#line 4371 "D:/FromTiers/git/nvFX/FxParser/Grammar.cpp"
	break;
      case 423: /* "bexplist" */

/* Line 1000 of yacc.c  */
#line 461 "Grammar.ypp"
	{ if((yyvaluep->boolList)) delete (yyvaluep->boolList); };

/* Line 1000 of yacc.c  */
#line 4380 "D:/FromTiers/git/nvFX/FxParser/Grammar.cpp"
	break;
      case 448: /* "arglist" */

/* Line 1000 of yacc.c  */
#line 462 "Grammar.ypp"
	{ if((yyvaluep->argList)) delete (yyvaluep->argList); };

/* Line 1000 of yacc.c  */
#line 4389 "D:/FromTiers/git/nvFX/FxParser/Grammar.cpp"
	break;
      case 492: /* "valueassignment_f" */

/* Line 1000 of yacc.c  */
#line 459 "Grammar.ypp"
	{ if((yyvaluep->floatVecList)) delete (yyvaluep->floatVecList); };

/* Line 1000 of yacc.c  */
#line 4398 "D:/FromTiers/git/nvFX/FxParser/Grammar.cpp"
	break;
      case 493: /* "valueassignment_fv" */

/* Line 1000 of yacc.c  */
#line 459 "Grammar.ypp"
	{ if((yyvaluep->floatVecList)) delete (yyvaluep->floatVecList); };

/* Line 1000 of yacc.c  */
#line 4407 "D:/FromTiers/git/nvFX/FxParser/Grammar.cpp"
	break;
      case 494: /* "valueassignment_fvl" */

/* Line 1000 of yacc.c  */
#line 459 "Grammar.ypp"
	{ if((yyvaluep->floatVecList)) delete (yyvaluep->floatVecList); };

/* Line 1000 of yacc.c  */
#line 4416 "D:/FromTiers/git/nvFX/FxParser/Grammar.cpp"
	break;

      default:
	break;
    }
}

/* Prevent warnings from -Wmissing-prototypes.  */
#ifdef YYPARSE_PARAM
#if defined __STDC__ || defined __cplusplus
int yyparse (void *YYPARSE_PARAM);
#else
int yyparse ();
#endif
#else /* ! YYPARSE_PARAM */
#if defined __STDC__ || defined __cplusplus
int yyparse (void);
#else
int yyparse ();
#endif
#endif /* ! YYPARSE_PARAM */


/* The lookahead symbol.  */
int yychar;

/* The semantic value of the lookahead symbol.  */
YYSTYPE yylval;

/* Number of syntax errors so far.  */
int yynerrs;



/*-------------------------.
| yyparse or yypush_parse.  |
`-------------------------*/

#ifdef YYPARSE_PARAM
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
int
yyparse (void *YYPARSE_PARAM)
#else
int
yyparse (YYPARSE_PARAM)
    void *YYPARSE_PARAM;
#endif
#else /* ! YYPARSE_PARAM */
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
int
yyparse (void)
#else
int
yyparse ()

#endif
#endif
{


    int yystate;
    /* Number of tokens to shift before error messages enabled.  */
    int yyerrstatus;

    /* The stacks and their tools:
       `yyss': related to states.
       `yyvs': related to semantic values.

       Refer to the stacks thru separate pointers, to allow yyoverflow
       to reallocate them elsewhere.  */

    /* The state stack.  */
    yytype_int16 yyssa[YYINITDEPTH];
    yytype_int16 *yyss;
    yytype_int16 *yyssp;

    /* The semantic value stack.  */
    YYSTYPE yyvsa[YYINITDEPTH];
    YYSTYPE *yyvs;
    YYSTYPE *yyvsp;

    YYSIZE_T yystacksize;

  int yyn;
  int yyresult;
  /* Lookahead token as an internal (translated) token number.  */
  int yytoken;
  /* The variables used to return semantic value and location from the
     action routines.  */
  YYSTYPE yyval;

#if YYERROR_VERBOSE
  /* Buffer for error messages, and its allocated size.  */
  char yymsgbuf[128];
  char *yymsg = yymsgbuf;
  YYSIZE_T yymsg_alloc = sizeof yymsgbuf;
#endif

#define YYPOPSTACK(N)   (yyvsp -= (N), yyssp -= (N))

  /* The number of symbols on the RHS of the reduced rule.
     Keep to zero when no symbol should be popped.  */
  int yylen = 0;

  yytoken = 0;
  yyss = yyssa;
  yyvs = yyvsa;
  yystacksize = YYINITDEPTH;

  YYDPRINTF ((stderr, "Starting parse\n"));

  yystate = 0;
  yyerrstatus = 0;
  yynerrs = 0;
  yychar = YYEMPTY; /* Cause a token to be read.  */

  /* Initialize stack pointers.
     Waste one element of value and location stack
     so that they stay on the same level as the state stack.
     The wasted elements are never initialized.  */
  yyssp = yyss;
  yyvsp = yyvs;

/* User initialization code.  */

/* Line 1242 of yacc.c  */
#line 389 "Grammar.ypp"
{
    /*$$.container = Container::create(NULL);
    printf("My initial action %p\n", $$.container );*/
    line_num        = 1;
    bracket_level   = 0;
    //curContainer    = NULL; initialized in loadEffectFromFile()
    curTechnique    = NULL;
    curPass         = NULL;
    curShader       = NULL;
    curProgram      = NULL;
    curSamplerState = NULL;
    curRState       = NULL;
    curCSState      = NULL;
    curDSTState     = NULL;
    curPRState    = NULL;
    curResource     = NULL;
    curFBO          = NULL;
    curUniform      = NULL;
    curCstBuffer    = NULL;
    bCurCstBufferAlreadyPopulated = false;
    pShdRep         = nvFX::getShaderModuleRepositorySingleton()->getExInterface();
    pFboRep         = nvFX::getFrameBufferObjectsRepositorySingleton()->getExInterface();
    pResRep         = nvFX::getResourceRepositorySingleton()->getExInterface();
    pCRep           = nvFX::getCstBufferRepositorySingleton()->getExInterface();
    pURep           = nvFX::getUniformRepositorySingleton()->getExInterface();
}

/* Line 1242 of yacc.c  */
#line 4574 "D:/FromTiers/git/nvFX/FxParser/Grammar.cpp"

  goto yysetstate;

/*------------------------------------------------------------.
| yynewstate -- Push a new state, which is found in yystate.  |
`------------------------------------------------------------*/
 yynewstate:
  /* In all cases, when you get here, the value and location stacks
     have just been pushed.  So pushing a state here evens the stacks.  */
  yyssp++;

 yysetstate:
  *yyssp = yystate;

  if (yyss + yystacksize - 1 <= yyssp)
    {
      /* Get the current used size of the three stacks, in elements.  */
      YYSIZE_T yysize = yyssp - yyss + 1;

#ifdef yyoverflow
      {
	/* Give user a chance to reallocate the stack.  Use copies of
	   these so that the &'s don't force the real ones into
	   memory.  */
	YYSTYPE *yyvs1 = yyvs;
	yytype_int16 *yyss1 = yyss;

	/* Each stack pointer address is followed by the size of the
	   data in use in that stack, in bytes.  This used to be a
	   conditional around just the two extra args, but that might
	   be undefined if yyoverflow is a macro.  */
	yyoverflow (YY_("memory exhausted"),
		    &yyss1, yysize * sizeof (*yyssp),
		    &yyvs1, yysize * sizeof (*yyvsp),
		    &yystacksize);

	yyss = yyss1;
	yyvs = yyvs1;
      }
#else /* no yyoverflow */
# ifndef YYSTACK_RELOCATE
      goto yyexhaustedlab;
# else
      /* Extend the stack our own way.  */
      if (YYMAXDEPTH <= yystacksize)
	goto yyexhaustedlab;
      yystacksize *= 2;
      if (YYMAXDEPTH < yystacksize)
	yystacksize = YYMAXDEPTH;

      {
	yytype_int16 *yyss1 = yyss;
	union yyalloc *yyptr =
	  (union yyalloc *) YYSTACK_ALLOC (YYSTACK_BYTES (yystacksize));
	if (! yyptr)
	  goto yyexhaustedlab;
	YYSTACK_RELOCATE (yyss_alloc, yyss);
	YYSTACK_RELOCATE (yyvs_alloc, yyvs);
#  undef YYSTACK_RELOCATE
	if (yyss1 != yyssa)
	  YYSTACK_FREE (yyss1);
      }
# endif
#endif /* no yyoverflow */

      yyssp = yyss + yysize - 1;
      yyvsp = yyvs + yysize - 1;

      YYDPRINTF ((stderr, "Stack size increased to %lu\n",
		  (unsigned long int) yystacksize));

      if (yyss + yystacksize - 1 <= yyssp)
	YYABORT;
    }

  YYDPRINTF ((stderr, "Entering state %d\n", yystate));

  if (yystate == YYFINAL)
    YYACCEPT;

  goto yybackup;

/*-----------.
| yybackup.  |
`-----------*/
yybackup:

  /* Do appropriate processing given the current state.  Read a
     lookahead token if we need one and don't already have one.  */

  /* First try to decide what to do without reference to lookahead token.  */
  yyn = yypact[yystate];
  if (yyn == YYPACT_NINF)
    goto yydefault;

  /* Not known => get a lookahead token if don't already have one.  */

  /* YYCHAR is either YYEMPTY or YYEOF or a valid lookahead symbol.  */
  if (yychar == YYEMPTY)
    {
      YYDPRINTF ((stderr, "Reading a token: "));
      yychar = YYLEX;
    }

  if (yychar <= YYEOF)
    {
      yychar = yytoken = YYEOF;
      YYDPRINTF ((stderr, "Now at end of input.\n"));
    }
  else
    {
      yytoken = YYTRANSLATE (yychar);
      YY_SYMBOL_PRINT ("Next token is", yytoken, &yylval, &yylloc);
    }

  /* If the proper action on seeing token YYTOKEN is to reduce or to
     detect an error, take that action.  */
  yyn += yytoken;
  if (yyn < 0 || YYLAST < yyn || yycheck[yyn] != yytoken)
    goto yydefault;
  yyn = yytable[yyn];
  if (yyn <= 0)
    {
      if (yyn == 0 || yyn == YYTABLE_NINF)
	goto yyerrlab;
      yyn = -yyn;
      goto yyreduce;
    }

  /* Count tokens shifted since error; after three, turn off error
     status.  */
  if (yyerrstatus)
    yyerrstatus--;

  /* Shift the lookahead token.  */
  YY_SYMBOL_PRINT ("Shifting", yytoken, &yylval, &yylloc);

  /* Discard the shifted token.  */
  yychar = YYEMPTY;

  yystate = yyn;
  *++yyvsp = yylval;

  goto yynewstate;


/*-----------------------------------------------------------.
| yydefault -- do the default action for the current state.  |
`-----------------------------------------------------------*/
yydefault:
  yyn = yydefact[yystate];
  if (yyn == 0)
    goto yyerrlab;
  goto yyreduce;


/*-----------------------------.
| yyreduce -- Do a reduction.  |
`-----------------------------*/
yyreduce:
  /* yyn is the number of a rule to reduce with.  */
  yylen = yyr2[yyn];

  /* If YYLEN is nonzero, implement the default value of the action:
     `$$ = $1'.

     Otherwise, the following line sets YYVAL to garbage.
     This behavior is undocumented and Bison
     users should not rely upon it.  Assigning to YYVAL
     unconditionally makes the parser a bit smaller, and it avoids a
     GCC warning that YYVAL may be used uninitialized.  */
  yyval = yyvsp[1-yylen];


  YY_REDUCE_PRINT (yyn);
  switch (yyn)
    {
        case 29:

/* Line 1455 of yacc.c  */
#line 1053 "Grammar.ypp"
    {
    curContainer->separateShadersEnable((yyvsp[(4) - (4)].bval));
;}
    break;

  case 30:

/* Line 1455 of yacc.c  */
#line 1061 "Grammar.ypp"
    { (yyval.s) = (yyvsp[(1) - (1)].s); ;}
    break;

  case 31:

/* Line 1455 of yacc.c  */
#line 1062 "Grammar.ypp"
    { (yyval.s) = (yyvsp[(1) - (1)].s); ;}
    break;

  case 32:

/* Line 1455 of yacc.c  */
#line 1068 "Grammar.ypp"
    {  (yyval.fval)  =  (yyvsp[(1) - (1)].fval); ;}
    break;

  case 33:

/* Line 1455 of yacc.c  */
#line 1069 "Grammar.ypp"
    {  (yyval.fval)  =  (yyvsp[(1) - (3)].fval)  +  (yyvsp[(3) - (3)].fval); ;}
    break;

  case 34:

/* Line 1455 of yacc.c  */
#line 1070 "Grammar.ypp"
    {  (yyval.fval)  =  (float)(yyvsp[(1) - (3)].ival)  +  (yyvsp[(3) - (3)].fval); ;}
    break;

  case 35:

/* Line 1455 of yacc.c  */
#line 1071 "Grammar.ypp"
    {  (yyval.fval)  =  (yyvsp[(1) - (3)].fval)  +  (float)(yyvsp[(3) - (3)].ival); ;}
    break;

  case 36:

/* Line 1455 of yacc.c  */
#line 1072 "Grammar.ypp"
    {  (yyval.fval)  =  (yyvsp[(1) - (3)].fval)  -  (yyvsp[(3) - (3)].fval); ;}
    break;

  case 37:

/* Line 1455 of yacc.c  */
#line 1073 "Grammar.ypp"
    {  (yyval.fval)  =  (float)(yyvsp[(1) - (3)].ival)  -  (yyvsp[(3) - (3)].fval); ;}
    break;

  case 38:

/* Line 1455 of yacc.c  */
#line 1074 "Grammar.ypp"
    {  (yyval.fval)  =  (yyvsp[(1) - (3)].fval)  -  (float)(yyvsp[(3) - (3)].ival); ;}
    break;

  case 39:

/* Line 1455 of yacc.c  */
#line 1075 "Grammar.ypp"
    {  (yyval.fval)  =  (yyvsp[(1) - (3)].fval)  *  (yyvsp[(3) - (3)].fval); ;}
    break;

  case 40:

/* Line 1455 of yacc.c  */
#line 1076 "Grammar.ypp"
    {  (yyval.fval)  =  (float)(yyvsp[(1) - (3)].ival)  *  (yyvsp[(3) - (3)].fval); ;}
    break;

  case 41:

/* Line 1455 of yacc.c  */
#line 1077 "Grammar.ypp"
    {  (yyval.fval)  =  (yyvsp[(1) - (3)].fval)  *  (float)(yyvsp[(3) - (3)].ival); ;}
    break;

  case 42:

/* Line 1455 of yacc.c  */
#line 1078 "Grammar.ypp"
    {  (yyval.fval)  =  (yyvsp[(1) - (3)].fval)  /  (yyvsp[(3) - (3)].fval); ;}
    break;

  case 43:

/* Line 1455 of yacc.c  */
#line 1079 "Grammar.ypp"
    {  (yyval.fval)  =  (float)(yyvsp[(1) - (3)].ival)  /  (yyvsp[(3) - (3)].fval); ;}
    break;

  case 44:

/* Line 1455 of yacc.c  */
#line 1080 "Grammar.ypp"
    {  (yyval.fval)  =  (yyvsp[(1) - (3)].fval)  /  (float)(yyvsp[(3) - (3)].ival); ;}
    break;

  case 45:

/* Line 1455 of yacc.c  */
#line 1081 "Grammar.ypp"
    {  (yyval.fval)  =  (float)(yyvsp[(1) - (3)].ival)  /  (float)(yyvsp[(3) - (3)].ival); ;}
    break;

  case 46:

/* Line 1455 of yacc.c  */
#line 1082 "Grammar.ypp"
    {  (yyval.fval)  =  -(yyvsp[(2) - (2)].fval); ;}
    break;

  case 47:

/* Line 1455 of yacc.c  */
#line 1083 "Grammar.ypp"
    {  (yyval.fval)  =  pow  ((yyvsp[(1) - (3)].fval),  (yyvsp[(3) - (3)].fval)); ;}
    break;

  case 48:

/* Line 1455 of yacc.c  */
#line 1084 "Grammar.ypp"
    {  (yyval.fval)  =  pow  ((float)(yyvsp[(1) - (3)].ival),  (yyvsp[(3) - (3)].fval)); ;}
    break;

  case 49:

/* Line 1455 of yacc.c  */
#line 1085 "Grammar.ypp"
    {  (yyval.fval)  =  pow  ((yyvsp[(1) - (3)].fval),  (float)(yyvsp[(3) - (3)].ival)); ;}
    break;

  case 50:

/* Line 1455 of yacc.c  */
#line 1086 "Grammar.ypp"
    {  (yyval.fval)  =  pow  ((float)(yyvsp[(1) - (3)].ival),  (float)(yyvsp[(3) - (3)].ival)); ;}
    break;

  case 51:

/* Line 1455 of yacc.c  */
#line 1087 "Grammar.ypp"
    {  (yyval.fval)  =  (yyvsp[(2) - (3)].fval); ;}
    break;

  case 52:

/* Line 1455 of yacc.c  */
#line 1095 "Grammar.ypp"
    {  (yyval.ival)  =  (yyvsp[(1) - (1)].ival); ;}
    break;

  case 53:

/* Line 1455 of yacc.c  */
#line 1096 "Grammar.ypp"
    {  (yyval.ival)  =  (yyvsp[(1) - (3)].ival)  +  (yyvsp[(3) - (3)].ival); ;}
    break;

  case 54:

/* Line 1455 of yacc.c  */
#line 1097 "Grammar.ypp"
    {  (yyval.ival)  =  (yyvsp[(1) - (3)].ival)  -  (yyvsp[(3) - (3)].ival); ;}
    break;

  case 55:

/* Line 1455 of yacc.c  */
#line 1098 "Grammar.ypp"
    {  (yyval.ival)  =  (yyvsp[(1) - (3)].ival)  *  (yyvsp[(3) - (3)].ival); ;}
    break;

  case 56:

/* Line 1455 of yacc.c  */
#line 1099 "Grammar.ypp"
    {  (yyval.ival)  =  -(yyvsp[(2) - (2)].ival); ;}
    break;

  case 57:

/* Line 1455 of yacc.c  */
#line 1100 "Grammar.ypp"
    {  (yyval.ival)  =  (yyvsp[(2) - (3)].ival); ;}
    break;

  case 58:

/* Line 1455 of yacc.c  */
#line 1105 "Grammar.ypp"
    {  (yyval.bval)  =  true; ;}
    break;

  case 59:

/* Line 1455 of yacc.c  */
#line 1106 "Grammar.ypp"
    {  (yyval.bval)  =  false; ;}
    break;

  case 60:

/* Line 1455 of yacc.c  */
#line 1112 "Grammar.ypp"
    { (yyval.bval) = (yyvsp[(1) - (1)].bval); ;}
    break;

  case 61:

/* Line 1455 of yacc.c  */
#line 1113 "Grammar.ypp"
    { 
    if(((yyvsp[(1) - (1)].ival) != 0)&&((yyvsp[(1) - (1)].ival) != 1)) yyerror("    Warning : truncation of float to bool");
    (yyval.bval) = (yyvsp[(1) - (1)].ival) ? true: false; ;}
    break;

  case 62:

/* Line 1455 of yacc.c  */
#line 1116 "Grammar.ypp"
    { 
    if(((yyvsp[(1) - (1)].fval) != 0.0f)&&((yyvsp[(1) - (1)].fval) != 1.0f)) yyerror("    Warning : truncation of float to bool");
    (yyval.bval) = (yyvsp[(1) - (1)].fval) ? true: false; ;}
    break;

  case 63:

/* Line 1455 of yacc.c  */
#line 1120 "Grammar.ypp"
    { (yyval.ival) = (yyvsp[(1) - (1)].ival); ;}
    break;

  case 64:

/* Line 1455 of yacc.c  */
#line 1121 "Grammar.ypp"
    { 
    if(((yyvsp[(1) - (1)].fval) - (int)(yyvsp[(1) - (1)].fval))!=0.0f)
        yyerror("    Warning : truncation of float to int");
    (yyval.ival) = (int)(yyvsp[(1) - (1)].fval); ;}
    break;

  case 65:

/* Line 1455 of yacc.c  */
#line 1125 "Grammar.ypp"
    { (yyval.ival) = (yyvsp[(1) - (1)].bval) ? 1: 0; ;}
    break;

  case 66:

/* Line 1455 of yacc.c  */
#line 1128 "Grammar.ypp"
    { (yyval.fval) = (yyvsp[(1) - (1)].fval); ;}
    break;

  case 67:

/* Line 1455 of yacc.c  */
#line 1129 "Grammar.ypp"
    { (yyval.fval) = (float)(yyvsp[(1) - (1)].ival); ;}
    break;

  case 68:

/* Line 1455 of yacc.c  */
#line 1130 "Grammar.ypp"
    { (yyval.fval) = (yyvsp[(1) - (1)].bval) ? 1.0f: 0.0f; ;}
    break;

  case 71:

/* Line 1455 of yacc.c  */
#line 1155 "Grammar.ypp"
    { (yyval.shaderList) = NULL; ;}
    break;

  case 72:

/* Line 1455 of yacc.c  */
#line 1156 "Grammar.ypp"
    {
    (yyval.shaderList) = new ShaderList;
    IShader *pShader = curContainer->findShader((yyvsp[(1) - (1)].s)->c_str());
    if(pShader == NULL) {
        // second chance: look for it in the global repository
        pShader = pShdRep->findShader((yyvsp[(1) - (1)].s)->c_str());
        if(pShader == NULL) {
            yyerror("Error : Shader not found:");
            yyerror((yyvsp[(1) - (1)].s)->c_str());
        }
    }
    if(pShader)
        ((yyval.shaderList))->push_back(pShader);
    delete (yyvsp[(1) - (1)].s);
;}
    break;

  case 73:

/* Line 1455 of yacc.c  */
#line 1171 "Grammar.ypp"
    {
    (yyval.shaderList) = (yyvsp[(1) - (3)].shaderList);
    IShader *pShader = curContainer->findShader((yyvsp[(3) - (3)].s)->c_str());
    if(pShader == NULL) {
        // second chance: look for it in the global repository
        pShader = pShdRep->findShader((yyvsp[(3) - (3)].s)->c_str());
        if(pShader == NULL) {
            yyerror("Error : Shader not found:");
            yyerror((yyvsp[(3) - (3)].s)->c_str());
        }
    }
    if(pShader)
        ((yyval.shaderList))->push_back(pShader);
    delete (yyvsp[(3) - (3)].s);
;}
    break;

  case 74:

/* Line 1455 of yacc.c  */
#line 1188 "Grammar.ypp"
    { (yyval.shaderList) = NULL; ;}
    break;

  case 75:

/* Line 1455 of yacc.c  */
#line 1204 "Grammar.ypp"
    { (yyval.shaderList) = (yyvsp[(1) - (1)].shaderList); ;}
    break;

  case 76:

/* Line 1455 of yacc.c  */
#line 1205 "Grammar.ypp"
    { (yyval.shaderList) = (yyvsp[(2) - (3)].shaderList); ;}
    break;

  case 77:

/* Line 1455 of yacc.c  */
#line 1212 "Grammar.ypp"
    { (yyval.varNameList) = NULL; ;}
    break;

  case 78:

/* Line 1455 of yacc.c  */
#line 1213 "Grammar.ypp"
    {
    (yyval.varNameList) = new VarNameList;
    (yyval.varNameList)->push_back(*(yyvsp[(1) - (1)].s));
    delete (yyvsp[(1) - (1)].s);
;}
    break;

  case 79:

/* Line 1455 of yacc.c  */
#line 1218 "Grammar.ypp"
    {
    (yyval.varNameList) = (yyvsp[(1) - (3)].varNameList);
    (yyval.varNameList)->push_back(*(yyvsp[(3) - (3)].s));
    delete (yyvsp[(3) - (3)].s);
;}
    break;

  case 82:

/* Line 1455 of yacc.c  */
#line 1243 "Grammar.ypp"
    {
    (yyval.floatList) = new FloatList;
    (yyval.floatList)->push_back((yyvsp[(1) - (1)].fval));
;}
    break;

  case 83:

/* Line 1455 of yacc.c  */
#line 1247 "Grammar.ypp"
    {
    (yyval.floatList) = new FloatList;
    (yyval.floatList)->push_back((float)(yyvsp[(1) - (1)].ival));
;}
    break;

  case 84:

/* Line 1455 of yacc.c  */
#line 1251 "Grammar.ypp"
    {
    (yyval.floatList) = (yyvsp[(1) - (3)].floatList);
    (yyval.floatList)->push_back((yyvsp[(3) - (3)].fval));
;}
    break;

  case 85:

/* Line 1455 of yacc.c  */
#line 1255 "Grammar.ypp"
    {
    (yyval.floatList) = (yyvsp[(1) - (3)].floatList);
    (yyval.floatList)->push_back((float)(yyvsp[(3) - (3)].ival));
;}
    break;

  case 86:

/* Line 1455 of yacc.c  */
#line 1259 "Grammar.ypp"
    { 
    (yyval.floatList) = (yyvsp[(1) - (2)].floatList);
    yyerror ("Error in the list of floats");
;}
    break;

  case 87:

/* Line 1455 of yacc.c  */
#line 1265 "Grammar.ypp"
    {
    (yyval.floatVecList) = new FloatVecList;
    (yyval.floatVecList)->push_back((yyvsp[(2) - (3)].floatList));
;}
    break;

  case 88:

/* Line 1455 of yacc.c  */
#line 1269 "Grammar.ypp"
    {
    (yyval.floatVecList) = (yyvsp[(1) - (5)].floatVecList);
    (yyval.floatVecList)->push_back((yyvsp[(4) - (5)].floatList));
;}
    break;

  case 89:

/* Line 1455 of yacc.c  */
#line 1273 "Grammar.ypp"
    { (yyval.floatVecList) = (yyvsp[(1) - (2)].floatVecList); yyerror ("Error in the list of default values"); ;}
    break;

  case 90:

/* Line 1455 of yacc.c  */
#line 1276 "Grammar.ypp"
    {
    (yyval.intList) = new IntList;
    (yyval.intList)->push_back((yyvsp[(1) - (1)].ival));
;}
    break;

  case 91:

/* Line 1455 of yacc.c  */
#line 1285 "Grammar.ypp"
    {
    (yyval.intList) = (yyvsp[(1) - (3)].intList);
    (yyval.intList)->push_back((yyvsp[(3) - (3)].ival));
;}
    break;

  case 92:

/* Line 1455 of yacc.c  */
#line 1296 "Grammar.ypp"
    {
    (yyval.intVecList) = new IntVecList;
    (yyval.intVecList)->push_back((yyvsp[(2) - (3)].intList));
;}
    break;

  case 93:

/* Line 1455 of yacc.c  */
#line 1300 "Grammar.ypp"
    {
    (yyval.intVecList) = (yyvsp[(1) - (5)].intVecList);
    (yyval.intVecList)->push_back((yyvsp[(4) - (5)].intList));
;}
    break;

  case 94:

/* Line 1455 of yacc.c  */
#line 1304 "Grammar.ypp"
    { yyerror ("Error in the list of default values"); ;}
    break;

  case 95:

/* Line 1455 of yacc.c  */
#line 1307 "Grammar.ypp"
    {
    (yyval.boolList) = new BoolList;
    (yyval.boolList)->push_back((yyvsp[(1) - (1)].bval));
;}
    break;

  case 96:

/* Line 1455 of yacc.c  */
#line 1311 "Grammar.ypp"
    {
    (yyval.boolList) = (yyvsp[(1) - (3)].boolList);
    (yyval.boolList)->push_back((yyvsp[(3) - (3)].bval));
;}
    break;

  case 97:

/* Line 1455 of yacc.c  */
#line 1331 "Grammar.ypp"
    { 
    (yyval.argVal) = new ArgVal(IUniform::TInt); (yyval.argVal)->val.i[0] = (yyvsp[(1) - (1)].ival); 
    LOGD("Arg val = %d\n", (yyvsp[(1) - (1)].ival));
;}
    break;

  case 98:

/* Line 1455 of yacc.c  */
#line 1335 "Grammar.ypp"
    { 
    (yyval.argVal) = new ArgVal(IUniform::TFloat); (yyval.argVal)->val.f[0] = (yyvsp[(1) - (1)].fval);
    LOGD("Arg val = %f\n", (yyvsp[(1) - (1)].fval));
;}
    break;

  case 99:

/* Line 1455 of yacc.c  */
#line 1339 "Grammar.ypp"
    {
    IResource* pRes = curContainer->findResource((yyvsp[(1) - (1)].s)->c_str());
    if(pRes) {
        nvFX::IUniform::Type utype;
        switch(pRes->getType())
        {
        case RESOURCE_UNKNOWN: utype = IUniform::TUndefined; break;
        case RESTEX_1D: utype = IUniform::TTexture1D; break;
        case RESTEX_2D: utype = IUniform::TTexture2D; break;
        case RESTEX_2DRECT: utype = IUniform::TTexture2DRect; break;
        case RESTEX_3D: utype = IUniform::TTexture3D; break;
        case RESTEX_CUBE_MAP: utype = IUniform::TTextureCube; break;
        default:
            assert(!"missing convertion. TODO or to CHECK");
        }
        (yyval.argVal) = new ArgVal(utype); (yyval.argVal)->val.pResource = pRes;
        LOGD("Arg val = %s as a resource pointer\n", (yyvsp[(1) - (1)].s)->c_str());
    } else {
        IUniform* pU = curContainer->findUniform((yyvsp[(1) - (1)].s)->c_str());
        if(pU) {
            (yyval.argVal) = new ArgVal(IUniform::TUniform); (yyval.argVal)->val.pUniform = pU;
            LOGD("Arg val = %s from a uniform pointer\n", (yyvsp[(1) - (1)].s)->c_str());
        } else {
            LOGD("Arg val = %s - unknown parameter !\n", (yyvsp[(1) - (1)].s)->c_str());
            yyerror("Unknown parameter\n"); yyerror((yyvsp[(1) - (1)].s)->c_str()); yyerror("\n");
        }
    }
    delete (yyvsp[(1) - (1)].s);
;}
    break;

  case 100:

/* Line 1455 of yacc.c  */
#line 1368 "Grammar.ypp"
    { 
    yyerror("Error in argument\n");
;}
    break;

  case 101:

/* Line 1455 of yacc.c  */
#line 1373 "Grammar.ypp"
    { (yyval.argValList) = new ArgValList; ;}
    break;

  case 102:

/* Line 1455 of yacc.c  */
#line 1374 "Grammar.ypp"
    { (yyval.argValList) = new ArgValList; ;}
    break;

  case 103:

/* Line 1455 of yacc.c  */
#line 1375 "Grammar.ypp"
    {
    (yyval.argValList) = new ArgValList;
    (yyval.argValList)->push_back((yyvsp[(1) - (1)].argVal));
    ;}
    break;

  case 104:

/* Line 1455 of yacc.c  */
#line 1379 "Grammar.ypp"
    {
    (yyval.argValList) = (yyvsp[(1) - (3)].argValList);
    (yyval.argValList)->push_back((yyvsp[(3) - (3)].argVal));
    ;}
    break;

  case 105:

/* Line 1455 of yacc.c  */
#line 1383 "Grammar.ypp"
    { yyerror("Error in argument list\n");;}
    break;

  case 207:

/* Line 1455 of yacc.c  */
#line 1505 "Grammar.ypp"
    { (yyval.clear_mode) = (yyvsp[(1) - (1)].clear_mode); ;}
    break;

  case 208:

/* Line 1455 of yacc.c  */
#line 1506 "Grammar.ypp"
    { (yyval.clear_mode) = (ClearMode)((unsigned int)(yyvsp[(3) - (3)].clear_mode) | (unsigned int)(yyvsp[(1) - (3)].clear_mode)); ;}
    break;

  case 212:

/* Line 1455 of yacc.c  */
#line 1516 "Grammar.ypp"
    {
    //if($4->size() > 0)
    //    curPass->createState(IPassState::TVertexShader, & (*$4)[0], (int)$4->size());
    delete (yyvsp[(4) - (5)].s);
    ;}
    break;

  case 213:

/* Line 1455 of yacc.c  */
#line 1521 "Grammar.ypp"
    {
    //if($4->size() > 0)
    //    curPass->createState(IPassState::TVertexShader, & (*$4)[0], (int)$4->size());
    ;}
    break;

  case 214:

/* Line 1455 of yacc.c  */
#line 1525 "Grammar.ypp"
    {
    if((yyvsp[(4) - (5)].shaderList)->size() > 0)
        curPass->createState(IPassState::TVertexShader, & (*(yyvsp[(4) - (5)].shaderList))[0], (int)(yyvsp[(4) - (5)].shaderList)->size());
    delete (yyvsp[(4) - (5)].shaderList);
    ;}
    break;

  case 215:

/* Line 1455 of yacc.c  */
#line 1530 "Grammar.ypp"
    {
    if((yyvsp[(7) - (8)].shaderList)->size() > 0)
        curPass->createState(IPassState::TVertexShader, & (*(yyvsp[(7) - (8)].shaderList))[0], (int)(yyvsp[(7) - (8)].shaderList)->size(), (yyvsp[(4) - (8)].s)->c_str());
    delete (yyvsp[(4) - (8)].s);
    delete (yyvsp[(7) - (8)].shaderList);
    ;}
    break;

  case 216:

/* Line 1455 of yacc.c  */
#line 1536 "Grammar.ypp"
    {
    if((yyvsp[(4) - (5)].shaderList)->size()>0)
        curPass->createState(IPassState::TFragmentShader, & (*(yyvsp[(4) - (5)].shaderList))[0], (int)(yyvsp[(4) - (5)].shaderList)->size());
    delete (yyvsp[(4) - (5)].shaderList);
    ;}
    break;

  case 217:

/* Line 1455 of yacc.c  */
#line 1541 "Grammar.ypp"
    {
    if((yyvsp[(7) - (8)].shaderList)->size()>0)
        curPass->createState(IPassState::TFragmentShader, & (*(yyvsp[(7) - (8)].shaderList))[0], (int)(yyvsp[(7) - (8)].shaderList)->size(), (yyvsp[(4) - (8)].s)->c_str());
    delete (yyvsp[(4) - (8)].s);
    delete (yyvsp[(7) - (8)].shaderList);
    ;}
    break;

  case 218:

/* Line 1455 of yacc.c  */
#line 1547 "Grammar.ypp"
    {
    if((yyvsp[(4) - (5)].shaderList)->size() > 0)
        curPass->createState(IPassState::TComputeShader, & (*(yyvsp[(4) - (5)].shaderList))[0], (int)(yyvsp[(4) - (5)].shaderList)->size());
    delete (yyvsp[(4) - (5)].shaderList);
    ;}
    break;

  case 219:

/* Line 1455 of yacc.c  */
#line 1552 "Grammar.ypp"
    {
    if((yyvsp[(7) - (8)].shaderList)->size() > 0)
        curPass->createState(IPassState::TComputeShader, & (*(yyvsp[(7) - (8)].shaderList))[0], (int)(yyvsp[(7) - (8)].shaderList)->size(), (yyvsp[(4) - (8)].s)->c_str());
    delete (yyvsp[(4) - (8)].s);
    delete (yyvsp[(7) - (8)].shaderList);
    ;}
    break;

  case 220:

/* Line 1455 of yacc.c  */
#line 1558 "Grammar.ypp"
    {
    if((yyvsp[(4) - (5)].shaderList)->size() > 0)
        curPass->createState(IPassState::TPathShader, & (*(yyvsp[(4) - (5)].shaderList))[0], (int)(yyvsp[(4) - (5)].shaderList)->size());
    delete (yyvsp[(4) - (5)].shaderList);
    ;}
    break;

  case 221:

/* Line 1455 of yacc.c  */
#line 1563 "Grammar.ypp"
    {
    if((yyvsp[(7) - (8)].shaderList)->size() > 0)
        curPass->createState(IPassState::TPathShader, & (*(yyvsp[(7) - (8)].shaderList))[0], (int)(yyvsp[(7) - (8)].shaderList)->size(), (yyvsp[(4) - (8)].s)->c_str());
    delete (yyvsp[(4) - (8)].s);
    delete (yyvsp[(7) - (8)].shaderList);
    ;}
    break;

  case 222:

/* Line 1455 of yacc.c  */
#line 1569 "Grammar.ypp"
    {
    if((yyvsp[(4) - (5)].shaderList)->size() > 0)
        curPass->createState(IPassState::TGeometryShader, & (*(yyvsp[(4) - (5)].shaderList))[0], (int)(yyvsp[(4) - (5)].shaderList)->size());
    delete (yyvsp[(4) - (5)].shaderList);
    ;}
    break;

  case 223:

/* Line 1455 of yacc.c  */
#line 1574 "Grammar.ypp"
    {
    if((yyvsp[(7) - (8)].shaderList)->size() > 0)
        curPass->createState(IPassState::TGeometryShader, & (*(yyvsp[(7) - (8)].shaderList))[0], (int)(yyvsp[(7) - (8)].shaderList)->size(), (yyvsp[(4) - (8)].s)->c_str());
    delete (yyvsp[(4) - (8)].s);
    delete (yyvsp[(7) - (8)].shaderList);
    ;}
    break;

  case 224:

/* Line 1455 of yacc.c  */
#line 1580 "Grammar.ypp"
    {
    if((yyvsp[(4) - (5)].shaderList)->size() > 0)
        curPass->createState(IPassState::TTCSShader, & (*(yyvsp[(4) - (5)].shaderList))[0], (int)(yyvsp[(4) - (5)].shaderList)->size());
    delete (yyvsp[(4) - (5)].shaderList);
    ;}
    break;

  case 225:

/* Line 1455 of yacc.c  */
#line 1585 "Grammar.ypp"
    {
    if((yyvsp[(7) - (8)].shaderList)->size() > 0)
        curPass->createState(IPassState::TTCSShader, & (*(yyvsp[(7) - (8)].shaderList))[0], (int)(yyvsp[(7) - (8)].shaderList)->size(), (yyvsp[(4) - (8)].s)->c_str());
    delete (yyvsp[(4) - (8)].s);
    delete (yyvsp[(7) - (8)].shaderList);
    ;}
    break;

  case 226:

/* Line 1455 of yacc.c  */
#line 1591 "Grammar.ypp"
    {
    if((yyvsp[(4) - (5)].shaderList)->size() > 0)
        curPass->createState(IPassState::TTESShader, & (*(yyvsp[(4) - (5)].shaderList))[0], (int)(yyvsp[(4) - (5)].shaderList)->size());
    delete (yyvsp[(4) - (5)].shaderList);
    ;}
    break;

  case 227:

/* Line 1455 of yacc.c  */
#line 1596 "Grammar.ypp"
    {
    if((yyvsp[(7) - (8)].shaderList)->size() > 0)
        curPass->createState(IPassState::TTESShader, & (*(yyvsp[(7) - (8)].shaderList))[0], (int)(yyvsp[(7) - (8)].shaderList)->size(), (yyvsp[(4) - (8)].s)->c_str());
    delete (yyvsp[(4) - (8)].s);
    delete (yyvsp[(7) - (8)].shaderList);
    ;}
    break;

  case 228:

/* Line 1455 of yacc.c  */
#line 1602 "Grammar.ypp"
    {
    if((yyvsp[(4) - (5)].shaderList)->size() > 0)
        curPass->createStateOverride(IPassState::TVertexShader, & (*(yyvsp[(4) - (5)].shaderList))[0], (int)(yyvsp[(4) - (5)].shaderList)->size());
    delete (yyvsp[(4) - (5)].shaderList);
    ;}
    break;

  case 229:

/* Line 1455 of yacc.c  */
#line 1607 "Grammar.ypp"
    {
    if((yyvsp[(7) - (8)].shaderList)->size() > 0)
        curPass->createStateOverride(IPassState::TVertexShader, & (*(yyvsp[(7) - (8)].shaderList))[0], (int)(yyvsp[(7) - (8)].shaderList)->size(), (yyvsp[(4) - (8)].s)->c_str());
    delete (yyvsp[(4) - (8)].s);
    delete (yyvsp[(7) - (8)].shaderList);
    ;}
    break;

  case 230:

/* Line 1455 of yacc.c  */
#line 1613 "Grammar.ypp"
    {
    if((yyvsp[(4) - (5)].shaderList)->size()>0)
        curPass->createStateOverride(IPassState::TFragmentShader, & (*(yyvsp[(4) - (5)].shaderList))[0], (int)(yyvsp[(4) - (5)].shaderList)->size());
    delete (yyvsp[(4) - (5)].shaderList);
    ;}
    break;

  case 231:

/* Line 1455 of yacc.c  */
#line 1618 "Grammar.ypp"
    {
    if((yyvsp[(7) - (8)].shaderList)->size()>0)
        curPass->createStateOverride(IPassState::TFragmentShader, & (*(yyvsp[(7) - (8)].shaderList))[0], (int)(yyvsp[(7) - (8)].shaderList)->size(), (yyvsp[(4) - (8)].s)->c_str());
    delete (yyvsp[(4) - (8)].s);
    delete (yyvsp[(7) - (8)].shaderList);
    ;}
    break;

  case 232:

/* Line 1455 of yacc.c  */
#line 1624 "Grammar.ypp"
    {
    if((yyvsp[(4) - (5)].shaderList)->size() > 0)
        curPass->createStateOverride(IPassState::TComputeShader, & (*(yyvsp[(4) - (5)].shaderList))[0], (int)(yyvsp[(4) - (5)].shaderList)->size());
    delete (yyvsp[(4) - (5)].shaderList);
    ;}
    break;

  case 233:

/* Line 1455 of yacc.c  */
#line 1629 "Grammar.ypp"
    {
    if((yyvsp[(7) - (8)].shaderList)->size() > 0)
        curPass->createStateOverride(IPassState::TComputeShader, & (*(yyvsp[(7) - (8)].shaderList))[0], (int)(yyvsp[(7) - (8)].shaderList)->size(), (yyvsp[(4) - (8)].s)->c_str());
    delete (yyvsp[(4) - (8)].s);
    delete (yyvsp[(7) - (8)].shaderList);
    ;}
    break;

  case 234:

/* Line 1455 of yacc.c  */
#line 1635 "Grammar.ypp"
    {
    if((yyvsp[(4) - (5)].shaderList)->size() > 0)
        curPass->createStateOverride(IPassState::TPathShader, & (*(yyvsp[(4) - (5)].shaderList))[0], (int)(yyvsp[(4) - (5)].shaderList)->size());
    delete (yyvsp[(4) - (5)].shaderList);
    ;}
    break;

  case 235:

/* Line 1455 of yacc.c  */
#line 1640 "Grammar.ypp"
    {
    if((yyvsp[(7) - (8)].shaderList)->size() > 0)
        curPass->createStateOverride(IPassState::TPathShader, & (*(yyvsp[(7) - (8)].shaderList))[0], (int)(yyvsp[(7) - (8)].shaderList)->size(), (yyvsp[(4) - (8)].s)->c_str());
    delete (yyvsp[(4) - (8)].s);
    delete (yyvsp[(7) - (8)].shaderList);
    ;}
    break;

  case 236:

/* Line 1455 of yacc.c  */
#line 1646 "Grammar.ypp"
    {
    if((yyvsp[(4) - (5)].shaderList)->size() > 0)
        curPass->createStateOverride(IPassState::TGeometryShader, & (*(yyvsp[(4) - (5)].shaderList))[0], (int)(yyvsp[(4) - (5)].shaderList)->size());
    delete (yyvsp[(4) - (5)].shaderList);
    ;}
    break;

  case 237:

/* Line 1455 of yacc.c  */
#line 1651 "Grammar.ypp"
    {
    if((yyvsp[(7) - (8)].shaderList)->size() > 0)
        curPass->createStateOverride(IPassState::TGeometryShader, & (*(yyvsp[(7) - (8)].shaderList))[0], (int)(yyvsp[(7) - (8)].shaderList)->size(), (yyvsp[(4) - (8)].s)->c_str());
    delete (yyvsp[(4) - (8)].s);
    delete (yyvsp[(7) - (8)].shaderList);
    ;}
    break;

  case 238:

/* Line 1455 of yacc.c  */
#line 1657 "Grammar.ypp"
    {
    if((yyvsp[(4) - (5)].shaderList)->size() > 0)
        curPass->createStateOverride(IPassState::TTCSShader, & (*(yyvsp[(4) - (5)].shaderList))[0], (int)(yyvsp[(4) - (5)].shaderList)->size());
    delete (yyvsp[(4) - (5)].shaderList);
    ;}
    break;

  case 239:

/* Line 1455 of yacc.c  */
#line 1662 "Grammar.ypp"
    {
    if((yyvsp[(7) - (8)].shaderList)->size() > 0)
        curPass->createStateOverride(IPassState::TTCSShader, & (*(yyvsp[(7) - (8)].shaderList))[0], (int)(yyvsp[(7) - (8)].shaderList)->size(), (yyvsp[(4) - (8)].s)->c_str());
    delete (yyvsp[(4) - (8)].s);
    delete (yyvsp[(7) - (8)].shaderList);
    ;}
    break;

  case 240:

/* Line 1455 of yacc.c  */
#line 1668 "Grammar.ypp"
    {
    if((yyvsp[(4) - (5)].shaderList)->size() > 0)
        curPass->createStateOverride(IPassState::TTESShader, & (*(yyvsp[(4) - (5)].shaderList))[0], (int)(yyvsp[(4) - (5)].shaderList)->size());
    delete (yyvsp[(4) - (5)].shaderList);
    ;}
    break;

  case 241:

/* Line 1455 of yacc.c  */
#line 1673 "Grammar.ypp"
    {
    if((yyvsp[(7) - (8)].shaderList)->size() > 0)
        curPass->createStateOverride(IPassState::TTESShader, & (*(yyvsp[(7) - (8)].shaderList))[0], (int)(yyvsp[(7) - (8)].shaderList)->size(), (yyvsp[(4) - (8)].s)->c_str());
    delete (yyvsp[(4) - (8)].s);
    delete (yyvsp[(7) - (8)].shaderList);
    ;}
    break;

  case 242:

/* Line 1455 of yacc.c  */
#line 1679 "Grammar.ypp"
    {
    #ifndef OGLES2
    // we prefer this loop : we don't have the char ** available here... using vector of std::string...
    int sz = (int)(yyvsp[(8) - (10)].varNameList)->size();
    const char ** srnames = (const char **)malloc(sizeof(const char *)*sz);
    for(int i=0; i<(int)(yyvsp[(8) - (10)].varNameList)->size(); i++)
        srnames[i] = (*(yyvsp[(8) - (10)].varNameList))[i].c_str();
    curPass->createState((yyvsp[(4) - (10)].s)->c_str(), srnames, sz);
    delete [] srnames;
    #else
        yyerror("Not supported in ES2\n");
    #endif
    delete (yyvsp[(4) - (10)].s);
    delete (yyvsp[(8) - (10)].varNameList);
    ;}
    break;

  case 243:

/* Line 1455 of yacc.c  */
#line 1694 "Grammar.ypp"
    {
    #ifndef OGLES2
    curPass->createState((yyvsp[(4) - (8)].s)->c_str(), 0, (yyvsp[(7) - (8)].s)->c_str());
    #else
        yyerror("Not supported in ES2\n");
    #endif
    delete (yyvsp[(4) - (8)].s);
    delete (yyvsp[(7) - (8)].s);
    ;}
    break;

  case 244:

/* Line 1455 of yacc.c  */
#line 1703 "Grammar.ypp"
    {
    ValueList* pvalueList = (yyvsp[(6) - (7)].valueList);
    if(pvalueList && pvalueList->floatVecList)
    {
        FloatVecList *pfvl = pvalueList->floatVecList;
        FloatList* pfl = (*pfvl)[0];
        float* pf = &(*pfl)[0];
        curPass->createState((yyvsp[(4) - (7)].s)->c_str(), 0, pf, (int)pfl->size());
        delete pfl;
        delete pfvl;
        delete pvalueList;
    }
    delete (yyvsp[(4) - (7)].s);
    ;}
    break;

  case 245:

/* Line 1455 of yacc.c  */
#line 1760 "Grammar.ypp"
    {
    #ifndef OGLES2
    curPass->createState((yyvsp[(4) - (11)].s)->c_str(), (yyvsp[(6) - (11)].ival), (yyvsp[(10) - (11)].s)->c_str());
    #else
        yyerror("Not supported in ES2\n");
    #endif
    delete (yyvsp[(4) - (11)].s);
    delete (yyvsp[(10) - (11)].s);
    ;}
    break;

  case 246:

/* Line 1455 of yacc.c  */
#line 1779 "Grammar.ypp"
    {
    // walk through the vector list and check few things then send the packed version
    int szvec = 0;
    float *farray = NULL;
    float *p = NULL;
    for(int i=0; i<(int)(yyvsp[(11) - (13)].floatVecList)->size(); i++)
    {
        if(szvec == 0) {
            szvec = (int)(*(yyvsp[(11) - (13)].floatVecList))[i]->size();
            p = farray = new float[szvec * (yyvsp[(11) - (13)].floatVecList)->size()]; // we assume all vectors are the same size
        }
        else if(szvec != (*(yyvsp[(11) - (13)].floatVecList))[i]->size()) {
            yyerror("Vector list has inconsistent vectors\n");
            continue;
        }
        FloatList* pfl = (*(yyvsp[(11) - (13)].floatVecList))[i];
        for(int j=0; j<(int)pfl->size(); j++)
            *p++ = (*pfl)[j];
        delete pfl;
    }
    if(szvec == 0)
        yyerror("Error : vectors are 0 sized\n");
    else
        curPass->createState((yyvsp[(4) - (13)].s)->c_str(), (yyvsp[(6) - (13)].ival), farray, szvec, (int)(yyvsp[(11) - (13)].floatVecList)->size());
    delete farray;
    delete (yyvsp[(4) - (13)].s);
    delete (yyvsp[(11) - (13)].floatVecList);
    ;}
    break;

  case 247:

/* Line 1455 of yacc.c  */
#line 1807 "Grammar.ypp"
    {
    IntVecList *pvl = (yyvsp[(10) - (11)].intVecList);
    IntList* pl = (*pvl)[0];
    int* p = &(*pl)[0];
    curPass->createState((yyvsp[(4) - (11)].s)->c_str(), (yyvsp[(6) - (11)].ival), p, (int)pl->size());
    delete (yyvsp[(4) - (11)].s);
    delete pl;
    delete pvl;
    ;}
    break;

  case 248:

/* Line 1455 of yacc.c  */
#line 1816 "Grammar.ypp"
    {
    curPass->createState((yyvsp[(4) - (13)].s)->c_str(), (yyvsp[(6) - (13)].ival), &(*(yyvsp[(11) - (13)].boolList))[0], (int)(yyvsp[(11) - (13)].boolList)->size());
    delete (yyvsp[(4) - (13)].s);
    delete (yyvsp[(11) - (13)].boolList);
    ;}
    break;

  case 249:

/* Line 1455 of yacc.c  */
#line 1821 "Grammar.ypp"
    {
    yyerror("TODO: SHDPROFILE for HLSL to do...\n");
    delete (yyvsp[(4) - (5)].s);
    ;}
    break;

  case 250:

/* Line 1455 of yacc.c  */
#line 1825 "Grammar.ypp"
    {
    IResource *pRes = curContainer->findResource((yyvsp[(7) - (8)].s)->c_str());
    if(pRes) {
        IPassState *pS = curPass->findState((yyvsp[(4) - (8)].s)->c_str(), 0);
        if(pS)
        {
            IUniform *pU;
            pU = pS->getUniform();
            pU->setTextureResource(pRes);
        }
        else
            pS = curPass->createStateTextureResource((yyvsp[(4) - (8)].s)->c_str(), pRes);
    }
    else yyerror("Error in sampler resource assignment : cannot find the resource");
    delete (yyvsp[(4) - (8)].s);
    delete (yyvsp[(7) - (8)].s);
;}
    break;

  case 251:

/* Line 1455 of yacc.c  */
#line 1842 "Grammar.ypp"
    {
    IResource *pRes = curContainer->findResource((yyvsp[(7) - (8)].s)->c_str());
    if(pRes) {
        IPassState *pS = curPass->findStateOverride((yyvsp[(4) - (8)].s)->c_str());
        if(pS)
        {
            IUniform *pU;
            pU = pS->getUniform();
            pU->setTextureResource(pRes);
        }
        else
            pS = curPass->createStateTextureResourceOverride((yyvsp[(4) - (8)].s)->c_str(), pRes);
    }
    else yyerror("Error in sampler resource assignment : cannot find the resource");
    delete (yyvsp[(4) - (8)].s);
    delete (yyvsp[(7) - (8)].s);
;}
    break;

  case 252:

/* Line 1455 of yacc.c  */
#line 1859 "Grammar.ypp"
    {
    IResource *pRes = curContainer->findResource((yyvsp[(7) - (8)].s)->c_str());
    if(pRes) {
        IPassState *pS = curPass->findState((yyvsp[(4) - (8)].s)->c_str(), 0);
        if(pS)
        {
            IUniform *pU;
            pU = pS->getUniform();
            pU->setSamplerResource(pRes, -1);
        }
        else
            pS = curPass->createStateSamplerResource((yyvsp[(4) - (8)].s)->c_str(), pRes, -1);
    }
    else yyerror("Error in sampler resource assignment : cannot find the resource");
    delete (yyvsp[(4) - (8)].s);
    delete (yyvsp[(7) - (8)].s);
;}
    break;

  case 253:

/* Line 1455 of yacc.c  */
#line 1876 "Grammar.ypp"
    {
    IResource *pRes = curContainer->findResource((yyvsp[(7) - (8)].s)->c_str());
    if(pRes) {
        IPassState *pS = curPass->findStateOverride((yyvsp[(4) - (8)].s)->c_str());
        if(pS)
        {
            IUniform *pU;
            pU = pS->getUniform();
            pU->setSamplerResource(pRes, -1);
        }
        else
            pS = curPass->createStateSamplerResourceOverride((yyvsp[(4) - (8)].s)->c_str(), pRes, -1);
    }
    else yyerror("Error in sampler resource assignment : cannot find the resource");
    delete (yyvsp[(4) - (8)].s);
    delete (yyvsp[(7) - (8)].s);
;}
    break;

  case 254:

/* Line 1455 of yacc.c  */
#line 1893 "Grammar.ypp"
    {
    IResource *pRes = curContainer->findResource((yyvsp[(9) - (10)].s)->c_str());
    if(pRes) {
        IPassState *pS = curPass->findState((yyvsp[(4) - (10)].s)->c_str(), 0);
        if(pS)
        {
            IUniform *pU;
            pU = pS->getUniform();
            pU->setSamplerResource(pRes, -1);
        }
        else
            pS = curPass->createStateSamplerResource((yyvsp[(4) - (10)].s)->c_str(), pRes, -1);
    }
    else yyerror("Error in sampler resource assignment : cannot find the resource");
    delete (yyvsp[(4) - (10)].s);
    delete (yyvsp[(9) - (10)].s);
;}
    break;

  case 255:

/* Line 1455 of yacc.c  */
#line 1910 "Grammar.ypp"
    {
    IResource *pRes = curContainer->findResource((yyvsp[(8) - (12)].s)->c_str());
    if(pRes) {
        IPassState *pS = curPass->findState((yyvsp[(4) - (12)].s)->c_str(), 0);
        if(pS)
        {
            IUniform *pU;
            pU = pS->getUniform();
            pU->setSamplerResource(pRes, (yyvsp[(10) - (12)].ival));
        }
        else
            pS = curPass->createStateSamplerResource((yyvsp[(4) - (12)].s)->c_str(), pRes, (yyvsp[(10) - (12)].ival));
    }
    else yyerror("Error in sampler resource assignment : cannot find the resource");
    delete (yyvsp[(4) - (12)].s);
    delete (yyvsp[(8) - (12)].s);
;}
    break;

  case 256:

/* Line 1455 of yacc.c  */
#line 1927 "Grammar.ypp"
    {
    IResource *pRes = curContainer->findResource((yyvsp[(8) - (12)].s)->c_str());
    if(pRes) {
        IPassState *pS = curPass->findStateOverride((yyvsp[(4) - (12)].s)->c_str());
        if(pS)
        {
            IUniform *pU;
            pU = pS->getUniform();
            pU->setSamplerResource(pRes, (yyvsp[(10) - (12)].ival));
        }
        else
            pS = curPass->createStateSamplerResourceOverride((yyvsp[(4) - (12)].s)->c_str(), pRes, (yyvsp[(10) - (12)].ival));
    }
    else yyerror("Error in sampler resource assignment : cannot find the resource");
    delete (yyvsp[(4) - (12)].s);
    delete (yyvsp[(8) - (12)].s);
;}
    break;

  case 257:

/* Line 1455 of yacc.c  */
#line 1944 "Grammar.ypp"
    {
    IResource *pRes = curContainer->findResource((yyvsp[(7) - (8)].s)->c_str());
    if(pRes) {
        IPassState *pS = curPass->findState((yyvsp[(4) - (8)].s)->c_str(), 0);
        if(pS)
        {
            IUniform *pU;
            pU = pS->getUniform();
            pU->setImageResource(pRes, -1);
        }
        else
            pS = curPass->createStateImageResource((yyvsp[(4) - (8)].s)->c_str(), pRes, -1);
    }
    else yyerror("Error in sampler resource assignment : cannot find the resource");
    delete (yyvsp[(4) - (8)].s);
    delete (yyvsp[(7) - (8)].s);
;}
    break;

  case 258:

/* Line 1455 of yacc.c  */
#line 1961 "Grammar.ypp"
    {
    IResource *pRes = curContainer->findResource((yyvsp[(7) - (8)].s)->c_str());
    if(pRes) {
        IPassState *pS = curPass->findStateOverride((yyvsp[(4) - (8)].s)->c_str());
        if(pS)
        {
            IUniform *pU;
            pU = pS->getUniform();
            pU->setImageResource(pRes, -1);
        }
        else
            pS = curPass->createStateImageResourceOverride((yyvsp[(4) - (8)].s)->c_str(), pRes, -1);
    }
    else yyerror("Error in sampler resource assignment : cannot find the resource");
    delete (yyvsp[(4) - (8)].s);
    delete (yyvsp[(7) - (8)].s);
;}
    break;

  case 259:

/* Line 1455 of yacc.c  */
#line 1978 "Grammar.ypp"
    {
    IResource *pRes = curContainer->findResource((yyvsp[(9) - (10)].s)->c_str());
    if(pRes) {
        IPassState *pS = curPass->findState((yyvsp[(4) - (10)].s)->c_str(), 0);
        if(pS)
        {
            IUniform *pU;
            pU = pS->getUniform();
            pU->setImageResource(pRes, -1);
        }
        else
            pS = curPass->createStateImageResource((yyvsp[(4) - (10)].s)->c_str(), pRes, -1);
    }
    else yyerror("Error in sampler resource assignment : cannot find the resource");
    delete (yyvsp[(4) - (10)].s);
    delete (yyvsp[(9) - (10)].s);
;}
    break;

  case 260:

/* Line 1455 of yacc.c  */
#line 1995 "Grammar.ypp"
    {
    IResource *pRes = curContainer->findResource((yyvsp[(8) - (12)].s)->c_str());
    if(pRes) {
        IPassState *pS = curPass->findState((yyvsp[(4) - (12)].s)->c_str(), 0);
        if(pS)
        {
            IUniform *pU;
            pU = pS->getUniform();
            pU->setImageResource(pRes, (yyvsp[(10) - (12)].ival));
        }
        else
            pS = curPass->createStateImageResource((yyvsp[(4) - (12)].s)->c_str(), pRes, (yyvsp[(10) - (12)].ival));
    }
    else yyerror("Error in sampler resource assignment : cannot find the resource");
    delete (yyvsp[(4) - (12)].s);
    delete (yyvsp[(8) - (12)].s);
;}
    break;

  case 261:

/* Line 1455 of yacc.c  */
#line 2012 "Grammar.ypp"
    {
    IPassState *pS = curPass->findState((yyvsp[(4) - (8)].s)->c_str(), 0);
    if(pS)
    {
        IUniform *pU;
        pU = pS->getUniform();
        if(pU)
            pU->setSamplerUnit((yyvsp[(7) - (8)].ival));
    }
    else
        pS = curPass->createStateSamplerResource((yyvsp[(4) - (8)].s)->c_str(), (IResource*)NULL, (yyvsp[(7) - (8)].ival));
    delete (yyvsp[(4) - (8)].s);
;}
    break;

  case 262:

/* Line 1455 of yacc.c  */
#line 2025 "Grammar.ypp"
    {
    IPassState *pS = curPass->findStateOverride((yyvsp[(4) - (8)].s)->c_str());
    if(pS)
    {
        IUniform *pU;
        pU = pS->getUniform();
        if(pU)
            pU->setSamplerUnit((yyvsp[(7) - (8)].ival));
    }
    else
        pS = curPass->createStateSamplerResourceOverride((yyvsp[(4) - (8)].s)->c_str(), (IResource*)NULL, (yyvsp[(7) - (8)].ival));
    delete (yyvsp[(4) - (8)].s);
;}
    break;

  case 263:

/* Line 1455 of yacc.c  */
#line 2038 "Grammar.ypp"
    {
    IPassState *pS = curPass->findState((yyvsp[(4) - (8)].s)->c_str(), 0);
    if(pS)
    {
        IUniform *pU;
        pU = pS->getUniform();
        if(pU)
            pU->setImageUnit((yyvsp[(7) - (8)].ival));
    }
    else
        pS = curPass->createStateSamplerResource((yyvsp[(4) - (8)].s)->c_str(), (IResource*)NULL, (yyvsp[(7) - (8)].ival));
    delete (yyvsp[(4) - (8)].s);
;}
    break;

  case 264:

/* Line 1455 of yacc.c  */
#line 2051 "Grammar.ypp"
    {
    IPassState *pS = curPass->findStateOverride((yyvsp[(4) - (8)].s)->c_str());
    if(pS)
    {
        IUniform *pU;
        pU = pS->getUniform();
        if(pU)
            pU->setImageUnit((yyvsp[(7) - (8)].ival));
    }
    else
        pS = curPass->createStateSamplerResourceOverride((yyvsp[(4) - (8)].s)->c_str(), (IResource*)NULL, (yyvsp[(7) - (8)].ival));
    delete (yyvsp[(4) - (8)].s);
;}
    break;

  case 265:

/* Line 1455 of yacc.c  */
#line 2064 "Grammar.ypp"
    {
    IPassState *pS = curPass->findState((yyvsp[(4) - (10)].s)->c_str());
    if(pS)
    {
        IUniform *pU;
        pU = pS->getUniform();
        if(pU)
            pU->setSamplerUnit((yyvsp[(9) - (10)].ival));
    }
    else
        pS = curPass->createStateSamplerResource((yyvsp[(4) - (10)].s)->c_str(), (IResource*)NULL, (yyvsp[(9) - (10)].ival));
    delete (yyvsp[(4) - (10)].s);
;}
    break;

  case 266:

/* Line 1455 of yacc.c  */
#line 2077 "Grammar.ypp"
    {
    IPassState *pS = curPass->findState((yyvsp[(4) - (10)].s)->c_str());
    if(pS)
    {
        IUniform *pU;
        pU = pS->getUniform();
        if(pU)
            pU->setImageUnit((yyvsp[(9) - (10)].ival));
    }
    else
        pS = curPass->createStateSamplerResource((yyvsp[(4) - (10)].s)->c_str(), (IResource*)NULL, (yyvsp[(9) - (10)].ival));
    delete (yyvsp[(4) - (10)].s);
;}
    break;

  case 267:

/* Line 1455 of yacc.c  */
#line 2090 "Grammar.ypp"
    {
    IPassState *pS = curPass->findState((yyvsp[(4) - (8)].s)->c_str(), 0);
    ISamplerState * pState = curContainer->findSamplerState((yyvsp[(7) - (8)].s)->c_str());
    if(!pState)
        yyerror("Error: sampler-state referenced not found\n");
    else {
        if(pS)
        {
            IUniform *pU;
            pU = pS->getUniform();
            if(pU && pState)
                pU->setSamplerState(pState);
            if(!pU)
                yyerror("Error: IUniform parameter not found\n");
        } else {
            pS = curPass->createStateSamplerState((yyvsp[(4) - (8)].s)->c_str(), pState);
        }
    }
    delete (yyvsp[(4) - (8)].s);
    delete (yyvsp[(7) - (8)].s);
;}
    break;

  case 268:

/* Line 1455 of yacc.c  */
#line 2111 "Grammar.ypp"
    {
    IPassState *pS = curPass->findStateOverride((yyvsp[(4) - (8)].s)->c_str());
    ISamplerState * pState = curContainer->findSamplerState((yyvsp[(7) - (8)].s)->c_str());
    if(!pState)
        yyerror("Error: sampler-state referenced not found\n");
    else {
        if(pS)
        {
            IUniform *pU;
            pU = pS->getUniform();
            if(pU && pState)
                pU->setSamplerState(pState);
            if(!pU)
                yyerror("Error: IUniform parameter not found\n");
        } else {
            pS = curPass->createStateSamplerStateOverride((yyvsp[(4) - (8)].s)->c_str(), pState);
        }
    }
    delete (yyvsp[(4) - (8)].s);
    delete (yyvsp[(7) - (8)].s);
;}
    break;

  case 269:

/* Line 1455 of yacc.c  */
#line 2132 "Grammar.ypp"
    { // same as above but different way to write it
    IPassState *pS = curPass->findState((yyvsp[(4) - (10)].s)->c_str());
    ISamplerState * pState = curContainer->findSamplerState((yyvsp[(9) - (10)].s)->c_str());
    if(!pState)
        yyerror("Error: sampler-state referenced not found\n");
    else {
        if(pS)
        {
            IUniform *pU;
            pU = pS->getUniform();
            if(pU && pState)
                pU->setSamplerState(pState);
            if(!pU)
                yyerror("Error: IUniform parameter not found\n");
        } else {
            pS = curPass->createStateSamplerState((yyvsp[(4) - (10)].s)->c_str(), pState);
        }
    }
    delete (yyvsp[(4) - (10)].s);
    delete (yyvsp[(9) - (10)].s);
;}
    break;

  case 270:

/* Line 1455 of yacc.c  */
#line 2153 "Grammar.ypp"
    { // same as above but different way to write it
    IStateGroupDST* pSG = curContainer->findStateGroupDST((yyvsp[(4) - (5)].s)->c_str());
    if(!pSG)
        yyerror("Error: State group not found\n");
    else {
        // TODO: shall we check if a previous state group was assigned ? And rais an error/warning ?
        curPass->createState(pSG);
    }
    delete (yyvsp[(4) - (5)].s);
;}
    break;

  case 271:

/* Line 1455 of yacc.c  */
#line 2163 "Grammar.ypp"
    { // same as above but different way to write it
    IStateGroupDST* pSG = curContainer->findStateGroupDST((yyvsp[(4) - (6)].s)->c_str());
    if(!pSG)
        yyerror("Error: State group not found\n");
    else {
        // TODO: shall we check if a previous state group was assigned ? And rais an error/warning ?
        curPass->createState(pSG);
    }
    delete (yyvsp[(4) - (6)].s);
;}
    break;

  case 272:

/* Line 1455 of yacc.c  */
#line 2173 "Grammar.ypp"
    { // same as above but different way to write it
    IStateGroupRaster* pSG = curContainer->findStateGroupRaster((yyvsp[(4) - (5)].s)->c_str());
    if(!pSG)
        yyerror("Error: State group not found\n");
    else {
        // TODO: shall we check if a previous state group was assigned ? And rais an error/warning ?
        curPass->createState(pSG);
    }
    delete (yyvsp[(4) - (5)].s);
;}
    break;

  case 273:

/* Line 1455 of yacc.c  */
#line 2183 "Grammar.ypp"
    { // same as above but different way to write it
    IStateGroupRaster* pSG = curContainer->findStateGroupRaster((yyvsp[(4) - (6)].s)->c_str());
    if(!pSG)
        yyerror("Error: State group not found\n");
    else {
        // TODO: shall we check if a previous state group was assigned ? And rais an error/warning ?
        curPass->createState(pSG);
    }
    delete (yyvsp[(4) - (6)].s);
;}
    break;

  case 274:

/* Line 1455 of yacc.c  */
#line 2193 "Grammar.ypp"
    { // same as above but different way to write it
    IStateGroupCS* pSG = curContainer->findStateGroupCS((yyvsp[(4) - (5)].s)->c_str());
    if(!pSG)
        yyerror("Error: State group not found\n");
    else {
        // TODO: shall we check if a previous state group was assigned ? And rais an error/warning ?
        curPass->createState(pSG);
    }
    delete (yyvsp[(4) - (5)].s);
;}
    break;

  case 275:

/* Line 1455 of yacc.c  */
#line 2203 "Grammar.ypp"
    { // same as above but different way to write it
    IStateGroupCS* pSG = curContainer->findStateGroupCS((yyvsp[(4) - (6)].s)->c_str());
    if(!pSG)
        yyerror("Error: State group not found\n");
    else {
        // TODO: shall we check if a previous state group was assigned ? And rais an error/warning ?
        curPass->createState(pSG);
    }
    delete (yyvsp[(4) - (6)].s);
;}
    break;

  case 276:

/* Line 1455 of yacc.c  */
#line 2216 "Grammar.ypp"
    {
    curPass->createStateRenderMode((yyvsp[(4) - (5)].render_mode));
;}
    break;

  case 277:

/* Line 1455 of yacc.c  */
#line 2219 "Grammar.ypp"
    {
    curPass->createStateRenderGroup((yyvsp[(4) - (5)].ival));
;}
    break;

  case 278:

/* Line 1455 of yacc.c  */
#line 2222 "Grammar.ypp"
    {
    if((*(yyvsp[(4) - (5)].s)) == "BackBuffer")
        curPass->createStateBlitFBOToActiveTarget(NULL);
    else {
        IFrameBufferObject* pFBO = curContainer->findFbo((yyvsp[(4) - (5)].s)->c_str());
        if(!pFBO) {
            yyerror("Error: could not find the Frame Buffer Object");
            yyerror((yyvsp[(4) - (5)].s)->c_str());
        } else {
            curPass->createStateBlitFBOToActiveTarget(pFBO);
        }
    }
    delete (yyvsp[(4) - (5)].s)
;}
    break;

  case 279:

/* Line 1455 of yacc.c  */
#line 2236 "Grammar.ypp"
    {
    curPass->createStateClearMode((yyvsp[(4) - (5)].clear_mode));
;}
    break;

  case 280:

/* Line 1455 of yacc.c  */
#line 2239 "Grammar.ypp"
    {
    if((*(yyvsp[(4) - (5)].s)) == "backbuffer")
        curPass->createStateRenderTarget(NULL);
    else {
        IFrameBufferObject* pFBO = curContainer->findFbo((yyvsp[(4) - (5)].s)->c_str());
        if(!pFBO) {
            yyerror("Error: could not find the Frame Buffer Object");
            yyerror((yyvsp[(4) - (5)].s)->c_str());
        } else {
            curPass->createStateRenderTarget(pFBO);
        }
    }
    delete (yyvsp[(4) - (5)].s);
;}
    break;

  case 281:

/* Line 1455 of yacc.c  */
#line 2253 "Grammar.ypp"
    {
    FloatVecList *pfvl = (yyvsp[(7) - (8)].floatVecList);
    FloatList* pfl = (*pfvl)[0];
    float* pf = &(*pfl)[0];
    if((yyvsp[(4) - (8)].ival) < 4)
        curPass->createStateClearColor((yyvsp[(4) - (8)].ival), (*pfl)[0], (*pfl)[1], (*pfl)[2], (*pfl)[3]);
    else
        yyerror("ClearColor[n] can only clear up to 4 targets\n");
    delete (yyvsp[(7) - (8)].floatVecList);
;}
    break;

  case 282:

/* Line 1455 of yacc.c  */
#line 2263 "Grammar.ypp"
    {
    curPass->createStateClearColor(0, (*(yyvsp[(5) - (7)].floatList))[0], (*(yyvsp[(5) - (7)].floatList))[1], (*(yyvsp[(5) - (7)].floatList))[2], (*(yyvsp[(5) - (7)].floatList))[3]);
    delete (yyvsp[(5) - (7)].floatList);
;}
    break;

  case 283:

/* Line 1455 of yacc.c  */
#line 2267 "Grammar.ypp"
    {
    if((yyvsp[(5) - (7)].intList)->size() >= 4)
        curPass->createStateViewport((*(yyvsp[(5) - (7)].intList))[0], (*(yyvsp[(5) - (7)].intList))[1], (*(yyvsp[(5) - (7)].intList))[2], (*(yyvsp[(5) - (7)].intList))[3], ((yyvsp[(5) - (7)].intList)->size() >= 5)?(*(yyvsp[(5) - (7)].intList))[4]:0.0f, ((yyvsp[(5) - (7)].intList)->size() >= 6)?(*(yyvsp[(5) - (7)].intList))[5]:0.0f);
    else
        yyerror("Viewport needs a 4 or 6 components vector\n");
    delete (yyvsp[(5) - (7)].intList);
;}
    break;

  case 284:

/* Line 1455 of yacc.c  */
#line 2274 "Grammar.ypp"
    {
    if(((*(yyvsp[(4) - (8)].s)) == "backbuffer")||((*(yyvsp[(6) - (8)].s)) == "backbuffer")) {
            yyerror("Error: cannot swap resources with backbuffer target");
    } else {
        IFrameBufferObject* pFBO1 = curContainer->findFbo((yyvsp[(4) - (8)].s)->c_str());
        if(!pFBO1) {
            yyerror("Error: could not find the Frame Buffer Object");
            yyerror((yyvsp[(4) - (8)].s)->c_str());
        } else {
            IFrameBufferObject* pFBO2 = curContainer->findFbo((yyvsp[(6) - (8)].s)->c_str());
            if(!pFBO2) {
                yyerror("Error: could not find the Frame Buffer Object");
                yyerror((yyvsp[(6) - (8)].s)->c_str());
            } else {
                curPass->createStateSwapResources(pFBO1, pFBO2);
            }
        }
    }
    delete (yyvsp[(4) - (8)].s);
    delete (yyvsp[(6) - (8)].s);
;}
    break;

  case 285:

/* Line 1455 of yacc.c  */
#line 2295 "Grammar.ypp"
    {
    curPass->setLoopCount((yyvsp[(4) - (5)].ival));
;}
    break;

  case 286:

/* Line 1455 of yacc.c  */
#line 2298 "Grammar.ypp"
    {
    curPass->setActive((yyvsp[(4) - (5)].bval));
;}
    break;

  case 287:

/* Line 1455 of yacc.c  */
#line 2304 "Grammar.ypp"
    {
    LOGD("computeGroups\n");
#ifdef USEGLSLCOMPUTE
    if((yyvsp[(5) - (7)].intList)->size() < 2)
        yyerror("Grid size must have 2 or 3 values\n");
    else
        curPass->createStateComputeGroup((*(yyvsp[(5) - (7)].intList))[0], (*(yyvsp[(5) - (7)].intList))[1], (yyvsp[(5) - (7)].intList)->size() > 2 ? (*(yyvsp[(5) - (7)].intList))[2] : 1);
#else
    yyerror("Compute Not supported in this build\n");
#endif
    delete (yyvsp[(5) - (7)].intList);
;}
    break;

  case 288:

/* Line 1455 of yacc.c  */
#line 2319 "Grammar.ypp"
    {
    LOGD("CUDAMODULE\n");
#ifdef USECUDA
    if((yyvsp[(4) - (5)].shaderList)->size() > 0)
        curPass->createState(IPassState::TCUDAModule, & (*(yyvsp[(4) - (5)].shaderList))[0], (int)(yyvsp[(4) - (5)].shaderList)->size());
    delete (yyvsp[(4) - (5)].shaderList);
#else
    LOGI("CUDA Not supported in this build\n");
#endif
;}
    break;

  case 289:

/* Line 1455 of yacc.c  */
#line 2329 "Grammar.ypp"
    {
    LOGD("CUDAKERNEL\n");
#ifdef USECUDA
    // CUDAMODULE gathered pieces of CUDA code. We expect some kernels to be in there
    // let's find the named kernel and check if the arguments are matching the assignements, here
    bool bRes = true;
    IShader* pShd = curContainer->findShader((yyvsp[(5) - (9)].s)->c_str());
    if(pShd)
    {
        IShaderEx* pShdEx = pShd->getExInterface();
        for(int i=0; i<(int)(yyvsp[(7) - (9)].argValList)->size(); i++)
        {
            // get the argument of this 
            IShaderEx::Argument arg = pShdEx->getArgument(i);
            // compare if the one from the pass-state is compatible
            if(checkArgCompatibility((*(yyvsp[(7) - (9)].argValList))[i], arg) == false)
            {
                bRes = false;
                yyerror("Argument values don't match the Kernel's argument!\n");
                break;
            }
        }
        if(bRes) {
            ArgVal* vals = NULL;
            if((yyvsp[(7) - (9)].argValList)->size() > 0) {
                vals = new ArgVal[(yyvsp[(7) - (9)].argValList)->size()];
                for(int i=0; i<(int)(yyvsp[(7) - (9)].argValList)->size(); i++)
                    vals[i] = *((*(yyvsp[(7) - (9)].argValList))[i]);
            }
            curPass->createStateKernelEntry((yyvsp[(5) - (9)].s)->c_str(), (int)(yyvsp[(7) - (9)].argValList)->size(), vals);
            // we dont "delete [] vals;" because this pointer got passed to the pass-state and will be passed
            // down to the kernel execution.
        }
    } else {
        yyerror("CUDAKernel name Not found !\n");
    }
    for(int i=0; i<(int)(yyvsp[(7) - (9)].argValList)->size(); i++)
        delete (*(yyvsp[(7) - (9)].argValList))[i]; // free after done using it
#else
    LOGI("CUDA Not supported in this build\n");
    for(int i=0; i<(int)(yyvsp[(7) - (9)].argValList)->size(); i++)
        delete (*(yyvsp[(7) - (9)].argValList))[i];
#endif
    delete (yyvsp[(7) - (9)].argValList);
    delete (yyvsp[(5) - (9)].s);
;}
    break;

  case 290:

/* Line 1455 of yacc.c  */
#line 2375 "Grammar.ypp"
    {
    LOGD("CUDASHMEM\n");
#ifdef USECUDA
    curPass->createStateSharedMemory((yyvsp[(4) - (5)].ival))
#else
    LOGI("CUDA Not supported in this build\n");
#endif
;}
    break;

  case 291:

/* Line 1455 of yacc.c  */
#line 2383 "Grammar.ypp"
    {
    LOGD("CUDAGRID\n");
#ifdef USECUDA
    IntVecList* pIVL = (yyvsp[(4) - (5)].intVecList);
    IntList* pIL = pIVL->size() >=1 ? (*pIVL)[0]:NULL;
    if(pIL && (pIL->size()>=2))
    {
        if(pIL->size() < 2)
            yyerror("Grid size must have 2 or 3 values\n");
        curPass->createStateGridSz((*pIL)[0], (*pIL)[1], pIL->size() > 2 ? (*pIL)[2] : 1);
    } else
        yyerror("Error in cuda grid setup\n");
#else
    LOGI("CUDA Not supported in this build\n");
#endif
    delete (yyvsp[(4) - (5)].intVecList);
;}
    break;

  case 292:

/* Line 1455 of yacc.c  */
#line 2400 "Grammar.ypp"
    {
    LOGD("CUDABLOCK\n");
#ifdef USECUDA
    IntVecList* pIVL = (yyvsp[(4) - (5)].intVecList);
    IntList* pIL = pIVL->size() >=1 ? (*pIVL)[0]:NULL;
    if(pIL && (pIL->size()>=2))
    {
        if(pIL->size() < 2)
            yyerror("Grid size must have 2 values\n");
        curPass->createStateBlockSz((*pIL)[0], (*pIL)[1], pIL->size()>2 ? (*pIL)[2] : 1);
    } else
        yyerror("Error in cuda grid setup\n");
#else
    LOGI("CUDA Not supported in this build\n");
#endif
    delete (yyvsp[(4) - (5)].intVecList);
;}
    break;

  case 293:

/* Line 1455 of yacc.c  */
#line 2420 "Grammar.ypp"
    {
    curPass->bindAttribute((yyvsp[(4) - (8)].s)->c_str(), (yyvsp[(7) - (8)].ival));
    delete (yyvsp[(4) - (8)].s);
;}
    break;

  case 294:

/* Line 1455 of yacc.c  */
#line 2424 "Grammar.ypp"
    { 
    yyerror("Error in a state of the Pass\n");
;}
    break;

  case 295:

/* Line 1455 of yacc.c  */
#line 2433 "Grammar.ypp"
    {
    //printf("Creation of PASS\n");
    curPass = curTechnique->addPass()->getExInterface();
    curAnnotations = curPass->annotations()->getExInterface();
    ;}
    break;

  case 296:

/* Line 1455 of yacc.c  */
#line 2438 "Grammar.ypp"
    {
    LOGI("Pass with no name...\n");
    curAnnotations = NULL;
    curRState = NULL;
    curCSState = NULL;
    curDSTState = NULL;
    curPRState = NULL;
    lex_pop_state();
;}
    break;

  case 297:

/* Line 1455 of yacc.c  */
#line 2447 "Grammar.ypp"
    {
    //printf("Creation of PASS %s\n", $2->c_str());
    curPass = curTechnique->addPass((yyvsp[(2) - (2)].s)->c_str())->getExInterface();
    curAnnotations = curPass->annotations()->getExInterface();
    ;}
    break;

  case 298:

/* Line 1455 of yacc.c  */
#line 2452 "Grammar.ypp"
    {
    LOGD("Pass %s...\n", (yyvsp[(2) - (7)].s)->c_str() );
    delete (yyvsp[(2) - (7)].s);
    curAnnotations = NULL;
    curRState = NULL;
    curCSState = NULL;
    curDSTState = NULL;
    curPRState = NULL;
    lex_pop_state();
;}
    break;

  case 299:

/* Line 1455 of yacc.c  */
#line 2465 "Grammar.ypp"
    {
    curTechnique->bindAttribute((yyvsp[(3) - (7)].s)->c_str(), (yyvsp[(6) - (7)].ival));
    delete (yyvsp[(3) - (7)].s);
;}
    break;

  case 300:

/* Line 1455 of yacc.c  */
#line 2469 "Grammar.ypp"
    {
    curTechnique->bindAttribute((yyvsp[(3) - (7)].s)->c_str(), (yyvsp[(6) - (7)].ival));
    delete (yyvsp[(3) - (7)].s);
;}
    break;

  case 301:

/* Line 1455 of yacc.c  */
#line 2473 "Grammar.ypp"
    {
    ITechnique *pTech = curContainer->findTechnique((yyvsp[(2) - (3)].s)->c_str());
    if(pTech == NULL)
    {
        yyerror("sub-technique not found\n");
    } else if(!curTechnique->addSubTechnique(pTech))
    {
        yyerror("Error in adding a sub-technique\n");
    }
    delete (yyvsp[(2) - (3)].s);
;}
    break;

  case 302:

/* Line 1455 of yacc.c  */
#line 2484 "Grammar.ypp"
    {
    ITechnique *pTech = curContainer->findTechnique((yyvsp[(2) - (4)].s)->c_str());
    if(pTech == NULL)
    {
        yyerror("sub-technique not found\n");
    } else if(!curTechnique->addSubTechnique(pTech, false))
    {
        yyerror("Error in adding a sub-technique\n");
    }
    delete (yyvsp[(2) - (4)].s);
;}
    break;

  case 305:

/* Line 1455 of yacc.c  */
#line 2504 "Grammar.ypp"
    { yyerror("Error in Pass list\n");;}
    break;

  case 306:

/* Line 1455 of yacc.c  */
#line 2510 "Grammar.ypp"
    {
    //printf("Creation of Technique for NO name\n");
    curTechnique = curContainer->createTechnique()->getExInterface();
    curAnnotations = curTechnique->annotations()->getExInterface();
;}
    break;

  case 307:

/* Line 1455 of yacc.c  */
#line 2514 "Grammar.ypp"
    { 
    curAnnotations = NULL;
    lex_pop_state();
;}
    break;

  case 308:

/* Line 1455 of yacc.c  */
#line 2518 "Grammar.ypp"
    {
    LOGD("creation of Technique %s...\n", (yyvsp[(2) - (2)].s)->c_str() );
    curTechnique = curContainer->createTechnique((yyvsp[(2) - (2)].s)->c_str())->getExInterface();
    curAnnotations = curTechnique->annotations()->getExInterface();
    delete (yyvsp[(2) - (2)].s);
;}
    break;

  case 309:

/* Line 1455 of yacc.c  */
#line 2523 "Grammar.ypp"
    { 
    lex_pop_state();
    curAnnotations = NULL;
;}
    break;

  case 310:

/* Line 1455 of yacc.c  */
#line 2533 "Grammar.ypp"
    { (yyval.bval)=false; ;}
    break;

  case 311:

/* Line 1455 of yacc.c  */
#line 2534 "Grammar.ypp"
    { (yyval.bval)=true; ;}
    break;

  case 312:

/* Line 1455 of yacc.c  */
#line 2539 "Grammar.ypp"
    { (yyval.bval)=true; ;}
    break;

  case 313:

/* Line 1455 of yacc.c  */
#line 2540 "Grammar.ypp"
    { (yyval.bval)=false; ;}
    break;

  case 314:

/* Line 1455 of yacc.c  */
#line 2541 "Grammar.ypp"
    { (yyval.bval)=true; ;}
    break;

  case 315:

/* Line 1455 of yacc.c  */
#line 2549 "Grammar.ypp"
    {
    if(!namespaceName.empty())
        (*(yyvsp[(1) - (1)].s)) = namespaceName + std::string("_NS_") + (*(yyvsp[(1) - (1)].s));
    (yyval.s) = (yyvsp[(1) - (1)].s);
;}
    break;

  case 316:

/* Line 1455 of yacc.c  */
#line 2558 "Grammar.ypp"
    { (yyval.global_varname_annots).global = false; (yyval.global_varname_annots).varName = NULL; ;}
    break;

  case 317:

/* Line 1455 of yacc.c  */
#line 2559 "Grammar.ypp"
    { (yyval.global_varname_annots).global = false; (yyval.global_varname_annots).varName = (yyvsp[(1) - (2)].s); ;}
    break;

  case 318:

/* Line 1455 of yacc.c  */
#line 2561 "Grammar.ypp"
    { 
    // for the case of items within a namespace :
    // copy the name-space annotations to this one
    if(curAnnotations) curAnnotations->copy(namespaceAnnotations);
    (yyval.global_varname_annots).global = (yyvsp[(1) - (3)].bval); 
    (yyval.global_varname_annots).varName = (yyvsp[(2) - (3)].s);
;}
    break;

  case 319:

/* Line 1455 of yacc.c  */
#line 2573 "Grammar.ypp"
    {
    Sglobal_varname &gva = (yyvsp[(2) - (4)].global_varname_annots);
    LOGD("GLSLShader %s...\n", gva.varName ? gva.varName->c_str() : "Un-named");
    IShader* pShader = curContainer->createGLSLShader(gva.varName ? gva.varName->c_str() : NULL, gva.global);
    if(pShader)
    {
        curShader = pShader->getExInterface();
        // Now we copy the possible annotations that were filled in arraysz_semantic_annotations_uniformvalueassigment
        curShader->annotations()->getExInterface()->clear();
        curShader->annotations()->getExInterface()->copy(curAnnotations);

        if(curShader->getShaderCodeSz() == 0) // ==0 means it needs to have some code loaded now
            curShader->loadShaderCode((yyvsp[(4) - (4)].s)->c_str(), line_num, file_name.c_str() );
        else
            LOGI("Skipping the shader code of %s : some is already loaded\n", gva.varName ? gva.varName->c_str() : "Un-named");
    }
    else yyerror("Failed in creating shader");
    delete gva.varName;
    delete (yyvsp[(4) - (4)].s);
;}
    break;

  case 320:

/* Line 1455 of yacc.c  */
#line 2597 "Grammar.ypp"
    {
    LOGD("SVG Path %s...\n", (yyvsp[(2) - (4)].global_varname_annots).varName ? (yyvsp[(2) - (4)].global_varname_annots).varName->c_str() : "Un-named" );
    if(curShader = curContainer->createPathShader((yyvsp[(2) - (4)].global_varname_annots).varName ? (yyvsp[(2) - (4)].global_varname_annots).varName->c_str() : NULL, (yyvsp[(2) - (4)].global_varname_annots).global)->getExInterface())
    {
        // Now we copy the possible annotations that were filled in arraysz_semantic_annotations_uniformvalueassigment
        curShader->annotations()->getExInterface()->clear();
        curShader->annotations()->getExInterface()->copy(curAnnotations);

        if(curShader->getShaderCodeSz() == 0) // ==0 means it needs to have some code loaded now
            curShader->loadShaderCode((yyvsp[(4) - (4)].s)->c_str(), line_num, file_name.c_str() );
        else
            LOGI("Skipping the shader code of %s : some is already loaded\n", (yyvsp[(2) - (4)].global_varname_annots).varName ? (yyvsp[(2) - (4)].global_varname_annots).varName->c_str() : "Un-named");
    }
    else yyerror("Failed in creating shader");
    delete (yyvsp[(2) - (4)].global_varname_annots).varName;
    delete (yyvsp[(4) - (4)].s);
;}
    break;

  case 321:

/* Line 1455 of yacc.c  */
#line 2618 "Grammar.ypp"
    {
    LOGD("PS Path %s...\n", (yyvsp[(2) - (4)].global_varname_annots).varName ? (yyvsp[(2) - (4)].global_varname_annots).varName->c_str() : "Un-named" );
    if(curShader = curContainer->createPathShader((yyvsp[(2) - (4)].global_varname_annots).varName ? (yyvsp[(2) - (4)].global_varname_annots).varName->c_str() : NULL, (yyvsp[(2) - (4)].global_varname_annots).global, true)->getExInterface())
    {
        // Now we copy the possible annotations that were filled in arraysz_semantic_annotations_uniformvalueassigment
        curShader->annotations()->getExInterface()->clear();
        curShader->annotations()->getExInterface()->copy(curAnnotations);

        if(curShader->getShaderCodeSz() == 0) // ==0 means it needs to have some code loaded now
            curShader->loadShaderCode((yyvsp[(4) - (4)].s)->c_str(), line_num, file_name.c_str() );
        else
            LOGI("Skipping the shader code of %s : some is already loaded\n", (yyvsp[(2) - (4)].global_varname_annots).varName ? (yyvsp[(2) - (4)].global_varname_annots).varName->c_str() : "Un-named");
    }
    else yyerror("Failed in creating shader");
    delete (yyvsp[(2) - (4)].global_varname_annots).varName;
    delete (yyvsp[(4) - (4)].s);
;}
    break;

  case 322:

/* Line 1455 of yacc.c  */
#line 2639 "Grammar.ypp"
    {
    LOGD("HLSL11Shader %s...\n", (yyvsp[(2) - (4)].global_varname_annots).varName ? (yyvsp[(2) - (4)].global_varname_annots).varName->c_str() : "Un-named" );
    if((yyvsp[(2) - (4)].global_varname_annots).global) { // if we need to make this shader GLOBAL
        if(curShader = pShdRep->findShader((yyvsp[(2) - (4)].global_varname_annots).varName ? (yyvsp[(2) - (4)].global_varname_annots).varName->c_str() : NULL)->getExInterface())
        {
            LOGI("    Warning : %s is already global. Skipping the declaration...\n", (yyvsp[(2) - (4)].global_varname_annots).varName ? (yyvsp[(2) - (4)].global_varname_annots).varName->c_str() : "Un-named" );
            if(curContainer->addHLSL11Shader(curShader))
                pShdRep->addShader(curShader);
            else
                yyerror("Failed in adding shader to the effect container");
        } else {
            if(curShader = curContainer->createHLSL11Shader((yyvsp[(2) - (4)].global_varname_annots).varName ? (yyvsp[(2) - (4)].global_varname_annots).varName->c_str() : NULL)->getExInterface()) {
                // Now we copy the possible annotations that were filled in arraysz_semantic_annotations_uniformvalueassigment
                curShader->annotations()->getExInterface()->clear();
                curShader->annotations()->getExInterface()->copy(curAnnotations);

                curShader->loadShaderCode((yyvsp[(4) - (4)].s)->c_str(), line_num, file_name.c_str() );
                pShdRep->addShader(curShader);
            } else
                yyerror("Failed in creating shader");
        }
    } else {
        if(curShader = curContainer->createHLSL11Shader((yyvsp[(2) - (4)].global_varname_annots).varName ? (yyvsp[(2) - (4)].global_varname_annots).varName->c_str() : NULL)->getExInterface())
        {
            // Now we copy the possible annotations that were filled in arraysz_semantic_annotations_uniformvalueassigment
            curShader->annotations()->getExInterface()->clear();
            curShader->annotations()->getExInterface()->copy(curAnnotations);

            curShader->loadShaderCode((yyvsp[(4) - (4)].s)->c_str(), line_num, file_name.c_str() );
        } else yyerror("Failed in creating shader");
    }
    delete (yyvsp[(2) - (4)].global_varname_annots).varName;
    delete (yyvsp[(4) - (4)].s);
;}
    break;

  case 323:

/* Line 1455 of yacc.c  */
#line 2677 "Grammar.ypp"
    {
    LOGD("HLSL10Shader %s...\n", (yyvsp[(2) - (4)].global_varname_annots).varName ? (yyvsp[(2) - (4)].global_varname_annots).varName->c_str() : "Un-named" );
    if((yyvsp[(2) - (4)].global_varname_annots).global) { // if we need to make this shader GLOBAL
        if(curShader = pShdRep->findShader((yyvsp[(2) - (4)].global_varname_annots).varName ? (yyvsp[(2) - (4)].global_varname_annots).varName->c_str() : NULL)->getExInterface())
        {
            LOGI("    Warning : %s is already global. Skipping the declaration...\n", (yyvsp[(2) - (4)].global_varname_annots).varName ? (yyvsp[(2) - (4)].global_varname_annots).varName->c_str() : "Un-named" );
            if(curContainer->addHLSL10Shader(curShader))
                pShdRep->addShader(curShader);
            else
                yyerror("Failed in adding shader to the effect container");
        } else {
            if(curShader = curContainer->createHLSL10Shader((yyvsp[(2) - (4)].global_varname_annots).varName ? (yyvsp[(2) - (4)].global_varname_annots).varName->c_str() : NULL)->getExInterface()) {
                // Now we copy the possible annotations that were filled in arraysz_semantic_annotations_uniformvalueassigment
                curShader->annotations()->getExInterface()->clear();
                curShader->annotations()->getExInterface()->copy(curAnnotations);

                curShader->loadShaderCode((yyvsp[(4) - (4)].s)->c_str(), line_num, file_name.c_str() );
                pShdRep->addShader(curShader);
            } else
                yyerror("Failed in creating shader");
        }
    } else {
        if(curShader = curContainer->createHLSL10Shader((yyvsp[(2) - (4)].global_varname_annots).varName ? (yyvsp[(2) - (4)].global_varname_annots).varName->c_str() : NULL)->getExInterface())
        {
            // Now we copy the possible annotations that were filled in arraysz_semantic_annotations_uniformvalueassigment
            curShader->annotations()->getExInterface()->clear();
            curShader->annotations()->getExInterface()->copy(curAnnotations);

            curShader->loadShaderCode((yyvsp[(4) - (4)].s)->c_str(), line_num, file_name.c_str() );
        } else yyerror("Failed in creating shader");
    }
    delete (yyvsp[(2) - (4)].global_varname_annots).varName;
    delete (yyvsp[(4) - (4)].s);
;}
    break;

  case 324:

/* Line 1455 of yacc.c  */
#line 2715 "Grammar.ypp"
    { (yyval.bval) = false; ;}
    break;

  case 325:

/* Line 1455 of yacc.c  */
#line 2716 "Grammar.ypp"
    { (yyval.bval) = true; ;}
    break;

  case 326:

/* Line 1455 of yacc.c  */
#line 2718 "Grammar.ypp"
    { (yyval.bval) = false; ;}
    break;

  case 327:

/* Line 1455 of yacc.c  */
#line 2719 "Grammar.ypp"
    { (yyval.bval) = true; ;}
    break;

  case 328:

/* Line 1455 of yacc.c  */
#line 2724 "Grammar.ypp"
    {
    (yyval.argument) = new Argument((yyvsp[(2) - (3)].bval) ? nvFX::TFloatPtr : nvFX::TFloat, (yyvsp[(3) - (3)].s));
    LOGD("Argument %s %s\n", (yyvsp[(2) - (3)].bval) ? "TFloatPtr" : "TFloat", (yyvsp[(3) - (3)].s)->c_str());
    delete (yyvsp[(3) - (3)].s);
;}
    break;

  case 329:

/* Line 1455 of yacc.c  */
#line 2729 "Grammar.ypp"
    {
    (yyval.argument) = new Argument((yyvsp[(1) - (4)].bval) ? ((yyvsp[(3) - (4)].bval) ? nvFX::TUIntPtr : nvFX::TUInt) : ((yyvsp[(3) - (4)].bval) ? nvFX::TIntPtr : nvFX::TInt), (yyvsp[(4) - (4)].s));
    LOGD("Argument %s %s\n", (yyvsp[(1) - (4)].bval) ? ((yyvsp[(3) - (4)].bval) ? "TUIntPtr" : "TUInt") : ((yyvsp[(3) - (4)].bval) ? "TIntPtr" : "TInt"), (yyvsp[(4) - (4)].s)->c_str() );
    delete (yyvsp[(4) - (4)].s);
;}
    break;

  case 330:

/* Line 1455 of yacc.c  */
#line 2738 "Grammar.ypp"
    { (yyval.argList) = new ArgList; ;}
    break;

  case 331:

/* Line 1455 of yacc.c  */
#line 2739 "Grammar.ypp"
    { (yyval.argList) = new ArgList; ;}
    break;

  case 332:

/* Line 1455 of yacc.c  */
#line 2740 "Grammar.ypp"
    {
    (yyval.argList) = new ArgList;
    (yyval.argList)->push_back((yyvsp[(1) - (1)].argument));
    ;}
    break;

  case 333:

/* Line 1455 of yacc.c  */
#line 2744 "Grammar.ypp"
    {
    (yyval.argList) = (yyvsp[(1) - (3)].argList);
    (yyval.argList)->push_back((yyvsp[(3) - (3)].argument));
    ;}
    break;

  case 334:

/* Line 1455 of yacc.c  */
#line 2752 "Grammar.ypp"
    {
    LOGD("CUDAShader %s...\n", (yyvsp[(2) - (7)].global_varname_annots).varName ? (yyvsp[(2) - (7)].global_varname_annots).varName->c_str() : "Un-named" );
#ifdef USECUDA
    bool bNew = true;
    if((yyvsp[(2) - (7)].global_varname_annots).global) { // if we need to make this shader GLOBAL
        if(curShader = pShdRep->findShader((yyvsp[(2) - (7)].global_varname_annots).varName ? (yyvsp[(2) - (7)].global_varname_annots).varName->c_str() : NULL)->getExInterface())
        {
            LOGI("    Warning : %s is already global. Skipping the declaration...\n", (yyvsp[(2) - (7)].global_varname_annots).varName ? (yyvsp[(2) - (7)].global_varname_annots).varName->c_str() : NULL );
            bNew = false;
            if(curContainer->addCUDAShader(curShader))
                pShdRep->addShader(curShader);
            else
                yyerror("Failed in adding shader to the effect container");
        } else {
            if(curShader = curContainer->createCUDAShader((yyvsp[(2) - (7)].global_varname_annots).varName ? (yyvsp[(2) - (7)].global_varname_annots).varName->c_str() : NULL)->getExInterface()) {
                // Now we copy the possible annotations that were filled in arraysz_semantic_annotations_uniformvalueassigment
                curShader->annotations()->getExInterface()->clear();
                curShader->annotations()->getExInterface()->copy(curAnnotations);

                curShader->loadShaderCode((yyvsp[(7) - (7)].s)->c_str(), line_num, file_name.c_str() );
                pShdRep->addShader(curShader);
            } else
                yyerror("Failed in creating shader");
        }
    } else {
        if(curShader = curContainer->createCUDAShader((yyvsp[(2) - (7)].global_varname_annots).varName ? (yyvsp[(2) - (7)].global_varname_annots).varName->c_str() : NULL)->getExInterface())
        {
            // Now we copy the possible annotations that were filled in arraysz_semantic_annotations_uniformvalueassigment
            curShader->annotations()->getExInterface()->clear();
            curShader->annotations()->getExInterface()->copy(curAnnotations);

            curShader->loadShaderCode((yyvsp[(7) - (7)].s)->c_str(), line_num, file_name.c_str() );
        } else yyerror("Failed in creating shader");
    }
    // Now take care of the arguments
    if(bNew)
    {
        IShaderEx::Argument *args = new IShaderEx::Argument[(yyvsp[(4) - (7)].argList)->size()];
        for(int i=0; i< (int)(yyvsp[(4) - (7)].argList)->size(); i++)
        {
            args[i].t = (*(yyvsp[(4) - (7)].argList))[i]->type;
            args[i].argName = (*(yyvsp[(4) - (7)].argList))[i]->name.c_str();
        }
        // send the list of arguments
        curShader->setAsKernel((int)(yyvsp[(4) - (7)].argList)->size(), args);
        delete args;
        for(int i=0; i< (int)(yyvsp[(4) - (7)].argList)->size(); i++)
            delete (*(yyvsp[(4) - (7)].argList))[i]; // done... delete
    }
#else
    LOGI("CUDA Not supported in this build\n");
#endif
    delete (yyvsp[(2) - (7)].global_varname_annots).varName;
    delete (yyvsp[(4) - (7)].argList);
    delete (yyvsp[(7) - (7)].s);
;}
    break;

  case 335:

/* Line 1455 of yacc.c  */
#line 2812 "Grammar.ypp"
    {
    LOGD("CUDAShader %s...\n", (yyvsp[(2) - (4)].global_varname_annots).varName ? (yyvsp[(2) - (4)].global_varname_annots).varName->c_str() : "Un-named" );
#ifdef USECUDA
    if((yyvsp[(2) - (4)].global_varname_annots).global) { // if we need to make this shader GLOBAL
        if(curShader = pShdRep->findShader((yyvsp[(2) - (4)].global_varname_annots).varName ? (yyvsp[(2) - (4)].global_varname_annots).varName->c_str() : NULL)->getExInterface())
        {
            LOGI("    Warning : %s is already global. Skipping the declaration...\n", (yyvsp[(2) - (4)].global_varname_annots).varName ? (yyvsp[(2) - (4)].global_varname_annots).varName->c_str() : "Un-named" );
            if(curContainer->addCUDAShader(curShader))
                pShdRep->addShader(curShader);
            else
                yyerror("Failed in adding shader to the effect container");
        } else {
            if(curShader = curContainer->createCUDAShader((yyvsp[(2) - (4)].global_varname_annots).varName ? (yyvsp[(2) - (4)].global_varname_annots).varName->c_str() : NULL)->getExInterface()) {
                // Now we copy the possible annotations that were filled in arraysz_semantic_annotations_uniformvalueassigment
                curShader->annotations()->getExInterface()->clear();
                curShader->annotations()->getExInterface()->copy(curAnnotations);

                curShader->loadShaderCode((yyvsp[(4) - (4)].s)->c_str(), line_num, file_name.c_str() );
                pShdRep->addShader(curShader);
            } else
                yyerror("Failed in creating shader");
        }
    } else {
        if(curShader = curContainer->createCUDAShader((yyvsp[(2) - (4)].global_varname_annots).varName ? (yyvsp[(2) - (4)].global_varname_annots).varName->c_str() : NULL)->getExInterface())
        {
            // Now we copy the possible annotations that were filled in arraysz_semantic_annotations_uniformvalueassigment
            curShader->annotations()->getExInterface()->clear();
            curShader->annotations()->getExInterface()->copy(curAnnotations);

            curShader->loadShaderCode((yyvsp[(4) - (4)].s)->c_str(), line_num, file_name.c_str() );
        } else yyerror("Failed in creating shader");
    }
#else
    LOGI("CUDA Not supported in this build\n");
#endif
    delete (yyvsp[(2) - (4)].global_varname_annots).varName;
    delete (yyvsp[(4) - (4)].s);
;}
    break;

  case 337:

/* Line 1455 of yacc.c  */
#line 2854 "Grammar.ypp"
    {
    assert(curResource);
    // for now, sampler-state MUST be decalred before the texture work. Or it won't be found
    ISamplerState * pState = curContainer->findSamplerState((yyvsp[(4) - (5)].s)->c_str());
    if(!pState)
        yyerror("Error: sampler-state referenced not found. Did you declare it before the texture using it ?\n");
    curResource->setSamplerState(pState);
    delete (yyvsp[(4) - (5)].s);
;}
    break;

  case 338:

/* Line 1455 of yacc.c  */
#line 2882 "Grammar.ypp"
    { 
    yyerror("Error in texture params declaration\n"); 
;}
    break;

  case 339:

/* Line 1455 of yacc.c  */
#line 2889 "Grammar.ypp"
    { (yyval.resourceType)=nvFX::RESOURCE_UNKNOWN; ;}
    break;

  case 340:

/* Line 1455 of yacc.c  */
#line 2890 "Grammar.ypp"
    { (yyval.resourceType)=nvFX::RESTEX_1D; ;}
    break;

  case 341:

/* Line 1455 of yacc.c  */
#line 2891 "Grammar.ypp"
    { (yyval.resourceType)=nvFX::RESTEX_2D; ;}
    break;

  case 342:

/* Line 1455 of yacc.c  */
#line 2892 "Grammar.ypp"
    { (yyval.resourceType)=nvFX::RESTEX_3D; ;}
    break;

  case 343:

/* Line 1455 of yacc.c  */
#line 2893 "Grammar.ypp"
    { (yyval.resourceType)=nvFX::RESTEX_2DRECT; ;}
    break;

  case 344:

/* Line 1455 of yacc.c  */
#line 2894 "Grammar.ypp"
    { (yyval.resourceType)=nvFX::RESTEX_CUBE_MAP; ;}
    break;

  case 345:

/* Line 1455 of yacc.c  */
#line 2896 "Grammar.ypp"
    {
    curResource = curContainer->createResource((yyvsp[(2) - (3)].s)->c_str(), (yyvsp[(1) - (3)].resourceType), true/*ADD AS nvFX token*/)->getExInterface();
    curResource->setGLTexture(0); // this will tell the resource it is made of some external resource (reference to an existing texture)
    lex_pop_state();
    delete (yyvsp[(2) - (3)].s);
    ;}
    break;

  case 346:

/* Line 1455 of yacc.c  */
#line 2902 "Grammar.ypp"
    {
    curResource = curContainer->createResource((yyvsp[(2) - (2)].s)->c_str(), (yyvsp[(1) - (2)].resourceType), true/*ADD AS nvFX token*/)->getExInterface();
    curResource->setGLTexture(0); // this will tell the resource it is made of some external resource (reference to an existing texture)
    curAnnotations = curResource->annotations()->getExInterface();
    delete (yyvsp[(2) - (2)].s);
;}
    break;

  case 347:

/* Line 1455 of yacc.c  */
#line 2907 "Grammar.ypp"
    { 
    // for the case of items within a namespace :
    // copy the name-space annotations to this one
    if(curAnnotations) curAnnotations->copy(namespaceAnnotations);
    curAnnotations = NULL;
    lex_pop_state(); lex_pop_state(); 
;}
    break;

  case 349:

/* Line 1455 of yacc.c  */
#line 2919 "Grammar.ypp"
    {
    if(curResource) {
        // for now, sampler-state MUST be decalred before the texture work. Or it won't be found
        ISamplerState * pState = curContainer->findSamplerState((yyvsp[(4) - (5)].s)->c_str());
        if(!pState)
            yyerror("Error: sampler-state referenced not found. Did you declare it before the texture using it ?\n");
        curResource->setSamplerState(pState);
    }
    delete (yyvsp[(4) - (5)].s);
;}
    break;

  case 350:

/* Line 1455 of yacc.c  */
#line 2929 "Grammar.ypp"
    {
    if(curResource)
        if(!curResource->annotations()->getExInterface()->addAnnotation((yyvsp[(2) - (5)].s)->c_str(), (yyvsp[(4) - (5)].s)->c_str()))
            yyerror("err\n");
    delete (yyvsp[(4) - (5)].s);
    ;}
    break;

  case 351:

/* Line 1455 of yacc.c  */
#line 2935 "Grammar.ypp"
    {
    if(curResource)
        curResource->setMSAAApplicationDependent();
;}
    break;

  case 352:

/* Line 1455 of yacc.c  */
#line 2939 "Grammar.ypp"
    {
    if(curResource)
        curResource->setMSAA((*(yyvsp[(5) - (7)].intList))[0], (yyvsp[(5) - (7)].intList)->size() > 1 ? (*(yyvsp[(5) - (7)].intList))[1] : 0);
    delete (yyvsp[(5) - (7)].intList);
;}
    break;

  case 353:

/* Line 1455 of yacc.c  */
#line 2944 "Grammar.ypp"
    {
    if(curResource)
        curResource->setMSAA((yyvsp[(4) - (5)].ival));
;}
    break;

  case 354:

/* Line 1455 of yacc.c  */
#line 2948 "Grammar.ypp"
    {
    if(curResource)
        curResource->setDimensionsApplicationDependent();
;}
    break;

  case 355:

/* Line 1455 of yacc.c  */
#line 2952 "Grammar.ypp"
    {
    int d = (int)(yyvsp[(5) - (7)].intList)->size();
    if(curResource)
        curResource->setDimensions((*(yyvsp[(5) - (7)].intList))[0], d > 1 ? (*(yyvsp[(5) - (7)].intList))[1] : 1, d > 2 ? (*(yyvsp[(5) - (7)].intList))[2] : 1);
    delete (yyvsp[(5) - (7)].intList);
;}
    break;

  case 356:

/* Line 1455 of yacc.c  */
#line 2958 "Grammar.ypp"
    {
    if(curResource)
        curResource->setDimensionFactor((yyvsp[(4) - (5)].fval), (yyvsp[(4) - (5)].fval));
;}
    break;

  case 357:

/* Line 1455 of yacc.c  */
#line 2962 "Grammar.ypp"
    {
    if(curResource) {
        IResource* pRes = curContainer->findResource((yyvsp[(4) - (5)].s)->c_str());
        if(!pRes)
        {
            yyerror("Error: Could not find the Resource :");
            yyerror((yyvsp[(4) - (5)].s)->c_str());
        } else {
            curResource->setResolveTarget(pRes);
        }
    }
    delete (yyvsp[(4) - (5)].s);
;}
    break;

  case 358:

/* Line 1455 of yacc.c  */
#line 2975 "Grammar.ypp"
    {
    if(curResource) {
        if(!curResource->setFormat((yyvsp[(4) - (5)].resource_format)))
        {
            yyerror("Error: Bad format\n");
        }
    }
;}
    break;

  case 359:

/* Line 1455 of yacc.c  */
#line 2987 "Grammar.ypp"
    {
    curResource = curContainer->createResource((yyvsp[(2) - (3)].s)->c_str(), RESTEX_2D, (yyvsp[(3) - (3)].bval))->getExInterface();
    if(curResource->getFormat() != FMT_UNKNOWN)
        curResource = NULL;
    else
        curAnnotations = curResource->annotations()->getExInterface();
    delete (yyvsp[(2) - (3)].s);
;}
    break;

  case 360:

/* Line 1455 of yacc.c  */
#line 2994 "Grammar.ypp"
    {
    // for the case of items within a namespace :
    // copy the name-space annotations to this one
    if(curAnnotations) curAnnotations->copy(namespaceAnnotations);
;}
    break;

  case 361:

/* Line 1455 of yacc.c  */
#line 2998 "Grammar.ypp"
    { 
    curAnnotations = NULL;
    lex_pop_state(); lex_pop_state(); 
;}
    break;

  case 362:

/* Line 1455 of yacc.c  */
#line 3002 "Grammar.ypp"
    {
    curResource = curContainer->createResource((yyvsp[(2) - (3)].s)->c_str(), RESTEX_2DRECT, (yyvsp[(3) - (3)].bval))->getExInterface();
    if(curResource->getFormat() != FMT_UNKNOWN)
        curResource = NULL;
    else
        curAnnotations = curResource->annotations()->getExInterface();
    delete (yyvsp[(2) - (3)].s);
;}
    break;

  case 363:

/* Line 1455 of yacc.c  */
#line 3009 "Grammar.ypp"
    {
    // for the case of items within a namespace :
    // copy the name-space annotations to this one
    if(curAnnotations) curAnnotations->copy(namespaceAnnotations);
;}
    break;

  case 364:

/* Line 1455 of yacc.c  */
#line 3013 "Grammar.ypp"
    { 
    curAnnotations = NULL;
    lex_pop_state(); lex_pop_state(); 
;}
    break;

  case 365:

/* Line 1455 of yacc.c  */
#line 3017 "Grammar.ypp"
    {
    curResource = curContainer->createResource((yyvsp[(2) - (3)].s)->c_str(), RESRBUF_2D, (yyvsp[(3) - (3)].bval))->getExInterface();
    if(curResource->getFormat() != FMT_UNKNOWN)
        curResource = NULL;
    else
        curAnnotations = curResource->annotations()->getExInterface();
    delete (yyvsp[(2) - (3)].s);
;}
    break;

  case 366:

/* Line 1455 of yacc.c  */
#line 3024 "Grammar.ypp"
    {
    // for the case of items within a namespace :
    // copy the name-space annotations to this one
    if(curAnnotations) curAnnotations->copy(namespaceAnnotations);
;}
    break;

  case 367:

/* Line 1455 of yacc.c  */
#line 3028 "Grammar.ypp"
    { 
    curAnnotations = NULL;
    lex_pop_state(); lex_pop_state(); 
;}
    break;

  case 369:

/* Line 1455 of yacc.c  */
#line 3037 "Grammar.ypp"
    {
    if(curFBO)
      if(!curFBO->annotations()->getExInterface()->addAnnotation((yyvsp[(2) - (5)].s)->c_str(), (yyvsp[(4) - (5)].s)->c_str()))
        yyerror("err\n");
    delete (yyvsp[(4) - (5)].s);
    ;}
    break;

  case 370:

/* Line 1455 of yacc.c  */
#line 3046 "Grammar.ypp"
    {
    if(curFBO)
    {
        IResource* pRes = curContainer->findResource((yyvsp[(4) - (5)].s)->c_str());
        if(!pRes)
        {
            yyerror("Error: Could not find the Texture resource :");
            yyerror((yyvsp[(4) - (5)].s)->c_str());
        } else {
            curFBO->setDSTResource(pRes);
        }
    }
    delete (yyvsp[(4) - (5)].s);
;}
    break;

  case 371:

/* Line 1455 of yacc.c  */
#line 3060 "Grammar.ypp"
    { /*liststart VARNAME ',' VARNAME ',' VARNAME ',' VARNAME listend ';' { */
    if(curFBO) {
        IResource* pRes[4/*MAX_RENDERTARGETS*/];
        if((yyvsp[(5) - (7)].varNameList)->size() > 4)
            yyerror("Error: Can't assign more than 4 color targets in a Framebuffer");
        else {
            bool ok = true;
            for(int i=0; i<(int)(yyvsp[(5) - (7)].varNameList)->size(); i++)
            {
                pRes[i] = curContainer->findResource((*(yyvsp[(5) - (7)].varNameList))[i].c_str());
                if(pRes[i]==NULL)
                {
                    yyerror("Error: Could not find some of the Textures");
                    ok = false;
                }
            }
            if(ok) curFBO->setColorResource(pRes, (int)(yyvsp[(5) - (7)].varNameList)->size());
        }
    }
    delete (yyvsp[(5) - (7)].varNameList);
;}
    break;

  case 372:

/* Line 1455 of yacc.c  */
#line 3081 "Grammar.ypp"
    {
    if(curFBO) {
        IResource* pRes = curContainer->findResource((yyvsp[(4) - (5)].s)->c_str());
        if(!pRes)
        {
            yyerror("Error: Could not find the Texture :");
            yyerror((yyvsp[(4) - (5)].s)->c_str());
        } else {
            curFBO->setColorResource(&pRes, 1);
        }
    }
    delete (yyvsp[(4) - (5)].s);
;}
    break;

  case 373:

/* Line 1455 of yacc.c  */
#line 3098 "Grammar.ypp"
    {
    curFBO = curContainer->createFBO((yyvsp[(2) - (3)].s)->c_str(), (yyvsp[(3) - (3)].bval))->getExInterface();
    if(curFBO->getDSTResource() || curFBO->getColorResource(0))
        curFBO = NULL; // we already have it defined. Don't want to re-define it again
    else
        curAnnotations = curFBO->annotations()->getExInterface();
    delete (yyvsp[(2) - (3)].s);
;}
    break;

  case 374:

/* Line 1455 of yacc.c  */
#line 3105 "Grammar.ypp"
    {
    // for the case of items within a namespace :
    // copy the name-space annotations to this one
    if(curAnnotations) curAnnotations->copy(namespaceAnnotations);
;}
    break;

  case 375:

/* Line 1455 of yacc.c  */
#line 3109 "Grammar.ypp"
    { 
    curAnnotations = NULL;
    lex_pop_state(); lex_pop_state(); // fbo and then fbo1 state
;}
    break;

  case 382:

/* Line 1455 of yacc.c  */
#line 3124 "Grammar.ypp"
    { yyerror("Error in Sampler State filtering\n");;}
    break;

  case 387:

/* Line 1455 of yacc.c  */
#line 3131 "Grammar.ypp"
    { yyerror("Error in Sampler State UV Addressing\n");;}
    break;

  case 391:

/* Line 1455 of yacc.c  */
#line 3137 "Grammar.ypp"
    { yyerror("Error in Sampler State depth texture mode\n");;}
    break;

  case 400:

/* Line 1455 of yacc.c  */
#line 3148 "Grammar.ypp"
    { yyerror("Error in Sampler State compare Fn\n");;}
    break;

  case 403:

/* Line 1455 of yacc.c  */
#line 3153 "Grammar.ypp"
    { yyerror("Error in Sampler State compare mode\n");;}
    break;

  case 405:

/* Line 1455 of yacc.c  */
#line 3160 "Grammar.ypp"
    {
    curSamplerState->getExInterface()->createState(getGLEnumValue((yyvsp[(2) - (5)].glenum)), getGLEnumValue((yyvsp[(4) - (5)].glenum)));
;}
    break;

  case 406:

/* Line 1455 of yacc.c  */
#line 3163 "Grammar.ypp"
    {
    curSamplerState->getExInterface()->createState(getGLEnumValue((yyvsp[(2) - (5)].glenum)), getGLEnumValue((yyvsp[(4) - (5)].glenum)));
;}
    break;

  case 407:

/* Line 1455 of yacc.c  */
#line 3166 "Grammar.ypp"
    {
    curSamplerState->getExInterface()->createState(getGLEnumValue((yyvsp[(2) - (5)].glenum)), getGLEnumValue((yyvsp[(4) - (5)].glenum)));
;}
    break;

  case 408:

/* Line 1455 of yacc.c  */
#line 3169 "Grammar.ypp"
    {
    curSamplerState->getExInterface()->createState(getGLEnumValue((yyvsp[(2) - (5)].glenum)), getGLEnumValue((yyvsp[(4) - (5)].glenum)));
;}
    break;

  case 409:

/* Line 1455 of yacc.c  */
#line 3172 "Grammar.ypp"
    {
    curSamplerState->getExInterface()->createState(getGLEnumValue((yyvsp[(2) - (5)].glenum)), getGLEnumValue((yyvsp[(4) - (5)].glenum)));
;}
    break;

  case 410:

/* Line 1455 of yacc.c  */
#line 3175 "Grammar.ypp"
    {
    curSamplerState->getExInterface()->createState(getGLEnumValue((yyvsp[(2) - (5)].glenum)), getGLEnumValue((yyvsp[(4) - (5)].glenum)));
;}
    break;

  case 411:

/* Line 1455 of yacc.c  */
#line 3178 "Grammar.ypp"
    {
    curSamplerState->getExInterface()->createState(getGLEnumValue((yyvsp[(2) - (5)].glenum)), getGLEnumValue((yyvsp[(4) - (5)].glenum)));
;}
    break;

  case 412:

/* Line 1455 of yacc.c  */
#line 3181 "Grammar.ypp"
    {
    curSamplerState->getExInterface()->createState(getGLEnumValue((yyvsp[(2) - (5)].glenum)), getGLEnumValue((yyvsp[(4) - (5)].glenum)));
;}
    break;

  case 413:

/* Line 1455 of yacc.c  */
#line 3184 "Grammar.ypp"
    { yyerror("Error in one of sampler-state states\n"); ;}
    break;

  case 414:

/* Line 1455 of yacc.c  */
#line 3189 "Grammar.ypp"
    {
    LOGD("Sampler-state %s\n", (yyvsp[(2) - (2)].s)->c_str() );
    curSamplerState = curContainer->createSamplerState((yyvsp[(2) - (2)].s)->c_str())->getExInterface();
    curAnnotations = curSamplerState->annotations()->getExInterface();
    delete (yyvsp[(2) - (2)].s);
;}
    break;

  case 415:

/* Line 1455 of yacc.c  */
#line 3194 "Grammar.ypp"
    { 
    // for the case of items within a namespace :
    // copy the name-space annotations to this one
    if(curAnnotations) curAnnotations->copy(namespaceAnnotations);
    lex_pop_state(); curAnnotations = NULL;;}
    break;

  case 416:

/* Line 1455 of yacc.c  */
#line 3199 "Grammar.ypp"
    { 
    yyerror("Error sampler-state declaration\n"); 
;}
    break;

  case 417:

/* Line 1455 of yacc.c  */
#line 3214 "Grammar.ypp"
    { check_curRState(); if(!curRState->createState(getGLEnumValue((yyvsp[(1) - (4)].glenum)), ((yyvsp[(3) - (4)].fval)))) yyerror("Failed to create state!\n"); ;}
    break;

  case 418:

/* Line 1455 of yacc.c  */
#line 3215 "Grammar.ypp"
    { check_curRState(); if(!curRState->createState(getGLEnumValue((yyvsp[(1) - (4)].glenum)), (GLenum)(((yyvsp[(3) - (4)].bval) )? GL_TRUE: GL_FALSE))) yyerror("Failed to create state!\n"); ;}
    break;

  case 419:

/* Line 1455 of yacc.c  */
#line 3216 "Grammar.ypp"
    { check_curRState(); if(!curRState->createState(getGLEnumValue((yyvsp[(1) - (4)].glenum)), (GLenum)(((yyvsp[(3) - (4)].bval) )? GL_TRUE: GL_FALSE))) yyerror("Failed to create state!\n"); ;}
    break;

  case 420:

/* Line 1455 of yacc.c  */
#line 3217 "Grammar.ypp"
    { check_curRState(); if(!curRState->createState(getGLEnumValue((yyvsp[(1) - (4)].glenum)), (GLenum)(((yyvsp[(3) - (4)].bval) )? GL_TRUE: GL_FALSE))) yyerror("Failed to create state!\n"); ;}
    break;

  case 421:

/* Line 1455 of yacc.c  */
#line 3218 "Grammar.ypp"
    { check_curRState(); if(!curRState->createState(getGLEnumValue((yyvsp[(1) - (4)].glenum)), getGLEnumValue((yyvsp[(3) - (4)].glenum)))) yyerror("Failed to create state!\n"); ;}
    break;

  case 422:

/* Line 1455 of yacc.c  */
#line 3219 "Grammar.ypp"
    { check_curRState(); if(!curRState->createState(getGLEnumValue((yyvsp[(1) - (4)].glenum)), ((yyvsp[(3) - (4)].fval)))) yyerror("Failed to create state!\n"); ;}
    break;

  case 423:

/* Line 1455 of yacc.c  */
#line 3220 "Grammar.ypp"
    { check_curRState(); if(!curRState->createState(getGLEnumValue((yyvsp[(1) - (4)].glenum)), (GLenum)(((yyvsp[(3) - (4)].bval) )? GL_TRUE: GL_FALSE))) yyerror("Failed to create state!\n"); ;}
    break;

  case 424:

/* Line 1455 of yacc.c  */
#line 3222 "Grammar.ypp"
    { check_curRState(); if(!curRState->createState(getGLEnumValue((yyvsp[(1) - (4)].glenum)), (unsigned int)(yyvsp[(3) - (4)].ival))) yyerror("Failed to create state!\n"); ;}
    break;

  case 425:

/* Line 1455 of yacc.c  */
#line 3223 "Grammar.ypp"
    { check_curRState(); if(!curRState->createState(getGLEnumValue((yyvsp[(1) - (4)].glenum)), (unsigned int)(yyvsp[(3) - (4)].ival))) yyerror("Failed to create state!\n"); ;}
    break;

  case 426:

/* Line 1455 of yacc.c  */
#line 3224 "Grammar.ypp"
    { check_curRState(); if(!curRState->createState(getGLEnumValue((yyvsp[(1) - (4)].glenum)), (GLenum)((yyvsp[(3) - (4)].bval) ? GL_TRUE: GL_FALSE))) yyerror("Failed to create state!\n"); ;}
    break;

  case 427:

/* Line 1455 of yacc.c  */
#line 3225 "Grammar.ypp"
    { check_curRState(); if(!curRState->createState(getGLEnumValue((yyvsp[(1) - (4)].glenum)), (GLenum)((yyvsp[(3) - (4)].bval) ? GL_TRUE: GL_FALSE))) yyerror("Failed to create state!\n"); ;}
    break;

  case 428:

/* Line 1455 of yacc.c  */
#line 3226 "Grammar.ypp"
    { check_curRState(); if(!curRState->createState(getGLEnumValue((yyvsp[(1) - (4)].glenum)), getGLEnumValue((yyvsp[(3) - (4)].glenum)))) yyerror("Failed to create state!\n"); ;}
    break;

  case 429:

/* Line 1455 of yacc.c  */
#line 3227 "Grammar.ypp"
    { check_curRState(); if(!curRState->createState(getGLEnumValue((yyvsp[(1) - (4)].glenum)), getGLEnumValue((yyvsp[(3) - (4)].glenum)))) yyerror("Failed to create state!\n"); ;}
    break;

  case 430:

/* Line 1455 of yacc.c  */
#line 3228 "Grammar.ypp"
    { check_curRState(); if(!curRState->createState(getGLEnumValue((yyvsp[(1) - (4)].glenum)), getGLEnumValue((yyvsp[(3) - (4)].glenum)))) yyerror("Failed to create state!\n"); ;}
    break;

  case 431:

/* Line 1455 of yacc.c  */
#line 3229 "Grammar.ypp"
    { check_curRState(); if(!curRState->createState(getGLEnumValue((yyvsp[(1) - (4)].glenum)), (GLenum)((yyvsp[(3) - (4)].bval) ? GL_TRUE: GL_FALSE))) yyerror("Failed to create state!\n"); ;}
    break;

  case 432:

/* Line 1455 of yacc.c  */
#line 3230 "Grammar.ypp"
    { check_curRState(); if(!curRState->createState(getGLEnumValue((yyvsp[(1) - (8)].glenum)), getGLEnumValue((yyvsp[(4) - (8)].glenum)), getGLEnumValue((yyvsp[(6) - (8)].glenum)))) yyerror("Failed to create state!\n"); ;}
    break;

  case 433:

/* Line 1455 of yacc.c  */
#line 3231 "Grammar.ypp"
    { check_curRState(); if(!curRState->createState(getGLEnumValue((yyvsp[(1) - (4)].glenum)), (yyvsp[(3) - (4)].fval))) yyerror("Failed to create state!\n"); ;}
    break;

  case 434:

/* Line 1455 of yacc.c  */
#line 3232 "Grammar.ypp"
    { check_curRState(); if(!curRState->createState(getGLEnumValue((yyvsp[(1) - (4)].glenum)), (yyvsp[(3) - (4)].fval))) yyerror("Failed to create state!\n"); ;}
    break;

  case 435:

/* Line 1455 of yacc.c  */
#line 3233 "Grammar.ypp"
    {
    check_curRState();
    if(!curRState->createState(getGLEnumValue(GLPOLYGONOFFSETFACTOR), (yyvsp[(4) - (8)].fval))) yyerror("Failed to create state!\n");
    if(!curRState->createState(getGLEnumValue(GLPOLYGONOFFSETUNITS), (yyvsp[(6) - (8)].fval))) yyerror("Failed to create state!\n");
;}
    break;

  case 436:

/* Line 1455 of yacc.c  */
#line 3238 "Grammar.ypp"
    { check_curRState(); if(!curRState->createState(getGLEnumValue((yyvsp[(1) - (4)].glenum)), (GLenum)((yyvsp[(3) - (4)].bval) ? GL_TRUE: GL_FALSE))) yyerror("Failed to create state!\n"); ;}
    break;

  case 437:

/* Line 1455 of yacc.c  */
#line 3239 "Grammar.ypp"
    { check_curRState(); if(!curRState->createState(getGLEnumValue((yyvsp[(1) - (4)].glenum)), (GLenum)((yyvsp[(3) - (4)].bval) ? GL_TRUE: GL_FALSE))) yyerror("Failed to create state!\n"); ;}
    break;

  case 438:

/* Line 1455 of yacc.c  */
#line 3240 "Grammar.ypp"
    { check_curRState(); if(!curRState->createState(getGLEnumValue((yyvsp[(1) - (4)].glenum)), (GLenum)((yyvsp[(3) - (4)].bval) ? GL_TRUE: GL_FALSE))) yyerror("Failed to create state!\n"); ;}
    break;

  case 439:

/* Line 1455 of yacc.c  */
#line 3242 "Grammar.ypp"
    { check_curRState(); if(!curRState->createState(getGLEnumValue((yyvsp[(1) - (4)].glenum)), (GLenum)((yyvsp[(3) - (4)].bval) ? GL_TRUE: GL_FALSE))) yyerror("Failed to create state!\n"); ;}
    break;

  case 440:

/* Line 1455 of yacc.c  */
#line 3243 "Grammar.ypp"
    { check_curCSState(); if(!curCSState->createState(getGLEnumValue((yyvsp[(1) - (4)].glenum)), (GLenum)((yyvsp[(3) - (4)].bval) ? GL_TRUE: GL_FALSE))) yyerror("Failed to create state!\n"); ;}
    break;

  case 441:

/* Line 1455 of yacc.c  */
#line 3244 "Grammar.ypp"
    { check_curCSState(); if(!curCSState->createState(getGLEnumValue((yyvsp[(1) - (4)].glenum)), getGLEnumValue((yyvsp[(3) - (4)].glenum)))) yyerror("Failed to create state!\n"); ;}
    break;

  case 442:

/* Line 1455 of yacc.c  */
#line 3245 "Grammar.ypp"
    { check_curCSState(); if(!curCSState->createState(getGLEnumValue((yyvsp[(1) - (4)].glenum)), (yyvsp[(3) - (4)].fval))) yyerror("Failed to create state!\n"); ;}
    break;

  case 443:

/* Line 1455 of yacc.c  */
#line 3246 "Grammar.ypp"
    {
    check_curCSState();
    if(!curCSState->createState(getGLEnumValue(GLALPHATESTFUNC), getGLEnumValue((yyvsp[(4) - (8)].glenum)))) yyerror("Failed to create state!\n");
    if(!curCSState->createState(getGLEnumValue(GLALPHATESTREF), (yyvsp[(6) - (8)].fval))) yyerror("Failed to create state!\n");
;}
    break;

  case 444:

/* Line 1455 of yacc.c  */
#line 3251 "Grammar.ypp"
    { check_curCSState(); if(!curCSState->createState(getGLEnumValue((yyvsp[(1) - (4)].glenum)), (GLenum)((yyvsp[(3) - (4)].bval) ? GL_TRUE: GL_FALSE))) yyerror("Failed to create state!\n"); ;}
    break;

  case 445:

/* Line 1455 of yacc.c  */
#line 3252 "Grammar.ypp"
    {
    check_curCSState();
    if(!curCSState->createState(getGLEnumValue(GLBLENDSRC), getGLEnumValue((yyvsp[(4) - (8)].glenum)))) yyerror("Failed to create state!\n");
    if(!curCSState->createState(getGLEnumValue(GLBLENDDST), getGLEnumValue((yyvsp[(6) - (8)].glenum)))) yyerror("Failed to create state!\n");
;}
    break;

  case 446:

/* Line 1455 of yacc.c  */
#line 3257 "Grammar.ypp"
    {
    check_curCSState();
    if(!curCSState->createState(getGLEnumValue(GLBLENDSRCRGB), getGLEnumValue((yyvsp[(4) - (12)].glenum)))) yyerror("Failed to create state!\n");
    if(!curCSState->createState(getGLEnumValue(GLBLENDDSTRGB), getGLEnumValue((yyvsp[(6) - (12)].glenum)))) yyerror("Failed to create state!\n");
    if(!curCSState->createState(getGLEnumValue(GLBLENDSRCALPHA), getGLEnumValue((yyvsp[(8) - (12)].glenum)))) yyerror("Failed to create state!\n");
    if(!curCSState->createState(getGLEnumValue(GLBLENDDSTALPHA), getGLEnumValue((yyvsp[(10) - (12)].glenum)))) yyerror("Failed to create state!\n");
;}
    break;

  case 447:

/* Line 1455 of yacc.c  */
#line 3264 "Grammar.ypp"
    { check_curCSState(); if(!curCSState->createState(getGLEnumValue((yyvsp[(1) - (4)].glenum)), getGLEnumValue((yyvsp[(3) - (4)].glenum)))) yyerror("Failed to create state!\n"); ;}
    break;

  case 448:

/* Line 1455 of yacc.c  */
#line 3265 "Grammar.ypp"
    { check_curCSState(); if(!curCSState->createState(getGLEnumValue((yyvsp[(1) - (4)].glenum)), getGLEnumValue((yyvsp[(3) - (4)].glenum)))) yyerror("Failed to create state!\n"); ;}
    break;

  case 449:

/* Line 1455 of yacc.c  */
#line 3266 "Grammar.ypp"
    { check_curCSState(); if(!curCSState->createState(getGLEnumValue((yyvsp[(1) - (4)].glenum)), getGLEnumValue((yyvsp[(3) - (4)].glenum)))) yyerror("Failed to create state!\n"); ;}
    break;

  case 450:

/* Line 1455 of yacc.c  */
#line 3267 "Grammar.ypp"
    { check_curCSState(); if(!curCSState->createState(getGLEnumValue((yyvsp[(1) - (4)].glenum)), getGLEnumValue((yyvsp[(3) - (4)].glenum)))) yyerror("Failed to create state!\n"); ;}
    break;

  case 451:

/* Line 1455 of yacc.c  */
#line 3268 "Grammar.ypp"
    { check_curCSState(); if(!curCSState->createState(getGLEnumValue((yyvsp[(1) - (4)].glenum)), getGLEnumValue((yyvsp[(3) - (4)].glenum)))) yyerror("Failed to create state!\n"); ;}
    break;

  case 452:

/* Line 1455 of yacc.c  */
#line 3269 "Grammar.ypp"
    { check_curCSState(); if(!curCSState->createState(getGLEnumValue((yyvsp[(1) - (4)].glenum)), getGLEnumValue((yyvsp[(3) - (4)].glenum)))) yyerror("Failed to create state!\n"); ;}
    break;

  case 453:

/* Line 1455 of yacc.c  */
#line 3271 "Grammar.ypp"
    { check_curCSState(); if(!curCSState->createState(getGLEnumValue((yyvsp[(1) - (4)].glenum)), getGLEnumValue((yyvsp[(3) - (4)].glenum)))) yyerror("Failed to create state!\n"); ;}
    break;

  case 454:

/* Line 1455 of yacc.c  */
#line 3272 "Grammar.ypp"
    { check_curCSState(); if(!curCSState->createState(getGLEnumValue((yyvsp[(1) - (4)].glenum)), getGLEnumValue((yyvsp[(3) - (4)].glenum)))) yyerror("Failed to create state!\n"); ;}
    break;

  case 455:

/* Line 1455 of yacc.c  */
#line 3273 "Grammar.ypp"
    {
    check_curCSState();
    if(!curCSState->createState(getGLEnumValue(GLBLENDEQUATIONRGB), getGLEnumValue((yyvsp[(3) - (4)].glenum)))) yyerror("Failed to create state!\n");
    if(!curCSState->createState(getGLEnumValue(GLBLENDEQUATIONALPHA), getGLEnumValue((yyvsp[(3) - (4)].glenum)))) yyerror("Failed to create state!\n");
;}
    break;

  case 456:

/* Line 1455 of yacc.c  */
#line 3278 "Grammar.ypp"
    {
    check_curCSState();
    if(!curCSState->createState(getGLEnumValue(GLBLENDEQUATIONRGB), getGLEnumValue((yyvsp[(4) - (8)].glenum)))) yyerror("Failed to create state!\n");
    if(!curCSState->createState(getGLEnumValue(GLBLENDEQUATIONALPHA), getGLEnumValue((yyvsp[(6) - (8)].glenum)))) yyerror("Failed to create state!\n");
;}
    break;

  case 457:

/* Line 1455 of yacc.c  */
#line 3288 "Grammar.ypp"
    { check_curCSState(); if(!curCSState->createState(getGLEnumValue((yyvsp[(1) - (12)].glenum)), (yyvsp[(4) - (12)].fval), (yyvsp[(6) - (12)].fval), (yyvsp[(8) - (12)].fval), (yyvsp[(10) - (12)].fval))) yyerror("Failed to create state!\n"); ;}
    break;

  case 458:

/* Line 1455 of yacc.c  */
#line 3289 "Grammar.ypp"
    { check_curCSState(); if(!curCSState->createState(getGLEnumValue((yyvsp[(1) - (4)].glenum)), (GLenum)((yyvsp[(3) - (4)].bval) ? GL_TRUE: GL_FALSE))) yyerror("Failed to create state!\n"); ;}
    break;

  case 459:

/* Line 1455 of yacc.c  */
#line 3290 "Grammar.ypp"
    { check_curCSState(); if(!curCSState->createState(getGLEnumValue((yyvsp[(1) - (4)].glenum)), (GLenum)((yyvsp[(3) - (4)].bval) ? GL_TRUE: GL_FALSE))) yyerror("Failed to create state!\n"); ;}
    break;

  case 460:

/* Line 1455 of yacc.c  */
#line 3291 "Grammar.ypp"
    { check_curCSState(); if(!curCSState->createState(getGLEnumValue((yyvsp[(1) - (4)].glenum)), (GLenum)((yyvsp[(3) - (4)].bval) ? GL_TRUE: GL_FALSE))) yyerror("Failed to create state!\n"); ;}
    break;

  case 461:

/* Line 1455 of yacc.c  */
#line 3292 "Grammar.ypp"
    { check_curCSState(); if(!curCSState->createState(getGLEnumValue((yyvsp[(1) - (4)].glenum)), getGLEnumValue((yyvsp[(3) - (4)].glenum)))) yyerror("Failed to create state!\n"); ;}
    break;

  case 462:

/* Line 1455 of yacc.c  */
#line 3294 "Grammar.ypp"
    { check_curCSState(); if(!curCSState->createState(getGLEnumValue((yyvsp[(1) - (12)].glenum)), 
    (GLboolean)((yyvsp[(4) - (12)].bval) ? GL_TRUE: GL_FALSE),
    (GLboolean)((yyvsp[(6) - (12)].bval) ? GL_TRUE: GL_FALSE),
    (GLboolean)((yyvsp[(8) - (12)].bval) ? GL_TRUE: GL_FALSE),
    (GLboolean)((yyvsp[(10) - (12)].bval) ? GL_TRUE: GL_FALSE)
    )) yyerror("Failed to create state!\n"); ;}
    break;

  case 463:

/* Line 1455 of yacc.c  */
#line 3300 "Grammar.ypp"
    { check_curCSState(); if(!curCSState->createState(getGLEnumValue((yyvsp[(1) - (4)].glenum)), (GLenum)((yyvsp[(3) - (4)].bval) ? GL_TRUE: GL_FALSE))) yyerror("Failed to create state!\n"); ;}
    break;

  case 464:

/* Line 1455 of yacc.c  */
#line 3301 "Grammar.ypp"
    { check_curCSState(); if(!curCSState->createState(getGLEnumValue((yyvsp[(1) - (4)].glenum)), (GLenum)((yyvsp[(3) - (4)].bval) ? GL_TRUE: GL_FALSE))) yyerror("Failed to create state!\n"); ;}
    break;

  case 465:

/* Line 1455 of yacc.c  */
#line 3303 "Grammar.ypp"
    { check_curCSState(); if(!curCSState->createState(getGLEnumValue((yyvsp[(1) - (4)].glenum)), (GLenum)((yyvsp[(3) - (4)].bval) ? GL_TRUE: GL_FALSE))) yyerror("Failed to create state!\n"); ;}
    break;

  case 466:

/* Line 1455 of yacc.c  */
#line 3304 "Grammar.ypp"
    { check_curCSState(); if(!curCSState->createState(getGLEnumValue((yyvsp[(1) - (4)].glenum)), (GLenum)((yyvsp[(3) - (4)].bval) ? GL_TRUE: GL_FALSE))) yyerror("Failed to create state!\n"); ;}
    break;

  case 467:

/* Line 1455 of yacc.c  */
#line 3305 "Grammar.ypp"
    { check_curDSTState();if(!curDSTState->createState(getGLEnumValue((yyvsp[(1) - (4)].glenum)), (GLenum)((yyvsp[(3) - (4)].bval) ? GL_TRUE: GL_FALSE))) yyerror("Failed to create state!\n"); ;}
    break;

  case 468:

/* Line 1455 of yacc.c  */
#line 3306 "Grammar.ypp"
    { check_curDSTState(); if(!curDSTState->createState(getGLEnumValue((yyvsp[(1) - (4)].glenum)), getGLEnumValue((yyvsp[(3) - (4)].glenum)))) yyerror("Failed to create state!\n"); ;}
    break;

  case 469:

/* Line 1455 of yacc.c  */
#line 3307 "Grammar.ypp"
    { check_curDSTState(); if(!curDSTState->createState(getGLEnumValue((yyvsp[(1) - (4)].glenum)), (GLenum)((yyvsp[(3) - (4)].bval) ? GL_TRUE: GL_FALSE))) yyerror("Failed to create state!\n"); ;}
    break;

  case 470:

/* Line 1455 of yacc.c  */
#line 3308 "Grammar.ypp"
    { check_curDSTState(); if(!curDSTState->createState(getGLEnumValue((yyvsp[(1) - (4)].glenum)), (GLenum)((yyvsp[(3) - (4)].bval) ? GL_TRUE: GL_FALSE))) yyerror("Failed to create state!\n"); ;}
    break;

  case 471:

/* Line 1455 of yacc.c  */
#line 3309 "Grammar.ypp"
    { check_curDSTState(); if(!curDSTState->createState(getGLEnumValue((yyvsp[(1) - (4)].glenum)), (GLenum)((yyvsp[(3) - (4)].bval) ? GL_TRUE: GL_FALSE))) yyerror("Failed to create state!\n"); ;}
    break;

  case 472:

/* Line 1455 of yacc.c  */
#line 3311 "Grammar.ypp"
    { check_curDSTState(); if(!curDSTState->createState(getGLEnumValue((yyvsp[(1) - (8)].glenum)), (yyvsp[(4) - (8)].fval), (yyvsp[(6) - (8)].fval))) yyerror("Failed to create state!\n"); ;}
    break;

  case 473:

/* Line 1455 of yacc.c  */
#line 3312 "Grammar.ypp"
    { check_curDSTState(); if(!curDSTState->createState(getGLEnumValue((yyvsp[(1) - (4)].glenum)), (GLenum)((yyvsp[(3) - (4)].bval) ? GL_TRUE: GL_FALSE))) yyerror("Failed to create state!\n"); ;}
    break;

  case 474:

/* Line 1455 of yacc.c  */
#line 3313 "Grammar.ypp"
    { check_curDSTState(); if(!curDSTState->createState(getGLEnumValue((yyvsp[(1) - (4)].glenum)), getGLEnumValue((yyvsp[(3) - (4)].glenum)))) yyerror("Failed to create state!\n"); ;}
    break;

  case 475:

/* Line 1455 of yacc.c  */
#line 3314 "Grammar.ypp"
    { 
    check_curDSTState();
    if(!curDSTState->createState(getGLEnumValue((yyvsp[(1) - (10)].glenum)), getGLEnumValue((yyvsp[(4) - (10)].glenum)))) yyerror("Failed to create state!\n");
    if(!curDSTState->createState(getGLEnumValue(GLSTENCILREF), (unsigned int)(yyvsp[(6) - (10)].ival))) yyerror("Failed to create state!\n");
    if(!curDSTState->createState(getGLEnumValue(GLSTENCILVALUEMASK), (unsigned int)(yyvsp[(8) - (10)].ival))) yyerror("Failed to create state!\n");
;}
    break;

  case 476:

/* Line 1455 of yacc.c  */
#line 3320 "Grammar.ypp"
    { 
    check_curDSTState();
    if(!curDSTState->createState(getGLEnumValue((yyvsp[(4) - (12)].glenum) == GLFRONT ? GLSTENCILFUNC : GLSTENCILBACKFUNC), getGLEnumValue((yyvsp[(6) - (12)].glenum)))) yyerror("Failed to create state!\n");
    if(!curDSTState->createState(getGLEnumValue((yyvsp[(4) - (12)].glenum) == GLFRONT ? GLSTENCILREF : GLSTENCILBACKREF), (unsigned int)(yyvsp[(8) - (12)].ival))) yyerror("Failed to create state!\n");
    if(!curDSTState->createState(getGLEnumValue((yyvsp[(4) - (12)].glenum) == GLFRONT ? GLSTENCILVALUEMASK : GLSTENCILBACKVALUEMASK), (unsigned int)(yyvsp[(10) - (12)].ival))) yyerror("Failed to create state!\n");
;}
    break;

  case 477:

/* Line 1455 of yacc.c  */
#line 3326 "Grammar.ypp"
    { check_curDSTState(); if(!curDSTState->createState(getGLEnumValue((yyvsp[(1) - (4)].glenum)), (unsigned int)(yyvsp[(3) - (4)].ival))) yyerror("Failed to create state!\n"); ;}
    break;

  case 478:

/* Line 1455 of yacc.c  */
#line 3327 "Grammar.ypp"
    { check_curDSTState(); if(!curDSTState->createState(getGLEnumValue((yyvsp[(1) - (4)].glenum)), (unsigned int)(yyvsp[(3) - (4)].ival))) yyerror("Failed to create state!\n"); ;}
    break;

  case 479:

/* Line 1455 of yacc.c  */
#line 3328 "Grammar.ypp"
    { check_curDSTState(); if(!curDSTState->createState(getGLEnumValue((yyvsp[(1) - (4)].glenum)), (unsigned int)(yyvsp[(3) - (4)].ival))) yyerror("Failed to create state!\n"); ;}
    break;

  case 480:

/* Line 1455 of yacc.c  */
#line 3329 "Grammar.ypp"
    { check_curDSTState(); if(!curDSTState->createState(getGLEnumValue((yyvsp[(1) - (4)].glenum)), getGLEnumValue((yyvsp[(3) - (4)].glenum)))) yyerror("Failed to create state!\n"); ;}
    break;

  case 481:

/* Line 1455 of yacc.c  */
#line 3330 "Grammar.ypp"
    { check_curDSTState(); if(!curDSTState->createState(getGLEnumValue((yyvsp[(1) - (4)].glenum)), getGLEnumValue((yyvsp[(3) - (4)].glenum)))) yyerror("Failed to create state!\n"); ;}
    break;

  case 482:

/* Line 1455 of yacc.c  */
#line 3331 "Grammar.ypp"
    { check_curDSTState(); if(!curDSTState->createState(getGLEnumValue((yyvsp[(1) - (4)].glenum)), getGLEnumValue((yyvsp[(3) - (4)].glenum)))) yyerror("Failed to create state!\n"); ;}
    break;

  case 483:

/* Line 1455 of yacc.c  */
#line 3332 "Grammar.ypp"
    {
    // a concatenated version of GLSTENCILPASSDEPTHFAIL+GLSTENCILPASSDEPTHPASS+GLSTENCILWRITEMASK == GL function arguments
    check_curDSTState();
    if(!curDSTState->createState(getGLEnumValue(GLSTENCILFAIL), getGLEnumValue((yyvsp[(4) - (10)].glenum)))) yyerror("Failed to create state!\n");
    if(!curDSTState->createState(getGLEnumValue(GLSTENCILPASSDEPTHFAIL), getGLEnumValue((yyvsp[(6) - (10)].glenum)))) yyerror("Failed to create state!\n");
    if(!curDSTState->createState(getGLEnumValue(GLSTENCILPASSDEPTHPASS), getGLEnumValue((yyvsp[(8) - (10)].glenum)))) yyerror("Failed to create state!\n");
;}
    break;

  case 484:

/* Line 1455 of yacc.c  */
#line 3339 "Grammar.ypp"
    { 
    check_curDSTState();
    if(!curDSTState->createState(getGLEnumValue((yyvsp[(4) - (12)].glenum) == GLFRONT ? GLSTENCILFAIL : GLSTENCILBACKFAIL), getGLEnumValue((yyvsp[(6) - (12)].glenum)))) yyerror("Failed to create state!\n");
    if(!curDSTState->createState(getGLEnumValue((yyvsp[(4) - (12)].glenum) == GLFRONT ? GLSTENCILPASSDEPTHFAIL : GLSTENCILBACKPASSDEPTHFAIL), (unsigned int)(yyvsp[(8) - (12)].glenum))) yyerror("Failed to create state!\n");
    if(!curDSTState->createState(getGLEnumValue((yyvsp[(4) - (12)].glenum) == GLFRONT ? GLSTENCILPASSDEPTHPASS : GLSTENCILBACKPASSDEPTHPASS), (unsigned int)(yyvsp[(10) - (12)].glenum))) yyerror("Failed to create state!\n");
;}
    break;

  case 485:

/* Line 1455 of yacc.c  */
#line 3352 "Grammar.ypp"
    { check_curDSTState(); if(!curDSTState->createState(getGLEnumValue((yyvsp[(1) - (4)].glenum)), getGLEnumValue((yyvsp[(3) - (4)].glenum)))) yyerror("Failed to create state!\n"); ;}
    break;

  case 486:

/* Line 1455 of yacc.c  */
#line 3353 "Grammar.ypp"
    { 
    check_curDSTState();
    if(!curDSTState->createState(getGLEnumValue((yyvsp[(1) - (10)].glenum)), getGLEnumValue((yyvsp[(4) - (10)].glenum)))) yyerror("Failed to create state!\n");
    if(!curDSTState->createState(getGLEnumValue(GLSTENCILBACKREF), (unsigned int)(yyvsp[(6) - (10)].ival))) yyerror("Failed to create state!\n");
    if(!curDSTState->createState(getGLEnumValue(GLSTENCILBACKVALUEMASK), (unsigned int)(yyvsp[(8) - (10)].ival))) yyerror("Failed to create state!\n");
;}
    break;

  case 487:

/* Line 1455 of yacc.c  */
#line 3359 "Grammar.ypp"
    { check_curDSTState(); if(!curDSTState->createState(getGLEnumValue((yyvsp[(1) - (4)].glenum)), (unsigned int)(yyvsp[(3) - (4)].ival))) yyerror("Failed to create state!\n"); ;}
    break;

  case 488:

/* Line 1455 of yacc.c  */
#line 3360 "Grammar.ypp"
    { check_curDSTState(); if(!curDSTState->createState(getGLEnumValue((yyvsp[(1) - (4)].glenum)), (unsigned int)(yyvsp[(3) - (4)].ival))) yyerror("Failed to create state!\n"); ;}
    break;

  case 489:

/* Line 1455 of yacc.c  */
#line 3361 "Grammar.ypp"
    { check_curDSTState(); if(!curDSTState->createState(getGLEnumValue((yyvsp[(1) - (4)].glenum)), getGLEnumValue((yyvsp[(3) - (4)].glenum)))) yyerror("Failed to create state!\n"); ;}
    break;

  case 490:

/* Line 1455 of yacc.c  */
#line 3362 "Grammar.ypp"
    { check_curDSTState(); if(!curDSTState->createState(getGLEnumValue((yyvsp[(1) - (4)].glenum)), getGLEnumValue((yyvsp[(3) - (4)].glenum)))) yyerror("Failed to create state!\n"); ;}
    break;

  case 491:

/* Line 1455 of yacc.c  */
#line 3363 "Grammar.ypp"
    { check_curDSTState(); if(!curDSTState->createState(getGLEnumValue((yyvsp[(1) - (4)].glenum)), getGLEnumValue((yyvsp[(3) - (4)].glenum)))) yyerror("Failed to create state!\n"); ;}
    break;

  case 492:

/* Line 1455 of yacc.c  */
#line 3364 "Grammar.ypp"
    { check_curDSTState(); if(!curDSTState->createState(getGLEnumValue((yyvsp[(1) - (4)].glenum)), (unsigned int)(yyvsp[(3) - (4)].ival))) yyerror("Failed to create state!\n"); ;}
    break;

  case 494:

/* Line 1455 of yacc.c  */
#line 3371 "Grammar.ypp"
    { check_curRState(); if(!curRState->createState(getGLEnumValue((yyvsp[(2) - (5)].glenum)), (yyvsp[(4) - (5)].fval))) yyerror("Failed to create state!\n"); ;}
    break;

  case 495:

/* Line 1455 of yacc.c  */
#line 3372 "Grammar.ypp"
    { check_curRState(); if(!curRState->createState(getGLEnumValue((yyvsp[(2) - (5)].glenum)), (GLenum)((yyvsp[(4) - (5)].bval) ? GL_TRUE: GL_FALSE))) yyerror("Failed to create state!\n"); ;}
    break;

  case 496:

/* Line 1455 of yacc.c  */
#line 3373 "Grammar.ypp"
    { check_curRState(); if(!curRState->createState(getGLEnumValue((yyvsp[(2) - (5)].glenum)), (GLenum)((yyvsp[(4) - (5)].bval) ? GL_TRUE: GL_FALSE))) yyerror("Failed to create state!\n"); ;}
    break;

  case 497:

/* Line 1455 of yacc.c  */
#line 3374 "Grammar.ypp"
    { check_curRState(); if(!curRState->createState(getGLEnumValue((yyvsp[(2) - (5)].glenum)), (GLenum)((yyvsp[(4) - (5)].bval) ? GL_TRUE: GL_FALSE))) yyerror("Failed to create state!\n"); ;}
    break;

  case 498:

/* Line 1455 of yacc.c  */
#line 3375 "Grammar.ypp"
    { check_curRState(); if(!curRState->createState(getGLEnumValue((yyvsp[(2) - (5)].glenum)), getGLEnumValue((yyvsp[(4) - (5)].glenum)))) yyerror("Failed to create state!\n"); ;}
    break;

  case 499:

/* Line 1455 of yacc.c  */
#line 3376 "Grammar.ypp"
    { check_curRState(); if(!curRState->createState(getGLEnumValue((yyvsp[(2) - (5)].glenum)), (yyvsp[(4) - (5)].fval))) yyerror("Failed to create state!\n"); ;}
    break;

  case 500:

/* Line 1455 of yacc.c  */
#line 3377 "Grammar.ypp"
    { check_curRState(); if(!curRState->createState(getGLEnumValue((yyvsp[(2) - (5)].glenum)), (GLenum)((yyvsp[(4) - (5)].bval) ? GL_TRUE: GL_FALSE))) yyerror("Failed to create state!\n"); ;}
    break;

  case 501:

/* Line 1455 of yacc.c  */
#line 3379 "Grammar.ypp"
    { check_curRState(); if(!curRState->createState(getGLEnumValue((yyvsp[(2) - (5)].glenum)), (unsigned int)(yyvsp[(4) - (5)].ival))) yyerror("Failed to create state!\n"); ;}
    break;

  case 502:

/* Line 1455 of yacc.c  */
#line 3380 "Grammar.ypp"
    { check_curRState(); if(!curRState->createState(getGLEnumValue((yyvsp[(2) - (5)].glenum)), (unsigned int)(yyvsp[(4) - (5)].ival))) yyerror("Failed to create state!\n"); ;}
    break;

  case 503:

/* Line 1455 of yacc.c  */
#line 3381 "Grammar.ypp"
    { check_curRState(); if(!curRState->createState(getGLEnumValue((yyvsp[(2) - (5)].glenum)), (GLenum)((yyvsp[(4) - (5)].bval) ? GL_TRUE: GL_FALSE))) yyerror("Failed to create state!\n"); ;}
    break;

  case 504:

/* Line 1455 of yacc.c  */
#line 3382 "Grammar.ypp"
    { check_curRState(); if(!curRState->createState(getGLEnumValue((yyvsp[(2) - (5)].glenum)), (GLenum)((yyvsp[(4) - (5)].bval) ? GL_TRUE: GL_FALSE))) yyerror("Failed to create state!\n"); ;}
    break;

  case 505:

/* Line 1455 of yacc.c  */
#line 3383 "Grammar.ypp"
    { check_curRState(); if(!curRState->createState(getGLEnumValue((yyvsp[(2) - (5)].glenum)), getGLEnumValue((yyvsp[(4) - (5)].glenum)))) yyerror("Failed to create state!\n"); ;}
    break;

  case 506:

/* Line 1455 of yacc.c  */
#line 3384 "Grammar.ypp"
    { check_curRState(); if(!curRState->createState(getGLEnumValue((yyvsp[(2) - (5)].glenum)), getGLEnumValue((yyvsp[(4) - (5)].glenum)))) yyerror("Failed to create state!\n"); ;}
    break;

  case 507:

/* Line 1455 of yacc.c  */
#line 3385 "Grammar.ypp"
    { check_curRState(); if(!curRState->createState(getGLEnumValue((yyvsp[(2) - (5)].glenum)), (GLenum)((yyvsp[(4) - (5)].bval) ? GL_TRUE: GL_FALSE))) yyerror("Failed to create state!\n"); ;}
    break;

  case 508:

/* Line 1455 of yacc.c  */
#line 3386 "Grammar.ypp"
    { check_curRState(); if(!curRState->createState(getGLEnumValue((yyvsp[(2) - (5)].glenum)), getGLEnumValue((yyvsp[(4) - (5)].glenum)), getGLEnumValue((yyvsp[(4) - (5)].glenum)))) yyerror("Failed to create state!\n"); ;}
    break;

  case 509:

/* Line 1455 of yacc.c  */
#line 3387 "Grammar.ypp"
    { if(!curRState->createState(getGLEnumValue((yyvsp[(2) - (9)].glenum)), getGLEnumValue((yyvsp[(5) - (9)].glenum)), getGLEnumValue((yyvsp[(7) - (9)].glenum)))) yyerror("Failed to create state!\n"); ;}
    break;

  case 510:

/* Line 1455 of yacc.c  */
#line 3388 "Grammar.ypp"
    { check_curRState(); if(!curRState->createState(getGLEnumValue((yyvsp[(2) - (5)].glenum)), (yyvsp[(4) - (5)].fval))) yyerror("Failed to create state!\n"); ;}
    break;

  case 511:

/* Line 1455 of yacc.c  */
#line 3389 "Grammar.ypp"
    { check_curRState(); if(!curRState->createState(getGLEnumValue((yyvsp[(2) - (5)].glenum)), (yyvsp[(4) - (5)].fval))) yyerror("Failed to create state!\n"); ;}
    break;

  case 512:

/* Line 1455 of yacc.c  */
#line 3390 "Grammar.ypp"
    { check_curRState(); if(!curRState->createState(getGLEnumValue((yyvsp[(2) - (5)].glenum)), (GLenum)((yyvsp[(4) - (5)].bval) ? GL_TRUE: GL_FALSE))) yyerror("Failed to create state!\n"); ;}
    break;

  case 513:

/* Line 1455 of yacc.c  */
#line 3391 "Grammar.ypp"
    { check_curRState(); if(!curRState->createState(getGLEnumValue((yyvsp[(2) - (5)].glenum)), (GLenum)((yyvsp[(4) - (5)].bval) ? GL_TRUE: GL_FALSE))) yyerror("Failed to create state!\n"); ;}
    break;

  case 514:

/* Line 1455 of yacc.c  */
#line 3392 "Grammar.ypp"
    { check_curRState(); if(!curRState->createState(getGLEnumValue((yyvsp[(2) - (5)].glenum)), (GLenum)((yyvsp[(4) - (5)].bval) ? GL_TRUE: GL_FALSE))) yyerror("Failed to create state!\n"); ;}
    break;

  case 515:

/* Line 1455 of yacc.c  */
#line 3394 "Grammar.ypp"
    { check_curRState(); if(!curRState->createState(getGLEnumValue((yyvsp[(2) - (5)].glenum)), (GLenum)((yyvsp[(4) - (5)].bval) ? GL_TRUE: GL_FALSE))) yyerror("Failed to create state!\n"); ;}
    break;

  case 516:

/* Line 1455 of yacc.c  */
#line 3395 "Grammar.ypp"
    { yyerror("Error in Rasterizer-state list\n"); ;}
    break;

  case 517:

/* Line 1455 of yacc.c  */
#line 3400 "Grammar.ypp"
    {
    LOGD("Rasterization state %s\n", (yyvsp[(2) - (2)].s)->c_str() );
    curRState = curContainer->createStateGroup_Raster((yyvsp[(2) - (2)].s)->c_str())->getExInterface();
    curAnnotations = curRState->annotations()->getExInterface();
    delete (yyvsp[(2) - (2)].s);
;}
    break;

  case 518:

/* Line 1455 of yacc.c  */
#line 3405 "Grammar.ypp"
    { 
    // for the case of items within a namespace :
    // copy the name-space annotations to this one
    if(curAnnotations) curAnnotations->copy(namespaceAnnotations);
    curAnnotations = NULL; curRState = NULL; lex_pop_state(); ;}
    break;

  case 519:

/* Line 1455 of yacc.c  */
#line 3410 "Grammar.ypp"
    { yyerror("Error in Rasterizer-state declaration\n"); ;}
    break;

  case 521:

/* Line 1455 of yacc.c  */
#line 3416 "Grammar.ypp"
    { check_curCSState(); if(!curCSState->createState(getGLEnumValue((yyvsp[(2) - (5)].glenum)), (GLenum)((yyvsp[(4) - (5)].bval) ? GL_TRUE: GL_FALSE))) yyerror("Failed to create state!\n"); ;}
    break;

  case 522:

/* Line 1455 of yacc.c  */
#line 3417 "Grammar.ypp"
    { check_curCSState(); if(!curCSState->createState(getGLEnumValue((yyvsp[(2) - (5)].glenum)), getGLEnumValue((yyvsp[(4) - (5)].glenum)))) yyerror("Failed to create state!\n"); ;}
    break;

  case 523:

/* Line 1455 of yacc.c  */
#line 3418 "Grammar.ypp"
    { check_curCSState(); if(!curCSState->createState(getGLEnumValue((yyvsp[(2) - (5)].glenum)), (yyvsp[(4) - (5)].fval))) yyerror("Failed to create state!\n"); ;}
    break;

  case 524:

/* Line 1455 of yacc.c  */
#line 3419 "Grammar.ypp"
    { check_curCSState(); if(!curCSState->createState(getGLEnumValue((yyvsp[(2) - (5)].glenum)), (GLenum)((yyvsp[(4) - (5)].bval) ? GL_TRUE: GL_FALSE))) yyerror("Failed to create state!\n"); ;}
    break;

  case 525:

/* Line 1455 of yacc.c  */
#line 3420 "Grammar.ypp"
    { check_curCSState(); if(!curCSState->createState(getGLEnumValue((yyvsp[(2) - (5)].glenum)), getGLEnumValue((yyvsp[(4) - (5)].glenum)))) yyerror("Failed to create state!\n"); ;}
    break;

  case 526:

/* Line 1455 of yacc.c  */
#line 3421 "Grammar.ypp"
    { check_curCSState(); if(!curCSState->createState(getGLEnumValue((yyvsp[(2) - (5)].glenum)), getGLEnumValue((yyvsp[(4) - (5)].glenum)))) yyerror("Failed to create state!\n"); ;}
    break;

  case 527:

/* Line 1455 of yacc.c  */
#line 3422 "Grammar.ypp"
    { check_curCSState(); if(!curCSState->createState(getGLEnumValue((yyvsp[(2) - (5)].glenum)), getGLEnumValue((yyvsp[(4) - (5)].glenum)))) yyerror("Failed to create state!\n"); ;}
    break;

  case 528:

/* Line 1455 of yacc.c  */
#line 3423 "Grammar.ypp"
    { check_curCSState(); if(!curCSState->createState(getGLEnumValue((yyvsp[(2) - (5)].glenum)), getGLEnumValue((yyvsp[(4) - (5)].glenum)))) yyerror("Failed to create state!\n"); ;}
    break;

  case 529:

/* Line 1455 of yacc.c  */
#line 3424 "Grammar.ypp"
    { check_curCSState(); if(!curCSState->createState(getGLEnumValue((yyvsp[(2) - (5)].glenum)), getGLEnumValue((yyvsp[(4) - (5)].glenum)))) yyerror("Failed to create state!\n"); ;}
    break;

  case 530:

/* Line 1455 of yacc.c  */
#line 3425 "Grammar.ypp"
    { check_curCSState(); if(!curCSState->createState(getGLEnumValue((yyvsp[(2) - (5)].glenum)), getGLEnumValue((yyvsp[(4) - (5)].glenum)))) yyerror("Failed to create state!\n"); ;}
    break;

  case 531:

/* Line 1455 of yacc.c  */
#line 3427 "Grammar.ypp"
    { check_curCSState(); if(!curCSState->createState(getGLEnumValue((yyvsp[(2) - (5)].glenum)), getGLEnumValue((yyvsp[(4) - (5)].glenum)))) yyerror("Failed to create state!\n"); ;}
    break;

  case 532:

/* Line 1455 of yacc.c  */
#line 3428 "Grammar.ypp"
    { check_curCSState(); if(!curCSState->createState(getGLEnumValue((yyvsp[(2) - (5)].glenum)), getGLEnumValue((yyvsp[(4) - (5)].glenum)))) yyerror("Failed to create state!\n"); ;}
    break;

  case 533:

/* Line 1455 of yacc.c  */
#line 3429 "Grammar.ypp"
    {
    check_curCSState();
    if(!curCSState->createState(getGLEnumValue(GLBLENDEQUATIONRGB), getGLEnumValue((yyvsp[(3) - (4)].glenum)))) yyerror("Failed to create state!\n");
    if(!curCSState->createState(getGLEnumValue(GLBLENDEQUATIONALPHA), getGLEnumValue((yyvsp[(3) - (4)].glenum)))) yyerror("Failed to create state!\n");
;}
    break;

  case 534:

/* Line 1455 of yacc.c  */
#line 3434 "Grammar.ypp"
    {
    check_curCSState();
    if(!curCSState->createState(getGLEnumValue(GLBLENDEQUATIONRGB), getGLEnumValue((yyvsp[(4) - (8)].glenum)))) yyerror("Failed to create state!\n");
    if(!curCSState->createState(getGLEnumValue(GLBLENDEQUATIONALPHA), getGLEnumValue((yyvsp[(6) - (8)].glenum)))) yyerror("Failed to create state!\n");
;}
    break;

  case 535:

/* Line 1455 of yacc.c  */
#line 3444 "Grammar.ypp"
    { check_curCSState(); if(!curCSState->createState(getGLEnumValue((yyvsp[(2) - (13)].glenum)), (yyvsp[(5) - (13)].fval), (yyvsp[(7) - (13)].fval), (yyvsp[(9) - (13)].fval), (yyvsp[(11) - (13)].fval))) yyerror("Failed to create state!\n"); ;}
    break;

  case 536:

/* Line 1455 of yacc.c  */
#line 3445 "Grammar.ypp"
    { check_curCSState(); if(!curCSState->createState(getGLEnumValue((yyvsp[(2) - (5)].glenum)), (GLenum)((yyvsp[(4) - (5)].bval) ? GL_TRUE: GL_FALSE))) yyerror("Failed to create state!\n"); ;}
    break;

  case 537:

/* Line 1455 of yacc.c  */
#line 3446 "Grammar.ypp"
    { check_curCSState(); if(!curCSState->createState(getGLEnumValue((yyvsp[(2) - (5)].glenum)), (GLenum)((yyvsp[(4) - (5)].bval) ? GL_TRUE: GL_FALSE))) yyerror("Failed to create state!\n"); ;}
    break;

  case 538:

/* Line 1455 of yacc.c  */
#line 3447 "Grammar.ypp"
    { check_curCSState(); if(!curCSState->createState(getGLEnumValue((yyvsp[(2) - (5)].glenum)), (GLenum)((yyvsp[(4) - (5)].bval) ? GL_TRUE: GL_FALSE))) yyerror("Failed to create state!\n"); ;}
    break;

  case 539:

/* Line 1455 of yacc.c  */
#line 3448 "Grammar.ypp"
    { check_curCSState(); if(!curCSState->createState(getGLEnumValue((yyvsp[(2) - (5)].glenum)), getGLEnumValue((yyvsp[(4) - (5)].glenum)))) yyerror("Failed to create state!\n"); ;}
    break;

  case 540:

/* Line 1455 of yacc.c  */
#line 3450 "Grammar.ypp"
    { check_curCSState(); if(!curCSState->createState(getGLEnumValue((yyvsp[(2) - (13)].glenum)), 
    (GLboolean)((yyvsp[(5) - (13)].bval) ? GL_TRUE: GL_FALSE),
    (GLboolean)((yyvsp[(7) - (13)].bval) ? GL_TRUE: GL_FALSE),
    (GLboolean)((yyvsp[(9) - (13)].bval) ? GL_TRUE: GL_FALSE),
    (GLboolean)((yyvsp[(11) - (13)].bval) ? GL_TRUE: GL_FALSE)
    )) yyerror("Failed to create state!\n"); ;}
    break;

  case 541:

/* Line 1455 of yacc.c  */
#line 3456 "Grammar.ypp"
    { check_curCSState(); if(!curCSState->createState(getGLEnumValue((yyvsp[(2) - (5)].glenum)), (GLenum)((yyvsp[(4) - (5)].bval) ? GL_TRUE: GL_FALSE))) yyerror("Failed to create state!\n"); ;}
    break;

  case 542:

/* Line 1455 of yacc.c  */
#line 3457 "Grammar.ypp"
    { check_curCSState(); if(!curCSState->createState(getGLEnumValue((yyvsp[(2) - (5)].glenum)), (GLenum)((yyvsp[(4) - (5)].bval) ? GL_TRUE: GL_FALSE))) yyerror("Failed to create state!\n"); ;}
    break;

  case 543:

/* Line 1455 of yacc.c  */
#line 3459 "Grammar.ypp"
    { check_curCSState(); if(!curCSState->createState(getGLEnumValue((yyvsp[(2) - (5)].glenum)), (GLenum)((yyvsp[(4) - (5)].bval) ? GL_TRUE: GL_FALSE))) yyerror("Failed to create state!\n"); ;}
    break;

  case 544:

/* Line 1455 of yacc.c  */
#line 3460 "Grammar.ypp"
    { check_curCSState(); if(!curCSState->createState(getGLEnumValue((yyvsp[(2) - (5)].glenum)), (GLenum)((yyvsp[(4) - (5)].bval) ? GL_TRUE: GL_FALSE))) yyerror("Failed to create state!\n"); ;}
    break;

  case 545:

/* Line 1455 of yacc.c  */
#line 3461 "Grammar.ypp"
    { yyerror("Error in Color sample state list\n"); ;}
    break;

  case 546:

/* Line 1455 of yacc.c  */
#line 3466 "Grammar.ypp"
    {
    LOGD("Color sample state %s\n", (yyvsp[(2) - (2)].s)->c_str() );
    curCSState = curContainer->createStateGroup_CS((yyvsp[(2) - (2)].s)->c_str())->getExInterface();
    curAnnotations = curCSState->annotations()->getExInterface();
    delete (yyvsp[(2) - (2)].s);
;}
    break;

  case 547:

/* Line 1455 of yacc.c  */
#line 3471 "Grammar.ypp"
    { 
    // for the case of items within a namespace :
    // copy the name-space annotations to this one
    if(curAnnotations) curAnnotations->copy(namespaceAnnotations);
    curAnnotations = NULL; curCSState = NULL; lex_pop_state(); ;}
    break;

  case 548:

/* Line 1455 of yacc.c  */
#line 3476 "Grammar.ypp"
    { yyerror("Error in color sampler state declaration\n"); ;}
    break;

  case 550:

/* Line 1455 of yacc.c  */
#line 3482 "Grammar.ypp"
    { check_curDSTState(); if(!curDSTState->createState(getGLEnumValue((yyvsp[(2) - (5)].glenum)), (GLenum)((yyvsp[(4) - (5)].bval) ? GL_TRUE: GL_FALSE))) yyerror("Failed to create state!\n"); ;}
    break;

  case 551:

/* Line 1455 of yacc.c  */
#line 3483 "Grammar.ypp"
    { check_curDSTState(); if(!curDSTState->createState(getGLEnumValue((yyvsp[(2) - (5)].glenum)), getGLEnumValue((yyvsp[(4) - (5)].glenum)))) yyerror("Failed to create state!\n"); ;}
    break;

  case 552:

/* Line 1455 of yacc.c  */
#line 3484 "Grammar.ypp"
    { check_curDSTState(); if(!curDSTState->createState(getGLEnumValue((yyvsp[(2) - (5)].glenum)), (GLenum)((yyvsp[(4) - (5)].bval) ? GL_TRUE: GL_FALSE))) yyerror("Failed to create state!\n"); ;}
    break;

  case 553:

/* Line 1455 of yacc.c  */
#line 3485 "Grammar.ypp"
    { check_curDSTState(); if(!curDSTState->createState(getGLEnumValue((yyvsp[(2) - (5)].glenum)), (GLenum)((yyvsp[(4) - (5)].bval) ? GL_TRUE: GL_FALSE))) yyerror("Failed to create state!\n"); ;}
    break;

  case 554:

/* Line 1455 of yacc.c  */
#line 3486 "Grammar.ypp"
    { check_curDSTState(); if(!curDSTState->createState(getGLEnumValue((yyvsp[(2) - (5)].glenum)), (GLenum)((yyvsp[(4) - (5)].bval) ? GL_TRUE: GL_FALSE))) yyerror("Failed to create state!\n"); ;}
    break;

  case 555:

/* Line 1455 of yacc.c  */
#line 3488 "Grammar.ypp"
    { check_curDSTState(); if(!curDSTState->createState(getGLEnumValue((yyvsp[(2) - (9)].glenum)), (yyvsp[(5) - (9)].fval), (yyvsp[(7) - (9)].fval))) yyerror("Failed to create state!\n"); ;}
    break;

  case 556:

/* Line 1455 of yacc.c  */
#line 3489 "Grammar.ypp"
    { check_curDSTState(); if(!curDSTState->createState(getGLEnumValue((yyvsp[(2) - (5)].glenum)), (GLenum)((yyvsp[(4) - (5)].bval) ? GL_TRUE: GL_FALSE))) yyerror("Failed to create state!\n"); ;}
    break;

  case 557:

/* Line 1455 of yacc.c  */
#line 3490 "Grammar.ypp"
    { check_curDSTState(); if(!curDSTState->createState(getGLEnumValue((yyvsp[(2) - (5)].glenum)), getGLEnumValue((yyvsp[(4) - (5)].glenum)))) yyerror("Failed to create state!\n"); ;}
    break;

  case 558:

/* Line 1455 of yacc.c  */
#line 3491 "Grammar.ypp"
    { check_curDSTState(); if(!curDSTState->createState(getGLEnumValue((yyvsp[(2) - (5)].glenum)), (unsigned int)(yyvsp[(4) - (5)].ival))) yyerror("Failed to create state!\n"); ;}
    break;

  case 559:

/* Line 1455 of yacc.c  */
#line 3492 "Grammar.ypp"
    { check_curDSTState(); if(!curDSTState->createState(getGLEnumValue((yyvsp[(2) - (5)].glenum)), (unsigned int)(yyvsp[(4) - (5)].ival))) yyerror("Failed to create state!\n"); ;}
    break;

  case 560:

/* Line 1455 of yacc.c  */
#line 3493 "Grammar.ypp"
    { check_curDSTState(); if(!curDSTState->createState(getGLEnumValue((yyvsp[(2) - (5)].glenum)), getGLEnumValue((yyvsp[(4) - (5)].glenum)))) yyerror("Failed to create state!\n"); ;}
    break;

  case 561:

/* Line 1455 of yacc.c  */
#line 3494 "Grammar.ypp"
    { check_curDSTState(); if(!curDSTState->createState(getGLEnumValue((yyvsp[(2) - (5)].glenum)), getGLEnumValue((yyvsp[(4) - (5)].glenum)))) yyerror("Failed to create state!\n"); ;}
    break;

  case 562:

/* Line 1455 of yacc.c  */
#line 3495 "Grammar.ypp"
    { check_curDSTState(); if(!curDSTState->createState(getGLEnumValue((yyvsp[(2) - (5)].glenum)), getGLEnumValue((yyvsp[(4) - (5)].glenum)))) yyerror("Failed to create state!\n"); ;}
    break;

  case 563:

/* Line 1455 of yacc.c  */
#line 3496 "Grammar.ypp"
    { check_curDSTState(); if(!curDSTState->createState(getGLEnumValue((yyvsp[(2) - (5)].glenum)), (unsigned int)(yyvsp[(4) - (5)].ival))) yyerror("Failed to create state!\n"); ;}
    break;

  case 564:

/* Line 1455 of yacc.c  */
#line 3504 "Grammar.ypp"
    { check_curDSTState(); if(!curDSTState->createState(getGLEnumValue((yyvsp[(2) - (5)].glenum)), getGLEnumValue((yyvsp[(4) - (5)].glenum)))) yyerror("Failed to create state!\n"); ;}
    break;

  case 565:

/* Line 1455 of yacc.c  */
#line 3505 "Grammar.ypp"
    { check_curDSTState(); if(!curDSTState->createState(getGLEnumValue((yyvsp[(2) - (5)].glenum)), (unsigned int)(yyvsp[(4) - (5)].ival))) yyerror("Failed to create state!\n"); ;}
    break;

  case 566:

/* Line 1455 of yacc.c  */
#line 3506 "Grammar.ypp"
    { check_curDSTState(); if(!curDSTState->createState(getGLEnumValue((yyvsp[(2) - (5)].glenum)), (unsigned int)(yyvsp[(4) - (5)].ival))) yyerror("Failed to create state!\n"); ;}
    break;

  case 567:

/* Line 1455 of yacc.c  */
#line 3507 "Grammar.ypp"
    { check_curDSTState(); if(!curDSTState->createState(getGLEnumValue((yyvsp[(2) - (5)].glenum)), getGLEnumValue((yyvsp[(4) - (5)].glenum)))) yyerror("Failed to create state!\n"); ;}
    break;

  case 568:

/* Line 1455 of yacc.c  */
#line 3508 "Grammar.ypp"
    { check_curDSTState(); if(!curDSTState->createState(getGLEnumValue((yyvsp[(2) - (5)].glenum)), getGLEnumValue((yyvsp[(4) - (5)].glenum)))) yyerror("Failed to create state!\n"); ;}
    break;

  case 569:

/* Line 1455 of yacc.c  */
#line 3509 "Grammar.ypp"
    { check_curDSTState(); if(!curDSTState->createState(getGLEnumValue((yyvsp[(2) - (5)].glenum)), getGLEnumValue((yyvsp[(4) - (5)].glenum)))) yyerror("Failed to create state!\n"); ;}
    break;

  case 570:

/* Line 1455 of yacc.c  */
#line 3510 "Grammar.ypp"
    { check_curDSTState(); if(!curDSTState->createState(getGLEnumValue((yyvsp[(2) - (5)].glenum)), (unsigned int)(yyvsp[(4) - (5)].ival))) yyerror("Failed to create state!\n"); ;}
    break;

  case 571:

/* Line 1455 of yacc.c  */
#line 3511 "Grammar.ypp"
    { yyerror("Error in Depth Stencil state list\n"); ;}
    break;

  case 572:

/* Line 1455 of yacc.c  */
#line 3516 "Grammar.ypp"
    {
    LOGD("Depth-Stencil state %s\n", (yyvsp[(2) - (2)].s)->c_str() );
    curDSTState = curContainer->createStateGroup_DST((yyvsp[(2) - (2)].s)->c_str())->getExInterface();
    curAnnotations = curDSTState->annotations()->getExInterface();
    delete (yyvsp[(2) - (2)].s);
;}
    break;

  case 573:

/* Line 1455 of yacc.c  */
#line 3521 "Grammar.ypp"
    { 
    // for the case of items within a namespace :
    // copy the name-space annotations to this one
    if(curAnnotations) curAnnotations->copy(namespaceAnnotations);
    curAnnotations = NULL; curDSTState = NULL; lex_pop_state(); ;}
    break;

  case 574:

/* Line 1455 of yacc.c  */
#line 3526 "Grammar.ypp"
    { yyerror("Error in DST-state declaration\n"); ;}
    break;

  case 575:

/* Line 1455 of yacc.c  */
#line 3529 "Grammar.ypp"
    { LOGI("TODO"); ;}
    break;

  case 576:

/* Line 1455 of yacc.c  */
#line 3530 "Grammar.ypp"
    { LOGI("TODO"); ;}
    break;

  case 577:

/* Line 1455 of yacc.c  */
#line 3531 "Grammar.ypp"
    { LOGI("TODO"); ;}
    break;

  case 578:

/* Line 1455 of yacc.c  */
#line 3532 "Grammar.ypp"
    { LOGI("TODO"); ;}
    break;

  case 579:

/* Line 1455 of yacc.c  */
#line 3533 "Grammar.ypp"
    { LOGI("TODO"); ;}
    break;

  case 580:

/* Line 1455 of yacc.c  */
#line 3534 "Grammar.ypp"
    { LOGI("TODO"); ;}
    break;

  case 581:

/* Line 1455 of yacc.c  */
#line 3535 "Grammar.ypp"
    { LOGI("TODO"); ;}
    break;

  case 582:

/* Line 1455 of yacc.c  */
#line 3536 "Grammar.ypp"
    { LOGI("TODO"); ;}
    break;

  case 583:

/* Line 1455 of yacc.c  */
#line 3537 "Grammar.ypp"
    { LOGI("TODO"); ;}
    break;

  case 584:

/* Line 1455 of yacc.c  */
#line 3538 "Grammar.ypp"
    { LOGI("TODO"); ;}
    break;

  case 585:

/* Line 1455 of yacc.c  */
#line 3539 "Grammar.ypp"
    { LOGI("TODO"); ;}
    break;

  case 586:

/* Line 1455 of yacc.c  */
#line 3540 "Grammar.ypp"
    { LOGI("TODO"); ;}
    break;

  case 587:

/* Line 1455 of yacc.c  */
#line 3541 "Grammar.ypp"
    { LOGI("TODO"); ;}
    break;

  case 588:

/* Line 1455 of yacc.c  */
#line 3542 "Grammar.ypp"
    { LOGI("TODO"); ;}
    break;

  case 589:

/* Line 1455 of yacc.c  */
#line 3543 "Grammar.ypp"
    { LOGI("TODO"); ;}
    break;

  case 590:

/* Line 1455 of yacc.c  */
#line 3544 "Grammar.ypp"
    { LOGI("TODO"); ;}
    break;

  case 591:

/* Line 1455 of yacc.c  */
#line 3545 "Grammar.ypp"
    { LOGI("TODO"); ;}
    break;

  case 592:

/* Line 1455 of yacc.c  */
#line 3546 "Grammar.ypp"
    { LOGI("TODO"); ;}
    break;

  case 593:

/* Line 1455 of yacc.c  */
#line 3547 "Grammar.ypp"
    { LOGI("TODO"); ;}
    break;

  case 594:

/* Line 1455 of yacc.c  */
#line 3548 "Grammar.ypp"
    { LOGI("TODO"); ;}
    break;

  case 595:

/* Line 1455 of yacc.c  */
#line 3549 "Grammar.ypp"
    { LOGI("TODO"); ;}
    break;

  case 596:

/* Line 1455 of yacc.c  */
#line 3550 "Grammar.ypp"
    { LOGI("TODO"); ;}
    break;

  case 598:

/* Line 1455 of yacc.c  */
#line 3559 "Grammar.ypp"
    { LOGI("TODO"); ;}
    break;

  case 599:

/* Line 1455 of yacc.c  */
#line 3560 "Grammar.ypp"
    { 
#ifndef OGLES2
    check_curPRState(); if(!curPRState->createState(getGLEnumValue((yyvsp[(2) - (5)].glenum)), getGLEnumValue((yyvsp[(4) - (5)].glenum)))) yyerror("Failed to create state!\n");
#endif
;}
    break;

  case 600:

/* Line 1455 of yacc.c  */
#line 3565 "Grammar.ypp"
    {
#ifndef OGLES2
    check_curPRState(); if(!curPRState->createState(getGLEnumValue((yyvsp[(2) - (5)].glenum)), getGLEnumValue((yyvsp[(4) - (5)].glenum)))) yyerror("Failed to create state!\n");
#endif
;}
    break;

  case 601:

/* Line 1455 of yacc.c  */
#line 3570 "Grammar.ypp"
    {
#ifndef OGLES2
    LOGI("TODO");
#endif
;}
    break;

  case 602:

/* Line 1455 of yacc.c  */
#line 3575 "Grammar.ypp"
    {
#ifndef OGLES2
    LOGI("TODO");
#endif
;}
    break;

  case 603:

/* Line 1455 of yacc.c  */
#line 3580 "Grammar.ypp"
    {
#ifndef OGLES2
    check_curPRState(); if(!curPRState->createState(getGLEnumValue((yyvsp[(2) - (5)].glenum)), getGLEnumValue((yyvsp[(4) - (5)].glenum)))) yyerror("Failed to create state!\n");
#endif
;}
    break;

  case 604:

/* Line 1455 of yacc.c  */
#line 3585 "Grammar.ypp"
    {
#ifndef OGLES2
    check_curPRState(); if(!curPRState->createState(getGLEnumValue((yyvsp[(2) - (5)].glenum)), (yyvsp[(4) - (5)].fval))) yyerror("Failed to create state!\n");
#endif
;}
    break;

  case 605:

/* Line 1455 of yacc.c  */
#line 3590 "Grammar.ypp"
    {
#ifndef OGLES2
    check_curPRState(); if(!curPRState->createState(getGLEnumValue((yyvsp[(2) - (5)].glenum)), getGLEnumValue((yyvsp[(4) - (5)].glenum)))) yyerror("Failed to create state!\n");
#endif
;}
    break;

  case 606:

/* Line 1455 of yacc.c  */
#line 3595 "Grammar.ypp"
    {
#ifndef OGLES2
    check_curPRState(); if(!curPRState->createState(getGLEnumValue((yyvsp[(2) - (5)].glenum)), getGLEnumValue((yyvsp[(4) - (5)].glenum)))) yyerror("Failed to create state!\n");
#endif
;}
    break;

  case 607:

/* Line 1455 of yacc.c  */
#line 3600 "Grammar.ypp"
    {
#ifndef OGLES2
    check_curPRState(); if(!curPRState->createState(getGLEnumValue((yyvsp[(2) - (5)].glenum)), getGLEnumValue((yyvsp[(4) - (5)].glenum)))) yyerror("Failed to create state!\n");
#endif
;}
    break;

  case 608:

/* Line 1455 of yacc.c  */
#line 3605 "Grammar.ypp"
    {
#ifndef OGLES2
    check_curPRState(); if(!curPRState->createState(getGLEnumValue((yyvsp[(2) - (5)].glenum)), getGLEnumValue((yyvsp[(4) - (5)].glenum)))) yyerror("Failed to create state!\n");
#endif
;}
    break;

  case 609:

/* Line 1455 of yacc.c  */
#line 3610 "Grammar.ypp"
    {
#ifndef OGLES2
    check_curPRState(); if(!curPRState->createState(getGLEnumValue((yyvsp[(2) - (5)].glenum)), getGLEnumValue((yyvsp[(4) - (5)].glenum)))) yyerror("Failed to create state!\n");
#endif
;}
    break;

  case 610:

/* Line 1455 of yacc.c  */
#line 3615 "Grammar.ypp"
    {
#ifndef OGLES2
    check_curPRState(); if(!curPRState->createState(getGLEnumValue((yyvsp[(2) - (5)].glenum)), (yyvsp[(4) - (5)].fval))) yyerror("Failed to create state!\n");
#endif
;}
    break;

  case 611:

/* Line 1455 of yacc.c  */
#line 3620 "Grammar.ypp"
    {
#ifndef OGLES2
    check_curPRState(); if(!curPRState->createState(getGLEnumValue((yyvsp[(2) - (5)].glenum)), (yyvsp[(4) - (5)].fval))) yyerror("Failed to create state!\n");
#endif
;}
    break;

  case 612:

/* Line 1455 of yacc.c  */
#line 3625 "Grammar.ypp"
    {
#ifndef OGLES2
    check_curPRState(); if(!curPRState->createState(getGLEnumValue((yyvsp[(2) - (5)].glenum)), getGLEnumValue((yyvsp[(4) - (5)].glenum)))) yyerror("Failed to create state!\n");
#endif
;}
    break;

  case 613:

/* Line 1455 of yacc.c  */
#line 3630 "Grammar.ypp"
    {
#ifndef OGLES2
    check_curPRState(); if(!curPRState->createState(getGLEnumValue((yyvsp[(2) - (5)].glenum)), (yyvsp[(4) - (5)].fval))) yyerror("Failed to create state!\n");
#endif
;}
    break;

  case 614:

/* Line 1455 of yacc.c  */
#line 3635 "Grammar.ypp"
    {
#ifndef OGLES2
    check_curPRState(); if(!curPRState->createState(getGLEnumValue((yyvsp[(2) - (5)].glenum)), getGLEnumValue((yyvsp[(4) - (5)].glenum)))) yyerror("Failed to create state!\n");
#endif
;}
    break;

  case 615:

/* Line 1455 of yacc.c  */
#line 3640 "Grammar.ypp"
    {
#ifndef OGLES2
    check_curPRState(); if(!curPRState->createState(getGLEnumValue((yyvsp[(2) - (5)].glenum)), (yyvsp[(4) - (5)].ival))) yyerror("Failed to create state!\n");
#endif
;}
    break;

  case 616:

/* Line 1455 of yacc.c  */
#line 3645 "Grammar.ypp"
    {
#ifndef OGLES2
    check_curPRState(); if(!curPRState->createState(getGLEnumValue((yyvsp[(2) - (5)].glenum)), getGLEnumValue((yyvsp[(4) - (5)].glenum)))) yyerror("Failed to create state!\n");
#endif
;}
    break;

  case 617:

/* Line 1455 of yacc.c  */
#line 3650 "Grammar.ypp"
    {
#ifndef OGLES2
    check_curPRState(); if(!curPRState->createState(getGLEnumValue((yyvsp[(2) - (5)].glenum)), getGLEnumValue((yyvsp[(4) - (5)].glenum)))) yyerror("Failed to create state!\n");
#endif
;}
    break;

  case 618:

/* Line 1455 of yacc.c  */
#line 3655 "Grammar.ypp"
    {
#ifndef OGLES2
    check_curPRState(); if(!curPRState->createState(getGLEnumValue((yyvsp[(2) - (5)].glenum)), getGLEnumValue((yyvsp[(4) - (5)].glenum)))) yyerror("Failed to create state!\n");
#endif
;}
    break;

  case 619:

/* Line 1455 of yacc.c  */
#line 3660 "Grammar.ypp"
    {
#ifndef OGLES2
    check_curPRState(); if(!curPRState->createState(getGLEnumValue((yyvsp[(2) - (5)].glenum)), getGLEnumValue((yyvsp[(4) - (5)].glenum)))) yyerror("Failed to create state!\n");
#endif
;}
    break;

  case 620:

/* Line 1455 of yacc.c  */
#line 3671 "Grammar.ypp"
    { yyerror("Error in Path-Rendering state list\n"); ;}
    break;

  case 621:

/* Line 1455 of yacc.c  */
#line 3677 "Grammar.ypp"
    {
#ifndef OGLES2
    LOGD("Path Rendering state %s\n", (yyvsp[(2) - (2)].s)->c_str() ); 
    curPRState = curContainer->createStateGroup_PR((yyvsp[(2) - (2)].s)->c_str())->getExInterface();
    curAnnotations = curPRState->annotations()->getExInterface();
#endif
    delete (yyvsp[(2) - (2)].s);
;}
    break;

  case 622:

/* Line 1455 of yacc.c  */
#line 3684 "Grammar.ypp"
    {
#ifndef OGLES2
    // for the case of items within a namespace :
    // copy the name-space annotations to this one
    if(curAnnotations) curAnnotations->copy(namespaceAnnotations);
    curAnnotations = NULL;
    curPRState = NULL;
#endif
    lex_pop_state(); ;}
    break;

  case 623:

/* Line 1455 of yacc.c  */
#line 3693 "Grammar.ypp"
    { yyerror("Error in DST-state declaration\n"); ;}
    break;

  case 625:

/* Line 1455 of yacc.c  */
#line 3704 "Grammar.ypp"
    {
    if(!curAnnotations)
        curAnnotations = IAnnotationEx::getAnnotationSingleton(2); // need a temporary place since nothing was initialized
    if(!curAnnotations->addAnnotation((yyvsp[(2) - (5)].s)->c_str(), (yyvsp[(4) - (5)].s)->c_str()))
        yyerror("err\n");
    delete (yyvsp[(4) - (5)].s);
    ;}
    break;

  case 626:

/* Line 1455 of yacc.c  */
#line 3711 "Grammar.ypp"
    {
    if(!curAnnotations)
        curAnnotations = IAnnotationEx::getAnnotationSingleton(2); // need a temporary place since nothing was initialized
    else if(!curAnnotations->addAnnotation((yyvsp[(2) - (5)].s)->c_str(), (yyvsp[(4) - (5)].fval)))
        yyerror("err\n");
    ;}
    break;

  case 627:

/* Line 1455 of yacc.c  */
#line 3717 "Grammar.ypp"
    {
    if(!curAnnotations)
        curAnnotations = IAnnotationEx::getAnnotationSingleton(2); // need a temporary place since nothing was initialized
    else if(!curAnnotations->addAnnotation((yyvsp[(2) - (5)].s)->c_str(), (yyvsp[(4) - (5)].ival)))
        yyerror("err\n");
    ;}
    break;

  case 628:

/* Line 1455 of yacc.c  */
#line 3723 "Grammar.ypp"
    {
    if(!curAnnotations)
        curAnnotations = IAnnotationEx::getAnnotationSingleton(2); // need a temporary place since nothing was initialized
    else if(!curAnnotations->addAnnotation((yyvsp[(2) - (5)].s)->c_str(), (yyvsp[(4) - (5)].bval)))
        yyerror("err\n");
    ;}
    break;

  case 629:

/* Line 1455 of yacc.c  */
#line 3735 "Grammar.ypp"
    { yyerror("Error in annotation\n"); ;}
    break;

  case 633:

/* Line 1455 of yacc.c  */
#line 3746 "Grammar.ypp"
    {
        LOGD("value %f\n", (yyvsp[(2) - (2)].fval) );
        FloatList* pfl = new FloatList;
        pfl->push_back((yyvsp[(2) - (2)].fval));
        (yyval.floatVecList) = new FloatVecList;
        (yyval.floatVecList)->push_back(pfl);
;}
    break;

  case 634:

/* Line 1455 of yacc.c  */
#line 3756 "Grammar.ypp"
    {
    (yyval.floatVecList) = new FloatVecList;
    (yyval.floatVecList)->push_back((yyvsp[(3) - (4)].floatList));
;}
    break;

  case 635:

/* Line 1455 of yacc.c  */
#line 3763 "Grammar.ypp"
    {
    (yyval.floatVecList) = (yyvsp[(3) - (4)].floatVecList);
;}
    break;

  case 636:

/* Line 1455 of yacc.c  */
#line 3769 "Grammar.ypp"
    {
        LOGD("value %d\n", (yyvsp[(2) - (2)].ival) );
        IntList* pfl = new IntList;
        pfl->push_back((yyvsp[(2) - (2)].ival));
        (yyval.intVecList) = new IntVecList;
        (yyval.intVecList)->push_back(pfl);
;}
    break;

  case 637:

/* Line 1455 of yacc.c  */
#line 3776 "Grammar.ypp"
    {
      LOGI("Error in valueassignment_i\n");
  ;}
    break;

  case 638:

/* Line 1455 of yacc.c  */
#line 3788 "Grammar.ypp"
    {
        LOGD("value %d\n", (yyvsp[(2) - (2)].bval) );
        BoolList* pfl = new BoolList;
        pfl->push_back((yyvsp[(2) - (2)].bval));
        (yyval.boolVecList) = new BoolVecList;
        (yyval.boolVecList)->push_back(pfl);
;}
    break;

  case 639:

/* Line 1455 of yacc.c  */
#line 3798 "Grammar.ypp"
    {
    (yyval.boolVecList) = new BoolVecList;
    (yyval.boolVecList)->push_back((yyvsp[(3) - (4)].boolList));
;}
    break;

  case 640:

/* Line 1455 of yacc.c  */
#line 3804 "Grammar.ypp"
    { (yyval.precisionType)=IUniform::PHighP; ;}
    break;

  case 641:

/* Line 1455 of yacc.c  */
#line 3805 "Grammar.ypp"
    { (yyval.precisionType)=IUniform::PMediumP; ;}
    break;

  case 642:

/* Line 1455 of yacc.c  */
#line 3806 "Grammar.ypp"
    { (yyval.precisionType)=IUniform::PLowP; ;}
    break;

  case 643:

/* Line 1455 of yacc.c  */
#line 3813 "Grammar.ypp"
    {
    (yyval.uniformQualifiers).hasUniformKeyword = true;
    (yyval.uniformQualifiers).global = false;
    (yyval.uniformQualifiers).precisionType = IUniform::PHighP;
;}
    break;

  case 644:

/* Line 1455 of yacc.c  */
#line 3818 "Grammar.ypp"
    {
    (yyval.uniformQualifiers).hasUniformKeyword = true;
    (yyval.uniformQualifiers).global = (yyvsp[(2) - (2)].bval);
    (yyval.uniformQualifiers).precisionType = IUniform::PHighP;
;}
    break;

  case 645:

/* Line 1455 of yacc.c  */
#line 3823 "Grammar.ypp"
    {
    (yyval.uniformQualifiers).hasUniformKeyword = true;
    (yyval.uniformQualifiers).global = false;
    (yyval.uniformQualifiers).precisionType = (yyvsp[(2) - (2)].precisionType);
;}
    break;

  case 646:

/* Line 1455 of yacc.c  */
#line 3828 "Grammar.ypp"
    {
    (yyval.uniformQualifiers).hasUniformKeyword = true;
    (yyval.uniformQualifiers).global = (yyvsp[(2) - (3)].bval);
    (yyval.uniformQualifiers).precisionType = (yyvsp[(3) - (3)].precisionType);
;}
    break;

  case 647:

/* Line 1455 of yacc.c  */
#line 3833 "Grammar.ypp"
    {
    (yyval.uniformQualifiers).hasUniformKeyword = false;
    (yyval.uniformQualifiers).global = (yyvsp[(1) - (1)].bval);
#ifdef OGLES2
    (yyval.uniformQualifiers).precisionType = IUniform::PMediumP; 
#else
    (yyval.uniformQualifiers).precisionType = IUniform::PHighP;
#endif
;}
    break;

  case 648:

/* Line 1455 of yacc.c  */
#line 3842 "Grammar.ypp"
    {
    (yyval.uniformQualifiers).hasUniformKeyword = false;
    (yyval.uniformQualifiers).global = false;
    (yyval.uniformQualifiers).precisionType = (yyvsp[(1) - (1)].precisionType);
;}
    break;

  case 649:

/* Line 1455 of yacc.c  */
#line 3847 "Grammar.ypp"
    {
    (yyval.uniformQualifiers).hasUniformKeyword = false;
    (yyval.uniformQualifiers).global = (yyvsp[(1) - (2)].bval);
    (yyval.uniformQualifiers).precisionType = (yyvsp[(2) - (2)].precisionType);
;}
    break;

  case 650:

/* Line 1455 of yacc.c  */
#line 3858 "Grammar.ypp"
    { (yyval.valueList) = new ValueList; 
    memset((yyval.valueList), 0, sizeof(ValueList));
    (yyval.valueList)->floatVecList = (yyvsp[(1) - (1)].floatVecList); ;}
    break;

  case 651:

/* Line 1455 of yacc.c  */
#line 3861 "Grammar.ypp"
    { (yyval.valueList) = new ValueList; 
    memset((yyval.valueList), 0, sizeof(ValueList));
    (yyval.valueList)->floatVecList = (yyvsp[(1) - (1)].floatVecList); ;}
    break;

  case 652:

/* Line 1455 of yacc.c  */
#line 3864 "Grammar.ypp"
    { (yyval.valueList) = new ValueList; 
    memset((yyval.valueList), 0, sizeof(ValueList));
    (yyval.valueList)->floatVecList = (yyvsp[(1) - (1)].floatVecList); ;}
    break;

  case 653:

/* Line 1455 of yacc.c  */
#line 3867 "Grammar.ypp"
    { (yyval.valueList) = new ValueList; 
    memset((yyval.valueList), 0, sizeof(ValueList));
    (yyval.valueList)->intVecList = (yyvsp[(1) - (1)].intVecList); ;}
    break;

  case 654:

/* Line 1455 of yacc.c  */
#line 3873 "Grammar.ypp"
    { (yyval.valueList) = new ValueList; 
    memset((yyval.valueList), 0, sizeof(ValueList));
    (yyval.valueList)->boolVecList = (yyvsp[(1) - (1)].boolVecList); ;}
    break;

  case 655:

/* Line 1455 of yacc.c  */
#line 3876 "Grammar.ypp"
    { (yyval.valueList) = new ValueList; 
    memset((yyval.valueList), 0, sizeof(ValueList));
    (yyval.valueList)->boolVecList = (yyvsp[(1) - (1)].boolVecList); ;}
    break;

  case 656:

/* Line 1455 of yacc.c  */
#line 3879 "Grammar.ypp"
    { 
    (yyval.valueList) = NULL; yyerror("Error in value assignment\n"); 
;}
    break;

  case 657:

/* Line 1455 of yacc.c  */
#line 3885 "Grammar.ypp"
    {
    LOGD("semantic %s\n", (yyvsp[(2) - (2)].s)->c_str() );
    (yyval.s) = (yyvsp[(2) - (2)].s);
;}
    break;

  case 658:

/* Line 1455 of yacc.c  */
#line 3892 "Grammar.ypp"
    { 
    (yyval.ival) = -1;
;}
    break;

  case 659:

/* Line 1455 of yacc.c  */
#line 3895 "Grammar.ypp"
    {
    (yyval.ival) = (yyvsp[(2) - (3)].ival);
;}
    break;

  case 660:

/* Line 1455 of yacc.c  */
#line 3906 "Grammar.ypp"
    {
    (yyval.saszsauva) = new SASzSAUva;
    memset((yyval.saszsauva), 0, sizeof(SASzSAUva));
;}
    break;

  case 661:

/* Line 1455 of yacc.c  */
#line 3910 "Grammar.ypp"
    {
    (yyval.saszsauva) = new SASzSAUva;
    memset((yyval.saszsauva), 0, sizeof(SASzSAUva));
;}
    break;

  case 662:

/* Line 1455 of yacc.c  */
#line 3914 "Grammar.ypp"
    {
    (yyval.saszsauva) = new SASzSAUva;
    memset((yyval.saszsauva), 0, sizeof(SASzSAUva));
    (yyval.saszsauva)->valueList = (yyvsp[(1) - (1)].valueList);
;}
    break;

  case 663:

/* Line 1455 of yacc.c  */
#line 3919 "Grammar.ypp"
    {
    (yyval.saszsauva) = new SASzSAUva;
    memset((yyval.saszsauva), 0, sizeof(SASzSAUva));
    (yyval.saszsauva)->valueList = (yyvsp[(2) - (2)].valueList);
;}
    break;

  case 664:

/* Line 1455 of yacc.c  */
#line 3924 "Grammar.ypp"
    {
    (yyval.saszsauva) = new SASzSAUva;
    memset((yyval.saszsauva), 0, sizeof(SASzSAUva));
    (yyval.saszsauva)->semantic = (yyvsp[(1) - (1)].s);
;}
    break;

  case 665:

/* Line 1455 of yacc.c  */
#line 3929 "Grammar.ypp"
    {
    (yyval.saszsauva) = new SASzSAUva;
    memset((yyval.saszsauva), 0, sizeof(SASzSAUva));
    (yyval.saszsauva)->semantic = (yyvsp[(1) - (2)].s);
;}
    break;

  case 666:

/* Line 1455 of yacc.c  */
#line 3934 "Grammar.ypp"
    {
    (yyval.saszsauva) = new SASzSAUva;
    memset((yyval.saszsauva), 0, sizeof(SASzSAUva));
    (yyval.saszsauva)->semantic = (yyvsp[(1) - (2)].s);
    (yyval.saszsauva)->valueList = (yyvsp[(2) - (2)].valueList);
;}
    break;

  case 667:

/* Line 1455 of yacc.c  */
#line 3940 "Grammar.ypp"
    {
    (yyval.saszsauva) = new SASzSAUva;
    memset((yyval.saszsauva), 0, sizeof(SASzSAUva));
    (yyval.saszsauva)->semantic = (yyvsp[(1) - (3)].s);
    (yyval.saszsauva)->valueList = (yyvsp[(3) - (3)].valueList);
;}
    break;

  case 668:

/* Line 1455 of yacc.c  */
#line 3946 "Grammar.ypp"
    {
    (yyval.saszsauva) = new SASzSAUva;
    memset((yyval.saszsauva), 0, sizeof(SASzSAUva));
    (yyval.saszsauva)->arraySz = (yyvsp[(1) - (1)].ival);
;}
    break;

  case 669:

/* Line 1455 of yacc.c  */
#line 3951 "Grammar.ypp"
    {
    (yyval.saszsauva) = new SASzSAUva;
    memset((yyval.saszsauva), 0, sizeof(SASzSAUva));
    (yyval.saszsauva)->arraySz = (yyvsp[(1) - (2)].ival);
;}
    break;

  case 670:

/* Line 1455 of yacc.c  */
#line 3956 "Grammar.ypp"
    {
    (yyval.saszsauva) = new SASzSAUva;
    memset((yyval.saszsauva), 0, sizeof(SASzSAUva));
    (yyval.saszsauva)->arraySz = (yyvsp[(1) - (2)].ival);
    (yyval.saszsauva)->valueList = (yyvsp[(2) - (2)].valueList);
;}
    break;

  case 671:

/* Line 1455 of yacc.c  */
#line 3962 "Grammar.ypp"
    {
    (yyval.saszsauva) = new SASzSAUva;
    memset((yyval.saszsauva), 0, sizeof(SASzSAUva));
    (yyval.saszsauva)->arraySz = (yyvsp[(1) - (3)].ival);
    (yyval.saszsauva)->valueList = (yyvsp[(3) - (3)].valueList);
;}
    break;

  case 672:

/* Line 1455 of yacc.c  */
#line 3968 "Grammar.ypp"
    {
    (yyval.saszsauva) = new SASzSAUva;
    memset((yyval.saszsauva), 0, sizeof(SASzSAUva));
    (yyval.saszsauva)->arraySz = (yyvsp[(1) - (2)].ival);
    (yyval.saszsauva)->semantic = (yyvsp[(2) - (2)].s);
;}
    break;

  case 673:

/* Line 1455 of yacc.c  */
#line 3974 "Grammar.ypp"
    {
    (yyval.saszsauva) = new SASzSAUva;
    memset((yyval.saszsauva), 0, sizeof(SASzSAUva));
    (yyval.saszsauva)->arraySz = (yyvsp[(1) - (3)].ival);
    (yyval.saszsauva)->semantic = (yyvsp[(2) - (3)].s);
;}
    break;

  case 674:

/* Line 1455 of yacc.c  */
#line 3980 "Grammar.ypp"
    {
    (yyval.saszsauva) = new SASzSAUva;
    memset((yyval.saszsauva), 0, sizeof(SASzSAUva));
    (yyval.saszsauva)->arraySz = (yyvsp[(1) - (3)].ival);
    (yyval.saszsauva)->semantic = (yyvsp[(2) - (3)].s);
    (yyval.saszsauva)->valueList = (yyvsp[(3) - (3)].valueList);
;}
    break;

  case 675:

/* Line 1455 of yacc.c  */
#line 3987 "Grammar.ypp"
    {
    (yyval.saszsauva) = new SASzSAUva;
    memset((yyval.saszsauva), 0, sizeof(SASzSAUva));
    (yyval.saszsauva)->arraySz = (yyvsp[(1) - (4)].ival);
    (yyval.saszsauva)->semantic = (yyvsp[(2) - (4)].s);
    (yyval.saszsauva)->valueList = (yyvsp[(4) - (4)].valueList);
;}
    break;

  case 676:

/* Line 1455 of yacc.c  */
#line 3999 "Grammar.ypp"
    { (yyval.uniformType) = nvFX::IUniform::TFloat; ;}
    break;

  case 677:

/* Line 1455 of yacc.c  */
#line 4000 "Grammar.ypp"
    { (yyval.uniformType) = nvFX::IUniform::TVec2; ;}
    break;

  case 678:

/* Line 1455 of yacc.c  */
#line 4001 "Grammar.ypp"
    { (yyval.uniformType) = nvFX::IUniform::TVec3; ;}
    break;

  case 679:

/* Line 1455 of yacc.c  */
#line 4002 "Grammar.ypp"
    { (yyval.uniformType) = nvFX::IUniform::TVec4; ;}
    break;

  case 680:

/* Line 1455 of yacc.c  */
#line 4003 "Grammar.ypp"
    { (yyval.uniformType) = nvFX::IUniform::TInt; ;}
    break;

  case 681:

/* Line 1455 of yacc.c  */
#line 4004 "Grammar.ypp"
    { (yyval.uniformType) = nvFX::IUniform::TInt2; ;}
    break;

  case 682:

/* Line 1455 of yacc.c  */
#line 4005 "Grammar.ypp"
    { (yyval.uniformType) = nvFX::IUniform::TInt3; ;}
    break;

  case 683:

/* Line 1455 of yacc.c  */
#line 4006 "Grammar.ypp"
    { (yyval.uniformType) = nvFX::IUniform::TInt4; ;}
    break;

  case 684:

/* Line 1455 of yacc.c  */
#line 4007 "Grammar.ypp"
    { (yyval.uniformType) = nvFX::IUniform::TBool; ;}
    break;

  case 685:

/* Line 1455 of yacc.c  */
#line 4008 "Grammar.ypp"
    { (yyval.uniformType) = nvFX::IUniform::TBool2; ;}
    break;

  case 686:

/* Line 1455 of yacc.c  */
#line 4009 "Grammar.ypp"
    { (yyval.uniformType) = nvFX::IUniform::TBool3; ;}
    break;

  case 687:

/* Line 1455 of yacc.c  */
#line 4010 "Grammar.ypp"
    { (yyval.uniformType) = nvFX::IUniform::TBool4; ;}
    break;

  case 688:

/* Line 1455 of yacc.c  */
#line 4011 "Grammar.ypp"
    { (yyval.uniformType) = nvFX::IUniform::TMat2; ;}
    break;

  case 689:

/* Line 1455 of yacc.c  */
#line 4012 "Grammar.ypp"
    { (yyval.uniformType) = nvFX::IUniform::TMat3; ;}
    break;

  case 690:

/* Line 1455 of yacc.c  */
#line 4013 "Grammar.ypp"
    { (yyval.uniformType) = nvFX::IUniform::TMat4; ;}
    break;

  case 691:

/* Line 1455 of yacc.c  */
#line 4014 "Grammar.ypp"
    { (yyval.uniformType) = nvFX::IUniform::TTexture1D; ;}
    break;

  case 692:

/* Line 1455 of yacc.c  */
#line 4015 "Grammar.ypp"
    { (yyval.uniformType) = nvFX::IUniform::TTexture2D; ;}
    break;

  case 693:

/* Line 1455 of yacc.c  */
#line 4016 "Grammar.ypp"
    { (yyval.uniformType) = nvFX::IUniform::TTexture2DShadow; ;}
    break;

  case 694:

/* Line 1455 of yacc.c  */
#line 4017 "Grammar.ypp"
    { (yyval.uniformType) = nvFX::IUniform::TTexture2DRect; ;}
    break;

  case 695:

/* Line 1455 of yacc.c  */
#line 4018 "Grammar.ypp"
    { (yyval.uniformType) = nvFX::IUniform::TTexture3D; ;}
    break;

  case 696:

/* Line 1455 of yacc.c  */
#line 4019 "Grammar.ypp"
    { (yyval.uniformType) = nvFX::IUniform::TTextureCube; ;}
    break;

  case 697:

/* Line 1455 of yacc.c  */
#line 4020 "Grammar.ypp"
    { (yyval.uniformType) = nvFX::IUniform::TTexture1D; ;}
    break;

  case 698:

/* Line 1455 of yacc.c  */
#line 4021 "Grammar.ypp"
    { (yyval.uniformType) = nvFX::IUniform::TTexture2D; ;}
    break;

  case 699:

/* Line 1455 of yacc.c  */
#line 4022 "Grammar.ypp"
    { (yyval.uniformType) = nvFX::IUniform::TTexture2DShadow; ;}
    break;

  case 700:

/* Line 1455 of yacc.c  */
#line 4023 "Grammar.ypp"
    { (yyval.uniformType) = nvFX::IUniform::TTexture2DRect; ;}
    break;

  case 701:

/* Line 1455 of yacc.c  */
#line 4024 "Grammar.ypp"
    { (yyval.uniformType) = nvFX::IUniform::TTexture3D; ;}
    break;

  case 702:

/* Line 1455 of yacc.c  */
#line 4025 "Grammar.ypp"
    { (yyval.uniformType) = nvFX::IUniform::TTextureCube; ;}
    break;

  case 703:

/* Line 1455 of yacc.c  */
#line 4026 "Grammar.ypp"
    { yyerror("Error : unknown type\n"); ;}
    break;

  case 704:

/* Line 1455 of yacc.c  */
#line 4032 "Grammar.ypp"
    {
        curAnnotations = IAnnotationEx::getAnnotationSingleton(); // need a temporary place
        curAnnotations->clear();
    ;}
    break;

  case 705:

/* Line 1455 of yacc.c  */
#line 4035 "Grammar.ypp"
    {
    SUniformQualifiers &q = (yyvsp[(1) - (6)].uniformQualifiers);
    IUniform::Type     &t = (yyvsp[(2) - (6)].uniformType);
    std::string*            &varName = (yyvsp[(3) - (6)].s);
    SASzSAUva*              &a = (yyvsp[(5) - (6)].saszsauva);
    // for the case of items within a namespace :
    // copy the name-space annotations to this one
    if(curAnnotations) curAnnotations->copy(namespaceAnnotations);

    createUniform(t, varName->c_str(), a->semantic?a->semantic->c_str():NULL, q.global, q.precisionType);
    if(curUniform == NULL)
    {
        yyerror("(DEBUG error) curUniform == NULL");
        yyerror(varName->c_str());
    } else {
        if(a->arraySz > 1)
            curUniform->getExInterface()->setArraySz(a->arraySz);
        // Now we copy the possible annotations that were filled in arraysz_semantic_annotations_uniformvalueassigment
        curUniform->annotations()->getExInterface()->clear();
        curUniform->annotations()->getExInterface()->copy(curAnnotations);
        curAnnotations->clear();
        curAnnotations = curUniform->annotations()->getExInterface();
    }
    if(a->valueList)
    {
        if(a->valueList->floatVecList)
        {
            float *farray;
            int dim = getTypeDim(t);
            int sz = (int)a->valueList->floatVecList->size();
            if(a->arraySz <= 0)
                a->arraySz = (int)sz;
            farray = flattenFVecList(a->arraySz, dim, a->valueList->floatVecList);
            if(farray)
            {
                curUniform->setValuefv(farray, dim, a->arraySz, 0);
                delete farray;
            }
            delete a->valueList->floatVecList;
        }
        else if(a->valueList->intVecList)
        {
            int *farray;
            int dim = getTypeDim(t);
            int sz = (int)a->valueList->intVecList->size();
            if(a->arraySz <= 0)
                a->arraySz = (int)sz;
            farray = flattenIVecList(a->arraySz, dim, a->valueList->intVecList);
            if(farray)
            {
                curUniform->setValueiv(farray, dim, a->arraySz, 0);
                delete farray;
            }
            delete a->valueList->intVecList;
        }
        else if(a->valueList->boolVecList)
        {
            bool *farray;
            int dim = getTypeDim(t);
            int sz = (int)a->valueList->boolVecList->size();
            if(a->arraySz <= 0)
                a->arraySz = (int)sz;
            farray = flattenBVecList(a->arraySz, dim, a->valueList->boolVecList);
            if(farray)
            {
                curUniform->setValuebv(farray, dim, a->arraySz, 0);
                delete farray;
            }
            delete a->valueList->boolVecList;
        }
        delete a->valueList;
    }
    if(a->semantic) delete a->semantic;
    delete a;
    delete varName;
    curUniform = NULL;
    curAnnotations = NULL;
  ;}
    break;

  case 727:

/* Line 1455 of yacc.c  */
#line 4155 "Grammar.ypp"
    {
    LOGD("Namespace %s\n", (yyvsp[(2) - (2)].s)->c_str() );
    namespaceName = *(yyvsp[(2) - (2)].s);
    curAnnotations = IAnnotationEx::getAnnotationSingleton(1); // need a temporary place
    curAnnotations->clear();
    // keep the pointer here because the idea of namespace annotations is to copy them later
    // on every single item in the namespace
    namespaceAnnotations = curAnnotations;
;}
    break;

  case 728:

/* Line 1455 of yacc.c  */
#line 4163 "Grammar.ypp"
    { 
    namespaceName.clear(); 
    curAnnotations = NULL;
    namespaceAnnotations = NULL;
    delete (yyvsp[(2) - (7)].s);
;}
    break;

  case 732:

/* Line 1455 of yacc.c  */
#line 4181 "Grammar.ypp"
    {
    if((yyvsp[(2) - (2)].global_varname_annots).varName == NULL)
        yyerror("a Constant buffer must have a name\n");
    LOGD("ConstantBuffer %s\n", (yyvsp[(2) - (2)].global_varname_annots).varName ? (yyvsp[(2) - (2)].global_varname_annots).varName->c_str() : "Un-named" );
    ICstBuffer* p = curContainer->createCstBuffer((yyvsp[(2) - (2)].global_varname_annots).varName ? (yyvsp[(2) - (2)].global_varname_annots).varName->c_str() : NULL, (yyvsp[(2) - (2)].global_varname_annots).global);
    // it is possible that it failed because already a Global one with same name is here
    if(p == NULL)
    {
        if((yyvsp[(2) - (2)].global_varname_annots).varName)
            p = pCRep->find((yyvsp[(2) - (2)].global_varname_annots).varName ? (yyvsp[(2) - (2)].global_varname_annots).varName->c_str() : NULL);
        if(p == NULL)
            yyerror("Error in Constant Buffer creation\n");
        else {
            curContainer->addCstBuffer(p);
            pCRep->addCstBuffer(p);
            bCurCstBufferAlreadyPopulated = true; // we prevent sub-uniforms to be created during this parsing
        }
    } else {
        bCurCstBufferAlreadyPopulated = false;
    }
    curCstBuffer = p ? p->getExInterface() : NULL;
    if(curCstBuffer)
    {
        IAnnotationEx* pA = curCstBuffer->annotations()->getExInterface();
        pA->copy(curAnnotations);
        // for the case of items within a namespace :
        // copy the name-space annotations to this one
        pA->copy(namespaceAnnotations);
    }

    delete (yyvsp[(2) - (2)].global_varname_annots).varName;
;}
    break;

  case 733:

/* Line 1455 of yacc.c  */
#line 4212 "Grammar.ypp"
    {
    curCstBuffer = NULL;
    curAnnotations = NULL;
    lex_pop_state();
;}
    break;

  case 734:

/* Line 1455 of yacc.c  */
#line 4223 "Grammar.ypp"
    {
    delete (yyvsp[(4) - (6)].varNameList);
    ;}
    break;

  case 735:

/* Line 1455 of yacc.c  */
#line 4226 "Grammar.ypp"
    {
    delete (yyvsp[(5) - (7)].varNameList);
    ;}
    break;

  case 736:

/* Line 1455 of yacc.c  */
#line 4229 "Grammar.ypp"
    {
    delete (yyvsp[(4) - (5)].s)
    ;}
    break;

  case 737:

/* Line 1455 of yacc.c  */
#line 4232 "Grammar.ypp"
    {
    delete (yyvsp[(4) - (5)].s)
    ;}
    break;

  case 738:

/* Line 1455 of yacc.c  */
#line 4235 "Grammar.ypp"
    {
    delete (yyvsp[(4) - (5)].s)
    ;}
    break;

  case 739:

/* Line 1455 of yacc.c  */
#line 4238 "Grammar.ypp"
    {
    delete (yyvsp[(4) - (5)].s)
    ;}
    break;

  case 740:

/* Line 1455 of yacc.c  */
#line 4241 "Grammar.ypp"
    {
;}
    break;

  case 741:

/* Line 1455 of yacc.c  */
#line 4243 "Grammar.ypp"
    {
    delete (yyvsp[(4) - (5)].s)
    ;}
    break;

  case 742:

/* Line 1455 of yacc.c  */
#line 4246 "Grammar.ypp"
    {
    // we prefer this loop : we don't have the char ** available here... using vector of std::string...
    //for(int i=0; i<(int)$8->size(); i++)
    //    curXX->createState($4->c_str(), i, (*$8)[i].c_str());
    delete (yyvsp[(4) - (10)].s);
    delete (yyvsp[(8) - (10)].varNameList);
    ;}
    break;

  case 743:

/* Line 1455 of yacc.c  */
#line 4253 "Grammar.ypp"
    {
    //curPass->createState($4->c_str(), 0, $7->c_str());
    delete (yyvsp[(4) - (8)].s);
    delete (yyvsp[(7) - (8)].s);
    ;}
    break;

  case 744:

/* Line 1455 of yacc.c  */
#line 4258 "Grammar.ypp"
    {
    ValueList* pvalueList = (yyvsp[(6) - (7)].valueList);
    if(pvalueList && pvalueList->floatVecList)
    {
        FloatVecList *pfvl = pvalueList->floatVecList;
        FloatList* pfl = (*pfvl)[0];
        float* pf = &(*pfl)[0];
        //curPass->createState($4->c_str(), 0, pf, (int)pfl->size());
        delete pfl;
        delete pfvl;
        delete pvalueList;
    }
    delete (yyvsp[(4) - (7)].s);
    ;}
    break;

  case 745:

/* Line 1455 of yacc.c  */
#line 4274 "Grammar.ypp"
    {
   ;}
    break;

  case 746:

/* Line 1455 of yacc.c  */
#line 4275 "Grammar.ypp"
    {
   ;}
    break;

  case 747:

/* Line 1455 of yacc.c  */
#line 4276 "Grammar.ypp"
    {
   ;}
    break;

  case 749:

/* Line 1455 of yacc.c  */
#line 4285 "Grammar.ypp"
    {
    delete (yyvsp[(5) - (7)].varNameList);
    ;}
    break;

  case 750:

/* Line 1455 of yacc.c  */
#line 4288 "Grammar.ypp"
    {
    delete (yyvsp[(5) - (7)].varNameList);
    ;}
    break;

  case 751:

/* Line 1455 of yacc.c  */
#line 4291 "Grammar.ypp"
    {
    // we prefer this loop : we don't have the char ** available here... using vector of std::string...
    //for(int i=0; i<(int)$8->size(); i++)
    //    curXX->createState($4->c_str(), i, (*$8)[i].c_str());
    delete (yyvsp[(4) - (10)].s);
    delete (yyvsp[(8) - (10)].varNameList);
    ;}
    break;

  case 752:

/* Line 1455 of yacc.c  */
#line 4298 "Grammar.ypp"
    {
    //curPass->createState($4->c_str(), 0, $7->c_str());
    delete (yyvsp[(4) - (8)].s);
    delete (yyvsp[(7) - (8)].s);
    ;}
    break;

  case 753:

/* Line 1455 of yacc.c  */
#line 4303 "Grammar.ypp"
    {
    ValueList* pvalueList = (yyvsp[(6) - (7)].valueList);
    if(pvalueList && pvalueList->floatVecList)
    {
        FloatVecList *pfvl = pvalueList->floatVecList;
        FloatList* pfl = (*pfvl)[0];
        float* pf = &(*pfl)[0];
        //curPass->createState($4->c_str(), 0, pf, (int)pfl->size());
        delete pfl;
        delete pfvl;
        delete pvalueList;
    }
    delete (yyvsp[(4) - (7)].s);
    ;}
    break;

  case 754:

/* Line 1455 of yacc.c  */
#line 4319 "Grammar.ypp"
    {
   ;}
    break;

  case 755:

/* Line 1455 of yacc.c  */
#line 4320 "Grammar.ypp"
    {
   ;}
    break;

  case 756:

/* Line 1455 of yacc.c  */
#line 4321 "Grammar.ypp"
    {
   ;}
    break;

  case 758:

/* Line 1455 of yacc.c  */
#line 4330 "Grammar.ypp"
    {
    //if(curResource) {
    //    if(!curResource->setFormat($4))
    //    {
    //        yyerror("Error: Bad format\n");
    //    }
    //}
;}
    break;

  case 759:

/* Line 1455 of yacc.c  */
#line 4338 "Grammar.ypp"
    {
    //if(curResource)
    //    curResource->setDimensionsApplicationDependent();
;}
    break;

  case 760:

/* Line 1455 of yacc.c  */
#line 4342 "Grammar.ypp"
    {
    int d = (int)(yyvsp[(5) - (7)].intList)->size();
    //if(curResource)
    //    curResource->setDimensions((*$5)[0], d > 1 ? (*$5)[1] : 1, d > 2 ? (*$5)[2] : 1);
    delete (yyvsp[(5) - (7)].intList);
;}
    break;

  case 761:

/* Line 1455 of yacc.c  */
#line 4350 "Grammar.ypp"
    {
    delete (yyvsp[(2) - (2)].s);
   ;}
    break;

  case 762:

/* Line 1455 of yacc.c  */
#line 4352 "Grammar.ypp"
    {
   ;}
    break;

  case 763:

/* Line 1455 of yacc.c  */
#line 4353 "Grammar.ypp"
    {
   ;}
    break;

  case 765:

/* Line 1455 of yacc.c  */
#line 4362 "Grammar.ypp"
    {
    delete (yyvsp[(4) - (5)].s);
;}
    break;

  case 766:

/* Line 1455 of yacc.c  */
#line 4365 "Grammar.ypp"
    {
    delete (yyvsp[(4) - (5)].s);
;}
    break;

  case 767:

/* Line 1455 of yacc.c  */
#line 4370 "Grammar.ypp"
    {
   ;}
    break;

  case 768:

/* Line 1455 of yacc.c  */
#line 4371 "Grammar.ypp"
    {
   ;}
    break;

  case 769:

/* Line 1455 of yacc.c  */
#line 4372 "Grammar.ypp"
    {
   ;}
    break;



/* Line 1455 of yacc.c  */
#line 10734 "D:/FromTiers/git/nvFX/FxParser/Grammar.cpp"
      default: break;
    }
  YY_SYMBOL_PRINT ("-> $$ =", yyr1[yyn], &yyval, &yyloc);

  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);

  *++yyvsp = yyval;

  /* Now `shift' the result of the reduction.  Determine what state
     that goes to, based on the state we popped back to and the rule
     number reduced by.  */

  yyn = yyr1[yyn];

  yystate = yypgoto[yyn - YYNTOKENS] + *yyssp;
  if (0 <= yystate && yystate <= YYLAST && yycheck[yystate] == *yyssp)
    yystate = yytable[yystate];
  else
    yystate = yydefgoto[yyn - YYNTOKENS];

  goto yynewstate;


/*------------------------------------.
| yyerrlab -- here on detecting error |
`------------------------------------*/
yyerrlab:
  /* If not already recovering from an error, report this error.  */
  if (!yyerrstatus)
    {
      ++yynerrs;
#if ! YYERROR_VERBOSE
      yyerror (YY_("syntax error"));
#else
      {
	YYSIZE_T yysize = yysyntax_error (0, yystate, yychar);
	if (yymsg_alloc < yysize && yymsg_alloc < YYSTACK_ALLOC_MAXIMUM)
	  {
	    YYSIZE_T yyalloc = 2 * yysize;
	    if (! (yysize <= yyalloc && yyalloc <= YYSTACK_ALLOC_MAXIMUM))
	      yyalloc = YYSTACK_ALLOC_MAXIMUM;
	    if (yymsg != yymsgbuf)
	      YYSTACK_FREE (yymsg);
	    yymsg = (char *) YYSTACK_ALLOC (yyalloc);
	    if (yymsg)
	      yymsg_alloc = yyalloc;
	    else
	      {
		yymsg = yymsgbuf;
		yymsg_alloc = sizeof yymsgbuf;
	      }
	  }

	if (0 < yysize && yysize <= yymsg_alloc)
	  {
	    (void) yysyntax_error (yymsg, yystate, yychar);
	    yyerror (yymsg);
	  }
	else
	  {
	    yyerror (YY_("syntax error"));
	    if (yysize != 0)
	      goto yyexhaustedlab;
	  }
      }
#endif
    }



  if (yyerrstatus == 3)
    {
      /* If just tried and failed to reuse lookahead token after an
	 error, discard it.  */

      if (yychar <= YYEOF)
	{
	  /* Return failure if at end of input.  */
	  if (yychar == YYEOF)
	    YYABORT;
	}
      else
	{
	  yydestruct ("Error: discarding",
		      yytoken, &yylval);
	  yychar = YYEMPTY;
	}
    }

  /* Else will try to reuse lookahead token after shifting the error
     token.  */
  goto yyerrlab1;


/*---------------------------------------------------.
| yyerrorlab -- error raised explicitly by YYERROR.  |
`---------------------------------------------------*/
yyerrorlab:

  /* Pacify compilers like GCC when the user code never invokes
     YYERROR and the label yyerrorlab therefore never appears in user
     code.  */
  if (/*CONSTCOND*/ 0)
     goto yyerrorlab;

  /* Do not reclaim the symbols of the rule which action triggered
     this YYERROR.  */
  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);
  yystate = *yyssp;
  goto yyerrlab1;


/*-------------------------------------------------------------.
| yyerrlab1 -- common code for both syntax error and YYERROR.  |
`-------------------------------------------------------------*/
yyerrlab1:
  yyerrstatus = 3;	/* Each real token shifted decrements this.  */

  for (;;)
    {
      yyn = yypact[yystate];
      if (yyn != YYPACT_NINF)
	{
	  yyn += YYTERROR;
	  if (0 <= yyn && yyn <= YYLAST && yycheck[yyn] == YYTERROR)
	    {
	      yyn = yytable[yyn];
	      if (0 < yyn)
		break;
	    }
	}

      /* Pop the current state because it cannot handle the error token.  */
      if (yyssp == yyss)
	YYABORT;


      yydestruct ("Error: popping",
		  yystos[yystate], yyvsp);
      YYPOPSTACK (1);
      yystate = *yyssp;
      YY_STACK_PRINT (yyss, yyssp);
    }

  *++yyvsp = yylval;


  /* Shift the error token.  */
  YY_SYMBOL_PRINT ("Shifting", yystos[yyn], yyvsp, yylsp);

  yystate = yyn;
  goto yynewstate;


/*-------------------------------------.
| yyacceptlab -- YYACCEPT comes here.  |
`-------------------------------------*/
yyacceptlab:
  yyresult = 0;
  goto yyreturn;

/*-----------------------------------.
| yyabortlab -- YYABORT comes here.  |
`-----------------------------------*/
yyabortlab:
  yyresult = 1;
  goto yyreturn;

#if !defined(yyoverflow) || YYERROR_VERBOSE
/*-------------------------------------------------.
| yyexhaustedlab -- memory exhaustion comes here.  |
`-------------------------------------------------*/
yyexhaustedlab:
  yyerror (YY_("memory exhausted"));
  yyresult = 2;
  /* Fall through.  */
#endif

yyreturn:
  if (yychar != YYEMPTY)
     yydestruct ("Cleanup: discarding lookahead",
		 yytoken, &yylval);
  /* Do not reclaim the symbols of the rule which action triggered
     this YYABORT or YYACCEPT.  */
  YYPOPSTACK (yylen);
  YY_STACK_PRINT (yyss, yyssp);
  while (yyssp != yyss)
    {
      yydestruct ("Cleanup: popping",
		  yystos[*yyssp], yyvsp);
      YYPOPSTACK (1);
    }
#ifndef yyoverflow
  if (yyss != yyssa)
    YYSTACK_FREE (yyss);
#endif
#if YYERROR_VERBOSE
  if (yymsg != yymsgbuf)
    YYSTACK_FREE (yymsg);
#endif
  /* Make sure YYID is used.  */
  return YYID (yyresult);
}



/* Line 1675 of yacc.c  */
#line 4377 "Grammar.ypp"


