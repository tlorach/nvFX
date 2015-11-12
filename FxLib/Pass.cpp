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
//#include <assert.h> //pas le droits aux assers
#include "Fx.h"
#pragma warning(disable:4996)
using namespace nvFX;

/***************************************************************************/
/* PassState PassState PassState PassState PassState PassState PassState   */
/***************************************************************************/
IStateGroupRaster*  PassState::getStateGroupRaster()
{
    return (m_type == TStateGroup_Raster) ? m_pSGR: NULL;
};
IStateGroupCS*      PassState::getStateGroupCS() 
{
    return (m_type == TStateGroup_CS) ? m_pSGCS: NULL;
};
IStateGroupDST*     PassState::getStateGroupDST() 
{
    return (m_type == TStateGroup_DST) ? m_pSGDST: NULL;
};
#ifndef OGLES2
IStateGroupPath*    PassState::getStateGroupPathRendering() 
{
    return (m_type == TStateGroup_PR) ? m_pSGPR: NULL;
};
#endif
IFrameBufferObject* PassState::getFBOTarget() 
{
    return (m_type == TFBOTarget) ? m_FBOTarget: NULL;
};
IFrameBufferObject* PassState::getBlitFBOToActiveTarget() 
{
    return (m_type == TBlitFBOSrc) ? m_BlitFBOSrc: NULL;
};

/*************************************************************************/ /**
 ** 
 ** 
 **/ /*************************************************************************/ 
PassState::~PassState()
{
    //LOGD("create PassState %s\n", name ? name : "-");
    switch(m_type)
    {
    case TUniform:
        delete m_uniformSrc;
        m_uniformSrc = NULL;
        m_uniformDst = NULL;
        //delete m_varName;
        //delete m_subiexecuteroutineName; // floats and int share the same ptr
        break;
#ifdef USECUDA
    case TCUDAKernelEntry:
        if(m_argvalues)
            delete [] m_argvalues;
        m_argvalues = NULL;
        break;
#endif
    }
}
/*************************************************************************/ /**
 ** 
 ** 
 **/ /*************************************************************************/ 
PassState::PassState(Container * pCont, Pass* pParent, const char *name)
{
    //LOGD("create PassState %s\n", name ? name : "-");
    m_type = TUniform;
    m_parent = pParent;
    m_container = pCont;
    if(name)
        m_name = std::string(name);
    m_uniformSrc = NULL;
    m_uniformDst = NULL;
    m_pSGDST = NULL;
}
/*************************************************************************/ /**
 ** copy constructor. BUT FOR NOW LIMITED
 ** 
 **/ /*************************************************************************/ 
PassState::PassState(Container * pCont, Pass* pParent, PassState* pState)
{
    // For now let's copy only the Uniform assignment pass-states. No need for anything else
    /*assert(pState->m_type == TUniform);*/
  if (pState)    
  {
    CHECK_TRUE(pState->m_type == TUniform);
    m_type = pState->m_type;
    m_name = pState->m_name;
    m_uniformSrc = new_Uniform(pState->m_uniformSrc->getName());
    m_uniformSrc->copyFrom(pState->m_uniformSrc);
  }
    m_parent = pParent;
    m_container = pCont;    
    m_uniformDst = NULL;
}
/*************************************************************************/ /**
 ** 
 ** 
 **/ /*************************************************************************/ 
bool PassState::setShaders(Type type, IShader **progs, int numProgs)
{
    switch(type)
    {
    case TVertexShader:
    case TFragmentShader:
    case TCUDAModule:
    case TGeometryShader:
    case TTCSShader:
    case TTESShader:
    case TComputeShader:
    case TPathShader:
        break;
    default:
        return false;
    }
    m_type = type;
    m_shaders.clear();
    for(int i=0; i<numProgs; i++)
    {
        m_shaders.push_back(static_cast<Shader*>(progs[i]));
    }
    return true;
}

/*************************************************************************/ /**
 ** 
 ** 
 **/ /*************************************************************************/ 
void PassState::setStateGroup(IStateGroupRaster *pSGR)
{
    m_type = TStateGroup_Raster;
    m_pSGR = static_cast<StateGroupRaster*>(pSGR);
}
/*************************************************************************/ /**
 ** 
 ** 
 **/ /*************************************************************************/ 
void PassState::setStateGroup(IStateGroupCS *pSGCS)
{
    m_type = TStateGroup_CS;
    m_pSGCS = static_cast<StateGroupCS*>(pSGCS);
}
/*************************************************************************/ /**
 ** 
 ** 
 **/ /*************************************************************************/ 
void PassState::setStateGroup(IStateGroupDST *pSGDST)
{
    m_type = TStateGroup_DST;
    m_pSGDST = static_cast<StateGroupDST*>(pSGDST);
}
#ifndef OGLES2
/*************************************************************************/ /**
 ** 
 ** 
 **/ /*************************************************************************/ 
void PassState::setStateGroup(IStateGroupPath *pSGPR)
{
    m_type = TStateGroup_PR;
    m_pSGPR = static_cast<StateGroupPath*>(pSGPR);
}
/*************************************************************************/ /**
 ** 
 ** 
 **/ /*************************************************************************/ 
bool PassState::setUniform(const char *dstVarName, int idx, const char *srcVarName)
{
    if((!srcVarName)||(!dstVarName))
        return false;
    if(!m_uniformSrc)
    {
        m_uniformSrc = new_Uniform(dstVarName);
        // DON'T do it here... messes up the parsing m_uniformDst = static_cast<nvFX::Uniform*>(m_container->createUniform(dstVarName)); // we won't set its type here... should be allright
    }
    else { CHECK_TRUE(m_type == TSubRoutineUniform); }
    return m_uniformSrc->setSubroutine(srcVarName, idx) ? true : false;
}
/*************************************************************************/ /**
 ** 
 ** 
 **/ /*************************************************************************/ 
bool PassState::setUniform(const char *dstVarName, const char **srcVarNames, int numNames)
{
    if((!srcVarNames)||(!dstVarName))
        return false;
    if(!m_uniformSrc)
    {
        m_uniformSrc = new_Uniform(dstVarName);
        // DON'T do it here... messes up the parsing m_uniformDst = static_cast<nvFX::Uniform*>(m_container->createUniform(dstVarName)); // we won't set its type here... should be allright
    }
    else { CHECK_TRUE(m_type == TSubRoutineUniform); }
    m_type = TSubRoutineUniform;
    return m_uniformSrc->setSubroutines(srcVarNames, numNames) ? true : false;
}
#endif
/*************************************************************************/ /**
 ** 
 ** myVariable[idx] = [3.0, 2.1, 5.2]; // nComps = 3
 ** idx ?? TODO...
 ** nComps > 4 ? TODO...
 ** what about array of vectors ?? TODO
 **
 **/ /*************************************************************************/ 
bool PassState::setUniform(const char *varName, int idx, float *pF, int nComps, int nVecs)
{
    if(!varName)
        return false;
    if(!m_uniformSrc)
    {
        m_uniformSrc = new_Uniform(varName);
        // DON'T do it here... messes up the parsing m_uniformDst = static_cast<nvFX::Uniform*>(m_container->createUniform(varName)); // we won't set its type here... should be allright
    }
    /*else { assert(m_type == TUniform); }*/
    else { CHECK_TRUE(m_type == TUniform); }

    m_type = TUniform;
    return m_uniformSrc->setValuefv(pF, nComps, nVecs) ? true : false; // and idx ??
}
/*************************************************************************/ /**
 ** idx ?? TODO...
 ** nComps > 4 ? TODO...
 ** what about array of vectors ?? TODO
 ** 
 **/ /*************************************************************************/ 
bool PassState::setUniform(const char *varName, int idx, int *pI, int nComps, int nVecs)
{
    if(!varName)
        return false;
    if(!m_uniformSrc)
    {
        m_uniformSrc = new_Uniform(varName);
        // DON'T do it here... messes up the parsing m_uniformDst = static_cast<nvFX::Uniform*>(m_container->createUniform(varName)); // we won't set its type here... should be allright
    }
    /*else { assert(m_type == TUniform); }*/
    else { CHECK_TRUE(m_type == TUniform); }
    m_type = TUniform;
    return m_uniformSrc->setValueiv(pI, nComps, nVecs) ? true : false;
}

/*************************************************************************/ /**
 ** 
 ** 
 **/ /*************************************************************************/ 
bool PassState::setSamplerResource(const char *varName, IResource *pRes, int texUnit, ISamplerState *pSState)
{
    if(/*(!pTex)||*/(!varName))
        return false;
    if(!m_uniformSrc)
    {
        m_uniformSrc = new_Uniform(varName);
        // DON'T do it here... messes up the parsing m_uniformDst = static_cast<nvFX::Uniform*>(m_container->createUniform(varName)); // we won't set its type here... should be allright
    }
    /*else { assert(m_type == TUniform); }*/
    else { CHECK_TRUE(m_type == TUniform); }
    m_type = TUniform;
    return m_uniformSrc->setSamplerResource(pRes, texUnit, pSState) ? true : false;
}

/*************************************************************************/ /**
 ** 
 ** 
 **/ /*************************************************************************/ 
bool PassState::setTextureResource(const char *varName, IResource *pRes)
{
    if(/*(!pTex)||*/(!varName))
        return false;
    if(!m_uniformSrc)
    {
        m_uniformSrc = new_Uniform(varName);
        // DON'T do it here... messes up the parsing m_uniformDst = static_cast<nvFX::Uniform*>(m_container->createUniform(varName)); // we won't set its type here... should be allright
    }
    /*else { assert(m_type == TUniform); }*/
    else { CHECK_TRUE(m_type == TUniform); }
    m_type = TUniform;
    return m_uniformSrc->setTextureResource(pRes) ? true : false;
}
/*************************************************************************/ /**
 ** 
 ** 
 **/ /*************************************************************************/ 
bool PassState::setImageResource(const char *varName, IResource *pRes, int unit)
{
    if(/*(!pTex)||*/(!varName))
        return false;
    if(!m_uniformSrc)
    {
        m_uniformSrc = new_Uniform(varName);
        // DON'T do it here... messes up the parsing m_uniformDst = static_cast<nvFX::Uniform*>(m_container->createUniform(varName)); // we won't set its type here... should be allright
    }
    //else { assert(m_type == TUniform); }
    else { CHECK_TRUE(m_type == TUniform); }
    m_type = TUniform;
    return m_uniformSrc->setImageResource(pRes, unit) ? true : false;
}

/*************************************************************************/ /**
 ** 
 ** 
 **/ /*************************************************************************/ 
bool PassState::setSamplerState(const char *varName, ISamplerState* pSState)
{
    if(!pSState)
        return false;
    if(!m_uniformSrc)
    {
        m_uniformSrc = new_Uniform(varName);
        // DON'T do it here... messes up the parsing m_uniformDst = static_cast<nvFX::Uniform*>(m_container->createUniform(varName)); // we won't set its type here... should be allright
    }
    //else { assert(m_type == TUniform); }
    else { CHECK_TRUE(m_type == TUniform); }
    m_type = TUniform;
    return m_uniformSrc->setSamplerState(pSState) ? true : false;
}

bool PassState::setRenderingMode(RenderingMode rmode)
{
    m_type = TRenderMode;
    m_rendermode = rmode;
    return true;
}

bool PassState::setRenderGroup(int g)
{
    m_type = TRenderGroup;
    m_rendergroup = g;
    return true;
}
bool        PassState::setClearMode(ClearMode cmode)
{
    m_type = TClearMode;
    m_clearMode = cmode;
    return true;
}
bool        PassState::setClearColor(int i, float r, float g, float b, float a)
{
    m_type = TClearColor;
    m_colorTarget = i;
    m_clearColor[0] = r;
    m_clearColor[1] = g;
    m_clearColor[2] = b;
    m_clearColor[3] = a;
    return true;
}
bool        PassState::setRenderTarget(IFrameBufferObject *p)
{
    m_type = TFBOTarget;
    m_FBOTarget = static_cast<FrameBufferObject*>(p);
    return true;
}
bool        PassState::setSwapResources(IFrameBufferObject *p1, IFrameBufferObject *p2)
{
    m_type = TSwapResource;
    m_FBO[0] = static_cast<FrameBufferObject*>(p1);
    m_FBO[1] = static_cast<FrameBufferObject*>(p2);
    return true;
}
bool        PassState::setBlitFBOToActiveTarget(IFrameBufferObject *p)
{
    m_type = TBlitFBOSrc;
    m_BlitFBOSrc = static_cast<FrameBufferObject*>(p);
    return true;
}
bool        PassState::setViewport(int x, int y, int w, int h, float minZ, float maxZ)
{
    m_type = TViewport;
    m_viewport[0] = x;
    m_viewport[1] = y;
    m_viewport[2] = w;
    m_viewport[3] = h;
    m_viewportZ[0] = minZ;
    m_viewportZ[1] = maxZ;
    return true;
}
/*************************************************************************/ /**
 ** 
 ** 
 **/ /*************************************************************************/ 
//bool PassState::setTextureResource(const char *varName, IResource *pRes)
//{
//    return false;
//}

#ifdef USECUDA
/*************************************************************************/ /**
 ** 
 ** 
 **/ /*************************************************************************/ 
bool        PassState::setStateKernelEntry(const char * kernelName, int numArgs, ArgVal *argvalues)
{
    m_type = TCUDAKernelEntry;
    m_name = kernelName;
    m_numArgs = numArgs;
    // let's keep the existing ptr
    m_argvalues = argvalues;//new ArgVal[numArgs];
    m_kernelEntryId = -1;
    return true;
}
bool        PassState::setStateBlockSz(int x, int y, int z)
{
    m_type = TCUDABlockSz;
    m_blockSz[0] = x;
    m_blockSz[1] = y;
    m_blockSz[2] = z;
    return true;
}
bool        PassState::setStateGridSz(int x, int y, int z)
{
    m_type = TCUDAGridSz;
    m_gridSz[0] = x;
    m_gridSz[1] = y;
    m_gridSz[2] = z;
    return true;
}
bool        PassState::setStateSharedMemory(int Sz)
{
    m_type = TCUDAShMemSz;
    m_sharedMemSz = Sz;
    return true;
}
#else
bool        PassState::setStateKernelEntry(const char * kernelName, int numArgs, ArgVal *argvalues)
{
    return false;
}
bool        PassState::setStateBlockSz(int x, int y, int z)
{
    return false;
}
bool        PassState::setStateGridSz(int x, int y, int z)
{
    return false;
}
bool        PassState::setStateSharedMemory(int Sz)
{
    return false;
}
#endif
bool        PassState::setStateComputeGroup(int x, int y, int z)
{
    m_type = TComputeGroup;
    m_gridSz[0] = x;
    m_gridSz[1] = y;
    m_gridSz[2] = z;
    return true;
}
/*************************************************************************/ /**
 ** 
 ** 
 ** 
 **/ /*************************************************************************/ 
static char *debugPassStateType[] =
{
    "TUniform", ///< a uniform parameter assignment from the pass
    "TStateGroup_Raster", ///< a state group assignment
    "TStateGroup_CS", ///< a state-group assignment
    "TStateGroup_DST", ///< a state-group assignment
    "TStateGroup_PR", ///< a state-group assignment
    "TVertexShader", ///< a vertex shader assignment
    "TFragmentShader", ///< a fragment shader assignment
    "TGeometryShader", ///< a geom shader assignment
    "TTCSShader", ///< a Tess ctrl point shader (== D3D Hull)
    "TTESShader", ///< a Tess evaluation shader
    "TComputeShader", ///< a compute shader assignment (GLSL or DX compute)
    "TComputeGroup", ///< a compute group dimension (GLSL or DX compute)
    #ifndef OGLES2
    "TSubRoutineUniform", ///< a uniform subroutine parameter assignment from the pass
    #endif
    // Additional type of pass-states
    "TRenderMode",
    "TRenderGroup",
    "TClearMode",
    "TClearColor",
    "TFBOTarget",
    "TBlitFBOSrc",
    "TViewport",
    "TSwapResource", ///< for FBO+texture resource swapping operation
    "TCUDAModule", ///< a CUDA module : some CUDACode/CUDAKernel put together
    "TCUDAKernelEntry", ///< defines the CUDA Entry point use for invokation (func name + args vals)
    "TCUDAGridSz", ///< size of the Grid for CUDA
    "TCUDABlockSz", ///< size of the Block for CUDA
    "TCUDAShMemSz", ///< size of the shared memory
    "TPathShader", ///< the pass state specifies some vector commands (SVG/PS). No a shader but set of instructions !
    "TPathFillMode", ///< typically INVERT, COUNT_UP_NV, COUNT_DOWN_NV or PATH_FILL_MODE_NV
    "TPathReference",
    "TPathMask",
    "TPathTransformType",///< typically TRANSLATE_X_NV etc.
    "TPathCoverMode", ///< typically CONVEX_HULL_NV or BOUNDING_BOX_NV
    "TPathTransfValues",
};
/*************************************************************************/ /**
 ** Note: it is possible that we can avoid this method
 ** Almost all the states don't have action and were used for init time (validate)
 ** \arg bUniform : allows if true, to update uniforms defined in the pass.
 ** \Note if the pass doesn't have any program, the uniform will update *all* existing programs (!)
 ** this is an interesting case for GLSL-ES : the way to set default values thanks to a pass
 ** \Note : this implementation is not efficient : we should bind programs for many uniforms, rather than the opposite.
 ** For now, assume that this solution is for "offline"/init operation. and certainly not for rendering loop !!!
 **/ /*************************************************************************/ 
