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
/* GLSLProgram GLSLProgram GLSLProgram GLSLProgram GLSLProgram GLSLProgram   */
/*****************************************************************************/

/*************************************************************************/ /**
 ** 
 ** 
 **/ /*************************************************************************/ 
GLSLProgram::~GLSLProgram()
{
    cleanup();
    if(m_program)
    {
#if 1 //def OGLES2
        glDeleteProgram(m_program);
#else
        glDeleteObjectARB(m_program);
#endif
        m_program = 0;
    }
}

/*************************************************************************/ /**
 ** 
 ** 
 **/ /*************************************************************************/ 
GLSLProgram::GLSLProgram(Container *pCont) : Program(pCont)
{
    GLenum err = glGetError(); // we don't want to have errors from outside interfere with this method
    m_program = glCreateProgram();
	m_linkNeeded = true;
    m_usable = false;
    //m_bound = false;
    #ifdef USE_OLDPROGRAM
    m_separable = false;
    #else
    if (pCont)
    {
    m_separable    = pCont->separateShadersEnabled();
    if(m_separable)
    {
		// glProgramParameteri is part of GL_ARB_get_program_binary 
		// so it could be NULL for < 4.1 !
        // if you want to avoid this issue, use glProgramParameteriARB instead
        glProgramParameteri/*ARB*/(m_program, GL_PROGRAM_SEPARABLE, m_separable);
        if((glGetError()==GL_INVALID_ENUM)||(glGenProgramPipelines == NULL))
        {
            LOGI("Warning : ARB_separate_shader_objects not available\n");
            m_separable = false;
            pCont->separateShadersEnable(false);
        }
    }
    }
    #endif
    m_shaderFlags = 0;
#ifdef SHADERCONCAT_USE
    m_vtxShader = 0;
    m_fragShader = 0;
#endif
}
/*************************************************************************/ /**
 ** 
 ** 
 **/ /*************************************************************************/ 
bool GLSLProgram::addFragmentShader(IShader* pShader, IContainer* pContainer)
{
    return addShader(FX_FRAGPROG, pShader, pContainer);
}
/*************************************************************************/ /**
 ** 
 ** 
 **/ /*************************************************************************/ 
bool GLSLProgram::addVertexShader(IShader* pShader, IContainer* pContainer)
{
    return addShader(FX_VTXPROG, pShader, pContainer);
}

/*************************************************************************/ /**
 ** 
 ** 
 **/ /*************************************************************************/ 
bool GLSLProgram::addShader(ShaderType type, IShader* pShader, IContainer* pContainer)
{
    if(!pShader)
        return false;
#ifndef SHADERCONCAT_USE
    if(!static_cast<Shader*>(pShader)->compileShader(type, pContainer))
        return false;
    GLhandleARB o = static_cast<Shader*>(pShader)->getGLSLShaderObj(type);
    if(o == 0)
        return false;
#  if 1 //def OGLES2
    glAttachShader(m_program, o);
#  else
    glAttachObjectARB(m_program, o);
#  endif
    GLenum err = glGetError();
    if(err != GL_NONE)
    {
        // NOTE: iOS seems to have a bug if we attach many shaders of the same domain :-(
        nvFX::printf("Error while Attaching Shader %s to the program %d :Error 0x%x\n", pShader->getName(), m_program, err);
        return false;
    }
#endif
    switch(type)
    {
    case FX_VTXPROG:
        m_vtxShaders[std::string(pShader->getName())] = static_cast<GLSLShader*>(pShader);
        m_shaderFlags |= FX_VERTEX_SHADER_BIT;
        break;
    case FX_GEOMPROG:
        m_geomShaders[std::string(pShader->getName())] = static_cast<GLSLShader*>(pShader);
        m_shaderFlags |= FX_GEOMETRY_SHADER_BIT;
        break;
    case FX_TCSPROG:
        m_tcsShaders[std::string(pShader->getName())] = static_cast<GLSLShader*>(pShader);
        m_shaderFlags |= FX_TESS_CONTROL_SHADER_BIT;
        break;
    case FX_TESPROG:
        m_tesShaders[std::string(pShader->getName())] = static_cast<GLSLShader*>(pShader);
        m_shaderFlags |= FX_TESS_EVALUATION_SHADER_BIT;
        break;
    case FX_FRAGPROG:
        m_fragShaders[std::string(pShader->getName())] = static_cast<GLSLShader*>(pShader);
        m_shaderFlags |= FX_FRAGMENT_SHADER_BIT;
        break;
    case FX_COMPUTEPROG:
        m_computeShaders[std::string(pShader->getName())] = static_cast<GLSLShader*>(pShader);
        m_shaderFlags |= FX_COMPUTE_SHADER_BIT;
        CHECK_TRUE(m_shaderFlags == FX_COMPUTE_SHADER_BIT);
        if(m_shaderFlags != FX_COMPUTE_SHADER_BIT)
            return false;
        break;
    }
    // add in the shader the target reference of this program
    static_cast<Shader*>(pShader)->addUser(this);
    return true;
}

