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
#ifdef WIN32
#include <windows.h> // for OutputDebugString()
#endif
#include <stdarg.h> // Container::eprintf()

#include <fstream>
#include <assert.h>
#include "Fx.h"
#include "OpenGL_enums.h"
#pragma warning(disable:4996)
using namespace nvFX;

// to be part of the header for generated code
//bool createCstBuffer(const char*name, bool bGlobal)
//{
//    ICstBuffer* p = curContainer->createCstBuffer(name, bGlobal);
//    // it is possible that it failed because already a Global one with same name is here
//    if(p == NULL)
//    {
//        if($2)
//            p = pCRep->find(name);
//        if(p == NULL)
//            LOGE("Error in Constant Buffer creation\n");
//        else {
//            curContainer->addCstBuffer(p);
//            pCRep->addCstBuffer(p);
//            bCurCstBufferAlreadyPopulated = true; // we prevent sub-uniforms to be created during this parsing
//        }
//    } else {
//        bCurCstBufferAlreadyPopulated = false;
//    }
//    curCstBuffer = p ? p->getExInterface() : NULL;
//}

#define  P(...)  { fprintf(fd, __VA_ARGS__); }
#define  PL(...)  { fprintf(fd, __VA_ARGS__); fprintf(fd, "\n"); }

#define CASELIST\
    CASEUTYPE(TUndefined, Undefined)\
    CASEUTYPE(TInt, int)\
    CASEUTYPE(TInt2, int2)\
    CASEUTYPE(TInt3, int3)\
    CASEUTYPE(TInt4, int4)\
    CASEUTYPE(TBool, bool)\
    CASEUTYPE(TBool2, bool2)\
    CASEUTYPE(TBool3, bool3)\
    CASEUTYPE(TBool4, bool4)\
    CASEUTYPE(TFloat, float)\
    CASEUTYPE(TVec2, vec2)\
    CASEUTYPE(TVec3, vec3)\
    CASEUTYPE(TVec4, vec4)\
    CASEUTYPE(TMat2, mat2)\
    CASEUTYPE(TMat3, mat3)\
    CASEUTYPE(TMat4, mat4)
    //case TUBO : return "int"
    //TCB,  // Constant Buffer (D3D)
    //TUniform,
    //TTexture,
    //TFXTexture,
    //case TTexUnit : return "int",
    // TODO : add missing cases
#ifndef OGLES2
    //TSubroutineUniform,
    //TSubroutineUniformByID,
#endif

#undef  RESOURCEFORMATITEM
#define RESOURCEFORMATITEM(a) case a: return #a;
#undef  RESOURCEFORMATITEM_VAL
#define RESOURCEFORMATITEM_VAL(a,b) case a: return #a;
#ifdef OGLES2
#undef  RESOURCEFORMATITEM_ES
#define RESOURCEFORMATITEM_ES(a) case a: return #a;
#else
#define RESOURCEFORMATITEM_ES(a)
#endif

const char *formatAsString(ResourceFormat fmt)
{
    switch(fmt)
    {
        RESOURCEFORMATLIST
    }
    return "UNKNOWN";
}

const char* getTypeAsCharNVFX(IUniform::Type t)
{
    #define CASEUTYPE(a,b) case IUniform::a : return #b;
    switch(t)
    {
        CASELIST
    default:
        CHECK_TRUE_MSG(false,"Check what to do here");
        break;
    }
    return "**Error**";
}

const char* getTypeAsChar(IUniform::Type t)
{
    #undef  CASEUTYPE
    #define CASEUTYPE(a,b) case IUniform::a : return #a;
    switch(t)
    {
        CASELIST
    default:
        CHECK_TRUE_MSG(false,"Check what to do here");
        break;
    }
    return "**Error**";
}

void printCodeForCpp(FILE *fd, const char *codeStr)
{
    const char* p = codeStr;
    const char* p2 = codeStr;
    int sz;
    while(*p2 != '\0')
    {
        sz = 0;
        while((*p2 != '\n')&&(*p2 != '\0'))
        {
            sz++;
            p2++;
        }
        fprintf(fd, "\"");
        fwrite(p, 1, sz, fd);
        fprintf(fd, "\\n\"\n");
        p2++;
        p = p2;
    }
    fprintf(fd, ";\n");
}

