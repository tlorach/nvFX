
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
            p = curContainer->createUniform(name, namespaceName.c_str(), sem, global);
            if(p) {
                p->getExInterface()->setType(T);
                p->getExInterface()->setPrecision(precision);
            } else { // failed. Try to find it in the global repository
                if(global)
                    p = pURep->find(name);
                if(p==NULL)
                    yyerror("failed to create global uniform\n");
                else if(p->getType() != T)
                    yyerror("uniform differs in its type the the global instance of the same name. Make it local ?\n");
                else {
                    curContainer->addUniform(p);
                    pURep->addUniform(p);
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
#line 266 "C:/p4/GitHub/nvFX/FxParser/Grammar.cpp"

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
#line 220 "Grammar.ypp"

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
#line 464 "C:/p4/GitHub/nvFX/FxParser/Grammar.cpp"

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
     TEXTURE2DRECT_TYPE = 308,
     TEXTURE3D_TYPE = 309,
     TEXTURECUBE_TYPE = 310,
     SAMPLER1D_TYPE = 311,
     SAMPLER2D_TYPE = 312,
     SAMPLER2DRECT_TYPE = 313,
     SAMPLER3D_TYPE = 314,
     SAMPLERCUBE_TYPE = 315,
     EXTENSION = 316,
     SEPARATE_SHADER = 317,
     DEPTHMASK = 100,
     DEPTHFUNC = 101,
     VERTEXPROGRAM = 318,
     FRAGMENTPROGRAM = 319,
     GEOMETRYPROGRAM = 320,
     HULLPROGRAM = 321,
     EVALPROGRAM = 322,
     SHDPROFILE = 323,
     SAMPLERRESOURCE = 324,
     SAMPLERTEXUNIT = 325,
     SETSAMPLERSTATE = 326,
     SETDSTSTATE = 327,
     SETRASTERIZATIONSTATE = 328,
     SETCOLORSAMPLESTATE = 329,
     IMAGERESOURCE = 330,
     IMAGEUNIT = 331,
     IMAGEACCESS = 332,
     IMAGELAYER = 333,
     IMAGELAYERED = 334,
     WRITE_ONLY = 335,
     READ_ONLY = 336,
     READ_WRITE = 337,
     VERTEXPROGRAMOVERRIDE = 338,
     FRAGMENTPROGRAMOVERRIDE = 339,
     GEOMETRYPROGRAMOVERRIDE = 340,
     HULLPROGRAMOVERRIDE = 341,
     EVALPROGRAMOVERRIDE = 342,
     IMAGEACCESSOVERRIDE = 343,
     IMAGELAYEROVERRIDE = 344,
     IMAGELAYEREDOVERRIDE = 345,
     IMAGERESOURCEOVERRIDE = 346,
     UNIFORMOVERRIDE = 347,
     IMAGEUNITOVERRIDE = 348,
     TEXTURERESOURCEOVERRIDE = 349,
     SAMPLERRESOURCEOVERRIDE = 350,
     SAMPLERTEXUNITOVERRIDE = 351,
     SETSAMPLERSTATEOVERRIDE = 352,
     GLACCUM = 353,
     GLLOAD = 354,
     GLRETURN = 355,
     GLMULT = 356,
     GLADD = 357,
     GLNEVER = 358,
     GLLESS = 359,
     GLEQUAL = 360,
     GLGREATER = 361,
     GLNOTEQUAL = 362,
     GLALWAYS = 363,
     GLLEQUAL = 364,
     GLGEQUAL = 365,
     GLKEEP = 366,
     GLREPLACE = 367,
     GLINCR = 368,
     GLDECR = 369,
     GLUPPERLEFT = 370,
     GLLOWERLEFT = 371,
     GLFRONTLEFT = 372,
     GLFRONTRIGHT = 373,
     GLBACKLEFT = 374,
     GLBACKRIGHT = 375,
     GLFRONT = 376,
     GLBACK = 377,
     GLLEFT = 378,
     GLRIGHT = 379,
     GLFRONTANDBACK = 380,
     GLCCW = 381,
     GLCW = 382,
     GLPOINT = 383,
     GLLINE = 384,
     GLFILL = 385,
     GLZERO = 386,
     GLONE = 387,
     GLSRCCOLOR = 388,
     GLONEMINUSSRCCOLOR = 389,
     GLSRCALPHA = 390,
     GLONEMINUSSRCALPHA = 391,
     GLDSTALPHA = 392,
     GLONEMINUSDSTALPHA = 393,
     GLDSTCOLOR = 394,
     GLONEMINUSDSTCOLOR = 395,
     GLSRCALPHASATURATE = 396,
     GLCONSTANTCOLOR = 397,
     GLONEMINUSCONSTANTCOLOR = 398,
     GLCONSTANTALPHA = 399,
     GLONEMINUSCONSTANTALPHA = 400,
     GLFUNCADD = 401,
     GLMIN = 402,
     GLMAX = 403,
     GLFUNCSUBTRACT = 404,
     GLFUNCREVERSESUBTRACT = 405,
     GLCLEAR = 406,
     GLSET = 407,
     GLCOPY = 408,
     GLCOPYINVERTED = 409,
     GLNOOP = 410,
     GLINVERT = 411,
     GLAND = 412,
     GLNAND = 413,
     GLOR = 414,
     GLNOR = 415,
     GLXOR = 416,
     GLEQUIV = 417,
     GLANDREVERSE = 418,
     GLANDINVERTED = 419,
     GLORREVERSE = 420,
     GLORINVERTED = 421,
     GLTEXTUREMAGFILTER = 422,
     GLTEXTUREMINFILTER = 423,
     GLTEXTUREWRAPS = 424,
     GLTEXTUREWRAPT = 425,
     GLTEXTUREWRAPR = 426,
     GLNEAREST = 427,
     GLLINEAR = 428,
     GLNEARESTMIPMAPNEAREST = 429,
     GLLINEARMIPMAPNEAREST = 430,
     GLNEARESTMIPMAPLINEAR = 431,
     GLLINEARMIPMAPLINEAR = 432,
     GLCLAMPTOEDGE = 433,
     GLCLAMP = 434,
     GLREPEAT = 435,
     GLMIRROREDREPEAT = 436,
     GLPOINTSIZE = 437,
     GLPOINTSMOOTH = 438,
     GLPOINTSPRITE = 439,
     GLPROGRAMPOINTSIZE = 440,
     GLPOINTSPRITECOORDORIGIN = 441,
     GLLINEWIDTH = 442,
     GLLINESMOOTH = 443,
     GLCOMBINEDLINESTIPPLE = 444,
     GLLINESTIPPLEPATTERN = 445,
     GLLINESTIPPLEREPEAT = 446,
     GLLINESTIPPLE = 447,
     GLCULLFACE = 448,
     GLCULLFACEMODE = 449,
     GLFRONTFACE = 450,
     GLPOLYGONSMOOTH = 451,
     GLPOLYGONMODE = 452,
     GLPOLYGONOFFSET = 453,
     GLPOLYGONOFFSETFACTOR = 454,
     GLPOLYGONOFFSETUNITS = 455,
     GLPOLYGONOFFSETPOINT = 456,
     GLPOLYGONOFFSETLINE = 457,
     GLPOLYGONOFFSETFILL = 458,
     GLPOLYGONSTIPPLEBIT = 459,
     GLRASTERIZERDISCARD = 460,
     GLALPHATEST = 461,
     GLALPHAFUNC = 462,
     GLALPHATESTFUNC = 463,
     GLALPHATESTREF = 464,
     GLBLENDFUNC = 465,
     GLBLENDFUNCSEPARATE = 466,
     GLBLEND = 467,
     GLBLENDSRC = 468,
     GLBLENDSRCRGB = 469,
     GLBLENDSRCALPHA = 470,
     GLBLENDDST = 471,
     GLBLENDDSTRGB = 472,
     GLBLENDDSTALPHA = 473,
     GLBLENDEQUATIONSEPARATE = 474,
     GLBLENDEQUATION = 475,
     GLBLENDEQUATIONRGB = 476,
     GLBLENDEQUATIONALPHA = 477,
     GLBLENDCOLOR = 478,
     GLFRAMEBUFFERSRGB = 479,
     GLDITHER = 480,
     GLCOLORLOGICOP = 481,
     GLLOGICOPMODE = 482,
     GLCOLORWRITEMASK = 483,
     GLMULTISAMPLE = 484,
     GLSAMPLEMASK = 485,
     GLSAMPLEMASKVALUE = 486,
     GLSAMPLEALPHATOONE = 487,
     GLSAMPLEALPHATOCOVERAGE = 488,
     GLDEPTHTEST = 489,
     GLDEPTHFUNC = 490,
     GLDEPTHWRITEMASK = 491,
     GLDEPTHCLAMP = 492,
     GLDEPTHBOUNDSTESTEXT = 493,
     GLDEPTHBOUNDSEXT = 494,
     GLSTENCILTEST = 495,
     GLSTENCILFUNC = 496,
     GLSTENCILFUNCSEPARATE = 497,
     GLSTENCILVALUEMASK = 498,
     GLSTENCILREF = 499,
     GLSTENCILFAIL = 500,
     GLSTENCILPASSDEPTHFAIL = 501,
     GLSTENCILPASSDEPTHPASS = 502,
     GLSTENCILOP = 503,
     GLSTENCILOPSEPARATE = 504,
     GLSTENCILBACKOP = 505,
     GLSTENCILWRITEMASK = 506,
     GLSTENCILBACKFUNC = 507,
     GLSTENCILBACKVALUEMASK = 508,
     GLSTENCILBACKREF = 509,
     GLSTENCILBACKFAIL = 510,
     GLSTENCILBACKPASSDEPTHFAIL = 511,
     GLSTENCILBACKPASSDEPTHPASS = 512,
     GLSTENCILBACKWRITEMASK = 513,
     ATTRIBUTE = 514,
     RENDERGROUP = 515,
     RENDERMODE = 516,
     RENDERMODE_ENUM = 517,
     BLITFBOTOACTIVETARGET = 518,
     CLEARMODE = 519,
     CLEAR_ENUM = 520,
     CURRENTTARGET = 521,
     CLEARCOLOR = 522,
     VIEWPORT = 523,
     FBO = 524,
     RENDERBUFFER = 525,
     RENDERTEXTURE = 526,
     RENDERTEXTURERECT = 527,
     DSTTARGET = 528,
     COLORTARGETS = 529,
     RESOURCEMSAA = 530,
     RESOURCESIZE = 531,
     RESOURCESIZEFACTOR = 532,
     RESOURCERESOLVETARGET = 533,
     RESOURCEFORMAT = 534,
     RESOURCEFORMAT_ENUM = 535,
     APPDEFINED = 536,
     SWAPRESOURCES = 537,
     LOOP = 538,
     ACTIVE = 539,
     OFF = 540,
     GLOBAL = 541,
     LOCAL = 542,
     CUDAKERNEL = 543,
     CUDACODE = 544,
     CUDAMODULE = 545,
     CUDASHAREDMEMORY = 546,
     CUDAGRID = 547,
     CUDABLOCK = 548,
     OPTIX_CONTEXT = 549,
     OPTIX_CONTEXT_ENTRY = 550,
     RAY_GENERATION_PROGRAM = 551,
     EXCEPTION_PROGRAM = 552,
     INTERSECTION_PROGRAM = 553,
     BOUNDING_BOX_PROGRAM = 554,
     MISS_PROGRAM = 555,
     DEFAULT_MATERIAL = 556,
     STACK_SIZE = 557,
     OUTPUT_BUFFER = 558,
     OPTIX_MATERIAL = 559,
     CLOSEST_HIT_PROGRAM = 560,
     ANY_HIT_PROGRAM = 561,
     OPTIX_BUFFER = 562,
     PTX_PROGRAM_FROM_FILE = 563,
     FILE_NAME = 564,
     ENTRY_POINT = 565,
     COMPUTEGROUPS = 566,
     COMPUTEPROGRAM = 567,
     COMPUTEPROGRAMOVERRIDE = 568,
     SVGPATH = 569,
     PSPATH = 570,
     PATHSOURCEOVERRIDE = 571,
     PATHSOURCE = 572,
     GLPATHSTENCILDEPTHOFFSET = 573,
     GLPATHSTENCILFUNC = 574,
     GLPATHFOGGEN = 575,
     GLPATHTEXGEN = 576,
     GLPATHCOLORGEN = 577,
     GLPATHCOVERDEPTHFUNC = 578,
     GLPATHSTROKEWIDTH = 579,
     GLPATHINITIALENDCAP = 580,
     GLPATHTERMINALENDCAP = 581,
     GLPATHINITIALDASHCAP = 582,
     GLPATHTERMINALDASHCAP = 583,
     GLPATHJOINSTYLE = 584,
     GLPATHMITERLIMIT = 585,
     GLPATHDASHOFFSET = 586,
     GLPATHDASHOFFSETRESET = 587,
     GLPATHCLIENTLENGTH = 588,
     GLPATHFILLMODE = 589,
     GLPATHFILLMASK = 590,
     GLPATHFILLCOVERMODE = 591,
     GLPATHSTROKEMASK = 592,
     GLPATHSTROKEREFERENCE = 593,
     GLPATHSTROKECOVERMODE = 594,
     GLPATHTRANSFORMTYPE = 595,
     GLPATHTRANSFORM = 596,
     GLFOGCOORDINATE = 597,
     GLFRAGMENTDEPTH = 598,
     GLOBJECTLINEAR = 599,
     GLPATHOBJECTBOUNDINGBOX = 600,
     GLEYELINEAR = 601,
     GLFLAT = 602,
     GLSQUARE = 603,
     GLROUND = 604,
     GLTRIANGULAR = 605,
     GLMITERREVERT = 606,
     GLMITERTRUNCATE = 607,
     GLBEVEL = 608,
     GLMOVETORESET = 609,
     GLMOVETOCONTINUES = 610,
     GLCOUNTUP = 611,
     GLCOUNTDOWN = 612,
     GLBOUNDINGBOX = 613,
     GLCONVEXHULL = 614,
     GLTRANSLATE2D = 615,
     GLTRANSLATEX = 616,
     GLTRANSLATEY = 617,
     GLTRANSLATE3D = 618,
     GLAFFINE2D = 619,
     GLAFFINE3D = 620,
     GLTRANSPOSEAFFINE2D = 621,
     GLTRANSPOSEAFFINE3D = 622,
     GLLUMINANCE = 623,
     GLALPHA = 624,
     GLINTENSITY = 625,
     GLLUMINANCEALPHA = 626,
     GLRGB = 627,
     GLRGBA = 628,
     NEG = 629
   };
#endif



#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
typedef union YYSTYPE
{

/* Line 214 of yacc.c  */
#line 422 "Grammar.ypp"

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
#line 895 "C:/p4/GitHub/nvFX/FxParser/Grammar.cpp"
} YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
#endif


/* Copy the second part of user declarations.  */


/* Line 264 of yacc.c  */
#line 907 "C:/p4/GitHub/nvFX/FxParser/Grammar.cpp"

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
#define YYLAST   5006

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  396
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  125
/* YYNRULES -- Number of rules.  */
#define YYNRULES  748
/* YYNRULES -- Number of states.  */
#define YYNSTATES  2123

/* YYTRANSLATE(YYLEX) -- Bison symbol number corresponding to YYLEX.  */
#define YYUNDEFTOK  2
#define YYMAXUTOK   629

#define YYTRANSLATE(YYX)						\
  ((unsigned int) (YYX) <= YYMAXUTOK ? yytranslate[YYX] : YYUNDEFTOK)

/* YYTRANSLATE[YYLEX] -- Bison symbol number corresponding to YYLEX.  */
static const yytype_uint16 yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     385,   386,   379,   378,   387,   377,   395,   380,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,   384,   383,
     391,   376,   392,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,   393,     2,   394,   382,     2,     2,     2,     2,     2,
      63,    64,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,   388,   390,   389,     2,     2,     2,     2,
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
      55,    56,    57,    58,    59,    60,    61,    62,    65,    66,
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
     367,   368,   369,   370,   371,   372,   373,   374,   375,   381
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
    1539,  1541,  1543,  1545,  1547,  1549,  1551,  1553,  1555,  1556,
    1562,  1568,  1574,  1580,  1586,  1590,  1591,  1599,  1602,  1607,
    1612,  1617,  1622,  1627,  1632,  1637,  1642,  1647,  1652,  1657,
    1662,  1667,  1672,  1677,  1686,  1691,  1696,  1705,  1710,  1715,
    1720,  1725,  1730,  1735,  1740,  1749,  1754,  1763,  1776,  1781,
    1786,  1791,  1796,  1801,  1806,  1811,  1816,  1821,  1830,  1843,
    1848,  1853,  1858,  1863,  1876,  1881,  1886,  1891,  1896,  1901,
    1906,  1911,  1916,  1921,  1930,  1935,  1940,  1951,  1964,  1969,
    1974,  1979,  1984,  1989,  1994,  2005,  2018,  2023,  2034,  2039,
    2044,  2049,  2054,  2059,  2064,  2065,  2071,  2077,  2083,  2089,
    2095,  2101,  2107,  2113,  2119,  2125,  2131,  2137,  2143,  2149,
    2155,  2165,  2171,  2177,  2183,  2189,  2195,  2201,  2205,  2206,
    2214,  2217,  2218,  2224,  2230,  2236,  2242,  2248,  2254,  2260,
    2266,  2272,  2278,  2284,  2290,  2295,  2304,  2318,  2324,  2330,
    2336,  2342,  2356,  2362,  2368,  2374,  2380,  2384,  2385,  2393,
    2396,  2397,  2403,  2409,  2415,  2421,  2427,  2437,  2443,  2449,
    2455,  2461,  2467,  2473,  2479,  2485,  2491,  2497,  2503,  2509,
    2515,  2521,  2527,  2531,  2532,  2540,  2544,  2553,  2558,  2563,
    2577,  2591,  2596,  2601,  2606,  2611,  2616,  2621,  2626,  2631,
    2636,  2641,  2646,  2651,  2656,  2661,  2666,  2671,  2676,  2677,
    2687,  2693,  2699,  2714,  2729,  2735,  2741,  2747,  2753,  2759,
    2765,  2771,  2777,  2783,  2789,  2795,  2801,  2807,  2813,  2819,
    2825,  2831,  2835,  2836,  2844,  2848,  2849,  2855,  2861,  2867,
    2873,  2876,  2880,  2881,  2883,  2886,  2891,  2896,  2899,  2902,
    2905,  2910,  2912,  2914,  2916,  2918,  2921,  2924,  2928,  2930,
    2932,  2935,  2937,  2939,  2941,  2943,  2945,  2947,  2949,  2952,
    2955,  2959,  2960,  2962,  2964,  2967,  2969,  2972,  2975,  2979,
    2981,  2984,  2987,  2991,  2994,  2998,  3002,  3007,  3009,  3011,
    3013,  3015,  3017,  3019,  3021,  3023,  3025,  3027,  3029,  3031,
    3033,  3035,  3037,  3039,  3041,  3043,  3045,  3047,  3049,  3051,
    3053,  3055,  3057,  3059,  3060,  3067,  3069,  3072,  3074,  3076,
    3078,  3080,  3082,  3084,  3086,  3088,  3090,  3092,  3094,  3096,
    3098,  3100,  3102,  3104,  3106,  3108,  3111,  3112,  3120,  3121,
    3124,  3128,  3129,  3134,  3141,  3149,  3155,  3161,  3167,  3173,
    3179,  3185,  3196,  3205,  3213,  3214,  3215,  3224,  3225,  3233,
    3241,  3252,  3261,  3269,  3270,  3271,  3280,  3281,  3287,  3293,
    3301,  3302,  3303,  3312,  3313,  3319,  3325,  3326,  3327
};

/* YYRHS -- A `-1'-separated list of the rules' RHS.  */
static const yytype_int16 yyrhs[] =
{
     397,     0,    -1,    -1,   397,   383,    -1,   397,   398,    -1,
     397,   426,    -1,   397,   433,    -1,   397,   437,    -1,   397,
     436,    -1,   397,   443,    -1,   397,   442,    -1,   397,   446,
      -1,   397,   449,    -1,   397,   457,    -1,   397,   463,    -1,
     397,   473,    -1,   397,   477,    -1,   397,   470,    -1,   397,
     467,    -1,   397,   500,    -1,   397,   503,    -1,   397,   497,
      -1,   397,   434,    -1,   397,   435,    -1,   397,   506,    -1,
     397,   510,    -1,   397,   514,    -1,   397,   518,    -1,   397,
       1,    -1,    61,    62,   384,   402,    -1,     9,    -1,    22,
      -1,     6,    -1,   400,   378,   400,    -1,   401,   378,   400,
      -1,   400,   378,   401,    -1,   400,   377,   400,    -1,   401,
     377,   400,    -1,   400,   377,   401,    -1,   400,   379,   400,
      -1,   401,   379,   400,    -1,   400,   379,   401,    -1,   400,
     380,   400,    -1,   401,   380,   400,    -1,   400,   380,   401,
      -1,   401,   380,   401,    -1,   377,   400,    -1,   400,   382,
     400,    -1,   401,   382,   400,    -1,   400,   382,   401,    -1,
     401,   382,   401,    -1,   385,   400,   386,    -1,     7,    -1,
     401,   378,   401,    -1,   401,   377,   401,    -1,   401,   379,
     401,    -1,   377,   401,    -1,   385,   401,   386,    -1,     4,
      -1,     5,    -1,   402,    -1,   401,    -1,   400,    -1,   401,
      -1,   400,    -1,   402,    -1,   400,    -1,   401,    -1,   402,
      -1,    -1,   387,    -1,     5,    -1,    22,    -1,   407,   406,
      22,    -1,    -1,   407,    -1,   410,   407,   411,    -1,    -1,
      22,    -1,   409,   406,    22,    -1,   388,    -1,   389,    -1,
     400,    -1,   401,    -1,   412,   387,   400,    -1,   412,   387,
     401,    -1,   412,     1,    -1,   410,   412,   411,    -1,   413,
     387,   410,   412,   411,    -1,   413,     1,    -1,   401,    -1,
     414,   387,   401,    -1,   410,   414,   411,    -1,   415,   387,
     410,   414,   411,    -1,   415,     1,    -1,   402,    -1,   416,
     387,   402,    -1,   401,    -1,   400,    -1,    22,    -1,     1,
      -1,    -1,    29,    -1,   417,    -1,   418,   387,   417,    -1,
     418,     1,    -1,   100,    -1,   101,    -1,   102,    -1,   103,
      -1,   104,    -1,   105,    -1,   106,    -1,   107,    -1,   108,
      -1,   109,    -1,   110,    -1,   111,    -1,   112,    -1,   113,
      -1,   114,    -1,   115,    -1,   116,    -1,   117,    -1,   118,
      -1,   119,    -1,   120,    -1,   121,    -1,   122,    -1,   123,
      -1,   124,    -1,   125,    -1,   126,    -1,   127,    -1,   128,
      -1,   129,    -1,   130,    -1,   131,    -1,   132,    -1,   133,
      -1,   134,    -1,   135,    -1,   136,    -1,   137,    -1,   138,
      -1,   139,    -1,   140,    -1,   141,    -1,   142,    -1,   143,
      -1,   144,    -1,   145,    -1,   146,    -1,   147,    -1,   148,
      -1,   149,    -1,   150,    -1,   151,    -1,   152,    -1,   153,
      -1,   154,    -1,   155,    -1,   156,    -1,   157,    -1,   158,
      -1,   159,    -1,   160,    -1,   161,    -1,   162,    -1,   163,
      -1,   164,    -1,   165,    -1,   166,    -1,   167,    -1,   168,
      -1,   370,    -1,   371,    -1,   372,    -1,   373,    -1,   374,
      -1,   375,    -1,   344,    -1,   345,    -1,   346,    -1,   347,
      -1,   348,    -1,   349,    -1,   350,    -1,   351,    -1,   352,
      -1,   353,    -1,   354,    -1,   355,    -1,   356,    -1,   357,
      -1,   358,    -1,   359,    -1,   360,    -1,   361,    -1,   362,
      -1,   363,    -1,   364,    -1,   365,    -1,   366,    -1,   367,
      -1,   368,    -1,   369,    -1,   267,    -1,   420,   390,   267,
      -1,    -1,   421,   465,    -1,   421,   475,    -1,   421,   296,
     376,    22,   383,    -1,   421,   297,   376,   401,   383,    -1,
     421,    65,   376,   408,   383,    -1,   421,    65,   391,   399,
     392,   376,   408,   383,    -1,   421,    66,   376,   408,   383,
      -1,   421,    66,   391,   399,   392,   376,   408,   383,    -1,
     421,   314,   376,   408,   383,    -1,   421,   314,   391,   399,
     392,   376,   408,   383,    -1,   421,   319,   376,   408,   383,
      -1,   421,   319,   391,   399,   392,   376,   408,   383,    -1,
     421,    67,   376,   408,   383,    -1,   421,    67,   391,   399,
     392,   376,   408,   383,    -1,   421,    68,   376,   408,   383,
      -1,   421,    68,   391,   399,   392,   376,   408,   383,    -1,
     421,    69,   376,   408,   383,    -1,   421,    69,   391,   399,
     392,   376,   408,   383,    -1,   421,    85,   376,   408,   383,
      -1,   421,    85,   391,   399,   392,   376,   408,   383,    -1,
     421,    86,   376,   408,   383,    -1,   421,    86,   391,   399,
     392,   376,   408,   383,    -1,   421,   315,   376,   408,   383,
      -1,   421,   315,   391,   399,   392,   376,   408,   383,    -1,
     421,   318,   376,   408,   383,    -1,   421,   318,   391,   399,
     392,   376,   408,   383,    -1,   421,    87,   376,   408,   383,
      -1,   421,    87,   391,   399,   392,   376,   408,   383,    -1,
     421,    88,   376,   408,   383,    -1,   421,    88,   391,   399,
     392,   376,   408,   383,    -1,   421,    89,   376,   408,   383,
      -1,   421,    89,   391,   399,   392,   376,   408,   383,    -1,
     421,    34,   385,    22,   386,   376,   410,   409,   411,   383,
      -1,   421,    34,   385,    22,   386,   376,    22,   383,    -1,
     421,    34,   385,    22,   386,   490,   383,    -1,   421,    34,
     385,    22,   393,   401,   394,   386,   376,    22,   383,    -1,
     421,    34,   385,    22,   393,   401,   394,   386,   376,   410,
     413,   411,   383,    -1,   421,    34,   385,    22,   393,   401,
     394,   386,   376,   415,   383,    -1,   421,    34,   385,    22,
     393,   401,   394,   386,   376,   410,   416,   411,   383,    -1,
     421,    70,   376,     9,   383,    -1,   421,    23,   385,    22,
     386,   376,    22,   383,    -1,   421,    96,   385,    22,   386,
     376,    22,   383,    -1,   421,    71,   385,    22,   386,   376,
      22,   383,    -1,   421,    97,   385,    22,   386,   376,    22,
     383,    -1,   421,    34,   385,    22,   395,    71,   386,   376,
      22,   383,    -1,   421,    71,   385,    22,   386,   376,   388,
      22,   387,   401,   389,   383,    -1,   421,    97,   385,    22,
     386,   376,   388,    22,   387,   401,   389,   383,    -1,   421,
      77,   385,    22,   386,   376,    22,   383,    -1,   421,    93,
     385,    22,   386,   376,    22,   383,    -1,   421,    34,   385,
      22,   395,    77,   386,   376,    22,   383,    -1,   421,    77,
     385,    22,   386,   376,   388,    22,   387,   401,   389,   383,
      -1,   421,    72,   385,    22,   386,   376,   401,   383,    -1,
     421,    98,   385,    22,   386,   376,   401,   383,    -1,   421,
      78,   385,    22,   386,   376,   401,   383,    -1,   421,    95,
     385,    22,   386,   376,   401,   383,    -1,   421,    34,   385,
      22,   395,    72,   386,   376,   401,   383,    -1,   421,    34,
     385,    22,   395,    78,   386,   376,   401,   383,    -1,   421,
      73,   385,    22,   386,   376,    22,   383,    -1,   421,    99,
     385,    22,   386,   376,    22,   383,    -1,   421,    34,   385,
      22,   395,    73,   386,   376,    22,   383,    -1,   421,    74,
     376,    22,   383,    -1,   421,    74,   385,    22,   386,   383,
      -1,   421,    75,   376,    22,   383,    -1,   421,    75,   385,
      22,   386,   383,    -1,   421,    76,   376,    22,   383,    -1,
     421,    76,   385,    22,   386,   383,    -1,   421,   263,   376,
     264,   383,    -1,   421,   262,   376,   404,   383,    -1,   421,
     265,   376,    22,   383,    -1,   421,   266,   376,   420,   383,
      -1,   421,   268,   376,    22,   383,    -1,   421,   269,   393,
     404,   394,   376,   413,   383,    -1,   421,   269,   376,   410,
     412,   411,   383,    -1,   421,   270,   376,   410,   414,   411,
     383,    -1,   421,   284,   385,    22,   387,    22,   386,   383,
      -1,   421,   285,   376,   404,   383,    -1,   421,   286,   376,
     403,   383,    -1,   421,   313,   376,   410,   414,   411,   383,
      -1,   421,   292,   376,   408,   383,    -1,   421,   290,   376,
     388,    22,   387,   418,   389,   383,    -1,   421,   293,   376,
     401,   383,    -1,   421,   294,   376,   415,   383,    -1,   421,
     295,   376,   415,   383,    -1,   421,   261,   385,   399,   386,
     376,   404,   383,    -1,   421,     1,   383,    -1,    -1,    20,
     423,   481,   388,   421,   389,    -1,    -1,    20,    22,   424,
     481,   388,   421,   389,    -1,   261,   385,    22,   386,   376,
     404,   383,    -1,   261,   385,     9,   386,   376,   404,   383,
      -1,    19,    22,   383,    -1,    19,    22,   287,   383,    -1,
     422,    -1,   425,   422,    -1,   425,     1,    -1,    -1,    19,
     427,   481,   388,   425,   389,    -1,    -1,    19,    22,   428,
     481,   388,   425,   389,    -1,   289,    -1,   288,    -1,    -1,
     289,    -1,   288,    -1,    22,    -1,    -1,   431,   481,    -1,
     429,   431,   481,    -1,    10,   432,   388,    21,    -1,   316,
     432,   388,    21,    -1,   317,   432,   388,    21,    -1,    12,
     432,   388,    21,    -1,    11,   432,   388,    21,    -1,    -1,
      30,    -1,    -1,   379,    -1,    36,   439,    22,    -1,   438,
      47,   439,    22,    -1,    -1,    29,    -1,   440,    -1,   441,
     387,   440,    -1,   290,   432,   385,   441,   386,   388,    21,
      -1,   291,   432,   388,    21,    -1,    -1,   444,    73,   376,
      22,   383,    -1,   444,     1,    -1,    23,    -1,    24,    -1,
      25,    -1,    26,    -1,    27,    -1,    28,    -1,   445,    22,
     383,    -1,    -1,   445,   431,   447,   481,   388,   444,   389,
      -1,    -1,   448,    73,   376,    22,   383,    -1,   448,    22,
     376,     9,   383,    -1,   448,   277,   376,   283,   383,    -1,
     448,   277,   376,   410,   414,   411,   383,    -1,   448,   277,
     376,   404,   383,    -1,   448,   278,   376,   283,   383,    -1,
     448,   278,   376,   410,   414,   411,   383,    -1,   448,   279,
     376,   405,   383,    -1,   448,   280,   376,    22,   383,    -1,
     448,   281,   376,   282,   383,    -1,    -1,    -1,   273,   431,
     430,   450,   481,   451,   388,   448,   389,    -1,    -1,    -1,
     274,   431,   430,   452,   481,   453,   388,   448,   389,    -1,
      -1,    -1,   272,   431,   430,   454,   481,   455,   388,   448,
     389,    -1,    -1,   456,    22,   376,     9,   383,    -1,   456,
     275,   376,    22,   383,    -1,   456,   276,   376,   410,   409,
     411,   383,    -1,   456,   276,   376,    22,   383,    -1,    -1,
      -1,   271,   431,   430,   458,   481,   459,   388,   456,   389,
      -1,   174,    -1,   175,    -1,   176,    -1,   177,    -1,   178,
      -1,   179,    -1,     1,    -1,   182,    -1,   181,    -1,   183,
      -1,   180,    -1,     1,    -1,    -1,   462,   170,   376,   460,
     383,    -1,   462,   169,   376,   460,   383,    -1,   462,   171,
     376,   461,   383,    -1,   462,   172,   376,   461,   383,    -1,
     462,   173,   376,   461,   383,    -1,   462,     1,   383,    -1,
      -1,    14,   431,   464,   481,   388,   462,   389,    -1,   463,
       1,    -1,   184,   376,   405,   383,    -1,   185,   376,   403,
     383,    -1,   186,   376,   403,   383,    -1,   187,   376,   403,
     383,    -1,   188,   376,   419,   383,    -1,   189,   376,   405,
     383,    -1,   190,   376,   403,   383,    -1,   192,   376,   404,
     383,    -1,   193,   376,   404,   383,    -1,   194,   376,   403,
     383,    -1,   195,   376,   403,   383,    -1,   195,   376,   419,
     383,    -1,   196,   376,   419,   383,    -1,   197,   376,   419,
     383,    -1,   198,   376,   403,   383,    -1,   199,   376,   410,
     419,   406,   419,   411,   383,    -1,   201,   376,   405,   383,
      -1,   202,   376,   405,   383,    -1,   200,   376,   410,   405,
     387,   405,   411,   383,    -1,   203,   376,   403,   383,    -1,
     204,   376,   403,   383,    -1,   205,   376,   403,   383,    -1,
     207,   376,   403,   383,    -1,   208,   376,   403,   383,    -1,
     210,   376,   419,   383,    -1,   211,   376,   405,   383,    -1,
     209,   376,   410,   419,   387,   405,   411,   383,    -1,   214,
     376,   403,   383,    -1,   212,   376,   388,   419,   387,   419,
     389,   383,    -1,   213,   376,   388,   419,   387,   419,   387,
     419,   387,   419,   389,   383,    -1,   215,   376,   419,   383,
      -1,   216,   376,   419,   383,    -1,   217,   376,   419,   383,
      -1,   218,   376,   419,   383,    -1,   219,   376,   419,   383,
      -1,   220,   376,   419,   383,    -1,   223,   376,   419,   383,
      -1,   224,   376,   419,   383,    -1,   222,   376,   419,   383,
      -1,   221,   376,   410,   419,   387,   419,   411,   383,    -1,
     225,   376,   410,   405,   406,   405,   406,   405,   406,   405,
     411,   383,    -1,   226,   376,   403,   383,    -1,   227,   376,
     403,   383,    -1,   228,   376,   403,   383,    -1,   229,   376,
     419,   383,    -1,   230,   376,   410,   403,   406,   403,   406,
     403,   406,   403,   411,   383,    -1,   231,   376,   403,   383,
      -1,   232,   376,   403,   383,    -1,   234,   376,   403,   383,
      -1,   235,   376,   403,   383,    -1,   236,   376,   403,   383,
      -1,   237,   376,   419,   383,    -1,   238,   376,   403,   383,
      -1,   239,   376,   403,   383,    -1,   240,   376,   403,   383,
      -1,   241,   376,   410,   405,   406,   405,   411,   383,    -1,
     242,   376,   403,   383,    -1,   243,   376,   419,   383,    -1,
     243,   376,   388,   419,   387,   404,   387,   404,   389,   383,
      -1,   244,   376,   388,   419,   387,   419,   387,   404,   387,
     404,   389,   383,    -1,   245,   376,   404,   383,    -1,   246,
     376,   404,   383,    -1,   253,   376,   404,   383,    -1,   247,
     376,   419,   383,    -1,   248,   376,   419,   383,    -1,   249,
     376,   419,   383,    -1,   250,   376,   388,   419,   387,   419,
     387,   419,   389,   383,    -1,   251,   376,   388,   419,   387,
     419,   387,   419,   387,   419,   389,   383,    -1,   254,   376,
     419,   383,    -1,   254,   376,   388,   419,   387,   404,   387,
     404,   389,   383,    -1,   255,   376,   404,   383,    -1,   256,
     376,   404,   383,    -1,   257,   376,   419,   383,    -1,   258,
     376,   419,   383,    -1,   259,   376,   419,   383,    -1,   260,
     376,   404,   383,    -1,    -1,   466,   184,   376,   405,   383,
      -1,   466,   185,   376,   403,   383,    -1,   466,   186,   376,
     403,   383,    -1,   466,   187,   376,   403,   383,    -1,   466,
     188,   376,   419,   383,    -1,   466,   189,   376,   405,   383,
      -1,   466,   190,   376,   403,   383,    -1,   466,   192,   376,
     404,   383,    -1,   466,   193,   376,   404,   383,    -1,   466,
     194,   376,   403,   383,    -1,   466,   195,   376,   403,   383,
      -1,   466,   196,   376,   419,   383,    -1,   466,   197,   376,
     419,   383,    -1,   466,   198,   376,   403,   383,    -1,   466,
     199,   376,   419,   383,    -1,   466,   199,   376,   410,   419,
     406,   419,   411,   383,    -1,   466,   201,   376,   405,   383,
      -1,   466,   202,   376,   405,   383,    -1,   466,   203,   376,
     403,   383,    -1,   466,   204,   376,   403,   383,    -1,   466,
     205,   376,   403,   383,    -1,   466,   207,   376,   403,   383,
      -1,   466,     1,   383,    -1,    -1,    18,   431,   468,   481,
     388,   466,   389,    -1,   467,     1,    -1,    -1,   469,   208,
     376,   403,   383,    -1,   469,   210,   376,   419,   383,    -1,
     469,   211,   376,   405,   383,    -1,   469,   214,   376,   403,
     383,    -1,   469,   215,   376,   419,   383,    -1,   469,   216,
     376,   419,   383,    -1,   469,   217,   376,   419,   383,    -1,
     469,   218,   376,   419,   383,    -1,   469,   219,   376,   419,
     383,    -1,   469,   220,   376,   419,   383,    -1,   469,   223,
     376,   419,   383,    -1,   469,   224,   376,   419,   383,    -1,
     222,   376,   419,   383,    -1,   221,   376,   410,   419,   387,
     419,   411,   383,    -1,   469,   225,   376,   410,   405,   406,
     405,   406,   405,   406,   405,   411,   383,    -1,   469,   226,
     376,   403,   383,    -1,   469,   227,   376,   403,   383,    -1,
     469,   228,   376,   403,   383,    -1,   469,   229,   376,   419,
     383,    -1,   469,   230,   376,   410,   403,   406,   403,   406,
     403,   406,   403,   411,   383,    -1,   469,   231,   376,   403,
     383,    -1,   469,   232,   376,   403,   383,    -1,   469,   234,
     376,   403,   383,    -1,   469,   235,   376,   403,   383,    -1,
     469,     1,   383,    -1,    -1,    17,   431,   471,   481,   388,
     469,   389,    -1,   470,     1,    -1,    -1,   472,   236,   376,
     403,   383,    -1,   472,   237,   376,   419,   383,    -1,   472,
     238,   376,   403,   383,    -1,   472,   239,   376,   403,   383,
      -1,   472,   240,   376,   403,   383,    -1,   472,   241,   376,
     410,   405,   406,   405,   411,   383,    -1,   472,   242,   376,
     403,   383,    -1,   472,   243,   376,   419,   383,    -1,   472,
     245,   376,   404,   383,    -1,   472,   246,   376,   404,   383,
      -1,   472,   247,   376,   419,   383,    -1,   472,   248,   376,
     419,   383,    -1,   472,   249,   376,   419,   383,    -1,   472,
     253,   376,   404,   383,    -1,   472,   254,   376,   419,   383,
      -1,   472,   255,   376,   404,   383,    -1,   472,   256,   376,
     404,   383,    -1,   472,   257,   376,   419,   383,    -1,   472,
     258,   376,   419,   383,    -1,   472,   259,   376,   419,   383,
      -1,   472,   260,   376,   404,   383,    -1,   472,     1,   383,
      -1,    -1,    15,   431,   474,   481,   388,   472,   389,    -1,
     473,     1,   383,    -1,   320,   376,   388,   405,   387,   405,
     389,   383,    -1,   321,   376,   419,   383,    -1,   322,   376,
     419,   383,    -1,   323,   393,   404,   394,   376,   388,   419,
     387,   404,   387,   405,   389,   383,    -1,   324,   393,   404,
     394,   376,   388,   419,   387,   419,   387,   405,   389,   383,
      -1,   325,   376,   419,   383,    -1,   326,   376,   405,   383,
      -1,   327,   376,   419,   383,    -1,   328,   376,   419,   383,
      -1,   329,   376,   419,   383,    -1,   330,   376,   419,   383,
      -1,   331,   376,   419,   383,    -1,   332,   376,   405,   383,
      -1,   333,   376,   405,   383,    -1,   334,   376,   419,   383,
      -1,   335,   376,   405,   383,    -1,   336,   376,   419,   383,
      -1,   337,   376,   404,   383,    -1,   338,   376,   419,   383,
      -1,   341,   376,   419,   383,    -1,   339,   376,   419,   383,
      -1,   340,   376,   419,   383,    -1,    -1,   476,   320,   376,
     388,   405,   387,   405,   389,   383,    -1,   476,   321,   376,
     419,   383,    -1,   476,   322,   376,   419,   383,    -1,   476,
     323,   393,   404,   394,   376,   388,   419,   387,   404,   387,
     405,   389,   383,    -1,   476,   324,   393,   404,   394,   376,
     388,   419,   387,   419,   387,   405,   389,   383,    -1,   476,
     325,   376,   419,   383,    -1,   476,   326,   376,   405,   383,
      -1,   476,   327,   376,   419,   383,    -1,   476,   328,   376,
     419,   383,    -1,   476,   329,   376,   419,   383,    -1,   476,
     330,   376,   419,   383,    -1,   476,   331,   376,   419,   383,
      -1,   476,   332,   376,   405,   383,    -1,   476,   333,   376,
     405,   383,    -1,   476,   334,   376,   419,   383,    -1,   476,
     335,   376,   405,   383,    -1,   476,   336,   376,   419,   383,
      -1,   476,   337,   376,   404,   383,    -1,   476,   338,   376,
     419,   383,    -1,   476,   341,   376,   419,   383,    -1,   476,
     339,   376,   419,   383,    -1,   476,   340,   376,   419,   383,
      -1,   476,     1,   383,    -1,    -1,    16,   431,   478,   481,
     388,   476,   389,    -1,   477,     1,   383,    -1,    -1,   479,
      22,   376,     9,   383,    -1,   479,    22,   376,   405,   383,
      -1,   479,    22,   376,   401,   383,    -1,   479,    22,   376,
     402,   383,    -1,   479,     1,    -1,   391,   479,   392,    -1,
      -1,   480,    -1,   376,   400,    -1,   376,   410,   412,   411,
      -1,   376,   410,   413,   411,    -1,   376,   401,    -1,   376,
       1,    -1,   376,   402,    -1,   376,   410,   416,   411,    -1,
      31,    -1,    32,    -1,    33,    -1,    34,    -1,    34,   429,
      -1,    34,   488,    -1,    34,   429,   488,    -1,   429,    -1,
     488,    -1,   429,   488,    -1,   482,    -1,   483,    -1,   484,
      -1,   485,    -1,   486,    -1,   487,    -1,     1,    -1,   384,
      22,    -1,   393,   394,    -1,   393,   401,   394,    -1,    -1,
     480,    -1,   490,    -1,   480,   490,    -1,   491,    -1,   491,
     480,    -1,   491,   490,    -1,   491,   480,   490,    -1,   492,
      -1,   492,   480,    -1,   492,   490,    -1,   492,   480,   490,
      -1,   492,   491,    -1,   492,   491,   480,    -1,   492,   491,
     490,    -1,   492,   491,   480,   490,    -1,    36,    -1,    37,
      -1,    38,    -1,    39,    -1,    47,    -1,    48,    -1,    49,
      -1,    50,    -1,    43,    -1,    44,    -1,    45,    -1,    46,
      -1,    40,    -1,    41,    -1,    42,    -1,    51,    -1,    52,
      -1,    53,    -1,    54,    -1,    55,    -1,    56,    -1,    57,
      -1,    58,    -1,    59,    -1,    60,    -1,     1,    -1,    -1,
     489,   494,    22,   496,   493,   383,    -1,   495,    -1,   497,
     495,    -1,   433,    -1,   497,    -1,   437,    -1,   436,    -1,
     443,    -1,   442,    -1,   446,    -1,   449,    -1,   457,    -1,
     463,    -1,   473,    -1,   477,    -1,   470,    -1,   467,    -1,
     503,    -1,   434,    -1,   435,    -1,   498,    -1,   499,   498,
      -1,    -1,     3,    22,   501,   481,   388,   499,   389,    -1,
      -1,   388,   389,    -1,   388,   497,   389,    -1,    -1,    35,
     432,   504,   502,    -1,   298,   376,   410,   409,   411,   383,
      -1,   505,   299,   376,   410,   409,   411,   383,    -1,   505,
     300,   376,    22,   383,    -1,   505,   301,   376,    22,   383,
      -1,   505,   302,   376,    22,   383,    -1,   505,   303,   376,
      22,   383,    -1,   505,   304,   376,   401,   383,    -1,   505,
     305,   376,    22,   383,    -1,   505,    34,   385,    22,   386,
     376,   410,   409,   411,   383,    -1,   505,    34,   385,    22,
     386,   376,    22,   383,    -1,   505,    34,   385,    22,   386,
     490,   383,    -1,    -1,    -1,   296,   432,   507,   481,   508,
     388,   505,   389,    -1,    -1,   509,   308,   376,   410,   409,
     411,   383,    -1,   509,   307,   376,   410,   409,   411,   383,
      -1,   509,    34,   385,    22,   386,   376,   410,   409,   411,
     383,    -1,   509,    34,   385,    22,   386,   376,    22,   383,
      -1,   509,    34,   385,    22,   386,   490,   383,    -1,    -1,
      -1,   306,   432,   511,   481,   512,   388,   509,   389,    -1,
      -1,   513,   281,   376,   282,   383,    -1,   513,   278,   376,
     283,   383,    -1,   513,   278,   376,   410,   414,   411,   383,
      -1,    -1,    -1,   309,    22,   515,   481,   516,   388,   513,
     389,    -1,    -1,   517,   311,   376,     9,   383,    -1,   517,
     312,   376,     9,   383,    -1,    -1,    -1,   310,   432,   519,
     481,   520,   388,   517,   389,    -1
};

/* YYRLINE[YYN] -- source line where rule number YYN was defined.  */
static const yytype_uint16 yyrline[] =
{
       0,  1011,  1011,  1012,  1013,  1014,  1016,  1018,  1020,  1022,
    1023,  1024,  1025,  1026,  1027,  1028,  1029,  1030,  1031,  1032,
    1033,  1034,  1035,  1037,  1039,  1040,  1041,  1042,  1043,  1046,
    1055,  1056,  1062,  1063,  1064,  1065,  1066,  1067,  1068,  1069,
    1070,  1071,  1072,  1073,  1074,  1075,  1076,  1077,  1078,  1079,
    1080,  1081,  1089,  1090,  1091,  1092,  1093,  1094,  1099,  1100,
    1106,  1107,  1110,  1114,  1115,  1119,  1122,  1123,  1124,  1142,
    1144,  1149,  1150,  1165,  1182,  1198,  1199,  1206,  1207,  1212,
    1224,  1229,  1237,  1241,  1245,  1249,  1253,  1259,  1263,  1267,
    1270,  1279,  1290,  1294,  1298,  1301,  1305,  1325,  1329,  1333,
    1362,  1367,  1368,  1369,  1373,  1377,  1394,  1395,  1396,  1397,
    1398,  1399,  1400,  1401,  1402,  1403,  1404,  1405,  1406,  1407,
    1408,  1409,  1410,  1411,  1412,  1413,  1414,  1415,  1416,  1417,
    1418,  1419,  1420,  1421,  1422,  1423,  1424,  1425,  1426,  1427,
    1428,  1429,  1430,  1431,  1432,  1433,  1434,  1435,  1436,  1437,
    1438,  1439,  1440,  1441,  1442,  1443,  1444,  1445,  1446,  1447,
    1448,  1449,  1450,  1451,  1452,  1453,  1454,  1455,  1456,  1457,
    1458,  1459,  1460,  1461,  1462,  1464,  1465,  1466,  1467,  1468,
    1469,  1470,  1471,  1472,  1473,  1474,  1475,  1476,  1477,  1478,
    1479,  1480,  1481,  1482,  1483,  1484,  1485,  1486,  1487,  1488,
    1489,  1490,  1491,  1492,  1493,  1494,  1495,  1499,  1500,  1507,
    1508,  1509,  1510,  1515,  1519,  1524,  1530,  1535,  1541,  1546,
    1552,  1557,  1563,  1568,  1574,  1579,  1585,  1590,  1596,  1601,
    1607,  1612,  1618,  1623,  1629,  1634,  1640,  1645,  1651,  1656,
    1662,  1667,  1673,  1688,  1697,  1754,  1773,  1801,  1810,  1815,
    1819,  1836,  1853,  1870,  1887,  1904,  1921,  1938,  1955,  1972,
    1989,  2006,  2019,  2032,  2045,  2058,  2071,  2084,  2105,  2126,
    2147,  2157,  2167,  2177,  2187,  2197,  2210,  2213,  2216,  2230,
    2233,  2247,  2257,  2261,  2268,  2289,  2292,  2298,  2313,  2323,
    2369,  2377,  2394,  2414,  2418,  2427,  2427,  2441,  2441,  2459,
    2463,  2467,  2478,  2496,  2497,  2498,  2504,  2504,  2512,  2512,
    2527,  2528,  2533,  2534,  2535,  2543,  2552,  2553,  2554,  2567,
    2591,  2612,  2633,  2671,  2709,  2710,  2712,  2713,  2718,  2723,
    2732,  2733,  2734,  2738,  2746,  2806,  2847,  2848,  2876,  2883,
    2884,  2885,  2886,  2887,  2888,  2890,  2896,  2896,  2912,  2913,
    2923,  2929,  2933,  2938,  2942,  2946,  2952,  2956,  2969,  2981,
    2988,  2981,  2996,  3003,  2996,  3011,  3018,  3011,  3030,  3031,
    3040,  3054,  3075,  3092,  3099,  3092,  3112,  3113,  3114,  3115,
    3116,  3117,  3118,  3121,  3122,  3123,  3124,  3125,  3132,  3133,
    3136,  3139,  3142,  3145,  3148,  3153,  3153,  3163,  3178,  3179,
    3180,  3181,  3182,  3183,  3184,  3186,  3187,  3188,  3189,  3190,
    3191,  3192,  3193,  3194,  3195,  3196,  3197,  3202,  3203,  3204,
    3206,  3207,  3208,  3209,  3210,  3215,  3216,  3221,  3228,  3229,
    3230,  3231,  3232,  3233,  3235,  3236,  3237,  3242,  3251,  3253,
    3254,  3255,  3256,  3257,  3264,  3265,  3267,  3268,  3269,  3270,
    3271,  3272,  3273,  3274,  3276,  3277,  3278,  3284,  3290,  3291,
    3292,  3293,  3294,  3295,  3296,  3303,  3316,  3317,  3323,  3324,
    3325,  3326,  3327,  3328,  3334,  3335,  3336,  3337,  3338,  3339,
    3340,  3341,  3343,  3344,  3345,  3346,  3347,  3348,  3349,  3350,
    3351,  3352,  3353,  3354,  3355,  3356,  3358,  3359,  3364,  3364,
    3374,  3379,  3380,  3381,  3382,  3383,  3384,  3385,  3386,  3387,
    3388,  3389,  3391,  3392,  3393,  3398,  3407,  3409,  3410,  3411,
    3412,  3413,  3420,  3421,  3423,  3424,  3425,  3430,  3430,  3440,
    3445,  3446,  3447,  3448,  3449,  3450,  3451,  3453,  3454,  3455,
    3456,  3457,  3458,  3459,  3460,  3468,  3469,  3470,  3471,  3472,
    3473,  3474,  3475,  3480,  3480,  3490,  3493,  3494,  3495,  3496,
    3497,  3498,  3499,  3500,  3501,  3502,  3503,  3504,  3505,  3506,
    3507,  3508,  3509,  3510,  3511,  3512,  3513,  3514,  3522,  3523,
    3524,  3529,  3534,  3539,  3544,  3549,  3554,  3559,  3564,  3569,
    3574,  3579,  3584,  3589,  3594,  3599,  3604,  3609,  3614,  3619,
    3624,  3635,  3641,  3641,  3657,  3667,  3668,  3675,  3681,  3687,
    3699,  3702,  3704,  3705,  3710,  3720,  3727,  3733,  3740,  3752,
    3762,  3768,  3769,  3770,  3777,  3782,  3787,  3792,  3797,  3806,
    3811,  3822,  3825,  3828,  3831,  3837,  3840,  3843,  3849,  3856,
    3859,  3870,  3874,  3878,  3883,  3888,  3893,  3898,  3904,  3910,
    3915,  3920,  3926,  3932,  3938,  3944,  3951,  3963,  3964,  3965,
    3966,  3967,  3968,  3969,  3970,  3971,  3972,  3973,  3974,  3975,
    3976,  3977,  3978,  3979,  3980,  3981,  3982,  3983,  3984,  3985,
    3986,  3987,  3988,  3994,  3994,  4078,  4079,  4083,  4084,  4085,
    4086,  4087,  4088,  4089,  4090,  4091,  4092,  4093,  4094,  4095,
    4096,  4097,  4098,  4099,  4103,  4104,  4117,  4117,  4136,  4137,
    4138,  4143,  4143,  4185,  4188,  4191,  4194,  4197,  4200,  4203,
    4205,  4208,  4215,  4220,  4236,  4237,  4236,  4246,  4247,  4250,
    4253,  4260,  4265,  4281,  4282,  4281,  4291,  4292,  4300,  4304,
    4312,  4314,  4312,  4323,  4324,  4327,  4332,  4333,  4332
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
  "TEXTURE2D_TYPE", "TEXTURE2DRECT_TYPE", "TEXTURE3D_TYPE",
  "TEXTURECUBE_TYPE", "SAMPLER1D_TYPE", "SAMPLER2D_TYPE",
  "SAMPLER2DRECT_TYPE", "SAMPLER3D_TYPE", "SAMPLERCUBE_TYPE", "EXTENSION",
  "SEPARATE_SHADER", "DEPTHMASK", "DEPTHFUNC", "VERTEXPROGRAM",
  "FRAGMENTPROGRAM", "GEOMETRYPROGRAM", "HULLPROGRAM", "EVALPROGRAM",
  "SHDPROFILE", "SAMPLERRESOURCE", "SAMPLERTEXUNIT", "SETSAMPLERSTATE",
  "SETDSTSTATE", "SETRASTERIZATIONSTATE", "SETCOLORSAMPLESTATE",
  "IMAGERESOURCE", "IMAGEUNIT", "IMAGEACCESS", "IMAGELAYER",
  "IMAGELAYERED", "WRITE_ONLY", "READ_ONLY", "READ_WRITE",
  "VERTEXPROGRAMOVERRIDE", "FRAGMENTPROGRAMOVERRIDE",
  "GEOMETRYPROGRAMOVERRIDE", "HULLPROGRAMOVERRIDE", "EVALPROGRAMOVERRIDE",
  "IMAGEACCESSOVERRIDE", "IMAGELAYEROVERRIDE", "IMAGELAYEREDOVERRIDE",
  "IMAGERESOURCEOVERRIDE", "UNIFORMOVERRIDE", "IMAGEUNITOVERRIDE",
  "TEXTURERESOURCEOVERRIDE", "SAMPLERRESOURCEOVERRIDE",
  "SAMPLERTEXUNITOVERRIDE", "SETSAMPLERSTATEOVERRIDE", "GLACCUM", "GLLOAD",
  "GLRETURN", "GLMULT", "GLADD", "GLNEVER", "GLLESS", "GLEQUAL",
  "GLGREATER", "GLNOTEQUAL", "GLALWAYS", "GLLEQUAL", "GLGEQUAL", "GLKEEP",
  "GLREPLACE", "GLINCR", "GLDECR", "GLUPPERLEFT", "GLLOWERLEFT",
  "GLFRONTLEFT", "GLFRONTRIGHT", "GLBACKLEFT", "GLBACKRIGHT", "GLFRONT",
  "GLBACK", "GLLEFT", "GLRIGHT", "GLFRONTANDBACK", "GLCCW", "GLCW",
  "GLPOINT", "GLLINE", "GLFILL", "GLZERO", "GLONE", "GLSRCCOLOR",
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
  "GLPOINTSIZE", "GLPOINTSMOOTH", "GLPOINTSPRITE", "GLPROGRAMPOINTSIZE",
  "GLPOINTSPRITECOORDORIGIN", "GLLINEWIDTH", "GLLINESMOOTH",
  "GLCOMBINEDLINESTIPPLE", "GLLINESTIPPLEPATTERN", "GLLINESTIPPLEREPEAT",
  "GLLINESTIPPLE", "GLCULLFACE", "GLCULLFACEMODE", "GLFRONTFACE",
  "GLPOLYGONSMOOTH", "GLPOLYGONMODE", "GLPOLYGONOFFSET",
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
  "fbo", "$@12", "$@13", "sfilter", "suvmapping", "sstates", "sstate",
  "$@14", "passglsstates", "rstates", "rstate", "$@15", "cstates",
  "csstate", "$@16", "dststates", "dststate", "$@17", "passprstates",
  "prstates", "prstate", "$@18", "annotation", "annotations2",
  "annotations", "valueassignment_f", "valueassignment_fv",
  "valueassignment_fvl", "valueassignment_i", "valueassignment_b",
  "valueassignment_bv", "precision", "uniform_qualifiers",
  "uniformvalueassigment", "semantic", "arraySz",
  "arraysz_semantic_annotations_uniformvalueassigment", "uniform_type",
  "uniform", "$@19", "uniforms", "namespaceitem", "namespaceitems",
  "namespace", "$@20", "cstbufferuniforms", "cstbuffer", "$@21",
  "optix_context_data", "optix_context", "$@22", "$@23",
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
     315,   316,   317,   100,   101,   318,   319,   320,   321,   322,
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
     623,   624,   625,   626,   627,   628,    61,    45,    43,    42,
      47,   629,    94,    59,    58,    40,    41,    44,   123,   125,
     124,    60,    62,    91,    93,    46
};
# endif

/* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_uint16 yyr1[] =
{
       0,   396,   397,   397,   397,   397,   397,   397,   397,   397,
     397,   397,   397,   397,   397,   397,   397,   397,   397,   397,
     397,   397,   397,   397,   397,   397,   397,   397,   397,   398,
     399,   399,   400,   400,   400,   400,   400,   400,   400,   400,
     400,   400,   400,   400,   400,   400,   400,   400,   400,   400,
     400,   400,   401,   401,   401,   401,   401,   401,   402,   402,
     403,   403,   403,   404,   404,   404,   405,   405,   405,   406,
     406,   407,   407,   407,   408,   408,   408,   409,   409,   409,
     410,   411,   412,   412,   412,   412,   412,   413,   413,   413,
     414,   414,   415,   415,   415,   416,   416,   417,   417,   417,
     417,   418,   418,   418,   418,   418,   419,   419,   419,   419,
     419,   419,   419,   419,   419,   419,   419,   419,   419,   419,
     419,   419,   419,   419,   419,   419,   419,   419,   419,   419,
     419,   419,   419,   419,   419,   419,   419,   419,   419,   419,
     419,   419,   419,   419,   419,   419,   419,   419,   419,   419,
     419,   419,   419,   419,   419,   419,   419,   419,   419,   419,
     419,   419,   419,   419,   419,   419,   419,   419,   419,   419,
     419,   419,   419,   419,   419,   419,   419,   419,   419,   419,
     419,   419,   419,   419,   419,   419,   419,   419,   419,   419,
     419,   419,   419,   419,   419,   419,   419,   419,   419,   419,
     419,   419,   419,   419,   419,   419,   419,   420,   420,   421,
     421,   421,   421,   421,   421,   421,   421,   421,   421,   421,
     421,   421,   421,   421,   421,   421,   421,   421,   421,   421,
     421,   421,   421,   421,   421,   421,   421,   421,   421,   421,
     421,   421,   421,   421,   421,   421,   421,   421,   421,   421,
     421,   421,   421,   421,   421,   421,   421,   421,   421,   421,
     421,   421,   421,   421,   421,   421,   421,   421,   421,   421,
     421,   421,   421,   421,   421,   421,   421,   421,   421,   421,
     421,   421,   421,   421,   421,   421,   421,   421,   421,   421,
     421,   421,   421,   421,   421,   423,   422,   424,   422,   422,
     422,   422,   422,   425,   425,   425,   427,   426,   428,   426,
     429,   429,   430,   430,   430,   431,   432,   432,   432,   433,
     434,   435,   436,   437,   438,   438,   439,   439,   440,   440,
     441,   441,   441,   441,   442,   443,   444,   444,   444,   445,
     445,   445,   445,   445,   445,   446,   447,   446,   448,   448,
     448,   448,   448,   448,   448,   448,   448,   448,   448,   450,
     451,   449,   452,   453,   449,   454,   455,   449,   456,   456,
     456,   456,   456,   458,   459,   457,   460,   460,   460,   460,
     460,   460,   460,   461,   461,   461,   461,   461,   462,   462,
     462,   462,   462,   462,   462,   464,   463,   463,   465,   465,
     465,   465,   465,   465,   465,   465,   465,   465,   465,   465,
     465,   465,   465,   465,   465,   465,   465,   465,   465,   465,
     465,   465,   465,   465,   465,   465,   465,   465,   465,   465,
     465,   465,   465,   465,   465,   465,   465,   465,   465,   465,
     465,   465,   465,   465,   465,   465,   465,   465,   465,   465,
     465,   465,   465,   465,   465,   465,   465,   465,   465,   465,
     465,   465,   465,   465,   465,   465,   465,   465,   465,   465,
     465,   465,   465,   465,   466,   466,   466,   466,   466,   466,
     466,   466,   466,   466,   466,   466,   466,   466,   466,   466,
     466,   466,   466,   466,   466,   466,   466,   466,   468,   467,
     467,   469,   469,   469,   469,   469,   469,   469,   469,   469,
     469,   469,   469,   469,   469,   469,   469,   469,   469,   469,
     469,   469,   469,   469,   469,   469,   469,   471,   470,   470,
     472,   472,   472,   472,   472,   472,   472,   472,   472,   472,
     472,   472,   472,   472,   472,   472,   472,   472,   472,   472,
     472,   472,   472,   474,   473,   473,   475,   475,   475,   475,
     475,   475,   475,   475,   475,   475,   475,   475,   475,   475,
     475,   475,   475,   475,   475,   475,   475,   475,   476,   476,
     476,   476,   476,   476,   476,   476,   476,   476,   476,   476,
     476,   476,   476,   476,   476,   476,   476,   476,   476,   476,
     476,   476,   478,   477,   477,   479,   479,   479,   479,   479,
     479,   480,   481,   481,   482,   483,   484,   485,   485,   486,
     487,   488,   488,   488,   489,   489,   489,   489,   489,   489,
     489,   490,   490,   490,   490,   490,   490,   490,   491,   492,
     492,   493,   493,   493,   493,   493,   493,   493,   493,   493,
     493,   493,   493,   493,   493,   493,   493,   494,   494,   494,
     494,   494,   494,   494,   494,   494,   494,   494,   494,   494,
     494,   494,   494,   494,   494,   494,   494,   494,   494,   494,
     494,   494,   494,   496,   495,   497,   497,   498,   498,   498,
     498,   498,   498,   498,   498,   498,   498,   498,   498,   498,
     498,   498,   498,   498,   499,   499,   501,   500,   502,   502,
     502,   504,   503,   505,   505,   505,   505,   505,   505,   505,
     505,   505,   505,   505,   507,   508,   506,   509,   509,   509,
     509,   509,   509,   511,   512,   510,   513,   513,   513,   513,
     515,   516,   514,   517,   517,   517,   519,   520,   518
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
       1,     1,     1,     1,     1,     1,     1,     1,     0,     5,
       5,     5,     5,     5,     3,     0,     7,     2,     4,     4,
       4,     4,     4,     4,     4,     4,     4,     4,     4,     4,
       4,     4,     4,     8,     4,     4,     8,     4,     4,     4,
       4,     4,     4,     4,     8,     4,     8,    12,     4,     4,
       4,     4,     4,     4,     4,     4,     4,     8,    12,     4,
       4,     4,     4,    12,     4,     4,     4,     4,     4,     4,
       4,     4,     4,     8,     4,     4,    10,    12,     4,     4,
       4,     4,     4,     4,    10,    12,     4,    10,     4,     4,
       4,     4,     4,     4,     0,     5,     5,     5,     5,     5,
       5,     5,     5,     5,     5,     5,     5,     5,     5,     5,
       9,     5,     5,     5,     5,     5,     5,     3,     0,     7,
       2,     0,     5,     5,     5,     5,     5,     5,     5,     5,
       5,     5,     5,     5,     4,     8,    13,     5,     5,     5,
       5,    13,     5,     5,     5,     5,     3,     0,     7,     2,
       0,     5,     5,     5,     5,     5,     9,     5,     5,     5,
       5,     5,     5,     5,     5,     5,     5,     5,     5,     5,
       5,     5,     3,     0,     7,     3,     8,     4,     4,    13,
      13,     4,     4,     4,     4,     4,     4,     4,     4,     4,
       4,     4,     4,     4,     4,     4,     4,     4,     0,     9,
       5,     5,    14,    14,     5,     5,     5,     5,     5,     5,
       5,     5,     5,     5,     5,     5,     5,     5,     5,     5,
       5,     3,     0,     7,     3,     0,     5,     5,     5,     5,
       2,     3,     0,     1,     2,     4,     4,     2,     2,     2,
       4,     1,     1,     1,     1,     2,     2,     3,     1,     1,
       2,     1,     1,     1,     1,     1,     1,     1,     2,     2,
       3,     0,     1,     1,     2,     1,     2,     2,     3,     1,
       2,     2,     3,     2,     3,     3,     4,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     0,     6,     1,     2,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     2,     0,     7,     0,     2,
       3,     0,     4,     6,     7,     5,     5,     5,     5,     5,
       5,    10,     8,     7,     0,     0,     8,     0,     7,     7,
      10,     8,     7,     0,     0,     8,     0,     5,     5,     7,
       0,     0,     8,     0,     5,     5,     0,     0,     8
};

/* YYDEFACT[STATE-NAME] -- Default rule to reduce with in state
   STATE-NUM when YYTABLE doesn't specify something else to do.  Zero
   means the default is an error.  */
static const yytype_uint16 yydefact[] =
{
       2,     0,     1,    28,     0,   316,   316,   316,     0,     0,
       0,     0,     0,   306,   339,   340,   341,   342,   343,   344,
     621,   622,   623,   624,   316,     0,     0,     0,     0,     0,
     311,   310,   316,   316,   316,   316,     0,   316,   316,   316,
       3,     4,     5,   628,     6,    22,    23,     8,     7,    10,
       9,     0,    11,    12,    13,     0,     0,     0,     0,     0,
     629,     0,   685,    21,    19,    20,    24,    25,    26,    27,
     706,   315,     0,   612,     0,     0,     0,   395,   553,   602,
     527,   498,   308,   612,   625,   626,   711,     0,   312,   312,
     312,   312,     0,     0,   724,   733,   740,   746,     0,     0,
     630,   315,   346,   397,   500,   529,     0,     0,   682,   657,
     658,   659,   660,   669,   670,   671,   665,   666,   667,   668,
     661,   662,   663,   664,   672,   673,   674,   675,   676,   677,
     678,   679,   680,   681,     0,   686,   612,   612,   605,   613,
     317,     0,     0,     0,   612,   612,   612,   612,   612,   612,
       0,   627,   708,     0,   314,   313,   373,   365,   359,   362,
     330,     0,   612,   612,   612,   612,     0,     0,   345,   612,
     555,   604,   683,     0,   318,     0,   319,   323,   322,     0,
       0,     0,     0,     0,     0,     0,     0,   712,    58,    59,
      29,   612,   612,   612,   612,   331,   325,   326,     0,   332,
       0,   335,   725,   734,   741,   747,   320,   321,     0,     0,
       0,   610,     0,   611,   388,   530,   578,   501,   474,     0,
       0,   295,     0,   303,     0,   709,     0,   374,   366,   360,
     363,   327,     0,   326,     0,   324,     0,     0,     0,     0,
     336,   637,     0,     0,     0,     0,   631,   632,   633,   634,
     635,   636,   643,     0,     0,     0,   687,   702,   703,   690,
     689,   692,   691,   693,   694,   695,     0,     0,     0,     0,
       0,   688,   704,     0,   701,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   297,   612,     0,   305,   307,
     304,   710,     0,     0,     0,     0,   328,     0,     0,   333,
       0,   727,   736,   743,     0,   618,    32,    52,     0,     0,
      80,   614,   617,   619,     0,   638,     0,     0,   639,     0,
     644,     0,   647,     0,   651,     0,   684,   707,   705,     0,
      66,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     396,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   554,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   603,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   528,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   499,   309,     0,   301,
     612,     0,     0,     0,   368,   348,   348,   348,   329,   334,
       0,     0,     0,     0,     0,   338,     0,   347,    46,    56,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    82,    83,    95,     0,     0,     0,     0,    56,
       0,     0,     0,     0,   640,   648,   652,     0,   655,   606,
     608,   609,   607,   394,     0,     0,     0,     0,     0,   552,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   601,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   106,   107,   108,   109,   110,
     111,   112,   113,   114,   115,   116,   117,   118,   119,   120,
     121,   122,   123,   124,   125,   126,   127,   128,   129,   130,
     131,   132,   133,   134,   135,   136,   137,   138,   139,   140,
     141,   142,   143,   144,   145,   146,   147,   148,   149,   150,
     151,   152,   153,   154,   155,   156,   157,   158,   159,   160,
     161,   162,   163,   164,   165,   166,   167,   168,   169,   170,
     171,   172,   173,   174,   181,   182,   183,   184,   185,   186,
     187,   188,   189,   190,   191,   192,   193,   194,   195,   196,
     197,   198,   199,   200,   201,   202,   203,   204,   205,   206,
     175,   176,   177,   178,   179,   180,     0,   526,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     497,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   302,     0,   209,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     726,     0,     0,     0,   735,     0,     0,   742,     0,     0,
     748,     0,    51,    57,    36,    38,    33,    35,    39,    41,
      42,    44,    47,    49,    37,    54,    34,    53,    40,    55,
      43,    45,    48,    50,     0,    86,     0,    81,   615,    89,
       0,   616,     0,   620,    54,    53,    55,   656,   382,   376,
     377,   378,   379,   380,   381,     0,     0,   387,   386,   384,
     383,   385,     0,     0,     0,    62,    61,    60,     0,     0,
       0,     0,     0,     0,     0,     0,    64,    63,    65,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    67,    68,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   514,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   209,     0,     0,     0,     0,     0,     0,   375,
       0,     0,     0,     0,     0,     0,     0,   367,   361,   364,
      77,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    87,    84,    85,
       0,    96,   390,   389,   391,   392,   393,   531,   532,   533,
     534,   535,    69,   537,   538,   539,   540,   541,   542,   543,
     544,   545,   546,   547,   548,   549,   550,   551,     0,   580,
     581,     0,     0,   584,   585,   586,   587,   588,   589,   590,
     591,   592,   593,   594,   595,   596,   597,   599,   600,   598,
       0,   502,   503,   504,   505,   506,   507,   508,   509,   510,
     511,   512,   513,    69,   517,   518,   519,   520,    69,   522,
     523,   524,   525,   475,   476,   477,   478,   479,   480,   481,
     482,   483,   484,   485,   486,   487,   488,    69,   489,   491,
     492,   493,   494,   495,   496,     0,     0,     0,     0,     0,
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
      69,   715,   716,   717,   718,   719,   720,     0,    69,    69,
     738,    90,     0,   737,   744,   745,    88,     0,     0,     0,
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
     356,   357,   358,    79,   713,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   515,     0,
       0,     0,     0,     0,     0,     0,     0,   214,    69,     0,
     216,     0,   222,     0,   224,     0,   226,     0,   249,     0,
       0,     0,   270,     0,   272,     0,   274,     0,     0,     0,
     228,     0,   230,     0,   236,     0,   238,     0,   240,     0,
       0,     0,     0,     0,     0,     0,   398,   399,   400,   401,
     402,   403,   404,   405,   406,   407,   408,   409,   410,   411,
     412,    69,     0,   414,   415,   417,   418,   419,   420,   421,
       0,   422,   423,     0,     0,   425,   428,   429,   430,   431,
     432,   433,     0,   436,   434,   435,    69,   439,   440,   441,
     442,    69,   444,   445,   446,   447,   448,   449,   450,   451,
     452,    69,   454,     0,   455,     0,   458,   459,   461,   462,
     463,     0,     0,   460,     0,   466,   468,   469,   470,   471,
     472,   473,     0,   277,   276,   278,   279,     0,   280,     0,
       0,     0,     0,   285,   286,     0,   288,   290,     0,    94,
     291,     0,   292,   212,   213,     0,   218,     0,   232,     0,
     234,     0,   220,     0,     0,   557,   558,     0,     0,   561,
     562,   563,   564,   565,   566,   567,   568,   569,   570,   571,
     572,   573,   574,   576,   577,   575,     0,     0,     0,     0,
      77,   723,   714,     0,    77,   732,   729,   728,    91,   739,
     536,   579,     0,     0,    69,    69,   490,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    73,    76,    74,    74,
      74,    74,    74,     0,     0,     0,   271,   273,   275,     0,
       0,    74,    74,    74,    74,    74,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   208,     0,     0,
       0,     0,     0,    92,     0,     0,    74,    74,    74,    74,
       0,     0,     0,   371,   352,   355,   722,    69,   731,    69,
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
     721,   730,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   413,   416,   424,   426,
       0,   437,    69,    69,   453,     0,     0,     0,     0,     0,
     104,   289,   556,     0,     0,     0,     0,   516,   521,   242,
       0,     0,     0,   254,   265,   269,   259,   266,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   582,   583,   245,     0,     0,   247,     0,     0,     0,
       0,     0,     0,   456,     0,   464,     0,   467,     0,     0,
       0,     0,   255,   260,   256,     0,     0,     0,     0,     0,
       0,     0,   246,   248,   427,   438,   443,   457,   465,     0,
       0,   559,   560
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
      -1,     1,    41,  1414,   775,   776,   777,   778,   789,   333,
    1377,  1409,  1410,  1168,  1411,   748,   476,   477,  1748,  1544,
     478,  1942,  1943,   646,  1532,   873,   223,   286,   440,   224,
      42,    83,   149,    72,   156,    73,    74,   256,   257,   258,
     259,   260,   198,   232,   199,   200,   261,   262,   304,    51,
     263,   169,   698,   264,   193,   294,   194,   295,   192,   293,
     697,   265,   191,   292,   765,   772,   276,   266,   144,  1153,
     282,   267,   148,   281,   268,   147,   277,   269,   145,  1154,
     278,   270,   146,   175,   139,   140,   246,   247,   248,   249,
     250,   251,    60,    61,   252,   253,   254,   255,   134,    62,
     209,   271,   272,   273,    64,   136,   187,   274,   152,   451,
      66,   162,   236,   452,    67,   163,   237,   453,    68,   164,
     238,   454,    69,   165,   239
};

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
#define YYPACT_NINF -1824
static const yytype_int16 yypact[] =
{
   -1824,  2239, -1824, -1824,     0,    -4,    -4,    -4,    27,    27,
      27,    27,    27,    87, -1824, -1824, -1824, -1824, -1824, -1824,
   -1824, -1824, -1824,   313,    -4,   -27,    27,    27,    27,    27,
   -1824, -1824,    -4,    -4,    -4,    -4,   142,    -4,    -4,    -4,
   -1824, -1824, -1824,   463, -1824, -1824, -1824, -1824, -1824, -1824,
   -1824,   159, -1824, -1824, -1824,  2660,  3092,  3787,  3841,  4161,
   -1824,  2070, -1824,   309, -1824, -1824, -1824, -1824, -1824, -1824,
   -1824, -1824,    27,  -187,  -308,  -119,  -114, -1824, -1824, -1824,
   -1824, -1824, -1824,  -187,   463, -1824, -1824,   -96,  -143,  -143,
    -143,  -143,   -94,   -84, -1824, -1824, -1824, -1824,   -67,   -57,
   -1824,   -50, -1824, -1824, -1824, -1824,   -17,    -6, -1824, -1824,
   -1824, -1824, -1824, -1824, -1824, -1824, -1824, -1824, -1824, -1824,
   -1824, -1824, -1824, -1824, -1824, -1824, -1824, -1824, -1824, -1824,
   -1824, -1824, -1824, -1824,   317, -1824,  -187,  -187, -1824, -1824,
   -1824,   368,   370,   375,  -187,  -187,  -187,  -187,  -187,  -187,
      61, -1824,    72,   238, -1824, -1824, -1824, -1824, -1824, -1824,
     318,   492,  -187,  -187,  -187,  -187,   515,   538, -1824,  -187,
   -1824, -1824, -1824,   178, -1824,     7, -1824, -1824, -1824,   202,
     223,   234,   244,   245,   248,     6,   215, -1824, -1824, -1824,
   -1824,  -187,  -187,  -187,  -187, -1824, -1824,   224,   562, -1824,
     200, -1824, -1824, -1824, -1824, -1824, -1824, -1824,   249,     4,
    3155, -1824,   250, -1824, -1824, -1824, -1824,   326, -1824,     6,
     622,   624,   263, -1824,    12, -1824,   221, -1824, -1824, -1824,
   -1824, -1824,   642,   224,   277,   175,   286,   287,   301,   315,
   -1824, -1824,   134,   679,    67,    55, -1824, -1824, -1824, -1824,
   -1824, -1824, -1824,    38,     9,   335, -1824, -1824, -1824, -1824,
   -1824, -1824, -1824, -1824, -1824, -1824,   291,  1038,  2069,  2134,
    2201,   309, -1824,  2691, -1824,   323,    37,   587,  1057,   340,
     344,     5,  1028,    33,  -260, -1824,  -187,    92, -1824, -1824,
   -1824, -1824,   321,   336,   345,   358, -1824,   710,   724, -1824,
     452, -1824, -1824, -1824,    13, -1824, -1824, -1824,    88,    88,
   -1824,   573,   608, -1824,   147, -1824,    95,    95, -1824,   -20,
   -1824,    70, -1824,   106, -1824,    44, -1824, -1824, -1824,   369,
     573,   357,   372,   376,   378,   389,   390,   396,   398,   400,
   -1824,   397,   401,   405,   416,   417,   433,   439,   443,   444,
     445,   446,   461,   462,   475,   478,   482,   483,   485,   488,
     501,   503,   505, -1824,   428,   521,   525,   541,   438,   498,
     542,   543,   545,   548,   549,   550,   554,   556,   557,   558,
     563,   564,   565,   566,   567,   578,   580, -1824,   517,  4631,
     574,   582,   585,   590,   591,   592,   594,   595,   598,   599,
     601,   602,   603,   607,   619,   621,   623,   625,   626,   627,
     628,   629,   630, -1824,   615,   631,   632,   634,   636,   637,
     638,   640,   641,   645,   646,   649,   650,   651,   654,   655,
     656,   657,   658,   661,   662,   665, -1824, -1824,   617, -1824,
    -187,   540,   659,   660, -1824, -1824, -1824, -1824, -1824, -1824,
     666,   394,   207,  -223,  -195, -1824,   667, -1824,   669,   675,
     261,   333,    88,    88,    88,    88,    88,    88,    88,    88,
      88,    88,   573,   608, -1824,    88,    29,    50,  -121, -1824,
      -8,    95,    95,    95, -1824, -1824, -1824,   117, -1824, -1824,
   -1824, -1824, -1824, -1824,   552,   552,   130,   130,   130, -1824,
     153,  4631,   153,   153,   153,   517,   153,  4631,   153,   153,
    4631,  4631,  4631,   153,  4631,   153,   153,  4631,  4631,  4631,
     153, -1824,   671,  4631,  4631,   153,   153,  4631,   153,  4631,
    4631,  4631,  4631,  4631,   153,   153,  4631,   153,  4631,   153,
    4631,  4631,  4631,  4631,  4631, -1824, -1824, -1824, -1824, -1824,
   -1824, -1824, -1824, -1824, -1824, -1824, -1824, -1824, -1824, -1824,
   -1824, -1824, -1824, -1824, -1824, -1824, -1824, -1824, -1824, -1824,
   -1824, -1824, -1824, -1824, -1824, -1824, -1824, -1824, -1824, -1824,
   -1824, -1824, -1824, -1824, -1824, -1824, -1824, -1824, -1824, -1824,
   -1824, -1824, -1824, -1824, -1824, -1824, -1824, -1824, -1824, -1824,
   -1824, -1824, -1824, -1824, -1824, -1824, -1824, -1824, -1824, -1824,
   -1824, -1824, -1824, -1824, -1824, -1824, -1824, -1824, -1824, -1824,
   -1824, -1824, -1824, -1824, -1824, -1824, -1824, -1824, -1824, -1824,
   -1824, -1824, -1824, -1824, -1824, -1824, -1824, -1824, -1824, -1824,
   -1824, -1824, -1824, -1824, -1824, -1824,   664, -1824,   153,  4631,
     153,   153,  4631,  4631,  4631,  4631,  4631,  4631,  4631,  4631,
     517,   153,   153,   153,  4631,   517,   153,   153,   153,   153,
   -1824,   153,   153,   153,   153,  4631,   153,   153,   153,   153,
     153,   153,  4631,  4631,   153,  4204,   153,   153,   153,   153,
     153,   153, -1824,   680, -1824,   668,   691,    75,   265,   527,
     593,   517,   635,   698,   699,   705,   706,   707,   708,   711,
   -1824,   643,   712,   713, -1824,   715,   722, -1824,   723,   727,
   -1824,  1078, -1824, -1824,   237,   273,   237,   273,   669,   675,
     669,   675,   669,   675,   237,   273,   237,   273,   669,   675,
     669,   675,   669,   675,    29, -1824,    88, -1824, -1824, -1824,
     517, -1824,   238, -1824,   728,   728, -1824, -1824, -1824, -1824,
   -1824, -1824, -1824, -1824, -1824,   729,   730, -1824, -1824, -1824,
   -1824, -1824,   731,   732,   733,   573,   608, -1824,   735,   737,
     738,   739,   741,   153,   742,   743,   573,   608, -1824,   744,
     745,   749,   751,   753,   756,   763,   764,   766,   768,   771,
     772,   773,   153,   775,   777,   717,   767,   779,   608, -1824,
     780,   781,   783,   784,   786,   787,   788,   789,   793,   794,
     795,   801,   808,   810,   811,   812,   813, -1824,   819,   821,
     823,   824,   825,   826,   828,   845,   851,   853,   854,   858,
     153,   859,   860,   861,   863,   153,   866,   867,   868,   870,
     872,   873,   874,   875,   876,   878,   879,   881,   882,   883,
     885,   886,   887,   889,  4631,   890,   891,   892,   893,   894,
     895,   896, -1824,  3419,   153,   153,   904,   905,   906, -1824,
     907,   910,   912,   913,   914,   915,   916, -1824, -1824, -1824,
    1273,  1275,   517,  1277,  1278,  1280,  1281,    95,  1282,  1283,
     517,   517,  -233,  1024,  1298,  1299,   933, -1824,   573,   608,
      88, -1824, -1824, -1824, -1824, -1824, -1824, -1824, -1824, -1824,
   -1824, -1824,   930, -1824, -1824, -1824, -1824, -1824, -1824, -1824,
   -1824, -1824, -1824, -1824, -1824, -1824, -1824, -1824,   931, -1824,
   -1824,   943,   944, -1824, -1824, -1824, -1824, -1824, -1824, -1824,
   -1824, -1824, -1824, -1824, -1824, -1824, -1824, -1824, -1824, -1824,
    4631, -1824, -1824, -1824, -1824, -1824, -1824, -1824, -1824, -1824,
   -1824, -1824, -1824,   930, -1824, -1824, -1824, -1824,   930, -1824,
   -1824, -1824, -1824, -1824, -1824, -1824, -1824, -1824, -1824, -1824,
   -1824, -1824, -1824, -1824, -1824, -1824, -1824,   930, -1824, -1824,
   -1824, -1824, -1824, -1824, -1824,  3696,   938,   937,   939,  -348,
    -303,  -291,  -266,  -193,   947,   940,   945,   950,  -213,  -209,
    -183,   951,   952,  -111,   -56,   -42,   -39,   -23,   954,   956,
     957,   958,   959,   960,   970,   972,   973,   974,   975,   976,
     980,   981,   982,   983,   984,   985,   986,   987,   988,   989,
     990,   993,   994,   996,   998,   999,  1000,  1023,  1025,  1026,
    1027,  1029,  1030,  1031,  1032,  1034,  1035,  1036,  1037,  1039,
    1040,  1042,  1044,  1045,  1046,  1047,  1048,  1049,  1050,  1052,
    1053,  1054,  1055,  1056,  1058,  1060,  1061,  1062,  1063,  1064,
    1065,  1067,  1071,  1073,  1074,  1076,  1077,  1079,  1081,  1083,
    1084,  1085,  1086,  1087,  1088,  1089,  1090,  1015,  1091,  1094,
    1098,  1099,  1101,  -311,  1103,  1095,  1105,  1106,  1107,  1108,
    1110,  1111,  1112,  1113,  1115,  1117,   108,   136,   161,   164,
    1118,  1119,  1120,  1021,  1051,  1121,  1122,  1124,  1125,  1126,
    1127,  1128,  1129,  1130,  1131,  1132,  1133,  1137,  1138,  1140,
    1141,  1143, -1824, -1824, -1824,   949,  1148,  1426,  1498,    -5,
    1523,  1511,   183,  -220,   153,  1513,  1256, -1824,    99,  1153,
    1273,  1158,  1159,  1160,  1162,   284,  1163,  1161,  1273,  1273,
    1165,    95,  1169,  1171,  1172, -1824,    29, -1824,   153,   153,
    1170,  1173,  1168,   153,   153,  4631, -1824, -1824,  1537,  1538,
      15,   112,    15,   112,    15,   112,    15,   112,    15,   112,
    1553,  1541,  1542,  1543,  1544,  1547,  1548,  1549,  1551,  1554,
    1555,  1556,    15,   112,    15,   112,    15,   112,    15,   112,
      15,   112,  1558,  1559,  1560,  1561,  1564,  1566,   153,   153,
     153,   153,  4631,   153,   153,   153,   153,   153,  4135,  4631,
    4631,   153,   517,   517,   153,   153,   153,   153,   153,   153,
     153,   517,  4631,   153,  1201,  1202,   153,  4631,  4631,  4631,
    4631,  4631,  4631,   517,  4631,  4631,  4631,   517,   153,   153,
     153,  4631,   517,   153,   153,   153,   153,   153,  4631,   153,
     153,   153,   517,   153,  4493,  1203,   153,   153,  4631,  4631,
    4631,  1204,  1207,   153,  4562,   153,   153,  4631,  4631,  4631,
     153,   112,   153,  1333,  1577,  1334,  1578,   517,   153,   517,
    1580,   153,   153,  1215,    15,    95,   517,   517,  1582,    95,
     517,    15,   112,    15,   112,    15,   112,    15,   112,  1217,
    4631,  4631,   153,   153,  4631,   153,  4631,  4631,  4631,  4631,
    4631,   153,   153,  4631,   153,  4631,   153,  4631,  4631,  4631,
    4631, -1824, -1824,  1223,  1226,  1227,  1273,  1228,  1229,  1230,
    1231,    95,  1232,    95,  1233,  1234,  1237,  1599,  1240,     3,
      99, -1824, -1824, -1824, -1824, -1824, -1824,    22,    99,    99,
   -1824,   199,   167, -1824, -1824, -1824, -1824,  1168,  1235,  4631,
    4631,  1242,   930,   930,  1168,  1241,   219, -1824, -1824,  -169,
    1243,   114, -1824, -1824,  1236,  1246,  1238,  1248,  1244,  1249,
    1247,  1250,  1251,  1252,  1254,  1258,  1259,  1255,  1261,  1265,
    1266,  1268,  1267,  1270,  1271,  1276,  1262,  1279,  1269,  1284,
    1274,  1285,  1286,  1288,  1287,  1272,  1289,  1294,  1295,  1300,
    1301,  1290,  1291,  1302,  1305,  1306,  1310,  1311,  1313,  1316,
    1317,  1318,  1321,  1322,  1323,  1324,  4631,   153,  1325,  1327,
    1328,  1329,  1330,  1331,  1335,  4631,  1336,  1337,  4631,  4631,
    1338,  1339,  1340,  1341,  1342,  1343,  1344,  4631,  1345,  1348,
    1356,   153,  1364,  1365,  1366,  1368,   153,  1369,  1370,  1371,
    1372,  1373,  1374,  1375,  1376,  1377,   153,  1378,  4631,  1379,
    4631,  1380,  1381,  1382,  1384,  1385,  4631,  4631,  1386,  4631,
    1387,  1388,  1390,  1391,  1392,  1393,  1394,  1395,  1397,  1400,
    1401, -1824,  -198,  1404,    88,  1402,    95,  1407,  1405,  1406,
    1612,  1408,   385,    95,    32,    47,  1412,   408,    95,  1416,
    1409,  1417,  1415,  1425,  1418,  1428,  1421,   153,  1431,  1432,
    1422,  1423,  1435,  1436,  1437,  1438,  1439,  1440,  1442,  1443,
    1444,  1446,  1447,  1448,  1449,  1450,  1451,  1452,  1453, -1824,
   -1824, -1824,    99, -1824, -1824, -1824, -1824,   167, -1824,   167,
   -1824, -1824, -1824, -1824, -1824,    86,  1454,  1455,   143,  1456,
    1457,  1458,    95,  1460,  1461,  1463,  1464,  1466, -1824,   153,
     153,  1467,  1293,    56,    95,   613,  1620, -1824,    99,  1296,
   -1824,  1433, -1824,  1471, -1824,  1478, -1824,  1479, -1824,  1481,
    1482,  1483, -1824,  1477, -1824,  1480, -1824,  1484,  1486,  1488,
   -1824,  1489, -1824,  1490, -1824,  1492, -1824,  1493, -1824,  1494,
    1495,  1496,  1497,  1499,  1500,  1502, -1824, -1824, -1824, -1824,
   -1824, -1824, -1824, -1824, -1824, -1824, -1824, -1824, -1824, -1824,
   -1824,   930,  1487, -1824, -1824, -1824, -1824, -1824, -1824, -1824,
    1501, -1824, -1824,  1503,  1504, -1824, -1824, -1824, -1824, -1824,
   -1824, -1824,  1507, -1824, -1824, -1824,   930, -1824, -1824, -1824,
   -1824,   930, -1824, -1824, -1824, -1824, -1824, -1824, -1824, -1824,
   -1824,   930, -1824,  1508, -1824,  1509, -1824, -1824, -1824, -1824,
   -1824,  1514,  1515, -1824,  1516, -1824, -1824, -1824, -1824, -1824,
   -1824, -1824,  1505, -1824, -1824, -1824, -1824,  1396, -1824,    29,
    1510,   167,  1638, -1824, -1824,  1517, -1824, -1824,   167, -1824,
   -1824,   517, -1824, -1824, -1824,   167, -1824,  1529, -1824,  1531,
   -1824,  1534, -1824,  1535,  1526, -1824, -1824,  1536,  1539, -1824,
   -1824, -1824, -1824, -1824, -1824, -1824, -1824, -1824, -1824, -1824,
   -1824, -1824, -1824, -1824, -1824, -1824,  1506,  1533,  1540,  1545,
     257, -1824, -1824,  1546,   257, -1824, -1824, -1824,   199, -1824,
   -1824, -1824,   153,  4631,   930,   930, -1824,  1643,   173,  1552,
     195,  1528,  1532,  1550,  1562,  1563, -1824, -1824,    15,    15,
      15,    15,    15,     2,    95,  1694, -1824, -1824, -1824,    45,
      95,    15,    15,    15,    15,    15,  1756,    95,  1857,    81,
      95,  1858,  4631,   153,   153,  4631,  4631,  4631,   153,   153,
     153,   153,  4631,  4631,  4631,   153,   153, -1824,  1567,   517,
    1568,  1569,    98, -1824,    95,  1570,    15,    15,    15,    15,
     153,  1571,  1572, -1824, -1824, -1824, -1824,    99, -1824,    99,
    1574,  1575,   153,   153,  1573,  1581,   257, -1824,  1579,  1565,
    1587,  1590,  1591,  1592,  1588,  1589,  1595,  1596,  1597,  1598,
    1862,   411,  1600,  1601,  1863,   419,  1602,  1603,  1604,  1605,
    1607,  1608,   435,  1609,  1610,  1865,   507,  1611,  1168,  1168,
    1168,  1584,  1583,  1168,   930,   930,  1168,  1613,  1614,  1615,
    1617,  1618,  1616, -1824,    58, -1824,  1623, -1824, -1824, -1824,
     573,   608, -1824,    53,   167, -1824,  1624,  1626,  1627,  1628,
    1585,  4631,  4631,  1629,  1630,   153,   153,  1168,  1168, -1824,
   -1824,    99,  1606,  1898,    95,  1899,  1902,    95, -1824, -1824,
   -1824, -1824, -1824, -1824,  1631, -1824, -1824, -1824,  1632, -1824,
   -1824, -1824, -1824, -1824, -1824, -1824, -1824, -1824, -1824,  1633,
   -1824, -1824,  1639,  1640,  1641,  1642,  4631,  1644,   153,   153,
    1645,   153,   153,  4631,  4631,   153, -1824, -1824, -1824, -1824,
     190,  1646, -1824, -1824, -1824, -1824, -1824,  1647,  1634,  1651,
   -1824, -1824,  1656,  1658,  1648,  1650,  1667,   102,  1668,   529,
    1670,  1676,   532,    95,    95,    95, -1824, -1824, -1824, -1824,
    1673, -1824,   930,   930, -1824,  1675,  1685,  1684,  1687,  1686,
   -1824, -1824, -1824,   153,  4631,  1679,  1693, -1824, -1824, -1824,
    1695,    71,    60, -1824, -1824, -1824, -1824, -1824,   128,   206,
     246,  4631,   153,   153,  1699,   153,  1705,  4631,  1706,  1690,
    1703, -1824, -1824, -1824,    50,  -121, -1824,  1708,  1715,  1716,
    1742,  1168,  1168, -1824,  1743, -1824,  1744, -1824,   153,   153,
    1722,  1751, -1824, -1824, -1824,  1753,  1754,  1755,  1757,  1758,
    1750,  1764, -1824, -1824, -1824, -1824, -1824, -1824, -1824,  1759,
    1760, -1824, -1824
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
   -1824, -1824, -1824,  -456,  1059,   -58,  -153,  -391,  -383,  1206,
    -901,   514, -1162, -1163,  -144,  -408,  -474, -1823, -1172, -1325,
    -135,   -83, -1824,  1525, -1824,  1066,  -177, -1824, -1824,  1711,
   -1824, -1824, -1824,   528,   652,  1068,   644,  1930,  1931,  1936,
    1938,  1941, -1824,  1721,  1723, -1824,  1956,  1995, -1824, -1824,
    1996, -1824,   123,  1997, -1824, -1824, -1824, -1824, -1824, -1824,
   -1824,  2014, -1824, -1824,  1521,    96, -1824,  2033, -1824, -1824,
   -1824,  2146, -1824, -1824,  2153, -1824, -1824,  2154, -1824, -1824,
   -1824,  2155, -1824, -1824,  -125,   800, -1824, -1824, -1824, -1824,
   -1824, -1824,   455, -1824,  -242,  1909, -1824, -1824, -1824,   -44,
   -1824,    40,  1891, -1824, -1824, -1824, -1824,  2169, -1824, -1824,
   -1824, -1824, -1824, -1824, -1824, -1824, -1824, -1824, -1824, -1824,
   -1824, -1824, -1824, -1824, -1824
};

/* YYTABLE[YYPACT[STATE-NUM]].  What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule which
   number is the opposite.  If zero, do what YYDEFACT says.
   If YYTABLE_NINF, syntax error.  */
#define YYTABLE_NINF -701
static const yytype_int16 yytable[] =
{
     190,   744,  1545,   320,   241,   241,   390,  1380,   211,  1392,
     241,   322,   324,   288,   455,  1388,  1389,  1365,    71,   135,
    1407,  1188,    70,   241,  1899,   220,   221,   438,  1200,   212,
     745,   220,   221,  1749,   288,    87,  1934,  1408,   334,   241,
    1415,    63,  1417,  1201,  1419,   241,  1421,   290,  1749,    71,
    1180,   749,   220,   221,  2009,   715,   241,   241,   716,   749,
    1435,  1749,  1437,  1372,  1439,  1317,  1441,  1903,  1443,   751,
     753,   241,  1193,  1202,   307,   188,   189,  1194,   307,   485,
     141,   486,  1318,   488,   245,  1204,   456,   305,  1203,   313,
     188,   189,   306,   307,   306,   307,  1195,   876,   314,  1937,
    1205,   442,   307,  1914,   306,   307,   290,   241,  1789,    82,
    1206,   780,   781,   782,   443,   784,   718,   719,   241,  1407,
    1938,  1412,   332,   439,  2060,  1207,   790,  1939,   321,   323,
     794,   767,   796,   797,  1413,   305,  1408,   801,   188,   189,
     306,   307,   805,   806,   305,   154,   155,   188,   189,   306,
     307,   188,   189,   306,   307,   310,   821,   188,   189,   306,
     307,   474,  1541,  1214,    96,  1793,   717,  1216,   310,  1549,
     475,  1551,  1215,  1553,   305,  1555,  1217,   188,   189,   306,
     307,   101,   135,  1208,   312,  1736,   319,   188,   189,   306,
     307,  1937,  1737,  1218,   720,  1885,   306,   307,  1209,  1587,
     487,  1589,  1219,  1582,   138,   196,   335,   336,   337,   338,
     339,   197,  1938,   391,   -75,   392,   393,   331,  1187,   394,
     395,   396,   397,   398,   399,   400,   226,   135,   401,   402,
     403,   404,   405,   406,   407,   408,   409,   410,  2084,   411,
     412,   711,   188,   189,   544,   757,    20,    21,    22,    23,
     459,   461,    20,    21,    22,    23,   473,   828,   479,   480,
     831,   188,   189,   306,   307,  1222,   752,   222,   747,   142,
     841,   842,   843,   222,   143,   846,   847,   848,   849,  1167,
    1223,   851,   852,   853,    30,    31,   856,   880,   153,   859,
     860,   160,   103,   863,   222,   857,   858,   868,   869,   870,
     871,  -696,  -696,  -696,   161,  -696,  -696,  -696,  -696,  -696,
     768,   769,   770,   771,  -696,  -696,  -696,  -696,  -696,  -696,
    1224,   166,  -696,  -696,  -696,  -696,  -696,   188,   189,   306,
     307,   167,   329,   168,  1226,  1225,   907,  1228,   881,   172,
      20,    21,    22,    23,    20,    21,    22,   195,   196,  1227,
     877,   878,  1229,  1230,   197,   788,   788,   481,   482,   483,
     788,   783,   788,   788,  1741,  -324,   170,   788,  1231,   481,
     482,   483,   788,   788,   484,   809,  1755,   171,   723,  1595,
     242,   809,   809,   310,   809,   242,   788,  -641,   243,   176,
    1900,   177,  -649,   243,   413,   138,   178,   244,  1598,   213,
     138,   289,   457,   310,   725,   727,   729,   731,   733,   735,
     737,   739,   741,   743,   242,  1750,   746,   473,   747,  1751,
     242,  -645,   437,   754,   755,   756,   340,  -653,   702,   138,
    1752,   242,  1808,  1904,  1751,   138,  1186,   750,  -642,   747,
    2010,  2007,  2011,  2086,   316,   750,   242,  1751,   316,   185,
     787,   787,   317,  -646,   978,   787,   317,   787,   787,   310,
     186,   318,   787,   308,   879,   308,  1369,   787,   787,  1915,
     808,   309,   316,   309,   310,   308,   808,   808,    85,   808,
     317,   787,   242,   309,  1331,  -101,  1187,  -101,   747,  -650,
     310,  1155,  1156,   242,    20,    21,    22,   809,   100,  1332,
    -654,  1609,  1610,    30,    31,   481,   482,   483,  1616,    30,
      31,   308,  1333,   201,   712,   713,   840,  2087,   809,   309,
     308,   845,   310,   809,   308,   788,   788,  1334,   309,    43,
     308,   310,   309,   809,   809,   310,   206,  1335,   309,   151,
    1337,   864,   882,   883,   884,   885,   886,   279,   280,   880,
     308,    84,  1336,   758,  1602,  1338,   747,   890,   309,   207,
     308,   310,  -696,  -696,  -696,  -696,   210,   308,   309,   699,
     700,   310,   481,   482,   483,   309,   481,   482,   483,  -696,
    -696,  -696,  -696,   481,   482,   483,   234,   235,   341,  1888,
     214,    43,   808,   773,   774,  2088,   714,    30,    31,   911,
     881,    30,    31,   231,   225,  1613,   910,  -696,  -696,   233,
     291,   215,  1614,   808,  1615,   880,   464,   465,   808,   466,
     787,   787,   216,   481,   482,   483,   275,  1877,   808,   808,
     809,  1879,   217,   218,   308,  2089,   219,   240,   462,   463,
     464,   465,   309,   466,   284,   310,   285,   722,   287,   809,
      75,    76,   469,   470,   887,   471,  1894,  1895,  1896,  1897,
    1898,   481,   482,   483,   296,   298,   881,  1385,    86,  1906,
    1907,  1908,  1909,  1910,   300,   301,    92,    93,    94,    95,
    -696,    97,    98,    99,  1811,  1812,  1813,   809,   909,   302,
    1814,  1815,  1944,   703,   704,   705,   706,   707,   708,   709,
     308,   315,  2062,   303,  1946,  1947,  1948,  1949,   309,   444,
     467,   468,   469,   470,    43,   471,   388,  1616,   326,   723,
     389,   788,   788,  1961,   445,   808,   759,   760,   761,   762,
     763,   764,   448,   446,   467,   468,   469,   470,    43,   471,
     490,   157,   158,   159,   808,   449,   447,  1416,  1170,  1418,
     450,  1420,   489,  1422,    43,   491,  1178,  1179,  1181,   492,
    1378,   493,   481,   482,   483,   494,   495,  1436,  1747,  1438,
    1842,  1440,   496,  1442,   497,  1444,   498,   500,  1396,  1370,
     499,   501,   808,   710,  1401,   481,   482,   483,   481,   482,
     483,  1754,   502,   503,  1975,  1848,   481,   482,   483,    43,
    1849,    43,  1979,  1403,   882,   883,   884,   885,   886,   504,
    1850,   521,   481,   482,   483,   505,   787,   787,  1986,   506,
     507,   508,   509,   342,   343,   344,   345,   346,   347,   348,
     349,   525,   350,   351,   352,   353,   354,   510,   511,  1175,
     355,   356,   357,   358,   359,   360,   361,   362,  1452,  1453,
    1454,   512,   473,  1457,   513,  1527,  1460,  1461,   514,   515,
    1465,   516,  1458,  1459,   517,  1470,  1471,  1472,  1473,  1474,
     882,   883,   884,   885,   886,  1480,  1550,   518,  1552,   519,
    1554,   520,  1556,   150,   481,   482,   483,  1492,  1493,  1494,
    1990,   526,  1497,  1498,  1499,  1500,  1501,   522,  1503,  1504,
    1505,   523,  1507,  1882,  1883,   310,   481,   482,   483,   481,
     482,   483,  2064,  1511,  1512,  2067,   888,   524,   527,   528,
    1518,   529,  1521,  1522,   530,   531,   532,  1526,   694,  1528,
     533,  1539,   534,   535,   536,  1535,   173,   174,  1538,   537,
     538,   539,   540,   541,   179,   180,   181,   182,   183,   184,
     462,   463,   464,   465,   542,   466,   543,   647,   648,  1560,
    1561,   649,   202,   203,   204,   205,   650,   651,   652,   208,
     653,   654,  1597,  1574,   655,   656,   363,   657,   658,   659,
    1600,  1601,   889,   660,  1603,   467,   468,   469,   470,  1604,
     471,   227,   228,   229,   230,   661,  1611,   662,   670,   663,
     692,   664,   665,   666,   667,   668,   669,   671,   672,   788,
     673,   809,   674,   675,   676,  1366,   677,   678,  1371,  1373,
     891,   679,   680,  1998,  1999,   681,   682,   683,   899,   414,
     684,   685,   686,   687,   688,   809,   809,   689,   690,   104,
     809,   691,   701,   721,   874,   695,   696,   827,  -700,  -700,
    -700,   466,  -700,  -700,  -700,  -700,  -700,   471,   364,   802,
    1739,  -700,  -700,  -700,  -700,  -700,  -700,   875,   872,  -700,
    -700,  -700,  -700,  -700,   892,   893,    77,    78,    79,    80,
      81,   894,   895,   896,   897,   809,   441,   898,   900,   901,
     809,   902,   788,   788,    88,    89,    90,    91,   903,   904,
     906,   809,   809,   905,   787,  1701,   808,   483,  1466,  1467,
     809,   941,   912,   913,   914,   915,   916,  1475,   917,   102,
     918,   919,   920,  1391,   921,   923,   924,   925,   926,  1487,
     808,   808,   927,  1491,   928,   808,   929,  1596,  1496,   930,
     137,  2072,  2073,   788,   788,  1599,   931,   932,  1506,   933,
     788,   934,   788,   788,   935,   936,   937,   788,   939,   788,
     940,   942,   943,   944,   945,   788,   946,   947,   788,   948,
     949,   950,   951,  1534,  1786,  1536,   952,   953,   954,  1787,
     808,  1788,  1543,  1543,   955,   808,  1548,   787,   787,   788,
     788,   956,   809,   957,   958,   959,   808,   808,   809,   809,
     960,   809,   961,   788,   962,   808,   963,   964,   965,   966,
    1817,   967,   415,   416,   417,   418,   419,   420,   421,  1805,
     422,   423,   424,   425,   426,   427,   428,   429,   968,   430,
     431,   432,   433,   434,   969,   435,   970,   971,   787,   787,
     693,   972,   974,   975,   976,   787,   977,   787,   787,   979,
     980,   981,   787,   982,   787,   983,   984,   985,   986,   987,
     787,   988,   989,   787,   990,   991,   992,  1542,   993,   994,
     995,  1547,   996,   998,   999,  1000,  1001,  1002,  1003,  1004,
    1157,  1158,  1159,  1160,   787,   787,  1161,   808,  1162,  1163,
    1164,  1165,  1166,   808,   808,  1167,   808,  1169,   787,  1171,
    1172,   311,  1173,  1174,  1176,  1177,  1182,  1183,  1184,  -700,
    -700,  -700,  -700,  1391,   809,  1391,  1185,  1187,  1189,  1190,
    1191,  1197,  1198,  1210,  1199,  1211,  -700,  -700,  -700,  -700,
    1212,  1858,  1361,  1860,   330,  1213,  1220,  1221,   809,  1232,
    1863,  1233,  1234,  1235,  1236,  1237,  1238,  1865,  1239,  1240,
    1241,  1242,  1243,   809,  -700,  -700,  1244,  1245,  1246,  1247,
    1248,  1249,  1250,  1251,  1252,  1253,  1254,   458,   460,  1255,
    1256,  1809,  1257,   472,  1258,  1259,  1260,   365,   366,   367,
     368,   369,   370,   371,   372,   373,   374,   375,   376,   377,
     378,   379,   380,   381,   382,   383,   384,   385,   386,  1261,
    1311,  1262,  1263,  1264,   809,  1265,  1266,  1267,  1268,   808,
    1269,  1270,  1271,  1272,  1342,  1273,  1274,   436,  1275,  1880,
    1276,  1277,  1278,  1279,  1280,  1281,  1282,  -700,  1283,  1284,
    1285,  1286,  1287,   808,  1288,  1363,  1289,  1290,  1291,  1292,
    1293,  1294,   313,  1295,  1343,   313,   387,  1296,   808,  1297,
    1298,  1790,  1299,  1300,  1794,  1301,   809,  1302,  1925,  1303,
    1304,  1305,  1306,  1307,  1308,  1309,  1310,  1312,  1927,  1953,
    1313,  1954,  1931,  1932,  1314,  1315,   473,  1316,  1391,  1319,
    1320,  1321,  1322,  1323,  1324,  1391,  1325,  1326,  1327,  1328,
    1391,  1329,  1958,  1330,  1339,  1340,  1341,  1344,  1345,   808,
    1346,  1347,  1348,  1349,  1350,  1351,  1352,  1353,  1354,  1355,
    1992,  1993,  1994,  1356,  1357,  1997,  1358,  1359,  2000,  1360,
    1364,   724,   726,   728,   730,   732,   734,   736,   738,   740,
     742,  1362,  1367,  1368,   472,  1375,  2012,   312,  1376,  1379,
     312,  1381,  1382,  1383,  1798,  1384,  1386,  1387,  1390,  2024,
    2025,   808,  1393,  2026,  1394,  1395,  1810,   747,  1399,  1405,
    1406,  1400,  1423,  1424,  1425,  1426,  1427,   786,   786,  1428,
    1429,  1430,   786,  1431,   786,   786,  1432,  1433,  1434,   786,
    1445,  1446,  1447,  1448,   786,   786,  1449,   330,  1450,  1478,
    1479,  1510,  1516,   330,   330,  1517,   330,  1529,   786,  1530,
    1533,  1531,  1537,  1540,  1546,  1557,  1579,  1864,  2043,  1580,
    1581,  1583,  1584,  1585,  1586,  1588,  1590,  1591,  2045,  2046,
    1592,  1593,  2049,  1594,  1605,  1608,  1617,  1612,  1619,  1620,
    1621,  1622,  1624,  1626,  1745,  1628,  1623,   474,  1632,  1625,
    1629,   474,  1816,  1627,  1630,  1631,   475,  1633,  1634,   788,
     475,  1636,  1635,  1637,  1641,   313,  1638,  1639,  1650,  1640,
    1861,  1643,  1642,  1857,  1886,  1884,  1645,  1644,  1646,  1807,
    2079,  1648,  1818,  1656,  1657,  1651,  2100,  2101,  1647,  1649,
    1652,  1653,  2092,  2106,  2107,  1658,  1654,  1655,  1659,  1660,
     809,   809,  2094,  1661,  1662,   809,  1663,   809,   788,  1664,
    1665,  1666,   788,   788,  1667,  1668,  1669,  1670,  1673,   330,
    1674,  1675,  1676,  1677,  1678,   475,  1902,   809,  1679,  1681,
    1682,  1685,  1686,  1687,  1688,  1689,  1690,  1691,  1693,   809,
     330,  1694,   473,   474,   810,   330,   473,   786,   786,  1695,
     816,   817,   475,   819,   787,   330,   330,  1697,  1698,  1699,
     312,  1700,  1702,  1703,  1704,  1705,  1706,  1707,  1708,  1709,
    1710,  1712,  1714,  1716,  1717,  1718,  1901,  1719,  1720,  1723,
    1725,  1726,  1905,  1727,  1728,  1729,  1730,  1731,  1911,  1912,
    1733,  1732,  1916,  1734,  1735,   808,   808,  1738,  1743,  1744,
     808,  1746,   808,   787,  1742,  1753,  1740,   787,   787,  1756,
    1758,  1757,   809,   809,  1941,   908,  1391,  1759,  1760,  1819,
    1761,  1762,   808,  1763,  1765,  1766,  1767,  1768,  1769,  1770,
    1771,  1772,  1773,  1774,   808,  1775,  1776,  1777,   473,  1778,
    1779,  1780,  1781,  1782,  1783,  1784,  1785,  1791,  1792,  1795,
    1796,  1797,   330,  1799,  1800,   809,  1801,  1820,   788,   788,
    1806,  1802,   788,  1803,  1821,  1822,   830,  1823,  1824,  1825,
    1826,   330,  1829,  1827,  1830,  1831,  1832,  1828,  1833,  1834,
    1835,  1836,  1837,  1838,  1843,  1839,  1840,   850,  1841,  1913,
    1917,  1856,   855,  2061,  1974,  1978,  1859,  1989,  1844,  1873,
    1845,  1846,   866,   867,  1847,  1851,  1852,   808,   808,   330,
     788,  1853,  1854,  1855,  1862,  1866,  2029,  1867,   474,  2032,
    1868,  1869,  1871,  1870,  1889,  1872,  1874,   475,  1890,   809,
    2028,  2030,   788,  1875,  2031,  1618,  2085,  2050,  1876,  1878,
     283,    44,    45,   786,   786,  1887,  1891,    46,  1005,    47,
     808,  1963,    48,   787,   787,   809,   809,   787,  1892,  1893,
    1933,  1935,  1941,  1945,   297,  1936,  1959,    49,   299,  1951,
    1952,  1955,  1956,  1964,  1960,  1962,  1965,  1966,  1967,   472,
    1996,  1968,  1969,  1995,  2017,  2068,  2069,  2070,  1970,  1971,
    1972,  1973,  2027,  1976,  1977,  1980,  1981,  1982,  1983,   922,
    1984,  1985,  1987,  1988,  1991,   787,    50,    52,    53,  2006,
    2001,  2002,  2003,  1391,  2004,  2005,  2008,  2013,   938,  2014,
    2015,  2016,  2020,  2021,   808,    54,   766,   787,  2033,  2034,
    2035,  2053,  2036,  2037,  2038,  2039,   779,  2041,  2044,  2051,
    2052,  2057,   785,  2058,    55,   791,   792,   793,  2054,   795,
     808,   808,   798,   799,   800,  2055,   973,  2056,   803,   804,
    2059,  2063,   807,  2065,   811,   812,   813,   814,   815,  2066,
    2071,   818,  2081,   820,  2074,   822,   823,   824,   825,   826,
     105,   108,  2075,  2076,  2077,  2078,  2082,  2098,  2083,  -699,
    -699,  -699,  2093,  -699,  -699,  -699,  -699,  -699,  2095,  2097,
    2099,  2102,  -699,  -699,  -699,  -699,  -699,  -699,  2103,  2104,
    -699,  -699,  -699,  -699,  -699,  2112,   109,   110,   111,   112,
     113,   114,   115,   116,   117,   118,   119,   120,   121,   122,
     123,   124,   125,   126,   127,   128,   129,   130,   131,   132,
     133,  2105,  2108,  2109,  2113,   106,  2114,  2115,  2116,  2119,
    2117,  2118,  2121,  2122,  -697,  -697,  -697,    56,  -697,  -697,
    -697,  -697,  -697,  2120,    57,    58,    59,  -697,  -697,  -697,
    -697,  -697,  -697,   325,   328,  -697,  -697,  -697,  -697,  -697,
      65,     0,     0,     0,   829,     0,     0,   832,   833,   834,
     835,   836,   837,   838,   839,     0,     0,     0,     0,   844,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     854,     0,   107,     0,     0,     0,     0,   861,   862,     0,
     865,  -698,  -698,  -698,     0,  -698,  -698,  -698,  -698,  -698,
       0,   786,     0,   330,  -698,  -698,  -698,  -698,  -698,  -698,
       0,     0,  -698,  -698,  -698,  -698,  -698,     0,     0,     2,
       3,     0,     4,     0,     0,     0,     0,   330,   330,     5,
       6,     7,   330,     8,     9,    10,    11,    12,    13,     0,
       0,     0,    14,    15,    16,    17,    18,    19,     0,     0,
      20,    21,    22,    23,    24,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   330,     0,     0,
      25,     0,   330,     0,   786,   786,     0,     0,     0,     0,
       0,     0,     0,   330,   330,     0,     0,     0,     0,     0,
       0,     0,   330,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
    -699,  -699,  -699,  -699,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   786,   786,  -699,  -699,  -699,
    -699,     0,   786,     0,   786,   786,     0,     0,     0,   786,
    1374,   786,     0,     0,     0,     0,     0,   786,     0,     0,
     786,     0,     0,     0,     0,  -699,  -699,     0,     0,   997,
       0,     0,     0,     0,  1397,  1398,     0,     0,     0,  1402,
       0,   786,   786,     0,   330,  -697,  -697,  -697,  -697,     0,
     330,   330,     0,   330,     0,   786,     0,     0,     0,     0,
       0,     0,  -697,  -697,  -697,  -697,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,  1451,     0,     0,     0,     0,  1456,
    -697,  -697,     0,     0,     0,     0,     0,     0,  -699,     0,
    1468,  1469,     0,     0,     0,     0,     0,     0,     0,  1477,
       0,     0,  -698,  -698,  -698,  -698,     0,     0,     0,     0,
       0,     0,     0,     0,     0,  1192,     0,     0,     0,  -698,
    -698,  -698,  -698,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      26,    27,    28,    29,     0,     0,     0,  -698,  -698,     0,
       0,     0,     0,  -697,     0,     0,   330,    30,    31,    32,
      33,     0,     0,     0,     0,    34,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    35,     0,     0,    36,    37,
     330,  1563,     0,     0,     0,    38,    39,  1569,  1570,     0,
    1572,     0,     0,     0,     0,   330,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
    -698,     0,     0,   472,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   330,     0,     0,     0,
       0,     0,    40,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   311,     0,     0,   311,     0,     0,
     -14,   103,     0,   -14,     0,     0,     0,     0,   330,     0,
     -14,   -14,   -14,  1672,   -14,   -14,   -14,   -14,   -14,   -14,
       0,     0,     0,   -14,   -14,   -14,   -14,   -14,   -14,     0,
       0,   -14,   -14,   -14,   -14,   -14,     0,  1696,     0,     0,
       0,     5,     6,     7,     0,     8,     9,    10,    11,    12,
       0,     0,  1711,     0,    14,    15,    16,    17,    18,    19,
    1404,   -14,    20,    21,    22,    23,    24,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,  1764,     0,     0,     0,  1455,     0,     0,
       0,     0,     0,  1462,  1463,  1464,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,  1476,     0,     0,
       0,     0,  1481,  1482,  1483,  1484,  1485,  1486,     0,  1488,
    1489,  1490,     0,     0,     0,     0,  1495,     0,     0,     0,
       0,     0,     0,  1502,     0,  1804,     0,     0,     0,  1509,
       0,     0,     0,  1513,  1514,  1515,     0,     0,     0,  1520,
       0,     0,  1523,  1524,  1525,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   472,
       0,     0,     0,   472,     0,     0,     0,     0,     0,     0,
       0,   786,     0,     0,     0,  1558,  1559,   311,     0,  1562,
       0,  1564,  1565,  1566,  1567,  1568,     0,     0,  1571,     0,
    1573,     0,  1575,  1576,  1577,  1578,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   330,   330,     0,     0,     0,   330,     0,   330,
     786,     0,     0,     0,   786,   786,     0,     0,     0,     0,
       0,  1940,     0,     0,  1606,  1607,     0,     0,     0,   330,
       0,   -14,   -14,   -14,   -14,     0,     0,     0,     0,     0,
       0,   330,     0,     0,     0,   472,     0,     0,   -14,   -14,
     -14,   -14,     0,     0,     0,     0,   -14,     0,     0,     0,
       0,     0,    26,    27,    28,    29,   -14,     0,     0,   -14,
     -14,     0,     0,     0,     0,     0,   -14,   -14,     0,    30,
      31,    32,    33,     0,     0,     0,     0,     0,     0,     0,
       0,  1671,     0,     0,     0,     0,     0,     0,     0,     0,
    1680,     0,     0,  1683,  1684,     0,     0,    38,    39,     0,
       0,     0,  1692,     0,   330,   330,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,  1713,     0,  1715,     0,     0,     0,     0,
       0,  1721,  1722,   -14,  1724,     0,     0,     0,     0,  1919,
    1920,     0,     0,     0,  1924,     0,  1926,   330,     0,     0,
     786,   786,     0,     0,   786,     0,     0,     0,     0,  1940,
       0,     0,     0,     0,     0,     0,  1950,     0,     0,     0,
     327,     0,     0,     0,     0,     0,     0,     0,  1957,     0,
       0,     0,   -18,   104,     0,   -18,     0,     0,     0,     0,
       0,     0,   -18,   -18,   -18,     0,   -18,   -18,   -18,   -18,
     -18,   -18,   786,     0,     0,   -18,   -18,   -18,   -18,   -18,
     -18,     0,     0,   -18,   -18,   -18,   -18,   -18,     0,     0,
       0,   330,     0,     0,   786,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   -18,     0,     0,     0,   330,   330,     0,
       0,  2022,  2023,     0,     0,     5,     6,     7,     0,     8,
       9,    10,    11,    12,     0,     0,     0,     0,    14,    15,
      16,    17,    18,    19,     0,     0,    20,    21,    22,    23,
      24,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,  2042,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,  2091,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,  2110,  2111,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,  1881,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   -18,   -18,   -18,   -18,  1918,     0,     0,
    1921,  1922,  1923,     0,     0,     0,     0,  1928,  1929,  1930,
     -18,   -18,   -18,   -18,     0,     0,     0,     0,   -18,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   -18,     0,
       0,   -18,   -18,     0,     0,     0,     0,     0,   -18,   -18,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
    1006,     0,     0,     0,     0,     0,    26,    27,    28,    29,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,  1007,    30,    31,    32,    33,     0,     0,     0,
       0,     0,     0,  1008,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    38,    39,     0,     0,   -18,  2018,  2019,     0,     0,
       0,     0,     0,     0,  1009,  1010,  1011,  1012,  1013,  1014,
    1015,  1016,  1017,  1018,  1019,  1020,  1021,  1022,     0,     0,
       0,     0,     0,     0,  1023,  1024,  1025,  1026,  1027,     0,
       0,     0,  1028,     0,  1029,  1030,  1031,  1032,  1033,     0,
       0,  2040,     0,     0,     0,     0,     0,     0,  2047,  2048,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,  2080,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,  2090,     0,     0,     0,
       0,     0,  2096,  1034,  1035,  1036,  1037,  1038,  1039,  1040,
       0,  1041,  1042,  1043,  1044,  1045,  1046,  1047,  1048,  1049,
    1050,  1051,  1052,  1053,  1054,     0,  1055,  1056,  1057,  1058,
    1059,  1060,  1061,  1062,  1063,  1064,  1065,  1066,  1067,  1068,
    1069,  1070,  1071,  1072,  1073,  1074,  1075,  1076,  1077,  1078,
    1079,  1080,     0,  1081,  1082,  1083,  1084,  1085,  1086,  1087,
    1088,  1089,  1090,  1091,  1092,  1093,  1094,  1095,  1096,  1097,
    1098,     0,  1099,  1100,  1101,  1102,  1103,  1104,  1105,  1106,
    1107,  1108,  1109,     0,  1110,  1111,     0,  1112,  1113,  1114,
       0,     0,     0,     0,     0,     0,     0,  1006,     0,     0,
       0,     0,     0,  1115,  1116,  1117,     0,     0,     0,  1118,
       0,  1119,  1120,  1121,  1122,  1123,  1124,     0,     0,  1007,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
    1008,     0,  1125,  1126,  1127,     0,     0,  1128,  1129,  1130,
    1131,  1132,  1133,  1134,  1135,  1136,  1137,  1138,  1139,  1140,
    1141,  1142,  1143,  1144,  1145,  1146,  1147,  1148,  1149,  1150,
    1151,  1009,  1010,  1011,  1012,  1013,  1014,  1015,  1016,  1017,
    1018,  1019,  1020,  1021,  1022,     0,     0,     0,     0,     0,
       0,  1023,  1024,  1025,  1026,  1027,     0,   -17,   105,  1028,
     -17,  1029,  1030,  1031,  1032,  1033,     0,   -17,   -17,   -17,
       0,   -17,   -17,   -17,   -17,   -17,   -17,     0,  1152,     0,
     -17,   -17,   -17,   -17,   -17,   -17,     0,     0,   -17,   -17,
     -17,   -17,   -17,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   -15,   106,     0,   -15,     0,     0,     0,   -17,     0,
       0,   -15,   -15,   -15,     0,   -15,   -15,   -15,   -15,   -15,
     -15,     0,     0,     0,   -15,   -15,   -15,   -15,   -15,   -15,
       0,     0,   -15,   -15,   -15,   -15,   -15,     0,     0,     0,
    1034,  1035,  1036,  1037,  1038,  1039,  1040,     0,  1041,  1042,
    1043,  1044,  1045,  1046,  1047,  1048,  1049,  1050,  1051,  1052,
    1053,  1054,   -15,  1055,  1056,  1057,  1058,  1059,  1060,  1061,
    1062,  1063,  1064,  1065,  1066,  1067,  1068,  1069,  1070,  1071,
    1072,  1073,  1074,  1075,  1076,  1077,  1078,  1079,  1080,     0,
    1081,  1082,  1083,  1084,  1085,  1086,  1087,  1088,  1089,  1090,
    1091,  1092,  1093,  1094,  1095,  1096,  1097,  1098,     0,  1099,
    1100,  1101,  1102,  1103,  1104,  1105,  1106,  1107,  1108,  1109,
       0,  1110,  1111,     0,  1112,  1113,  1114,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
    1115,  1116,  1117,     0,     0,     0,  1118,     0,  1119,  1120,
    1121,  1122,  1123,  1124,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,  1125,
    1126,  1127,     0,     0,  1128,  1129,  1130,  1131,  1132,  1133,
    1134,  1135,  1136,  1137,  1138,  1139,  1140,  1141,  1142,  1143,
    1144,  1145,  1146,  1147,  1148,  1149,  1150,  1151,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   -17,   -17,
     -17,   -17,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   -17,   -17,   -17,   -17,     0,
       0,     0,     0,   -17,     0,  1196,     0,     0,     0,     0,
       0,     0,     0,   -17,     0,     0,   -17,   -17,     0,     0,
       0,     0,     0,   -17,   -17,     0,     0,     0,     0,     0,
       0,     0,   -15,   -15,   -15,   -15,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   -15,
     -15,   -15,   -15,     0,     0,     0,     0,   -15,     0,   188,
     189,   306,   307,     0,     0,     0,     0,   -15,     0,     0,
     -15,   -15,     0,     0,     0,     0,     0,   -15,   -15,     0,
       0,   -16,   107,     0,   -16,     0,     0,     0,     0,     0,
     -17,   -16,   -16,   -16,     0,   -16,   -16,   -16,   -16,   -16,
     -16,     0,     0,     0,   -16,   -16,   -16,   -16,   -16,   -16,
       0,     0,   -16,   -16,   -16,   -16,   -16,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   -16,     0,   -15,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   545,   546,   547,   548,   549,
     550,   551,   552,   553,   554,   555,   556,   557,   558,   559,
     560,   561,   562,   563,   564,   565,   566,   567,   568,   569,
     570,   571,   572,   573,   574,   575,   576,   577,   578,   579,
     580,   581,   582,   583,   584,   585,   586,   587,   588,   589,
     590,   591,   592,   593,   594,   595,   596,   597,   598,   599,
     600,   601,   602,   603,   604,   605,   606,   607,   608,   609,
     610,   611,   612,   613,   545,   546,   547,   548,   549,   550,
     551,   552,   553,   554,   555,   556,   557,   558,   559,   560,
     561,   562,   563,   564,   565,   566,   567,   568,   569,   570,
     571,   572,   573,   574,   575,   576,   577,   578,   579,   580,
     581,   582,   583,   584,   585,   586,   587,   588,   589,   590,
     591,   592,   593,   594,   595,   596,   597,   598,   599,   600,
     601,   602,   603,   604,   605,   606,   607,   608,   609,   610,
     611,   612,   613,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   -16,   -16,   -16,   -16,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   -16,
     -16,   -16,   -16,     0,     0,     0,     0,   -16,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   -16,     0,     0,
     -16,   -16,     0,     0,     0,     0,     0,   -16,   -16,   614,
     615,   616,   617,   618,   619,   620,   621,   622,   623,   624,
     625,   626,   627,   628,   629,   630,   631,   632,   633,   634,
     635,   636,   637,   638,   639,   640,   641,   642,   643,   644,
     645,     0,   308,     0,     0,     0,     0,     0,     0,     0,
     309,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   -16,     0,     0,     0,   614,   615,
     616,   617,   618,   619,   620,   621,   622,   623,   624,   625,
     626,   627,   628,   629,   630,   631,   632,   633,   634,   635,
     636,   637,   638,   639,   640,   641,   642,   643,   644,   645,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   310,   545,   546,   547,   548,   549,   550,   551,
     552,   553,   554,   555,   556,   557,   558,   559,   560,   561,
     562,   563,   564,   565,   566,   567,   568,   569,   570,   571,
     572,   573,   574,   575,   576,   577,   578,   579,   580,   581,
     582,   583,   584,   585,   586,   587,   588,   589,   590,   591,
     592,   593,   594,   595,   596,   597,   598,   599,   600,   601,
     602,   603,   604,   605,   606,   607,   608,   609,   610,   611,
     612,   613,   545,   546,   547,   548,   549,   550,   551,   552,
     553,   554,   555,   556,   557,   558,   559,   560,   561,   562,
     563,   564,   565,   566,   567,   568,   569,   570,   571,   572,
     573,   574,   575,   576,   577,   578,   579,   580,   581,   582,
     583,   584,   585,   586,   587,   588,   589,   590,   591,   592,
     593,   594,   595,   596,   597,   598,   599,   600,   601,   602,
     603,   604,   605,   606,   607,   608,   609,   610,   611,   612,
     613,   545,   546,   547,   548,   549,   550,   551,   552,   553,
     554,   555,   556,   557,   558,   559,   560,   561,   562,   563,
     564,   565,   566,   567,   568,   569,   570,   571,   572,   573,
     574,   575,   576,   577,   578,   579,   580,   581,   582,   583,
     584,   585,   586,   587,   588,   589,   590,   591,   592,   593,
     594,   595,   596,   597,   598,   599,   600,   601,   602,   603,
     604,   605,   606,   607,   608,   609,   610,   611,   612,   613,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   614,   615,   616,
     617,   618,   619,   620,   621,   622,   623,   624,   625,   626,
     627,   628,   629,   630,   631,   632,   633,   634,   635,   636,
     637,   638,   639,   640,   641,   642,   643,   644,   645,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,  1508,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   614,   615,   616,   617,
     618,   619,   620,   621,   622,   623,   624,   625,   626,   627,
     628,   629,   630,   631,   632,   633,   634,   635,   636,   637,
     638,   639,   640,   641,   642,   643,   644,   645,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
    1519,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   614,   615,   616,   617,   618,
     619,   620,   621,   622,   623,   624,   625,   626,   627,   628,
     629,   630,   631,   632,   633,   634,   635,   636,   637,   638,
     639,   640,   641,   642,   643,   644,   645
};

static const yytype_int16 yycheck[] =
{
     153,   475,  1327,   245,     1,     1,     1,  1170,     1,  1181,
       1,   253,   254,     1,     1,  1178,  1179,    22,    22,    63,
       5,   922,    22,     1,    22,    19,    20,   287,   376,    22,
       1,    19,    20,     1,     1,    62,  1859,    22,     1,     1,
    1202,     1,  1204,   391,  1206,     1,  1208,   224,     1,    22,
     283,     1,    19,    20,     1,   278,     1,     1,   281,     1,
    1222,     1,  1224,   283,  1226,   376,  1228,    22,  1230,   477,
     478,     1,   973,   376,     7,     4,     5,   978,     7,   321,
     388,   323,   393,   325,   209,   376,    73,     1,   391,   242,
       4,     5,     6,     7,     6,     7,   997,    22,   242,     1,
     391,     9,     7,    22,     6,     7,   283,     1,    22,    22,
     376,   502,   503,   504,    22,   506,   311,   312,     1,     5,
      22,     9,   275,   383,    22,   391,   509,    29,   253,   254,
     513,     1,   515,   516,    22,     1,    22,   520,     4,     5,
       6,     7,   525,   526,     1,   288,   289,     4,     5,     6,
       7,     4,     5,     6,     7,   388,   539,     4,     5,     6,
       7,   314,  1324,   376,    22,    22,   389,   376,   388,  1331,
     314,  1333,   385,  1335,     1,  1337,   385,     4,     5,     6,
       7,    22,   226,   376,   242,   383,   244,     4,     5,     6,
       7,     1,   390,   376,   389,    22,     6,     7,   391,  1371,
     325,  1373,   385,  1366,   391,    30,   169,   170,   171,   172,
     173,    36,    22,   208,   383,   210,   211,   275,   387,   214,
     215,   216,   217,   218,   219,   220,   186,   271,   223,   224,
     225,   226,   227,   228,   229,   230,   231,   232,  2061,   234,
     235,    34,     4,     5,   388,   487,    31,    32,    33,    34,
     308,   309,    31,    32,    33,    34,   314,   648,   316,   317,
     651,     4,     5,     6,     7,   376,   387,   261,   389,   388,
     661,   662,   663,   261,   388,   666,   667,   668,   669,    22,
     391,   672,   673,   674,   288,   289,   677,    22,   384,   680,
     681,   385,     1,   684,   261,   678,   679,   688,   689,   690,
     691,    10,    11,    12,   388,    14,    15,    16,    17,    18,
     180,   181,   182,   183,    23,    24,    25,    26,    27,    28,
     376,   388,    31,    32,    33,    34,    35,     4,     5,     6,
       7,   388,     9,   383,   376,   391,   744,   376,    73,    22,
      31,    32,    33,    34,    31,    32,    33,    29,    30,   391,
     275,   276,   391,   376,    36,   508,   509,   377,   378,   379,
     513,   505,   515,   516,  1536,    47,   383,   520,   391,   377,
     378,   379,   525,   526,   394,   528,  1548,   383,   386,   376,
     376,   534,   535,   388,   537,   376,   539,   383,   384,    21,
     388,    21,   383,   384,   389,   391,    21,   393,   376,   392,
     391,   389,   389,   388,   462,   463,   464,   465,   466,   467,
     468,   469,   470,   471,   376,   383,   387,   475,   389,   387,
     376,   383,   389,   481,   482,   483,   389,   383,    34,   391,
     383,   376,   376,   388,   387,   391,   910,   387,   383,   389,
     387,   383,   389,   383,   377,   387,   376,   387,   377,   388,
     508,   509,   385,   383,   845,   513,   385,   515,   516,   388,
     388,   394,   520,   377,   389,   377,   283,   525,   526,   388,
     528,   385,   377,   385,   388,   377,   534,   535,    23,   537,
     385,   539,   376,   385,   376,   387,   387,   389,   389,   383,
     388,   874,   875,   376,    31,    32,    33,   650,    43,   391,
     383,  1402,  1403,   288,   289,   377,   378,   379,  1409,   288,
     289,   377,   376,    21,   307,   308,   660,   389,   671,   385,
     377,   665,   388,   676,   377,   678,   679,   391,   385,     1,
     377,   388,   385,   686,   687,   388,    21,   376,   385,    84,
     376,   685,   277,   278,   279,   280,   281,   221,   222,    22,
     377,    23,   391,     1,   387,   391,   389,   701,   385,    21,
     377,   388,   271,   272,   273,   274,   388,   377,   385,   446,
     447,   388,   377,   378,   379,   385,   377,   378,   379,   288,
     289,   290,   291,   377,   378,   379,   386,   387,     1,   394,
     388,    63,   650,   497,   498,   389,   389,   288,   289,   752,
      73,   288,   289,   379,   389,   386,   750,   316,   317,    47,
     389,   388,   393,   671,   395,    22,   379,   380,   676,   382,
     678,   679,   388,   377,   378,   379,   376,  1790,   686,   687,
     783,  1794,   388,   388,   377,   389,   388,   388,   377,   378,
     379,   380,   385,   382,    22,   388,    22,   386,   385,   802,
       6,     7,   379,   380,   389,   382,  1818,  1819,  1820,  1821,
    1822,   377,   378,   379,    22,   388,    73,   383,    24,  1831,
    1832,  1833,  1834,  1835,   388,   388,    32,    33,    34,    35,
     389,    37,    38,    39,    71,    72,    73,   840,   746,   388,
      77,    78,  1864,   299,   300,   301,   302,   303,   304,   305,
     377,    22,  2027,   388,  1866,  1867,  1868,  1869,   385,   388,
     377,   378,   379,   380,   186,   382,   376,  1618,   383,   386,
     376,   874,   875,  1886,   388,   783,   174,   175,   176,   177,
     178,   179,    22,   388,   377,   378,   379,   380,   210,   382,
     383,    89,    90,    91,   802,    21,   388,  1203,   892,  1205,
     298,  1207,   383,  1209,   226,   383,   900,   901,   902,   383,
    1168,   383,   377,   378,   379,   376,   376,  1223,   383,  1225,
    1671,  1227,   376,  1229,   376,  1231,   376,   376,  1186,  1162,
     383,   376,   840,   389,  1192,   377,   378,   379,   377,   378,
     379,   383,   376,   376,   383,  1696,   377,   378,   379,   271,
    1701,   273,   383,  1194,   277,   278,   279,   280,   281,   376,
    1711,   383,   377,   378,   379,   376,   874,   875,   383,   376,
     376,   376,   376,   236,   237,   238,   239,   240,   241,   242,
     243,   393,   245,   246,   247,   248,   249,   376,   376,   897,
     253,   254,   255,   256,   257,   258,   259,   260,  1239,  1240,
    1241,   376,   910,  1244,   376,  1311,  1247,  1248,   376,   376,
    1251,   376,  1245,  1246,   376,  1256,  1257,  1258,  1259,  1260,
     277,   278,   279,   280,   281,  1266,  1332,   376,  1334,   376,
    1336,   376,  1338,    83,   377,   378,   379,  1278,  1279,  1280,
     383,   393,  1283,  1284,  1285,  1286,  1287,   376,  1289,  1290,
    1291,   376,  1293,  1804,  1805,   388,   377,   378,   379,   377,
     378,   379,   383,  1296,  1297,   383,   389,   376,   376,   376,
    1303,   376,  1305,  1306,   376,   376,   376,  1310,   388,  1312,
     376,  1322,   376,   376,   376,  1318,   136,   137,  1321,   376,
     376,   376,   376,   376,   144,   145,   146,   147,   148,   149,
     377,   378,   379,   380,   376,   382,   376,   383,   376,  1342,
    1343,   376,   162,   163,   164,   165,   376,   376,   376,   169,
     376,   376,  1380,  1356,   376,   376,   389,   376,   376,   376,
    1388,  1389,   389,   376,  1392,   377,   378,   379,   380,  1397,
     382,   191,   192,   193,   194,   376,  1404,   376,   383,   376,
     383,   376,   376,   376,   376,   376,   376,   376,   376,  1162,
     376,  1164,   376,   376,   376,  1159,   376,   376,  1162,  1163,
     385,   376,   376,  1924,  1925,   376,   376,   376,   385,     1,
     376,   376,   376,   376,   376,  1188,  1189,   376,   376,     1,
    1193,   376,   376,   376,   376,   386,   386,   383,    10,    11,
      12,   382,    14,    15,    16,    17,    18,   382,     1,   388,
    1534,    23,    24,    25,    26,    27,    28,   376,   388,    31,
      32,    33,    34,    35,   376,   376,     8,     9,    10,    11,
      12,   376,   376,   376,   376,  1238,   286,   376,   376,   376,
    1243,   376,  1245,  1246,    26,    27,    28,    29,   376,   376,
      22,  1254,  1255,   376,  1162,  1496,  1164,   379,  1252,  1253,
    1263,   394,   383,   383,   383,   383,   383,  1261,   383,    51,
     383,   383,   383,  1181,   383,   383,   383,   383,   383,  1273,
    1188,  1189,   383,  1277,   383,  1193,   383,  1379,  1282,   383,
      72,  2042,  2043,  1296,  1297,  1387,   383,   383,  1292,   383,
    1303,   383,  1305,  1306,   383,   383,   383,  1310,   383,  1312,
     383,   394,   383,   383,   383,  1318,   383,   383,  1321,   383,
     383,   383,   383,  1317,  1582,  1319,   383,   383,   383,  1587,
    1238,  1589,  1326,  1327,   383,  1243,  1330,  1245,  1246,  1342,
    1343,   383,  1345,   383,   383,   383,  1254,  1255,  1351,  1352,
     387,  1354,   383,  1356,   383,  1263,   383,   383,   383,   383,
    1618,   383,   184,   185,   186,   187,   188,   189,   190,  1610,
     192,   193,   194,   195,   196,   197,   198,   199,   383,   201,
     202,   203,   204,   205,   383,   207,   383,   383,  1296,  1297,
     440,   383,   383,   383,   383,  1303,   383,  1305,  1306,   383,
     383,   383,  1310,   383,  1312,   383,   383,   383,   383,   383,
    1318,   383,   383,  1321,   383,   383,   383,  1325,   383,   383,
     383,  1329,   383,   383,   383,   383,   383,   383,   383,   383,
     376,   376,   376,   376,  1342,  1343,   376,  1345,   376,   376,
     376,   376,   376,  1351,  1352,    22,  1354,    22,  1356,    22,
      22,   242,    22,    22,    22,    22,   282,     9,     9,   271,
     272,   273,   274,  1371,  1467,  1373,   383,   387,   387,   376,
     376,   383,   385,   376,   385,   385,   288,   289,   290,   291,
     385,  1739,   383,  1741,   275,   385,   385,   385,  1491,   385,
    1748,   385,   385,   385,   385,   385,   376,  1755,   376,   376,
     376,   376,   376,  1506,   316,   317,   376,   376,   376,   376,
     376,   376,   376,   376,   376,   376,   376,   308,   309,   376,
     376,  1613,   376,   314,   376,   376,   376,   320,   321,   322,
     323,   324,   325,   326,   327,   328,   329,   330,   331,   332,
     333,   334,   335,   336,   337,   338,   339,   340,   341,   376,
     385,   376,   376,   376,  1557,   376,   376,   376,   376,  1467,
     376,   376,   376,   376,   393,   376,   376,   389,   376,  1802,
     376,   376,   376,   376,   376,   376,   376,   389,   376,   376,
     376,   376,   376,  1491,   376,     9,   376,   376,   376,   376,
     376,   376,  1595,   376,   393,  1598,   389,   376,  1506,   376,
     376,  1595,   376,   376,  1598,   376,  1609,   376,  1849,   376,
     376,   376,   376,   376,   376,   376,   376,   376,  1851,  1877,
     376,  1879,  1855,  1856,   376,   376,  1534,   376,  1536,   376,
     385,   376,   376,   376,   376,  1543,   376,   376,   376,   376,
    1548,   376,  1883,   376,   376,   376,   376,   376,   376,  1557,
     376,   376,   376,   376,   376,   376,   376,   376,   376,   376,
    1918,  1919,  1920,   376,   376,  1923,   376,   376,  1926,   376,
      22,   462,   463,   464,   465,   466,   467,   468,   469,   470,
     471,   383,     9,    22,   475,    22,  1944,  1595,   282,   386,
    1598,   383,   383,   383,  1602,   383,   383,   386,   383,  1957,
    1958,  1609,   383,  1961,   383,   383,  1614,   389,   388,    22,
      22,   388,     9,    22,    22,    22,    22,   508,   509,    22,
      22,    22,   513,    22,   515,   516,    22,    22,    22,   520,
      22,    22,    22,    22,   525,   526,    22,   528,    22,   388,
     388,   388,   388,   534,   535,   388,   537,   264,   539,    22,
      22,   267,    22,   388,    22,   388,   383,  1751,  1999,   383,
     383,   383,   383,   383,   383,   383,   383,   383,  2001,  2002,
     383,    22,  2005,   383,   389,   383,   383,   386,   392,   383,
     392,   383,   383,   383,    22,   383,   392,  1790,   383,   392,
     386,  1794,    22,   392,   386,   386,  1790,   386,   383,  1802,
    1794,   383,   386,   386,   392,  1808,   386,   386,   386,   383,
      22,   392,   383,   267,  1808,    22,   392,   383,   383,   376,
    2053,   383,   376,   383,   383,   386,  2084,  2085,   392,   392,
     386,   386,  2073,  2091,  2092,   383,   386,   386,   383,   383,
    1843,  1844,  2075,   383,   383,  1848,   383,  1850,  1851,   383,
     383,   383,  1855,  1856,   383,   383,   383,   383,   383,   650,
     383,   383,   383,   383,   383,  1859,    22,  1870,   383,   383,
     383,   383,   383,   383,   383,   383,   383,   383,   383,  1882,
     671,   383,  1790,  1886,   528,   676,  1794,   678,   679,   383,
     534,   535,  1886,   537,  1802,   686,   687,   383,   383,   383,
    1808,   383,   383,   383,   383,   383,   383,   383,   383,   383,
     383,   383,   383,   383,   383,   383,  1824,   383,   383,   383,
     383,   383,  1830,   383,   383,   383,   383,   383,    22,  1837,
     383,   386,  1840,   383,   383,  1843,  1844,   383,   383,   383,
    1848,   383,  1850,  1851,   387,   383,   394,  1855,  1856,   383,
     383,   392,  1955,  1956,  1862,   746,  1864,   392,   383,   376,
     392,   383,  1870,   392,   383,   383,   394,   394,   383,   383,
     383,   383,   383,   383,  1882,   383,   383,   383,  1886,   383,
     383,   383,   383,   383,   383,   383,   383,   383,   383,   383,
     383,   383,   783,   383,   383,  1998,   383,   376,  2001,  2002,
     383,   387,  2005,   387,   376,   376,   650,   376,   376,   376,
     383,   802,   376,   383,   376,   376,   376,   383,   376,   376,
     376,   376,   376,   376,   387,   376,   376,   671,   376,    22,
      22,   376,   676,  2027,    22,    22,   376,    22,   387,   383,
     387,   387,   686,   687,   387,   387,   387,  1955,  1956,   840,
    2053,   387,   387,   387,   387,   376,  1964,   376,  2061,  1967,
     376,   376,   376,   387,   386,   376,   383,  2061,   386,  2072,
      22,    22,  2075,   383,    22,  1411,  2061,  2010,   383,   383,
     219,     1,     1,   874,   875,   383,   386,     1,   872,     1,
    1998,   376,     1,  2001,  2002,  2098,  2099,  2005,   386,   386,
     383,   383,  2010,   383,   233,   386,   383,     1,   235,   388,
     388,   387,   387,   376,   383,   386,   376,   376,   376,   910,
     387,   383,   383,   389,   389,  2033,  2034,  2035,   383,   383,
     383,   383,   376,   383,   383,   383,   383,   383,   383,   783,
     383,   383,   383,   383,   383,  2053,     1,     1,     1,   383,
     387,   387,   387,  2061,   387,   387,   383,   383,   802,   383,
     383,   383,   383,   383,  2072,     1,   495,  2075,   387,   387,
     387,   387,   383,   383,   383,   383,   501,   383,   383,   383,
     383,   383,   507,   383,     1,   510,   511,   512,   387,   514,
    2098,  2099,   517,   518,   519,   389,   840,   389,   523,   524,
     383,   383,   527,   383,   529,   530,   531,   532,   533,   383,
     387,   536,   383,   538,   389,   540,   541,   542,   543,   544,
       1,     1,   387,   389,   387,   389,   383,   387,   383,    10,
      11,    12,   383,    14,    15,    16,    17,    18,   383,   383,
     387,   383,    23,    24,    25,    26,    27,    28,   383,   383,
      31,    32,    33,    34,    35,   383,    36,    37,    38,    39,
      40,    41,    42,    43,    44,    45,    46,    47,    48,    49,
      50,    51,    52,    53,    54,    55,    56,    57,    58,    59,
      60,   389,   389,   389,   383,     1,   383,   383,   383,   389,
     383,   383,   383,   383,    10,    11,    12,     1,    14,    15,
      16,    17,    18,   389,     1,     1,     1,    23,    24,    25,
      26,    27,    28,   254,   273,    31,    32,    33,    34,    35,
       1,    -1,    -1,    -1,   649,    -1,    -1,   652,   653,   654,
     655,   656,   657,   658,   659,    -1,    -1,    -1,    -1,   664,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     675,    -1,     1,    -1,    -1,    -1,    -1,   682,   683,    -1,
     685,    10,    11,    12,    -1,    14,    15,    16,    17,    18,
      -1,  1162,    -1,  1164,    23,    24,    25,    26,    27,    28,
      -1,    -1,    31,    32,    33,    34,    35,    -1,    -1,     0,
       1,    -1,     3,    -1,    -1,    -1,    -1,  1188,  1189,    10,
      11,    12,  1193,    14,    15,    16,    17,    18,    19,    -1,
      -1,    -1,    23,    24,    25,    26,    27,    28,    -1,    -1,
      31,    32,    33,    34,    35,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,  1238,    -1,    -1,
      61,    -1,  1243,    -1,  1245,  1246,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,  1254,  1255,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,  1263,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     271,   272,   273,   274,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,  1296,  1297,   288,   289,   290,
     291,    -1,  1303,    -1,  1305,  1306,    -1,    -1,    -1,  1310,
    1164,  1312,    -1,    -1,    -1,    -1,    -1,  1318,    -1,    -1,
    1321,    -1,    -1,    -1,    -1,   316,   317,    -1,    -1,   864,
      -1,    -1,    -1,    -1,  1188,  1189,    -1,    -1,    -1,  1193,
      -1,  1342,  1343,    -1,  1345,   271,   272,   273,   274,    -1,
    1351,  1352,    -1,  1354,    -1,  1356,    -1,    -1,    -1,    -1,
      -1,    -1,   288,   289,   290,   291,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,  1238,    -1,    -1,    -1,    -1,  1243,
     316,   317,    -1,    -1,    -1,    -1,    -1,    -1,   389,    -1,
    1254,  1255,    -1,    -1,    -1,    -1,    -1,    -1,    -1,  1263,
      -1,    -1,   271,   272,   273,   274,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   960,    -1,    -1,    -1,   288,
     289,   290,   291,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     271,   272,   273,   274,    -1,    -1,    -1,   316,   317,    -1,
      -1,    -1,    -1,   389,    -1,    -1,  1467,   288,   289,   290,
     291,    -1,    -1,    -1,    -1,   296,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   306,    -1,    -1,   309,   310,
    1491,  1345,    -1,    -1,    -1,   316,   317,  1351,  1352,    -1,
    1354,    -1,    -1,    -1,    -1,  1506,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     389,    -1,    -1,  1534,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,  1557,    -1,    -1,    -1,
      -1,    -1,   383,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,  1595,    -1,    -1,  1598,    -1,    -1,
       0,     1,    -1,     3,    -1,    -1,    -1,    -1,  1609,    -1,
      10,    11,    12,  1467,    14,    15,    16,    17,    18,    19,
      -1,    -1,    -1,    23,    24,    25,    26,    27,    28,    -1,
      -1,    31,    32,    33,    34,    35,    -1,  1491,    -1,    -1,
      -1,    10,    11,    12,    -1,    14,    15,    16,    17,    18,
      -1,    -1,  1506,    -1,    23,    24,    25,    26,    27,    28,
    1195,    61,    31,    32,    33,    34,    35,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,  1557,    -1,    -1,    -1,  1242,    -1,    -1,
      -1,    -1,    -1,  1248,  1249,  1250,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,  1262,    -1,    -1,
      -1,    -1,  1267,  1268,  1269,  1270,  1271,  1272,    -1,  1274,
    1275,  1276,    -1,    -1,    -1,    -1,  1281,    -1,    -1,    -1,
      -1,    -1,    -1,  1288,    -1,  1609,    -1,    -1,    -1,  1294,
      -1,    -1,    -1,  1298,  1299,  1300,    -1,    -1,    -1,  1304,
      -1,    -1,  1307,  1308,  1309,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,  1790,
      -1,    -1,    -1,  1794,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,  1802,    -1,    -1,    -1,  1340,  1341,  1808,    -1,  1344,
      -1,  1346,  1347,  1348,  1349,  1350,    -1,    -1,  1353,    -1,
    1355,    -1,  1357,  1358,  1359,  1360,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,  1843,  1844,    -1,    -1,    -1,  1848,    -1,  1850,
    1851,    -1,    -1,    -1,  1855,  1856,    -1,    -1,    -1,    -1,
      -1,  1862,    -1,    -1,  1399,  1400,    -1,    -1,    -1,  1870,
      -1,   271,   272,   273,   274,    -1,    -1,    -1,    -1,    -1,
      -1,  1882,    -1,    -1,    -1,  1886,    -1,    -1,   288,   289,
     290,   291,    -1,    -1,    -1,    -1,   296,    -1,    -1,    -1,
      -1,    -1,   271,   272,   273,   274,   306,    -1,    -1,   309,
     310,    -1,    -1,    -1,    -1,    -1,   316,   317,    -1,   288,
     289,   290,   291,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,  1466,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
    1475,    -1,    -1,  1478,  1479,    -1,    -1,   316,   317,    -1,
      -1,    -1,  1487,    -1,  1955,  1956,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,  1508,    -1,  1510,    -1,    -1,    -1,    -1,
      -1,  1516,  1517,   383,  1519,    -1,    -1,    -1,    -1,  1843,
    1844,    -1,    -1,    -1,  1848,    -1,  1850,  1998,    -1,    -1,
    2001,  2002,    -1,    -1,  2005,    -1,    -1,    -1,    -1,  2010,
      -1,    -1,    -1,    -1,    -1,    -1,  1870,    -1,    -1,    -1,
     389,    -1,    -1,    -1,    -1,    -1,    -1,    -1,  1882,    -1,
      -1,    -1,     0,     1,    -1,     3,    -1,    -1,    -1,    -1,
      -1,    -1,    10,    11,    12,    -1,    14,    15,    16,    17,
      18,    19,  2053,    -1,    -1,    23,    24,    25,    26,    27,
      28,    -1,    -1,    31,    32,    33,    34,    35,    -1,    -1,
      -1,  2072,    -1,    -1,  2075,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    61,    -1,    -1,    -1,  2098,  2099,    -1,
      -1,  1955,  1956,    -1,    -1,    10,    11,    12,    -1,    14,
      15,    16,    17,    18,    -1,    -1,    -1,    -1,    23,    24,
      25,    26,    27,    28,    -1,    -1,    31,    32,    33,    34,
      35,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,  1998,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,  2072,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,  2098,  2099,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,  1803,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   271,   272,   273,   274,  1842,    -1,    -1,
    1845,  1846,  1847,    -1,    -1,    -1,    -1,  1852,  1853,  1854,
     288,   289,   290,   291,    -1,    -1,    -1,    -1,   296,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   306,    -1,
      -1,   309,   310,    -1,    -1,    -1,    -1,    -1,   316,   317,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
       1,    -1,    -1,    -1,    -1,    -1,   271,   272,   273,   274,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    23,   288,   289,   290,   291,    -1,    -1,    -1,
      -1,    -1,    -1,    34,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   316,   317,    -1,    -1,   383,  1951,  1952,    -1,    -1,
      -1,    -1,    -1,    -1,    65,    66,    67,    68,    69,    70,
      71,    72,    73,    74,    75,    76,    77,    78,    -1,    -1,
      -1,    -1,    -1,    -1,    85,    86,    87,    88,    89,    -1,
      -1,    -1,    93,    -1,    95,    96,    97,    98,    99,    -1,
      -1,  1996,    -1,    -1,    -1,    -1,    -1,    -1,  2003,  2004,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,  2054,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,  2071,    -1,    -1,    -1,
      -1,    -1,  2077,   184,   185,   186,   187,   188,   189,   190,
      -1,   192,   193,   194,   195,   196,   197,   198,   199,   200,
     201,   202,   203,   204,   205,    -1,   207,   208,   209,   210,
     211,   212,   213,   214,   215,   216,   217,   218,   219,   220,
     221,   222,   223,   224,   225,   226,   227,   228,   229,   230,
     231,   232,    -1,   234,   235,   236,   237,   238,   239,   240,
     241,   242,   243,   244,   245,   246,   247,   248,   249,   250,
     251,    -1,   253,   254,   255,   256,   257,   258,   259,   260,
     261,   262,   263,    -1,   265,   266,    -1,   268,   269,   270,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,     1,    -1,    -1,
      -1,    -1,    -1,   284,   285,   286,    -1,    -1,    -1,   290,
      -1,   292,   293,   294,   295,   296,   297,    -1,    -1,    23,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      34,    -1,   313,   314,   315,    -1,    -1,   318,   319,   320,
     321,   322,   323,   324,   325,   326,   327,   328,   329,   330,
     331,   332,   333,   334,   335,   336,   337,   338,   339,   340,
     341,    65,    66,    67,    68,    69,    70,    71,    72,    73,
      74,    75,    76,    77,    78,    -1,    -1,    -1,    -1,    -1,
      -1,    85,    86,    87,    88,    89,    -1,     0,     1,    93,
       3,    95,    96,    97,    98,    99,    -1,    10,    11,    12,
      -1,    14,    15,    16,    17,    18,    19,    -1,   389,    -1,
      23,    24,    25,    26,    27,    28,    -1,    -1,    31,    32,
      33,    34,    35,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,     0,     1,    -1,     3,    -1,    -1,    -1,    61,    -1,
      -1,    10,    11,    12,    -1,    14,    15,    16,    17,    18,
      19,    -1,    -1,    -1,    23,    24,    25,    26,    27,    28,
      -1,    -1,    31,    32,    33,    34,    35,    -1,    -1,    -1,
     184,   185,   186,   187,   188,   189,   190,    -1,   192,   193,
     194,   195,   196,   197,   198,   199,   200,   201,   202,   203,
     204,   205,    61,   207,   208,   209,   210,   211,   212,   213,
     214,   215,   216,   217,   218,   219,   220,   221,   222,   223,
     224,   225,   226,   227,   228,   229,   230,   231,   232,    -1,
     234,   235,   236,   237,   238,   239,   240,   241,   242,   243,
     244,   245,   246,   247,   248,   249,   250,   251,    -1,   253,
     254,   255,   256,   257,   258,   259,   260,   261,   262,   263,
      -1,   265,   266,    -1,   268,   269,   270,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     284,   285,   286,    -1,    -1,    -1,   290,    -1,   292,   293,
     294,   295,   296,   297,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   313,
     314,   315,    -1,    -1,   318,   319,   320,   321,   322,   323,
     324,   325,   326,   327,   328,   329,   330,   331,   332,   333,
     334,   335,   336,   337,   338,   339,   340,   341,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   271,   272,
     273,   274,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   288,   289,   290,   291,    -1,
      -1,    -1,    -1,   296,    -1,   389,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   306,    -1,    -1,   309,   310,    -1,    -1,
      -1,    -1,    -1,   316,   317,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   271,   272,   273,   274,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   288,
     289,   290,   291,    -1,    -1,    -1,    -1,   296,    -1,     4,
       5,     6,     7,    -1,    -1,    -1,    -1,   306,    -1,    -1,
     309,   310,    -1,    -1,    -1,    -1,    -1,   316,   317,    -1,
      -1,     0,     1,    -1,     3,    -1,    -1,    -1,    -1,    -1,
     383,    10,    11,    12,    -1,    14,    15,    16,    17,    18,
      19,    -1,    -1,    -1,    23,    24,    25,    26,    27,    28,
      -1,    -1,    31,    32,    33,    34,    35,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    61,    -1,   383,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   100,   101,   102,   103,   104,
     105,   106,   107,   108,   109,   110,   111,   112,   113,   114,
     115,   116,   117,   118,   119,   120,   121,   122,   123,   124,
     125,   126,   127,   128,   129,   130,   131,   132,   133,   134,
     135,   136,   137,   138,   139,   140,   141,   142,   143,   144,
     145,   146,   147,   148,   149,   150,   151,   152,   153,   154,
     155,   156,   157,   158,   159,   160,   161,   162,   163,   164,
     165,   166,   167,   168,   100,   101,   102,   103,   104,   105,
     106,   107,   108,   109,   110,   111,   112,   113,   114,   115,
     116,   117,   118,   119,   120,   121,   122,   123,   124,   125,
     126,   127,   128,   129,   130,   131,   132,   133,   134,   135,
     136,   137,   138,   139,   140,   141,   142,   143,   144,   145,
     146,   147,   148,   149,   150,   151,   152,   153,   154,   155,
     156,   157,   158,   159,   160,   161,   162,   163,   164,   165,
     166,   167,   168,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   271,   272,   273,   274,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   288,
     289,   290,   291,    -1,    -1,    -1,    -1,   296,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   306,    -1,    -1,
     309,   310,    -1,    -1,    -1,    -1,    -1,   316,   317,   344,
     345,   346,   347,   348,   349,   350,   351,   352,   353,   354,
     355,   356,   357,   358,   359,   360,   361,   362,   363,   364,
     365,   366,   367,   368,   369,   370,   371,   372,   373,   374,
     375,    -1,   377,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     385,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   383,    -1,    -1,    -1,   344,   345,
     346,   347,   348,   349,   350,   351,   352,   353,   354,   355,
     356,   357,   358,   359,   360,   361,   362,   363,   364,   365,
     366,   367,   368,   369,   370,   371,   372,   373,   374,   375,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   388,   100,   101,   102,   103,   104,   105,   106,
     107,   108,   109,   110,   111,   112,   113,   114,   115,   116,
     117,   118,   119,   120,   121,   122,   123,   124,   125,   126,
     127,   128,   129,   130,   131,   132,   133,   134,   135,   136,
     137,   138,   139,   140,   141,   142,   143,   144,   145,   146,
     147,   148,   149,   150,   151,   152,   153,   154,   155,   156,
     157,   158,   159,   160,   161,   162,   163,   164,   165,   166,
     167,   168,   100,   101,   102,   103,   104,   105,   106,   107,
     108,   109,   110,   111,   112,   113,   114,   115,   116,   117,
     118,   119,   120,   121,   122,   123,   124,   125,   126,   127,
     128,   129,   130,   131,   132,   133,   134,   135,   136,   137,
     138,   139,   140,   141,   142,   143,   144,   145,   146,   147,
     148,   149,   150,   151,   152,   153,   154,   155,   156,   157,
     158,   159,   160,   161,   162,   163,   164,   165,   166,   167,
     168,   100,   101,   102,   103,   104,   105,   106,   107,   108,
     109,   110,   111,   112,   113,   114,   115,   116,   117,   118,
     119,   120,   121,   122,   123,   124,   125,   126,   127,   128,
     129,   130,   131,   132,   133,   134,   135,   136,   137,   138,
     139,   140,   141,   142,   143,   144,   145,   146,   147,   148,
     149,   150,   151,   152,   153,   154,   155,   156,   157,   158,
     159,   160,   161,   162,   163,   164,   165,   166,   167,   168,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   344,   345,   346,
     347,   348,   349,   350,   351,   352,   353,   354,   355,   356,
     357,   358,   359,   360,   361,   362,   363,   364,   365,   366,
     367,   368,   369,   370,   371,   372,   373,   374,   375,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   388,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   344,   345,   346,   347,
     348,   349,   350,   351,   352,   353,   354,   355,   356,   357,
     358,   359,   360,   361,   362,   363,   364,   365,   366,   367,
     368,   369,   370,   371,   372,   373,   374,   375,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     388,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   344,   345,   346,   347,   348,
     349,   350,   351,   352,   353,   354,   355,   356,   357,   358,
     359,   360,   361,   362,   363,   364,   365,   366,   367,   368,
     369,   370,   371,   372,   373,   374,   375
};

/* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
   symbol of state STATE-NUM.  */
static const yytype_uint16 yystos[] =
{
       0,   397,     0,     1,     3,    10,    11,    12,    14,    15,
      16,    17,    18,    19,    23,    24,    25,    26,    27,    28,
      31,    32,    33,    34,    35,    61,   271,   272,   273,   274,
     288,   289,   290,   291,   296,   306,   309,   310,   316,   317,
     383,   398,   426,   429,   433,   434,   435,   436,   437,   442,
     443,   445,   446,   449,   457,   463,   467,   470,   473,   477,
     488,   489,   495,   497,   500,   503,   506,   510,   514,   518,
      22,    22,   429,   431,   432,   432,   432,   431,   431,   431,
     431,   431,    22,   427,   429,   488,   432,    62,   431,   431,
     431,   431,   432,   432,   432,   432,    22,   432,   432,   432,
     488,    22,   431,     1,     1,     1,     1,     1,     1,    36,
      37,    38,    39,    40,    41,    42,    43,    44,    45,    46,
      47,    48,    49,    50,    51,    52,    53,    54,    55,    56,
      57,    58,    59,    60,   494,   495,   501,   431,   391,   480,
     481,   388,   388,   388,   464,   474,   478,   471,   468,   428,
     481,   488,   504,   384,   288,   289,   430,   430,   430,   430,
     385,   388,   507,   511,   515,   519,   388,   388,   383,   447,
     383,   383,    22,   481,   481,   479,    21,    21,    21,   481,
     481,   481,   481,   481,   481,   388,   388,   502,     4,     5,
     402,   458,   454,   450,   452,    29,    30,    36,   438,   440,
     441,    21,   481,   481,   481,   481,    21,    21,   481,   496,
     388,     1,    22,   392,   388,   388,   388,   388,   388,   388,
      19,    20,   261,   422,   425,   389,   497,   481,   481,   481,
     481,   379,   439,    47,   386,   387,   508,   512,   516,   520,
     388,     1,   376,   384,   393,   480,   482,   483,   484,   485,
     486,   487,   490,   491,   492,   493,   433,   434,   435,   436,
     437,   442,   443,   446,   449,   457,   463,   467,   470,   473,
     477,   497,   498,   499,   503,   376,   462,   472,   476,   221,
     222,   469,   466,   425,    22,    22,   423,   385,     1,   389,
     422,   389,   459,   455,   451,   453,    22,   439,   388,   440,
     388,   388,   388,   388,   444,     1,     6,     7,   377,   385,
     388,   400,   401,   402,   410,    22,   377,   385,   394,   401,
     490,   480,   490,   480,   490,   491,   383,   389,   498,     9,
     400,   401,   402,   405,     1,   169,   170,   171,   172,   173,
     389,     1,   236,   237,   238,   239,   240,   241,   242,   243,
     245,   246,   247,   248,   249,   253,   254,   255,   256,   257,
     258,   259,   260,   389,     1,   320,   321,   322,   323,   324,
     325,   326,   327,   328,   329,   330,   331,   332,   333,   334,
     335,   336,   337,   338,   339,   340,   341,   389,   376,   376,
       1,   208,   210,   211,   214,   215,   216,   217,   218,   219,
     220,   223,   224,   225,   226,   227,   228,   229,   230,   231,
     232,   234,   235,   389,     1,   184,   185,   186,   187,   188,
     189,   190,   192,   193,   194,   195,   196,   197,   198,   199,
     201,   202,   203,   204,   205,   207,   389,   389,   287,   383,
     424,   481,     9,    22,   388,   388,   388,   388,    22,    21,
     298,   505,   509,   513,   517,     1,    73,   389,   400,   401,
     400,   401,   377,   378,   379,   380,   382,   377,   378,   379,
     380,   382,   400,   401,   402,   410,   412,   413,   416,   401,
     401,   377,   378,   379,   394,   490,   490,   480,   490,   383,
     383,   383,   383,   383,   376,   376,   376,   376,   376,   383,
     376,   376,   376,   376,   376,   376,   376,   376,   376,   376,
     376,   376,   376,   376,   376,   376,   376,   376,   376,   376,
     376,   383,   376,   376,   376,   393,   393,   376,   376,   376,
     376,   376,   376,   376,   376,   376,   376,   376,   376,   376,
     376,   376,   376,   376,   410,   100,   101,   102,   103,   104,
     105,   106,   107,   108,   109,   110,   111,   112,   113,   114,
     115,   116,   117,   118,   119,   120,   121,   122,   123,   124,
     125,   126,   127,   128,   129,   130,   131,   132,   133,   134,
     135,   136,   137,   138,   139,   140,   141,   142,   143,   144,
     145,   146,   147,   148,   149,   150,   151,   152,   153,   154,
     155,   156,   157,   158,   159,   160,   161,   162,   163,   164,
     165,   166,   167,   168,   344,   345,   346,   347,   348,   349,
     350,   351,   352,   353,   354,   355,   356,   357,   358,   359,
     360,   361,   362,   363,   364,   365,   366,   367,   368,   369,
     370,   371,   372,   373,   374,   375,   419,   383,   376,   376,
     376,   376,   376,   376,   376,   376,   376,   376,   376,   376,
     376,   376,   376,   376,   376,   376,   376,   376,   376,   376,
     383,   376,   376,   376,   376,   376,   376,   376,   376,   376,
     376,   376,   376,   376,   376,   376,   376,   376,   376,   376,
     376,   376,   383,   481,   388,   386,   386,   456,   448,   448,
     448,   376,    34,   299,   300,   301,   302,   303,   304,   305,
     389,    34,   307,   308,   389,   278,   281,   389,   311,   312,
     389,   376,   386,   386,   400,   401,   400,   401,   400,   401,
     400,   401,   400,   401,   400,   401,   400,   401,   400,   401,
     400,   401,   400,   401,   412,     1,   387,   389,   411,     1,
     387,   411,   387,   411,   401,   401,   401,   490,     1,   174,
     175,   176,   177,   178,   179,   460,   460,     1,   180,   181,
     182,   183,   461,   461,   461,   400,   401,   402,   403,   419,
     403,   403,   403,   410,   403,   419,   400,   401,   402,   404,
     404,   419,   419,   419,   404,   419,   404,   404,   419,   419,
     419,   404,   388,   419,   419,   404,   404,   419,   401,   402,
     405,   419,   419,   419,   419,   419,   405,   405,   419,   405,
     419,   404,   419,   419,   419,   419,   419,   383,   403,   419,
     405,   403,   419,   419,   419,   419,   419,   419,   419,   419,
     410,   403,   403,   403,   419,   410,   403,   403,   403,   403,
     405,   403,   403,   403,   419,   405,   403,   404,   404,   403,
     403,   419,   419,   403,   410,   419,   405,   405,   403,   403,
     403,   403,   388,   421,   376,   376,    22,   275,   276,   389,
      22,    73,   277,   278,   279,   280,   281,   389,   389,   389,
     410,   385,   376,   376,   376,   376,   376,   376,   376,   385,
     376,   376,   376,   376,   376,   376,    22,   411,   400,   401,
     410,   402,   383,   383,   383,   383,   383,   383,   383,   383,
     383,   383,   405,   383,   383,   383,   383,   383,   383,   383,
     383,   383,   383,   383,   383,   383,   383,   383,   405,   383,
     383,   394,   394,   383,   383,   383,   383,   383,   383,   383,
     383,   383,   383,   383,   383,   383,   383,   383,   383,   383,
     387,   383,   383,   383,   383,   383,   383,   383,   383,   383,
     383,   383,   383,   405,   383,   383,   383,   383,   403,   383,
     383,   383,   383,   383,   383,   383,   383,   383,   383,   383,
     383,   383,   383,   383,   383,   383,   383,   419,   383,   383,
     383,   383,   383,   383,   383,   421,     1,    23,    34,    65,
      66,    67,    68,    69,    70,    71,    72,    73,    74,    75,
      76,    77,    78,    85,    86,    87,    88,    89,    93,    95,
      96,    97,    98,    99,   184,   185,   186,   187,   188,   189,
     190,   192,   193,   194,   195,   196,   197,   198,   199,   200,
     201,   202,   203,   204,   205,   207,   208,   209,   210,   211,
     212,   213,   214,   215,   216,   217,   218,   219,   220,   221,
     222,   223,   224,   225,   226,   227,   228,   229,   230,   231,
     232,   234,   235,   236,   237,   238,   239,   240,   241,   242,
     243,   244,   245,   246,   247,   248,   249,   250,   251,   253,
     254,   255,   256,   257,   258,   259,   260,   261,   262,   263,
     265,   266,   268,   269,   270,   284,   285,   286,   290,   292,
     293,   294,   295,   296,   297,   313,   314,   315,   318,   319,
     320,   321,   322,   323,   324,   325,   326,   327,   328,   329,
     330,   331,   332,   333,   334,   335,   336,   337,   338,   339,
     340,   341,   389,   465,   475,   404,   404,   376,   376,   376,
     376,   376,   376,   376,   376,   376,   376,    22,   409,    22,
     410,    22,    22,    22,    22,   401,    22,    22,   410,   410,
     283,   410,   282,     9,     9,   383,   412,   387,   406,   387,
     376,   376,   419,   406,   406,   406,   389,   383,   385,   385,
     376,   391,   376,   391,   376,   391,   376,   391,   376,   391,
     376,   385,   385,   385,   376,   385,   376,   385,   376,   385,
     385,   385,   376,   391,   376,   391,   376,   391,   376,   391,
     376,   391,   385,   385,   385,   385,   385,   385,   376,   376,
     376,   376,   376,   376,   376,   376,   376,   376,   376,   376,
     376,   376,   376,   376,   376,   376,   376,   376,   376,   376,
     376,   376,   376,   376,   376,   376,   376,   376,   376,   376,
     376,   376,   376,   376,   376,   376,   376,   376,   376,   376,
     376,   376,   376,   376,   376,   376,   376,   376,   376,   376,
     376,   376,   376,   376,   376,   376,   376,   376,   376,   376,
     376,   376,   376,   376,   376,   376,   376,   376,   376,   376,
     376,   385,   376,   376,   376,   376,   376,   376,   393,   376,
     385,   376,   376,   376,   376,   376,   376,   376,   376,   376,
     376,   376,   391,   376,   391,   376,   391,   376,   391,   376,
     376,   376,   393,   393,   376,   376,   376,   376,   376,   376,
     376,   376,   376,   376,   376,   376,   376,   376,   376,   376,
     376,   383,   383,     9,    22,    22,   410,     9,    22,   283,
     404,   410,   283,   410,   405,    22,   282,   406,   411,   386,
     409,   383,   383,   383,   383,   383,   383,   386,   409,   409,
     383,   401,   414,   383,   383,   383,   411,   405,   405,   388,
     388,   411,   405,   403,   419,    22,    22,     5,    22,   407,
     408,   410,     9,    22,   399,   408,   399,   408,   399,   408,
     399,   408,   399,     9,    22,    22,    22,    22,    22,    22,
      22,    22,    22,    22,    22,   408,   399,   408,   399,   408,
     399,   408,   399,   408,   399,    22,    22,    22,    22,    22,
      22,   405,   403,   403,   403,   419,   405,   403,   404,   404,
     403,   403,   419,   419,   419,   403,   410,   410,   405,   405,
     403,   403,   403,   403,   403,   410,   419,   405,   388,   388,
     403,   419,   419,   419,   419,   419,   419,   410,   419,   419,
     419,   410,   403,   403,   403,   419,   410,   403,   403,   403,
     403,   403,   419,   403,   403,   403,   410,   403,   388,   419,
     388,   404,   404,   419,   419,   419,   388,   388,   404,   388,
     419,   404,   404,   419,   419,   419,   404,   399,   404,   264,
      22,   267,   420,    22,   410,   404,   410,    22,   404,   403,
     388,   408,   401,   410,   415,   415,    22,   401,   410,   408,
     399,   408,   399,   408,   399,   408,   399,   388,   419,   419,
     404,   404,   419,   405,   419,   419,   419,   419,   419,   405,
     405,   419,   405,   419,   404,   419,   419,   419,   419,   383,
     383,   383,   409,   383,   383,   383,   383,   414,   383,   414,
     383,   383,   383,    22,   383,   376,   490,   411,   376,   490,
     411,   411,   387,   411,   411,   389,   419,   419,   383,   406,
     406,   411,   386,   386,   393,   395,   406,   383,   407,   392,
     383,   392,   383,   392,   383,   392,   383,   392,   383,   386,
     386,   386,   383,   386,   383,   386,   383,   386,   386,   386,
     383,   392,   383,   392,   383,   392,   383,   392,   383,   392,
     386,   386,   386,   386,   386,   386,   383,   383,   383,   383,
     383,   383,   383,   383,   383,   383,   383,   383,   383,   383,
     383,   419,   405,   383,   383,   383,   383,   383,   383,   383,
     419,   383,   383,   419,   419,   383,   383,   383,   383,   383,
     383,   383,   419,   383,   383,   383,   405,   383,   383,   383,
     383,   403,   383,   383,   383,   383,   383,   383,   383,   383,
     383,   405,   383,   419,   383,   419,   383,   383,   383,   383,
     383,   419,   419,   383,   419,   383,   383,   383,   383,   383,
     383,   383,   386,   383,   383,   383,   383,   390,   383,   412,
     394,   414,   387,   383,   383,    22,   383,   383,   414,     1,
     383,   387,   383,   383,   383,   414,   383,   392,   383,   392,
     383,   392,   383,   392,   405,   383,   383,   394,   394,   383,
     383,   383,   383,   383,   383,   383,   383,   383,   383,   383,
     383,   383,   383,   383,   383,   383,   411,   411,   411,    22,
     410,   383,   383,    22,   410,   383,   383,   383,   401,   383,
     383,   383,   387,   387,   405,   403,   383,   376,   376,   490,
     401,    71,    72,    73,    77,    78,    22,   411,   376,   376,
     376,   376,   376,   376,   376,   376,   383,   383,   383,   376,
     376,   376,   376,   376,   376,   376,   376,   376,   376,   376,
     376,   376,   406,   387,   387,   387,   387,   387,   406,   406,
     406,   387,   387,   387,   387,   387,   376,   267,   411,   376,
     411,    22,   387,   411,   410,   411,   376,   376,   376,   376,
     387,   376,   376,   383,   383,   383,   383,   409,   383,   409,
     404,   419,   406,   406,    22,    22,   410,   383,   394,   386,
     386,   386,   386,   386,   408,   408,   408,   408,   408,    22,
     388,   401,    22,    22,   388,   401,   408,   408,   408,   408,
     408,    22,   401,    22,    22,   388,   401,    22,   419,   405,
     405,   419,   419,   419,   405,   403,   405,   404,   419,   419,
     419,   404,   404,   383,   413,   383,   386,     1,    22,    29,
     400,   401,   417,   418,   414,   383,   408,   408,   408,   408,
     405,   388,   388,   411,   411,   387,   387,   405,   403,   383,
     383,   409,   386,   376,   376,   376,   376,   376,   383,   383,
     383,   383,   383,   383,    22,   383,   383,   383,    22,   383,
     383,   383,   383,   383,   383,   383,   383,   383,   383,    22,
     383,   383,   411,   411,   411,   389,   387,   411,   406,   406,
     411,   387,   387,   387,   387,   387,   383,   383,   383,     1,
     387,   389,   411,   383,   383,   383,   383,   389,   419,   419,
     383,   383,   405,   405,   411,   411,   411,   376,    22,   401,
      22,    22,   401,   387,   387,   387,   383,   383,   383,   383,
     419,   383,   405,   403,   383,   404,   404,   419,   419,   404,
     417,   383,   383,   387,   387,   389,   389,   383,   383,   383,
      22,   410,   415,   383,   383,   383,   383,   383,   401,   401,
     401,   387,   406,   406,   389,   387,   389,   387,   389,   404,
     419,   383,   383,   383,   413,   416,   383,   389,   389,   389,
     419,   405,   403,   383,   404,   383,   419,   383,   387,   387,
     411,   411,   383,   383,   383,   389,   411,   411,   389,   389,
     405,   405,   383,   383,   383,   383,   383,   383,   383,   389,
     389,   383,   383
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
      case 407: /* "shaderlist" */

/* Line 1000 of yacc.c  */
#line 462 "Grammar.ypp"
	{ if((yyvaluep->shaderList)) delete (yyvaluep->shaderList); };

/* Line 1000 of yacc.c  */
#line 4276 "C:/p4/GitHub/nvFX/FxParser/Grammar.cpp"
	break;
      case 408: /* "one_or_more_shaders" */

/* Line 1000 of yacc.c  */
#line 462 "Grammar.ypp"
	{ if((yyvaluep->shaderList)) delete (yyvaluep->shaderList); };

/* Line 1000 of yacc.c  */
#line 4285 "C:/p4/GitHub/nvFX/FxParser/Grammar.cpp"
	break;
      case 409: /* "varnamelist" */

/* Line 1000 of yacc.c  */
#line 461 "Grammar.ypp"
	{ if((yyvaluep->varNameList)) delete (yyvaluep->varNameList); };

/* Line 1000 of yacc.c  */
#line 4294 "C:/p4/GitHub/nvFX/FxParser/Grammar.cpp"
	break;
      case 412: /* "fexplist" */

/* Line 1000 of yacc.c  */
#line 463 "Grammar.ypp"
	{ if((yyvaluep->floatList)) delete (yyvaluep->floatList); };

/* Line 1000 of yacc.c  */
#line 4303 "C:/p4/GitHub/nvFX/FxParser/Grammar.cpp"
	break;
      case 413: /* "fexpveclist" */

/* Line 1000 of yacc.c  */
#line 464 "Grammar.ypp"
	{ if((yyvaluep->floatVecList)) delete (yyvaluep->floatVecList); };

/* Line 1000 of yacc.c  */
#line 4312 "C:/p4/GitHub/nvFX/FxParser/Grammar.cpp"
	break;
      case 414: /* "iexplist" */

/* Line 1000 of yacc.c  */
#line 465 "Grammar.ypp"
	{ if((yyvaluep->intList)) delete (yyvaluep->intList); };

/* Line 1000 of yacc.c  */
#line 4321 "C:/p4/GitHub/nvFX/FxParser/Grammar.cpp"
	break;
      case 416: /* "bexplist" */

/* Line 1000 of yacc.c  */
#line 466 "Grammar.ypp"
	{ if((yyvaluep->boolList)) delete (yyvaluep->boolList); };

/* Line 1000 of yacc.c  */
#line 4330 "C:/p4/GitHub/nvFX/FxParser/Grammar.cpp"
	break;
      case 441: /* "arglist" */

/* Line 1000 of yacc.c  */
#line 467 "Grammar.ypp"
	{ if((yyvaluep->argList)) delete (yyvaluep->argList); };

/* Line 1000 of yacc.c  */
#line 4339 "C:/p4/GitHub/nvFX/FxParser/Grammar.cpp"
	break;
      case 482: /* "valueassignment_f" */

/* Line 1000 of yacc.c  */
#line 464 "Grammar.ypp"
	{ if((yyvaluep->floatVecList)) delete (yyvaluep->floatVecList); };

/* Line 1000 of yacc.c  */
#line 4348 "C:/p4/GitHub/nvFX/FxParser/Grammar.cpp"
	break;
      case 483: /* "valueassignment_fv" */

/* Line 1000 of yacc.c  */
#line 464 "Grammar.ypp"
	{ if((yyvaluep->floatVecList)) delete (yyvaluep->floatVecList); };

/* Line 1000 of yacc.c  */
#line 4357 "C:/p4/GitHub/nvFX/FxParser/Grammar.cpp"
	break;
      case 484: /* "valueassignment_fvl" */

/* Line 1000 of yacc.c  */
#line 464 "Grammar.ypp"
	{ if((yyvaluep->floatVecList)) delete (yyvaluep->floatVecList); };

/* Line 1000 of yacc.c  */
#line 4366 "C:/p4/GitHub/nvFX/FxParser/Grammar.cpp"
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
#line 394 "Grammar.ypp"
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
#line 4524 "C:/p4/GitHub/nvFX/FxParser/Grammar.cpp"

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
#line 1047 "Grammar.ypp"
    {
    curContainer->separateShadersEnable((yyvsp[(4) - (4)].bval));
;}
    break;

  case 30:

/* Line 1455 of yacc.c  */
#line 1055 "Grammar.ypp"
    { (yyval.s) = (yyvsp[(1) - (1)].s); ;}
    break;

  case 31:

/* Line 1455 of yacc.c  */
#line 1056 "Grammar.ypp"
    { (yyval.s) = (yyvsp[(1) - (1)].s); ;}
    break;

  case 32:

/* Line 1455 of yacc.c  */
#line 1062 "Grammar.ypp"
    {  (yyval.fval)  =  (yyvsp[(1) - (1)].fval); ;}
    break;

  case 33:

/* Line 1455 of yacc.c  */
#line 1063 "Grammar.ypp"
    {  (yyval.fval)  =  (yyvsp[(1) - (3)].fval)  +  (yyvsp[(3) - (3)].fval); ;}
    break;

  case 34:

/* Line 1455 of yacc.c  */
#line 1064 "Grammar.ypp"
    {  (yyval.fval)  =  (float)(yyvsp[(1) - (3)].ival)  +  (yyvsp[(3) - (3)].fval); ;}
    break;

  case 35:

/* Line 1455 of yacc.c  */
#line 1065 "Grammar.ypp"
    {  (yyval.fval)  =  (yyvsp[(1) - (3)].fval)  +  (float)(yyvsp[(3) - (3)].ival); ;}
    break;

  case 36:

/* Line 1455 of yacc.c  */
#line 1066 "Grammar.ypp"
    {  (yyval.fval)  =  (yyvsp[(1) - (3)].fval)  -  (yyvsp[(3) - (3)].fval); ;}
    break;

  case 37:

/* Line 1455 of yacc.c  */
#line 1067 "Grammar.ypp"
    {  (yyval.fval)  =  (float)(yyvsp[(1) - (3)].ival)  -  (yyvsp[(3) - (3)].fval); ;}
    break;

  case 38:

/* Line 1455 of yacc.c  */
#line 1068 "Grammar.ypp"
    {  (yyval.fval)  =  (yyvsp[(1) - (3)].fval)  -  (float)(yyvsp[(3) - (3)].ival); ;}
    break;

  case 39:

/* Line 1455 of yacc.c  */
#line 1069 "Grammar.ypp"
    {  (yyval.fval)  =  (yyvsp[(1) - (3)].fval)  *  (yyvsp[(3) - (3)].fval); ;}
    break;

  case 40:

/* Line 1455 of yacc.c  */
#line 1070 "Grammar.ypp"
    {  (yyval.fval)  =  (float)(yyvsp[(1) - (3)].ival)  *  (yyvsp[(3) - (3)].fval); ;}
    break;

  case 41:

/* Line 1455 of yacc.c  */
#line 1071 "Grammar.ypp"
    {  (yyval.fval)  =  (yyvsp[(1) - (3)].fval)  *  (float)(yyvsp[(3) - (3)].ival); ;}
    break;

  case 42:

/* Line 1455 of yacc.c  */
#line 1072 "Grammar.ypp"
    {  (yyval.fval)  =  (yyvsp[(1) - (3)].fval)  /  (yyvsp[(3) - (3)].fval); ;}
    break;

  case 43:

/* Line 1455 of yacc.c  */
#line 1073 "Grammar.ypp"
    {  (yyval.fval)  =  (float)(yyvsp[(1) - (3)].ival)  /  (yyvsp[(3) - (3)].fval); ;}
    break;

  case 44:

/* Line 1455 of yacc.c  */
#line 1074 "Grammar.ypp"
    {  (yyval.fval)  =  (yyvsp[(1) - (3)].fval)  /  (float)(yyvsp[(3) - (3)].ival); ;}
    break;

  case 45:

/* Line 1455 of yacc.c  */
#line 1075 "Grammar.ypp"
    {  (yyval.fval)  =  (float)(yyvsp[(1) - (3)].ival)  /  (float)(yyvsp[(3) - (3)].ival); ;}
    break;

  case 46:

/* Line 1455 of yacc.c  */
#line 1076 "Grammar.ypp"
    {  (yyval.fval)  =  -(yyvsp[(2) - (2)].fval); ;}
    break;

  case 47:

/* Line 1455 of yacc.c  */
#line 1077 "Grammar.ypp"
    {  (yyval.fval)  =  pow  ((yyvsp[(1) - (3)].fval),  (yyvsp[(3) - (3)].fval)); ;}
    break;

  case 48:

/* Line 1455 of yacc.c  */
#line 1078 "Grammar.ypp"
    {  (yyval.fval)  =  pow  ((float)(yyvsp[(1) - (3)].ival),  (yyvsp[(3) - (3)].fval)); ;}
    break;

  case 49:

/* Line 1455 of yacc.c  */
#line 1079 "Grammar.ypp"
    {  (yyval.fval)  =  pow  ((yyvsp[(1) - (3)].fval),  (float)(yyvsp[(3) - (3)].ival)); ;}
    break;

  case 50:

/* Line 1455 of yacc.c  */
#line 1080 "Grammar.ypp"
    {  (yyval.fval)  =  pow  ((float)(yyvsp[(1) - (3)].ival),  (float)(yyvsp[(3) - (3)].ival)); ;}
    break;

  case 51:

/* Line 1455 of yacc.c  */
#line 1081 "Grammar.ypp"
    {  (yyval.fval)  =  (yyvsp[(2) - (3)].fval); ;}
    break;

  case 52:

/* Line 1455 of yacc.c  */
#line 1089 "Grammar.ypp"
    {  (yyval.ival)  =  (yyvsp[(1) - (1)].ival); ;}
    break;

  case 53:

/* Line 1455 of yacc.c  */
#line 1090 "Grammar.ypp"
    {  (yyval.ival)  =  (yyvsp[(1) - (3)].ival)  +  (yyvsp[(3) - (3)].ival); ;}
    break;

  case 54:

/* Line 1455 of yacc.c  */
#line 1091 "Grammar.ypp"
    {  (yyval.ival)  =  (yyvsp[(1) - (3)].ival)  -  (yyvsp[(3) - (3)].ival); ;}
    break;

  case 55:

/* Line 1455 of yacc.c  */
#line 1092 "Grammar.ypp"
    {  (yyval.ival)  =  (yyvsp[(1) - (3)].ival)  *  (yyvsp[(3) - (3)].ival); ;}
    break;

  case 56:

/* Line 1455 of yacc.c  */
#line 1093 "Grammar.ypp"
    {  (yyval.ival)  =  -(yyvsp[(2) - (2)].ival); ;}
    break;

  case 57:

/* Line 1455 of yacc.c  */
#line 1094 "Grammar.ypp"
    {  (yyval.ival)  =  (yyvsp[(2) - (3)].ival); ;}
    break;

  case 58:

/* Line 1455 of yacc.c  */
#line 1099 "Grammar.ypp"
    {  (yyval.bval)  =  true; ;}
    break;

  case 59:

/* Line 1455 of yacc.c  */
#line 1100 "Grammar.ypp"
    {  (yyval.bval)  =  false; ;}
    break;

  case 60:

/* Line 1455 of yacc.c  */
#line 1106 "Grammar.ypp"
    { (yyval.bval) = (yyvsp[(1) - (1)].bval); ;}
    break;

  case 61:

/* Line 1455 of yacc.c  */
#line 1107 "Grammar.ypp"
    { 
    if(((yyvsp[(1) - (1)].ival) != 0)&&((yyvsp[(1) - (1)].ival) != 1)) yyerror("    Warning : truncation of float to bool");
    (yyval.bval) = (yyvsp[(1) - (1)].ival) ? true: false; ;}
    break;

  case 62:

/* Line 1455 of yacc.c  */
#line 1110 "Grammar.ypp"
    { 
    if(((yyvsp[(1) - (1)].fval) != 0.0f)&&((yyvsp[(1) - (1)].fval) != 1.0f)) yyerror("    Warning : truncation of float to bool");
    (yyval.bval) = (yyvsp[(1) - (1)].fval) ? true: false; ;}
    break;

  case 63:

/* Line 1455 of yacc.c  */
#line 1114 "Grammar.ypp"
    { (yyval.ival) = (yyvsp[(1) - (1)].ival); ;}
    break;

  case 64:

/* Line 1455 of yacc.c  */
#line 1115 "Grammar.ypp"
    { 
    if(((yyvsp[(1) - (1)].fval) - (int)(yyvsp[(1) - (1)].fval))!=0.0f)
        yyerror("    Warning : truncation of float to int");
    (yyval.ival) = (int)(yyvsp[(1) - (1)].fval); ;}
    break;

  case 65:

/* Line 1455 of yacc.c  */
#line 1119 "Grammar.ypp"
    { (yyval.ival) = (yyvsp[(1) - (1)].bval) ? 1: 0; ;}
    break;

  case 66:

/* Line 1455 of yacc.c  */
#line 1122 "Grammar.ypp"
    { (yyval.fval) = (yyvsp[(1) - (1)].fval); ;}
    break;

  case 67:

/* Line 1455 of yacc.c  */
#line 1123 "Grammar.ypp"
    { (yyval.fval) = (float)(yyvsp[(1) - (1)].ival); ;}
    break;

  case 68:

/* Line 1455 of yacc.c  */
#line 1124 "Grammar.ypp"
    { (yyval.fval) = (yyvsp[(1) - (1)].bval) ? 1.0f: 0.0f; ;}
    break;

  case 71:

/* Line 1455 of yacc.c  */
#line 1149 "Grammar.ypp"
    { (yyval.shaderList) = NULL; ;}
    break;

  case 72:

/* Line 1455 of yacc.c  */
#line 1150 "Grammar.ypp"
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
#line 1165 "Grammar.ypp"
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
#line 1182 "Grammar.ypp"
    { (yyval.shaderList) = NULL; ;}
    break;

  case 75:

/* Line 1455 of yacc.c  */
#line 1198 "Grammar.ypp"
    { (yyval.shaderList) = (yyvsp[(1) - (1)].shaderList); ;}
    break;

  case 76:

/* Line 1455 of yacc.c  */
#line 1199 "Grammar.ypp"
    { (yyval.shaderList) = (yyvsp[(2) - (3)].shaderList); ;}
    break;

  case 77:

/* Line 1455 of yacc.c  */
#line 1206 "Grammar.ypp"
    { (yyval.varNameList) = NULL; ;}
    break;

  case 78:

/* Line 1455 of yacc.c  */
#line 1207 "Grammar.ypp"
    {
    (yyval.varNameList) = new VarNameList;
    (yyval.varNameList)->push_back(*(yyvsp[(1) - (1)].s));
    delete (yyvsp[(1) - (1)].s);
;}
    break;

  case 79:

/* Line 1455 of yacc.c  */
#line 1212 "Grammar.ypp"
    {
    (yyval.varNameList) = (yyvsp[(1) - (3)].varNameList);
    (yyval.varNameList)->push_back(*(yyvsp[(3) - (3)].s));
    delete (yyvsp[(3) - (3)].s);
;}
    break;

  case 82:

/* Line 1455 of yacc.c  */
#line 1237 "Grammar.ypp"
    {
    (yyval.floatList) = new FloatList;
    (yyval.floatList)->push_back((yyvsp[(1) - (1)].fval));
;}
    break;

  case 83:

/* Line 1455 of yacc.c  */
#line 1241 "Grammar.ypp"
    {
    (yyval.floatList) = new FloatList;
    (yyval.floatList)->push_back((float)(yyvsp[(1) - (1)].ival));
;}
    break;

  case 84:

/* Line 1455 of yacc.c  */
#line 1245 "Grammar.ypp"
    {
    (yyval.floatList) = (yyvsp[(1) - (3)].floatList);
    (yyval.floatList)->push_back((yyvsp[(3) - (3)].fval));
;}
    break;

  case 85:

/* Line 1455 of yacc.c  */
#line 1249 "Grammar.ypp"
    {
    (yyval.floatList) = (yyvsp[(1) - (3)].floatList);
    (yyval.floatList)->push_back((float)(yyvsp[(3) - (3)].ival));
;}
    break;

  case 86:

/* Line 1455 of yacc.c  */
#line 1253 "Grammar.ypp"
    { 
    (yyval.floatList) = (yyvsp[(1) - (2)].floatList);
    yyerror ("Error in the list of floats");
;}
    break;

  case 87:

/* Line 1455 of yacc.c  */
#line 1259 "Grammar.ypp"
    {
    (yyval.floatVecList) = new FloatVecList;
    (yyval.floatVecList)->push_back((yyvsp[(2) - (3)].floatList));
;}
    break;

  case 88:

/* Line 1455 of yacc.c  */
#line 1263 "Grammar.ypp"
    {
    (yyval.floatVecList) = (yyvsp[(1) - (5)].floatVecList);
    (yyval.floatVecList)->push_back((yyvsp[(4) - (5)].floatList));
;}
    break;

  case 89:

/* Line 1455 of yacc.c  */
#line 1267 "Grammar.ypp"
    { (yyval.floatVecList) = (yyvsp[(1) - (2)].floatVecList); yyerror ("Error in the list of default values"); ;}
    break;

  case 90:

/* Line 1455 of yacc.c  */
#line 1270 "Grammar.ypp"
    {
    (yyval.intList) = new IntList;
    (yyval.intList)->push_back((yyvsp[(1) - (1)].ival));
;}
    break;

  case 91:

/* Line 1455 of yacc.c  */
#line 1279 "Grammar.ypp"
    {
    (yyval.intList) = (yyvsp[(1) - (3)].intList);
    (yyval.intList)->push_back((yyvsp[(3) - (3)].ival));
;}
    break;

  case 92:

/* Line 1455 of yacc.c  */
#line 1290 "Grammar.ypp"
    {
    (yyval.intVecList) = new IntVecList;
    (yyval.intVecList)->push_back((yyvsp[(2) - (3)].intList));
;}
    break;

  case 93:

/* Line 1455 of yacc.c  */
#line 1294 "Grammar.ypp"
    {
    (yyval.intVecList) = (yyvsp[(1) - (5)].intVecList);
    (yyval.intVecList)->push_back((yyvsp[(4) - (5)].intList));
;}
    break;

  case 94:

/* Line 1455 of yacc.c  */
#line 1298 "Grammar.ypp"
    { yyerror ("Error in the list of default values"); ;}
    break;

  case 95:

/* Line 1455 of yacc.c  */
#line 1301 "Grammar.ypp"
    {
    (yyval.boolList) = new BoolList;
    (yyval.boolList)->push_back((yyvsp[(1) - (1)].bval));
;}
    break;

  case 96:

/* Line 1455 of yacc.c  */
#line 1305 "Grammar.ypp"
    {
    (yyval.boolList) = (yyvsp[(1) - (3)].boolList);
    (yyval.boolList)->push_back((yyvsp[(3) - (3)].bval));
;}
    break;

  case 97:

/* Line 1455 of yacc.c  */
#line 1325 "Grammar.ypp"
    { 
    (yyval.argVal) = new ArgVal(IUniform::TInt); (yyval.argVal)->val.i[0] = (yyvsp[(1) - (1)].ival); 
    LOGD("Arg val = %d\n", (yyvsp[(1) - (1)].ival));
;}
    break;

  case 98:

/* Line 1455 of yacc.c  */
#line 1329 "Grammar.ypp"
    { 
    (yyval.argVal) = new ArgVal(IUniform::TFloat); (yyval.argVal)->val.f[0] = (yyvsp[(1) - (1)].fval);
    LOGD("Arg val = %f\n", (yyvsp[(1) - (1)].fval));
;}
    break;

  case 99:

/* Line 1455 of yacc.c  */
#line 1333 "Grammar.ypp"
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
#line 1362 "Grammar.ypp"
    { 
    yyerror("Error in argument\n");
;}
    break;

  case 101:

/* Line 1455 of yacc.c  */
#line 1367 "Grammar.ypp"
    { (yyval.argValList) = new ArgValList; ;}
    break;

  case 102:

/* Line 1455 of yacc.c  */
#line 1368 "Grammar.ypp"
    { (yyval.argValList) = new ArgValList; ;}
    break;

  case 103:

/* Line 1455 of yacc.c  */
#line 1369 "Grammar.ypp"
    {
    (yyval.argValList) = new ArgValList;
    (yyval.argValList)->push_back((yyvsp[(1) - (1)].argVal));
    ;}
    break;

  case 104:

/* Line 1455 of yacc.c  */
#line 1373 "Grammar.ypp"
    {
    (yyval.argValList) = (yyvsp[(1) - (3)].argValList);
    (yyval.argValList)->push_back((yyvsp[(3) - (3)].argVal));
    ;}
    break;

  case 105:

/* Line 1455 of yacc.c  */
#line 1377 "Grammar.ypp"
    { yyerror("Error in argument list\n");;}
    break;

  case 207:

/* Line 1455 of yacc.c  */
#line 1499 "Grammar.ypp"
    { (yyval.clear_mode) = (yyvsp[(1) - (1)].clear_mode); ;}
    break;

  case 208:

/* Line 1455 of yacc.c  */
#line 1500 "Grammar.ypp"
    { (yyval.clear_mode) = (ClearMode)((unsigned int)(yyvsp[(3) - (3)].clear_mode) | (unsigned int)(yyvsp[(1) - (3)].clear_mode)); ;}
    break;

  case 212:

/* Line 1455 of yacc.c  */
#line 1510 "Grammar.ypp"
    {
    //if($4->size() > 0)
    //    curPass->createState(IPassState::TVertexShader, & (*$4)[0], (int)$4->size());
    delete (yyvsp[(4) - (5)].s);
    ;}
    break;

  case 213:

/* Line 1455 of yacc.c  */
#line 1515 "Grammar.ypp"
    {
    //if($4->size() > 0)
    //    curPass->createState(IPassState::TVertexShader, & (*$4)[0], (int)$4->size());
    ;}
    break;

  case 214:

/* Line 1455 of yacc.c  */
#line 1519 "Grammar.ypp"
    {
    if((yyvsp[(4) - (5)].shaderList)->size() > 0)
        curPass->createState(IPassState::TVertexShader, & (*(yyvsp[(4) - (5)].shaderList))[0], (int)(yyvsp[(4) - (5)].shaderList)->size());
    delete (yyvsp[(4) - (5)].shaderList);
    ;}
    break;

  case 215:

/* Line 1455 of yacc.c  */
#line 1524 "Grammar.ypp"
    {
    if((yyvsp[(7) - (8)].shaderList)->size() > 0)
        curPass->createState(IPassState::TVertexShader, & (*(yyvsp[(7) - (8)].shaderList))[0], (int)(yyvsp[(7) - (8)].shaderList)->size(), (yyvsp[(4) - (8)].s)->c_str());
    delete (yyvsp[(4) - (8)].s);
    delete (yyvsp[(7) - (8)].shaderList);
    ;}
    break;

  case 216:

/* Line 1455 of yacc.c  */
#line 1530 "Grammar.ypp"
    {
    if((yyvsp[(4) - (5)].shaderList)->size()>0)
        curPass->createState(IPassState::TFragmentShader, & (*(yyvsp[(4) - (5)].shaderList))[0], (int)(yyvsp[(4) - (5)].shaderList)->size());
    delete (yyvsp[(4) - (5)].shaderList);
    ;}
    break;

  case 217:

/* Line 1455 of yacc.c  */
#line 1535 "Grammar.ypp"
    {
    if((yyvsp[(7) - (8)].shaderList)->size()>0)
        curPass->createState(IPassState::TFragmentShader, & (*(yyvsp[(7) - (8)].shaderList))[0], (int)(yyvsp[(7) - (8)].shaderList)->size(), (yyvsp[(4) - (8)].s)->c_str());
    delete (yyvsp[(4) - (8)].s);
    delete (yyvsp[(7) - (8)].shaderList);
    ;}
    break;

  case 218:

/* Line 1455 of yacc.c  */
#line 1541 "Grammar.ypp"
    {
    if((yyvsp[(4) - (5)].shaderList)->size() > 0)
        curPass->createState(IPassState::TComputeShader, & (*(yyvsp[(4) - (5)].shaderList))[0], (int)(yyvsp[(4) - (5)].shaderList)->size());
    delete (yyvsp[(4) - (5)].shaderList);
    ;}
    break;

  case 219:

/* Line 1455 of yacc.c  */
#line 1546 "Grammar.ypp"
    {
    if((yyvsp[(7) - (8)].shaderList)->size() > 0)
        curPass->createState(IPassState::TComputeShader, & (*(yyvsp[(7) - (8)].shaderList))[0], (int)(yyvsp[(7) - (8)].shaderList)->size(), (yyvsp[(4) - (8)].s)->c_str());
    delete (yyvsp[(4) - (8)].s);
    delete (yyvsp[(7) - (8)].shaderList);
    ;}
    break;

  case 220:

/* Line 1455 of yacc.c  */
#line 1552 "Grammar.ypp"
    {
    if((yyvsp[(4) - (5)].shaderList)->size() > 0)
        curPass->createState(IPassState::TPathShader, & (*(yyvsp[(4) - (5)].shaderList))[0], (int)(yyvsp[(4) - (5)].shaderList)->size());
    delete (yyvsp[(4) - (5)].shaderList);
    ;}
    break;

  case 221:

/* Line 1455 of yacc.c  */
#line 1557 "Grammar.ypp"
    {
    if((yyvsp[(7) - (8)].shaderList)->size() > 0)
        curPass->createState(IPassState::TPathShader, & (*(yyvsp[(7) - (8)].shaderList))[0], (int)(yyvsp[(7) - (8)].shaderList)->size(), (yyvsp[(4) - (8)].s)->c_str());
    delete (yyvsp[(4) - (8)].s);
    delete (yyvsp[(7) - (8)].shaderList);
    ;}
    break;

  case 222:

/* Line 1455 of yacc.c  */
#line 1563 "Grammar.ypp"
    {
    if((yyvsp[(4) - (5)].shaderList)->size() > 0)
        curPass->createState(IPassState::TGeometryShader, & (*(yyvsp[(4) - (5)].shaderList))[0], (int)(yyvsp[(4) - (5)].shaderList)->size());
    delete (yyvsp[(4) - (5)].shaderList);
    ;}
    break;

  case 223:

/* Line 1455 of yacc.c  */
#line 1568 "Grammar.ypp"
    {
    if((yyvsp[(7) - (8)].shaderList)->size() > 0)
        curPass->createState(IPassState::TGeometryShader, & (*(yyvsp[(7) - (8)].shaderList))[0], (int)(yyvsp[(7) - (8)].shaderList)->size(), (yyvsp[(4) - (8)].s)->c_str());
    delete (yyvsp[(4) - (8)].s);
    delete (yyvsp[(7) - (8)].shaderList);
    ;}
    break;

  case 224:

/* Line 1455 of yacc.c  */
#line 1574 "Grammar.ypp"
    {
    if((yyvsp[(4) - (5)].shaderList)->size() > 0)
        curPass->createState(IPassState::TTCSShader, & (*(yyvsp[(4) - (5)].shaderList))[0], (int)(yyvsp[(4) - (5)].shaderList)->size());
    delete (yyvsp[(4) - (5)].shaderList);
    ;}
    break;

  case 225:

/* Line 1455 of yacc.c  */
#line 1579 "Grammar.ypp"
    {
    if((yyvsp[(7) - (8)].shaderList)->size() > 0)
        curPass->createState(IPassState::TTCSShader, & (*(yyvsp[(7) - (8)].shaderList))[0], (int)(yyvsp[(7) - (8)].shaderList)->size(), (yyvsp[(4) - (8)].s)->c_str());
    delete (yyvsp[(4) - (8)].s);
    delete (yyvsp[(7) - (8)].shaderList);
    ;}
    break;

  case 226:

/* Line 1455 of yacc.c  */
#line 1585 "Grammar.ypp"
    {
    if((yyvsp[(4) - (5)].shaderList)->size() > 0)
        curPass->createState(IPassState::TTESShader, & (*(yyvsp[(4) - (5)].shaderList))[0], (int)(yyvsp[(4) - (5)].shaderList)->size());
    delete (yyvsp[(4) - (5)].shaderList);
    ;}
    break;

  case 227:

/* Line 1455 of yacc.c  */
#line 1590 "Grammar.ypp"
    {
    if((yyvsp[(7) - (8)].shaderList)->size() > 0)
        curPass->createState(IPassState::TTESShader, & (*(yyvsp[(7) - (8)].shaderList))[0], (int)(yyvsp[(7) - (8)].shaderList)->size(), (yyvsp[(4) - (8)].s)->c_str());
    delete (yyvsp[(4) - (8)].s);
    delete (yyvsp[(7) - (8)].shaderList);
    ;}
    break;

  case 228:

/* Line 1455 of yacc.c  */
#line 1596 "Grammar.ypp"
    {
    if((yyvsp[(4) - (5)].shaderList)->size() > 0)
        curPass->createStateOverride(IPassState::TVertexShader, & (*(yyvsp[(4) - (5)].shaderList))[0], (int)(yyvsp[(4) - (5)].shaderList)->size());
    delete (yyvsp[(4) - (5)].shaderList);
    ;}
    break;

  case 229:

/* Line 1455 of yacc.c  */
#line 1601 "Grammar.ypp"
    {
    if((yyvsp[(7) - (8)].shaderList)->size() > 0)
        curPass->createStateOverride(IPassState::TVertexShader, & (*(yyvsp[(7) - (8)].shaderList))[0], (int)(yyvsp[(7) - (8)].shaderList)->size(), (yyvsp[(4) - (8)].s)->c_str());
    delete (yyvsp[(4) - (8)].s);
    delete (yyvsp[(7) - (8)].shaderList);
    ;}
    break;

  case 230:

/* Line 1455 of yacc.c  */
#line 1607 "Grammar.ypp"
    {
    if((yyvsp[(4) - (5)].shaderList)->size()>0)
        curPass->createStateOverride(IPassState::TFragmentShader, & (*(yyvsp[(4) - (5)].shaderList))[0], (int)(yyvsp[(4) - (5)].shaderList)->size());
    delete (yyvsp[(4) - (5)].shaderList);
    ;}
    break;

  case 231:

/* Line 1455 of yacc.c  */
#line 1612 "Grammar.ypp"
    {
    if((yyvsp[(7) - (8)].shaderList)->size()>0)
        curPass->createStateOverride(IPassState::TFragmentShader, & (*(yyvsp[(7) - (8)].shaderList))[0], (int)(yyvsp[(7) - (8)].shaderList)->size(), (yyvsp[(4) - (8)].s)->c_str());
    delete (yyvsp[(4) - (8)].s);
    delete (yyvsp[(7) - (8)].shaderList);
    ;}
    break;

  case 232:

/* Line 1455 of yacc.c  */
#line 1618 "Grammar.ypp"
    {
    if((yyvsp[(4) - (5)].shaderList)->size() > 0)
        curPass->createStateOverride(IPassState::TComputeShader, & (*(yyvsp[(4) - (5)].shaderList))[0], (int)(yyvsp[(4) - (5)].shaderList)->size());
    delete (yyvsp[(4) - (5)].shaderList);
    ;}
    break;

  case 233:

/* Line 1455 of yacc.c  */
#line 1623 "Grammar.ypp"
    {
    if((yyvsp[(7) - (8)].shaderList)->size() > 0)
        curPass->createStateOverride(IPassState::TComputeShader, & (*(yyvsp[(7) - (8)].shaderList))[0], (int)(yyvsp[(7) - (8)].shaderList)->size(), (yyvsp[(4) - (8)].s)->c_str());
    delete (yyvsp[(4) - (8)].s);
    delete (yyvsp[(7) - (8)].shaderList);
    ;}
    break;

  case 234:

/* Line 1455 of yacc.c  */
#line 1629 "Grammar.ypp"
    {
    if((yyvsp[(4) - (5)].shaderList)->size() > 0)
        curPass->createStateOverride(IPassState::TPathShader, & (*(yyvsp[(4) - (5)].shaderList))[0], (int)(yyvsp[(4) - (5)].shaderList)->size());
    delete (yyvsp[(4) - (5)].shaderList);
    ;}
    break;

  case 235:

/* Line 1455 of yacc.c  */
#line 1634 "Grammar.ypp"
    {
    if((yyvsp[(7) - (8)].shaderList)->size() > 0)
        curPass->createStateOverride(IPassState::TPathShader, & (*(yyvsp[(7) - (8)].shaderList))[0], (int)(yyvsp[(7) - (8)].shaderList)->size(), (yyvsp[(4) - (8)].s)->c_str());
    delete (yyvsp[(4) - (8)].s);
    delete (yyvsp[(7) - (8)].shaderList);
    ;}
    break;

  case 236:

/* Line 1455 of yacc.c  */
#line 1640 "Grammar.ypp"
    {
    if((yyvsp[(4) - (5)].shaderList)->size() > 0)
        curPass->createStateOverride(IPassState::TGeometryShader, & (*(yyvsp[(4) - (5)].shaderList))[0], (int)(yyvsp[(4) - (5)].shaderList)->size());
    delete (yyvsp[(4) - (5)].shaderList);
    ;}
    break;

  case 237:

/* Line 1455 of yacc.c  */
#line 1645 "Grammar.ypp"
    {
    if((yyvsp[(7) - (8)].shaderList)->size() > 0)
        curPass->createStateOverride(IPassState::TGeometryShader, & (*(yyvsp[(7) - (8)].shaderList))[0], (int)(yyvsp[(7) - (8)].shaderList)->size(), (yyvsp[(4) - (8)].s)->c_str());
    delete (yyvsp[(4) - (8)].s);
    delete (yyvsp[(7) - (8)].shaderList);
    ;}
    break;

  case 238:

/* Line 1455 of yacc.c  */
#line 1651 "Grammar.ypp"
    {
    if((yyvsp[(4) - (5)].shaderList)->size() > 0)
        curPass->createStateOverride(IPassState::TTCSShader, & (*(yyvsp[(4) - (5)].shaderList))[0], (int)(yyvsp[(4) - (5)].shaderList)->size());
    delete (yyvsp[(4) - (5)].shaderList);
    ;}
    break;

  case 239:

/* Line 1455 of yacc.c  */
#line 1656 "Grammar.ypp"
    {
    if((yyvsp[(7) - (8)].shaderList)->size() > 0)
        curPass->createStateOverride(IPassState::TTCSShader, & (*(yyvsp[(7) - (8)].shaderList))[0], (int)(yyvsp[(7) - (8)].shaderList)->size(), (yyvsp[(4) - (8)].s)->c_str());
    delete (yyvsp[(4) - (8)].s);
    delete (yyvsp[(7) - (8)].shaderList);
    ;}
    break;

  case 240:

/* Line 1455 of yacc.c  */
#line 1662 "Grammar.ypp"
    {
    if((yyvsp[(4) - (5)].shaderList)->size() > 0)
        curPass->createStateOverride(IPassState::TTESShader, & (*(yyvsp[(4) - (5)].shaderList))[0], (int)(yyvsp[(4) - (5)].shaderList)->size());
    delete (yyvsp[(4) - (5)].shaderList);
    ;}
    break;

  case 241:

/* Line 1455 of yacc.c  */
#line 1667 "Grammar.ypp"
    {
    if((yyvsp[(7) - (8)].shaderList)->size() > 0)
        curPass->createStateOverride(IPassState::TTESShader, & (*(yyvsp[(7) - (8)].shaderList))[0], (int)(yyvsp[(7) - (8)].shaderList)->size(), (yyvsp[(4) - (8)].s)->c_str());
    delete (yyvsp[(4) - (8)].s);
    delete (yyvsp[(7) - (8)].shaderList);
    ;}
    break;

  case 242:

/* Line 1455 of yacc.c  */
#line 1673 "Grammar.ypp"
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
#line 1688 "Grammar.ypp"
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
#line 1697 "Grammar.ypp"
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
#line 1754 "Grammar.ypp"
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
#line 1773 "Grammar.ypp"
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
#line 1801 "Grammar.ypp"
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
#line 1810 "Grammar.ypp"
    {
    curPass->createState((yyvsp[(4) - (13)].s)->c_str(), (yyvsp[(6) - (13)].ival), &(*(yyvsp[(11) - (13)].boolList))[0], (int)(yyvsp[(11) - (13)].boolList)->size());
    delete (yyvsp[(4) - (13)].s);
    delete (yyvsp[(11) - (13)].boolList);
    ;}
    break;

  case 249:

/* Line 1455 of yacc.c  */
#line 1815 "Grammar.ypp"
    {
    yyerror("TODO: SHDPROFILE for HLSL to do...\n");
    delete (yyvsp[(4) - (5)].s);
    ;}
    break;

  case 250:

/* Line 1455 of yacc.c  */
#line 1819 "Grammar.ypp"
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
#line 1836 "Grammar.ypp"
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
#line 1853 "Grammar.ypp"
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
#line 1870 "Grammar.ypp"
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
#line 1887 "Grammar.ypp"
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
#line 1904 "Grammar.ypp"
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
#line 1921 "Grammar.ypp"
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
#line 1938 "Grammar.ypp"
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
#line 1955 "Grammar.ypp"
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
#line 1972 "Grammar.ypp"
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
#line 1989 "Grammar.ypp"
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
#line 2006 "Grammar.ypp"
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
#line 2019 "Grammar.ypp"
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
#line 2032 "Grammar.ypp"
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
#line 2045 "Grammar.ypp"
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
#line 2058 "Grammar.ypp"
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
#line 2071 "Grammar.ypp"
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
#line 2084 "Grammar.ypp"
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
#line 2105 "Grammar.ypp"
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
#line 2126 "Grammar.ypp"
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
#line 2147 "Grammar.ypp"
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
#line 2157 "Grammar.ypp"
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
#line 2167 "Grammar.ypp"
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
#line 2177 "Grammar.ypp"
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
#line 2187 "Grammar.ypp"
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
#line 2197 "Grammar.ypp"
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
#line 2210 "Grammar.ypp"
    {
    curPass->createStateRenderMode((yyvsp[(4) - (5)].render_mode));
;}
    break;

  case 277:

/* Line 1455 of yacc.c  */
#line 2213 "Grammar.ypp"
    {
    curPass->createStateRenderGroup((yyvsp[(4) - (5)].ival));
;}
    break;

  case 278:

/* Line 1455 of yacc.c  */
#line 2216 "Grammar.ypp"
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
#line 2230 "Grammar.ypp"
    {
    curPass->createStateClearMode((yyvsp[(4) - (5)].clear_mode));
;}
    break;

  case 280:

/* Line 1455 of yacc.c  */
#line 2233 "Grammar.ypp"
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
#line 2247 "Grammar.ypp"
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
#line 2257 "Grammar.ypp"
    {
    curPass->createStateClearColor(0, (*(yyvsp[(5) - (7)].floatList))[0], (*(yyvsp[(5) - (7)].floatList))[1], (*(yyvsp[(5) - (7)].floatList))[2], (*(yyvsp[(5) - (7)].floatList))[3]);
    delete (yyvsp[(5) - (7)].floatList);
;}
    break;

  case 283:

/* Line 1455 of yacc.c  */
#line 2261 "Grammar.ypp"
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
#line 2268 "Grammar.ypp"
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
#line 2289 "Grammar.ypp"
    {
    curPass->setLoopCount((yyvsp[(4) - (5)].ival));
;}
    break;

  case 286:

/* Line 1455 of yacc.c  */
#line 2292 "Grammar.ypp"
    {
    curPass->setActive((yyvsp[(4) - (5)].bval));
;}
    break;

  case 287:

/* Line 1455 of yacc.c  */
#line 2298 "Grammar.ypp"
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
#line 2313 "Grammar.ypp"
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
#line 2323 "Grammar.ypp"
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
#line 2369 "Grammar.ypp"
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
#line 2377 "Grammar.ypp"
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
#line 2394 "Grammar.ypp"
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
#line 2414 "Grammar.ypp"
    {
    curPass->bindAttribute((yyvsp[(4) - (8)].s)->c_str(), (yyvsp[(7) - (8)].ival));
    delete (yyvsp[(4) - (8)].s);
;}
    break;

  case 294:

/* Line 1455 of yacc.c  */
#line 2418 "Grammar.ypp"
    { 
    yyerror("Error in a state of the Pass\n");
;}
    break;

  case 295:

/* Line 1455 of yacc.c  */
#line 2427 "Grammar.ypp"
    {
    //printf("Creation of PASS\n");
    curPass = curTechnique->addPass()->getExInterface();
    curAnnotations = curPass->annotations()->getExInterface();
    ;}
    break;

  case 296:

/* Line 1455 of yacc.c  */
#line 2432 "Grammar.ypp"
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
#line 2441 "Grammar.ypp"
    {
    //printf("Creation of PASS %s\n", $2->c_str());
    curPass = curTechnique->addPass((yyvsp[(2) - (2)].s)->c_str())->getExInterface();
    curAnnotations = curPass->annotations()->getExInterface();
    ;}
    break;

  case 298:

/* Line 1455 of yacc.c  */
#line 2446 "Grammar.ypp"
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
#line 2459 "Grammar.ypp"
    {
    curTechnique->bindAttribute((yyvsp[(3) - (7)].s)->c_str(), (yyvsp[(6) - (7)].ival));
    delete (yyvsp[(3) - (7)].s);
;}
    break;

  case 300:

/* Line 1455 of yacc.c  */
#line 2463 "Grammar.ypp"
    {
    curTechnique->bindAttribute((yyvsp[(3) - (7)].s)->c_str(), (yyvsp[(6) - (7)].ival));
    delete (yyvsp[(3) - (7)].s);
;}
    break;

  case 301:

/* Line 1455 of yacc.c  */
#line 2467 "Grammar.ypp"
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
#line 2478 "Grammar.ypp"
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
#line 2498 "Grammar.ypp"
    { yyerror("Error in Pass list\n");;}
    break;

  case 306:

/* Line 1455 of yacc.c  */
#line 2504 "Grammar.ypp"
    {
    //printf("Creation of Technique for NO name\n");
    curTechnique = curContainer->createTechnique()->getExInterface();
    curAnnotations = curTechnique->annotations()->getExInterface();
;}
    break;

  case 307:

/* Line 1455 of yacc.c  */
#line 2508 "Grammar.ypp"
    { 
    curAnnotations = NULL;
    lex_pop_state();
;}
    break;

  case 308:

/* Line 1455 of yacc.c  */
#line 2512 "Grammar.ypp"
    {
    LOGD("creation of Technique %s...\n", (yyvsp[(2) - (2)].s)->c_str() );
    curTechnique = curContainer->createTechnique((yyvsp[(2) - (2)].s)->c_str())->getExInterface();
    curAnnotations = curTechnique->annotations()->getExInterface();
    delete (yyvsp[(2) - (2)].s);
;}
    break;

  case 309:

/* Line 1455 of yacc.c  */
#line 2517 "Grammar.ypp"
    { 
    lex_pop_state();
    curAnnotations = NULL;
;}
    break;

  case 310:

/* Line 1455 of yacc.c  */
#line 2527 "Grammar.ypp"
    { (yyval.bval)=false; ;}
    break;

  case 311:

/* Line 1455 of yacc.c  */
#line 2528 "Grammar.ypp"
    { (yyval.bval)=true; ;}
    break;

  case 312:

/* Line 1455 of yacc.c  */
#line 2533 "Grammar.ypp"
    { (yyval.bval)=true; ;}
    break;

  case 313:

/* Line 1455 of yacc.c  */
#line 2534 "Grammar.ypp"
    { (yyval.bval)=false; ;}
    break;

  case 314:

/* Line 1455 of yacc.c  */
#line 2535 "Grammar.ypp"
    { (yyval.bval)=true; ;}
    break;

  case 315:

/* Line 1455 of yacc.c  */
#line 2543 "Grammar.ypp"
    {
    if(!namespaceName.empty())
        (*(yyvsp[(1) - (1)].s)) = namespaceName + std::string("_NS_") + (*(yyvsp[(1) - (1)].s));
    (yyval.s) = (yyvsp[(1) - (1)].s);
;}
    break;

  case 316:

/* Line 1455 of yacc.c  */
#line 2552 "Grammar.ypp"
    { (yyval.global_varname_annots).global = false; (yyval.global_varname_annots).varName = NULL; ;}
    break;

  case 317:

/* Line 1455 of yacc.c  */
#line 2553 "Grammar.ypp"
    { (yyval.global_varname_annots).global = false; (yyval.global_varname_annots).varName = (yyvsp[(1) - (2)].s); ;}
    break;

  case 318:

/* Line 1455 of yacc.c  */
#line 2555 "Grammar.ypp"
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
#line 2567 "Grammar.ypp"
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
#line 2591 "Grammar.ypp"
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
#line 2612 "Grammar.ypp"
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
#line 2633 "Grammar.ypp"
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
#line 2671 "Grammar.ypp"
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
#line 2709 "Grammar.ypp"
    { (yyval.bval) = false; ;}
    break;

  case 325:

/* Line 1455 of yacc.c  */
#line 2710 "Grammar.ypp"
    { (yyval.bval) = true; ;}
    break;

  case 326:

/* Line 1455 of yacc.c  */
#line 2712 "Grammar.ypp"
    { (yyval.bval) = false; ;}
    break;

  case 327:

/* Line 1455 of yacc.c  */
#line 2713 "Grammar.ypp"
    { (yyval.bval) = true; ;}
    break;

  case 328:

/* Line 1455 of yacc.c  */
#line 2718 "Grammar.ypp"
    {
    (yyval.argument) = new Argument((yyvsp[(2) - (3)].bval) ? nvFX::TFloatPtr : nvFX::TFloat, (yyvsp[(3) - (3)].s));
    LOGD("Argument %s %s\n", (yyvsp[(2) - (3)].bval) ? "TFloatPtr" : "TFloat", (yyvsp[(3) - (3)].s)->c_str());
    delete (yyvsp[(3) - (3)].s);
;}
    break;

  case 329:

/* Line 1455 of yacc.c  */
#line 2723 "Grammar.ypp"
    {
    (yyval.argument) = new Argument((yyvsp[(1) - (4)].bval) ? ((yyvsp[(3) - (4)].bval) ? nvFX::TUIntPtr : nvFX::TUInt) : ((yyvsp[(3) - (4)].bval) ? nvFX::TIntPtr : nvFX::TInt), (yyvsp[(4) - (4)].s));
    LOGD("Argument %s %s\n", (yyvsp[(1) - (4)].bval) ? ((yyvsp[(3) - (4)].bval) ? "TUIntPtr" : "TUInt") : ((yyvsp[(3) - (4)].bval) ? "TIntPtr" : "TInt"), (yyvsp[(4) - (4)].s)->c_str() );
    delete (yyvsp[(4) - (4)].s);
;}
    break;

  case 330:

/* Line 1455 of yacc.c  */
#line 2732 "Grammar.ypp"
    { (yyval.argList) = new ArgList; ;}
    break;

  case 331:

/* Line 1455 of yacc.c  */
#line 2733 "Grammar.ypp"
    { (yyval.argList) = new ArgList; ;}
    break;

  case 332:

/* Line 1455 of yacc.c  */
#line 2734 "Grammar.ypp"
    {
    (yyval.argList) = new ArgList;
    (yyval.argList)->push_back((yyvsp[(1) - (1)].argument));
    ;}
    break;

  case 333:

/* Line 1455 of yacc.c  */
#line 2738 "Grammar.ypp"
    {
    (yyval.argList) = (yyvsp[(1) - (3)].argList);
    (yyval.argList)->push_back((yyvsp[(3) - (3)].argument));
    ;}
    break;

  case 334:

/* Line 1455 of yacc.c  */
#line 2746 "Grammar.ypp"
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
#line 2806 "Grammar.ypp"
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
#line 2848 "Grammar.ypp"
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
#line 2876 "Grammar.ypp"
    { 
    yyerror("Error in texture params declaration\n"); 
;}
    break;

  case 339:

/* Line 1455 of yacc.c  */
#line 2883 "Grammar.ypp"
    { (yyval.resourceType)=nvFX::RESOURCE_UNKNOWN; ;}
    break;

  case 340:

/* Line 1455 of yacc.c  */
#line 2884 "Grammar.ypp"
    { (yyval.resourceType)=nvFX::RESTEX_1D; ;}
    break;

  case 341:

/* Line 1455 of yacc.c  */
#line 2885 "Grammar.ypp"
    { (yyval.resourceType)=nvFX::RESTEX_2D; ;}
    break;

  case 342:

/* Line 1455 of yacc.c  */
#line 2886 "Grammar.ypp"
    { (yyval.resourceType)=nvFX::RESTEX_3D; ;}
    break;

  case 343:

/* Line 1455 of yacc.c  */
#line 2887 "Grammar.ypp"
    { (yyval.resourceType)=nvFX::RESTEX_2DRECT; ;}
    break;

  case 344:

/* Line 1455 of yacc.c  */
#line 2888 "Grammar.ypp"
    { (yyval.resourceType)=nvFX::RESTEX_CUBE_MAP; ;}
    break;

  case 345:

/* Line 1455 of yacc.c  */
#line 2890 "Grammar.ypp"
    {
    curResource = curContainer->createResource((yyvsp[(2) - (3)].s)->c_str(), (yyvsp[(1) - (3)].resourceType), true/*ADD AS nvFX token*/)->getExInterface();
    curResource->setGLTexture(0); // this will tell the resource it is made of some external resource (reference to an existing texture)
    lex_pop_state();
    delete (yyvsp[(2) - (3)].s);
    ;}
    break;

  case 346:

/* Line 1455 of yacc.c  */
#line 2896 "Grammar.ypp"
    {
    curResource = curContainer->createResource((yyvsp[(2) - (2)].s)->c_str(), (yyvsp[(1) - (2)].resourceType), true/*ADD AS nvFX token*/)->getExInterface();
    curResource->setGLTexture(0); // this will tell the resource it is made of some external resource (reference to an existing texture)
    curAnnotations = curResource->annotations()->getExInterface();
    delete (yyvsp[(2) - (2)].s);
;}
    break;

  case 347:

/* Line 1455 of yacc.c  */
#line 2901 "Grammar.ypp"
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
#line 2913 "Grammar.ypp"
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
#line 2923 "Grammar.ypp"
    {
    if(curResource)
        if(!curResource->annotations()->getExInterface()->addAnnotation((yyvsp[(2) - (5)].s)->c_str(), (yyvsp[(4) - (5)].s)->c_str()))
            yyerror("err\n");
    delete (yyvsp[(4) - (5)].s);
    ;}
    break;

  case 351:

/* Line 1455 of yacc.c  */
#line 2929 "Grammar.ypp"
    {
    if(curResource)
        curResource->setMSAAApplicationDependent();
;}
    break;

  case 352:

/* Line 1455 of yacc.c  */
#line 2933 "Grammar.ypp"
    {
    if(curResource)
        curResource->setMSAA((*(yyvsp[(5) - (7)].intList))[0], (yyvsp[(5) - (7)].intList)->size() > 1 ? (*(yyvsp[(5) - (7)].intList))[1] : 0);
    delete (yyvsp[(5) - (7)].intList);
;}
    break;

  case 353:

/* Line 1455 of yacc.c  */
#line 2938 "Grammar.ypp"
    {
    if(curResource)
        curResource->setMSAA((yyvsp[(4) - (5)].ival));
;}
    break;

  case 354:

/* Line 1455 of yacc.c  */
#line 2942 "Grammar.ypp"
    {
    if(curResource)
        curResource->setDimensionsApplicationDependent();
;}
    break;

  case 355:

/* Line 1455 of yacc.c  */
#line 2946 "Grammar.ypp"
    {
    int d = (int)(yyvsp[(5) - (7)].intList)->size();
    if(curResource)
        curResource->setDimensions((*(yyvsp[(5) - (7)].intList))[0], d > 1 ? (*(yyvsp[(5) - (7)].intList))[1] : 1, d > 2 ? (*(yyvsp[(5) - (7)].intList))[2] : 1);
    delete (yyvsp[(5) - (7)].intList);
;}
    break;

  case 356:

/* Line 1455 of yacc.c  */
#line 2952 "Grammar.ypp"
    {
    if(curResource)
        curResource->setDimensionFactor((yyvsp[(4) - (5)].fval), (yyvsp[(4) - (5)].fval));
;}
    break;

  case 357:

/* Line 1455 of yacc.c  */
#line 2956 "Grammar.ypp"
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
#line 2969 "Grammar.ypp"
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
#line 2981 "Grammar.ypp"
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
#line 2988 "Grammar.ypp"
    {
    // for the case of items within a namespace :
    // copy the name-space annotations to this one
    if(curAnnotations) curAnnotations->copy(namespaceAnnotations);
;}
    break;

  case 361:

/* Line 1455 of yacc.c  */
#line 2992 "Grammar.ypp"
    { 
    curAnnotations = NULL;
    lex_pop_state(); lex_pop_state(); 
;}
    break;

  case 362:

/* Line 1455 of yacc.c  */
#line 2996 "Grammar.ypp"
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
#line 3003 "Grammar.ypp"
    {
    // for the case of items within a namespace :
    // copy the name-space annotations to this one
    if(curAnnotations) curAnnotations->copy(namespaceAnnotations);
;}
    break;

  case 364:

/* Line 1455 of yacc.c  */
#line 3007 "Grammar.ypp"
    { 
    curAnnotations = NULL;
    lex_pop_state(); lex_pop_state(); 
;}
    break;

  case 365:

/* Line 1455 of yacc.c  */
#line 3011 "Grammar.ypp"
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
#line 3018 "Grammar.ypp"
    {
    // for the case of items within a namespace :
    // copy the name-space annotations to this one
    if(curAnnotations) curAnnotations->copy(namespaceAnnotations);
;}
    break;

  case 367:

/* Line 1455 of yacc.c  */
#line 3022 "Grammar.ypp"
    { 
    curAnnotations = NULL;
    lex_pop_state(); lex_pop_state(); 
;}
    break;

  case 369:

/* Line 1455 of yacc.c  */
#line 3031 "Grammar.ypp"
    {
    if(curFBO)
      if(!curFBO->annotations()->getExInterface()->addAnnotation((yyvsp[(2) - (5)].s)->c_str(), (yyvsp[(4) - (5)].s)->c_str()))
        yyerror("err\n");
    delete (yyvsp[(4) - (5)].s);
    ;}
    break;

  case 370:

/* Line 1455 of yacc.c  */
#line 3040 "Grammar.ypp"
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
#line 3054 "Grammar.ypp"
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
#line 3075 "Grammar.ypp"
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
#line 3092 "Grammar.ypp"
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
#line 3099 "Grammar.ypp"
    {
    // for the case of items within a namespace :
    // copy the name-space annotations to this one
    if(curAnnotations) curAnnotations->copy(namespaceAnnotations);
;}
    break;

  case 375:

/* Line 1455 of yacc.c  */
#line 3103 "Grammar.ypp"
    { 
    curAnnotations = NULL;
    lex_pop_state(); lex_pop_state(); // fbo and then fbo1 state
;}
    break;

  case 382:

/* Line 1455 of yacc.c  */
#line 3118 "Grammar.ypp"
    { yyerror("Error in Sampler State filtering\n");;}
    break;

  case 387:

/* Line 1455 of yacc.c  */
#line 3125 "Grammar.ypp"
    { yyerror("Error in Sampler State UV Addressing\n");;}
    break;

  case 389:

/* Line 1455 of yacc.c  */
#line 3133 "Grammar.ypp"
    {
    curSamplerState->getExInterface()->createState(getGLEnumValue((yyvsp[(2) - (5)].glenum)), getGLEnumValue((yyvsp[(4) - (5)].glenum)));
;}
    break;

  case 390:

/* Line 1455 of yacc.c  */
#line 3136 "Grammar.ypp"
    {
    curSamplerState->getExInterface()->createState(getGLEnumValue((yyvsp[(2) - (5)].glenum)), getGLEnumValue((yyvsp[(4) - (5)].glenum)));
;}
    break;

  case 391:

/* Line 1455 of yacc.c  */
#line 3139 "Grammar.ypp"
    {
    curSamplerState->getExInterface()->createState(getGLEnumValue((yyvsp[(2) - (5)].glenum)), getGLEnumValue((yyvsp[(4) - (5)].glenum)));
;}
    break;

  case 392:

/* Line 1455 of yacc.c  */
#line 3142 "Grammar.ypp"
    {
    curSamplerState->getExInterface()->createState(getGLEnumValue((yyvsp[(2) - (5)].glenum)), getGLEnumValue((yyvsp[(4) - (5)].glenum)));
;}
    break;

  case 393:

/* Line 1455 of yacc.c  */
#line 3145 "Grammar.ypp"
    {
    curSamplerState->getExInterface()->createState(getGLEnumValue((yyvsp[(2) - (5)].glenum)), getGLEnumValue((yyvsp[(4) - (5)].glenum)));
;}
    break;

  case 394:

/* Line 1455 of yacc.c  */
#line 3148 "Grammar.ypp"
    { yyerror("Error in one of sampler-state states\n"); ;}
    break;

  case 395:

/* Line 1455 of yacc.c  */
#line 3153 "Grammar.ypp"
    {
    LOGD("Sampler-state %s\n", (yyvsp[(2) - (2)].s)->c_str() );
    curSamplerState = curContainer->createSamplerState((yyvsp[(2) - (2)].s)->c_str())->getExInterface();
    curAnnotations = curSamplerState->annotations()->getExInterface();
    delete (yyvsp[(2) - (2)].s);
;}
    break;

  case 396:

/* Line 1455 of yacc.c  */
#line 3158 "Grammar.ypp"
    { 
    // for the case of items within a namespace :
    // copy the name-space annotations to this one
    if(curAnnotations) curAnnotations->copy(namespaceAnnotations);
    lex_pop_state(); curAnnotations = NULL;;}
    break;

  case 397:

/* Line 1455 of yacc.c  */
#line 3163 "Grammar.ypp"
    { 
    yyerror("Error sampler-state declaration\n"); 
;}
    break;

  case 398:

/* Line 1455 of yacc.c  */
#line 3178 "Grammar.ypp"
    { check_curRState(); if(!curRState->createState(getGLEnumValue((yyvsp[(1) - (4)].glenum)), ((yyvsp[(3) - (4)].fval)))) yyerror("Failed to create state!\n"); ;}
    break;

  case 399:

/* Line 1455 of yacc.c  */
#line 3179 "Grammar.ypp"
    { check_curRState(); if(!curRState->createState(getGLEnumValue((yyvsp[(1) - (4)].glenum)), (GLenum)(((yyvsp[(3) - (4)].bval) )? GL_TRUE: GL_FALSE))) yyerror("Failed to create state!\n"); ;}
    break;

  case 400:

/* Line 1455 of yacc.c  */
#line 3180 "Grammar.ypp"
    { check_curRState(); if(!curRState->createState(getGLEnumValue((yyvsp[(1) - (4)].glenum)), (GLenum)(((yyvsp[(3) - (4)].bval) )? GL_TRUE: GL_FALSE))) yyerror("Failed to create state!\n"); ;}
    break;

  case 401:

/* Line 1455 of yacc.c  */
#line 3181 "Grammar.ypp"
    { check_curRState(); if(!curRState->createState(getGLEnumValue((yyvsp[(1) - (4)].glenum)), (GLenum)(((yyvsp[(3) - (4)].bval) )? GL_TRUE: GL_FALSE))) yyerror("Failed to create state!\n"); ;}
    break;

  case 402:

/* Line 1455 of yacc.c  */
#line 3182 "Grammar.ypp"
    { check_curRState(); if(!curRState->createState(getGLEnumValue((yyvsp[(1) - (4)].glenum)), getGLEnumValue((yyvsp[(3) - (4)].glenum)))) yyerror("Failed to create state!\n"); ;}
    break;

  case 403:

/* Line 1455 of yacc.c  */
#line 3183 "Grammar.ypp"
    { check_curRState(); if(!curRState->createState(getGLEnumValue((yyvsp[(1) - (4)].glenum)), ((yyvsp[(3) - (4)].fval)))) yyerror("Failed to create state!\n"); ;}
    break;

  case 404:

/* Line 1455 of yacc.c  */
#line 3184 "Grammar.ypp"
    { check_curRState(); if(!curRState->createState(getGLEnumValue((yyvsp[(1) - (4)].glenum)), (GLenum)(((yyvsp[(3) - (4)].bval) )? GL_TRUE: GL_FALSE))) yyerror("Failed to create state!\n"); ;}
    break;

  case 405:

/* Line 1455 of yacc.c  */
#line 3186 "Grammar.ypp"
    { check_curRState(); if(!curRState->createState(getGLEnumValue((yyvsp[(1) - (4)].glenum)), (unsigned int)(yyvsp[(3) - (4)].ival))) yyerror("Failed to create state!\n"); ;}
    break;

  case 406:

/* Line 1455 of yacc.c  */
#line 3187 "Grammar.ypp"
    { check_curRState(); if(!curRState->createState(getGLEnumValue((yyvsp[(1) - (4)].glenum)), (unsigned int)(yyvsp[(3) - (4)].ival))) yyerror("Failed to create state!\n"); ;}
    break;

  case 407:

/* Line 1455 of yacc.c  */
#line 3188 "Grammar.ypp"
    { check_curRState(); if(!curRState->createState(getGLEnumValue((yyvsp[(1) - (4)].glenum)), (GLenum)((yyvsp[(3) - (4)].bval) ? GL_TRUE: GL_FALSE))) yyerror("Failed to create state!\n"); ;}
    break;

  case 408:

/* Line 1455 of yacc.c  */
#line 3189 "Grammar.ypp"
    { check_curRState(); if(!curRState->createState(getGLEnumValue((yyvsp[(1) - (4)].glenum)), (GLenum)((yyvsp[(3) - (4)].bval) ? GL_TRUE: GL_FALSE))) yyerror("Failed to create state!\n"); ;}
    break;

  case 409:

/* Line 1455 of yacc.c  */
#line 3190 "Grammar.ypp"
    { check_curRState(); if(!curRState->createState(getGLEnumValue((yyvsp[(1) - (4)].glenum)), getGLEnumValue((yyvsp[(3) - (4)].glenum)))) yyerror("Failed to create state!\n"); ;}
    break;

  case 410:

/* Line 1455 of yacc.c  */
#line 3191 "Grammar.ypp"
    { check_curRState(); if(!curRState->createState(getGLEnumValue((yyvsp[(1) - (4)].glenum)), getGLEnumValue((yyvsp[(3) - (4)].glenum)))) yyerror("Failed to create state!\n"); ;}
    break;

  case 411:

/* Line 1455 of yacc.c  */
#line 3192 "Grammar.ypp"
    { check_curRState(); if(!curRState->createState(getGLEnumValue((yyvsp[(1) - (4)].glenum)), getGLEnumValue((yyvsp[(3) - (4)].glenum)))) yyerror("Failed to create state!\n"); ;}
    break;

  case 412:

/* Line 1455 of yacc.c  */
#line 3193 "Grammar.ypp"
    { check_curRState(); if(!curRState->createState(getGLEnumValue((yyvsp[(1) - (4)].glenum)), (GLenum)((yyvsp[(3) - (4)].bval) ? GL_TRUE: GL_FALSE))) yyerror("Failed to create state!\n"); ;}
    break;

  case 413:

/* Line 1455 of yacc.c  */
#line 3194 "Grammar.ypp"
    { check_curRState(); if(!curRState->createState(getGLEnumValue((yyvsp[(1) - (8)].glenum)), getGLEnumValue((yyvsp[(4) - (8)].glenum)), getGLEnumValue((yyvsp[(6) - (8)].glenum)))) yyerror("Failed to create state!\n"); ;}
    break;

  case 414:

/* Line 1455 of yacc.c  */
#line 3195 "Grammar.ypp"
    { check_curRState(); if(!curRState->createState(getGLEnumValue((yyvsp[(1) - (4)].glenum)), (yyvsp[(3) - (4)].fval))) yyerror("Failed to create state!\n"); ;}
    break;

  case 415:

/* Line 1455 of yacc.c  */
#line 3196 "Grammar.ypp"
    { check_curRState(); if(!curRState->createState(getGLEnumValue((yyvsp[(1) - (4)].glenum)), (yyvsp[(3) - (4)].fval))) yyerror("Failed to create state!\n"); ;}
    break;

  case 416:

/* Line 1455 of yacc.c  */
#line 3197 "Grammar.ypp"
    {
    check_curRState();
    if(!curRState->createState(getGLEnumValue(GLPOLYGONOFFSETFACTOR), (yyvsp[(4) - (8)].fval))) yyerror("Failed to create state!\n");
    if(!curRState->createState(getGLEnumValue(GLPOLYGONOFFSETUNITS), (yyvsp[(6) - (8)].fval))) yyerror("Failed to create state!\n");
;}
    break;

  case 417:

/* Line 1455 of yacc.c  */
#line 3202 "Grammar.ypp"
    { check_curRState(); if(!curRState->createState(getGLEnumValue((yyvsp[(1) - (4)].glenum)), (GLenum)((yyvsp[(3) - (4)].bval) ? GL_TRUE: GL_FALSE))) yyerror("Failed to create state!\n"); ;}
    break;

  case 418:

/* Line 1455 of yacc.c  */
#line 3203 "Grammar.ypp"
    { check_curRState(); if(!curRState->createState(getGLEnumValue((yyvsp[(1) - (4)].glenum)), (GLenum)((yyvsp[(3) - (4)].bval) ? GL_TRUE: GL_FALSE))) yyerror("Failed to create state!\n"); ;}
    break;

  case 419:

/* Line 1455 of yacc.c  */
#line 3204 "Grammar.ypp"
    { check_curRState(); if(!curRState->createState(getGLEnumValue((yyvsp[(1) - (4)].glenum)), (GLenum)((yyvsp[(3) - (4)].bval) ? GL_TRUE: GL_FALSE))) yyerror("Failed to create state!\n"); ;}
    break;

  case 420:

/* Line 1455 of yacc.c  */
#line 3206 "Grammar.ypp"
    { check_curRState(); if(!curRState->createState(getGLEnumValue((yyvsp[(1) - (4)].glenum)), (GLenum)((yyvsp[(3) - (4)].bval) ? GL_TRUE: GL_FALSE))) yyerror("Failed to create state!\n"); ;}
    break;

  case 421:

/* Line 1455 of yacc.c  */
#line 3207 "Grammar.ypp"
    { check_curCSState(); if(!curCSState->createState(getGLEnumValue((yyvsp[(1) - (4)].glenum)), (GLenum)((yyvsp[(3) - (4)].bval) ? GL_TRUE: GL_FALSE))) yyerror("Failed to create state!\n"); ;}
    break;

  case 422:

/* Line 1455 of yacc.c  */
#line 3208 "Grammar.ypp"
    { check_curCSState(); if(!curCSState->createState(getGLEnumValue((yyvsp[(1) - (4)].glenum)), getGLEnumValue((yyvsp[(3) - (4)].glenum)))) yyerror("Failed to create state!\n"); ;}
    break;

  case 423:

/* Line 1455 of yacc.c  */
#line 3209 "Grammar.ypp"
    { check_curCSState(); if(!curCSState->createState(getGLEnumValue((yyvsp[(1) - (4)].glenum)), (yyvsp[(3) - (4)].fval))) yyerror("Failed to create state!\n"); ;}
    break;

  case 424:

/* Line 1455 of yacc.c  */
#line 3210 "Grammar.ypp"
    {
    check_curCSState();
    if(!curCSState->createState(getGLEnumValue(GLALPHATESTFUNC), getGLEnumValue((yyvsp[(4) - (8)].glenum)))) yyerror("Failed to create state!\n");
    if(!curCSState->createState(getGLEnumValue(GLALPHATESTREF), (yyvsp[(6) - (8)].fval))) yyerror("Failed to create state!\n");
;}
    break;

  case 425:

/* Line 1455 of yacc.c  */
#line 3215 "Grammar.ypp"
    { check_curCSState(); if(!curCSState->createState(getGLEnumValue((yyvsp[(1) - (4)].glenum)), (GLenum)((yyvsp[(3) - (4)].bval) ? GL_TRUE: GL_FALSE))) yyerror("Failed to create state!\n"); ;}
    break;

  case 426:

/* Line 1455 of yacc.c  */
#line 3216 "Grammar.ypp"
    {
    check_curCSState();
    if(!curCSState->createState(getGLEnumValue(GLBLENDSRC), getGLEnumValue((yyvsp[(4) - (8)].glenum)))) yyerror("Failed to create state!\n");
    if(!curCSState->createState(getGLEnumValue(GLBLENDDST), getGLEnumValue((yyvsp[(6) - (8)].glenum)))) yyerror("Failed to create state!\n");
;}
    break;

  case 427:

/* Line 1455 of yacc.c  */
#line 3221 "Grammar.ypp"
    {
    check_curCSState();
    if(!curCSState->createState(getGLEnumValue(GLBLENDSRCRGB), getGLEnumValue((yyvsp[(4) - (12)].glenum)))) yyerror("Failed to create state!\n");
    if(!curCSState->createState(getGLEnumValue(GLBLENDDSTRGB), getGLEnumValue((yyvsp[(6) - (12)].glenum)))) yyerror("Failed to create state!\n");
    if(!curCSState->createState(getGLEnumValue(GLBLENDSRCALPHA), getGLEnumValue((yyvsp[(8) - (12)].glenum)))) yyerror("Failed to create state!\n");
    if(!curCSState->createState(getGLEnumValue(GLBLENDDSTALPHA), getGLEnumValue((yyvsp[(10) - (12)].glenum)))) yyerror("Failed to create state!\n");
;}
    break;

  case 428:

/* Line 1455 of yacc.c  */
#line 3228 "Grammar.ypp"
    { check_curCSState(); if(!curCSState->createState(getGLEnumValue((yyvsp[(1) - (4)].glenum)), getGLEnumValue((yyvsp[(3) - (4)].glenum)))) yyerror("Failed to create state!\n"); ;}
    break;

  case 429:

/* Line 1455 of yacc.c  */
#line 3229 "Grammar.ypp"
    { check_curCSState(); if(!curCSState->createState(getGLEnumValue((yyvsp[(1) - (4)].glenum)), getGLEnumValue((yyvsp[(3) - (4)].glenum)))) yyerror("Failed to create state!\n"); ;}
    break;

  case 430:

/* Line 1455 of yacc.c  */
#line 3230 "Grammar.ypp"
    { check_curCSState(); if(!curCSState->createState(getGLEnumValue((yyvsp[(1) - (4)].glenum)), getGLEnumValue((yyvsp[(3) - (4)].glenum)))) yyerror("Failed to create state!\n"); ;}
    break;

  case 431:

/* Line 1455 of yacc.c  */
#line 3231 "Grammar.ypp"
    { check_curCSState(); if(!curCSState->createState(getGLEnumValue((yyvsp[(1) - (4)].glenum)), getGLEnumValue((yyvsp[(3) - (4)].glenum)))) yyerror("Failed to create state!\n"); ;}
    break;

  case 432:

/* Line 1455 of yacc.c  */
#line 3232 "Grammar.ypp"
    { check_curCSState(); if(!curCSState->createState(getGLEnumValue((yyvsp[(1) - (4)].glenum)), getGLEnumValue((yyvsp[(3) - (4)].glenum)))) yyerror("Failed to create state!\n"); ;}
    break;

  case 433:

/* Line 1455 of yacc.c  */
#line 3233 "Grammar.ypp"
    { check_curCSState(); if(!curCSState->createState(getGLEnumValue((yyvsp[(1) - (4)].glenum)), getGLEnumValue((yyvsp[(3) - (4)].glenum)))) yyerror("Failed to create state!\n"); ;}
    break;

  case 434:

/* Line 1455 of yacc.c  */
#line 3235 "Grammar.ypp"
    { check_curCSState(); if(!curCSState->createState(getGLEnumValue((yyvsp[(1) - (4)].glenum)), getGLEnumValue((yyvsp[(3) - (4)].glenum)))) yyerror("Failed to create state!\n"); ;}
    break;

  case 435:

/* Line 1455 of yacc.c  */
#line 3236 "Grammar.ypp"
    { check_curCSState(); if(!curCSState->createState(getGLEnumValue((yyvsp[(1) - (4)].glenum)), getGLEnumValue((yyvsp[(3) - (4)].glenum)))) yyerror("Failed to create state!\n"); ;}
    break;

  case 436:

/* Line 1455 of yacc.c  */
#line 3237 "Grammar.ypp"
    {
    check_curCSState();
    if(!curCSState->createState(getGLEnumValue(GLBLENDEQUATIONRGB), getGLEnumValue((yyvsp[(3) - (4)].glenum)))) yyerror("Failed to create state!\n");
    if(!curCSState->createState(getGLEnumValue(GLBLENDEQUATIONALPHA), getGLEnumValue((yyvsp[(3) - (4)].glenum)))) yyerror("Failed to create state!\n");
;}
    break;

  case 437:

/* Line 1455 of yacc.c  */
#line 3242 "Grammar.ypp"
    {
    check_curCSState();
    if(!curCSState->createState(getGLEnumValue(GLBLENDEQUATIONRGB), getGLEnumValue((yyvsp[(4) - (8)].glenum)))) yyerror("Failed to create state!\n");
    if(!curCSState->createState(getGLEnumValue(GLBLENDEQUATIONALPHA), getGLEnumValue((yyvsp[(6) - (8)].glenum)))) yyerror("Failed to create state!\n");
;}
    break;

  case 438:

/* Line 1455 of yacc.c  */
#line 3252 "Grammar.ypp"
    { check_curCSState(); if(!curCSState->createState(getGLEnumValue((yyvsp[(1) - (12)].glenum)), (yyvsp[(4) - (12)].fval), (yyvsp[(6) - (12)].fval), (yyvsp[(8) - (12)].fval), (yyvsp[(10) - (12)].fval))) yyerror("Failed to create state!\n"); ;}
    break;

  case 439:

/* Line 1455 of yacc.c  */
#line 3253 "Grammar.ypp"
    { check_curCSState(); if(!curCSState->createState(getGLEnumValue((yyvsp[(1) - (4)].glenum)), (GLenum)((yyvsp[(3) - (4)].bval) ? GL_TRUE: GL_FALSE))) yyerror("Failed to create state!\n"); ;}
    break;

  case 440:

/* Line 1455 of yacc.c  */
#line 3254 "Grammar.ypp"
    { check_curCSState(); if(!curCSState->createState(getGLEnumValue((yyvsp[(1) - (4)].glenum)), (GLenum)((yyvsp[(3) - (4)].bval) ? GL_TRUE: GL_FALSE))) yyerror("Failed to create state!\n"); ;}
    break;

  case 441:

/* Line 1455 of yacc.c  */
#line 3255 "Grammar.ypp"
    { check_curCSState(); if(!curCSState->createState(getGLEnumValue((yyvsp[(1) - (4)].glenum)), (GLenum)((yyvsp[(3) - (4)].bval) ? GL_TRUE: GL_FALSE))) yyerror("Failed to create state!\n"); ;}
    break;

  case 442:

/* Line 1455 of yacc.c  */
#line 3256 "Grammar.ypp"
    { check_curCSState(); if(!curCSState->createState(getGLEnumValue((yyvsp[(1) - (4)].glenum)), getGLEnumValue((yyvsp[(3) - (4)].glenum)))) yyerror("Failed to create state!\n"); ;}
    break;

  case 443:

/* Line 1455 of yacc.c  */
#line 3258 "Grammar.ypp"
    { check_curCSState(); if(!curCSState->createState(getGLEnumValue((yyvsp[(1) - (12)].glenum)), 
    (GLboolean)((yyvsp[(4) - (12)].bval) ? GL_TRUE: GL_FALSE),
    (GLboolean)((yyvsp[(6) - (12)].bval) ? GL_TRUE: GL_FALSE),
    (GLboolean)((yyvsp[(8) - (12)].bval) ? GL_TRUE: GL_FALSE),
    (GLboolean)((yyvsp[(10) - (12)].bval) ? GL_TRUE: GL_FALSE)
    )) yyerror("Failed to create state!\n"); ;}
    break;

  case 444:

/* Line 1455 of yacc.c  */
#line 3264 "Grammar.ypp"
    { check_curCSState(); if(!curCSState->createState(getGLEnumValue((yyvsp[(1) - (4)].glenum)), (GLenum)((yyvsp[(3) - (4)].bval) ? GL_TRUE: GL_FALSE))) yyerror("Failed to create state!\n"); ;}
    break;

  case 445:

/* Line 1455 of yacc.c  */
#line 3265 "Grammar.ypp"
    { check_curCSState(); if(!curCSState->createState(getGLEnumValue((yyvsp[(1) - (4)].glenum)), (GLenum)((yyvsp[(3) - (4)].bval) ? GL_TRUE: GL_FALSE))) yyerror("Failed to create state!\n"); ;}
    break;

  case 446:

/* Line 1455 of yacc.c  */
#line 3267 "Grammar.ypp"
    { check_curCSState(); if(!curCSState->createState(getGLEnumValue((yyvsp[(1) - (4)].glenum)), (GLenum)((yyvsp[(3) - (4)].bval) ? GL_TRUE: GL_FALSE))) yyerror("Failed to create state!\n"); ;}
    break;

  case 447:

/* Line 1455 of yacc.c  */
#line 3268 "Grammar.ypp"
    { check_curCSState(); if(!curCSState->createState(getGLEnumValue((yyvsp[(1) - (4)].glenum)), (GLenum)((yyvsp[(3) - (4)].bval) ? GL_TRUE: GL_FALSE))) yyerror("Failed to create state!\n"); ;}
    break;

  case 448:

/* Line 1455 of yacc.c  */
#line 3269 "Grammar.ypp"
    { check_curDSTState();if(!curDSTState->createState(getGLEnumValue((yyvsp[(1) - (4)].glenum)), (GLenum)((yyvsp[(3) - (4)].bval) ? GL_TRUE: GL_FALSE))) yyerror("Failed to create state!\n"); ;}
    break;

  case 449:

/* Line 1455 of yacc.c  */
#line 3270 "Grammar.ypp"
    { check_curDSTState(); if(!curDSTState->createState(getGLEnumValue((yyvsp[(1) - (4)].glenum)), getGLEnumValue((yyvsp[(3) - (4)].glenum)))) yyerror("Failed to create state!\n"); ;}
    break;

  case 450:

/* Line 1455 of yacc.c  */
#line 3271 "Grammar.ypp"
    { check_curDSTState(); if(!curDSTState->createState(getGLEnumValue((yyvsp[(1) - (4)].glenum)), (GLenum)((yyvsp[(3) - (4)].bval) ? GL_TRUE: GL_FALSE))) yyerror("Failed to create state!\n"); ;}
    break;

  case 451:

/* Line 1455 of yacc.c  */
#line 3272 "Grammar.ypp"
    { check_curDSTState(); if(!curDSTState->createState(getGLEnumValue((yyvsp[(1) - (4)].glenum)), (GLenum)((yyvsp[(3) - (4)].bval) ? GL_TRUE: GL_FALSE))) yyerror("Failed to create state!\n"); ;}
    break;

  case 452:

/* Line 1455 of yacc.c  */
#line 3273 "Grammar.ypp"
    { check_curDSTState(); if(!curDSTState->createState(getGLEnumValue((yyvsp[(1) - (4)].glenum)), (GLenum)((yyvsp[(3) - (4)].bval) ? GL_TRUE: GL_FALSE))) yyerror("Failed to create state!\n"); ;}
    break;

  case 453:

/* Line 1455 of yacc.c  */
#line 3275 "Grammar.ypp"
    { check_curDSTState(); if(!curDSTState->createState(getGLEnumValue((yyvsp[(1) - (8)].glenum)), (yyvsp[(4) - (8)].fval), (yyvsp[(6) - (8)].fval))) yyerror("Failed to create state!\n"); ;}
    break;

  case 454:

/* Line 1455 of yacc.c  */
#line 3276 "Grammar.ypp"
    { check_curDSTState(); if(!curDSTState->createState(getGLEnumValue((yyvsp[(1) - (4)].glenum)), (GLenum)((yyvsp[(3) - (4)].bval) ? GL_TRUE: GL_FALSE))) yyerror("Failed to create state!\n"); ;}
    break;

  case 455:

/* Line 1455 of yacc.c  */
#line 3277 "Grammar.ypp"
    { check_curDSTState(); if(!curDSTState->createState(getGLEnumValue((yyvsp[(1) - (4)].glenum)), getGLEnumValue((yyvsp[(3) - (4)].glenum)))) yyerror("Failed to create state!\n"); ;}
    break;

  case 456:

/* Line 1455 of yacc.c  */
#line 3278 "Grammar.ypp"
    { 
    check_curDSTState();
    if(!curDSTState->createState(getGLEnumValue((yyvsp[(1) - (10)].glenum)), getGLEnumValue((yyvsp[(4) - (10)].glenum)))) yyerror("Failed to create state!\n");
    if(!curDSTState->createState(getGLEnumValue(GLSTENCILREF), (unsigned int)(yyvsp[(6) - (10)].ival))) yyerror("Failed to create state!\n");
    if(!curDSTState->createState(getGLEnumValue(GLSTENCILVALUEMASK), (unsigned int)(yyvsp[(8) - (10)].ival))) yyerror("Failed to create state!\n");
;}
    break;

  case 457:

/* Line 1455 of yacc.c  */
#line 3284 "Grammar.ypp"
    { 
    check_curDSTState();
    if(!curDSTState->createState(getGLEnumValue((yyvsp[(4) - (12)].glenum) == GLFRONT ? GLSTENCILFUNC : GLSTENCILBACKFUNC), getGLEnumValue((yyvsp[(6) - (12)].glenum)))) yyerror("Failed to create state!\n");
    if(!curDSTState->createState(getGLEnumValue((yyvsp[(4) - (12)].glenum) == GLFRONT ? GLSTENCILREF : GLSTENCILBACKREF), (unsigned int)(yyvsp[(8) - (12)].ival))) yyerror("Failed to create state!\n");
    if(!curDSTState->createState(getGLEnumValue((yyvsp[(4) - (12)].glenum) == GLFRONT ? GLSTENCILVALUEMASK : GLSTENCILBACKVALUEMASK), (unsigned int)(yyvsp[(10) - (12)].ival))) yyerror("Failed to create state!\n");
;}
    break;

  case 458:

/* Line 1455 of yacc.c  */
#line 3290 "Grammar.ypp"
    { check_curDSTState(); if(!curDSTState->createState(getGLEnumValue((yyvsp[(1) - (4)].glenum)), (unsigned int)(yyvsp[(3) - (4)].ival))) yyerror("Failed to create state!\n"); ;}
    break;

  case 459:

/* Line 1455 of yacc.c  */
#line 3291 "Grammar.ypp"
    { check_curDSTState(); if(!curDSTState->createState(getGLEnumValue((yyvsp[(1) - (4)].glenum)), (unsigned int)(yyvsp[(3) - (4)].ival))) yyerror("Failed to create state!\n"); ;}
    break;

  case 460:

/* Line 1455 of yacc.c  */
#line 3292 "Grammar.ypp"
    { check_curDSTState(); if(!curDSTState->createState(getGLEnumValue((yyvsp[(1) - (4)].glenum)), (unsigned int)(yyvsp[(3) - (4)].ival))) yyerror("Failed to create state!\n"); ;}
    break;

  case 461:

/* Line 1455 of yacc.c  */
#line 3293 "Grammar.ypp"
    { check_curDSTState(); if(!curDSTState->createState(getGLEnumValue((yyvsp[(1) - (4)].glenum)), getGLEnumValue((yyvsp[(3) - (4)].glenum)))) yyerror("Failed to create state!\n"); ;}
    break;

  case 462:

/* Line 1455 of yacc.c  */
#line 3294 "Grammar.ypp"
    { check_curDSTState(); if(!curDSTState->createState(getGLEnumValue((yyvsp[(1) - (4)].glenum)), getGLEnumValue((yyvsp[(3) - (4)].glenum)))) yyerror("Failed to create state!\n"); ;}
    break;

  case 463:

/* Line 1455 of yacc.c  */
#line 3295 "Grammar.ypp"
    { check_curDSTState(); if(!curDSTState->createState(getGLEnumValue((yyvsp[(1) - (4)].glenum)), getGLEnumValue((yyvsp[(3) - (4)].glenum)))) yyerror("Failed to create state!\n"); ;}
    break;

  case 464:

/* Line 1455 of yacc.c  */
#line 3296 "Grammar.ypp"
    {
    // a concatenated version of GLSTENCILPASSDEPTHFAIL+GLSTENCILPASSDEPTHPASS+GLSTENCILWRITEMASK == GL function arguments
    check_curDSTState();
    if(!curDSTState->createState(getGLEnumValue(GLSTENCILFAIL), getGLEnumValue((yyvsp[(4) - (10)].glenum)))) yyerror("Failed to create state!\n");
    if(!curDSTState->createState(getGLEnumValue(GLSTENCILPASSDEPTHFAIL), getGLEnumValue((yyvsp[(6) - (10)].glenum)))) yyerror("Failed to create state!\n");
    if(!curDSTState->createState(getGLEnumValue(GLSTENCILPASSDEPTHPASS), getGLEnumValue((yyvsp[(8) - (10)].glenum)))) yyerror("Failed to create state!\n");
;}
    break;

  case 465:

/* Line 1455 of yacc.c  */
#line 3303 "Grammar.ypp"
    { 
    check_curDSTState();
    if(!curDSTState->createState(getGLEnumValue((yyvsp[(4) - (12)].glenum) == GLFRONT ? GLSTENCILFAIL : GLSTENCILBACKFAIL), getGLEnumValue((yyvsp[(6) - (12)].glenum)))) yyerror("Failed to create state!\n");
    if(!curDSTState->createState(getGLEnumValue((yyvsp[(4) - (12)].glenum) == GLFRONT ? GLSTENCILPASSDEPTHFAIL : GLSTENCILBACKPASSDEPTHFAIL), (unsigned int)(yyvsp[(8) - (12)].glenum))) yyerror("Failed to create state!\n");
    if(!curDSTState->createState(getGLEnumValue((yyvsp[(4) - (12)].glenum) == GLFRONT ? GLSTENCILPASSDEPTHPASS : GLSTENCILBACKPASSDEPTHPASS), (unsigned int)(yyvsp[(10) - (12)].glenum))) yyerror("Failed to create state!\n");
;}
    break;

  case 466:

/* Line 1455 of yacc.c  */
#line 3316 "Grammar.ypp"
    { check_curDSTState(); if(!curDSTState->createState(getGLEnumValue((yyvsp[(1) - (4)].glenum)), getGLEnumValue((yyvsp[(3) - (4)].glenum)))) yyerror("Failed to create state!\n"); ;}
    break;

  case 467:

/* Line 1455 of yacc.c  */
#line 3317 "Grammar.ypp"
    { 
    check_curDSTState();
    if(!curDSTState->createState(getGLEnumValue((yyvsp[(1) - (10)].glenum)), getGLEnumValue((yyvsp[(4) - (10)].glenum)))) yyerror("Failed to create state!\n");
    if(!curDSTState->createState(getGLEnumValue(GLSTENCILBACKREF), (unsigned int)(yyvsp[(6) - (10)].ival))) yyerror("Failed to create state!\n");
    if(!curDSTState->createState(getGLEnumValue(GLSTENCILBACKVALUEMASK), (unsigned int)(yyvsp[(8) - (10)].ival))) yyerror("Failed to create state!\n");
;}
    break;

  case 468:

/* Line 1455 of yacc.c  */
#line 3323 "Grammar.ypp"
    { check_curDSTState(); if(!curDSTState->createState(getGLEnumValue((yyvsp[(1) - (4)].glenum)), (unsigned int)(yyvsp[(3) - (4)].ival))) yyerror("Failed to create state!\n"); ;}
    break;

  case 469:

/* Line 1455 of yacc.c  */
#line 3324 "Grammar.ypp"
    { check_curDSTState(); if(!curDSTState->createState(getGLEnumValue((yyvsp[(1) - (4)].glenum)), (unsigned int)(yyvsp[(3) - (4)].ival))) yyerror("Failed to create state!\n"); ;}
    break;

  case 470:

/* Line 1455 of yacc.c  */
#line 3325 "Grammar.ypp"
    { check_curDSTState(); if(!curDSTState->createState(getGLEnumValue((yyvsp[(1) - (4)].glenum)), getGLEnumValue((yyvsp[(3) - (4)].glenum)))) yyerror("Failed to create state!\n"); ;}
    break;

  case 471:

/* Line 1455 of yacc.c  */
#line 3326 "Grammar.ypp"
    { check_curDSTState(); if(!curDSTState->createState(getGLEnumValue((yyvsp[(1) - (4)].glenum)), getGLEnumValue((yyvsp[(3) - (4)].glenum)))) yyerror("Failed to create state!\n"); ;}
    break;

  case 472:

/* Line 1455 of yacc.c  */
#line 3327 "Grammar.ypp"
    { check_curDSTState(); if(!curDSTState->createState(getGLEnumValue((yyvsp[(1) - (4)].glenum)), getGLEnumValue((yyvsp[(3) - (4)].glenum)))) yyerror("Failed to create state!\n"); ;}
    break;

  case 473:

/* Line 1455 of yacc.c  */
#line 3328 "Grammar.ypp"
    { check_curDSTState(); if(!curDSTState->createState(getGLEnumValue((yyvsp[(1) - (4)].glenum)), (unsigned int)(yyvsp[(3) - (4)].ival))) yyerror("Failed to create state!\n"); ;}
    break;

  case 475:

/* Line 1455 of yacc.c  */
#line 3335 "Grammar.ypp"
    { check_curRState(); if(!curRState->createState(getGLEnumValue((yyvsp[(2) - (5)].glenum)), (yyvsp[(4) - (5)].fval))) yyerror("Failed to create state!\n"); ;}
    break;

  case 476:

/* Line 1455 of yacc.c  */
#line 3336 "Grammar.ypp"
    { check_curRState(); if(!curRState->createState(getGLEnumValue((yyvsp[(2) - (5)].glenum)), (GLenum)((yyvsp[(4) - (5)].bval) ? GL_TRUE: GL_FALSE))) yyerror("Failed to create state!\n"); ;}
    break;

  case 477:

/* Line 1455 of yacc.c  */
#line 3337 "Grammar.ypp"
    { check_curRState(); if(!curRState->createState(getGLEnumValue((yyvsp[(2) - (5)].glenum)), (GLenum)((yyvsp[(4) - (5)].bval) ? GL_TRUE: GL_FALSE))) yyerror("Failed to create state!\n"); ;}
    break;

  case 478:

/* Line 1455 of yacc.c  */
#line 3338 "Grammar.ypp"
    { check_curRState(); if(!curRState->createState(getGLEnumValue((yyvsp[(2) - (5)].glenum)), (GLenum)((yyvsp[(4) - (5)].bval) ? GL_TRUE: GL_FALSE))) yyerror("Failed to create state!\n"); ;}
    break;

  case 479:

/* Line 1455 of yacc.c  */
#line 3339 "Grammar.ypp"
    { check_curRState(); if(!curRState->createState(getGLEnumValue((yyvsp[(2) - (5)].glenum)), getGLEnumValue((yyvsp[(4) - (5)].glenum)))) yyerror("Failed to create state!\n"); ;}
    break;

  case 480:

/* Line 1455 of yacc.c  */
#line 3340 "Grammar.ypp"
    { check_curRState(); if(!curRState->createState(getGLEnumValue((yyvsp[(2) - (5)].glenum)), (yyvsp[(4) - (5)].fval))) yyerror("Failed to create state!\n"); ;}
    break;

  case 481:

/* Line 1455 of yacc.c  */
#line 3341 "Grammar.ypp"
    { check_curRState(); if(!curRState->createState(getGLEnumValue((yyvsp[(2) - (5)].glenum)), (GLenum)((yyvsp[(4) - (5)].bval) ? GL_TRUE: GL_FALSE))) yyerror("Failed to create state!\n"); ;}
    break;

  case 482:

/* Line 1455 of yacc.c  */
#line 3343 "Grammar.ypp"
    { check_curRState(); if(!curRState->createState(getGLEnumValue((yyvsp[(2) - (5)].glenum)), (unsigned int)(yyvsp[(4) - (5)].ival))) yyerror("Failed to create state!\n"); ;}
    break;

  case 483:

/* Line 1455 of yacc.c  */
#line 3344 "Grammar.ypp"
    { check_curRState(); if(!curRState->createState(getGLEnumValue((yyvsp[(2) - (5)].glenum)), (unsigned int)(yyvsp[(4) - (5)].ival))) yyerror("Failed to create state!\n"); ;}
    break;

  case 484:

/* Line 1455 of yacc.c  */
#line 3345 "Grammar.ypp"
    { check_curRState(); if(!curRState->createState(getGLEnumValue((yyvsp[(2) - (5)].glenum)), (GLenum)((yyvsp[(4) - (5)].bval) ? GL_TRUE: GL_FALSE))) yyerror("Failed to create state!\n"); ;}
    break;

  case 485:

/* Line 1455 of yacc.c  */
#line 3346 "Grammar.ypp"
    { check_curRState(); if(!curRState->createState(getGLEnumValue((yyvsp[(2) - (5)].glenum)), (GLenum)((yyvsp[(4) - (5)].bval) ? GL_TRUE: GL_FALSE))) yyerror("Failed to create state!\n"); ;}
    break;

  case 486:

/* Line 1455 of yacc.c  */
#line 3347 "Grammar.ypp"
    { check_curRState(); if(!curRState->createState(getGLEnumValue((yyvsp[(2) - (5)].glenum)), getGLEnumValue((yyvsp[(4) - (5)].glenum)))) yyerror("Failed to create state!\n"); ;}
    break;

  case 487:

/* Line 1455 of yacc.c  */
#line 3348 "Grammar.ypp"
    { check_curRState(); if(!curRState->createState(getGLEnumValue((yyvsp[(2) - (5)].glenum)), getGLEnumValue((yyvsp[(4) - (5)].glenum)))) yyerror("Failed to create state!\n"); ;}
    break;

  case 488:

/* Line 1455 of yacc.c  */
#line 3349 "Grammar.ypp"
    { check_curRState(); if(!curRState->createState(getGLEnumValue((yyvsp[(2) - (5)].glenum)), (GLenum)((yyvsp[(4) - (5)].bval) ? GL_TRUE: GL_FALSE))) yyerror("Failed to create state!\n"); ;}
    break;

  case 489:

/* Line 1455 of yacc.c  */
#line 3350 "Grammar.ypp"
    { check_curRState(); if(!curRState->createState(getGLEnumValue((yyvsp[(2) - (5)].glenum)), getGLEnumValue((yyvsp[(4) - (5)].glenum)), getGLEnumValue((yyvsp[(4) - (5)].glenum)))) yyerror("Failed to create state!\n"); ;}
    break;

  case 490:

/* Line 1455 of yacc.c  */
#line 3351 "Grammar.ypp"
    { if(!curRState->createState(getGLEnumValue((yyvsp[(2) - (9)].glenum)), getGLEnumValue((yyvsp[(5) - (9)].glenum)), getGLEnumValue((yyvsp[(7) - (9)].glenum)))) yyerror("Failed to create state!\n"); ;}
    break;

  case 491:

/* Line 1455 of yacc.c  */
#line 3352 "Grammar.ypp"
    { check_curRState(); if(!curRState->createState(getGLEnumValue((yyvsp[(2) - (5)].glenum)), (yyvsp[(4) - (5)].fval))) yyerror("Failed to create state!\n"); ;}
    break;

  case 492:

/* Line 1455 of yacc.c  */
#line 3353 "Grammar.ypp"
    { check_curRState(); if(!curRState->createState(getGLEnumValue((yyvsp[(2) - (5)].glenum)), (yyvsp[(4) - (5)].fval))) yyerror("Failed to create state!\n"); ;}
    break;

  case 493:

/* Line 1455 of yacc.c  */
#line 3354 "Grammar.ypp"
    { check_curRState(); if(!curRState->createState(getGLEnumValue((yyvsp[(2) - (5)].glenum)), (GLenum)((yyvsp[(4) - (5)].bval) ? GL_TRUE: GL_FALSE))) yyerror("Failed to create state!\n"); ;}
    break;

  case 494:

/* Line 1455 of yacc.c  */
#line 3355 "Grammar.ypp"
    { check_curRState(); if(!curRState->createState(getGLEnumValue((yyvsp[(2) - (5)].glenum)), (GLenum)((yyvsp[(4) - (5)].bval) ? GL_TRUE: GL_FALSE))) yyerror("Failed to create state!\n"); ;}
    break;

  case 495:

/* Line 1455 of yacc.c  */
#line 3356 "Grammar.ypp"
    { check_curRState(); if(!curRState->createState(getGLEnumValue((yyvsp[(2) - (5)].glenum)), (GLenum)((yyvsp[(4) - (5)].bval) ? GL_TRUE: GL_FALSE))) yyerror("Failed to create state!\n"); ;}
    break;

  case 496:

/* Line 1455 of yacc.c  */
#line 3358 "Grammar.ypp"
    { check_curRState(); if(!curRState->createState(getGLEnumValue((yyvsp[(2) - (5)].glenum)), (GLenum)((yyvsp[(4) - (5)].bval) ? GL_TRUE: GL_FALSE))) yyerror("Failed to create state!\n"); ;}
    break;

  case 497:

/* Line 1455 of yacc.c  */
#line 3359 "Grammar.ypp"
    { yyerror("Error in Rasterizer-state list\n"); ;}
    break;

  case 498:

/* Line 1455 of yacc.c  */
#line 3364 "Grammar.ypp"
    {
    LOGD("Rasterization state %s\n", (yyvsp[(2) - (2)].s)->c_str() );
    curRState = curContainer->createStateGroup_Raster((yyvsp[(2) - (2)].s)->c_str())->getExInterface();
    curAnnotations = curRState->annotations()->getExInterface();
    delete (yyvsp[(2) - (2)].s);
;}
    break;

  case 499:

/* Line 1455 of yacc.c  */
#line 3369 "Grammar.ypp"
    { 
    // for the case of items within a namespace :
    // copy the name-space annotations to this one
    if(curAnnotations) curAnnotations->copy(namespaceAnnotations);
    curAnnotations = NULL; curRState = NULL; lex_pop_state(); ;}
    break;

  case 500:

/* Line 1455 of yacc.c  */
#line 3374 "Grammar.ypp"
    { yyerror("Error in Rasterizer-state declaration\n"); ;}
    break;

  case 502:

/* Line 1455 of yacc.c  */
#line 3380 "Grammar.ypp"
    { check_curCSState(); if(!curCSState->createState(getGLEnumValue((yyvsp[(2) - (5)].glenum)), (GLenum)((yyvsp[(4) - (5)].bval) ? GL_TRUE: GL_FALSE))) yyerror("Failed to create state!\n"); ;}
    break;

  case 503:

/* Line 1455 of yacc.c  */
#line 3381 "Grammar.ypp"
    { check_curCSState(); if(!curCSState->createState(getGLEnumValue((yyvsp[(2) - (5)].glenum)), getGLEnumValue((yyvsp[(4) - (5)].glenum)))) yyerror("Failed to create state!\n"); ;}
    break;

  case 504:

/* Line 1455 of yacc.c  */
#line 3382 "Grammar.ypp"
    { check_curCSState(); if(!curCSState->createState(getGLEnumValue((yyvsp[(2) - (5)].glenum)), (yyvsp[(4) - (5)].fval))) yyerror("Failed to create state!\n"); ;}
    break;

  case 505:

/* Line 1455 of yacc.c  */
#line 3383 "Grammar.ypp"
    { check_curCSState(); if(!curCSState->createState(getGLEnumValue((yyvsp[(2) - (5)].glenum)), (GLenum)((yyvsp[(4) - (5)].bval) ? GL_TRUE: GL_FALSE))) yyerror("Failed to create state!\n"); ;}
    break;

  case 506:

/* Line 1455 of yacc.c  */
#line 3384 "Grammar.ypp"
    { check_curCSState(); if(!curCSState->createState(getGLEnumValue((yyvsp[(2) - (5)].glenum)), getGLEnumValue((yyvsp[(4) - (5)].glenum)))) yyerror("Failed to create state!\n"); ;}
    break;

  case 507:

/* Line 1455 of yacc.c  */
#line 3385 "Grammar.ypp"
    { check_curCSState(); if(!curCSState->createState(getGLEnumValue((yyvsp[(2) - (5)].glenum)), getGLEnumValue((yyvsp[(4) - (5)].glenum)))) yyerror("Failed to create state!\n"); ;}
    break;

  case 508:

/* Line 1455 of yacc.c  */
#line 3386 "Grammar.ypp"
    { check_curCSState(); if(!curCSState->createState(getGLEnumValue((yyvsp[(2) - (5)].glenum)), getGLEnumValue((yyvsp[(4) - (5)].glenum)))) yyerror("Failed to create state!\n"); ;}
    break;

  case 509:

/* Line 1455 of yacc.c  */
#line 3387 "Grammar.ypp"
    { check_curCSState(); if(!curCSState->createState(getGLEnumValue((yyvsp[(2) - (5)].glenum)), getGLEnumValue((yyvsp[(4) - (5)].glenum)))) yyerror("Failed to create state!\n"); ;}
    break;

  case 510:

/* Line 1455 of yacc.c  */
#line 3388 "Grammar.ypp"
    { check_curCSState(); if(!curCSState->createState(getGLEnumValue((yyvsp[(2) - (5)].glenum)), getGLEnumValue((yyvsp[(4) - (5)].glenum)))) yyerror("Failed to create state!\n"); ;}
    break;

  case 511:

/* Line 1455 of yacc.c  */
#line 3389 "Grammar.ypp"
    { check_curCSState(); if(!curCSState->createState(getGLEnumValue((yyvsp[(2) - (5)].glenum)), getGLEnumValue((yyvsp[(4) - (5)].glenum)))) yyerror("Failed to create state!\n"); ;}
    break;

  case 512:

/* Line 1455 of yacc.c  */
#line 3391 "Grammar.ypp"
    { check_curCSState(); if(!curCSState->createState(getGLEnumValue((yyvsp[(2) - (5)].glenum)), getGLEnumValue((yyvsp[(4) - (5)].glenum)))) yyerror("Failed to create state!\n"); ;}
    break;

  case 513:

/* Line 1455 of yacc.c  */
#line 3392 "Grammar.ypp"
    { check_curCSState(); if(!curCSState->createState(getGLEnumValue((yyvsp[(2) - (5)].glenum)), getGLEnumValue((yyvsp[(4) - (5)].glenum)))) yyerror("Failed to create state!\n"); ;}
    break;

  case 514:

/* Line 1455 of yacc.c  */
#line 3393 "Grammar.ypp"
    {
    check_curCSState();
    if(!curCSState->createState(getGLEnumValue(GLBLENDEQUATIONRGB), getGLEnumValue((yyvsp[(3) - (4)].glenum)))) yyerror("Failed to create state!\n");
    if(!curCSState->createState(getGLEnumValue(GLBLENDEQUATIONALPHA), getGLEnumValue((yyvsp[(3) - (4)].glenum)))) yyerror("Failed to create state!\n");
;}
    break;

  case 515:

/* Line 1455 of yacc.c  */
#line 3398 "Grammar.ypp"
    {
    check_curCSState();
    if(!curCSState->createState(getGLEnumValue(GLBLENDEQUATIONRGB), getGLEnumValue((yyvsp[(4) - (8)].glenum)))) yyerror("Failed to create state!\n");
    if(!curCSState->createState(getGLEnumValue(GLBLENDEQUATIONALPHA), getGLEnumValue((yyvsp[(6) - (8)].glenum)))) yyerror("Failed to create state!\n");
;}
    break;

  case 516:

/* Line 1455 of yacc.c  */
#line 3408 "Grammar.ypp"
    { check_curCSState(); if(!curCSState->createState(getGLEnumValue((yyvsp[(2) - (13)].glenum)), (yyvsp[(5) - (13)].fval), (yyvsp[(7) - (13)].fval), (yyvsp[(9) - (13)].fval), (yyvsp[(11) - (13)].fval))) yyerror("Failed to create state!\n"); ;}
    break;

  case 517:

/* Line 1455 of yacc.c  */
#line 3409 "Grammar.ypp"
    { check_curCSState(); if(!curCSState->createState(getGLEnumValue((yyvsp[(2) - (5)].glenum)), (GLenum)((yyvsp[(4) - (5)].bval) ? GL_TRUE: GL_FALSE))) yyerror("Failed to create state!\n"); ;}
    break;

  case 518:

/* Line 1455 of yacc.c  */
#line 3410 "Grammar.ypp"
    { check_curCSState(); if(!curCSState->createState(getGLEnumValue((yyvsp[(2) - (5)].glenum)), (GLenum)((yyvsp[(4) - (5)].bval) ? GL_TRUE: GL_FALSE))) yyerror("Failed to create state!\n"); ;}
    break;

  case 519:

/* Line 1455 of yacc.c  */
#line 3411 "Grammar.ypp"
    { check_curCSState(); if(!curCSState->createState(getGLEnumValue((yyvsp[(2) - (5)].glenum)), (GLenum)((yyvsp[(4) - (5)].bval) ? GL_TRUE: GL_FALSE))) yyerror("Failed to create state!\n"); ;}
    break;

  case 520:

/* Line 1455 of yacc.c  */
#line 3412 "Grammar.ypp"
    { check_curCSState(); if(!curCSState->createState(getGLEnumValue((yyvsp[(2) - (5)].glenum)), getGLEnumValue((yyvsp[(4) - (5)].glenum)))) yyerror("Failed to create state!\n"); ;}
    break;

  case 521:

/* Line 1455 of yacc.c  */
#line 3414 "Grammar.ypp"
    { check_curCSState(); if(!curCSState->createState(getGLEnumValue((yyvsp[(2) - (13)].glenum)), 
    (GLboolean)((yyvsp[(5) - (13)].bval) ? GL_TRUE: GL_FALSE),
    (GLboolean)((yyvsp[(7) - (13)].bval) ? GL_TRUE: GL_FALSE),
    (GLboolean)((yyvsp[(9) - (13)].bval) ? GL_TRUE: GL_FALSE),
    (GLboolean)((yyvsp[(11) - (13)].bval) ? GL_TRUE: GL_FALSE)
    )) yyerror("Failed to create state!\n"); ;}
    break;

  case 522:

/* Line 1455 of yacc.c  */
#line 3420 "Grammar.ypp"
    { check_curCSState(); if(!curCSState->createState(getGLEnumValue((yyvsp[(2) - (5)].glenum)), (GLenum)((yyvsp[(4) - (5)].bval) ? GL_TRUE: GL_FALSE))) yyerror("Failed to create state!\n"); ;}
    break;

  case 523:

/* Line 1455 of yacc.c  */
#line 3421 "Grammar.ypp"
    { check_curCSState(); if(!curCSState->createState(getGLEnumValue((yyvsp[(2) - (5)].glenum)), (GLenum)((yyvsp[(4) - (5)].bval) ? GL_TRUE: GL_FALSE))) yyerror("Failed to create state!\n"); ;}
    break;

  case 524:

/* Line 1455 of yacc.c  */
#line 3423 "Grammar.ypp"
    { check_curCSState(); if(!curCSState->createState(getGLEnumValue((yyvsp[(2) - (5)].glenum)), (GLenum)((yyvsp[(4) - (5)].bval) ? GL_TRUE: GL_FALSE))) yyerror("Failed to create state!\n"); ;}
    break;

  case 525:

/* Line 1455 of yacc.c  */
#line 3424 "Grammar.ypp"
    { check_curCSState(); if(!curCSState->createState(getGLEnumValue((yyvsp[(2) - (5)].glenum)), (GLenum)((yyvsp[(4) - (5)].bval) ? GL_TRUE: GL_FALSE))) yyerror("Failed to create state!\n"); ;}
    break;

  case 526:

/* Line 1455 of yacc.c  */
#line 3425 "Grammar.ypp"
    { yyerror("Error in Color sample state list\n"); ;}
    break;

  case 527:

/* Line 1455 of yacc.c  */
#line 3430 "Grammar.ypp"
    {
    LOGD("Color sample state %s\n", (yyvsp[(2) - (2)].s)->c_str() );
    curCSState = curContainer->createStateGroup_CS((yyvsp[(2) - (2)].s)->c_str())->getExInterface();
    curAnnotations = curCSState->annotations()->getExInterface();
    delete (yyvsp[(2) - (2)].s);
;}
    break;

  case 528:

/* Line 1455 of yacc.c  */
#line 3435 "Grammar.ypp"
    { 
    // for the case of items within a namespace :
    // copy the name-space annotations to this one
    if(curAnnotations) curAnnotations->copy(namespaceAnnotations);
    curAnnotations = NULL; curCSState = NULL; lex_pop_state(); ;}
    break;

  case 529:

/* Line 1455 of yacc.c  */
#line 3440 "Grammar.ypp"
    { yyerror("Error in color sampler state declaration\n"); ;}
    break;

  case 531:

/* Line 1455 of yacc.c  */
#line 3446 "Grammar.ypp"
    { check_curDSTState(); if(!curDSTState->createState(getGLEnumValue((yyvsp[(2) - (5)].glenum)), (GLenum)((yyvsp[(4) - (5)].bval) ? GL_TRUE: GL_FALSE))) yyerror("Failed to create state!\n"); ;}
    break;

  case 532:

/* Line 1455 of yacc.c  */
#line 3447 "Grammar.ypp"
    { check_curDSTState(); if(!curDSTState->createState(getGLEnumValue((yyvsp[(2) - (5)].glenum)), getGLEnumValue((yyvsp[(4) - (5)].glenum)))) yyerror("Failed to create state!\n"); ;}
    break;

  case 533:

/* Line 1455 of yacc.c  */
#line 3448 "Grammar.ypp"
    { check_curDSTState(); if(!curDSTState->createState(getGLEnumValue((yyvsp[(2) - (5)].glenum)), (GLenum)((yyvsp[(4) - (5)].bval) ? GL_TRUE: GL_FALSE))) yyerror("Failed to create state!\n"); ;}
    break;

  case 534:

/* Line 1455 of yacc.c  */
#line 3449 "Grammar.ypp"
    { check_curDSTState(); if(!curDSTState->createState(getGLEnumValue((yyvsp[(2) - (5)].glenum)), (GLenum)((yyvsp[(4) - (5)].bval) ? GL_TRUE: GL_FALSE))) yyerror("Failed to create state!\n"); ;}
    break;

  case 535:

/* Line 1455 of yacc.c  */
#line 3450 "Grammar.ypp"
    { check_curDSTState(); if(!curDSTState->createState(getGLEnumValue((yyvsp[(2) - (5)].glenum)), (GLenum)((yyvsp[(4) - (5)].bval) ? GL_TRUE: GL_FALSE))) yyerror("Failed to create state!\n"); ;}
    break;

  case 536:

/* Line 1455 of yacc.c  */
#line 3452 "Grammar.ypp"
    { check_curDSTState(); if(!curDSTState->createState(getGLEnumValue((yyvsp[(2) - (9)].glenum)), (yyvsp[(5) - (9)].fval), (yyvsp[(7) - (9)].fval))) yyerror("Failed to create state!\n"); ;}
    break;

  case 537:

/* Line 1455 of yacc.c  */
#line 3453 "Grammar.ypp"
    { check_curDSTState(); if(!curDSTState->createState(getGLEnumValue((yyvsp[(2) - (5)].glenum)), (GLenum)((yyvsp[(4) - (5)].bval) ? GL_TRUE: GL_FALSE))) yyerror("Failed to create state!\n"); ;}
    break;

  case 538:

/* Line 1455 of yacc.c  */
#line 3454 "Grammar.ypp"
    { check_curDSTState(); if(!curDSTState->createState(getGLEnumValue((yyvsp[(2) - (5)].glenum)), getGLEnumValue((yyvsp[(4) - (5)].glenum)))) yyerror("Failed to create state!\n"); ;}
    break;

  case 539:

/* Line 1455 of yacc.c  */
#line 3455 "Grammar.ypp"
    { check_curDSTState(); if(!curDSTState->createState(getGLEnumValue((yyvsp[(2) - (5)].glenum)), (unsigned int)(yyvsp[(4) - (5)].ival))) yyerror("Failed to create state!\n"); ;}
    break;

  case 540:

/* Line 1455 of yacc.c  */
#line 3456 "Grammar.ypp"
    { check_curDSTState(); if(!curDSTState->createState(getGLEnumValue((yyvsp[(2) - (5)].glenum)), (unsigned int)(yyvsp[(4) - (5)].ival))) yyerror("Failed to create state!\n"); ;}
    break;

  case 541:

/* Line 1455 of yacc.c  */
#line 3457 "Grammar.ypp"
    { check_curDSTState(); if(!curDSTState->createState(getGLEnumValue((yyvsp[(2) - (5)].glenum)), getGLEnumValue((yyvsp[(4) - (5)].glenum)))) yyerror("Failed to create state!\n"); ;}
    break;

  case 542:

/* Line 1455 of yacc.c  */
#line 3458 "Grammar.ypp"
    { check_curDSTState(); if(!curDSTState->createState(getGLEnumValue((yyvsp[(2) - (5)].glenum)), getGLEnumValue((yyvsp[(4) - (5)].glenum)))) yyerror("Failed to create state!\n"); ;}
    break;

  case 543:

/* Line 1455 of yacc.c  */
#line 3459 "Grammar.ypp"
    { check_curDSTState(); if(!curDSTState->createState(getGLEnumValue((yyvsp[(2) - (5)].glenum)), getGLEnumValue((yyvsp[(4) - (5)].glenum)))) yyerror("Failed to create state!\n"); ;}
    break;

  case 544:

/* Line 1455 of yacc.c  */
#line 3460 "Grammar.ypp"
    { check_curDSTState(); if(!curDSTState->createState(getGLEnumValue((yyvsp[(2) - (5)].glenum)), (unsigned int)(yyvsp[(4) - (5)].ival))) yyerror("Failed to create state!\n"); ;}
    break;

  case 545:

/* Line 1455 of yacc.c  */
#line 3468 "Grammar.ypp"
    { check_curDSTState(); if(!curDSTState->createState(getGLEnumValue((yyvsp[(2) - (5)].glenum)), getGLEnumValue((yyvsp[(4) - (5)].glenum)))) yyerror("Failed to create state!\n"); ;}
    break;

  case 546:

/* Line 1455 of yacc.c  */
#line 3469 "Grammar.ypp"
    { check_curDSTState(); if(!curDSTState->createState(getGLEnumValue((yyvsp[(2) - (5)].glenum)), (unsigned int)(yyvsp[(4) - (5)].ival))) yyerror("Failed to create state!\n"); ;}
    break;

  case 547:

/* Line 1455 of yacc.c  */
#line 3470 "Grammar.ypp"
    { check_curDSTState(); if(!curDSTState->createState(getGLEnumValue((yyvsp[(2) - (5)].glenum)), (unsigned int)(yyvsp[(4) - (5)].ival))) yyerror("Failed to create state!\n"); ;}
    break;

  case 548:

/* Line 1455 of yacc.c  */
#line 3471 "Grammar.ypp"
    { check_curDSTState(); if(!curDSTState->createState(getGLEnumValue((yyvsp[(2) - (5)].glenum)), getGLEnumValue((yyvsp[(4) - (5)].glenum)))) yyerror("Failed to create state!\n"); ;}
    break;

  case 549:

/* Line 1455 of yacc.c  */
#line 3472 "Grammar.ypp"
    { check_curDSTState(); if(!curDSTState->createState(getGLEnumValue((yyvsp[(2) - (5)].glenum)), getGLEnumValue((yyvsp[(4) - (5)].glenum)))) yyerror("Failed to create state!\n"); ;}
    break;

  case 550:

/* Line 1455 of yacc.c  */
#line 3473 "Grammar.ypp"
    { check_curDSTState(); if(!curDSTState->createState(getGLEnumValue((yyvsp[(2) - (5)].glenum)), getGLEnumValue((yyvsp[(4) - (5)].glenum)))) yyerror("Failed to create state!\n"); ;}
    break;

  case 551:

/* Line 1455 of yacc.c  */
#line 3474 "Grammar.ypp"
    { check_curDSTState(); if(!curDSTState->createState(getGLEnumValue((yyvsp[(2) - (5)].glenum)), (unsigned int)(yyvsp[(4) - (5)].ival))) yyerror("Failed to create state!\n"); ;}
    break;

  case 552:

/* Line 1455 of yacc.c  */
#line 3475 "Grammar.ypp"
    { yyerror("Error in Depth Stencil state list\n"); ;}
    break;

  case 553:

/* Line 1455 of yacc.c  */
#line 3480 "Grammar.ypp"
    {
    LOGD("Depth-Stencil state %s\n", (yyvsp[(2) - (2)].s)->c_str() );
    curDSTState = curContainer->createStateGroup_DST((yyvsp[(2) - (2)].s)->c_str())->getExInterface();
    curAnnotations = curDSTState->annotations()->getExInterface();
    delete (yyvsp[(2) - (2)].s);
;}
    break;

  case 554:

/* Line 1455 of yacc.c  */
#line 3485 "Grammar.ypp"
    { 
    // for the case of items within a namespace :
    // copy the name-space annotations to this one
    if(curAnnotations) curAnnotations->copy(namespaceAnnotations);
    curAnnotations = NULL; curDSTState = NULL; lex_pop_state(); ;}
    break;

  case 555:

/* Line 1455 of yacc.c  */
#line 3490 "Grammar.ypp"
    { yyerror("Error in DST-state declaration\n"); ;}
    break;

  case 556:

/* Line 1455 of yacc.c  */
#line 3493 "Grammar.ypp"
    { LOGI("TODO"); ;}
    break;

  case 557:

/* Line 1455 of yacc.c  */
#line 3494 "Grammar.ypp"
    { LOGI("TODO"); ;}
    break;

  case 558:

/* Line 1455 of yacc.c  */
#line 3495 "Grammar.ypp"
    { LOGI("TODO"); ;}
    break;

  case 559:

/* Line 1455 of yacc.c  */
#line 3496 "Grammar.ypp"
    { LOGI("TODO"); ;}
    break;

  case 560:

/* Line 1455 of yacc.c  */
#line 3497 "Grammar.ypp"
    { LOGI("TODO"); ;}
    break;

  case 561:

/* Line 1455 of yacc.c  */
#line 3498 "Grammar.ypp"
    { LOGI("TODO"); ;}
    break;

  case 562:

/* Line 1455 of yacc.c  */
#line 3499 "Grammar.ypp"
    { LOGI("TODO"); ;}
    break;

  case 563:

/* Line 1455 of yacc.c  */
#line 3500 "Grammar.ypp"
    { LOGI("TODO"); ;}
    break;

  case 564:

/* Line 1455 of yacc.c  */
#line 3501 "Grammar.ypp"
    { LOGI("TODO"); ;}
    break;

  case 565:

/* Line 1455 of yacc.c  */
#line 3502 "Grammar.ypp"
    { LOGI("TODO"); ;}
    break;

  case 566:

/* Line 1455 of yacc.c  */
#line 3503 "Grammar.ypp"
    { LOGI("TODO"); ;}
    break;

  case 567:

/* Line 1455 of yacc.c  */
#line 3504 "Grammar.ypp"
    { LOGI("TODO"); ;}
    break;

  case 568:

/* Line 1455 of yacc.c  */
#line 3505 "Grammar.ypp"
    { LOGI("TODO"); ;}
    break;

  case 569:

/* Line 1455 of yacc.c  */
#line 3506 "Grammar.ypp"
    { LOGI("TODO"); ;}
    break;

  case 570:

/* Line 1455 of yacc.c  */
#line 3507 "Grammar.ypp"
    { LOGI("TODO"); ;}
    break;

  case 571:

/* Line 1455 of yacc.c  */
#line 3508 "Grammar.ypp"
    { LOGI("TODO"); ;}
    break;

  case 572:

/* Line 1455 of yacc.c  */
#line 3509 "Grammar.ypp"
    { LOGI("TODO"); ;}
    break;

  case 573:

/* Line 1455 of yacc.c  */
#line 3510 "Grammar.ypp"
    { LOGI("TODO"); ;}
    break;

  case 574:

/* Line 1455 of yacc.c  */
#line 3511 "Grammar.ypp"
    { LOGI("TODO"); ;}
    break;

  case 575:

/* Line 1455 of yacc.c  */
#line 3512 "Grammar.ypp"
    { LOGI("TODO"); ;}
    break;

  case 576:

/* Line 1455 of yacc.c  */
#line 3513 "Grammar.ypp"
    { LOGI("TODO"); ;}
    break;

  case 577:

/* Line 1455 of yacc.c  */
#line 3514 "Grammar.ypp"
    { LOGI("TODO"); ;}
    break;

  case 579:

/* Line 1455 of yacc.c  */
#line 3523 "Grammar.ypp"
    { LOGI("TODO"); ;}
    break;

  case 580:

/* Line 1455 of yacc.c  */
#line 3524 "Grammar.ypp"
    { 
#ifndef OGLES2
    check_curPRState(); if(!curPRState->createState(getGLEnumValue((yyvsp[(2) - (5)].glenum)), getGLEnumValue((yyvsp[(4) - (5)].glenum)))) yyerror("Failed to create state!\n");
#endif
;}
    break;

  case 581:

/* Line 1455 of yacc.c  */
#line 3529 "Grammar.ypp"
    {
#ifndef OGLES2
    check_curPRState(); if(!curPRState->createState(getGLEnumValue((yyvsp[(2) - (5)].glenum)), getGLEnumValue((yyvsp[(4) - (5)].glenum)))) yyerror("Failed to create state!\n");
#endif
;}
    break;

  case 582:

/* Line 1455 of yacc.c  */
#line 3534 "Grammar.ypp"
    {
#ifndef OGLES2
    LOGI("TODO");
#endif
;}
    break;

  case 583:

/* Line 1455 of yacc.c  */
#line 3539 "Grammar.ypp"
    {
#ifndef OGLES2
    LOGI("TODO");
#endif
;}
    break;

  case 584:

/* Line 1455 of yacc.c  */
#line 3544 "Grammar.ypp"
    {
#ifndef OGLES2
    check_curPRState(); if(!curPRState->createState(getGLEnumValue((yyvsp[(2) - (5)].glenum)), getGLEnumValue((yyvsp[(4) - (5)].glenum)))) yyerror("Failed to create state!\n");
#endif
;}
    break;

  case 585:

/* Line 1455 of yacc.c  */
#line 3549 "Grammar.ypp"
    {
#ifndef OGLES2
    check_curPRState(); if(!curPRState->createState(getGLEnumValue((yyvsp[(2) - (5)].glenum)), (yyvsp[(4) - (5)].fval))) yyerror("Failed to create state!\n");
#endif
;}
    break;

  case 586:

/* Line 1455 of yacc.c  */
#line 3554 "Grammar.ypp"
    {
#ifndef OGLES2
    check_curPRState(); if(!curPRState->createState(getGLEnumValue((yyvsp[(2) - (5)].glenum)), getGLEnumValue((yyvsp[(4) - (5)].glenum)))) yyerror("Failed to create state!\n");
#endif
;}
    break;

  case 587:

/* Line 1455 of yacc.c  */
#line 3559 "Grammar.ypp"
    {
#ifndef OGLES2
    check_curPRState(); if(!curPRState->createState(getGLEnumValue((yyvsp[(2) - (5)].glenum)), getGLEnumValue((yyvsp[(4) - (5)].glenum)))) yyerror("Failed to create state!\n");
#endif
;}
    break;

  case 588:

/* Line 1455 of yacc.c  */
#line 3564 "Grammar.ypp"
    {
#ifndef OGLES2
    check_curPRState(); if(!curPRState->createState(getGLEnumValue((yyvsp[(2) - (5)].glenum)), getGLEnumValue((yyvsp[(4) - (5)].glenum)))) yyerror("Failed to create state!\n");
#endif
;}
    break;

  case 589:

/* Line 1455 of yacc.c  */
#line 3569 "Grammar.ypp"
    {
#ifndef OGLES2
    check_curPRState(); if(!curPRState->createState(getGLEnumValue((yyvsp[(2) - (5)].glenum)), getGLEnumValue((yyvsp[(4) - (5)].glenum)))) yyerror("Failed to create state!\n");
#endif
;}
    break;

  case 590:

/* Line 1455 of yacc.c  */
#line 3574 "Grammar.ypp"
    {
#ifndef OGLES2
    check_curPRState(); if(!curPRState->createState(getGLEnumValue((yyvsp[(2) - (5)].glenum)), getGLEnumValue((yyvsp[(4) - (5)].glenum)))) yyerror("Failed to create state!\n");
#endif
;}
    break;

  case 591:

/* Line 1455 of yacc.c  */
#line 3579 "Grammar.ypp"
    {
#ifndef OGLES2
    check_curPRState(); if(!curPRState->createState(getGLEnumValue((yyvsp[(2) - (5)].glenum)), (yyvsp[(4) - (5)].fval))) yyerror("Failed to create state!\n");
#endif
;}
    break;

  case 592:

/* Line 1455 of yacc.c  */
#line 3584 "Grammar.ypp"
    {
#ifndef OGLES2
    check_curPRState(); if(!curPRState->createState(getGLEnumValue((yyvsp[(2) - (5)].glenum)), (yyvsp[(4) - (5)].fval))) yyerror("Failed to create state!\n");
#endif
;}
    break;

  case 593:

/* Line 1455 of yacc.c  */
#line 3589 "Grammar.ypp"
    {
#ifndef OGLES2
    check_curPRState(); if(!curPRState->createState(getGLEnumValue((yyvsp[(2) - (5)].glenum)), getGLEnumValue((yyvsp[(4) - (5)].glenum)))) yyerror("Failed to create state!\n");
#endif
;}
    break;

  case 594:

/* Line 1455 of yacc.c  */
#line 3594 "Grammar.ypp"
    {
#ifndef OGLES2
    check_curPRState(); if(!curPRState->createState(getGLEnumValue((yyvsp[(2) - (5)].glenum)), (yyvsp[(4) - (5)].fval))) yyerror("Failed to create state!\n");
#endif
;}
    break;

  case 595:

/* Line 1455 of yacc.c  */
#line 3599 "Grammar.ypp"
    {
#ifndef OGLES2
    check_curPRState(); if(!curPRState->createState(getGLEnumValue((yyvsp[(2) - (5)].glenum)), getGLEnumValue((yyvsp[(4) - (5)].glenum)))) yyerror("Failed to create state!\n");
#endif
;}
    break;

  case 596:

/* Line 1455 of yacc.c  */
#line 3604 "Grammar.ypp"
    {
#ifndef OGLES2
    check_curPRState(); if(!curPRState->createState(getGLEnumValue((yyvsp[(2) - (5)].glenum)), (yyvsp[(4) - (5)].ival))) yyerror("Failed to create state!\n");
#endif
;}
    break;

  case 597:

/* Line 1455 of yacc.c  */
#line 3609 "Grammar.ypp"
    {
#ifndef OGLES2
    check_curPRState(); if(!curPRState->createState(getGLEnumValue((yyvsp[(2) - (5)].glenum)), getGLEnumValue((yyvsp[(4) - (5)].glenum)))) yyerror("Failed to create state!\n");
#endif
;}
    break;

  case 598:

/* Line 1455 of yacc.c  */
#line 3614 "Grammar.ypp"
    {
#ifndef OGLES2
    check_curPRState(); if(!curPRState->createState(getGLEnumValue((yyvsp[(2) - (5)].glenum)), getGLEnumValue((yyvsp[(4) - (5)].glenum)))) yyerror("Failed to create state!\n");
#endif
;}
    break;

  case 599:

/* Line 1455 of yacc.c  */
#line 3619 "Grammar.ypp"
    {
#ifndef OGLES2
    check_curPRState(); if(!curPRState->createState(getGLEnumValue((yyvsp[(2) - (5)].glenum)), getGLEnumValue((yyvsp[(4) - (5)].glenum)))) yyerror("Failed to create state!\n");
#endif
;}
    break;

  case 600:

/* Line 1455 of yacc.c  */
#line 3624 "Grammar.ypp"
    {
#ifndef OGLES2
    check_curPRState(); if(!curPRState->createState(getGLEnumValue((yyvsp[(2) - (5)].glenum)), getGLEnumValue((yyvsp[(4) - (5)].glenum)))) yyerror("Failed to create state!\n");
#endif
;}
    break;

  case 601:

/* Line 1455 of yacc.c  */
#line 3635 "Grammar.ypp"
    { yyerror("Error in Path-Rendering state list\n"); ;}
    break;

  case 602:

/* Line 1455 of yacc.c  */
#line 3641 "Grammar.ypp"
    {
#ifndef OGLES2
    LOGD("Path Rendering state %s\n", (yyvsp[(2) - (2)].s)->c_str() ); 
    curPRState = curContainer->createStateGroup_PR((yyvsp[(2) - (2)].s)->c_str())->getExInterface();
    curAnnotations = curPRState->annotations()->getExInterface();
#endif
    delete (yyvsp[(2) - (2)].s);
;}
    break;

  case 603:

/* Line 1455 of yacc.c  */
#line 3648 "Grammar.ypp"
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

  case 604:

/* Line 1455 of yacc.c  */
#line 3657 "Grammar.ypp"
    { yyerror("Error in DST-state declaration\n"); ;}
    break;

  case 606:

/* Line 1455 of yacc.c  */
#line 3668 "Grammar.ypp"
    {
    if(!curAnnotations)
        curAnnotations = IAnnotationEx::getAnnotationSingleton(2); // need a temporary place since nothing was initialized
    if(!curAnnotations->addAnnotation((yyvsp[(2) - (5)].s)->c_str(), (yyvsp[(4) - (5)].s)->c_str()))
        yyerror("err\n");
    delete (yyvsp[(4) - (5)].s);
    ;}
    break;

  case 607:

/* Line 1455 of yacc.c  */
#line 3675 "Grammar.ypp"
    {
    if(!curAnnotations)
        curAnnotations = IAnnotationEx::getAnnotationSingleton(2); // need a temporary place since nothing was initialized
    else if(!curAnnotations->addAnnotation((yyvsp[(2) - (5)].s)->c_str(), (yyvsp[(4) - (5)].fval)))
        yyerror("err\n");
    ;}
    break;

  case 608:

/* Line 1455 of yacc.c  */
#line 3681 "Grammar.ypp"
    {
    if(!curAnnotations)
        curAnnotations = IAnnotationEx::getAnnotationSingleton(2); // need a temporary place since nothing was initialized
    else if(!curAnnotations->addAnnotation((yyvsp[(2) - (5)].s)->c_str(), (yyvsp[(4) - (5)].ival)))
        yyerror("err\n");
    ;}
    break;

  case 609:

/* Line 1455 of yacc.c  */
#line 3687 "Grammar.ypp"
    {
    if(!curAnnotations)
        curAnnotations = IAnnotationEx::getAnnotationSingleton(2); // need a temporary place since nothing was initialized
    else if(!curAnnotations->addAnnotation((yyvsp[(2) - (5)].s)->c_str(), (yyvsp[(4) - (5)].bval)))
        yyerror("err\n");
    ;}
    break;

  case 610:

/* Line 1455 of yacc.c  */
#line 3699 "Grammar.ypp"
    { yyerror("Error in annotation\n"); ;}
    break;

  case 614:

/* Line 1455 of yacc.c  */
#line 3710 "Grammar.ypp"
    {
        LOGD("value %f\n", (yyvsp[(2) - (2)].fval) );
        FloatList* pfl = new FloatList;
        pfl->push_back((yyvsp[(2) - (2)].fval));
        (yyval.floatVecList) = new FloatVecList;
        (yyval.floatVecList)->push_back(pfl);
;}
    break;

  case 615:

/* Line 1455 of yacc.c  */
#line 3720 "Grammar.ypp"
    {
    (yyval.floatVecList) = new FloatVecList;
    (yyval.floatVecList)->push_back((yyvsp[(3) - (4)].floatList));
;}
    break;

  case 616:

/* Line 1455 of yacc.c  */
#line 3727 "Grammar.ypp"
    {
    (yyval.floatVecList) = (yyvsp[(3) - (4)].floatVecList);
;}
    break;

  case 617:

/* Line 1455 of yacc.c  */
#line 3733 "Grammar.ypp"
    {
        LOGD("value %d\n", (yyvsp[(2) - (2)].ival) );
        IntList* pfl = new IntList;
        pfl->push_back((yyvsp[(2) - (2)].ival));
        (yyval.intVecList) = new IntVecList;
        (yyval.intVecList)->push_back(pfl);
;}
    break;

  case 618:

/* Line 1455 of yacc.c  */
#line 3740 "Grammar.ypp"
    {
      LOGI("Error in valueassignment_i\n");
  ;}
    break;

  case 619:

/* Line 1455 of yacc.c  */
#line 3752 "Grammar.ypp"
    {
        LOGD("value %d\n", (yyvsp[(2) - (2)].bval) );
        BoolList* pfl = new BoolList;
        pfl->push_back((yyvsp[(2) - (2)].bval));
        (yyval.boolVecList) = new BoolVecList;
        (yyval.boolVecList)->push_back(pfl);
;}
    break;

  case 620:

/* Line 1455 of yacc.c  */
#line 3762 "Grammar.ypp"
    {
    (yyval.boolVecList) = new BoolVecList;
    (yyval.boolVecList)->push_back((yyvsp[(3) - (4)].boolList));
;}
    break;

  case 621:

/* Line 1455 of yacc.c  */
#line 3768 "Grammar.ypp"
    { (yyval.precisionType)=IUniform::PHighP; ;}
    break;

  case 622:

/* Line 1455 of yacc.c  */
#line 3769 "Grammar.ypp"
    { (yyval.precisionType)=IUniform::PMediumP; ;}
    break;

  case 623:

/* Line 1455 of yacc.c  */
#line 3770 "Grammar.ypp"
    { (yyval.precisionType)=IUniform::PLowP; ;}
    break;

  case 624:

/* Line 1455 of yacc.c  */
#line 3777 "Grammar.ypp"
    {
    (yyval.uniformQualifiers).hasUniformKeyword = true;
    (yyval.uniformQualifiers).global = false;
    (yyval.uniformQualifiers).precisionType = IUniform::PHighP;
;}
    break;

  case 625:

/* Line 1455 of yacc.c  */
#line 3782 "Grammar.ypp"
    {
    (yyval.uniformQualifiers).hasUniformKeyword = true;
    (yyval.uniformQualifiers).global = (yyvsp[(2) - (2)].bval);
    (yyval.uniformQualifiers).precisionType = IUniform::PHighP;
;}
    break;

  case 626:

/* Line 1455 of yacc.c  */
#line 3787 "Grammar.ypp"
    {
    (yyval.uniformQualifiers).hasUniformKeyword = true;
    (yyval.uniformQualifiers).global = false;
    (yyval.uniformQualifiers).precisionType = (yyvsp[(2) - (2)].precisionType);
;}
    break;

  case 627:

/* Line 1455 of yacc.c  */
#line 3792 "Grammar.ypp"
    {
    (yyval.uniformQualifiers).hasUniformKeyword = true;
    (yyval.uniformQualifiers).global = (yyvsp[(2) - (3)].bval);
    (yyval.uniformQualifiers).precisionType = (yyvsp[(3) - (3)].precisionType);
;}
    break;

  case 628:

/* Line 1455 of yacc.c  */
#line 3797 "Grammar.ypp"
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

  case 629:

/* Line 1455 of yacc.c  */
#line 3806 "Grammar.ypp"
    {
    (yyval.uniformQualifiers).hasUniformKeyword = false;
    (yyval.uniformQualifiers).global = false;
    (yyval.uniformQualifiers).precisionType = (yyvsp[(1) - (1)].precisionType);
;}
    break;

  case 630:

/* Line 1455 of yacc.c  */
#line 3811 "Grammar.ypp"
    {
    (yyval.uniformQualifiers).hasUniformKeyword = false;
    (yyval.uniformQualifiers).global = (yyvsp[(1) - (2)].bval);
    (yyval.uniformQualifiers).precisionType = (yyvsp[(2) - (2)].precisionType);
;}
    break;

  case 631:

/* Line 1455 of yacc.c  */
#line 3822 "Grammar.ypp"
    { (yyval.valueList) = new ValueList; 
    memset((yyval.valueList), 0, sizeof(ValueList));
    (yyval.valueList)->floatVecList = (yyvsp[(1) - (1)].floatVecList); ;}
    break;

  case 632:

/* Line 1455 of yacc.c  */
#line 3825 "Grammar.ypp"
    { (yyval.valueList) = new ValueList; 
    memset((yyval.valueList), 0, sizeof(ValueList));
    (yyval.valueList)->floatVecList = (yyvsp[(1) - (1)].floatVecList); ;}
    break;

  case 633:

/* Line 1455 of yacc.c  */
#line 3828 "Grammar.ypp"
    { (yyval.valueList) = new ValueList; 
    memset((yyval.valueList), 0, sizeof(ValueList));
    (yyval.valueList)->floatVecList = (yyvsp[(1) - (1)].floatVecList); ;}
    break;

  case 634:

/* Line 1455 of yacc.c  */
#line 3831 "Grammar.ypp"
    { (yyval.valueList) = new ValueList; 
    memset((yyval.valueList), 0, sizeof(ValueList));
    (yyval.valueList)->intVecList = (yyvsp[(1) - (1)].intVecList); ;}
    break;

  case 635:

/* Line 1455 of yacc.c  */
#line 3837 "Grammar.ypp"
    { (yyval.valueList) = new ValueList; 
    memset((yyval.valueList), 0, sizeof(ValueList));
    (yyval.valueList)->boolVecList = (yyvsp[(1) - (1)].boolVecList); ;}
    break;

  case 636:

/* Line 1455 of yacc.c  */
#line 3840 "Grammar.ypp"
    { (yyval.valueList) = new ValueList; 
    memset((yyval.valueList), 0, sizeof(ValueList));
    (yyval.valueList)->boolVecList = (yyvsp[(1) - (1)].boolVecList); ;}
    break;

  case 637:

/* Line 1455 of yacc.c  */
#line 3843 "Grammar.ypp"
    { 
    (yyval.valueList) = NULL; yyerror("Error in value assignment\n"); 
;}
    break;

  case 638:

/* Line 1455 of yacc.c  */
#line 3849 "Grammar.ypp"
    {
    LOGD("semantic %s\n", (yyvsp[(2) - (2)].s)->c_str() );
    (yyval.s) = (yyvsp[(2) - (2)].s);
;}
    break;

  case 639:

/* Line 1455 of yacc.c  */
#line 3856 "Grammar.ypp"
    { 
    (yyval.ival) = -1;
;}
    break;

  case 640:

/* Line 1455 of yacc.c  */
#line 3859 "Grammar.ypp"
    {
    (yyval.ival) = (yyvsp[(2) - (3)].ival);
;}
    break;

  case 641:

/* Line 1455 of yacc.c  */
#line 3870 "Grammar.ypp"
    {
    (yyval.saszsauva) = new SASzSAUva;
    memset((yyval.saszsauva), 0, sizeof(SASzSAUva));
;}
    break;

  case 642:

/* Line 1455 of yacc.c  */
#line 3874 "Grammar.ypp"
    {
    (yyval.saszsauva) = new SASzSAUva;
    memset((yyval.saszsauva), 0, sizeof(SASzSAUva));
;}
    break;

  case 643:

/* Line 1455 of yacc.c  */
#line 3878 "Grammar.ypp"
    {
    (yyval.saszsauva) = new SASzSAUva;
    memset((yyval.saszsauva), 0, sizeof(SASzSAUva));
    (yyval.saszsauva)->valueList = (yyvsp[(1) - (1)].valueList);
;}
    break;

  case 644:

/* Line 1455 of yacc.c  */
#line 3883 "Grammar.ypp"
    {
    (yyval.saszsauva) = new SASzSAUva;
    memset((yyval.saszsauva), 0, sizeof(SASzSAUva));
    (yyval.saszsauva)->valueList = (yyvsp[(2) - (2)].valueList);
;}
    break;

  case 645:

/* Line 1455 of yacc.c  */
#line 3888 "Grammar.ypp"
    {
    (yyval.saszsauva) = new SASzSAUva;
    memset((yyval.saszsauva), 0, sizeof(SASzSAUva));
    (yyval.saszsauva)->semantic = (yyvsp[(1) - (1)].s);
;}
    break;

  case 646:

/* Line 1455 of yacc.c  */
#line 3893 "Grammar.ypp"
    {
    (yyval.saszsauva) = new SASzSAUva;
    memset((yyval.saszsauva), 0, sizeof(SASzSAUva));
    (yyval.saszsauva)->semantic = (yyvsp[(1) - (2)].s);
;}
    break;

  case 647:

/* Line 1455 of yacc.c  */
#line 3898 "Grammar.ypp"
    {
    (yyval.saszsauva) = new SASzSAUva;
    memset((yyval.saszsauva), 0, sizeof(SASzSAUva));
    (yyval.saszsauva)->semantic = (yyvsp[(1) - (2)].s);
    (yyval.saszsauva)->valueList = (yyvsp[(2) - (2)].valueList);
;}
    break;

  case 648:

/* Line 1455 of yacc.c  */
#line 3904 "Grammar.ypp"
    {
    (yyval.saszsauva) = new SASzSAUva;
    memset((yyval.saszsauva), 0, sizeof(SASzSAUva));
    (yyval.saszsauva)->semantic = (yyvsp[(1) - (3)].s);
    (yyval.saszsauva)->valueList = (yyvsp[(3) - (3)].valueList);
;}
    break;

  case 649:

/* Line 1455 of yacc.c  */
#line 3910 "Grammar.ypp"
    {
    (yyval.saszsauva) = new SASzSAUva;
    memset((yyval.saszsauva), 0, sizeof(SASzSAUva));
    (yyval.saszsauva)->arraySz = (yyvsp[(1) - (1)].ival);
;}
    break;

  case 650:

/* Line 1455 of yacc.c  */
#line 3915 "Grammar.ypp"
    {
    (yyval.saszsauva) = new SASzSAUva;
    memset((yyval.saszsauva), 0, sizeof(SASzSAUva));
    (yyval.saszsauva)->arraySz = (yyvsp[(1) - (2)].ival);
;}
    break;

  case 651:

/* Line 1455 of yacc.c  */
#line 3920 "Grammar.ypp"
    {
    (yyval.saszsauva) = new SASzSAUva;
    memset((yyval.saszsauva), 0, sizeof(SASzSAUva));
    (yyval.saszsauva)->arraySz = (yyvsp[(1) - (2)].ival);
    (yyval.saszsauva)->valueList = (yyvsp[(2) - (2)].valueList);
;}
    break;

  case 652:

/* Line 1455 of yacc.c  */
#line 3926 "Grammar.ypp"
    {
    (yyval.saszsauva) = new SASzSAUva;
    memset((yyval.saszsauva), 0, sizeof(SASzSAUva));
    (yyval.saszsauva)->arraySz = (yyvsp[(1) - (3)].ival);
    (yyval.saszsauva)->valueList = (yyvsp[(3) - (3)].valueList);
;}
    break;

  case 653:

/* Line 1455 of yacc.c  */
#line 3932 "Grammar.ypp"
    {
    (yyval.saszsauva) = new SASzSAUva;
    memset((yyval.saszsauva), 0, sizeof(SASzSAUva));
    (yyval.saszsauva)->arraySz = (yyvsp[(1) - (2)].ival);
    (yyval.saszsauva)->semantic = (yyvsp[(2) - (2)].s);
;}
    break;

  case 654:

/* Line 1455 of yacc.c  */
#line 3938 "Grammar.ypp"
    {
    (yyval.saszsauva) = new SASzSAUva;
    memset((yyval.saszsauva), 0, sizeof(SASzSAUva));
    (yyval.saszsauva)->arraySz = (yyvsp[(1) - (3)].ival);
    (yyval.saszsauva)->semantic = (yyvsp[(2) - (3)].s);
;}
    break;

  case 655:

/* Line 1455 of yacc.c  */
#line 3944 "Grammar.ypp"
    {
    (yyval.saszsauva) = new SASzSAUva;
    memset((yyval.saszsauva), 0, sizeof(SASzSAUva));
    (yyval.saszsauva)->arraySz = (yyvsp[(1) - (3)].ival);
    (yyval.saszsauva)->semantic = (yyvsp[(2) - (3)].s);
    (yyval.saszsauva)->valueList = (yyvsp[(3) - (3)].valueList);
;}
    break;

  case 656:

/* Line 1455 of yacc.c  */
#line 3951 "Grammar.ypp"
    {
    (yyval.saszsauva) = new SASzSAUva;
    memset((yyval.saszsauva), 0, sizeof(SASzSAUva));
    (yyval.saszsauva)->arraySz = (yyvsp[(1) - (4)].ival);
    (yyval.saszsauva)->semantic = (yyvsp[(2) - (4)].s);
    (yyval.saszsauva)->valueList = (yyvsp[(4) - (4)].valueList);
;}
    break;

  case 657:

/* Line 1455 of yacc.c  */
#line 3963 "Grammar.ypp"
    { (yyval.uniformType) = nvFX::IUniform::TFloat; ;}
    break;

  case 658:

/* Line 1455 of yacc.c  */
#line 3964 "Grammar.ypp"
    { (yyval.uniformType) = nvFX::IUniform::TVec2; ;}
    break;

  case 659:

/* Line 1455 of yacc.c  */
#line 3965 "Grammar.ypp"
    { (yyval.uniformType) = nvFX::IUniform::TVec3; ;}
    break;

  case 660:

/* Line 1455 of yacc.c  */
#line 3966 "Grammar.ypp"
    { (yyval.uniformType) = nvFX::IUniform::TVec4; ;}
    break;

  case 661:

/* Line 1455 of yacc.c  */
#line 3967 "Grammar.ypp"
    { (yyval.uniformType) = nvFX::IUniform::TInt; ;}
    break;

  case 662:

/* Line 1455 of yacc.c  */
#line 3968 "Grammar.ypp"
    { (yyval.uniformType) = nvFX::IUniform::TInt2; ;}
    break;

  case 663:

/* Line 1455 of yacc.c  */
#line 3969 "Grammar.ypp"
    { (yyval.uniformType) = nvFX::IUniform::TInt3; ;}
    break;

  case 664:

/* Line 1455 of yacc.c  */
#line 3970 "Grammar.ypp"
    { (yyval.uniformType) = nvFX::IUniform::TInt4; ;}
    break;

  case 665:

/* Line 1455 of yacc.c  */
#line 3971 "Grammar.ypp"
    { (yyval.uniformType) = nvFX::IUniform::TBool; ;}
    break;

  case 666:

/* Line 1455 of yacc.c  */
#line 3972 "Grammar.ypp"
    { (yyval.uniformType) = nvFX::IUniform::TBool2; ;}
    break;

  case 667:

/* Line 1455 of yacc.c  */
#line 3973 "Grammar.ypp"
    { (yyval.uniformType) = nvFX::IUniform::TBool3; ;}
    break;

  case 668:

/* Line 1455 of yacc.c  */
#line 3974 "Grammar.ypp"
    { (yyval.uniformType) = nvFX::IUniform::TBool4; ;}
    break;

  case 669:

/* Line 1455 of yacc.c  */
#line 3975 "Grammar.ypp"
    { (yyval.uniformType) = nvFX::IUniform::TMat2; ;}
    break;

  case 670:

/* Line 1455 of yacc.c  */
#line 3976 "Grammar.ypp"
    { (yyval.uniformType) = nvFX::IUniform::TMat3; ;}
    break;

  case 671:

/* Line 1455 of yacc.c  */
#line 3977 "Grammar.ypp"
    { (yyval.uniformType) = nvFX::IUniform::TMat4; ;}
    break;

  case 672:

/* Line 1455 of yacc.c  */
#line 3978 "Grammar.ypp"
    { (yyval.uniformType) = nvFX::IUniform::TTexture1D; ;}
    break;

  case 673:

/* Line 1455 of yacc.c  */
#line 3979 "Grammar.ypp"
    { (yyval.uniformType) = nvFX::IUniform::TTexture2D; ;}
    break;

  case 674:

/* Line 1455 of yacc.c  */
#line 3980 "Grammar.ypp"
    { (yyval.uniformType) = nvFX::IUniform::TTexture2DRect; ;}
    break;

  case 675:

/* Line 1455 of yacc.c  */
#line 3981 "Grammar.ypp"
    { (yyval.uniformType) = nvFX::IUniform::TTexture3D; ;}
    break;

  case 676:

/* Line 1455 of yacc.c  */
#line 3982 "Grammar.ypp"
    { (yyval.uniformType) = nvFX::IUniform::TTextureCube; ;}
    break;

  case 677:

/* Line 1455 of yacc.c  */
#line 3983 "Grammar.ypp"
    { (yyval.uniformType) = nvFX::IUniform::TTexture1D; ;}
    break;

  case 678:

/* Line 1455 of yacc.c  */
#line 3984 "Grammar.ypp"
    { (yyval.uniformType) = nvFX::IUniform::TTexture2D; ;}
    break;

  case 679:

/* Line 1455 of yacc.c  */
#line 3985 "Grammar.ypp"
    { (yyval.uniformType) = nvFX::IUniform::TTexture2DRect; ;}
    break;

  case 680:

/* Line 1455 of yacc.c  */
#line 3986 "Grammar.ypp"
    { (yyval.uniformType) = nvFX::IUniform::TTexture3D; ;}
    break;

  case 681:

/* Line 1455 of yacc.c  */
#line 3987 "Grammar.ypp"
    { (yyval.uniformType) = nvFX::IUniform::TTextureCube; ;}
    break;

  case 682:

/* Line 1455 of yacc.c  */
#line 3988 "Grammar.ypp"
    { yyerror("Error : unknown type\n"); ;}
    break;

  case 683:

/* Line 1455 of yacc.c  */
#line 3994 "Grammar.ypp"
    {
        curAnnotations = IAnnotationEx::getAnnotationSingleton(); // need a temporary place
        curAnnotations->clear();
    ;}
    break;

  case 684:

/* Line 1455 of yacc.c  */
#line 3997 "Grammar.ypp"
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

  case 706:

/* Line 1455 of yacc.c  */
#line 4117 "Grammar.ypp"
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

  case 707:

/* Line 1455 of yacc.c  */
#line 4125 "Grammar.ypp"
    { 
    namespaceName.clear(); 
    curAnnotations = NULL;
    namespaceAnnotations = NULL;
    delete (yyvsp[(2) - (7)].s);
;}
    break;

  case 711:

/* Line 1455 of yacc.c  */
#line 4143 "Grammar.ypp"
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

  case 712:

/* Line 1455 of yacc.c  */
#line 4174 "Grammar.ypp"
    {
    curCstBuffer = NULL;
    curAnnotations = NULL;
    lex_pop_state();
;}
    break;

  case 713:

/* Line 1455 of yacc.c  */
#line 4185 "Grammar.ypp"
    {
    delete (yyvsp[(4) - (6)].varNameList);
    ;}
    break;

  case 714:

/* Line 1455 of yacc.c  */
#line 4188 "Grammar.ypp"
    {
    delete (yyvsp[(5) - (7)].varNameList);
    ;}
    break;

  case 715:

/* Line 1455 of yacc.c  */
#line 4191 "Grammar.ypp"
    {
    delete (yyvsp[(4) - (5)].s)
    ;}
    break;

  case 716:

/* Line 1455 of yacc.c  */
#line 4194 "Grammar.ypp"
    {
    delete (yyvsp[(4) - (5)].s)
    ;}
    break;

  case 717:

/* Line 1455 of yacc.c  */
#line 4197 "Grammar.ypp"
    {
    delete (yyvsp[(4) - (5)].s)
    ;}
    break;

  case 718:

/* Line 1455 of yacc.c  */
#line 4200 "Grammar.ypp"
    {
    delete (yyvsp[(4) - (5)].s)
    ;}
    break;

  case 719:

/* Line 1455 of yacc.c  */
#line 4203 "Grammar.ypp"
    {
;}
    break;

  case 720:

/* Line 1455 of yacc.c  */
#line 4205 "Grammar.ypp"
    {
    delete (yyvsp[(4) - (5)].s)
    ;}
    break;

  case 721:

/* Line 1455 of yacc.c  */
#line 4208 "Grammar.ypp"
    {
    // we prefer this loop : we don't have the char ** available here... using vector of std::string...
    //for(int i=0; i<(int)$8->size(); i++)
    //    curXX->createState($4->c_str(), i, (*$8)[i].c_str());
    delete (yyvsp[(4) - (10)].s);
    delete (yyvsp[(8) - (10)].varNameList);
    ;}
    break;

  case 722:

/* Line 1455 of yacc.c  */
#line 4215 "Grammar.ypp"
    {
    //curPass->createState($4->c_str(), 0, $7->c_str());
    delete (yyvsp[(4) - (8)].s);
    delete (yyvsp[(7) - (8)].s);
    ;}
    break;

  case 723:

/* Line 1455 of yacc.c  */
#line 4220 "Grammar.ypp"
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

  case 724:

/* Line 1455 of yacc.c  */
#line 4236 "Grammar.ypp"
    {
   ;}
    break;

  case 725:

/* Line 1455 of yacc.c  */
#line 4237 "Grammar.ypp"
    {
   ;}
    break;

  case 726:

/* Line 1455 of yacc.c  */
#line 4238 "Grammar.ypp"
    {
   ;}
    break;

  case 728:

/* Line 1455 of yacc.c  */
#line 4247 "Grammar.ypp"
    {
    delete (yyvsp[(5) - (7)].varNameList);
    ;}
    break;

  case 729:

/* Line 1455 of yacc.c  */
#line 4250 "Grammar.ypp"
    {
    delete (yyvsp[(5) - (7)].varNameList);
    ;}
    break;

  case 730:

/* Line 1455 of yacc.c  */
#line 4253 "Grammar.ypp"
    {
    // we prefer this loop : we don't have the char ** available here... using vector of std::string...
    //for(int i=0; i<(int)$8->size(); i++)
    //    curXX->createState($4->c_str(), i, (*$8)[i].c_str());
    delete (yyvsp[(4) - (10)].s);
    delete (yyvsp[(8) - (10)].varNameList);
    ;}
    break;

  case 731:

/* Line 1455 of yacc.c  */
#line 4260 "Grammar.ypp"
    {
    //curPass->createState($4->c_str(), 0, $7->c_str());
    delete (yyvsp[(4) - (8)].s);
    delete (yyvsp[(7) - (8)].s);
    ;}
    break;

  case 732:

/* Line 1455 of yacc.c  */
#line 4265 "Grammar.ypp"
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

  case 733:

/* Line 1455 of yacc.c  */
#line 4281 "Grammar.ypp"
    {
   ;}
    break;

  case 734:

/* Line 1455 of yacc.c  */
#line 4282 "Grammar.ypp"
    {
   ;}
    break;

  case 735:

/* Line 1455 of yacc.c  */
#line 4283 "Grammar.ypp"
    {
   ;}
    break;

  case 737:

/* Line 1455 of yacc.c  */
#line 4292 "Grammar.ypp"
    {
    //if(curResource) {
    //    if(!curResource->setFormat($4))
    //    {
    //        yyerror("Error: Bad format\n");
    //    }
    //}
;}
    break;

  case 738:

/* Line 1455 of yacc.c  */
#line 4300 "Grammar.ypp"
    {
    //if(curResource)
    //    curResource->setDimensionsApplicationDependent();
;}
    break;

  case 739:

/* Line 1455 of yacc.c  */
#line 4304 "Grammar.ypp"
    {
    int d = (int)(yyvsp[(5) - (7)].intList)->size();
    //if(curResource)
    //    curResource->setDimensions((*$5)[0], d > 1 ? (*$5)[1] : 1, d > 2 ? (*$5)[2] : 1);
    delete (yyvsp[(5) - (7)].intList);
;}
    break;

  case 740:

/* Line 1455 of yacc.c  */
#line 4312 "Grammar.ypp"
    {
    delete (yyvsp[(2) - (2)].s);
   ;}
    break;

  case 741:

/* Line 1455 of yacc.c  */
#line 4314 "Grammar.ypp"
    {
   ;}
    break;

  case 742:

/* Line 1455 of yacc.c  */
#line 4315 "Grammar.ypp"
    {
   ;}
    break;

  case 744:

/* Line 1455 of yacc.c  */
#line 4324 "Grammar.ypp"
    {
    delete (yyvsp[(4) - (5)].s);
;}
    break;

  case 745:

/* Line 1455 of yacc.c  */
#line 4327 "Grammar.ypp"
    {
    delete (yyvsp[(4) - (5)].s);
;}
    break;

  case 746:

/* Line 1455 of yacc.c  */
#line 4332 "Grammar.ypp"
    {
   ;}
    break;

  case 747:

/* Line 1455 of yacc.c  */
#line 4333 "Grammar.ypp"
    {
   ;}
    break;

  case 748:

/* Line 1455 of yacc.c  */
#line 4334 "Grammar.ypp"
    {
   ;}
    break;



/* Line 1455 of yacc.c  */
#line 10622 "C:/p4/GitHub/nvFX/FxParser/Grammar.cpp"
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
#line 4339 "Grammar.ypp"


