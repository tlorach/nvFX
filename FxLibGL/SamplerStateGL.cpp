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
#include <fstream>
#include <assert.h>
#include "Fx.h"
#include "FxGL.h"
#pragma warning(disable:4996)
using namespace nvFX;

/*******************************************************************************/
/*  SamplerStateGL SamplerStateGL SamplerStateGL SamplerStateGL SamplerStateGL */
/*******************************************************************************/

/*************************************************************************/ /**
 ** 
 ** 
 **/ /*************************************************************************/ 
SamplerStateGL::~SamplerStateGL()
{
}

/*************************************************************************/ /**
 ** 
 ** 
 **/ /*************************************************************************/ 
SamplerStateGL::SamplerStateGL(Container *pCont, const char *name) : SamplerState(pCont, name)
{
}

// Was to test a wrapper...
//class GLTexture {
//private:
//    void setAttr(GLenum name, GLenum v) {
//        #ifdef NODSA
//        // non-DSA function calls...
//        #else
//        glTextureParameteriEXT( texture, target, name, v);
//        #endif
//        #ifdef DEBUG
//        // debug checking...
//        #endif
//    }
//public:
//    struct Target { // encapsulate in a struct to help auto-completion
//        enum E {
//            TEX2D = GL_TEXTURE_2D,
//            TEX3D = GL_TEXTURE_3D,
//        };
//    };
//    struct FilterMode {
//        enum E {
//            NEAREST = GL_NEAREST,
//            LINEAR = GL_LINEAR,
//            NEAREST_MIPMAP_NEAREST = GL_NEAREST_MIPMAP_NEAREST,
//            LINEAR_MIPMAP_NEAREST = GL_LINEAR_MIPMAP_NEAREST,
//            NEAREST_MIPMAP_LINEAR = GL_NEAREST_MIPMAP_LINEAR,
//            LINEAR_MIPMAP_LINEAR = GL_LINEAR_MIPMAP_LINEAR,
//        };
//    };
//    struct Mode {
//        enum E {
//            CLAMP = GL_CLAMP,
//            REPEAT = GL_REPEAT,
//            //...
//        };
//    };
//    GLuint texture;
//    Target::E target;
//
//    GLTexture(GLuint texture_, Target::E target_) : texture(texture_), target(target_) {}
//    GLTexture(GLuint texture_, GLenum target_) : texture(texture_) {
//        /*Check if target is valid enum*/ target = ((Target::E)target_); }
//
//    void setMagFilter(FilterMode::E mode) { setAttr(GL_TEXTURE_MAG_FILTER, mode); }
//    void setMinFilter(FilterMode::E mode) { setAttr(GL_TEXTURE_MIN_FILTER, mode); }
//    void setWrapS(Mode::E wrapMode) { setAttr(GL_TEXTURE_WRAP_S, wrapMode); }
//    void setWrapT(Mode::E wrapMode) { setAttr(GL_TEXTURE_WRAP_S, wrapMode); }
//    void setWrapR(Mode::E wrapMode) { setAttr(GL_TEXTURE_WRAP_S, wrapMode); }
//};
/*************************************************************************/ /**
 ** 
 ** see http://www.opengl.org/sdk/docs/man/xhtml/glTexParameter.xml
 ** \todo arguments need unit # in order to work with glBindSampler.
 ** or: create a new method for the case of >= OGL 3.3
 **/ /*************************************************************************/ 
void SamplerStateGL::updateGLTexture(GLenum target, GLint tex, bool bindTexture)
{
    //// Using the enum for target:
    //GLTexture texwrap(tex, GLTexture::Target::TEX2D);
    //// using a direct enum for target:
    //GLTexture texwrap2(tex, GL_TEXTURE_2D);

    //texwrap.setMagFilter(GLTexture::FilterMode::LINEAR_MIPMAP_NEAREST);
    //// or:
    //GLTexture(4, GLTexture::Target::TEX2D).setWrapS(GLTexture::Mode::REPEAT);
    //GLTexture(4, GLTexture::Target::TEX2D).setWrapT(GLTexture::Mode::REPEAT);

    if(bindTexture)
        glBindTexture(target, tex);
    StateVec::iterator it = m_states.begin();
    while(it != m_states.end())
    {
        //see http://www.opengl.org/sdk/docs/man/xhtml/glTexParameter.xml
        if(*it)
            glTexParameteri(target, (*it)->sstate, (*it)->value);
        ++it;
    }
    // if available (OGL 3.3 or higher) :
    // glBindSampler(unit, sampler);
}

SamplerState*  SamplerStateGL::update(void *data, Pass *pass, int layerID, bool bCreateIfNeeded)
{
    return this;
}

SamplerState*  SamplerStateGL::updateForTarget(void *data, int target)
{
    return this;
}