#define SGRSCASEERR(s) case s : sprintf(sval, "0/*ERROR*/"); return #s;
#define SGRSCASE(s) case s : sprintf(sval, "0x%x", pS->values[0]); return #s;
#define SGRSCASE0(s) case s : sprintf(sval, "0x%x", pS->value); return #s;
#define SGRSCASE2(s) case s : sprintf(sval, "0x%x, 0x%x", pS->values[0], pS->values[1]); return #s;
#define SGRSCASEF(s) case s : sprintf(sval, "%f", pS->fvalue); return #s;
#define SGRSCASE1F(s) case s : sprintf(sval, "%f", pS->fvals[0]); return #s;
#define SGRSCASE2F(s) case s : sprintf(sval, "%f, %f", pS->fvals[0], pS->fvals[1]); return #s;
#define SGRSCASE4F(s) case s : sprintf(sval, "%f, %f, %f, %f", pS->fvals[0], pS->fvals[1], pS->fvals[2], pS->fvals[3]); return #s;
#define SGRSCASE4B(s) case s : sprintf(sval, "%s, %s, %s, %s", pS->bvals[0]?"true":"false", pS->bvals[1]?"true":"false", pS->bvals[2]?"true":"false", pS->bvals[3]?"true":"false"); return #s;
/*************************************************************************/ /**
 ** 
 ** 
 **/ /*************************************************************************/ 
static const char* getSGRState(StateGroupRaster::State* pS, char *sval, int sz)
{
    switch(pS->state)
    {
#ifndef OGLES2

    SGRSCASE(GL_POINT_SMOOTH)
    SGRSCASE(GL_POINT_SPRITE)
    SGRSCASE(GL_LINE_SMOOTH)
    SGRSCASE(GL_LINE_STIPPLE)
    SGRSCASE(GL_POLYGON_SMOOTH)
    SGRSCASE(GL_POLYGON_OFFSET_POINT)
    SGRSCASE(GL_POLYGON_OFFSET_LINE)
    SGRSCASE(GL_POLYGON_STIPPLE)
    SGRSCASE(GL_RASTERIZER_DISCARD)
#endif
    SGRSCASE(GL_POLYGON_OFFSET_FILL)
    SGRSCASE(GL_CULL_FACE)
#ifndef OGLES2
//    case GL_POINT_SPRITE_COORD_ORIGIN:
    SGRSCASE2(GL_POLYGON_MODE)
//    case GL_COMBINED_LINE_STIPPLE_NV:
//    case GL_LINE_STIPPLE_PATTERN:
//    case GL_LINE_STIPPLE_REPEAT:
    SGRSCASE1F(GL_POINT_SIZE)
#endif
    SGRSCASE(GL_CULL_FACE_MODE)
    SGRSCASE(GL_FRONT_FACE)
    SGRSCASE1F(GL_LINE_WIDTH)
    SGRSCASE2F(GL_POLYGON_OFFSET_FACTOR) // set GL_POLYGON_OFFSET_UNITS, too
    }
    sval[0] = '\0';
    return "Error in getSGRState()";
}

/*************************************************************************/ /**
 ** 
 ** 
 **/ /*************************************************************************/ 
static const char* getSGCSState(StateGroupCS::State* pS, char *sval, int sz)
{
    switch(pS->state)
    {
    //case GL_PER_DRAW_BUFFER_BLEND_FUNCTION_NV:
    //case GL_PER_DRAW_BUFFER_BLEND_EQUATION_NV:
    //case GL_PER_DRAW_BUFFER_BLEND_ENABLE_NV:
    //case GL_PER_DRAW_BUFFER_COLOR_WRITEMASK_NV:
#ifndef OGLES2
    SGRSCASE(GL_FRAMEBUFFER_SRGB)
    SGRSCASE(GL_COLOR_LOGIC_OP)
    SGRSCASE(GL_ALPHA_TEST)
    SGRSCASE(GL_MULTISAMPLE)
    SGRSCASE(GL_SAMPLE_MASK)
    SGRSCASE(GL_SAMPLE_ALPHA_TO_ONE)
#endif
    SGRSCASE(GL_DITHER)
    SGRSCASE(GL_SAMPLE_ALPHA_TO_COVERAGE)
    SGRSCASE(GL_BLEND)
#ifndef OGLES2
    SGRSCASEF(GL_ALPHA_TEST_REF) // in GL_ALPHA_TEST_FUNC
    SGRSCASE(GL_ALPHA_TEST_FUNC)
    SGRSCASE(GL_BLEND_SRC)
    SGRSCASE(GL_BLEND_DST)
#endif
    SGRSCASE(GL_BLEND_SRC_RGB)
    SGRSCASE(GL_BLEND_SRC_ALPHA)// in GL_BLEND_SRC_RGB
    SGRSCASE(GL_BLEND_DST_RGB)// in GL_BLEND_SRC_RGB
    SGRSCASE(GL_BLEND_DST_ALPHA)// in GL_BLEND_SRC_RGB
    //case GL_BLEND_EQUATION_RGBA_NV:
    SGRSCASE(GL_BLEND_EQUATION_RGB)
    SGRSCASE(GL_BLEND_EQUATION_ALPHA) // in GL_BLEND_EQUATION_RGB
#ifndef OGLES2
    SGRSCASE(GL_LOGIC_OP_MODE)
    //GL_SAMPLE_MASK_VALUE:
#endif
    SGRSCASE4B(GL_COLOR_WRITEMASK)
    SGRSCASE4F(GL_BLEND_COLOR)
    }
    sval[0] = '\0';
    return "Error in getSGCSState()";
}

