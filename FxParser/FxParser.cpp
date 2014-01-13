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
    #include <string>
    #include <assert.h>
    #include <stdio.h>

    #include "Fx.h"
    #include "FxParser.h"
    #include "Grammar.hpp"

    //
    // informations to access lex.cpp stuff
    //
    struct yy_buffer_state;
    typedef struct yy_buffer_state *YY_BUFFER_STATE;

    //extern YY_BUFFER_STATE yy_scan_string( const char *yy_str );
    //extern void yy_delete_buffer( YY_BUFFER_STATE b );
    //extern YY_BUFFER_STATE yy_create_buffer( FILE *file, int size );
    //extern void yy_switch_to_buffer( YY_BUFFER_STATE new_buffer );

    #define yyparse FxParserparse
    #define yyin FxParserin
    #define yyerror FxParsererror
    extern int FxParserparse();
    extern FILE *FxParserin;
    extern YY_BUFFER_STATE FxParser_scan_string( const char *yy_str );
    extern void FxParser_delete_buffer( YY_BUFFER_STATE b );
    extern YY_BUFFER_STATE FxParser_create_buffer( FILE *file, int size );
    extern void FxParser_switch_to_buffer( YY_BUFFER_STATE new_buffer );

    namespace nvFX
    {
    IncludeCb g_includeCb = NULL;

    int nParseErrors = 0;

    void setIncludeCallback(IncludeCb cbPtr)
    {
        g_includeCb = cbPtr;
    }

    bool loadEffect(IContainer* pCont, const char * str)
    {
        if(!pCont)
            return false;
    #if YYDEBUG
        yydebug = 1;
    #endif
        nParseErrors = 0;
        curContainer = pCont->getExInterface();

        YY_BUFFER_STATE state;
        state = FxParser_scan_string(str);
        FxParser_switch_to_buffer(state);

        int res = yyparse();

        FxParser_delete_buffer(state);

        // Now do a special thing : gather the created Uniforms 
        // and add them to the generic GLSLShader (un-named)
        Shader *pShader;
        for(int i=0; pShader = (Shader*)pCont->findShader(i); i++)
        {
            const char* name = pShader->getName();
            if((*name == '\0')&&(pShader->getType() == TGLSL))
            {
                pShader->addHeaderCode("    #define INFX\n");
        // To remove: now the grammar of nvFX allows to choose.
        //#ifndef USE_OLDPROGRAM
        //        pShader->addHeaderCode("    #extension GL_ARB_separate_shader_objects : enable\n");
        //#endif
                break;
            }
        }
    	//TODO: replace with 
    	//pCont->getExInterface()->addConsolidateShaderCode();
        LOGD("building code from nvFX uniforms...\n");
        nvFX::buildCodeFromUniforms(pCont);
    #ifdef USECUDA
        LOGD("building code from nvFX uniforms for CUDA...\n");
        nvFX::buildCodeFromUniformsCUDA(pCont);
    #endif
        nvFX::printf("Done\n");
    	return nParseErrors > 0 ? false : true;
    }

    bool loadEffectFromFile(IContainer* pCont, const char * filename)
    {
        if(!pCont)
            return false;
        // set the container name to this file name
        pCont->setName(filename);
    #if YYDEBUG
        yydebug = 1;
    #endif
        nParseErrors = 0;
        curContainer = pCont->getExInterface();
        LOGD("Opening file %s...\n", filename);
        yyin  =  fopen(  filename,  "r"  );
        if(!yyin)
        {
            //LOGE("Error : %s file not found\n", filename);
            return false;
        }
        file_name = std::string(filename);

        YY_BUFFER_STATE state;
        state = FxParser_create_buffer(yyin, 16384/*YY_BUF_SIZE*/ );
        FxParser_switch_to_buffer(state);

        LOGD("Parsing...\n");
        int res = yyparse();

        FxParser_delete_buffer(state);
        fclose(yyin);

        // Now do a special thing : gather the created Uniforms 
        // and add them to the generic GLSLShader (un-named)
        Shader *pShader;
        for(int i=0; pShader = (Shader*)pCont->findShader(i); i++)
        {
            const char* name = pShader->getName();
            if((*name == '\0')&&(pShader->getType() == TGLSL))
            {
                LOGD("adding #define INFX to header ...\n");
    #ifndef OGLES2
                if(curContainer->separateShadersEnabled())
                {
                    LOGD("Using separable shaders...\n");
                    pShader->appendCode(
                    "#extension GL_ARB_separate_shader_objects : enable\n"
                    "#define INFX\n"
                    );
                } else
                    pShader->addHeaderCode("#define INFX\n");
    #else
                    pShader->addHeaderCode("#define INES2\n");
    #endif
                break;
            }
        }
        LOGD("building code from nvFX uniforms...\n");
        nvFX::buildCodeFromUniforms(pCont);
    #ifdef USECUDA
        LOGD("building code from nvFX uniforms for CUDA...\n");
        nvFX::buildCodeFromUniformsCUDA(pCont);
    #endif
        LOGD("\nDone\n");
        file_name.clear();
        return nParseErrors > 0 ? false : true;
    }

    } //namespace nvFX

    void FxParsererror(char const *s)
    {
        LOGE("%s(%d) : %s\n", file_name.c_str(), line_num, s);
        nParseErrors++;
    }

    extern "C" 
    {
        int FxParserwrap ( void )
        {
            return 1;
        }
    }

    GLenum getGLEnumValue(GLenum BisonEnum)
    {
        switch(BisonEnum)
        {
        case GLACCUM: return 0x0100;
        case GLLOAD: return 0x0101;
        case GLRETURN: return 0x0102;
        case GLMULT: return 0x0103;
        case GLADD: return 0x0104;
        case GLNEVER: return 0x0200;
        case GLLESS: return 0x0201;
        case GLEQUAL: return 0x0202;
        case GLGREATER: return 0x0204;
        case GLNOTEQUAL: return 0x0205;
        case GLALWAYS: return 0x0207;
        case GLLEQUAL: return 0x0203;
        case GLGEQUAL: return 0x0206;
        case GLKEEP: return 0x1E00;
        case GLUPPERLEFT: return 0x8CA2;
        case GLLOWERLEFT: return 0x8CA1;
        case GLFRONTLEFT: return 0x0400;
        case GLFRONTRIGHT: return 0x0401;
        case GLBACKLEFT: return 0x0402;
        case GLBACKRIGHT: return 0x0403;
        case GLFRONT: return 0x0404;
        case GLBACK: return 0x0405;
        case GLLEFT: return 0x0406;
        case GLRIGHT: return 0x0407;
        case GLFRONTANDBACK: return 0x0408;
        case GLCCW: return 0x0901;
        case GLCW: return 0x0900;
        case GLPOINT: return 0x1B00;
        case GLLINE: return 0x1B01;
        case GLFILL: return 0x1B02;
        case GLZERO: return 0;
        case GLONE: return 1;
        case GLSRCCOLOR: return 0x0300;
        case GLONEMINUSSRCCOLOR: return 0x0301;
        case GLSRCALPHA: return 0x0302;
        case GLONEMINUSSRCALPHA: return 0x0303;
        case GLDSTALPHA: return 0x0304;
        case GLONEMINUSDSTALPHA: return 0x0305;
        case GLDSTCOLOR: return 0x0306;
        case GLONEMINUSDSTCOLOR: return 0x0307;
        case GLSRCALPHASATURATE: return 0x0308;
        case GLCONSTANTCOLOR: return 0x8001;
        case GLONEMINUSCONSTANTCOLOR: return 0x8002;
        case GLCONSTANTALPHA: return 0x8003;
        case GLONEMINUSCONSTANTALPHA: return 0x8004;
        case GLFUNCADD: return 0x8006;
        case GLMIN: return 0x8007;
        case GLMAX: return 0x8008;
        case GLFUNCSUBTRACT: return 0x800A;
        case GLFUNCREVERSESUBTRACT: return 0x800B;
        case GLCLEAR: return 0x1500;
        case GLSET: return 0x150F;
        case GLCOPY: return 0x1503;
        case GLCOPYINVERTED: return 0x150C;
        case GLNOOP: return 0x1505;
        case GLINVERT: return 0x150A;
        case GLAND: return 0x1501;
        case GLNAND: return 0x150E;
        case GLOR: return 0x1507;
        case GLNOR: return 0x1508;
        case GLXOR: return 0x1506;
        case GLEQUIV: return 0x1509;
        case GLANDREVERSE: return 0x1502;
        case GLANDINVERTED: return 0x1504;
        case GLORREVERSE: return 0x150B;
        case GLORINVERTED: return 0x150D;

    /*------------------------------------------------------------------
      sampler-state token
      Using the same values as OpenGL spec
    */
        case GLTEXTUREMAGFILTER: return 0x2800;
        case GLTEXTUREMINFILTER: return 0x2801;
        case GLTEXTUREWRAPS: return 0x2802;
        case GLTEXTUREWRAPT: return 0x2803;
        case GLTEXTUREWRAPR: return 0x8072;

        case GLNEAREST: return 0x2600;
        case GLLINEAR: return 0x2601;
        case GLNEARESTMIPMAPNEAREST: return 0x2700;
        case GLLINEARMIPMAPNEAREST: return 0x2701;
        case GLNEARESTMIPMAPLINEAR: return 0x2702;
        case GLLINEARMIPMAPLINEAR: return 0x2703;
        case GLCLAMP: return 0x2900;
        case GLCLAMPTOEDGE: return 0x812F;
        case GLREPEAT: return 0x2901;
        case GLMIRROREDREPEAT: return 0x8370;

    /*------------------------------------------------------------------
    */
    //rasterization_state rasterState
        case GLPOINTSIZE: return 0x0B11;
        case GLPOINTSMOOTH: return 0x0B10;
        case GLPOINTSPRITE: return 0x8861;
        case GLPROGRAMPOINTSIZE: return 0x8642;
        case GLPOINTSPRITECOORDORIGIN: return 0x8CA0;
        case GLLINEWIDTH: return 0x0B21;
        case GLLINESMOOTH: return 0x0B20;
        //case GLCOMBINEDLINESTIPPLENV: return ??;
        case GLLINESTIPPLEPATTERN: return 0x0B25;
        case GLLINESTIPPLEREPEAT: return 0x0B26;
        case GLLINESTIPPLE: return 0x0B24;
        case GLCULLFACE: return 0x0B44;
        case GLCULLFACEMODE: return 0x0B45;
        case GLFRONTFACE: return 0x0B46;
        case GLPOLYGONSMOOTH: return 0x0B41;
        case GLPOLYGONMODE: return 0x0B40;
        case GLPOLYGONOFFSETFACTOR: return 0x8038;
        case GLPOLYGONOFFSETUNITS: return 0x2A00;
        case GLPOLYGONOFFSETPOINT: return 0x2A01;
        case GLPOLYGONOFFSETLINE: return 0x2A02;
        case GLPOLYGONOFFSETFILL: return 0x8037;
        case GLPOLYGONSTIPPLEBIT: return 0x00000010;
        case GLRASTERIZERDISCARD: return 0x8C89;

    //color_sample_state rasterState
        case GLALPHATEST: return 0x0BC0;
        case GLALPHATESTFUNC: return 0x0BC1;
        case GLALPHATESTREF: return 0x0BC2;
        case GLBLEND: return 0x0BE2;
        case GLBLENDSRC: return 0x0BE1;
        case GLBLENDSRCRGB: return 0x80C9;
        case GLBLENDSRCALPHA: return 0x80CB;
        case GLBLENDDST: return 0x0BE0;
        case GLBLENDDSTRGB: return 0x80C8;
        case GLBLENDDSTALPHA: return 0x80CA;
    //GL_BLEND_EQUATION_RGBA_NV
        case GLBLENDEQUATIONRGB: return 0x8009;
        case GLBLENDEQUATIONALPHA: return 0x883D;
    //GL_PER_DRAW_BUFFER_BLEND_FUNCTION_NV
    //GL_PER_DRAW_BUFFER_BLEND_EQUATION_NV
    //GL_PER_DRAW_BUFFER_BLEND_ENABLE_NV
    //GL_PER_DRAW_BUFFER_COLOR_WRITEMASK_NV
        case GLBLENDCOLOR: return 0x8005;
        case GLFRAMEBUFFERSRGB: return 0x8DB9;
        case GLDITHER: return 0x0BD0;
        case GLCOLORLOGICOP: return 0x0BF2;
        case GLLOGICOPMODE: return 0x0BF0;
        case GLCOLORWRITEMASK: return 0x0C23;
        case GLMULTISAMPLE: return 0x809D;
        case GLSAMPLEMASK: return 0x8E51;
        case GLSAMPLEMASKVALUE: return 0x8E52;
        case GLSAMPLEALPHATOONE: return 0x809F;
        case GLSAMPLEALPHATOCOVERAGE: return 0x809E;

    //dst_state dstState
        case GLDEPTHTEST: return 0x0B71;
        case GLDEPTHFUNC: return 0x0B74;
        case GLDEPTHWRITEMASK: return 0x0B72;
        case GLDEPTHCLAMP: return 0x864F;
        case GLDEPTHBOUNDSTESTEXT: return 0x8890;
        case GLDEPTHBOUNDSEXT: return 0x8891;
        case GLREPLACE: return 0x1E01;
        case GLINCR: return 0x1E02;
        case GLDECR: return 0x1E03;
        case GLSTENCILTEST: return 0x0B90;
        case GLSTENCILFUNC: return 0x0B92;
        case GLSTENCILVALUEMASK: return 0x0B93;
        case GLSTENCILREF: return 0x0B97;
        case GLSTENCILFAIL: return 0x0B94;
        case GLSTENCILPASSDEPTHFAIL: return 0x0B95;
        case GLSTENCILPASSDEPTHPASS: return 0x0B96;
        case GLSTENCILWRITEMASK: return 0x0B98;
    //GL_STENCIL_FRONT_FUNC
    //GL_STENCIL_FRONT_VALUE_MASK
    //GL_STENCIL_FRONT_REF
    //GL_STENCIL_FRONT_FAIL
    //GL_STENCIL_FRONT_PASS_DEPTH_FAIL
    //GL_STENCIL_FRONT_PASS_DEPTH_PASS
    //GL_STENCIL_FRONT_WRITEMASK
        case GLSTENCILBACKFUNC: return 0x8800;
        case GLSTENCILBACKVALUEMASK: return 0x8CA4;
        case GLSTENCILBACKREF: return 0x8CA3;
        case GLSTENCILBACKFAIL: return 0x8801;
        case GLSTENCILBACKPASSDEPTHFAIL: return 0x8802;
        case GLSTENCILBACKPASSDEPTHPASS: return 0x8803;
        case GLSTENCILBACKWRITEMASK: return 0x8CA5;

    // From NV Path rendering
        case GLFOGCOORDINATE: return 0x8451;
        case GLFRAGMENTDEPTH: return 0x8452;
        case GLOBJECTLINEAR: return 0x2401;
        case GLPATHOBJECTBOUNDINGBOX: return 0x908A;
        case GLEYELINEAR: return 0x2400;
        case GLFLAT: return 0x1D00;
        case GLSQUARE: return 0x90A3;
        case GLROUND: return 0x90A4;
        case GLTRIANGULAR: return 0x90A5;
        case GLMITERREVERT: return 0x90A7;
        case GLMITERTRUNCATE: return 0x90A8;
        case GLBEVEL: return 0x90A6;
        case GLMOVETORESET: return 0x90B5;
        case GLMOVETOCONTINUES: return 0x90B6;
        case GLCOUNTUP: return 0x9088;
        //case GLINVERT: return 0x150A;
        case GLCOUNTDOWN: return 0x9089;
        case GLBOUNDINGBOX: return 0x908A;
        case GLCONVEXHULL: return 0x908B;
        case GLTRANSLATE2D: return 0x9090;
        case GLTRANSLATEX: return 0x908E;
        case GLTRANSLATEY: return 0x908F;
        case GLTRANSLATE3D: return 0x9091;
        case GLAFFINE2D: return 0x9092;
        case GLAFFINE3D: return 0x9094;
        case GLTRANSPOSEAFFINE2D: return 0x9096;
        case GLTRANSPOSEAFFINE3D: return 0x9098;

        case GLPATHSTENCILFUNC:         return 0x90B7;
        case GLPATHFOGGEN:              return 0x90AC;
        case GLPATHTEXGEN:              return 0x90B0;
        case GLPATHCOLORGEN:            return 0x90B2;
        case GLPATHCOVERDEPTHFUNC:      return 0x90BF;
        case GLPATHSTROKEWIDTH:         return 0x9075;
        case GLPATHINITIALENDCAP:       return 0x9077;
        case GLPATHTERMINALENDCAP:      return 0x9078;
        case GLPATHINITIALDASHCAP:      return 0x907C;
        case GLPATHTERMINALDASHCAP:     return 0x907D;
        case GLPATHJOINSTYLE:           return 0x9079;
        case GLPATHMITERLIMIT:          return 0x907A;
        case GLPATHDASHOFFSET:          return 0x907E;
        case GLPATHDASHOFFSETRESET:     return 0x90B4;
        case GLPATHCLIENTLENGTH:        return 0x907F;
        case GLPATHFILLMODE:            return 0x9080;
        case GLPATHFILLMASK:            return 0x9081;
        case GLPATHFILLCOVERMODE:       return 0x9082;
        case GLPATHSTROKECOVERMODE:     return 0x9083;
        case GLPATHSTROKEMASK:          return 0x9084;

    default:
        break;
    }
    //OutputDebugString("Failed to find BisonEnum equivalent in OpenGL\n");
    return BisonEnum;
    }

    int *flattenIVecList(int t, int vecDim, IntVecList* pintVecList)
    {
        int szvec = 0;
        int *array = NULL;
        // walk through the vector list and check few things then send the packed version
        int *p = NULL;
        if (NULL == pintVecList)
          return 0;
        for(int i=0; i<(int)pintVecList->size(); i++)
        {
            if(szvec == 0) {
                szvec = (int)((*pintVecList)[i]->size());
                p = array = new int[szvec * pintVecList->size()]; // we assume all vectors are the same size
            }
            else if(szvec != (*pintVecList)[i]->size()) {
                yyerror("Vector list has inconsistent vectors\n");
                continue;
            }
            IntList* pfl = (*pintVecList)[i];
            if (NULL!=pfl)
            {
            for(int j=0; j<(int)pfl->size(); j++)
                *p++ = (*pfl)[j];
            delete pfl;
              pfl = NULL;
            }
        }
        if(vecDim < 0)
            vecDim = (int)pintVecList->size();
        // we do the test here in any case : the previous loop needed to do some "delete" anyways
        if((szvec != vecDim) || (t != pintVecList->size()))
        {
            yyerror("Vector dimension in value assignment don't match the type\n");
            delete array;
            return NULL;
        }
        return array;
    }
    bool *flattenBVecList(int t, int vecDim, BoolVecList* pboolVecList)
    {
        int szvec = 0;
        bool *array = NULL;
        // walk through the vector list and check few things then send the packed version
        bool *p = NULL;
        for(int i=0; i<(int)pboolVecList->size(); i++)
        {
            if(szvec == 0) {
                szvec = (int)((*pboolVecList)[i]->size());
                p = array = new bool[szvec * pboolVecList->size()]; // we assume all vectors are the same size
            }
            else if(szvec != (*pboolVecList)[i]->size()) {
                yyerror("Vector list has inconsistent vectors\n");
                continue;
            }
            BoolList* pfl = (*pboolVecList)[i];
            for(int j=0; j<(int)pfl->size(); j++)
                *p++ = (*pfl)[j];
            delete pfl;
        }
        if(vecDim < 0)
            vecDim = (int)pboolVecList->size();
        // we do the test here in any case : the previous loop needed to do some "delete" anyways
        if((szvec != vecDim) || (t != pboolVecList->size()))
        {
            yyerror("Vector dimension in value assignment don't match the type\n");
            delete array;
            return NULL;
        }
        return array;
    }

    float *flattenFVecList(int t, int vecDim, FloatVecList* pfloatVecList)
    {
      if (NULL == pfloatVecList)
        return false;
        int szvec = 0;
        int arraySz = 0;
        float *array = NULL;
        // walk through the vector list and check few things then send the packed version
        float *p = NULL;
        for(int i=0; i<(int)pfloatVecList->size(); i++)
        {
            if(szvec == 0) {
                szvec = (int)((*pfloatVecList)[i]->size());
                arraySz = szvec * pfloatVecList->size();
                p = array = new float[arraySz]; // we assume all vectors are the same size
            }
            else if(szvec != (*pfloatVecList)[i]->size()) {
                yyerror("Vector list has inconsistent vectors\n");
                continue;
            }
            FloatList* pfl = (*pfloatVecList)[i];
            if (NULL!=pfl)
            {
            for(int j=0; j<(int)pfl->size(); j++)
                *p++ = (*pfl)[j];
            delete pfl;
              pfl = NULL;
            }
        }
        if(vecDim < 0)
            vecDim = (int)pfloatVecList->size();
        // we do the test here in any case : the previous loop needed to do some "delete" anyways
        if((t * vecDim) != (arraySz))
        {
            yyerror("Vector dimension in value assignment don't match the type\n");
            delete array;
            return NULL;
        }
        return array;
    }


