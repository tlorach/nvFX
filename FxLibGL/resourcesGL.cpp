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
/// \brief
#define BFMTCASE(a, i, e, s) a = i,
#ifndef OGLES2
/// \brief this internal format list \e must be the same as ResourceFormat !
#define FORMATENUMS\
    /*BFMTCASE(FMT_INTENSITY, , GL_INTENSITY8)*/\
    BFMTCASE(FMT_INTENSITY8, 1, GL_INTENSITY8, 1)\
    BFMTCASE(FMT_INTENSITY16F, 2, GL_INTENSITY16F_ARB, 2)\
    BFMTCASE(FMT_INTENSITY32F, 3, GL_INTENSITY32F_ARB, 4)\
    /*BFMTCASE(FMT_LUMINANCE_ALPHA, , GL_LUMINANCE8_ALPHA8, 2)*/\
    BFMTCASE(FMT_LUMINANCE_ALPHA8, 4, GL_LUMINANCE8_ALPHA8, 2)\
    /*BFMTCASE(FMT_LA, , GL_LUMINANCE8_ALPHA8, 2)*/\
    /*BFMTCASE(FMT_LA8, , GL_LUMINANCE8_ALPHA8, 2)*/\
    BFMTCASE(FMT_RG16F_NV, 5, GL_FLOAT_RG16_NV, 4)\
    /*BFMTCASE(FMT_LA16F, , GL_LUMINANCE_ALPHA16F_ARB, 4)*/\
    BFMTCASE(FMT_LUMINANCE_ALPHA16F, 6, GL_LUMINANCE_ALPHA16F_ARB, 4)\
    /*BFMTCASE(FMT_LA32F, , GL_LUMINANCE_ALPHA32F_ARB, 8)*/\
    BFMTCASE(FMT_LUMINANCE_ALPHA32F, 7, GL_LUMINANCE_ALPHA32F_ARB, 8)\
    BFMTCASE(FMT_RG32F_NV, 8, GL_FLOAT_RG32_NV, 8)\
    /*BFMTCASE(FMT_RGB, , GL_RGB8, 3)*/\
    BFMTCASE(FMT_RGB8, 9, GL_RGB8, 3)\
    BFMTCASE(FMT_RGB16F, 10, GL_RGB16F_ARB, 6)\
    BFMTCASE(FMT_RGB32F, 11, GL_RGB32F_ARB, 12)\
    /*BFMTCASE(FMT_RGBA, , GL_RGBA8, 4)*/\
    BFMTCASE(FMT_RGBA8, 12, GL_RGBA8, 4)\
    BFMTCASE(FMT_RGBA16F, 13, GL_RGBA16F_ARB, 8)\
    BFMTCASE(FMT_RGBA32F, 14, GL_RGBA32F_ARB, 16)\
    BFMTCASE(FMT_DEPTH24STENCIL8, 15, GL_DEPTH24_STENCIL8, 4)\
    BFMTCASE(FMT_DEPTH_COMPONENT32F, 16, 0x8CAC, 4) /* see NV_depth_buffer_float or ARB_depth_buffer_float*/ \
    BFMTCASE(FMT_DEPTH32F_STENCIL8, 17, 0x8CAD, 4) /* see ARB_depth_buffer_float*/ \
    BFMTCASE(FMT_RGBA8UI, 18, GL_RGBA8UI, 4) /* see ARB_depth_buffer_float*/ \
    BFMTCASE(FMT_FLOATRGBA16NV, 19, GL_FLOAT_RGBA16_NV, 8) /*see GL_NV_float_buffer*/\
    BFMTCASE(FMT_RG,20, GL_RG, 2)\
    BFMTCASE(FMT_RG_INTEGER,21, GL_RG_INTEGER, 8)\
    BFMTCASE(FMT_R8,22, GL_R8, 1)\
    BFMTCASE(FMT_R16,23, GL_R16, 2)\
    BFMTCASE(FMT_RG8,24, GL_RG8, 2)\
    BFMTCASE(FMT_RG16,25, GL_RG16, 4)\
    BFMTCASE(FMT_R16F,26, GL_R16F, 2)\
    BFMTCASE(FMT_R32F,27, GL_R32F, 4)\
    BFMTCASE(FMT_RG16F,28, GL_RG16F, 4)\
    BFMTCASE(FMT_RG32F,29, GL_RG32F, 8)\
    BFMTCASE(FMT_R8I,30, GL_R8I, 1)\
    BFMTCASE(FMT_R8UI,31, GL_R8UI, 1)\
    BFMTCASE(FMT_R16I,32, GL_R16I, 2)\
    BFMTCASE(FMT_R16UI,33, GL_R16UI, 2)\
    BFMTCASE(FMT_R32I,34, GL_R32I, 4)\
    BFMTCASE(FMT_R32UI,35, GL_R32UI, 4)\
    BFMTCASE(FMT_RG8I,36, GL_RG8I, 2)\
    BFMTCASE(FMT_RG8UI,37, GL_RG8UI, 2)\
    BFMTCASE(FMT_RG16I,38, GL_RG16I, 4)\
    BFMTCASE(FMT_RG16UI,39, GL_RG16UI, 4)\
    BFMTCASE(FMT_RG32I,40, GL_RG32I, 8)\
    BFMTCASE(FMT_RG32UI,41, GL_RG32UI, 8)
#else
/// \brief this internal format list \e must be the same as ResourceFormat !
#define FORMATENUMS\
    BFMTCASE(FMT_ALPHA, 1, GL_ALPHA, 1)\
    BFMTCASE(FMT_LUMINANCE, 2, GL_LUMINANCE, 1)\
    BFMTCASE(FMT_LUMINANCE_ALPHA, 3, GL_LUMINANCE_ALPHA, 2)\
    BFMTCASE(FMT_RGB, 4, GL_RGB, 3)\
    BFMTCASE(FMT_RGBA, 5, GL_RGBA, 4)