static const char* getSGDSTState(StateGroupDST::State* pS, char *sval, int sz)
{
    switch(pS->state)
    {
    SGRSCASE0(GL_DEPTH_TEST)
#ifndef OGLES2
    SGRSCASE0(GL_DEPTH_CLAMP)
    SGRSCASE0(GL_DEPTH_BOUNDS_TEST_EXT)
#endif
    SGRSCASE0(GL_STENCIL_TEST)
    SGRSCASE0(GL_DEPTH_FUNC)
    SGRSCASE0(GL_DEPTH_WRITEMASK)
#ifndef OGLES2
    //GL_DEPTH_BOUNDS_EXT:
#endif
    //case GL_STENCIL_FRONT_REF:
    SGRSCASE0(GL_STENCIL_BACK_REF)//GL_STENCIL_BACK_FUNC
    SGRSCASE0(GL_STENCIL_REF) // in GL_STENCIL_FUNC
    SGRSCASE0(GL_STENCIL_BACK_VALUE_MASK)//GL_STENCIL_BACK_FUNC
    SGRSCASE0(GL_STENCIL_VALUE_MASK) // in GL_STENCIL_FUNC
    //case GL_STENCIL_FRONT_FUNC:
    SGRSCASE0(GL_STENCIL_FUNC)
    SGRSCASE0(GL_STENCIL_BACK_FUNC)
    SGRSCASE0(GL_STENCIL_PASS_DEPTH_PASS)
    SGRSCASE0(GL_STENCIL_PASS_DEPTH_FAIL)
    //case GL_STENCIL_FRONT_FAIL:
    SGRSCASE0(GL_STENCIL_FAIL)
    //case GL_STENCIL_BACK_PASS_DEPTH_PASS:
    SGRSCASE0(GL_STENCIL_BACK_FAIL)
    //case GL_STENCIL_FRONT_PASS_DEPTH_FAIL:
    SGRSCASE0(GL_STENCIL_BACK_PASS_DEPTH_FAIL)
    //case GL_STENCIL_FRONT_PASS_DEPTH_PASS:
    SGRSCASEERR(GL_STENCIL_BACK_WRITEMASK)
    SGRSCASEERR(GL_STENCIL_WRITEMASK)
    //case GL_STENCIL_FRONT_VALUE_MASK:
    //case GL_STENCIL_FRONT_WRITEMASK:
    }
    sval[0] = '\0';
    return "Error in getSGDSTState()";
}

void printfData(FILE* fd, Uniform::Type t, Uniform::ShadowedData *data, int sz)
{
    switch(t)
    {
    case IUniform::TInt:
    case IUniform::TInt2:
    case IUniform::TInt3:
    case IUniform::TInt4:
      {
        P("    int v[] = {")
        for(int i=0; i<sz*((int)t-(int)IUniform::TInt+1); i++)
        {
            P("%d", data->ivals[i])
            if(i < sz-1) P(", ")
        }
        PL("};")
        PL("    curUniform->setValueiv(v, %d, %d, 0); // (array, dim, arraysz, offset)",((int)t-(int)IUniform::TInt+1) ,sz)
        break;
      }
    case IUniform::TBool:
    case IUniform::TBool2:
    case IUniform::TBool3:
    case IUniform::TBool4:
      {
        P("    int v[] = {")
        for(int i=0; i<sz*((int)t-(int)IUniform::TBool+1); i++)
        {
            P("%d", data->ivals[i])
            if(i < sz-1) P(", ")
        }
        PL("};")
        PL("    curUniform->setValuebv(v, %d, %d, 0); // (array, dim, arraysz, offset)",((int)t-(int)IUniform::TBool+1) ,sz)
        break;
      }
    case IUniform::TFloat:
    case IUniform::TVec2:
    case IUniform::TVec3:
    case IUniform::TVec4:
      {
        P("    float v[] = {")
        for(int i=0; i<sz*((int)t-(int)IUniform::TFloat+1); i++)
        {
            P("%f", data->fvals[i])
            if(i < sz-1) P(", ")
        }
        PL("};")
        PL("    curUniform->setValuefv(v, %d, %d, 0); // (array, dim, arraysz, offset)",((int)t-(int)IUniform::TFloat+1) ,sz)
        break;
      }
    case IUniform::TMat2:
    case IUniform::TMat3:
    case IUniform::TMat4:
        PL("//TODO matrix default values !")
        break;
    default:
        PL("ERR")
    }
}

