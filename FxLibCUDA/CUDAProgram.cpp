#define SHADERCONCAT_USE
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
#include "FxCUDA.h"

#pragma warning(disable:4996)
using namespace nvFX;


namespace nvFX
{
    extern CUdevice    cudaDevice;
    extern CUcontext   cudaContext;
    extern int cutilDrvGetMaxGflopsDeviceId();
    extern unsigned int saveTempFile(const char *name, const char *str);
    extern std::string compileCUDA(const char * srcname, unsigned int checksum);

    static int CUDAModules = 1;
}

/*****************************************************************************/
/* CUDAProgram CUDAProgram CUDAProgram CUDAProgram CUDAProgram CUDAProgram   */
/*****************************************************************************/

/*************************************************************************/ /**
 ** 
 ** 
 **/ /*************************************************************************/ 
CUDAProgram::~CUDAProgram()
{
    cleanup();
}

/*************************************************************************/ /**
 ** 
 ** 
 **/ /*************************************************************************/ 
CUDAProgram::CUDAProgram(Container *pCont) : Program(pCont)
{
    //
    // CUDA Context
    //
    if(!cudaContext)
    {
        CUdevice cuda_device;
        //if ( /*specific device*/ ) {
        //    cutilGLDeviceInitDrv(cuda_device, argc, argv);
        //    cutilDrvSafeCallNoSync(cuDeviceGet(&g_oDevice, cuda_device));
        //} else 
        {
            cuda_device = cutilDrvGetMaxGflopsDeviceId();
            cuDeviceGet(&cudaDevice, cuda_device );
        }
        // Create CUDA Device w/ GL interop
        // (use CU_CTX_BLOCKING_SYNC for better CPU synchronization)
        cuGLCtxCreate(&cudaContext, CU_CTX_BLOCKING_SYNC, cudaDevice);
        if(cudaContext == NULL)
        { LOGI("Error>> Could not create the CUDA context !\n"); }
        LOGI("Event>>Created CUDA context 0x%x \n", cudaContext);
    }
	m_linkNeeded = true;
    m_usable = false;
    m_moduleID = CUDAModules++;
    m_cuModule = NULL;
}


/*************************************************************************/ /**
 ** 
 ** 
 **/ /*************************************************************************/ 