void PassState::apply(bool bUniforms, PassInfo * pr)
{
    NXPROFILEFUNCCOL2(debugPassStateType[(int)m_type]/*__FUNCTION__*/, COLOR_MAGENTA, 3);
    switch(m_type)
    {
    case TComputeShader:
        // don't do anything : the compiled prgrams (shaders) are handled by the pass
        break;
    case TVertexShader:
        // don't do anything : the compiled prgrams (shaders) are handled by the pass
        break;
    case TFragmentShader:
        // don't do anything : the compiled prgrams (shaders) are handled by the pass
        break;
    case TStateGroup_CS:
        m_pSGCS->apply();
        break;
    case TStateGroup_DST:
        m_pSGDST->apply();
        break;
#ifndef OGLES2
    case TStateGroup_PR:
        // in Path rendering, we also want to flag pr to tell about it, so we know dedicated values may be there
      if (pr)
      {
        pr->flags |= PR_PATHRENDERING;
        m_pSGPR->apply(m_parent, pr, 0);
      }
        break;
#endif
    case TStateGroup_Raster:
        m_pSGR->apply();
        break;
    case TUniform:
        if(bUniforms)
        {
            // here we set the destination uniform from a source uniform
            if(m_uniformDst == NULL)
                m_uniformDst = static_cast<Uniform*>(m_container->createUniform(m_uniformSrc->m_name.c_str(), NULL, NULL, false /*be local*/));
            if(m_uniformDst == NULL)
                m_uniformDst = static_cast<Uniform*>(m_container->findUniform(m_uniformSrc->m_name.c_str()));
            if(m_uniformDst == NULL)
            { LOGE("No way to find or create the destination Uniform %s for the pass !\n", m_uniformSrc->m_name.c_str()); }
            else
                m_uniformDst->setFromUniform(m_uniformSrc);
        }
        break;
#ifndef OGLES2
    case TSubRoutineUniform:
        //m_uniform->update();
        break;
#endif
    case TRenderMode:
        if(pr) {
            pr->flags |= PR_RENDERINGMODE;
            pr->renderingMode = m_rendermode;
        }
        break;
    case TRenderGroup:
        if(pr) {
            pr->flags |= PR_RENDERINGGROUP;
            pr->renderingGroup = m_rendergroup;
        }
        break;
    case TClearMode:
        if(pr)
        {
            pr->flags |= PR_CLEARMODE;
            pr->clearMode = m_clearMode;
        }
        break;
    case TClearColor:
        if(pr) {
            pr->flags |= PR_CLEARCOLOR;
            memcpy(pr->clearColor+m_colorTarget, m_clearColor, sizeof(float)*4);
            pr->clearColorValid[m_colorTarget] = true;
        }
        break;
    case TFBOTarget:
        if(pr) {
            pr->flags |= PR_CURRENTFBO;
            pr->currentFBO = m_FBOTarget ? m_FBOTarget : NULL;
        }
        break;
    case TSwapResource:
        if(pr) {
            pr->flags |= PR_SWAPRESOURCES;
            pr->swapResourceFBOs[0] = m_FBO[0];
            pr->swapResourceFBOs[1] = m_FBO[1];
        }
        break;
    case TBlitFBOSrc:
        if(pr) 
        {
            pr->flags |= PR_BLITSOURCEFBO;
            pr->blitSourceFBO = m_BlitFBOSrc;
            // a blit implicitly tells that we don't need to do anything else
            pr->renderingMode = DO_NOTHING;
        }
        break;
    case TViewport:
        if(pr) {
            pr->flags |= PR_VIEWPORT;
            memcpy(pr->viewport, m_viewport, sizeof(int)*4);
            memcpy(pr->viewportZ, m_viewportZ, sizeof(float)*2);
        }
        break;
    // Commented : the validation is enough: This texture will be setup and added to "usedTextures" in the pass
    // No need to process it during the execution of the pass
    //case TTexture:
    //    if(m_pTexture)
    //    {
    //        Texture* pTex = static_cast<Texture*>(m_pTexture);
    //        pTex->update(NULL, m_parent, m_parent->m_activeStatesLayer, false, true);
    //    }
    //    break;
#ifdef USECUDA
    // TCUDAKernelEntry state is registered as both setup and execution state (see Pass::createStateKernelEntry)
    // Which is why we have it here.
    // We could have stored the kernerEntryId directly into the body of the pass (in StatesLayer along with programCUDA)
    // rather than keeping it into the state. But for now let's stick to this approach
    case TCUDAKernelEntry:
        CHECK_TRUE(false); // should not happen anymore
        if(pr) {
            pr->flags |= PR_CUDA;
            pr->kernelEntry = m_kernelEntryId;
            pr->renderingMode = DO_NOTHING;
        }
        break;
    case TCUDAModule:
    case TCUDAGridSz:
    case TCUDABlockSz:
    case TCUDAShMemSz:
#endif
    default:
        CHECK_TRUE_MSG(false,"cannot happen");
        break;
    }
}

/***************************************************************************/
/* Pass Pass Pass Pass Pass Pass Pass Pass Pass Pass Pass Pass Pass Pass   */
/***************************************************************************/

/*************************************************************************/ /**
 ** 
 ** 
 **/ /*************************************************************************/ 
Pass::~Pass()
{
    StatesLayerMap::iterator iPM = m_statesLayers.begin();
    while(iPM != m_statesLayers.end())
    {
        StatesLayer &sl = iPM->second;
        StateVec::iterator it = sl.statesForExecution.begin();
        while(it != sl.statesForExecution.end())
        {
            if(*it)
                delete *it;
            ++it;
        }
        it = sl.statesForValidation.begin();
        while(it != sl.statesForValidation.end())
        {
            if(*it)
                delete *it;
            ++it;
        }
//#ifndef USE_OLDPROGRAM
        ShaderModuleRepository* pShdRep = static_cast<ShaderModuleRepository*>(nvFX::getShaderModuleRepositorySingleton());
        if(sl.programPipeline)
        {
            delete_ProgramPipeline(sl.programPipeline, iPM->first);
        }
//#endif
        sl.statesForExecution.clear();
        sl.statesForValidation.clear();
        ++iPM;

        if (NULL!=sl.program)
        {
          delete_Program(sl.program);
          sl.program->releaseTarget(this, iPM->first);
          sl.program = NULL;
          pShdRep->releaseProgram(sl.program);
        }
    }

    if (NULL!=m_pBaseStatesLayer->program)
    {
      delete_Program(m_pBaseStatesLayer->program);
      m_pBaseStatesLayer->program = NULL;
    }
    
}

void Pass::delete_ProgramPipeline(ProgramPipeline* pp, int layerID)
{
    // un-register targets for the programs
    Program* p;
    int i;
    ShaderModuleRepository* pShdRep = static_cast<ShaderModuleRepository*>(nvFX::getShaderModuleRepositorySingleton());
    for(i=0; p = static_cast<Program*>(pp->getShaderProgram(i)); i++)
    {
        p->releaseTarget(this, layerID);
        pShdRep->releaseProgram(p);
    }
    ::delete_ProgramPipeline(pp);
}

/*************************************************************************/ /**
 ** 
 ** 
 **/ /*************************************************************************/ 
Pass::Pass(Container *pCont, Technique *pParent, const char *name)
{
    m_loopCnt = 1;
    m_overrideID = 0;
    m_parent = pParent;
    m_validated = 0;
    if(name)
        m_name = std::string(name);
    m_container = pCont;
    StatesLayer &sl = m_statesLayers[0]; // create it
    m_pActiveStatesLayer = &sl;
    m_pBaseStatesLayer = &sl;
    m_activeStatesLayer = 0;
    // -1 if this pass doesn't have any compute
    m_GLSLComputeGroupSize[0] = -1;
    m_GLSLComputeGroupSize[1] = -1;
    m_GLSLComputeGroupSize[2] = -1;
}

/*************************************************************************/ /**
 ** 
 ** 
 **/ /*************************************************************************/ 
bool Pass::invalidate()
{
    if(!(m_validated & VALIDATED_PASS))
        return true;
    LOGD("Invalidating Pass %s\n", m_name.c_str());
    invalidateResources();
    m_validated &= ~VALIDATED_PASS;
    StatesLayerMap::iterator iPM = m_statesLayers.begin();
    m_container->invalidateTargets(this, -1);
    while(iPM != m_statesLayers.end())
    {
        StatesLayer &sl = iPM->second;
        if(sl.program)
        {
            sl.program->linkNeeded(1);
        }
//#ifndef USE_OLDPROGRAM
        if(sl.programPipeline)
        {
            //sl.programPipeline->
        }
//#endif
        ++iPM;
    }
    return true;
}

IProgram* Pass::createShaderProgram(std::map<std::string, PassState*> &passStateShader, ShaderType type, ProgramPipeline* &programPipeline, int layerID)
{
    IShader *pShader;
    std::map<std::string, PassState*>::iterator iMPS;
    int sz = 0;
    iMPS = passStateShader.begin();
    // count the shader modules
    while(iMPS != passStateShader.end())
    {
        for(int i=0; (pShader = iMPS->second->getShader(i)); i++)
            sz++;
        ++iMPS;
    }
    // put them in a temporary array
    IShader** pShaders = new IShader*[sz];
    iMPS = passStateShader.begin();
    sz = 0;
    while(iMPS != passStateShader.end())
    {
        for(int i=0; (pShader = iMPS->second->getShader(i)); i++)
            pShaders[sz++] = pShader;
        ++iMPS;
    }
    // create the shader program through the main effect container
    Program* pShdProg = static_cast<Program*>(m_container->createShaderProgram(type, sz, pShaders));
    // the idea: fill the pointer here only if no shader-pipeline used
    delete [] pShaders;
    if(!pShdProg)
    {
        nvFX::printf("Encountered errors during creation of shader-program\n");
        if(programPipeline)
            ::delete_ProgramPipeline(programPipeline);
        programPipeline = NULL;
        return NULL;
    }
    // add the shader program to the program-pipeline if exist
    if(programPipeline)
    {
        programPipeline->addProgramShader(pShdProg);
        pShdProg->addTarget(this, layerID);
    }
    return pShdProg;
}
/*************************************************************************/ /**
 ** 
 ** 
 **/ /*************************************************************************/ 