/*************************************************************************/ /**
 ** 
 ** 
 **/ /*************************************************************************/ 
IShader*    GLSLProgram::getShader(int n, ShaderType *t)
{
    ShaderMap::iterator it;
    it = m_vtxShaders.begin();
    while(it != m_vtxShaders.end())
    {
        if(n == 0)
        {
            if(t) *t = FX_VTXPROG;
            return it->second;
        }
        --n;
        ++it;
    }
    it = m_geomShaders.begin();
    while(it != m_geomShaders.end())
    {
        if(n == 0)
        {
            if(t) *t = FX_GEOMPROG;
            return it->second;
        }
        --n;
        ++it;
    }
    it = m_tcsShaders.begin();
    while(it != m_tcsShaders.end())
    {
        if(n == 0)
        {
            if(t) *t = FX_TCSPROG;
            return it->second;
        }
        --n;
        ++it;
    }
    it = m_tesShaders.begin();
    while(it != m_tesShaders.end())
    {
        if(n == 0)
        {
            if(t) *t = FX_TESPROG;
            return it->second;
        }
        --n;
        ++it;
    }
    it = m_fragShaders.begin();
    while(it != m_fragShaders.end())
    {
        if(n == 0)
        {
            if(t) *t = FX_FRAGPROG;
            return it->second;
        }
        --n;
        ++it;
    }
    it = m_computeShaders.begin();
    while(it != m_computeShaders.end())
    {
        if(n == 0)
        {
            if(t) *t = FX_COMPUTEPROG;
            return it->second;
        }
        --n;
        ++it;
    }
    // TODO: Add the other stages (Geom etc.)
    return NULL;
}
/*************************************************************************/ /**
 ** 
 ** 
 **/ /*************************************************************************/ 
IShader*    GLSLProgram::getShader(IShader *pShader, ShaderType *t)
{
    ShaderMap::iterator it;
    it = m_vtxShaders.begin();
    while(it != m_vtxShaders.end())
    {
        if(it->second == pShader)
        {
            if(t) *t = FX_VTXPROG;
            return it->second;
        }
        ++it;
    }
    it = m_geomShaders.begin();
    while(it != m_geomShaders.end())
    {
        if(it->second == pShader)
        {
            if(t) *t = FX_GEOMPROG;
            return it->second;
        }
        ++it;
    }
    it = m_tcsShaders.begin();
    while(it != m_tcsShaders.end())
    {
        if(it->second == pShader)
        {
            if(t) *t = FX_TCSPROG;
            return it->second;
        }
        ++it;
    }
    it = m_tesShaders.begin();
    while(it != m_tesShaders.end())
    {
        if(it->second == pShader)
        {
            if(t) *t = FX_TESPROG;
            return it->second;
        }
        ++it;
    }
    it = m_fragShaders.begin();
    while(it != m_fragShaders.end())
    {
        if(it->second == pShader)
        {
            if(t) *t = FX_FRAGPROG;
            return it->second;
        }
        ++it;
    }
    it = m_computeShaders.begin();
    while(it != m_computeShaders.end())
    {
        if(it->second == pShader)
        {
            if(t) *t = FX_COMPUTEPROG;
            return it->second;
        }
        ++it;
    }
    // TODO: Add the other stages (Geom etc.)
    return NULL;
}
/*************************************************************************/ /**
 ** 
 ** 
 **/ /*************************************************************************/ 
IShader*    GLSLProgram::getShader(const char *name, ShaderType *t)
{
    ShaderMap::iterator it;
    it = m_vtxShaders.find(name);
    if(it != m_vtxShaders.end())
    {
        if(t) *t = FX_VTXPROG;
        return it->second;
    }
    if(it != m_geomShaders.end())
    {
        if(t) *t = FX_GEOMPROG;
        return it->second;
    }
    if(it != m_tcsShaders.end())
    {
        if(t) *t = FX_TCSPROG;
        return it->second;
    }
    if(it != m_tesShaders.end())
    {
        if(t) *t = FX_TESPROG;
        return it->second;
    }
    it = m_fragShaders.find(name);
    if(it != m_fragShaders.end())
    {
        if(t) *t = FX_FRAGPROG;
        return it->second;
    }
    it = m_computeShaders.find(name);
    if(it != m_computeShaders.end())
    {
        if(t) *t = FX_COMPUTEPROG;
        return it->second;
    }
    // TODO: Add the other stages (Geom etc.)
    return NULL;
}
/*************************************************************************/ /**
 ** 
 ** 
 **/ /*************************************************************************/ 
