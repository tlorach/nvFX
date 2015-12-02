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
#   define  LOGI(...)  { printf(__VA_ARGS__); printf("\n"); }
#   define  LOGE(...)  { printf(">>Error : "); printf(__VA_ARGS__); printf("\n"); }
#ifdef WIN32
#include <windows.h> // for OutputDebugString
#endif
#include <fstream>
#include <assert.h>
#include "FxCUDA.h"
#pragma warning(disable:4996)
using namespace nvFX;

namespace nvFX
{
	// TODO: Need to find a more flexible way to expose these parameters
	// easiest would be to add them in the effect as additional arguments...
	// TODO: need to see how can CUDA avoid this dependency to C/C++ compiler
    // you MUST use "call" if you want system() to work under Windows (!!?!?!!! @@!#$@#!?!?!!!)
    const char *NVCC = "call nvcc.exe";
    //const char *NVCC = "call \"%CUDA_BIN_PATH%\\nvcc.exe\"";
    const char *optiXIncPath = "%OPTIX_INC_PATH%";
    const char *tempPath = "C:\\tmp";//"%TEMP%";
    const char *ccbin = " -ccbin=\"C:\\Program Files (x86)\\Microsoft Visual Studio 11.0\\VC\" ";
    const char *compileCUDACmdCubin = " -gencode=arch=compute_20,code=sm_20 -use_fast_math -maxrregcount=32 -cubin -o \"FILET\" \"FILES\"";
    const char *compileCUDACmdPTX = " -gencode=arch=compute_20,code=sm_20 -use_fast_math -maxrregcount=32 -ptx -o \"FILET\" \"FILES\"";
    const char *compileCUDAOptiXCmd = " -gencode=arch=compute_20,code=sm_20 -use_fast_math -maxrregcount=32 -ptx -o \"FILET\" \"FILES\"";;

CUdevice    cudaDevice  = NULL;
CUcontext   cudaContext = NULL;

Program* new_ProgramCUDA(Container *pCont)
{
    return new CUDAProgram(pCont);
}
void delete_ProgramCUDA(IProgram *pProg)
{
    CUDAProgram *p = static_cast<CUDAProgram *>(pProg);
    delete p;
}

Shader* new_ShaderCUDA(const char* name)
{
    return new CUDAShader(name);
}
void delete_ShaderCUDA(IShader *pShd)
{
    CUDAShader *p = static_cast<CUDAShader *>(pShd);
    delete p;
}

//----------------------------------------------------------------
// This function returns the best GPU (with maximum GFLOPS)
int cutilDrvGetMaxGflopsDeviceId()
{
        int device_count = 0;
        CUdevice current_device = 0;

        CUresult res;
        res = cuInit(0);
        if(res)
            return -1;
        res = cuDeviceGetCount(&device_count);
        if(res)
            return -1;

        CUdevice max_gflops_device = 0;
        int max_gflops = 0;
        int multiProcessorCount, clockRate;
    
    while( current_device < device_count )
    {
        res = cuDeviceGetAttribute( &multiProcessorCount, CU_DEVICE_ATTRIBUTE_MULTIPROCESSOR_COUNT, current_device );
        if(res)
            return -1;
        res = cuDeviceGetAttribute( &clockRate, CU_DEVICE_ATTRIBUTE_CLOCK_RATE, current_device );
        if(res)
            return -1;

        int gflops = multiProcessorCount * clockRate;
        if( gflops > max_gflops )
        {
            max_gflops        = gflops;
            max_gflops_device = current_device;
        }
        ++current_device;
    }

    return max_gflops_device;
}


std::string compileCUDA(const char * srcname, unsigned int checksum)
{
    // TODO: do a better job at appending temporary folder location !!
    // CANCELED for now
    LOGI("Compiling OptiX-CUDA %s...\n", /*tempPath,*/ srcname );
    std::string cmd = NVCC /*+ std::string(" -I") + optiXIncPath*/ + std::string(ccbin) + std::string(compileCUDACmdPTX) + std::string(" 2>CUDA_log.txt");
    int psrc = (int)cmd.find("FILES");
    std::string fname = /*std::string(tempPath) + std::string("\\") +*/ std::string(srcname) + std::string(".cu");;
    cmd.replace(psrc, 5, fname);
    std::string dstname(srcname);
    int pos = (int)dstname.rfind(".");
    char checksumStr[50];
    sprintf(checksumStr, "%x", checksum);
    dstname = dstname.substr(0, pos) + std::string("_") + std::string(checksumStr) + std::string(".ptx");
    psrc = (int)cmd.find("FILET");
    cmd.replace(psrc, 5, dstname);
    FILE *fdOut = fopen(dstname.c_str(), "r");
    if(fdOut)
    {
        fclose(fdOut);
        LOGI("Module %s already there, using it instead of compiling CUDA code\n", dstname.c_str() );
        return dstname;
    }
    LOGI("Compiling CUDA Module %s\n", cmd.c_str() );
    _flushall();
    int res = system(cmd.c_str());
    if(res) {
        LOGI("Error>>Compile command failed... Check CUDA_log.txt\n" ); 
        return std::string();
    } else {
        LOGI("Yes>>Compilation Done\n");
    }
    return dstname;
}

unsigned int saveTempFile(const char *name, const char *str)
{
    std::string n;
    unsigned long checksum = 0;
    const char *c;
    int i=1;
    for(c = str; *c != '\0'; c++, i++)
    {
        checksum += i*(unsigned int)*c;
    }
    n = /*std::string(tempPath)+std::string("\\")+*/std::string(name)+std::string(".cu");
    FILE *fd = fopen(n.c_str(), "w");
    if(!fd)
        return 0;
    fprintf(fd, "//Checksum=%x\n", checksum);
    fprintf(fd, "%s", str);
    fclose(fd);
    return checksum;
}

std::string buildCodeForThisUniformCUDA(IContainer* pCont, IUniform* pUniform)
{
    std::string code;
    const char * name = pUniform->getName();
    switch(pUniform->getType())
    {
        // TODO...
    case IUniform::TInt: code += "//__constant__ int "; break;
    case IUniform::TInt2: code += "//__constant__ int2 "; break;
    case IUniform::TInt3: code += "//__constant__ int3 "; break;
    case IUniform::TInt4: code += "//__constant__ int4 "; break;
    case IUniform::TBool: code += "//__constant__ bool "; break;
    case IUniform::TBool2: code += "//__constant__ bool2 "; break;
    case IUniform::TBool3: code += "//__constant__ bool3 "; break;
    case IUniform::TBool4: code += "//__constant__ bool4 "; break;
    case IUniform::TFloat: code += "//__constant__ float "; break;
    case IUniform::TVec2: code += "//__constant__ float2 "; break;
    case IUniform::TVec3: code += "//__constant__ float3 "; break;
    case IUniform::TVec4: code += "//__constant__ float4 "; break;
    case IUniform::TMat2: code += "//__constant__ float2x2 "; break;
    case IUniform::TMat3: code += "//__constant__ float3x3 "; break;
    case IUniform::TMat4: code += "//__constant__ float4x4 "; break;
    // texture part
    case IUniform::TTexture1D: code += "texture<float4, 1, cudaReadModeElementType> "; break;
    case IUniform::TTexture2D: code += "texture<float4, 2, cudaReadModeElementType> "; break;
    case IUniform::TTexture2DRect: code += "// TODO: TexRect for CUDA ?"; break;
    case IUniform::TTexture3D: code += "texture<float4, 3, cudaReadModeElementType> "; break;
    case IUniform::TTextureCube: code += "// TODO: TexCube for CUDA ? "; break;
    }
    code += name;

    switch(pUniform->getType())
    {
    case IUniform::TTexture1D:
    case IUniform::TTexture2D:
    case IUniform::TTexture2DRect:
    case IUniform::TTexture3D:
    case IUniform::TTextureCube:
        code += "(0, cudaFilterModeLinear)";
        break;
    default:
        {
        int sz = pUniform->getArraySz();
        if(sz > 1)
        {
            char tmp[10]; sprintf(tmp, "[%d]", sz);
            code += tmp;
        }
        }
        break;
    }
    // NO need to append the default value : this will be done through this Uniform anyways
    code += ";\n";
    return code;
}


bool buildCodeFromUniformsCUDA(IContainer* pCont)
{
    std::string code;
    IUniform *pUniform;
    code = std::string("//nvFX-Generated\n");
    //
    // Walk through uniform parameters in CstBuffers
    //
    ICstBuffer *pCstBuffer;
    for(int i=0; pCstBuffer = pCont->findCstBuffer(i); i++)
    {
        code += "//struct S";
        code += pCstBuffer->getName();
        code += " {\n";
        for(int i=0; pUniform = pCstBuffer->findUniform(i); i++)
        {
            code += buildCodeForThisUniformCUDA(pCont, pUniform);
        }
        code += "//};\n//__constant__ S";
        code += pCstBuffer->getName();
        code += " ";
        code += pCstBuffer->getName();
        code += ";\n";
    }
    //
    // Walk through uniform parameters in GLOBAL space
    // and add them to a special cbuffer named nvfx_global
    // NOTE: uniforms that aren't in any cbuffer are put by default to a cbuffer called $Globals
    // However I decided to not use it here by default to avoid problems when some effects didn't
    // put all global parameters outside of HLSL source code.
    //
    for(int i=0; pUniform = pCont->findUniform(i); i++)
    {
        const char * name = pUniform->getName();
        // IF the uniform has '.' : we declared a member of a struct. We can't append it to GLSL !
        if(strchr(name, '.'))
            continue;
        code += buildCodeForThisUniformCUDA(pCont, pUniform);
    }
#pragma MESSAGE("Sampler states for CUDA should be setup via CUDA Api")
    ISamplerState *pSS;
    for(int i=0; pSS = pCont->findSamplerState(i); i++)
    {
        const char * name = pSS->getName();
        code += "//CUDA doesn't handle sampler states. Need to setup via API... TODO";
        code += "//SamplerState ";
        code += name;
        code += ";\n";
        // No need to append any state : they will be handled by the runtime
    }
    //
    // Now add this generated code as header to every Shader
    //
    Shader *pShader = static_cast<Shader*>(pCont->getExInterface()->createCUDAShader(NULL));
    return pShader->loadShaderCode(code.c_str());
}


} //namespace nvFX