bool Pass::validate()
{
    NXPROFILEFUNCCOL2(__FUNCTION__, COLOR_BLUE2, 2);
    bool bErr = false;
    if((m_validated&VALIDATED_PASS))
    {
        //
        // Already validated : let's just update active uniforms, if needed
        //
        // for now : let's just test the default layer.
//#ifdef USE_OLDPROGRAM
//        if(m_pBaseStatesLayer->program)
//#else
        if(m_pBaseStatesLayer->program || m_pBaseStatesLayer->programPipeline)
//#endif
        {
            //if(!sl.program->bind())
            //    return false;
            // update uniforms related to this pass
            m_container->updateCstBuffers(this, -1, false, false);
            m_container->updateUniforms(this, -1, false, true);
            m_container->updateSamplerStates(this, -1, false, true);
            //m_program->unbind();
        }
        return true;
    }
    //
    // Validate the State-Groups
    // Note : we will loop many times in the same m_state for purpose of clarity... could be optimized
    //
    StateVec::iterator it = m_pBaseStatesLayer->statesForExecution.begin();
    while(it != m_pBaseStatesLayer->statesForExecution.end())
    {
        PassState::Type t = (*it)->getType();
        switch(t)
        {
        case PassState::TStateGroup_CS:
            if(!(*it)->m_pSGCS->validate())
                bErr = true;
            break;
        case PassState::TStateGroup_DST:
            if(!(*it)->m_pSGDST->validate())
                bErr = true;
            break;
        case PassState::TStateGroup_Raster:
            if(!(*it)->m_pSGR->validate())
                bErr = true;
            break;
#ifndef OGLES2
        case PassState::TStateGroup_PR:
            if(!(*it)->m_pSGPR->validate())
                bErr = true;
            break;
#endif
        }
        ++it;
    }
    //
    // The pass is not validated. Let's do it
    // let's invalidate the uniforms for this pass, if ever some kept a reference to the pass
    //
    m_container->invalidateTargets(this, -1);
    //
    //create the program here
    //
    // Check if we need to create a program
    it = m_pBaseStatesLayer->statesForValidation.begin();
    bool bHasProgram = false;
    while(it != m_pBaseStatesLayer->statesForValidation.end())
    {
        PassState::Type t = (*it)->getType();
        if(   (t == PassState::TVertexShader)
            ||(t == PassState::TFragmentShader)
            ||(t == PassState::TGeometryShader)
            ||(t == PassState::TTCSShader)
            ||(t == PassState::TTESShader)
            ||(t == PassState::TComputeShader)
            ||(t == PassState::TCUDAModule)
            ||(t == PassState::TPathShader) )
        {
            bHasProgram = true;
            break;
        }
        ++it;
    }
    //
    // create the whole program thing
    // We really assume that if not validated, the pass never had a previous program already there
    //
    if(bHasProgram)
    {
        //
        // Validate Global objects.
        // NOTE: this would be better to do this outside of any specific pass : here we will validate them
        // as many times as there are passes... useless !
        //
        //m_container->validateUniforms();
        m_container->validateSamplerStates();
        //m_container->validateTextures();
        {
            IShader *pShader;
            std::map<std::string, PassState*> passStatesBaseLayerVS;
            std::map<std::string, PassState*> passStatesBaseLayerGS;
            std::map<std::string, PassState*> passStatesBaseLayerTCS;
            std::map<std::string, PassState*> passStatesBaseLayerTES;
            std::map<std::string, PassState*> passStatesBaseLayerPS;
            std::map<std::string, PassState*> passStatesBaseLayerCS;
            std::map<std::string, PassState*> passStatesBaseLayerPathSource;
            PassState*                        passStatesBaseLayerCGrpSz;
#ifdef USECUDA
            std::map<std::string, PassState*> passStatesBaseLayerCUDA;
            PassState*                        passStateCUDAEntry = NULL;
            PassState*                        passStateCUDAGridSz = NULL;
            PassState*                        passStateCUDABlockSz = NULL;
            PassState*                        passStateCUDAShMemSz = NULL;
#endif
            it = m_pBaseStatesLayer->statesForValidation.begin();
            while(it != m_pBaseStatesLayer->statesForValidation.end())
            {
                PassState::Type t = (*it)->getType();
                const char * name;
                name = (*it)->getName();
                if(name == NULL)
                    name = "";
                switch(t)
                {
                case PassState::TVertexShader:
                    passStatesBaseLayerVS[name] = (*it);
                    break;
                case PassState::TGeometryShader:
                    passStatesBaseLayerGS[name] = (*it);
                    break;
                case PassState::TTCSShader:
                    passStatesBaseLayerTCS[name] = (*it);
                    break;
                case PassState::TTESShader:
                    passStatesBaseLayerTES[name] = (*it);
                    break;
                case PassState::TFragmentShader:
                    passStatesBaseLayerPS[name] = (*it);
                    break;
                case PassState::TComputeShader:
                    passStatesBaseLayerCS[name] = (*it);
                    break;
                case PassState::TComputeGroup:
                    passStatesBaseLayerCGrpSz = (*it);
                    break;
                case PassState::TPathShader:
                    passStatesBaseLayerPathSource[name] = (*it);
                    break;
#ifdef USECUDA
                case PassState::TCUDAModule:
                    passStatesBaseLayerCUDA[name] = (*it);
                    break;
                case PassState::TCUDAKernelEntry:
                    passStateCUDAEntry = (*it); // keep track of the CUDA entry for this pass
                    break;
                case PassState::TCUDAGridSz:
                    passStateCUDAGridSz = (*it);
                    break;
                case PassState::TCUDABlockSz:
                    passStateCUDABlockSz = (*it);
                    break;
                case PassState::TCUDAShMemSz:
                    passStateCUDAShMemSz = (*it);
                    break;
#endif
                default:
                    break;
                // TODO: add more for tess, geom shaders...
                }
                ++it;
            }
#ifdef USECUDA
            // Let's create CUDA module (through the class Program) if needed
            if(!passStatesBaseLayerCUDA.empty())
            {
                if(m_pBaseStatesLayer->programCUDA)
                {
                    m_container->invalidateTargets(this, 0);
                }
                int sz = 0;
                std::map<std::string, PassState*>::iterator iMPS = passStatesBaseLayerCUDA.begin();
                while(iMPS != passStatesBaseLayerCUDA.end())
                {
                    for(int i=0; (pShader = iMPS->second->getShader(i)); i++)
                        sz++;
                    ++iMPS;
                }
                IShader** pShaders = new IShader*[sz];
                iMPS = passStatesBaseLayerCUDA.begin();
                sz = 0;
                while(iMPS != passStatesBaseLayerCUDA.end())
                {
                    for(int i=0; (pShader = iMPS->second->getShader(i)); i++)
                        pShaders[sz++] = pShader;
                    ++iMPS;
                }
                m_pBaseStatesLayer->programCUDA = (Program*)m_container->createShaderProgram(nvFX::FX_CUDAPROG, sz, pShaders);
                delete [] pShaders;
                if(!m_pBaseStatesLayer->programCUDA)
                {
                    nvFX::printf("Encountered errors during creation of shader-program (Module) for CUDA\n");
                    return false;
                }
                if(!m_pBaseStatesLayer->programCUDA->bind(m_container))
                {
                    delete m_pBaseStatesLayer->programCUDA;
                    m_pBaseStatesLayer->programCUDA = NULL;
                    nvFX::printf("Encountered errors during validation of Pass (CUDA Module) %s\n", m_name.c_str());
                    return false;
                }
                // Now let's prepare the CUDA kernel entry point for later execution of this pass
                if(!passStateCUDAEntry)
                    nvFX::printf("Error: no CUDA entry for this pass %s\n", m_name.c_str());
                else if(!passStateCUDABlockSz)
                    nvFX::printf("Error: no CUDA Block size for this pass %s\n", m_name.c_str());
                else if(!passStateCUDAGridSz)
                    nvFX::printf("Error: no CUDA Grid size for this pass %s\n", m_name.c_str());
                else
                {
                    m_pBaseStatesLayer->kernelEntry = m_pBaseStatesLayer->programCUDA->createKernelEntry(
                        passStateCUDAEntry->m_name.c_str(),
                        passStateCUDAGridSz->m_gridSz,
                        passStateCUDABlockSz->m_blockSz,
                        passStateCUDAShMemSz ? passStateCUDAShMemSz->m_sharedMemSz : 0,
                        passStateCUDAEntry->m_numArgs, passStateCUDAEntry->m_argvalues);
                    passStateCUDAEntry->m_kernelEntryId = m_pBaseStatesLayer->kernelEntry; // not really useful but let's do it to be clean
                }
            }
#endif
//#ifdef USE_OLDPROGRAM
            // CANCELED:
            // create a fake program just to see how the separate shader is working.
            // we cannot wait for real program to be created because we would already 
            // have taken the path of separate vs. non-separate
            //Program *p = new_Program(m_container);
            //delete_Program(p);
            //
            //
            // ====> Case where we don't support separate shaders
            //
            if(m_container->separateShadersEnabled() == false)
            {
                //
                // take care of the base layer
                //
                if(m_pBaseStatesLayer->program)
                {
                    m_container->invalidateTargets(this, 0);
                    delete_Program(m_pBaseStatesLayer->program);
                }
                //
                // send them to the program
                // Note: the passStatesBaseLayerVS order depends on the map sorting (!)
                // Note that this non-separable shader doesn't really need to work with
                // programs that would be stored outside to optimize...
                // in this case, 1 Pass == 1 Program. but this can be improved if needed
                //
                m_pBaseStatesLayer->program = new_Program(m_container);
                std::map<std::string, PassState*>::iterator iMPS = passStatesBaseLayerVS.begin();
                while(iMPS != passStatesBaseLayerVS.end())
                {
                    for(int i=0; (pShader = iMPS->second->getShader(i)); i++)
                        if(!m_pBaseStatesLayer->program->addVertexShader(pShader, m_container))
                        {
                            nvFX::eprintf("Encountered errors during creation of program Pass %s\n", m_name.c_str());
                            delete_Program(m_pBaseStatesLayer->program);
                            m_pBaseStatesLayer->program = NULL;
                            return false;
                        }
                    ++iMPS;
                }
                iMPS = passStatesBaseLayerPS.begin();
                while(iMPS != passStatesBaseLayerPS.end())
                {
                    for(int i=0; (pShader = iMPS->second->getShader(i)); i++)
                        if(!m_pBaseStatesLayer->program->addFragmentShader(pShader, m_container))
                        {
                            nvFX::eprintf("Encountered errors during creation of program Pass %s\n", m_name.c_str());
                            delete_Program(m_pBaseStatesLayer->program);
                            m_pBaseStatesLayer->program = NULL;
                            return false;
                        }
                    ++iMPS;
                }
                iMPS = passStatesBaseLayerGS.begin();
                while(iMPS != passStatesBaseLayerGS.end())
                {
                    for(int i=0; (pShader = iMPS->second->getShader(i)); i++)
                        if(!m_pBaseStatesLayer->program->addShader(nvFX::FX_GEOMPROG, pShader, m_container))
                        {
                            nvFX::eprintf("Encountered errors during creation of program Pass %s\n", m_name.c_str());
                            delete_Program(m_pBaseStatesLayer->program);
                            m_pBaseStatesLayer->program = NULL;
                            return false;
                        }
                    ++iMPS;
                }
                iMPS = passStatesBaseLayerTCS.begin();
                while(iMPS != passStatesBaseLayerTCS.end())
                {
                    for(int i=0; (pShader = iMPS->second->getShader(i)); i++)
                        if(!m_pBaseStatesLayer->program->addShader(nvFX::FX_TCSPROG, pShader, m_container))
                        {
                            nvFX::printf("Encountered errors during creation of program Pass %s\n", m_name.c_str());
                            delete_Program(m_pBaseStatesLayer->program);
                            m_pBaseStatesLayer->program = NULL;
                            return false;
                        }
                    ++iMPS;
                }
                iMPS = passStatesBaseLayerTES.begin();
                while(iMPS != passStatesBaseLayerTES.end())
                {
                    for(int i=0; (pShader = iMPS->second->getShader(i)); i++)
                        if(!m_pBaseStatesLayer->program->addShader(nvFX::FX_TESPROG, pShader, m_container))
                        {
                            nvFX::printf("Encountered errors during creation of program Pass %s\n", m_name.c_str());
                            delete_Program(m_pBaseStatesLayer->program);
                            m_pBaseStatesLayer->program = NULL;
                            return false;
                        }
                    ++iMPS;
                }
                // TODO: this program can ONLY be alone. Check and raise warning if NOT
                if(passStatesBaseLayerCS.size() > 0)
                {
                    m_pBaseStatesLayer->program->cleanup(); // to remove any other shader
                    iMPS = passStatesBaseLayerCS.begin();
                    while(iMPS != passStatesBaseLayerCS.end())
                    {
                        for(int i=0; (pShader = iMPS->second->getShader(i)); i++)
                            if(!m_pBaseStatesLayer->program->addShader(nvFX::FX_COMPUTEPROG, pShader, m_container))
                            {
                                nvFX::printf("Encountered errors during creation of program Pass %s\n", m_name.c_str());
                                delete_Program(m_pBaseStatesLayer->program);
                                m_pBaseStatesLayer->program = NULL;
                                return false;
                            }
                        ++iMPS;
                    }
    #pragma MESSAGE(__FILE__"(1051): TODO TODO TODO TODO : generalize the approach of locally storing states in the Pass")
                    // Now store the group size (== CUDA Grid) so there is no need to walk through the list of pass states during execution
                    if(passStatesBaseLayerCGrpSz)
                    {
                        m_GLSLComputeGroupSize[0] = passStatesBaseLayerCGrpSz->m_gridSz[0];
                        m_GLSLComputeGroupSize[1] = passStatesBaseLayerCGrpSz->m_gridSz[1];
                        m_GLSLComputeGroupSize[2] = passStatesBaseLayerCGrpSz->m_gridSz[2];
                    } else {
                        m_GLSLComputeGroupSize[0] = 0;
                        m_GLSLComputeGroupSize[1] = 0;
                        m_GLSLComputeGroupSize[2] = 0;
                        CHECK_TRUE(false); // Case where the Group size is not specified : we should adapt to the 
                        // render target size !! TODO
                    }
                }
                // TODO: this program can ONLY be alone. Check and raise Warning if NOT
                if(passStatesBaseLayerPathSource.size() > 0)
                {
                    m_pBaseStatesLayer->program->cleanup(); // to remove any other shader
                    iMPS = passStatesBaseLayerPathSource.begin();
                    while(iMPS != passStatesBaseLayerPathSource.end())
                    {
                        for(int i=0; (pShader = iMPS->second->getShader(i)); i++)
                            if(!m_pBaseStatesLayer->program->addShader(nvFX::FX_PATHPROG, pShader, m_container))
                            {
                                nvFX::printf("Encountered errors during creation of program Pass %s\n", m_name.c_str());
                                return false;
                            }
                        ++iMPS;
                    }
                }
                //
                // Now we can link (inside bind())
                //
                if(m_pBaseStatesLayer->program->getNumShaders() > 0)
                {
                    if(!m_pBaseStatesLayer->program->bind(m_container))
                    {
                        nvFX::printf("Encountered errors during validation of Pass (Shader Linkage) %s\n", m_name.c_str());
                        delete_Program(m_pBaseStatesLayer->program);
                        m_pBaseStatesLayer->program = NULL;
                        return false;
                    }
                } else {
                    delete_Program(m_pBaseStatesLayer->program);
                    m_pBaseStatesLayer->program = NULL;
                }
            } else
            //
            // ====> Separable Shader case:
            //
			{
//#else
                //
                // take care of the base layer
                //
                if(m_pBaseStatesLayer->programPipeline)
                {
                    // NOTE: this actually means that validation discards the entire program pipeline
                    // and possibly related programs to re-create them.
                    // TODO: we might be able to do more optimal approach instead
                    m_container->invalidateTargets(this, 0);
                    Pass::delete_ProgramPipeline(m_pBaseStatesLayer->programPipeline, 0);
                }
                //
                // send them to the program
                // Note: the passStatesBaseLayerVS order depends on the map sorting (!)
                //
                m_pBaseStatesLayer->programPipeline = new_ProgramPipeline(m_container);
    // TODO TODO TODO TODO TODO TODO TODO
    // factorize the following code instead of duplicating it like that !
    #pragma MESSAGE(__FILE__"(950): TODO TODO TODO TODO : factorize the following code instead of duplicating it like that !")
                //
                // Vertex program
                //
                if(passStatesBaseLayerVS.size() > 0)
                {
                    if(!createShaderProgram(passStatesBaseLayerVS, nvFX::FX_VTXPROG, m_pBaseStatesLayer->programPipeline, 0))
                        return false;
                }
                //
                // Geometry program
                //
                if(passStatesBaseLayerGS.size() > 0)
                {
                    if(!createShaderProgram(passStatesBaseLayerGS, nvFX::FX_GEOMPROG, m_pBaseStatesLayer->programPipeline, 0))
                        return false;
                }
                //
                // TCS program
                //
                if(passStatesBaseLayerTCS.size() > 0)
                {
                    if(!createShaderProgram(passStatesBaseLayerTCS, nvFX::FX_TCSPROG, m_pBaseStatesLayer->programPipeline, 0))
                        return false;
                }
                //
                // Vertex program
                //
                if(passStatesBaseLayerTES.size() > 0)
                {
                    if(!createShaderProgram(passStatesBaseLayerTES, nvFX::FX_TESPROG, m_pBaseStatesLayer->programPipeline, 0))
                        return false;
                }
                //
                // Fragment program
                //
                if(passStatesBaseLayerPS.size() > 0)
                {
                    if(!createShaderProgram(passStatesBaseLayerPS, nvFX::FX_FRAGPROG, m_pBaseStatesLayer->programPipeline, 0))
                        return false;
                }
                //
                // compute program
                //
                if(passStatesBaseLayerCS.size() > 0)
                {
                    IProgram* pShdProg = createShaderProgram(passStatesBaseLayerCS, nvFX::FX_COMPUTEPROG, m_pBaseStatesLayer->programPipeline, 0);
                    if(pShdProg == NULL)
                        return false;
                    // in this special Compute case, let's store the only one (there can't be more) program in m_pBaseStatesLayer->program
                    m_pBaseStatesLayer->program = static_cast<Program*>(pShdProg);
    #pragma MESSAGE(__FILE__"(1051): TODO TODO TODO TODO : generalize the approach of locally storing states in the Pass")
                    // Now store the group size (== CUDA Grid) so there is no need to walk through the list of pass states during execution
                    if(passStatesBaseLayerCGrpSz)
                    {
                        m_GLSLComputeGroupSize[0] = passStatesBaseLayerCGrpSz->m_gridSz[0];
                        m_GLSLComputeGroupSize[1] = passStatesBaseLayerCGrpSz->m_gridSz[1];
                        m_GLSLComputeGroupSize[2] = passStatesBaseLayerCGrpSz->m_gridSz[2];
                    } else {
                        m_GLSLComputeGroupSize[0] = 0;
                        m_GLSLComputeGroupSize[1] = 0;
                        m_GLSLComputeGroupSize[2] = 0;
                        CHECK_TRUE(false); // Case where the Group size is not specified : we should adapt to the 
                        // render target size !! TODO
                    }
                }
                //
                // Path Rendering Program
                //
                if(passStatesBaseLayerPathSource.size() > 0)
                {
                    IProgram* pShdProg = createShaderProgram(passStatesBaseLayerPathSource, nvFX::FX_PATHPROG, m_pBaseStatesLayer->programPipeline, 0);
                    if(pShdProg == NULL)
                        return false;
                    // in this special Path-rendering case, let's store the only one (there can't be more) program in m_pBaseStatesLayer->program
                    m_pBaseStatesLayer->program = static_cast<Program*>(pShdProg);
                }
                // Now we want to validate
                //
                if(m_pBaseStatesLayer->programPipeline && !m_pBaseStatesLayer->programPipeline->validate())
                {
                    delete_ProgramPipeline(m_pBaseStatesLayer->programPipeline, 0);
                    m_pBaseStatesLayer->programPipeline = NULL;
                    nvFX::printf("Encountered errors during validation of Pass (Shader Linkage) %s\n", m_name.c_str());
                    return false;
                }
            } // separate shader case
//#endif
            //==============================================================================
            //               **  Here we take care of the other layers  **
            // The idea : we take the base shaders and possibly overwrite some from the layer
            // which is why we use a map with the group-name as the key
            // However this leads to changes of order of shader code... LAME_MODE could be a problem ? To see...
            //
            m_container->invalidateTargets(this, -1);
            StatesLayerMap::iterator iSL = m_statesLayers.begin();
            while(iSL != m_statesLayers.end())
            {
                if(iSL->first == 0)
                {
                    ++iSL;
                    continue;
                }
                int layerID = iSL->first;
                StatesLayer &sl = iSL->second;
//#ifndef USE_OLDPROGRAM
                if(sl.programPipeline)
                {
                    delete_ProgramPipeline(sl.programPipeline, layerID);
                }
//#endif
                // copy the base maps and overwrite what needs to be overridden
                // Only needed for statesForValidation
                std::map<std::string, PassState*> passStatesLayerVS(passStatesBaseLayerVS);
                std::map<std::string, PassState*> passStatesLayerGS(passStatesBaseLayerGS);
                std::map<std::string, PassState*> passStatesLayerTCS(passStatesBaseLayerTCS);
                std::map<std::string, PassState*> passStatesLayerTES(passStatesBaseLayerTES);
                std::map<std::string, PassState*> passStatesLayerPS(passStatesBaseLayerPS);
                std::map<std::string, PassState*> passStatesLayerCS(passStatesBaseLayerCS);
                std::map<std::string, PassState*> passStatesLayerPathSource(passStatesBaseLayerPathSource);
#ifdef USECUDA
                std::map<std::string, PassState*> passStatesLayerCUDA(passStatesBaseLayerCUDA);
#endif
                it = sl.statesForValidation.begin();
                while(it != sl.statesForValidation.end())
                {
                    PassState::Type t = (*it)->getType();
                    const char * name;
                    name = (*it)->getName();
                    if(name == NULL)
                        name = "";
                    switch(t)
                    {
                    case PassState::TVertexShader:
                        passStatesLayerVS[name] = (*it);
                        break;
                    case PassState::TGeometryShader:
                        passStatesLayerGS[name] = (*it);
                        break;
                    case PassState::TTCSShader:
                        passStatesLayerTCS[name] = (*it);
                        break;
                    case PassState::TTESShader:
                        passStatesLayerTES[name] = (*it);
                        break;
                    case PassState::TFragmentShader:
                        passStatesLayerPS[name] = (*it);
                        break;
                    case PassState::TComputeShader:
                        passStatesLayerCS[name] = (*it);
                        break;
                    case PassState::TPathShader:
                        passStatesLayerPathSource[name] = (*it);
                        break;
#ifdef USECUDA
                    case PassState::TCUDAModule:
                        passStatesLayerCUDA[name] = (*it);
#endif
                    default:
                        break;
                    // TODO: add more for tess, geom shaders...
                    }
                    ++it;
                }
#ifdef USECUDA
            // Let's create CUDA module (through the class Program) if needed
            if(!passStatesBaseLayerCUDA.empty())
            {
                if(sl.programCUDA)
                {
                    m_container->invalidateTargets(this, 0);
                }
                int sz = 0;
                std::map<std::string, PassState*>::iterator iMPS = iMPS = passStatesBaseLayerCUDA.begin();
                while(iMPS != passStatesBaseLayerCUDA.end())
                {
                    for(int i=0; (pShader = iMPS->second->getShader(i)); i++)
                        sz++;
                    ++iMPS;
                }
                IShader** pShaders = new IShader*[sz];
                iMPS = passStatesBaseLayerCUDA.begin();
                sz = 0;
                while(iMPS != passStatesBaseLayerCUDA.end())
                {
                    for(int i=0; (pShader = iMPS->second->getShader(i)); i++)
                        pShaders[sz++] = pShader;
                    ++iMPS;
                }
                sl.programCUDA = (Program*)m_container->createShaderProgram(nvFX::FX_CUDAPROG, sz, pShaders);
                delete [] pShaders;
                if(!sl.programCUDA)
                {
                    nvFX::printf("Encountered errors during creation of shader-program (Module) for CUDA\n");
                    return false;
                }
                if(!sl.programCUDA->bind(m_container))
                {
                    delete sl.programCUDA;
                    sl.programCUDA = NULL;
                    nvFX::printf("Encountered errors during validation of Pass (CUDA Module) %s\n", m_name.c_str());
                    return false;
                }
            }
#endif
//#ifdef USE_OLDPROGRAM
				// NON separable shader case:
                if(m_container->separateShadersEnabled() == false)
                {
                    #pragma MESSAGE("WARNING: not using Compute, GS, TCS and TES in Old program (non-separable) mode")
                    //
                    // send them to the program
                    // Note: the passStatesLayerVS order depends on the map sorting (!)
                    //
                    sl.program = new_Program(m_container);
                    std::map<std::string, PassState*>::iterator iMPS = passStatesLayerVS.begin();
                    while(iMPS != passStatesLayerVS.end())
                    {
                        for(int i=0; (pShader = iMPS->second->getShader(i)); i++)
                            if(!sl.program->addVertexShader(pShader, m_container))
                                return false;
                        ++iMPS;
                    }
                    iMPS = passStatesLayerPS.begin();
                    while(iMPS != passStatesLayerPS.end())
                    {
                        for(int i=0; (pShader = iMPS->second->getShader(i)); i++)
                            if(!sl.program->addFragmentShader(pShader, m_container))
                                return false;
                        ++iMPS;
                    }
                    iMPS = passStatesLayerGS.begin();
                    while(iMPS != passStatesLayerGS.end())
                    {
                        for(int i=0; (pShader = iMPS->second->getShader(i)); i++)
                            if(!sl.program->addShader(nvFX::FX_GEOMPROG, pShader, m_container))
                                return false;
                        ++iMPS;
                    }
                    iMPS = passStatesBaseLayerTCS.begin();
                    while(iMPS != passStatesBaseLayerTCS.end())
                    {
                        for(int i=0; (pShader = iMPS->second->getShader(i)); i++)
                            if(!sl.program->addShader(nvFX::FX_TCSPROG, pShader, m_container))
                            {
                                nvFX::printf("Encountered errors during creation of program Pass %s\n", m_name.c_str());
                                return false;
                            }
                        ++iMPS;
                    }
                    iMPS = passStatesBaseLayerTES.begin();
                    while(iMPS != passStatesBaseLayerTES.end())
                    {
                        for(int i=0; (pShader = iMPS->second->getShader(i)); i++)
                            if(!sl.program->addShader(nvFX::FX_TESPROG, pShader, m_container))
                            {
                                nvFX::printf("Encountered errors during creation of program Pass %s\n", m_name.c_str());
                                return false;
                            }
                        ++iMPS;
                    }
                    if(passStatesBaseLayerCS.size() > 0)
                    {
                        iMPS = passStatesBaseLayerCS.begin();
                        sl.program->cleanup(); // to remove any other shader
                        while(iMPS != passStatesBaseLayerCS.end())
                        {
                            for(int i=0; (pShader = iMPS->second->getShader(i)); i++)
                                if(!sl.program->addShader(nvFX::FX_COMPUTEPROG, pShader, m_container))
                                {
                                    nvFX::printf("Encountered errors during creation of program Pass %s\n", m_name.c_str());
                                    return false;
                                }
                            ++iMPS;
                        }
                    }
                    // TODO: this program can ONLY be alone. Check and raise Warning if NOT
                    if(passStatesBaseLayerPathSource.size() > 0)
                    {
                        sl.program->cleanup(); // to remove any other shader
                        iMPS = passStatesBaseLayerPathSource.begin();
                        while(iMPS != passStatesBaseLayerPathSource.end())
                        {
                            for(int i=0; (pShader = iMPS->second->getShader(i)); i++)
                                if(!sl.program->addShader(nvFX::FX_PATHPROG, pShader, m_container))
                                {
                                    nvFX::printf("Encountered errors during creation of program Pass %s\n", m_name.c_str());
                                    return false;
                                }
                            ++iMPS;
                        }
                    }
                    //
                    // Now we can link (inside bind())
                    //
                    if(sl.program->getNumShaders() > 0)
                    {
                        if(!sl.program->bind(m_container))
                        {
                            delete sl.program;
                            sl.program = NULL;
                            nvFX::printf("Encountered errors during validation of Pass (Shader Linkage) %s\n", m_name.c_str());
                            return false;
                        }
                    } else {
                        // we don't have any program
                        delete sl.program;
                        sl.program = NULL;
                    }
                }
//#else			// Separable shader case:
                else {
                    //
                    // send them to the program
                    // Note: the passStatesLayerVS order depends on the map sorting (!)
                    //
                    sl.programPipeline = new_ProgramPipeline(m_container);
                    //
                    // Vertex Shader
                    //
                    if(passStatesBaseLayerVS.size() > 0)
                    {
                        if(!createShaderProgram(passStatesLayerVS, nvFX::FX_VTXPROG, sl.programPipeline, layerID))
                            return false;
                    }
                    //
                    // Geometry shader
                    //
                    if(passStatesBaseLayerGS.size() > 0)
                    {
                        if(!createShaderProgram(passStatesLayerGS, nvFX::FX_GEOMPROG, sl.programPipeline, layerID))
                            return false;
                    }
                    //
                    // TCS shader
                    //
                    if(passStatesBaseLayerTCS.size() > 0)
                    {
                        if(!createShaderProgram(passStatesLayerTCS, nvFX::FX_TCSPROG, sl.programPipeline, layerID))
                            return false;
                    }
                    //
                    // TES shader
                    //
                    if(passStatesBaseLayerTES.size() > 0)
                    {
                        if(!createShaderProgram(passStatesLayerTES, nvFX::FX_TESPROG, sl.programPipeline, layerID))
                            return false;
                    }
                    //
                    // Fragment program
                    //
                    if(passStatesBaseLayerPS.size() > 0)
                    {
                        if(!createShaderProgram(passStatesLayerPS, nvFX::FX_FRAGPROG, sl.programPipeline, layerID))
                            return false;
                    }
                    //
                    // Compute program
                    //
                    if(passStatesBaseLayerCS.size() > 0)
                    {
                        IProgram* pShdProg = createShaderProgram(passStatesLayerCS, nvFX::FX_COMPUTEPROG, sl.programPipeline, layerID);
                        if(pShdProg == NULL)
                            return false;
                        // in this special Compute case, let's store the only one (there can't be more) program in m_pBaseStatesLayer->program
                        sl.program = static_cast<Program*>(pShdProg);
                    }
                    //
                    // Path Shader program
                    //
                    if(passStatesBaseLayerPathSource.size() > 0)
                    {
                        IProgram* pShdProg = createShaderProgram(passStatesBaseLayerPathSource, nvFX::FX_PATHPROG, sl.programPipeline, layerID);
                        if(pShdProg == NULL)
                            return false;
                        // in this special Path-rendering case, let's store the only one (there can't be more) program in m_pBaseStatesLayer->program
                        sl.program = static_cast<Program*>(pShdProg);
                    }
                    //
                    // Now we want to validate
                    //
                    if(sl.programPipeline && !sl.programPipeline->validate())
                    {
                        delete_ProgramPipeline(sl.programPipeline, layerID);
                        sl.program = NULL;
                        nvFX::printf("Encountered errors during validation of Pass (Shader Linkage) %s\n", m_name.c_str());
                        return false;
                    }
                } // separate-shader case
//#endif
                ++iSL;
            } // while(iSL != m_statesLayers.end())
            //
            // Bind attributes according to the mapping, if needed
            //
            bindAttributes();
        }
        //
        // Setup some variable values on request of PassState...
        //
        it = m_pBaseStatesLayer->statesForValidation.begin();
        while(it != m_pBaseStatesLayer->statesForValidation.end())
        {
            PassState::Type t = (*it)->getType();
            switch(t)
            {
            case PassState::TUniform:
#ifndef OGLES2
            case PassState::TSubRoutineUniform:
#endif
                {
                    Uniform *uniform = static_cast<Uniform*>((*it)->getUniform());
                    // invalidate the targets : a new program was created so we need to make sure offset are re-computed
                    uniform->invalidateTarget(this, -1);
                    // this update will query targets to connect the uniform to targets ('true' flag)
                    // this method will setup the uniforms but more importantly will queue them to the usedUniforms table
                    // so that pass execution will update them when needed.
                    uniform->update(uniform->m_data, this, -1, true);
                }
                break;
            }
            ++it;
        }
        //
        // Now let's update uniforms from the possible ones stored in the Container
        //
        m_container->updateCstBuffers(this, -1, true, false);
        m_container->updateUniforms(this, -1, true, false);
        m_container->updateSamplerStates(this, -1, true, false);
        //
        // Now let's walk throught uniforms to check the status of Texture Units
        // If some didn't get updated properly, we will assign a texture unit
        // TODO: and issue a warning in case of conflict (?)
        //
        validateUniformTextureUnits(true);

//#ifdef USE_OLDPROGRAM
        if(m_pBaseStatesLayer->program && !m_pBaseStatesLayer->programPipeline)
            m_pBaseStatesLayer->program->unbind(m_container);
//#else
//        todo
//#endif
        //
        // TODO : manage the sub-routines in the pass-states
        //
    } //bHasProgram
    //
    // maintain the reference-user-counter of the used resources
    //
    //StatesLayerMap::iterator iSL = m_statesLayers.begin();
    //while(iSL != m_statesLayers.end())
    //{
    //    int layerID = iSL->first;
    //    StatesLayer &sl = iSL->second;
    //    it = sl.statesForExecution.begin();
    //    while(it != sl.statesForExecution.end())
    //    {
    //        PassState* pS = (*it);
    //        PassState::Type t = pS->getType();
    //        switch(t)
    //        {
    //        case PassState::TFBOTarget:
    //            {
    //                IFrameBufferObject* pFBO = pS->getFBOTarget();
    //                if(pFBO)
    //                {
    //                    LOGD("Pass %s needs resources from FBO %s for target\n", m_name.c_str(), pFBO->getName() );
    //                    Resource *pRes;
    //                    for(int i=0; pRes = static_cast<Resource*>(pFBO->getColorResource(i)); i++)
    //                    {
    //                        pRes->incUserCnt();
    //                    }
    //                    if(pRes = static_cast<Resource*>(pFBO->getDSTResource()))
    //                    {
    //                        pRes->incUserCnt();
    //                    }
    //                }
    //            }
    //            break;
    //        case PassState::TBlitFBOSrc:
    //            {
    //                IFrameBufferObject* pFBO = pS->getBlitFBOToActiveTarget();
    //                LOGD("Pass %s needs resources from FBO %s for blitting\n", m_name.c_str(), pFBO->getName() );
    //                Resource *pRes;
    //                for(int i=0; pRes = static_cast<Resource*>(pFBO->getColorResource(i)); i++)
    //                {
    //                    pRes->incUserCnt();
    //                }
    //                if(pRes = static_cast<Resource*>(pFBO->getDSTResource()))
    //                    pRes->incUserCnt();
    //            }
    //            break;
    //        case PassState::TUniform:
    //            {
    //                Uniform* pU = static_cast<Uniform*>(pS->getUniform());
    //                Uniform::ShadowedData* pData = pU->getShadowedData();
    //                if(pData->tex.pRes)
    //                {
    //                    LOGD("Pass %s needs resource %s as a texture\n", m_name.c_str(), pData->tex.pRes->getName() );
    //                    // at last: we can increase the counter of real users (users who need the resource to be allocated)
    //                    pData->tex.pRes->incUserCnt();
    //                }
    //            }
    //            break;
    //        case PassState::TSwapResource:
    //            assert(!"TODO: PassState::TSwapResource");
    //            break;
    //        }
    //        ++it;
    //    }
    //    ++iSL;
    //}
    if(bErr)
        m_validated &= ~VALIDATED_PASS;
    else
        m_validated |= VALIDATED_PASS;
    return m_validated;
}
/*************************************************************************/ /**
 **
 ** 
 **/ /*************************************************************************/ 
