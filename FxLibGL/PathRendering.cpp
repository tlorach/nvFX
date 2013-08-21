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
#include <windows.h> //for OutputDebugString
#endif
#include <ctype.h>
#include <fstream>
#include <assert.h>
#include "Fx.h"
#include "FxGL.h"

#pragma warning(disable:4996)
using namespace nvFX;

/*****************************************************************************/
/* PathProgram PathProgram PathProgram PathProgram PathProgram PathProgram   */
/* PathProgram PathProgram PathProgram PathProgram PathProgram PathProgram   */
/*****************************************************************************/

/*************************************************************************/ /**
 ** 
 ** 
 **/ /*************************************************************************/ 
PathProgram::~PathProgram()
{
    cleanup();
}

/*************************************************************************/ /**
 ** 
 ** 
 **/ /*************************************************************************/ 
PathProgram::PathProgram(Container *pCont) : Program(pCont)
{
    m_program = 0; // Not used for path
    m_linkNeeded = false;
    m_usable = false;
    m_separable = false;
    m_shaderFlags = 0;
}
/*************************************************************************/ /**
 ** 
 ** 
 **/ /*************************************************************************/ 
bool PathProgram::addFragmentShader(IShader* pShader, IContainer* pContainer)
{
    return false;
}
/*************************************************************************/ /**
 ** 
 ** 
 **/ /*************************************************************************/ 
bool PathProgram::addVertexShader(IShader* pShader, IContainer* pContainer)
{
    return false;
}

/*************************************************************************/ /**
 ** 
 ** 
 **/ /*************************************************************************/ 
bool PathProgram::addShader(ShaderType type, IShader* pShader, IContainer* pContainer)
{
    if(!pShader)
        return false;
    if(!static_cast<Shader*>(pShader)->compileShader(type, pContainer))
        return false;
    GLhandleARB o = static_cast<Shader*>(pShader)->getGLSLShaderObj(type);
    if(o == 0)
        return false;
    switch(type)
    {
    case FX_PATHPROG:
        m_shaders.push_back(static_cast<PathShader*>(pShader));
        m_shaderFlags |= FX_VERTEX_SHADER_BIT;
        break;
    default:
        return false;
    }
    return true;
}

/*************************************************************************/ /**
 ** 
 ** 
 **/ /*************************************************************************/ 
IShader*    PathProgram::getShader(int n, ShaderType *t)
{
    ShaderVec::iterator it;
    it = m_shaders.begin();
    while(it != m_shaders.end())
    {
        if(n == 0)
        {
            if(t) *t = FX_PATHPROG;
            return (*it);
        }
        --n;
        ++it;
    }
    return NULL;
}
/*************************************************************************/ /**
 ** 
 ** 
 **/ /*************************************************************************/ 
IShader*    PathProgram::getShader(IShader *pShader, ShaderType *t)
{
    ShaderVec::iterator it;
    it = m_shaders.begin();
    while(it != m_shaders.end())
    {
        if((*it) == pShader)
        {
            if(t) *t = FX_PATHPROG;
            return (*it);
        }
        ++it;
    }
    return NULL;
}
/*************************************************************************/ /**
 ** 
 ** 
 **/ /*************************************************************************/ 
IShader*    PathProgram::getShader(const char *name, ShaderType *t)
{
    ShaderVec::iterator it;
    it = m_shaders.begin();
    while(it != m_shaders.end())
    {
        if(!strcmp((*it)->getName(), name))
        {
            if(t) *t = FX_PATHPROG;
            return (*it);
        }
        ++it;
    }
    return NULL;
}
/*************************************************************************/ /**
 ** 
 ** 
 **/ /*************************************************************************/ 
int         PathProgram::getNumShaders()
{
    return (int)m_shaders.size();
}

/*************************************************************************/ /**
 ** 
 ** 
 **/ /*************************************************************************/ 
//TODO: geom, tess...

void PathProgram::cleanup()
{
    ShaderVec::iterator ivshd = m_shaders.begin();
    while(ivshd != m_shaders.end())
    {
        GLhandleARB o = (*ivshd)->getGLSLShaderObj(FX_VTXPROG);
        //detach it from the program...
        ++ivshd;
    }
    m_shaders.clear();
}
/*************************************************************************/ /**
 ** 
 **
 **/ /*************************************************************************/ 
bool PathProgram::link(IContainer* pContainer)
{
    // This is a fake program : Path rendering doesn't need any linkage because
    // there is not program. The so called program will just be used to invoke
    // instances of Path objects
    return true;
}
/*************************************************************************/ /**
 ** 
 ** 
 **/ /*************************************************************************/ 
bool PathProgram::bind(IContainer* pContainer)
{
    // No binding... since no program, anyways
    return true;
}
/*************************************************************************/ /**
 ** 
 ** 
 **/ /*************************************************************************/ 
