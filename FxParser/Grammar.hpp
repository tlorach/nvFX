
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

/* Line 1676 of yacc.c  */
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



/* Line 1676 of yacc.c  */
#line 645 "C:/p4/GitHub/nvFX/FxParser/Grammar.hpp"
} YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
#endif

extern YYSTYPE FxParserlval;