bool Pass::validateResources()
{
    bool bRes = true;
    if(m_validated != VALIDATED_PASS)
        return false;
    StateVec::iterator it;
    // Note: currently we will validate resources for *all the layers*
    // so if a Pass has many instances with different resources for each,
    // all might be validated.
    // QUESTION: do we really want to care so much about each layer ?
    StatesLayerMap::iterator iSL = m_statesLayers.begin();
    while(iSL != m_statesLayers.end())
    {
        int layerID = iSL->first;
        StatesLayer &sl = iSL->second;
        it = sl.statesForExecution.begin();
        while(it != sl.statesForExecution.end())
        {
            PassState* pS = (*it);
            PassState::Type t = pS->getType();
            switch(t)
            {
            case PassState::TFBOTarget:
                {
                    IFrameBufferObject* pFBO = pS->getFBOTarget();
                    if(pFBO)
                    {
                        LOGD("Pass %s (layerID %d) validating resources from FBO %s for target\n", m_name.c_str(), layerID, pFBO->getName() );
                        Resource *pRes;
                        for(int i=0; pRes = static_cast<Resource*>(pFBO->getColorResource(i)); i++)
                        {
                            if(pRes->incUserCnt() == 1)
                                if(!pRes->validate())
                                    bRes = false;
                        }
                        if(pRes = static_cast<Resource*>(pFBO->getDSTResource()))
                        {
                            if(pRes->incUserCnt() == 1)
                                if(!pRes->validate())
                                    bRes = false;
                        }
                        // maybe we need to re-validate the FBO
                        pFBO->getExInterface()->validate();
                    }
                }
                break;
            case PassState::TBlitFBOSrc:
                {
                    IFrameBufferObject* pFBO = pS->getBlitFBOToActiveTarget();
                    LOGD("Pass %s (layerID %d) validating resources from FBO %s for blitting\n", m_name.c_str(), layerID, pFBO->getName() );
                    Resource *pRes;
                    for(int i=0; pRes = static_cast<Resource*>(pFBO->getColorResource(i)); i++)
                    {
                        if(pRes->incUserCnt() == 1)
                            if(!pRes->validate())
                                bRes = false;
                    }
                    if(pRes = static_cast<Resource*>(pFBO->getDSTResource()))
                    {
                        if(pRes->incUserCnt() == 1)
                            if(!pRes->validate())
                                bRes = false;
                    }
                    // maybe we need to re-validate the FBO
                    pFBO->getExInterface()->validate();
                }
                break;
            case PassState::TUniform:
                {
                    Uniform* pU = static_cast<Uniform*>(pS->getUniform());
                    Uniform::ShadowedData* pData = pU->getShadowedData();
                    if(pData->tex.pRes)
                    {
                        LOGD("Pass %s (layerID %d) validating resource %s as a texture\n", m_name.c_str(), layerID, pData->tex.pRes->getName() );
                        // at last: we can increase the counter of real users (users who need the resource to be allocated)
                        if(pData->tex.pRes->incUserCnt() == 1)
                            if(!pData->tex.pRes->validate())
                                bRes = false;
                    }
                }
                break;
            case PassState::TSwapResource:
                assert(!"TODO: PassState::TSwapResource");
                break;
            }
            ++it;
        }
        ++iSL;
    }
    m_validated |= VALIDATED_RESOURCES;
    return bRes;
}
/*************************************************************************/ /**
 ** 
 ** 
 **/ /*************************************************************************/ 