void PathProgram::unbind(IContainer* pContainer)
{
    // No unbinding... since no program, anyways
}

/*************************************************************************/ /**
 ** 
 ** 
 **/ /*************************************************************************/ 
int PathProgram::getUniformLocation(const char* name)
{
#pragma MESSAGE(__FILE__"(212): TODO TODO TODO TODO : add fake uniform location for the Matrices MV+Proj UBO")
    return 0;
}
/*************************************************************************/ /**
 ** 
 ** 
 **/ /*************************************************************************/ 

void PathProgram::setUniform(const char* name, float val)
{
    assert(!"DO we want to fake some special uniforms for Path setup such as thickness... ?");
}
/*************************************************************************/ /**
 ** 
 ** 
 **/ /*************************************************************************/ 
void PathProgram::setUniform(const char* name, int val)
{
    assert(!"DO we want to fake some special uniforms for Path setup such as thickness... ?");
}
/*************************************************************************/ /**
 ** 
 ** 
 **/ /*************************************************************************/ 
void PathProgram::setUniformVector(const char * name, const float* val, int count, int numUniforms)
{
    assert(!"DO we want to fake some special uniforms for Path setup such as thickness... ?");
}
/*************************************************************************/ /**
 ** 
 ** 
 **/ /*************************************************************************/ 
void PathProgram::setUniformVector(const char * name, const int* val, int count, int numUniforms)
{
    assert(!"DO we want to fake some special uniforms for Path setup such as thickness... ?");
}
/*************************************************************************/ /**
 ** 
 ** 
 **/ /*************************************************************************/ 
void PathProgram::setSampler(const char * texname, int i)
{
    assert(!"DO we want to setup some texture for Path rendering ?");
}
/*************************************************************************/ /**
 ** 
 ** 
 **/ /*************************************************************************/ 
void PathProgram::bindTexture(ResourceType target, const char * texname, GLuint texid, int i)
{
    assert(!"DO we want to setup some texture for Path rendering ?");
}

/*************************************************************************/ /**
 ** 
 ** 
 **/ /*************************************************************************/ 
bool PathProgram::bindSubRoutineToUniform(int uniformID, GLenum shadertype, char **subroutineNames, int numNames)
{
    return false;
}
int  PathProgram::getSubRoutineID(const char *name, GLenum shadertype)
{
    return -1;
}
int  PathProgram::getAttribLocation(const char* attrName)
{
    return -1;
}
void PathProgram::bindAttribLocation(int i, const char* attrName)
{
}
bool PathProgram::execute(int szx, int szy, int szz)
{
    return false;
}
bool PathProgram::execute(RenderingMode mode, const PassInfo::PathInfo *p)
{
    // TODO: link operation should generate a table of path objects to send to instanced function
    // will replace m_shaders[0]->getGLSLShaderObj() ...
    if(m_shaders.empty())// TODO : linkage should set (!m_usable)
        return false;
    switch(mode)
    {
    case RENDER_STENCILFILLPATH:
        glStencilFillPathNV(m_shaders[0]->getGLSLShaderObj(0), (GLenum)p->fillMode, p->fillMask);
        //glStencilFillPathInstancedNV(m_shaders.size(),
        //    GL_UTF8_NV, paths, 0,
        //    (GLenum)p->fillMode, p->mask,
        //    (GLenum)p->transformType, p->transfValues);
        break;
    case RENDER_STENCILSTROKEPATH:
        glStencilStrokePathNV(m_shaders[0]->getGLSLShaderObj(0), p->strokeReference, p->strokeMask);
        break;
    case RENDER_COVERFILLPATH:
        glCoverFillPathNV(m_shaders[0]->getGLSLShaderObj(0), (GLenum)p->fillCoverMode);
        break;
    case RENDER_COVERSTROKEPATH:
        glCoverStrokePathNV(m_shaders[0]->getGLSLShaderObj(0), (GLenum)p->strokeCoverMode);
        break;
    }
    return true;
}

/********************************************************************************/
/* PathShader PathShader PathShader PathShader PathShader PathShader PathShader */
/* PathShader PathShader PathShader PathShader PathShader PathShader PathShader */
/********************************************************************************/

/*************************************************************************/ /**
 ** 
 ** 
 **/ /*************************************************************************/ 
PathShader::~PathShader()
{
    cleanupShader();
}

/*************************************************************************/ /**
 ** 
 ** 
 **/ /*************************************************************************/ 
PathShader::PathShader(const char * name, bool bPostscript) : Shader(name)
{
    m_bPostscript = bPostscript;
    m_targetType = TPATH;
}
/*************************************************************************/ /**
 ** 
 ** 
 **/ /*************************************************************************/ 