bool Annotation::serialize(FILE* fd, bool asCPP)
{
    if(m_floats.size() > 0)
    {
        Annotation::FloatMap::iterator it = m_floats.begin();
        while(it != m_floats.end())
        {
            PL("    pAnnot->getExInterface()->addAnnotation(\"%s\", %f)", it->first.c_str(), it->second);
            //it->second
            ++it;
        }
    }
    if(m_ints.size() > 0)
    {
        Annotation::IntMap::iterator it = m_ints.begin();
        while(it != m_ints.end())
        {
            PL("    pAnnot->getExInterface()->addAnnotation(\"%s\", %d)", it->first.c_str(), it->second);
            //it->second
            ++it;
        }
    }
    if(m_strings.size() > 0)
    {
        Annotation::StringMap::iterator it = m_strings.begin();
        while(it != m_strings.end())
        {
            PL("    pAnnot->getExInterface()->addAnnotation(\"%s\", \"%s\")", it->first.c_str(), it->second.c_str());
            //it->second
            ++it;
        }
    }
    return true;
}

bool Uniform::serialize(FILE* fd, bool asCPP)
{
    IUniformRepositoryEx* pURep = getUniformRepositorySingleton()->getExInterface();
    bool bGlobal = false;
    if(pURep->find(this))
        bGlobal = true;
    if(asCPP)
    {
        PL("    //========> Uniform %s", getName())
        PL("    {")
        P("    createUniform(%s, \"%s\"", getTypeAsChar(getType()), getName())
        const char* sem = getSemantic();
        if(sem && sem[0]!='\0')
            P(", %s", sem)
        else
            P(", NULL")
        PL(", %s); // (type, name, sem, bGlobal) => sets curUniform", bGlobal?"true":"false");
        if(m_data)
        {
            printfData(fd, getType(), m_data, m_arraySz);
        } else {
        }
        if((m_annotations.m_ints.size() > 0)
         ||(m_annotations.m_strings.size() > 0)
         ||(m_annotations.m_floats.size() > 0) )
            PL("    Annotation* pAnnot = curUniform->annotations();")
        m_annotations.serialize(fd, asCPP);
        PL("    }")
    } else {
        PL("uniform %s %s %s ", bGlobal?"global":"", getTypeAsCharNVFX(getType()), m_name.c_str())
    }
    //Annotation              m_annotations;
    //std::string             m_groupname; // essentially used when created from "Introspec"
    //std::string             m_semantic; // essentially used when created from "Introspec"
    //// TODO: write the code when some targets need to be removed
    //std::vector<STarget>    m_targets;
    //Type                    m_type;
    //int                     m_idx;
    //int                     m_arraySz;

    return true;
}