bool Pass::invalidateResources()
{
    bool bRes = true;
    if(m_validated != (VALIDATED_PASS|VALIDATED_RESOURCES))
        return false;
    StateVec::iterator it;
    // Note: currently we will invalidate resources for *all the layers*
    // so if a Pass has many instances with different resources for each,
    // all might be validated.
    // QUESTION: do we really want to care so much about each layer ?
    StatesLayerMap::iterator iSL = m_statesLayers.begin();
    while(iSL != m_statesLayers.end())
    {
        int layerID = iSL->first;
        StatesLayer &sl = iSL->second;
        it = sl.statesForExecution.begin();
        while(it != sl.statesForExecution.end())
        {
            PassState* pS = (*it);
            PassState::Type t = pS->getType();
            switch(t)
            {
            case PassState::TFBOTarget:
                {
                    IFrameBufferObject* pFBO = pS->getFBOTarget();
                    if(pFBO)
                    {
                        bool bLostResource = false;
                        Resource *pRes;
                        LOGD("Pass %s (layerID %d) INvalidating resource from FBO %s (ID=%d) for target\n", m_name.c_str(), layerID, pFBO->getName(), pFBO->getID() );
                        for(int i=0; pRes = static_cast<Resource*>(pFBO->getColorResource(i)); i++)
                        {
                            if(pRes->decUserCnt() == 0)
                                if(pRes->invalidate())
                                    bLostResource = true;
                                else bRes = false;
                        }
                        if(pRes = static_cast<Resource*>(pFBO->getDSTResource()))
                        {
                            if(pRes->decUserCnt() == 0)
                                if(pRes->invalidate())
                                    bLostResource = true;
                                else bRes = false;
                        }
                        // if any of the related resource got discarded, invalidate the FBO
                        // reason: we did not add FBOs some userCnt mechanism. So we must bypass it
                        if(bLostResource)
                            pFBO->getExInterface()->invalidate();
                    }
                }
                break;
            case PassState::TBlitFBOSrc:
                {
                    IFrameBufferObject* pFBO = pS->getBlitFBOToActiveTarget();
                    LOGD("Pass %s (layerID %d) INvalidating resources from FBO %s (ID=%d)for blitting\n", m_name.c_str(), layerID, pFBO->getName(), pFBO->getID() );
                    bool bLostResource = false;
                    Resource *pRes;
                    for(int i=0; pRes = static_cast<Resource*>(pFBO->getColorResource(i)); i++)
                    {
                        if(pRes->decUserCnt() == 0)
                            if(pRes->invalidate())
                                bLostResource = true;
                            else bRes = false;
                    }
                    if(pRes = static_cast<Resource*>(pFBO->getDSTResource()))
                        if(pRes->decUserCnt() == 0)
                            if(pRes->invalidate())
                                bLostResource = true;
                            else bRes = false;
                    // if any of the related resource got discarded, invalidate the FBO
                    if(bLostResource)
                        pFBO->getExInterface()->invalidate();
                }
                break;
            case PassState::TUniform:
                {
                    Uniform* pU = static_cast<Uniform*>(pS->getUniform());
                    Uniform::ShadowedData* pData = pU->getShadowedData();
                    if(pData->tex.pRes)
                    {
                        LOGD("Pass %s (layerID %d) validating resource %s as a texture\n", m_name.c_str(), layerID, pData->tex.pRes->getName() );
                        // at last: we can increase the counter of real users (users who need the resource to be allocated)
                        if(pData->tex.pRes->decUserCnt() == 0 )
                            if(!pData->tex.pRes->invalidate())
                                bRes = false;
                    }
                }
                break;
            case PassState::TSwapResource:
                assert(!"TODO: PassState::TSwapResource");
                break;
            }
            ++it;
        }
        ++iSL;
    }
    //
    // We need to walk through the FBO repository to check if some must be invalidated
    // this is not so cool but the problem is that some resource might have been released
    // from a pass and now would make the FBO inconsistent if ever this same texture
    // was used for it. This FBO didn't get previously deleted certainly because none of its
    // resources user counter did reach 0...
    // Note that we have to put this call in the Pass::invalidateResources() because
    // one might call this method directly rather than using the Technique::invalidateResources()
    //
    FrameBufferObjectsRepository *pFBORep = static_cast<FrameBufferObjectsRepository *>(nvFX::getFrameBufferObjectsRepositorySingleton());
    pFBORep->updateValidated();

    m_validated &= ~VALIDATED_RESOURCES;
    return bRes;
}

/*************************************************************************/ /**
 ** 
 ** 
 **/ /*************************************************************************/ 
bool Pass::validateUniformTextureUnit(Uniform* pU, bool allowUnitAssignment, int layerID)
{
    bool bRes = true;
    if (NULL == pU)
      return false;
    StatesLayerMap::iterator it = m_statesLayers.begin();
    StatesLayerMap::iterator itEnd = m_statesLayers.end();
    if(layerID >= 0)
    {
        it = m_statesLayers.find(layerID);
        if(it == itEnd)
            return false;
        itEnd = it;
        ++itEnd;
    }
    while(it != itEnd)
    {
        StatesLayer& sl = it->second;
        switch(pU->getType())
        {
        case IUniform::TTexture:
        case IUniform::TTexture1D:
        case IUniform::TTexture2D:
        case IUniform::TTexture2DRect:
        case IUniform::TTexture3D:
        case IUniform::TTextureCube:
            if(pU->m_data && (pU->m_data->tex.textureUnit >= 0))
            {
				Uniform* pUInUnit = sl.textureUnits[pU->m_data->tex.textureUnit];
				// let's compare with the NAME. Because of the situation where the Pass can contain
				// an instance of a local uniform, used to assign things from the pass, as a pass-state
                if(pUInUnit	&& !(pUInUnit->m_name == pU->m_name) )
                {
                    LOGD("WARNING from Pass %s: unit #%d already used by %s. Conflicting with %s...\n", m_name.c_str(), pU->m_data->tex.textureUnit, sl.textureUnits[pU->m_data->tex.textureUnit]->getName(),pU->getName());
                    // Let's push to another level the unit... other passes could fail but will push again
                    // setting it to -1 will force the rest of the functio to find it a new location
                    pU->setSamplerUnit(-1);
                    break;
                } else if(pUInUnit == NULL) {
					LOGD("NOTE from Pass %s: %s now in unit #%d\n", m_name.c_str(), pU->getName(),pU->m_data->tex.textureUnit);
					sl.textureUnits[pU->m_data->tex.textureUnit] = pU;
				}
            }
            break;
        case IUniform::TTexUnit:
            CHECK_TRUE(false); //TODO
            break;
        default:
            CHECK_TRUE(false);
            break;
        }
        int availableTexUnit = 0;
        while(sl.textureUnits[availableTexUnit] && (availableTexUnit<32) )
        {
            if(availableTexUnit >= 32)
            {
                LOGE("ERROR from Pass %s: no more texture units available for %s !\n", m_name.c_str(), pU->getName() );
                bRes = false;
                break;
            }
            ++availableTexUnit;
        }
        switch(pU->getType())
        {
        case IUniform::TTexture:
        case IUniform::TTexture1D:
        case IUniform::TTexture2D:
        case IUniform::TTexture2DRect:
        case IUniform::TTexture3D:
        case IUniform::TTextureCube:
            if((pU->m_data==NULL)||(pU->m_data->tex.textureUnit < 0))
            {
                pU->setSamplerUnit(availableTexUnit);
                LOGD("NOTE from Pass %s: %s now in unit #%d\n",m_name.c_str(), pU->getName(),availableTexUnit);
                sl.textureUnits[availableTexUnit] = pU;
            }
            break;
        case IUniform::TTexUnit:
            CHECK_TRUE(false); //TODO
            break;
        default:
            CHECK_TRUE(false);
            break;
        }
        ++it;
    }
    return bRes;
}
/*************************************************************************/ /**
 ** 
 ** 
 **/ /*************************************************************************/ 
bool Pass::validateUniformTextureUnits(bool allowUnitAssignment)
{
    bool bRes = true;
    StatesLayerMap::iterator it = m_statesLayers.begin();
    while(it != m_statesLayers.end())
    {
        StatesLayer& sl = it->second;
        memset(sl.textureUnits, 0, sizeof(Uniform*)*32);
        //
        // walk through the execution states : the Uniform assignment
        // apply() the ones for samplers so that the destination gets updated to some default values
        //
        StateVec::iterator its = m_pBaseStatesLayer->statesForExecution.begin();
        while(its != m_pBaseStatesLayer->statesForExecution.end())
        {
            if(((*its)->getType() == IPassState::TUniform) )
            {
                IUniform *pU = (*its)->getUniform();
                switch(pU->getType())
                {
                case IUniform::TTexture:
                case IUniform::TTexture1D:
                case IUniform::TTexture2D:
                case IUniform::TTexture2DRect:
                case IUniform::TTexture3D:
                case IUniform::TTextureCube:
					validateUniformTextureUnit((*its)->m_uniformSrc, it->first);
                    (*its)->apply(true, NULL);
                    break;
                default:
                    break;
                }
            }
            ++its;
        }
        //
        // Now we can check more generally
        //
        UniformVec::iterator itu = sl.usedUniforms.begin();
        while(itu != sl.usedUniforms.end())
        {
            switch(itu->pU->getType())
            {
            case IUniform::TTexture:
            case IUniform::TTexture1D:
            case IUniform::TTexture2D:
            case IUniform::TTexture2DRect:
            case IUniform::TTexture3D:
            case IUniform::TTextureCube:
                if(itu->pU->m_data && (itu->pU->m_data->tex.textureUnit >= 0))
                {
					Uniform* pUInUnit = sl.textureUnits[itu->pU->m_data->tex.textureUnit];
					// let's compare with the NAME. Because of the situation where the Pass can contain
					// an instance of a local uniform, used to assign things from the pass, as a pass-state
                    if(pUInUnit	&& !(pUInUnit->m_name == itu->pU->m_name) )
                    {
                        LOGD("WARNING from pass %s: unit #%d already used by %s. %s might need another unit...\n",m_name.c_str(),itu->pU->m_data->tex.textureUnit, sl.textureUnits[itu->pU->m_data->tex.textureUnit]->getName(),itu->pU->getName());
                        // reset it so that later we'll assign another unit
                        itu->pU->setSamplerUnit(-1);
                        break;
                    } else if(pUInUnit == NULL) {
						LOGD("NOTE from pass %s: %s in unit #%d\n",m_name.c_str(), itu->pU->getName(),itu->pU->m_data->tex.textureUnit);
						sl.textureUnits[itu->pU->m_data->tex.textureUnit] = itu->pU;
					}
                }
                break;
            case IUniform::TTexUnit:
                CHECK_TRUE(false); //TODO
                break;
            default:
                break;
            }
            ++itu;
        }
        // Now let's assign units for those that need one.
        // this will *change* the uniform unit value !
        // Thus impacting other passes that use this uniform !
        int availableTexUnit = 0;
        itu = sl.usedUniforms.begin();
        while(itu != sl.usedUniforms.end())
        {
            while(sl.textureUnits[availableTexUnit] && (availableTexUnit<32) )
            {
                if(availableTexUnit >= 32)
                {
                    LOGE("ERROR from Pass %s: no more texture units available for %s !\n",m_name.c_str(), itu->pU->getName() );
                    bRes = false;
                    break;
                }
                ++availableTexUnit;
            }
            switch(itu->pU->getType())
            {
            case IUniform::TTexture:
            case IUniform::TTexture1D:
            case IUniform::TTexture2D:
            case IUniform::TTexture2DRect:
            case IUniform::TTexture3D:
            case IUniform::TTextureCube:
                if((itu->pU->m_data==NULL)||(itu->pU->m_data->tex.textureUnit < 0))
                {
                    LOGD("NOTE from pass %s: %s now in unit #%d\n",m_name.c_str(), itu->pU->getName(),availableTexUnit);
                    itu->pU->setSamplerUnit(availableTexUnit);
                    // we need to update again the uniform since the unit changed
                    itu->pU->update(itu->pU->m_data, this, it->first, true);
                    sl.textureUnits[availableTexUnit] = itu->pU;
                }
                break;
            case IUniform::TTexUnit:
                CHECK_TRUE(false); //TODO
                break;
            default:
                break;
            }
            ++itu;
        }
        ++it;
    }
    return bRes;
}

/*************************************************************************/ /**
 ** 
 ** 
 **/ /*************************************************************************/ 
