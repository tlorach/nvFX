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
#include <windows.h>
#endif
#include <fstream>
#include <assert.h>
#include "Fx.h"
#include "FxGL.h"
#pragma warning(disable:4996)
using namespace nvFX;

/*************************************************************************/ /**
 ** 
 ** 
 **/ /*************************************************************************/ 
inline GLenum convertShaderType(int type)
{
    GLenum shdtype;
    switch(type)
    {
    case FX_VTXPROG:
#if 1 //def OGLES2
        shdtype = GL_VERTEX_SHADER;
#else
        shdtype = GL_VERTEX_SHADER_ARB;
#endif
        break;
    case FX_FRAGPROG:
#if 1 //def OGLES2
        shdtype = GL_FRAGMENT_SHADER;
#else
        shdtype = GL_FRAGMENT_SHADER_ARB;
#endif
        break;
#ifndef OGLES2
    case FX_GEOMPROG:
        shdtype = GL_GEOMETRY_SHADER_ARB;
        break;
    case FX_TCSPROG:
        shdtype = GL_TESS_CONTROL_SHADER;
        break;
    case FX_TESPROG:
        shdtype = GL_TESS_EVALUATION_SHADER;
        break;
    case GL_VERTEX_SHADER_ARB:
    case GL_FRAGMENT_SHADER_ARB:
    case GL_GEOMETRY_SHADER_ARB:
    case GL_TESS_CONTROL_SHADER:
    case GL_TESS_EVALUATION_SHADER:
        shdtype = type;
        break;
    case FX_COMPUTEPROG:
        shdtype = 0x91B9;/*GL_COMPUTE_SHADER*/
        break;
#endif
    default:
        shdtype = 0;
    }
    return shdtype;
}
/*************************************************************************/
/* GLSLShader GLSLShader GLSLShader GLSLShader GLSLShader GLSLShader     */
/*************************************************************************/

/*************************************************************************/ /**
 ** 
 ** 
 **/ /*************************************************************************/ 
GLSLShader::~GLSLShader()
{
    cleanupShader();
}

/*************************************************************************/ /**
 ** 
 ** 
 **/ /*************************************************************************/ 
GLSLShader::GLSLShader(const char * name) : Shader(name)
{
    m_targetType = TGLSL;
}
/*************************************************************************/ /**
 ** 
 ** 
 **/ /*************************************************************************/ 
bool GLSLShader::isCompiled(int type)
{
    ShaderObjects::iterator iShd = m_shaderObjs.find(convertShaderType(type));
    if(iShd == m_shaderObjs.end())
        return false;
    if(iShd->second == 0)
        return false;
    return true;
}
/*************************************************************************/ /**
 ** 
 ** 
 **/ /*************************************************************************/ 
#ifdef USE_CG
bool GLSLShader::compileShader(ShaderType type, CGprofile p)
#else
bool GLSLShader::compileShader(ShaderType type, IContainer *pContainer)
#endif
{
    if(m_shaderCode.empty())
        return false;
    GLenum shdtype = convertShaderType(type);
    GLhandleARB &shaderObj = m_shaderObjs[shdtype];
    if(!shaderObj)
    {
#if 1 //def OGLES2
        shaderObj = glCreateShader((GLenum)shdtype);
#else
        shaderObj = glCreateShaderObjectARB((GLenum)shdtype);
#endif
        // add common parts
        std::string strShd;
        GLSLShader *pShd;
        std::vector<codeInfo> codeBlocks;
        int lineOffset = 0;
        if (NULL!=pContainer)
        {
        for(int i=0; pShd = static_cast<GLSLShader*>(pContainer->findShader(i)); i++)
        {
            if((*(pShd->getName()) == '\0') && (pShd->getType() == TGLSL) )
            {
                strShd += pShd->getShaderCode();
                std::vector<codeInfo>::iterator iCI;
                // calculate all the source code ranges for line # error tracking
                for(iCI = pShd->m_startLineNumbers.begin(); iCI != pShd->m_startLineNumbers.end(); iCI++)
                    codeBlocks.push_back(codeInfo(iCI->lineInFX, iCI->lineinShader + lineOffset, pShd->m_totalLines, iCI->fname.c_str()));
                lineOffset += pShd->m_totalLines;
            }
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
        glShaderSource(shaderObj, 1, &progString, &size);
        glCompileShader(shaderObj);
        //
        // Get Log even if things went well... for warnings or other messages
        //
        char buf[1024];
        int len;
#if 1 //def OGLES2
        glGetShaderInfoLog (shaderObj, 1024, &len, buf);
#else
        glGetInfoLogARB(shaderObj, 1024, &len, buf);
#endif
        if(len)
        {
            bool iOS = false;
            char *p = strstr(buf, "0(");
            //nvFX::printf("Log for Shader %d:\n%s\n", shaderObj,strShd.c_str());
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
                if/*assert*/( iCBFound != codeBlocks.end() )
                    nvFX::printf("%s(%d)", iCBFound->fname.c_str(), iCBFound->lineInFX + offsetNum);
                else
                    nvFX::printf("()");
                pNum = strstr(p, "0(");
                if((pNum==NULL) && iOS)
                    pNum = strstr(p, "0:"); // Only if under iOS
                while(pNum && (!isdigit(pNum[2])))
                {
                    char* pNum2 = strstr(pNum+2, "0(");
                    if(pNum2==NULL)
                        pNum = strstr(pNum+2, "0:");
                    else pNum = pNum2;
                }
                if(pNum) *pNum='\0';
                nvFX::printf("%s", p);
                p = pNum;
            }
            //else nvFX::printf("Log for %d:\n%s\n", shaderObj, buf);
        }
        //
        // Check if we failed
        //
        GLint status;
        glGetShaderiv(shaderObj, GL_COMPILE_STATUS, &status);
#ifndef SHADERCONCAT_USE
        if(status == GL_FALSE)
        {
#endif
            //nvFX::printf("GLSL Shader code: \n%s\n", strShd.c_str());
#if 1 //def OGLES2
            glDeleteShader(shaderObj);
#else
            glDeleteObjectARB(shaderObj);
#endif
            shaderObj = 0;
#ifndef SHADERCONCAT_USE
            return false;
        }
#else
        if(status == GL_FALSE)
            return false;
#endif
    }
    return true;
}
/*************************************************************************/ /**
 ** 
 ** 
 **/ /*************************************************************************/ 
void GLSLShader::cleanupShader()
{
    ShaderObjects::iterator iShd = m_shaderObjs.begin();
    while(iShd != m_shaderObjs.end())
    {
        GLhandleARB &o = iShd->second;
#if 1 //def OGLES2
        if(o) glDeleteShader(o);
#else
        if(o) glDeleteObjectARB(o);
#endif
        o = 0;
        ++iShd;
    }
    m_shaderObjs.clear();
}
/*************************************************************************/ /**
 ** 
 ** 
 **/ /*************************************************************************/ 
GLhandleARB GLSLShader::getGLSLShaderObj(GLenum type)
{
    GLenum shdtype = convertShaderType(type);
    ShaderObjects::iterator iShd = m_shaderObjs.find(shdtype);
    if(iShd == m_shaderObjs.end())
        return 0;
    return iShd->second;
}

