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
#   define  LOGI(...)  { printf(__VA_ARGS__); printf("\n"); }
#   define  LOGE(...)  { printf(">>Error : "); printf(__VA_ARGS__); printf("\n"); }
#ifdef WIN32
#include <windows.h> // for OutputDebugString
#endif
#include <fstream>
#include <assert.h>
#include "Fx.h"
#include "FxGL.h"
#pragma warning(disable:4996)
using namespace nvFX;

namespace nvFX
{
/*************************************************************************
 **
 **
 **
 *************************************************************************/ 
Resource* new_Resource(ResourceRepository* pCont, const char* name)
{
    return new ResourceGL(pCont, name);
}
void      delete_Resource(Resource *p)
{
    delete p;
}
//FrameBufferObject*  new_FrameBufferObject(FrameBufferObjectsRepository *pCont, const char* name)
//{
//    return new FrameBufferObjectGL(pCont, name);
//}
//void delete_FrameBufferObject(FrameBufferObject *p)
//{
//    FrameBufferObjectGL *pP = static_cast<FrameBufferObjectGL *>(p);
//    delete pP;
//}
ProgramPipeline*    new_ProgramPipeline(Container *pCont)
{
    return new GLSLProgramPipeline(pCont);
}
void                delete_ProgramPipeline(ProgramPipeline *p)
{
    GLSLProgramPipeline *pP = static_cast<GLSLProgramPipeline *>(p);
    delete pP;
}
/*ShaderProgram*      new_ShaderProgram(Container *pCont, ShaderType type, int numShaders, IShader ** pShaders)
{
    return new GLSLShaderProgram(pCont, type, numShaders, pShaders);
}

void                delete_ShaderProgram(ShaderProgram *p)
{
    GLSLShaderProgram *pP = static_cast<GLSLShaderProgram *>(p);
    delete pP;
}*/

CstBuffer *new_CstBuffer(const char* name)
{
    return new CstBufferGL(name);
}
void delete_CstBuffer(CstBuffer *p)
{
    CstBufferGL *pS = static_cast<CstBufferGL *>(p);
    delete pS;
}

SamplerState *new_SamplerState(Container *pCont, const char* name)
{
    return new SamplerStateGL(pCont, name);
}
void delete_SamplerState(SamplerState *p)
{
    SamplerStateGL *pS = static_cast<SamplerStateGL *>(p);
    delete pS;
}

StateGroupRaster *new_StateGroupRaster(Container *pCont, const char* name)
{
    return new StateGroupRasterGL(pCont, name);
}
void delete_StateGroupRaster(StateGroupRaster *p)
{
    StateGroupRasterGL *pGL = static_cast<StateGroupRasterGL *>(p);
    delete pGL;
}

StateGroupCS *new_StateGroupCS(Container *pCont, const char* name)
{
    return new StateGroupCSGL(pCont, name);
}
void delete_StateGroupCS(StateGroupCS *p)
{
    StateGroupCSGL *pGL = static_cast<StateGroupCSGL *>(p);
    delete pGL;
}

StateGroupDST *new_StateGroupDST(Container *pCont, const char* name)
{
    return new StateGroupDSTGL(pCont, name);
}
void delete_StateGroupDST(StateGroupDST *p)
{
    StateGroupDSTGL *pGL = static_cast<StateGroupDSTGL *>(p);
    delete pGL;
}

#ifndef OGLES2
StateGroupPath *new_StateGroupPR(Container *pCont, const char* name)
{
    return new StateGroupPathGL(pCont, name);
}
void delete_StateGroupPR(StateGroupPath *p)
{
    StateGroupPathGL *pGL = static_cast<StateGroupPathGL *>(p);
    delete pGL;
}
#endif

Program* new_Program(Container *pCont)
{
    return new GLSLProgram(pCont);
}
Program* new_ProgramPath(Container *pCont)
{
#ifndef OGLES2
    return new PathProgram(pCont);
#else
    return NULL;
#endif
}
void delete_Program(IProgram *pProg)
{
    GLSLProgram *p = static_cast<GLSLProgram *>(pProg);
    delete p;
}

Shader* new_ShaderPath(const char* name, bool bPostscript)
{
#ifndef OGLES2
    return new PathShader(name, bPostscript);
#else
    return NULL;
#endif
}
Shader* new_GLSLShader(const char* name)
{
    return new GLSLShader(name);
}
Shader* new_HLSL10Shader(const char* name)
{
    return NULL;
}
Shader* new_HLSL11Shader(const char* name)
{
    return NULL;
}
void delete_Shader(IShader *pShader)
{
    delete pShader;
}

Uniform* new_Uniform(const char* name, const char* groupname, const char* semantic)
{
    UniformGLSL *pUniform = new UniformGLSL(name, groupname, semantic);
    return pUniform;
}
void delete_Uniform(Uniform *pUniform)
{
    delete pUniform;
}

/*************************************************************************
 **
 **
 **
 *************************************************************************/ 
bool checkGLError(const char *str)
{
    //char tmp[200];
    GLenum e = glGetError();
    if(GL_NO_ERROR == e)
        return false;
    switch(e)
    {
    case GL_INVALID_ENUM:
        nvFX::printf("%s : Invalid Enum\n", str);
        break;
    case GL_INVALID_VALUE:
        nvFX::printf("%s : Invalid Value\n", str);
        break;
    case GL_INVALID_OPERATION:
        nvFX::printf("%s : Invalid Operation\n", str);
        break;
#if defined(WIN32) && !defined(ES2EMU)
    case GL_STACK_OVERFLOW:
        nvFX::printf("%s : Stack Overflow\n", str);
        break;
    case GL_STACK_UNDERFLOW:
        nvFX::printf("%s : Stack Underflow\n", str);
        break;
#endif
    case GL_OUT_OF_MEMORY:
        nvFX::printf("%s : OUT of Memory\n", str);
        break;
    default:
        nvFX::printf("%s : Unknown Error %x\n", str, e);
        return true;
    }
    return true;
}

/*************************************************************************
 **
 **
 **
 *************************************************************************/ 
std::string buildCodeForThisUniform(IContainer* pCont, Uniform* pUniform)
{
    std::string code;
    const char * name = pUniform->getName();
    code += "uniform ";
#ifdef OGLES2
    IUniform::PrecisionType precision = pUniform->getPrecision();
    switch(precision)
    {
        case IUniform::PHighP: code += "highp "; break;
        case IUniform::PMediumP: code += "mediump "; break;
        case IUniform::PLowP: code += "lowp "; break;
    }
#endif
    switch(pUniform->getType())
    {
        case IUniform::TInt: code += "int "; break;
        case IUniform::TInt2: code += "ivec2 "; break;
        case IUniform::TInt3: code += "ivec3 "; break;
        case IUniform::TInt4: code += "ivec4 "; break;
        case IUniform::TBool: code += "bool "; break;
        case IUniform::TBool2: code += "bvec2 "; break;
        case IUniform::TBool3: code += "bvec3 "; break;
        case IUniform::TBool4: code += "bvec4 "; break;
        case IUniform::TFloat: code += "float "; break;
        case IUniform::TVec2: code += "vec2 "; break;
        case IUniform::TVec3: code += "vec3 "; break;
        case IUniform::TVec4: code += "vec4 "; break;
        case IUniform::TMat2: code += "mat2 "; break;
        case IUniform::TMat3: code += "mat3 "; break;
        case IUniform::TMat4: code += "mat4 "; break;
        // texture part. Obviously we don't expect them in constant buffers but let's use buildCodeForThisUniform for this anyways
        case IUniform::TTexture1D: code += "sampler1D "; break;
        case IUniform::TTexture2D: code += "sampler2D "; break;
        case IUniform::TTexture2DRect: code += "sampler2DRect "; break;
        case IUniform::TTexture3D: code += "sampler3D "; break;
        case IUniform::TTextureCube: code += "samplerCube "; break;
          if(pUniform->getShadowedData())
          {
              
              Resource* pRes = pUniform->getShadowedData()->tex.pRes;
              if(pRes)
                switch(pRes->getType())
                {
                case IUniform::TTexture1D: code += "sampler1D "; break;
                case IUniform::TTexture2D: code += "sampler2D "; break;
                case IUniform::TTexture2DRect: code += "sampler2DRect "; break;
                case IUniform::TTexture3D: code += "sampler3D "; break;
                case IUniform::TTextureCube: code += "samplerCube "; break;
                default: code += "sampler Type Not implemented..."; break;
              }
              else code += "No resource attached to sampler "; break;
          }
          else code += "No data attached to sampler "; break;
    }
    code += name;
    int sz = pUniform->getArraySz();
    if(sz > 1)
    {
        char tmp[50]; sprintf(tmp, "[%d]", sz);
        code += tmp;
    }
    // NO need to append the default value : this will be done through this Uniform anyways
    code += ";\n";
    return code;
}

/*************************************************************************
 **
 **
 **
 *************************************************************************/ 
bool buildCodeFromUniforms(IContainer* pCont)
{
    Uniform *pUniform;
    std::string code;
    code = std::string("//nvFX-Generated\n");
    //
    // Walk through uniform parameters in GLOBAL space
    //
    for(int i=0; pUniform = static_cast<Uniform*>(pCont->findUniform(i)); i++)
    {
        const char * name = pUniform->getName();
        // IF the uniform has '.' : we declared a member of a struct. We can't append it to GLSL !
        if(strchr(name, '.'))
            continue;
        code += buildCodeForThisUniform(pCont, pUniform);
    }
    //
    // Walk through uniform parameters in CstBuffers
    //
    ICstBuffer *pCstBuffer;
    for(int i=0; pCstBuffer = pCont->findCstBuffer(i); i++)
    {
        // Do we want to allow various layout ?!?
#ifndef OGLES2
        code += "layout(std140) uniform ";
        code += pCstBuffer->getName();
        code += " {\n";
#endif
        for(int i=0; pUniform = static_cast<Uniform*>(pCstBuffer->findUniform(i)); i++)
        {
            code += buildCodeForThisUniform(pCont, pUniform);
        }
#ifndef OGLES2
        code += " };\n";
#endif
    }
    Shader *pShader = static_cast<Shader*>(static_cast<Container*>(pCont)->createGLSLShader());
    return pShader->loadShaderCode(code.c_str());//"#define INFX\n");
}

/*************************************************************************
 **
 **  NA on OpenGL
 **
 *************************************************************************/ 
void* Pass::getD3DIASignature()
{
    return NULL;
}
int Pass::getD3DIASignatureSize()
{
    return 0;
}

/*************************************************************************
 **
 ** Pass Pass Pass Pass Pass Pass Pass Pass Pass Pass Pass Pass Pass Pass
 **
 *************************************************************************/ 
void Pass::setViewPort(int *viewport, float *viewportZ)
{
    glViewport(viewport[0], viewport[1], viewport[2], viewport[3]);
#pragma MESSAGE("TODO TODO TODO TODO TODO : Depth in Pass::setViewPort(int *viewport, float viewportZ)")
    //CHECKGLERRORS("");
}

/// \brief clears the viewport and eventually sets the clear color
/// OpenGL is a real pain in the ass with setting clear color...
void Pass::clearRenderTargets(PassInfo   *pr)
{
    NXPROFILEFUNCCOL2(__FUNCTION__, COLOR_GREEN, 13);
    int numRTs = 0;
    if(pr->currentFBO)
        numRTs = pr->currentFBO->getNumRenderTargets();
    GLbitfield clearflags[2];
    switch(pr->clearMode)
    {
    case Clear_all:
        clearflags[0] = ( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
        clearflags[1] = GL_COLOR_BUFFER_BIT;
        break;
    case Clear_color:
        clearflags[0] = GL_COLOR_BUFFER_BIT;
        clearflags[1] = GL_COLOR_BUFFER_BIT;
        break;
    case Clear_depth:
        clearflags[0] = ( GL_DEPTH_BUFFER_BIT );
        clearflags[1] = 0;
        break;
    case Clear_stencil:
        clearflags[0] = ( GL_STENCIL_BUFFER_BIT );
        clearflags[1] = 0;
        break;
    case Clear_color_depth:
        clearflags[0] = ( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
        clearflags[1] = GL_COLOR_BUFFER_BIT;
        break;
    case Clear_depth_stencil:
        clearflags[0] = ( GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT );
        clearflags[1] = 0;
        break;
    }
    int i;
    if(numRTs == 0)
    {
#ifndef OGLES2
        // Warning: bug to solve. If a previous pass using FBOs changed the target
        // this call will generate an error.
        // glBindFramebuffer(GL_FRAMEBUFFER, 0); can help
        // however I need to also setup again the viewport
        // easiest way to do for now: add "currentTarget = backbuffer;"
        // in the pass so that it does the clean setup for the backbuffer
        //glBindFramebuffer(GL_FRAMEBUFFER, 0);
        glDrawBuffer(GL_BACK);
#endif
        if(pr->flags & PR_CLEARCOLOR)
            glClearColor(pr->clearColor[0][0], pr->clearColor[0][1],pr->clearColor[0][2],pr->clearColor[0][3]);
        glClear( clearflags[0] );
    }
    else {
        for(i=0; i<numRTs; i++)
        {
#ifndef OGLES2
            glDrawBuffer(GL_COLOR_ATTACHMENT0_EXT + i);
#else
            assert(0);
            #pragma MESSAGE("TODO TODO TODO TODO TODO : HOW do we clear all the render targets in OGLES2 ?")
#endif
            if(pr->flags & PR_CLEARCOLOR)
                glClearColor(pr->clearColor[i][0], pr->clearColor[i][1],pr->clearColor[i][2],pr->clearColor[i][3]);
            // Sooo... calling numRTs times glClear. First one clearing DST if needed; others clearing only color
            glClear( clearflags[i == 0 ? 0 : 1] );
        }
#ifndef OGLES2
        glDrawBuffers(numRTs , drawBuffers);
#endif
    }
    CHECKGLERRORS("");
}
/*************************************************************************/ /**

 */ /*********************************************************************/
void Pass::drawFullscreenQuad(float size, float offsetx, float offsety)
{
    {
        struct Quad
        {
            Quad() {
                //glGenVertexArrays(1, &vao);
                //glBindVertexArray(vao);
                {
                    glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &MaxAttr);
                    glGenBuffers(1, &vbo);
                    glBindBuffer(GL_ARRAY_BUFFER, vbo);
                    glBufferData(GL_ARRAY_BUFFER, sizeof(float)*(4*6), NULL, GL_DYNAMIC_DRAW);
                    //glEnableVertexAttribArray(0);
                    //glEnableVertexAttribArray(1);
                    //glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &MaxAttr);
                    //for(int i=2; i< MaxAttr; i++)
                    //    glDisableVertexAttribArray(i);
                    //glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float)*6, NULL);
                    //glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(float)*6, (GLvoid*)(sizeof(float)*3));
                }
                //glBindVertexArray(0);
                glBindBuffer(GL_ARRAY_BUFFER, 0);
            }
            void draw(float size, float offsetx, float offsety)
            {
                glBindBuffer(GL_ARRAY_BUFFER, vbo);
                float* p = (float*)glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY);
                *p++ = 0.0f; *p++ = 0.0f; *p++ = 0.0f; *p++ = offsetx; *p++ = offsety; *p++ = 0.5f;
                *p++ = 1.0f; *p++ = 0.0f; *p++ = 0.0f; *p++ = offsetx + size; *p++ = offsety; *p++ = 0.5f;
                *p++ = 0.0f; *p++ = 1.0f; *p++ = 0.0f; *p++ = offsetx; *p++ = offsety + size; *p++ = 0.5f;
                *p++ = 1.0f; *p++ = 1.0f; *p++ = 0.0f; *p++ = offsetx + size; *p++ = offsety + size; *p++ = 0.5f;
                glUnmapBuffer(GL_ARRAY_BUFFER);
                //glBindBuffer(GL_ARRAY_BUFFER, 0);
                //glBindVertexArray(vao);
                glEnableVertexAttribArray(0);
                glEnableVertexAttribArray(1);
                for(int i=2; i< MaxAttr; i++)
                    glDisableVertexAttribArray(i);
                glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float)*6, (GLvoid*)(sizeof(float)*3));
                glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(float)*6, NULL);
                glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
                glBindBuffer(GL_ARRAY_BUFFER, 0);
                glDisableVertexAttribArray(0);
                glDisableVertexAttribArray(1);
            }
            GLint MaxAttr;
            GLuint  vbo;
            GLuint  vao;
        };
        static Quad squad;
        squad.draw(size, offsetx, offsety);
    }
    //CHECKGLERRORS("");
}