#endif
// See FxLib.h nvFX::ResourceFormat instead
//enum BufferFormat {    FORMATENUMS };

#undef BFMTCASE
#define BFMTCASE(a, i, e, s) case a: return e;
/// \brief
GLenum ResourceFormatAsGL(ResourceFormat f)
{
    switch(f) { FORMATENUMS }
    return 0;
}

#undef BFMTCASE
#define BFMTCASE(a, i, e, s) case a: return #a;
/// \brief
const char * ResourceFormatAsString(ResourceFormat f)
{
    switch(f) { FORMATENUMS }
    return "Error>> unknown format";
}

#undef BFMTCASE
#define BFMTCASE(a, i, e, s) case e: return #e;
/// \brief
const char * ResourceFormatAsString(GLenum f)
{
    switch(f) { FORMATENUMS }
    return "Error>> unknown format";
}

#undef BFMTCASE
#define BFMTCASE(a, i, e, s) case i: return s;
/// \brief
int ResourceFormatByteSize(int f)
{
    switch(f) { FORMATENUMS }
    return 0;
}
#undef BFMTCASE
#define BFMTCASE(a, i, e, s) case a: return s;
/// \brief
int ResourceFormatByteSize(ResourceFormat f)
{
    switch(f) { FORMATENUMS }
    return 0;
}

/*************************************************************************/ /**
 ** \brief
 **
 **/ /*************************************************************************/ 
bool createRenderBuffer(int w, int h, GLenum format, int depthSamples, int coverageSamples)
{
    NXPROFILEFUNCCOL2(__FUNCTION__, COLOR_CYAN, 12);
    CHECKGLERRORS("before createRenderBuffer");
#ifndef OGLES2
    int query;
    bool multisample = depthSamples > 1;
    bool csaa = (coverageSamples > depthSamples) && (glRenderbufferStorageMultisampleCoverageNV != NULL);
    //
    // NVIDIA CSAA
    //
    if(csaa)
    {
        glRenderbufferStorageMultisampleCoverageNV( GL_RENDERBUFFER, coverageSamples, depthSamples, format, w, h);

        glGetRenderbufferParameteriv( GL_RENDERBUFFER, GL_RENDERBUFFER_COVERAGE_SAMPLES_NV, &query);
        if ( query < coverageSamples)
        {    
            return false;
        }
        else if ( query > coverageSamples) 
            coverageSamples = query;

        glGetRenderbufferParameteriv( GL_RENDERBUFFER, GL_RENDERBUFFER_COLOR_SAMPLES_NV, &query);
        if ( query < depthSamples)
        {    
            return false;
        }
        else if ( query > depthSamples) 
            depthSamples = query;
    }
    else if(multisample)
    {
        glRenderbufferStorageMultisample( GL_RENDERBUFFER, depthSamples, format, w, h);
        glGetRenderbufferParameteriv( GL_RENDERBUFFER, GL_RENDERBUFFER_SAMPLES, &query);
        if ( query < depthSamples) 
        {    
            return false;
        }
        else if ( query > depthSamples) 
            depthSamples = query;

    }
    else
#endif
    {
        glRenderbufferStorage (GL_RENDERBUFFER, format, w, h);
    }
    if(CHECKGLERRORS("createRenderBuffer"))
        return false;
    return true;
}

/*************************************************************************/ /**
 ** \brief
 **
 **/ /*************************************************************************/ 
ResourceGL::ResourceGL(ResourceRepository* pCont, const char *name) : 
#ifdef USECUDA
ResourceCUDA(pCont, name)
#else
Resource(pCont, name)
#endif
{
}
ResourceGL::~ResourceGL()
{
    invalidate();
}

/*************************************************************************/ /**
 ** \brief
 **
 **/ /*************************************************************************/ 
