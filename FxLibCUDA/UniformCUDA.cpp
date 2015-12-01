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
#include "FxCUDA.h"
#pragma warning(disable:4996)
using namespace nvFX;

/*************************************************************************
 **
 ** UniformCUDA UniformCUDA UniformCUDA UniformCUDA UniformCUDA UniformCUDA
 ** UniformCUDA UniformCUDA UniformCUDA UniformCUDA UniformCUDA UniformCUDA
 **
 *************************************************************************/ 
UniformCUDA::UniformCUDA(const char* name, const char* groupname, const char* semantic) :
    Uniform(name, groupname, semantic)
{
}
/*************************************************************************/ /**
 ** 
 ** 
 **/ /*************************************************************************/ 
UniformCUDA::~UniformCUDA()
{
    //Uniform::~Uniform(); we don't have to call this...
    if(m_data)
    {
#ifndef OGLES2
        if(m_type == TSubroutineUniform)
            for(int i=0; i<16; i++)
                if(m_data->subroutineNames[i]) delete [] m_data->subroutineNames[i];
#endif
    }
}

Uniform*  UniformCUDA::update(ShadowedData* data, Pass *pass, int layerID, bool bCreateIfNeeded)
{
    CUresult res;
    CUarray cuArray = NULL;
    switch(m_type)
    {
    case IUniform::TTexture1D:
    case IUniform::TTexture2D:
    case IUniform::TTexture2DRect:
    case IUniform::TTexture3D:
    case IUniform::TTextureCube:
        //if(data && data->tex.pRes)
            return updateTextures(data, pass, layerID, bCreateIfNeeded);
        break;
        // TODO whenever necessary
    }
    return this;
}
Uniform*  UniformCUDA::updateTextures(ShadowedData* data, Pass *pass, int layerID, bool bCreateIfNeeded)
{
    CUresult res;
    CUarray cuArray = NULL;
    if(data && data->tex.pRes)
    {
        // texture reference taken from the CUDA program :
        // the uniform texture will take care of this with :
        //CUtexref texRef; cuModuleGetTexRef(&texRef, getModule(), name);
        cuArray = static_cast<ResourceCUDA*>(data->tex.pRes)->mapResource();
    }
    if(cuArray)
    {
        for(int i=0; i<(int)m_targets.size(); i++)
        {
            STarget &t = m_targets[i];
            if(((pass == NULL) || ((t.pass == pass)&&(t.passLayerId == layerID)))&&(t.ttype == TCUDA))
            {
                res = cuTexRefSetArray( (CUtexref)t.cudaTexRef, cuArray, CU_TRSA_OVERRIDE_FORMAT );
                if(res)
                {
                    LOGE("Error>> CUDA failed to set texture reference to CUDA sampler\n");
                    return false;
                }
            }
        }
    }
    //
    // a pass ptr was specified and nothing found, we shall try to find the uniform in the pass program
    //
    if(pass && bCreateIfNeeded)
    {
        STarget t;
        int p = 0;
        t.uniformLocation = -1;
        t.dirty = false;
        t.valid = false;
        t.pass = pass;
        t.ttype = TCUDA;
        t.passLayerId = layerID;
        t.shaderProgramNumber = 0;
        // cast it to CUDAProgram : it can't be another class in this case
        IProgram* prg = pass->getProgram(layerID);
        // NOTE: TODO... make the test cleaner ! Hacky for now.
        // a non-separable program with 0 ID can only be a CUDA program... well...
        if(prg && (prg->getProgram()==0))
        {
            CUmodule module = (CUmodule)(static_cast<CUDAProgram*>(prg)->m_cuModule);
            CUtexref texRef;
            res = cuModuleGetTexRef(&texRef, module, m_name.c_str());
            t.cudaTexRef = texRef;
            if(res)
            {
                LOGI("Warning : texture %s referenced in the pass %s is not used in the Cuda module of this pass\n", pass->getName(), m_name.c_str());
            } else // Store the target if the shader needed it
            {
                // NOTE: maybe we should use a std::map<pass,target> for targets...
                int i = 0;
                for(i=0; i<(int)m_targets.size(); i++)
                    if((m_targets[i].pass == pass)
                    && (m_targets[i].passLayerId == layerID)
                    && (m_targets[i].ttype == t.ttype))
                    {
                        m_targets[i] = t;
                        break;
                    }
                if(i == m_targets.size())
                {
                    m_targets.push_back(t);
                    pass->addUniform(this, i);
                }
                // prepare the resource related to this texture
                // maybe we should put this call somewhere else
                if(data && data->tex.pRes)
                    static_cast<ResourceCUDA*>(data->tex.pRes)->setupAsCUDATexture();
                // update again now we found it
                return UniformCUDA::updateForTarget(data, i);
            }
        }
    }
    return this;
}

Uniform*  UniformCUDA::updateForTarget(ShadowedData* data, int target)
{
    CUresult res;
    CUarray cuArray = NULL;
	UniformCUDA::STarget &t = m_targets[target];
	m_activeTarget = target;
    if(data && data->tex.pRes)
    {
        cuArray = static_cast<ResourceCUDA*>(data->tex.pRes)->mapResource();
    }
    if(cuArray)
    {
        if(t.ttype == TCUDA)
        {
            res = cuTexRefSetArray( (CUtexref)t.cudaTexRef, cuArray, CU_TRSA_OVERRIDE_FORMAT );
            if(res)
            {
                LOGE("Error>> CUDA failed to set texture reference to CUDA sampler\n");
                return this;
            }
        }
    }
    return this;
}