#pragma MESSAGE("TODO: implement the use of flags in cancelInternalAction")
bool Pass::execute(PassInfo * pr, unsigned int cancelInternalAction)
{
    NXPROFILEFUNCCOL2(__FUNCTION__, COLOR_BLUE2, 2);
    static PassInfo passinfo;
    if(!pr)
    {
        memset(&passinfo, 0, sizeof(passinfo));
        pr = &passinfo;
        pr->loopCnt = m_loopCnt;
    }
    //
    // Set some values back to default state
    //
    pr->flags = 0;
    pr->blitSourceFBO = NULL;
    //pr->currentFBO = NULL;
    pr->clearMode = Clear_none;
    pr->overrideID = m_overrideID;
#pragma MESSAGE("TODO : define MAX_RENDERTARGETS and replace 4 by MAX_RENDERTARGETS in the code...")
    memset(pr->clearColorValid, 0, sizeof(bool)*4/*MAX_RENDERTARGETS*/);

    if(!(m_validated&VALIDATED_PASS))
    {
        nvFX::printf("Warning : Pass %s must be validated before executing it\n", m_name.c_str());
        if(!validate()) // commented : we should just fail... up to the app to validate() if needed
          return false;
    }
    // decrement the loop counter if possible. But... Shall we do it here ?
    pr->loopCnt--;
    // --------------------------------------------------------------------
    // Bind the program if exists
    //
//#ifndef USE_OLDPROGRAM
    if(m_pActiveStatesLayer->programPipeline)
    {
        if(!m_pActiveStatesLayer->programPipeline->bind())
        {
            nvFX::printf("Encountered errors during execution of Pass %s (prog. pipeline binding())\n", m_name.c_str());
            return false;
        }
        pr->flags |= PR_SHADERS;
        if(m_pActiveStatesLayer->programPipeline->m_shaderFlags & FX_TESS_CONTROL_SHADER_BIT)
            pr->flags |= PR_TESSSHADERS;
    } else // give a chance to the alternate old solution
//#endif
    if(m_pActiveStatesLayer->program)
    {
        pr->flags |= PR_SHADERS;
        if(!m_pActiveStatesLayer->program->bind(m_container))
        {
            nvFX::printf("Encountered errors during execution of Pass %s (program binding)\n", m_name.c_str());
            return false;
        }
    }
    // ---------------------------------------------------------------------
    // Execute states
    // Note: we could do a more efficient job than this...
    //
    //m_pBaseStatesLayer : the base layer in which most of the execution states reside
    StateVec::iterator it = m_pBaseStatesLayer->statesForExecution.begin();
    while(it != m_pBaseStatesLayer->statesForExecution.end())
    {
        //#pragma MESSAGE("TODO TODO TODO TODO TODO TODO : make a faster solution for uniform def. values update through special pass")
        (*it)->apply(true, pr);
        ++it;
    }
    //m_pActiveStatesLayer : it could contain some overridden pass-states (sampler uniform...)
    // NOTE: another solution could be to have the statelayer copy the baselayer's states, to avoid this second loop...
    if(m_pActiveStatesLayer != m_pBaseStatesLayer)
    {
        StateVec::iterator it = m_pActiveStatesLayer->statesForExecution.begin();
        while(it != m_pActiveStatesLayer->statesForExecution.end())
        {
            //#pragma MESSAGE("TODO TODO TODO TODO TODO TODO : make a faster solution for uniform def. values update through special pass")
            (*it)->apply(true, pr);
            ++it;
        }
    }
    // -----------------------------------------------------------------------
    // update uniforms if needed : only for this pass and no binding necessary
    // we focus the active state layer ID, this time : to target the right shaders (program)
    //
    // Used uniforms (and that are not in pass-state list)
    UniformVec::iterator itu = m_pActiveStatesLayer->usedUniforms.begin();
    while(itu != m_pActiveStatesLayer->usedUniforms.end())
    {
        UniformAndTarget &ut = *itu;
        Uniform::STarget  &t = ut.pU->m_targets[ut.target];
        if(ut.pU->m_data)
        {
            if(t.dirty)
            {
                // when dirty, the special case of uniform-sampler requires to update
                // how passes do maintain texture units (for the automatic assignment)
                switch(ut.pU->getType())
                {
                case IUniform::TTexture:
                case IUniform::TTexture1D:
                case IUniform::TTexture2D:
                case IUniform::TTexture2DRect:
                case IUniform::TTexture3D:
                case IUniform::TTextureCube:
                  {
                    int u = ut.pU->m_data ? ut.pU->m_data->tex.textureUnit : -1;
                    if(ut.pU->m_data && (u >= 0) )
                      m_pActiveStatesLayer->textureUnits[u] = ut.pU;
                  }
                  break;
                default:
                    break;
                }
                ut.pU->updateForTarget(ut.pU->m_data, ut.target);
            } else // Same as when dirty... because this doesn't make it for now
                ut.pU->updateForTarget(ut.pU->m_data, ut.target);
        }
        ++itu;
    }
    // -----------------------------------------------------------------------
    // Used Constant Buffers
	//
    CstBufferVec::iterator itc = m_pActiveStatesLayer->usedCstBuffers.begin();
    while(itc != m_pActiveStatesLayer->usedCstBuffers.end())
    {
        CstBufferAndTarget &ut = *itc;
        ut.pC->updateForTarget(ut.target);
        ++itc;
    }
    // -----------------------------------------------------------------------
    // Used samplerstates
	//
    SamplerStateVec::iterator its = m_pActiveStatesLayer->usedSamplerStates.begin();
    while(its != m_pActiveStatesLayer->usedSamplerStates.end())
    {
        SamplerStateAndTarget &ut = *its;
        ut.pS->updateForTarget(ut.pS->m_data, ut.target);
        ++its;
    }
    // ================================================================
    // Now let's take few actions especially for the scene-level effect
    // ================================================================
    FrameBufferObjectsRepository* pRep = 
        static_cast<FrameBufferObjectsRepository*>(nvFX::getFrameBufferObjectsRepositorySingleton());
    bool bRes = true;
    if(pr->flags & PR_CURRENTFBO)
    {
        // NOTE: canceled this test for now. This is tricky : we may not be able to rely on this to
        // decide to optimize the binding.
        //if(pr->prevFBO != pr->currentFBO)
        {
            pRep->setCurrent(pr->currentFBO, ((pr->flags & PR_VIEWPORT)));
        }
        pr->prevFBO = pr->currentFBO;
    }
    if(pr->flags & PR_SWAPRESOURCES)
    {
        FrameBufferObjectsRepository::swapResources(pr->swapResourceFBOs[0], pr->swapResourceFBOs[1]);
    }
    if(pr->flags & PR_VIEWPORT)
        setViewPort(pr->viewport, pr->viewportZ);
    // TODO ? shall we try to avoid redundant changes of clear color when possible ?
    if(pr->flags & PR_CLEARMODE)
    {
        clearRenderTargets(pr);
    }
    if(pr->flags & PR_BLITSOURCEFBO)
    {
        bRes = pRep->blit(pr->currentFBO, pr->blitSourceFBO);
    }
    switch(pr->renderingMode)
    {
    case RENDER_FULLSCREEN_QUAD:
        drawFullscreenQuad();
        break;
    case RENDER_DEBUG_QUAD0:
        drawFullscreenQuad(0.8f, -0.9f, -0.9f);
        break;
    case RENDER_DEBUG_QUAD1:
        drawFullscreenQuad(0.8f, 0.1f, -0.9f);
        break;
    case RENDER_DEBUG_QUAD2:
        drawFullscreenQuad(0.8f, -0.9f, 0.1f);
        break;
    case RENDER_DEBUG_QUAD3:
        drawFullscreenQuad(0.8f, 0.1f, 0.1f);
        break;
    case RENDER_STENCILFILLPATH:
    case RENDER_STENCILSTROKEPATH:
    case RENDER_COVERFILLPATH:
    case RENDER_COVERSTROKEPATH:
        // program in this case can execute vector commands
        bRes = m_pActiveStatesLayer->program->execute(pr->renderingMode, &(pr->pathRenderingSettings));
        break;
    }
#ifdef USECUDA
    //
    // Execute the CUDA Kernel
    //
    //if(pr->flags & PR_CUDA) // previous way to do
    if(m_pActiveStatesLayer->kernelEntry >= 0)
    {
        pr->flags &= PR_CUDA;
        // maybe pr->kernelEntry could be avoided : by storing this ID directly into the Pass (m_pActiveStatesLayer)
        m_pActiveStatesLayer->programCUDA->executeKernelEntry(m_pActiveStatesLayer->kernelEntry);
        // CUDA required that we map the resources for access in the kernel
        // let's release these resources (unmap)
        UniformVec::iterator itr = m_pActiveStatesLayer->usedUniforms.begin();
        while(itr != m_pActiveStatesLayer->usedUniforms.end())
        {
            Uniform* pU = itr->pU;
            switch(pU->getType())
            {
            case IUniform::TTexture:
            case IUniform::TTexture1D:
            case IUniform::TTexture2D:
            case IUniform::TTexture2DRect:
            case IUniform::TTexture3D:
            case IUniform::TTextureCube:
              {
                if(pU->m_data && pU->m_data->tex.pRes)
                    pU->m_data->tex.pRes->unmapFromCUDA();
              }
              break;
            }
            ++itr;
        }
        if(m_pBaseStatesLayer != m_pActiveStatesLayer)
        {
            itr = m_pBaseStatesLayer->usedUniforms.begin();
            while(itr != m_pBaseStatesLayer->usedUniforms.end())
            {
                Uniform* pU = itr->pU;
                switch(pU->getType())
                {
                case IUniform::TTexture:
                case IUniform::TTexture1D:
                case IUniform::TTexture2D:
                case IUniform::TTexture2DRect:
                case IUniform::TTexture3D:
                case IUniform::TTextureCube:
                  {
                    if(pU->m_data && pU->m_data->tex.pRes)
                        pU->m_data->tex.pRes->unmapFromCUDA();
                  }
                  break;
                }
                ++itr;
            }
        }
    }
#endif
    //
    // Execute Compute
    //
    //if(pr->flags & PR_COMPUTE) // we assume the previous actions did set correctly this flag
    // For now let's rule that -1 would mean no use of compute and >= means compute used
    // When group size == 0, we must compute the Group size according to a render target... TODO
    if(m_GLSLComputeGroupSize[0]>=0)
    {
CHECK_TRUE(m_GLSLComputeGroupSize[0]>0); // for now...
        // TODO ? Shall we put the Group Size in pr ? In the case of "cancelInternalAction" ?
        // pr->ComputeGroupSz[0] = m_GLSLComputeGroupSize[0];
        // pr->ComputeGroupSz[1] = m_GLSLComputeGroupSize[1];
        // pr->ComputeGroupSz[2] = m_GLSLComputeGroupSize[2];
        //m_pBaseStatesLayer->programPipeline->execute();
        pr->flags &= PR_COMPUTE;
        // program in this case MUST have the compute program (the only one)
        bRes = m_pActiveStatesLayer->program->execute(
            m_GLSLComputeGroupSize[0], m_GLSLComputeGroupSize[1], m_GLSLComputeGroupSize[2]);
    }
    return bRes;
}

/*************************************************************************/ /**
 ** \brief update a specific uniform
 ** 
 **/ /*************************************************************************/ 
void Pass::unbindProgram()
{
    if(m_pBaseStatesLayer->programPipeline)
        m_pBaseStatesLayer->programPipeline->unbind();
    else if(m_pBaseStatesLayer->program)
        m_pBaseStatesLayer->program->unbind(m_container);
}
/*************************************************************************/ /**
 ** \brief update a specific uniform
 ** 
 **/ /*************************************************************************/ 
bool Pass::updateUniform(Uniform *uniform)
{
    nvFX::printf("updateuniform not implemented\n");
    return false;
}

/*************************************************************************/ /**
 ** NOTE: sorry but now this name allows to either find back 
 ** - a uniform variable assigment made in the pass
 ** - a named group for shaders
 ** both are very different but both use the name of the pass-state as the storage
 ** not really consistent but working for now...
 ** TODO: change this to a decent way.by adding a special method :
 ** - findStateGroup(groupname)
 ** - findState(name)
 **
 ** \note for now, we only search in the base layer
 ** 
 **/ /*************************************************************************/ 
IPassState* Pass::findState(const char *name, int layerID)
{
    NXPROFILEFUNCCOL2(__FUNCTION__, COLOR_BLUE2, 2);
    if(!name)
        return NULL;
    StatesLayer* pSL = m_pActiveStatesLayer;
    if(layerID == 0)
        pSL = m_pBaseStatesLayer;
    else if(layerID < 0)
        pSL = m_pActiveStatesLayer;
    else {
        StatesLayerMap::iterator iSL = m_statesLayers.find(layerID);
        if(iSL == m_statesLayers.end())
            return NULL;
        pSL = &(iSL->second);
    }

    StateVec::iterator it = pSL->statesForValidation.begin();
    while(it != pSL->statesForValidation.end())
    {
        if(!strcmp((*it)->getName(), name))
            return *it;
        ++it;
    }
    it = pSL->statesForExecution.begin();
    while(it != pSL->statesForExecution.end())
    {
        if(!strcmp((*it)->getName(), name))
            return *it;
        ++it;
    }
    return NULL;
}

/*************************************************************************/ /**
 ** 
 **/ /*************************************************************************/ 
IPassState* Pass::findStateOverride(const char *name)
{
    NXPROFILEFUNCCOL2(__FUNCTION__, COLOR_BLUE2, 2);
    PassState *p = NULL;
    for(int i=0; i<(int)m_statesOverride.size(); i++)
    {
        if(!strcmp(m_statesOverride[i]->getName(), name))
        {
            p = m_statesOverride[i];
            break;
        }
    }
    return p;
}

/*************************************************************************/ /**
 ** \arg \e groupName is optional and allows you to group shaders.
 ** this can help to find them back easily and for example replace them at runtime.
 ** typical example : when one wants to replace at runtime some lighting implementation
 ** 
 **/ /*************************************************************************/ 
IPassState* Pass::createStateOverride(IPassState::Type type, IShader *shd, const char *groupName)
{
   return  createStateOverride(type, &shd, 1, groupName);
}
/*************************************************************************/ /**
 ** \arg \b groupName is optional and allows you to group shaders.
 ** this can help to find them back easily and for example replace them at runtime.
 ** typical example : when one wants to replace at runtime some lighting implementation
 **
 **/ /*************************************************************************/ 
IPassState* Pass::createStateOverride(IPassState::Type type, IShader **shd, int numShaders, const char *groupName)
{
    if(m_overrideID == 0)
        getID(&m_overrideID, 1);
    PassState *p = NULL;
    if(groupName == NULL)
        groupName = "";
    // NOTE: I know this is not clean : the group name here suddently becomes the state name
    // I should have a special field for this. For now this is good enough but not really
    // consistent.
    for(int i=0; i<(int)m_statesOverride.size(); i++)
    {
        if(!strcmp(m_statesOverride[i]->getName(), groupName))
        {
            p = m_statesOverride[i];
            break;
        }
    }
    if(p && (p->getType() != type))
        return NULL;
    if(!p)
    {
        p = new PassState(m_container, this, groupName);
        m_statesOverride.push_back(p);
    }
    if(!p->setShaders(type, shd, numShaders))
    {
        CHECK_TRUE(false);
        nvFX::printf("Error>>Wrong type for shader assigment\n");
        return NULL;
    }
    return p;
}
/*************************************************************************/ /**
 ** 
 **/ /*************************************************************************/ 
IPassState*      Pass::createStateSamplerResourceOverride(const char *varName, IResource *pRes, int texUnit)
{
    PassState *p = new PassState(m_container, this, varName); // name it the same as the uniform it relates to
    p->setSamplerResource(varName, pRes, texUnit);
    m_statesOverride.push_back(p);
    return p;
}
/*************************************************************************/ /**
 ** 
 **/ /*************************************************************************/ 
IPassState*      Pass::createStateTextureResourceOverride(const char *varName, IResource *pRes)
{
    PassState *p = new PassState(m_container, this, varName); // name it the same as the uniform it relates to
    p->setTextureResource(varName, pRes);
    m_statesOverride.push_back(p);
    return p;
}
/*************************************************************************/ /**
 ** 
 **/ /*************************************************************************/ 
IPassState*      Pass::createStateImageResourceOverride(const char *varName, IResource *pRes, int unit)
{
    PassState *p = new PassState(m_container, this, varName); // name it the same as the uniform it relates to
    p->setImageResource(varName, pRes, unit);
    m_statesOverride.push_back(p);
    return p;
}
/*************************************************************************/ /**
 ** 
 **/ /*************************************************************************/ 
IPassState*      Pass::createStateSamplerStateOverride(const char *varName, ISamplerState * pSState)
{
    PassState *p = new PassState(m_container, this, varName);
    p->setSamplerState(varName, pSState);
    m_statesOverride.push_back(p);
    return p;
}
/*************************************************************************/ /**
 ** \arg \e groupName is optional and allows you to group shaders.
 ** this can help to find them back easily and for example replace them at runtime.
 ** typical example : when one wants to replace at runtime some lighting implementation
 ** 
 **/ /*************************************************************************/ 
IPassState* Pass::createState(IPassState::Type type, IShader *shd, const char *groupName, bool bUseActiveLayer)
{
    StatesLayer* pLayer = bUseActiveLayer ? m_pActiveStatesLayer : m_pBaseStatesLayer;
    // creates a state layer layerID if doesn't exist
//#ifdef USE_OLDPROGRAM
    if(pLayer->program && !pLayer->programPipeline)
    {
        delete pLayer->program;
        pLayer->program = NULL;
    }
//#endif
    m_validated &= ~VALIDATED_PASS;
    PassState *p = NULL;
    // NOTE: I know this is not clean : the group name here suddently becomes the state name
    // I should have a special field for this. For now this is good enough but not really
    // consistent.
    if(groupName)
        p = static_cast<PassState*>(findState(groupName, -1));
    if(p && (p->getType() != type))
        return NULL;
    if(!p)
    {
        p = new PassState(m_container, this, groupName);
        pLayer->statesForValidation.push_back(p);
    }
    if(!p->setShaders(type, &shd, 1))
    {
        CHECK_TRUE(false);
        nvFX::printf("Error>>Wrong type for shader assigment\n");
        return NULL;
    }
    // let's invalidate the uniforms for this pass, if ever some kept a reference to the pass
    // the reason: we need to re calculate uniform locations since the new shaders
    // may have modified these offsets
    m_container->invalidateTargets(this, bUseActiveLayer ? m_activeStatesLayer : 0);
    return p;
}
/*************************************************************************/ /**
 ** \arg \b groupName is optional and allows you to group shaders.
 ** this can help to find them back easily and for example replace them at runtime.
 ** typical example : when one wants to replace at runtime some lighting implementation
 **
 ** \arg bool bUseActiveLayer : when true, it will use m_pActiveStatesLayer. false: base layer
 **
 **/ /*************************************************************************/ 