int         GLSLProgram::getNumShaders()
{
    return (int)m_vtxShaders.size() + (int)m_fragShaders.size() + (int)m_computeShaders.size()
        +(int)m_geomShaders.size() + (int)m_tcsShaders.size() + (int)m_tesShaders.size();
}

/*************************************************************************/ /**
 ** 
 ** 
 **/ /*************************************************************************/ 
//TODO: geom, tess...

void GLSLProgram::cleanup()
{
    /*ShaderVec::iterator icshd = m_commonShaders.begin();
    while(icshd != m_commonShaders.end())
    {
        (*icshd)->cleanupShader();
        ++icshd;
    }*/
    if(m_program)
    {
#ifndef SHADERCONCAT_USE
        // TODO: add the missing ones...
        ShaderMap::iterator ivshd = m_vtxShaders.begin();
        while(ivshd != m_vtxShaders.end())
        {
            //ivshd->second->cleanupShader();
            GLhandleARB o = ivshd->second->getGLSLShaderObj(FX_VTXPROG);
            ivshd->second->removeUser(this);
            glDetachShader(m_program, o);
            ++ivshd;
        }
#ifndef OGLES2
        ShaderMap::iterator igshd = m_geomShaders.begin();
        while(igshd != m_geomShaders.end())
        {
            //igshd->second->cleanupShader();
            GLhandleARB o = igshd->second->getGLSLShaderObj(FX_GEOMPROG);
            igshd->second->removeUser(this);
            glDetachObjectARB(m_program, o);
            ++igshd;
        }
        ShaderMap::iterator icshd = m_tcsShaders.begin();
        while(icshd != m_tcsShaders.end())
        {
            //icshd->second->cleanupShader();
            GLhandleARB o = icshd->second->getGLSLShaderObj(FX_TCSPROG);
            icshd->second->removeUser(this);
            glDetachObjectARB(m_program, o);
            ++icshd;
        }
        ShaderMap::iterator ieshd = m_tesShaders.begin();
        while(ieshd != m_tesShaders.end())
        {
            //ieshd->second->cleanupShader();
            GLhandleARB o = ieshd->second->getGLSLShaderObj(FX_TESPROG);
            ieshd->second->removeUser(this);
            glDetachObjectARB(m_program, o);
            ++ieshd;
        }
#endif
        ShaderMap::iterator ifshd = m_fragShaders.begin();
        while(ifshd != m_fragShaders.end())
        {
            //ifshd->second->cleanupShader();
            GLhandleARB o = ifshd->second->getGLSLShaderObj(FX_FRAGPROG);
            ifshd->second->removeUser(this);
#if 1 //def OGLES2
            glDetachShader(m_program, o);
#else
            glDetachObjectARB(m_program, o);
#endif
            ++ifshd;
        }
#ifndef OGLES2
        icshd = m_computeShaders.begin();
        while(icshd != m_computeShaders.end())
        {
            //icshd->second->cleanupShader();
            GLhandleARB o = icshd->second->getGLSLShaderObj(FX_COMPUTEPROG);
            icshd->second->removeUser(this);
            glDetachObjectARB(m_program, o);
            ++icshd;
        }
#endif
#else
    if(m_vtxShader)
#  if 1 //def OGLES2
        glDeleteShader(m_vtxShader);
#  else
        glDeleteObjectARB(m_vtxShader);
#  endif
    m_vtxShader = 0;
    if(m_fragShader)
#  if 1 //def OGLES2
        glDeleteShader(m_fragShader);
#  else
        glDeleteObjectARB(m_fragShader);
#  endif
    m_fragShader = 0;
#endif
    }
#ifndef SHADERCONCAT_USE
    //
    // then delete the allocations
    //
	// NO : the shaders don't belong to the program. They belong to the container
    //ShaderMap::iterator it = m_vtxShaders.begin();
    //while(it != m_vtxShaders.end())
    //{
    //    delete it->second;
    //    ++it;
    //}
    m_vtxShaders.clear();

    m_geomShaders.clear();
    m_tcsShaders.clear();
    m_tesShaders.clear();

    //it = m_fragShaders.begin();
    //while(it != m_fragShaders.end())
    //{
    //    delete it->second;
    //    ++it;
    //}
    m_fragShaders.clear();

    //it = m_computeShaders.begin();
    //while(it != m_computeShaders.end())
    //{
    //    delete it->second;
    //    ++it;
    //}
    m_computeShaders.clear();
    // TODO : take care of other pipeline stages
#endif
}
/*************************************************************************/ /**
 ** 
 ** TODO: in SHADERCONCAT_USE, add error log with proper line numbers to nvFX files
 **
 **/ /*************************************************************************/ 