bool ResourceGL::createRenderResource()
{
    NXPROFILEFUNCCOL2(__FUNCTION__, COLOR_CYAN, 12);
    /*if(m_OGLId)
    {
        switch(m_type)
        {
        case RESRBUF_2D:
            glDeleteRenderbuffers(1, &m_OGLId);
            break;
        case RESTEX_1D:
        case RESTEX_2D:
        case RESTEX_2DRECT:
        //case RESTEX_3D:
        //case RESTEX_CUBE:
            glDeleteTextures(1, &(m_OGLId));
            break;
        }
        m_OGLId = 0;
    }*/
    if(m_creationData.appDepSz)
    {
        const int* vp = m_pOwner->getViewport();
        m_creationData.appDepSz = true;
        m_creationData.sz[0] = vp[2];
        m_creationData.sz[1] = vp[3];
    }
    if(m_creationData.szFact[0] == 0.0f)
        m_creationData.szFact[0] = 1.0f;
    if(m_creationData.szFact[1] == 0.0f)
        m_creationData.szFact[1] = 1.0f;

    LOGD("Creating RT: format %s / %d x %d / sz factor %f x %f / AA depth=%d coverage=%d\n", 
        ResourceFormatAsString(m_creationData.fmt),
        m_creationData.sz[0], m_creationData.sz[1],
        m_creationData.szFact[0], m_creationData.szFact[1],
        m_creationData.msaa[0], m_creationData.msaa[1]);

    // Shall we store the real size of the resource in the class ?
    int width = (int)((float)m_creationData.sz[0] * m_creationData.szFact[0]);
    int height = (int)((float)m_creationData.sz[1] * m_creationData.szFact[1]);

    //#undef BFMCASE
    //#define BFMTCASE(a, i, e) case a: texture->format = e;  texture->StupidOGLExternalFmt=GL_LUMINANCE; break;
    GLenum format, StupidOGLExternalFmt, FreakinType;
    switch(m_creationData.fmt)
    {
    default:
#ifdef OGLES2
    case FMT_ALPHA:             format = GL_ALPHA;              StupidOGLExternalFmt=GL_ALPHA;      FreakinType = GL_FLOAT; break;
    case FMT_LUMINANCE:         format = GL_LUMINANCE;          StupidOGLExternalFmt=GL_LUMINANCE;  FreakinType = GL_FLOAT; break;
    case FMT_LUMINANCE_ALPHA:   format = GL_LUMINANCE_ALPHA;    StupidOGLExternalFmt=GL_LUMINANCE_ALPHA; FreakinType = GL_FLOAT; break;
    case FMT_RGB:               format = GL_RGB;                StupidOGLExternalFmt=GL_RGB;        FreakinType = GL_FLOAT; break;
    case FMT_RGBA:              format = GL_RGBA;               StupidOGLExternalFmt=GL_RGBA;       FreakinType = GL_FLOAT; break;
#else
    //case INTENSITY:
    case FMT_INTENSITY8:        format = GL_INTENSITY8;         StupidOGLExternalFmt=GL_BGRA; FreakinType = GL_FLOAT; break;
    case FMT_INTENSITY16F:      format = GL_INTENSITY16F_ARB;   StupidOGLExternalFmt=GL_BGRA; FreakinType = GL_FLOAT; break;
    case FMT_INTENSITY32F:      format = GL_INTENSITY32F_ARB;   StupidOGLExternalFmt=GL_BGRA; FreakinType = GL_FLOAT; break;

    //case FMT_LUMINANCE_ALPHA:
    //case FMT_LA:
    //case FMT_LA8:
    case FMT_LUMINANCE_ALPHA8:  format = GL_LUMINANCE8_ALPHA8;  StupidOGLExternalFmt=GL_RGB; FreakinType = GL_FLOAT; break;
    case FMT_RG16F_NV:          format = GL_FLOAT_RG16_NV;      StupidOGLExternalFmt=GL_BGRA; FreakinType = GL_FLOAT; break;
    //case FMT_LA16F:
    case FMT_LUMINANCE_ALPHA16F:format = GL_LUMINANCE_ALPHA16F_ARB; StupidOGLExternalFmt=GL_LUMINANCE; FreakinType = GL_FLOAT; break;
    //case FMT_LA32F:
    case FMT_LUMINANCE_ALPHA32F:format = GL_LUMINANCE_ALPHA32F_ARB; StupidOGLExternalFmt=GL_RGB; FreakinType = GL_FLOAT; break;
    case FMT_RG32F_NV:          format = GL_FLOAT_RG32_NV;      StupidOGLExternalFmt=GL_RGB; FreakinType = GL_FLOAT; break;

    //case FMT_RGB:
    case FMT_RGB8:              format = GL_RGB8;               StupidOGLExternalFmt=GL_RGB; FreakinType = GL_FLOAT; break;
    case FMT_RGB16F:            format = GL_RGB16F_ARB;         StupidOGLExternalFmt=GL_RGB; FreakinType = GL_FLOAT; break;
    case FMT_RGB32F:            format = GL_RGB32F_ARB;         StupidOGLExternalFmt=GL_RGB; FreakinType = GL_FLOAT; break;

    //case FMT_RGBA:
    case FMT_RGBA8:             format = GL_RGBA8;              StupidOGLExternalFmt=GL_RGBA; FreakinType = GL_FLOAT; break;
    case FMT_RGBA8UI:           format = GL_RGBA8UI;            StupidOGLExternalFmt=GL_RGBA_INTEGER; FreakinType = GL_FLOAT/*GL_UNSIGNED_BYTE*/; break;
    case FMT_RGBA16F:           format = GL_RGBA16F_ARB;        StupidOGLExternalFmt=GL_RGBA; FreakinType = GL_FLOAT; break;
    case FMT_RGBA32F:           format = GL_RGBA32F_ARB;        StupidOGLExternalFmt=GL_RGBA; FreakinType = GL_FLOAT; break;

    case FMT_DEPTH24STENCIL8:   format = GL_DEPTH24_STENCIL8; StupidOGLExternalFmt=GL_DEPTH_COMPONENT; FreakinType = GL_FLOAT; break;

    case FMT_DEPTH_COMPONENT32F://GL_DEPTH_COMPONENT32F_ARB is 0x8CAC
        format = 0x8CAC;//or we could use GL_DEPTH_COMPONENT32F_NV; 
        StupidOGLExternalFmt=GL_LUMINANCE; FreakinType = GL_FLOAT; 
        break;

    case FMT_DEPTH32F_STENCIL8://GL_DEPTH32F_STENCIL8_ARB is 0x8CAD
        format = 0x8CAD;
        StupidOGLExternalFmt=GL_LUMINANCE; FreakinType = GL_FLOAT; 
        break;
    case FMT_FLOATRGBA16NV:                 format = GL_FLOAT_RGBA16_NV;        StupidOGLExternalFmt=GL_RGBA; FreakinType = GL_FLOAT; break;
    case FMT_RG:                            format = GL_RG;                     StupidOGLExternalFmt=GL_RG; FreakinType = GL_FLOAT; break;
    case FMT_RG_INTEGER:                    format = GL_RG_INTEGER;             StupidOGLExternalFmt=GL_RG_INTEGER; FreakinType = GL_INT; break;
    case FMT_R8:                            format = GL_R8;                     StupidOGLExternalFmt=GL_RGBA; FreakinType = GL_FLOAT; break;
    case FMT_R16:                           format = GL_R16;                    StupidOGLExternalFmt=GL_RGBA; FreakinType = GL_FLOAT; break;
    case FMT_RG8:                           format = GL_RG8;                    StupidOGLExternalFmt=GL_RG; FreakinType = GL_FLOAT; break;
    case FMT_RG16:                          format = GL_RG16;                   StupidOGLExternalFmt=GL_RG; FreakinType = GL_FLOAT; break;
    case FMT_R16F:                          format = GL_R16F;                   StupidOGLExternalFmt=GL_RED; FreakinType = GL_FLOAT; break;
    case FMT_R32F:                             format = GL_R32F;                   StupidOGLExternalFmt=GL_RED; FreakinType = GL_FLOAT; break;
    case FMT_RG16F:                         format = GL_RG16F;                  StupidOGLExternalFmt=GL_RG; FreakinType = GL_FLOAT; break;
    case FMT_RG32F:                         format = GL_RG32F;                  StupidOGLExternalFmt=GL_RG; FreakinType = GL_FLOAT; break;
    case FMT_R8I:                           format = GL_R8I;                    StupidOGLExternalFmt=GL_RED_INTEGER; FreakinType = GL_INT; break;
    case FMT_R8UI:                          format = GL_R8UI;                   StupidOGLExternalFmt=GL_RED_INTEGER; FreakinType = GL_UNSIGNED_BYTE; break;
    case FMT_R16I:                          format = GL_R16I;                   StupidOGLExternalFmt=GL_RED_INTEGER; FreakinType = GL_INT; break;
    case FMT_R16UI:                         format = GL_R16UI;                  StupidOGLExternalFmt=GL_RED_INTEGER; FreakinType = GL_UNSIGNED_BYTE; break;
    case FMT_R32I:                          format = GL_R32I;                   StupidOGLExternalFmt=GL_RED_INTEGER; FreakinType = GL_INT; break;
    case FMT_R32UI:                         format = GL_R32UI;                  StupidOGLExternalFmt=GL_RED_INTEGER; FreakinType = GL_UNSIGNED_BYTE; break;
    case FMT_RG8I:                          format = GL_RG8I;                   StupidOGLExternalFmt=GL_RG_INTEGER; FreakinType = GL_INT; break;
    case FMT_RG8UI:                         format = GL_RG8UI;                  StupidOGLExternalFmt=GL_RG_INTEGER; FreakinType = GL_UNSIGNED_BYTE; break;
    case FMT_RG16I:                         format = GL_RG16I;                  StupidOGLExternalFmt=GL_RG_INTEGER; FreakinType = GL_INT; break;
    case FMT_RG16UI:                        format = GL_RG16UI;                 StupidOGLExternalFmt=GL_RG_INTEGER; FreakinType = GL_UNSIGNED_BYTE; break;
    case FMT_RG32I:                         format = GL_RG32I;                  StupidOGLExternalFmt=GL_RG_INTEGER; FreakinType = GL_INT; break;
    case FMT_RG32UI:                        format = GL_RG32UI;                 StupidOGLExternalFmt=GL_RG_INTEGER; FreakinType = GL_UNSIGNED_INT; break;
#endif
    }
    GLenum target;
    switch(m_type)
    {
#ifndef OGLES2
    case RESTEX_1D:
        if(!m_OGLId)
            glGenTextures(1, &m_OGLId);
        target = GL_TEXTURE_1D;
        glBindTexture( target, m_OGLId);
#pragma MESSAGE("TODO TODO TODO TODO TODO TODO : multisample texture for 1D and others...")
#pragma MESSAGE("TODO TODO TODO TODO TODO TODO : multisample texture :check HW capabilities, first...")
        // Oh my god !!!!! WTF, OpenGL !?!!??!!
        glTexParameteri(target, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(target, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(target, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(target, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

        glTexImage1D(target, 0, format, width, 0, StupidOGLExternalFmt, FreakinType, NULL);
        glBindTexture( target, 0);
        break;
    case RESTEX_2DRECT:
        if(m_creationData.msaa[0] > 1)
        {
            LOGE("Error>> Can't create a Rectangle texture in MSAA (%s)\n", m_name.c_str());
            m_OGLId = 0;
            return false;
        }
#endif
    case RESTEX_2D:
        if(!m_OGLId)
            glGenTextures(1, &m_OGLId);
#ifndef OGLES2
        if(m_creationData.msaa[0] > 1)
        {
            target = GL_TEXTURE_2D_MULTISAMPLE;
            glBindTexture( target, m_OGLId);
            // Oh my god !!!!! WTF, OpenGL !?!!??!!
            glTexParameteri(target, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
            glTexParameteri(target, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
            glTexParameteri(target, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
            glTexParameteri(target, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

            glTexImage2DMultisample(target, m_creationData.msaa[0], format, width, height, GL_TRUE/*fixedsamplelocations*/);
            glBindTexture( target, 0);
        } else
#endif
        {
#ifndef OGLES2
            target = m_type == RESTEX_2D ? GL_TEXTURE_2D : GL_TEXTURE_RECTANGLE;
#else
            target = GL_TEXTURE_2D;
#endif
            glBindTexture( target, m_OGLId);
            // Oh my god !!!!! WTF, OpenGL !?!!??!!
            glTexParameteri(target, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
            glTexParameteri(target, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
            glTexParameteri(target, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
            glTexParameteri(target, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

#ifndef OGLES2
            glTexImage2D( target, 0, format, width, height, 0, StupidOGLExternalFmt, FreakinType, NULL);
#else
            glTexImage2D( target, 0, format, width, height, 0, StupidOGLExternalFmt, GL_FLOAT, NULL);
#endif
            // Check in any case... 
            if(checkGLError("glTexImage2D"))
            {
                LOGI("NOTE: glTexImage2D( target, 0, format, width, height, 0, StupidOGLExternalFmt, FreakinType, NULL); is very picky for nothing (NULL ptr passed !!!): the format/type must match internal format. I could have messed-up in the tables...");
            }
            glBindTexture( target, 0);
        }
        break;
    /*case RESTEX_3D:
        if(!m_OGLId) glGenTextures(1, &m_OGLId);
        target = GL_TEXTURE_3D;
        if(!m_OGLId) glBindTexture( target, m_OGLId);
        glTexImage3D(target, 0, format, width, height, depth, 0, StupidOGLExternalFmt, FreakinType, NULL);
        break;
      case RESTEX_CUBE:
        if(!m_OGLId) glGenTextures(1, &m_OGLId);
      */
    //
    // Render buffer
    //
    case RESRBUF_2D:
        m_OGLId = 0;
        // we have a relation with a texture supposed to be target for a 'resolve'
        // if we aren't in MSAA, let's use it instead and let's avoid allocating the renderbuffer
        // this will save use some useless intermediate step and memory
        if((m_creationData.resolveTarget))
        {
            if((m_creationData.resolveTarget->getWidth() != width)
             ||(m_creationData.resolveTarget->getHeight() != height))
            {
                LOGD("Warning>> we can't use ResolveTarget = %s for %s : sizes don't match\n", m_creationData.resolveTarget->getName(), m_name.c_str());
                m_creationData.resolveTarget = NULL;
            }
            //to remove else m_creationData.resolveTarget->addRef();
        }

        if((m_creationData.resolveTarget)&&(m_creationData.msaa[0] < 2))
        {
            LOGD("using RT %s instead of %s (no alloc on rb id %d)\n", m_creationData.resolveTarget->getName(), m_name.c_str(), m_OGLId);
            m_OGLId = m_creationData.resolveTarget->getGLTextureID();
            m_type = RESTEX_2D;
        }
        else
        {
            if(!m_OGLId)
                glGenRenderbuffers(1, &m_OGLId);
            glBindRenderbuffer(GL_RENDERBUFFER, m_OGLId);
            bool res = createRenderBuffer(width, height, format, m_creationData.msaa[0], m_creationData.msaa[1]);
            if(!res)
            {    
                glDeleteRenderbuffers(1, &m_OGLId);
                m_OGLId = 0;
                return false;
            }
            LOGD("Create Renderbuffer %s (id %d)\n", m_name.c_str(), m_OGLId);
        }
        break;
    default:
        CHECK_TRUE(false);
    }
    if(CHECKGLERRORS("createRenderTexture"))
    {
        LOGE("Error>> Failed to create %s\n", m_name.c_str());
        return false;
    } else {
        //glTexParameterf( target, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        //glTexParameterf( target, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        //glTexParameterf( target, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        //glTexParameterf( target, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        //// additional stuff:
        //glTexParameterf( target, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
        //glTexParameterf( target, GL_DEPTH_TEXTURE_MODE, GL_LUMINANCE);
        LOGD("Create Texture RT %s (id %d)\n", m_name.c_str(), m_OGLId);
    }
    return true;
}

/*************************************************************************/ /**
 ** \brief
 **
 **/ /*************************************************************************/ 
bool ResourceGL::invalidate()
{
    if(m_OGLId > 0)
    {
        LOGD("Invalidating Resource %s (ID=%d)\n", m_name.c_str(), m_OGLId);
        switch(m_type)
        {
        case RESRBUF_2D:
            if(!m_creationData.bExternal)
            {
                LOGD("glDeleteRenderbuffers on Renderbuffer %s (id %d) \n", m_name.c_str(), m_OGLId);
                glDeleteRenderbuffers(1, &(m_OGLId));
            }
            m_OGLId = 0;
            break;
        case RESTEX_1D:
        case RESTEX_2D:
        case RESTEX_2DRECT:
            //case RESTEX_3D:
            //case RESTEX_CUBE:
            if(!m_creationData.bExternal)
            {
                LOGD("glDeleteTextures Texture %s (id %d) m_refCnt == %d... Deleting it\n", m_name.c_str(), m_OGLId);
                glDeleteTextures(1, &(m_OGLId));
            }
            m_OGLId = 0;
        break;
        }
    }
    return true;
}

/*************************************************************************/ /**
 ** \brief
 **
 **/ /*************************************************************************/ 
bool        ResourceGL::validate()
{
    NXPROFILEFUNCCOL2(__FUNCTION__, COLOR_CYAN, 12);
    if(validationNeeded())
        return createRenderResource();
    return true;
}

/*************************************************************************/ /**
 ** \brief
 **
 **/ /*************************************************************************/ 
bool        ResourceGL::validationNeeded()
{
    NXPROFILEFUNCCOL2(__FUNCTION__, COLOR_CYAN, 12);
    if(m_creationData.bExternal) // external resources don't need any validation. We don't own them
        return false;
    if(m_OGLId)
    {
        const int* vp = m_pOwner->getViewport();
        const int* msaa = m_pOwner->getMsaa();
        if(m_creationData.appDepSz && 
            ((m_creationData.sz[0] != vp[2]) || 
            (m_creationData.sz[1] != vp[3])))
            return true;
        if(m_creationData.appDepMSAA &&
            ((m_creationData.msaa[0] != msaa[0]) ||
            (m_creationData.msaa[1] != msaa[1])))
            return true;
        return false;
    }
    return true;
}
/*************************************************************************
 **
 ** FrameBufferObject FrameBufferObject FrameBufferObject FrameBufferObject
 **
 *************************************************************************/ 
FrameBufferObject::~FrameBufferObject()
{
    // No reference counter to decrement... not used here
    m_dst = NULL;
    int i=0;
    for(; i<(int)m_colors.size(); i++)
    {
        m_colors[i] = NULL;
    }
    if(m_fboID)
        glDeleteFramebuffers(1, &m_fboID);
    m_fboID = 0;
}

bool checkFramebufferStatus()
{
    GLenum status;
    status = (GLenum) glCheckFramebufferStatus(GL_FRAMEBUFFER);
    switch(status) {
        case GL_FRAMEBUFFER_COMPLETE:
            return true;
        case GL_FRAMEBUFFER_UNSUPPORTED:
            LOGE("Error>> Unsupported framebuffer format\n");
            //assert(!"Unsupported framebuffer format");
            break;
        case GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT:
            LOGE("Error>> Framebuffer incomplete, missing attachment\n");
            //assert(!"Framebuffer incomplete, missing attachment");
            break;
        case GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT:
            LOGE("Error>> Framebuffer incomplete attachment\n");
            //assert(!"Framebuffer incomplete attachment");
            break;
#ifndef OGLES2
        case GL_FRAMEBUFFER_INCOMPLETE_FORMATS_EXT:
            LOGE("Error>> Framebuffer incomplete, attached images must have same format\n");
            //assert(!"Framebuffer incomplete, attached images must have same format");
            break;
        case GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER:
            LOGE("Error>> Framebuffer incomplete, missing draw buffer\n");
            //assert(!"Framebuffer incomplete, missing draw buffer");
            break;
        case GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER:
            LOGE("Error>> Framebuffer incomplete, missing read buffer\n");
            //assert(!"Framebuffer incomplete, missing read buffer");
            break;
        case GL_FRAMEBUFFER_INCOMPLETE_DIMENSIONS_EXT:
            LOGE("Error>> Framebuffer incomplete, attached images must have same dimensions\n");
            //assert(!"Framebuffer incomplete, attached images must have same dimensions");
            break;
#else
        case GL_FRAMEBUFFER_INCOMPLETE_DIMENSIONS:
            LOGE("Error>> Framebuffer incomplete, attached images must have same dimensions\n");
            //assert(!"Framebuffer incomplete, attached images must have same dimensions");
            break;
#endif
        default:
            LOGE("Error>> unknown error %x (But can happen if you attached incompatible resources... of different size, for example)\n", status);
            //assert(!"unknown FBO Error");
            break;
    }
    return false;
}

#ifndef OGLES2
GLenum drawBuffers[] = 
{
    GL_COLOR_ATTACHMENT0,
    GL_COLOR_ATTACHMENT1,
    GL_COLOR_ATTACHMENT2,
    GL_COLOR_ATTACHMENT3,
    GL_COLOR_ATTACHMENT4,
    GL_COLOR_ATTACHMENT5,
    GL_COLOR_ATTACHMENT6
};
#endif
/*************************************************************************/ /**
 ** \brief
 **
 **/ /*************************************************************************/ 
bool FrameBufferObject::validate()
{
    NXPROFILEFUNCCOL2(__FUNCTION__, COLOR_CYAN, 12);
    bool validated = m_fboID ? true : false;
    // we should be able to only get #0... whatever
    for(int i=0; i<(int)m_colors.size(); i++)
    {
        if(m_colors[i] <= 0)
            break;
        if((!m_colors[i]->isValidated())
         ||(m_colors[i]->getWidth() != m_w)
         ||(m_colors[i]->getHeight() != m_h) )
        {
            validated = false;
            break;
        }
        // TODO: FBO to re-validate if MSAA of resources changed
        //if((m_colors[i]->m_creationData.msaa() != m_w)...
        //{
        //    validated = false;
        //    break;
        //}
    }
    if(validated && m_dst &&((!m_dst->isValidated())
     ||(m_dst->getWidth() != m_w)
     ||(m_dst->getHeight() != m_h)))
    {
        validated = false;
    }
    if(validated)
        return true;
    bool fbo_failed = false;
    IResourceRepository* pRep = nvFX::getResourceRepositorySingleton();

    //
    // check validity of resources: the FBO can't take the liberty to create them
    // so we can save memory when not needed
    //
    for(int i=0; i<(int)m_colors.size(); i++)
        if(!m_colors[i]->isValidated() )
            fbo_failed = true;
    if(m_dst && (!m_dst->isValidated()) )
            fbo_failed = true;
    if(fbo_failed)
    {
        if(m_fboID) {
            LOGD("Invalidating FBO %s (ID=%d)\n", m_name.c_str(), m_fboID);
            glDeleteFramebuffers(1, &(m_fboID));
        }
        m_fboID = 0;
        return false;
    }

    if(!m_fboID)
        glGenFramebuffers(1, &m_fboID);
    LOGD("Creation of FBO (OGL id %d) : %s\n", m_fboID, m_name.c_str());
    glBindFramebuffer(GL_FRAMEBUFFER, m_fboID); 
    //
    // Walk through the referenced textures/renderbuffers
    //
    for(int i=0; i<(int)m_colors.size(); i++)
    {
        if(m_colors[i] <= 0)
        {
            //glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i, GL_TEXTURE_2D, 0, 0);
            //continue;
            break;
        }
        if(m_colors[i])
        {
            // only work with already validated resources
            m_colors[i]->validate();
            switch(m_colors[i]->m_type)
            {
            case RESTEX_1D:
            case RESTEX_2D:
            case RESTEX_2DRECT:
            //case RESTEX_3D:
            //case RESTEX_CUBE:
            {    
                Resource *pResource;
                if(m_colors[i]->m_creationData.resolveTarget) // case where we shortcut the rb to the texutre directly
                {
                    pResource = m_colors[i]->m_creationData.resolveTarget;
                    LOGD("RE-Attaching %s to FBO %s at Color %d (via %s)\n", m_colors[i]->m_creationData.resolveTarget->getName(), m_name.c_str(), i, m_colors[i]->getName());
                } else {
                    pResource = m_colors[i];
                }
                // There is a bug preventing the creation of the FBO if filtering is not NEAREST
                GLenum target = getGLTarget(pResource->m_type);
                glBindTexture( target, pResource->m_OGLId);
                glTexParameteri(target, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
                glTexParameteri(target, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
                glBindTexture( target, 0);
                glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i, target, pResource->m_OGLId, 0);
            }
            break;
            case RESRBUF_2D:
            {
                LOGD("RE-Attaching %s to FBO %s at Color %d\n", m_colors[i]->getName(), m_name.c_str(), i);
                glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i, GL_RENDERBUFFER, m_colors[i]->m_OGLId);
            }
            break;
            default:
                assert(0);
            }
            //
            // take the size of the buffer for FBO : blit operation would require it
            //
            m_w = m_colors[i]->getWidth();
            m_h = m_colors[i]->getHeight();
        }
        else 
        {
            LOGE("Error>> Could not find Texture/renderbuffer %d for FBO");
            glDeleteFramebuffers(1, &m_fboID);
            m_fboID = 0;
            return false;
        }
    }
    //
    // Depth buffer
    //
    if(m_dst)
    {
#ifndef OGLES2
        m_dst->validate();
        LOGD(" Attaching %s to FBO DST %s\n", m_dst->m_name.c_str(), m_name.c_str());
        //to remove... m_dst->addRef();
        switch(m_dst->m_type) // we are dealing with a texture
        {
        case RESTEX_1D:
        case RESTEX_2D:
        case RESTEX_2DRECT:
        //case RESTEX_3D:
        //case RESTEX_CUBE:
        {
            Resource *pResource;
            if(m_dst->m_creationData.resolveTarget)
            {
                pResource = m_dst->m_creationData.resolveTarget;
                LOGD("RE-Attaching %s to FBO %s at dst (via %s)\n", pResource->getName(), m_name.c_str(), m_dst->getName());
            } else {
                pResource = m_dst;
            }
            // There is a bug preventing the creation of the FBO if filtering is not NEAREST
            GLenum target = getGLTarget(pResource->m_type);
            // There is a bug preventing the creation of the FBO if filtering is not NEAREST
            glBindTexture( target, pResource->m_OGLId);
            glTexParameteri(target, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
            glTexParameteri(target, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
            glBindTexture( target, 0);
            glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, target, pResource->m_OGLId, 0);
            switch(m_dst->m_creationData.fmt)
            {
            case FMT_DEPTH24STENCIL8:
            case FMT_DEPTH32F_STENCIL8: //0x8CAD...GL_DEPTH32F_STENCIL8_ARB
                glFramebufferTexture2D(GL_FRAMEBUFFER, GL_STENCIL_ATTACHMENT, target, pResource->m_OGLId, 0);
            }
        }
        break;
        case RESRBUF_2D:
        {
            glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_STENCIL_ATTACHMENT, GL_RENDERBUFFER, 0);
            glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, m_dst->m_OGLId);
            switch(m_dst->m_creationData.fmt)
            {
            case FMT_DEPTH24STENCIL8:
            case FMT_DEPTH32F_STENCIL8: //0x8CAD...GL_DEPTH32F_STENCIL8_ARB
                glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_STENCIL_ATTACHMENT, GL_RENDERBUFFER, m_dst->m_OGLId);
            }
        }
        break;
        default:
            assert(0);
            break;
        }
        //if(fbo_failed)
        //{
        //    LOGE("Error>> Could not find Depth Texture/renderbuffer %d for FBO");
        //    glDeleteFramebuffers(1, &m_fboID);
        //    m_fboID = 0;
        //    return NULL;
        //}
        //
        // take the size of the buffer for FBO : blit operation would require it
        // Note: was already done in colors[]. But what if we have a depth-only FBO ?
        //
        m_w = m_dst->getWidth();
        m_h = m_dst->getHeight();
#else
        LOGE("OGL ES2 can't have depth buffer attachment !\n");
#endif
    } 
    if(!checkFramebufferStatus())
    {
        // NOTE: let's keep the FBO and Render target references for now :
        // in some cases, resizing the window will simply make this FBO work !
        LOGE("Error>> Error in FBO %s\n", m_name.c_str());
        for(int i=0; i<(int)m_colors.size(); i++)
        {
            LOGE("Error>>  FBO RT %s (%s %dx%d AA(%d,%d))\n", 
                m_colors[i]->m_name.c_str(), 
                ResourceFormatAsString(m_colors[i]->m_creationData.fmt),
                m_colors[i]->getWidth(), m_colors[i]->getHeight(),
                m_colors[i]->m_creationData.msaa[0], m_colors[i]->m_creationData.msaa[1]);
            //m_colors[i]->Release();
        }
        if(m_dst)
        {
            LOGE("Error>>  FBO RT %s (%s %dx%d AA(%d,%d))\n", 
                m_dst->m_name.c_str(), 
                ResourceFormatAsString(m_dst->m_creationData.fmt),
                m_dst->getWidth(), m_dst->getHeight(),
                m_dst->m_creationData.msaa[0], m_dst->m_creationData.msaa[1]);
            //m_dst->Release();
        }
        //glDeleteFramebuffers(1, &m_fboID);
        //m_fboID = 0;
        //delete fbo;
        //return NULL;
    }
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    return true;
}
/*************************************************************************/ /**
 ** \brief
 **
 **/ /*************************************************************************/ 
bool FrameBufferObject::setCurrent(bool bAdjustViewport)
{
    NXPROFILEFUNCCOL2(__FUNCTION__, COLOR_CYAN, 12);
    assert(m_fboID > 0);
    /* Doesn't solve my issue...
    //
    // WTF, OpenGL !?!!??!! There is a problem (BUG?!?)
    //
    for(int i=0; i<m_colors.size(); i++)
    {
        Resource* pRes = m_colors[i];
        switch(pRes->getType())
        {
        case RESTEX_1D:
            glBindTexture( GL_TEXTURE_1D, pRes->getGLTextureID()); // Use DSA asap... yacky
            glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
            glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
            glBindTexture( GL_TEXTURE_1D, 0);
            break;
        case RESTEX_2D:
            glBindTexture( GL_TEXTURE_2D, pRes->getGLTextureID()); // Use DSA asap... yacky
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
            glBindTexture( GL_TEXTURE_2D, 0);
            break;
        case RESTEX_2DRECT:
            glBindTexture( GL_TEXTURE_RECTANGLE, pRes->getGLTextureID()); // Use DSA asap... yacky
            glTexParameteri(GL_TEXTURE_RECTANGLE, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
            glTexParameteri(GL_TEXTURE_RECTANGLE, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
            glTexParameteri(GL_TEXTURE_RECTANGLE, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_RECTANGLE, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
            glBindTexture( GL_TEXTURE_RECTANGLE, 0);
            break;
        //case RESTEX_3D:
        //    glBindTexture( GL_TEXTURE_3D, pRes->getGLTextureID()); // Use DSA asap... yacky
        //    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        //    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        //    glBindTexture( GL_TEXTURE_3D, 0);
        //    break;
        //case RESTEX_CUBE:
        //    glBindTexture( GL_TEXTURE_CUBE_MAP, pRes->getGLTextureID()); // Use DSA asap... yacky
        //    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        //    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        //    glBindTexture( GL_TEXTURE_CUBE_MAP, 0);
        //    break;
        }
    }*/

    glBindFramebuffer(GL_FRAMEBUFFER, m_fboID);
#ifndef OGLES2
    glDrawBuffers((int)m_colors.size() , drawBuffers);
#endif
    if(bAdjustViewport)
        glViewport(0,0,m_w,m_h);
    return true;//PRINT_GL_ERROR("setCurrent") ? false : true;
}
/*************************************************************************/ /**
 ** \brief
 **
 **/ /*************************************************************************/ 
bool FrameBufferObject::blitTo(IFrameBufferObject* pIDst)
{
    NXPROFILEFUNCCOL2(__FUNCTION__, COLOR_CYAN, 12);
    return m_pOwner->blit(pIDst, this);
}
bool      FrameBufferObjectsRepository::blit(IFrameBufferObject* pIDst, IFrameBufferObject* pISrc)
{
#ifndef OGLES2
    NXPROFILEFUNCCOL2(__FUNCTION__, COLOR_CYAN, 12);
    FrameBufferObject* pDst = pIDst ? static_cast<FrameBufferObject*>(pIDst) : NULL;
    FrameBufferObject* pSrc = pISrc ? static_cast<FrameBufferObject*>(pISrc) : NULL;
    if(pDst != pSrc)
    {
        // For now, 
        // let's avoid any case involving the backbuffer
        // let's just test on color[0] : if the target, as textures, are the same
        // then it means we are in the case of resolveTextureTarget hack. So no need to blit anything
        if((pDst != 0)&&(pSrc != 0))
        {
            assert(pDst->m_colors[0]);
            assert(pSrc->m_colors[0]);
            if((pDst->m_colors[0]->m_OGLId == pSrc->m_colors[0]->m_OGLId)
             &&(pDst->m_colors[0]->m_type == pSrc->m_colors[0]->m_type) )
            {
                LOGD("Debug message: No Blit FBO required...\n");
                return true;
            }
        }
        if(pSrc == 0)
            glBindFramebuffer( GL_READ_FRAMEBUFFER, (GLuint)m_backbuffer); // bind whatever FBO is needed
        else
            glBindFramebuffer( GL_READ_FRAMEBUFFER, pSrc->m_fboID);
        if(pDst == 0)
            glBindFramebuffer( GL_DRAW_FRAMEBUFFER, (GLuint)m_backbuffer);
        else
            glBindFramebuffer( GL_DRAW_FRAMEBUFFER, pDst->m_fboID);
        // we MUST have one of them != NULL
        int wpSrc = pSrc ? pSrc->m_w : m_vp[2];// pDst->m_w;
        int hpSrc = pSrc ? pSrc->m_h : m_vp[3];// pDst->m_h;
        int wpDst = pDst ? pDst->m_w : m_vp[2];// pSrc->m_w;
        int hpDst = pDst ? pDst->m_h : m_vp[3];// pSrc->m_h;
// Now... seems to work. Need to check deeper, though
//#pragma MESSAGE("**NOTE** TRISTAN - NVFBORepository::BlitFBOFromExternalID : I have a BUG when blitting to a different size than the source...")
        // Note: we could have taken pDst/pSrc->colors[]->w or pDst/pSrc->pDst->w... but was easier to copy values here
        glBlitFramebuffer( 0, 0, wpSrc, hpSrc, 0, 0, wpDst, hpDst, GL_COLOR_BUFFER_BIT/*|GL_DEPTH_BUFFER_BIT|GL_STENCIL_BUFFER_BIT*/, GL_LINEAR );
        //glBindFramebuffer( GL_DRAW_FRAMEBUFFER, 0);
        //glBindFramebuffer( GL_READ_FRAMEBUFFER, 0);
        if(PRINT_GL_ERROR("glBlitFramebuffer"))
        {
            LOGE("Error>> glBlitFramebuffer failed (%s -> %s)\n", 
                pSrc ? pSrc->m_name.c_str() : "backbuffer",
                pDst ? pDst->m_name.c_str() : "backbuffer");
            return false;
        }
    }
    else
    {
        LOGE("Error>> glBlitFramebuffer cannot blit to itself\n");
    }
    return true;
#else
    LOGE("Error>> cannot blit in OGL ES2\n");
    return false;
#endif
}
/*************************************************************************/ /**
 ** \brief
 **
 **/ /*************************************************************************/ 
bool FrameBufferObject::invalidate()
{
    LOGD("Invalidating FBO %s (ID=%d)\n", m_name.c_str(), m_fboID);
    glDeleteFramebuffers(1, &(m_fboID));
    m_fboID = 0;
    return true;
}

/*************************************************************************
 **
 ** FrameBufferObject FrameBufferObject FrameBufferObject FrameBufferObject
 **
 *************************************************************************/ 
bool FrameBufferObjectsRepository::setCurrent(IFrameBufferObject* pFBO, bool bAdjustViewport)
{
    NXPROFILEFUNCCOL2(__FUNCTION__, COLOR_CYAN, 12);
    if(pFBO == NULL)
    {
        glBindFramebuffer(GL_FRAMEBUFFER, (int)m_backbuffer);
        CHECKGLERRORS("");
#ifndef OGLES2
        if(m_backbuffer == 0)
            glDrawBuffer(GL_BACK);
        else
            glDrawBuffer(GL_COLOR_ATTACHMENT0);
#endif
        if(bAdjustViewport)
            glViewport(m_vp[0], m_vp[1], m_vp[2], m_vp[3]);
        CHECKGLERRORS("");
        return true;
    }
    bool bRes = static_cast<FrameBufferObject*>(pFBO)->setCurrent(bAdjustViewport);
    CHECKGLERRORS("");
    return bRes;
    }


} //namespace nvFX