IPassState* Pass::createState(IPassState::Type type, IShader **shd, int numShaders, const char *groupName, bool bUseActiveLayer)
{
    StatesLayer* pLayer = bUseActiveLayer ? m_pActiveStatesLayer : m_pBaseStatesLayer;
    m_validated &= ~VALIDATED_PASS;
//#ifdef USE_OLDPROGRAM
    if(pLayer->program  && !pLayer->programPipeline)
    {
        delete pLayer->program;
        pLayer->program = NULL;
    }
//#endif
    PassState *p = NULL;
    if(numShaders < 1)
        return NULL;
    // NOTE: I know this is not clean : the group name here suddently becomes the state name
    // I should have a special field for this. For now this is good enough but not really
    // consistent.
    if(groupName)
        p = static_cast<PassState*>(findState(groupName, -1));
	if(p) // if already existing: let's return NULL : we can't create it a second time
		return NULL;
    if(p && (p->getType() != type))
        return NULL;
    if(!p)
    {
        p = new PassState(m_container, this, groupName);
        pLayer->statesForValidation.push_back(p);
    }
    if(!p->setShaders(type, shd, numShaders))
    {
        CHECK_TRUE(false);
        nvFX::printf("Error>>Wrong type for shader assigment\n");
        return NULL;
    }
    // let's invalidate the uniforms for this pass, if ever some kept a reference to the pass
    // the reason: we need to re calculate uniform locations since the new shaders
    // may have modified these offsets
    m_container->invalidateTargets(this, bUseActiveLayer ? m_activeStatesLayer : 0);
    return p;
}
/*************************************************************************/ /**
 ** 
 ** 
 **/ /*************************************************************************/ 
#ifndef OGLES2
IPassState* Pass::createState(const char *srcVarName, int idx, const char *dstVarName)
{
    // creates a state layer 0 if doesn't exist
    m_validated &= ~VALIDATED_PASS;
    PassState *p = NULL;
    // first, look for an existing state that would already assign data to this variable
    StateVec::iterator it = m_pBaseStatesLayer->statesForExecution.begin();
    while(it != m_pBaseStatesLayer->statesForExecution.end())
    {
        if((*it)->getType() == IPassState::TSubRoutineUniform)
        {
            // uniform must be there in any case...
            if(!strcmp((*it)->getUniform()->getName(), srcVarName))
            {
                p = *it;
                break;
            }
        }
        ++it;
    }
    if(!p)
    {
        // I know... this may require change : I am assigning here the srcVarName to the state
        // for the case of uniform assignment. This will allow to find them back with
        // findState(groupName)... which is not very clean at all... I know
        // OR the opposite : what is not clean is to use this name for the group-name...
        // TODO: resolve this
        p = new PassState(m_container, this, srcVarName);
        m_pBaseStatesLayer->statesForExecution.push_back(p);
    }
    p->setUniform(srcVarName, idx, dstVarName);
    return p;
}

/*************************************************************************/ /**
 ** 
 ** 
 **/ /*************************************************************************/ 
IPassState* Pass::createState(const char *srcVarName, const char **dstVarNames, int numNames)
{
    m_validated &= ~VALIDATED_PASS;
    PassState *p = NULL;
    // first, look for an existing state that would already assign data to this variable
    StateVec::iterator it = m_pBaseStatesLayer->statesForExecution.begin();
    while(it != m_pBaseStatesLayer->statesForExecution.end())
    {
        if((*it)->getType() == IPassState::TSubRoutineUniform)
        {
            // uniform must be there in any case...
            if(!strcmp((*it)->getUniform()->getName(), srcVarName))
            {
                p = *it;
                break;
            }
        }
        ++it;
    }
    if(!p)
    {
        // I know... this may require change : I am assigning here the srcVarName to the state
        // for the case of uniform assignment. This will allow to find them back with
        // findState(groupName)... which is not very clean at all... I know
        // OR the opposite : what is not clean is to use this name for the group-name...
        // TODO: resolve this
        p = new PassState(m_container, this, srcVarName);
        m_pBaseStatesLayer->statesForExecution.push_back(p);
    }
    p->setUniform(srcVarName, dstVarNames, numNames);
    return p;
}
#endif
/*************************************************************************/ /**
 ** 
 ** 
 **/ /*************************************************************************/ 
IPassState* Pass::createState(const char *varName, int idx, float *pF, int nComps)
{
    m_validated &= ~VALIDATED_PASS;
    // first, look for an existing state that would already assign data to this variable
    StateVec::iterator it = m_pBaseStatesLayer->statesForExecution.begin();
    PassState* p = new PassState(m_container, this);
    p->setUniform(varName, idx, pF, nComps);
    m_pBaseStatesLayer->statesForExecution.push_back(p);
    return p;
}

/*************************************************************************/ /**
 ** 
 ** 
 **/ /*************************************************************************/ 
IPassState* Pass::createState(const char *varName, int idx, int *pI, int nComps)
{
    m_validated &= ~VALIDATED_PASS;
    PassState *p = new PassState(m_container, this);
    p->setUniform(varName, idx, pI, nComps);
    m_pBaseStatesLayer->statesForExecution.push_back(p);
    return p;
}

/*************************************************************************/ /**
 ** TODO
 ** 
 **/ /*************************************************************************/ 
IPassState* Pass::createState(const char *varName, int idx, float *pF, int nComps, int nVecs)
{
    m_validated &= ~VALIDATED_PASS;
    PassState *p = new PassState(m_container, this);
    p->setUniform(varName, idx, pF, nComps, nVecs);
    m_pBaseStatesLayer->statesForExecution.push_back(p);
    return p;
}
/*************************************************************************/ /**
 ** TODO
 ** 
 **/ /*************************************************************************/ 
IPassState* Pass::createState(const char *varName, int idx, int *pI, int nComps, int nVecs)
{
    m_validated &= ~VALIDATED_PASS;
    PassState *p = new PassState(m_container, this);
    p->setUniform(varName, idx, pI, nComps, nVecs);
    m_pBaseStatesLayer->statesForExecution.push_back(p);
    return p;
}
/*************************************************************************/ /**
 ** TODO
 ** 
 **/ /*************************************************************************/ 
IPassState* Pass::createState(IStateGroupRaster *pSGRaster)
{
    m_validated &= ~VALIDATED_PASS;
    PassState *p = new PassState(m_container, this);
    p->setStateGroup(pSGRaster);
    m_pBaseStatesLayer->statesForExecution.push_back(p);
    return p;
}
/*************************************************************************/ /**
 ** 
 ** 
 **/ /*************************************************************************/ 
IPassState* Pass::createState(IStateGroupCS *pSGCS)
{
    m_validated &= ~VALIDATED_PASS;
    PassState *p = new PassState(m_container, this);
    p->setStateGroup(pSGCS);
    m_pBaseStatesLayer->statesForExecution.push_back(p);
    return p;
}
/*************************************************************************/ /**
 ** 
 ** 
 **/ /*************************************************************************/ 
IPassState* Pass::createState(IStateGroupDST *pSGDST)
{
    m_validated &= ~VALIDATED_PASS;
    PassState *p = new PassState(m_container, this);
    p->setStateGroup(pSGDST);
    m_pBaseStatesLayer->statesForExecution.push_back(p);
    return p;
}
#ifndef OGLES2
/*************************************************************************/ /**
 ** 
 ** 
 **/ /*************************************************************************/ 
IPassState* Pass::createState(IStateGroupPath *pSGPR)
{
    m_validated &= ~VALIDATED_PASS;
    PassState *p = new PassState(m_container, this);
    if (NULL!=p)
    {
    p->setStateGroup(pSGPR);
    m_pBaseStatesLayer->statesForExecution.push_back(p);
    }
    return p;
}
#endif
/*************************************************************************/ /**
 ** 
 **/ /*************************************************************************/ 
IPassState*      Pass::createStateFromState(IPassState* pState)
{
    if (NULL == pState)
      return NULL;
    m_validated &= ~VALIDATED_PASS;
    switch(pState->getType())
    {
    case IPassState::TUniform:
        break;
    default:
        CHECK_TRUE(false);
        LOGI("ERROR: Not implemented (yet). Cannot copy this pass-state...\n");
        return NULL;
    }
    PassState *p = new PassState(m_container, this, static_cast<PassState*>(pState));
    // NOTE: if we want this method to be generalized, some pass-states might be for
    // statesForExecution, while others for statesForValidation !
    // So improving this method must deal with this detail...
    m_pActiveStatesLayer->statesForExecution.push_back(p);
    return p;
}
/*************************************************************************/ /**
 ** 
 ** 
 **/ /*************************************************************************/ 
bool Pass::destroy(IPassState* p)
{
    m_validated &= ~VALIDATED_PASS;
    if (NULL == p)
      return false;
    StatesLayerMap::iterator iSL = m_statesLayers.begin();
    while(iSL != m_statesLayers.end())
    {
        StatesLayer &sl = iSL->second;
        for(int i=0; i<2; i++)
        {
            StateVec::iterator it = i == 0 ? sl.statesForValidation.begin() : sl.statesForExecution.begin();
            StateVec::iterator itEnd = i == 0 ? sl.statesForValidation.end() : sl.statesForExecution.end();
            while(it != itEnd)
            {
                if(*it == p)
                {
                    switch(p->getType())
                    {
                    case PassState::TFragmentShader:
                    case PassState::TVertexShader:
                      {
                        int refs = 1;
        #ifndef USE_OLDPROGRAM
                        if(sl.programPipeline)
                        {
                            Program *prog = static_cast<Program*>(sl.programPipeline->removeProgramShader(asShaderFlag(p->getType())) );
                            ShaderModuleRepository* pShdRep = static_cast<ShaderModuleRepository*>(nvFX::getShaderModuleRepositorySingleton());
                            pShdRep->releaseProgram(prog);
                            refs = prog->releaseTarget(this, iSL->first);
                        } else 
        #endif
                        if(sl.program)
                        {
                            ShaderModuleRepository* pShdRep = static_cast<ShaderModuleRepository*>(nvFX::getShaderModuleRepositorySingleton());
                            pShdRep->releaseProgram(sl.program);
                            refs = sl.program->releaseTarget(this, iSL->first);
                        }
                        sl.program = NULL;
                        // Invalidate the uniform targets so that we don't update uniforms of a pass that may not be ready yet
                        m_container->invalidateTargets(this, iSL->first);
                      }
                      break;
                    }
                    delete *it;
                    if(i==0) sl.statesForValidation.erase(it);
                    else     sl.statesForExecution.erase(it);
                    break;
                }
                ++it;
            }
        }
        ++iSL;
    }
    nvFX::printf("Pass::destroy : pass state not found\n");
    return false;
}

/*************************************************************************/ /**
 ** 
 ** 
 **/ /*************************************************************************/ 
IPassState* Pass::createStateSamplerResource(const char *varName, IResource *pRes, int texUnit)
{
    m_validated &= ~VALIDATED_PASS;
    PassState *p = new PassState(m_container, this, varName); // name it the same as the uniform it relates to
    p->setSamplerResource(varName, pRes, texUnit);
    m_pBaseStatesLayer->statesForExecution.push_back(p);
    return p;
}

/*************************************************************************/ /**
 ** 
 ** 
 **/ /*************************************************************************/ 
IPassState* Pass::createStateTextureResource(const char *varName, IResource *pRes)
{
    m_validated &= ~VALIDATED_PASS;
    PassState *p = new PassState(m_container, this, varName); // name it the same as the uniform it relates to
    p->setTextureResource(varName, pRes);
    m_pBaseStatesLayer->statesForExecution.push_back(p);
    return p;
}

/*************************************************************************/ /**
 ** 
 ** 
 **/ /*************************************************************************/ 
IPassState* Pass::createStateImageResource(const char *varName, IResource *pRes, int unit)
{
    m_validated &= ~VALIDATED_PASS;
    PassState *p = new PassState(m_container, this, varName); // name it the same as the uniform it relates to
    p->setImageResource(varName, pRes, unit);
    m_pBaseStatesLayer->statesForExecution.push_back(p);
    return p;
}

/*************************************************************************/ /**
 ** 
 ** 
 **/ /*************************************************************************/ 
IPassState* Pass::createStateSamplerState(const char *varName, ISamplerState * pSState)
{
    m_validated &= ~VALIDATED_PASS;
    PassState *p = new PassState(m_container, this, varName);
    p->setSamplerState(varName, pSState);
    m_pBaseStatesLayer->statesForExecution.push_back(p);
    return p;
}

/*************************************************************************/ /**
 ** 
 ** 
 **/ /*************************************************************************/ 
IPassState*            Pass::createStateRenderMode(RenderingMode rmode)
{
    m_validated &= ~VALIDATED_PASS;
    PassState *p = new PassState(m_container, this);
    p->setRenderingMode(rmode);
    m_pBaseStatesLayer->statesForExecution.push_back(p);
    return p;
}
IPassState*            Pass::createStateRenderGroup(int renderGroup)
{
    m_validated &= ~VALIDATED_PASS;
    PassState *p = new PassState(m_container, this);
    p->setRenderGroup(renderGroup);
    m_pBaseStatesLayer->statesForExecution.push_back(p);
    return p;
}
IPassState*            Pass::createStateClearMode(ClearMode cmode)
{
    m_validated &= ~VALIDATED_PASS;
    PassState *p = new PassState(m_container, this);
    p->setClearMode(cmode);
    m_pBaseStatesLayer->statesForExecution.push_back(p);
    return p;
}
IPassState*      Pass::createStateClearColor(int i, float r, float g, float b, float a)
{
    m_validated &= ~VALIDATED_PASS;
    PassState *p = new PassState(m_container, this);
    p->setClearColor(i,r,g,b,a);
    m_pBaseStatesLayer->statesForExecution.push_back(p);
    return p;
}

IPassState*            Pass::createStateRenderTarget(IFrameBufferObject *p)
{
    m_validated &= ~VALIDATED_PASS;
    PassState *pPS = new PassState(m_container, this);
    pPS->setRenderTarget(p);
    m_pBaseStatesLayer->statesForExecution.push_back(pPS);
    return pPS;
}
IPassState*            Pass::createStateBlitFBOToActiveTarget(IFrameBufferObject *p)
{
    m_validated &= ~VALIDATED_PASS;
    PassState *pPS = new PassState(m_container, this);
    pPS->setBlitFBOToActiveTarget(p);
    m_pBaseStatesLayer->statesForExecution.push_back(pPS);
    return pPS;
}

IPassState*            Pass::createStateViewport(int x, int y, int w, int h, float minZ, float maxZ)
{
    m_validated &= ~VALIDATED_PASS;
    PassState *pPS = new PassState(m_container, this);
    pPS->setViewport(x,y,w,h, minZ, maxZ);
    m_pBaseStatesLayer->statesForExecution.push_back(pPS);
    return pPS;
}
IPassState*      Pass::createStateSwapResources(IFrameBufferObject *p1, IFrameBufferObject *p2)
{
    m_validated &= ~VALIDATED_PASS;
    PassState *pPS = new PassState(m_container, this);
    pPS->setSwapResources(p1, p2);
    m_pBaseStatesLayer->statesForExecution.push_back(pPS);
    return pPS;
}

/*************************************************************************/ /**
 ** 
 ** 
 **/ /*************************************************************************/ 
bool            Pass::bindAttribute(const char * name, int unit)
{
    // TODO: check if valid ? if the unit was already used ?
    // later we want to also raise warnings if the names aren't in the shader...
    m_attributes[unit] = std::string(name);
    return true;
}
/*************************************************************************/ /**
 ** 
 ** 
 **/ /*************************************************************************/ 
bool Pass::bindAttributes()
{
    return bindAttributes(m_attributes);
}
bool Pass::bindAttributes(AttribMap &attributes)
{
    bool bDidBindLocation = false;
    StatesLayerMap::iterator iSL = m_statesLayers.begin();
    while(iSL != m_statesLayers.end())
    {
        int p=0;
        StatesLayer &sl = iSL->second;
        Program*           program = sl.program;
        if(sl.programPipeline)
            program = static_cast<Program*>(sl.programPipeline->getShaderProgram(p++));
        // Note: only the program having vertex shaders should be affected... but we still loop into all...
        // TODO: optimize to only run through programs with vtx shader ??
        while(program)
        {
            if(attributes.size() == 0)
                return true;
            // TODO: check if valid ? if the unit was already used ?
            // later we want to also raise warnings if the names aren't in the shader...
            AttribMap::iterator it = attributes.begin();
            while(it != attributes.end() )
            {
                int a = program->getAttribLocation(it->second.c_str());
                if(a >= 0)
                {
                    bDidBindLocation = true;
                    program->bindAttribLocation(it->first, it->second.c_str());
                }
                ++it;
            }
            if(sl.programPipeline)
                program = static_cast<Program*>(sl.programPipeline->getShaderProgram(p++));
            else program = NULL;
        }
        ++iSL;
    }
    if(!bDidBindLocation)
        return true;
    //
    // invalidate the related CstBuffers
    //
    m_container->invalidateTargets(this, -1);
    //
    // link again
    //
    iSL = m_statesLayers.begin();
    while(iSL != m_statesLayers.end())
    {
        int p=0;
        StatesLayer &sl = iSL->second;
        Program*           program = sl.program;
        if(sl.programPipeline)
            program = static_cast<Program*>(sl.programPipeline->getShaderProgram(p++));
        while(program)
        {
            //
            // WARNING !!!
            // Linkage will break the glUniformBlockBinding() !
            //
            program->link(m_container);
            if(sl.programPipeline)
                program = static_cast<Program*>(sl.programPipeline->getShaderProgram(p++));
            else program = NULL;
        }
        ++iSL;
    }
    // re-update the Cst buffers
    m_container->updateCstBuffers(this, -1, true, true);
    return true;
}
/*************************************************************************/ /**
 ** 
 ** 
 **/ /*************************************************************************/ 
