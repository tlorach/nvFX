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
//#   define  LOGI(...)  { nvFX::printf(__VA_ARGS__); printf("\n"); }
//#   define  LOGE(...)  { nvFX::printf(">>Error : "); printf(__VA_ARGS__); printf("\n"); }
#ifdef WIN32
#include <windows.h> // for OutputDebugString
#endif
#include <fstream>
#include <assert.h>
#include "FxGL.h"
#include "FxCUDA.h"
#pragma warning(disable:4996)
using namespace nvFX;

namespace nvFX
{

/*****************************************************************************/
/** TextureCUDA TextureCUDA TextureCUDA TextureCUDA TextureCUDA TextureCUDA **/
/*****************************************************************************/
//TO REMOVE
//TextureCUDA::TextureCUDA(Container* pCont, const char * name) :
//    Texture(pCont, name)
//{
//}
//
//TextureCUDA::~TextureCUDA()
//{
//}
//
//Texture*  TextureCUDA::update2(void *data, Pass *pass, bool bBindProgram, bool bCreateIfNeeded)
//{
//    int id;
//    for(int i=0; (id=pass->getLayerId(i)) >= 0; i++)
//        update(data, pass, i, bBindProgram, bCreateIfNeeded);
//    return this;
//}
//
//Texture*  TextureCUDA::update(void *data, Pass *pass, int layerID, bool bBindProgram, bool bCreateIfNeeded)
//{
//    CUresult res;
//    CUarray cuArray = NULL;
//    if(layerID < 0)
//        return update2(data, pass, bBindProgram, bCreateIfNeeded);
//    if(m_resource)
//    {
//        // texture reference taken from the CUDA program :
//        // the uniform texture will take care of this with :
//        //CUtexref texRef; cuModuleGetTexRef(&texRef, getModule(), name);
//        cuArray = static_cast<ResourceCUDA*>(m_resource)->mapResource();
//    }
//    if(cuArray)
//    {
//        for(int i=0; i<(int)m_targets.size(); i++)
//        {
//            STarget &t = m_targets[i];
//            if(((pass == NULL) || ((t.pass == pass)&&(t.passLayerId == layerID)))&&(t.ttype == TCUDA))
//            {
//                res = cuTexRefSetArray( (CUtexref)t.cudaTexRef, cuArray, CU_TRSA_OVERRIDE_FORMAT );
//                if(res)
//                {
//                    LOGE("Error>> CUDA failed to set texture reference to CUDA sampler\n");
//                    return false;
//                }
//            }
//        }
//    }
//    //
//    // a pass ptr was specified and nothing found, we shall try to find the uniform in the pass program
//    //
//    if(pass && bCreateIfNeeded)
//    {
//        STarget t;
//        int p = 0;
//        t.index = -1;
//        t.dirty = false;
//        t.valid = false;
//        t.pass = pass;
//        t.ttype = TCUDA;
//        t.passLayerId = layerID;
//        t.shaderProgramNumber = 0;
//        // cast it to CUDAProgram : it can't be another class in this case
//        IProgram* prg = pass->getProgram(layerID);
//        if(prg)
//        {
//            CUmodule module = (CUmodule)(static_cast<CUDAProgram*>(prg)->m_cuModule);
//            CUtexref texRef;
//            res = cuModuleGetTexRef(&texRef, module, m_name.c_str());
//            t.cudaTexRef = texRef;
//            if(res)
//            {
//                LOGI("Warning : texture %s referenced in the pass %s is not used in the Cuda module of this pass\n", pass->getName(), m_name.c_str());
//            } else // Store the target if the shader needed it
//            {
//                // NOTE: maybe we should use a std::map<pass,target> for targets...
//                int i = 0;
//                for(i=0; i<(int)m_targets.size(); i++)
//                    if((m_targets[i].pass == pass)
//                    && (m_targets[i].passLayerId == layerID)
//                    && (m_targets[i].ttype == t.ttype))
//                    {
//                        m_targets[i] = t;
//                        break;
//                    }
//                if(i == m_targets.size())
//                {
//                    m_targets.push_back(t);
//                    pass->addTexture(this, i);
//                }
//                // prepare the resource related to this texture
//                // maybe we should put this call somewhere else
//                if(m_resource)
//                    static_cast<ResourceCUDA*>(m_resource)->setupAsCUDATexture();
//                // update again now we found it
//                return updateForTarget(data, t);
//            }
//        }
//    }
//    return this;
//}
//
//Texture*  TextureCUDA::updateForTarget(void *data, STarget &t)
//{
//    CUresult res;
//    CUarray cuArray = NULL;
//    if(m_resource)
//    {
//        cuArray = static_cast<ResourceCUDA*>(m_resource)->mapResource();
//    }
//    if(cuArray)
//    {
//        if(t.ttype == TCUDA)
//        {
//            res = cuTexRefSetArray( (CUtexref)t.cudaTexRef, cuArray, CU_TRSA_OVERRIDE_FORMAT );
//            if(res)
//            {
//                LOGE("Error>> CUDA failed to set texture reference to CUDA sampler\n");
//                return this;
//            }
//        }
//    }
//    return this;
//}
//
//void TextureCUDA::update(GLenum target, GLint tex, bool bindTexture)
//{
//    // Nothing to do here : OpenGL-specific
//    // return false; ?
//}
//
////void TextureCUDA::setD3DResourceView(void *p)
////{
////}
//

/*************************************************************************/ /**
 ** \brief
 **
 **/ /*************************************************************************/ 
ResourceCUDA::ResourceCUDA(ResourceRepository* pCont, const char *name) : Resource(pCont, name)
{
        m_size = 0;
        m_xByteSz = 0;
        m_pitch = 0;
        m_dptr = NULL;
        m_cudaResource = NULL;
        //m_texRef = 0;
}
ResourceCUDA::~ResourceCUDA()
{
}

//----------------------------------------------------------------
/// setup for interop : prt refers to a texture in which we write
/// results. This must be seen as a linear buffer. So we need to
/// allocate a temporary linear buffer that will be then copied
/// back to the texture
//
bool ResourceCUDA::setupAsCUDATarget()
{
    int fmtSz = ResourceFormatByteSize(m_creationData.fmt);
    CUresult res;
    m_xByteSz = m_creationData.sz[0] * fmtSz;
    m_size = m_xByteSz * m_creationData.sz[1];
    if(m_dptr)
    {
        res = cuMemFree(m_dptr);
        if(res) {
			LOGE("cuMemFree Failed for resource %s\n", m_name.c_str() );
            return false;
		}
    }
    res = cuMemAllocPitch( &m_dptr, &m_pitch, m_xByteSz, m_creationData.sz[1], 4);
    if(res)
        return false;
    float pitchToSendToKernel = (float)m_pitch / (float)fmtSz;
#pragma MESSAGE("TODO TODO TODO TODO TODO TODO : send pitch to the kernel !")
    LOGI("Event>>cuMemAllocPitch : Pitch of Target buffer (%d, %d) allocation = %d, %f\n", m_creationData.sz[0], m_xByteSz, m_pitch, pitchToSendToKernel);
    //
    // Register the texture to CUDA to be able to copy data back in it
    //
    GLenum target;
    if(m_cudaResource)
    {
        res = cuGraphicsUnregisterResource(m_cudaResource);
        if(res)
            return false;
    }
    switch(m_type)
    {
    case RESTEX_1D:
        target = GL_TEXTURE_1D;
        break;
    case RESTEX_2D:
        target = GL_TEXTURE_2D;
        break;
    case RESTEX_2DRECT:
    case RESRBUF_2D:
    case RESOURCE_UNKNOWN:
    //case RESTEX_3D:
    //case RESTEX_CUBE_MAP:
    default:
        LOGE("Failed to register the resource %s for CUDA : may be a render buffer\n", m_name.c_str());
        return false;
    };
    res = cuGraphicsGLRegisterImage( 
        &m_cudaResource, m_OGLId, target, 
        CU_GRAPHICS_MAP_RESOURCE_FLAGS_WRITE_DISCARD );
    if(res)
    {
        LOGE("Failed to register the texture %s for CUDA (as write discard)\n", m_name.c_str());
        return 0;
    }
    return true;
}

//----------------------------------------------------------------
/// Prepare what is needed to perform interop so the OGL texture 
/// can be exposed to CUDA
bool ResourceCUDA::setupAsCUDATexture()
{
    CUresult res;
    GLenum target;
    if(m_cudaResource)
        return true;
    // let's not unregister if the resource is here. We assume m_cudaResource is valid if non NULL
    //res = cuGraphicsUnregisterResource(m_cudaResource);
    if(m_OGLId == 0)
        validate();
    if(m_OGLId == 0)
        return false;
    switch(m_type)
    {
    case RESTEX_1D:
        target = GL_TEXTURE_1D;
        break;
    case RESTEX_2D:
        target = GL_TEXTURE_2D;
        break;
    case RESTEX_2DRECT:
        LOGI("WARNING>> RESTEX_2DRECT unhandled by CUDA");
        return false;
    case RESRBUF_2D:
        LOGI("WARNING>> RESRBUF_2D unhandled by CUDA");
        return false;
    case RESTEX_3D:
        LOGI("WARNING>> RESTEX_3D unhandled by CUDA");
        return false;
    case RESTEX_CUBE_MAP:
        LOGI("WARNING>> RESTEX_CUBE_MAP unhandled by CUDA");
        return false;
    case RESOURCE_UNKNOWN:
    default:
        assert(!"Resource unknown and unhandled by CUDA");
        return false;
    };
    res = cuGraphicsGLRegisterImage( 
        &m_cudaResource, m_OGLId, target, 
        CU_GRAPHICS_MAP_RESOURCE_FLAGS_READ_ONLY );
    if(res)
    {
        LOGE("Failed to register the texture %s for CUDA (as Read only)\n", m_name.c_str());
        return false;
    }
    return true;
}

CUarray ResourceCUDA::mapResource(CUstream streamID)
{
    CUresult err;
    if(m_cudaResource == NULL)
    {
        if(!setupAsCUDATexture())
            return NULL;
    }
    err = cuGraphicsMapResources( 1, &m_cudaResource, streamID );
    if(err == CUDA_ERROR_ALREADY_MAPPED)
    {
        LOGI("Warning>> cuGraphicsMapResources was already done for %s. Often happens if setting  more than 1 time a texture for some resource\n", m_name.c_str());
    }
    else if(err != CUDA_SUCCESS)
    {
        LOGE("Error>> CUDA failed map some source resources\n");
        return NULL;
    }
    CUarray cuArray;
    err = cuGraphicsSubResourceGetMappedArray( &cuArray, m_cudaResource, 0/*arrayIndex*/, 0/*mipLevel*/);
    if(err)
    {
        LOGE("Error>> CUDA failed to get mapped resources pointer\n");
        err = cuGraphicsUnmapResources( 1, &m_cudaResource, streamID );
        return NULL;
    }
    return cuArray;
}
bool ResourceCUDA::unmapResource(CUstream streamID)
{
    CUresult err;
    if(m_cudaResource == NULL)
        return false;
    err = cuGraphicsUnmapResources( 1, &m_cudaResource, streamID );
    if(err)
        return false;
    return true;
}

bool ResourceCUDA::updateResourceFromCUDA(CUstream streamID)
{
    CUresult res;
    res = cuGraphicsMapResources( 1, &m_cudaResource, streamID );
    if(res)
    {
        LOGE("Error>> CUDA failed map some target resources\n");
        return false;
    }
    //
    // Walk through output resources and perform the copies
    //
    CUarray cuArray;
//#   define DBGDUMMYCOPY
#   ifdef DBGDUMMYCOPY
    // interop has issues... let's compare with a copy to a basic cuda array
    int www = m_xByteSz/4;
    CUDA_ARRAY_DESCRIPTOR descr = {
            www,//unsigned int Width;
            m_creationData.sz[1],//unsigned int Height;
            CU_AD_FORMAT_UNSIGNED_INT8,//CUarray_format Format;
            4//unsigned int NumChannels;
        };
    res = cuArrayCreate(&cuArray, &descr);
#   else
    res = cuGraphicsSubResourceGetMappedArray( &cuArray, m_cudaResource, 0/*arrayIndex*/, 0/*mipLevel*/);
#   endif
    if(res)
    {
        res = cuGraphicsUnmapResources( 1, &m_cudaResource, streamID );
        return false;
    }
    CUDA_MEMCPY2D copyInfo = {
        0,                    ///< Source X in bytes
        0,                    ///< Source Y
        CU_MEMORYTYPE_DEVICE,//< Source memory type (host, device, array)
        NULL,                ///< Source host pointer
        m_dptr,            ///< Source device pointer
        NULL,                ///< Source array reference
        m_pitch,            ///< Source pitch (ignored when src is array)

        0,                    ///< Destination X in bytes
        0,                    ///< Destination Y
        CU_MEMORYTYPE_ARRAY,///< Destination memory type (host, device, array)
        NULL,                ///< Destination host pointer
        NULL,                ///< Destination device pointer
        cuArray,            ///< Destination array reference
        0,                    ///< Destination pitch (ignored when dst is array)

        m_xByteSz,        ///< Width of 2D memory copy in bytes
        m_creationData.sz[1]             ///< Height of 2D memory copy
    };
    //LOGI("cuMemcpy2D(): CU_MEMORYTYPE_DEVICE source=%x pitch=%d CU_MEMORYTYPE_ARRAY=%x widthBytes=%d height=%d\n",m_dptr, m_pitch, cuArray, m_xByteSz, m_creationData.sz[1]);
    res = cuMemcpy2D( &copyInfo );
    if(res)
    {
        LOGE("Error>> CUDA failed to copy linear memory to texture (array memory)\n");
        res = cuGraphicsUnmapResources( 1, &m_cudaResource, streamID );
        return false;
    }
#   ifdef DBGDUMMYCOPY
    res = cuArrayDestroy(cuArray);
#   endif
    res = cuGraphicsUnmapResources( 1, &m_cudaResource, streamID );
    if(res)
    {
        LOGE("Error>> CUDA failed unmap the resource for output result of the kernel\n");
        return false;
    }
    return true;
}

} //namespace nvFX