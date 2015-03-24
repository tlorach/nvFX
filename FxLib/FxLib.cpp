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
#include <windows.h> // for OutputDebugString()
#endif
#include <stdarg.h> // Container::eprintf()

#include <fstream>
#include <set>
#include <assert.h>
#include "Fx.h"
#pragma warning(disable:4996)
using namespace nvFX;

#ifdef ANDROID
#   pragma message("Building for Android")
#endif

/*************************************************************************/ /**
 ** 
 ** - Major version : the 2 higher bytes
 ** - Minor version : the 2 lower bytes
 ** 
 **/ /*************************************************************************/ 
unsigned long nvFX::getVersion()
{
    return (NVFX_MAJOR_VERSION<<16)|NVFX_MINOR_VERSION;
}

/*************************************************************************/ /**
 ** 
 ** 
 ** 
 **/ /*************************************************************************/ 
static size_t fmt2_sz    = 0;
static char *fmt2 = NULL;
static FILE *fd = NULL;
static bool bLogReady = false;
void nvFX::eprintf(const char * fmt, ...)
{
//    int r = 0;
    if(fmt2_sz == 0) {
        fmt2_sz = 1024;
        fmt2 = (char*)malloc(fmt2_sz);
    }
    va_list  vlist;
    va_start(vlist, fmt);
    while((vsnprintf(fmt2, fmt2_sz, fmt, vlist)) < 0) // means there wasn't anough room
    {
        fmt2_sz *= 2;
        if(fmt2) free(fmt2);
        fmt2 = (char*)malloc(fmt2_sz);
    }
#ifdef WIN32
    //OutputDebugStringA(fmt2);
#ifdef _DEBUG
    if(bLogReady == false)
    {
        fd = fopen("Log_nvFX.txt", "w");
        bLogReady = true;
    }
    if(fd)
        fprintf(fd, fmt2);

#endif
#endif
    if(g_errFunc)
        g_errFunc(fmt2);
    else
        ::printf(fmt2);
}
void nvFX::printf(const char * fmt, ...)
{
//    int r = 0;
    if(fmt2_sz == 0) {
        fmt2_sz = 1024;
        fmt2 = (char*)malloc(fmt2_sz);
    }
    va_list  vlist;
    va_start(vlist, fmt);
    while((vsnprintf(fmt2, fmt2_sz, fmt, vlist)) < 0) // means there wasn't anough room
    {
        fmt2_sz *= 2;
        if(fmt2) free(fmt2);
        fmt2 = (char*)malloc(fmt2_sz);
    }
#ifdef WIN32
    //OutputDebugStringA(fmt2);
#ifdef _DEBUG
    if(bLogReady == false)
    {
        fd = fopen("Log_nvFX.txt", "w");
        bLogReady = true;
    }
    if(fd)
        fprintf(fd, fmt2);

#endif
#endif
    if(g_msgFunc)
        g_msgFunc(fmt2);
    else
        ::fprintf(stderr, fmt2);
}
/***************************************************************************/
/** Error Callback Error Callback Error Callback Error Callback           **/
/***************************************************************************/
nvFX::messageCallbackFunc nvFX::g_errFunc = NULL;
void nvFX::setErrorCallback(messageCallbackFunc func)
{
    g_errFunc = func;
}

nvFX::messageCallbackFunc nvFX::g_msgFunc = NULL;
void nvFX::setMessageCallback(messageCallbackFunc func)
{
    g_msgFunc = func;
}
/*************************************************************************/ /**
 ** 
 ** NSIGHT
 ** 
 **/ /*************************************************************************/ 
#ifdef USE_NSIGHT
nvtxRangePushEx_Pfn nvFX::nvtxRangePushEx_dyn;
nvtxRangePush_Pfn   nvFX::nvtxRangePush_dyn;
nvtxRangePop_Pfn    nvFX::nvtxRangePop_dyn;

