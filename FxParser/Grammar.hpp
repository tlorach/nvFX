
/* A Bison parser, made by GNU Bison 2.4.1.  */

/* Skeleton interface for Bison's Yacc-like parsers in C
   
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

/* "%code requires" blocks.  */

/* Line 1676 of yacc.c  */
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



/* Line 1676 of yacc.c  */
#line 214 "C:/p4/GitHub/nvFX/FxParser/Grammar.hpp"

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

/* Line 1676 of yacc.c  */
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



/* Line 1676 of yacc.c  */
#line 644 "C:/p4/GitHub/nvFX/FxParser/Grammar.hpp"
} YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
#endif

extern YYSTYPE FxParserlval;