void            Pass::clearAttributeBinding()
{
    m_attributes.clear();
}

/*************************************************************************/ /**
 ** 
 ** 
 **/ /*************************************************************************/ 
const char * Pass::getAttributeNameForLocation(int loc)
{
    if(m_attributes.empty())
        return NULL;
    AttribMap::iterator it = m_attributes.find(loc);
    if(it != m_attributes.end() )
        return it->second.c_str();
    return NULL;
}
/*************************************************************************/ /**
 ** 
 ** 
 **/ /*************************************************************************/ 
int Pass::getNumBoundAttributes()
{
    return (int)m_attributes.size();
}

/*************************************************************************/ /**
 ** 
 ** 
 **/ /*************************************************************************/ 
IProgram*    Pass::getProgram(int layerID)
{
#ifdef USECUDA
    if(layerID < 0)
        return m_pActiveStatesLayer->programCUDA ? m_pActiveStatesLayer->programCUDA: m_pActiveStatesLayer->program;
    if(layerID == 0)
        return m_pBaseStatesLayer->programCUDA ? m_pBaseStatesLayer->programCUDA: m_pBaseStatesLayer->program;
    StatesLayerMap::iterator iSL = m_statesLayers.find(layerID);
    if(iSL == m_statesLayers.end())
        return NULL;
    StatesLayer &sl = iSL->second;
    return sl.programCUDA ? sl.programCUDA: sl.program;
#else
    if(layerID < 0)
        return m_pActiveStatesLayer->program;
    if(layerID == 0)
        return m_pBaseStatesLayer->program;
    StatesLayerMap::iterator iSL = m_statesLayers.find(layerID);
    if(iSL == m_statesLayers.end())
        return NULL;
    StatesLayer &sl = iSL->second;
    return sl.program;
#endif
}

IProgramPipeline* Pass::getProgramPipeline(int id)
{
    StatesLayer *pSL;
    if(id == 0)
        pSL = m_pBaseStatesLayer;
    else if(id < 0)
        pSL = m_pActiveStatesLayer;
    else {
        StatesLayerMap::iterator it = m_statesLayers.find(id);
        if(it == m_statesLayers.end())
            return NULL;
        pSL = &(it->second);
    }
    return pSL->programPipeline;
}
/*************************************************************************/ /**
 ** \arg id is an arbitrary ID that allows to store in various layers
 ** the shaders. This will then allow to have many instances of the programs.
 **
 ** typical use-case : when we need almost the same shaders but with slightly different
 ** shaders, for example when lighting differs from a specific scene to another
 **
 ** \note the special ID 0 is the base layer for all the additional one : other
 ** layers will \e add or \e replace some other shaders. When a groupName with the same
 ** name is added, it will override the one from the base; when a new groupName is added,
 ** the whole program will have them available as any other part; when the shader with no
 ** groupName (NULL) is in a layer, the original one will be overridden...
 **/
bool Pass::setActiveProgramLayer(int id, bool createIfNA)
{
    bool found = true;
    if(id < 0)
        return false;
    if(!createIfNA)
    {
        StatesLayerMap::iterator iSL = m_statesLayers.find(id);
        if(iSL == m_statesLayers.end())
        {
            id = 0;
            found = false;
        }
    }
    StatesLayer &sl = m_statesLayers[id];
    m_pActiveStatesLayer = &sl;
    m_activeStatesLayer = id;
    return found;
}

int Pass::getActiveProgramLayer()
{
    return m_activeStatesLayer;
}

int Pass::getLayerId(int i)
{
    if(i >= (int)m_statesLayers.size())
        return -1;
    StatesLayerMap::iterator iSL = m_statesLayers.begin();
    while(i>0)
    {
        ++iSL;
        --i;
    }
    return iSL->first;
}

void Pass::removeProgramLayer(int id)
{
    if(id < 0)
        return /*false*/;
    StatesLayerMap::iterator iSL = m_statesLayers.find(id);
    if(iSL == m_statesLayers.end())
        return;
    int         layerID = iSL->first;
    StatesLayer &sl = iSL->second;
    //
    // Now we need to release things on this level
    //
    StateVec::iterator it = sl.statesForExecution.begin();
    while(it != sl.statesForExecution.end())
    {
        if(*it)
            delete *it;
        ++it;
    }
    it = sl.statesForValidation.begin();
    while(it != sl.statesForValidation.end())
    {
        if(*it)
            delete *it;
        ++it;
    }
//#ifndef USE_OLDPROGRAM
    if(sl.programPipeline)
    {
        delete_ProgramPipeline(sl.programPipeline, layerID);
    }
//#endif
    sl.statesForExecution.clear();
    sl.statesForValidation.clear();
    //
    // Now let's erase the slot sl
    //
    m_statesLayers.erase(iSL);
    setActiveProgramLayer(m_activeStatesLayer, false);
}

bool Pass::setupOverrides(ITechnique **dest, int numTechs)
{
    if(m_overrideID == 0)
        getID(&m_overrideID, 1);

    for(int i=0; i<numTechs; i++)
    {
        ITechnique *pT = dest[i];
        if(pT)
            for(int p=0; p<pT->getNumPasses(); p++)
            {
                IPass *pass = pT->getPass(p);
                setupOverrides(&pass, 1);
            }
    }
    return true;
}
bool Pass::setupOverrides(IPass **dest, int numPasses)
{
    if(m_overrideID == 0)
        getID(&m_overrideID, 1);

    for(int i=0; i<numPasses; i++)
    {
        Pass *pDestPass = static_cast<Pass*>(dest[i]);
        int layer = pDestPass->getActiveProgramLayer();
        StateVec::iterator iSO = m_statesOverride.begin();
        for(; iSO != m_statesOverride.end(); iSO++)
        {
            PassState* pSrcPassState = *iSO;
            PassState::Type t = pSrcPassState->getType();
            switch(t)
            {
            case IPassState::TCUDAModule:
                CHECK_TRUE_MSG(false,"TODO!!!");
                break;
            case IPassState::TVertexShader:
            case IPassState::TFragmentShader:
            case IPassState::TGeometryShader:
            case IPassState::TTCSShader:
            case IPassState::TTESShader:
            case IPassState::TComputeShader:
                {
                    // We ONLY create an instance if this group name is used
                    IPassState* pDstPassState = pDestPass->findState(pSrcPassState->m_name.c_str(), 0);
                    // Create only if we found the same group name in the base layer
                    if(pDstPassState)
                    {
                        pDestPass->invalidate(); // invalidate the whole pass since some stuff will change
						IShader** shaders =  new IShader*[pSrcPassState->m_shaders.size()];
						for(int s=0; s<(int)pSrcPassState->m_shaders.size(); s++)
							shaders[s] = pSrcPassState->m_shaders[s];
                        pDestPass->setActiveProgramLayer(m_overrideID, true);
                        if(! pDestPass->createState(t, shaders, (int)pSrcPassState->m_shaders.size(), pSrcPassState->m_name.c_str(),true) )
						{	// NOTE: this case can happen *when* a technique was included in another one:
							// the included technique might already have been used for instanciation of pass-states
							// thus we would encounter this redundancy
							// LOGI("NOTE: pass-state Override %s already created for pass %s\n", pSrcPassState->m_name.c_str(), m_name.c_str() );
						}
						delete [] shaders;
                    }
                }
                break;
            // typically used for samplers overriding
            case IPassState::TUniform:
                {
                    // In this case it is not necessary to find back the same state to perform overriding.
                    // So for now, let's optimize : if the overrid-ID doesn't exist, let's pass...
                    // this prevents every single sub-pass to instanciate a layer for nothing :
                    // if we create an instance here, the pass will have such an instance even if after all
                    // this instance isn't any different from the original. An instance of a pass deserve to be
                    // ONLY if some pieces of shaders are different because of a scene-level effect overriding them...
                    // LET'S ASSUME THAT UNIFORM OVERRIDING MAKES SENSE ONLY WHEN PIECES OF SHADERS ARE OVERRIDDEN...
                    if(!pDestPass->setActiveProgramLayer(m_overrideID, false/*DO NOT CREATE IT IF NOT HERE*/))
                    {
                        LOGI("Warning: pass-state Uniform Override skipped for %s\n", m_name.c_str() );
                        break;
                    }
                    pDestPass->invalidate(); // invalidate the whole pass since some stuff will change
                    // creates ON THE ACTIVE STATE LAYER a copy of the original state
                    PassState* pPS = static_cast<PassState*>(pDestPass->createStateFromState(pSrcPassState));
                    // let's validate here the texture unit #... to prevent conflicts
                    pDestPass->validateUniformTextureUnit(static_cast<Uniform*>(pPS->getUniform()), true, m_overrideID);
                    pPS->apply(true, NULL);
                }
                break;
            }
        }
        //
        // Now let's walk throught uniforms to check the status of Texture Units
        // If some didn't get updated properly, we will assign a texture unit
        // This action is the same as what is done during validation
        //
        // Commented out : we did it earlier in this function : pDestPass->validateUniformTextureUnit(static_cast<Uniform*>...
        //pDestPass->validateUniformTextureUnits(true);

        pDestPass->setActiveProgramLayer(layer);
    }
    return false;
}

bool Pass::releaseOverrides(ITechnique **dest, int numTechs)
{
    if(m_overrideID == 0)
        return false;

    for(int i=0; i<numTechs; i++)
    {
        ITechnique *pT = dest[i];
        for(int p=0; p<pT->getNumPasses(); p++)
        {
            IPass *pass = pT->getPass(p);
            releaseOverrides(&pass, 1);
        }
    }
    return true;
}
bool Pass::releaseOverrides(IPass **dest, int numPasses)
{
    if(m_overrideID == 0)
        return false;

    for(int i=0; i<numPasses; i++)
    {
        Pass *pDestPass = static_cast<Pass*>(dest[i]);
        pDestPass->removeProgramLayer(m_overrideID);
    }
    return true;
}

/*********************************************************************************************
 * NOTE: when adding a uniform to the pass, it could be that a similar uniform (same name) was referenced in the pass
 * as a pass-state : when assigning a value to this uniform from the pass.
 * In this case, we don't want to keep track of the additional uniform that would be create by nvFX or by the App :
 * pass-state uniform assignment overrides the other values
 */
void Pass::addUniform(Uniform *pU, int target)
{
    //CHECK_POINTER_RET(pU);
    if (!pU)
        return;
    Uniform::STarget &t = pU->m_targets[target];
    //
    // Let's look for the same name in pass-states, if ever...
    //
    /*OBSOLETE - StateVec::iterator its = m_pBaseStatesLayer->statesForExecution.begin();
    while(its != m_pBaseStatesLayer->statesForExecution.end())
    {
        //const char *name = (*it)->getName();
        if((*its)->m_type == PassState::TUniform)
        {
            if((*its)->m_uniform->m_name == pU->m_name)
                return;
        }
        ++its;
    }*/
    Pass::StatesLayerMap::iterator it = m_statesLayers.find(t.passLayerId);
    CHECK_TRUE(it != m_statesLayers.end());
    Pass::StatesLayer &sl = it->second;
    //
    // Check if a uniform with the same name and same target is there already
    // the only possible case of duplicate is when a Uniform got assigned through 
    // a pass (pass-state). In this case, the pass-state owns its unique nvFX::Uniform.
    // while another uniform of the same name could be created by the application
    // or by the parser when encountered a global uniform declaration
    // Note that uniform creation (through IContainer::createUniform() can lead to only one
    //
    int sz = (int)sl.usedUniforms.size();
    for(int i=0; i<sz; i++)
    {
        UniformAndTarget &ut = sl.usedUniforms[i];
        if((ut.pU->m_name == pU->m_name))
        {   // compare the targets. No need to duplicate if targets are the same
            if(!memcmp(&t, &(ut.pU->m_targets[ut.target]), sizeof(Uniform::STarget)))
                return;
            //else CHECK_TRUE(false); // just temporary : to test this special case when it happens
        }
    }

    sl.usedUniforms.push_back(Pass::UniformAndTarget(pU, target));
}
/*********************************************************************************************
 *
 */
void Pass::addCstBuffer(CstBuffer *pC, int target)
{
 //   CHECK_POINTER_RET(pC);
    if (!pC)
        return;
    CstBuffer::STarget &t = pC->m_targets[target];

    Pass::StatesLayerMap::iterator it = m_statesLayers.find(t.passLayerId);
    CHECK_TRUE(it != m_statesLayers.end());
    Pass::StatesLayer &sl = it->second;

    //
    // Check if a uniform with the same name and same target is there already
    //
    int sz = (int)sl.usedCstBuffers.size();
    for(int i=0; i<sz; i++)
    {
        CstBufferAndTarget &ut = sl.usedCstBuffers[i];
        if((ut.pC->m_name == pC->m_name))
        {   // compare the targets. No need to duplicate if targets are the same
            if(!memcmp(&t, &(ut.pC->m_targets[ut.target]), sizeof(Uniform::STarget)))
                return;
            //else CHECK_TRUE(false); // just temporary : to test this special case when it happens
        }
    }

    sl.usedCstBuffers.push_back(Pass::CstBufferAndTarget(pC, target));
}
/*********************************************************************************************
 *
 */
void Pass::addSamplerState(SamplerState *pS, int target)
{
    //CHECK_POINTER_RET(pS);
    if (!pS)
        return;
    SamplerState::STarget &t = pS->m_targets[target];

    Pass::StatesLayerMap::iterator it = m_statesLayers.find(t.passLayerId);
    CHECK_TRUE(it != m_statesLayers.end());
    Pass::StatesLayer &sl = it->second;
    sl.usedSamplerStates.push_back(Pass::SamplerStateAndTarget(pS, target));
}

/*********************************************************************************************
 *
 */
void            Pass::setLoopCount(int cnt)
{
    m_loopCnt = cnt > 0 ? cnt : 1;
}
int             Pass::getLoopCount()
{
    return m_loopCnt;
}
/*********************************************************************************************
 *
 */
void            Pass::setActive(bool bYes)
{
    m_parent->setPassActive(this, bYes);
}
bool            Pass::getActive()
{
    return m_parent->isPassActive(this);
}

#ifdef USECUDA
/*************************************************************************/ /**
 ** 
 **/ /*************************************************************************/ 
IPassState* Pass::createStateKernelEntry(const char * kernelName, int numArgs, ArgVal *argvalues)
{
    m_validated &= ~VALIDATED_PASS;
    PassState *p = new PassState(m_container, this);
    p->setStateKernelEntry(kernelName, numArgs, argvalues);
    m_pBaseStatesLayer->statesForValidation.push_back(p);
    return p;
}
IPassState* Pass::createStateBlockSz(int x, int y, int z)
{
    m_validated &= ~VALIDATED_PASS;
    PassState *p = new PassState(m_container, this);
    p->setStateBlockSz(x, y, z);
    m_pBaseStatesLayer->statesForValidation.push_back(p);
    return p;
}
IPassState* Pass::createStateGridSz(int x, int y, int z)
{
    m_validated &= ~VALIDATED_PASS;
    PassState *p = new PassState(m_container, this);
    p->setStateGridSz(x, y, z);
    m_pBaseStatesLayer->statesForValidation.push_back(p);
    return p;
}
IPassState* Pass::createStateSharedMemory(int Sz)
{
    m_validated &= ~VALIDATED_PASS;
    PassState *p = new PassState(m_container, this);
    p->setStateSharedMemory(Sz);
    m_pBaseStatesLayer->statesForValidation.push_back(p);
    return p;
}
#else
IPassState* Pass::createStateKernelEntry(const char * kernelName, int numArgs, ArgVal *argvalues)
{
    return NULL;
}
IPassState* Pass::createStateBlockSz(int x, int y, int z)
{
    return NULL;
}
IPassState* Pass::createStateGridSz(int x, int y, int z)
{
    return NULL;
}
IPassState* Pass::createStateSharedMemory(int Sz)
{
    return NULL;
}
#endif
IPassState* Pass::createStateComputeGroup(int x, int y, int z)
{
    m_validated &= ~VALIDATED_PASS;
    PassState *p = new PassState(m_container, this);
    p->setStateComputeGroup(x, y, z);
    m_pBaseStatesLayer->statesForValidation.push_back(p);
    return p;
}