nvtxEventAttributes_t nvFX::eventAttr = {
      NVTX_VERSION,
      sizeof(nvtxEventAttributes_t),
      1,
      NVTX_COLOR_ARGB,
      0xFF00FF11,
      NVTX_PAYLOAD_TYPE_INT64,
      0,
      NVTX_MESSAGE_TYPE_ASCII,
      NULL
    };
//-----------------------------------------------------------------------------
// Name: 
// Desc: 
//-----------------------------------------------------------------------------
void nvFX::initNSight()
{
#ifdef TESTIFNULL
#undef TESTIFNULL
#endif
#define TESTIFNULL(x) if(NULL == (x)) { {nvFX::printf("Error>> NSight dll doesn't have" #x "\n"); } return; }
    // Let's dynamically load this library so we can avoid it if not here.
    HMODULE hLibNSight = LoadLibraryA("nvToolsExt64_1.dll");
    if(!hLibNSight)
    {
        hLibNSight = LoadLibraryA("nvToolsExt32_1.dll");
        if(!hLibNSight)
        {
            nvFX::printf("Warning>> No NSight Tool dll found (nvToolsExt32/64_1.dll)\n");
            return;
        }
    }
    TESTIFNULL(nvtxRangePushEx_dyn = (nvtxRangePushEx_Pfn)GetProcAddress(hLibNSight,"nvtxRangePushEx"));
    TESTIFNULL(nvtxRangePush_dyn = (nvtxRangePush_Pfn)GetProcAddress(hLibNSight,"nvtxRangePushA"));
    TESTIFNULL(nvtxRangePop_dyn = (nvtxRangePop_Pfn)GetProcAddress(hLibNSight,"nvtxRangePop"));
    nvFX::printf("Yes>>NSight Custom markers available !\n");
    return;
}
#else
#define eventAttr 0
#endif // USE_NSIGHT

/***************************************************************************/
/**  **/
/***************************************************************************/
void nvFX::getID(int *ids, int numids)
{
    static int id = 1;
    for(int i=0; i<numids; i++)
        ids[i] = id++;
}


/*************************************************************************/ /**
 ** 
 ** 
 **/ /*************************************************************************/ 
//bool ShaderProgram::addShader(IShader* pShader)
//{
//    if(!pShader)
//        return false;
//    m_shaders.push_back(pShader);
//    return true;
//}

void Program::addTarget(Pass* p, int layerID)
{
    for(int i=0; i<m_targets.size(); i++)
    {
        STarget& t = m_targets[i];
        if((t.pass == p) && (t.passLayerId == layerID))
            return;
    }
    STarget t;
    t.pass = p;
    t.passLayerId = layerID;
    m_targets.push_back(t);
}
int Program::releaseTarget(Pass* p, int layerID)
{
    std::vector<STarget>::iterator it;
    for(it=m_targets.begin(); it != m_targets.end(); it++)
    {
        if((it->pass == p) && (it->passLayerId == layerID))
        {
            m_targets.erase(it);
            int sz = (int)m_targets.size();
            if(sz == 0) // the program gets deleted if no target anymore
            {
                m_container->destroy(this);
            }
            return sz;
        }
    }
    return (int)m_targets.size();
}

//
// ProgramPipeline ProgramPipeline ProgramPipeline ProgramPipeline ProgramPipeline
// ProgramPipeline ProgramPipeline ProgramPipeline ProgramPipeline ProgramPipeline
// ProgramPipeline ProgramPipeline ProgramPipeline ProgramPipeline ProgramPipeline
// ProgramPipeline ProgramPipeline ProgramPipeline ProgramPipeline ProgramPipeline
//


ProgramPipeline::ProgramPipeline(Container *pCont)
{
    m_container = pCont;
    m_validated = false;
    m_shaderFlags = 0;
}

/*************************************************************************/ /**
 ** 
 ** 
 **/ /*************************************************************************/ 
ProgramPipeline::~ProgramPipeline()
{
    m_progShaders.clear();
}
/*************************************************************************/ /**
 ** 
 ** 
 **/ /*************************************************************************/ 