bool PathShader::isCompiled(int type)
{
    return m_shaderObjs.empty() ? false : true;
}
/*************************************************************************/ /**
 ** 
 ** 
 **/ /*************************************************************************/ 
bool PathShader::compileShader(ShaderType type, IContainer *pContainer)
{
    if(m_shaderCode.empty())
        return false;
    GLhandleARB &shaderObj = m_shaderObjs[0];
    if(!shaderObj)
    {
        shaderObj = glGenPathsNV(1);
        // add common parts
        std::string strShd;
        PathShader *pShd;
        std::vector<codeInfo> codeBlocks;
        int lineOffset = 0;
        for(int i=0; pShd = static_cast<PathShader*>(pContainer->findShader(i)); i++)
        {
            if((*(pShd->getName()) == '\0') && (pShd->getType() == TPATH) )
            {
                strShd += pShd->getShaderCode();
                std::vector<codeInfo>::iterator iCI;
                // calculate all the source code ranges for line # error tracking
                for(iCI = pShd->m_startLineNumbers.begin(); iCI != pShd->m_startLineNumbers.end(); iCI++)
                    codeBlocks.push_back(codeInfo(iCI->lineInFX, iCI->lineinShader + lineOffset, pShd->m_totalLines, iCI->fname.c_str()));
                lineOffset += pShd->m_totalLines;
            }
        }
        // set source
        strShd += m_shaderCode.c_str();

        std::vector<codeInfo>::iterator iCI;
        // calculate all the source code ranges for line # error tracking
        for(iCI = m_startLineNumbers.begin(); iCI != m_startLineNumbers.end(); iCI++)
        {
            codeBlocks.push_back(codeInfo(iCI->lineInFX, iCI->lineinShader + lineOffset, m_totalLines, iCI->fname.c_str()));
        }
        lineOffset += m_totalLines;

        GLint size = (int)strShd.length();
        const /*GLcharARB*/char* progString = strShd.c_str();
        glPathStringNV(shaderObj, /*m_format*/GL_PATH_FORMAT_SVG_NV, (int)strShd.size(), progString);
#pragma MESSAGE(__FILE__"(670):TODO TODO TODO TODO : gather errors in the path creation")
        //
        // Get Log even if things went well... for warnings or other messages
        //
        /*char buf[1024];
        int len;
        GLenum err = glGetError();
        if(err)
        {
            bool iOS = false;
            char *p = strstr(buf, "0(");
            nvFX::printf("Log for Shader %d:\n%s\n", shaderObj,strShd.c_str());
            if(p==NULL)
            {
                // iOS error display :-(. Let's do the first one and tag iOS=true if we found something
                p = strstr(buf, "0:");
                iOS = true;
            }
            if(p==NULL)
                nvFX::printf("%s\n", buf);
            while(p)
            {
                *p = '\0';
                p+=2;
                char *pNum = p;
                while((*p != ')')&&(*p != ':'))
                {
                    if(*p == '\0') // for safety. Should not happen
                        break;
                    p++;
                }
                if(*p == '\0') // for safety. Should not happen
                    break;
                *p++ = '\0';
                int num = atoi(pNum);
#   ifdef ES2EMU
                num -= 139; // WHY ?!?!?!?!
#   endif
                num--;
                int offsetNum = num;
                std::vector<codeInfo>::iterator iCB;
                std::vector<codeInfo>::iterator iCBFound = codeBlocks.end();
                for(iCB = codeBlocks.begin(); iCB != codeBlocks.end(); iCB++)
                    if(num >= iCB->lineinShader)
                    {
                        if(num < (iCB->lineinShader + iCB->totalLinesInFX))
                        {
                            iCBFound = iCB;
                            offsetNum = num - iCB->lineinShader;
                            break;
                        }
                    }
                if( iCBFound != codeBlocks.end() )
                    nvFX::printf("%s(%d)", iCBFound->fname.c_str(), iCBFound->lineInFX + offsetNum);
                else
                    nvFX::printf("()");
                pNum = strstr(p, "0(");
                if((pNum==NULL) && iOS)
                    pNum = strstr(p, "0:"); // Only if under iOS
                //if(pNum) && (!isalnum(pNum[2])))
                //    pNum = NULL;
                if(pNum) *pNum='\0';
                nvFX::printf("%s", p);
                p = pNum;
            }
            //else nvFX::printf("Log for %d:\n%s\n", shaderObj, buf);
            delete the path object...
            return false;
        }*/
    }
    return true;
}
/*************************************************************************/ /**
 ** 
 ** 
 **/ /*************************************************************************/ 
