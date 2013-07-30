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
#include "FxD3D.h"
#pragma warning(disable:4996)
using namespace nvFX;

/*************************************************************************/ /**
 ** 
 ** 
 **/ /*************************************************************************/ 
inline GLenum convertShaderType(int type)
{
    return 0;
}
/*************************************************************************/
/* D3DShader D3DShader D3DShader D3DShader D3DShader D3DShader     */
/*************************************************************************/

/*************************************************************************/ /**
 ** 
 ** 
 **/ /*************************************************************************/ 
D3DShader::~D3DShader()
{
    cleanupShader();
}

/*************************************************************************/ /**
 ** 
 ** 
 **/ /*************************************************************************/ 
D3DShader::D3DShader(const char * name, IContainer* pCont) : Shader(name, pCont)
{
    m_targetType = THLSL;
}
/*************************************************************************/ /**
 ** 
 ** 
 **/ /*************************************************************************/ 
bool D3DShader::isCompiled(int type)
{
DebugBreak();
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
bool D3DShader::compileShader(GLenum type)
{
DebugBreak();
    if(m_shaderCode.empty())
        return false;
    GLenum shdtype = convertShaderType(type);
    GLhandleARB &shaderObj = m_shaderObjs[shdtype];
    if(!shaderObj)
    {
        //shaderObj = glCreateShaderObjectARB(shdtype);
        // add common parts
        std::string strShd;
        D3DShader *pShd;
        std::vector<codeInfo> codeBlocks;
        int lineOffset = 0;
        for(int i=0; pShd = static_cast<D3DShader*>(m_container->findShader(i)); i++)
        {
            if(*(pShd->getName()) == '\0')
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

        GLint size = strShd.length();
        const /*GLcharARB*/char* progString = strShd.c_str();
        //glShaderSource(shaderObj, 1, &progString, &size);
        //glCompileShader(shaderObj);
        //
        // Get Log even if things went well... for warnings or other messages
        //
        char buf[1024];
        int len = 0;
        //glGetInfoLogARB(shaderObj, 1024, &len, buf);
        if(len)
        {
            char *p = strstr(buf, "0(");
            nvFX::printf("Log for Shader %d:\n", shaderObj);
            if(p==NULL)
                p = strstr(buf, "0:"); // iOS error display
            if(p==NULL)
                nvFX::printf("%s\n", buf);
            while(p)
            {
                *p = '\0';
                p+=2;
                char *pNum = p;
                while((*p != ')')&&(*p != ':'))
                    p++;
                *p++ = '\0';
                int num = atoi(pNum);
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
                if(pNum==NULL)
                    pNum = strstr(p, "0:");
                //if(pNum) && (!isalnum(pNum[2])))
                //    pNum = NULL;
                if(pNum) *pNum='\0';
                nvFX::printf("%s", p);
                p = pNum;
            }
            //else nvFX::printf("Log for %d:\n%s\n", shaderObj, buf);
        }
        //
        // Check if we failed
        //
        GLint status = 0;
        //glGetShaderiv(shaderObj, GL_COMPILE_STATUS, &status);
#ifndef SHADERCONCAT_USE
        if(status == 0)
        {
#endif
            //nvFX::printf("Shader code: \n%s\n", strShd.c_str());
            //glDeleteObjectARB(shaderObj);
            shaderObj = 0;
#ifndef SHADERCONCAT_USE
            return false;
        }
#else
        if(status == 0)
            return false;
#endif
    }
    return true;
}
/*************************************************************************/ /**
 ** 
 ** 
 **/ /*************************************************************************/ 
void D3DShader::cleanupShader()
{
    ShaderObjects::iterator iShd = m_shaderObjs.begin();
    while(iShd != m_shaderObjs.end())
    {
        GLhandleARB &o = iShd->second;
        //if(o) glDeleteObjectARB(o);
        o = 0;
        ++iShd;
    }
    m_shaderObjs.clear();
}
/*************************************************************************/ /**
 ** 
 ** 
 **/ /*************************************************************************/ 
GLhandleARB D3DShader::getGLSLShaderObj(GLenum type)
{
    return 0;
    //GLenum shdtype = convertShaderType(type);
    //ShaderObjects::iterator iShd = m_shaderObjs.find(shdtype);
    //if(iShd == m_shaderObjs.end())
    //    return 0;
    //return iShd->second;
}