bool ProgramPipeline::addProgramShader(/*IShaderProgram*/IProgram* pProgShader)
{
    //int bits = 0;
    //for(int i=0; i<(int)m_progShaders.size(); i++)
    //{
    //    bits |= m_progShaders[i]->getProgramShaderFlags();
    //}
// use later m_shaderFlags to tell a pass execution that we might need some Patch prims...
    if (NULL == pProgShader)
      return false;
    unsigned int newbits = pProgShader->getProgramShaderFlags();
    if(newbits & m_shaderFlags) // we are assigning redundant shader stages!
        return false;
    m_shaderFlags |= newbits;
    m_progShaders.push_back(static_cast<Program*>(pProgShader));
    m_validated = false;
    return true;
}

/*************************************************************************/ /**
 ** 
 ** 
 **/ /*************************************************************************/ 
bool ProgramPipeline::removeProgramShader(IProgram* pProgShader)
{
    if (NULL == pProgShader)
      return false;
    std::vector</*ShaderProgram*/Program*>::iterator iP = m_progShaders.begin();
    for(; iP != m_progShaders.end(); iP++)
    {
        if(static_cast<IProgram*>(*iP) == pProgShader)
        {
            int fl = pProgShader->getProgramShaderFlags();
            m_progShaders.erase(iP);
            invalidate(fl);
            return true;
        }
    }
    return false;
}
/*************************************************************************/ /**
 ** This is supposed to remove one program by one
 ** returns the removed program ptr if succeeded. Otherwise NULL
 **/ /*************************************************************************/ 
IProgram* ProgramPipeline::removeProgramShader(int stageFlags)
{
    std::vector</*ShaderProgram*/Program*>::iterator iP = m_progShaders.begin();
    std::vector</*ShaderProgram*/Program*>::iterator iP2;
    while(iP != m_progShaders.end())
    {
        iP2 = iP;
        iP2++;
        int fl = (*iP)->getProgramShaderFlags();
        if(fl & stageFlags)
        {
            Program* prog = *iP;
            invalidate(fl);
            m_progShaders.erase(iP);
            return prog;
        }
        iP = iP2;
    }
    return NULL;
}

/*************************************************************************/ /**
 ** 
 ** 
 **/ /*************************************************************************/ 
bool ProgramPipeline::validate()
{
    bool bRes = true;
    for(int i=0; i<(int)m_progShaders.size(); i++)
    {
        if(!static_cast<Program*>(m_progShaders[i])->bind(m_container)) // should call link()...
            bRes = false;
    }
    return bRes;
}
/*************************************************************************/ /**
 ** 
 ** 
 **/ /*************************************************************************/ 
bool ProgramPipeline::invalidate(int stages)
{
    m_validated = false;
    m_shaderFlags &= ~stages;
    for(int i=0; i<(int)m_progShaders.size(); i++)
    {
        if(m_progShaders[i]->getProgramShaderFlags() & stages)
            m_progShaders[i]->unbind(m_container);
    }
    return true;
}

/*************************************************************************/ /**
 ** 
 ** 
 **/ /*************************************************************************/ 
bool ProgramPipeline::bind()
{
    bool bRes = true;
    for(int i=0; i<(int)m_progShaders.size(); i++)
    {
        if(!static_cast<Program*>(m_progShaders[i])->getExInterface()->bind(m_container))
            bRes = false;
    }
    return bRes;
}
/*************************************************************************/ /**
 ** 
 ** 
 **/ /*************************************************************************/ 
void ProgramPipeline::unbind()
{
    for(int i=0; i<(int)m_progShaders.size(); i++)
    {
        static_cast<Program*>(m_progShaders[i])->unbind(m_container);
    }
}

/*************************************************************************/ /**
 ** 
 ** 
 **/ /*************************************************************************/ 
/*IShaderProgram*/IProgram* ProgramPipeline::getShaderProgram(int i)
{
    if(i>=(int)m_progShaders.size())
        return NULL;
    return m_progShaders[i];
}

/*************************************************************************
 ** ShaderModuleRepository
 **
 *************************************************************************/
ShaderModuleRepository::~ShaderModuleRepository()
{
}