//
// ProgramPipeline ProgramPipeline ProgramPipeline ProgramPipeline ProgramPipeline
// ProgramPipeline ProgramPipeline ProgramPipeline ProgramPipeline ProgramPipeline
// ProgramPipeline ProgramPipeline ProgramPipeline ProgramPipeline ProgramPipeline
// ProgramPipeline ProgramPipeline ProgramPipeline ProgramPipeline ProgramPipeline
//
/*************************************************************************
 **
 **  
 **
 *************************************************************************/ 
GLSLProgramPipeline::GLSLProgramPipeline(Container *pCont) : ProgramPipeline(pCont)
{
    m_pipelineID = 0;
}
GLSLProgramPipeline::~GLSLProgramPipeline()
{
    if(m_pipelineID)
        glDeleteProgramPipelines(1, &m_pipelineID);
}
/*************************************************************************/ /**
 ** 
 ** 
 **/ /*************************************************************************/ 
bool GLSLProgramPipeline::validate()
{
#ifndef OGLES2
    NXPROFILEFUNCCOL2(__FUNCTION__, COLOR_BLUE, 13);
    if(m_validated)
        return true;
    if(m_pipelineID == 0)
        glGenProgramPipelines(1, &m_pipelineID);
    m_validated = true;
    for(int i=0; i<(int)m_progShaders.size(); i++)
    {
        int b = m_progShaders[i]->getProgramShaderFlags();
        glUseProgramStages(m_pipelineID, b, m_progShaders[i]->getProgram());
        CHECKGLERRORS("glUseProgramStages");
    }
    //int v;
    //if(glIsProgramPipeline(m_pipelineID))
    //{
    //    glGetProgramPipelineiv(m_pipelineID, GL_ACTIVE_PROGRAM, &v);
    //    glGetProgramPipelineiv(m_pipelineID, GL_VERTEX_SHADER, &v);
    //    glGetProgramPipelineiv(m_pipelineID, GL_FRAGMENT_SHADER, &v);
    //}
    return m_validated;
#else
    assert(0);
    return false;
#endif
}

