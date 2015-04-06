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
#pragma warning(disable:4996)
using namespace nvFX;

#pragma MESSAGE("TODO TODO TODO TODO : Implement ALL the D3D and OpenGL SamplerState parameters")

/***************************************************************************/
/*  SamplerState SamplerState SamplerState SamplerState SamplerState       */
/***************************************************************************/

/*************************************************************************/ /**
 ** 
 ** 
 **/ /*************************************************************************/ 
SamplerState::~SamplerState()
{
    StateVec::iterator it = m_states.begin();
    while(it != m_states.end())
    {
        if(*it)
            delete *it;
        ++it;
    }
    m_states.clear();
    //glDeleteSamplers()
}

/*************************************************************************/ /**
 ** 
 ** 
 **/ /*************************************************************************/ 
SamplerState::SamplerState(Container *pCont, const char *name)
{
    m_data = NULL;
    m_container = pCont;
    if(name)
        m_name = name;
	m_activeTarget = -1;
    //glGenSamplers(...)
}

/*************************************************************************/ /**
 ** \brief create a sampler-state
 **
 ** \todo for OpenGL 3.3, use Sampler Objects !
 ** See  http://www.opengl.org/sdk/docs/man4/xhtml/glBindSampler.xml 
 **
 **/ /*************************************************************************/ 
SamplerState::State* SamplerState::createState(GLenum sstate, GLenum value)
{
    // TODO: glSamplerParameteriv(...)
    m_states.push_back(new State(sstate, value) );
    return m_states.back();
}

void SamplerState::updateGLTexture(GLenum target, GLint tex, bool bindTexture)
{
    CHECK_TRUE(false);
}

ISamplerState*    SamplerState::deleteTarget(IPass *pass, int layerID)
{
    std::vector<STarget>::iterator it = m_targets.begin();
    std::vector<STarget>::iterator it2;
    while(it != m_targets.end())
    {
        STarget &t = *it;
        if(!t.valid)
            continue;
        if((pass != NULL) &&(t.pass != pass))
            continue;
        if((layerID > 0)&&(t.passLayerId != layerID))
            continue;
        it2 = it;
        ++it;
        m_targets.erase(it2);
    }
    return this;
}
ISamplerState*    SamplerState::invalidateTarget(IPass *pass, int layerID)
{
	if((m_activeTarget >= 0)&&(m_targets[m_activeTarget].pass == pass)&&(m_targets[m_activeTarget].passLayerId == layerID))
	{
		STarget &t = m_targets[m_activeTarget];
        t.valid = false;
        t.dirty = true;
        t.index = -1;
		return this;
	}
    int tsz = (int)m_targets.size();
    for(int i=0; i<tsz; i++)
    {
        STarget &t = m_targets[i];
        if(!t.valid)
            continue;
        if((pass != NULL) &&(t.pass != pass))
            continue;
        if((layerID > 0)&&(t.passLayerId != layerID))
            continue;
        t.valid = false;
        t.dirty = true;
        t.index = -1;
    }
    return this;
}

int SamplerState::getNumTargets(TargetType ttype)
{
    int n = 0;
    int tsz = (int)m_targets.size();
    if(ttype == TANY)
        return tsz;
    for(int i=0; i<tsz; i++)
    {
        STarget &t = m_targets[i];
        if(!t.valid)
            continue;
        if(t.ttype == ttype)
            n++;
    }
    return n;
}