bool Container::serialize(FILE* fd, bool asCPP)
{
    //=========> m_name;
    PL("//================================================");
    PL("// Effect %s", m_name.c_str());
    PL("//================================================");
    //=========> m_cstBuffers; ///< array of constant buffers
    PL("//================================================");
    PL("//================== CstBuffers ==================");
    PL("//================================================");
    if(m_cstBuffers.size() > 0)
    {
        ICstBufferRepositoryEx* pCRep = getCstBufferRepositorySingleton()->getExInterface();
        CstBufferVec::iterator it = m_cstBuffers.begin();
        while(it != m_cstBuffers.end())
        {
            if(*it)
            {
                bool bGlobal = false;
                if(pCRep->find(*it)) // if in global area, could be still used by some other effects
                {
                    bGlobal = true;
                }
                PL("    if(createCstBuffer(%s, %s)) // (name,bGlobal) => sets curCstBuffer", (*it)->getName(), bGlobal?"true":"false");
                PL("    {")
                CstBuffer::UniformVec::iterator itu = (*it)->m_uniforms.begin();
                while(itu != (*it)->m_uniforms.end())
                {
                    (*itu)->serialize(fd, asCPP);
                    ++itu;
                }
                PL("    curCstBuffer = NULL");
                PL("    }//createCstBuffer()");
            }
            ++it;
        }
    }
    //=========> m_uniforms; ///< map of uniforms out of any cst buffer
    PL("//================================================");
    PL("//================== Uniforms ====================");
    PL("//================================================");
    if(m_uniforms.size() > 0)
    {
        // TODO: we might want to sort them on m_groupname... later if needed
        UniformMap::iterator it = m_uniforms.begin();
        while(it != m_uniforms.end())
        {
            if(it->second)
            {
                it->second->serialize(fd, asCPP);
            }
            ++it;
        }
        PL("    curUniform = NULL")
    }
    const char *    stateName;
    char            sValue[50];
    if(m_stateGroupRasters.size() > 0)
    {
        PL("//================================================");
        PL("//================== State Groups Raster =========");
        PL("//================================================");
        StateGroupRasterVec::iterator it = m_stateGroupRasters.begin();
        while(it != m_stateGroupRasters.end())
        {
            if(*it) {
                StateGroupRaster* pSG = *it;
                if(asCPP)
                {
                    PL("    {");
                    PL("    curRState = curContainer->createStateGroup_Raster(%s)->getExInterface();", pSG->getName());
                    StateGroupRaster::State* pS;
                    for(int i=0; pS = pSG->getState(i); i++)
                    {
                        stateName = getSGRState(pS, sValue, 49);
                        PL("    if(!curRState->createState(%s, %s)) LOGE(\"Failed to create state\");", stateName, sValue)
                    }
                    PL("    }");
                }
            }
            ++it;
        }
        PL("    curRState = NULL;");
    }
    if(m_stateGroupCSs.size() > 0)
    {
        PL("//================================================");
        PL("//================== State Groups CS    ==========");
        PL("//================================================");
        StateGroupCSVec::iterator it = m_stateGroupCSs.begin();
        while(it != m_stateGroupCSs.end())
        {
            if(*it){
                StateGroupCS* pSG = *it;
                if(asCPP)
                {
                    PL("    {");
                    PL("    curCSState = curContainer->createStateGroup_CS(%s)->getExInterface();", pSG->getName());
                    StateGroupCS::State* pS;
                    for(int i=0; pS = pSG->getState(i); i++)
                    {
                        stateName = getSGCSState(pS, sValue, 49);
                        PL("    if(!curCSState->createState(%s, %s)) LOGE(\"Failed to create state\");", stateName, sValue)
                    }
                    PL("    }");
                }
            }
            ++it;
        }
        PL("    curCSState = NULL;");
    }
    if(m_stateGroupDSTs.size() > 0)
    {
        PL("//================================================");
        PL("//================== State Groups DST   ==========");
        PL("//================================================");
        StateGroupDSTVec::iterator it = m_stateGroupDSTs.begin();
        while(it != m_stateGroupDSTs.end())
        {
            if(*it) {
                StateGroupDST* pSG = *it;
                if(asCPP)
                {
                    PL("    {");
                    PL("    curDSTState = curContainer->createStateGroup_DST(%s)->getExInterface();", pSG->getName());
                    StateGroupDST::State* pS;
                    for(int i=0; pS = pSG->getState(i); i++)
                    {
                        stateName = getSGDSTState(pS, sValue, 49);
                        PL("    if(!curDSTState->createState(%s, %s)) LOGE(\"Failed to create state\");", stateName, sValue)
                    }
                    PL("    }");
                }
            }
            ++it;
        }
        PL("    curDSTState = NULL;");
    }
#ifndef OGLES2
    if(m_stateGroupPRs.size() > 0)
    {
        StateGroupPRVec::iterator i11 = m_stateGroupPRs.begin();
        while(i11 != m_stateGroupPRs.end())
        {
            if(*i11) {
                PL("    //Path Rendering TODO !! (%s)\n", (*i11)->getName() );
            }
            ++i11;
        }
    }
#endif

    //=========> m_resources;
    PL("//================================================");
    PL("//================== Resources ==============");
    PL("//================================================");
    if(m_resources.size() > 0)
    {
        ResourceRepository* pResRep = 
            static_cast<ResourceRepository*>(nvFX::getResourceRepositorySingleton());
        ResourceVec::iterator it = m_resources.begin();
        while(it != m_resources.end())
        {
            Resource* pRes = *it;
            if(pRes)
            {
                if(asCPP)
                {
                    const char* pResTypeStr;
                    switch(pRes->getType())
                    {
                    case RESTEX_1D:         pResTypeStr = "RESTEX_1D"; break;
                    case RESTEX_2D:         pResTypeStr = "RESTEX_2D"; break;
                    case RESTEX_3D:         pResTypeStr = "RESTEX_3D"; break;
                    case RESTEX_2DRECT:     pResTypeStr = "RESTEX_2DRECT"; break;
                    case RESTEX_CUBE_MAP:   pResTypeStr = "RESTEX_CUBE_MAP"; break;
                    case RESRBUF_2D:        pResTypeStr = "RESRBUF_2D"; break;
                    default:
                    case RESOURCE_UNKNOWN:
                        pResTypeStr = "//Error : resource unknown";
                        PL("**ERROR**")
                        break;
                    }
                    const char* bGlobalStr = "false";
                    if(pResRep->find(pRes->getName()))
                        bGlobalStr = "true";
                    PL("    curResource = curContainer->createResource(%s, %s, %s)->getExInterface();", pRes->getName(), pResTypeStr, bGlobalStr)
                    PL("    if(curResource->getFormat() != FMT_UNKNOWN) curResource = NULL; // if not unknown, means it was already created.")
                    PL("    else {")
                    const char* fmtStr = formatAsString(pRes->getFormat());
                    PL("    curResource->setFormat(%s)", fmtStr)
                    if(pRes->getDimensionsApplicationDependent())
                        PL("    curResource->setDimensionsApplicationDependent(true);")
                    else {
                        int w = pRes->getWidth();
                        int h = pRes->getHeight();
                        int d = 1;//pRes->getDepth(); TODO
                        PL("    curResource->setDimensions(%d, %d, %d);", w, h, d)
                    }
                    float sw, sh;
                    pRes->getDimensionFactor(sw, sh);
                    if((sw != 1.0f)||(sh != 1.0f))
                        PL("    curResource->setDimensionFactor(%f, %f);", sw, sh)
                    ISamplerState* pState = pRes->getSamplerState();
                    if(pState)
                    {
                        PL("    ISamplerState * pState = curContainer->findSamplerState(\"%s\");", pState->getName() )
                        PL("    if(!pState) LOGE(\"Error: sampler-state referenced not found. Did you declare it before the texture using it ?\n\");")
                        PL("    curResource->setSamplerState(pState);")
                    }
                    if(pRes->getMSAAApplicationDependent())
                        PL("curResource->setMSAAApplicationDependent(true);")
                    else {
                        int d, s;
                        pRes->getMSAA(d,s);
                        PL("    curResource->setMSAA(%d, %d);", d, s)
                    }
                    IResource* pResResolve = pRes->getResolveTarget();
                    if(pResResolve)
                    {
                        PL("    IResource* pRT = curContainer->findResource(\"%s\");", pResResolve->getName() )
                        PL("    curResource->setResolveTarget(pRT);")
                    }
                    if((pRes->m_annotations.m_ints.size() > 0)
                     ||(pRes->m_annotations.m_strings.size() > 0)
                     ||(pRes->m_annotations.m_floats.size() > 0) )
                        PL("    Annotation* pAnnot = curResource->annotations();")
                    pRes->m_annotations.serialize(fd, asCPP);
                    PL("    }")
                } else {
                    PL("//TODO")
                }
            }
            ++it;
        }
        PL("    curResource = NULL;")
    }
    //=========> m_fbos;
    PL("//================================================");
    PL("//================== FBOs ========================");
    PL("//================================================");
    if(m_fbos.size() > 0)
    {
        FrameBufferObjectsRepository* pFboRep = 
            static_cast<FrameBufferObjectsRepository*>(nvFX::getFrameBufferObjectsRepositorySingleton());
        FboVec::iterator it = m_fbos.begin();
        while(it != m_fbos.end())
        {
            FrameBufferObject* pFBO = *it;
            if(asCPP)
            {
                int n;
                bool bGlobal = pFboRep->find(pFBO->getName()) ? true:false;
                PL("    curFBO = curContainer->createFBO(%d, %s)->getExInterface();", pFBO->getName(), bGlobal ? "true":"false")
                PL("    if(curFBO->getDSTResource() || curFBO->getColorResource(0))")
                PL("        curFBO = NULL; // we already have it defined. Don't want to re-define it again")
                for(n=0; (pFBO->getColorResource(n)) && (n<4/*MAX_RENDERTARGETS*/); n++);
                PL("    {")
                PL("    IResource* pRes[%d];", n)
                PL("    bool ok = true;")
                for(int i=0; i<n; i++)
                {
                    Resource* pRes = static_cast<Resource*>(pFBO->getColorResource(i));
                    PL("    pRes[%d] = curContainer->findResource(\"%s\");", i, pRes->getName() )
                    PL("    if(pRes[%d]==NULL) ok = false;",i)
                }
                PL("    if(ok) curFBO->setColorResource(pRes, %d);", n)
                PL("    else LOGE(\"Error while adding render targets in FBO\");")
                Resource* pRes = static_cast<Resource*>(pFBO->getDSTResource());
                PL("    pRes[0] = curContainer->findResource(\"%s\");", pRes->getName() )
                PL("    if(pRes[0]==NULL) LOGE(\"Error while adding render targets in FBO\");")
                PL("    else curFBO->setDSTResource(pRes[0]);")
                PL("    }")
                if((pFBO->m_annotations.m_ints.size() > 0)
                 ||(pFBO->m_annotations.m_strings.size() > 0)
                 ||(pFBO->m_annotations.m_floats.size() > 0) )
                    PL("    Annotation* pAnnot = curFBO->annotations();")
                pFBO->m_annotations.serialize(fd, asCPP);
            }
            ++it;
        }
        PL("    curFBO = NULL")
    }

    //=========> m_samplerStates;
    PL("//================================================");
    PL("//================== Sampler-states ==============");
    PL("//================================================");
    if(m_samplerStates.size() > 0)
    {
        SStateVec::iterator it = m_samplerStates.begin();
        while(it != m_samplerStates.end())
        {
            if(*it) {
                SamplerState* pSState = *it;
                if(asCPP)
                {
                    PL("    curSamplerState = curContainer->createSamplerState(\"%s\")->getExInterface();", pSState->getName())
                    SamplerState::StateVec::iterator is = pSState->m_states.begin();
                    while(is != pSState->m_states.end())
                    {
                        const char* ssEnumStr;
                        switch((*is)->sstate)
                        {
                        case GL_TEXTURE_MIN_FILTER: ssEnumStr = "GL_TEXTURE_MIN_FILTER"; break;
                        case GL_TEXTURE_MAG_FILTER: ssEnumStr = "GL_TEXTURE_MAG_FILTER";break;
                        case GL_TEXTURE_WRAP_S: ssEnumStr = "GL_TEXTURE_WRAP_S";break;
                        case GL_TEXTURE_WRAP_T: ssEnumStr = "GL_TEXTURE_WRAP_T";break;
                        case GL_TEXTURE_WRAP_R: ssEnumStr = "GL_TEXTURE_WRAP_R";break;
                        default: ssEnumStr = "WRONG Enum"; 
                            PL("**ERROR**")
                            break;
                        }
                        PL("        curSamplerState->getExInterface()->createState(0x%x/*%s*/, 0x%x);", (*is)->value, ssEnumStr, (*is)->value);
                        ++is;
                    }
                }
            }
            ++it;
        }
        PL("    curSamplerState = NULL");
    }

    //=========> m_shaders;
    PL("//================================================");
    PL("//================== Shader modules ==============");
    PL("//================================================");
    if(m_shaders.size() > 0)
    {
        IShaderModuleRepository* pShdRep = getShaderModuleRepositorySingleton();
        ShaderVec::iterator it = m_shaders.begin();
        // First : the ones with no name : better to process them ahead (like in a nvFX file)
        PL("//==================> Global (un-named) Shaders");
        while(it != m_shaders.end())
        {
            if(*it)
            {
                Shader* pShader = *it;
                if(asCPP)
                {
                    bool bGlobal = false;
                    const char* shdName = pShader->getName();
                    if(shdName[0] == '\0')
                    {
                        const char* codeStr = pShader->getShaderCode();
                        // need to pass on the generated code created for uniforms and constants
                        if(strncmp("//nvFX-Generated\n", codeStr, 16))
                        {
                            PL("    if(curShader = curContainer->createGLSLShader()->getExInterface())")
                            PL("    {")
                            PL("        if(curShader->getShaderCodeSz() == 0) { // ==0 means it needs to have some code loaded now")
                            PL("            static const char *shaderCodeStr = ")
                            printCodeForCpp(fd, codeStr);
                            PL("            // TODO : line # and file name can be specified withing this c++ file. Could help")
                            PL("            curShader->loadShaderCode(shaderCodeStr, /*line_num*/0, \"cpp-generated file\" );")
                            PL("        } else")
                            PL("            LOGI(\"Skipping the shader code of %s : some is already loaded\\n\");", pShader->getName())
                            PL("    }")
                        }
                    }
                }
            }
            ++it;
        }
        PL("//==================> Named Shaders");
        // Now, deal with named shaders
        it = m_shaders.begin();
        while(it != m_shaders.end())
        {
            if(*it)
            {
                Shader* pShader = *it;
                if(asCPP)
                {
                    const char* shdName = pShader->getName();
                    if(shdName[0] != '\0')
                    {
                        bool bGlobal = false;
                        if(pShdRep->findShader(shdName))
                            bGlobal = true;
                        PL("    if(curShader = curContainer->createGLSLShader(\"%s\", %s)->getExInterface())", pShader->getName(), bGlobal ? "true":"false")
                        PL("    {")
                        PL("        if(curShader->getShaderCodeSz() == 0) { // ==0 means it needs to have some code loaded now")
                        PL("            static const char *shaderCodeStr = ")
                        const char* codeStr = pShader->getShaderCode();
                        printCodeForCpp(fd, codeStr);
                        PL("            // TODO : line # and file name can be specified withing this c++ file. Could help")
                        PL("            curShader->loadShaderCode(shaderCodeStr, /*line_num*/0, \"cpp-generated file\" );")
                        PL("        } else")
                        PL("            LOGI(\"Skipping the shader code of %s : some is already loaded\\n\");", pShader->getName())
                        PL("    }")
                    }
                }
            }
            ++it;
        }
    }
    //=========> m_shaderprograms;
    //=========> m_techniques;
    PL("//================================================");
    PL("//================== Techniques ==================");
    PL("//================================================");
    if(m_techniques.size() > 0)
    {
        TechVec::iterator it = m_techniques.begin();
        while(it != m_techniques.end())
        {
            ++it;
        }
    }
    return true;
}