ICstBufferRepository* nvFX::getCstBufferRepositorySingleton()
{
    static CstBufferRepository repository;
    return &repository;
}
IUniformRepository* nvFX::getUniformRepositorySingleton()
{
    static UniformRepository repository;
    return &repository;
}
IShaderModuleRepository* nvFX::getShaderModuleRepositorySingleton()
{
    static ShaderModuleRepository repository;
    return &repository;
}
IShader* ShaderModuleRepository::findShader(const char * name, TargetType t)
{
    ShdMap::iterator it = m_shaders.find(name);
    if(it == m_shaders.end())
        return NULL;
    if((t == TANY) || (t == it->second.p->getType()))
        return it->second.p;
    return NULL;
}

IShader*    ShaderModuleRepository::getShader(int i)
{
    ShdMap::iterator it = m_shaders.begin();
    for(;(i>0) && (it!=m_shaders.end());i--)
        ++it;
    return (it == m_shaders.end()) ? NULL:it->second.p;
}
IProgram*   ShaderModuleRepository::getProgram(int i)
{
    if(i >= m_programs.size())
        return NULL;
    return m_programs[i].p;
}


static IShader* shdArray[100];
extern bool findMatchingAnnotation(IAnnotation* pAnnot, const char *annotationName, const char* value);

