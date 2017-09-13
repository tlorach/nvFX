
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



/* Line 1676 of yacc.c  */
#line 215 "D:/FromTiers/git/nvFX/FxParser/Grammar.hpp"

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

/* Line 1676 of yacc.c  */
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



/* Line 1676 of yacc.c  */
#line 653 "D:/FromTiers/git/nvFX/FxParser/Grammar.hpp"
} YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
#endif

extern YYSTYPE FxParserlval;