bool CUDAProgram::addShader(ShaderType type, IShader* pShader, IContainer* pContainer)
{
    if(!pShader)
        return false;
    switch(type)
    {
    case FX_CUDAPROG:
        m_shaders[std::string(pShader->getName())] = static_cast<CUDAShader*>(pShader);
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
IShader*    CUDAProgram::getShader(int n, ShaderType *t)
{
    ShaderMap::iterator it;
    it = m_shaders.begin();
    while(it != m_shaders.end())
    {
        if(n == 0)
        {
            if(t) *t = FX_CUDAPROG;
            return it->second;
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
IShader*    CUDAProgram::getShader(IShader *pShader, ShaderType *t)
{
    ShaderMap::iterator it;
    it = m_shaders.begin();
    while(it != m_shaders.end())
    {
        if(it->second == pShader)
        {
            if(t) *t = FX_VTXPROG;
            return it->second;
        }
        ++it;
    }
    return NULL;
}
/*************************************************************************/ /**
 ** 
 ** 
 **/ /*************************************************************************/ 
IShader*    CUDAProgram::getShader(const char *name, ShaderType *t)
{
    ShaderMap::iterator it;
    it = m_shaders.find(name);
    if(it != m_shaders.end())
    {
        if(t) *t = FX_VTXPROG;
        return it->second;
    }
    return NULL;
}
/*************************************************************************/ /**
 ** 
 ** 
 **/ /*************************************************************************/ 
int         CUDAProgram::getNumShaders()
{
    return (int)m_shaders.size();
}

/*************************************************************************/ /**
 ** 
 ** 
 **/ /*************************************************************************/ 
//TODO: geom, tess...

void CUDAProgram::cleanup()
{
    //
    // then delete the allocations
    //
    ShaderMap::iterator it = m_shaders.begin();
    while(it != m_shaders.end())
    {
        ++it;
    }
    m_shaders.clear();
}
/*************************************************************************/ /**
 ** 
 **
 **/ /*************************************************************************/ 
bool CUDAProgram::link(IContainer* pContainer)
{
    CUresult cuStatus;
    if(m_linkNeeded)
    {
        //
        // add common parts for line-number conversion :-(
        //
        std::vector<CUDAShader::codeInfo> codeBlocks;
        int lineOffset = 0;
        ShaderMap::iterator iShd, iEnd;
//nvFX::printf("=====================================\n" );
        iShd = m_shaders.begin();
        iEnd = m_shaders.end();
//nvFX::printf("%s Shader code : \n", s==0 ? "Vertex" : "Fragment");
        for(;iShd != iEnd; iShd++)
        {
//nvFX::printf("----\n%s\n", iShd->second->m_shaderCode.c_str() );
            for(int n=0; n<(int)iShd->second->m_startLineNumbers.size(); n++)
                codeBlocks.push_back(iShd->second->m_startLineNumbers[n]);
        }

        m_fullCode.clear();
        // Add common header code
        IShader *pShd;
        for(int i=0; pShd = static_cast<CUDAShader*>(pContainer->findShader(i)); i++)
            if((*(pShd->getName()) == '\0') && (pShd->getType() == TCUDA) )
            {
                m_fullCode += pShd->getShaderCode();
            }
        iShd = m_shaders.begin();
        iEnd = m_shaders.end();
        std::string tempName(pContainer->getName());
        for(;iShd != iEnd; iShd++)
        {
            m_fullCode += iShd->second->getShaderCode();
            tempName += std::string("_");
            tempName += iShd->second->getName();
        }
        //
        // Save the temp file and compile it
        //
        unsigned int checksum = saveTempFile(tempName.c_str(), m_fullCode.c_str());
        std::string targetName = compileCUDA(tempName.c_str(), checksum);
        m_fullCode.clear();
        m_linkNeeded = false;
        if(targetName.empty())
        {
            return false;
        }
        //
        // load the compiled file
        //
        FILE *fp = fopen(targetName.c_str(), "rb");
        fseek(fp, 0, SEEK_END);
        int file_size = ftell(fp);
        char *buf = new char[file_size+1];
        fseek(fp, 0, SEEK_SET);
        fread(buf, sizeof(char), file_size, fp);
        fclose(fp);
        buf[file_size] = '\0';
        std::string ptx_source(buf);
        delete[] buf;

        if (targetName.rfind(".ptx") != std::string::npos) 
        {
            // in this branch we use compilation with parameters
            const unsigned int jitNumOptions = 3;
            CUjit_option *jitOptions = new CUjit_option[jitNumOptions];
            void **jitOptVals = new void*[jitNumOptions];

            // set up size of compilation log buffer
            jitOptions[0] = CU_JIT_INFO_LOG_BUFFER_SIZE_BYTES;
            int jitLogBufferSize = 1024;
            jitOptVals[0] = (void *)jitLogBufferSize;

            // set up pointer to the compilation log buffer
            jitOptions[1] = CU_JIT_INFO_LOG_BUFFER;
            char *jitLogBuffer = new char[jitLogBufferSize];
            jitOptVals[1] = jitLogBuffer;

            // set up pointer to set the Maximum # of registers for a particular kernel
            jitOptions[2] = CU_JIT_MAX_REGISTERS;
            int jitRegCount = 32;
            jitOptVals[2] = (void *)jitRegCount;

            cuStatus = cuModuleLoadDataEx(&m_cuModule, ptx_source.c_str(), jitNumOptions, jitOptions, (void **)jitOptVals);
            if (cuStatus != CUDA_SUCCESS) {
                printf("cuModuleLoadDataEx error!\n");
                return false;
            }
            //printf("> PTX JIT log:\n%s\n", jitLogBuffer);
        } else {
            cuStatus = cuModuleLoad(&m_cuModule, targetName.c_str());
            if (cuStatus != CUDA_SUCCESS) {
                m_cuModule = NULL;
                printf("cuModuleLoad error!\n");
                return false;
            }
        }

        printf(">> modInitCTX<%s> initialized %s", 
            targetName.c_str(), (cuStatus == CUDA_SUCCESS) ? "SUCCESS!\n" : "FAILED\n");

    } //if(m_linkNeeded)
    return true;
}
/*************************************************************************/ /**
 ** 
 ** TODO: in SHADERCONCAT_USE, add error log with proper line numbers to nvFX files
 ** 
 **/ /*************************************************************************/ 
bool CUDAProgram::bind(IContainer* pContainer)
{
    if(m_linkNeeded)
        if(!link(pContainer))
            return false;
    return true;
}
/*************************************************************************/ /**
 ** 
 ** 
 **/ /*************************************************************************/ 
void CUDAProgram::unbind(IContainer* pContainer)
{
}

// NOTE: we could avoid redundant setup by first checking if the required entry combination is already
// available. However this is not such a mandatory optimization...
int CUDAProgram::createKernelEntry(const char*kernelName, 
        int *gridSz/*[3]*/, int *blockSz/*[3]*/, int ShMemSz,
        int numArgs, ArgVal *args)
{
    KernelEntry ke;
    CUresult cuStatus = cuModuleGetFunction( &ke.cudaFunc, m_cuModule, kernelName);
	if(cuStatus)
		return -1;
    memcpy(ke.gridSz, gridSz, sizeof(int)*3);
    memcpy(ke.blockSz, blockSz, sizeof(int)*3);
    ke.ShMemSz = ShMemSz;
    ke.numArgs = numArgs;
    ke.funcName = std::string(kernelName);
    ke.args = args;

    for(int i=0; i<ke.numArgs; i++)
    {
        switch(ke.args[i].type)
        {
        case IUniform::TInt: case IUniform::TInt2: case IUniform::TInt3: case IUniform::TInt4:
            break;
        //case IUniform::TBool: case IUniform::TBool2: case IUniform::TBool3: case IUniform::TBool4:
        //    break;
        case IUniform::TFloat: case IUniform::TVec2: case IUniform::TVec3: case IUniform::TVec4:
            break;
        //case IUniform::TMat2: case IUniform::TMat3: case IUniform::TMat4:
        case IUniform::TUBO:
            break;
        case IUniform::TCB:
            break;
        case IUniform::TUniform:
            assert(!"TODO");
            break;
        case IUniform::TTexture:
        case IUniform::TTexture1D:
        case IUniform::TTexture2D:
        case IUniform::TTexture2DRect:
        case IUniform::TTexture3D:
        case IUniform::TTextureCube:
            {
            ResourceCUDA* pRes = static_cast<ResourceCUDA*>(ke.args[i].val.pResource);
            if(pRes->validationNeeded())
            {
                pRes->validate();
                pRes->setupAsCUDATarget();
            }
            break;
            }
        //case IUniform::TTexUnit:
        default:
            assert(0);
            break;
        }
    }
    m_kernelEntries.push_back(ke);
    return (int)m_kernelEntries.size()-1;
}

bool CUDAProgram::executeKernelEntry(int entryID)
{
    CUresult cuStatus;
    if(entryID >= (int)m_kernelEntries.size())
        return false;
    KernelEntry &ke = m_kernelEntries[entryID];
    if(ke.cudaFunc == NULL)
        return false;

    // setup execution parameters
    cuStatus = cuFuncSetBlockShape( ke.cudaFunc, ke.blockSz[0], ke.blockSz[1], ke.blockSz[2] );
    if(cuStatus)
        return false;
	cuStatus = cuFuncSetSharedSize(ke.cudaFunc, ke.ShMemSz);
    if(cuStatus)
        return false;
    int offset = 0;
    int byteSize = 0;
    for(int i=0; i<ke.numArgs; i++)
    {
        switch(ke.args[i].type)
        {
        case IUniform::TInt: case IUniform::TInt2: case IUniform::TInt3: case IUniform::TInt4:
            byteSize = sizeof(int)*(1 + (int)ke.args[i].type-(int)TInt);
            cuStatus = cuParamSetv(ke.cudaFunc, offset, ke.args[i].val.i, byteSize);
            if(cuStatus)
                return false;
            break;
        //case IUniform::TBool: case IUniform::TBool2: case IUniform::TBool3: case IUniform::TBool4:
        //    break;
        case IUniform::TFloat: case IUniform::TVec2: case IUniform::TVec3: case IUniform::TVec4:
            byteSize = sizeof(float)*(1 + (int)ke.args[i].type-(int)TFloat);
            cuStatus = cuParamSetv(ke.cudaFunc, offset, ke.args[i].val.f, byteSize);
            if(cuStatus)
                return false;
            break;
        //case IUniform::TMat2: case IUniform::TMat3: case IUniform::TMat4:
        case IUniform::TUBO:
            byteSize = sizeof(CUdeviceptr);
            cuStatus = cuParamSetv(ke.cudaFunc, offset, ke.args[i].val.i, byteSize);
            if(cuStatus)
                return false;
            break;
        case IUniform::TCB:
            byteSize = sizeof(CUdeviceptr);
            cuStatus = cuParamSetv(ke.cudaFunc, offset, ke.args[i].val.i, byteSize);
            if(cuStatus)
                return false;
            break;
        case IUniform::TUniform:
            // for uniforms, we shall copy the uniform data value(s) to be sure we have the updated version
            // Note: we could also define a "target" for this Uniform so that the argument is one of its targets... maybe later
            assert(!"TODO");
            cuStatus = cuParamSetv(ke.cudaFunc, offset, ke.args[i].val.i, byteSize);
            if(cuStatus)
                return false;
            break;
        case IUniform::TTexture:
        case IUniform::TTexture1D:
        case IUniform::TTexture2D:
        case IUniform::TTexture2DRect:
        case IUniform::TTexture3D:
        case IUniform::TTextureCube:
            {
            byteSize = sizeof(CUdeviceptr);
            ResourceCUDA* pRes = static_cast<ResourceCUDA*>(ke.args[i].val.pResource);
            // we assume the resource is ready to be shared as an argument for the kernel
            cuStatus = cuParamSetv(ke.cudaFunc, offset, &pRes->m_dptr, byteSize);
            if(cuStatus)
                return false;
            byteSize = 8; // No idea how to do : the dev ptr is 64 bits although sizeof(CUdeviceptr) is 32 bits...
            break;
            }
        //case IUniform::TTexUnit:
        default:
            assert(0);
            return false;
            break;
        }
        offset += byteSize;
    }
    cuStatus = cuParamSetSize(ke.cudaFunc,offset);
    if(cuStatus)
        return false;

    cuStatus = cuLaunchGrid( ke.cudaFunc, ke.gridSz[0], ke.gridSz[1] );
    if(cuStatus)
        return false;

    //
    // Now we must put back the possible changes from CUDA to the texture(s)
    //
    for(int i=0; i<ke.numArgs; i++)
    {
        switch(ke.args[i].type)
        {
        case IUniform::TTexture:
        case IUniform::TTexture1D:
        case IUniform::TTexture2D:
        case IUniform::TTexture2DRect:
        case IUniform::TTexture3D:
        case IUniform::TTextureCube:
            {
            byteSize = sizeof(CUdeviceptr);
            ResourceCUDA* pRes = static_cast<ResourceCUDA*>(ke.args[i].val.pResource);
            pRes->updateResourceFromCUDA();
            break;
            }
        default:
            break;
        }
    }
    return true;
}

//----------------------------------------------------------------
/// regular CUDA Buffer
//
CUDABuffer *CUDAProgram::CreateCudaBuffer(const char *name, int Sz)
{
	CUDABuffer &buf = m_buffers[std::string(name)];
    assert(buf.dptr == NULL);
	buf.size = Sz;
    buf.dptr = NULL;
	CUresult res = cuMemAlloc( &buf.dptr, buf.size);
	if(res)
		return NULL;
	return &buf;
}