int ShaderModuleRepository::gatherFromAnnotation(IShader** &ppShaders, const char *annotationName, const char* str, int *ival, float *fval)
{
    memset(shdArray, 0, 100*sizeof(IShader*));
    ppShaders = (IShader**)shdArray;
    int nshaders = 0;
    if(!annotationName)
        return 0;
    ShdMap::iterator i2 = m_shaders.begin();
    while(i2 != m_shaders.end())
    {
        Shader* pShd = static_cast<Shader*>(i2->second.p);
        if(pShd)
        {
            IAnnotation* pAnnot = pShd->annotations();
            if((str && findMatchingAnnotation(pAnnot, annotationName, str))
             ||(ival && (pAnnot->getAnnotationInt(annotationName)==ival[0]))
             ||(fval && (pAnnot->getAnnotationFloat(annotationName)==fval[0])))
            {
                if(nshaders >= 100)
                {
                    nvFX::printf(">>Error : gathering of shader exceeded 100 !");
                    return nshaders;
                }
                shdArray[nshaders++] = pShd;
            }
        }
        ++i2;
    }
    // warning : means that we cannot put in the same list some global shaders from here with
    // local shader previously found in this method...
    if(nshaders == 0)
    {
        // Try in the global repository
        return static_cast<ShaderModuleRepository*>(getShaderModuleRepositorySingleton())->gatherFromAnnotation(ppShaders, annotationName, str, ival, fval);
    }
    return nshaders;
}
int ShaderModuleRepository::gatherFromAnnotation(IShader** &ppShaders, const char *annotationName, const char* value)
{
    return gatherFromAnnotation(ppShaders, annotationName, value, NULL, NULL);
}
int ShaderModuleRepository::gatherFromAnnotation(IShader** &ppShaders, const char *annotationName, int value)
{
    return gatherFromAnnotation(ppShaders, annotationName, NULL, &value, NULL);
}
int ShaderModuleRepository::gatherFromAnnotation(IShader** &ppShaders, const char *annotationName, float value)
{
    return gatherFromAnnotation(ppShaders, annotationName, NULL, NULL, &value);
}
int ShaderModuleRepository::gatherFromNamespace(IShader** &ppShaders, const char *ns)
{
    memset(shdArray, 0, 100*sizeof(IShader*));
    ppShaders = (IShader**)shdArray;
    int nshaders = 0;
    if(!ns)
        return 0;
    std::string strNS(ns);
    ShdMap::iterator i2 = m_shaders.begin();
    while(i2 != m_shaders.end())
    {
        Shader* pShd = static_cast<Shader*>(i2->second.p);
        if(pShd && (pShd-> m_name.ns() == strNS))
        {
            if(nshaders >= 100)
            {
                nvFX::printf(">>Error : gathering of shader exceeded 100 !");
                return nshaders;
            }
            shdArray[nshaders++] = pShd;
        }
        ++i2;
    }
    return nshaders;
}
IProgram*   ShaderModuleRepository::findProgram(ShaderType type, int numShaders, IShader ** pShaders)
{
    int sz = (int)m_programs.size();
    for(int i=0; i<sz; i++)
    {
        IProgram* p = m_programs[i].p;
        if(numShaders != p->getNumShaders())
            continue;
        int j;
        for(j=0; j<numShaders; j++)
        {
            ShaderType t;
            IShader* pShd;
            pShd = p->getShader(0, &t);
            if(t != type)
                return NULL;
            if((pShd == NULL) || (findShader(pShd->getName()) == NULL) )
                break;
        }
        if(j == numShaders)
            return p;
    }
    return NULL;
}
int ShaderModuleRepository::addShader(IShader *p)
{
    if(!p)
        return -1;
    Shd &shd = m_shaders[p->getName()];
    if(shd.p && (shd.p != p))
    {
        LOGD("WARNING: Global Repository already has a shader named %s\n", p->getName());
        return -1;
    }
    shd.p = p;
    LOGD("added Shader %s as Global (refCnt=%d)\n", p->getName(), shd.refCnt+1);
    return ++shd.refCnt;
}
int ShaderModuleRepository::addProgram(IProgram* p)
{
    if(!p)
        return -1;
    int sz = (int)m_programs.size();
    for(int i=0; i<sz; i++)
    {
        if(m_programs[i].p == p)
        {
            return ++m_programs[i].refCnt;
        }
    }
    m_programs.push_back(Prog(p, 1));
    LOGD("added Program %d as Global (refCnt=%d)\n", p->getProgram(), 1);
    return 1;
}
int ShaderModuleRepository::releaseProgram(IProgram *p)
{
    if(!p)
        return -1;
    ProgVec::iterator it = m_programs.begin();
    while(it != m_programs.end())
    {
        if((*it).p == p)
        {
            int a = p->getProgram(); //for debug
            int c = --(*it).refCnt;
#ifdef _DEBUG
                LOGD("Program %d (", (*it).p->getProgram() );
                IShader* pshd;
                for(int ii=0; pshd = (*it).p->getShader(ii); ii++)
                    LOGD("%s ", pshd->getName());
                LOGD(") ");
#endif
            if(c <= 0)
            {
#ifdef _DEBUG
                LOGD("no more used. REMOVING it from ShaderModuleRepository\n", a);
#endif
                m_programs.erase(it);
                return 0;
            } else
#ifdef _DEBUG
                LOGD("still used (%d). KEEPING it in ShaderModuleRepository\n", a, c);
#endif
                return c;
        }
        ++it;
    }
    return -1;
}
int ShaderModuleRepository::releaseShader(IShader* p)
{
    ShdMap::iterator it = m_shaders.begin();
    while(it != m_shaders.end())
    {
        if(it->second.p == p)
        {
            int c = --it->second.refCnt;
            if(c <= 0)
            {
                LOGD("Shader %s no more used. Removing it from ShaderModuleRepository\n", it->first.c_str());
                m_shaders.erase(it);
                return 0;
            }
            LOGD("Shader %s still used (%d). Keeping it in ShaderModuleRepository\n", it->first.c_str(), c);
            return c;
        }
        ++it;
    }
    return -1;
}

/*************************************************************************/ /**
 ** 
 ** 
 **/ /*************************************************************************/ 