/*************************************************************************/ /**
 ** 
 ** 
 **/ /*************************************************************************/ 
bool GLSLProgramPipeline::invalidate(int stages)
{
#ifndef OGLES2
    LOGD("Invalidating Program pipeline %d\n", m_pipelineID);
    NXPROFILEFUNCCOL2(__FUNCTION__, COLOR_BLUE, 13);
    if(m_pipelineID == 0)
        return false;
    m_validated = false;
    m_shaderFlags &= ~stages;
    glUseProgramStages(m_pipelineID, stages, 0);
    CHECKGLERRORS("glUseProgramStages");
    return true;
#else
    assert(0);
    return false;
#endif
}
/*************************************************************************/ /**
 ** 
 ** 
 **/ /*************************************************************************/ 
bool GLSLProgramPipeline::bind()
{
#ifndef OGLES2
    NXPROFILEFUNCCOL2(__FUNCTION__, COLOR_BLUE, 13);
    if(!m_validated)
        if(!validate())
            return false;
    glUseProgram(0);
    glBindProgramPipeline(m_pipelineID);
    //int v;
    //glGetProgramPipelineiv(m_pipelineID, GL_ACTIVE_PROGRAM, &v);
    //glGetProgramPipelineiv(m_pipelineID, GL_VERTEX_SHADER, &v);
    //glGetProgramPipelineiv(m_pipelineID, GL_FRAGMENT_SHADER, &v);
    CHECKGLERRORS("glBindProgramPipeline");
    return true;
#else
    assert(0);
    return false;
#endif
}
/*************************************************************************/ /**
 ** 
 ** 
 **/ /*************************************************************************/ 
void GLSLProgramPipeline::unbind()
{
#ifndef OGLES2
    NXPROFILEFUNCCOL2(__FUNCTION__, COLOR_BLUE, 13);
    glBindProgramPipeline(0);
    CHECKGLERRORS("glBindProgramPipeline::unbind()");
#else
    assert(0);
#endif
}

GLenum getGLAccess(AccessMode a)
{
#ifndef OGLES2
    switch(a)
    {
    case ACCESS_READ_WRITE: return GL_READ_WRITE;
    case ACCESS_READ_ONLY:  return GL_READ_ONLY;
    case ACCESS_WRITE_ONLY: return GL_WRITE_ONLY;
    }
#endif
    return 0;
}

} //namespace nvFX