bool     Container::serializeToCpp(const char* fname)
{
    FILE* fd = fopen(fname, "w");
    if(!fd)
        return false;
    //
    // write a header for few required helpers
    //
    P(
    "#include \"FxLibEx.h\"\n"
    "#ifdef DECLNVFXHELPERS\n"
    "//\n"
    "// variables maintained during effect creation\n"
    "//\n"
    "IContainerEx*       curContainer = NULL;\n"
    "ITechniqueEx*       curTechnique = NULL;\n"
    "IPassEx*            curPass = NULL;\n"
    "IShaderEx*          curShader = NULL;\n"
    "IProgramEx*         curProgram = NULL;\n"
    "ISamplerStateEx*    curSamplerState = NULL;\n"
    "IStateGroupRasterEx* curRState = NULL;\n"
    "IStateGroupCSEx*    curCSState = NULL;\n"
    "IStateGroupDSTEx*   curDSTState = NULL;\n"
    "IStateGroupPathEx*  curPRState = NULL;\n"
    "ITextureEx*         curTexture = NULL;\n"
    "IResourceEx*        curResource = NULL;\n"
    "IFrameBufferObjectEx* curFBO = NULL;\n"
    "IUniformEx*         curUniform = NULL;\n"
    "ICstBufferEx*       curCstBuffer = NULL;\n"
    "bool                bCurCstBufferAlreadyPopulated = false; // true to prevent creation of sub-uniforms\n"
    "std::string         curGroupName; // UniformGroup group\n"
    "IShaderModuleRepositoryEx* pShdRep = NULL;\n"
    "IFrameBufferObjectsRepositoryEx* pFboRep = NULL;\n"
    "IResourceRepositoryEx* pResRep = NULL;\n"
    "IUniformRepositoryEx* pURep = NULL;\n"
    "ICstBufferRepositoryEx* pCRep = NULL;\n\n"
    "//\n"
    "// helper to create a uniform\n"
    "//\n"
    "void createUniform(nvFX::IUniform::Type T, const char* name, const char* sem, bool global=true)\n"
    "{\n"
    "    extern void yyerror(char  const  *);\n"
    "    IUniform *p = NULL;\n"
    "    if(curCstBuffer) // if a constant buffer is available, populate it\n"
    "    {\n"
    "        // only if allowed (if it came from an existing Global one, we don't want to add anything\n"
    "        if(bCurCstBufferAlreadyPopulated) {\n"
    "            LOGD(\"Skipping the sub-uniform of the constant\\n\");\n"
    "            p = curCstBuffer->findUniform(name);\n"
    "            if(p == NULL)\n"
    "                yyerror(\"Declaration of the CstBuffer differs from the one already in global area\\n\");\n"
    "        } else {\n"
    "            p = curCstBuffer->getExInterface()->createUniform(name, sem);\n"
    "            p->getExInterface()->setType(T);\n"
    "        }\n"
    "    } else { // otherwise just make a regular uniform creation at the root of the effect\n"
    "        p = curContainer->createUniform(name, curGroupName.c_str(), sem, global);\n"
    "        if(p)\n"
    "            p->getExInterface()->setType(T);\n"
    "        else { // failed. Try to find it in the global repository\n"
    "            if(global)\n"
    "                p = pURep->find(name);\n"
    "            if(p==NULL)\n"
    "                yyerror(\"failed to create global uniform\n\");\n"
    "            else if(p->getType() != T)\n"
    "                yyerror(\"uniform differs in its type the the global instance of the same name. Make it local ?\\n\");\n"
    "            else {\n"
    "                curContainer->addUniform(p);\n"
    "                pURep->addUniform(p);\n"
    "            }\n"
    "        }\n"
    "    }\n"
    "    curUniform = p ? p->getExInterface() : NULL;\n"
    "}\n"
    "#else\n"
    "extern IContainerEx*       curContainer;\n"
    "extern ITechniqueEx*       curTechnique;\n"
    "extern IPassEx*            curPass;\n"
    "extern IShaderEx*          curShader;\n"
    "extern IProgramEx*         curProgram;\n"
    "extern ISamplerStateEx*    curSamplerState;\n"
    "extern IStateGroupRasterEx* curRState;\n"
    "extern IStateGroupCSEx*    curCSState;\n"
    "extern IStateGroupDSTEx*   curDSTState;\n"
    "extern IStateGroupPathEx*  curPRState;\n"
    "extern ITextureEx*         curTexture;\n"
    "extern IResourceEx*        curResource;\n"
    "extern IFrameBufferObjectEx* curFBO;\n"
    "extern IUniformEx*         curUniform;\n"
    "extern ICstBufferEx*       curCstBuffer;\n"
    "extern bool                bCurCstBufferAlreadyPopulated; // true to prevent creation of sub-uniforms\n"
    "extern std::string         curGroupName; // UniformGroup group\n"
    "extern IShaderModuleRepositoryEx* pShdRep;\n"
    "extern IFrameBufferObjectsRepositoryEx* pFboRep;\n"
    "extern IResourceRepositoryEx* pResRep;\n"
    "extern IUniformRepositoryEx* pURep;\n"
    "extern ICstBufferRepositoryEx* pCRep;\n"
    "extern void createUniform(nvFX::IUniform::Type T, const char* name, const char* sem, bool global=true)\n"
    "#endif\n\n"
    )
    //
    // Write the effect
    //
    return serialize(fd, true);
}

bool     Container::serializeToNvFX(const char* fname)
{
    FILE* fd = fopen(fname, "w");
    if(!fd)
        return false;
    return serialize(fd, false);
}