void Container::consolidateShaders()
{
    ShaderModuleRepository* pShdRep = 
        static_cast<ShaderModuleRepository*>(getShaderModuleRepositorySingleton());
    std::set<IProgram *> usedPrograms;
    int sz = (int)m_shaderprograms.size();
    for(int i=0; i<sz; i++)
    {
        //assert(m_shaderprograms[i]);
        CHECK_POINTER(m_shaderprograms[i]);
        usedPrograms.insert(m_shaderprograms[i]);
    }
    // Clear the table : we are about to re-create it
    m_shaderprograms.clear();
    // Walk through techs/passes and check if any pass is using it
    TechVec::iterator it = m_techniques.begin();
    while(it != m_techniques.end())
    {
        Technique::PassVec::iterator ip = (*it)->m_passes.begin();
        while(ip != (*it)->m_passes.end())
        {
            Pass::StatesLayerMap::iterator isl = ip->pass->m_statesLayers.begin();
            while(isl != ip->pass->m_statesLayers.end())
            {
                IProgram* p;
                CHECK_TRUE_MSG(isl->second.program == NULL, "TODO: handle consolidation for non-separable programs");
                if(isl->second.programPipeline)
                  for(int i=0; p = isl->second.programPipeline->getShaderProgram(i); i++)
                  {
                    std::set<IProgram *>::iterator iip;
                    // at last we have a program
                    iip = usedPrograms.find(p);
#ifdef _DEBUG
                        //LOGD("Program %d (", p->getProgram() );
                        //IShader* pshd;
                        //for(int ii=0; pshd = p->getShader(ii); ii++)
                        //    LOGD("%s ", pshd->getName());
#endif
                    if(iip != usedPrograms.end())
                    {
#ifdef _DEBUG
                        LOGD("Program %d (", p->getProgram() );
                        IShader* pshd;
                        for(int ii=0; pshd = p->getShader(ii); ii++)
                            LOGD("%s ", pshd->getName());
                        LOGD(") Found as used by some pass (%s)\n", ip->pass->getName());
#endif
                        usedPrograms.erase(iip);
                        m_shaderprograms.push_back(p); // add it back, because used here
                    } else {
#ifdef _DEBUG
                        //LOGD(") used in pass %s BUT not found\n", ip->pass->getName());
#endif
                    }
                  } // for()
                ++isl;
            }
            ++ip;
        }
        ++it;
    }
    std::set<IProgram *>::iterator iip = usedPrograms.begin();
    while(iip != usedPrograms.end())
    {
#ifdef _DEBUG
        LOGD("Program %d (", (*iip)->getProgram() );
        IShader* pshd;
        for(int ii=0; pshd = (*iip)->getShader(ii); ii++)
            LOGD("%s ", pshd->getName());
#endif
        int c = pShdRep->releaseProgram(*iip);
        if(c <= 0)
        {
    #ifdef _DEBUG
            LOGD(") Not used anymore anywhere. DELETING it\n");
    #endif
            // good enough : query the first shader in the program
            switch((*iip)->getShader(0)->getType())
            {
            case TGLSL:
            case THLSL:
                delete_Program(*iip);
                break;
            case TCUDA:
                delete_ProgramCUDA(*iip);
                break;
            }
        } else {
#ifdef _DEBUG
        LOGD(") Still used as Global (%d)\n", c);
#endif
        }
        ++iip;
    }
}

/*************************************************************************/ /**
 ** 
 ** 
 **/ /*************************************************************************/ 
void Shader::cleanupSourceCode()
{
    m_shaderCode.clear();
    m_startLineNumbers.clear();
    m_totalLines = 0;
}
/*************************************************************************/ /**
 ** 
 ** 
 **/ /*************************************************************************/ 
bool Shader::loadShaderCode(const char *code, int startLineNumber, const char *fname)
{
    if(!code)
        return false;
    m_shaderCode = std::string(code);
    m_startLineNumbers.clear();
    m_totalLines = 0;
    for(int i=0; code[i]!='\0'; i++)
        if(code[i]=='\n') m_totalLines++;
    startLineNumber -= m_totalLines;
    m_startLineNumbers.push_back(codeInfo(startLineNumber, 0, m_totalLines, fname));
    return true;
}
/*************************************************************************/ /**
 ** 
 ** 
 **/ /*************************************************************************/ 