void PathShader::cleanupShader()
{
    ShaderObjects::iterator iShd = m_shaderObjs.begin();
    while(iShd != m_shaderObjs.end())
    {
        GLuint &o = iShd->second;
        if(o)
            glDeletePathsNV(o, 1);
        o = 0;
        ++iShd;
    }
    m_shaderObjs.clear();
}
/*************************************************************************/ /**
 ** 
 ** 
 **/ /*************************************************************************/ 
GLhandleARB PathShader::getGLSLShaderObj(GLenum type)
{
    if(m_shaderObjs.empty())
        return 0;
    return m_shaderObjs[0];
}

/***************************************************************************************/
/* StateGroupRasterGL StateGroupRasterGL StateGroupRasterGL StateGroupRasterGL */
/***************************************************************************************/

/*************************************************************************/ /**
 ** The Path state group is somewhat a strange one in the sense that
 ** - some states apply to a specific path object
 ** - some states apply to all (no targeted path object)
 ** - some states aren't states : they are arguments later used for the gl function for Path rendering.
 **   In this case, we must return these arguments : using the current PassInfo
 ** 
 **/ /*************************************************************************/ 
void StateGroupPathGL::apply(IPass *pass, PassInfo * pr, int n)
{
    Pass *p = static_cast<Pass*>(pass);
    Program *prog = p->m_pActiveStatesLayer->program;
    // TODO: Test if the program is for Path rendering
    if(prog == NULL)
        return;
    int start = n;
    int end = n+1;
    if(n<0) {
        start = 0;
        end = prog->getNumShaders();
    }
    // We might want to loop into each attached path object to locally change values...
    StateVec::iterator it = m_states.begin();
    while(it != m_states.end())
    {
        State *s = *it;
        if(s)
        {
            switch(s->state)
            {
            case GL_PATH_STENCIL_DEPTH_OFFSET_FACTOR_NV:
            // No need of case GL_PATH_STENCIL_DEPTH_OFFSET_UNITS_NV:
                glPathStencilDepthOffsetNV(s->fval[0], (int)s->fval[1]);
                break;
            case GL_PATH_STROKE_WIDTH_NV:
            case GL_PATH_MITER_LIMIT_NV:
            case GL_PATH_DASH_OFFSET_NV:
            case GL_PATH_CLIENT_LENGTH_NV:
                for(int i=start; i<end; i++)
                {
                    Shader* pShader = static_cast<Shader*>(prog->getShader(i));
                    if(pShader == NULL)
                        break;
                    GLhandleARB pathObj = pShader->getGLSLShaderObj(0);
                    glPathParameterfNV(pathObj, s->state, s->fval[0]);
                    CHECKGLERRORS("glPathParameterfNV");
                }
                break;
            case GL_PATH_STENCIL_FUNC_NV:
            case GL_PATH_FOG_GEN_MODE_NV:
            case GL_PATH_COVER_DEPTH_FUNC_NV:
            case GL_PATH_INITIAL_END_CAP_NV:
            case GL_PATH_TERMINAL_END_CAP_NV:
            case GL_PATH_INITIAL_DASH_CAP_NV:
            case GL_PATH_TERMINAL_DASH_CAP_NV:
            case GL_PATH_JOIN_STYLE_NV:
            case GL_PATH_DASH_OFFSET_RESET_NV:
                for(int i=start; i<end; i++)
                {
                    Shader* pShader = static_cast<Shader*>(prog->getShader(i));
                    if(pShader == NULL)
                        break;
                    GLhandleARB pathObj = pShader->getGLSLShaderObj(0);
                    glPathParameteriNV(pathObj, s->state, s->ival);
                    CHECKGLERRORS("glPathParameteriNV");
                }
                break;
             // arguments for glStencilFillPathNV
            case GL_PATH_FILL_MODE_NV:
                pr->pathRenderingSettings.fillMode = s->value;
                break;
            case GL_PATH_FILL_MASK_NV:
                pr->pathRenderingSettings.fillMask = s->ival;
                break;
            // argument for glCoverFillPathNV
            case GL_PATH_FILL_COVER_MODE_NV:
                pr->pathRenderingSettings.fillCoverMode = s->value;
                break;
            // argument for glCoverStrokePathNV
            case GL_PATH_STROKE_COVER_MODE_NV:
                pr->pathRenderingSettings.strokeCoverMode = s->value;
                break;
            // arguments for glStencilStrokePathNV
            case GL_PATH_STROKE_MASK_NV:
                pr->pathRenderingSettings.strokeMask = s->ival;
                break;
            default:
#ifdef WIN32
                OutputDebugStringA("Unknowns STATE for StateGroupPath::apply()\n");
                assert(0);
#endif
                break;
            }
        }
        ++it;
    }
}
/*************************************************************************/ /**
 ** In OpenGL, validate is not mandatory.
 ** But it will be used when we will use the extension for state-groups...
 **/ /*************************************************************************/ 
bool StateGroupPathGL::validate()
{
    return true;
}