bool GLSLProgram::link(IContainer* pContainer)
{
    char localBuf[1024];
    int len;
    char* buf = localBuf;
    buf[0] = '\0';
    if(m_linkNeeded)
    {
        //
        // add common parts for line-number conversion :-(
        //
        std::vector<GLSLShader::codeInfo> codeBlocks;
        int lineOffset = 0;
        ShaderMap::iterator iShd, iEnd;
//nvFX::printf("=====================================\n" );
        for(int s=0; s<6; s++)
        {
            switch(s)
            {
            case 0:
                iShd = m_vtxShaders.begin();
                iEnd = m_vtxShaders.end();
                break;
            case 1:
                iShd = m_fragShaders.begin();
                iEnd = m_fragShaders.end();
                break;
            case 2:
                iShd = m_computeShaders.begin();
                iEnd = m_computeShaders.end();
                break;
            case 3:
                iShd = m_geomShaders.begin();
                iEnd = m_geomShaders.end();
                break;
            case 4:
                iShd = m_tcsShaders.begin();
                iEnd = m_tcsShaders.end();
                break;
            case 5:
                iShd = m_tesShaders.begin();
                iEnd = m_tesShaders.end();
                break;
            }
            for(;iShd != iEnd; iShd++)
            {
//nvFX::printf("----\n%s\n", iShd->second->m_shaderCode.c_str() );
                for(int n=0; n<(int)iShd->second->m_startLineNumbers.size(); n++)
                    codeBlocks.push_back(iShd->second->m_startLineNumbers[n]);
            }
        }
#ifdef SHADERCONCAT_USE
        //------------------------------------------------------------------------------------------
        // SHADERCONCAT_USE SHADERCONCAT_USE SHADERCONCAT_USE SHADERCONCAT_USE SHADERCONCAT_USE SHADERCONCAT_USE SHADERCONCAT_USE SHADERCONCAT_USE SHADERCONCAT_USE SHADERCONCAT_USE
        #pragma MESSAGE("WARNING : No use of Compute in lame use !!")
        GLenum err = glGetError();
        m_VPCode.clear();
        m_FPCode.clear();
        // Add common header code
        IShader *pShd;
        for(int i=0; pShd = static_cast<GLSLShader*>(pContainer->findShader(i)); i++)
            if((*(pShd->getName()) == '\0') && (pShd->getType() == TGLSL) )
            {
                m_VPCode += pShd->getShaderCode();
                m_FPCode += pShd->getShaderCode();
            }
        iShd = m_vtxShaders.begin();
        iEnd = m_vtxShaders.end();
        for(;iShd != iEnd; iShd++)
            m_VPCode += iShd->second->getShaderCode();
        iShd = m_fragShaders.begin();
        iEnd = m_fragShaders.end();
        for(;iShd != iEnd; iShd++)
            m_FPCode += iShd->second->getShaderCode();
#if 1 //def OGLES2
        if(m_vtxShaders.size())
            m_vtxShader = glCreateShader(GL_VERTEX_SHADER);
        if(m_fragShaders.size())
            m_fragShader = glCreateShader(GL_FRAGMENT_SHADER);
#else
        if(m_vtxShaders.size())
            m_vtxShader = glCreateShaderObjectARB(GL_VERTEX_SHADER);
        if(m_fragShaders.size())
            m_fragShader = glCreateShaderObjectARB(GL_FRAGMENT_SHADER);
#endif
        GLint size[2] = { m_VPCode.length(), m_FPCode.length() };
        const char* progString[2] = { m_VPCode.c_str(), m_FPCode.c_str() };
        if(m_vtxShader)
        {
            //printf("===============================================================\nVP Code: \n%s", progString[0]);
            glShaderSource(m_vtxShader, 1, progString, size);
            glCompileShader(m_vtxShader);
#if 1 //def OGLES2
            glGetShaderInfoLog (m_vtxShader, 1024, &len, buf);
#else
            glGetInfoLogARB(m_vtxShader, 1024, &len, buf);
#endif
            if(len)
                nvFX::printf("Log for Shader: \n%s\n", buf);
#if 1 //def OGLES2
            glAttachShader(m_program, m_vtxShader);
#else
            glAttachObjectARB(m_program, m_vtxShader);
#endif
        }
        if(m_fragShader)
        {
            //printf("===============================================================\nFP Code: \n%s", progString[1]);
            glShaderSource(m_fragShader, 1, progString+1, size+1);
            glCompileShader(m_fragShader);
#if 1 //def OGLES2
            glGetShaderInfoLog (m_fragShader, 1024, &len, buf);
#else
            glGetInfoLogARB(m_fragShader, 1024, &len, buf);
#endif
            if(len)
                nvFX::printf("Log for Shader: \n%s\n", buf);
#if 1 //def OGLES2
            glAttachShader(m_program, m_fragShader);
#else
            glAttachObjectARB(m_program, m_fragShader);
#endif
        }
        err = glGetError();
        if(err != GL_NONE)
        {
            nvFX::printf("Error while Attaching Shaders to the program %d :Error 0x%x\n", m_program, err);
            return false;
        }
        // SHADERCONCAT_USE SHADERCONCAT_USE SHADERCONCAT_USE SHADERCONCAT_USE SHADERCONCAT_USE SHADERCONCAT_USE SHADERCONCAT_USE SHADERCONCAT_USE SHADERCONCAT_USE SHADERCONCAT_USE
        //------------------------------------------------------------------------------------------
#endif
        glLinkProgram(m_program);
        //
        // Get Log even if things went well... for warnings or other messages
        //
#if 1 //def OGLES2
        glGetProgramInfoLog (m_program, 1024, &len, buf);
#else
        glGetInfoLogARB(m_program, 1023, &len, buf);
#endif
        if(len)
        {
            //nvFX::printf("\n======================\n%s\n======================\n", buf);
            char *p = strstr(buf, "0(");
            //nvFX::printf("Log for Program %d (", m_program);
            for(int s=0; s<3; s++)
            {
                switch(s)
                {
                case 0:
                    iShd = m_vtxShaders.begin();
                    iEnd = m_vtxShaders.end();
                    break;
                case 1:
                    iShd = m_fragShaders.begin();
                    iEnd = m_fragShaders.end();
                    break;
                case 2:
                    iShd = m_computeShaders.begin();
                    iEnd = m_computeShaders.end();
                    break;
                case 3:
                    iShd = m_geomShaders.begin();
                    iEnd = m_geomShaders.end();
                    break;
                case 4:
                    iShd = m_tcsShaders.begin();
                    iEnd = m_tcsShaders.end();
                    break;
                case 5:
                    iShd = m_tesShaders.begin();
                    iEnd = m_tesShaders.end();
                    break;
                }
                for(;iShd != iEnd; iShd++)
                {
                    nvFX::printf("%s, ", iShd->second->m_name.c_str() );
                }
            }
            nvFX::printf(")\n");
#ifdef OGLES2
            if(p==NULL)
                p = strstr(buf, "0:"); // iOS error display
#endif
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
#   ifdef ES2EMU
                num -= 139; // WHY ?!?!?!?!
#   endif
                num--;
                CHECK_TRUE( num >= 0 );
                int offsetNum = num;
                std::vector<GLSLShader::codeInfo>::iterator iCB;
                std::vector<GLSLShader::codeInfo>::iterator iCBFound = codeBlocks.end();
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
        GLint status = GL_FALSE;
        glGetProgramiv(m_program, GL_LINK_STATUS, &status);
        if(status == GL_FALSE)
        {
#if 1 //def OGLES2
            glDeleteProgram(m_program);
#else
            glDeleteObjectARB(m_program);
#endif
            m_program = 0;
            //m_bound = false;
            return false;
        }
        else
            m_linkNeeded = false;
    } //if(m_linkNeeded)
    else // we still invoke link : needed after glBindAttribLocation
    {
        glLinkProgram(m_program);
        //
        // Get Log even if things went well... for warnings or other messages
        //
        localBuf[0] = '\0';
#if 1 //def OGLES2
        glGetProgramInfoLog (m_program, 1024, &len, localBuf);
#else
        glGetInfoLogARB(m_program, 1024, &len, localBuf);
#endif
        if(len)
        {
            nvFX::printf("linking again messages : %s\n", localBuf);
            //assert(0);
        }
    }
    return true;
}
/*************************************************************************/ /**
 ** 
 ** TODO: in SHADERCONCAT_USE, add error log with proper line numbers to nvFX files
 ** 
 **/ /*************************************************************************/ 
bool GLSLProgram::bind(IContainer* pContainer)
{
    if(m_linkNeeded)
        if(!link(pContainer))
            return false;
    glUseProgram(m_program);
    //
    // If we have some sub-routines, bind them
    // Any time you get an error from here : often means that the size of m_srIndicesXX is different
    // from the expected size of sub-routines (the spec tells you must provide all the offset for all the uniform sub-routines)
    //
#ifndef OGLES2
    if(m_srIndicesVS.size()>0)
        glUniformSubroutinesuiv(GL_VERTEX_SHADER_ARB, (int)m_srIndicesVS.size(), &(m_srIndicesVS[0]));
    if(m_srIndicesFS.size()>0)
        glUniformSubroutinesuiv(GL_FRAGMENT_SHADER_ARB, (int)m_srIndicesFS.size(), &(m_srIndicesFS[0]));
    if(m_srIndicesGS.size()>0)
        glUniformSubroutinesuiv(GL_GEOMETRY_SHADER_ARB, (int)m_srIndicesGS.size(), &(m_srIndicesGS[0]));
    if(m_srIndicesTCS.size()>0)
        glUniformSubroutinesuiv(GL_TESS_CONTROL_SHADER, (int)m_srIndicesTCS.size(), &(m_srIndicesTCS[0]));
    if(m_srIndicesTES.size()>0)
        glUniformSubroutinesuiv(GL_TESS_EVALUATION_SHADER, (int)m_srIndicesTES.size(), &(m_srIndicesTES[0]));
    if(m_srIndicesCS.size()>0)
        glUniformSubroutinesuiv(0x91B9/*GL_COMPUTE_SHADER*/, (int)m_srIndicesCS.size(), &(m_srIndicesCS[0]));
#endif
    return true;
}
/*************************************************************************/ /**
 ** 
 ** 
 **/ /*************************************************************************/ 
void GLSLProgram::unbind(IContainer* pContainer)
{
	//m_bound = false;
	glUseProgram(0);
}

/*************************************************************************/ /**
 ** 
 ** 
 **/ /*************************************************************************/ 
int GLSLProgram::getUniformLocation(const char* name)
{
    return glGetUniformLocation(m_program, name);
}

/*************************************************************************/ /**
 ** 
 ** 
 **/ /*************************************************************************/ 
int GLSLProgram::getASMCode(char* buffer, int bufLen)
{
    if (strstr((const char*)glGetString(GL_VENDOR), "NVIDIA"))
    {
        GLsizei binaryLength = 0;
        GLenum format = 0;
        glGetProgramiv(m_program, GL_PROGRAM_BINARY_LENGTH,&binaryLength);
        std::string binary(size_t(binaryLength+1),0);
        glGetProgramBinary(m_program,binaryLength,NULL,&format,&binary[0]);
        size_t pos = binary.rfind("!!NV");
        const char* nvasm = pos != std::string::npos ? &binary[pos] : NULL;
        if(nvasm)
        {
            int l = strlen(nvasm);
            if(buffer)
            {
                int l2 = bufLen < l ? bufLen:l;
                strncpy(buffer, nvasm, l2);
                buffer[l2-1] = '\0';
            }
            return l+1;
        }
    }
    return 0;
}

/*************************************************************************/ /**
 ** 
 ** 
 **/ /*************************************************************************/ 

void GLSLProgram::setUniform(const char* name, float val)
{
	glUniform1f(glGetUniformLocation(m_program, name), val);
}
/*************************************************************************/ /**
 ** 
 ** 
 **/ /*************************************************************************/ 
void GLSLProgram::setUniform(const char* name, int val)
{
    glUniform1i(glGetUniformLocation(m_program, name), val);
}
/*************************************************************************/ /**
 ** 
 ** 
 **/ /*************************************************************************/ 
void GLSLProgram::setUniformVector(const char * name, const float* val, int count, int numUniforms)
{
    GLint location = glGetUniformLocation(m_program, name);
    printf("location for %s = %d\n", name, location);
    if (location >= 0) 
    {
        switch (count) {
            case 1:
                glUniform1fv(location, numUniforms, val);
                break;
            case 2:
                glUniform2fv(location, numUniforms, val);
                break;
            case 3:
                glUniform3fv(location, numUniforms, val);
                break;
            case 4:
                glUniform4fv(location, numUniforms, val);
                break;
        }
    } else {
        printf("BAD uniform offset for %s!\n", name);
    }
    GLenum ee = glGetError();
    if(ee){
        printf("GL Error %d\n", ee);
    }
}
/*************************************************************************/ /**
 ** 
 ** 
 **/ /*************************************************************************/ 
void GLSLProgram::setUniformVector(const char * name, const int* val, int count, int numUniforms)
{
    GLint location = glGetUniformLocation(m_program, name);
    if (location == -1) {
        return;
    }
    switch (count) {
        case 1:
            glUniform1iv(location, numUniforms, val);
            break;
        case 2:
            glUniform2iv(location, numUniforms, val);
            break;
        case 3:
            glUniform3iv(location, numUniforms, val);
            break;
        case 4:
            glUniform4iv(location, numUniforms, val);
            break;
    }
}
/*************************************************************************/ /**
 ** 
 ** 
 **/ /*************************************************************************/ 
void GLSLProgram::setSampler(const char * texname, int i)
{
    GLint linked = GL_TRUE;
    glGetProgramiv(m_program, GL_LINK_STATUS, &linked);
    if (linked != GL_TRUE) {
        return;
    }
    GLint location = glGetUniformLocation(m_program, texname);
    if (location == -1) {
        return;
    }
    glUniform1i(location, i);
}
/*************************************************************************/ /**
 ** TODO: remove this method...
 ** we can't assign separately sub-routines... we must update all at the same time
 **/ /*************************************************************************/ 
void GLSLProgram::bindTexture(ResourceType target, const char * texname, GLuint texid, int i)
{
    glActiveTexture(GL_TEXTURE0 + i);
    glBindTexture(target, texid);
    setSampler(texname, i);
    glActiveTexture(GL_TEXTURE0);
}

/*************************************************************************/ /**
 ** \brief associate to a sub-routine uniform ID some sub-routine implementations
 **
 ** this method is not supposed to be called every frame. It should be called
 ** during init time. tables (m_srIndicesVS etc.) will contain the proper relations
 ** that will be invoked at bind() time (via glUniformSubroutinesuiv)
 ** \todo This function could double check possible errors and return details
 ** for now, if wrong binding, bind() will generate an error (via glUniformSubroutinesuiv)
 **/ /*************************************************************************/ 
#ifndef OGLES2
bool GLSLProgram::bindSubRoutineToUniform(int uniformID, GLenum shadertype, char **subroutineNames, int numNames)
{
    bool bRes = true;
    //GLint maxlength, umaxlength;
    //GLint sru, srul, sr;
    //glGetProgramStageiv(m_program, shadertype, GL_ACTIVE_SUBROUTINE_MAX_LENGTH, &maxlength);
    //glGetProgramStageiv(m_program, shadertype, GL_ACTIVE_SUBROUTINE_UNIFORM_MAX_LENGTH, &umaxlength);
    //glGetProgramStageiv(m_program, shadertype, GL_ACTIVE_SUBROUTINE_UNIFORM_LOCATIONS, &srul);
    //glGetProgramStageiv(m_program, shadertype, GL_ACTIVE_SUBROUTINE_UNIFORMS, &sru);
    //glGetProgramStageiv(m_program, shadertype, GL_ACTIVE_SUBROUTINES, &sr);
    //void GetActiveSubroutineUniformiv(m_program, shadertype,
    //                                  uint index, enum pname, int *values);
    //void GetActiveSubroutineUniformName(m_program, shadertype,
    //                                  uint index, sizei bufsize,
    //                                        sizei *length, char *name);

    int i=0;
    while(i < numNames)
    {
        if(*subroutineNames)
        {
            GLint id = glGetSubroutineIndex(m_program, shadertype, *subroutineNames);
            if(id >= 0)
            {
                switch(shadertype)
                {
                case GL_VERTEX_SHADER_ARB:
                    if(uniformID >= (int)m_srIndicesVS.size())
                        m_srIndicesVS.resize(uniformID+1);
                    m_srIndicesVS[uniformID] = id;
                    break;
                case GL_FRAGMENT_SHADER_ARB:
                    if(uniformID >= (int)m_srIndicesFS.size())
                        m_srIndicesFS.resize(uniformID+1);
                    m_srIndicesFS[uniformID] = id;
                    break;
                case GL_GEOMETRY_SHADER_ARB:
                    if(uniformID >= (int)m_srIndicesGS.size())
                        m_srIndicesGS.resize(uniformID+1);
                    m_srIndicesGS[uniformID] = id;
                    break;
                case GL_TESS_CONTROL_SHADER:
                    if(uniformID >= (int)m_srIndicesTCS.size())
                        m_srIndicesTCS.resize(uniformID+1);
                    m_srIndicesTCS[uniformID] = id;
                    break;
                case GL_TESS_EVALUATION_SHADER:
                    if(uniformID >= (int)m_srIndicesTES.size())
                        m_srIndicesTES.resize(uniformID+1);
                    m_srIndicesTES[uniformID] = id;
                    break;
                case 0x91B9/*GL_COMPUTE_SHADER*/:
                    if(uniformID >= (int)m_srIndicesCS.size())
                        m_srIndicesCS.resize(uniformID+1);
                    m_srIndicesCS[uniformID] = id;
                    break;
                }
            } else
                bRes = false;
        }
        ++uniformID;
        ++subroutineNames;
        ++i;
    }
    return bRes;
}
int GLSLProgram::getSubRoutineID(const char *name, GLenum shadertype)
{
    return glGetSubroutineIndex(m_program, shadertype, name);
}

#endif //Subroutine

int  GLSLProgram::getAttribLocation(const char* attrName)
{
    return glGetAttribLocation(m_program, attrName);
}
void GLSLProgram::bindAttribLocation(int i, const char* attrName)
{
    glBindAttribLocation(m_program, i, attrName);
}

/////////////////////////////////////////////////////////////////////////////
// TEMP TEMP TEMP TEMP TEMP TEMP TEMP TEMP TEMP TEMP TEMP TEMP TEMP TEMP TEMP
// TEMP TEMP TEMP TEMP TEMP TEMP TEMP TEMP TEMP TEMP TEMP TEMP TEMP TEMP TEMP
// temporary if Glew doesn't support it
#define        MAX_COMPUTE_UNIFORM_BLOCKS                      0x91BB
#define        MAX_COMPUTE_TEXTURE_IMAGE_UNITS                 0x91BC
#define        MAX_COMPUTE_IMAGE_UNIFORMS                      0x91BD
#define        MAX_COMPUTE_SHARED_MEMORY_SIZE                  0x8262  
#define        MAX_COMPUTE_UNIFORM_COMPONENTS                  0x8263  
#define        MAX_COMPUTE_ATOMIC_COUNTER_BUFFERS              0x8264  
#define        MAX_COMPUTE_ATOMIC_COUNTERS                     0x8265  
#define        MAX_COMBINED_COMPUTE_UNIFORM_COMPONENTS         0x8266  
//Accepted by the <pname> parameter of GetIntegeri_v, GetBooleani_v,
//GetFloati_v, GetDoublei_v and GetInteger64i_v:
#define        MAX_COMPUTE_WORK_GROUP_COUNT                    0x91BE
#define        MAX_COMPUTE_WORK_GROUP_SIZE                     0x91BF
//Accepted by the <pname> parameter of GetProgramiv:
#define        COMPUTE_LOCAL_WORK_SIZE                         0x8267  

#ifndef OGLES2
	typedef void (GLAPIENTRY * PFNGLDISPATCHCOMPUTEARBPROC) (GLuint num_groups_x,GLuint num_groups_y,GLuint num_groups_z);
	PFNGLDISPATCHCOMPUTEARBPROC glDispatchComputeARB;
#endif
// TEMP TEMP TEMP TEMP TEMP TEMP TEMP TEMP TEMP TEMP TEMP TEMP TEMP TEMP TEMP
// TEMP TEMP TEMP TEMP TEMP TEMP TEMP TEMP TEMP TEMP TEMP TEMP TEMP TEMP TEMP
/////////////////////////////////////////////////////////////////////////////
bool GLSLProgram::execute(int szx, int szy, int szz)
{
#ifdef USEGLSLCOMPUTE
    if(!glDispatchComputeARB)
#ifdef WIN32
        glDispatchComputeARB =  (PFNGLDISPATCHCOMPUTEARBPROC)wglGetProcAddress("glDispatchCompute");
#else
	glDispatchComputeARB =  (PFNGLDISPATCHCOMPUTEARBPROC)glXGetProcAddress((GLubyte*)"glDispatchCompute");
#endif
    if(!glDispatchComputeARB)
        return false;
    glDispatchComputeARB(szx, szy, szz);
    GLenum err = glGetError();
    if(err == GL_NONE)
        return true;
    // TODO...
    //INVALID_OPERATION is generated by DispatchCompute if the current program
    //pipeline object contains no executable code for the compute shader stage.
    //INVALID_VALUE is generated by DispatchCompute if any of <num_groups_x>,
    //<num_groups_y> or <num_groups_z> is greater than the value of
    //MAX_COMPUTE_WORK_GROUP_COUNT for the corresponding dimension.
    //INVALID_OPERATION is generated by DispatchComputeIndirect if no buffer is
    //bound to the DRAW_INDIRECT_BUFFER target.
#endif
    return false;
}