bool Shader::addHeaderCode(const char *code, int startLineNumber, const char *fname)
{
    if(!code)
        return false;
    int nlines = 0;
    for(int i = 0; code[i]!='\0'; i++)
        if(code[i]=='\n') nlines++;
    nlines++; // because we also added a \n later here
    m_totalLines += nlines;
    std::vector<codeInfo>::iterator iCI;
    for(iCI = m_startLineNumbers.begin(); iCI != m_startLineNumbers.end(); iCI++)
        iCI->lineinShader += nlines;
    m_startLineNumbers.push_back(codeInfo(startLineNumber, 0, nlines, fname));
    m_shaderCode = std::string(code) + std::string("\n") + m_shaderCode;
    return true;
}
/*************************************************************************/ /**
 ** 
 ** 
 **/ /*************************************************************************/ 
bool Shader::appendCode(const char *code, int startLineNumber, const char *fname)
{
    if(!code)
        return false;
    int nlines = 0;
    for(int i = 0; code[i]!='\0'; i++)
        if(code[i]=='\n') nlines++;
    nlines++; // because we also added a \n later here
    m_totalLines += nlines;
    // TODO: maybe this line number is making things wrong...
    m_startLineNumbers.push_back(codeInfo(startLineNumber, 0, nlines, fname));
    m_shaderCode += std::string("\n") + std::string(code);
    return true;
}
/*************************************************************************/ /**
 ** 
 ** 
 **/ /*************************************************************************/ 
bool    Shader::loadShaderCodeFromFile(const char *fname)
{
    if(!fname)
        return false;
    std::ifstream ifs;
    ifs.open(fname);
    if(ifs.bad())
        return false;

    std::string file((std::istreambuf_iterator<char>(ifs)), std::istreambuf_iterator<char>());
    if(file.size()==0)
    {
        if(fname)	nvFX::printf("\nGLSL ERROR: loading file %s \n", fname ? fname : "NULL");
        fflush(stdout);
//#		ifdef _DEBUG
//		_asm {int 3 }
//#		endif
        return false;
    }
    return true;
}

/*************************************************************************/ /**
 ** 
 ** 
 **/ /*************************************************************************/ 
bool    Shader::setAsKernel(int nArgs, Argument* args, const char * funcName)
{
    if(!funcName)
        funcName = m_name.c_str();
    if(!funcName)
        return false;
    std::string funcHeader;
    m_kernelName = std::string(funcName);
    int i=0;

    for(; i<m_kernelArgs.size(); i++)
        delete [] m_kernelArgs[i].argName;
   
    m_kernelArgs.clear();
    funcHeader = "extern \"C\" __global__ void " + m_kernelName + "(";
    for(i=0; i<nArgs; i++)
    {
        int l = (int)strlen(args[i].argName)+1;
        IShaderEx::Argument arg(args[i].t, new char[l]);
        strncpy((char*)arg.argName, args[i].argName, l);
        m_kernelArgs.push_back(arg);
        switch(args[i].t)
        {
        case TInt:    funcHeader += std::string("int ") + std::string(args[i].argName); break;
        case TInt2:    funcHeader += std::string("int2 ") + std::string(args[i].argName); break;
        case TInt3:    funcHeader += std::string("int3 ") + std::string(args[i].argName); break;
        case TInt4:    funcHeader += std::string("int4 ") + std::string(args[i].argName); break;
        case TUInt:    funcHeader += std::string("unsigned int ") + std::string(args[i].argName); break;
        case TUInt2:    funcHeader += std::string("uint2 ") + std::string(args[i].argName); break;
        case TUInt3:    funcHeader += std::string("uint3 ") + std::string(args[i].argName); break;
        case TUInt4:    funcHeader += std::string("uint4 ") + std::string(args[i].argName); break;
        case TFloat:    funcHeader += std::string("float ") + std::string(args[i].argName); break;
        case TVec2:    funcHeader += std::string("float2 ") + std::string(args[i].argName); break;
        case TVec3:    funcHeader += std::string("float3 ") + std::string(args[i].argName); break;
        case TVec4:    funcHeader += std::string("float4 ") + std::string(args[i].argName); break;
        //case TBool, TBool2, TBool3, TBool4,
        //case TMat2, TMat3, TMat4, 
        case TIntPtr:    funcHeader += std::string("int* ") + std::string(args[i].argName); break;
        case TInt2Ptr:    funcHeader += std::string("int2* ") + std::string(args[i].argName); break;
        case TInt3Ptr:    funcHeader += std::string("int3* ") + std::string(args[i].argName); break;
        case TInt4Ptr:    funcHeader += std::string("int4* ") + std::string(args[i].argName); break;
        case TUIntPtr:    funcHeader += std::string("unsigned int* ") + std::string(args[i].argName); break;
        case TUInt2Ptr:    funcHeader += std::string("uint2* ") + std::string(args[i].argName); break;
        case TUInt3Ptr:    funcHeader += std::string("uint3* ") + std::string(args[i].argName); break;
        case TUInt4Ptr:    funcHeader += std::string("uint4* ") + std::string(args[i].argName); break;
        case TFloatPtr:    funcHeader += std::string("float* ") + std::string(args[i].argName); break;
        case TVec2Ptr:    funcHeader += std::string("float2* ") + std::string(args[i].argName); break;
        case TVec3Ptr:    funcHeader += std::string("float3* ") + std::string(args[i].argName); break;
        case TVec4Ptr:    funcHeader += std::string("float4* ") + std::string(args[i].argName); break;
        case TUndefined:
        default:
            funcHeader += "<ARGUMENT ERROR>"; break;
        }
        if(i < (nArgs-1)) funcHeader += ", ";
    }
    funcHeader += ") {\n";
    //
    // because the kernel in an effect is declared without function header and brackets,
    // decorate the code with the function header and the '{' '}'
    //
    m_shaderCode = funcHeader + m_shaderCode + "}";
    return true;
}
/*************************************************************************/ /**
 ** 
 ** 
 **/ /*************************************************************************/ 
