
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
#line 42 "Grammar.ypp"

    /* Documentation on the keywords and grammar created in : ..\doxygen\Keywords.dxt */
    #include  <math.h>
    #include  <assert.h>
    #include  <stdio.h>
    #include  <stdlib.h>
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
#line 245 "C:/p4/GitHub/nvFX/FxParser/Grammar.cpp"

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
#line 205 "Grammar.ypp"

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
#line 443 "C:/p4/GitHub/nvFX/FxParser/Grammar.cpp"

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
     GLPOINTSPRITECOORDORIGIN = 440,
     GLLINEWIDTH = 441,
     GLLINESMOOTH = 442,
     GLCOMBINEDLINESTIPPLE = 443,
     GLLINESTIPPLEPATTERN = 444,
     GLLINESTIPPLEREPEAT = 445,
     GLLINESTIPPLE = 446,
     GLCULLFACE = 447,
     GLCULLFACEMODE = 448,
     GLFRONTFACE = 449,
     GLPOLYGONSMOOTH = 450,
     GLPOLYGONMODE = 451,
     GLPOLYGONOFFSET = 452,
     GLPOLYGONOFFSETFACTOR = 453,
     GLPOLYGONOFFSETUNITS = 454,
     GLPOLYGONOFFSETPOINT = 455,
     GLPOLYGONOFFSETLINE = 456,
     GLPOLYGONOFFSETFILL = 457,
     GLPOLYGONSTIPPLEBIT = 458,
     GLRASTERIZERDISCARD = 459,
     GLALPHATEST = 460,
     GLALPHAFUNC = 461,
     GLALPHATESTFUNC = 462,
     GLALPHATESTREF = 463,
     GLBLENDFUNC = 464,
     GLBLENDFUNCSEPARATE = 465,
     GLBLEND = 466,
     GLBLENDSRC = 467,
     GLBLENDSRCRGB = 468,
     GLBLENDSRCALPHA = 469,
     GLBLENDDST = 470,
     GLBLENDDSTRGB = 471,
     GLBLENDDSTALPHA = 472,
     GLBLENDEQUATIONSEPARATE = 473,
     GLBLENDEQUATION = 474,
     GLBLENDEQUATIONRGB = 475,
     GLBLENDEQUATIONALPHA = 476,
     GLBLENDCOLOR = 477,
     GLFRAMEBUFFERSRGB = 478,
     GLDITHER = 479,
     GLCOLORLOGICOP = 480,
     GLLOGICOPMODE = 481,
     GLCOLORWRITEMASK = 482,
     GLMULTISAMPLE = 483,
     GLSAMPLEMASK = 484,
     GLSAMPLEMASKVALUE = 485,
     GLSAMPLEALPHATOONE = 486,
     GLSAMPLEALPHATOCOVERAGE = 487,
     GLDEPTHTEST = 488,
     GLDEPTHFUNC = 489,
     GLDEPTHWRITEMASK = 490,
     GLDEPTHCLAMP = 491,
     GLDEPTHBOUNDSTESTEXT = 492,
     GLDEPTHBOUNDSEXT = 493,
     GLSTENCILTEST = 494,
     GLSTENCILFUNC = 495,
     GLSTENCILFUNCSEPARATE = 496,
     GLSTENCILVALUEMASK = 497,
     GLSTENCILREF = 498,
     GLSTENCILFAIL = 499,
     GLSTENCILPASSDEPTHFAIL = 500,
     GLSTENCILPASSDEPTHPASS = 501,
     GLSTENCILOP = 502,
     GLSTENCILOPSEPARATE = 503,
     GLSTENCILBACKOP = 504,
     GLSTENCILWRITEMASK = 505,
     GLSTENCILBACKFUNC = 506,
     GLSTENCILBACKVALUEMASK = 507,
     GLSTENCILBACKREF = 508,
     GLSTENCILBACKFAIL = 509,
     GLSTENCILBACKPASSDEPTHFAIL = 510,
     GLSTENCILBACKPASSDEPTHPASS = 511,
     GLSTENCILBACKWRITEMASK = 512,
     ATTRIBUTE = 513,
     RENDERGROUP = 514,
     RENDERMODE = 515,
     RENDERMODE_ENUM = 516,
     BLITFBOTOACTIVETARGET = 517,
     CLEARMODE = 518,
     CLEAR_ENUM = 519,
     CURRENTTARGET = 520,
     CLEARCOLOR = 521,
     VIEWPORT = 522,
     FBO = 523,
     RENDERBUFFER = 524,
     RENDERTEXTURE = 525,
     RENDERTEXTURERECT = 526,
     DSTTARGET = 527,
     COLORTARGETS = 528,
     RESOURCEMSAA = 529,
     RESOURCESIZE = 530,
     RESOURCESIZEFACTOR = 531,
     RESOURCERESOLVETARGET = 532,
     RESOURCEFORMAT = 533,
     RESOURCEFORMAT_ENUM = 534,
     APPDEFINED = 535,
     SWAPRESOURCES = 536,
     LOOP = 537,
     ACTIVE = 538,
     OFF = 539,
     GLOBAL = 540,
     LOCAL = 541,
     CUDAKERNEL = 542,
     CUDACODE = 543,
     CUDAMODULE = 544,
     CUDASHAREDMEMORY = 545,
     CUDAGRID = 546,
     CUDABLOCK = 547,
     OPTIX_CONTEXT = 548,
     OPTIX_CONTEXT_ENTRY = 549,
     RAY_GENERATION_PROGRAM = 550,
     EXCEPTION_PROGRAM = 551,
     INTERSECTION_PROGRAM = 552,
     BOUNDING_BOX_PROGRAM = 553,
     MISS_PROGRAM = 554,
     DEFAULT_MATERIAL = 555,
     STACK_SIZE = 556,
     OUTPUT_BUFFER = 557,
     OPTIX_MATERIAL = 558,
     CLOSEST_HIT_PROGRAM = 559,
     ANY_HIT_PROGRAM = 560,
     OPTIX_BUFFER = 561,
     PTX_PROGRAM_FROM_FILE = 562,
     FILE_NAME = 563,
     ENTRY_POINT = 564,
     COMPUTEGROUPS = 565,
     COMPUTEPROGRAM = 566,
     COMPUTEPROGRAMOVERRIDE = 567,
     SVGPATH = 568,
     PSPATH = 569,
     PATHSOURCEOVERRIDE = 570,
     PATHSOURCE = 571,
     GLPATHSTENCILDEPTHOFFSET = 572,
     GLPATHSTENCILFUNC = 573,
     GLPATHFOGGEN = 574,
     GLPATHTEXGEN = 575,
     GLPATHCOLORGEN = 576,
     GLPATHCOVERDEPTHFUNC = 577,
     GLPATHSTROKEWIDTH = 578,
     GLPATHINITIALENDCAP = 579,
     GLPATHTERMINALENDCAP = 580,
     GLPATHINITIALDASHCAP = 581,
     GLPATHTERMINALDASHCAP = 582,
     GLPATHJOINSTYLE = 583,
     GLPATHMITERLIMIT = 584,
     GLPATHDASHOFFSET = 585,
     GLPATHDASHOFFSETRESET = 586,
     GLPATHCLIENTLENGTH = 587,
     GLPATHFILLMODE = 588,
     GLPATHFILLMASK = 589,
     GLPATHFILLCOVERMODE = 590,
     GLPATHSTROKEMASK = 591,
     GLPATHSTROKEREFERENCE = 592,
     GLPATHSTROKECOVERMODE = 593,
     GLPATHTRANSFORMTYPE = 594,
     GLPATHTRANSFORM = 595,
     GLFOGCOORDINATE = 596,
     GLFRAGMENTDEPTH = 597,
     GLOBJECTLINEAR = 598,
     GLPATHOBJECTBOUNDINGBOX = 599,
     GLEYELINEAR = 600,
     GLFLAT = 601,
     GLSQUARE = 602,
     GLROUND = 603,
     GLTRIANGULAR = 604,
     GLMITERREVERT = 605,
     GLMITERTRUNCATE = 606,
     GLBEVEL = 607,
     GLMOVETORESET = 608,
     GLMOVETOCONTINUES = 609,
     GLCOUNTUP = 610,
     GLCOUNTDOWN = 611,
     GLBOUNDINGBOX = 612,
     GLCONVEXHULL = 613,
     GLTRANSLATE2D = 614,
     GLTRANSLATEX = 615,
     GLTRANSLATEY = 616,
     GLTRANSLATE3D = 617,
     GLAFFINE2D = 618,
     GLAFFINE3D = 619,
     GLTRANSPOSEAFFINE2D = 620,
     GLTRANSPOSEAFFINE3D = 621,
     GLLUMINANCE = 622,
     GLALPHA = 623,
     GLINTENSITY = 624,
     GLLUMINANCEALPHA = 625,
     GLRGB = 626,
     GLRGBA = 627,
     NEG = 628
   };
#endif



#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
typedef union YYSTYPE
{

/* Line 214 of yacc.c  */
#line 407 "Grammar.ypp"

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
#line 873 "C:/p4/GitHub/nvFX/FxParser/Grammar.cpp"
} YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
#endif


/* Copy the second part of user declarations.  */


/* Line 264 of yacc.c  */
#line 885 "C:/p4/GitHub/nvFX/FxParser/Grammar.cpp"

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
#define YYLAST   5217

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  395
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  125
/* YYNRULES -- Number of rules.  */
#define YYNRULES  746
/* YYNRULES -- Number of states.  */
#define YYNSTATES  2115

/* YYTRANSLATE(YYLEX) -- Bison symbol number corresponding to YYLEX.  */
#define YYUNDEFTOK  2
#define YYMAXUTOK   628

#define YYTRANSLATE(YYX)						\
  ((unsigned int) (YYX) <= YYMAXUTOK ? yytranslate[YYX] : YYUNDEFTOK)

/* YYTRANSLATE[YYLEX] -- Bison symbol number corresponding to YYLEX.  */
static const yytype_uint16 yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     384,   385,   378,   377,   386,   376,   394,   379,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,   383,   382,
     390,   375,   391,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,   392,     2,   393,   381,     2,     2,     2,     2,     2,
      63,    64,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,   387,   389,   388,     2,     2,     2,     2,
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
     367,   368,   369,   370,   371,   372,   373,   374,   380
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
    1662,  1667,  1672,  1681,  1686,  1691,  1700,  1705,  1710,  1715,
    1720,  1725,  1730,  1735,  1744,  1749,  1758,  1771,  1776,  1781,
    1786,  1791,  1796,  1801,  1806,  1811,  1816,  1825,  1838,  1843,
    1848,  1853,  1858,  1871,  1876,  1881,  1886,  1891,  1896,  1901,
    1906,  1911,  1916,  1925,  1930,  1935,  1946,  1959,  1964,  1969,
    1974,  1979,  1984,  1989,  2000,  2013,  2018,  2029,  2034,  2039,
    2044,  2049,  2054,  2059,  2060,  2066,  2072,  2078,  2084,  2090,
    2096,  2102,  2108,  2114,  2120,  2126,  2132,  2138,  2144,  2154,
    2160,  2166,  2172,  2178,  2184,  2190,  2194,  2195,  2203,  2206,
    2207,  2213,  2219,  2225,  2231,  2237,  2243,  2249,  2255,  2261,
    2267,  2273,  2279,  2284,  2293,  2307,  2313,  2319,  2325,  2331,
    2345,  2351,  2357,  2363,  2369,  2373,  2374,  2382,  2385,  2386,
    2392,  2398,  2404,  2410,  2416,  2426,  2432,  2438,  2444,  2450,
    2456,  2462,  2468,  2474,  2480,  2486,  2492,  2498,  2504,  2510,
    2516,  2520,  2521,  2529,  2533,  2542,  2547,  2552,  2566,  2580,
    2585,  2590,  2595,  2600,  2605,  2610,  2615,  2620,  2625,  2630,
    2635,  2640,  2645,  2650,  2655,  2660,  2665,  2666,  2676,  2682,
    2688,  2703,  2718,  2724,  2730,  2736,  2742,  2748,  2754,  2760,
    2766,  2772,  2778,  2784,  2790,  2796,  2802,  2808,  2814,  2820,
    2824,  2825,  2833,  2837,  2838,  2844,  2850,  2856,  2862,  2865,
    2869,  2870,  2872,  2875,  2880,  2885,  2888,  2891,  2894,  2899,
    2901,  2903,  2905,  2907,  2910,  2913,  2917,  2919,  2921,  2924,
    2926,  2928,  2930,  2932,  2934,  2936,  2938,  2941,  2944,  2948,
    2949,  2951,  2953,  2956,  2958,  2961,  2964,  2968,  2970,  2973,
    2976,  2980,  2983,  2987,  2991,  2996,  2998,  3000,  3002,  3004,
    3006,  3008,  3010,  3012,  3014,  3016,  3018,  3020,  3022,  3024,
    3026,  3028,  3030,  3032,  3034,  3036,  3038,  3040,  3042,  3044,
    3046,  3048,  3049,  3056,  3058,  3061,  3063,  3065,  3067,  3069,
    3071,  3073,  3075,  3077,  3079,  3081,  3083,  3085,  3087,  3089,
    3091,  3093,  3095,  3097,  3100,  3101,  3109,  3110,  3113,  3117,
    3118,  3123,  3130,  3138,  3144,  3150,  3156,  3162,  3168,  3174,
    3185,  3194,  3202,  3203,  3204,  3213,  3214,  3222,  3230,  3241,
    3250,  3258,  3259,  3260,  3269,  3270,  3276,  3282,  3290,  3291,
    3292,  3301,  3302,  3308,  3314,  3315,  3316
};

/* YYRHS -- A `-1'-separated list of the rules' RHS.  */
static const yytype_int16 yyrhs[] =
{
     396,     0,    -1,    -1,   396,   382,    -1,   396,   397,    -1,
     396,   425,    -1,   396,   432,    -1,   396,   436,    -1,   396,
     435,    -1,   396,   442,    -1,   396,   441,    -1,   396,   445,
      -1,   396,   448,    -1,   396,   456,    -1,   396,   462,    -1,
     396,   472,    -1,   396,   476,    -1,   396,   469,    -1,   396,
     466,    -1,   396,   499,    -1,   396,   502,    -1,   396,   496,
      -1,   396,   433,    -1,   396,   434,    -1,   396,   505,    -1,
     396,   509,    -1,   396,   513,    -1,   396,   517,    -1,   396,
       1,    -1,    61,    62,   383,   401,    -1,     9,    -1,    22,
      -1,     6,    -1,   399,   377,   399,    -1,   400,   377,   399,
      -1,   399,   377,   400,    -1,   399,   376,   399,    -1,   400,
     376,   399,    -1,   399,   376,   400,    -1,   399,   378,   399,
      -1,   400,   378,   399,    -1,   399,   378,   400,    -1,   399,
     379,   399,    -1,   400,   379,   399,    -1,   399,   379,   400,
      -1,   400,   379,   400,    -1,   376,   399,    -1,   399,   381,
     399,    -1,   400,   381,   399,    -1,   399,   381,   400,    -1,
     400,   381,   400,    -1,   384,   399,   385,    -1,     7,    -1,
     400,   377,   400,    -1,   400,   376,   400,    -1,   400,   378,
     400,    -1,   376,   400,    -1,   384,   400,   385,    -1,     4,
      -1,     5,    -1,   401,    -1,   400,    -1,   399,    -1,   400,
      -1,   399,    -1,   401,    -1,   399,    -1,   400,    -1,   401,
      -1,    -1,   386,    -1,     5,    -1,    22,    -1,   406,   405,
      22,    -1,    -1,   406,    -1,   409,   406,   410,    -1,    -1,
      22,    -1,   408,   405,    22,    -1,   387,    -1,   388,    -1,
     399,    -1,   400,    -1,   411,   386,   399,    -1,   411,   386,
     400,    -1,   411,     1,    -1,   409,   411,   410,    -1,   412,
     386,   409,   411,   410,    -1,   412,     1,    -1,   400,    -1,
     413,   386,   400,    -1,   409,   413,   410,    -1,   414,   386,
     409,   413,   410,    -1,   414,     1,    -1,   401,    -1,   415,
     386,   401,    -1,   400,    -1,   399,    -1,    22,    -1,     1,
      -1,    -1,    29,    -1,   416,    -1,   417,   386,   416,    -1,
     417,     1,    -1,   100,    -1,   101,    -1,   102,    -1,   103,
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
      -1,   369,    -1,   370,    -1,   371,    -1,   372,    -1,   373,
      -1,   374,    -1,   343,    -1,   344,    -1,   345,    -1,   346,
      -1,   347,    -1,   348,    -1,   349,    -1,   350,    -1,   351,
      -1,   352,    -1,   353,    -1,   354,    -1,   355,    -1,   356,
      -1,   357,    -1,   358,    -1,   359,    -1,   360,    -1,   361,
      -1,   362,    -1,   363,    -1,   364,    -1,   365,    -1,   366,
      -1,   367,    -1,   368,    -1,   266,    -1,   419,   389,   266,
      -1,    -1,   420,   464,    -1,   420,   474,    -1,   420,   295,
     375,    22,   382,    -1,   420,   296,   375,   400,   382,    -1,
     420,    65,   375,   407,   382,    -1,   420,    65,   390,   398,
     391,   375,   407,   382,    -1,   420,    66,   375,   407,   382,
      -1,   420,    66,   390,   398,   391,   375,   407,   382,    -1,
     420,   313,   375,   407,   382,    -1,   420,   313,   390,   398,
     391,   375,   407,   382,    -1,   420,   318,   375,   407,   382,
      -1,   420,   318,   390,   398,   391,   375,   407,   382,    -1,
     420,    67,   375,   407,   382,    -1,   420,    67,   390,   398,
     391,   375,   407,   382,    -1,   420,    68,   375,   407,   382,
      -1,   420,    68,   390,   398,   391,   375,   407,   382,    -1,
     420,    69,   375,   407,   382,    -1,   420,    69,   390,   398,
     391,   375,   407,   382,    -1,   420,    85,   375,   407,   382,
      -1,   420,    85,   390,   398,   391,   375,   407,   382,    -1,
     420,    86,   375,   407,   382,    -1,   420,    86,   390,   398,
     391,   375,   407,   382,    -1,   420,   314,   375,   407,   382,
      -1,   420,   314,   390,   398,   391,   375,   407,   382,    -1,
     420,   317,   375,   407,   382,    -1,   420,   317,   390,   398,
     391,   375,   407,   382,    -1,   420,    87,   375,   407,   382,
      -1,   420,    87,   390,   398,   391,   375,   407,   382,    -1,
     420,    88,   375,   407,   382,    -1,   420,    88,   390,   398,
     391,   375,   407,   382,    -1,   420,    89,   375,   407,   382,
      -1,   420,    89,   390,   398,   391,   375,   407,   382,    -1,
     420,    34,   384,    22,   385,   375,   409,   408,   410,   382,
      -1,   420,    34,   384,    22,   385,   375,    22,   382,    -1,
     420,    34,   384,    22,   385,   489,   382,    -1,   420,    34,
     384,    22,   392,   400,   393,   385,   375,    22,   382,    -1,
     420,    34,   384,    22,   392,   400,   393,   385,   375,   409,
     412,   410,   382,    -1,   420,    34,   384,    22,   392,   400,
     393,   385,   375,   414,   382,    -1,   420,    34,   384,    22,
     392,   400,   393,   385,   375,   409,   415,   410,   382,    -1,
     420,    70,   375,     9,   382,    -1,   420,    23,   384,    22,
     385,   375,    22,   382,    -1,   420,    96,   384,    22,   385,
     375,    22,   382,    -1,   420,    71,   384,    22,   385,   375,
      22,   382,    -1,   420,    97,   384,    22,   385,   375,    22,
     382,    -1,   420,    34,   384,    22,   394,    71,   385,   375,
      22,   382,    -1,   420,    71,   384,    22,   385,   375,   387,
      22,   386,   400,   388,   382,    -1,   420,    97,   384,    22,
     385,   375,   387,    22,   386,   400,   388,   382,    -1,   420,
      77,   384,    22,   385,   375,    22,   382,    -1,   420,    93,
     384,    22,   385,   375,    22,   382,    -1,   420,    34,   384,
      22,   394,    77,   385,   375,    22,   382,    -1,   420,    77,
     384,    22,   385,   375,   387,    22,   386,   400,   388,   382,
      -1,   420,    72,   384,    22,   385,   375,   400,   382,    -1,
     420,    98,   384,    22,   385,   375,   400,   382,    -1,   420,
      78,   384,    22,   385,   375,   400,   382,    -1,   420,    95,
     384,    22,   385,   375,   400,   382,    -1,   420,    34,   384,
      22,   394,    72,   385,   375,   400,   382,    -1,   420,    34,
     384,    22,   394,    78,   385,   375,   400,   382,    -1,   420,
      73,   384,    22,   385,   375,    22,   382,    -1,   420,    99,
     384,    22,   385,   375,    22,   382,    -1,   420,    34,   384,
      22,   394,    73,   385,   375,    22,   382,    -1,   420,    74,
     375,    22,   382,    -1,   420,    74,   384,    22,   385,   382,
      -1,   420,    75,   375,    22,   382,    -1,   420,    75,   384,
      22,   385,   382,    -1,   420,    76,   375,    22,   382,    -1,
     420,    76,   384,    22,   385,   382,    -1,   420,   262,   375,
     263,   382,    -1,   420,   261,   375,   403,   382,    -1,   420,
     264,   375,    22,   382,    -1,   420,   265,   375,   419,   382,
      -1,   420,   267,   375,    22,   382,    -1,   420,   268,   392,
     403,   393,   375,   412,   382,    -1,   420,   268,   375,   409,
     411,   410,   382,    -1,   420,   269,   375,   409,   413,   410,
     382,    -1,   420,   283,   384,    22,   386,    22,   385,   382,
      -1,   420,   284,   375,   403,   382,    -1,   420,   285,   375,
     402,   382,    -1,   420,   312,   375,   409,   413,   410,   382,
      -1,   420,   291,   375,   407,   382,    -1,   420,   289,   375,
     387,    22,   386,   417,   388,   382,    -1,   420,   292,   375,
     400,   382,    -1,   420,   293,   375,   414,   382,    -1,   420,
     294,   375,   414,   382,    -1,   420,   260,   384,   398,   385,
     375,   403,   382,    -1,   420,     1,   382,    -1,    -1,    20,
     422,   480,   387,   420,   388,    -1,    -1,    20,    22,   423,
     480,   387,   420,   388,    -1,   260,   384,    22,   385,   375,
     403,   382,    -1,   260,   384,     9,   385,   375,   403,   382,
      -1,    19,    22,   382,    -1,    19,    22,   286,   382,    -1,
     421,    -1,   424,   421,    -1,   424,     1,    -1,    -1,    19,
     426,   480,   387,   424,   388,    -1,    -1,    19,    22,   427,
     480,   387,   424,   388,    -1,   288,    -1,   287,    -1,    -1,
     288,    -1,   287,    -1,    22,    -1,    -1,   430,   480,    -1,
     428,   430,   480,    -1,    10,   431,   387,    21,    -1,   315,
     431,   387,    21,    -1,   316,   431,   387,    21,    -1,    12,
     431,   387,    21,    -1,    11,   431,   387,    21,    -1,    -1,
      30,    -1,    -1,   378,    -1,    36,   438,    22,    -1,   437,
      47,   438,    22,    -1,    -1,    29,    -1,   439,    -1,   440,
     386,   439,    -1,   289,   431,   384,   440,   385,   387,    21,
      -1,   290,   431,   387,    21,    -1,    -1,   443,    73,   375,
      22,   382,    -1,   443,     1,    -1,    23,    -1,    24,    -1,
      25,    -1,    26,    -1,    27,    -1,    28,    -1,   444,    22,
     382,    -1,    -1,   444,   430,   446,   480,   387,   443,   388,
      -1,    -1,   447,    73,   375,    22,   382,    -1,   447,    22,
     375,     9,   382,    -1,   447,   276,   375,   282,   382,    -1,
     447,   276,   375,   409,   413,   410,   382,    -1,   447,   276,
     375,   403,   382,    -1,   447,   277,   375,   282,   382,    -1,
     447,   277,   375,   409,   413,   410,   382,    -1,   447,   278,
     375,   404,   382,    -1,   447,   279,   375,    22,   382,    -1,
     447,   280,   375,   281,   382,    -1,    -1,    -1,   272,   430,
     429,   449,   480,   450,   387,   447,   388,    -1,    -1,    -1,
     273,   430,   429,   451,   480,   452,   387,   447,   388,    -1,
      -1,    -1,   271,   430,   429,   453,   480,   454,   387,   447,
     388,    -1,    -1,   455,    22,   375,     9,   382,    -1,   455,
     274,   375,    22,   382,    -1,   455,   275,   375,   409,   408,
     410,   382,    -1,   455,   275,   375,    22,   382,    -1,    -1,
      -1,   270,   430,   429,   457,   480,   458,   387,   455,   388,
      -1,   174,    -1,   175,    -1,   176,    -1,   177,    -1,   178,
      -1,   179,    -1,     1,    -1,   182,    -1,   181,    -1,   183,
      -1,   180,    -1,     1,    -1,    -1,   461,   170,   375,   459,
     382,    -1,   461,   169,   375,   459,   382,    -1,   461,   171,
     375,   460,   382,    -1,   461,   172,   375,   460,   382,    -1,
     461,   173,   375,   460,   382,    -1,   461,     1,   382,    -1,
      -1,    14,   430,   463,   480,   387,   461,   388,    -1,   462,
       1,    -1,   184,   375,   404,   382,    -1,   185,   375,   402,
     382,    -1,   186,   375,   402,   382,    -1,   187,   375,   418,
     382,    -1,   188,   375,   404,   382,    -1,   189,   375,   402,
     382,    -1,   191,   375,   403,   382,    -1,   192,   375,   403,
     382,    -1,   193,   375,   402,   382,    -1,   194,   375,   402,
     382,    -1,   194,   375,   418,   382,    -1,   195,   375,   418,
     382,    -1,   196,   375,   418,   382,    -1,   197,   375,   402,
     382,    -1,   198,   375,   409,   418,   405,   418,   410,   382,
      -1,   200,   375,   404,   382,    -1,   201,   375,   404,   382,
      -1,   199,   375,   409,   404,   386,   404,   410,   382,    -1,
     202,   375,   402,   382,    -1,   203,   375,   402,   382,    -1,
     204,   375,   402,   382,    -1,   206,   375,   402,   382,    -1,
     207,   375,   402,   382,    -1,   209,   375,   418,   382,    -1,
     210,   375,   404,   382,    -1,   208,   375,   409,   418,   386,
     404,   410,   382,    -1,   213,   375,   402,   382,    -1,   211,
     375,   387,   418,   386,   418,   388,   382,    -1,   212,   375,
     387,   418,   386,   418,   386,   418,   386,   418,   388,   382,
      -1,   214,   375,   418,   382,    -1,   215,   375,   418,   382,
      -1,   216,   375,   418,   382,    -1,   217,   375,   418,   382,
      -1,   218,   375,   418,   382,    -1,   219,   375,   418,   382,
      -1,   222,   375,   418,   382,    -1,   223,   375,   418,   382,
      -1,   221,   375,   418,   382,    -1,   220,   375,   409,   418,
     386,   418,   410,   382,    -1,   224,   375,   409,   404,   405,
     404,   405,   404,   405,   404,   410,   382,    -1,   225,   375,
     402,   382,    -1,   226,   375,   402,   382,    -1,   227,   375,
     402,   382,    -1,   228,   375,   418,   382,    -1,   229,   375,
     409,   402,   405,   402,   405,   402,   405,   402,   410,   382,
      -1,   230,   375,   402,   382,    -1,   231,   375,   402,   382,
      -1,   233,   375,   402,   382,    -1,   234,   375,   402,   382,
      -1,   235,   375,   402,   382,    -1,   236,   375,   418,   382,
      -1,   237,   375,   402,   382,    -1,   238,   375,   402,   382,
      -1,   239,   375,   402,   382,    -1,   240,   375,   409,   404,
     405,   404,   410,   382,    -1,   241,   375,   402,   382,    -1,
     242,   375,   418,   382,    -1,   242,   375,   387,   418,   386,
     403,   386,   403,   388,   382,    -1,   243,   375,   387,   418,
     386,   418,   386,   403,   386,   403,   388,   382,    -1,   244,
     375,   403,   382,    -1,   245,   375,   403,   382,    -1,   252,
     375,   403,   382,    -1,   246,   375,   418,   382,    -1,   247,
     375,   418,   382,    -1,   248,   375,   418,   382,    -1,   249,
     375,   387,   418,   386,   418,   386,   418,   388,   382,    -1,
     250,   375,   387,   418,   386,   418,   386,   418,   386,   418,
     388,   382,    -1,   253,   375,   418,   382,    -1,   253,   375,
     387,   418,   386,   403,   386,   403,   388,   382,    -1,   254,
     375,   403,   382,    -1,   255,   375,   403,   382,    -1,   256,
     375,   418,   382,    -1,   257,   375,   418,   382,    -1,   258,
     375,   418,   382,    -1,   259,   375,   403,   382,    -1,    -1,
     465,   184,   375,   404,   382,    -1,   465,   185,   375,   402,
     382,    -1,   465,   186,   375,   402,   382,    -1,   465,   187,
     375,   418,   382,    -1,   465,   188,   375,   404,   382,    -1,
     465,   189,   375,   402,   382,    -1,   465,   191,   375,   403,
     382,    -1,   465,   192,   375,   403,   382,    -1,   465,   193,
     375,   402,   382,    -1,   465,   194,   375,   402,   382,    -1,
     465,   195,   375,   418,   382,    -1,   465,   196,   375,   418,
     382,    -1,   465,   197,   375,   402,   382,    -1,   465,   198,
     375,   418,   382,    -1,   465,   198,   375,   409,   418,   405,
     418,   410,   382,    -1,   465,   200,   375,   404,   382,    -1,
     465,   201,   375,   404,   382,    -1,   465,   202,   375,   402,
     382,    -1,   465,   203,   375,   402,   382,    -1,   465,   204,
     375,   402,   382,    -1,   465,   206,   375,   402,   382,    -1,
     465,     1,   382,    -1,    -1,    18,   430,   467,   480,   387,
     465,   388,    -1,   466,     1,    -1,    -1,   468,   207,   375,
     402,   382,    -1,   468,   209,   375,   418,   382,    -1,   468,
     210,   375,   404,   382,    -1,   468,   213,   375,   402,   382,
      -1,   468,   214,   375,   418,   382,    -1,   468,   215,   375,
     418,   382,    -1,   468,   216,   375,   418,   382,    -1,   468,
     217,   375,   418,   382,    -1,   468,   218,   375,   418,   382,
      -1,   468,   219,   375,   418,   382,    -1,   468,   222,   375,
     418,   382,    -1,   468,   223,   375,   418,   382,    -1,   221,
     375,   418,   382,    -1,   220,   375,   409,   418,   386,   418,
     410,   382,    -1,   468,   224,   375,   409,   404,   405,   404,
     405,   404,   405,   404,   410,   382,    -1,   468,   225,   375,
     402,   382,    -1,   468,   226,   375,   402,   382,    -1,   468,
     227,   375,   402,   382,    -1,   468,   228,   375,   418,   382,
      -1,   468,   229,   375,   409,   402,   405,   402,   405,   402,
     405,   402,   410,   382,    -1,   468,   230,   375,   402,   382,
      -1,   468,   231,   375,   402,   382,    -1,   468,   233,   375,
     402,   382,    -1,   468,   234,   375,   402,   382,    -1,   468,
       1,   382,    -1,    -1,    17,   430,   470,   480,   387,   468,
     388,    -1,   469,     1,    -1,    -1,   471,   235,   375,   402,
     382,    -1,   471,   236,   375,   418,   382,    -1,   471,   237,
     375,   402,   382,    -1,   471,   238,   375,   402,   382,    -1,
     471,   239,   375,   402,   382,    -1,   471,   240,   375,   409,
     404,   405,   404,   410,   382,    -1,   471,   241,   375,   402,
     382,    -1,   471,   242,   375,   418,   382,    -1,   471,   244,
     375,   403,   382,    -1,   471,   245,   375,   403,   382,    -1,
     471,   246,   375,   418,   382,    -1,   471,   247,   375,   418,
     382,    -1,   471,   248,   375,   418,   382,    -1,   471,   252,
     375,   403,   382,    -1,   471,   253,   375,   418,   382,    -1,
     471,   254,   375,   403,   382,    -1,   471,   255,   375,   403,
     382,    -1,   471,   256,   375,   418,   382,    -1,   471,   257,
     375,   418,   382,    -1,   471,   258,   375,   418,   382,    -1,
     471,   259,   375,   403,   382,    -1,   471,     1,   382,    -1,
      -1,    15,   430,   473,   480,   387,   471,   388,    -1,   472,
       1,   382,    -1,   319,   375,   387,   404,   386,   404,   388,
     382,    -1,   320,   375,   418,   382,    -1,   321,   375,   418,
     382,    -1,   322,   392,   403,   393,   375,   387,   418,   386,
     403,   386,   404,   388,   382,    -1,   323,   392,   403,   393,
     375,   387,   418,   386,   418,   386,   404,   388,   382,    -1,
     324,   375,   418,   382,    -1,   325,   375,   404,   382,    -1,
     326,   375,   418,   382,    -1,   327,   375,   418,   382,    -1,
     328,   375,   418,   382,    -1,   329,   375,   418,   382,    -1,
     330,   375,   418,   382,    -1,   331,   375,   404,   382,    -1,
     332,   375,   404,   382,    -1,   333,   375,   418,   382,    -1,
     334,   375,   404,   382,    -1,   335,   375,   418,   382,    -1,
     336,   375,   403,   382,    -1,   337,   375,   418,   382,    -1,
     340,   375,   418,   382,    -1,   338,   375,   418,   382,    -1,
     339,   375,   418,   382,    -1,    -1,   475,   319,   375,   387,
     404,   386,   404,   388,   382,    -1,   475,   320,   375,   418,
     382,    -1,   475,   321,   375,   418,   382,    -1,   475,   322,
     392,   403,   393,   375,   387,   418,   386,   403,   386,   404,
     388,   382,    -1,   475,   323,   392,   403,   393,   375,   387,
     418,   386,   418,   386,   404,   388,   382,    -1,   475,   324,
     375,   418,   382,    -1,   475,   325,   375,   404,   382,    -1,
     475,   326,   375,   418,   382,    -1,   475,   327,   375,   418,
     382,    -1,   475,   328,   375,   418,   382,    -1,   475,   329,
     375,   418,   382,    -1,   475,   330,   375,   418,   382,    -1,
     475,   331,   375,   404,   382,    -1,   475,   332,   375,   404,
     382,    -1,   475,   333,   375,   418,   382,    -1,   475,   334,
     375,   404,   382,    -1,   475,   335,   375,   418,   382,    -1,
     475,   336,   375,   403,   382,    -1,   475,   337,   375,   418,
     382,    -1,   475,   340,   375,   418,   382,    -1,   475,   338,
     375,   418,   382,    -1,   475,   339,   375,   418,   382,    -1,
     475,     1,   382,    -1,    -1,    16,   430,   477,   480,   387,
     475,   388,    -1,   476,     1,   382,    -1,    -1,   478,    22,
     375,     9,   382,    -1,   478,    22,   375,   404,   382,    -1,
     478,    22,   375,   400,   382,    -1,   478,    22,   375,   401,
     382,    -1,   478,     1,    -1,   390,   478,   391,    -1,    -1,
     479,    -1,   375,   399,    -1,   375,   409,   411,   410,    -1,
     375,   409,   412,   410,    -1,   375,   400,    -1,   375,     1,
      -1,   375,   401,    -1,   375,   409,   415,   410,    -1,    31,
      -1,    32,    -1,    33,    -1,    34,    -1,    34,   428,    -1,
      34,   487,    -1,    34,   428,   487,    -1,   428,    -1,   487,
      -1,   428,   487,    -1,   481,    -1,   482,    -1,   483,    -1,
     484,    -1,   485,    -1,   486,    -1,     1,    -1,   383,    22,
      -1,   392,   393,    -1,   392,   400,   393,    -1,    -1,   479,
      -1,   489,    -1,   479,   489,    -1,   490,    -1,   490,   479,
      -1,   490,   489,    -1,   490,   479,   489,    -1,   491,    -1,
     491,   479,    -1,   491,   489,    -1,   491,   479,   489,    -1,
     491,   490,    -1,   491,   490,   479,    -1,   491,   490,   489,
      -1,   491,   490,   479,   489,    -1,    36,    -1,    37,    -1,
      38,    -1,    39,    -1,    47,    -1,    48,    -1,    49,    -1,
      50,    -1,    43,    -1,    44,    -1,    45,    -1,    46,    -1,
      40,    -1,    41,    -1,    42,    -1,    51,    -1,    52,    -1,
      53,    -1,    54,    -1,    55,    -1,    56,    -1,    57,    -1,
      58,    -1,    59,    -1,    60,    -1,     1,    -1,    -1,   488,
     493,    22,   495,   492,   382,    -1,   494,    -1,   496,   494,
      -1,   432,    -1,   496,    -1,   436,    -1,   435,    -1,   442,
      -1,   441,    -1,   445,    -1,   448,    -1,   456,    -1,   462,
      -1,   472,    -1,   476,    -1,   469,    -1,   466,    -1,   502,
      -1,   433,    -1,   434,    -1,   497,    -1,   498,   497,    -1,
      -1,     3,    22,   500,   480,   387,   498,   388,    -1,    -1,
     387,   388,    -1,   387,   496,   388,    -1,    -1,    35,   431,
     503,   501,    -1,   297,   375,   409,   408,   410,   382,    -1,
     504,   298,   375,   409,   408,   410,   382,    -1,   504,   299,
     375,    22,   382,    -1,   504,   300,   375,    22,   382,    -1,
     504,   301,   375,    22,   382,    -1,   504,   302,   375,    22,
     382,    -1,   504,   303,   375,   400,   382,    -1,   504,   304,
     375,    22,   382,    -1,   504,    34,   384,    22,   385,   375,
     409,   408,   410,   382,    -1,   504,    34,   384,    22,   385,
     375,    22,   382,    -1,   504,    34,   384,    22,   385,   489,
     382,    -1,    -1,    -1,   295,   431,   506,   480,   507,   387,
     504,   388,    -1,    -1,   508,   307,   375,   409,   408,   410,
     382,    -1,   508,   306,   375,   409,   408,   410,   382,    -1,
     508,    34,   384,    22,   385,   375,   409,   408,   410,   382,
      -1,   508,    34,   384,    22,   385,   375,    22,   382,    -1,
     508,    34,   384,    22,   385,   489,   382,    -1,    -1,    -1,
     305,   431,   510,   480,   511,   387,   508,   388,    -1,    -1,
     512,   280,   375,   281,   382,    -1,   512,   277,   375,   282,
     382,    -1,   512,   277,   375,   409,   413,   410,   382,    -1,
      -1,    -1,   308,    22,   514,   480,   515,   387,   512,   388,
      -1,    -1,   516,   310,   375,     9,   382,    -1,   516,   311,
     375,     9,   382,    -1,    -1,    -1,   309,   431,   518,   480,
     519,   387,   516,   388,    -1
};

/* YYRLINE[YYN] -- source line where rule number YYN was defined.  */
static const yytype_uint16 yyrline[] =
{
       0,   997,   997,   998,   999,  1000,  1002,  1004,  1006,  1008,
    1009,  1010,  1011,  1012,  1013,  1014,  1015,  1016,  1017,  1018,
    1019,  1020,  1021,  1023,  1025,  1026,  1027,  1028,  1029,  1032,
    1041,  1042,  1048,  1049,  1050,  1051,  1052,  1053,  1054,  1055,
    1056,  1057,  1058,  1059,  1060,  1061,  1062,  1063,  1064,  1065,
    1066,  1067,  1075,  1076,  1077,  1078,  1079,  1080,  1085,  1086,
    1092,  1093,  1096,  1100,  1101,  1105,  1108,  1109,  1110,  1128,
    1130,  1135,  1136,  1151,  1168,  1184,  1185,  1192,  1193,  1198,
    1210,  1215,  1223,  1227,  1231,  1235,  1239,  1245,  1249,  1253,
    1256,  1265,  1276,  1280,  1284,  1287,  1291,  1311,  1315,  1319,
    1348,  1353,  1354,  1355,  1359,  1363,  1380,  1381,  1382,  1383,
    1384,  1385,  1386,  1387,  1388,  1389,  1390,  1391,  1392,  1393,
    1394,  1395,  1396,  1397,  1398,  1399,  1400,  1401,  1402,  1403,
    1404,  1405,  1406,  1407,  1408,  1409,  1410,  1411,  1412,  1413,
    1414,  1415,  1416,  1417,  1418,  1419,  1420,  1421,  1422,  1423,
    1424,  1425,  1426,  1427,  1428,  1429,  1430,  1431,  1432,  1433,
    1434,  1435,  1436,  1437,  1438,  1439,  1440,  1441,  1442,  1443,
    1444,  1445,  1446,  1447,  1448,  1450,  1451,  1452,  1453,  1454,
    1455,  1456,  1457,  1458,  1459,  1460,  1461,  1462,  1463,  1464,
    1465,  1466,  1467,  1468,  1469,  1470,  1471,  1472,  1473,  1474,
    1475,  1476,  1477,  1478,  1479,  1480,  1481,  1485,  1486,  1493,
    1494,  1495,  1496,  1501,  1505,  1510,  1516,  1521,  1527,  1532,
    1538,  1543,  1549,  1554,  1560,  1565,  1571,  1576,  1582,  1587,
    1593,  1598,  1604,  1609,  1615,  1620,  1626,  1631,  1637,  1642,
    1648,  1653,  1659,  1674,  1683,  1740,  1759,  1787,  1796,  1801,
    1805,  1822,  1839,  1856,  1873,  1890,  1907,  1924,  1941,  1958,
    1975,  1992,  2005,  2018,  2031,  2044,  2057,  2070,  2091,  2112,
    2133,  2143,  2153,  2163,  2173,  2183,  2196,  2199,  2202,  2216,
    2219,  2233,  2243,  2247,  2254,  2275,  2278,  2284,  2299,  2309,
    2355,  2363,  2380,  2400,  2404,  2413,  2413,  2427,  2427,  2445,
    2449,  2453,  2464,  2482,  2483,  2484,  2490,  2490,  2498,  2498,
    2513,  2514,  2519,  2520,  2521,  2529,  2538,  2539,  2540,  2553,
    2577,  2598,  2619,  2657,  2695,  2696,  2698,  2699,  2704,  2709,
    2718,  2719,  2720,  2724,  2732,  2792,  2833,  2834,  2862,  2869,
    2870,  2871,  2872,  2873,  2874,  2876,  2882,  2882,  2898,  2899,
    2909,  2915,  2919,  2924,  2928,  2932,  2938,  2942,  2955,  2967,
    2974,  2967,  2982,  2989,  2982,  2997,  3004,  2997,  3016,  3017,
    3026,  3040,  3061,  3078,  3085,  3078,  3098,  3099,  3100,  3101,
    3102,  3103,  3104,  3107,  3108,  3109,  3110,  3111,  3118,  3119,
    3122,  3125,  3128,  3131,  3134,  3139,  3139,  3149,  3164,  3165,
    3166,  3167,  3168,  3169,  3171,  3172,  3173,  3174,  3175,  3176,
    3177,  3178,  3179,  3180,  3181,  3182,  3187,  3188,  3189,  3191,
    3192,  3193,  3194,  3195,  3200,  3201,  3206,  3213,  3214,  3215,
    3216,  3217,  3218,  3220,  3221,  3222,  3227,  3236,  3238,  3239,
    3240,  3241,  3242,  3249,  3250,  3252,  3253,  3254,  3255,  3256,
    3257,  3258,  3259,  3261,  3262,  3263,  3269,  3275,  3276,  3277,
    3278,  3279,  3280,  3281,  3288,  3301,  3302,  3308,  3309,  3310,
    3311,  3312,  3313,  3319,  3320,  3321,  3322,  3323,  3324,  3325,
    3327,  3328,  3329,  3330,  3331,  3332,  3333,  3334,  3335,  3336,
    3337,  3338,  3339,  3340,  3342,  3343,  3348,  3348,  3358,  3363,
    3364,  3365,  3366,  3367,  3368,  3369,  3370,  3371,  3372,  3373,
    3375,  3376,  3377,  3382,  3391,  3393,  3394,  3395,  3396,  3397,
    3404,  3405,  3407,  3408,  3409,  3414,  3414,  3424,  3429,  3430,
    3431,  3432,  3433,  3434,  3435,  3437,  3438,  3439,  3440,  3441,
    3442,  3443,  3444,  3452,  3453,  3454,  3455,  3456,  3457,  3458,
    3459,  3464,  3464,  3474,  3477,  3478,  3479,  3480,  3481,  3482,
    3483,  3484,  3485,  3486,  3487,  3488,  3489,  3490,  3491,  3492,
    3493,  3494,  3495,  3496,  3497,  3498,  3506,  3507,  3508,  3513,
    3518,  3523,  3528,  3533,  3538,  3543,  3548,  3553,  3558,  3563,
    3568,  3573,  3578,  3583,  3588,  3593,  3598,  3603,  3608,  3619,
    3625,  3625,  3641,  3651,  3652,  3659,  3665,  3671,  3683,  3686,
    3688,  3689,  3694,  3704,  3711,  3717,  3724,  3736,  3746,  3752,
    3753,  3754,  3761,  3766,  3771,  3776,  3781,  3790,  3795,  3806,
    3809,  3812,  3815,  3821,  3824,  3827,  3833,  3840,  3843,  3854,
    3858,  3862,  3867,  3872,  3877,  3882,  3888,  3894,  3899,  3904,
    3910,  3916,  3922,  3928,  3935,  3947,  3948,  3949,  3950,  3951,
    3952,  3953,  3954,  3955,  3956,  3957,  3958,  3959,  3960,  3961,
    3962,  3963,  3964,  3965,  3966,  3967,  3968,  3969,  3970,  3971,
    3972,  3978,  3978,  4062,  4063,  4067,  4068,  4069,  4070,  4071,
    4072,  4073,  4074,  4075,  4076,  4077,  4078,  4079,  4080,  4081,
    4082,  4083,  4087,  4088,  4101,  4101,  4120,  4121,  4122,  4127,
    4127,  4169,  4172,  4175,  4178,  4181,  4184,  4187,  4189,  4192,
    4199,  4204,  4220,  4221,  4220,  4230,  4231,  4234,  4237,  4244,
    4249,  4265,  4266,  4265,  4275,  4276,  4284,  4288,  4296,  4298,
    4296,  4307,  4308,  4311,  4316,  4317,  4316
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
  "GLPOINTSIZE", "GLPOINTSMOOTH", "GLPOINTSPRITE",
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
     623,   624,   625,   626,   627,    61,    45,    43,    42,    47,
     628,    94,    59,    58,    40,    41,    44,   123,   125,   124,
      60,    62,    91,    93,    46
};
# endif

/* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_uint16 yyr1[] =
{
       0,   395,   396,   396,   396,   396,   396,   396,   396,   396,
     396,   396,   396,   396,   396,   396,   396,   396,   396,   396,
     396,   396,   396,   396,   396,   396,   396,   396,   396,   397,
     398,   398,   399,   399,   399,   399,   399,   399,   399,   399,
     399,   399,   399,   399,   399,   399,   399,   399,   399,   399,
     399,   399,   400,   400,   400,   400,   400,   400,   401,   401,
     402,   402,   402,   403,   403,   403,   404,   404,   404,   405,
     405,   406,   406,   406,   407,   407,   407,   408,   408,   408,
     409,   410,   411,   411,   411,   411,   411,   412,   412,   412,
     413,   413,   414,   414,   414,   415,   415,   416,   416,   416,
     416,   417,   417,   417,   417,   417,   418,   418,   418,   418,
     418,   418,   418,   418,   418,   418,   418,   418,   418,   418,
     418,   418,   418,   418,   418,   418,   418,   418,   418,   418,
     418,   418,   418,   418,   418,   418,   418,   418,   418,   418,
     418,   418,   418,   418,   418,   418,   418,   418,   418,   418,
     418,   418,   418,   418,   418,   418,   418,   418,   418,   418,
     418,   418,   418,   418,   418,   418,   418,   418,   418,   418,
     418,   418,   418,   418,   418,   418,   418,   418,   418,   418,
     418,   418,   418,   418,   418,   418,   418,   418,   418,   418,
     418,   418,   418,   418,   418,   418,   418,   418,   418,   418,
     418,   418,   418,   418,   418,   418,   418,   419,   419,   420,
     420,   420,   420,   420,   420,   420,   420,   420,   420,   420,
     420,   420,   420,   420,   420,   420,   420,   420,   420,   420,
     420,   420,   420,   420,   420,   420,   420,   420,   420,   420,
     420,   420,   420,   420,   420,   420,   420,   420,   420,   420,
     420,   420,   420,   420,   420,   420,   420,   420,   420,   420,
     420,   420,   420,   420,   420,   420,   420,   420,   420,   420,
     420,   420,   420,   420,   420,   420,   420,   420,   420,   420,
     420,   420,   420,   420,   420,   420,   420,   420,   420,   420,
     420,   420,   420,   420,   420,   422,   421,   423,   421,   421,
     421,   421,   421,   424,   424,   424,   426,   425,   427,   425,
     428,   428,   429,   429,   429,   430,   431,   431,   431,   432,
     433,   434,   435,   436,   437,   437,   438,   438,   439,   439,
     440,   440,   440,   440,   441,   442,   443,   443,   443,   444,
     444,   444,   444,   444,   444,   445,   446,   445,   447,   447,
     447,   447,   447,   447,   447,   447,   447,   447,   447,   449,
     450,   448,   451,   452,   448,   453,   454,   448,   455,   455,
     455,   455,   455,   457,   458,   456,   459,   459,   459,   459,
     459,   459,   459,   460,   460,   460,   460,   460,   461,   461,
     461,   461,   461,   461,   461,   463,   462,   462,   464,   464,
     464,   464,   464,   464,   464,   464,   464,   464,   464,   464,
     464,   464,   464,   464,   464,   464,   464,   464,   464,   464,
     464,   464,   464,   464,   464,   464,   464,   464,   464,   464,
     464,   464,   464,   464,   464,   464,   464,   464,   464,   464,
     464,   464,   464,   464,   464,   464,   464,   464,   464,   464,
     464,   464,   464,   464,   464,   464,   464,   464,   464,   464,
     464,   464,   464,   464,   464,   464,   464,   464,   464,   464,
     464,   464,   464,   465,   465,   465,   465,   465,   465,   465,
     465,   465,   465,   465,   465,   465,   465,   465,   465,   465,
     465,   465,   465,   465,   465,   465,   467,   466,   466,   468,
     468,   468,   468,   468,   468,   468,   468,   468,   468,   468,
     468,   468,   468,   468,   468,   468,   468,   468,   468,   468,
     468,   468,   468,   468,   468,   470,   469,   469,   471,   471,
     471,   471,   471,   471,   471,   471,   471,   471,   471,   471,
     471,   471,   471,   471,   471,   471,   471,   471,   471,   471,
     471,   473,   472,   472,   474,   474,   474,   474,   474,   474,
     474,   474,   474,   474,   474,   474,   474,   474,   474,   474,
     474,   474,   474,   474,   474,   474,   475,   475,   475,   475,
     475,   475,   475,   475,   475,   475,   475,   475,   475,   475,
     475,   475,   475,   475,   475,   475,   475,   475,   475,   475,
     477,   476,   476,   478,   478,   478,   478,   478,   478,   479,
     480,   480,   481,   482,   483,   484,   484,   485,   486,   487,
     487,   487,   488,   488,   488,   488,   488,   488,   488,   489,
     489,   489,   489,   489,   489,   489,   490,   491,   491,   492,
     492,   492,   492,   492,   492,   492,   492,   492,   492,   492,
     492,   492,   492,   492,   492,   493,   493,   493,   493,   493,
     493,   493,   493,   493,   493,   493,   493,   493,   493,   493,
     493,   493,   493,   493,   493,   493,   493,   493,   493,   493,
     493,   495,   494,   496,   496,   497,   497,   497,   497,   497,
     497,   497,   497,   497,   497,   497,   497,   497,   497,   497,
     497,   497,   498,   498,   500,   499,   501,   501,   501,   503,
     502,   504,   504,   504,   504,   504,   504,   504,   504,   504,
     504,   504,   506,   507,   505,   508,   508,   508,   508,   508,
     508,   510,   511,   509,   512,   512,   512,   512,   514,   515,
     513,   516,   516,   516,   518,   519,   517
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
       4,     4,     8,     4,     4,     8,     4,     4,     4,     4,
       4,     4,     4,     8,     4,     8,    12,     4,     4,     4,
       4,     4,     4,     4,     4,     4,     8,    12,     4,     4,
       4,     4,    12,     4,     4,     4,     4,     4,     4,     4,
       4,     4,     8,     4,     4,    10,    12,     4,     4,     4,
       4,     4,     4,    10,    12,     4,    10,     4,     4,     4,
       4,     4,     4,     0,     5,     5,     5,     5,     5,     5,
       5,     5,     5,     5,     5,     5,     5,     5,     9,     5,
       5,     5,     5,     5,     5,     3,     0,     7,     2,     0,
       5,     5,     5,     5,     5,     5,     5,     5,     5,     5,
       5,     5,     4,     8,    13,     5,     5,     5,     5,    13,
       5,     5,     5,     5,     3,     0,     7,     2,     0,     5,
       5,     5,     5,     5,     9,     5,     5,     5,     5,     5,
       5,     5,     5,     5,     5,     5,     5,     5,     5,     5,
       3,     0,     7,     3,     8,     4,     4,    13,    13,     4,
       4,     4,     4,     4,     4,     4,     4,     4,     4,     4,
       4,     4,     4,     4,     4,     4,     0,     9,     5,     5,
      14,    14,     5,     5,     5,     5,     5,     5,     5,     5,
       5,     5,     5,     5,     5,     5,     5,     5,     5,     3,
       0,     7,     3,     0,     5,     5,     5,     5,     2,     3,
       0,     1,     2,     4,     4,     2,     2,     2,     4,     1,
       1,     1,     1,     2,     2,     3,     1,     1,     2,     1,
       1,     1,     1,     1,     1,     1,     2,     2,     3,     0,
       1,     1,     2,     1,     2,     2,     3,     1,     2,     2,
       3,     2,     3,     3,     4,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     0,     6,     1,     2,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     2,     0,     7,     0,     2,     3,     0,
       4,     6,     7,     5,     5,     5,     5,     5,     5,    10,
       8,     7,     0,     0,     8,     0,     7,     7,    10,     8,
       7,     0,     0,     8,     0,     5,     5,     7,     0,     0,
       8,     0,     5,     5,     0,     0,     8
};

/* YYDEFACT[STATE-NAME] -- Default rule to reduce with in state
   STATE-NUM when YYTABLE doesn't specify something else to do.  Zero
   means the default is an error.  */
static const yytype_uint16 yydefact[] =
{
       2,     0,     1,    28,     0,   316,   316,   316,     0,     0,
       0,     0,     0,   306,   339,   340,   341,   342,   343,   344,
     619,   620,   621,   622,   316,     0,     0,     0,     0,     0,
     311,   310,   316,   316,   316,   316,     0,   316,   316,   316,
       3,     4,     5,   626,     6,    22,    23,     8,     7,    10,
       9,     0,    11,    12,    13,     0,     0,     0,     0,     0,
     627,     0,   683,    21,    19,    20,    24,    25,    26,    27,
     704,   315,     0,   610,     0,     0,     0,   395,   551,   600,
     525,   496,   308,   610,   623,   624,   709,     0,   312,   312,
     312,   312,     0,     0,   722,   731,   738,   744,     0,     0,
     628,   315,   346,   397,   498,   527,     0,     0,   680,   655,
     656,   657,   658,   667,   668,   669,   663,   664,   665,   666,
     659,   660,   661,   662,   670,   671,   672,   673,   674,   675,
     676,   677,   678,   679,     0,   684,   610,   610,   603,   611,
     317,     0,     0,     0,   610,   610,   610,   610,   610,   610,
       0,   625,   706,     0,   314,   313,   373,   365,   359,   362,
     330,     0,   610,   610,   610,   610,     0,     0,   345,   610,
     553,   602,   681,     0,   318,     0,   319,   323,   322,     0,
       0,     0,     0,     0,     0,     0,     0,   710,    58,    59,
      29,   610,   610,   610,   610,   331,   325,   326,     0,   332,
       0,   335,   723,   732,   739,   745,   320,   321,     0,     0,
       0,   608,     0,   609,   388,   528,   576,   499,   473,     0,
       0,   295,     0,   303,     0,   707,     0,   374,   366,   360,
     363,   327,     0,   326,     0,   324,     0,     0,     0,     0,
     336,   635,     0,     0,     0,     0,   629,   630,   631,   632,
     633,   634,   641,     0,     0,     0,   685,   700,   701,   688,
     687,   690,   689,   691,   692,   693,     0,     0,     0,     0,
       0,   686,   702,     0,   699,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   297,   610,     0,   305,   307,
     304,   708,     0,     0,     0,     0,   328,     0,     0,   333,
       0,   725,   734,   741,     0,   616,    32,    52,     0,     0,
      80,   612,   615,   617,     0,   636,     0,     0,   637,     0,
     642,     0,   645,     0,   649,     0,   682,   705,   703,     0,
      66,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     396,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   552,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   601,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   526,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   497,   309,     0,   301,   610,
       0,     0,     0,   368,   348,   348,   348,   329,   334,     0,
       0,     0,     0,     0,   338,     0,   347,    46,    56,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    82,    83,    95,     0,     0,     0,     0,    56,     0,
       0,     0,     0,   638,   646,   650,     0,   653,   604,   606,
     607,   605,   394,     0,     0,     0,     0,     0,   550,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     599,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   106,   107,   108,   109,   110,   111,
     112,   113,   114,   115,   116,   117,   118,   119,   120,   121,
     122,   123,   124,   125,   126,   127,   128,   129,   130,   131,
     132,   133,   134,   135,   136,   137,   138,   139,   140,   141,
     142,   143,   144,   145,   146,   147,   148,   149,   150,   151,
     152,   153,   154,   155,   156,   157,   158,   159,   160,   161,
     162,   163,   164,   165,   166,   167,   168,   169,   170,   171,
     172,   173,   174,   181,   182,   183,   184,   185,   186,   187,
     188,   189,   190,   191,   192,   193,   194,   195,   196,   197,
     198,   199,   200,   201,   202,   203,   204,   205,   206,   175,
     176,   177,   178,   179,   180,     0,   524,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   495,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     302,     0,   209,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   724,     0,
       0,     0,   733,     0,     0,   740,     0,     0,   746,     0,
      51,    57,    36,    38,    33,    35,    39,    41,    42,    44,
      47,    49,    37,    54,    34,    53,    40,    55,    43,    45,
      48,    50,     0,    86,     0,    81,   613,    89,     0,   614,
       0,   618,    54,    53,    55,   654,   382,   376,   377,   378,
     379,   380,   381,     0,     0,   387,   386,   384,   383,   385,
       0,     0,     0,    62,    61,    60,     0,     0,     0,     0,
       0,     0,     0,     0,    64,    63,    65,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    67,    68,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   512,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   209,
       0,     0,     0,     0,     0,     0,   375,     0,     0,     0,
       0,     0,     0,     0,   367,   361,   364,    77,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    87,    84,    85,     0,    96,   390,
     389,   391,   392,   393,   529,   530,   531,   532,   533,    69,
     535,   536,   537,   538,   539,   540,   541,   542,   543,   544,
     545,   546,   547,   548,   549,     0,   578,   579,     0,     0,
     582,   583,   584,   585,   586,   587,   588,   589,   590,   591,
     592,   593,   594,   595,   597,   598,   596,     0,   500,   501,
     502,   503,   504,   505,   506,   507,   508,   509,   510,   511,
      69,   515,   516,   517,   518,    69,   520,   521,   522,   523,
     474,   475,   476,   477,   478,   479,   480,   481,   482,   483,
     484,   485,   486,    69,   487,   489,   490,   491,   492,   493,
     494,     0,     0,     0,     0,     0,     0,     0,     0,     0,
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
       0,     0,     0,     0,     0,     0,     0,   296,   210,   211,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    78,    69,     0,    77,     0,     0,     0,     0,
       0,     0,     0,    77,    77,     0,     0,     0,     0,     0,
     337,     0,    70,     0,     0,     0,     0,     0,     0,     0,
       0,   298,   294,     0,     0,    74,     0,    74,     0,    74,
       0,    74,     0,    74,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    74,     0,    74,
       0,    74,     0,    74,     0,    74,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    74,     0,
       0,     0,     0,     0,     0,    74,     0,    74,     0,    74,
       0,    74,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   300,   299,     0,     0,     0,
      77,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    69,   713,   714,   715,   716,   717,
     718,     0,    69,    69,   736,    90,     0,   735,   742,   743,
      88,     0,     0,     0,     0,     0,    69,    69,     0,     0,
       0,    71,    72,    69,     0,     0,    30,    31,     0,     0,
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
       0,     0,     0,     0,   207,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   369,   370,   372,    69,   350,   349,   351,   353,
       0,   354,     0,   356,   357,   358,    79,   711,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   513,     0,     0,     0,     0,     0,     0,     0,     0,
     214,    69,     0,   216,     0,   222,     0,   224,     0,   226,
       0,   249,     0,     0,     0,   270,     0,   272,     0,   274,
       0,     0,     0,   228,     0,   230,     0,   236,     0,   238,
       0,   240,     0,     0,     0,     0,     0,     0,     0,   398,
     399,   400,   401,   402,   403,   404,   405,   406,   407,   408,
     409,   410,   411,    69,     0,   413,   414,   416,   417,   418,
     419,   420,     0,   421,   422,     0,     0,   424,   427,   428,
     429,   430,   431,   432,     0,   435,   433,   434,    69,   438,
     439,   440,   441,    69,   443,   444,   445,   446,   447,   448,
     449,   450,   451,    69,   453,     0,   454,     0,   457,   458,
     460,   461,   462,     0,     0,   459,     0,   465,   467,   468,
     469,   470,   471,   472,     0,   277,   276,   278,   279,     0,
     280,     0,     0,     0,     0,   285,   286,     0,   288,   290,
       0,    94,   291,     0,   292,   212,   213,     0,   218,     0,
     232,     0,   234,     0,   220,     0,     0,   555,   556,     0,
       0,   559,   560,   561,   562,   563,   564,   565,   566,   567,
     568,   569,   570,   571,   572,   574,   575,   573,     0,     0,
       0,     0,    77,   721,   712,     0,    77,   730,   727,   726,
      91,   737,   534,   577,     0,     0,    69,    69,   488,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    73,    76,
      74,    74,    74,    74,    74,     0,     0,     0,   271,   273,
     275,     0,     0,    74,    74,    74,    74,    74,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   208,
       0,     0,     0,     0,     0,    92,     0,     0,    74,    74,
      74,    74,     0,     0,     0,   371,   352,   355,   720,    69,
     729,    69,     0,     0,     0,     0,     0,     0,    77,   244,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    69,    69,     0,     0,
       0,     0,     0,     0,     0,   282,     0,   283,     0,   100,
      99,   102,    98,    97,   103,     0,     0,   287,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   250,   243,    69,     0,     0,     0,     0,     0,     0,
     215,   217,   223,   225,   227,   252,     0,   261,   267,   257,
       0,   263,   229,   231,   237,   239,   241,   258,   264,   251,
     253,     0,   262,   268,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   293,   281,
     284,   105,     0,     0,    93,   219,   233,   235,   221,     0,
       0,     0,   719,   728,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   412,   415,
     423,   425,     0,   436,    69,    69,   452,     0,     0,     0,
       0,     0,   104,   289,   554,     0,     0,     0,     0,   514,
     519,   242,     0,     0,     0,   254,   265,   269,   259,   266,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   580,   581,   245,     0,     0,   247,     0,
       0,     0,     0,     0,     0,   455,     0,   463,     0,   466,
       0,     0,     0,     0,   255,   260,   256,     0,     0,     0,
       0,     0,     0,     0,   246,   248,   426,   437,   442,   456,
     464,     0,     0,   557,   558
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
      -1,     1,    41,  1408,   773,   774,   775,   776,   787,   333,
    1371,  1403,  1404,  1163,  1405,   746,   475,   476,  1740,  1537,
     477,  1934,  1935,   645,  1525,   870,   223,   286,   439,   224,
      42,    83,   149,    72,   156,    73,    74,   256,   257,   258,
     259,   260,   198,   232,   199,   200,   261,   262,   304,    51,
     263,   169,   696,   264,   193,   294,   194,   295,   192,   293,
     695,   265,   191,   292,   763,   770,   276,   266,   144,  1148,
     282,   267,   148,   281,   268,   147,   277,   269,   145,  1149,
     278,   270,   146,   175,   139,   140,   246,   247,   248,   249,
     250,   251,    60,    61,   252,   253,   254,   255,   134,    62,
     209,   271,   272,   273,    64,   136,   187,   274,   152,   450,
      66,   162,   236,   451,    67,   163,   237,   452,    68,   164,
     238,   453,    69,   165,   239
};

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
#define YYPACT_NINF -1843
static const yytype_int16 yypact[] =
{
   -1843,  3569, -1843, -1843,   -10,    -7,    -7,    -7,    64,    64,
      64,    64,    64,   131, -1843, -1843, -1843, -1843, -1843, -1843,
   -1843, -1843, -1843,   278,    -7,   -20,    64,    64,    64,    64,
   -1843, -1843,    -7,    -7,    -7,    -7,   137,    -7,    -7,    -7,
   -1843, -1843, -1843,   262, -1843, -1843, -1843, -1843, -1843, -1843,
   -1843,   231, -1843, -1843, -1843,  3879,  3931,  3979,  4019,  4059,
   -1843,  2559, -1843,   319, -1843, -1843, -1843, -1843, -1843, -1843,
   -1843, -1843,    64,  -118,   -22,    19,    49, -1843, -1843, -1843,
   -1843, -1843, -1843,  -118,   262, -1843, -1843,    94,    88,    88,
      88,    88,    66,    93, -1843, -1843, -1843, -1843,   113,   124,
   -1843,   146, -1843, -1843, -1843, -1843,   158,   160, -1843, -1843,
   -1843, -1843, -1843, -1843, -1843, -1843, -1843, -1843, -1843, -1843,
   -1843, -1843, -1843, -1843, -1843, -1843, -1843, -1843, -1843, -1843,
   -1843, -1843, -1843, -1843,   472, -1843,  -118,  -118, -1843, -1843,
   -1843,   499,   527,   529,  -118,  -118,  -118,  -118,  -118,  -118,
     166, -1843,   183,   509, -1843, -1843, -1843, -1843, -1843, -1843,
     551,   555,  -118,  -118,  -118,  -118,   565,   579, -1843,  -118,
   -1843, -1843, -1843,   222, -1843,     1, -1843, -1843, -1843,   225,
     249,   265,   266,   272,   275,    97,   281, -1843, -1843, -1843,
   -1843,  -118,  -118,  -118,  -118, -1843, -1843,   252,   621, -1843,
     197, -1843, -1843, -1843, -1843, -1843, -1843, -1843,   288,     4,
    2969, -1843,   285, -1843, -1843, -1843, -1843,   371, -1843,    97,
     656,   657,   305, -1843,    26, -1843,   286, -1843, -1843, -1843,
   -1843, -1843,   674,   252,   289,   328,   310,   312,   324,   326,
   -1843, -1843,   165,   693,    48,    34, -1843, -1843, -1843, -1843,
   -1843, -1843, -1843,     9,    18,   338, -1843, -1843, -1843, -1843,
   -1843, -1843, -1843, -1843, -1843, -1843,   273,  1774,  2389,  2484,
    2683,   319, -1843,  4371, -1843,   229,    47,    10,     2,   357,
     380,   230,   556,    42,  -199, -1843,  -118,   179, -1843, -1843,
   -1843, -1843,   340,   359,   374,   382, -1843,   756,   758, -1843,
     484, -1843, -1843, -1843,    17, -1843, -1843, -1843,    95,    95,
   -1843,   613,   648, -1843,   171, -1843,   125,   125, -1843,  -151,
   -1843,   106, -1843,   107, -1843,    13, -1843, -1843, -1843,   400,
     613,   417,   410,   418,   419,   433,   447,   455,   456,   461,
   -1843,   459,   462,   470,   473,   474,   476,   479,   480,   481,
     483,   487,   502,   514,   516,   525,   526,   528,   530,   531,
     532,   542,   544, -1843,   465,   546,   548,   549,   467,   482,
     553,   558,   567,   570,   571,   572,   580,   581,   583,   590,
     595,   599,   600,   610,   611,   612,   618, -1843,   517,  4843,
     504,   624,   626,   628,   630,   633,   635,   636,   640,   641,
     642,   643,   644,   647,   653,   658,   659,   662,   663,   664,
     665,   666,   668, -1843,   547,   677,   680,   681,   682,   683,
     684,   685,   686,   687,   689,   690,   691,   692,   700,   704,
     706,   707,   710,   713,   714, -1843, -1843,   582, -1843,  -118,
     539,   569,   622, -1843, -1843, -1843, -1843, -1843, -1843,   718,
      45,   -28,  -246,  -148, -1843,   719, -1843,   651,   716,   436,
     537,    95,    95,    95,    95,    95,    95,    95,    95,    95,
      95,   613,   648, -1843,    95,    24,    25,  -276, -1843,   287,
     125,   125,   125, -1843, -1843, -1843,   133, -1843, -1843, -1843,
   -1843, -1843, -1843,   589,   589,   421,   421,   421, -1843,   175,
    4843,   175,   175,   175,   517,   175,  4843,   175,   175,  4843,
    4843,  4843,   175,  4843,   175,   175,  4843,  4843,  4843,   175,
   -1843,   711,  4843,  4843,   175,   175,  4843,   175,  4843,  4843,
    4843,  4843,  4843,   175,   175,  4843,   175,  4843,   175,  4843,
    4843,  4843,  4843,  4843, -1843, -1843, -1843, -1843, -1843, -1843,
   -1843, -1843, -1843, -1843, -1843, -1843, -1843, -1843, -1843, -1843,
   -1843, -1843, -1843, -1843, -1843, -1843, -1843, -1843, -1843, -1843,
   -1843, -1843, -1843, -1843, -1843, -1843, -1843, -1843, -1843, -1843,
   -1843, -1843, -1843, -1843, -1843, -1843, -1843, -1843, -1843, -1843,
   -1843, -1843, -1843, -1843, -1843, -1843, -1843, -1843, -1843, -1843,
   -1843, -1843, -1843, -1843, -1843, -1843, -1843, -1843, -1843, -1843,
   -1843, -1843, -1843, -1843, -1843, -1843, -1843, -1843, -1843, -1843,
   -1843, -1843, -1843, -1843, -1843, -1843, -1843, -1843, -1843, -1843,
   -1843, -1843, -1843, -1843, -1843, -1843, -1843, -1843, -1843, -1843,
   -1843, -1843, -1843, -1843, -1843,   667, -1843,   175,  4843,   175,
     175,  4843,  4843,  4843,  4843,  4843,  4843,  4843,  4843,   517,
     175,   175,   175,  4843,   517,   175,   175,   175,   175, -1843,
     175,   175,   175,  4843,   175,   175,   175,   175,   175,   175,
    4843,  4843,   175,  4417,   175,   175,   175,   175,   175,   175,
   -1843,   712, -1843,   725,   726,   -15,   511,   593,   632,   517,
     721,   731,   732,   734,   735,   736,   737,   738, -1843,   730,
     740,   741, -1843,   742,   743, -1843,   744,   745, -1843,  1046,
   -1843, -1843,  -108,   292,  -108,   292,   651,   716,   651,   716,
     651,   716,  -108,   292,  -108,   292,   651,   716,   651,   716,
     651,   716,    24, -1843,    95, -1843, -1843, -1843,   517, -1843,
     509, -1843,   746,   746, -1843, -1843, -1843, -1843, -1843, -1843,
   -1843, -1843, -1843,   739,   747, -1843, -1843, -1843, -1843, -1843,
     748,   749,   750,   613,   648, -1843,   751,   752,   753,   754,
     757,   175,   759,   760,   613,   648, -1843,   761,   763,   767,
     768,   770,   773,   775,   776,   778,   779,   781,   782,   783,
     175,   784,   785,   777,   780,   786,   648, -1843,   787,   789,
     792,   793,   795,   797,   798,   799,   800,   803,   805,   806,
     807,   808,   809,   812,   790, -1843,   814,   816,   817,   818,
     819,   820,   821,   822,   823,   824,   825,   826,   175,   829,
     830,   831,   834,   175,   835,   836,   837,   838,   839,   840,
     842,   843,   844,   845,   846,   847,   848,   849,   850,   851,
     852,  4843,   854,   855,   856,   858,   859,   860,   863, -1843,
    3129,   175,   175,   765,   871,   872, -1843,   873,   874,   875,
     876,   877,   878,   890, -1843, -1843, -1843,  1101,  1105,   517,
    1106,  1244,  1245,  1247,   125,  1248,  1249,   517,   517,  -249,
     991,  1264,  1265,   893, -1843,   613,   648,    95, -1843, -1843,
   -1843, -1843, -1843, -1843, -1843, -1843, -1843, -1843, -1843,   891,
   -1843, -1843, -1843, -1843, -1843, -1843, -1843, -1843, -1843, -1843,
   -1843, -1843, -1843, -1843, -1843,   892, -1843, -1843,   901,   905,
   -1843, -1843, -1843, -1843, -1843, -1843, -1843, -1843, -1843, -1843,
   -1843, -1843, -1843, -1843, -1843, -1843, -1843,  4843, -1843, -1843,
   -1843, -1843, -1843, -1843, -1843, -1843, -1843, -1843, -1843, -1843,
     891, -1843, -1843, -1843, -1843,   891, -1843, -1843, -1843, -1843,
   -1843, -1843, -1843, -1843, -1843, -1843, -1843, -1843, -1843, -1843,
   -1843, -1843, -1843,   891, -1843, -1843, -1843, -1843, -1843, -1843,
   -1843,  3478,   900,   899,   902,  -298,  -146,   -12,     7,   101,
     909,   903,   906,   907,  -362,  -319,  -175,   910,   911,   112,
     122,   147,   164,   204,   912,   913,   914,   915,   916,   919,
     918,   929,   930,   935,   936,   937,   939,   940,   941,   942,
     945,   947,   948,   949,   950,   954,   959,   962,   963,   964,
     965,   966,   967,   968,   969,   970,   972,   973,   974,   975,
     976,   978,   979,   980,   981,   982,   983,   986,   987,   988,
     992,   993,   994,   995,   996,   997,  1001,  1008,  1009,  1013,
    1015,  1016,  1017,  1018,  1019,  1020,  1021,  1023,  1024,  1025,
    1026,  1027,  1028,  1029,  1030,  1031,  1032,  1033,  1035,  1036,
    1037,  1042,  1034,  1044,  1045,  1047,  1048,  1051,  -278,  1052,
    1050,  1053,  1054,  1055,  1056,  1057,  1061,  1062,  1064,  1065,
    1066,   218,   220,   221,   224,  1067,  1069,  1070,   934,  1022,
    1071,  1072,  1073,  1075,  1076,  1077,  1078,  1079,  1080,  1081,
    1082,  1083,  1084,  1085,  1086,  1087,  1088, -1843, -1843, -1843,
     984,  1090,  1276,  1399,   117,  1456,  1447,   180,  -243,   175,
    1452,  1196, -1843,  -221,  1096,  1101,  1104,  1107,  1108,  1110,
     316,  1111,  1098,  1101,  1101,  1112,   125,  1114,  1115,  1116,
   -1843,    24, -1843,   175,   175,  1100,  1113,  1117,   175,   175,
    4843, -1843, -1843,  1479,  1480,    44,   294,    44,   294,    44,
     294,    44,   294,    44,   294,  1494,  1482,  1484,  1485,  1486,
    1487,  1488,  1489,  1492,  1495,  1496,  1497,    44,   294,    44,
     294,    44,   294,    44,   294,    44,   294,  1498,  1501,  1503,
    1505,  1506,  1508,   175,   175,   175,  4843,   175,   175,   175,
     175,   175,  4346,  4843,  4843,   175,   517,   517,   175,   175,
     175,   175,   175,   175,   175,   517,  4843,   175,  1129,  1144,
     175,  4843,  4843,  4843,  4843,  4843,  4843,   517,  4843,  4843,
    4843,   517,   175,   175,   175,  4843,   517,   175,   175,   175,
     175,   175,  4843,   175,   175,   175,   517,   175,  4705,  1145,
     175,   175,  4843,  4843,  4843,  1146,  1147,   175,  4774,   175,
     175,  4843,  4843,  4843,   175,   294,   175,  1272,  1517,  1274,
    1520,   517,   175,   517,  1522,   175,   175,  1158,    44,   125,
     517,   517,  1524,   125,   517,    44,   294,    44,   294,    44,
     294,    44,   294,  1160,  4843,  4843,   175,   175,  4843,   175,
    4843,  4843,  4843,  4843,  4843,   175,   175,  4843,   175,  4843,
     175,  4843,  4843,  4843,  4843, -1843, -1843,  1166,  1167,  1168,
    1101,  1169,  1170,  1171,  1172,   125,  1173,   125,  1175,  1176,
    1178,  1539,  1180,    29,  -221, -1843, -1843, -1843, -1843, -1843,
   -1843,    67,  -221,  -221, -1843,    -8,   148, -1843, -1843, -1843,
   -1843,  1117,  1177,  4843,  4843,  1181,   891,   891,  1117,  1179,
     241, -1843, -1843,  -154,  1185,   169, -1843, -1843,  1183,  1186,
    1187,  1188,  1189,  1190,  1191,  1194,  1192,  1195,  1184,  1199,
    1200,  1197,  1201,  1206,  1204,  1208,  1207,  1210,  1211,  1209,
    1202,  1215,  1212,  1216,  1214,  1217,  1218,  1219,  1220,  1221,
    1222,  1223,  1227,  1228,  1229,  1233,  1234,  1235,  1236,  1237,
    1238,  1239,  1240,  1242,  1243,  1246,  1252,  1253,  1255,  4843,
     175,  1256,  1257,  1258,  1260,  1261,  1262,  1263,  4843,  1266,
    1267,  4843,  4843,  1268,  1269,  1270,  1271,  1273,  1275,  1277,
    4843,  1279,  1280,  1281,   175,  1282,  1285,  1286,  1287,   175,
    1288,  1289,  1290,  1291,  1292,  1294,  1295,  1296,  1297,   175,
    1298,  4843,  1299,  4843,  1302,  1303,  1304,  1306,  1309,  4843,
    4843,  1310,  4843,  1311,  1314,  1315,  1316,  1317,  1318,  1320,
    1319,  1324,  1325,  1326, -1843,  -177,  1328,    95,  1230,   125,
    1327,  1332,  1333,  1559,  1334,   352,   125,    43,    84,  1336,
     398,   125,  1337,  1329,  1341,  1338,  1345,  1342,  1346,  1343,
     175,  1350,  1353,  1347,  1348,  1360,  1361,  1362,  1363,  1364,
    1365,  1366,  1369,  1370,  1375,  1376,  1377,  1379,  1380,  1381,
    1387,  1388, -1843, -1843, -1843,  -221, -1843, -1843, -1843, -1843,
     148, -1843,   148, -1843, -1843, -1843, -1843, -1843,   119,  1390,
    1391,   142,  1392,  1394,  1395,   125,  1398,  1400,  1405,  1407,
    1410, -1843,   175,   175,  1421,  1225,    90,   125,   609,  1604,
   -1843,  -221,  1283, -1843,  1406, -1843,  1429, -1843,  1435, -1843,
    1436, -1843,  1437,  1438,  1439, -1843,  1433, -1843,  1434, -1843,
    1440,  1442,  1443, -1843,  1445, -1843,  1446, -1843,  1448, -1843,
    1449, -1843,  1450,  1451,  1453,  1455,  1457,  1459,  1460, -1843,
   -1843, -1843, -1843, -1843, -1843, -1843, -1843, -1843, -1843, -1843,
   -1843, -1843, -1843,   891,  1462, -1843, -1843, -1843, -1843, -1843,
   -1843, -1843,  1464, -1843, -1843,  1468,  1470, -1843, -1843, -1843,
   -1843, -1843, -1843, -1843,  1471, -1843, -1843, -1843,   891, -1843,
   -1843, -1843, -1843,   891, -1843, -1843, -1843, -1843, -1843, -1843,
   -1843, -1843, -1843,   891, -1843,  1472, -1843,  1473, -1843, -1843,
   -1843, -1843, -1843,  1475,  1477, -1843,  1478, -1843, -1843, -1843,
   -1843, -1843, -1843, -1843,  1461, -1843, -1843, -1843, -1843,  1572,
   -1843,    24,  1467,   148,  1605, -1843, -1843,  1481, -1843, -1843,
     148, -1843, -1843,   517, -1843, -1843, -1843,   148, -1843,  1490,
   -1843,  1491, -1843,  1493, -1843,  1499,  1483, -1843, -1843,  1500,
    1504, -1843, -1843, -1843, -1843, -1843, -1843, -1843, -1843, -1843,
   -1843, -1843, -1843, -1843, -1843, -1843, -1843, -1843,  1463,  1502,
    1507,  1509,    99, -1843, -1843,  1511,    99, -1843, -1843, -1843,
      -8, -1843, -1843, -1843,   175,  4843,   891,   891, -1843,  1608,
     151,  1512,  -101,  1454,  1458,  1510,  1513,  1514, -1843, -1843,
      44,    44,    44,    44,    44,   118,   125,  1609, -1843, -1843,
   -1843,   129,   125,    44,    44,    44,    44,    44,  1624,   125,
    1632,   132,   125,  1634,  4843,   175,   175,  4843,  4843,  4843,
     175,   175,   175,   175,  4843,  4843,  4843,   175,   175, -1843,
    1515,   517,  1523,  1519,    31, -1843,   125,  1525,    44,    44,
      44,    44,   175,  1521,  1526, -1843, -1843, -1843, -1843,  -221,
   -1843,  -221,  1529,  1530,   175,   175,  1528,  1537,    99, -1843,
    1527,  1545,  1546,  1547,  1548,  1549,  1543,  1544,  1550,  1551,
    1553,  1554,  1848,   442,  1557,  1560,  1849,   450,  1561,  1563,
    1564,  1565,  1566,  1567,   520,  1568,  1570,  1850,   554,  1574,
    1117,  1117,  1117,  1540,  1577,  1117,   891,   891,  1117,  1578,
    1579,  1580,  1581,  1582,  1587, -1843,    87, -1843,  1589, -1843,
   -1843, -1843,   613,   648, -1843,    35,   148, -1843,  1590,  1591,
    1592,  1593,  1541,  4843,  4843,  1597,  1598,   175,   175,  1117,
    1117, -1843, -1843,  -221,  1555,  1854,   125,  1856,  1858,   125,
   -1843, -1843, -1843, -1843, -1843, -1843,  1595, -1843, -1843, -1843,
    1599, -1843, -1843, -1843, -1843, -1843, -1843, -1843, -1843, -1843,
   -1843,  1600, -1843, -1843,  1601,  1606,  1607,  1611,  4843,  1612,
     175,   175,  1613,   175,   175,  4843,  4843,   175, -1843, -1843,
   -1843, -1843,   201,  1614, -1843, -1843, -1843, -1843, -1843,  1615,
    1616,  1618, -1843, -1843,  1602,  1603,  1617,  1619,  1625,   138,
    1627,   561,  1628,  1631,   606,   125,   125,   125, -1843, -1843,
   -1843, -1843,  1620, -1843,   891,   891, -1843,  1626,  1629,  1637,
    1636,  1638, -1843, -1843, -1843,   175,  4843,  1635,  1646, -1843,
   -1843, -1843,  1647,   123,    92, -1843, -1843, -1843, -1843, -1843,
      50,   243,   246,  4843,   175,   175,  1653,   175,  1654,  4843,
    1655,  1662,  1663, -1843, -1843, -1843,    25,  -276, -1843,  1657,
    1658,  1670,  1665,  1117,  1117, -1843,  1667, -1843,  1668, -1843,
     175,   175,  1675,  1676, -1843, -1843, -1843,  1677,  1678,  1683,
    1684,  1687,  1682,  1685, -1843, -1843, -1843, -1843, -1843, -1843,
   -1843,  1689,  1690, -1843, -1843
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
   -1843, -1843, -1843,   629,   794,   176,  -153,  1645,  1241,  1203,
    -825,   477, -1088, -1157,   -32,    -9,  -473, -1842, -1152, -1317,
    -172,  -119, -1843,  -459, -1843,  1091,  -184, -1843, -1843,  1666,
   -1843, -1843, -1843,   654,   617,   951,  1039,  1885,  1889,  1895,
    1930,  1953, -1843,  1767,  1785, -1843,  2075,  2076, -1843, -1843,
    2077, -1843,   203,  2078, -1843, -1843, -1843, -1843, -1843, -1843,
   -1843,  2079, -1843, -1843,  1588,   120, -1843,  2080, -1843, -1843,
   -1843,  2082, -1843, -1843,  2085, -1843, -1843,  2086, -1843, -1843,
   -1843,  2087, -1843, -1843,  -135,   804, -1843, -1843, -1843, -1843,
   -1843, -1843,    52, -1843,  -225,  1838, -1843, -1843, -1843,   304,
   -1843,    20,  1822, -1843, -1843, -1843, -1843,  2095, -1843, -1843,
   -1843, -1843, -1843, -1843, -1843, -1843, -1843, -1843, -1843, -1843,
   -1843, -1843, -1843, -1843, -1843
};

/* YYTABLE[YYPACT[STATE-NUM]].  What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule which
   number is the opposite.  If zero, do what YYDEFACT says.
   If YYTABLE_NINF, syntax error.  */
#define YYTABLE_NINF -699
static const yytype_int16 yytable[] =
{
     190,   742,   211,   364,  1538,   241,   709,   873,  1374,  1926,
     241,   341,    70,  1209,   241,    71,  1382,  1383,   454,   241,
     320,    63,  1210,   212,  1386,   743,   747,   288,   322,   324,
     241,   713,  1929,  1175,   714,   241,  2001,   306,   307,  1366,
     290,   777,    87,   288,  1741,   220,   221,   783,   334,  1401,
     789,   790,   791,  1930,   793,   307,  1211,   796,   797,   798,
    1931,   220,   221,   801,   802,  1212,  1402,   805,   241,   809,
     810,   811,   812,   813,   245,    85,   816,  1195,   818,   700,
     820,   821,   822,   823,   824,  1741,    71,   437,   747,   313,
     455,   241,  1196,  1741,  1183,   100,   484,  1311,   485,   290,
     487,   306,   307,   188,   189,   306,   307,   241,   241,  1409,
     750,  1411,   745,  1413,  1312,  1415,   220,   221,   321,   323,
     305,  1162,   332,   188,   189,   306,   307,   188,   189,  1429,
     307,  1431,   307,  1433,   241,  1435,   151,  1437,   310,  1359,
    1891,  1781,   715,   305,   310,  1188,   188,   189,   306,   307,
    1189,  1895,   305,    82,  1906,   188,   189,   306,   307,    96,
    2052,   473,   716,   717,  1785,  1182,   305,   745,  1190,   188,
     189,   306,   307,  1877,  1401,   188,   189,   306,   307,   188,
     189,   306,   307,   438,   188,   189,   306,   307,   441,   827,
     486,  1402,   830,   831,   832,   833,   834,   835,   836,   837,
    1213,   442,  1929,  1575,   842,  1728,   226,   306,   307,  1214,
     314,  2076,  1729,  1580,   851,  1582,   335,   336,   337,   338,
     339,   858,   859,  1930,   862,   480,   481,   482,   -75,  1197,
    1534,   390,  1182,   188,   189,   306,   307,  1542,   329,  1544,
     718,  1546,   483,  1548,  1198,   342,   343,   344,   345,   346,
     347,   348,   349,   101,   350,   351,   352,   353,   354,   874,
     875,   755,   355,   356,   357,   358,   359,   360,   361,   362,
     463,   464,   138,   465,   103,   480,   481,   482,   710,   711,
      30,    31,   474,  -694,  -694,  -694,   222,  -694,  -694,  -694,
    -694,  -694,  1880,    20,    21,    22,  -694,  -694,  -694,  -694,
    -694,  -694,   222,  1406,  -694,  -694,  -694,  -694,  -694,    20,
      21,    22,    20,    21,    22,    23,  1407,    20,    21,    22,
      23,   365,   366,   367,   368,   369,   370,   371,   372,   373,
     374,   375,   376,   377,   378,   379,   380,   381,   382,   383,
     384,   385,   386,   701,   702,   703,   704,   705,   706,   707,
      20,    21,    22,    23,   786,   786,   543,   222,   196,   786,
     712,   786,   786,  1199,   197,   141,   786,   135,   480,   481,
     482,   786,   786,   876,   807,   154,   155,  1733,  1200,   242,
     807,   807,  1201,   807,   242,   786,  -639,   243,   242,  1747,
     387,  -643,   213,   242,   138,  -651,   244,  1202,   363,   138,
    -647,   243,   993,   138,  1588,   456,   142,   308,   138,   242,
     744,   748,   745,   745,   289,   309,  -640,  -101,   312,  -101,
     319,  2002,   765,  2003,   316,  1742,   480,   481,   482,  1743,
     436,   310,   317,   708,  1181,   340,   143,   391,  2079,   392,
     393,   318,  1591,   394,   395,   396,   397,   398,   399,   400,
     160,   331,   401,   402,   403,   404,   405,   406,   407,   408,
     409,   410,  1363,   411,   412,  1800,  1744,   749,   751,  1999,
    1743,   308,   781,   748,  2078,   308,  1203,   153,  1743,   309,
     161,   242,   242,   309,   458,   460,   310,  1217,  -644,  -648,
     472,  1204,   478,   479,   172,   308,   807,  1219,  1187,   316,
     166,   316,  1218,   309,   310,  1892,   310,   317,   242,   317,
     310,   167,  1220,   188,   189,  -652,  1896,   807,   308,  1907,
     176,   807,  1221,   786,   786,   310,   309,   308,   168,   310,
     135,   807,   807,   877,  1595,   309,   745,  1222,   310,  1223,
     170,   308,   171,  -694,  -694,  -694,  -694,   308,   177,   309,
     178,   308,   310,   185,  1224,   309,   308,   414,   310,   309,
    -694,  -694,  -694,  -694,   309,    30,    31,   310,    30,    31,
     186,  1602,  1603,    30,    31,   135,   201,   308,  1609,  1225,
     195,   196,   234,   235,   878,   309,   206,   197,  -694,  -694,
     756,   279,   280,  1325,  1226,  1327,  1329,   908,  -324,  1331,
     207,   766,   767,   768,   769,   308,    30,    31,  1326,   210,
    1328,  1330,   214,   309,  1332,   877,   771,   772,   413,   480,
     481,   482,   480,   481,   482,  1869,  1606,   838,   807,  1871,
     231,  2080,   843,  1607,  2081,  1608,   215,   723,   725,   727,
     729,   731,   733,   735,   737,   739,   741,   807,   697,   698,
     472,   861,   216,   217,   877,    43,   752,   753,   754,   218,
     275,  -694,   219,   480,   481,   482,   878,   887,   233,   225,
     468,   469,   721,   470,   291,   240,   298,    84,   284,   285,
    1803,  1804,  1805,   785,   785,   807,  1806,  1807,   785,   287,
     785,   785,   480,   481,   482,   785,   296,   300,  1379,   301,
     785,   785,  2054,   806,  1936,   878,   157,   158,   159,   806,
     806,   302,   806,   303,   785,   315,   907,    43,   786,   786,
     326,  1953,  1886,  1887,  1888,  1889,  1890,   443,   480,   481,
     482,  1398,   388,   904,  1739,  1898,  1899,  1900,  1901,  1902,
     415,   416,   417,   418,   419,   420,   444,   421,   422,   423,
     424,   425,   426,   427,   428,   389,   429,   430,   431,   432,
     433,   445,   434,   757,   758,   759,   760,   761,   762,   446,
    1938,  1939,  1940,  1941,   480,   481,   482,  1448,   447,   448,
    1746,   449,   488,  1455,  1456,  1457,  1609,   879,   880,   881,
     882,   883,   490,   466,   467,   468,   469,  1469,   470,   489,
     491,   492,  1474,  1475,  1476,  1477,  1478,  1479,   493,  1481,
    1482,  1483,   461,   462,   463,   464,  1488,   465,   480,   481,
     482,   720,   494,  1495,  1967,   806,   480,   481,   482,  1502,
     495,   496,  1971,  1506,  1507,  1508,   497,   499,  1834,  1513,
      43,   498,  1516,  1517,  1518,   500,   806,   520,   501,   502,
     806,   503,   785,   785,   504,   505,   506,  1165,   507,   524,
     806,   806,   508,  1840,    43,  1173,  1174,  1176,  1841,   879,
     880,   881,   882,   883,   525,  1551,  1552,   509,  1842,  1555,
      43,  1557,  1558,  1559,  1560,  1561,   646,   150,  1564,   510,
    1566,   511,  1568,  1569,  1570,  1571,   480,   481,   482,   884,
     512,   513,  1978,   514,   310,   515,   516,   517,   879,   880,
     881,   882,   883,   466,   467,   468,   469,   518,   470,   519,
     906,   521,   721,   522,   523,    43,   692,    43,   526,   669,
     480,   481,   482,   527,  1599,  1600,  1982,   480,   481,   482,
     173,   174,   528,  2056,   435,   529,   530,   531,   179,   180,
     181,   182,   183,   184,   693,   532,   533,   806,   534,    77,
      78,    79,    80,    81,   690,   535,   202,   203,   204,   205,
     536,  1874,  1875,   208,   537,   538,   806,    88,    89,    90,
      91,   885,   480,   481,   482,   539,   540,   541,  2059,   461,
     462,   463,   464,   542,   465,   227,   228,   229,   230,   647,
    1663,   648,   102,   649,   786,   650,   807,   694,   651,  1672,
     652,   653,  1675,  1676,   806,   654,   655,   656,   657,   658,
     886,  1684,   659,   137,   466,   467,   468,   469,   660,   470,
     807,   807,   465,   661,   662,   807,   311,   663,   664,   665,
     666,   667,  1705,   668,  1707,    75,    76,   785,   785,   825,
    1713,  1714,   670,  1716,  1731,   671,   672,   673,   674,   675,
     676,   677,   678,    86,   679,   680,   681,   682,   903,   330,
    1170,    92,    93,    94,    95,   683,    97,    98,    99,   684,
     807,   685,   686,   472,   807,   687,   786,   786,   688,   689,
     440,  1990,  1991,   699,   719,   807,   807,   470,   800,   869,
     871,   872,   457,   459,   807,   888,   889,   890,   471,   891,
     892,   893,   894,   895,   896,   897,   898,   899,   900,   901,
     902,   909,  1360,  1162,   482,  1365,  1367,  1164,  1166,   910,
     911,   912,   913,   914,   915,   916,   917,   786,   786,   918,
    1152,   920,   921,   922,   786,   923,   786,   786,  1589,   924,
     925,   786,   926,   786,  1372,   927,  1592,   928,   929,   786,
     930,   931,   786,   932,   933,   934,   936,   937,   940,   941,
     938,   942,  1390,   939,   943,   944,   957,   945,  1395,   946,
     947,   948,   949,   786,   786,   950,   807,   951,   952,   953,
     954,   955,   807,   807,   956,   807,   958,   786,   959,   960,
     961,   962,   963,   964,   965,   966,   967,   968,   969,  2064,
    2065,   971,   972,   973,  1459,  1460,   974,   976,   977,   978,
     979,   980,   981,  1468,   982,   983,   984,   985,   986,   987,
     988,   989,   990,   991,   992,  1480,   994,   995,   996,  1484,
     997,   998,   999,   691,  1489,  1000,  1153,  1154,  1155,  1156,
    1157,  1158,  1159,  1160,  1499,   722,   724,   726,   728,   730,
     732,   734,   736,   738,   740,  1161,  1167,  1168,   471,  1169,
    1171,  1172,  1177,  1178,  1179,  1180,  1185,  1182,  1184,  1527,
    1186,  1529,  1192,  1193,  1205,  1357,  1194,  1206,  1536,  1536,
    1207,  1208,  1541,  1233,  1215,  1216,  1227,  1228,  1229,  1230,
    1231,   784,   784,  1232,  1234,  1235,   784,   807,   784,   784,
    1236,  1237,  1238,   784,  1239,  1240,  1241,  1242,   784,   784,
    1243,   330,  1244,  1245,  1246,  1247,  1336,   330,   330,  1248,
     330,   807,   784,   785,  1249,   806,  1873,  1250,  1251,  1252,
    1253,  1254,  1255,  1256,  1257,  1258,   807,  1259,  1260,  1261,
    1262,  1263,  1385,  1264,  1265,  1266,  1267,  1268,  1269,   806,
     806,  1270,  1271,  1272,   806,  1590,  1355,  1273,  1274,  1275,
    1276,  1277,  1278,  1593,  1594,  1910,  1279,  1596,  1913,  1914,
    1915,  1801,  1597,  1280,  1281,  1920,  1921,  1922,  1282,  1604,
    1283,  1284,  1285,  1286,  1287,  1288,  1289,   807,  1290,  1291,
    1292,  1293,  1294,  1295,  1296,  1297,  1298,  1299,  1300,   806,
    1301,  1302,  1303,   806,  1337,   785,   785,  1304,  1305,  1306,
    1307,  1358,  1308,  1309,   806,   806,  1310,  1313,  1315,  1316,
    1317,  1318,  1319,   806,  1314,   313,  1320,  1321,   313,  1322,
    1323,  1324,  1333,   330,  1334,  1335,  1338,  1339,  1340,   807,
    1341,  1342,  1343,  1344,  1345,  1346,  1347,  1348,  1349,  1350,
    1351,  1352,  1353,  1354,   330,  1361,   785,   785,   330,  1362,
     784,   784,  1356,   785,  1369,   785,   785,  1370,   330,   330,
     785,  1373,   785,  1381,  2010,  2011,  1375,  1393,   785,  1376,
    1377,   785,  1378,  1380,  1384,  1535,  1387,  1388,  1389,  1540,
    1394,  1399,  1400,  1417,  1418,   745,  1419,  1420,  1421,  1422,
    1423,  1424,   785,   785,  1425,   806,  1471,  1426,  1427,  1428,
    1439,   806,   806,  1440,   806,  1441,   785,  1442,  1443,  2032,
    1444,  1472,  1503,  1509,  1510,  1522,  2039,  2040,   905,  1523,
    1524,  1385,  1526,  1385,  1530,  1533,  1539,  1550,  1572,  1573,
    1574,  1576,  1577,  1578,  1579,  1581,  1782,  1583,  1584,  1786,
    1585,  1586,  1587,  1601,  1605,  1598,  1778,  1610,  1613,  1622,
    1615,  1779,  1617,  1780,  1612,   330,  1619,  1621,  1614,  1625,
    1616,  1737,  1618,  1620,  1623,  1624,  1626,  2072,  1627,  1628,
    1629,  1633,  1630,  1634,   330,  1631,  1632,  1635,  1637,  1639,
    1799,  1641,  1809,  1636,  2082,  1638,  1643,  1644,  1645,  1640,
    2088,  1642,  1646,  1647,  1648,  1649,  1650,  1651,  1652,  1653,
    1654,  1655,  1656,  1732,  1657,  1658,  1808,  1853,  1659,   473,
    1876,  1894,   330,   473,  1660,  1661,   806,  1662,  1665,  1666,
    1667,   786,  1668,  1669,  1670,  1671,  1903,   313,  1673,  1674,
    1677,  1678,  1679,  1680,  1905,  1681,  1909,  1682,  1810,  1683,
     806,  1685,  1686,  1687,  1689,   784,   784,  1690,  1691,  1692,
    1694,  1695,  1696,  1697,  1698,   806,  1699,  1700,  1701,  1702,
    1704,  1706,   807,   807,  1708,  1709,  1710,   807,  1711,   807,
     786,  1712,  1715,  1717,   786,   786,  1718,  1719,  1720,  1721,
    1722,   471,  1723,   472,  1724,  1385,  1725,  1726,  1727,   807,
    1730,  1856,  1385,  1734,  1735,  1736,  1738,  1385,  1745,  1748,
    1749,   807,  1850,  1750,  1852,   473,   806,  1752,  1754,  1751,
     808,  1855,  1757,  1753,  1755,  1758,   814,   815,  1857,   817,
    1759,  1760,  1761,  1762,  1763,  1764,  1765,  1766,  1767,   788,
     474,  1768,  1769,   792,   474,   794,   795,  1770,  1771,  1772,
     799,  1773,  1774,  1775,   312,   803,   804,   312,  1878,  1776,
    1777,  1790,  1783,  1784,  1787,   104,  1788,  1789,   806,   819,
    1791,  1811,  1792,  1802,  -698,  -698,  -698,  1793,  -698,  -698,
    -698,  -698,  -698,  1794,   807,   807,  1795,  -698,  -698,  -698,
    -698,  -698,  -698,  1798,  1812,  -698,  -698,  -698,  -698,  -698,
    1813,  1814,  1815,  1816,  1817,  1818,  1819,  1821,  1822,   474,
    1823,  1824,  1820,  1825,  1826,  1827,  1828,  1410,  1829,  1412,
    1830,  1414,  1831,  1416,  1832,  1833,  1848,   807,  1849,  1881,
     786,   786,  1851,  1882,   786,  1865,   474,  1430,  1835,  1432,
    1836,  1434,   828,  1436,  1837,  1438,  1838,  1839,  1843,  1844,
    1945,  1845,  1946,  1846,  1847,  1858,  1859,  1854,  1860,  1862,
    1966,  1970,  1981,   848,  1861,  1863,  2020,   852,  2022,  1864,
    2023,  2077,  1611,  2042,  1866,   283,    44,   863,   864,  1867,
      45,  1868,   786,  1870,  1879,  1883,    46,  1925,  1884,  1885,
     473,  1984,  1985,  1986,  1928,  1927,  1989,  1937,  1943,  1992,
    1951,   807,  1954,  1944,   786,  1947,  1948,   854,   855,  1952,
    1955,  1956,  1957,  1958,  1959,  1960,  1961,  2004,  1987,  2009,
    2019,    47,  1962,  1963,  1520,  1964,  1965,   807,   807,  1968,
    2016,  2017,  1969,  1972,  2018,  1973,  1974,  1975,  1976,  1977,
    1979,   784,  1980,   330,    48,  1543,  1983,  1545,   472,  1547,
    1001,  1549,   472,  1988,  1993,  1994,  1995,  1996,  1997,  1998,
     785,  2000,  2005,  2006,  2007,  2008,   312,   330,   330,  2012,
    2013,  2025,   330,  2028,   919,  2026,  2027,  2053,  2029,  2030,
    2047,  2048,  1893,  2031,  2033,  2036,  2043,  2044,  1897,  2049,
     297,  2050,  2045,   935,  2046,  1904,  2063,  2051,  1908,  2055,
    2057,   806,   806,  2058,  2066,  2067,   806,  2073,   806,   785,
     299,   474,  2069,   785,   785,  2068,  2070,   330,  2074,  2075,
    1933,   330,  1385,   784,   784,  2085,  2087,  2089,   806,  2094,
    2095,   970,   330,   330,  -698,  -698,  -698,  -698,  2090,  2091,
     806,   330,  2096,  2097,   472,  2100,  2101,  2104,  2105,  2106,
    2107,  -698,  -698,  -698,  -698,  2108,  2109,  2092,  2093,  2110,
    2111,  2113,  2114,  2112,  2098,  2099,    49,    50,    52,    53,
      54,    55,   764,    56,   784,   784,    57,    58,    59,  -698,
    -698,   784,   325,   784,   784,   328,    65,     0,   784,     0,
     784,     0,     0,     0,     0,     0,   784,     0,     0,   784,
       0,     0,  1150,  1151,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   806,   806,     0,     0,     0,     0,     0,
     784,   784,  2021,   330,     0,  2024,     0,     0,     0,   330,
     330,     0,   330,     0,   784,     0,   778,   779,   780,     0,
     782,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,  -698,     0,     0,     0,   806,     0,     0,   785,
     785,     0,     0,   785,     0,     0,     0,     0,  1933,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,  2060,  2061,  2062,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   785,     0,     0,     0,     0,     0,     0,     0,  1385,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     806,     0,     0,   785,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   330,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   806,   806,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   330,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   826,   330,     0,   829,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   839,   840,   841,     0,     0,
     844,   845,   846,   847,     0,     0,   849,   850,     0,     0,
     853,   471,     0,   856,   857,     0,     0,   860,     0,     0,
       0,   865,   866,   867,   868,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   330,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,  1368,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   311,     0,     0,   311,  1391,  1392,     0,     0,
     105,  1396,     0,     0,     0,     0,   330,     0,  1364,  -697,
    -697,  -697,     0,  -697,  -697,  -697,  -697,  -697,     0,     0,
       0,     0,  -697,  -697,  -697,  -697,  -697,  -697,     0,     0,
    -697,  -697,  -697,  -697,  -697,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,  1445,     0,     0,     0,
    1449,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,  1461,  1462,     0,     0,     0,     0,     0,     0,     0,
    1470,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
    1451,  1452,     0,     0,     0,   106,     0,     0,   975,     0,
       0,     0,     0,     0,  -695,  -695,  -695,     0,  -695,  -695,
    -695,  -695,  -695,     0,     0,     0,     0,  -695,  -695,  -695,
    -695,  -695,  -695,     0,     0,  -695,  -695,  -695,  -695,  -695,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,  1504,  1505,     0,     0,     0,     0,     0,  1511,     0,
    1514,  1515,  1556,     0,     0,  1519,     0,  1521,  1562,  1563,
       0,  1565,     0,  1528,     0,     0,  1531,     0,     0,     0,
     108,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   471,  1553,  1554,     0,
     471,     0,     0,     0,     0,     0,     0,     0,   784,     0,
       0,  1567,     0,     0,   311,   109,   110,   111,   112,   113,
     114,   115,   116,   117,   118,   119,   120,   121,   122,   123,
     124,   125,   126,   127,   128,   129,   130,   131,   132,   133,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   330,
     330,     0,     0,     0,   330,     0,   330,   784,     0,     0,
       0,   784,   784,     0,     0,     0,     0,     0,  1932,     0,
       0,     0,     0,     0,     0,     0,   330,     0,     0,  -697,
    -697,  -697,  -697,  1664,     0,     0,     0,     0,   330,     0,
       0,     0,   471,     0,     0,     0,  -697,  -697,  -697,  -697,
       0,     0,     0,     0,   107,     0,     0,  1688,     0,     0,
       0,     0,     0,  -696,  -696,  -696,     0,  -696,  -696,  -696,
    -696,  -696,  1703,     0,  -697,  -697,  -696,  -696,  -696,  -696,
    -696,  -696,     0,     0,  -696,  -696,  -696,  -696,  -696,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   330,   330,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,  1756,  -695,  -695,  -695,  -695,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,  -695,  -695,  -695,  -695,     0,     0,  -697,     0,     0,
       0,     0,     0,     0,   330,     0,     0,   784,   784,     0,
       0,   784,     0,     0,     0,     0,  1932,     0,     0,  -695,
    -695,     0,     0,     0,     0,  1796,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,  1397,     0,     0,     0,     0,   784,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   330,     0,
       0,   784,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,  -695,     0,     0,     0,     0,     0,     0,  1446,
    1447,     0,     0,  1450,   330,   330,  1453,  1454,     0,     0,
    1458,     0,     0,     0,     0,  1463,  1464,  1465,  1466,  1467,
       0,     0,     0,     0,     0,  1473,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,  1485,  1486,  1487,
       0,     0,  1490,  1491,  1492,  1493,  1494,     0,  1496,  1497,
    1498,     0,  1500,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,  -696,  -696,  -696,  -696,     0,     0,     0,
       0,  1532,     0,     0,     0,     0,     0,     0,     0,     0,
    -696,  -696,  -696,  -696,     0,     0,     0,     0,     0,     5,
       6,     7,     0,     8,     9,    10,    11,    12,     0,     0,
       0,     0,    14,    15,    16,    17,    18,    19,  -696,  -696,
      20,    21,    22,    23,    24,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,  1872,     0,     0,  1911,  1912,
       0,     0,     0,  1916,     0,  1918,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,  1942,     0,     0,     0,     0,
       0,  -696,     0,     0,     0,     0,     0,  1949,     0,     0,
       0,     0,     0,     0,  1919,     0,     0,     0,  1923,  1924,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
    1002,     0,     0,     0,  1693,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
    2014,  2015,  1003,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,  1004,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,  2034,  1005,  1006,  1007,  1008,  1009,  1010,
    1011,  1012,  1013,  1014,  1015,  1016,  1017,  1018,     0,     0,
       0,     0,     0,     0,  1019,  1020,  1021,  1022,  1023,     0,
       0,     0,  1024,     0,  1025,  1026,  1027,  1028,  1029,     0,
       0,     0,     0,     0,  2037,  2038,     0,     0,  2041,    26,
      27,    28,    29,     0,     0,     0,     0,     0,  1797,     0,
       0,     0,     0,     0,     0,     0,    30,    31,    32,    33,
       0,     0,     0,     0,     0,     0,     0,  2083,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    38,    39,  2071,     0,     0,     0,
       0,     0,     0,  2102,  2103,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,  2086,     0,
       0,     0,     0,  1030,  1031,  1032,  1033,  1034,  1035,     0,
    1036,  1037,  1038,  1039,  1040,  1041,  1042,  1043,  1044,  1045,
    1046,  1047,  1048,  1049,     0,  1050,  1051,  1052,  1053,  1054,
    1055,  1056,  1057,  1058,  1059,  1060,  1061,  1062,  1063,  1064,
    1065,  1066,  1067,  1068,  1069,  1070,  1071,  1072,  1073,  1074,
    1075,     0,  1076,  1077,  1078,  1079,  1080,  1081,  1082,  1083,
    1084,  1085,  1086,  1087,  1088,  1089,  1090,  1091,  1092,  1093,
       0,  1094,  1095,  1096,  1097,  1098,  1099,  1100,  1101,  1102,
    1103,  1104,     0,  1105,  1106,     0,  1107,  1108,  1109,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,  1110,  1111,  1112,     0,     0,     0,  1113,     0,
    1114,  1115,  1116,  1117,  1118,  1119,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,  1120,  1121,  1122,     0,     0,  1123,  1124,  1125,  1126,
    1127,  1128,  1129,  1130,  1131,  1132,  1133,  1134,  1135,  1136,
    1137,  1138,  1139,  1140,  1141,  1142,  1143,  1144,  1145,  1146,
       0,     0,     0,     0,     0,     0,     0,     0,     0,  1002,
       0,     0,     0,     0,     0,     0,  1917,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,  1003,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,  1004,     0,     0,     0,     0,  1147,     0,     0,
    1950,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,  1005,  1006,  1007,  1008,  1009,  1010,  1011,
    1012,  1013,  1014,  1015,  1016,  1017,  1018,     0,     0,     0,
       0,     0,     0,  1019,  1020,  1021,  1022,  1023,     0,     2,
       3,  1024,     4,  1025,  1026,  1027,  1028,  1029,     0,     5,
       6,     7,     0,     8,     9,    10,    11,    12,    13,     0,
       0,     0,    14,    15,    16,    17,    18,    19,     0,     0,
      20,    21,    22,    23,    24,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      25,     0,     0,     0,     0,     0,  2035,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,  1030,  1031,  1032,  1033,  1034,  1035,     0,  1036,
    1037,  1038,  1039,  1040,  1041,  1042,  1043,  1044,  1045,  1046,
    1047,  1048,  1049,     0,  1050,  1051,  1052,  1053,  1054,  1055,
    1056,  1057,  1058,  1059,  1060,  1061,  1062,  1063,  1064,  1065,
    1066,  1067,  1068,  1069,  1070,  1071,  1072,  1073,  1074,  1075,
    2084,  1076,  1077,  1078,  1079,  1080,  1081,  1082,  1083,  1084,
    1085,  1086,  1087,  1088,  1089,  1090,  1091,  1092,  1093,     0,
    1094,  1095,  1096,  1097,  1098,  1099,  1100,  1101,  1102,  1103,
    1104,     0,  1105,  1106,     0,  1107,  1108,  1109,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,  1110,  1111,  1112,     0,     0,     0,  1113,     0,  1114,
    1115,  1116,  1117,  1118,  1119,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
    1120,  1121,  1122,     0,     0,  1123,  1124,  1125,  1126,  1127,
    1128,  1129,  1130,  1131,  1132,  1133,  1134,  1135,  1136,  1137,
    1138,  1139,  1140,  1141,  1142,  1143,  1144,  1145,  1146,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    26,
      27,    28,    29,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    30,    31,    32,    33,
       0,     0,     0,     0,    34,     0,  1191,     0,     0,     0,
       0,     0,     0,     0,    35,     0,     0,    36,    37,   -14,
     103,     0,   -14,     0,    38,    39,     0,     0,     0,   -14,
     -14,   -14,     0,   -14,   -14,   -14,   -14,   -14,   -14,     0,
       0,     0,   -14,   -14,   -14,   -14,   -14,   -14,     0,     0,
     -14,   -14,   -14,   -14,   -14,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   -18,   104,     0,   -18,     0,     0,     0,     0,     0,
     -14,   -18,   -18,   -18,     0,   -18,   -18,   -18,   -18,   -18,
     -18,    40,     0,     0,   -18,   -18,   -18,   -18,   -18,   -18,
       0,     0,   -18,   -18,   -18,   -18,   -18,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   -17,
     105,     0,   -17,     0,     0,     0,     0,     0,     0,   -17,
     -17,   -17,   -18,   -17,   -17,   -17,   -17,   -17,   -17,     0,
       0,     0,   -17,   -17,   -17,   -17,   -17,   -17,     0,     0,
     -17,   -17,   -17,   -17,   -17,     0,     0,     0,     0,   -15,
     106,     0,   -15,     0,     0,     0,     0,     0,     0,   -15,
     -15,   -15,     0,   -15,   -15,   -15,   -15,   -15,   -15,     0,
     -17,     0,   -15,   -15,   -15,   -15,   -15,   -15,     0,     0,
     -15,   -15,   -15,   -15,   -15,     0,     0,     0,     0,   -16,
     107,     0,   -16,     0,     0,     0,     0,     0,     0,   -16,
     -16,   -16,     0,   -16,   -16,   -16,   -16,   -16,   -16,     0,
     -15,     0,   -16,   -16,   -16,   -16,   -16,   -16,     0,     0,
     -16,   -16,   -16,   -16,   -16,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     -16,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   -14,
     -14,   -14,   -14,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   -14,   -14,   -14,   -14,
       0,     0,     0,     0,   -14,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   -14,     0,     0,   -14,   -14,     0,
       0,     0,     0,     0,   -14,   -14,     0,     0,     0,     0,
       0,   -18,   -18,   -18,   -18,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   -18,   -18,
     -18,   -18,     0,     0,     0,     0,   -18,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   -18,     0,     0,   -18,
     -18,     0,     0,     0,     0,     0,   -18,   -18,     0,   -17,
     -17,   -17,   -17,     0,     0,     0,     0,     0,     0,     0,
       0,   -14,     0,     0,     0,     0,   -17,   -17,   -17,   -17,
       0,     0,     0,     0,   -17,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   -17,     0,     0,   -17,   -17,   -15,
     -15,   -15,   -15,     0,   -17,   -17,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   -15,   -15,   -15,   -15,
       0,     0,     0,   -18,   -15,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   -15,     0,     0,   -15,   -15,   -16,
     -16,   -16,   -16,     0,   -15,   -15,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   -16,   -16,   -16,   -16,
     188,   189,   306,   307,   -16,     0,     0,     0,     0,     0,
       0,   -17,     0,     0,   -16,     0,     0,   -16,   -16,     0,
       0,     0,     0,     0,   -16,   -16,     0,     0,     0,     0,
       0,     5,     6,     7,     0,     8,     9,    10,    11,    12,
       0,     0,     0,     0,    14,    15,    16,    17,    18,    19,
       0,   -15,    20,    21,    22,    23,    24,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   -16,     0,     0,     0,     0,   544,   545,   546,   547,
     548,   549,   550,   551,   552,   553,   554,   555,   556,   557,
     558,   559,   560,   561,   562,   563,   564,   565,   566,   567,
     568,   569,   570,   571,   572,   573,   574,   575,   576,   577,
     578,   579,   580,   581,   582,   583,   584,   585,   586,   587,
     588,   589,   590,   591,   592,   593,   594,   595,   596,   597,
     598,   599,   600,   601,   602,   603,   604,   605,   606,   607,
     608,   609,   610,   611,   612,     0,     0,   544,   545,   546,
     547,   548,   549,   550,   551,   552,   553,   554,   555,   556,
     557,   558,   559,   560,   561,   562,   563,   564,   565,   566,
     567,   568,   569,   570,   571,   572,   573,   574,   575,   576,
     577,   578,   579,   580,   581,   582,   583,   584,   585,   586,
     587,   588,   589,   590,   591,   592,   593,   594,   595,   596,
     597,   598,   599,   600,   601,   602,   603,   604,   605,   606,
     607,   608,   609,   610,   611,   612,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    26,    27,    28,    29,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    30,    31,
      32,    33,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    38,    39,     0,   613,
     614,   615,   616,   617,   618,   619,   620,   621,   622,   623,
     624,   625,   626,   627,   628,   629,   630,   631,   632,   633,
     634,   635,   636,   637,   638,   639,   640,   641,   642,   643,
     644,     0,   308,     0,     0,     0,     0,     0,     0,     0,
     309,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   327,
     613,   614,   615,   616,   617,   618,   619,   620,   621,   622,
     623,   624,   625,   626,   627,   628,   629,   630,   631,   632,
     633,   634,   635,   636,   637,   638,   639,   640,   641,   642,
     643,   644,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   310,   544,   545,   546,   547,   548,
     549,   550,   551,   552,   553,   554,   555,   556,   557,   558,
     559,   560,   561,   562,   563,   564,   565,   566,   567,   568,
     569,   570,   571,   572,   573,   574,   575,   576,   577,   578,
     579,   580,   581,   582,   583,   584,   585,   586,   587,   588,
     589,   590,   591,   592,   593,   594,   595,   596,   597,   598,
     599,   600,   601,   602,   603,   604,   605,   606,   607,   608,
     609,   610,   611,   612,   544,   545,   546,   547,   548,   549,
     550,   551,   552,   553,   554,   555,   556,   557,   558,   559,
     560,   561,   562,   563,   564,   565,   566,   567,   568,   569,
     570,   571,   572,   573,   574,   575,   576,   577,   578,   579,
     580,   581,   582,   583,   584,   585,   586,   587,   588,   589,
     590,   591,   592,   593,   594,   595,   596,   597,   598,   599,
     600,   601,   602,   603,   604,   605,   606,   607,   608,   609,
     610,   611,   612,   544,   545,   546,   547,   548,   549,   550,
     551,   552,   553,   554,   555,   556,   557,   558,   559,   560,
     561,   562,   563,   564,   565,   566,   567,   568,   569,   570,
     571,   572,   573,   574,   575,   576,   577,   578,   579,   580,
     581,   582,   583,   584,   585,   586,   587,   588,   589,   590,
     591,   592,   593,   594,   595,   596,   597,   598,   599,   600,
     601,   602,   603,   604,   605,   606,   607,   608,   609,   610,
     611,   612,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   613,   614,
     615,   616,   617,   618,   619,   620,   621,   622,   623,   624,
     625,   626,   627,   628,   629,   630,   631,   632,   633,   634,
     635,   636,   637,   638,   639,   640,   641,   642,   643,   644,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,  1501,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   613,   614,   615,
     616,   617,   618,   619,   620,   621,   622,   623,   624,   625,
     626,   627,   628,   629,   630,   631,   632,   633,   634,   635,
     636,   637,   638,   639,   640,   641,   642,   643,   644,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,  1512,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   613,   614,   615,   616,
     617,   618,   619,   620,   621,   622,   623,   624,   625,   626,
     627,   628,   629,   630,   631,   632,   633,   634,   635,   636,
     637,   638,   639,   640,   641,   642,   643,   644
};

static const yytype_int16 yycheck[] =
{
     153,   474,     1,     1,  1321,     1,    34,    22,  1165,  1851,
       1,     1,    22,   375,     1,    22,  1173,  1174,     1,     1,
     245,     1,   384,    22,  1176,     1,     1,     1,   253,   254,
       1,   277,     1,   282,   280,     1,     1,     6,     7,   282,
     224,   500,    62,     1,     1,    19,    20,   506,     1,     5,
     509,   510,   511,    22,   513,     7,   375,   516,   517,   518,
      29,    19,    20,   522,   523,   384,    22,   526,     1,   528,
     529,   530,   531,   532,   209,    23,   535,   375,   537,    34,
     539,   540,   541,   542,   543,     1,    22,   286,     1,   242,
      73,     1,   390,     1,   919,    43,   321,   375,   323,   283,
     325,     6,     7,     4,     5,     6,     7,     1,     1,  1197,
     386,  1199,   388,  1201,   392,  1203,    19,    20,   253,   254,
       1,    22,   275,     4,     5,     6,     7,     4,     5,  1217,
       7,  1219,     7,  1221,     1,  1223,    84,  1225,   387,    22,
      22,    22,   388,     1,   387,   970,     4,     5,     6,     7,
     975,    22,     1,    22,    22,     4,     5,     6,     7,    22,
      22,   314,   310,   311,    22,   386,     1,   388,   993,     4,
       5,     6,     7,    22,     5,     4,     5,     6,     7,     4,
       5,     6,     7,   382,     4,     5,     6,     7,     9,   648,
     325,    22,   651,   652,   653,   654,   655,   656,   657,   658,
     375,    22,     1,  1360,   663,   382,   186,     6,     7,   384,
     242,  2053,   389,  1365,   673,  1367,   169,   170,   171,   172,
     173,   680,   681,    22,   683,   376,   377,   378,   382,   375,
    1318,     1,   386,     4,     5,     6,     7,  1325,     9,  1327,
     388,  1329,   393,  1331,   390,   235,   236,   237,   238,   239,
     240,   241,   242,    22,   244,   245,   246,   247,   248,   274,
     275,   486,   252,   253,   254,   255,   256,   257,   258,   259,
     378,   379,   390,   381,     1,   376,   377,   378,   306,   307,
     287,   288,   314,    10,    11,    12,   260,    14,    15,    16,
      17,    18,   393,    31,    32,    33,    23,    24,    25,    26,
      27,    28,   260,     9,    31,    32,    33,    34,    35,    31,
      32,    33,    31,    32,    33,    34,    22,    31,    32,    33,
      34,   319,   320,   321,   322,   323,   324,   325,   326,   327,
     328,   329,   330,   331,   332,   333,   334,   335,   336,   337,
     338,   339,   340,   298,   299,   300,   301,   302,   303,   304,
      31,    32,    33,    34,   507,   508,   388,   260,    30,   512,
     388,   514,   515,   375,    36,   387,   519,    63,   376,   377,
     378,   524,   525,   388,   527,   287,   288,  1529,   390,   375,
     533,   534,   375,   536,   375,   538,   382,   383,   375,  1541,
     388,   382,   391,   375,   390,   382,   392,   390,   388,   390,
     382,   383,   861,   390,   375,   388,   387,   376,   390,   375,
     386,   386,   388,   388,   388,   384,   382,   386,   242,   388,
     244,   386,     1,   388,   376,   382,   376,   377,   378,   386,
     388,   387,   384,   388,   907,   388,   387,   207,   388,   209,
     210,   393,   375,   213,   214,   215,   216,   217,   218,   219,
     384,   275,   222,   223,   224,   225,   226,   227,   228,   229,
     230,   231,   282,   233,   234,   375,   382,   476,   477,   382,
     386,   376,   504,   386,   382,   376,   375,   383,   386,   384,
     387,   375,   375,   384,   308,   309,   387,   375,   382,   382,
     314,   390,   316,   317,    22,   376,   649,   375,   957,   376,
     387,   376,   390,   384,   387,   387,   387,   384,   375,   384,
     387,   387,   390,     4,     5,   382,   387,   670,   376,   387,
      21,   674,   375,   676,   677,   387,   384,   376,   382,   387,
     226,   684,   685,    22,   386,   384,   388,   390,   387,   375,
     382,   376,   382,   270,   271,   272,   273,   376,    21,   384,
      21,   376,   387,   387,   390,   384,   376,     1,   387,   384,
     287,   288,   289,   290,   384,   287,   288,   387,   287,   288,
     387,  1396,  1397,   287,   288,   271,    21,   376,  1403,   375,
      29,    30,   385,   386,    73,   384,    21,    36,   315,   316,
       1,   220,   221,   375,   390,   375,   375,   750,    47,   375,
      21,   180,   181,   182,   183,   376,   287,   288,   390,   387,
     390,   390,   387,   384,   390,    22,   496,   497,   388,   376,
     377,   378,   376,   377,   378,  1782,   385,   659,   781,  1786,
     378,   388,   664,   392,   388,   394,   387,   461,   462,   463,
     464,   465,   466,   467,   468,   469,   470,   800,   445,   446,
     474,   683,   387,   387,    22,     1,   480,   481,   482,   387,
     375,   388,   387,   376,   377,   378,    73,   699,    47,   388,
     378,   379,   385,   381,   388,   387,   387,    23,    22,    22,
      71,    72,    73,   507,   508,   838,    77,    78,   512,   384,
     514,   515,   376,   377,   378,   519,    22,   387,   382,   387,
     524,   525,  2019,   527,  1856,    73,    89,    90,    91,   533,
     534,   387,   536,   387,   538,    22,   748,    63,   871,   872,
     382,  1878,  1810,  1811,  1812,  1813,  1814,   387,   376,   377,
     378,  1190,   375,   742,   382,  1823,  1824,  1825,  1826,  1827,
     184,   185,   186,   187,   188,   189,   387,   191,   192,   193,
     194,   195,   196,   197,   198,   375,   200,   201,   202,   203,
     204,   387,   206,   174,   175,   176,   177,   178,   179,   387,
    1858,  1859,  1860,  1861,   376,   377,   378,  1236,    22,    21,
     382,   297,   382,  1242,  1243,  1244,  1611,   276,   277,   278,
     279,   280,   382,   376,   377,   378,   379,  1256,   381,   382,
     382,   382,  1261,  1262,  1263,  1264,  1265,  1266,   375,  1268,
    1269,  1270,   376,   377,   378,   379,  1275,   381,   376,   377,
     378,   385,   375,  1282,   382,   649,   376,   377,   378,  1288,
     375,   375,   382,  1292,  1293,  1294,   375,   375,  1663,  1298,
     186,   382,  1301,  1302,  1303,   375,   670,   382,   375,   375,
     674,   375,   676,   677,   375,   375,   375,   889,   375,   392,
     684,   685,   375,  1688,   210,   897,   898,   899,  1693,   276,
     277,   278,   279,   280,   392,  1334,  1335,   375,  1703,  1338,
     226,  1340,  1341,  1342,  1343,  1344,   382,    83,  1347,   375,
    1349,   375,  1351,  1352,  1353,  1354,   376,   377,   378,   388,
     375,   375,   382,   375,   387,   375,   375,   375,   276,   277,
     278,   279,   280,   376,   377,   378,   379,   375,   381,   375,
     744,   375,   385,   375,   375,   271,   387,   273,   375,   382,
     376,   377,   378,   375,  1393,  1394,   382,   376,   377,   378,
     136,   137,   375,   382,   388,   375,   375,   375,   144,   145,
     146,   147,   148,   149,   385,   375,   375,   781,   375,     8,
       9,    10,    11,    12,   382,   375,   162,   163,   164,   165,
     375,  1796,  1797,   169,   375,   375,   800,    26,    27,    28,
      29,   388,   376,   377,   378,   375,   375,   375,   382,   376,
     377,   378,   379,   375,   381,   191,   192,   193,   194,   375,
    1459,   375,    51,   375,  1157,   375,  1159,   385,   375,  1468,
     375,   375,  1471,  1472,   838,   375,   375,   375,   375,   375,
     388,  1480,   375,    72,   376,   377,   378,   379,   375,   381,
    1183,  1184,   381,   375,   375,  1188,   242,   375,   375,   375,
     375,   375,  1501,   375,  1503,     6,     7,   871,   872,   382,
    1509,  1510,   375,  1512,  1527,   375,   375,   375,   375,   375,
     375,   375,   375,    24,   375,   375,   375,   375,    22,   275,
     894,    32,    33,    34,    35,   375,    37,    38,    39,   375,
    1233,   375,   375,   907,  1237,   375,  1239,  1240,   375,   375,
     286,  1916,  1917,   375,   375,  1248,  1249,   381,   387,   387,
     375,   375,   308,   309,  1257,   384,   375,   375,   314,   375,
     375,   375,   375,   375,   384,   375,   375,   375,   375,   375,
     375,   382,  1154,    22,   378,  1157,  1158,    22,    22,   382,
     382,   382,   382,   382,   382,   382,   382,  1290,  1291,   382,
     375,   382,   382,   382,  1297,   382,  1299,  1300,  1373,   382,
     382,  1304,   382,  1306,  1163,   382,  1381,   382,   382,  1312,
     382,   382,  1315,   382,   382,   382,   382,   382,   382,   382,
     393,   382,  1181,   393,   382,   382,   386,   382,  1187,   382,
     382,   382,   382,  1336,  1337,   382,  1339,   382,   382,   382,
     382,   382,  1345,  1346,   382,  1348,   382,  1350,   382,   382,
     382,   382,   382,   382,   382,   382,   382,   382,   382,  2034,
    2035,   382,   382,   382,  1246,  1247,   382,   382,   382,   382,
     382,   382,   382,  1255,   382,   382,   382,   382,   382,   382,
     382,   382,   382,   382,   382,  1267,   382,   382,   382,  1271,
     382,   382,   382,   439,  1276,   382,   375,   375,   375,   375,
     375,   375,   375,   375,  1286,   461,   462,   463,   464,   465,
     466,   467,   468,   469,   470,   375,    22,    22,   474,    22,
      22,    22,   281,     9,     9,   382,   375,   386,   386,  1311,
     375,  1313,   382,   384,   375,     9,   384,   384,  1320,  1321,
     384,   384,  1324,   375,   384,   384,   384,   384,   384,   384,
     384,   507,   508,   384,   375,   375,   512,  1460,   514,   515,
     375,   375,   375,   519,   375,   375,   375,   375,   524,   525,
     375,   527,   375,   375,   375,   375,   392,   533,   534,   375,
     536,  1484,   538,  1157,   375,  1159,  1795,   375,   375,   375,
     375,   375,   375,   375,   375,   375,  1499,   375,   375,   375,
     375,   375,  1176,   375,   375,   375,   375,   375,   375,  1183,
    1184,   375,   375,   375,  1188,  1374,   382,   375,   375,   375,
     375,   375,   375,  1382,  1383,  1834,   375,  1386,  1837,  1838,
    1839,  1606,  1391,   375,   375,  1844,  1845,  1846,   375,  1398,
     375,   375,   375,   375,   375,   375,   375,  1550,   375,   375,
     375,   375,   375,   375,   375,   375,   375,   375,   375,  1233,
     375,   375,   375,  1237,   392,  1239,  1240,   375,   384,   375,
     375,    22,   375,   375,  1248,  1249,   375,   375,   375,   375,
     375,   375,   375,  1257,   384,  1588,   375,   375,  1591,   375,
     375,   375,   375,   649,   375,   375,   375,   375,   375,  1602,
     375,   375,   375,   375,   375,   375,   375,   375,   375,   375,
     375,   375,   375,   375,   670,     9,  1290,  1291,   674,    22,
     676,   677,   382,  1297,    22,  1299,  1300,   281,   684,   685,
    1304,   385,  1306,   385,  1943,  1944,   382,   387,  1312,   382,
     382,  1315,   382,   382,   382,  1319,   382,   382,   382,  1323,
     387,    22,    22,     9,    22,   388,    22,    22,    22,    22,
      22,    22,  1336,  1337,    22,  1339,   387,    22,    22,    22,
      22,  1345,  1346,    22,  1348,    22,  1350,    22,    22,  1988,
      22,   387,   387,   387,   387,   263,  1995,  1996,   744,    22,
     266,  1365,    22,  1367,    22,   387,    22,   387,   382,   382,
     382,   382,   382,   382,   382,   382,  1588,   382,   382,  1591,
     382,    22,   382,   382,   385,   388,  1575,   382,   382,   385,
     382,  1580,   382,  1582,   391,   781,   382,   382,   391,   382,
     391,    22,   391,   391,   385,   385,   385,  2046,   382,   385,
     382,   382,   385,   391,   800,   385,   385,   382,   382,   382,
     375,   382,  1611,   391,  2063,   391,   385,   385,   385,   391,
    2069,   391,   385,   385,   385,   382,   382,   382,   382,   382,
     382,   382,   382,   393,   382,   382,    22,    22,   382,  1782,
      22,    22,   838,  1786,   382,   382,  1460,   382,   382,   382,
     382,  1794,   382,   382,   382,   382,    22,  1800,   382,   382,
     382,   382,   382,   382,    22,   382,    22,   382,   375,   382,
    1484,   382,   382,   382,   382,   871,   872,   382,   382,   382,
     382,   382,   382,   382,   382,  1499,   382,   382,   382,   382,
     382,   382,  1835,  1836,   382,   382,   382,  1840,   382,  1842,
    1843,   382,   382,   382,  1847,  1848,   382,   382,   382,   382,
     382,   907,   382,  1527,   385,  1529,   382,   382,   382,  1862,
     382,  1743,  1536,   386,   382,   382,   382,  1541,   382,   382,
     391,  1874,  1731,   382,  1733,  1878,  1550,   382,   382,   391,
     527,  1740,   382,   391,   391,   382,   533,   534,  1747,   536,
     393,   393,   382,   382,   382,   382,   382,   382,   382,   508,
    1782,   382,   382,   512,  1786,   514,   515,   382,   382,   382,
     519,   382,   382,   382,  1588,   524,   525,  1591,  1800,   382,
     382,  1595,   382,   382,   382,     1,   382,   382,  1602,   538,
     382,   375,   382,  1607,    10,    11,    12,   382,    14,    15,
      16,    17,    18,   386,  1947,  1948,   386,    23,    24,    25,
      26,    27,    28,   382,   375,    31,    32,    33,    34,    35,
     375,   375,   375,   375,   375,   382,   382,   375,   375,  1851,
     375,   375,   382,   375,   375,   375,   375,  1198,   375,  1200,
     375,  1202,   375,  1204,   375,   375,   375,  1990,   266,   385,
    1993,  1994,   375,   385,  1997,   382,  1878,  1218,   386,  1220,
     386,  1222,   649,  1224,   386,  1226,   386,   386,   386,   386,
    1869,   386,  1871,   386,   386,   375,   375,   386,   375,   386,
      22,    22,    22,   670,   375,   375,    22,   674,    22,   375,
      22,  2053,  1405,  2002,   382,   219,     1,   684,   685,   382,
       1,   382,  2045,   382,   382,   385,     1,   382,   385,   385,
    2053,  1910,  1911,  1912,   385,   382,  1915,   382,   387,  1918,
     382,  2064,   385,   387,  2067,   386,   386,   676,   677,   382,
     375,   375,   375,   375,   375,   382,   382,  1936,   388,   388,
     375,     1,   382,   382,  1305,   382,   382,  2090,  2091,   382,
    1949,  1950,   382,   382,  1953,   382,   382,   382,   382,   382,
     382,  1157,   382,  1159,     1,  1326,   382,  1328,  1782,  1330,
     869,  1332,  1786,   386,   386,   386,   386,   386,   386,   382,
    1794,   382,   382,   382,   382,   382,  1800,  1183,  1184,   382,
     382,   386,  1188,   382,   781,   386,   386,  2019,   382,   382,
     388,   388,  1816,   382,   382,   382,   382,   382,  1822,   382,
     233,   382,   386,   800,   386,  1829,   386,   382,  1832,   382,
     382,  1835,  1836,   382,   388,   386,  1840,   382,  1842,  1843,
     235,  2053,   386,  1847,  1848,   388,   388,  1233,   382,   382,
    1854,  1237,  1856,  1239,  1240,   382,   382,   382,  1862,   382,
     382,   838,  1248,  1249,   270,   271,   272,   273,   386,   386,
    1874,  1257,   382,   388,  1878,   388,   388,   382,   382,   382,
     382,   287,   288,   289,   290,   382,   382,  2076,  2077,   382,
     388,   382,   382,   388,  2083,  2084,     1,     1,     1,     1,
       1,     1,   494,     1,  1290,  1291,     1,     1,     1,   315,
     316,  1297,   254,  1299,  1300,   273,     1,    -1,  1304,    -1,
    1306,    -1,    -1,    -1,    -1,    -1,  1312,    -1,    -1,  1315,
      -1,    -1,   871,   872,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,  1947,  1948,    -1,    -1,    -1,    -1,    -1,
    1336,  1337,  1956,  1339,    -1,  1959,    -1,    -1,    -1,  1345,
    1346,    -1,  1348,    -1,  1350,    -1,   501,   502,   503,    -1,
     505,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   388,    -1,    -1,    -1,  1990,    -1,    -1,  1993,
    1994,    -1,    -1,  1997,    -1,    -1,    -1,    -1,  2002,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,  2025,  2026,  2027,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,  2045,    -1,    -1,    -1,    -1,    -1,    -1,    -1,  2053,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
    2064,    -1,    -1,  2067,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,  1460,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,  2090,  2091,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,  1484,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   647,  1499,    -1,   650,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   660,   661,   662,    -1,    -1,
     665,   666,   667,   668,    -1,    -1,   671,   672,    -1,    -1,
     675,  1527,    -1,   678,   679,    -1,    -1,   682,    -1,    -1,
      -1,   686,   687,   688,   689,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,  1550,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,  1159,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,  1588,    -1,    -1,  1591,  1183,  1184,    -1,    -1,
       1,  1188,    -1,    -1,    -1,    -1,  1602,    -1,  1157,    10,
      11,    12,    -1,    14,    15,    16,    17,    18,    -1,    -1,
      -1,    -1,    23,    24,    25,    26,    27,    28,    -1,    -1,
      31,    32,    33,    34,    35,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,  1233,    -1,    -1,    -1,
    1237,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,  1248,  1249,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
    1257,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
    1239,  1240,    -1,    -1,    -1,     1,    -1,    -1,   843,    -1,
      -1,    -1,    -1,    -1,    10,    11,    12,    -1,    14,    15,
      16,    17,    18,    -1,    -1,    -1,    -1,    23,    24,    25,
      26,    27,    28,    -1,    -1,    31,    32,    33,    34,    35,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,  1290,  1291,    -1,    -1,    -1,    -1,    -1,  1297,    -1,
    1299,  1300,  1339,    -1,    -1,  1304,    -1,  1306,  1345,  1346,
      -1,  1348,    -1,  1312,    -1,    -1,  1315,    -1,    -1,    -1,
       1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,  1782,  1336,  1337,    -1,
    1786,    -1,    -1,    -1,    -1,    -1,    -1,    -1,  1794,    -1,
      -1,  1350,    -1,    -1,  1800,    36,    37,    38,    39,    40,
      41,    42,    43,    44,    45,    46,    47,    48,    49,    50,
      51,    52,    53,    54,    55,    56,    57,    58,    59,    60,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,  1835,
    1836,    -1,    -1,    -1,  1840,    -1,  1842,  1843,    -1,    -1,
      -1,  1847,  1848,    -1,    -1,    -1,    -1,    -1,  1854,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,  1862,    -1,    -1,   270,
     271,   272,   273,  1460,    -1,    -1,    -1,    -1,  1874,    -1,
      -1,    -1,  1878,    -1,    -1,    -1,   287,   288,   289,   290,
      -1,    -1,    -1,    -1,     1,    -1,    -1,  1484,    -1,    -1,
      -1,    -1,    -1,    10,    11,    12,    -1,    14,    15,    16,
      17,    18,  1499,    -1,   315,   316,    23,    24,    25,    26,
      27,    28,    -1,    -1,    31,    32,    33,    34,    35,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,  1947,  1948,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,  1550,   270,   271,   272,   273,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   287,   288,   289,   290,    -1,    -1,   388,    -1,    -1,
      -1,    -1,    -1,    -1,  1990,    -1,    -1,  1993,  1994,    -1,
      -1,  1997,    -1,    -1,    -1,    -1,  2002,    -1,    -1,   315,
     316,    -1,    -1,    -1,    -1,  1602,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,  1189,    -1,    -1,    -1,    -1,  2045,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,  2064,    -1,
      -1,  2067,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   388,    -1,    -1,    -1,    -1,    -1,    -1,  1234,
    1235,    -1,    -1,  1238,  2090,  2091,  1241,  1242,    -1,    -1,
    1245,    -1,    -1,    -1,    -1,  1250,  1251,  1252,  1253,  1254,
      -1,    -1,    -1,    -1,    -1,  1260,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,  1272,  1273,  1274,
      -1,    -1,  1277,  1278,  1279,  1280,  1281,    -1,  1283,  1284,
    1285,    -1,  1287,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   270,   271,   272,   273,    -1,    -1,    -1,
      -1,  1316,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     287,   288,   289,   290,    -1,    -1,    -1,    -1,    -1,    10,
      11,    12,    -1,    14,    15,    16,    17,    18,    -1,    -1,
      -1,    -1,    23,    24,    25,    26,    27,    28,   315,   316,
      31,    32,    33,    34,    35,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,  1794,    -1,    -1,  1835,  1836,
      -1,    -1,    -1,  1840,    -1,  1842,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,  1862,    -1,    -1,    -1,    -1,
      -1,   388,    -1,    -1,    -1,    -1,    -1,  1874,    -1,    -1,
      -1,    -1,    -1,    -1,  1843,    -1,    -1,    -1,  1847,  1848,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
       1,    -1,    -1,    -1,  1489,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
    1947,  1948,    23,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    34,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,  1990,    65,    66,    67,    68,    69,    70,
      71,    72,    73,    74,    75,    76,    77,    78,    -1,    -1,
      -1,    -1,    -1,    -1,    85,    86,    87,    88,    89,    -1,
      -1,    -1,    93,    -1,    95,    96,    97,    98,    99,    -1,
      -1,    -1,    -1,    -1,  1993,  1994,    -1,    -1,  1997,   270,
     271,   272,   273,    -1,    -1,    -1,    -1,    -1,  1603,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   287,   288,   289,   290,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,  2064,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   315,   316,  2045,    -1,    -1,    -1,
      -1,    -1,    -1,  2090,  2091,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,  2067,    -1,
      -1,    -1,    -1,   184,   185,   186,   187,   188,   189,    -1,
     191,   192,   193,   194,   195,   196,   197,   198,   199,   200,
     201,   202,   203,   204,    -1,   206,   207,   208,   209,   210,
     211,   212,   213,   214,   215,   216,   217,   218,   219,   220,
     221,   222,   223,   224,   225,   226,   227,   228,   229,   230,
     231,    -1,   233,   234,   235,   236,   237,   238,   239,   240,
     241,   242,   243,   244,   245,   246,   247,   248,   249,   250,
      -1,   252,   253,   254,   255,   256,   257,   258,   259,   260,
     261,   262,    -1,   264,   265,    -1,   267,   268,   269,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   283,   284,   285,    -1,    -1,    -1,   289,    -1,
     291,   292,   293,   294,   295,   296,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   312,   313,   314,    -1,    -1,   317,   318,   319,   320,
     321,   322,   323,   324,   325,   326,   327,   328,   329,   330,
     331,   332,   333,   334,   335,   336,   337,   338,   339,   340,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,     1,
      -1,    -1,    -1,    -1,    -1,    -1,  1841,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    23,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    34,    -1,    -1,    -1,    -1,   388,    -1,    -1,
    1875,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    65,    66,    67,    68,    69,    70,    71,
      72,    73,    74,    75,    76,    77,    78,    -1,    -1,    -1,
      -1,    -1,    -1,    85,    86,    87,    88,    89,    -1,     0,
       1,    93,     3,    95,    96,    97,    98,    99,    -1,    10,
      11,    12,    -1,    14,    15,    16,    17,    18,    19,    -1,
      -1,    -1,    23,    24,    25,    26,    27,    28,    -1,    -1,
      31,    32,    33,    34,    35,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      61,    -1,    -1,    -1,    -1,    -1,  1991,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   184,   185,   186,   187,   188,   189,    -1,   191,
     192,   193,   194,   195,   196,   197,   198,   199,   200,   201,
     202,   203,   204,    -1,   206,   207,   208,   209,   210,   211,
     212,   213,   214,   215,   216,   217,   218,   219,   220,   221,
     222,   223,   224,   225,   226,   227,   228,   229,   230,   231,
    2065,   233,   234,   235,   236,   237,   238,   239,   240,   241,
     242,   243,   244,   245,   246,   247,   248,   249,   250,    -1,
     252,   253,   254,   255,   256,   257,   258,   259,   260,   261,
     262,    -1,   264,   265,    -1,   267,   268,   269,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   283,   284,   285,    -1,    -1,    -1,   289,    -1,   291,
     292,   293,   294,   295,   296,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     312,   313,   314,    -1,    -1,   317,   318,   319,   320,   321,
     322,   323,   324,   325,   326,   327,   328,   329,   330,   331,
     332,   333,   334,   335,   336,   337,   338,   339,   340,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   270,
     271,   272,   273,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   287,   288,   289,   290,
      -1,    -1,    -1,    -1,   295,    -1,   388,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   305,    -1,    -1,   308,   309,     0,
       1,    -1,     3,    -1,   315,   316,    -1,    -1,    -1,    10,
      11,    12,    -1,    14,    15,    16,    17,    18,    19,    -1,
      -1,    -1,    23,    24,    25,    26,    27,    28,    -1,    -1,
      31,    32,    33,    34,    35,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,     0,     1,    -1,     3,    -1,    -1,    -1,    -1,    -1,
      61,    10,    11,    12,    -1,    14,    15,    16,    17,    18,
      19,   382,    -1,    -1,    23,    24,    25,    26,    27,    28,
      -1,    -1,    31,    32,    33,    34,    35,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,     0,
       1,    -1,     3,    -1,    -1,    -1,    -1,    -1,    -1,    10,
      11,    12,    61,    14,    15,    16,    17,    18,    19,    -1,
      -1,    -1,    23,    24,    25,    26,    27,    28,    -1,    -1,
      31,    32,    33,    34,    35,    -1,    -1,    -1,    -1,     0,
       1,    -1,     3,    -1,    -1,    -1,    -1,    -1,    -1,    10,
      11,    12,    -1,    14,    15,    16,    17,    18,    19,    -1,
      61,    -1,    23,    24,    25,    26,    27,    28,    -1,    -1,
      31,    32,    33,    34,    35,    -1,    -1,    -1,    -1,     0,
       1,    -1,     3,    -1,    -1,    -1,    -1,    -1,    -1,    10,
      11,    12,    -1,    14,    15,    16,    17,    18,    19,    -1,
      61,    -1,    23,    24,    25,    26,    27,    28,    -1,    -1,
      31,    32,    33,    34,    35,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      61,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   270,
     271,   272,   273,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   287,   288,   289,   290,
      -1,    -1,    -1,    -1,   295,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   305,    -1,    -1,   308,   309,    -1,
      -1,    -1,    -1,    -1,   315,   316,    -1,    -1,    -1,    -1,
      -1,   270,   271,   272,   273,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   287,   288,
     289,   290,    -1,    -1,    -1,    -1,   295,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   305,    -1,    -1,   308,
     309,    -1,    -1,    -1,    -1,    -1,   315,   316,    -1,   270,
     271,   272,   273,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   382,    -1,    -1,    -1,    -1,   287,   288,   289,   290,
      -1,    -1,    -1,    -1,   295,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   305,    -1,    -1,   308,   309,   270,
     271,   272,   273,    -1,   315,   316,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   287,   288,   289,   290,
      -1,    -1,    -1,   382,   295,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   305,    -1,    -1,   308,   309,   270,
     271,   272,   273,    -1,   315,   316,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   287,   288,   289,   290,
       4,     5,     6,     7,   295,    -1,    -1,    -1,    -1,    -1,
      -1,   382,    -1,    -1,   305,    -1,    -1,   308,   309,    -1,
      -1,    -1,    -1,    -1,   315,   316,    -1,    -1,    -1,    -1,
      -1,    10,    11,    12,    -1,    14,    15,    16,    17,    18,
      -1,    -1,    -1,    -1,    23,    24,    25,    26,    27,    28,
      -1,   382,    31,    32,    33,    34,    35,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   382,    -1,    -1,    -1,    -1,   100,   101,   102,   103,
     104,   105,   106,   107,   108,   109,   110,   111,   112,   113,
     114,   115,   116,   117,   118,   119,   120,   121,   122,   123,
     124,   125,   126,   127,   128,   129,   130,   131,   132,   133,
     134,   135,   136,   137,   138,   139,   140,   141,   142,   143,
     144,   145,   146,   147,   148,   149,   150,   151,   152,   153,
     154,   155,   156,   157,   158,   159,   160,   161,   162,   163,
     164,   165,   166,   167,   168,    -1,    -1,   100,   101,   102,
     103,   104,   105,   106,   107,   108,   109,   110,   111,   112,
     113,   114,   115,   116,   117,   118,   119,   120,   121,   122,
     123,   124,   125,   126,   127,   128,   129,   130,   131,   132,
     133,   134,   135,   136,   137,   138,   139,   140,   141,   142,
     143,   144,   145,   146,   147,   148,   149,   150,   151,   152,
     153,   154,   155,   156,   157,   158,   159,   160,   161,   162,
     163,   164,   165,   166,   167,   168,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   270,   271,   272,   273,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   287,   288,
     289,   290,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   315,   316,    -1,   343,
     344,   345,   346,   347,   348,   349,   350,   351,   352,   353,
     354,   355,   356,   357,   358,   359,   360,   361,   362,   363,
     364,   365,   366,   367,   368,   369,   370,   371,   372,   373,
     374,    -1,   376,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     384,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   388,
     343,   344,   345,   346,   347,   348,   349,   350,   351,   352,
     353,   354,   355,   356,   357,   358,   359,   360,   361,   362,
     363,   364,   365,   366,   367,   368,   369,   370,   371,   372,
     373,   374,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   387,   100,   101,   102,   103,   104,
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
     166,   167,   168,   100,   101,   102,   103,   104,   105,   106,
     107,   108,   109,   110,   111,   112,   113,   114,   115,   116,
     117,   118,   119,   120,   121,   122,   123,   124,   125,   126,
     127,   128,   129,   130,   131,   132,   133,   134,   135,   136,
     137,   138,   139,   140,   141,   142,   143,   144,   145,   146,
     147,   148,   149,   150,   151,   152,   153,   154,   155,   156,
     157,   158,   159,   160,   161,   162,   163,   164,   165,   166,
     167,   168,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   343,   344,
     345,   346,   347,   348,   349,   350,   351,   352,   353,   354,
     355,   356,   357,   358,   359,   360,   361,   362,   363,   364,
     365,   366,   367,   368,   369,   370,   371,   372,   373,   374,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   387,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   343,   344,   345,
     346,   347,   348,   349,   350,   351,   352,   353,   354,   355,
     356,   357,   358,   359,   360,   361,   362,   363,   364,   365,
     366,   367,   368,   369,   370,   371,   372,   373,   374,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   387,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   343,   344,   345,   346,
     347,   348,   349,   350,   351,   352,   353,   354,   355,   356,
     357,   358,   359,   360,   361,   362,   363,   364,   365,   366,
     367,   368,   369,   370,   371,   372,   373,   374
};

/* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
   symbol of state STATE-NUM.  */
static const yytype_uint16 yystos[] =
{
       0,   396,     0,     1,     3,    10,    11,    12,    14,    15,
      16,    17,    18,    19,    23,    24,    25,    26,    27,    28,
      31,    32,    33,    34,    35,    61,   270,   271,   272,   273,
     287,   288,   289,   290,   295,   305,   308,   309,   315,   316,
     382,   397,   425,   428,   432,   433,   434,   435,   436,   441,
     442,   444,   445,   448,   456,   462,   466,   469,   472,   476,
     487,   488,   494,   496,   499,   502,   505,   509,   513,   517,
      22,    22,   428,   430,   431,   431,   431,   430,   430,   430,
     430,   430,    22,   426,   428,   487,   431,    62,   430,   430,
     430,   430,   431,   431,   431,   431,    22,   431,   431,   431,
     487,    22,   430,     1,     1,     1,     1,     1,     1,    36,
      37,    38,    39,    40,    41,    42,    43,    44,    45,    46,
      47,    48,    49,    50,    51,    52,    53,    54,    55,    56,
      57,    58,    59,    60,   493,   494,   500,   430,   390,   479,
     480,   387,   387,   387,   463,   473,   477,   470,   467,   427,
     480,   487,   503,   383,   287,   288,   429,   429,   429,   429,
     384,   387,   506,   510,   514,   518,   387,   387,   382,   446,
     382,   382,    22,   480,   480,   478,    21,    21,    21,   480,
     480,   480,   480,   480,   480,   387,   387,   501,     4,     5,
     401,   457,   453,   449,   451,    29,    30,    36,   437,   439,
     440,    21,   480,   480,   480,   480,    21,    21,   480,   495,
     387,     1,    22,   391,   387,   387,   387,   387,   387,   387,
      19,    20,   260,   421,   424,   388,   496,   480,   480,   480,
     480,   378,   438,    47,   385,   386,   507,   511,   515,   519,
     387,     1,   375,   383,   392,   479,   481,   482,   483,   484,
     485,   486,   489,   490,   491,   492,   432,   433,   434,   435,
     436,   441,   442,   445,   448,   456,   462,   466,   469,   472,
     476,   496,   497,   498,   502,   375,   461,   471,   475,   220,
     221,   468,   465,   424,    22,    22,   422,   384,     1,   388,
     421,   388,   458,   454,   450,   452,    22,   438,   387,   439,
     387,   387,   387,   387,   443,     1,     6,     7,   376,   384,
     387,   399,   400,   401,   409,    22,   376,   384,   393,   400,
     489,   479,   489,   479,   489,   490,   382,   388,   497,     9,
     399,   400,   401,   404,     1,   169,   170,   171,   172,   173,
     388,     1,   235,   236,   237,   238,   239,   240,   241,   242,
     244,   245,   246,   247,   248,   252,   253,   254,   255,   256,
     257,   258,   259,   388,     1,   319,   320,   321,   322,   323,
     324,   325,   326,   327,   328,   329,   330,   331,   332,   333,
     334,   335,   336,   337,   338,   339,   340,   388,   375,   375,
       1,   207,   209,   210,   213,   214,   215,   216,   217,   218,
     219,   222,   223,   224,   225,   226,   227,   228,   229,   230,
     231,   233,   234,   388,     1,   184,   185,   186,   187,   188,
     189,   191,   192,   193,   194,   195,   196,   197,   198,   200,
     201,   202,   203,   204,   206,   388,   388,   286,   382,   423,
     480,     9,    22,   387,   387,   387,   387,    22,    21,   297,
     504,   508,   512,   516,     1,    73,   388,   399,   400,   399,
     400,   376,   377,   378,   379,   381,   376,   377,   378,   379,
     381,   399,   400,   401,   409,   411,   412,   415,   400,   400,
     376,   377,   378,   393,   489,   489,   479,   489,   382,   382,
     382,   382,   382,   375,   375,   375,   375,   375,   382,   375,
     375,   375,   375,   375,   375,   375,   375,   375,   375,   375,
     375,   375,   375,   375,   375,   375,   375,   375,   375,   375,
     382,   375,   375,   375,   392,   392,   375,   375,   375,   375,
     375,   375,   375,   375,   375,   375,   375,   375,   375,   375,
     375,   375,   375,   409,   100,   101,   102,   103,   104,   105,
     106,   107,   108,   109,   110,   111,   112,   113,   114,   115,
     116,   117,   118,   119,   120,   121,   122,   123,   124,   125,
     126,   127,   128,   129,   130,   131,   132,   133,   134,   135,
     136,   137,   138,   139,   140,   141,   142,   143,   144,   145,
     146,   147,   148,   149,   150,   151,   152,   153,   154,   155,
     156,   157,   158,   159,   160,   161,   162,   163,   164,   165,
     166,   167,   168,   343,   344,   345,   346,   347,   348,   349,
     350,   351,   352,   353,   354,   355,   356,   357,   358,   359,
     360,   361,   362,   363,   364,   365,   366,   367,   368,   369,
     370,   371,   372,   373,   374,   418,   382,   375,   375,   375,
     375,   375,   375,   375,   375,   375,   375,   375,   375,   375,
     375,   375,   375,   375,   375,   375,   375,   375,   375,   382,
     375,   375,   375,   375,   375,   375,   375,   375,   375,   375,
     375,   375,   375,   375,   375,   375,   375,   375,   375,   375,
     382,   480,   387,   385,   385,   455,   447,   447,   447,   375,
      34,   298,   299,   300,   301,   302,   303,   304,   388,    34,
     306,   307,   388,   277,   280,   388,   310,   311,   388,   375,
     385,   385,   399,   400,   399,   400,   399,   400,   399,   400,
     399,   400,   399,   400,   399,   400,   399,   400,   399,   400,
     399,   400,   411,     1,   386,   388,   410,     1,   386,   410,
     386,   410,   400,   400,   400,   489,     1,   174,   175,   176,
     177,   178,   179,   459,   459,     1,   180,   181,   182,   183,
     460,   460,   460,   399,   400,   401,   402,   418,   402,   402,
     402,   409,   402,   418,   399,   400,   401,   403,   403,   418,
     418,   418,   403,   418,   403,   403,   418,   418,   418,   403,
     387,   418,   418,   403,   403,   418,   400,   401,   404,   418,
     418,   418,   418,   418,   404,   404,   418,   404,   418,   403,
     418,   418,   418,   418,   418,   382,   402,   418,   404,   402,
     418,   418,   418,   418,   418,   418,   418,   418,   409,   402,
     402,   402,   418,   409,   402,   402,   402,   402,   404,   402,
     402,   418,   404,   402,   403,   403,   402,   402,   418,   418,
     402,   409,   418,   404,   404,   402,   402,   402,   402,   387,
     420,   375,   375,    22,   274,   275,   388,    22,    73,   276,
     277,   278,   279,   280,   388,   388,   388,   409,   384,   375,
     375,   375,   375,   375,   375,   375,   384,   375,   375,   375,
     375,   375,   375,    22,   410,   399,   400,   409,   401,   382,
     382,   382,   382,   382,   382,   382,   382,   382,   382,   404,
     382,   382,   382,   382,   382,   382,   382,   382,   382,   382,
     382,   382,   382,   382,   382,   404,   382,   382,   393,   393,
     382,   382,   382,   382,   382,   382,   382,   382,   382,   382,
     382,   382,   382,   382,   382,   382,   382,   386,   382,   382,
     382,   382,   382,   382,   382,   382,   382,   382,   382,   382,
     404,   382,   382,   382,   382,   402,   382,   382,   382,   382,
     382,   382,   382,   382,   382,   382,   382,   382,   382,   382,
     382,   382,   382,   418,   382,   382,   382,   382,   382,   382,
     382,   420,     1,    23,    34,    65,    66,    67,    68,    69,
      70,    71,    72,    73,    74,    75,    76,    77,    78,    85,
      86,    87,    88,    89,    93,    95,    96,    97,    98,    99,
     184,   185,   186,   187,   188,   189,   191,   192,   193,   194,
     195,   196,   197,   198,   199,   200,   201,   202,   203,   204,
     206,   207,   208,   209,   210,   211,   212,   213,   214,   215,
     216,   217,   218,   219,   220,   221,   222,   223,   224,   225,
     226,   227,   228,   229,   230,   231,   233,   234,   235,   236,
     237,   238,   239,   240,   241,   242,   243,   244,   245,   246,
     247,   248,   249,   250,   252,   253,   254,   255,   256,   257,
     258,   259,   260,   261,   262,   264,   265,   267,   268,   269,
     283,   284,   285,   289,   291,   292,   293,   294,   295,   296,
     312,   313,   314,   317,   318,   319,   320,   321,   322,   323,
     324,   325,   326,   327,   328,   329,   330,   331,   332,   333,
     334,   335,   336,   337,   338,   339,   340,   388,   464,   474,
     403,   403,   375,   375,   375,   375,   375,   375,   375,   375,
     375,   375,    22,   408,    22,   409,    22,    22,    22,    22,
     400,    22,    22,   409,   409,   282,   409,   281,     9,     9,
     382,   411,   386,   405,   386,   375,   375,   418,   405,   405,
     405,   388,   382,   384,   384,   375,   390,   375,   390,   375,
     390,   375,   390,   375,   390,   375,   384,   384,   384,   375,
     384,   375,   384,   375,   384,   384,   384,   375,   390,   375,
     390,   375,   390,   375,   390,   375,   390,   384,   384,   384,
     384,   384,   384,   375,   375,   375,   375,   375,   375,   375,
     375,   375,   375,   375,   375,   375,   375,   375,   375,   375,
     375,   375,   375,   375,   375,   375,   375,   375,   375,   375,
     375,   375,   375,   375,   375,   375,   375,   375,   375,   375,
     375,   375,   375,   375,   375,   375,   375,   375,   375,   375,
     375,   375,   375,   375,   375,   375,   375,   375,   375,   375,
     375,   375,   375,   375,   375,   375,   375,   375,   375,   375,
     375,   375,   375,   375,   375,   384,   375,   375,   375,   375,
     375,   375,   392,   375,   384,   375,   375,   375,   375,   375,
     375,   375,   375,   375,   375,   375,   390,   375,   390,   375,
     390,   375,   390,   375,   375,   375,   392,   392,   375,   375,
     375,   375,   375,   375,   375,   375,   375,   375,   375,   375,
     375,   375,   375,   375,   375,   382,   382,     9,    22,    22,
     409,     9,    22,   282,   403,   409,   282,   409,   404,    22,
     281,   405,   410,   385,   408,   382,   382,   382,   382,   382,
     382,   385,   408,   408,   382,   400,   413,   382,   382,   382,
     410,   404,   404,   387,   387,   410,   404,   402,   418,    22,
      22,     5,    22,   406,   407,   409,     9,    22,   398,   407,
     398,   407,   398,   407,   398,   407,   398,     9,    22,    22,
      22,    22,    22,    22,    22,    22,    22,    22,    22,   407,
     398,   407,   398,   407,   398,   407,   398,   407,   398,    22,
      22,    22,    22,    22,    22,   404,   402,   402,   418,   404,
     402,   403,   403,   402,   402,   418,   418,   418,   402,   409,
     409,   404,   404,   402,   402,   402,   402,   402,   409,   418,
     404,   387,   387,   402,   418,   418,   418,   418,   418,   418,
     409,   418,   418,   418,   409,   402,   402,   402,   418,   409,
     402,   402,   402,   402,   402,   418,   402,   402,   402,   409,
     402,   387,   418,   387,   403,   403,   418,   418,   418,   387,
     387,   403,   387,   418,   403,   403,   418,   418,   418,   403,
     398,   403,   263,    22,   266,   419,    22,   409,   403,   409,
      22,   403,   402,   387,   407,   400,   409,   414,   414,    22,
     400,   409,   407,   398,   407,   398,   407,   398,   407,   398,
     387,   418,   418,   403,   403,   418,   404,   418,   418,   418,
     418,   418,   404,   404,   418,   404,   418,   403,   418,   418,
     418,   418,   382,   382,   382,   408,   382,   382,   382,   382,
     413,   382,   413,   382,   382,   382,    22,   382,   375,   489,
     410,   375,   489,   410,   410,   386,   410,   410,   388,   418,
     418,   382,   405,   405,   410,   385,   385,   392,   394,   405,
     382,   406,   391,   382,   391,   382,   391,   382,   391,   382,
     391,   382,   385,   385,   385,   382,   385,   382,   385,   382,
     385,   385,   385,   382,   391,   382,   391,   382,   391,   382,
     391,   382,   391,   385,   385,   385,   385,   385,   385,   382,
     382,   382,   382,   382,   382,   382,   382,   382,   382,   382,
     382,   382,   382,   418,   404,   382,   382,   382,   382,   382,
     382,   382,   418,   382,   382,   418,   418,   382,   382,   382,
     382,   382,   382,   382,   418,   382,   382,   382,   404,   382,
     382,   382,   382,   402,   382,   382,   382,   382,   382,   382,
     382,   382,   382,   404,   382,   418,   382,   418,   382,   382,
     382,   382,   382,   418,   418,   382,   418,   382,   382,   382,
     382,   382,   382,   382,   385,   382,   382,   382,   382,   389,
     382,   411,   393,   413,   386,   382,   382,    22,   382,   382,
     413,     1,   382,   386,   382,   382,   382,   413,   382,   391,
     382,   391,   382,   391,   382,   391,   404,   382,   382,   393,
     393,   382,   382,   382,   382,   382,   382,   382,   382,   382,
     382,   382,   382,   382,   382,   382,   382,   382,   410,   410,
     410,    22,   409,   382,   382,    22,   409,   382,   382,   382,
     400,   382,   382,   382,   386,   386,   404,   402,   382,   375,
     375,   489,   400,    71,    72,    73,    77,    78,    22,   410,
     375,   375,   375,   375,   375,   375,   375,   375,   382,   382,
     382,   375,   375,   375,   375,   375,   375,   375,   375,   375,
     375,   375,   375,   375,   405,   386,   386,   386,   386,   386,
     405,   405,   405,   386,   386,   386,   386,   386,   375,   266,
     410,   375,   410,    22,   386,   410,   409,   410,   375,   375,
     375,   375,   386,   375,   375,   382,   382,   382,   382,   408,
     382,   408,   403,   418,   405,   405,    22,    22,   409,   382,
     393,   385,   385,   385,   385,   385,   407,   407,   407,   407,
     407,    22,   387,   400,    22,    22,   387,   400,   407,   407,
     407,   407,   407,    22,   400,    22,    22,   387,   400,    22,
     418,   404,   404,   418,   418,   418,   404,   402,   404,   403,
     418,   418,   418,   403,   403,   382,   412,   382,   385,     1,
      22,    29,   399,   400,   416,   417,   413,   382,   407,   407,
     407,   407,   404,   387,   387,   410,   410,   386,   386,   404,
     402,   382,   382,   408,   385,   375,   375,   375,   375,   375,
     382,   382,   382,   382,   382,   382,    22,   382,   382,   382,
      22,   382,   382,   382,   382,   382,   382,   382,   382,   382,
     382,    22,   382,   382,   410,   410,   410,   388,   386,   410,
     405,   405,   410,   386,   386,   386,   386,   386,   382,   382,
     382,     1,   386,   388,   410,   382,   382,   382,   382,   388,
     418,   418,   382,   382,   404,   404,   410,   410,   410,   375,
      22,   400,    22,    22,   400,   386,   386,   386,   382,   382,
     382,   382,   418,   382,   404,   402,   382,   403,   403,   418,
     418,   403,   416,   382,   382,   386,   386,   388,   388,   382,
     382,   382,    22,   409,   414,   382,   382,   382,   382,   382,
     400,   400,   400,   386,   405,   405,   388,   386,   388,   386,
     388,   403,   418,   382,   382,   382,   412,   415,   382,   388,
     388,   388,   418,   404,   402,   382,   403,   382,   418,   382,
     386,   386,   410,   410,   382,   382,   382,   388,   410,   410,
     388,   388,   404,   404,   382,   382,   382,   382,   382,   382,
     382,   388,   388,   382,   382
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
      case 406: /* "shaderlist" */

/* Line 1000 of yacc.c  */
#line 447 "Grammar.ypp"
	{ if((yyvaluep->shaderList)) delete (yyvaluep->shaderList); };

/* Line 1000 of yacc.c  */
#line 4292 "C:/p4/GitHub/nvFX/FxParser/Grammar.cpp"
	break;
      case 407: /* "one_or_more_shaders" */

/* Line 1000 of yacc.c  */
#line 447 "Grammar.ypp"
	{ if((yyvaluep->shaderList)) delete (yyvaluep->shaderList); };

/* Line 1000 of yacc.c  */
#line 4301 "C:/p4/GitHub/nvFX/FxParser/Grammar.cpp"
	break;
      case 408: /* "varnamelist" */

/* Line 1000 of yacc.c  */
#line 446 "Grammar.ypp"
	{ if((yyvaluep->varNameList)) delete (yyvaluep->varNameList); };

/* Line 1000 of yacc.c  */
#line 4310 "C:/p4/GitHub/nvFX/FxParser/Grammar.cpp"
	break;
      case 411: /* "fexplist" */

/* Line 1000 of yacc.c  */
#line 448 "Grammar.ypp"
	{ if((yyvaluep->floatList)) delete (yyvaluep->floatList); };

/* Line 1000 of yacc.c  */
#line 4319 "C:/p4/GitHub/nvFX/FxParser/Grammar.cpp"
	break;
      case 412: /* "fexpveclist" */

/* Line 1000 of yacc.c  */
#line 449 "Grammar.ypp"
	{ if((yyvaluep->floatVecList)) delete (yyvaluep->floatVecList); };

/* Line 1000 of yacc.c  */
#line 4328 "C:/p4/GitHub/nvFX/FxParser/Grammar.cpp"
	break;
      case 413: /* "iexplist" */

/* Line 1000 of yacc.c  */
#line 450 "Grammar.ypp"
	{ if((yyvaluep->intList)) delete (yyvaluep->intList); };

/* Line 1000 of yacc.c  */
#line 4337 "C:/p4/GitHub/nvFX/FxParser/Grammar.cpp"
	break;
      case 415: /* "bexplist" */

/* Line 1000 of yacc.c  */
#line 451 "Grammar.ypp"
	{ if((yyvaluep->boolList)) delete (yyvaluep->boolList); };

/* Line 1000 of yacc.c  */
#line 4346 "C:/p4/GitHub/nvFX/FxParser/Grammar.cpp"
	break;
      case 440: /* "arglist" */

/* Line 1000 of yacc.c  */
#line 452 "Grammar.ypp"
	{ if((yyvaluep->argList)) delete (yyvaluep->argList); };

/* Line 1000 of yacc.c  */
#line 4355 "C:/p4/GitHub/nvFX/FxParser/Grammar.cpp"
	break;
      case 481: /* "valueassignment_f" */

/* Line 1000 of yacc.c  */
#line 449 "Grammar.ypp"
	{ if((yyvaluep->floatVecList)) delete (yyvaluep->floatVecList); };

/* Line 1000 of yacc.c  */
#line 4364 "C:/p4/GitHub/nvFX/FxParser/Grammar.cpp"
	break;
      case 482: /* "valueassignment_fv" */

/* Line 1000 of yacc.c  */
#line 449 "Grammar.ypp"
	{ if((yyvaluep->floatVecList)) delete (yyvaluep->floatVecList); };

/* Line 1000 of yacc.c  */
#line 4373 "C:/p4/GitHub/nvFX/FxParser/Grammar.cpp"
	break;
      case 483: /* "valueassignment_fvl" */

/* Line 1000 of yacc.c  */
#line 449 "Grammar.ypp"
	{ if((yyvaluep->floatVecList)) delete (yyvaluep->floatVecList); };

/* Line 1000 of yacc.c  */
#line 4382 "C:/p4/GitHub/nvFX/FxParser/Grammar.cpp"
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
#line 379 "Grammar.ypp"
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
#line 4540 "C:/p4/GitHub/nvFX/FxParser/Grammar.cpp"

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
#line 1033 "Grammar.ypp"
    {
    curContainer->separateShadersEnable((yyvsp[(4) - (4)].bval));
;}
    break;

  case 30:

/* Line 1455 of yacc.c  */
#line 1041 "Grammar.ypp"
    { (yyval.s) = (yyvsp[(1) - (1)].s); ;}
    break;

  case 31:

/* Line 1455 of yacc.c  */
#line 1042 "Grammar.ypp"
    { (yyval.s) = (yyvsp[(1) - (1)].s); ;}
    break;

  case 32:

/* Line 1455 of yacc.c  */
#line 1048 "Grammar.ypp"
    {  (yyval.fval)  =  (yyvsp[(1) - (1)].fval); ;}
    break;

  case 33:

/* Line 1455 of yacc.c  */
#line 1049 "Grammar.ypp"
    {  (yyval.fval)  =  (yyvsp[(1) - (3)].fval)  +  (yyvsp[(3) - (3)].fval); ;}
    break;

  case 34:

/* Line 1455 of yacc.c  */
#line 1050 "Grammar.ypp"
    {  (yyval.fval)  =  (float)(yyvsp[(1) - (3)].ival)  +  (yyvsp[(3) - (3)].fval); ;}
    break;

  case 35:

/* Line 1455 of yacc.c  */
#line 1051 "Grammar.ypp"
    {  (yyval.fval)  =  (yyvsp[(1) - (3)].fval)  +  (float)(yyvsp[(3) - (3)].ival); ;}
    break;

  case 36:

/* Line 1455 of yacc.c  */
#line 1052 "Grammar.ypp"
    {  (yyval.fval)  =  (yyvsp[(1) - (3)].fval)  -  (yyvsp[(3) - (3)].fval); ;}
    break;

  case 37:

/* Line 1455 of yacc.c  */
#line 1053 "Grammar.ypp"
    {  (yyval.fval)  =  (float)(yyvsp[(1) - (3)].ival)  -  (yyvsp[(3) - (3)].fval); ;}
    break;

  case 38:

/* Line 1455 of yacc.c  */
#line 1054 "Grammar.ypp"
    {  (yyval.fval)  =  (yyvsp[(1) - (3)].fval)  -  (float)(yyvsp[(3) - (3)].ival); ;}
    break;

  case 39:

/* Line 1455 of yacc.c  */
#line 1055 "Grammar.ypp"
    {  (yyval.fval)  =  (yyvsp[(1) - (3)].fval)  *  (yyvsp[(3) - (3)].fval); ;}
    break;

  case 40:

/* Line 1455 of yacc.c  */
#line 1056 "Grammar.ypp"
    {  (yyval.fval)  =  (float)(yyvsp[(1) - (3)].ival)  *  (yyvsp[(3) - (3)].fval); ;}
    break;

  case 41:

/* Line 1455 of yacc.c  */
#line 1057 "Grammar.ypp"
    {  (yyval.fval)  =  (yyvsp[(1) - (3)].fval)  *  (float)(yyvsp[(3) - (3)].ival); ;}
    break;

  case 42:

/* Line 1455 of yacc.c  */
#line 1058 "Grammar.ypp"
    {  (yyval.fval)  =  (yyvsp[(1) - (3)].fval)  /  (yyvsp[(3) - (3)].fval); ;}
    break;

  case 43:

/* Line 1455 of yacc.c  */
#line 1059 "Grammar.ypp"
    {  (yyval.fval)  =  (float)(yyvsp[(1) - (3)].ival)  /  (yyvsp[(3) - (3)].fval); ;}
    break;

  case 44:

/* Line 1455 of yacc.c  */
#line 1060 "Grammar.ypp"
    {  (yyval.fval)  =  (yyvsp[(1) - (3)].fval)  /  (float)(yyvsp[(3) - (3)].ival); ;}
    break;

  case 45:

/* Line 1455 of yacc.c  */
#line 1061 "Grammar.ypp"
    {  (yyval.fval)  =  (float)(yyvsp[(1) - (3)].ival)  /  (float)(yyvsp[(3) - (3)].ival); ;}
    break;

  case 46:

/* Line 1455 of yacc.c  */
#line 1062 "Grammar.ypp"
    {  (yyval.fval)  =  -(yyvsp[(2) - (2)].fval); ;}
    break;

  case 47:

/* Line 1455 of yacc.c  */
#line 1063 "Grammar.ypp"
    {  (yyval.fval)  =  pow  ((yyvsp[(1) - (3)].fval),  (yyvsp[(3) - (3)].fval)); ;}
    break;

  case 48:

/* Line 1455 of yacc.c  */
#line 1064 "Grammar.ypp"
    {  (yyval.fval)  =  pow  ((float)(yyvsp[(1) - (3)].ival),  (yyvsp[(3) - (3)].fval)); ;}
    break;

  case 49:

/* Line 1455 of yacc.c  */
#line 1065 "Grammar.ypp"
    {  (yyval.fval)  =  pow  ((yyvsp[(1) - (3)].fval),  (float)(yyvsp[(3) - (3)].ival)); ;}
    break;

  case 50:

/* Line 1455 of yacc.c  */
#line 1066 "Grammar.ypp"
    {  (yyval.fval)  =  pow  ((float)(yyvsp[(1) - (3)].ival),  (float)(yyvsp[(3) - (3)].ival)); ;}
    break;

  case 51:

/* Line 1455 of yacc.c  */
#line 1067 "Grammar.ypp"
    {  (yyval.fval)  =  (yyvsp[(2) - (3)].fval); ;}
    break;

  case 52:

/* Line 1455 of yacc.c  */
#line 1075 "Grammar.ypp"
    {  (yyval.ival)  =  (yyvsp[(1) - (1)].ival); ;}
    break;

  case 53:

/* Line 1455 of yacc.c  */
#line 1076 "Grammar.ypp"
    {  (yyval.ival)  =  (yyvsp[(1) - (3)].ival)  +  (yyvsp[(3) - (3)].ival); ;}
    break;

  case 54:

/* Line 1455 of yacc.c  */
#line 1077 "Grammar.ypp"
    {  (yyval.ival)  =  (yyvsp[(1) - (3)].ival)  -  (yyvsp[(3) - (3)].ival); ;}
    break;

  case 55:

/* Line 1455 of yacc.c  */
#line 1078 "Grammar.ypp"
    {  (yyval.ival)  =  (yyvsp[(1) - (3)].ival)  *  (yyvsp[(3) - (3)].ival); ;}
    break;

  case 56:

/* Line 1455 of yacc.c  */
#line 1079 "Grammar.ypp"
    {  (yyval.ival)  =  -(yyvsp[(2) - (2)].ival); ;}
    break;

  case 57:

/* Line 1455 of yacc.c  */
#line 1080 "Grammar.ypp"
    {  (yyval.ival)  =  (yyvsp[(2) - (3)].ival); ;}
    break;

  case 58:

/* Line 1455 of yacc.c  */
#line 1085 "Grammar.ypp"
    {  (yyval.bval)  =  true; ;}
    break;

  case 59:

/* Line 1455 of yacc.c  */
#line 1086 "Grammar.ypp"
    {  (yyval.bval)  =  false; ;}
    break;

  case 60:

/* Line 1455 of yacc.c  */
#line 1092 "Grammar.ypp"
    { (yyval.bval) = (yyvsp[(1) - (1)].bval); ;}
    break;

  case 61:

/* Line 1455 of yacc.c  */
#line 1093 "Grammar.ypp"
    { 
    if(((yyvsp[(1) - (1)].ival) != 0)&&((yyvsp[(1) - (1)].ival) != 1)) yyerror("    Warning : truncation of float to bool");
    (yyval.bval) = (yyvsp[(1) - (1)].ival) ? true: false; ;}
    break;

  case 62:

/* Line 1455 of yacc.c  */
#line 1096 "Grammar.ypp"
    { 
    if(((yyvsp[(1) - (1)].fval) != 0.0f)&&((yyvsp[(1) - (1)].fval) != 1.0f)) yyerror("    Warning : truncation of float to bool");
    (yyval.bval) = (yyvsp[(1) - (1)].fval) ? true: false; ;}
    break;

  case 63:

/* Line 1455 of yacc.c  */
#line 1100 "Grammar.ypp"
    { (yyval.ival) = (yyvsp[(1) - (1)].ival); ;}
    break;

  case 64:

/* Line 1455 of yacc.c  */
#line 1101 "Grammar.ypp"
    { 
    if(((yyvsp[(1) - (1)].fval) - (int)(yyvsp[(1) - (1)].fval))!=0.0f)
        yyerror("    Warning : truncation of float to int");
    (yyval.ival) = (int)(yyvsp[(1) - (1)].fval); ;}
    break;

  case 65:

/* Line 1455 of yacc.c  */
#line 1105 "Grammar.ypp"
    { (yyval.ival) = (yyvsp[(1) - (1)].bval) ? 1: 0; ;}
    break;

  case 66:

/* Line 1455 of yacc.c  */
#line 1108 "Grammar.ypp"
    { (yyval.fval) = (yyvsp[(1) - (1)].fval); ;}
    break;

  case 67:

/* Line 1455 of yacc.c  */
#line 1109 "Grammar.ypp"
    { (yyval.fval) = (float)(yyvsp[(1) - (1)].ival); ;}
    break;

  case 68:

/* Line 1455 of yacc.c  */
#line 1110 "Grammar.ypp"
    { (yyval.fval) = (yyvsp[(1) - (1)].bval) ? 1.0f: 0.0f; ;}
    break;

  case 71:

/* Line 1455 of yacc.c  */
#line 1135 "Grammar.ypp"
    { (yyval.shaderList) = NULL; ;}
    break;

  case 72:

/* Line 1455 of yacc.c  */
#line 1136 "Grammar.ypp"
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
#line 1151 "Grammar.ypp"
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
#line 1168 "Grammar.ypp"
    { (yyval.shaderList) = NULL; ;}
    break;

  case 75:

/* Line 1455 of yacc.c  */
#line 1184 "Grammar.ypp"
    { (yyval.shaderList) = (yyvsp[(1) - (1)].shaderList); ;}
    break;

  case 76:

/* Line 1455 of yacc.c  */
#line 1185 "Grammar.ypp"
    { (yyval.shaderList) = (yyvsp[(2) - (3)].shaderList); ;}
    break;

  case 77:

/* Line 1455 of yacc.c  */
#line 1192 "Grammar.ypp"
    { (yyval.varNameList) = NULL; ;}
    break;

  case 78:

/* Line 1455 of yacc.c  */
#line 1193 "Grammar.ypp"
    {
    (yyval.varNameList) = new VarNameList;
    (yyval.varNameList)->push_back(*(yyvsp[(1) - (1)].s));
    delete (yyvsp[(1) - (1)].s);
;}
    break;

  case 79:

/* Line 1455 of yacc.c  */
#line 1198 "Grammar.ypp"
    {
    (yyval.varNameList) = (yyvsp[(1) - (3)].varNameList);
    (yyval.varNameList)->push_back(*(yyvsp[(3) - (3)].s));
    delete (yyvsp[(3) - (3)].s);
;}
    break;

  case 82:

/* Line 1455 of yacc.c  */
#line 1223 "Grammar.ypp"
    {
    (yyval.floatList) = new FloatList;
    (yyval.floatList)->push_back((yyvsp[(1) - (1)].fval));
;}
    break;

  case 83:

/* Line 1455 of yacc.c  */
#line 1227 "Grammar.ypp"
    {
    (yyval.floatList) = new FloatList;
    (yyval.floatList)->push_back((float)(yyvsp[(1) - (1)].ival));
;}
    break;

  case 84:

/* Line 1455 of yacc.c  */
#line 1231 "Grammar.ypp"
    {
    (yyval.floatList) = (yyvsp[(1) - (3)].floatList);
    (yyval.floatList)->push_back((yyvsp[(3) - (3)].fval));
;}
    break;

  case 85:

/* Line 1455 of yacc.c  */
#line 1235 "Grammar.ypp"
    {
    (yyval.floatList) = (yyvsp[(1) - (3)].floatList);
    (yyval.floatList)->push_back((float)(yyvsp[(3) - (3)].ival));
;}
    break;

  case 86:

/* Line 1455 of yacc.c  */
#line 1239 "Grammar.ypp"
    { 
    (yyval.floatList) = (yyvsp[(1) - (2)].floatList);
    yyerror ("Error in the list of floats");
;}
    break;

  case 87:

/* Line 1455 of yacc.c  */
#line 1245 "Grammar.ypp"
    {
    (yyval.floatVecList) = new FloatVecList;
    (yyval.floatVecList)->push_back((yyvsp[(2) - (3)].floatList));
;}
    break;

  case 88:

/* Line 1455 of yacc.c  */
#line 1249 "Grammar.ypp"
    {
    (yyval.floatVecList) = (yyvsp[(1) - (5)].floatVecList);
    (yyval.floatVecList)->push_back((yyvsp[(4) - (5)].floatList));
;}
    break;

  case 89:

/* Line 1455 of yacc.c  */
#line 1253 "Grammar.ypp"
    { (yyval.floatVecList) = (yyvsp[(1) - (2)].floatVecList); yyerror ("Error in the list of default values"); ;}
    break;

  case 90:

/* Line 1455 of yacc.c  */
#line 1256 "Grammar.ypp"
    {
    (yyval.intList) = new IntList;
    (yyval.intList)->push_back((yyvsp[(1) - (1)].ival));
;}
    break;

  case 91:

/* Line 1455 of yacc.c  */
#line 1265 "Grammar.ypp"
    {
    (yyval.intList) = (yyvsp[(1) - (3)].intList);
    (yyval.intList)->push_back((yyvsp[(3) - (3)].ival));
;}
    break;

  case 92:

/* Line 1455 of yacc.c  */
#line 1276 "Grammar.ypp"
    {
    (yyval.intVecList) = new IntVecList;
    (yyval.intVecList)->push_back((yyvsp[(2) - (3)].intList));
;}
    break;

  case 93:

/* Line 1455 of yacc.c  */
#line 1280 "Grammar.ypp"
    {
    (yyval.intVecList) = (yyvsp[(1) - (5)].intVecList);
    (yyval.intVecList)->push_back((yyvsp[(4) - (5)].intList));
;}
    break;

  case 94:

/* Line 1455 of yacc.c  */
#line 1284 "Grammar.ypp"
    { yyerror ("Error in the list of default values"); ;}
    break;

  case 95:

/* Line 1455 of yacc.c  */
#line 1287 "Grammar.ypp"
    {
    (yyval.boolList) = new BoolList;
    (yyval.boolList)->push_back((yyvsp[(1) - (1)].bval));
;}
    break;

  case 96:

/* Line 1455 of yacc.c  */
#line 1291 "Grammar.ypp"
    {
    (yyval.boolList) = (yyvsp[(1) - (3)].boolList);
    (yyval.boolList)->push_back((yyvsp[(3) - (3)].bval));
;}
    break;

  case 97:

/* Line 1455 of yacc.c  */
#line 1311 "Grammar.ypp"
    { 
    (yyval.argVal) = new ArgVal(IUniform::TInt); (yyval.argVal)->val.i[0] = (yyvsp[(1) - (1)].ival); 
    LOGD("Arg val = %d\n", (yyvsp[(1) - (1)].ival));
;}
    break;

  case 98:

/* Line 1455 of yacc.c  */
#line 1315 "Grammar.ypp"
    { 
    (yyval.argVal) = new ArgVal(IUniform::TFloat); (yyval.argVal)->val.f[0] = (yyvsp[(1) - (1)].fval);
    LOGD("Arg val = %f\n", (yyvsp[(1) - (1)].fval));
;}
    break;

  case 99:

/* Line 1455 of yacc.c  */
#line 1319 "Grammar.ypp"
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
#line 1348 "Grammar.ypp"
    { 
    yyerror("Error in argument\n");
;}
    break;

  case 101:

/* Line 1455 of yacc.c  */
#line 1353 "Grammar.ypp"
    { (yyval.argValList) = new ArgValList; ;}
    break;

  case 102:

/* Line 1455 of yacc.c  */
#line 1354 "Grammar.ypp"
    { (yyval.argValList) = new ArgValList; ;}
    break;

  case 103:

/* Line 1455 of yacc.c  */
#line 1355 "Grammar.ypp"
    {
    (yyval.argValList) = new ArgValList;
    (yyval.argValList)->push_back((yyvsp[(1) - (1)].argVal));
    ;}
    break;

  case 104:

/* Line 1455 of yacc.c  */
#line 1359 "Grammar.ypp"
    {
    (yyval.argValList) = (yyvsp[(1) - (3)].argValList);
    (yyval.argValList)->push_back((yyvsp[(3) - (3)].argVal));
    ;}
    break;

  case 105:

/* Line 1455 of yacc.c  */
#line 1363 "Grammar.ypp"
    { yyerror("Error in argument list\n");;}
    break;

  case 207:

/* Line 1455 of yacc.c  */
#line 1485 "Grammar.ypp"
    { (yyval.clear_mode) = (yyvsp[(1) - (1)].clear_mode); ;}
    break;

  case 208:

/* Line 1455 of yacc.c  */
#line 1486 "Grammar.ypp"
    { (yyval.clear_mode) = (ClearMode)((unsigned int)(yyvsp[(3) - (3)].clear_mode) | (unsigned int)(yyvsp[(1) - (3)].clear_mode)); ;}
    break;

  case 212:

/* Line 1455 of yacc.c  */
#line 1496 "Grammar.ypp"
    {
    //if($4->size() > 0)
    //    curPass->createState(IPassState::TVertexShader, & (*$4)[0], (int)$4->size());
    delete (yyvsp[(4) - (5)].s);
    ;}
    break;

  case 213:

/* Line 1455 of yacc.c  */
#line 1501 "Grammar.ypp"
    {
    //if($4->size() > 0)
    //    curPass->createState(IPassState::TVertexShader, & (*$4)[0], (int)$4->size());
    ;}
    break;

  case 214:

/* Line 1455 of yacc.c  */
#line 1505 "Grammar.ypp"
    {
    if((yyvsp[(4) - (5)].shaderList)->size() > 0)
        curPass->createState(IPassState::TVertexShader, & (*(yyvsp[(4) - (5)].shaderList))[0], (int)(yyvsp[(4) - (5)].shaderList)->size());
    delete (yyvsp[(4) - (5)].shaderList);
    ;}
    break;

  case 215:

/* Line 1455 of yacc.c  */
#line 1510 "Grammar.ypp"
    {
    if((yyvsp[(7) - (8)].shaderList)->size() > 0)
        curPass->createState(IPassState::TVertexShader, & (*(yyvsp[(7) - (8)].shaderList))[0], (int)(yyvsp[(7) - (8)].shaderList)->size(), (yyvsp[(4) - (8)].s)->c_str());
    delete (yyvsp[(4) - (8)].s);
    delete (yyvsp[(7) - (8)].shaderList);
    ;}
    break;

  case 216:

/* Line 1455 of yacc.c  */
#line 1516 "Grammar.ypp"
    {
    if((yyvsp[(4) - (5)].shaderList)->size()>0)
        curPass->createState(IPassState::TFragmentShader, & (*(yyvsp[(4) - (5)].shaderList))[0], (int)(yyvsp[(4) - (5)].shaderList)->size());
    delete (yyvsp[(4) - (5)].shaderList);
    ;}
    break;

  case 217:

/* Line 1455 of yacc.c  */
#line 1521 "Grammar.ypp"
    {
    if((yyvsp[(7) - (8)].shaderList)->size()>0)
        curPass->createState(IPassState::TFragmentShader, & (*(yyvsp[(7) - (8)].shaderList))[0], (int)(yyvsp[(7) - (8)].shaderList)->size(), (yyvsp[(4) - (8)].s)->c_str());
    delete (yyvsp[(4) - (8)].s);
    delete (yyvsp[(7) - (8)].shaderList);
    ;}
    break;

  case 218:

/* Line 1455 of yacc.c  */
#line 1527 "Grammar.ypp"
    {
    if((yyvsp[(4) - (5)].shaderList)->size() > 0)
        curPass->createState(IPassState::TComputeShader, & (*(yyvsp[(4) - (5)].shaderList))[0], (int)(yyvsp[(4) - (5)].shaderList)->size());
    delete (yyvsp[(4) - (5)].shaderList);
    ;}
    break;

  case 219:

/* Line 1455 of yacc.c  */
#line 1532 "Grammar.ypp"
    {
    if((yyvsp[(7) - (8)].shaderList)->size() > 0)
        curPass->createState(IPassState::TComputeShader, & (*(yyvsp[(7) - (8)].shaderList))[0], (int)(yyvsp[(7) - (8)].shaderList)->size(), (yyvsp[(4) - (8)].s)->c_str());
    delete (yyvsp[(4) - (8)].s);
    delete (yyvsp[(7) - (8)].shaderList);
    ;}
    break;

  case 220:

/* Line 1455 of yacc.c  */
#line 1538 "Grammar.ypp"
    {
    if((yyvsp[(4) - (5)].shaderList)->size() > 0)
        curPass->createState(IPassState::TPathShader, & (*(yyvsp[(4) - (5)].shaderList))[0], (int)(yyvsp[(4) - (5)].shaderList)->size());
    delete (yyvsp[(4) - (5)].shaderList);
    ;}
    break;

  case 221:

/* Line 1455 of yacc.c  */
#line 1543 "Grammar.ypp"
    {
    if((yyvsp[(7) - (8)].shaderList)->size() > 0)
        curPass->createState(IPassState::TPathShader, & (*(yyvsp[(7) - (8)].shaderList))[0], (int)(yyvsp[(7) - (8)].shaderList)->size(), (yyvsp[(4) - (8)].s)->c_str());
    delete (yyvsp[(4) - (8)].s);
    delete (yyvsp[(7) - (8)].shaderList);
    ;}
    break;

  case 222:

/* Line 1455 of yacc.c  */
#line 1549 "Grammar.ypp"
    {
    if((yyvsp[(4) - (5)].shaderList)->size() > 0)
        curPass->createState(IPassState::TGeometryShader, & (*(yyvsp[(4) - (5)].shaderList))[0], (int)(yyvsp[(4) - (5)].shaderList)->size());
    delete (yyvsp[(4) - (5)].shaderList);
    ;}
    break;

  case 223:

/* Line 1455 of yacc.c  */
#line 1554 "Grammar.ypp"
    {
    if((yyvsp[(7) - (8)].shaderList)->size() > 0)
        curPass->createState(IPassState::TGeometryShader, & (*(yyvsp[(7) - (8)].shaderList))[0], (int)(yyvsp[(7) - (8)].shaderList)->size(), (yyvsp[(4) - (8)].s)->c_str());
    delete (yyvsp[(4) - (8)].s);
    delete (yyvsp[(7) - (8)].shaderList);
    ;}
    break;

  case 224:

/* Line 1455 of yacc.c  */
#line 1560 "Grammar.ypp"
    {
    if((yyvsp[(4) - (5)].shaderList)->size() > 0)
        curPass->createState(IPassState::TTCSShader, & (*(yyvsp[(4) - (5)].shaderList))[0], (int)(yyvsp[(4) - (5)].shaderList)->size());
    delete (yyvsp[(4) - (5)].shaderList);
    ;}
    break;

  case 225:

/* Line 1455 of yacc.c  */
#line 1565 "Grammar.ypp"
    {
    if((yyvsp[(7) - (8)].shaderList)->size() > 0)
        curPass->createState(IPassState::TTCSShader, & (*(yyvsp[(7) - (8)].shaderList))[0], (int)(yyvsp[(7) - (8)].shaderList)->size(), (yyvsp[(4) - (8)].s)->c_str());
    delete (yyvsp[(4) - (8)].s);
    delete (yyvsp[(7) - (8)].shaderList);
    ;}
    break;

  case 226:

/* Line 1455 of yacc.c  */
#line 1571 "Grammar.ypp"
    {
    if((yyvsp[(4) - (5)].shaderList)->size() > 0)
        curPass->createState(IPassState::TTESShader, & (*(yyvsp[(4) - (5)].shaderList))[0], (int)(yyvsp[(4) - (5)].shaderList)->size());
    delete (yyvsp[(4) - (5)].shaderList);
    ;}
    break;

  case 227:

/* Line 1455 of yacc.c  */
#line 1576 "Grammar.ypp"
    {
    if((yyvsp[(7) - (8)].shaderList)->size() > 0)
        curPass->createState(IPassState::TTESShader, & (*(yyvsp[(7) - (8)].shaderList))[0], (int)(yyvsp[(7) - (8)].shaderList)->size(), (yyvsp[(4) - (8)].s)->c_str());
    delete (yyvsp[(4) - (8)].s);
    delete (yyvsp[(7) - (8)].shaderList);
    ;}
    break;

  case 228:

/* Line 1455 of yacc.c  */
#line 1582 "Grammar.ypp"
    {
    if((yyvsp[(4) - (5)].shaderList)->size() > 0)
        curPass->createStateOverride(IPassState::TVertexShader, & (*(yyvsp[(4) - (5)].shaderList))[0], (int)(yyvsp[(4) - (5)].shaderList)->size());
    delete (yyvsp[(4) - (5)].shaderList);
    ;}
    break;

  case 229:

/* Line 1455 of yacc.c  */
#line 1587 "Grammar.ypp"
    {
    if((yyvsp[(7) - (8)].shaderList)->size() > 0)
        curPass->createStateOverride(IPassState::TVertexShader, & (*(yyvsp[(7) - (8)].shaderList))[0], (int)(yyvsp[(7) - (8)].shaderList)->size(), (yyvsp[(4) - (8)].s)->c_str());
    delete (yyvsp[(4) - (8)].s);
    delete (yyvsp[(7) - (8)].shaderList);
    ;}
    break;

  case 230:

/* Line 1455 of yacc.c  */
#line 1593 "Grammar.ypp"
    {
    if((yyvsp[(4) - (5)].shaderList)->size()>0)
        curPass->createStateOverride(IPassState::TFragmentShader, & (*(yyvsp[(4) - (5)].shaderList))[0], (int)(yyvsp[(4) - (5)].shaderList)->size());
    delete (yyvsp[(4) - (5)].shaderList);
    ;}
    break;

  case 231:

/* Line 1455 of yacc.c  */
#line 1598 "Grammar.ypp"
    {
    if((yyvsp[(7) - (8)].shaderList)->size()>0)
        curPass->createStateOverride(IPassState::TFragmentShader, & (*(yyvsp[(7) - (8)].shaderList))[0], (int)(yyvsp[(7) - (8)].shaderList)->size(), (yyvsp[(4) - (8)].s)->c_str());
    delete (yyvsp[(4) - (8)].s);
    delete (yyvsp[(7) - (8)].shaderList);
    ;}
    break;

  case 232:

/* Line 1455 of yacc.c  */
#line 1604 "Grammar.ypp"
    {
    if((yyvsp[(4) - (5)].shaderList)->size() > 0)
        curPass->createStateOverride(IPassState::TComputeShader, & (*(yyvsp[(4) - (5)].shaderList))[0], (int)(yyvsp[(4) - (5)].shaderList)->size());
    delete (yyvsp[(4) - (5)].shaderList);
    ;}
    break;

  case 233:

/* Line 1455 of yacc.c  */
#line 1609 "Grammar.ypp"
    {
    if((yyvsp[(7) - (8)].shaderList)->size() > 0)
        curPass->createStateOverride(IPassState::TComputeShader, & (*(yyvsp[(7) - (8)].shaderList))[0], (int)(yyvsp[(7) - (8)].shaderList)->size(), (yyvsp[(4) - (8)].s)->c_str());
    delete (yyvsp[(4) - (8)].s);
    delete (yyvsp[(7) - (8)].shaderList);
    ;}
    break;

  case 234:

/* Line 1455 of yacc.c  */
#line 1615 "Grammar.ypp"
    {
    if((yyvsp[(4) - (5)].shaderList)->size() > 0)
        curPass->createStateOverride(IPassState::TPathShader, & (*(yyvsp[(4) - (5)].shaderList))[0], (int)(yyvsp[(4) - (5)].shaderList)->size());
    delete (yyvsp[(4) - (5)].shaderList);
    ;}
    break;

  case 235:

/* Line 1455 of yacc.c  */
#line 1620 "Grammar.ypp"
    {
    if((yyvsp[(7) - (8)].shaderList)->size() > 0)
        curPass->createStateOverride(IPassState::TPathShader, & (*(yyvsp[(7) - (8)].shaderList))[0], (int)(yyvsp[(7) - (8)].shaderList)->size(), (yyvsp[(4) - (8)].s)->c_str());
    delete (yyvsp[(4) - (8)].s);
    delete (yyvsp[(7) - (8)].shaderList);
    ;}
    break;

  case 236:

/* Line 1455 of yacc.c  */
#line 1626 "Grammar.ypp"
    {
    if((yyvsp[(4) - (5)].shaderList)->size() > 0)
        curPass->createStateOverride(IPassState::TGeometryShader, & (*(yyvsp[(4) - (5)].shaderList))[0], (int)(yyvsp[(4) - (5)].shaderList)->size());
    delete (yyvsp[(4) - (5)].shaderList);
    ;}
    break;

  case 237:

/* Line 1455 of yacc.c  */
#line 1631 "Grammar.ypp"
    {
    if((yyvsp[(7) - (8)].shaderList)->size() > 0)
        curPass->createStateOverride(IPassState::TGeometryShader, & (*(yyvsp[(7) - (8)].shaderList))[0], (int)(yyvsp[(7) - (8)].shaderList)->size(), (yyvsp[(4) - (8)].s)->c_str());
    delete (yyvsp[(4) - (8)].s);
    delete (yyvsp[(7) - (8)].shaderList);
    ;}
    break;

  case 238:

/* Line 1455 of yacc.c  */
#line 1637 "Grammar.ypp"
    {
    if((yyvsp[(4) - (5)].shaderList)->size() > 0)
        curPass->createStateOverride(IPassState::TTCSShader, & (*(yyvsp[(4) - (5)].shaderList))[0], (int)(yyvsp[(4) - (5)].shaderList)->size());
    delete (yyvsp[(4) - (5)].shaderList);
    ;}
    break;

  case 239:

/* Line 1455 of yacc.c  */
#line 1642 "Grammar.ypp"
    {
    if((yyvsp[(7) - (8)].shaderList)->size() > 0)
        curPass->createStateOverride(IPassState::TTCSShader, & (*(yyvsp[(7) - (8)].shaderList))[0], (int)(yyvsp[(7) - (8)].shaderList)->size(), (yyvsp[(4) - (8)].s)->c_str());
    delete (yyvsp[(4) - (8)].s);
    delete (yyvsp[(7) - (8)].shaderList);
    ;}
    break;

  case 240:

/* Line 1455 of yacc.c  */
#line 1648 "Grammar.ypp"
    {
    if((yyvsp[(4) - (5)].shaderList)->size() > 0)
        curPass->createStateOverride(IPassState::TTESShader, & (*(yyvsp[(4) - (5)].shaderList))[0], (int)(yyvsp[(4) - (5)].shaderList)->size());
    delete (yyvsp[(4) - (5)].shaderList);
    ;}
    break;

  case 241:

/* Line 1455 of yacc.c  */
#line 1653 "Grammar.ypp"
    {
    if((yyvsp[(7) - (8)].shaderList)->size() > 0)
        curPass->createStateOverride(IPassState::TTESShader, & (*(yyvsp[(7) - (8)].shaderList))[0], (int)(yyvsp[(7) - (8)].shaderList)->size(), (yyvsp[(4) - (8)].s)->c_str());
    delete (yyvsp[(4) - (8)].s);
    delete (yyvsp[(7) - (8)].shaderList);
    ;}
    break;

  case 242:

/* Line 1455 of yacc.c  */
#line 1659 "Grammar.ypp"
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
#line 1674 "Grammar.ypp"
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
#line 1683 "Grammar.ypp"
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
#line 1740 "Grammar.ypp"
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
#line 1759 "Grammar.ypp"
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
#line 1787 "Grammar.ypp"
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
#line 1796 "Grammar.ypp"
    {
    curPass->createState((yyvsp[(4) - (13)].s)->c_str(), (yyvsp[(6) - (13)].ival), &(*(yyvsp[(11) - (13)].boolList))[0], (int)(yyvsp[(11) - (13)].boolList)->size());
    delete (yyvsp[(4) - (13)].s);
    delete (yyvsp[(11) - (13)].boolList);
    ;}
    break;

  case 249:

/* Line 1455 of yacc.c  */
#line 1801 "Grammar.ypp"
    {
    yyerror("TODO: SHDPROFILE for HLSL to do...\n");
    delete (yyvsp[(4) - (5)].s);
    ;}
    break;

  case 250:

/* Line 1455 of yacc.c  */
#line 1805 "Grammar.ypp"
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
#line 1822 "Grammar.ypp"
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
#line 1839 "Grammar.ypp"
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
#line 1856 "Grammar.ypp"
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
#line 1873 "Grammar.ypp"
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
#line 1890 "Grammar.ypp"
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
#line 1907 "Grammar.ypp"
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
#line 1924 "Grammar.ypp"
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
#line 1941 "Grammar.ypp"
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
#line 1958 "Grammar.ypp"
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
#line 1975 "Grammar.ypp"
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
#line 1992 "Grammar.ypp"
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
#line 2005 "Grammar.ypp"
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
#line 2018 "Grammar.ypp"
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
#line 2031 "Grammar.ypp"
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
#line 2044 "Grammar.ypp"
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
#line 2057 "Grammar.ypp"
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
#line 2070 "Grammar.ypp"
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
#line 2091 "Grammar.ypp"
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
#line 2112 "Grammar.ypp"
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
#line 2133 "Grammar.ypp"
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
#line 2143 "Grammar.ypp"
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
#line 2153 "Grammar.ypp"
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
#line 2163 "Grammar.ypp"
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
#line 2173 "Grammar.ypp"
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
#line 2183 "Grammar.ypp"
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
#line 2196 "Grammar.ypp"
    {
    curPass->createStateRenderMode((yyvsp[(4) - (5)].render_mode));
;}
    break;

  case 277:

/* Line 1455 of yacc.c  */
#line 2199 "Grammar.ypp"
    {
    curPass->createStateRenderGroup((yyvsp[(4) - (5)].ival));
;}
    break;

  case 278:

/* Line 1455 of yacc.c  */
#line 2202 "Grammar.ypp"
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
#line 2216 "Grammar.ypp"
    {
    curPass->createStateClearMode((yyvsp[(4) - (5)].clear_mode));
;}
    break;

  case 280:

/* Line 1455 of yacc.c  */
#line 2219 "Grammar.ypp"
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
#line 2233 "Grammar.ypp"
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
#line 2243 "Grammar.ypp"
    {
    curPass->createStateClearColor(0, (*(yyvsp[(5) - (7)].floatList))[0], (*(yyvsp[(5) - (7)].floatList))[1], (*(yyvsp[(5) - (7)].floatList))[2], (*(yyvsp[(5) - (7)].floatList))[3]);
    delete (yyvsp[(5) - (7)].floatList);
;}
    break;

  case 283:

/* Line 1455 of yacc.c  */
#line 2247 "Grammar.ypp"
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
#line 2254 "Grammar.ypp"
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
#line 2275 "Grammar.ypp"
    {
    curPass->setLoopCount((yyvsp[(4) - (5)].ival));
;}
    break;

  case 286:

/* Line 1455 of yacc.c  */
#line 2278 "Grammar.ypp"
    {
    curPass->setActive((yyvsp[(4) - (5)].bval));
;}
    break;

  case 287:

/* Line 1455 of yacc.c  */
#line 2284 "Grammar.ypp"
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
#line 2299 "Grammar.ypp"
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
#line 2309 "Grammar.ypp"
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
#line 2355 "Grammar.ypp"
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
#line 2363 "Grammar.ypp"
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
#line 2380 "Grammar.ypp"
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
#line 2400 "Grammar.ypp"
    {
    curPass->bindAttribute((yyvsp[(4) - (8)].s)->c_str(), (yyvsp[(7) - (8)].ival));
    delete (yyvsp[(4) - (8)].s);
;}
    break;

  case 294:

/* Line 1455 of yacc.c  */
#line 2404 "Grammar.ypp"
    { 
    yyerror("Error in a state of the Pass\n");
;}
    break;

  case 295:

/* Line 1455 of yacc.c  */
#line 2413 "Grammar.ypp"
    {
    //printf("Creation of PASS\n");
    curPass = curTechnique->addPass()->getExInterface();
    curAnnotations = curPass->annotations()->getExInterface();
    ;}
    break;

  case 296:

/* Line 1455 of yacc.c  */
#line 2418 "Grammar.ypp"
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
#line 2427 "Grammar.ypp"
    {
    //printf("Creation of PASS %s\n", $2->c_str());
    curPass = curTechnique->addPass((yyvsp[(2) - (2)].s)->c_str())->getExInterface();
    curAnnotations = curPass->annotations()->getExInterface();
    ;}
    break;

  case 298:

/* Line 1455 of yacc.c  */
#line 2432 "Grammar.ypp"
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
#line 2445 "Grammar.ypp"
    {
    curTechnique->bindAttribute((yyvsp[(3) - (7)].s)->c_str(), (yyvsp[(6) - (7)].ival));
    delete (yyvsp[(3) - (7)].s);
;}
    break;

  case 300:

/* Line 1455 of yacc.c  */
#line 2449 "Grammar.ypp"
    {
    curTechnique->bindAttribute((yyvsp[(3) - (7)].s)->c_str(), (yyvsp[(6) - (7)].ival));
    delete (yyvsp[(3) - (7)].s);
;}
    break;

  case 301:

/* Line 1455 of yacc.c  */
#line 2453 "Grammar.ypp"
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
#line 2464 "Grammar.ypp"
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
#line 2484 "Grammar.ypp"
    { yyerror("Error in Pass list\n");;}
    break;

  case 306:

/* Line 1455 of yacc.c  */
#line 2490 "Grammar.ypp"
    {
    //printf("Creation of Technique for NO name\n");
    curTechnique = curContainer->createTechnique()->getExInterface();
    curAnnotations = curTechnique->annotations()->getExInterface();
;}
    break;

  case 307:

/* Line 1455 of yacc.c  */
#line 2494 "Grammar.ypp"
    { 
    curAnnotations = NULL;
    lex_pop_state();
;}
    break;

  case 308:

/* Line 1455 of yacc.c  */
#line 2498 "Grammar.ypp"
    {
    LOGD("creation of Technique %s...\n", (yyvsp[(2) - (2)].s)->c_str() );
    curTechnique = curContainer->createTechnique((yyvsp[(2) - (2)].s)->c_str())->getExInterface();
    curAnnotations = curTechnique->annotations()->getExInterface();
    delete (yyvsp[(2) - (2)].s);
;}
    break;

  case 309:

/* Line 1455 of yacc.c  */
#line 2503 "Grammar.ypp"
    { 
    lex_pop_state();
    curAnnotations = NULL;
;}
    break;

  case 310:

/* Line 1455 of yacc.c  */
#line 2513 "Grammar.ypp"
    { (yyval.bval)=false; ;}
    break;

  case 311:

/* Line 1455 of yacc.c  */
#line 2514 "Grammar.ypp"
    { (yyval.bval)=true; ;}
    break;

  case 312:

/* Line 1455 of yacc.c  */
#line 2519 "Grammar.ypp"
    { (yyval.bval)=true; ;}
    break;

  case 313:

/* Line 1455 of yacc.c  */
#line 2520 "Grammar.ypp"
    { (yyval.bval)=false; ;}
    break;

  case 314:

/* Line 1455 of yacc.c  */
#line 2521 "Grammar.ypp"
    { (yyval.bval)=true; ;}
    break;

  case 315:

/* Line 1455 of yacc.c  */
#line 2529 "Grammar.ypp"
    {
    if(!namespaceName.empty())
        (*(yyvsp[(1) - (1)].s)) = namespaceName + std::string("_NS_") + (*(yyvsp[(1) - (1)].s));
    (yyval.s) = (yyvsp[(1) - (1)].s);
;}
    break;

  case 316:

/* Line 1455 of yacc.c  */
#line 2538 "Grammar.ypp"
    { (yyval.global_varname_annots).global = false; (yyval.global_varname_annots).varName = NULL; ;}
    break;

  case 317:

/* Line 1455 of yacc.c  */
#line 2539 "Grammar.ypp"
    { (yyval.global_varname_annots).global = false; (yyval.global_varname_annots).varName = (yyvsp[(1) - (2)].s); ;}
    break;

  case 318:

/* Line 1455 of yacc.c  */
#line 2541 "Grammar.ypp"
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
#line 2553 "Grammar.ypp"
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
#line 2577 "Grammar.ypp"
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
#line 2598 "Grammar.ypp"
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
#line 2619 "Grammar.ypp"
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
#line 2657 "Grammar.ypp"
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
#line 2695 "Grammar.ypp"
    { (yyval.bval) = false; ;}
    break;

  case 325:

/* Line 1455 of yacc.c  */
#line 2696 "Grammar.ypp"
    { (yyval.bval) = true; ;}
    break;

  case 326:

/* Line 1455 of yacc.c  */
#line 2698 "Grammar.ypp"
    { (yyval.bval) = false; ;}
    break;

  case 327:

/* Line 1455 of yacc.c  */
#line 2699 "Grammar.ypp"
    { (yyval.bval) = true; ;}
    break;

  case 328:

/* Line 1455 of yacc.c  */
#line 2704 "Grammar.ypp"
    {
    (yyval.argument) = new Argument((yyvsp[(2) - (3)].bval) ? nvFX::TFloatPtr : nvFX::TFloat, (yyvsp[(3) - (3)].s));
    LOGD("Argument %s %s\n", (yyvsp[(2) - (3)].bval) ? "TFloatPtr" : "TFloat", (yyvsp[(3) - (3)].s)->c_str());
    delete (yyvsp[(3) - (3)].s);
;}
    break;

  case 329:

/* Line 1455 of yacc.c  */
#line 2709 "Grammar.ypp"
    {
    (yyval.argument) = new Argument((yyvsp[(1) - (4)].bval) ? ((yyvsp[(3) - (4)].bval) ? nvFX::TUIntPtr : nvFX::TUInt) : ((yyvsp[(3) - (4)].bval) ? nvFX::TIntPtr : nvFX::TInt), (yyvsp[(4) - (4)].s));
    LOGD("Argument %s %s\n", (yyvsp[(1) - (4)].bval) ? ((yyvsp[(3) - (4)].bval) ? "TUIntPtr" : "TUInt") : ((yyvsp[(3) - (4)].bval) ? "TIntPtr" : "TInt"), (yyvsp[(4) - (4)].s)->c_str() );
    delete (yyvsp[(4) - (4)].s);
;}
    break;

  case 330:

/* Line 1455 of yacc.c  */
#line 2718 "Grammar.ypp"
    { (yyval.argList) = new ArgList; ;}
    break;

  case 331:

/* Line 1455 of yacc.c  */
#line 2719 "Grammar.ypp"
    { (yyval.argList) = new ArgList; ;}
    break;

  case 332:

/* Line 1455 of yacc.c  */
#line 2720 "Grammar.ypp"
    {
    (yyval.argList) = new ArgList;
    (yyval.argList)->push_back((yyvsp[(1) - (1)].argument));
    ;}
    break;

  case 333:

/* Line 1455 of yacc.c  */
#line 2724 "Grammar.ypp"
    {
    (yyval.argList) = (yyvsp[(1) - (3)].argList);
    (yyval.argList)->push_back((yyvsp[(3) - (3)].argument));
    ;}
    break;

  case 334:

/* Line 1455 of yacc.c  */
#line 2732 "Grammar.ypp"
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
#line 2792 "Grammar.ypp"
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
#line 2834 "Grammar.ypp"
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
#line 2862 "Grammar.ypp"
    { 
    yyerror("Error in texture params declaration\n"); 
;}
    break;

  case 339:

/* Line 1455 of yacc.c  */
#line 2869 "Grammar.ypp"
    { (yyval.resourceType)=nvFX::RESOURCE_UNKNOWN; ;}
    break;

  case 340:

/* Line 1455 of yacc.c  */
#line 2870 "Grammar.ypp"
    { (yyval.resourceType)=nvFX::RESTEX_1D; ;}
    break;

  case 341:

/* Line 1455 of yacc.c  */
#line 2871 "Grammar.ypp"
    { (yyval.resourceType)=nvFX::RESTEX_2D; ;}
    break;

  case 342:

/* Line 1455 of yacc.c  */
#line 2872 "Grammar.ypp"
    { (yyval.resourceType)=nvFX::RESTEX_3D; ;}
    break;

  case 343:

/* Line 1455 of yacc.c  */
#line 2873 "Grammar.ypp"
    { (yyval.resourceType)=nvFX::RESTEX_2DRECT; ;}
    break;

  case 344:

/* Line 1455 of yacc.c  */
#line 2874 "Grammar.ypp"
    { (yyval.resourceType)=nvFX::RESTEX_CUBE_MAP; ;}
    break;

  case 345:

/* Line 1455 of yacc.c  */
#line 2876 "Grammar.ypp"
    {
    curResource = curContainer->createResource((yyvsp[(2) - (3)].s)->c_str(), (yyvsp[(1) - (3)].resourceType), true/*ADD AS nvFX token*/)->getExInterface();
    curResource->setGLTexture(0); // this will tell the resource it is made of some external resource (reference to an existing texture)
    lex_pop_state();
    delete (yyvsp[(2) - (3)].s);
    ;}
    break;

  case 346:

/* Line 1455 of yacc.c  */
#line 2882 "Grammar.ypp"
    {
    curResource = curContainer->createResource((yyvsp[(2) - (2)].s)->c_str(), (yyvsp[(1) - (2)].resourceType), true/*ADD AS nvFX token*/)->getExInterface();
    curResource->setGLTexture(0); // this will tell the resource it is made of some external resource (reference to an existing texture)
    curAnnotations = curResource->annotations()->getExInterface();
    delete (yyvsp[(2) - (2)].s);
;}
    break;

  case 347:

/* Line 1455 of yacc.c  */
#line 2887 "Grammar.ypp"
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
#line 2899 "Grammar.ypp"
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
#line 2909 "Grammar.ypp"
    {
    if(curResource)
        if(!curResource->annotations()->getExInterface()->addAnnotation((yyvsp[(2) - (5)].s)->c_str(), (yyvsp[(4) - (5)].s)->c_str()))
            yyerror("err\n");
    delete (yyvsp[(4) - (5)].s);
    ;}
    break;

  case 351:

/* Line 1455 of yacc.c  */
#line 2915 "Grammar.ypp"
    {
    if(curResource)
        curResource->setMSAAApplicationDependent();
;}
    break;

  case 352:

/* Line 1455 of yacc.c  */
#line 2919 "Grammar.ypp"
    {
    if(curResource)
        curResource->setMSAA((*(yyvsp[(5) - (7)].intList))[0], (yyvsp[(5) - (7)].intList)->size() > 1 ? (*(yyvsp[(5) - (7)].intList))[1] : 0);
    delete (yyvsp[(5) - (7)].intList);
;}
    break;

  case 353:

/* Line 1455 of yacc.c  */
#line 2924 "Grammar.ypp"
    {
    if(curResource)
        curResource->setMSAA((yyvsp[(4) - (5)].ival));
;}
    break;

  case 354:

/* Line 1455 of yacc.c  */
#line 2928 "Grammar.ypp"
    {
    if(curResource)
        curResource->setDimensionsApplicationDependent();
;}
    break;

  case 355:

/* Line 1455 of yacc.c  */
#line 2932 "Grammar.ypp"
    {
    int d = (int)(yyvsp[(5) - (7)].intList)->size();
    if(curResource)
        curResource->setDimensions((*(yyvsp[(5) - (7)].intList))[0], d > 1 ? (*(yyvsp[(5) - (7)].intList))[1] : 1, d > 2 ? (*(yyvsp[(5) - (7)].intList))[2] : 1);
    delete (yyvsp[(5) - (7)].intList);
;}
    break;

  case 356:

/* Line 1455 of yacc.c  */
#line 2938 "Grammar.ypp"
    {
    if(curResource)
        curResource->setDimensionFactor((yyvsp[(4) - (5)].fval), (yyvsp[(4) - (5)].fval));
;}
    break;

  case 357:

/* Line 1455 of yacc.c  */
#line 2942 "Grammar.ypp"
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
#line 2955 "Grammar.ypp"
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
#line 2967 "Grammar.ypp"
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
#line 2974 "Grammar.ypp"
    {
    // for the case of items within a namespace :
    // copy the name-space annotations to this one
    if(curAnnotations) curAnnotations->copy(namespaceAnnotations);
;}
    break;

  case 361:

/* Line 1455 of yacc.c  */
#line 2978 "Grammar.ypp"
    { 
    curAnnotations = NULL;
    lex_pop_state(); lex_pop_state(); 
;}
    break;

  case 362:

/* Line 1455 of yacc.c  */
#line 2982 "Grammar.ypp"
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
#line 2989 "Grammar.ypp"
    {
    // for the case of items within a namespace :
    // copy the name-space annotations to this one
    if(curAnnotations) curAnnotations->copy(namespaceAnnotations);
;}
    break;

  case 364:

/* Line 1455 of yacc.c  */
#line 2993 "Grammar.ypp"
    { 
    curAnnotations = NULL;
    lex_pop_state(); lex_pop_state(); 
;}
    break;

  case 365:

/* Line 1455 of yacc.c  */
#line 2997 "Grammar.ypp"
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
#line 3004 "Grammar.ypp"
    {
    // for the case of items within a namespace :
    // copy the name-space annotations to this one
    if(curAnnotations) curAnnotations->copy(namespaceAnnotations);
;}
    break;

  case 367:

/* Line 1455 of yacc.c  */
#line 3008 "Grammar.ypp"
    { 
    curAnnotations = NULL;
    lex_pop_state(); lex_pop_state(); 
;}
    break;

  case 369:

/* Line 1455 of yacc.c  */
#line 3017 "Grammar.ypp"
    {
    if(curFBO)
      if(!curFBO->annotations()->getExInterface()->addAnnotation((yyvsp[(2) - (5)].s)->c_str(), (yyvsp[(4) - (5)].s)->c_str()))
        yyerror("err\n");
    delete (yyvsp[(4) - (5)].s);
    ;}
    break;

  case 370:

/* Line 1455 of yacc.c  */
#line 3026 "Grammar.ypp"
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
#line 3040 "Grammar.ypp"
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
#line 3061 "Grammar.ypp"
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
#line 3078 "Grammar.ypp"
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
#line 3085 "Grammar.ypp"
    {
    // for the case of items within a namespace :
    // copy the name-space annotations to this one
    if(curAnnotations) curAnnotations->copy(namespaceAnnotations);
;}
    break;

  case 375:

/* Line 1455 of yacc.c  */
#line 3089 "Grammar.ypp"
    { 
    curAnnotations = NULL;
    lex_pop_state(); lex_pop_state(); // fbo and then fbo1 state
;}
    break;

  case 382:

/* Line 1455 of yacc.c  */
#line 3104 "Grammar.ypp"
    { yyerror("Error in Sampler State filtering\n");;}
    break;

  case 387:

/* Line 1455 of yacc.c  */
#line 3111 "Grammar.ypp"
    { yyerror("Error in Sampler State UV Addressing\n");;}
    break;

  case 389:

/* Line 1455 of yacc.c  */
#line 3119 "Grammar.ypp"
    {
    curSamplerState->getExInterface()->createState(getGLEnumValue((yyvsp[(2) - (5)].glenum)), getGLEnumValue((yyvsp[(4) - (5)].glenum)));
;}
    break;

  case 390:

/* Line 1455 of yacc.c  */
#line 3122 "Grammar.ypp"
    {
    curSamplerState->getExInterface()->createState(getGLEnumValue((yyvsp[(2) - (5)].glenum)), getGLEnumValue((yyvsp[(4) - (5)].glenum)));
;}
    break;

  case 391:

/* Line 1455 of yacc.c  */
#line 3125 "Grammar.ypp"
    {
    curSamplerState->getExInterface()->createState(getGLEnumValue((yyvsp[(2) - (5)].glenum)), getGLEnumValue((yyvsp[(4) - (5)].glenum)));
;}
    break;

  case 392:

/* Line 1455 of yacc.c  */
#line 3128 "Grammar.ypp"
    {
    curSamplerState->getExInterface()->createState(getGLEnumValue((yyvsp[(2) - (5)].glenum)), getGLEnumValue((yyvsp[(4) - (5)].glenum)));
;}
    break;

  case 393:

/* Line 1455 of yacc.c  */
#line 3131 "Grammar.ypp"
    {
    curSamplerState->getExInterface()->createState(getGLEnumValue((yyvsp[(2) - (5)].glenum)), getGLEnumValue((yyvsp[(4) - (5)].glenum)));
;}
    break;

  case 394:

/* Line 1455 of yacc.c  */
#line 3134 "Grammar.ypp"
    { yyerror("Error in one of sampler-state states\n"); ;}
    break;

  case 395:

/* Line 1455 of yacc.c  */
#line 3139 "Grammar.ypp"
    {
    LOGD("Sampler-state %s\n", (yyvsp[(2) - (2)].s)->c_str() );
    curSamplerState = curContainer->createSamplerState((yyvsp[(2) - (2)].s)->c_str())->getExInterface();
    curAnnotations = curSamplerState->annotations()->getExInterface();
    delete (yyvsp[(2) - (2)].s);
;}
    break;

  case 396:

/* Line 1455 of yacc.c  */
#line 3144 "Grammar.ypp"
    { 
    // for the case of items within a namespace :
    // copy the name-space annotations to this one
    if(curAnnotations) curAnnotations->copy(namespaceAnnotations);
    lex_pop_state(); curAnnotations = NULL;;}
    break;

  case 397:

/* Line 1455 of yacc.c  */
#line 3149 "Grammar.ypp"
    { 
    yyerror("Error sampler-state declaration\n"); 
;}
    break;

  case 398:

/* Line 1455 of yacc.c  */
#line 3164 "Grammar.ypp"
    { check_curRState(); if(!curRState->createState(getGLEnumValue((yyvsp[(1) - (4)].glenum)), ((yyvsp[(3) - (4)].fval)))) yyerror("Failed to create state!\n"); ;}
    break;

  case 399:

/* Line 1455 of yacc.c  */
#line 3165 "Grammar.ypp"
    { check_curRState(); if(!curRState->createState(getGLEnumValue((yyvsp[(1) - (4)].glenum)), (GLenum)(((yyvsp[(3) - (4)].bval) )? GL_TRUE: GL_FALSE))) yyerror("Failed to create state!\n"); ;}
    break;

  case 400:

/* Line 1455 of yacc.c  */
#line 3166 "Grammar.ypp"
    { check_curRState(); if(!curRState->createState(getGLEnumValue((yyvsp[(1) - (4)].glenum)), (GLenum)(((yyvsp[(3) - (4)].bval) )? GL_TRUE: GL_FALSE))) yyerror("Failed to create state!\n"); ;}
    break;

  case 401:

/* Line 1455 of yacc.c  */
#line 3167 "Grammar.ypp"
    { check_curRState(); if(!curRState->createState(getGLEnumValue((yyvsp[(1) - (4)].glenum)), getGLEnumValue((yyvsp[(3) - (4)].glenum)))) yyerror("Failed to create state!\n"); ;}
    break;

  case 402:

/* Line 1455 of yacc.c  */
#line 3168 "Grammar.ypp"
    { check_curRState(); if(!curRState->createState(getGLEnumValue((yyvsp[(1) - (4)].glenum)), ((yyvsp[(3) - (4)].fval)))) yyerror("Failed to create state!\n"); ;}
    break;

  case 403:

/* Line 1455 of yacc.c  */
#line 3169 "Grammar.ypp"
    { check_curRState(); if(!curRState->createState(getGLEnumValue((yyvsp[(1) - (4)].glenum)), (GLenum)(((yyvsp[(3) - (4)].bval) )? GL_TRUE: GL_FALSE))) yyerror("Failed to create state!\n"); ;}
    break;

  case 404:

/* Line 1455 of yacc.c  */
#line 3171 "Grammar.ypp"
    { check_curRState(); if(!curRState->createState(getGLEnumValue((yyvsp[(1) - (4)].glenum)), (unsigned int)(yyvsp[(3) - (4)].ival))) yyerror("Failed to create state!\n"); ;}
    break;

  case 405:

/* Line 1455 of yacc.c  */
#line 3172 "Grammar.ypp"
    { check_curRState(); if(!curRState->createState(getGLEnumValue((yyvsp[(1) - (4)].glenum)), (unsigned int)(yyvsp[(3) - (4)].ival))) yyerror("Failed to create state!\n"); ;}
    break;

  case 406:

/* Line 1455 of yacc.c  */
#line 3173 "Grammar.ypp"
    { check_curRState(); if(!curRState->createState(getGLEnumValue((yyvsp[(1) - (4)].glenum)), (GLenum)((yyvsp[(3) - (4)].bval) ? GL_TRUE: GL_FALSE))) yyerror("Failed to create state!\n"); ;}
    break;

  case 407:

/* Line 1455 of yacc.c  */
#line 3174 "Grammar.ypp"
    { check_curRState(); if(!curRState->createState(getGLEnumValue((yyvsp[(1) - (4)].glenum)), (GLenum)((yyvsp[(3) - (4)].bval) ? GL_TRUE: GL_FALSE))) yyerror("Failed to create state!\n"); ;}
    break;

  case 408:

/* Line 1455 of yacc.c  */
#line 3175 "Grammar.ypp"
    { check_curRState(); if(!curRState->createState(getGLEnumValue((yyvsp[(1) - (4)].glenum)), getGLEnumValue((yyvsp[(3) - (4)].glenum)))) yyerror("Failed to create state!\n"); ;}
    break;

  case 409:

/* Line 1455 of yacc.c  */
#line 3176 "Grammar.ypp"
    { check_curRState(); if(!curRState->createState(getGLEnumValue((yyvsp[(1) - (4)].glenum)), getGLEnumValue((yyvsp[(3) - (4)].glenum)))) yyerror("Failed to create state!\n"); ;}
    break;

  case 410:

/* Line 1455 of yacc.c  */
#line 3177 "Grammar.ypp"
    { check_curRState(); if(!curRState->createState(getGLEnumValue((yyvsp[(1) - (4)].glenum)), getGLEnumValue((yyvsp[(3) - (4)].glenum)))) yyerror("Failed to create state!\n"); ;}
    break;

  case 411:

/* Line 1455 of yacc.c  */
#line 3178 "Grammar.ypp"
    { check_curRState(); if(!curRState->createState(getGLEnumValue((yyvsp[(1) - (4)].glenum)), (GLenum)((yyvsp[(3) - (4)].bval) ? GL_TRUE: GL_FALSE))) yyerror("Failed to create state!\n"); ;}
    break;

  case 412:

/* Line 1455 of yacc.c  */
#line 3179 "Grammar.ypp"
    { check_curRState(); if(!curRState->createState(getGLEnumValue((yyvsp[(1) - (8)].glenum)), getGLEnumValue((yyvsp[(4) - (8)].glenum)), getGLEnumValue((yyvsp[(6) - (8)].glenum)))) yyerror("Failed to create state!\n"); ;}
    break;

  case 413:

/* Line 1455 of yacc.c  */
#line 3180 "Grammar.ypp"
    { check_curRState(); if(!curRState->createState(getGLEnumValue((yyvsp[(1) - (4)].glenum)), (yyvsp[(3) - (4)].fval))) yyerror("Failed to create state!\n"); ;}
    break;

  case 414:

/* Line 1455 of yacc.c  */
#line 3181 "Grammar.ypp"
    { check_curRState(); if(!curRState->createState(getGLEnumValue((yyvsp[(1) - (4)].glenum)), (yyvsp[(3) - (4)].fval))) yyerror("Failed to create state!\n"); ;}
    break;

  case 415:

/* Line 1455 of yacc.c  */
#line 3182 "Grammar.ypp"
    {
    check_curRState();
    if(!curRState->createState(getGLEnumValue(GLPOLYGONOFFSETFACTOR), (yyvsp[(4) - (8)].fval))) yyerror("Failed to create state!\n");
    if(!curRState->createState(getGLEnumValue(GLPOLYGONOFFSETUNITS), (yyvsp[(6) - (8)].fval))) yyerror("Failed to create state!\n");
;}
    break;

  case 416:

/* Line 1455 of yacc.c  */
#line 3187 "Grammar.ypp"
    { check_curRState(); if(!curRState->createState(getGLEnumValue((yyvsp[(1) - (4)].glenum)), (GLenum)((yyvsp[(3) - (4)].bval) ? GL_TRUE: GL_FALSE))) yyerror("Failed to create state!\n"); ;}
    break;

  case 417:

/* Line 1455 of yacc.c  */
#line 3188 "Grammar.ypp"
    { check_curRState(); if(!curRState->createState(getGLEnumValue((yyvsp[(1) - (4)].glenum)), (GLenum)((yyvsp[(3) - (4)].bval) ? GL_TRUE: GL_FALSE))) yyerror("Failed to create state!\n"); ;}
    break;

  case 418:

/* Line 1455 of yacc.c  */
#line 3189 "Grammar.ypp"
    { check_curRState(); if(!curRState->createState(getGLEnumValue((yyvsp[(1) - (4)].glenum)), (GLenum)((yyvsp[(3) - (4)].bval) ? GL_TRUE: GL_FALSE))) yyerror("Failed to create state!\n"); ;}
    break;

  case 419:

/* Line 1455 of yacc.c  */
#line 3191 "Grammar.ypp"
    { check_curRState(); if(!curRState->createState(getGLEnumValue((yyvsp[(1) - (4)].glenum)), (GLenum)((yyvsp[(3) - (4)].bval) ? GL_TRUE: GL_FALSE))) yyerror("Failed to create state!\n"); ;}
    break;

  case 420:

/* Line 1455 of yacc.c  */
#line 3192 "Grammar.ypp"
    { check_curCSState(); if(!curCSState->createState(getGLEnumValue((yyvsp[(1) - (4)].glenum)), (GLenum)((yyvsp[(3) - (4)].bval) ? GL_TRUE: GL_FALSE))) yyerror("Failed to create state!\n"); ;}
    break;

  case 421:

/* Line 1455 of yacc.c  */
#line 3193 "Grammar.ypp"
    { check_curCSState(); if(!curCSState->createState(getGLEnumValue((yyvsp[(1) - (4)].glenum)), getGLEnumValue((yyvsp[(3) - (4)].glenum)))) yyerror("Failed to create state!\n"); ;}
    break;

  case 422:

/* Line 1455 of yacc.c  */
#line 3194 "Grammar.ypp"
    { check_curCSState(); if(!curCSState->createState(getGLEnumValue((yyvsp[(1) - (4)].glenum)), (yyvsp[(3) - (4)].fval))) yyerror("Failed to create state!\n"); ;}
    break;

  case 423:

/* Line 1455 of yacc.c  */
#line 3195 "Grammar.ypp"
    {
    check_curCSState();
    if(!curCSState->createState(getGLEnumValue(GLALPHATESTFUNC), getGLEnumValue((yyvsp[(4) - (8)].glenum)))) yyerror("Failed to create state!\n");
    if(!curCSState->createState(getGLEnumValue(GLALPHATESTREF), (yyvsp[(6) - (8)].fval))) yyerror("Failed to create state!\n");
;}
    break;

  case 424:

/* Line 1455 of yacc.c  */
#line 3200 "Grammar.ypp"
    { check_curCSState(); if(!curCSState->createState(getGLEnumValue((yyvsp[(1) - (4)].glenum)), (GLenum)((yyvsp[(3) - (4)].bval) ? GL_TRUE: GL_FALSE))) yyerror("Failed to create state!\n"); ;}
    break;

  case 425:

/* Line 1455 of yacc.c  */
#line 3201 "Grammar.ypp"
    {
    check_curCSState();
    if(!curCSState->createState(getGLEnumValue(GLBLENDSRC), getGLEnumValue((yyvsp[(4) - (8)].glenum)))) yyerror("Failed to create state!\n");
    if(!curCSState->createState(getGLEnumValue(GLBLENDDST), getGLEnumValue((yyvsp[(6) - (8)].glenum)))) yyerror("Failed to create state!\n");
;}
    break;

  case 426:

/* Line 1455 of yacc.c  */
#line 3206 "Grammar.ypp"
    {
    check_curCSState();
    if(!curCSState->createState(getGLEnumValue(GLBLENDSRCRGB), getGLEnumValue((yyvsp[(4) - (12)].glenum)))) yyerror("Failed to create state!\n");
    if(!curCSState->createState(getGLEnumValue(GLBLENDDSTRGB), getGLEnumValue((yyvsp[(6) - (12)].glenum)))) yyerror("Failed to create state!\n");
    if(!curCSState->createState(getGLEnumValue(GLBLENDSRCALPHA), getGLEnumValue((yyvsp[(8) - (12)].glenum)))) yyerror("Failed to create state!\n");
    if(!curCSState->createState(getGLEnumValue(GLBLENDDSTALPHA), getGLEnumValue((yyvsp[(10) - (12)].glenum)))) yyerror("Failed to create state!\n");
;}
    break;

  case 427:

/* Line 1455 of yacc.c  */
#line 3213 "Grammar.ypp"
    { check_curCSState(); if(!curCSState->createState(getGLEnumValue((yyvsp[(1) - (4)].glenum)), getGLEnumValue((yyvsp[(3) - (4)].glenum)))) yyerror("Failed to create state!\n"); ;}
    break;

  case 428:

/* Line 1455 of yacc.c  */
#line 3214 "Grammar.ypp"
    { check_curCSState(); if(!curCSState->createState(getGLEnumValue((yyvsp[(1) - (4)].glenum)), getGLEnumValue((yyvsp[(3) - (4)].glenum)))) yyerror("Failed to create state!\n"); ;}
    break;

  case 429:

/* Line 1455 of yacc.c  */
#line 3215 "Grammar.ypp"
    { check_curCSState(); if(!curCSState->createState(getGLEnumValue((yyvsp[(1) - (4)].glenum)), getGLEnumValue((yyvsp[(3) - (4)].glenum)))) yyerror("Failed to create state!\n"); ;}
    break;

  case 430:

/* Line 1455 of yacc.c  */
#line 3216 "Grammar.ypp"
    { check_curCSState(); if(!curCSState->createState(getGLEnumValue((yyvsp[(1) - (4)].glenum)), getGLEnumValue((yyvsp[(3) - (4)].glenum)))) yyerror("Failed to create state!\n"); ;}
    break;

  case 431:

/* Line 1455 of yacc.c  */
#line 3217 "Grammar.ypp"
    { check_curCSState(); if(!curCSState->createState(getGLEnumValue((yyvsp[(1) - (4)].glenum)), getGLEnumValue((yyvsp[(3) - (4)].glenum)))) yyerror("Failed to create state!\n"); ;}
    break;

  case 432:

/* Line 1455 of yacc.c  */
#line 3218 "Grammar.ypp"
    { check_curCSState(); if(!curCSState->createState(getGLEnumValue((yyvsp[(1) - (4)].glenum)), getGLEnumValue((yyvsp[(3) - (4)].glenum)))) yyerror("Failed to create state!\n"); ;}
    break;

  case 433:

/* Line 1455 of yacc.c  */
#line 3220 "Grammar.ypp"
    { check_curCSState(); if(!curCSState->createState(getGLEnumValue((yyvsp[(1) - (4)].glenum)), getGLEnumValue((yyvsp[(3) - (4)].glenum)))) yyerror("Failed to create state!\n"); ;}
    break;

  case 434:

/* Line 1455 of yacc.c  */
#line 3221 "Grammar.ypp"
    { check_curCSState(); if(!curCSState->createState(getGLEnumValue((yyvsp[(1) - (4)].glenum)), getGLEnumValue((yyvsp[(3) - (4)].glenum)))) yyerror("Failed to create state!\n"); ;}
    break;

  case 435:

/* Line 1455 of yacc.c  */
#line 3222 "Grammar.ypp"
    {
    check_curCSState();
    if(!curCSState->createState(getGLEnumValue(GLBLENDEQUATIONRGB), getGLEnumValue((yyvsp[(3) - (4)].glenum)))) yyerror("Failed to create state!\n");
    if(!curCSState->createState(getGLEnumValue(GLBLENDEQUATIONALPHA), getGLEnumValue((yyvsp[(3) - (4)].glenum)))) yyerror("Failed to create state!\n");
;}
    break;

  case 436:

/* Line 1455 of yacc.c  */
#line 3227 "Grammar.ypp"
    {
    check_curCSState();
    if(!curCSState->createState(getGLEnumValue(GLBLENDEQUATIONRGB), getGLEnumValue((yyvsp[(4) - (8)].glenum)))) yyerror("Failed to create state!\n");
    if(!curCSState->createState(getGLEnumValue(GLBLENDEQUATIONALPHA), getGLEnumValue((yyvsp[(6) - (8)].glenum)))) yyerror("Failed to create state!\n");
;}
    break;

  case 437:

/* Line 1455 of yacc.c  */
#line 3237 "Grammar.ypp"
    { check_curCSState(); if(!curCSState->createState(getGLEnumValue((yyvsp[(1) - (12)].glenum)), (yyvsp[(4) - (12)].fval), (yyvsp[(6) - (12)].fval), (yyvsp[(8) - (12)].fval), (yyvsp[(10) - (12)].fval))) yyerror("Failed to create state!\n"); ;}
    break;

  case 438:

/* Line 1455 of yacc.c  */
#line 3238 "Grammar.ypp"
    { check_curCSState(); if(!curCSState->createState(getGLEnumValue((yyvsp[(1) - (4)].glenum)), (GLenum)((yyvsp[(3) - (4)].bval) ? GL_TRUE: GL_FALSE))) yyerror("Failed to create state!\n"); ;}
    break;

  case 439:

/* Line 1455 of yacc.c  */
#line 3239 "Grammar.ypp"
    { check_curCSState(); if(!curCSState->createState(getGLEnumValue((yyvsp[(1) - (4)].glenum)), (GLenum)((yyvsp[(3) - (4)].bval) ? GL_TRUE: GL_FALSE))) yyerror("Failed to create state!\n"); ;}
    break;

  case 440:

/* Line 1455 of yacc.c  */
#line 3240 "Grammar.ypp"
    { check_curCSState(); if(!curCSState->createState(getGLEnumValue((yyvsp[(1) - (4)].glenum)), (GLenum)((yyvsp[(3) - (4)].bval) ? GL_TRUE: GL_FALSE))) yyerror("Failed to create state!\n"); ;}
    break;

  case 441:

/* Line 1455 of yacc.c  */
#line 3241 "Grammar.ypp"
    { check_curCSState(); if(!curCSState->createState(getGLEnumValue((yyvsp[(1) - (4)].glenum)), getGLEnumValue((yyvsp[(3) - (4)].glenum)))) yyerror("Failed to create state!\n"); ;}
    break;

  case 442:

/* Line 1455 of yacc.c  */
#line 3243 "Grammar.ypp"
    { check_curCSState(); if(!curCSState->createState(getGLEnumValue((yyvsp[(1) - (12)].glenum)), 
    (GLboolean)((yyvsp[(4) - (12)].bval) ? GL_TRUE: GL_FALSE),
    (GLboolean)((yyvsp[(6) - (12)].bval) ? GL_TRUE: GL_FALSE),
    (GLboolean)((yyvsp[(8) - (12)].bval) ? GL_TRUE: GL_FALSE),
    (GLboolean)((yyvsp[(10) - (12)].bval) ? GL_TRUE: GL_FALSE)
    )) yyerror("Failed to create state!\n"); ;}
    break;

  case 443:

/* Line 1455 of yacc.c  */
#line 3249 "Grammar.ypp"
    { check_curCSState(); if(!curCSState->createState(getGLEnumValue((yyvsp[(1) - (4)].glenum)), (GLenum)((yyvsp[(3) - (4)].bval) ? GL_TRUE: GL_FALSE))) yyerror("Failed to create state!\n"); ;}
    break;

  case 444:

/* Line 1455 of yacc.c  */
#line 3250 "Grammar.ypp"
    { check_curCSState(); if(!curCSState->createState(getGLEnumValue((yyvsp[(1) - (4)].glenum)), (GLenum)((yyvsp[(3) - (4)].bval) ? GL_TRUE: GL_FALSE))) yyerror("Failed to create state!\n"); ;}
    break;

  case 445:

/* Line 1455 of yacc.c  */
#line 3252 "Grammar.ypp"
    { check_curCSState(); if(!curCSState->createState(getGLEnumValue((yyvsp[(1) - (4)].glenum)), (GLenum)((yyvsp[(3) - (4)].bval) ? GL_TRUE: GL_FALSE))) yyerror("Failed to create state!\n"); ;}
    break;

  case 446:

/* Line 1455 of yacc.c  */
#line 3253 "Grammar.ypp"
    { check_curCSState(); if(!curCSState->createState(getGLEnumValue((yyvsp[(1) - (4)].glenum)), (GLenum)((yyvsp[(3) - (4)].bval) ? GL_TRUE: GL_FALSE))) yyerror("Failed to create state!\n"); ;}
    break;

  case 447:

/* Line 1455 of yacc.c  */
#line 3254 "Grammar.ypp"
    { check_curDSTState();if(!curDSTState->createState(getGLEnumValue((yyvsp[(1) - (4)].glenum)), (GLenum)((yyvsp[(3) - (4)].bval) ? GL_TRUE: GL_FALSE))) yyerror("Failed to create state!\n"); ;}
    break;

  case 448:

/* Line 1455 of yacc.c  */
#line 3255 "Grammar.ypp"
    { check_curDSTState(); if(!curDSTState->createState(getGLEnumValue((yyvsp[(1) - (4)].glenum)), getGLEnumValue((yyvsp[(3) - (4)].glenum)))) yyerror("Failed to create state!\n"); ;}
    break;

  case 449:

/* Line 1455 of yacc.c  */
#line 3256 "Grammar.ypp"
    { check_curDSTState(); if(!curDSTState->createState(getGLEnumValue((yyvsp[(1) - (4)].glenum)), (GLenum)((yyvsp[(3) - (4)].bval) ? GL_TRUE: GL_FALSE))) yyerror("Failed to create state!\n"); ;}
    break;

  case 450:

/* Line 1455 of yacc.c  */
#line 3257 "Grammar.ypp"
    { check_curDSTState(); if(!curDSTState->createState(getGLEnumValue((yyvsp[(1) - (4)].glenum)), (GLenum)((yyvsp[(3) - (4)].bval) ? GL_TRUE: GL_FALSE))) yyerror("Failed to create state!\n"); ;}
    break;

  case 451:

/* Line 1455 of yacc.c  */
#line 3258 "Grammar.ypp"
    { check_curDSTState(); if(!curDSTState->createState(getGLEnumValue((yyvsp[(1) - (4)].glenum)), (GLenum)((yyvsp[(3) - (4)].bval) ? GL_TRUE: GL_FALSE))) yyerror("Failed to create state!\n"); ;}
    break;

  case 452:

/* Line 1455 of yacc.c  */
#line 3260 "Grammar.ypp"
    { check_curDSTState(); if(!curDSTState->createState(getGLEnumValue((yyvsp[(1) - (8)].glenum)), (yyvsp[(4) - (8)].fval), (yyvsp[(6) - (8)].fval))) yyerror("Failed to create state!\n"); ;}
    break;

  case 453:

/* Line 1455 of yacc.c  */
#line 3261 "Grammar.ypp"
    { check_curDSTState(); if(!curDSTState->createState(getGLEnumValue((yyvsp[(1) - (4)].glenum)), (GLenum)((yyvsp[(3) - (4)].bval) ? GL_TRUE: GL_FALSE))) yyerror("Failed to create state!\n"); ;}
    break;

  case 454:

/* Line 1455 of yacc.c  */
#line 3262 "Grammar.ypp"
    { check_curDSTState(); if(!curDSTState->createState(getGLEnumValue((yyvsp[(1) - (4)].glenum)), getGLEnumValue((yyvsp[(3) - (4)].glenum)))) yyerror("Failed to create state!\n"); ;}
    break;

  case 455:

/* Line 1455 of yacc.c  */
#line 3263 "Grammar.ypp"
    { 
    check_curDSTState();
    if(!curDSTState->createState(getGLEnumValue((yyvsp[(1) - (10)].glenum)), getGLEnumValue((yyvsp[(4) - (10)].glenum)))) yyerror("Failed to create state!\n");
    if(!curDSTState->createState(getGLEnumValue(GLSTENCILREF), (unsigned int)(yyvsp[(6) - (10)].ival))) yyerror("Failed to create state!\n");
    if(!curDSTState->createState(getGLEnumValue(GLSTENCILVALUEMASK), (unsigned int)(yyvsp[(8) - (10)].ival))) yyerror("Failed to create state!\n");
;}
    break;

  case 456:

/* Line 1455 of yacc.c  */
#line 3269 "Grammar.ypp"
    { 
    check_curDSTState();
    if(!curDSTState->createState(getGLEnumValue((yyvsp[(4) - (12)].glenum) == GLFRONT ? GLSTENCILFUNC : GLSTENCILBACKFUNC), getGLEnumValue((yyvsp[(6) - (12)].glenum)))) yyerror("Failed to create state!\n");
    if(!curDSTState->createState(getGLEnumValue((yyvsp[(4) - (12)].glenum) == GLFRONT ? GLSTENCILREF : GLSTENCILBACKREF), (unsigned int)(yyvsp[(8) - (12)].ival))) yyerror("Failed to create state!\n");
    if(!curDSTState->createState(getGLEnumValue((yyvsp[(4) - (12)].glenum) == GLFRONT ? GLSTENCILVALUEMASK : GLSTENCILBACKVALUEMASK), (unsigned int)(yyvsp[(10) - (12)].ival))) yyerror("Failed to create state!\n");
;}
    break;

  case 457:

/* Line 1455 of yacc.c  */
#line 3275 "Grammar.ypp"
    { check_curDSTState(); if(!curDSTState->createState(getGLEnumValue((yyvsp[(1) - (4)].glenum)), (unsigned int)(yyvsp[(3) - (4)].ival))) yyerror("Failed to create state!\n"); ;}
    break;

  case 458:

/* Line 1455 of yacc.c  */
#line 3276 "Grammar.ypp"
    { check_curDSTState(); if(!curDSTState->createState(getGLEnumValue((yyvsp[(1) - (4)].glenum)), (unsigned int)(yyvsp[(3) - (4)].ival))) yyerror("Failed to create state!\n"); ;}
    break;

  case 459:

/* Line 1455 of yacc.c  */
#line 3277 "Grammar.ypp"
    { check_curDSTState(); if(!curDSTState->createState(getGLEnumValue((yyvsp[(1) - (4)].glenum)), (unsigned int)(yyvsp[(3) - (4)].ival))) yyerror("Failed to create state!\n"); ;}
    break;

  case 460:

/* Line 1455 of yacc.c  */
#line 3278 "Grammar.ypp"
    { check_curDSTState(); if(!curDSTState->createState(getGLEnumValue((yyvsp[(1) - (4)].glenum)), getGLEnumValue((yyvsp[(3) - (4)].glenum)))) yyerror("Failed to create state!\n"); ;}
    break;

  case 461:

/* Line 1455 of yacc.c  */
#line 3279 "Grammar.ypp"
    { check_curDSTState(); if(!curDSTState->createState(getGLEnumValue((yyvsp[(1) - (4)].glenum)), getGLEnumValue((yyvsp[(3) - (4)].glenum)))) yyerror("Failed to create state!\n"); ;}
    break;

  case 462:

/* Line 1455 of yacc.c  */
#line 3280 "Grammar.ypp"
    { check_curDSTState(); if(!curDSTState->createState(getGLEnumValue((yyvsp[(1) - (4)].glenum)), getGLEnumValue((yyvsp[(3) - (4)].glenum)))) yyerror("Failed to create state!\n"); ;}
    break;

  case 463:

/* Line 1455 of yacc.c  */
#line 3281 "Grammar.ypp"
    {
    // a concatenated version of GLSTENCILPASSDEPTHFAIL+GLSTENCILPASSDEPTHPASS+GLSTENCILWRITEMASK == GL function arguments
    check_curDSTState();
    if(!curDSTState->createState(getGLEnumValue(GLSTENCILFAIL), getGLEnumValue((yyvsp[(4) - (10)].glenum)))) yyerror("Failed to create state!\n");
    if(!curDSTState->createState(getGLEnumValue(GLSTENCILPASSDEPTHFAIL), getGLEnumValue((yyvsp[(6) - (10)].glenum)))) yyerror("Failed to create state!\n");
    if(!curDSTState->createState(getGLEnumValue(GLSTENCILPASSDEPTHPASS), getGLEnumValue((yyvsp[(8) - (10)].glenum)))) yyerror("Failed to create state!\n");
;}
    break;

  case 464:

/* Line 1455 of yacc.c  */
#line 3288 "Grammar.ypp"
    { 
    check_curDSTState();
    if(!curDSTState->createState(getGLEnumValue((yyvsp[(4) - (12)].glenum) == GLFRONT ? GLSTENCILFAIL : GLSTENCILBACKFAIL), getGLEnumValue((yyvsp[(6) - (12)].glenum)))) yyerror("Failed to create state!\n");
    if(!curDSTState->createState(getGLEnumValue((yyvsp[(4) - (12)].glenum) == GLFRONT ? GLSTENCILPASSDEPTHFAIL : GLSTENCILBACKPASSDEPTHFAIL), (unsigned int)(yyvsp[(8) - (12)].glenum))) yyerror("Failed to create state!\n");
    if(!curDSTState->createState(getGLEnumValue((yyvsp[(4) - (12)].glenum) == GLFRONT ? GLSTENCILPASSDEPTHPASS : GLSTENCILBACKPASSDEPTHPASS), (unsigned int)(yyvsp[(10) - (12)].glenum))) yyerror("Failed to create state!\n");
;}
    break;

  case 465:

/* Line 1455 of yacc.c  */
#line 3301 "Grammar.ypp"
    { check_curDSTState(); if(!curDSTState->createState(getGLEnumValue((yyvsp[(1) - (4)].glenum)), getGLEnumValue((yyvsp[(3) - (4)].glenum)))) yyerror("Failed to create state!\n"); ;}
    break;

  case 466:

/* Line 1455 of yacc.c  */
#line 3302 "Grammar.ypp"
    { 
    check_curDSTState();
    if(!curDSTState->createState(getGLEnumValue((yyvsp[(1) - (10)].glenum)), getGLEnumValue((yyvsp[(4) - (10)].glenum)))) yyerror("Failed to create state!\n");
    if(!curDSTState->createState(getGLEnumValue(GLSTENCILBACKREF), (unsigned int)(yyvsp[(6) - (10)].ival))) yyerror("Failed to create state!\n");
    if(!curDSTState->createState(getGLEnumValue(GLSTENCILBACKVALUEMASK), (unsigned int)(yyvsp[(8) - (10)].ival))) yyerror("Failed to create state!\n");
;}
    break;

  case 467:

/* Line 1455 of yacc.c  */
#line 3308 "Grammar.ypp"
    { check_curDSTState(); if(!curDSTState->createState(getGLEnumValue((yyvsp[(1) - (4)].glenum)), (unsigned int)(yyvsp[(3) - (4)].ival))) yyerror("Failed to create state!\n"); ;}
    break;

  case 468:

/* Line 1455 of yacc.c  */
#line 3309 "Grammar.ypp"
    { check_curDSTState(); if(!curDSTState->createState(getGLEnumValue((yyvsp[(1) - (4)].glenum)), (unsigned int)(yyvsp[(3) - (4)].ival))) yyerror("Failed to create state!\n"); ;}
    break;

  case 469:

/* Line 1455 of yacc.c  */
#line 3310 "Grammar.ypp"
    { check_curDSTState(); if(!curDSTState->createState(getGLEnumValue((yyvsp[(1) - (4)].glenum)), getGLEnumValue((yyvsp[(3) - (4)].glenum)))) yyerror("Failed to create state!\n"); ;}
    break;

  case 470:

/* Line 1455 of yacc.c  */
#line 3311 "Grammar.ypp"
    { check_curDSTState(); if(!curDSTState->createState(getGLEnumValue((yyvsp[(1) - (4)].glenum)), getGLEnumValue((yyvsp[(3) - (4)].glenum)))) yyerror("Failed to create state!\n"); ;}
    break;

  case 471:

/* Line 1455 of yacc.c  */
#line 3312 "Grammar.ypp"
    { check_curDSTState(); if(!curDSTState->createState(getGLEnumValue((yyvsp[(1) - (4)].glenum)), getGLEnumValue((yyvsp[(3) - (4)].glenum)))) yyerror("Failed to create state!\n"); ;}
    break;

  case 472:

/* Line 1455 of yacc.c  */
#line 3313 "Grammar.ypp"
    { check_curDSTState(); if(!curDSTState->createState(getGLEnumValue((yyvsp[(1) - (4)].glenum)), (unsigned int)(yyvsp[(3) - (4)].ival))) yyerror("Failed to create state!\n"); ;}
    break;

  case 474:

/* Line 1455 of yacc.c  */
#line 3320 "Grammar.ypp"
    { check_curRState(); if(!curRState->createState(getGLEnumValue((yyvsp[(2) - (5)].glenum)), (yyvsp[(4) - (5)].fval))) yyerror("Failed to create state!\n"); ;}
    break;

  case 475:

/* Line 1455 of yacc.c  */
#line 3321 "Grammar.ypp"
    { check_curRState(); if(!curRState->createState(getGLEnumValue((yyvsp[(2) - (5)].glenum)), (GLenum)((yyvsp[(4) - (5)].bval) ? GL_TRUE: GL_FALSE))) yyerror("Failed to create state!\n"); ;}
    break;

  case 476:

/* Line 1455 of yacc.c  */
#line 3322 "Grammar.ypp"
    { check_curRState(); if(!curRState->createState(getGLEnumValue((yyvsp[(2) - (5)].glenum)), (GLenum)((yyvsp[(4) - (5)].bval) ? GL_TRUE: GL_FALSE))) yyerror("Failed to create state!\n"); ;}
    break;

  case 477:

/* Line 1455 of yacc.c  */
#line 3323 "Grammar.ypp"
    { check_curRState(); if(!curRState->createState(getGLEnumValue((yyvsp[(2) - (5)].glenum)), getGLEnumValue((yyvsp[(4) - (5)].glenum)))) yyerror("Failed to create state!\n"); ;}
    break;

  case 478:

/* Line 1455 of yacc.c  */
#line 3324 "Grammar.ypp"
    { check_curRState(); if(!curRState->createState(getGLEnumValue((yyvsp[(2) - (5)].glenum)), (yyvsp[(4) - (5)].fval))) yyerror("Failed to create state!\n"); ;}
    break;

  case 479:

/* Line 1455 of yacc.c  */
#line 3325 "Grammar.ypp"
    { check_curRState(); if(!curRState->createState(getGLEnumValue((yyvsp[(2) - (5)].glenum)), (GLenum)((yyvsp[(4) - (5)].bval) ? GL_TRUE: GL_FALSE))) yyerror("Failed to create state!\n"); ;}
    break;

  case 480:

/* Line 1455 of yacc.c  */
#line 3327 "Grammar.ypp"
    { check_curRState(); if(!curRState->createState(getGLEnumValue((yyvsp[(2) - (5)].glenum)), (unsigned int)(yyvsp[(4) - (5)].ival))) yyerror("Failed to create state!\n"); ;}
    break;

  case 481:

/* Line 1455 of yacc.c  */
#line 3328 "Grammar.ypp"
    { check_curRState(); if(!curRState->createState(getGLEnumValue((yyvsp[(2) - (5)].glenum)), (unsigned int)(yyvsp[(4) - (5)].ival))) yyerror("Failed to create state!\n"); ;}
    break;

  case 482:

/* Line 1455 of yacc.c  */
#line 3329 "Grammar.ypp"
    { check_curRState(); if(!curRState->createState(getGLEnumValue((yyvsp[(2) - (5)].glenum)), (GLenum)((yyvsp[(4) - (5)].bval) ? GL_TRUE: GL_FALSE))) yyerror("Failed to create state!\n"); ;}
    break;

  case 483:

/* Line 1455 of yacc.c  */
#line 3330 "Grammar.ypp"
    { check_curRState(); if(!curRState->createState(getGLEnumValue((yyvsp[(2) - (5)].glenum)), (GLenum)((yyvsp[(4) - (5)].bval) ? GL_TRUE: GL_FALSE))) yyerror("Failed to create state!\n"); ;}
    break;

  case 484:

/* Line 1455 of yacc.c  */
#line 3331 "Grammar.ypp"
    { check_curRState(); if(!curRState->createState(getGLEnumValue((yyvsp[(2) - (5)].glenum)), getGLEnumValue((yyvsp[(4) - (5)].glenum)))) yyerror("Failed to create state!\n"); ;}
    break;

  case 485:

/* Line 1455 of yacc.c  */
#line 3332 "Grammar.ypp"
    { check_curRState(); if(!curRState->createState(getGLEnumValue((yyvsp[(2) - (5)].glenum)), getGLEnumValue((yyvsp[(4) - (5)].glenum)))) yyerror("Failed to create state!\n"); ;}
    break;

  case 486:

/* Line 1455 of yacc.c  */
#line 3333 "Grammar.ypp"
    { check_curRState(); if(!curRState->createState(getGLEnumValue((yyvsp[(2) - (5)].glenum)), (GLenum)((yyvsp[(4) - (5)].bval) ? GL_TRUE: GL_FALSE))) yyerror("Failed to create state!\n"); ;}
    break;

  case 487:

/* Line 1455 of yacc.c  */
#line 3334 "Grammar.ypp"
    { check_curRState(); if(!curRState->createState(getGLEnumValue((yyvsp[(2) - (5)].glenum)), getGLEnumValue((yyvsp[(4) - (5)].glenum)), getGLEnumValue((yyvsp[(4) - (5)].glenum)))) yyerror("Failed to create state!\n"); ;}
    break;

  case 488:

/* Line 1455 of yacc.c  */
#line 3335 "Grammar.ypp"
    { if(!curRState->createState(getGLEnumValue((yyvsp[(2) - (9)].glenum)), getGLEnumValue((yyvsp[(5) - (9)].glenum)), getGLEnumValue((yyvsp[(7) - (9)].glenum)))) yyerror("Failed to create state!\n"); ;}
    break;

  case 489:

/* Line 1455 of yacc.c  */
#line 3336 "Grammar.ypp"
    { check_curRState(); if(!curRState->createState(getGLEnumValue((yyvsp[(2) - (5)].glenum)), (yyvsp[(4) - (5)].fval))) yyerror("Failed to create state!\n"); ;}
    break;

  case 490:

/* Line 1455 of yacc.c  */
#line 3337 "Grammar.ypp"
    { check_curRState(); if(!curRState->createState(getGLEnumValue((yyvsp[(2) - (5)].glenum)), (yyvsp[(4) - (5)].fval))) yyerror("Failed to create state!\n"); ;}
    break;

  case 491:

/* Line 1455 of yacc.c  */
#line 3338 "Grammar.ypp"
    { check_curRState(); if(!curRState->createState(getGLEnumValue((yyvsp[(2) - (5)].glenum)), (GLenum)((yyvsp[(4) - (5)].bval) ? GL_TRUE: GL_FALSE))) yyerror("Failed to create state!\n"); ;}
    break;

  case 492:

/* Line 1455 of yacc.c  */
#line 3339 "Grammar.ypp"
    { check_curRState(); if(!curRState->createState(getGLEnumValue((yyvsp[(2) - (5)].glenum)), (GLenum)((yyvsp[(4) - (5)].bval) ? GL_TRUE: GL_FALSE))) yyerror("Failed to create state!\n"); ;}
    break;

  case 493:

/* Line 1455 of yacc.c  */
#line 3340 "Grammar.ypp"
    { check_curRState(); if(!curRState->createState(getGLEnumValue((yyvsp[(2) - (5)].glenum)), (GLenum)((yyvsp[(4) - (5)].bval) ? GL_TRUE: GL_FALSE))) yyerror("Failed to create state!\n"); ;}
    break;

  case 494:

/* Line 1455 of yacc.c  */
#line 3342 "Grammar.ypp"
    { check_curRState(); if(!curRState->createState(getGLEnumValue((yyvsp[(2) - (5)].glenum)), (GLenum)((yyvsp[(4) - (5)].bval) ? GL_TRUE: GL_FALSE))) yyerror("Failed to create state!\n"); ;}
    break;

  case 495:

/* Line 1455 of yacc.c  */
#line 3343 "Grammar.ypp"
    { yyerror("Error in Rasterizer-state list\n"); ;}
    break;

  case 496:

/* Line 1455 of yacc.c  */
#line 3348 "Grammar.ypp"
    {
    LOGD("Rasterization state %s\n", (yyvsp[(2) - (2)].s)->c_str() );
    curRState = curContainer->createStateGroup_Raster((yyvsp[(2) - (2)].s)->c_str())->getExInterface();
    curAnnotations = curRState->annotations()->getExInterface();
    delete (yyvsp[(2) - (2)].s);
;}
    break;

  case 497:

/* Line 1455 of yacc.c  */
#line 3353 "Grammar.ypp"
    { 
    // for the case of items within a namespace :
    // copy the name-space annotations to this one
    if(curAnnotations) curAnnotations->copy(namespaceAnnotations);
    curAnnotations = NULL; curRState = NULL; lex_pop_state(); ;}
    break;

  case 498:

/* Line 1455 of yacc.c  */
#line 3358 "Grammar.ypp"
    { yyerror("Error in Rasterizer-state declaration\n"); ;}
    break;

  case 500:

/* Line 1455 of yacc.c  */
#line 3364 "Grammar.ypp"
    { check_curCSState(); if(!curCSState->createState(getGLEnumValue((yyvsp[(2) - (5)].glenum)), (GLenum)((yyvsp[(4) - (5)].bval) ? GL_TRUE: GL_FALSE))) yyerror("Failed to create state!\n"); ;}
    break;

  case 501:

/* Line 1455 of yacc.c  */
#line 3365 "Grammar.ypp"
    { check_curCSState(); if(!curCSState->createState(getGLEnumValue((yyvsp[(2) - (5)].glenum)), getGLEnumValue((yyvsp[(4) - (5)].glenum)))) yyerror("Failed to create state!\n"); ;}
    break;

  case 502:

/* Line 1455 of yacc.c  */
#line 3366 "Grammar.ypp"
    { check_curCSState(); if(!curCSState->createState(getGLEnumValue((yyvsp[(2) - (5)].glenum)), (yyvsp[(4) - (5)].fval))) yyerror("Failed to create state!\n"); ;}
    break;

  case 503:

/* Line 1455 of yacc.c  */
#line 3367 "Grammar.ypp"
    { check_curCSState(); if(!curCSState->createState(getGLEnumValue((yyvsp[(2) - (5)].glenum)), (GLenum)((yyvsp[(4) - (5)].bval) ? GL_TRUE: GL_FALSE))) yyerror("Failed to create state!\n"); ;}
    break;

  case 504:

/* Line 1455 of yacc.c  */
#line 3368 "Grammar.ypp"
    { check_curCSState(); if(!curCSState->createState(getGLEnumValue((yyvsp[(2) - (5)].glenum)), getGLEnumValue((yyvsp[(4) - (5)].glenum)))) yyerror("Failed to create state!\n"); ;}
    break;

  case 505:

/* Line 1455 of yacc.c  */
#line 3369 "Grammar.ypp"
    { check_curCSState(); if(!curCSState->createState(getGLEnumValue((yyvsp[(2) - (5)].glenum)), getGLEnumValue((yyvsp[(4) - (5)].glenum)))) yyerror("Failed to create state!\n"); ;}
    break;

  case 506:

/* Line 1455 of yacc.c  */
#line 3370 "Grammar.ypp"
    { check_curCSState(); if(!curCSState->createState(getGLEnumValue((yyvsp[(2) - (5)].glenum)), getGLEnumValue((yyvsp[(4) - (5)].glenum)))) yyerror("Failed to create state!\n"); ;}
    break;

  case 507:

/* Line 1455 of yacc.c  */
#line 3371 "Grammar.ypp"
    { check_curCSState(); if(!curCSState->createState(getGLEnumValue((yyvsp[(2) - (5)].glenum)), getGLEnumValue((yyvsp[(4) - (5)].glenum)))) yyerror("Failed to create state!\n"); ;}
    break;

  case 508:

/* Line 1455 of yacc.c  */
#line 3372 "Grammar.ypp"
    { check_curCSState(); if(!curCSState->createState(getGLEnumValue((yyvsp[(2) - (5)].glenum)), getGLEnumValue((yyvsp[(4) - (5)].glenum)))) yyerror("Failed to create state!\n"); ;}
    break;

  case 509:

/* Line 1455 of yacc.c  */
#line 3373 "Grammar.ypp"
    { check_curCSState(); if(!curCSState->createState(getGLEnumValue((yyvsp[(2) - (5)].glenum)), getGLEnumValue((yyvsp[(4) - (5)].glenum)))) yyerror("Failed to create state!\n"); ;}
    break;

  case 510:

/* Line 1455 of yacc.c  */
#line 3375 "Grammar.ypp"
    { check_curCSState(); if(!curCSState->createState(getGLEnumValue((yyvsp[(2) - (5)].glenum)), getGLEnumValue((yyvsp[(4) - (5)].glenum)))) yyerror("Failed to create state!\n"); ;}
    break;

  case 511:

/* Line 1455 of yacc.c  */
#line 3376 "Grammar.ypp"
    { check_curCSState(); if(!curCSState->createState(getGLEnumValue((yyvsp[(2) - (5)].glenum)), getGLEnumValue((yyvsp[(4) - (5)].glenum)))) yyerror("Failed to create state!\n"); ;}
    break;

  case 512:

/* Line 1455 of yacc.c  */
#line 3377 "Grammar.ypp"
    {
    check_curCSState();
    if(!curCSState->createState(getGLEnumValue(GLBLENDEQUATIONRGB), getGLEnumValue((yyvsp[(3) - (4)].glenum)))) yyerror("Failed to create state!\n");
    if(!curCSState->createState(getGLEnumValue(GLBLENDEQUATIONALPHA), getGLEnumValue((yyvsp[(3) - (4)].glenum)))) yyerror("Failed to create state!\n");
;}
    break;

  case 513:

/* Line 1455 of yacc.c  */
#line 3382 "Grammar.ypp"
    {
    check_curCSState();
    if(!curCSState->createState(getGLEnumValue(GLBLENDEQUATIONRGB), getGLEnumValue((yyvsp[(4) - (8)].glenum)))) yyerror("Failed to create state!\n");
    if(!curCSState->createState(getGLEnumValue(GLBLENDEQUATIONALPHA), getGLEnumValue((yyvsp[(6) - (8)].glenum)))) yyerror("Failed to create state!\n");
;}
    break;

  case 514:

/* Line 1455 of yacc.c  */
#line 3392 "Grammar.ypp"
    { check_curCSState(); if(!curCSState->createState(getGLEnumValue((yyvsp[(2) - (13)].glenum)), (yyvsp[(5) - (13)].fval), (yyvsp[(7) - (13)].fval), (yyvsp[(9) - (13)].fval), (yyvsp[(11) - (13)].fval))) yyerror("Failed to create state!\n"); ;}
    break;

  case 515:

/* Line 1455 of yacc.c  */
#line 3393 "Grammar.ypp"
    { check_curCSState(); if(!curCSState->createState(getGLEnumValue((yyvsp[(2) - (5)].glenum)), (GLenum)((yyvsp[(4) - (5)].bval) ? GL_TRUE: GL_FALSE))) yyerror("Failed to create state!\n"); ;}
    break;

  case 516:

/* Line 1455 of yacc.c  */
#line 3394 "Grammar.ypp"
    { check_curCSState(); if(!curCSState->createState(getGLEnumValue((yyvsp[(2) - (5)].glenum)), (GLenum)((yyvsp[(4) - (5)].bval) ? GL_TRUE: GL_FALSE))) yyerror("Failed to create state!\n"); ;}
    break;

  case 517:

/* Line 1455 of yacc.c  */
#line 3395 "Grammar.ypp"
    { check_curCSState(); if(!curCSState->createState(getGLEnumValue((yyvsp[(2) - (5)].glenum)), (GLenum)((yyvsp[(4) - (5)].bval) ? GL_TRUE: GL_FALSE))) yyerror("Failed to create state!\n"); ;}
    break;

  case 518:

/* Line 1455 of yacc.c  */
#line 3396 "Grammar.ypp"
    { check_curCSState(); if(!curCSState->createState(getGLEnumValue((yyvsp[(2) - (5)].glenum)), getGLEnumValue((yyvsp[(4) - (5)].glenum)))) yyerror("Failed to create state!\n"); ;}
    break;

  case 519:

/* Line 1455 of yacc.c  */
#line 3398 "Grammar.ypp"
    { check_curCSState(); if(!curCSState->createState(getGLEnumValue((yyvsp[(2) - (13)].glenum)), 
    (GLboolean)((yyvsp[(5) - (13)].bval) ? GL_TRUE: GL_FALSE),
    (GLboolean)((yyvsp[(7) - (13)].bval) ? GL_TRUE: GL_FALSE),
    (GLboolean)((yyvsp[(9) - (13)].bval) ? GL_TRUE: GL_FALSE),
    (GLboolean)((yyvsp[(11) - (13)].bval) ? GL_TRUE: GL_FALSE)
    )) yyerror("Failed to create state!\n"); ;}
    break;

  case 520:

/* Line 1455 of yacc.c  */
#line 3404 "Grammar.ypp"
    { check_curCSState(); if(!curCSState->createState(getGLEnumValue((yyvsp[(2) - (5)].glenum)), (GLenum)((yyvsp[(4) - (5)].bval) ? GL_TRUE: GL_FALSE))) yyerror("Failed to create state!\n"); ;}
    break;

  case 521:

/* Line 1455 of yacc.c  */
#line 3405 "Grammar.ypp"
    { check_curCSState(); if(!curCSState->createState(getGLEnumValue((yyvsp[(2) - (5)].glenum)), (GLenum)((yyvsp[(4) - (5)].bval) ? GL_TRUE: GL_FALSE))) yyerror("Failed to create state!\n"); ;}
    break;

  case 522:

/* Line 1455 of yacc.c  */
#line 3407 "Grammar.ypp"
    { check_curCSState(); if(!curCSState->createState(getGLEnumValue((yyvsp[(2) - (5)].glenum)), (GLenum)((yyvsp[(4) - (5)].bval) ? GL_TRUE: GL_FALSE))) yyerror("Failed to create state!\n"); ;}
    break;

  case 523:

/* Line 1455 of yacc.c  */
#line 3408 "Grammar.ypp"
    { check_curCSState(); if(!curCSState->createState(getGLEnumValue((yyvsp[(2) - (5)].glenum)), (GLenum)((yyvsp[(4) - (5)].bval) ? GL_TRUE: GL_FALSE))) yyerror("Failed to create state!\n"); ;}
    break;

  case 524:

/* Line 1455 of yacc.c  */
#line 3409 "Grammar.ypp"
    { yyerror("Error in Color sample state list\n"); ;}
    break;

  case 525:

/* Line 1455 of yacc.c  */
#line 3414 "Grammar.ypp"
    {
    LOGD("Color sample state %s\n", (yyvsp[(2) - (2)].s)->c_str() );
    curCSState = curContainer->createStateGroup_CS((yyvsp[(2) - (2)].s)->c_str())->getExInterface();
    curAnnotations = curCSState->annotations()->getExInterface();
    delete (yyvsp[(2) - (2)].s);
;}
    break;

  case 526:

/* Line 1455 of yacc.c  */
#line 3419 "Grammar.ypp"
    { 
    // for the case of items within a namespace :
    // copy the name-space annotations to this one
    if(curAnnotations) curAnnotations->copy(namespaceAnnotations);
    curAnnotations = NULL; curCSState = NULL; lex_pop_state(); ;}
    break;

  case 527:

/* Line 1455 of yacc.c  */
#line 3424 "Grammar.ypp"
    { yyerror("Error in color sampler state declaration\n"); ;}
    break;

  case 529:

/* Line 1455 of yacc.c  */
#line 3430 "Grammar.ypp"
    { check_curDSTState(); if(!curDSTState->createState(getGLEnumValue((yyvsp[(2) - (5)].glenum)), (GLenum)((yyvsp[(4) - (5)].bval) ? GL_TRUE: GL_FALSE))) yyerror("Failed to create state!\n"); ;}
    break;

  case 530:

/* Line 1455 of yacc.c  */
#line 3431 "Grammar.ypp"
    { check_curDSTState(); if(!curDSTState->createState(getGLEnumValue((yyvsp[(2) - (5)].glenum)), getGLEnumValue((yyvsp[(4) - (5)].glenum)))) yyerror("Failed to create state!\n"); ;}
    break;

  case 531:

/* Line 1455 of yacc.c  */
#line 3432 "Grammar.ypp"
    { check_curDSTState(); if(!curDSTState->createState(getGLEnumValue((yyvsp[(2) - (5)].glenum)), (GLenum)((yyvsp[(4) - (5)].bval) ? GL_TRUE: GL_FALSE))) yyerror("Failed to create state!\n"); ;}
    break;

  case 532:

/* Line 1455 of yacc.c  */
#line 3433 "Grammar.ypp"
    { check_curDSTState(); if(!curDSTState->createState(getGLEnumValue((yyvsp[(2) - (5)].glenum)), (GLenum)((yyvsp[(4) - (5)].bval) ? GL_TRUE: GL_FALSE))) yyerror("Failed to create state!\n"); ;}
    break;

  case 533:

/* Line 1455 of yacc.c  */
#line 3434 "Grammar.ypp"
    { check_curDSTState(); if(!curDSTState->createState(getGLEnumValue((yyvsp[(2) - (5)].glenum)), (GLenum)((yyvsp[(4) - (5)].bval) ? GL_TRUE: GL_FALSE))) yyerror("Failed to create state!\n"); ;}
    break;

  case 534:

/* Line 1455 of yacc.c  */
#line 3436 "Grammar.ypp"
    { check_curDSTState(); if(!curDSTState->createState(getGLEnumValue((yyvsp[(2) - (9)].glenum)), (yyvsp[(5) - (9)].fval), (yyvsp[(7) - (9)].fval))) yyerror("Failed to create state!\n"); ;}
    break;

  case 535:

/* Line 1455 of yacc.c  */
#line 3437 "Grammar.ypp"
    { check_curDSTState(); if(!curDSTState->createState(getGLEnumValue((yyvsp[(2) - (5)].glenum)), (GLenum)((yyvsp[(4) - (5)].bval) ? GL_TRUE: GL_FALSE))) yyerror("Failed to create state!\n"); ;}
    break;

  case 536:

/* Line 1455 of yacc.c  */
#line 3438 "Grammar.ypp"
    { check_curDSTState(); if(!curDSTState->createState(getGLEnumValue((yyvsp[(2) - (5)].glenum)), getGLEnumValue((yyvsp[(4) - (5)].glenum)))) yyerror("Failed to create state!\n"); ;}
    break;

  case 537:

/* Line 1455 of yacc.c  */
#line 3439 "Grammar.ypp"
    { check_curDSTState(); if(!curDSTState->createState(getGLEnumValue((yyvsp[(2) - (5)].glenum)), (unsigned int)(yyvsp[(4) - (5)].ival))) yyerror("Failed to create state!\n"); ;}
    break;

  case 538:

/* Line 1455 of yacc.c  */
#line 3440 "Grammar.ypp"
    { check_curDSTState(); if(!curDSTState->createState(getGLEnumValue((yyvsp[(2) - (5)].glenum)), (unsigned int)(yyvsp[(4) - (5)].ival))) yyerror("Failed to create state!\n"); ;}
    break;

  case 539:

/* Line 1455 of yacc.c  */
#line 3441 "Grammar.ypp"
    { check_curDSTState(); if(!curDSTState->createState(getGLEnumValue((yyvsp[(2) - (5)].glenum)), getGLEnumValue((yyvsp[(4) - (5)].glenum)))) yyerror("Failed to create state!\n"); ;}
    break;

  case 540:

/* Line 1455 of yacc.c  */
#line 3442 "Grammar.ypp"
    { check_curDSTState(); if(!curDSTState->createState(getGLEnumValue((yyvsp[(2) - (5)].glenum)), getGLEnumValue((yyvsp[(4) - (5)].glenum)))) yyerror("Failed to create state!\n"); ;}
    break;

  case 541:

/* Line 1455 of yacc.c  */
#line 3443 "Grammar.ypp"
    { check_curDSTState(); if(!curDSTState->createState(getGLEnumValue((yyvsp[(2) - (5)].glenum)), getGLEnumValue((yyvsp[(4) - (5)].glenum)))) yyerror("Failed to create state!\n"); ;}
    break;

  case 542:

/* Line 1455 of yacc.c  */
#line 3444 "Grammar.ypp"
    { check_curDSTState(); if(!curDSTState->createState(getGLEnumValue((yyvsp[(2) - (5)].glenum)), (unsigned int)(yyvsp[(4) - (5)].ival))) yyerror("Failed to create state!\n"); ;}
    break;

  case 543:

/* Line 1455 of yacc.c  */
#line 3452 "Grammar.ypp"
    { check_curDSTState(); if(!curDSTState->createState(getGLEnumValue((yyvsp[(2) - (5)].glenum)), getGLEnumValue((yyvsp[(4) - (5)].glenum)))) yyerror("Failed to create state!\n"); ;}
    break;

  case 544:

/* Line 1455 of yacc.c  */
#line 3453 "Grammar.ypp"
    { check_curDSTState(); if(!curDSTState->createState(getGLEnumValue((yyvsp[(2) - (5)].glenum)), (unsigned int)(yyvsp[(4) - (5)].ival))) yyerror("Failed to create state!\n"); ;}
    break;

  case 545:

/* Line 1455 of yacc.c  */
#line 3454 "Grammar.ypp"
    { check_curDSTState(); if(!curDSTState->createState(getGLEnumValue((yyvsp[(2) - (5)].glenum)), (unsigned int)(yyvsp[(4) - (5)].ival))) yyerror("Failed to create state!\n"); ;}
    break;

  case 546:

/* Line 1455 of yacc.c  */
#line 3455 "Grammar.ypp"
    { check_curDSTState(); if(!curDSTState->createState(getGLEnumValue((yyvsp[(2) - (5)].glenum)), getGLEnumValue((yyvsp[(4) - (5)].glenum)))) yyerror("Failed to create state!\n"); ;}
    break;

  case 547:

/* Line 1455 of yacc.c  */
#line 3456 "Grammar.ypp"
    { check_curDSTState(); if(!curDSTState->createState(getGLEnumValue((yyvsp[(2) - (5)].glenum)), getGLEnumValue((yyvsp[(4) - (5)].glenum)))) yyerror("Failed to create state!\n"); ;}
    break;

  case 548:

/* Line 1455 of yacc.c  */
#line 3457 "Grammar.ypp"
    { check_curDSTState(); if(!curDSTState->createState(getGLEnumValue((yyvsp[(2) - (5)].glenum)), getGLEnumValue((yyvsp[(4) - (5)].glenum)))) yyerror("Failed to create state!\n"); ;}
    break;

  case 549:

/* Line 1455 of yacc.c  */
#line 3458 "Grammar.ypp"
    { check_curDSTState(); if(!curDSTState->createState(getGLEnumValue((yyvsp[(2) - (5)].glenum)), (unsigned int)(yyvsp[(4) - (5)].ival))) yyerror("Failed to create state!\n"); ;}
    break;

  case 550:

/* Line 1455 of yacc.c  */
#line 3459 "Grammar.ypp"
    { yyerror("Error in Depth Stencil state list\n"); ;}
    break;

  case 551:

/* Line 1455 of yacc.c  */
#line 3464 "Grammar.ypp"
    {
    LOGD("Depth-Stencil state %s\n", (yyvsp[(2) - (2)].s)->c_str() );
    curDSTState = curContainer->createStateGroup_DST((yyvsp[(2) - (2)].s)->c_str())->getExInterface();
    curAnnotations = curDSTState->annotations()->getExInterface();
    delete (yyvsp[(2) - (2)].s);
;}
    break;

  case 552:

/* Line 1455 of yacc.c  */
#line 3469 "Grammar.ypp"
    { 
    // for the case of items within a namespace :
    // copy the name-space annotations to this one
    if(curAnnotations) curAnnotations->copy(namespaceAnnotations);
    curAnnotations = NULL; curDSTState = NULL; lex_pop_state(); ;}
    break;

  case 553:

/* Line 1455 of yacc.c  */
#line 3474 "Grammar.ypp"
    { yyerror("Error in DST-state declaration\n"); ;}
    break;

  case 554:

/* Line 1455 of yacc.c  */
#line 3477 "Grammar.ypp"
    { LOGI("TODO"); ;}
    break;

  case 555:

/* Line 1455 of yacc.c  */
#line 3478 "Grammar.ypp"
    { LOGI("TODO"); ;}
    break;

  case 556:

/* Line 1455 of yacc.c  */
#line 3479 "Grammar.ypp"
    { LOGI("TODO"); ;}
    break;

  case 557:

/* Line 1455 of yacc.c  */
#line 3480 "Grammar.ypp"
    { LOGI("TODO"); ;}
    break;

  case 558:

/* Line 1455 of yacc.c  */
#line 3481 "Grammar.ypp"
    { LOGI("TODO"); ;}
    break;

  case 559:

/* Line 1455 of yacc.c  */
#line 3482 "Grammar.ypp"
    { LOGI("TODO"); ;}
    break;

  case 560:

/* Line 1455 of yacc.c  */
#line 3483 "Grammar.ypp"
    { LOGI("TODO"); ;}
    break;

  case 561:

/* Line 1455 of yacc.c  */
#line 3484 "Grammar.ypp"
    { LOGI("TODO"); ;}
    break;

  case 562:

/* Line 1455 of yacc.c  */
#line 3485 "Grammar.ypp"
    { LOGI("TODO"); ;}
    break;

  case 563:

/* Line 1455 of yacc.c  */
#line 3486 "Grammar.ypp"
    { LOGI("TODO"); ;}
    break;

  case 564:

/* Line 1455 of yacc.c  */
#line 3487 "Grammar.ypp"
    { LOGI("TODO"); ;}
    break;

  case 565:

/* Line 1455 of yacc.c  */
#line 3488 "Grammar.ypp"
    { LOGI("TODO"); ;}
    break;

  case 566:

/* Line 1455 of yacc.c  */
#line 3489 "Grammar.ypp"
    { LOGI("TODO"); ;}
    break;

  case 567:

/* Line 1455 of yacc.c  */
#line 3490 "Grammar.ypp"
    { LOGI("TODO"); ;}
    break;

  case 568:

/* Line 1455 of yacc.c  */
#line 3491 "Grammar.ypp"
    { LOGI("TODO"); ;}
    break;

  case 569:

/* Line 1455 of yacc.c  */
#line 3492 "Grammar.ypp"
    { LOGI("TODO"); ;}
    break;

  case 570:

/* Line 1455 of yacc.c  */
#line 3493 "Grammar.ypp"
    { LOGI("TODO"); ;}
    break;

  case 571:

/* Line 1455 of yacc.c  */
#line 3494 "Grammar.ypp"
    { LOGI("TODO"); ;}
    break;

  case 572:

/* Line 1455 of yacc.c  */
#line 3495 "Grammar.ypp"
    { LOGI("TODO"); ;}
    break;

  case 573:

/* Line 1455 of yacc.c  */
#line 3496 "Grammar.ypp"
    { LOGI("TODO"); ;}
    break;

  case 574:

/* Line 1455 of yacc.c  */
#line 3497 "Grammar.ypp"
    { LOGI("TODO"); ;}
    break;

  case 575:

/* Line 1455 of yacc.c  */
#line 3498 "Grammar.ypp"
    { LOGI("TODO"); ;}
    break;

  case 577:

/* Line 1455 of yacc.c  */
#line 3507 "Grammar.ypp"
    { LOGI("TODO"); ;}
    break;

  case 578:

/* Line 1455 of yacc.c  */
#line 3508 "Grammar.ypp"
    { 
#ifndef OGLES2
    check_curPRState(); if(!curPRState->createState(getGLEnumValue((yyvsp[(2) - (5)].glenum)), getGLEnumValue((yyvsp[(4) - (5)].glenum)))) yyerror("Failed to create state!\n");
#endif
;}
    break;

  case 579:

/* Line 1455 of yacc.c  */
#line 3513 "Grammar.ypp"
    {
#ifndef OGLES2
    check_curPRState(); if(!curPRState->createState(getGLEnumValue((yyvsp[(2) - (5)].glenum)), getGLEnumValue((yyvsp[(4) - (5)].glenum)))) yyerror("Failed to create state!\n");
#endif
;}
    break;

  case 580:

/* Line 1455 of yacc.c  */
#line 3518 "Grammar.ypp"
    {
#ifndef OGLES2
    LOGI("TODO");
#endif
;}
    break;

  case 581:

/* Line 1455 of yacc.c  */
#line 3523 "Grammar.ypp"
    {
#ifndef OGLES2
    LOGI("TODO");
#endif
;}
    break;

  case 582:

/* Line 1455 of yacc.c  */
#line 3528 "Grammar.ypp"
    {
#ifndef OGLES2
    check_curPRState(); if(!curPRState->createState(getGLEnumValue((yyvsp[(2) - (5)].glenum)), getGLEnumValue((yyvsp[(4) - (5)].glenum)))) yyerror("Failed to create state!\n");
#endif
;}
    break;

  case 583:

/* Line 1455 of yacc.c  */
#line 3533 "Grammar.ypp"
    {
#ifndef OGLES2
    check_curPRState(); if(!curPRState->createState(getGLEnumValue((yyvsp[(2) - (5)].glenum)), (yyvsp[(4) - (5)].fval))) yyerror("Failed to create state!\n");
#endif
;}
    break;

  case 584:

/* Line 1455 of yacc.c  */
#line 3538 "Grammar.ypp"
    {
#ifndef OGLES2
    check_curPRState(); if(!curPRState->createState(getGLEnumValue((yyvsp[(2) - (5)].glenum)), getGLEnumValue((yyvsp[(4) - (5)].glenum)))) yyerror("Failed to create state!\n");
#endif
;}
    break;

  case 585:

/* Line 1455 of yacc.c  */
#line 3543 "Grammar.ypp"
    {
#ifndef OGLES2
    check_curPRState(); if(!curPRState->createState(getGLEnumValue((yyvsp[(2) - (5)].glenum)), getGLEnumValue((yyvsp[(4) - (5)].glenum)))) yyerror("Failed to create state!\n");
#endif
;}
    break;

  case 586:

/* Line 1455 of yacc.c  */
#line 3548 "Grammar.ypp"
    {
#ifndef OGLES2
    check_curPRState(); if(!curPRState->createState(getGLEnumValue((yyvsp[(2) - (5)].glenum)), getGLEnumValue((yyvsp[(4) - (5)].glenum)))) yyerror("Failed to create state!\n");
#endif
;}
    break;

  case 587:

/* Line 1455 of yacc.c  */
#line 3553 "Grammar.ypp"
    {
#ifndef OGLES2
    check_curPRState(); if(!curPRState->createState(getGLEnumValue((yyvsp[(2) - (5)].glenum)), getGLEnumValue((yyvsp[(4) - (5)].glenum)))) yyerror("Failed to create state!\n");
#endif
;}
    break;

  case 588:

/* Line 1455 of yacc.c  */
#line 3558 "Grammar.ypp"
    {
#ifndef OGLES2
    check_curPRState(); if(!curPRState->createState(getGLEnumValue((yyvsp[(2) - (5)].glenum)), getGLEnumValue((yyvsp[(4) - (5)].glenum)))) yyerror("Failed to create state!\n");
#endif
;}
    break;

  case 589:

/* Line 1455 of yacc.c  */
#line 3563 "Grammar.ypp"
    {
#ifndef OGLES2
    check_curPRState(); if(!curPRState->createState(getGLEnumValue((yyvsp[(2) - (5)].glenum)), (yyvsp[(4) - (5)].fval))) yyerror("Failed to create state!\n");
#endif
;}
    break;

  case 590:

/* Line 1455 of yacc.c  */
#line 3568 "Grammar.ypp"
    {
#ifndef OGLES2
    check_curPRState(); if(!curPRState->createState(getGLEnumValue((yyvsp[(2) - (5)].glenum)), (yyvsp[(4) - (5)].fval))) yyerror("Failed to create state!\n");
#endif
;}
    break;

  case 591:

/* Line 1455 of yacc.c  */
#line 3573 "Grammar.ypp"
    {
#ifndef OGLES2
    check_curPRState(); if(!curPRState->createState(getGLEnumValue((yyvsp[(2) - (5)].glenum)), getGLEnumValue((yyvsp[(4) - (5)].glenum)))) yyerror("Failed to create state!\n");
#endif
;}
    break;

  case 592:

/* Line 1455 of yacc.c  */
#line 3578 "Grammar.ypp"
    {
#ifndef OGLES2
    check_curPRState(); if(!curPRState->createState(getGLEnumValue((yyvsp[(2) - (5)].glenum)), (yyvsp[(4) - (5)].fval))) yyerror("Failed to create state!\n");
#endif
;}
    break;

  case 593:

/* Line 1455 of yacc.c  */
#line 3583 "Grammar.ypp"
    {
#ifndef OGLES2
    check_curPRState(); if(!curPRState->createState(getGLEnumValue((yyvsp[(2) - (5)].glenum)), getGLEnumValue((yyvsp[(4) - (5)].glenum)))) yyerror("Failed to create state!\n");
#endif
;}
    break;

  case 594:

/* Line 1455 of yacc.c  */
#line 3588 "Grammar.ypp"
    {
#ifndef OGLES2
    check_curPRState(); if(!curPRState->createState(getGLEnumValue((yyvsp[(2) - (5)].glenum)), (yyvsp[(4) - (5)].ival))) yyerror("Failed to create state!\n");
#endif
;}
    break;

  case 595:

/* Line 1455 of yacc.c  */
#line 3593 "Grammar.ypp"
    {
#ifndef OGLES2
    check_curPRState(); if(!curPRState->createState(getGLEnumValue((yyvsp[(2) - (5)].glenum)), getGLEnumValue((yyvsp[(4) - (5)].glenum)))) yyerror("Failed to create state!\n");
#endif
;}
    break;

  case 596:

/* Line 1455 of yacc.c  */
#line 3598 "Grammar.ypp"
    {
#ifndef OGLES2
    check_curPRState(); if(!curPRState->createState(getGLEnumValue((yyvsp[(2) - (5)].glenum)), getGLEnumValue((yyvsp[(4) - (5)].glenum)))) yyerror("Failed to create state!\n");
#endif
;}
    break;

  case 597:

/* Line 1455 of yacc.c  */
#line 3603 "Grammar.ypp"
    {
#ifndef OGLES2
    check_curPRState(); if(!curPRState->createState(getGLEnumValue((yyvsp[(2) - (5)].glenum)), getGLEnumValue((yyvsp[(4) - (5)].glenum)))) yyerror("Failed to create state!\n");
#endif
;}
    break;

  case 598:

/* Line 1455 of yacc.c  */
#line 3608 "Grammar.ypp"
    {
#ifndef OGLES2
    check_curPRState(); if(!curPRState->createState(getGLEnumValue((yyvsp[(2) - (5)].glenum)), getGLEnumValue((yyvsp[(4) - (5)].glenum)))) yyerror("Failed to create state!\n");
#endif
;}
    break;

  case 599:

/* Line 1455 of yacc.c  */
#line 3619 "Grammar.ypp"
    { yyerror("Error in Path-Rendering state list\n"); ;}
    break;

  case 600:

/* Line 1455 of yacc.c  */
#line 3625 "Grammar.ypp"
    {
#ifndef OGLES2
    LOGD("Path Rendering state %s\n", (yyvsp[(2) - (2)].s)->c_str() ); 
    curPRState = curContainer->createStateGroup_PR((yyvsp[(2) - (2)].s)->c_str())->getExInterface();
    curAnnotations = curPRState->annotations()->getExInterface();
#endif
    delete (yyvsp[(2) - (2)].s);
;}
    break;

  case 601:

/* Line 1455 of yacc.c  */
#line 3632 "Grammar.ypp"
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

  case 602:

/* Line 1455 of yacc.c  */
#line 3641 "Grammar.ypp"
    { yyerror("Error in DST-state declaration\n"); ;}
    break;

  case 604:

/* Line 1455 of yacc.c  */
#line 3652 "Grammar.ypp"
    {
    if(!curAnnotations)
        curAnnotations = IAnnotationEx::getAnnotationSingleton(2); // need a temporary place since nothing was initialized
    if(!curAnnotations->addAnnotation((yyvsp[(2) - (5)].s)->c_str(), (yyvsp[(4) - (5)].s)->c_str()))
        yyerror("err\n");
    delete (yyvsp[(4) - (5)].s);
    ;}
    break;

  case 605:

/* Line 1455 of yacc.c  */
#line 3659 "Grammar.ypp"
    {
    if(!curAnnotations)
        curAnnotations = IAnnotationEx::getAnnotationSingleton(2); // need a temporary place since nothing was initialized
    else if(!curAnnotations->addAnnotation((yyvsp[(2) - (5)].s)->c_str(), (yyvsp[(4) - (5)].fval)))
        yyerror("err\n");
    ;}
    break;

  case 606:

/* Line 1455 of yacc.c  */
#line 3665 "Grammar.ypp"
    {
    if(!curAnnotations)
        curAnnotations = IAnnotationEx::getAnnotationSingleton(2); // need a temporary place since nothing was initialized
    else if(!curAnnotations->addAnnotation((yyvsp[(2) - (5)].s)->c_str(), (yyvsp[(4) - (5)].ival)))
        yyerror("err\n");
    ;}
    break;

  case 607:

/* Line 1455 of yacc.c  */
#line 3671 "Grammar.ypp"
    {
    if(!curAnnotations)
        curAnnotations = IAnnotationEx::getAnnotationSingleton(2); // need a temporary place since nothing was initialized
    else if(!curAnnotations->addAnnotation((yyvsp[(2) - (5)].s)->c_str(), (yyvsp[(4) - (5)].bval)))
        yyerror("err\n");
    ;}
    break;

  case 608:

/* Line 1455 of yacc.c  */
#line 3683 "Grammar.ypp"
    { yyerror("Error in annotation\n"); ;}
    break;

  case 612:

/* Line 1455 of yacc.c  */
#line 3694 "Grammar.ypp"
    {
        LOGD("value %f\n", (yyvsp[(2) - (2)].fval) );
        FloatList* pfl = new FloatList;
        pfl->push_back((yyvsp[(2) - (2)].fval));
        (yyval.floatVecList) = new FloatVecList;
        (yyval.floatVecList)->push_back(pfl);
;}
    break;

  case 613:

/* Line 1455 of yacc.c  */
#line 3704 "Grammar.ypp"
    {
    (yyval.floatVecList) = new FloatVecList;
    (yyval.floatVecList)->push_back((yyvsp[(3) - (4)].floatList));
;}
    break;

  case 614:

/* Line 1455 of yacc.c  */
#line 3711 "Grammar.ypp"
    {
    (yyval.floatVecList) = (yyvsp[(3) - (4)].floatVecList);
;}
    break;

  case 615:

/* Line 1455 of yacc.c  */
#line 3717 "Grammar.ypp"
    {
        LOGD("value %d\n", (yyvsp[(2) - (2)].ival) );
        IntList* pfl = new IntList;
        pfl->push_back((yyvsp[(2) - (2)].ival));
        (yyval.intVecList) = new IntVecList;
        (yyval.intVecList)->push_back(pfl);
;}
    break;

  case 616:

/* Line 1455 of yacc.c  */
#line 3724 "Grammar.ypp"
    {
      LOGI("Error in valueassignment_i\n");
  ;}
    break;

  case 617:

/* Line 1455 of yacc.c  */
#line 3736 "Grammar.ypp"
    {
        LOGD("value %d\n", (yyvsp[(2) - (2)].bval) );
        BoolList* pfl = new BoolList;
        pfl->push_back((yyvsp[(2) - (2)].bval));
        (yyval.boolVecList) = new BoolVecList;
        (yyval.boolVecList)->push_back(pfl);
;}
    break;

  case 618:

/* Line 1455 of yacc.c  */
#line 3746 "Grammar.ypp"
    {
    (yyval.boolVecList) = new BoolVecList;
    (yyval.boolVecList)->push_back((yyvsp[(3) - (4)].boolList));
;}
    break;

  case 619:

/* Line 1455 of yacc.c  */
#line 3752 "Grammar.ypp"
    { (yyval.precisionType)=IUniform::PHighP; ;}
    break;

  case 620:

/* Line 1455 of yacc.c  */
#line 3753 "Grammar.ypp"
    { (yyval.precisionType)=IUniform::PMediumP; ;}
    break;

  case 621:

/* Line 1455 of yacc.c  */
#line 3754 "Grammar.ypp"
    { (yyval.precisionType)=IUniform::PLowP; ;}
    break;

  case 622:

/* Line 1455 of yacc.c  */
#line 3761 "Grammar.ypp"
    {
    (yyval.uniformQualifiers).hasUniformKeyword = true;
    (yyval.uniformQualifiers).global = false;
    (yyval.uniformQualifiers).precisionType = IUniform::PHighP;
;}
    break;

  case 623:

/* Line 1455 of yacc.c  */
#line 3766 "Grammar.ypp"
    {
    (yyval.uniformQualifiers).hasUniformKeyword = true;
    (yyval.uniformQualifiers).global = (yyvsp[(2) - (2)].bval);
    (yyval.uniformQualifiers).precisionType = IUniform::PHighP;
;}
    break;

  case 624:

/* Line 1455 of yacc.c  */
#line 3771 "Grammar.ypp"
    {
    (yyval.uniformQualifiers).hasUniformKeyword = true;
    (yyval.uniformQualifiers).global = false;
    (yyval.uniformQualifiers).precisionType = (yyvsp[(2) - (2)].precisionType);
;}
    break;

  case 625:

/* Line 1455 of yacc.c  */
#line 3776 "Grammar.ypp"
    {
    (yyval.uniformQualifiers).hasUniformKeyword = true;
    (yyval.uniformQualifiers).global = (yyvsp[(2) - (3)].bval);
    (yyval.uniformQualifiers).precisionType = (yyvsp[(3) - (3)].precisionType);
;}
    break;

  case 626:

/* Line 1455 of yacc.c  */
#line 3781 "Grammar.ypp"
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

  case 627:

/* Line 1455 of yacc.c  */
#line 3790 "Grammar.ypp"
    {
    (yyval.uniformQualifiers).hasUniformKeyword = false;
    (yyval.uniformQualifiers).global = false;
    (yyval.uniformQualifiers).precisionType = (yyvsp[(1) - (1)].precisionType);
;}
    break;

  case 628:

/* Line 1455 of yacc.c  */
#line 3795 "Grammar.ypp"
    {
    (yyval.uniformQualifiers).hasUniformKeyword = false;
    (yyval.uniformQualifiers).global = (yyvsp[(1) - (2)].bval);
    (yyval.uniformQualifiers).precisionType = (yyvsp[(2) - (2)].precisionType);
;}
    break;

  case 629:

/* Line 1455 of yacc.c  */
#line 3806 "Grammar.ypp"
    { (yyval.valueList) = new ValueList; 
    memset((yyval.valueList), 0, sizeof(ValueList));
    (yyval.valueList)->floatVecList = (yyvsp[(1) - (1)].floatVecList); ;}
    break;

  case 630:

/* Line 1455 of yacc.c  */
#line 3809 "Grammar.ypp"
    { (yyval.valueList) = new ValueList; 
    memset((yyval.valueList), 0, sizeof(ValueList));
    (yyval.valueList)->floatVecList = (yyvsp[(1) - (1)].floatVecList); ;}
    break;

  case 631:

/* Line 1455 of yacc.c  */
#line 3812 "Grammar.ypp"
    { (yyval.valueList) = new ValueList; 
    memset((yyval.valueList), 0, sizeof(ValueList));
    (yyval.valueList)->floatVecList = (yyvsp[(1) - (1)].floatVecList); ;}
    break;

  case 632:

/* Line 1455 of yacc.c  */
#line 3815 "Grammar.ypp"
    { (yyval.valueList) = new ValueList; 
    memset((yyval.valueList), 0, sizeof(ValueList));
    (yyval.valueList)->intVecList = (yyvsp[(1) - (1)].intVecList); ;}
    break;

  case 633:

/* Line 1455 of yacc.c  */
#line 3821 "Grammar.ypp"
    { (yyval.valueList) = new ValueList; 
    memset((yyval.valueList), 0, sizeof(ValueList));
    (yyval.valueList)->boolVecList = (yyvsp[(1) - (1)].boolVecList); ;}
    break;

  case 634:

/* Line 1455 of yacc.c  */
#line 3824 "Grammar.ypp"
    { (yyval.valueList) = new ValueList; 
    memset((yyval.valueList), 0, sizeof(ValueList));
    (yyval.valueList)->boolVecList = (yyvsp[(1) - (1)].boolVecList); ;}
    break;

  case 635:

/* Line 1455 of yacc.c  */
#line 3827 "Grammar.ypp"
    { 
    (yyval.valueList) = NULL; yyerror("Error in value assignment\n"); 
;}
    break;

  case 636:

/* Line 1455 of yacc.c  */
#line 3833 "Grammar.ypp"
    {
    LOGD("semantic %s\n", (yyvsp[(2) - (2)].s)->c_str() );
    (yyval.s) = (yyvsp[(2) - (2)].s);
;}
    break;

  case 637:

/* Line 1455 of yacc.c  */
#line 3840 "Grammar.ypp"
    { 
    (yyval.ival) = -1;
;}
    break;

  case 638:

/* Line 1455 of yacc.c  */
#line 3843 "Grammar.ypp"
    {
    (yyval.ival) = (yyvsp[(2) - (3)].ival);
;}
    break;

  case 639:

/* Line 1455 of yacc.c  */
#line 3854 "Grammar.ypp"
    {
    (yyval.saszsauva) = new SASzSAUva;
    memset((yyval.saszsauva), 0, sizeof(SASzSAUva));
;}
    break;

  case 640:

/* Line 1455 of yacc.c  */
#line 3858 "Grammar.ypp"
    {
    (yyval.saszsauva) = new SASzSAUva;
    memset((yyval.saszsauva), 0, sizeof(SASzSAUva));
;}
    break;

  case 641:

/* Line 1455 of yacc.c  */
#line 3862 "Grammar.ypp"
    {
    (yyval.saszsauva) = new SASzSAUva;
    memset((yyval.saszsauva), 0, sizeof(SASzSAUva));
    (yyval.saszsauva)->valueList = (yyvsp[(1) - (1)].valueList);
;}
    break;

  case 642:

/* Line 1455 of yacc.c  */
#line 3867 "Grammar.ypp"
    {
    (yyval.saszsauva) = new SASzSAUva;
    memset((yyval.saszsauva), 0, sizeof(SASzSAUva));
    (yyval.saszsauva)->valueList = (yyvsp[(2) - (2)].valueList);
;}
    break;

  case 643:

/* Line 1455 of yacc.c  */
#line 3872 "Grammar.ypp"
    {
    (yyval.saszsauva) = new SASzSAUva;
    memset((yyval.saszsauva), 0, sizeof(SASzSAUva));
    (yyval.saszsauva)->semantic = (yyvsp[(1) - (1)].s);
;}
    break;

  case 644:

/* Line 1455 of yacc.c  */
#line 3877 "Grammar.ypp"
    {
    (yyval.saszsauva) = new SASzSAUva;
    memset((yyval.saszsauva), 0, sizeof(SASzSAUva));
    (yyval.saszsauva)->semantic = (yyvsp[(1) - (2)].s);
;}
    break;

  case 645:

/* Line 1455 of yacc.c  */
#line 3882 "Grammar.ypp"
    {
    (yyval.saszsauva) = new SASzSAUva;
    memset((yyval.saszsauva), 0, sizeof(SASzSAUva));
    (yyval.saszsauva)->semantic = (yyvsp[(1) - (2)].s);
    (yyval.saszsauva)->valueList = (yyvsp[(2) - (2)].valueList);
;}
    break;

  case 646:

/* Line 1455 of yacc.c  */
#line 3888 "Grammar.ypp"
    {
    (yyval.saszsauva) = new SASzSAUva;
    memset((yyval.saszsauva), 0, sizeof(SASzSAUva));
    (yyval.saszsauva)->semantic = (yyvsp[(1) - (3)].s);
    (yyval.saszsauva)->valueList = (yyvsp[(3) - (3)].valueList);
;}
    break;

  case 647:

/* Line 1455 of yacc.c  */
#line 3894 "Grammar.ypp"
    {
    (yyval.saszsauva) = new SASzSAUva;
    memset((yyval.saszsauva), 0, sizeof(SASzSAUva));
    (yyval.saszsauva)->arraySz = (yyvsp[(1) - (1)].ival);
;}
    break;

  case 648:

/* Line 1455 of yacc.c  */
#line 3899 "Grammar.ypp"
    {
    (yyval.saszsauva) = new SASzSAUva;
    memset((yyval.saszsauva), 0, sizeof(SASzSAUva));
    (yyval.saszsauva)->arraySz = (yyvsp[(1) - (2)].ival);
;}
    break;

  case 649:

/* Line 1455 of yacc.c  */
#line 3904 "Grammar.ypp"
    {
    (yyval.saszsauva) = new SASzSAUva;
    memset((yyval.saszsauva), 0, sizeof(SASzSAUva));
    (yyval.saszsauva)->arraySz = (yyvsp[(1) - (2)].ival);
    (yyval.saszsauva)->valueList = (yyvsp[(2) - (2)].valueList);
;}
    break;

  case 650:

/* Line 1455 of yacc.c  */
#line 3910 "Grammar.ypp"
    {
    (yyval.saszsauva) = new SASzSAUva;
    memset((yyval.saszsauva), 0, sizeof(SASzSAUva));
    (yyval.saszsauva)->arraySz = (yyvsp[(1) - (3)].ival);
    (yyval.saszsauva)->valueList = (yyvsp[(3) - (3)].valueList);
;}
    break;

  case 651:

/* Line 1455 of yacc.c  */
#line 3916 "Grammar.ypp"
    {
    (yyval.saszsauva) = new SASzSAUva;
    memset((yyval.saszsauva), 0, sizeof(SASzSAUva));
    (yyval.saszsauva)->arraySz = (yyvsp[(1) - (2)].ival);
    (yyval.saszsauva)->semantic = (yyvsp[(2) - (2)].s);
;}
    break;

  case 652:

/* Line 1455 of yacc.c  */
#line 3922 "Grammar.ypp"
    {
    (yyval.saszsauva) = new SASzSAUva;
    memset((yyval.saszsauva), 0, sizeof(SASzSAUva));
    (yyval.saszsauva)->arraySz = (yyvsp[(1) - (3)].ival);
    (yyval.saszsauva)->semantic = (yyvsp[(2) - (3)].s);
;}
    break;

  case 653:

/* Line 1455 of yacc.c  */
#line 3928 "Grammar.ypp"
    {
    (yyval.saszsauva) = new SASzSAUva;
    memset((yyval.saszsauva), 0, sizeof(SASzSAUva));
    (yyval.saszsauva)->arraySz = (yyvsp[(1) - (3)].ival);
    (yyval.saszsauva)->semantic = (yyvsp[(2) - (3)].s);
    (yyval.saszsauva)->valueList = (yyvsp[(3) - (3)].valueList);
;}
    break;

  case 654:

/* Line 1455 of yacc.c  */
#line 3935 "Grammar.ypp"
    {
    (yyval.saszsauva) = new SASzSAUva;
    memset((yyval.saszsauva), 0, sizeof(SASzSAUva));
    (yyval.saszsauva)->arraySz = (yyvsp[(1) - (4)].ival);
    (yyval.saszsauva)->semantic = (yyvsp[(2) - (4)].s);
    (yyval.saszsauva)->valueList = (yyvsp[(4) - (4)].valueList);
;}
    break;

  case 655:

/* Line 1455 of yacc.c  */
#line 3947 "Grammar.ypp"
    { (yyval.uniformType) = nvFX::IUniform::TFloat; ;}
    break;

  case 656:

/* Line 1455 of yacc.c  */
#line 3948 "Grammar.ypp"
    { (yyval.uniformType) = nvFX::IUniform::TVec2; ;}
    break;

  case 657:

/* Line 1455 of yacc.c  */
#line 3949 "Grammar.ypp"
    { (yyval.uniformType) = nvFX::IUniform::TVec3; ;}
    break;

  case 658:

/* Line 1455 of yacc.c  */
#line 3950 "Grammar.ypp"
    { (yyval.uniformType) = nvFX::IUniform::TVec4; ;}
    break;

  case 659:

/* Line 1455 of yacc.c  */
#line 3951 "Grammar.ypp"
    { (yyval.uniformType) = nvFX::IUniform::TInt; ;}
    break;

  case 660:

/* Line 1455 of yacc.c  */
#line 3952 "Grammar.ypp"
    { (yyval.uniformType) = nvFX::IUniform::TInt2; ;}
    break;

  case 661:

/* Line 1455 of yacc.c  */
#line 3953 "Grammar.ypp"
    { (yyval.uniformType) = nvFX::IUniform::TInt3; ;}
    break;

  case 662:

/* Line 1455 of yacc.c  */
#line 3954 "Grammar.ypp"
    { (yyval.uniformType) = nvFX::IUniform::TInt4; ;}
    break;

  case 663:

/* Line 1455 of yacc.c  */
#line 3955 "Grammar.ypp"
    { (yyval.uniformType) = nvFX::IUniform::TBool; ;}
    break;

  case 664:

/* Line 1455 of yacc.c  */
#line 3956 "Grammar.ypp"
    { (yyval.uniformType) = nvFX::IUniform::TBool2; ;}
    break;

  case 665:

/* Line 1455 of yacc.c  */
#line 3957 "Grammar.ypp"
    { (yyval.uniformType) = nvFX::IUniform::TBool3; ;}
    break;

  case 666:

/* Line 1455 of yacc.c  */
#line 3958 "Grammar.ypp"
    { (yyval.uniformType) = nvFX::IUniform::TBool4; ;}
    break;

  case 667:

/* Line 1455 of yacc.c  */
#line 3959 "Grammar.ypp"
    { (yyval.uniformType) = nvFX::IUniform::TMat2; ;}
    break;

  case 668:

/* Line 1455 of yacc.c  */
#line 3960 "Grammar.ypp"
    { (yyval.uniformType) = nvFX::IUniform::TMat3; ;}
    break;

  case 669:

/* Line 1455 of yacc.c  */
#line 3961 "Grammar.ypp"
    { (yyval.uniformType) = nvFX::IUniform::TMat4; ;}
    break;

  case 670:

/* Line 1455 of yacc.c  */
#line 3962 "Grammar.ypp"
    { (yyval.uniformType) = nvFX::IUniform::TTexture1D; ;}
    break;

  case 671:

/* Line 1455 of yacc.c  */
#line 3963 "Grammar.ypp"
    { (yyval.uniformType) = nvFX::IUniform::TTexture2D; ;}
    break;

  case 672:

/* Line 1455 of yacc.c  */
#line 3964 "Grammar.ypp"
    { (yyval.uniformType) = nvFX::IUniform::TTexture2DRect; ;}
    break;

  case 673:

/* Line 1455 of yacc.c  */
#line 3965 "Grammar.ypp"
    { (yyval.uniformType) = nvFX::IUniform::TTexture3D; ;}
    break;

  case 674:

/* Line 1455 of yacc.c  */
#line 3966 "Grammar.ypp"
    { (yyval.uniformType) = nvFX::IUniform::TTextureCube; ;}
    break;

  case 675:

/* Line 1455 of yacc.c  */
#line 3967 "Grammar.ypp"
    { (yyval.uniformType) = nvFX::IUniform::TTexture1D; ;}
    break;

  case 676:

/* Line 1455 of yacc.c  */
#line 3968 "Grammar.ypp"
    { (yyval.uniformType) = nvFX::IUniform::TTexture2D; ;}
    break;

  case 677:

/* Line 1455 of yacc.c  */
#line 3969 "Grammar.ypp"
    { (yyval.uniformType) = nvFX::IUniform::TTexture2DRect; ;}
    break;

  case 678:

/* Line 1455 of yacc.c  */
#line 3970 "Grammar.ypp"
    { (yyval.uniformType) = nvFX::IUniform::TTexture3D; ;}
    break;

  case 679:

/* Line 1455 of yacc.c  */
#line 3971 "Grammar.ypp"
    { (yyval.uniformType) = nvFX::IUniform::TTextureCube; ;}
    break;

  case 680:

/* Line 1455 of yacc.c  */
#line 3972 "Grammar.ypp"
    { yyerror("Error : unknown type\n"); ;}
    break;

  case 681:

/* Line 1455 of yacc.c  */
#line 3978 "Grammar.ypp"
    {
        curAnnotations = IAnnotationEx::getAnnotationSingleton(); // need a temporary place
        curAnnotations->clear();
    ;}
    break;

  case 682:

/* Line 1455 of yacc.c  */
#line 3981 "Grammar.ypp"
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

  case 704:

/* Line 1455 of yacc.c  */
#line 4101 "Grammar.ypp"
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

  case 705:

/* Line 1455 of yacc.c  */
#line 4109 "Grammar.ypp"
    { 
    namespaceName.clear(); 
    curAnnotations = NULL;
    namespaceAnnotations = NULL;
    delete (yyvsp[(2) - (7)].s);
;}
    break;

  case 709:

/* Line 1455 of yacc.c  */
#line 4127 "Grammar.ypp"
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

  case 710:

/* Line 1455 of yacc.c  */
#line 4158 "Grammar.ypp"
    {
    curCstBuffer = NULL;
    curAnnotations = NULL;
    lex_pop_state();
;}
    break;

  case 711:

/* Line 1455 of yacc.c  */
#line 4169 "Grammar.ypp"
    {
    delete (yyvsp[(4) - (6)].varNameList);
    ;}
    break;

  case 712:

/* Line 1455 of yacc.c  */
#line 4172 "Grammar.ypp"
    {
    delete (yyvsp[(5) - (7)].varNameList);
    ;}
    break;

  case 713:

/* Line 1455 of yacc.c  */
#line 4175 "Grammar.ypp"
    {
    delete (yyvsp[(4) - (5)].s)
    ;}
    break;

  case 714:

/* Line 1455 of yacc.c  */
#line 4178 "Grammar.ypp"
    {
    delete (yyvsp[(4) - (5)].s)
    ;}
    break;

  case 715:

/* Line 1455 of yacc.c  */
#line 4181 "Grammar.ypp"
    {
    delete (yyvsp[(4) - (5)].s)
    ;}
    break;

  case 716:

/* Line 1455 of yacc.c  */
#line 4184 "Grammar.ypp"
    {
    delete (yyvsp[(4) - (5)].s)
    ;}
    break;

  case 717:

/* Line 1455 of yacc.c  */
#line 4187 "Grammar.ypp"
    {
;}
    break;

  case 718:

/* Line 1455 of yacc.c  */
#line 4189 "Grammar.ypp"
    {
    delete (yyvsp[(4) - (5)].s)
    ;}
    break;

  case 719:

/* Line 1455 of yacc.c  */
#line 4192 "Grammar.ypp"
    {
    // we prefer this loop : we don't have the char ** available here... using vector of std::string...
    //for(int i=0; i<(int)$8->size(); i++)
    //    curXX->createState($4->c_str(), i, (*$8)[i].c_str());
    delete (yyvsp[(4) - (10)].s);
    delete (yyvsp[(8) - (10)].varNameList);
    ;}
    break;

  case 720:

/* Line 1455 of yacc.c  */
#line 4199 "Grammar.ypp"
    {
    //curPass->createState($4->c_str(), 0, $7->c_str());
    delete (yyvsp[(4) - (8)].s);
    delete (yyvsp[(7) - (8)].s);
    ;}
    break;

  case 721:

/* Line 1455 of yacc.c  */
#line 4204 "Grammar.ypp"
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

  case 722:

/* Line 1455 of yacc.c  */
#line 4220 "Grammar.ypp"
    {
   ;}
    break;

  case 723:

/* Line 1455 of yacc.c  */
#line 4221 "Grammar.ypp"
    {
   ;}
    break;

  case 724:

/* Line 1455 of yacc.c  */
#line 4222 "Grammar.ypp"
    {
   ;}
    break;

  case 726:

/* Line 1455 of yacc.c  */
#line 4231 "Grammar.ypp"
    {
    delete (yyvsp[(5) - (7)].varNameList);
    ;}
    break;

  case 727:

/* Line 1455 of yacc.c  */
#line 4234 "Grammar.ypp"
    {
    delete (yyvsp[(5) - (7)].varNameList);
    ;}
    break;

  case 728:

/* Line 1455 of yacc.c  */
#line 4237 "Grammar.ypp"
    {
    // we prefer this loop : we don't have the char ** available here... using vector of std::string...
    //for(int i=0; i<(int)$8->size(); i++)
    //    curXX->createState($4->c_str(), i, (*$8)[i].c_str());
    delete (yyvsp[(4) - (10)].s);
    delete (yyvsp[(8) - (10)].varNameList);
    ;}
    break;

  case 729:

/* Line 1455 of yacc.c  */
#line 4244 "Grammar.ypp"
    {
    //curPass->createState($4->c_str(), 0, $7->c_str());
    delete (yyvsp[(4) - (8)].s);
    delete (yyvsp[(7) - (8)].s);
    ;}
    break;

  case 730:

/* Line 1455 of yacc.c  */
#line 4249 "Grammar.ypp"
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

  case 731:

/* Line 1455 of yacc.c  */
#line 4265 "Grammar.ypp"
    {
   ;}
    break;

  case 732:

/* Line 1455 of yacc.c  */
#line 4266 "Grammar.ypp"
    {
   ;}
    break;

  case 733:

/* Line 1455 of yacc.c  */
#line 4267 "Grammar.ypp"
    {
   ;}
    break;

  case 735:

/* Line 1455 of yacc.c  */
#line 4276 "Grammar.ypp"
    {
    //if(curResource) {
    //    if(!curResource->setFormat($4))
    //    {
    //        yyerror("Error: Bad format\n");
    //    }
    //}
;}
    break;

  case 736:

/* Line 1455 of yacc.c  */
#line 4284 "Grammar.ypp"
    {
    //if(curResource)
    //    curResource->setDimensionsApplicationDependent();
;}
    break;

  case 737:

/* Line 1455 of yacc.c  */
#line 4288 "Grammar.ypp"
    {
    int d = (int)(yyvsp[(5) - (7)].intList)->size();
    //if(curResource)
    //    curResource->setDimensions((*$5)[0], d > 1 ? (*$5)[1] : 1, d > 2 ? (*$5)[2] : 1);
    delete (yyvsp[(5) - (7)].intList);
;}
    break;

  case 738:

/* Line 1455 of yacc.c  */
#line 4296 "Grammar.ypp"
    {
    delete (yyvsp[(2) - (2)].s);
   ;}
    break;

  case 739:

/* Line 1455 of yacc.c  */
#line 4298 "Grammar.ypp"
    {
   ;}
    break;

  case 740:

/* Line 1455 of yacc.c  */
#line 4299 "Grammar.ypp"
    {
   ;}
    break;

  case 742:

/* Line 1455 of yacc.c  */
#line 4308 "Grammar.ypp"
    {
    delete (yyvsp[(4) - (5)].s);
;}
    break;

  case 743:

/* Line 1455 of yacc.c  */
#line 4311 "Grammar.ypp"
    {
    delete (yyvsp[(4) - (5)].s);
;}
    break;

  case 744:

/* Line 1455 of yacc.c  */
#line 4316 "Grammar.ypp"
    {
   ;}
    break;

  case 745:

/* Line 1455 of yacc.c  */
#line 4317 "Grammar.ypp"
    {
   ;}
    break;

  case 746:

/* Line 1455 of yacc.c  */
#line 4318 "Grammar.ypp"
    {
   ;}
    break;



/* Line 1455 of yacc.c  */
#line 10624 "C:/p4/GitHub/nvFX/FxParser/Grammar.cpp"
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
#line 4323 "Grammar.ypp"