IShaderEx::Argument    Shader::getArgument(int i)
{
    if(i >= (int)m_kernelArgs.size())
        return IShaderEx::Argument();
    return m_kernelArgs[i];
}
/*************************************************************************/ /**
 ** 
 ** 
 **/ /*************************************************************************/ 
const char * Shader::isUsedAsKernel()
{
    return m_kernelName.size() > 0 ? m_kernelName.c_str() : NULL;
}

/*************************************************************************/ /**
 ** 
 ** 
 **/ /*************************************************************************/ 
Shader::Shader(const char *name)
{
    if(name) m_name = name;
    m_totalLines = 0;
    m_targetType = TANY;
}
Shader::~Shader()
{
    for(unsigned int i=0; i<m_kernelArgs.size(); i++)
        delete [] m_kernelArgs[i].argName;
    m_kernelArgs.clear();
}

void    Shader::addUser(Program *program)
{
    SUser u = { User_Program, program };
    m_users.insert (u);
}
void    Shader::removeUser(Program *program)
{
    SUser u = { User_Program, program };
    UserSet::iterator it;
    it = m_users.find(u);
    if(it == m_users.end())
        return;
    m_users.erase(it);
    releaseMe();
}
void    Shader::addUser(Container* pCont)
{
    SUser u;
    u.type = User_EffectContainer;
    u.container = pCont;
    m_users.insert (u);
}
void    Shader::removeUser(Container* pCont)
{
    SUser u;
    u.type = User_EffectContainer;
    u.container = pCont;
    UserSet::iterator it;
    it = m_users.find(u);
    if(it == m_users.end())
        return;
    m_users.erase(it);
    releaseMe();
}

void Shader::releaseMe()
{
    if(m_users.empty())
    {
        LOGD("Shader %s no more used... DELETING it\n", getName());
        switch(m_targetType)
        {
        case TGLSL:
        case THLSL:
            delete_Shader(this);
            break;
        case TCUDA:
            delete_ShaderCUDA(this);
            break;
        }
    }
}

/*************************************************************************/ /**
 ** 
 ** 
 **/ /*************************************************************************/ 
static const void*         g_device; // device. For DX... see how to do better
void        nvFX::setDevice(const void *p)
{
    g_device = p;
}
const void* nvFX::getDevice()
{
    return g_device;
}

